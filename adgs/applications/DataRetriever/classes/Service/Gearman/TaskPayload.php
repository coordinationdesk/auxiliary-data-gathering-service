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

namespace Acs\PHPImport\Service\Gearman;

use Acs\PHPImport\Config\Config;
use Monolog\Logger;

class TaskPayload {
	/**
	 * @var Config
	 */
	public $config;
	
	/**
	 * @var Logger
	 */
	public $logger;
	
	/**
	 * @var string
	 */
	public $pluginClass;
	
	/**
	 * @var array
	 */
	public $rulePayload;
	
	public function __construct(Config $config, Logger $logger, $pluginClass, $rulePayload) {
		$this->config = $config;
		$this->logger = $logger;
		$this->pluginClass = $pluginClass;
		$this->rulePayload = $rulePayload;
	}

	/**
	 * @return string
	 */
	public function serialize() {
		return serialize($this);
	}
	
	/**
	 * @param string $string
	 * @return TaskPayload
	 */
	static public function unserialize($string) {
		return unserialize($string);
	}
}

?>