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

namespace Acs\PHPImport\Service\Curl\HttpAuthentication;

use Acs\PHPImport\Config\Config;
use Monolog\Logger;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\Curl\CurlExt;
use Acs\PHPImport\Service\Tools\PluginTools;

/**
 * OAuth2 token info data structure
 */
class OAuth2TokenInfo {
	public $accessToken;
	public $expirationEpoch;
}

class HttpAuthenticationOAuth2 implements HttpAuthenticationInterface {
	static protected $accessToken;
	static protected $expirationEpoch;
	
	protected $username, $password;
	
	/**
	 * @var OAuth2TokenInfo
	 */
	protected $tokenInfo;
	
	/**
	 * @var Config
	 */
	protected $config;
	
	/**
	 * @var Logger
	 */
	protected $logger;
	
	/**
	 * @var CurlExt
	 */
	protected $curlExt;
	protected $repoAttributes;
	
	public function __construct(CurlExt $curlExt, $credentials, $repoAttributes) {
		$this->curlExt = $curlExt;
		$this->repoAttributes = $repoAttributes;
		
		$this->username = substr($credentials, 0, strpos($credentials, ':'));
		$this->password = substr($credentials, strpos($credentials, ':') + 1);
		
		$this->checkAuthAttributes();
		
		$this->config = Config::get();
		$this->logger = PHPImportLogger::get();
		
		// read current persisted token info, if any
		$this->tokenInfo = $this->readTokenInfo();
	}

	protected function checkAuthAttributes() {
		if (!property_exists($this->repoAttributes, 'client_id') ||
			!property_exists($this->repoAttributes, 'client_secret') ||
			!property_exists($this->repoAttributes, 'token_url')) {
			throw new \RuntimeException("Authentication attributes must have client_id, client_secret and token_url attributes");
		}
	}
	
	public function getAuthenticationHeaders() {
		$authHeader = "OAUTH2-ACCESS-TOKEN: ";
		if (property_exists($this->repoAttributes, 'oauth2_header')) {
			/*
			 * OAuth2 standard token can be specified in the repository configuration
			 * json oauth2_header attribute as:
			 *
			 * "Authorization: Bearer "
			 */
			$authHeader = $this->repoAttributes->oauth2_header;
		}
		return array(
			$authHeader . $this->tokenInfo->accessToken
		);
	}
	
	public function checkAndPerformAuthentication() {
		$expired = $this->tokenInfo->expirationEpoch != null && $this->tokenInfo->expirationEpoch < time();
		
		if ($this->tokenInfo->accessToken == null || $expired == true) {
			if ($expired) {
				$this->logger->notice("current OAuth2 token is expired, requesting a new one");
			} else {
				$this->logger->debug("requesting new OAuth2 access_token");
			}
			
			$this->curlExt->setDefaultHeaders();
			$this->curlExt->headers[] = "Authorization: Basic " . base64_encode(
				$this->repoAttributes->client_id . ':' . $this->repoAttributes->client_secret);
			
			$ret = $this->curlExt->post(
				$this->repoAttributes->token_url,
				array(
					'grant_type' => 'password',
					'username' => $this->username,
					'password' => $this->password,
					'scope' => 'openid'
				),
				PluginTools::mixCurlOptions(
					array(
						CURLOPT_SSL_VERIFYHOST => false,
						CURLOPT_SSL_VERIFYPEER => false
					),
					PluginTools::getRepoAttributesCurlOptions($this->repoAttributes))
				);
			
			$httpResponse = $this->curlExt->getLastHttpResponse();
			
			$this->logger->debug("OAuth2 access_token response code: {$httpResponse['http_code']}");
			
			try {
				try {
					$this->curlExt->checkLastResponse();
				} catch (\Exception $e) {
					$this->logger->err("OAuth2 access_token response:" . $ret);
					throw new \Exception($ret);
				}
				
				$jsonAuth = json_decode($ret);
				if ($jsonAuth === null) {
					throw new \Exception('response is not json: ' . $ret);
				}
				
				if (!property_exists($jsonAuth, 'access_token')) {
					throw new \Exception('json response does not contains access_token: ' . $ret);
				}
				
				if (property_exists($jsonAuth, 'expires_in')) {
					$this->logger->debug("token will expire in {$jsonAuth->expires_in} seconds");
					$this->tokenInfo->expirationEpoch = time() + $jsonAuth->expires_in;
				}
				
				$this->tokenInfo->accessToken = $jsonAuth->access_token;
				
				// persist token info
				$this->writeTokenInfo($this->tokenInfo);
				
			} catch (\Exception $e) {
				throw new \RuntimeException("Cannot get OAuth2 access token", null, $e);
			}
		}
	}
	
	/*
	 * OAuth2 token persistence methods
	 *
	 * https://projectit.exprivia.it/browse/CPS-118
	 *
	 */
	protected function getTokenInfoFile() {
		$path = $this->config->getRepoRuntimeDataDirectory();
		return sprintf("%s/%s-%s.phps", $path, $this->repoAttributes->client_id, $this->username);
	}
	
	protected function getEmptyTokenInfo() {
		return new OAuth2TokenInfo();
	}
	
	protected function readTokenInfo() {
		$tokenInfoFile = $this->getTokenInfoFile();
		
		if (!file_exists($tokenInfoFile)) {
			return $this->getEmptyTokenInfo();
		}
		
		$fp = fopen($tokenInfoFile, "r");
		
		if (flock($fp, LOCK_EX)) {  // acquire an exclusive lock
			$data = fread($fp, 8 * 1024);
			flock($fp, LOCK_UN);    // release the lock
			fclose($fp);
			
			$tokenData = unserialize($data);
			if (!is_object($tokenData) || !property_exists($tokenData, 'accessToken')) {
				return $this->getEmptyTokenInfo();
			} else {
				return $tokenData;
			}
			
		} else {
			fclose($fp);
			$this->logger->warning("cannot acquire the file lock on " . $tokenInfoFile);
			return $this->getEmptyTokenInfo();
		}
	}
	
	protected function writeTokenInfo($tokenInfo) {
		$tokenInfoFile = $this->getTokenInfoFile();
		
		$fp = fopen($tokenInfoFile, "w");
		
		if ($fp === false) {
			$this->logger->warning("cannot create the the token info file " . $tokenInfoFile);
			return;
		}
		
		if (flock($fp, LOCK_EX)) {  // acquire an exclusive lock
			$data = serialize($tokenInfo);
			$ret = fwrite($fp, $data);
			fflush($fp);            // flush output before releasing the lock
			flock($fp, LOCK_UN);    // release the lock
			fclose($fp);
			
			if ($ret != strlen($data)) {
				$this->logger->error("error writing to token info file " . $tokenInfoFile);
				@unlink($tokenInfoFile);
			}
		} else {
			fclose($fp);
			$this->logger->warning("cannot acquire the file lock on " . $tokenInfoFile);
		}
	}

}

?>