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
use Acs\PHPImport\Service\Gearman\PHPImportGearmanWorker;
use Acs\PHPImport\Service\Gearman\TaskPayload;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginInterface;

class DownloadWorker extends PHPImportGearmanWorker {
	protected function getGearmanServers() {
		return $this->config->getDownloadWorkerServers();
	}
	
	protected function getWorkerName() {
		return $this->config->getDownloadWorkerName();
	}
	
	public function executeTask($job) {
		$taskPayload = TaskPayload::unserialize($job->workload());
		$logger = $taskPayload->logger;
		$logger->debug("received download task");
		
		$classname = $taskPayload->pluginClass;
		
		try {
			$plugin = new $classname(
				$taskPayload->config, 
				$taskPayload->logger, 
				$taskPayload->rulePayload);
			
			if (! $plugin instanceof DataRetrieverPluginInterface) {
				throw new RuntimeException("class {$classname} is not an instance of DataRetrieverPluginInterface");
			}
			
			$plugin->download();

			$logger->debug("done download task");
		} catch(Exception $e) {
			$logger->error($e);
			throw $e;
		}
	}
}

$config = Config::get();
$worker = new DownloadWorker($config);
$worker->process();

?>