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
// $Author: andema $
// $Id: acs_app.php,v 1.54 2016/02/12 13:48:02 andema Exp $
// $Revision: 1.54 $
//

// ACS PHP Lib includes
require_once 'ACSPhpLib/acs_file.php';
require_once 'ACSPhpLib/acs_log.php';
require_once 'ACSPhpLib/acs_tools.php';
require_once 'ACSPhpLib/acs_exception.php';

/**
 * @package ACSPhpLib
 * @category Utilities
 */
class acs_app {

	/**
	 * Initialization variables
	 * @var array
	 */
    protected $_iniVars = null;
    /**
     * Log handler
     * @var acs_log
     */
	protected $_logHandler = null;
	/**
	 * Database connection
	 * @var acs_mdb2
	 */
	protected $_dbHandler = null;
	protected $_iniFileName = null;
    protected $_myClassName = null;
    protected $EMAIL_REPORT_TO_ADMINS = false;
    protected $threadsJoinPoint = null;

	const _RULER = "*******************************************************************************\n";


	public function __construct($iniFileName,$ident,$isWebInterface=false,$db_sections_list='db',$initype='Inifile',$inioptions=array(),$root=null) {
        try {
            // get the INI File Handler
            $this->_iniFileName = $iniFileName;
            $iniDbgFileName = dirname($iniFileName).'/dbg_'.basename($iniFileName);

    		$this->_iniVars = acs_file :: getINIFile($this->_iniFileName,$initype,$inioptions,$root);
            $hasDbgIni=file_exists($iniDbgFileName);
            if($hasDbgIni) {
                $dbgIniAr=acs_file :: getINIFile($iniDbgFileName);
                $this->_iniVars = acs_tools::array_overlay($this->_iniVars,$dbgIniAr);
            }

			$inidir = dirname($iniFileName);
            // Check for host Based INI file
            if(is_array($_SERVER) && array_key_exists('SERVER_NAME',$_SERVER)){
                $hostIniFileName=$inidir.'/vhosts/'.$_SERVER['SERVER_NAME'].'.ini';
                if(file_exists($hostIniFileName)) {
                    $hostIniAr=acs_file :: getINIFile($hostIniFileName);
                    $this->_iniVars = acs_tools::array_overlay($this->_iniVars,$hostIniAr);
                }
            }

            // Check for ini.d configuration directory
            if(!(array_key_exists('ignore_ini_d_files',$this->_iniVars['application']) && $this->_iniVars['application']['ignore_ini_d_files'])){
	            $inidpath = $inidir . "/ini.d/";
	            if (is_dir($inidpath))
	            {
		            $inid = acs_file::getDirList($inidpath);
		            sort($inid);
		            foreach ($inid as $ininame)
		            {
		                $iniAr=acs_file :: getINIFile($inidpath . $ininame);
		                $this->_iniVars = acs_tools::array_overlay($this->_iniVars,$iniAr);
		            }
	            }
            }

            if (!array_key_exists('temporary_dir', $this->_iniVars['application'])) {
	            // Set a hidden configuration value to store the temporary directory, if defined
	            $this->_iniVars['application']['temporary_dir'] = '<GLOBAL_TEMPORARY_DIR>';
            }
            
            // Substitute tokens from global config
            $this->_iniVars = acs_file :: resolveTokens($this->_iniVars);
            
            // Apply applications-shared configuration
			$this->applySharedConfigurations();
			
			// Set the static value of the temporary directory
			acs_file::getTempDir($this->_iniVars['application']['temporary_dir']);
            	
            // set time zone
            if(!array_key_exists('application',$this->_iniVars) or (!array_key_exists('timezone',$this->_iniVars['application']))) {
                $this->_iniVars['application']['timezone']="UTC";
            }
            date_default_timezone_set($this->_iniVars['application']['timezone']);

    		// get the Log Handler
            if(array_key_exists('webdebug',$this->_iniVars['log'])) $isWebInterface=!$this->_iniVars['log']['webdebug'];

            $this->_myClassName = get_class($this);
            if(array_key_exists($this->_myClassName,$this->_iniVars) and array_key_exists('webdebug',$this->_iniVars[$this->_myClassName])) $isWebInterface=!$this->_iniVars[$this->_myClassName]['webdebug'];
            $logObj = new acs_log($this->_iniVars,$ident,$isWebInterface );
    		$this->_logHandler = $logObj->getLoghandler();

            if($this->_iniVars['log']['profiler']){
            	if(extension_loaded('apd')){
            	   apd_set_pprof_trace();
                   $this->_logHandler->err("Advanced PHP Profiler is started !");
            	} else
                    $this->_logHandler->err("Advanced PHP Profiler not installed !");
            }

			$this->_logHandler->debug($this->_myClassName . ":" . __METHOD__ . " Running");
            if($hasDbgIni)
                $this->_logHandler->info(__METHOD__ . ":" . __LINE__ . " Using Debug Ini");

			$dbSectionsAr=explode('#',$db_sections_list);
            foreach($dbSectionsAr as $db_section){
                // get the DB Handler, if necessary and section exists... useful for ARACME
                if(!array_key_exists($db_section, $this->_iniVars)) continue;
                $this->_dbHandler = acs_tools::getDBHandler($db_section,$this->_iniVars,$this->_logHandler);
                break;
            }
            if(array_key_exists('email_report_to_admins',$this->_iniVars['log'])){
                $this->EMAIL_REPORT_TO_ADMINS = $this->_iniVars['log']['email_report_to_admins'];
                if (!$this->EMAIL_REPORT_TO_ADMINS)
                    $this->_logHandler->info("Report will not be sent to administrators !");
            }

        } catch (acs_exCritical $e) {
            $this->handleSevereError($e->getMessage(),true);
		} catch (Exception $e) {

			$msg = "Application terminated abnormally : " . $e->getMessage();
			if (!empty ($this->_logHandler))
				$this->_logHandler->crit($msg, true);
			// rethrow exception
			throw $e;
		}
	}

    function sendMailToAdmins($subject, $msg)
    {
        if (is_array($this->_iniVars['admin_emails'])) {
            foreach ($this->_iniVars['admin_emails'] as $address => $isOn) {
                if ($isOn) {
                    if (mail($address, $subject, $msg, "From: admin@{$_SERVER['SERVER_NAME']}\r\nX-Mailer: PHP/" . phpversion() )) {
                        $this->_logHandler->notice($address . " has been notified.");
                    } else {
                        $this->_logHandler->err(__METHOD__ . ":" . __LINE__ . " " . $address . " could not be notified.");
                    }
                }
            }
        } else {
            $this->_logHandler->err(__METHOD__ . ":" . __LINE__ . " " . "No email address found in the ADMIN_EMAILS section in toc.ini!");
        }
    }

	/**
	 * @SuppressWarnings(PHPMD.ExitExpression)
	 */
	function handleSevereError($msg) {
		if ($this->_iniVars['application']['severe_error_exception']){
			if (!empty ($this->_logHandler))
				$this->_logHandler->crit(__METHOD__ . ":" . __LINE__ . " " .$msg, true);
			throw new Exception($msg);
		}
		$msg = "Application terminated abnormally : " . $msg;
		if (!empty ($this->_logHandler))
			$this->_logHandler->crit($msg, true);
		else {
			$msg = self :: _RULER . "\n" . $msg . "\n" . self :: _RULER;
			echo $msg;
		}
		if ($this->EMAIL_REPORT_TO_ADMINS) {
            $this->sendMailToAdmins("Severe ERROR on Application", $msg);
		} else {
			$new_msg = "Email notification is disabled !";
			if (!empty ($this->_logHandler))
				$this->_logHandler->warning($new_msg);
		}
		die("\n$msg"); // terminate application
	}

    public function getSectionConfiguration($sectionname=''){
        // get class specific parameters, if any
        if(empty($sectionname))$sectionname=get_class($this);
        if(is_array($this->_iniVars) && array_key_exists($sectionname,$this->_iniVars)){
            $this->_logHandler->debug(__METHOD__ . ": found specific configuration for $sectionname");
            return $this->_iniVars[$sectionname];
        } else {
            $this->_logHandler->notice(__METHOD__ . ": no specific configuration found for $sectionname");
            return array();
        }
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////
    /////////    M U L T I T H R E A D   S U P P O R T
    /////////
    /////////////////////////////////////////////////////////////////////////////////////////////////

	protected function createChildProcess($runClassName, $runRow, $runName){
		$this->_logHandler->info("Creating new process for class $runClassName");
		$this->_logHandler->debug("New process payload is:\n".var_export($runRow,true));
		
		require_once "ACSPhpLib/extras/threadi/Loader.php";
		$thread = Threadi_ThreadFactory::getReturnableThread(array('acs_app_thread_worker', 'init'));
		$thread->start($runClassName, $this->_logHandler, $this->_iniVars, $runRow, $runName); 
		
		if(!isset($this->threadsJoinPoint)) $this->threadsJoinPoint = new Threadi_JoinPoint();
		$this->threadsJoinPoint->add($thread);
	}
	
	protected function waitChildren(){
		if(isset($this->threadsJoinPoint)) $this->threadsJoinPoint->waitTillReady();	
	}


    /////////////////////////////////////////////////////////////////////////////////////////////////
    /////////
    /////////    G E T T E R S   &    S E T T E R S
    /////////
    /////////////////////////////////////////////////////////////////////////////////////////////////
    public function & getLogHandler() {
        return $this->_logHandler;
    }
    public function & getDBHandler() {
        return $this->_dbHandler;
    }
    public function & getIniVars() {
        return $this->_iniVars;
    }
    public function getIniFileName() {
    	return $this->_iniFileName;
    }
    
    ////////////////////////////////////////////////////////
    //////	S H A R E D   C O N F I G U R A T I O N   //////
    ////////////////////////////////////////////////////////
    public function applySharedConfigurations($configPaths = null) {
    	$configPaths = $this->getSharedConfigurationPaths($configPaths);
    	if (!is_null($configPaths)) {
    		foreach ($configPaths as $configPath) {
    			if (!file_exists($configPath))
    				throw new acs_exFileError("Cannot find shared configuration file: {$configPath}");
   				$this->_iniVars = acs_file::resolveTokens($this->_iniVars, $configPath);
    		}
    	}
    }
    
    public function getSharedConfigurations($configPaths = null) {
    	$configPaths = $this->getSharedConfigurationPaths($configPaths);
    	if (is_null($configPaths))
    		return array();
    	$conf = array();
    	foreach ($configPaths as $configPath)
    		$conf = acs_tools::array_overlay($conf, acs_file::getGlobalConfig('tokens', $configPath));
    	return $conf;
    }
    
    public function getDefaultSharedConfigurationPaths() {
    	if (array_key_exists('application_shared_configuration',$this->_iniVars)) {
	    	return array_values($this->_iniVars['application_shared_configuration']);
    	} else {
    		return null;
    	}
    }
    
    protected function getSharedConfigurationPaths($configPath) {
    	if (is_null($configPath))
    		$configPath = $this->getDefaultSharedConfigurationPaths();
    	return $configPath;
    }
}

class acs_app_thread_worker {
	static public function init($classname, $logHandler, $iniVars, $payload, $runName) {
		$obj = new $classname($logHandler, $iniVars, $runName);
		return $obj->run($payload);
	}
}

class acs_child_log {
	protected $_logHandler = null;
	protected $runName = 'unknown';

	public function __construct($logHandler, $runName) {
		$this->_logHandler	= $logHandler;
		$this->runName		= $runName;
	}
	
	protected function getChildLogMsg($msg){
		return "[thread: {$this->runName}] - ".$msg;
	}
	
	public function crit($msg){ 	$this->_logHandler->crit($this->getChildLogMsg($msg)); }
	public function err($msg){ 		$this->_logHandler->err($this->getChildLogMsg($msg)); }
	public function warning($msg){ 	$this->_logHandler->warning($this->getChildLogMsg($msg)); }
	public function info($msg){ 	$this->_logHandler->info($this->getChildLogMsg($msg)); }
	public function notice($msg){ 	$this->_logHandler->notice($this->getChildLogMsg($msg)); }
	public function debug($msg){ 	$this->_logHandler->debug($this->getChildLogMsg($msg)); }
}

class acs_child_app {

	protected $_logHandler = null;
	protected $_iniVars = null;
	protected $_dbHandlersAr = array();
	protected $_dbHandler = null;


	public function __construct($logHandler, $iniVars, $runId) {
		$this->_logHandler = new acs_child_log($logHandler, $runId);
		$this->_iniVars=$iniVars;
	}
	
	protected function getDBHandler($db_section='db'){
		if(array_key_exists($db_section,$this->_dbHandlersAr)) return $this->_dbHandlersAr[$db_section];

        // get the DB Handler, if section exists
        if(!array_key_exists($db_section, $this->_iniVars)) throw new acs_exINIError("Invalid DB Section => $db_section");
        $this->_dbHandlersAr[$db_section] = acs_tools::getDBHandler($db_section,$this->_iniVars,$this->_logHandler);
        if($db_section=='db') $this->_dbHandler = $this->_dbHandlersAr[$db_section];
		
        return $this->_dbHandlersAr[$db_section];
	}
	
	public function run($payload){
		// to override
	}	
	
	public function thread_test($payload){
		$testAr = array('test1'=>'@','test2'=>'#','test3'=>'$','test4'=>'%');

		for($i=10; $i<100; $i++){
			$this->_logHandler->err("$i ".str_repeat($testAr[$payload['run_name']],100));
			usleep(10);
		}
		$msg='some message...';
		$this->_logHandler->crit($msg);
		$this->_logHandler->err($msg); 
		$this->_logHandler->warning($msg); 
		$this->_logHandler->info($msg); 
		$this->_logHandler->notice($msg); 
		$this->_logHandler->debug($msg); 
		echo 'fine processo '.$payload['run_name']."\n";
	}	
}
?>