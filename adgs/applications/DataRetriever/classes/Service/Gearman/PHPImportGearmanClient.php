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

use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Config\Config;

class PHPImportGearmanClient {
    static protected $instance = null;
    
    protected $gClient;
    
    protected function __construct(Config $config) {
        $this->gClient = new \GearmanClient();
        
        try{
        	$this->gClient->addServer($config->getDiscoveryWorkerServer());
        }catch(\Exception $e){
            PHPImportLogger::get()->err(get_class($e).' : '.$e->getMessage());
            PHPImportLogger::get()->err("It seems that the Gearman deamon has not been started. Please check it.");
            throw($e) ;
        }
        PHPImportLogger::get()->info("The gearman client to send jobs to the local job server has been defined.");    
    }
    
    static public function create(Config $config) {
    	self::$instance = new PHPImportGearmanClient($config);
    }
    
    static public function get() {
        return self::$instance->gClient;
    }
}

?>