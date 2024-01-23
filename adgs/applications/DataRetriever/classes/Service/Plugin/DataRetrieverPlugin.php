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

namespace Acs\PHPImport\Service\Plugin;


define('_COMPLETED_', 'COMPLETED');
define('_ERROR_', 'ERROR');
define('_DOWNLOADERROR_', 'RECOVERABLEERROR');
define('_BOOKED_', 'BOOKED');
define('_NOTFOUNDERROR_', 'LOST');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Tools\PluginTools;
use Acs\PHPImport\Service\Exception\PhpImportCannotDiscoveryProductsException;
use Acs\PHPImport\Service\Exception\PhpImportCannotFilterException;
use Acs\PHPImport\Service\Exception\PhpImportCannotDownloadException;
use Acs\PHPImport\Service\Exception\PhpImportException;
use Acs\PHPImport\Service\Gearman\TaskPayload;
use Acs\PHPImport\Service\Plugin\Data\Product;
use Acs\PHPImport\Service\Exception\PhpImportFileNotFoundException;
use Acs\PHPImport\Service\Exception\PhpImportBadRequestException;
use Acs\PHPImport\Service\DB\PHPImportDB;
use Acs\PHPImport\Service\Curl\Protocols\ProtocolInterface;

/**
 * Base implementation for the DataRetrieverPlugin
 * 
 * Derive from this class implementing the DataRetrieverPluginInterface
 * methods writing the code to deal with the discovery/download
 * protocol to be used
 *  
 * @author andrea
 *
 */
abstract class DataRetrieverPlugin implements DataRetrieverPluginInterface {
	protected $rulePayload;
	protected $config;
	protected $logger;
	protected $dbHandler;
	protected $downloadWorkerName;
	
	const _DOWNLOAD_PRIORITY_HIGH = 'high';
	const _DOWNLOAD_PRIORITY_MEDIUM = 'medium';
	const _DOWNLOAD_PRIORITY_LOW = 'low';
	
	/**
	 * Discovery process identifier (null during "download" action)
	 * @var string
	 */
	protected $identifier;
	
	public function __construct(Config $config, $logger, $rulePayload) {
		$this->config = $config;
		
		$this->logger = $logger;
		
		$dsn = $this->config->getdbConfPDODsn();
		$this->dbHandler = new \PDO($dsn);
		
		$this->rulePayload = $rulePayload;
		$this->downloadWorkerName = $this->config->getDownloadWorkerName();
	}
	
	public function discovery() {
		$this->identifier = $this->rulePayload['id_rule'] . "-" . $this->rulePayload['repo_name'] . "-" . $this->rulePayload['filetype'];
		
		$this->logger->debug($this->identifier . ": Class: " . __CLASS__ . " Method: " . __FUNCTION__);
		$this->logger->info($this->identifier . ": discovering products...");
		
		try {
			try {
				////////////
				//RETRY
				////////////
				$page = 1;
				do {
					$currentPageProducts = $this->fetchRetryProducts();
					$currentPageProductsCount = count($currentPageProducts);
					
					$this->logger->info($this->identifier . ": page {$page} RETRY products: {$currentPageProductsCount}");
					
					if ($currentPageProductsCount > 0) {
						////////////
						//DOWNLOAD
						////////////
						$this->logger->info($this->identifier . ": page {$page} scheduling " . count($currentPageProducts) . " RETRY products downloads");
						$this->downloadProducts($currentPageProducts);
					}
					$page++;
					
				} while ($currentPageProductsCount > 0);
				
				////////////
				//DISCOVERY
				////////////
				$page = 1;
				do {
					PluginTools::extendReceptionRuleLock(
						$this->dbHandler,
						$this->rulePayload['id_rule']);
					
					$currentPageProducts = $this->discoveryPage();
					$currentPageProductsCount = count($currentPageProducts);
					
					$this->logger->info($this->identifier . ": page {$page} DISCOVERED products: {$currentPageProductsCount}");
					
					// filter known products and umatching products from the list
					if ($currentPageProductsCount > 0) {
						$this->filterProducts($currentPageProducts);
						
						////////////
						//DOWNLOAD
						////////////
						$this->logger->info($this->identifier . ": page {$page} scheduling " . count($currentPageProducts) . " DISCOVERED products downloads");
						$this->downloadProducts($currentPageProducts);
					}
					$page++;
					
				} while ($currentPageProductsCount > 0 &&
					$this->bookedItemsLimitExceeded($this->rulePayload['id_rule']) == false);
				
			} catch (PhpImportCannotDiscoveryProductsException $e) {
				$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$this->rulePayload['id_rule']}\"|message=\"cannot discovery products: " . $e->getMessage() . "\"|");
				throw $e;
			} catch (PhpImportCannotFilterException $e) {
				$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$this->rulePayload['id_rule']}\"|message=\"cannot filter products: " . $e->getMessage() . "\"|");
				throw $e;
			} catch (PhpImportCannotDownloadException $e) {
				$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$this->rulePayload['id_rule']}\"|message=\"cannot download products: " . $e->getMessage() . "\"|");
				throw $e;
			}
		} catch (PhpImportException $e) {
			//update the retry counter and last visit, but not last good
			$retryCounter = $this->rulePayload['retrycounter'] + 1;
			
			$this->finalizeConnection($retryCounter, true, false);
			
			$this->dbHandler = null;
			throw $e;
		}
		
		////////////
		//CLOSURE
		////////////
		
		//update the retry counter, last visit and last good
		$this->finalizeConnection(0, true, true);
		
		//close connection
		$this->dbHandler = null;
	}
	
	public function download() {
		try {
			$workload = $this->rulePayload;
			$baseTempDir = $workload['baseTempDir'];
			$destinationDir = $workload['finalDir'];
			
			/* @var Product $product */
			$product = $workload['product'];
			$productName = $product->getName();
			$productId = $product->getId();
			$productPublicationDate = $product->getPublicationDate();
			$productEvictionDate = $product->getEvictionDate();
			
			$tmpIdentifier = "{$workload['identifier']}-{$productName}";
			$tmpDir = PluginTools::createTempDir($baseTempDir, $tmpIdentifier);
			
			$tmpFileName = $tmpDir . '/' . $productName;
			$this->logger->info($workload['identifier'].":downloading {$productName} ({$productId}) ...");
			
			/*
			 * Product download
			 */
			$lastHttpResponse = array();
			$startTime = microtime(true);
			$productCurl = $this->createProtocol($workload['timeout'], $workload['credentials'], @$workload['repoAttributes']);
			try {
				$productCurl->getProduct($workload['url'], $productId, $tmpFileName, $lastHttpResponse);
			} catch (\Exception $e) {
				$this->logger->err("{$workload['identifier']}: download product failed: $e");
				throw $e;
			}
			
			/*
			 * Modify local file timestamp to match the product publication date
			 */
			if ($this->config->getUpdateDownloadedFileTimestamp()) {
				$remoteMTime = strtotime($productPublicationDate);
				if (!touch($tmpFileName, $remoteMTime)) {
					$this->logger->warn("Cannot set {$tmpFileName} mtime to {$remoteMTime}");
				} else {
					$this->logger->debug("{$tmpFileName} mtime set to {$remoteMTime} ({$productPublicationDate}) as remote product {$productName}");
				}
			}
			
			/*
			 * Time/size/speed estimations
			 */
			$fileSizeBytes = filesize($tmpFileName);
			$transferTime = microtime(true) - $startTime;
			$fileSize = floatval($fileSizeBytes)/ (1024 * 1024);
			$this->logger->info($workload['identifier'].": {$productName} downloaded to $tmpFileName ($fileSize MB, $transferTime sec, " . ($fileSize / $transferTime) . " MB/sec)");
			
			/*
			 * Metadata download
			 */
			if ($this->config->getDownloadMetadataFile()) {
				$tmpMetadataName = $tmpFileName . '.METADATA';
				
				try {
					$productMetadata = $productCurl->getProductMetadata($workload['url'], $productId);
				} catch (\Exception $e) {
					throw new \Exception("{$workload['identifier']}: get product metadata failed: $e");
				}
				
				if (file_put_contents($tmpMetadataName, json_encode($productMetadata)) === false) {
					$error = error_get_last();
					throw new \Exception($error['message']);
				}
				
				// moving into the final dir the metadata file
				$finalMetadataName = $destinationDir . '/' . $productName.'.METADATA';
				if (rename($tmpMetadataName, $finalMetadataName) === false) {
					$moveError=error_get_last();
					throw new \Exception($workload['identifier']."Cannot move file $tmpMetadataName to $finalMetadataName"
						.". ".$moveError['message']);
				}
			}
			
			// moving into the final dir the product file
			$finalFileName = $destinationDir . '/' . $productName;
			if (rename($tmpFileName, $finalFileName) === false) {
				$moveError=error_get_last();
				throw new \Exception($workload['identifier']."Cannot move file $tmpFileName to $finalFileName"
					.". ".$moveError['message']);
			}
			
			// IMP|IN message generated at the end of the download
			$tranferTimeStr = sprintf("%0.3f", $transferTime);
			$conntimeStr = sprintf("%0.3f", $lastHttpResponse['connect_time']);
			$this->logger->notice("M&C|Data Import|IMP|IN|jobid=\"{$workload['idRule']}\"|host=\"{$workload['host']}\"|fromurl=\"{$workload['url']}\"|filename=\"".$productName."\"|filesize={$fileSizeBytes}|creationtime=\"{$productPublicationDate}\"|jobtime={$tranferTimeStr}|conntime={$conntimeStr}|");
			
		} catch (PhpImportFileNotFoundException $e) {
			$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$workload['idRule']}\"|filename=\"{$productName}\"|message=\"cannot download {$productName}: ".$e->getMessage()."\"|");
			
			PluginTools::markAs(
				$this->dbHandler,
				$productName,
				_NOTFOUNDERROR_,
				$workload['idRule'],
				$productPublicationDate,
				$workload['identifier'],
				$productId,
				$productEvictionDate);
			PluginTools::removeTempDir($tmpDir,$workload['identifier']);
			throw new \Exception($workload['identifier'].": cannot download {$productName}:".$e);
		} catch (PhpImportBadRequestException $e) {
			// this is the very same as PhpImportFileNotFoundException
			$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$workload['idRule']}\"|filename=\"{$productName}\"|message=\"cannot download {$productName}: ".$e->getMessage()."\"|");
			
			PluginTools::markAs(
				$this->dbHandler,
				$productName,
				_NOTFOUNDERROR_,
				$workload['idRule'],
				$productPublicationDate,
				$workload['identifier'],
				$productId,
				$productEvictionDate);
			PluginTools::removeTempDir($tmpDir,$workload['identifier']);
			throw new \Exception($workload['identifier'].": cannot download {$productName}:".$e);
		} catch (\Exception $e) {
			$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$workload['idRule']}\"|filename=\"{$productName}\"|message=\"cannot download {$productName}: ".$e->getMessage()."\"|");
			
			PluginTools::markAs(
				$this->dbHandler,
				$productName,
				_DOWNLOADERROR_,
				$workload['idRule'],
				$productPublicationDate,
				$workload['identifier'],
				$productId,
				$productEvictionDate);
			PluginTools::removeTempDir($tmpDir,$workload['identifier']);
			throw new \Exception($workload['identifier'].": cannot download {$productName}:".$e);
		}
		
		PluginTools::markAs(
			$this->dbHandler,
			$productName,
			_COMPLETED_,
			$workload['idRule'],
			$productPublicationDate,
			$workload['identifier'],
			$productId,
			$productEvictionDate);
		
		$this->logger->notice($workload['identifier'].$productName. "  downloaded from '{$workload['repoName']}' matching the defined rules for filetype: '{$workload['idRule']}'");
		
		PluginTools::removeTempDir($tmpDir,$workload['identifier']);
		
		//close connection
		PHPImportDB::close();
	}
	
	protected function finalizeConnection($retryCount, $insertVisitLastTime, $insertGoodTStamp) {
		//update the retry counter, last visit and last good
		PluginTools::setConnectionCounterAndLastTimes(
			$this->dbHandler,
			$this->rulePayload['id_rule'],
			$retryCount,
			$insertVisitLastTime,
			$insertGoodTStamp,
			$this->identifier);
		
		//unlock table
		PluginTools::unLockTable(
			$this->dbHandler,
			$this->rulePayload['id_rule'],
			$this->rulePayload['plugin'],
			$this->identifier);
		
		//close connection
		$this->dbHandler = null;
	}
	
	protected function filterProducts(& $productsList){
		$this->logger->debug($this->identifier.":Class: ".__CLASS__." Method: ".__FUNCTION__);
		
		$initialProductsCount = count($productsList);
		
		try {
			if ($this->filterByRegexp($productsList)){
				$this->filterAlreadyDownloaded($productsList);
			}
		} catch (\Exception $e) {
			throw new PhpImportCannotFilterException($e->getMessage(), null, $e);
		}
		
		$this->logger->info($this->identifier . ": filterProducts " . count($productsList) . "/{$initialProductsCount} products to be processed");
	}
	
	/** 
	 * This method verifies regexp (also for the virtual filetype) of the
	 * list of products returned
	 * @param array[Product] $productsList 
	 */
	protected function filterByRegexp(&$productsList){
		$this->logger->debug($this->identifier . ":Class: " . __CLASS__ . " Method: " . __FUNCTION__);
		
		$file_rule_ar = PluginTools::getRuleAr($this->dbHandler ,$this->rulePayload['regexp'],$this->rulePayload['id_filetype'],$this->identifier);
		
		$initialProductsCount = count($productsList);
		
		//finding matches
		$ret = false;
		foreach ($productsList as $index => $product){
			$found = false;
			foreach ($file_rule_ar as $file_rule){
				if (preg_match("/$file_rule/", $product->getName())) {
					$found = true;
					$ret = true;
				}
			}
			if ($found == false) {
				unset($productsList[$index]);
			}
		}
		
		$this->logger->info($this->identifier . ": filterByRegexp: " . ($initialProductsCount - count($productsList)) . " products removed");
		
		return $ret;
	}
	
	/**
	 * Remove from the input list the products already downloaded
	 * 
	 * @param array[Product] $productsList
	 * @throws \Exception
	 */
	protected function filterAlreadyDownloaded(& $productsList) {
		$this->logger->debug($this->identifier . ":Class: " . __CLASS__ . " Method: " . __FUNCTION__);
		
		$tm = $this->config->getTablesMapping();
		
		$initialProductsListCount = count($productsList);
		
		$chunkSize = 10;
		if ($initialProductsListCount > $chunkSize) {
			$chunckedProductList = array_chunk($productsList, $chunkSize, true);
		} else {
			$chunckedProductList = array($productsList);
		}
		
		foreach ($chunckedProductList as $chunk) {
			$inString = implode(",", array_map(function($item) {
				return "'" . $item->getName() . "'";
			}, $chunk));
			
			$sql = <<<SQL
SELECT filename FROM {$tm['t_srv_receptionruleshist']}
WHERE filename in ({$inString})
    AND receptionrule_id = {$this->rulePayload['id_rule']}
SQL;
			$stmt = $this->dbHandler->query($sql);
			if ($stmt === false){
				throw new \Exception("query to retrieve the already downloaded products failed");
			}
				
			// build an array with completed and booked filenames for this chunk
			$knownFiles = array();
			foreach ($stmt->fetchAll() as $row) {
				$knownFiles[] = $row['filename'];
			}
				
			// remove main array records corresponding to found completed and booked files
			foreach ($chunk as $productListKey => $productListItem) {
				if (in_array($productListItem->getName(), $knownFiles))
					unset($productsList[$productListKey]);
			}
		}
		$this->logger->info($this->identifier . ": filterAlreadyDownloaded: " . ($initialProductsListCount - count($productsList)) . " products removed");
	}
	
	protected function fetchRetryProducts() {
		$this->logger->debug($this->identifier . ":Class: " . __CLASS__ . " Method: " . __FUNCTION__);
		
		$tm = $this->config->getTablesMapping();
		
		$retryStatuses = "'" . _DOWNLOADERROR_ . "'";
		$sql = <<<SQL
SELECT filename, remoteuuid, filecreationtstamp, evictiondate
	FROM {$tm['t_srv_receptionruleshist']}
	WHERE
		status_id IN (SELECT id FROM {$tm['t_srv_statuses']} WHERE status IN ({$retryStatuses})) AND
		receptionrule_id = {$this->rulePayload['id_rule']}
LIMIT 100
SQL;
		$stmt = $this->dbHandler->query($sql);
		if ($stmt === false){
			throw new \Exception("query to retrieve the retry products failed");
		}
		
		$ret = array();
		foreach ($stmt->fetchAll() as $row) {
			$ret[] = new Product(
				$row['remoteuuid'],
				$row['filename'], 
				$row['filecreationtstamp'], 
				$row['evictiondate']);
		}
		
		return $ret;
	}
	
	protected function createGearmanClient() {
		$gClient = new \GearmanClient();
		$gClient->addServer($this->config->getDiscoveryWorkerDownloadServer());
		return $gClient;
	}
	
	/**
	 * Creates the gearman payload for a download task
	 * 
	 * @param string $url
	 * @param Product $product
	 * @return \Acs\PHPImport\Service\Gearman\TaskPayload
	 */
	protected function createGearmanPayload($url, $product) {
		$taskPayload = new TaskPayload(
			$this->config,
			$this->logger,
			get_class($this),
			array(
				'product' => $product,
				'credentials' => PluginTools::getRepositoryCredentialsFromPayload($this->rulePayload, $this->config),
				'timeout' => $this->rulePayload['timeout'],
				'url' => $url,
				'host' => $this->rulePayload['repo_hostname'],
				'idRule' => $this->rulePayload['id_rule'],
				'repoName' => $this->rulePayload['repo_name'],
				'finalDir' => $this->rulePayload['pds_final_dir'],
				'baseTempDir' => $this->config->getTempDir(),
				'identifier' => $this->identifier,
				'repoAttributes' => $this->getRepoJsonAttributes()
			)
			);
		
		return $taskPayload;
	}
	
	protected function getRepoJsonAttributes() {
		if (array_key_exists('repo_attributes', $this->rulePayload)) {
			$jsonObject = @json_decode($this->rulePayload['repo_attributes']);
			if ($jsonObject === null) {
				$this->logger->warn("CANNOT DECODE JSON repo_attributes: {$this->rulePayload['repo_attributes']}");
			}
			return $jsonObject;
		} else {
			return null;
		}
	}
	
	protected function selectDownloadWorker($repoName) {
		$defaultWorkerName = $this->config->getDownloadWorkerName();
		$workerNames = $this->config->getDownloadWorkerNameByRepo();
		
		$this->logger->info("configured workers: " . join(",", array_keys($workerNames)) . "; current reponame: " . $repoName);
		
		if (!array_key_exists($repoName, $workerNames)) {
			$this->logger->info("using worker {$defaultWorkerName}");
			return $defaultWorkerName;
		} else {
			$this->logger->info("using worker {$workerNames[$repoName]}");
			return $workerNames[$repoName];
		}
	}
	
	/**
	 * Returns the download priority evaluating the product properties, in the
	 * default implementation the product name against the configuration attributes 
	 * $downloadHighPriorityProductNameRegex
	 * $downloadNormalPriorityProductNameRegex
	 * $downloadLowPriorityProductNameRegex
	 * 
	 * @param Product $product
	 * @return int one of _DOWNLOAD_PRIORITY_{LOW,MEDIUM,HIGH} constants
	 */
	protected function getDownloadPriority(Product $product) {
		$regex = $this->config->getDownloadHighPriorityProductNameRegex();
		if ($regex && preg_match("/{$regex}/", $product->getName())) {
			return self::_DOWNLOAD_PRIORITY_HIGH;
		}

		$regex = $this->config->getDownloadNormalPriorityProductNameRegex();
		if ($regex && preg_match("/{$regex}/", $product->getName())) {
			return self::_DOWNLOAD_PRIORITY_MEDIUM;
		}

		$regex = $this->config->getDownloadLowPriorityProductNameRegex();
		if ($regex && preg_match("/{$regex}/", $product->getName())) {
			return self::_DOWNLOAD_PRIORITY_LOW;
		}
				
		return self::_DOWNLOAD_PRIORITY_MEDIUM;
	}
	
	protected function scheduleDownloadJob($gClient, TaskPayload $payload) {
		$gearman_payload = $payload->serialize();
		
		/* @var Product $product */
		$product = $payload->rulePayload['product'];
	
		/*
		 * get the destination download worker starting by a repo basis
		 */
		$downloadWorkerName = $this->selectDownloadWorker($payload->rulePayload['repoName']);
		
		/*
		 * schedule download job in background
		 */
		$priority = $this->getDownloadPriority($product);
		switch ($priority) {
			case self::_DOWNLOAD_PRIORITY_HIGH:
				$gClient->doHighBackground($downloadWorkerName, $gearman_payload);
				break;
			case self::_DOWNLOAD_PRIORITY_MEDIUM:
				$gClient->doBackground($downloadWorkerName, $gearman_payload);
				break;
			case self::_DOWNLOAD_PRIORITY_LOW:
				$gClient->doLowBackground($downloadWorkerName, $gearman_payload);
				break;
			default:
				$this->logger->err("unknown priority for product: " . $product->getName());
				PluginTools::markAs(
					$this->dbHandler,
					$product->getName(),
					_DOWNLOADERROR_,
					$this->rulePayload['id_rule'],
					'',
					$this->identifier,
					$product->getId(),
					null);
				return;
		}
		
		if ($gClient->returnCode() != \GEARMAN_SUCCESS) {
			$this->logger->err("cannot add task for product: " . $product->getName());
			PluginTools::markAs(
				$this->dbHandler,
				$product->getName(),
				_DOWNLOADERROR_,
				$this->rulePayload['id_rule'],
				'',
				$this->identifier,
				$product->getId(),
				null);
		} else {
			$this->logger->info("Task succesfully scheduled for product: {$product->getName()}; priority: {$priority}");
		}
	}
	
	/**
	 * Schedules a batch of products to download
	 * 
	 * @param array[Product] $productsToDownload
	 * @throws PhpImportCannotDownloadException
	 */
	protected function downloadProducts($productsToDownload) {
		$this->logger->debug($this->identifier . ":Class: " . __CLASS__ . " Method: " . __FUNCTION__);
		$this->logger->notice("M&C|Data Import|IMP|STATS|jobid=\"{$this->rulePayload['id_rule']}\"|host=\"{$this->rulePayload['repo_hostname']}\"|label=\"{$this->rulePayload['repo_remotepath']}\"|status=\"ONLINE\"|sublabel=\"pending files\"|value=" . count($productsToDownload) . "|");
		
		try {
			$gClient = $this->createGearmanClient();
			
			foreach ($productsToDownload as $product) {
				
				$productName = $product->getName();
				
				$this->logger->info("Adding a task to download: {$productName} ...");
				PluginTools::markAs(
					$this->dbHandler,
					$productName,
					_BOOKED_,
					$this->rulePayload['id_rule'],
					$product->getPublicationDate(),
					$this->identifier,
					$product->getId(),
					$product->getEvictionDate());
				
				$url = PluginTools::getRepositoryUrlFromPayload($this->rulePayload);
				$payload = $this->createGearmanPayload($url, $product);
				
				if ($product->getSize() != null) {
					$this->logger->info("M&C|Data Import|IMP|NEXT|jobid=\"{$this->rulePayload['id_rule']}\"|host=\"{$this->rulePayload['repo_hostname']}\"|fromurl=\"{$url}\"|filename=\"{$product->getName()}\"|filesize={$product->getSize()}|creationtime=\"{$product->getPublicationDate()}\"|");
				} else {
					$this->logger->info("M&C|Data Import|IMP|NEXT|jobid=\"{$this->rulePayload['id_rule']}\"|host=\"{$this->rulePayload['repo_hostname']}\"|fromurl=\"{$url}\"|filename=\"{$product->getName()}\"|creationtime=\"{$product->getPublicationDate()}\"|");
				}
				
				$this->scheduleDownloadJob($gClient, $payload);
			}
		} catch (\Exception $e) {
			throw new PhpImportCannotDownloadException($e->getMessage(), null, $e);
		}
	}
	
	/**
	 * http://projectit.acsys.it/browse/LTA-353
	 * check if the current list of booked items is above a configured limit
	 *
	 * @param integer $ruleId
	 */
	protected function bookedItemsLimitExceeded($ruleId) {
		$maxDiscoveryQueuedTasks = $this->config->getMaxDiscoveryQueuedTasks();
		if ($maxDiscoveryQueuedTasks == 0) {
			$this->logger->notice("discovery queued tasks limit check disabled for reception rule {$ruleId}");
			return false;
		}
		
		$tm = $this->config->getTablesMapping();
		
		$sql = <<<SQL
select
	count(1) as cnt
from
	{$tm['t_srv_receptionruleshist']} tsr
	inner join {$tm['t_srv_statuses']} tss on tss.id = tsr.status_id
where
	tss.status = 'BOOKED'
	and tsr.receptionrule_id = {$ruleId}
SQL;
		$stmt = $this->dbHandler->query($sql);
		if ($stmt === false){
			throw new \Exception("query to retrieve the queued tasks failed");
		}
		
		$countRow = $stmt->fetch(\PDO::FETCH_ASSOC);
		$count = $countRow['cnt'];
		$exceeded = $count > $maxDiscoveryQueuedTasks;
		
		$str = sprintf("check queued tasks: ruleid: %d; limit: %d; actual items: %d; %s",
			$ruleId,
			$maxDiscoveryQueuedTasks,
			$count,
			$exceeded ? "LIMIT EXCEEDED" : "ok");
		
		if ($exceeded) {
			$this->logger->warning($str);
		} else {
			$this->logger->info($str);
		}
		
		return $exceeded;
	}
	
	/**
	 * Discoveries new products paginating
	 * the catalogue results
	 * @return array[Product]
	 */
	public abstract function discoveryPage();
	
	/**
	 * 
	 * @param int $timeout
	 * @param string $credential username:password
	 * @param object $repoAttributes repository extended attributes
	 * @return ProtocolInterface
	 */
	public abstract function createProtocol($timeout, $credential, $repoAttributes);
}

?>