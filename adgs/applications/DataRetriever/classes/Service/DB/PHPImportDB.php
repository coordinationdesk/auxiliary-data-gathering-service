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

namespace Acs\PHPImport\Service\DB;

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Log\PHPImportLogger;

class PHPImportDB {
    static protected $instance = null;
    
    protected $db;
    
    protected function __construct(Config $config) {
        $dsn = $config->getdbConfPDODsn();
        $this->db = new \PDO($dsn);
    }
    
    static public function create(Config $config) {
        self::$instance = new PHPImportDB($config);
    }

    static public function close() {     
        PHPImportLogger::get()->debug("Closing DB connection.");
        self::$instance = null;
    }
    
    static public function get() {
        return self::$instance->db;
    }
}


?>