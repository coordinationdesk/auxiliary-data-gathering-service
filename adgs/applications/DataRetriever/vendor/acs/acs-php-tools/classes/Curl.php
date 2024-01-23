<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2010 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.)    |
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
// $Prod:  $
// $Author:  $
// $Id:  $
// $Revision: $
//

namespace Acs\PhpTools;

class Curl {
	const CURL_METHOD_GET		= 'GET';
	const CURL_METHOD_POST		= 'POST';
	const CURL_METHOD_HEAD		= 'HEAD';
	const CURL_METHOD_DELETE	= 'DELETE';
	const CURL_METHOD_PUT		= 'PUT';
	const CURL_METHOD_OPTIONS	= 'OPTIONS';
	
	protected $lastHttpResponse;
	
	/*
	 * These properties are read only on initConnection
	 */
	public $headers;
	public $userAgent;
	public $compression;
	public $cookieFile;
	public $proxy;
	public $timeout;
	public $autoCloseConnection = true;
	public $connection = null;

	public function __construct($timeout = 30, $cookies = false, $cookie = '/tmp/cookies.txt', $compression = 'gzip', $proxy = '') {
		$this->setDefaultHeaders();
		
		$this->userAgent = 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.0.3705; .NET CLR 1.1.4322; Media Center PC 4.0)';
		$this->compression = $compression;
		$this->proxy = $proxy;
		$this->cookies = $cookies;
		$this->timeout = $timeout;
		
		if ($this->cookies == true) {
			$this->cookie($cookie);
		}
	}

	public function setDefaultHeaders() {
		$this->headers = array();
		$this->headers[] = 'Accept: */*';
		$this->headers[] = 'Accept-Language: it-IT';
		$this->headers[] = 'Pragma: no-cache';
		$this->headers[] = 'Connection: Keep-Alive';
	}

	public function get($url, array $args = array(), array $additionalOptions = array(), $enc_type = PHP_QUERY_RFC1738) {
	    if (count($args) > 0) {
	        $url .= '?' . http_build_query($args, null, ini_get('arg_separator.output'), $enc_type);
	    }
	    
	    return $this->makeCall($url, self::CURL_METHOD_GET, $additionalOptions, null);
	}

	public function post($url, $data, array $additionalOptions = array(), $contentType = 'application/x-www-form-urlencoded') {

		if (is_array($data))
			$data = http_build_query($data);

		$this->headers[] = 'Content-Type: '.$contentType;
		$this->headers[] = 'Content-Length: '.strlen($data);

		$ret = $this->makeCall($url, self::CURL_METHOD_POST, $additionalOptions, $data);
		
		array_pop($this->headers);	// remove content-type
		array_pop($this->headers);	// remove content-length
		
		return $ret;
	}
	
	public function getLastHttpResponse() {
		return $this->lastHttpResponse;
	}
	
	public function getLastHttpResponseCode() {
		return $this->lastHttpResponse['http_code'];
	}
	
	public function getLastHttpResponseHeaders() {
		return $this->lastHttpResponse['headers'];
	}
	
	public function closeConnection() {
		if (isset($this->connection)) {
			curl_close($this->connection);
			$this->connection = null;
		}
	}
	
	protected function cookie($cookie_file) {
		if (file_exists($cookie_file)) {
			$this->cookieFile=$cookie_file;
		} else {
			if(@fopen($cookie_file,'w')){
				$this->cookieFile=$cookie_file;
				@fclose($this->cookieFile);
			}
		}
	}
	
	protected function initConnection() {
		if (!isset($this->connection)) {
			$this->connection = curl_init();
			curl_setopt($this->connection, CURLOPT_SSL_VERIFYPEER, FALSE);
			curl_setopt($this->connection, CURLOPT_SSL_VERIFYHOST, 2);
			curl_setopt($this->connection, CURLOPT_HEADER, 0);
			curl_setopt($this->connection, CURLOPT_USERAGENT, $this->userAgent);
			if ($this->cookies == true) {
				curl_setopt($this->connection, CURLOPT_COOKIEFILE, $this->cookieFile);
				curl_setopt($this->connection, CURLOPT_COOKIEJAR, $this->cookieFile);
			}
			curl_setopt($this->connection,CURLOPT_ENCODING , $this->compression);
			curl_setopt($this->connection, CURLOPT_TIMEOUT, $this->timeout);
			curl_setopt($this->connection, CURLOPT_RETURNTRANSFER, 1);
		}
		
		curl_setopt($this->connection, CURLOPT_FOLLOWLOCATION, 1);
	}
	
	protected function makeCall($url, $method, array $additionalOptions, $data) {
		$this->initConnection($method, $additionalOptions);
		
		curl_setopt($this->connection, CURLOPT_URL, $url);
		curl_setopt($this->connection, CURLOPT_HTTPHEADER, $this->headers);
		
		if ($method == self::CURL_METHOD_POST) {
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
		
		$return = curl_exec($this->connection);
		$this->lastHttpResponse = curl_getinfo($this->connection);
		
		if($this->autoCloseConnection) {
			$this->closeConnection();
		}
		
		return $return;
	}
}

?>