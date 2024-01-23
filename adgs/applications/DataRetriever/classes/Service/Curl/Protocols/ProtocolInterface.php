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

interface ProtocolInterface {
	/**
	 * fetches products from the remote repository
	 * @param string $url
	 * @param array $args
	 * @param array & $protocolInfo detailed information from the underlying protocol
	 * @return mixed list of products, in native protocol format
	 */
	public function discoveryProducts($url, $args, &$protocolInfo = null);
	
	/**
	 * get a product metadata information
	 * @param string $url
	 * @param string $productId
	 * @param array & $protocolInfo detailed information from the underlying protocol
	 * @return mixed product metadata, in native protocol format
	 */
	public function getProductMetadata($url, $productId, &$protocolInfo = null);
	
	/**
	 * downloads a product file from the remote repository
	 * @param string $url
	 * @param string $productId
	 * @param string $localFilename
	 * @param array & $protocolInfo detailed information from the underlying protocol
	 */
	public function getProduct($url, $productId, $localFilename, &$protocolInfo = null);
}

?>