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

namespace Acs\PHPImport\Service\Plugin;

use Acs\PHPImport\Service\Curl\Protocols\MpipFactory;
use Acs\PHPImport\Service\Tools\PluginTools;
use Acs\PHPImport\Service\Exception\PhpImportCannotDiscoveryProductsException;
use Acs\PHPImport\Service\Plugin\Data\Product;
use Acs\PHPImport\Config\Config;

class DataRetrieverPluginMpip extends DataRetrieverPlugin implements DataRetrieverPluginInterface {
	
	protected $startIngestionDate = null;
	protected $pageNumber = 0;
	
	protected function getFiletypeNames($parentFileType) {
		$tm = Config::get()->getTablesMapping();
		
		$sql = <<<SQL
SELECT tf.filetype FROM t_filetypes tf 
WHERE  
	case when 
			(SELECT count(*) FROM {$tm['t_filetypescomps']} where pfiletype = 
				(select id_filetype from {$tm['t_filetypes']} where filetype = '{$parentFileType}')) = 0 
		then
		tf.id_filetype = (select id_filetype from {$tm['t_filetypes']} where filetype = '{$parentFileType}')
	else
		tf.id_filetype in (SELECT cfiletype FROM {$tm['t_filetypescomps']} where pfiletype = 
			(select id_filetype from {$tm['t_filetypes']} where filetype = '{$parentFileType}'))
	end;
SQL;

		$filetypes = array();
		foreach ($this->dbHandler->query($sql) as $row) {
			$filetypes[] = $row['filetype'];
		}
		
		if (count($filetypes) == 0) {
			throw new \Exception("{$parentFileType} is not properly configured (does not exist or children are missing)");
		}
		
		return $filetypes;
	}
	
	protected function getDiscoveryFilter() {
		$args = array(
			'actives' => array(true),
			'pageNumber' => $this->pageNumber
		);
		
		if ($this->startIngestionDate == null) {
			$this->startIngestionDate = PluginTools::getMaxFileCreationTimeStampForRule(
				$this->dbHandler, 
				$this->rulePayload['id_rule'],
				$this->config->getDiscoveryTimeThresholdMSec());
		}
		if ($this->startIngestionDate != null) {
			$args['ingestionDate'] = $this->startIngestionDate;
		}
		if (array_key_exists('filetype', $this->rulePayload)) {
			$args['filetypes'] = $this->getFiletypeNames($this->rulePayload['filetype']);
		}
		
		$this->logger->debug("using filter = " . print_r($args, true));
		
		return $args;
	}
	
	/**
	 * Slims the memory footprint of the returned products list converting
	 * the returned json array in array of Product beans removing the
	 * unnecessary record fields
	 *
	 * @param array $discoveredProductsList
	 * @param array[Product] output array
	 */
	protected function cleanupDiscoveredProductsList($discoveredProductsList, & $currentPageProducts) {
		foreach ($discoveredProductsList as $k => $product) {
			$currentPageProducts[$k] = new Product(
				$product['filename'],
				$product['filename'],
				$product['ingestionDate'],
				null,
				null
				);
			unset($discoveredProductsList[$k]);
		}
	}
	
	protected function discoveryFilterPagePostProcess($currentPageProducts) {
		if (count($currentPageProducts) > 0) {
			$this->pageNumber++;
		} else {
			$this->logger->notice("empty product page");
		}
		
		return $currentPageProducts;
	}
	

	public function discoveryPage() {
		$this->logger->debug($this->identifier.":Class: ".__CLASS__." Method: ".__FUNCTION__);
		
		try {
			$discoveryCurl = $this->createProtocol(
				$this->rulePayload['timeout'],
				PluginTools::getRepositoryCredentialsFromPayload($this->rulePayload, $this->config),
				$this->getRepoJsonAttributes());
			
			$productsListJSON = $discoveryCurl->discoveryProducts(
				PluginTools::getRepositoryUrlFromPayload($this->rulePayload),
				$this->getDiscoveryFilter());

		} catch (\Exception $e) {
			$this->logger->error("M&C|Data Import|IMP|FAIL|jobid=\"{$this->rulePayload['id_rule']}\"|message=\"get product lists failed: " . $e->getMessage() . "\"|");
			$this->logger->notice("M&C|Data Import|IMP|STATS|jobid=\"{$this->rulePayload['id_rule']}\"|host=\"{$this->rulePayload['repo_hostname']}\"|label=\"{$this->rulePayload['repo_remotepath']}\"|status=\"FAILED\"|sublabel=\"pending files\"|value=-1|");
			throw new PhpImportCannotDiscoveryProductsException($this->identifier.":get product lists failed: " . $e->getMessage(), null, $e);
		}
		
		$currentPageProducts = array();
		$this->cleanupDiscoveredProductsList($productsListJSON, $currentPageProducts);
		
		return $this->discoveryFilterPagePostProcess($currentPageProducts);
	}

	public function createProtocol($timeout, $credential, $repoAttributes) {
		return MpipFactory::create($timeout, $credential, $repoAttributes);
	}
}

?>