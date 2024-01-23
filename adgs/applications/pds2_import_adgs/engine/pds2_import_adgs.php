<?php
// --------------------------------------------------------------------------
// Copyright 1995-2021, Exprivia SpA - DADF
// Via Della Bufalotta, 378 - 00139 Roma - Italy
// http://www.exprivia.com
//
// All Rights Reserved.
//
// --------------------------------------------------------------------------
// This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
// the contents of this file may not be disclosed to third parties, copied or
// duplicated in any form, in whole or in part, without the prior written
// permission of Exprivia, SpA
// --------------------------------------------------------------------------


require_once dirname(__FILE__) .'../../../pds2_import/engine/pds2_import.php';

class pds2_import_adgs extends pds2_import {
	protected $confFile;
	
	public function __construct($iniFile) {
		$this->confFile = $iniFile;
		
		if (empty($iniFile)) {
			$this->confFile = dirname(__FILE__).'/../config/pds2_import.ini';
		}
		if (!file_exists($this->confFile)) {
			$this->confFile = dirname(__FILE__) . '/../config/' . $iniFile;
		}
		
		parent :: __construct(basename($this->confFile, '.ini'));
	}

	protected function getImportIniFileName() {
        return $this->confFile;
	}
	
	protected function getHostname() {
		return @$this->_iniVars['import']['group'];
	}

    protected function getDaemonRunSQL($dbRuntable){
    	$pluginpath = dirname(__FILE__).'/plugins';
    	
    	$hostname = $this->getHostname();
    	$ruleslock_max_time=($this->_iniVars['import']['ruleslock_max_time'])?$this->_iniVars['import']['ruleslock_max_time']:'6 hours';

    	if ($hostname) {
    		$hostnameCondition = "rul.hostname={$this->_dbHandler->getSQLObj()->sql_quote_str($hostname)}";
    	} else {
    		$hostnameCondition = 'rul.hostname IS NULL';
    	}

    	return "SELECT DISTINCT rul.pollingperiod_sec, rul.retryperiod_sec, rul.id_receptionrule as runid, 
					rul.connretries,rul.retrycounter,
               		rep.symbolicname as filename,
                   	rep.symbolicname as repo_name,
                    rul.id_receptionrule as repo_id,
					rul.generateheader,
					rul.localpath as pds_final_dir,
					prot.protocol as repo_protocol,
					rep.username as repo_username,
					rep.password as repo_password,
					hosts.host_name as repo_hostname,
                    rep.remotepath as repo_remotepath,
					ftyp.filetype as filetype,
					ftyp.nameregularexpression as run_rule,
					ftyp.headerseparation,
					ftyp.hdrext,
					ftyp.dblext,
					{$this->_dbHandler->getSQLObj()->sql_format_date_time('rul.lastvisit')} as lastvisit,
        			{$this->_dbHandler->getSQLObj()->sql_auto_datetime()} as nowts,
					rul.connretries,
					rul.connfailureaddress,
					rul.procfailureaddress as onbadnamingconvention,
                    1 as run_operation_type,
                    1 as repo_operation, 
                    1 as repo_run_rule_operation_type,
                    1 as repo_run_operation_type,
                    1 as run_rule_operation_type,
                    0 as with_transaction,
                    rul.delete_on_remote,
                    'FTPHarvester' AS plugin_name,
                    plg.name AS plugin_class,
                    {$this->_dbHandler->getSQLObj()->sql_quote_str($pluginpath)} AS plugin_class_path,
					{$this->_dbHandler->getSQLObj()->sql_quote_str($ruleslock_max_time)} AS ruleslock_max_time,
 					rep.\"attributes\",
					rep.port as repo_port
                     FROM t_receptionrules as rul
                     INNER JOIN t_repositories as rep on rul.repository_id=rep.id_repository
                     INNER JOIN t_filetypes as ftyp on rul.filetype_id=ftyp.id_filetype
					 INNER JOIN t_users as usr on rul.user_id=usr.id_user
					 INNER JOIN t_discoveryplugins plg on plg.id_discoveryplugin=rul.discoveryplugin_id
                     INNER JOIN t_transferprotocols as prot on rep.transferprotocol_id=prot.id
                     INNER JOIN t_knownhosts as hosts on rep.host_id=hosts.id
					 LEFT JOIN t_receptionruleslock lock on (rul.id_receptionrule=lock.receptionrule_id and lock.responsible=plg.name)
					 WHERE 
						{$hostnameCondition}
						AND rul.active=true
						AND rul.usermode_id=usr.mode
						AND (lock.tstamp < current_timestamp - {$this->_dbHandler->getSQLObj()->sql_quote_str($ruleslock_max_time)}::interval OR lock.receptionrule_id is null)
						AND ( rul.lastvisit + (rul.pollingperiod_sec::text || ' seconds')::interval   
							+ (((rul.retrycounter * rul.retryperiod_sec)::text || ' seconds')::interval)) 
							< {$this->_dbHandler->getSQLObj()->sql_auto_datetime()} ";  
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////
/////////
/////////    L A U N CH E R
/////////
/////////////////////////////////////////////////////////////////////////////////////////////////
if (basename($_SERVER['argv'][0]) == basename(__FILE__)) {
	$engine = new pds2_import_adgs(basename(@ $_SERVER['argv'][1]));
    $engine->daemonize(60);
}
?>