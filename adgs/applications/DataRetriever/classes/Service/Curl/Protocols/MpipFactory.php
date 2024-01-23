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

use Acs\PHPImport\Service\Curl\CurlExt;
use Acs\PHPImport\Service\Curl\CurlDataRetriever;
use Acs\PHPImport\Service\Curl\HttpAuthentication\HttpAuthenticationFactory;

class MpipFactory {
	static public function create($timeout, $credentials, $repoAttributes) {
		$propagateHeadersRedirect = true;
		$propagateHeadersRedirectDownload = true;
		
		if ($repoAttributes != null) {
			if (property_exists($repoAttributes, 'propagate_headers_redirect')) {
				$propagateHeadersRedirect = $repoAttributes->propagate_headers_redirect;
			}
			if (property_exists($repoAttributes, 'propagate_headers_redirect_download')) {
				$propagateHeadersRedirectDownload = $repoAttributes->propagate_headers_redirect_download;
			}
		}
		
		$curlExt = new CurlExt($timeout, $propagateHeadersRedirect);
		return new Mpip(
			new CurlDataRetriever(
				$curlExt,
				HttpAuthenticationFactory::create(
					$curlExt,
					$credentials,
					$repoAttributes),
				$repoAttributes),
			$propagateHeadersRedirectDownload);
	}
}

?>