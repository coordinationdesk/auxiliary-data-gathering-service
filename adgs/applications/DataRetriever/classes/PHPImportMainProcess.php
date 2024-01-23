<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2007 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
// |                                                                           |
// | Via Della Bufalotta, 378 - 00139 Roma - Italy                             |
// | http://www.acsys.it                                                       |
// | All rights reserved.                                                      |
// +---------------------------------------------------------------------------+
// | This is UNPUBLISHED PROPRIETARY SOURCE CODE of A.C.S. S.p.A.              |
// | The contents of this file may not be disclosed to third parties, copied or|
// | duplicated in any form, in whole or in part, without the prior written    |
// | permission of A.C.S. S.p.A.                                               |
// +---------------------------------------------------------------------------+
//

namespace Acs\PHPImport;

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\DB\PHPImportDB;
use Acs\PHPImport\Service\Gearman\PHPImportGearmanClient;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\Tools\PluginTools;
use Acs\PHPImport\Service\Gearman\TaskPayload;

class PHPImportMainProcess {
    
    protected $serverGroupName;
    protected $phpImportIdentifier;
    protected $importWorkerName;
    protected $config;
    protected $bookedRulesIds;
    protected $tasksAR;
    
    public function __construct(Config $config) {
        $this->config = $config;
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
        
        $this->serverGroupName = $this->config->getServerGroupName();
        $this->phpImportIdentifier = $this->config->getPhpImportIdentifier() . '-' . getmypid() . '-' . php_uname('n');
        $this->importWorkerName = $this->config->getImportWorkerName();
        
        //max number of reception rules managed by the current istance
        $this->maxParallelJobs = $this->config->getMaxParallelJobs();
        
        PHPImportLogger::get()->info("Server Group Name: ".$this->serverGroupName);
        PHPImportLogger::get()->info("php Import Identifier: ".$this->phpImportIdentifier);
        PHPImportLogger::get()->info("php Import worker name: ".$this->importWorkerName);
        PHPImportLogger::get()->info("max Parallel Jobs: ".$this->maxParallelJobs);
        
        //create German client
        PHPImportGearmanClient::create($this->config);
    }
    
    public function run(){
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
        
        $this->unlockStuckReceptionRules($this->config->getRuleLockMaxTime());
        
        $this->bookedRulesIds = $this->bookReceptionRules();
        if (empty($this->bookedRulesIds)) {
            PHPImportLogger::get()->info("No repositories to visit.");
            return;
        }
        
        $currentReceptionRules = $this->getCurrentReceptionRules($this->bookedRulesIds);
            
        foreach ($currentReceptionRules as $ruleRecord) {
            try {
                if ($this->config->isDeleteFromHistoryEnabled()) {
                	PluginTools::cleanHistoryFromEvictedCompletedProducts(
	                	PHPImportDB::get(),
	                	$ruleRecord['id_rule'],
	                    $this->config->getDeleteHistoryTimeWindowThresholdSec());
                }
                $this->addRuleTask($ruleRecord['id_rule'], $ruleRecord['plugin_class'], $ruleRecord);
            
            } catch(\Exception $e) {
            	
                PHPImportLogger::get()->error("M&C|Data Import|IMP|FAIL|jobid=\"{$ruleRecord['id_rule']}\"|message=\"cannot handle the reception rule {$ruleRecord['id_rule']} : " . $e->getMessage() . "\"|");               
                
                PluginTools::unLockTable(
                	PHPImportDB::get(),
                	$ruleRecord['id_rule'],
                	$ruleRecord['plugin'],
                    __CLASS__);
                
                PHPImportLogger::get()->error($e);
            }
        }
            
    }

    /**
     * This method unlocks the receptionrules locked for a time greater
     than the allowed (configured in Config*.php)
     */
    public function unlockStuckReceptionRules($ruleLockMaxTime){
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
        
        $tm = $this->config->getTablesMapping();
        
        $sql="DELETE FROM {$tm['t_srv_receptionruleslock']}
            WHERE tstamp < now() - '$ruleLockMaxTime'::interval ";
        $stmt=PHPImportDB::get()->prepare($sql);
        $stmt->execute();
        $stmt->closeCursor();
        $stmt = null;
    }
    
    public function bookReceptionRules(){
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
        
        $tm = $this->config->getTablesMapping();
        
        if ($this->config->getSkipNullServerGroups()) {
        	$serverGroupCondition = 'rul.hostname=:serverGroupName';
        } else {
        	$serverGroupCondition = 'rul.hostname=:serverGroupName or rul.hostname IS NULL';
        }
        
        //the following query books the reception rules assigned to this servergroup, 
        //the rules are booked per phpImportIdentifier (unique inside a servergroup)
        $sql= "INSERT INTO {$tm['t_srv_receptionruleslock']} as lock
	               (SELECT rul.id_receptionrule,plg.name,now(),:processidentifier as processidentifier
                        FROM {$tm['t_srv_receptionrules']} as rul
                        INNER JOIN {$tm['t_discoveryplugins']} plg on plg.id_discoveryplugin=rul.discoveryplugin_id
                        LEFT JOIN {$tm['t_srv_receptionruleslock']} lock1 on (rul.id_receptionrule=lock1.receptionrule_id and lock1.responsible=plg.name)
                    WHERE 
                        ({$serverGroupCondition}) 
	                    AND rul.active=true
                        AND plg.name!=:noPlugin 
	                    AND ( rul.lastvisit + (rul.pollingperiod_sec::text || ' seconds')::interval 
		                      + (((rul.retrycounter * rul.retryperiod_sec)::text || ' seconds')::interval))
                            < now()
	                    AND lock1.receptionrule_id is null
                    LIMIT :maxParallelJobs)";
        $stmt=PHPImportDB::get()->prepare($sql);
        $stmt->bindValue(':serverGroupName', $this->serverGroupName,\PDO::PARAM_STR);
        $stmt->bindValue(':processidentifier', $this->phpImportIdentifier,\PDO::PARAM_STR);
        $stmt->bindValue(':maxParallelJobs', $this->maxParallelJobs);
        $stmt->bindValue(':noPlugin', $this->config->getNoPlugin(),\PDO::PARAM_STR);
        $stmt->execute();
        
        // The following query is to know which receptionrules have been just locked. If they have been looked 
        // in a previous run the processidentifier is different because there is the gearman identifier job information
        // appended
        $rows=PHPImportDB::get()->query("SELECT receptionrule_id
            FROM {$tm['t_srv_receptionruleslock']} WHERE processidentifier ='{$this->phpImportIdentifier}'");
        
        $receptionRulesIds=array();
        foreach($rows as $row) {
            PHPImportLogger::get()->debug("reception rules right booked: ".print_r($row,true));
            $receptionRulesIds[]=$row['receptionrule_id'];
        }
        $stmt->closeCursor(); 
        $stmt = null; 
        return $receptionRulesIds;
    }
    
    public function getCurrentReceptionRules($rulesIds){
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__); 
        
        $db = PHPImportDB::get();
        
        $tm = $this->config->getTablesMapping();
        
        // check if additional repository attributes exists
        $cntStmt = $db->query("SELECT count(*) cnt FROM information_schema.columns WHERE table_name='{$tm['t_srv_repositories']}' and column_name='attributes'");
        $cnt = $cntStmt->fetch(\PDO::FETCH_ASSOC | \PDO::FETCH_COLUMN);
        
        $ns = $this->config->getPluginsNamespace();
        $rulesIdsInString = implode(',',$rulesIds);
        $attributes = $cnt > 0 ? ', "attributes" repo_attributes' : '';
        $tempDir = $this->config->getTempDir();
        
        $ruleData = $db->query("SELECT rul.pollingperiod_sec, rul.retryperiod_sec, 
            rul.id_receptionrule as id_rule, rul.connretries,rul.retrycounter,
            rep.symbolicname as repo_name,rul.php_timeout_sec as timeout,
            rul.localpath as pds_final_dir, '{$tempDir}' base_temp_dir,
			lower(prot.protocol) as repo_protocol,
            rep.username as repo_username,rep.password as repo_password,
            hosts.host_name as repo_hostname,rep.remotepath as repo_remotepath,rep.port as repo_port,
            ftyp.filetype as filetype,ftyp.nameregularexpression as regexp,ftyp.id_filetype,
            rul.lastvisit as lastvisit,'{$ns}\' || plg.name AS plugin_class,
            plg.name as plugin
			$attributes
        FROM {$tm['t_srv_receptionrules']} as rul
            INNER JOIN {$tm['t_srv_repositories']} as rep on rul.repository_id=rep.id_repository
            INNER JOIN {$tm['t_filetypes']} as ftyp on rul.filetype_id=ftyp.id_filetype
            INNER JOIN {$tm['t_discoveryplugins']} plg on plg.id_discoveryplugin=rul.discoveryplugin_id
            INNER JOIN {$tm['t_transferprotocols']} as prot on rep.transferprotocol_id=prot.id
            INNER JOIN {$tm['t_knownhosts']} as hosts on rep.host_id=hosts.id
        WHERE rul.id_receptionrule in ($rulesIdsInString)"
            );
               
        return $ruleData->fetchAll(\PDO::FETCH_ASSOC);
    }
    
    public function addRuleTask($ruleId, $pluginClass, $rulePayload) {
        PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
        
        //*******It could be useful to check if the worker have a failure status and restart  
        //them using supervisor utils or advise by log
        //supervisorctl status import-worker:import-worker-0
        //import-worker:import-worker-0    RUNNING   pid 1445, uptime 0:58:50
        PHPImportLogger::get()->info("Adding a task for the receptionrule id: $ruleId...");
        
        $taskPayload = new TaskPayload(
        	$this->config, 
        	PHPImportLogger::get(), 
        	$pluginClass, 
        	$rulePayload);
        
        $this->tasksAr[$rulePayload['id_rule']] = PHPImportGearmanClient::get()->doBackground(
        	$this->importWorkerName, 
        	$taskPayload->serialize(),
        	$rulePayload['id_rule']);
        
        if (PHPImportGearmanClient::get()->returnCode() != \GEARMAN_SUCCESS){
            PHPImportLogger::get()->err("cannot add task for reception rule id: ".$rulePayload['id_rule']);
            PluginTools::unLockTable(PHPImportDB::get(),$rulePayload['id_rule'],$rulePayload['plugin'],
                __CLASS__);
            return;
        }
    }
    
}
?>