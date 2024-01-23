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
use Acs\PHPImport\Service\Curl\CurlExt;
use Acs\PHPImport\Service\Curl\HttpAuthentication\HttpAuthenticationFactory;

class ODataFactory {
	
	static public function create($timeout, $credentials, $repoAttributes) {
		$propagateHeadersRedirect = true;
		$propagateHeadersRedirectDownload = true;
		$omitFormat = false;
		
		if ($repoAttributes != null) {
			if (property_exists($repoAttributes, 'propagate_headers_redirect')) {
				$propagateHeadersRedirect = $repoAttributes->propagate_headers_redirect;
			}
			if (property_exists($repoAttributes, 'propagate_headers_redirect_download')) {
				$propagateHeadersRedirectDownload = $repoAttributes->propagate_headers_redirect_download;
			}
			if (property_exists($repoAttributes, 'omit_format') && $repoAttributes->omit_format == true) {
				$omitFormat = true;
			}
		}

		$curlExt = new CurlExt($timeout, $propagateHeadersRedirect);
		return new OData(
			new CurlDataRetriever(
				$curlExt, 
				HttpAuthenticationFactory::create(
					$curlExt, 
					$credentials, 
					$repoAttributes), 
				$repoAttributes), 
			$omitFormat, 
			$propagateHeadersRedirectDownload);
	}
}

?>