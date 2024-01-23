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

namespace Acs\PHPImport\Service;

use Acs\PHPImport\Config\Config;
use Monolog\Logger;
use Monolog\Formatter\LineFormatter;
use Monolog\Handler\StreamHandler;
use Acs\PHPImport\Service\Log\PHPImportLogger;

class PHPImportLoggerTest extends PHPImportLogger {
    
    public function __construct() {
    	/*
    	 * use the PHPImportCustomLogger if you want the debug messages
    	 * to be logged in the syslog as info messages
    	 */
    	// $this->log = new PHPImportCustomLogger('phpImport');
    	$this->log = new Logger('phpImport');
    	$handler = new StreamHandler('php://stdout', Logger::DEBUG);
    	
    	$dateFormat = "Y n j, g:i a";
    	// the default output format is "[%datetime%] %channel%.%level_name%: %message% %context% %extra%\n"
    	$output = "%message%\n";
    	$formatter = new LineFormatter($output, $dateFormat);
    	
    	$handler->setFormatter($formatter);
    	
    	$this->log->pushHandler($handler);
    }

    static public function create(Config $config) {
        self::$instance = new PHPImportLoggerTest($config);
    }
}

?>