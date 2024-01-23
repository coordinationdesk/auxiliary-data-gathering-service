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

use Acs\PHPImport\Service\Curl\Protocols\ODataFactory;
use Acs\PHPImport\Service\Exception\PhpImportCannotDiscoveryProductsException;
use Acs\PHPImport\Service\Plugin\Data\Product;
use Acs\PHPImport\Service\Tools\PluginTools;

class DataRetrieverPluginOData extends DataRetrieverPlugin implements DataRetrieverPluginInterface {
    
    protected $startPublicationDate = null;

    
    /**
     * Setup the discovery filter for the current page of products discovery
     * 
     * @return array associative with query filters
     */
    protected function getDiscoveryFilter() {
    	$args = array(
    		'$orderby' => 'PublicationDate asc',
    		'$top' => $this->config->getPripRequestTop()
    	);
    	
    	$repoAttributes = $this->getRepoJsonAttributes();
    	if ($repoAttributes !== null) {
    		
    		if (property_exists($repoAttributes, 'odata_filter')) {
    			$args['$filter'] = $repoAttributes->odata_filter;
    		}
    		
    	}
    	
    	// try to initialize the startPublicationDate
    	if ($this->startPublicationDate == null) {
    			
			$this->startPublicationDate = PluginTools::getMaxFileCreationTimeStampForRule(
				$this->dbHandler, 
				$this->rulePayload['id_rule'], 
				$this->config->getDiscoveryTimeThresholdMSec());
		}
    	
    	if ($this->startPublicationDate != null) {
    		$publicationFilter = 'PublicationDate ge ' . $this->startPublicationDate;
    		if (array_key_exists('$filter', $args)) {
    			$args['$filter'] .= ' and ' . $publicationFilter;
    		} else {
    			$args['$filter'] = $publicationFilter;
    		}
    	}
    	if (array_key_exists('$filter', $args)) {
    		$this->logger->debug("using filter = {$args['$filter']}");
    	}
    	
    	return $args;
    }
    
    /**
     * analyze the returned product from discovery to setup next filter values
     * and filters the product list if needed
     * 
     * @param array[Product] $currentPageProducts
     * @return array the actual list of products to be returned
     */
    protected function discoveryFilterPagePostProcess($currentPageProducts) {
    	$currentPageProductsCount = count($currentPageProducts);
    	
    	if ($currentPageProductsCount > 0) {
    		$lastRecordPublicationDate = $currentPageProducts[$currentPageProductsCount - 1]->getPublicationDate();
    		if ($this->startPublicationDate == $lastRecordPublicationDate) {
    			// this happens every time the end of the recordset is reached,
    			// due to the "GE" operator used in the PublicationDate $filter
    			$this->logger->notice("PublicationDate {$lastRecordPublicationDate} is the same of the previous request, reached the end of products discovery queue");
    			return array();
    		}
    		
    		// adjust next PublicationDate start to the last record of this page
    		$this->startPublicationDate = $lastRecordPublicationDate;
    	}
    	
    	return $currentPageProducts;
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
    			$product['Id'],
    			$product['Name'],
    			$product['PublicationDate'],
    			$product['EvictionDate'],
    			$product['ContentLength']
    		);
    		unset($discoveredProductsList[$k]);
    	}
    }
    
    public function discoveryPage() {
    	$this->logger->debug($this->identifier.":Class: ".__CLASS__." Method: ".__FUNCTION__);
    	
    	//call the getProductsList
    	//NOTE 1: HTTPS managed with CURLOPT_SSL_VERIFYHOST and CURLOPT_SSL_VERIFYPEER set to false, the current PRIPs
    	//do not have a valid certificate. It should support, instead a correct handshake.
    	// NOTE 2: Add a parameter for order the results is not needed because the PRIP ICD v1.3 states:
    	// :By default the Products query are to be ordered by Publication Date, in an ascending order
    	//(e.g. any query for new products published since a previous query will find the list ordered
    	//from oldest to newest)
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
    	$this->cleanupDiscoveredProductsList($productsListJSON['value'], $currentPageProducts);
    	
    	return $this->discoveryFilterPagePostProcess($currentPageProducts);
    }
    
    public function createProtocol($timeout, $credential, $repoAttributes) {
    	return ODataFactory::create($timeout, $credential, $repoAttributes);
    }
}

?>