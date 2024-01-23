#!/usr/bin/php
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

require_once(__DIR__.'/../vendor/autoload.php');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\PHPImportMainProcess;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\DB\PHPImportDB;

$config = Config::get();

ini_set('memory_limit', $config->getMemoryLimit());

/**
 * get logger
 */
PHPImportLogger::create($config);
PHPImportLogger::get()->info('Launching DataRetriever...');

/**
 * run
 */
$engine = new PHPImportMainProcess($config);
$delay = $config->getLoopDelay();
do {
    try{
        //if the instance already exists, the following line doesn't create a new connection
        PHPImportDB::create($config);
        $engine->run();
    } catch (Exception $e){
        PHPImportLogger::get()->err('DataRetriever error detected:'. $e->getMessage());
    }
    sleep($delay < 0 ? 0 : $delay);
}while($delay >=0);

?>