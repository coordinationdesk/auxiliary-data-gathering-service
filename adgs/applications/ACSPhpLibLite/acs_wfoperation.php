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
// $Prod: ACSPhpLib $  
// $Author$
// $Id$
// $Revision$
//
require_once 'ACSPhpLib/acs_exception.php';

class acs_wf_operation {
    const _OP_OK = 0;
    const _OP_NOK = 1;

    public static $countersString = array (
        self :: _OP_OK => 'OPERATION OK',
        self :: _OP_NOK => 'OPERATION NOK'
    );
    protected $_logHandler = null;
    protected $_dbHandler = null;
    protected $_iniVars = null;
    protected $_opVars = null;
    protected $_counters = null;
    protected $_simulation = false;
	protected $_dbHandlersAr = array();
    
    public function __construct(& $dbHandler, & $logHandler, & $storageHandler, & $iniVars, & $opVars) {
        $this->_dbHandler = $dbHandler;
        $this->_iniVars = $iniVars;
        $this->_opVars = $opVars;
        $this->_storageHandler = $storageHandler;
        $this->_simulation= $this->_opVars['mode']=='Simulation';

        if ($logHandler === null)
            throw new acs_exCritical(__METHOD__ . " : Log Handler undefined");
        $this->_logHandler = $logHandler;

        $this->_logHandler->debug(__METHOD__ . " " . __LINE__);
        
        $this->_counters = new acs_counters();
        foreach(self::$countersString as $op=>$label) {
            $this->_counters->newCounter($op,$label);
        }
    }

    public function execute($userid) {
        throw new acs_exCritical(__METHOD__." function undefined");
    }

    public function getReport($verbose) {
        return $this->_counters->getReport($verbose);
    }
    
    public function getLogHandler(){
        return $this->_logHandler;    
    }

	public function getDBHandler($db_section='db'){
		if(array_key_exists($db_section,$this->_dbHandlersAr)) return $this->_dbHandlersAr[$db_section];
        // get the DB Handler, if section exists
        if(!array_key_exists($db_section, $this->_iniVars)) throw new acs_exINIError("Invalid DB Section => $db_section");
        $this->_dbHandlersAr[$db_section] = acs_tools::getDBHandler($db_section,$this->_iniVars,$this->_logHandler);
        if($db_section=='db') $this->_dbHandler = $this->_dbHandlersAr[$db_section];
		
        return $this->_dbHandlersAr[$db_section];
	}
}
?>
