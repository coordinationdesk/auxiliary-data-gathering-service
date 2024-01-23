<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2006 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.)    |
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

// ACS PHP Lib includes

require_once 'ACSPhpLib/acs_app.php';
require_once 'ACSPhpLib/acs_file.php';
require_once 'ACSPhpLib/acs_exception.php';

// tick use required as of PHP 4.3.0
declare(ticks = 1);

define('DAEMON_INI_SECTION','daemon_handled_runs');
define('DAEMON_RUN_CLASS_NAME','daemon_run_class_name');
define('DAEMON_DB_RUN_TABLE','run_dbtable');
define('DAEMON_FIELD_ID_RUN','field_id_run');
define('DAEMON_FIELD_RUN_NAME','field_run_name');
define('DAEMON_FIELD_RUN_OPERATION_TYPE','field_run_operation_type');
define('DAEMON_FIELD_RUN_INTERVAL_SECONDS','field_run_interval_seconds');
define('DAEMON_FIELD_LAST_RUN_DATETIME','field_last_run_datetime');
define('DAEMON_GROUPING_FIELDS','grouping_fields');

define('RUN_DISABLED', 0);
define('RUN_ENABLED', 1);
define('RUN_SIMULATION', 2);

class acs_daemon extends acs_app {

    protected $pid     = null;
    protected $pidfile = null ;
    protected $lockfile  = null ;
    private $_itsMe=false;
    protected $db_section;

    public function __construct($_iniFileName,$ident='',$isWebInterface=false,$db_section='db') {
    	$this->db_section = $db_section;
        try {
            parent :: __construct($_iniFileName,$ident,$isWebInterface,$this->db_section);

            $this->lockfile = array_key_exists('lock_file', $this->_iniVars['log']) ? $this->_iniVars['log']['lock_file'] : null;
            if( $this->lockfile==null){
                $this->lockfile = "/var/lock/subsys/".basename(__FILE__,'.php');
            }
            if($ident!='') $this->lockfile=dirname($this->lockfile).'/'.$ident.'.lck';

            $this->pidfile=$this->_iniVars['log']['pid_file'];
            if( $this->pidfile==null ){
                $this->pidfile="/var/run/".basename(__FILE__,'.php').".pid";
            }
            if($ident!='') $this->pidfile=dirname($this->pidfile).'/'.$ident.'.pid';

            $this->_logHandler->debug("Registering shutdown functions");
            register_shutdown_function(array($this,"shutdown"), array($this->pidfile) );
    
            $this->pid=getmypid();
    
            $this->_logHandler->debug(__FUNCTION__." ".__LINE__." "."Installing Signal Handlers");
            // setup signal handlers
            
            if( function_exists("pcntl_signal")){
                pcntl_signal(SIGHUP,  array($this,"sig_handler"),false);
                pcntl_signal(SIGINT,  array($this,"sig_handler"),false);
                pcntl_signal(SIGQUIT, array($this,"sig_handler"),false);
                pcntl_signal(SIGABRT, array($this,"sig_handler"),false);
                pcntl_signal(SIGTERM, array($this,"sig_handler"),false);
            }
            
            $this->_logHandler->debug(__FUNCTION__." ".__LINE__." "."Verifying lockfile : {$this->lockfile}");

            // Set lockfile --> N.B. This is posix dependent
            $prevPID=@file_get_contents($this->pidfile);
            if($prevPID!==false){
                //ok, there is a pid, but let's check if it points to an existing process
                $procFile = "/proc/$prevPID/cmdline";
                if (file_exists($procFile)){
                    $checkPid = join('', file($procFile));

                    $thereIsPID= strpos($checkPid,$ident)!==false; 
                } else
                    $thereIsPID=false;
                
                if(!$thereIsPID){
                    // do not test unlink result to avoid race condition where the old process can unlink 
                    // the file before the new process can do it
                    @unlink($this->pidfile);
                    // check if the pidfile is still there (cannot remove because of permissions, etc.)
                    if (file_exists($this->pidfile))
                        throw new Exception("Could not delete PIDFILE ".$this->pidfile);

                    $this->_logHandler->info("Found the PID file for process $ident: it has been deleted because it does not belong to an active process!");
                }else {
                    $this->_logHandler->info("There is already an instance of $ident running.");
                    exit;
                }
            }
    
            if (file_exists($this->pidfile) ){
                $this->_logHandler->debug(__FUNCTION__." ".__LINE__." Removing Stale pidfile");
                @unlink($this->pidfile);
            }
    
            // 1) Create PID file
            $this->_logHandler->debug(__FUNCTION__." ".__LINE__." "."Creating ".$this->pidfile.", with pid ".$this->pid);
            
			if (file_put_contents($this->pidfile, $this->pid) === false) {
                $this->_logHandler->err(__FUNCTION__." ".__LINE__." Cannot create ".$this->pidfile);
            }
            $this->_itsMe=true;

        } catch (Exception $e) {
            $this->handleSevereError($e->getMessage(),true);
        }
    }
    
    public function daemonize($REPEAT_DELTATIME_SECS=10){
        // reschedule process every XXX seconds
        $time_start = acs_daemon::microtime_float();
        
        while(true){
            $currRunStartTime = acs_daemon::microtime_float();
            $this->pre_run();
            $currRunEndTime = acs_daemon::microtime_float();
            $secsFromStart = $currRunEndTime-$time_start;
            $currRunDuration = $currRunEndTime-$currRunStartTime;
            if($currRunDuration<1) {
                sleep(1);
                $secsFromStart++;
            } // to ensure that the process lasts at least 1 sec
        
            $nextSchedTime=ceil($secsFromStart/$REPEAT_DELTATIME_SECS)*$REPEAT_DELTATIME_SECS;
            if($nextSchedTime+$currRunDuration>=60) die;
            sleep($nextSchedTime-$secsFromStart);
        }
    }

	private function getDaemonINISectionParam($paramName){
	    	if(!array_key_exists($paramName,$this->_iniVars[DAEMON_INI_SECTION])){
	    		throw new acs_exINIError($paramName.' entry not found in '.DAEMON_INI_SECTION.' of Ini file');
	    	}
	    	return $this->_iniVars[DAEMON_INI_SECTION][$paramName];
	}

    protected function pre_run() {
        if(array_key_exists(DAEMON_INI_SECTION,$this->_iniVars)){
        	// get list of RUNs from DB
	    	$dbRuntable				= $this->getDaemonINISectionParam(DAEMON_DB_RUN_TABLE);
	    	$runClassName			= $this->getDaemonINISectionParam(DAEMON_RUN_CLASS_NAME);
	    	$fieldIDRun				= $this->getDaemonINISectionParam(DAEMON_FIELD_ID_RUN);
	    	$fieldRunName			= $this->getDaemonINISectionParam(DAEMON_FIELD_RUN_NAME);
	    	$fieldRunOperationType	= $this->getDaemonINISectionParam(DAEMON_FIELD_RUN_OPERATION_TYPE);
	    	$fieldRunIntervalseconds= $this->getDaemonINISectionParam(DAEMON_FIELD_RUN_INTERVAL_SECONDS);
	    	$fieldLastRunDateTime	= $this->getDaemonINISectionParam(DAEMON_FIELD_LAST_RUN_DATETIME);
        	$grouping_fields		= $this->getDaemonINISectionParam(DAEMON_GROUPING_FIELDS);
        	
        	$sql = $this->getDaemonRunSQL($dbRuntable);	
        	
        	$runRows = $this->_dbHandler->execQuery($sql, acs_idb::_SQL_GETALL);
        	
        	// now loop on all rows group them by RunID
        	// this is done as some overloaded SQL query may have joins that multiply the same process
        	$groupedProcessAr=array();
        	$groupingKeys = explode(',',$grouping_fields);
        	foreach($runRows as $runRow){
	        	$currKeysAr = array();
	        	reset($groupingKeys);
	        	foreach($groupingKeys as $groupingKey){
	        		if(!isset($runRow[$groupingKey])) 
	        			throw new acs_exBadParam("Invalid grouping field given: $groupingKey");
	        		$currKeysAr[]=$runRow[$groupingKey];
	        		
	        	}
	        	$currKey = join('-',$currKeysAr);

	        	$idRun = $runRow[$fieldIDRun];
	        	if(!is_numeric($idRun))
	        		throw new acs_exBadParam("Invalid ID Run field given: $idRun");
	        	
        		unset($runRow[$fieldIDRun]);
        		
        		$groupedProcessAr[$currKey][$fieldRunName]=$runRow[$fieldRunName];
        		unset($runRow[$fieldRunName]);
        		
        		$groupedProcessAr[$currKey][$fieldRunOperationType]=$runRow[$fieldRunOperationType];
        		unset($runRow[$fieldRunOperationType]);
        		
        		$groupedProcessAr[$currKey][$fieldRunIntervalseconds]=$runRow[$fieldRunIntervalseconds];
        		unset($runRow[$fieldRunIntervalseconds]);
        		
        		$groupedProcessAr[$currKey][$fieldLastRunDateTime]=$runRow[$fieldLastRunDateTime];
        		unset($runRow[$fieldLastRunDateTime]);
        		
        		$groupedProcessAr[$currKey]['extraData'][]=$runRow;
        	}
        	
        	// now loop on all rows and launch threads
        	foreach($groupedProcessAr as $idRun=>$runRow){
//        		print_r($runRow);
				try {
	        		$idRun = $runRow[$fieldIDRun];
	        		$runName = $runRow[$fieldRunName];
	        		$runIntervalseconds = $runRow[$fieldRunIntervalseconds];
	        		
	        		switch($runRow[$fieldRunOperationType]){
	        			case RUN_ENABLED:
	        			case RUN_SIMULATION:
	        				$this->createChildProcess($runClassName, $runRow, $runRow[$fieldRunName]);

 	        				break;
	        				
	        			case RUN_DISABLED:
	        				$this->_logHandler->warning("Process $runName (RunId = $idRun) is disabled");
	        				break;
	        				
	        			default:
	        				throw new acs_exBadParam("Unknown Run Type value ({$runRow[$fieldRunEnabled]})");
	        		}
	        	} catch(Exception $e){
	        		throw new Exception($e->getMessage()." (Run ID = $idRun)");
	        	}
	        	
        	}

        	$this->waitChildren();
        	
        	// This is not a good thing: the connection must to be re-established as the db handler is lost 
        	// when we create the process.....
        	// A deeper investigation is needed here in order to understand if and how a db connection
        	// pool can be implemented
        	$this->_dbHandler = null;
			$this->_dbHandler = acs_tools::getDBHandler($this->db_section, $this->_iniVars, $this->_logHandler);
        	
        } else {
			$this->run();	
        } 
    }
    

    public function run() {
        throw new acs_exCritical(__FUNCTION__ . " " . __LINE__ . " : run() method has not been overridden !");;
    }
    
    // sig_handler(int signo) {{{
    public function sig_handler($signo)
    {
        exit();
    }
    // }}}
    
    // microtime_float {{{
    public function microtime_float()
    {
        list($usec, $sec) = explode(" ", microtime());
        return ((float)$usec + (float)$sec);
    }
    // }}}
    
    // function shutdown(mixed $filename) {{{
    public function shutdown($filenameAr){
        if(!$this->_itsMe) return;
        foreach($filenameAr as $filename){
            @unlink($filename) ;
        }
    }
    
    protected function getDaemonRunSQL($dbRuntable){
    	return "SELECT * FROM $dbRuntable";
    }

}
?>
