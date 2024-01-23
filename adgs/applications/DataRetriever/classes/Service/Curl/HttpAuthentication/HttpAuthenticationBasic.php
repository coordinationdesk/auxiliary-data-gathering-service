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


class HttpAuthenticationBasic implements HttpAuthenticationInterface {
	
	protected $credentials;
	
	public function __construct($credentials) {
		$this->credentials = $credentials;
	}
	
	public function getAuthenticationHeaders() {
		return array(
			"Authorization: Basic " . base64_encode($this->credentials)
		);
	}
	
	public function checkAndPerformAuthentication() {
		
	}

}

?>