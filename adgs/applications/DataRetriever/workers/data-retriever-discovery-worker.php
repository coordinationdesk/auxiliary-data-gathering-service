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


//N.B. if some error happens here, the side effect is that the worker execution
//is terminated too early, supervisor restarts the processes only for  the 
//number of times defined in the configuration of supervisor 
//(...LTA_php_import/setup/conf/supervisor.d/import-worker.ini)
//once reached the max number of restart the status of the worker is set to 'failure'

require_once(__DIR__.'/../vendor/autoload.php');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Gearman\PHPImportGearmanWorker;
use Acs\PHPImport\Service\Gearman\TaskPayload;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginInterface;

class DiscoveryWorker extends PHPImportGearmanWorker {
	protected function getGearmanServers() {
		return array($this->config->getDiscoveryWorkerServer());
	}
	
	protected function getWorkerName() {
		return $this->config->getImportWorkerName();
	}

	public function executeTask($job) {
		/**
		 * @param serialized array $job. It contains the attributes of the single reception rule.
		 * The attributes Include the plugin name that has to manage the rule.
		 */
		$taskPayload = TaskPayload::unserialize($job->workload());
		$classname = $taskPayload->pluginClass;
		$logger = $taskPayload->logger;
		$logger->debug("received task for {$classname}");
		
		$job->sendStatus(1,3);
		try {
			$plugin = new $classname(
				$taskPayload->config, 
				$taskPayload->logger,
				$taskPayload->rulePayload);
			
			if (! $plugin instanceof DataRetrieverPluginInterface) {
				throw new RuntimeException("class {$classname} is not an instance of DataRetrieverPluginInterface");
			}
			
			$job->sendStatus(2,3);
			
			$plugin->discovery($taskPayload->rulePayload);
			
			$logger->debug("done task for {$classname}");
			
		} catch(Exception $e) {
			$logger->error($e);
			unset($plugin);
			throw $e;
		}
		
		// force plugin destruction
		unset($plugin);
		
		$job->sendStatus(3,3);
	}
}

$config = Config::get();
$worker = new DiscoveryWorker($config);
$worker->process();

?>