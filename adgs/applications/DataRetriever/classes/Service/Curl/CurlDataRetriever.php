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

namespace Acs\PHPImport\Service\Curl;

use Acs\PHPImport\Service\Curl\HttpAuthentication\HttpAuthenticationInterface;
use Acs\PHPImport\Service\Tools\PluginTools;

/**
 * Encapsulates a CurExt object to make calls to remote server,
 * but handles repository configuration via json object and
 * supports several methods of authentication
 * 
 * @author andrea
 *
 */
class CurlDataRetriever {
	/**
	 * @var CurlExt
	 */
	private $curl;

	/**
	 * @var HttpAuthenticationInterface
	 */
	protected $auth;
	
	/**
	 * @var \stdClass
	 */
	protected $repoAttributes;
	

	public function __construct(CurlExt $curlExt, HttpAuthenticationInterface $auth, $repoAttributes = null) {
		$this->curl = $curlExt;
		
		$this->auth = $auth;
		
		$this->repoAttributes = $repoAttributes;
	}
	
	public function setCurlTrace($active) {
		$this->curl->setVerbose($active);
	}
	
	protected function getDefaultCurlHeaders($contentType = null) {
		if ($contentType != null)
			return array('Content-Type: ' . $contentType);
		else
			return array();
	}
	
	protected function getCurlHeaders($contentType = null) {
		return array_merge(
			$this->getDefaultCurlHeaders($contentType),
			$this->auth->getAuthenticationHeaders()
		);
	}
	
	protected function getRepoAttributesCurlOptions() {
		return PluginTools::getRepoAttributesCurlOptions($this->repoAttributes);
	}
	
	protected function getCurlDefaultOptions($contentType = null) {
		$options = array(
			CURLOPT_HTTPHEADER => $this->getCurlHeaders($contentType),
			CURLOPT_SSL_VERIFYHOST => false,
			CURLOPT_SSL_VERIFYPEER => false,
			CURLOPT_FAILONERROR => true
			);
		return $options;
	}
	
	protected function getCurlOptions(array $additionalOptions = array(), $contentType = null) {
		// mix default and repo options
		$options = PluginTools::mixCurlOptions(
			$this->getCurlDefaultOptions($contentType),
			$this->getRepoAttributesCurlOptions()
			);

		// add user required options
		return PluginTools::mixCurlOptions(
			$options, 
			$additionalOptions
			);
	}
	
	public function getLastCurlResponse() {
		return $this->curl->getLastHttpResponse();
	}
	
	public function getLastCurlError() {
		return $this->curl->lastCurlError;
	}
	
	public function getLastCurlErrorMessage() {
		return $this->curl->lastCurlErrorMessage;
	}
	
	public function checkLastCurlResponse()	{
		$this->curl->checkLastResponse();
	}
	
	public function get($url, array $args = array(), array $additionalOptions = array(), $enc_type = PHP_QUERY_RFC1738) {
		$this->auth->checkAndPerformAuthentication();
		
		return $this->curl->get(
			$url, 
			$args, 
			$this->getCurlOptions($additionalOptions), 
			$enc_type);
	}
	
	public function post($url, $data, array $additionalOptions = array(), $contentType = 'application/x-www-form-urlencoded') {
		$this->auth->checkAndPerformAuthentication();

		return $this->curl->post(
			$url, 
			$data, 
			$this->getCurlOptions($additionalOptions, $contentType));
	}
	
	public function getPropagateHeadersRedirect() {
		return $this->curl->getPropagateHeadersRedirect();
	}
	
	/**
	 * @param boolean $propagateHeadersRedirect
	 */
	public function setPropagateHeadersRedirect($propagateHeadersRedirect) {
		$this->curl->setPropagateHeadersRedirect($propagateHeadersRedirect);
	}
}

?>