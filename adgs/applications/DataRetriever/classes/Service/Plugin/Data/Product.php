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

namespace Acs\PHPImport\Service\Plugin\Data;

/**
 * Product bean
 * 
 * @author andrea
 *
 */
class Product {
	/**
	 * @var string unique identifier of the product, usually an uuid
	 */
	protected $id;
	
	/**
	 * @var string usually the file name of the Product
	 */
	protected $name;
	
	/**
	 * @var string publication date, %Y-%m-%dT%H:%M:%SZ
	 */
	protected $publicationDate;
	
	/**
	 * @var string eviction date, %Y-%m-%dT%H:%M:%SZ
	 */
	protected $evictionDate;
	
	/**
	 * @var int
	 */
	protected $size;

	public function __construct($id, $name, $publicationDate, $evictionDate, $size = null) {
		$this->id = $id;
		$this->name = $name;
		$this->publicationDate = $publicationDate;
		$this->evictionDate = $evictionDate;
		$this->size = $size;
	}

	/**
	 * @return string
	 */
	public function getId()
	{
		return $this->id;
	}

	/**
	 * @return string
	 */
	public function getName()
	{
		return $this->name;
	}

	/**
	 * @return string
	 */
	public function getPublicationDate()
	{
		return $this->publicationDate;
	}

	/**
	 * @return string
	 */
	public function getEvictionDate()
	{
		return $this->evictionDate;
	}
	
	/**
	 * @return number
	 */
	public function getSize()
	{
		return $this->size;
	}
}

?>