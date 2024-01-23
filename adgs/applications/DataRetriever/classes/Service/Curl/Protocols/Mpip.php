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

namespace Acs\PHPImport\Service\Curl\Protocols;

use Acs\PHPImport\Service\Curl\CurlDataRetriever;

class Mpip implements ProtocolInterface {
	const MPIP_REQ_FILELIST_PATH = '/file';
	const MPIP_REQ_FILELIST_CONTENT_TYPE = 'application/json';
	const MPIP_REQ_DOWNLOAD_PATH = '/download';

	/**
	 * @var CurlDataRetriever
	 */
	protected $curlDataRetriever;
	
	/**
	 * specify if propagate headers following redirectes when downloading files
	 * @var bool
	 */
	protected $propagateHeadersRedirectDownload;
	
	public function __construct(CurlDataRetriever $curlDataRetriever, $propagateHeadersRedirectDownload) {
		$this->curlDataRetriever = $curlDataRetriever;

		$this->propagateHeadersRedirectDownload = $propagateHeadersRedirectDownload;
	}

	public function discoveryProducts($url, $filterPayload, & $protocolInfo = null) {
		$ret = $this->curlDataRetriever->post(
			$url . self::MPIP_REQ_FILELIST_PATH,
			json_encode($filterPayload),
			array(),
			self::MPIP_REQ_FILELIST_CONTENT_TYPE);
		
		if ($protocolInfo !== null) $protocolInfo = $this->curlDataRetriever->getLastCurlResponse();
		
		$this->curlDataRetriever->checkLastCurlResponse();
		
		$productsListJSON = json_decode($ret, true);
		if ($productsListJSON === null) {
			throw new \Exception("{$url}: 'get product lists' response is not a json");
		}
		
		return $productsListJSON;
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
				$url . self::MPIP_REQ_DOWNLOAD_PATH,
				array('filename' => $productId),
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

	public function getProductMetadata($url, $productId, & $protocolInfo = null) {
		/*
		 * MPIP extension does not provide a direct interface to get a product's metadata,
		 * it is necessary to call the "/files" endpoint to get the list of files
		 * matching the specified filename
		 */
		$filterPayload = array('filenames' => array($productId));
		$files = $this->discoveryProducts($url, $filterPayload, $protocolInfo);
		if (count($files) == 0) {
			throw new \Exception("cannot find product {$productId} on MPIP {$url}");
		}
		return $files[0];
	}

}

?>