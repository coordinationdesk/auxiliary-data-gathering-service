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

namespace Acs\PHPImport\Service\Gearman;

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Log\PHPImportLogger;

abstract class PHPImportGearmanWorker {
	/**
	 * @var Config
	 */
	protected $config;
	
	protected $worker;
	
	/**
	 * @var \Monolog\Logger
	 */
	protected $internalLogger; 
	
	/**
	 * Expressed in msec, timeout of gearman worker 
	 * (timeout time is calculated from the last worker activity)
	 * @var integer	
	 */
	protected $workerTimeout = 3000;
	
	/**
	 * In seconds, time after a new addServers registration
	 * should be performed
	 * @var integer	
	 */
	protected $registrationLoopTime = 60;
	
	/**
	 * Time of the last addServers registration
	 * @var int
	 */
	protected $lastRegistrationTime = null;
	
	protected $gearmanPort = 4730;
	
	public function __construct(Config $configuration) {
		$this->config = $configuration;
		
		// setup internal logger
		PHPImportLogger::create($this->config);
		$this->internalLogger = PHPImportLogger::get();
		
		$this->internalLogger->debug("starting " . $this->getWorkerName() . " worker");
	}
	
	protected abstract function getWorkerName();
	
	/**
	 * returns an array with the list of gearman server to register to
	 * if only the default server should be used, an array with a null value
	 * shall be returned
	 * @return array
	 */
	protected abstract function getGearmanServers();
	
	protected function getGearmanPort() {
		return $this->gearmanPort;
	}
	
	protected function setupWorker() {
		if ($this->worker != null) {
			$this->worker->unregisterAll();
			unset($this->worker);
		}

		if ($this->createGearmanWorker($this->getGearmanServers()) == false) {
			throw new \RuntimeException("cannot register at least one gearman server, quitting");
		}
		
		$this->internalLogger->debug("registering executeTask to " . $this->getWorkerName());
		$this->worker->addFunction($this->getWorkerName(), array($this, 'executeTask'));
		
		$this->internalLogger->debug("setting worker timeout to {$this->workerTimeout} msec");
		$this->worker->setTimeout($this->workerTimeout);
	}
	
	protected function createGearmanWorker(array $servers) {
		
		/*
		 * check which gearman server is responding
		 */
		$activeServers = array();
		foreach ($servers as $server) {
			$tempWorker = new \GearmanWorker();
			try {
				if ($server == null) $server = '127.0.0.1';

				$this->internalLogger->debug("testing gearman server {$server}");
				$tempWorker->addServer($server, $this->getGearmanPort());

				$activeServers[] = $server;
			
			} catch (\GearmanException $e) {
				$this->internalLogger->err("gearman server {$server} not responding: " . $e->getMessage());
			}
			// drop tempWorker, reset error state in case of not responding server
			unset($tempWorker);
		}
		
		/*
		 * register only to active servers
		 */
		unset($this->worker);
		$this->worker = new \GearmanWorker();
		
		/*
		 * Improvement on queue distribuion:
		 * shuffle servers array to equally distribute queue consuming on 
		 * each task server
		 */
		shuffle($activeServers);
		
		foreach ($activeServers as $server) {
			$this->worker->addServer($server, $this->getGearmanPort());
			$this->internalLogger->debug("succesfully registered on gearman server {$server}");
		}
		
		$this->lastRegistrationTime = time();
		return count($activeServers) > 0;
	}
	
	public function process() {
		$this->setupWorker();
		
		$i = 10;
		
		while ( $this->worker->work() || $this->worker->returnCode() == GEARMAN_TIMEOUT ) {
			if ($this->worker->returnCode() == GEARMAN_TIMEOUT) {
				// check if the elapsed time from the last registration is exceeded
				if (time() > $this->lastRegistrationTime + $this->registrationLoopTime) {
				
				$this->internalLogger->debug("re-registering gearman servers");
				$this->setupWorker();
				}
			} else {
				$i--;
				// the sleep is to avoid that supervisor thinks that the process 'Exited too quickly' and put the
				//status of the worker to 'failure'.This status requires a manual restart of the workers.
				sleep(1);
				if ($i == 0) {
					$this->worker->unregisterAll();
					// terminate execution to free every resource allocated,
					// the exit status is set to 1 to allow supervisor to restart the process.
					exit(1);
				}
			}
		}
		$this->worker->unregisterAll();
		
		// let supervisord restart the worker
		exit(1);
	}
	
	abstract public function executeTask($job);
}

?>