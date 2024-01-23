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

namespace Acs\PHPImport\Service\Curl;

use Acs\PhpTools\Curl;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\Exception\PhpImportCurlErrorException;
use Acs\PHPImport\Service\Exception\PhpImportTimeoutException;
use Acs\PHPImport\Service\Exception\PhpImportBadRequestException;
use Acs\PHPImport\Service\Exception\PhpImportFileNotFoundException;

/**
 * Extension for the library curl class, provides additional features
 * like verbose messages and redirect header propagation handling
 * 
 * @author andrea
 * 
 */
class CurlExt extends Curl {
	public $lastCurlError;
	public $lastCurlErrorMessage;
	protected $propagateHeadersRedirect = true;
	protected $verbose = 0;
	
	public function __construct($timeout = 30, $propagateHeadersRedirect = true, $cookies = false, $cookie = '/tmp/cookies.txt', $compression = 'gzip', $proxy = '') {
		parent::__construct($timeout, $cookies, $cookie, $compression, $proxy);
		$this->propagateHeadersRedirect = $propagateHeadersRedirect;
	}
	
	public function setVerbose($active) {
		$this->verbose = $active;		
	}
	
	protected function initConnection() {
		parent::initConnection();
		curl_setopt($this->connection, CURLOPT_VERBOSE, $this->verbose);
	}
	
	public function makeCall($url, $method, $additionalOptions, $data) {
		$this->initConnection();
		
		curl_setopt($this->connection, CURLOPT_URL, $url);
		curl_setopt($this->connection, CURLOPT_HTTPHEADER, $this->headers);
		
		$postSize = 0;
		
		if ($method == self::CURL_METHOD_POST) {
			$postSize = strlen($data);
			
			curl_setopt($this->connection, CURLOPT_POSTFIELDS, $data);
			curl_setopt($this->connection, CURLOPT_POST, 1);
		} else {
			curl_setopt($this->connection, CURLOPT_POST, 0);
			
			if ($method != self::CURL_METHOD_GET) {
				curl_setopt($this->connection, CURLOPT_CUSTOMREQUEST, $method);
			}
		}
		
		foreach ($additionalOptions as $k => $v) {
			curl_setopt($this->connection, $k, $v);
		}
		
		$logger = PHPImportLogger::get();
		$logger->info("CURL REQ : {$url} ({$postSize})");
		
		if ($this->propagateHeadersRedirect)
			$return = $this->curlExec();
		else 
			$return = $this->curlExecWithoutHeadersRedirect();
		
		$this->lastCurlError = curl_errno($this->connection);
		$this->lastCurlErrorMessage = curl_error($this->connection);
		
		$this->lastHttpResponse = curl_getinfo($this->connection);

		$logger->info("CURL RESP : {$url} (errno={$this->lastCurlError}, http={$this->lastHttpResponse['http_code']})");
		
		if($this->autoCloseConnection) {
			$this->closeConnection();
		}
		
		return $return;
	}
	
	protected function curlExec() {
		return curl_exec($this->connection);
	}
	
	protected function curlExecWithoutHeadersRedirect() {
		$logger = PHPImportLogger::get();
		
		// disable automatic location following
		curl_setopt($this->connection, CURLOPT_FOLLOWLOCATION, 0);
		
		$isRedirect = true;
		
		do {
			// make the request, first time with headers
			$return = curl_exec($this->connection);
			
			// after first call, reset headers
			curl_setopt($this->connection, CURLOPT_HTTPHEADER, array());
			
			$this->lastCurlError = curl_errno($this->connection);
			$this->lastCurlErrorMessage = curl_error($this->connection);
			
			$this->lastHttpResponse = curl_getinfo($this->connection);
			$code = $this->lastHttpResponse['http_code'];
			
			$isRedirect = intval($code / 100) == 3;
			if ($isRedirect) {
				$newUrl = $this->lastHttpResponse['redirect_url'];
				$logger->info("CURL REDIR : {$newUrl}");
				curl_setopt($this->connection, CURLOPT_URL, $newUrl);
			}
			
			// repeat while is a redirect
		} while ($isRedirect);
		
		return $return;
	}
	
	public function getLastCurlError() {
		return $this->lastCurlError;
	}
	
	public function getLastCurlErrorMessage() {
		return $this->lastCurlErrorMessage;
	}
	
	public function checkLastResponse()	{
		$response = $this->lastHttpResponse;

		// 22 = CURLE_HTTP_RETURNED_ERROR
		if ($this->lastCurlError != 0 &&
			$this->lastCurlError != 22) {
			throw new PhpImportCurlErrorException("error {$this->lastCurlError} returned by curl: " . $this->lastCurlErrorMessage);
		}
		
		if ($response['http_code'] == 200) {
			return true;
		}
		
		if ($response['http_code'] == 0) {
			$timeout = $this->timeout;
			if ($timeout !== null &&
				$response['total_time'] >= $timeout) {
					throw new PhpImportTimeoutException("TIMED OUT for url {$response['url']}");
				} else {
					throw new \Exception("Cannot contact url {$response['url']}: ");
				}
		}
		
		if ($response['http_code'] == 400) {
			throw new PhpImportBadRequestException("BAD REQUEST. REST error {$response['http_code']} for url {$response['url']} ");
		}
		
		if ($response['http_code'] == 401) {
			throw new \Exception("UNAUTHORIZED. REST error {$response['http_code']} for url {$response['url']} ");
		}
		
		if ($response['http_code'] == 404) {
			throw new PhpImportFileNotFoundException("NOT FOUND. REST error {$response['http_code']} for url {$response['url']}");
		}
		
		if ($response['http_code'] == 500) {
			throw new \Exception("INTERNAL SERVER ERROR. REST error {$response['http_code']} for url {$response['url']}: ");
		}
		
		$ret_category = (int)($response['http_code'] / 100);
		
		if ($ret_category == 2){
			return true;
		}
		
		if ($ret_category == 4 || 5){
			throw new \Exception("REST error {$response['http_code']} for url {$response['url']} ");
		}
	}
	
	/**
	 * @return boolean
	 */
	public function getPropagateHeadersRedirect()
	{
		return $this->propagateHeadersRedirect;
	}
	
	/**
	 * @param boolean $propagateHeadersRedirect
	 */
	public function setPropagateHeadersRedirect($propagateHeadersRedirect)
	{
		$this->propagateHeadersRedirect = $propagateHeadersRedirect;
		if ($propagateHeadersRedirect && is_resource($this->connection)) {
			curl_setopt($this->connection, CURLOPT_FOLLOWLOCATION, 1);
		}
	}
}

?>