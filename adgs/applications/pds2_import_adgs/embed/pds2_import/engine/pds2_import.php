#!/usr/bin/php
<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions 4 and 5                                                      |
// +---------------------------------------------------------------------------+
// | Copyright (c) 1995-2006 - Advanced Computer Systems S.p.A.                |
// | Copyright 1995-2007, Advanced Computer Systems ,Inc.                      |
// | Via Della Bufalotta, 378 - 00139 Roma - Italy                             |
// | http://www.acsys.it                                                       |
// | All rights reserved.                                                      |
// +---------------------------------------------------------------------------+
// | This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems; |
// | the contents of this file may not be disclosed to third parties, copied or|
// | duplicated in any form, in whole or in part, without the prior written    |
// | permission of Advanced Computer Systems, Inc.                             |
// +---------------------------------------------------------------------------+
//
// $Prod: $
// $Author$
// $Id$
// $Revision$
//
// ACS PHP Lib includes

require_once 'ACSPhpLib/acs_daemon.php';
require_once 'ACSPhpLib/acs_tools.php';

define('_OPTYPE_DISABLED',0);
define('_OPTYPE_ENABLED',1);
define('_OPTYPE_SIMULATED',2);

define('_RUNRULE_TYPE_CODE_STRING', 'S');
define('_RUNRULE_TYPE_CODE_REGEX', 'R');
define('_RUNRULE_TYPE_CODE_GUID', 'G');
define('_RUNRULE_TYPE_CODE_CONFIG', 'C');

class pds2_import extends acs_daemon {
		
	/*
	 * Class Constructor
	 * OPMAN-200 pass the correct process ident name to acs_daemon
	 */
	public function __construct($ident='Import') {
		parent :: __construct($this->getImportIniFileName(),$ident);
	}
	
	protected function getImportIniFileName() {
        return  dirname(__FILE__).'/../config/pds2_import.ini';
	}

    protected function getDaemonRunSQL($dbRuntable){
    	return "SELECT ru.*, repo_run_operation_type, xrure.*, re.*, repo_run_rule_operation_type, ruru.*, pl.*
				FROM $dbRuntable ru
				INNER JOIN pds_imp_x_runs_repos xrure ON xrure.run_id=ru.id_run 
				INNER JOIN pds_imp_repos re ON re.id_repo=xrure.repo_id
				INNER JOIN pds_imp_x_repos_run_rules xreruru ON xreruru.repo_id= re.id_repo
				INNER JOIN pds_imp_run_rules ruru ON ruru.id_run_rule=xreruru.run_rule_id
				INNER JOIN pds_imp_plugins pl ON pl.id_plugin=ruru.plugin_id
				UNION
				SELECT id_notification AS id_run, 'Notification'  AS run_name, 1 AS run_operation_type, 60 AS run_interval_seconds, NULL AS last_run_datetime, 1 AS repo_run_operation_type,
				id_notification AS run_id, 0 AS repo_id, 99 AS repo_run_priority,  1 AS repo_run_operation_type, 1 AS with_transaction, NULL AS transaction_grouping_rule, NULL AS transaction_start_conditions, 
				0 AS id_repo, 'Notification' AS repo_name, notification_uri AS repo_uri, 1 AS repo_operation, NULL AS repo_extra_params, 1 AS repo_run_rule_operation_type, 
				0 AS id_run_rule, 'Notification' AS run_rule_name, 1 AS run_rule_operation_type, 'H' AS rule_operation_code, notification_params AS run_rule, 'C' AS run_rule_type_code, n.plugin_id, p.*
				FROM pds_imp_notifications n INNER JOIN pds_imp_plugins p ON p.id_plugin = n.plugin_id
				WHERE notification_status = 0
				ORDER BY repo_run_priority ASC";
				// NOTE: the id_run of the second select of the union is used to book the corresponding record of the pds_imp_notifications table
				
//				ORDER BY repo_run_priority ASC, repo_run_rule_priority ASC";
    }
}

class pds_import_transaction {
	public	$found_files = array(),			// this is relevant for the transaction control
			$transferred_files = array(),	// summary of transferred files
			$output_files = array(),		// files that must be handled by the delivery manager
			$found_dirs = array(),
			//$transferred_dirs = array(),
			//$output_dirs = array(),
			$transaction_ready = false,
			$pds_temporary_dir,
			$pds_output_dir,
			$db_handler;
	
	public function __construct($temporary_dir, $output_dir, $db_handler) {
		$this->pds_temporary_dir = $temporary_dir;
		$this->pds_output_dir = $output_dir;
		$this->db_handler = $db_handler;
	}
}

class pds_import_child extends acs_child_app {
	protected $transaction_pool;
	
	public function run($payload) {
		try {
			$this->do_import($payload);
		} catch (Exception $e) {
			$this->_logHandler->err(__METHOD__ . ': ' . $e->getMessage());
		}
	}	
	
	// The following method loads up the requested plugin
	// and launches it with all requested set of parameters
	public function do_import($payload){
		$this->_logHandler->debug("THREAD PAYLOAD: " . var_export($payload,true));
//		print_r($payload);
//die;
//		$this->thread_test($payload);

		$extraData=$payload['extraData'];
		$with_transaction = $extraData[0]['with_transaction'];
		
		$run_name = "{$payload['run_name']}-{$extraData[0]['repo_name']}";
		
		if ($with_transaction)
			$this->_logHandler->notice("Processing transaction for $run_name");

		$timestamp = acs_tools::microtime_string();
		$tempdir = $this->_iniVars['import']['pds_temporary_dir'] . '/' . $timestamp . '_TMP';
		//get temp dir from $payload if it defined
		if(isset($this->_iniVars['import']['pds_temporary_dir_relative_path']) 
			&& isset($extraData[0]['pds_final_dir']))
				$tempdir=dirname($extraData[0]['pds_final_dir']).
					$this->_iniVars['import']['pds_temporary_dir_relative_path']. '/' . $timestamp . '_TMP';
		
		if (!mkdir($tempdir))
			throw new Exception(__METHOD__ . ": Cannot make temporary directory: {$tempdir}");

		$outdir = $this->_iniVars['import']['pds_output_dir'] . '/' . $timestamp . '_OUT';
		//get out dir from payload if it defined
		if(isset($extraData[0]['pds_final_dir']))
			$outdir=$extraData[0]['pds_final_dir'];

		if (!file_exists($outdir) && !mkdir($outdir)) {
			rmdir($tempdir);
			throw new Exception(__METHOD__ . ": Cannot make output directory: {$outdir}");
		}
		
		$plugin_objs = array();
		$this->transaction_pool = new pds_import_transaction($tempdir, $outdir, $this->getDBHandler());
		$transaction_can_start = true;
		$cnt = 0;

		
		// Plugins allocation and precondition checks
		foreach ($extraData as $repoRecord) {
			$cnt++;
			$simulate = false;
	        $repoName = $repoRecord['repo_name'];

			if ($repoRecord['repo_run_operation_type'] == _OPTYPE_DISABLED ||
				$repoRecord['repo_operation'] == _OPTYPE_DISABLED ||
				$repoRecord['repo_run_rule_operation_type'] == _OPTYPE_DISABLED ||
				$payload['run_operation_type'] == _OPTYPE_DISABLED) {
				$this->_logHandler->warning("Operations on repository $repoName are turned off !");
//				exit;
				continue;
			}
			if ($repoRecord['repo_run_operation_type'] == _OPTYPE_SIMULATED ||
				$repoRecord['repo_operation'] == _OPTYPE_SIMULATED ||
				$repoRecord['repo_run_rule_operation_type'] == _OPTYPE_SIMULATED ||
				$payload['run_operation_type'] == _OPTYPE_SIMULATED) {
				$this->_logHandler->warning("Operations on repository $repoName will be simulated !");
				$simulate = true;
			}
	
			$pluginName = $repoRecord['plugin_name'];
			$pluginClass = $repoRecord['plugin_class'];
			$pluginPath = isset($repoRecord['plugin_class_path']) ? $repoRecord['plugin_class_path'] : dirname(__FILE__).'/plugins';
	        
	        $pluginFname = $pluginPath . '/' . $pluginClass . '.php';

			try {

				if (!file_exists($pluginFname))
					throw new acs_exCritical(__METHOD__ . ": $pluginName : Plugin Class File not found : $pluginFname");
		
				require_once $pluginFname;
				// Check to see if the include declared the class
				if (!class_exists($pluginClass, false)) {
					throw new acs_exCritical(__METHOD__ . ": $pluginName : Plugin Class not found : $pluginClass");
				}

				$storagePtr= null;
				$currPluginObj = new $pluginClass($this->_dbHandler,$this->_logHandler,$storagePtr,$this->_iniVars,$repoRecord);
		
				$currPluginObj->setSimulation($simulate);	// propagate simulation from parent tables
				$currPluginObj->setTransactionPool($this->transaction_pool);
				$currPluginObj->setName("{$run_name}-{$pluginName}-{$cnt}");
				$currPluginObj->initialize($this->_current_user_id);

				
				// Check the precondition
				$transaction_can_start &= $precondition_ret = $currPluginObj->precondition($this->_current_user_id);
				
				if (!$precondition_ret)
					$this->_logHandler->info("Transaction precondition failed for plugin " . $currPluginObj->getName());
				
				$plugin_objs[] = $currPluginObj;
				
			} catch (Exception $e) {
	            // this is to ensure that the report is always called (simulation of a FINALLY directive)
	            if(isset($currPluginObj))
	            	$this->_reportAr[$currPluginObj->getName()]=$currPluginObj->getReport($this->_iniVars['verbose_report']);
				
				if ($with_transaction) {
					$this->cleanupTemp($plugin_objs, true); 
					throw $e;	// only a transaction must be interrupted due to an exception
				} else
					$this->_logHandler->err($e->getMessage());
			}
	        
	        if(isset($currPluginObj)) $this->_reportAr[$currPluginObj->getName()]=$currPluginObj->getReport($this->_iniVars['verbose_report']);
		}
		
		if ($with_transaction && ! $transaction_can_start) {
			$this->_logHandler->warning("Transaction cannot start for $run_name");
			$this->cleanupTemp($plugin_objs, true);
			return;
		}
		
		// Check the transaction consistency (all the foreseen files are present?)
		if ($with_transaction && (	!$this->transaction_pool->transaction_ready &&
									!$this->checkGrouping(	$this->transaction_pool->found_files,
														$extraData[0]['transaction_grouping_rule'],
														$extraData[0]['transaction_start_conditions'])
									)) {
			$this->_logHandler->warning("Transaction grouping check failed:\nfound files: " . join(',', $this->transaction_pool->found_files) . "grouping rule: {$extraData[0]['transaction_grouping_rule']}\nstart conditions: {$extraData[0]['transaction_start_conditions']}");
			$this->cleanupTemp($plugin_objs, true);
			return;
		}

		// Transaction execution (real file transfer)
		foreach ($plugin_objs as $obj) {
			try {
				$obj->execute($this->_current_user_id);
			} catch (Exception $e) {
	            // this is to ensure that the report is always called (simulation of a FINALLY directive)
	            $this->_reportAr[$obj->getName()]=$currPluginObj->getReport($this->_iniVars['verbose_report']);
				
				if ($with_transaction) {
					$this->cleanupTemp($plugin_objs, true);
					throw $e;	// only a transaction must be interrupted due to an exception
				}
				else
					$this->_logHandler->err($e->getMessage());
			}
			$this->_reportAr[$obj->getName()]=$currPluginObj->getReport($this->_iniVars['verbose_report']);
		}
		
		// Move the transaction output files to the output directory
		foreach ($this->transaction_pool->output_files as $temp_output_file) {
			$output_file = $this->transaction_pool->pds_output_dir . '/' . basename($temp_output_file);
			$this->_logHandler->debug("Moving {$temp_output_file} to {$output_file}");
			if (rename($temp_output_file, $output_file) === false) {
				if ($with_transaction) {
					$this->cleanupTemp($plugin_objs, true);
					throw new Exception("Cannot move output file from {$temp_output_file} to {$output_file}");	// only a transaction must be interrupted due to an exception
				}
				else
					$this->_logHandler->err("Cannot move file {$temp_output_file} to {$output_file}");
			}
		}
		
		// Clean-up (remove source files)
		foreach ($plugin_objs as $obj) {
			try {
				$obj->cleanup($this->_current_user_id);
			} catch (Exception $e) {
				$this->_logHandler->err($e->getMessage());
			}
		}
		
		// Clean-up the temporary files (and also output directory if no output files are generated)
		$this->cleanupTemp($plugin_objs, (sizeof($this->transaction_pool->output_files) == 0 && !isset($extraData[0]['pds_final_dir'])));
	}
	
	protected function cleanupTemp($plugin_objs, $with_output = false) {
		foreach ($plugin_objs as $obj) {
			try {
				$obj->cleanupTemp($this->_current_user_id);
			} catch (Exception $e) {
				$this->_logHandler->warning($e->getMessage());
			}
		}
		// finally, remove temporary directory
		acs_file::recursive_remove_directory($this->transaction_pool->pds_temporary_dir);
		
		if ($with_output)
			acs_file::recursive_remove_directory($this->transaction_pool->pds_output_dir);
	}
	
	protected function checkGrouping($found_files, $grouping_rule, $start_conditions) {
		foreach ($found_files as $file) {
			if (!preg_match("/{$grouping_rule}/", $file) ||
				!preg_match("/{$start_conditions}/", $file))
				return false;
		}
		$files = sizeof($found_files);
		$expected_files = sizeof(preg_split('/(?<!\\\)\|/', $start_conditions));
		$this->_logHandler->debug(__METHOD__ . ": found files: $files, expected files: $expected_files\nfiles: " . join(',', $found_files) . "\ngrouping: $grouping_rule\nstart cond: $start_conditions");
		return $files == $expected_files;
	}
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////    L A U N CH E R
/////////
/////////////////////////////////////////////////////////////////////////////////////////////////
if (basename($argv[0]) == basename(__FILE__)) {
    $repetions_x_minute = (isset($argv[1]) and is_numeric($argv[1])) ? $argv[1] : 1;
    $period=floor(60 / $repetions_x_minute);
    if($period<3 or $period>60){
    	die('Repetitions per minute must be within range 1 - 20');
    }
    $engine = new pds2_import($argv[1]);
    $engine->daemonize($period);
}
?>