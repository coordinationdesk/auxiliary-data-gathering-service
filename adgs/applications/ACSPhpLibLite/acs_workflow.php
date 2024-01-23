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
// $Prod: datamanager $  
// $Author$
// $Id$
// $Revision$
//
// ACS PHP Lib includes
require_once 'ACSPhpLib/acs_daemon.php';
require_once 'ACSPhpLib/acs_file.php';
require_once 'ACSPhpLib/acs_exception.php';
require_once 'ACSPhpLib/acs_tools.php';

/**
 * @package ACSPhpLib
 */
class acs_workflow extends acs_daemon {

	////////////////////////////////////////////////////////////////////////////
	///                                                                      ///
	///   GENERAL SETTINGS                                                   ///
	///                                                                      ///
	////////////////////////////////////////////////////////////////////////////
    protected $_plugin_prefix = null;
    protected $_reportAr=array();
    protected $_global_prefix='wf_';
    protected $_plugins_section='wf_plugins';
    protected $_extra_params_obj=null;
    protected $_current_user_id=null;
    protected $_mail_object='Operation Error';
    protected $_mail_sender = 'Admin';
    public    $workflowError=false;

	/*
	 * Class Constructor
	 */
	public function __construct($plugin_prefix='', $_iniFileName,$db_section='db') {
		try {
            // get plugin prefix
            $this->_plugin_prefix=$plugin_prefix;
            if(empty($this->_plugin_prefix)) 
                throw new acs_exCritical('No plugin prefix passed to the workflow engine');
            
			parent :: __construct($_iniFileName,$plugin_prefix,false,$db_section);

			$this->extendedInitialisation();

        } catch (acs_exCritical $e) {
            $this->handleSevereError($e->getMessage(),true);
		} catch (Exception $e) {
			// TODO
			echo $e->getMessage();
		}

	}

	public function run() {
		$this->_logHandler->debug(__METHOD__ . " " . __LINE__ . " ");

        $this->workflowError = false;

		// launch workflow main engine
		try {
			$afterStartEngine=false;
            $this->_startEngine();
            $afterStartEngine=true;
			$this->_createReports();
		} catch (Exception $e) {
            $this->workflowError = true;
			if(!$afterStartEngine){ // try to write the report in any case.....
				try {
                    $this->_createReports();    
                } catch (Exception $e) {}
            }
            $this->handleSevereError($e->getMessage());
		}
	}

	private function _startEngine() {
		$pluginCnt = 1;
        $pluginFound=false;
        while (($currOperation = $this->_iniVars[$this->_global_prefix.$this->_plugin_prefix.'_' . $pluginCnt]) != null) {
			$pluginFound=true;
            try {
                $currOpname = $currOperation['name'];
    			if (empty ($currOpname)) {
    				$currOpname = 'Operation ' . $pluginCnt;
    				$currOperation['name'] = $currOpname;
    				$this->_logHandler->warning("Unknown Name for $currOpname !");
    			}
    			$pluginCnt++;
    			if (!$currOperation['mode'] == 'Off') {
    				$this->_logHandler->warning("$currOpname is turned off !");
    				continue;
    			}
    			if ($currOperation['mode'] == 'Simulation')
    				$this->_logHandler->warning("$currOpname will be simulated !");
    
    			$currClass = $currOperation['class'];
    			$currOpPath = array_key_exists('path',$currOperation) ? $currOperation['path'] : $this->_iniVars[$this->_plugins_section]['operation_path'];
                $currDbHandler = array_key_exists('db_section',$currOperation) ? acs_tools::getDBHandler($currOperation['db_section'],$this->_iniVars,$this->_logHandler) : $this->_dbHandler;
                $currOpFname = $currOpPath . '/' . $currClass . '.php';
    			if (!file_exists($currOpFname))
    				throw new acs_exCritical(__METHOD__ . " " . __LINE__ . " : Operation Class File not found : $currOpFname");
    
    			require_once $currOpFname;
    
    			// Check to see if the include declared the class
    			if (!class_exists($currClass, false)) {
    				throw new acs_exCritical(__METHOD__ . " " . __LINE__ . " : Operation Class not found : $currClass");
    			}

				$loop = false;
				$iteration_start = 1;
    			$iterations = 1;
    			if ($currOperation['mode'] == 'Loop')
    			{
    				$loop = true;
    				$iteration_start = $currOperation['loop_min'];
    				if ($currOperation['loop_type'] == 'Random')
    					$iterations = rand($currOperation['loop_min'], $currOperation['loop_max']);
    				else
    					$iterations = $currOperation['loop_max'] - $currOperation['loop_min'];
    				$this->_logHandler->info("Loop operation $currOpname, start at $iteration_start for $iterations cycles");
    			}

				for ($c = $iteration_start; $c <= $iterations; $c++)
				{
					if ($loop)
						$currOperation['_iteration'] = $c;
		   			
		   			$currOpObj = new $currClass ($currDbHandler, $this->_logHandler,$this->_extra_params_obj,$this->_iniVars,$currOperation);
	    
	    			try {
    					if ($loop)
    						$this->_logHandler->debug("$currOpname executing iteration $c");
    					else
    						$this->_logHandler->debug("$currOpname execute");
                    	$currOpObj->execute($this->_current_user_id);
	    			} catch (Exception $e) {
	                    // this is to ensure that the report is always called (simulation of a FINALLY directive)
	                    $this->_reportAr[$currOpname]=$currOpObj->getReport($this->_iniVars['verbose_report']);
	    				throw $e;
	    			}
            	}
                
                $this->_reportAr[$currOpname]=$currOpObj->getReport($this->_iniVars['verbose_report']);
    			$currOpObj = NULL;
            } catch (Exception $e) {
                if ($currOperation['exit_on_fail']) throw new acs_exCritical($e->getMessage());
                $this->_logHandler->warning(__METHOD__ . " " . __LINE__ . " : $currOpname failed : ".$e->getMessage());
            }
		}
        if(!$pluginFound)
            throw new acs_exCritical('No plugin found with given prefix => '.$this->_plugin_prefix);
	}

	private function _createReports() {
		if (array_key_exists('print_report', $this->_iniVars['application']) &&
			!$this->_iniVars['application']['print_report'])
			return;

		// Print counters and reports
		$rep_header = sprintf("-- REPORT : %s --\n\n", strftime("%b %d %H:%M:%S", time()));
		
        $report='';
        foreach($this->_reportAr as $v){
            if($v!='') {
                $report.=$v."\n";
            }
        }

		$rep_footer = "____________________________________________________________\n";
		if ($report!='') {
			$report = $rep_header . $report . $rep_footer;
            $this->_logHandler->notice($report);
		} else {
			$report = "Nothing to do.";
            $this->_logHandler->debug($report);
		}

		// send email to the administrators if error occurred
		if ($this->workflowError && $this->_iniVars['email_report_to_admins']) {
			$emailsAr = split('#', $this->_dbpreferences['administrators_email']);
			if (is_array($emailsAr)) {
				foreach ($emailsAr as $recipient) {
					// send the report via email
					$this->_logHandler->notice(__METHOD__ . " " . __LINE__ . ' ' . 'Sending Log to ' . $recipient . "\n");
					mail($recipient, $this->_mail_object, $report, "From: {$this->_mail_sender}\nContent-type: text/plain\nX-Mailer: Net Cube & PHP/" . phpversion());
				}
			}
		}
	}

    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////
    /////////    OVERRIDABLE FUNCTION
    /////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    public function extendedInitialisation() {}
    
} //class acs_workflow

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////    L A U N CH E R
/////////
/////////////////////////////////////////////////////////////////////////////////////////////////
if (basename($argv[0]) == basename(__FILE__)) {
    $classname=basename($argv[0],'.php');
    $scheduler = new $classname($argv[1]);
    $scheduler->daemonize();
}


?>
