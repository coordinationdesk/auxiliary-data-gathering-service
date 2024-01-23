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

require_once(__DIR__.'/../vendor/autoload.php');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\PHPImportMainProcess;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\DB\PHPImportDB;

class PHPImportMainProcessTest extends PHPImportMainProcess {
	public function __construct($config) {
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
		
		// skip gearman client creation
	}
	
	public function addRuleTask($ruleId, $pluginClass, $rulePayload){
		PHPImportLogger::get()->debug("Class: ".__CLASS__." Method: ".__FUNCTION__);
		
		//*******It could be useful to check if the worker have a failure status and restart
		//them using supervisor utils or advise by log
		//supervisorctl status import-worker:import-worker-0
		//import-worker:import-worker-0    RUNNING   pid 1445, uptime 0:58:50
		PHPImportLogger::get()->info("Adding a task for the receptionrule id: $ruleId...");
		
		$attr = is_null($rulePayload['repo_attributes']) ? 'null' : "'{$rulePayload['repo_attributes']}'";
		$ns = $this->config->getPluginsNamespace();
		
		echo <<<EOF

\$wl = \$tiw->createWorkLoad(
	{$rulePayload['id_rule']},
	'{$rulePayload['pds_final_dir']}',
	'{$rulePayload['base_temp_dir']}',
	'{$ns}',
	'{$rulePayload['plugin']}',
	'{$rulePayload['repo_protocol']}',
	'{$rulePayload['repo_hostname']}',
	{$rulePayload['repo_port']},
	'{$rulePayload['repo_remotepath']}',
	'{$rulePayload['repo_username']}',
	'{$rulePayload['repo_password']}',
	{$attr}
);

EOF;
	}
}

$config = Config::get();

/**
 * get logger
 */
PHPImportLogger::create($config);
PHPImportLogger::get()->info('Launching PHP import...');

/**
 * run
 */
$engine = new PHPImportMainProcessTest($config);
try {
	//if the instance already exists, the following line doesn't create a new connection
	PHPImportDB::create($config);
	$engine->run();
} catch (Exception $e) {
	PHPImportLogger::get()->err('PHPImport error detected:'. $e->getMessage());
}

?>