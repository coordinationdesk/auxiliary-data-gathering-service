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

namespace Acs\PHPImport\Service\Curl\Protocols;

use Acs\PHPImport\Service\Curl\CurlDataRetriever;

class OData implements ProtocolInterface {
	
	/**
	 * @var CurlDataRetriever
	 */
	protected $curlDataRetriever;
	
	/**
	 * specify if propagate headers following redirectes when downloading files
	 * @var bool
	 */
	protected $propagateHeadersRedirectDownload;
	
	/**
	 * specify whether skip "format" argument in odata calls
	 * @var bool
	 */
	protected $omitFormat;
	
	public function __construct(CurlDataRetriever $curlDataRetriever, $omitFormat, $propagateHeadersRedirectDownload) {
		$this->curlDataRetriever = $curlDataRetriever;
		
		// for now, only limit headers during download redirects only
		$this->propagateHeadersRedirectDownload = $propagateHeadersRedirectDownload;
		
		$this->omitFormat = $omitFormat;
	}
	
	protected function getProductUrl($baseUrl, $productId, $getValue = false) {
		return sprintf("%s(%s)%s", $baseUrl, $productId, $getValue ? '/$value' : '');
	}
	
	protected function filterFormatArg(& $args) {
		if ($this->omitFormat == true) {
			unset($args['$format']);
		}
	}
	
	public function discoveryProducts($url, $args, & $protocolInfo = null) {
		
		$defaultArgs = array('$format' => 'application/json');
		$this->filterFormatArg($defaultArgs);
		$this->filterFormatArg($args);
		
		$ret = $this->curlDataRetriever->get(
			$url, 
			array_merge($defaultArgs, $args), 
			array(), 
			PHP_QUERY_RFC3986);

		if ($protocolInfo !== null) $protocolInfo = $this->curlDataRetriever->getLastCurlResponse();
		
		$this->curlDataRetriever->checkLastCurlResponse();
		
		$productsListJSON = json_decode($ret, true);
		if ($productsListJSON === null) {
			throw new \Exception("{$url}: 'get product lists' response is not a json");
		}
		
		return $productsListJSON;
	}
	
	public function getProductMetadata($url, $productId, & $protocolInfo = null) {
		
		$args = array('$format'=>'application/json');
		$this->filterFormatArg($args);
		
		$productMetadataJSON = $this->curlDataRetriever->get(
			$this->getProductUrl($url, $productId, false), 
			$args,
			array(),
			PHP_QUERY_RFC3986);
		
		if ($protocolInfo !== null) $protocolInfo = $this->curlDataRetriever->getLastCurlResponse();
		
		$this->curlDataRetriever->checkLastCurlResponse();
		
		$productMetadata = json_decode($productMetadataJSON, true);
		if ($productMetadata === null) {
			throw new \Exception("response is not a json");
		}
		return $productMetadata;
	}
	
	public function getProduct($url, $productId, $localFilename, & $protocolInfo = null) {
		$fh = fopen($localFilename, 'w');
		if ($fh === false) {
			throw new \Exception("Cannot open temporary file {$localFilename}");
		}
		
		// put the stream contents to the open file
		$options = array( CURLOPT_FILE => $fh );
		
		// change propagate headers to "download" propagate redirect headers
		$propagateHeaders = $this->curlDataRetriever->getPropagateHeadersRedirect();
		$this->curlDataRetriever->setPropagateHeadersRedirect($this->propagateHeadersRedirectDownload);
		
		try {
			$this->curlDataRetriever->get(
				$this->getProductUrl($url, $productId, true), 
				array(),
				$options,
				PHP_QUERY_RFC3986);
			
			// change back propagate headers option to previous value
			$this->curlDataRetriever->setPropagateHeadersRedirect($propagateHeaders);
			fclose($fh);
		} catch (\Exception $e) {
			// change back propagate headers option to previous value
			$this->curlDataRetriever->setPropagateHeadersRedirect($propagateHeaders);
			fclose($fh);
			throw $e;
		}
		
		if ($protocolInfo !== null) $protocolInfo = $this->curlDataRetriever->getLastCurlResponse();
		
		$this->curlDataRetriever->checkLastCurlResponse();
		
		if (!file_exists($localFilename)) {
			$error = error_get_last();
			throw new \Exception("Cannot write on $localFilename {$error['message']}");
		}
	}
}

?>