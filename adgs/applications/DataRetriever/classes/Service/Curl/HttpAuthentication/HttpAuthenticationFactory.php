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

use Acs\PHPImport\Service\Log\PHPImportLogger;

class HttpAuthenticationFactory {
	const _AUTH_TYPE_NONE = 'None';
	const _AUTH_TYPE_BASIC = 'Basic';
	const _AUTH_TYPE_OAUTH2 = 'OAuth2';
	
	public static function create($curlExt, $credentials, $repoAttributes) {
		$logger = PHPImportLogger::get();
		
		if ($credentials == null) {
			$authenticationType = self::_AUTH_TYPE_NONE;
		} else {
			$authenticationType = self::_AUTH_TYPE_BASIC;
		}
		if ($repoAttributes != null) {
			if (!property_exists($repoAttributes, 'authentication_type')) {
				$authenticationType = self::_AUTH_TYPE_BASIC;
			} else {
				$authenticationType = $repoAttributes->authentication_type;
			}
		}
		
		switch ($authenticationType) {
			case self::_AUTH_TYPE_NONE:
				$logger->debug("creating auth interface for no authentication");
				return new HttpAuthenticationNone();
				
			case self::_AUTH_TYPE_BASIC:
				$logger->debug("creating auth interface for basic authentication");
				return new HttpAuthenticationBasic($credentials);
				
			case self::_AUTH_TYPE_OAUTH2:
				$logger->debug("creating curl interface for OAuth2 authentication");
				return new HttpAuthenticationOAuth2($curlExt, $credentials, $repoAttributes);
				
			default:
				throw new \RuntimeException("unknown CurlOData class with credentials={$credentials}, authattr=" . print_r($repoAttributes, true));
		}
	}
}

?>