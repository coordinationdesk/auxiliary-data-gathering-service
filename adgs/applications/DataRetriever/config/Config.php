<?php
namespace Acs\PHPImport\Config;

use Monolog\Logger;


class Config {  
	
	protected $loggerClass = 'Acs\PHPImport\Service\Log\PHPImportLoggerSyslog';
	
	protected $memoryLimit = '2048M';
	protected $pluginsNamespace = 'Acs\PHPImport\Service\Plugin';
    
    protected $logName = 'php://stdout';
    protected $logLevel = Logger::INFO;
    protected $loopDelay = 30;
    protected $serverGroupName = 'X';
    protected $skipNullServerGroups = false;
    protected $phpImportIdentifier = 'data-retriever-{project}';
    protected $importWorkerName = 'discovery-worker-{project}';
    protected $downloadWorkerName = 'download-worker-{project}';
    protected $downloadWorkerNameByRepo = array();
    protected $downloadWorkerServers=array('127.0.0.1');
    protected $discoveryWorkerServer = 'localhost';
    protected $discoveryWorkerDownloadServer = 'localhost';	// where a discovery server places a download task
    protected $maxParallelJobs = 10;
    protected $noPlugin = 'DummyPlugin';
    protected $deleteHistoryTimeWindowThresholdSec = 3600;
    protected $pripRequestTop = 1000;
    protected $discoveryTimeThresholdMSec = 2000;
    protected $deleteFromHistoryEnabled = false;
    
    protected $dbConfPath = '/opt/exprivia/dbconf/db.conf';
    protected $dbConfHostIndex = 0;
    protected $decrypterCmdFullPath = '/opt/exprivia/bin/Decrypter.sh';
    protected $ruleLockMaxTime = '2 hours';
    
    protected $maxDiscoveryQueuedTasks = 100000;
    
    protected $downloadMetadataFile = true;
    
    protected $updateDownloadedFileTimestamp = true;
    
    protected $tempDir = '/tmp';
    
    protected $tablesMapping = array(
    	't_filetypes' => 't_filetypes',
    	't_filetypescomps' => 't_filetypescomps',
    	't_transferprotocols' => 't_transferprotocols',
    	't_knownhosts' => 't_knownhosts',
    	't_discoveryplugins' => 't_discoveryplugins',
    	't_srv_statuses' => 't_srv_statuses',
    	't_srv_repositories' => 't_srv_repositories',
    	't_srv_receptionrules' => 't_srv_receptionrules',
    	't_srv_receptionruleslock' => 't_srv_receptionruleslock',
    	't_srv_receptionruleshist' => 't_srv_receptionruleshist',
    );
    
    protected $repoRuntimeDataDirectory = '/var/tmp/';
    
    protected $downloadHighPriorityProductNameRegex = null;
    protected $downloadNormalPriorityProductNameRegex = null;
    protected $downloadLowPriorityProductNameRegex = null;

	/**
     * Returns the configuration singleton
     * @return Config
     */
	static public function get() {
    	date_default_timezone_set('UTC');
    	
    	$configName = getenv('PHP_IMPORT_CONFIGURATION');
    	if (!$configName) {
    		$configName = 'Config';
    	}
    	if (file_exists($configName)) {
    		require_once $configName;
    		$configClass = pathinfo($configName, PATHINFO_BASENAME);	
    	} else {
    		$configClass = 'Acs\PHPImport\Config\\' . $configName;
    	}
    	return new $configClass;
    }

	/**
     * Parses db.conf to get DB connection parameters AND  
     * calls the decrypter to decrypt the DB PWD
     *
     * @param string containing the path of the db.conf
     * @return array containing the DB connection parameters
     */
    public function parseDbConf($dbConfPath) {
        
        //parse db.conf  
        $lines = file($dbConfPath, FILE_IGNORE_NEW_LINES);
        if ($lines === false)
        	throw new \RuntimeException("Cannot open {$dbConfPath} for reading");
        
        $parsedDbConf=array('dbType'=>'pgsql');
        
        $ini = array();
        foreach($lines as $confline){
            if (empty($confline) || $confline[0] === '#') continue;
            list($key, $value) = explode(':', $confline);
            $ini[$key] = trim($value);
        }
        
        if (!array_key_exists('Postgres.PreferredHostIndex', $ini) === null)
        	throw new \RuntimeException("Cannot find postgres host index in db conf file");
        $dbIdx = $ini['Postgres.PreferredHostIndex'];
        $dbHostSection = "Postgres.host[$dbIdx].dbConnectionArbiterCParam";
        
        $parsedDbConf['dbPort'] = @$ini[$dbHostSection . '.port'];
        $parsedDbConf['dbHost'] = @$ini[$dbHostSection . '.hostname'];
        $parsedDbConf['dbName'] = @$ini[$dbHostSection . '.db'];
        $parsedDbConf['dbUser'] = @$ini[$dbHostSection . '.user'];
        $parsedDbConf['dbPasswordAlgo'] = @$ini[$dbHostSection . '.passwordEncryptionAlgo'];
        $parsedDbConf['dbPasswordSalt'] = @$parsedDbConf['dbPasswordAlgo'] == 259;
        $parsedDbConf['dbPassword'] = @$ini[$dbHostSection . '.password'];
        
        if (array_key_exists($dbHostSection . '.salt_random', $ini)) {
        	$dbPasswordSaltParams = array('-s', escapeshellarg($ini[$dbHostSection . '.salt_random']));
        }
        if (array_key_exists($dbHostSection . '.salt_string', $ini)) {
        	$dbPasswordSaltParams = array('-c', escapeshellarg($ini[$dbHostSection . '.salt_string']));
        }
        $dbPasswordParams = array('-b ', escapeshellarg($parsedDbConf['dbPassword']));
        
        // check if the DB pwd must be decrypted and how
        $decrypterCmd = array($this->getDecrypterCmdFullPath());
        
        switch ($parsedDbConf['dbPasswordAlgo']) {
        	case 256:
        	case 257:
        		self::arrayAppend($decrypterCmd, $dbPasswordParams);
        		break;
        	case 258:
        	case 259:
        		if (empty($dbPasswordSaltParams)) {
        			throw new \RuntimeException("Cannot read the salt parameter to call the Decrypter.");
        		}
        		self::arrayAppend($decrypterCmd, $dbPasswordSaltParams);
        		self::arrayAppend($decrypterCmd, $dbPasswordParams);
        		break;
        	default:
        		return $parsedDbConf;
        }

        // Decrypt pwd
        $return_var = 0;
        $output = array();
        
        $cmdStr = join(' ', $decrypterCmd);
        exec($cmdStr, $output , $return_var);
        
        if ($return_var != 0) {
            throw new \RuntimeException("$cmdStr exited with $return_var and output: " . print_r($output, true));
        }elseif(empty($output)){
            throw new \RuntimeException("$cmdStr produced no output");
        }
        $parsedDbConf['dbPassword'] = $output[0];
        
        return $parsedDbConf;
    }
    
    protected static function arrayAppend(& $array, $otherArray) {
    	$array = array_merge($array, $otherArray);
    }

    /**
     * @return string
     */
    public function getdbConfPDODsn() {
        $parsedDbConf = $this->parseDbConf($this->getDbConfPath());
        
        return
        sprintf("%s:host=%s;port=%d;dbname=%s;user=%s;password=%s",
            $parsedDbConf['dbType'],
            $parsedDbConf['dbHost'],
            $parsedDbConf['dbPort'],
            $parsedDbConf['dbName'],
            $parsedDbConf['dbUser'],
            $parsedDbConf['dbPassword']
            );
    }
    
    /**
     * @return string
     */
    public function getLogName()
    {
        return $this->logName;
    }
    
    /**
     * @return number
     */
    public function getLogLevel()
    {
        return $this->logLevel;
    }
       
    /**
     * @return number
     */
    public function getLoopDelay()
    {
        return $this->loopDelay;
    }
    
    /**
     * @return string
     */
    public function getServerGroupName()
    {
        return $this->serverGroupName;
    }
    
    /**
     * @return string
     */
    public function getPhpImportIdentifier()
    {
        return $this->phpImportIdentifier;
    }

    /**
     * @return string
     */
    public function getImportWorkerName()
    {
        return $this->importWorkerName;
    }
    
    /**
     * @return string
     */
    public function getDownloadWorkerName()
    {
        return $this->downloadWorkerName;
    }
    
    /**
     * @return array
     */
    public function getDownloadWorkerServers(){
        return $this->downloadWorkerServers;
    }

    
    /**
     * @return string
     */
    public function getMaxParallelJobs()
    {
        return $this->maxParallelJobs;
    }

    /**
     * @return string
     */
    public function getNoPlugin()
    {
        return $this->noPlugin;
    }
    
    /**
     * @return string
     */
    public function getDbConfPath()
    {
        return $this->dbConfPath;
    }
    
    /**
     * @return number
     */
    public function getDbConfHostIndex()
    {
        return $this->dbConfHostIndex;
    }

    /**
     * @return string
     */
    public function getDecrypterCmdFullPath()
    {
        return $this->decrypterCmdFullPath;
    }

    public function getRuleLockMaxTime()
    {
        return $this->ruleLockMaxTime;
    }
  
    /**
     * @return number
     */
    public function getDeleteHistoryTimeWindowThresholdSec()
    {
    	return $this->deleteHistoryTimeWindowThresholdSec;
    }
    
    /**
     * @return number
     */
    public function getPripRequestTop()
    {
    	return $this->pripRequestTop;
    }

    /**
     * @return number
     */
    public function getDiscoveryTimeThresholdMSec()
    {
    	return $this->discoveryTimeThresholdMSec;
    }

    /**
     * @return bool
     */
    public function isDeleteFromHistoryEnabled()
    {
    	return $this->deleteFromHistoryEnabled;
    }
    
    /**
     * @return number
     */
    public function getMaxDiscoveryQueuedTasks()
    {
    	return $this->maxDiscoveryQueuedTasks;
    }
    
    /**
     * @return string
     */
    public function getMemoryLimit()
    {
    	return $this->memoryLimit;
    }
    
    /**
     * @return string
     */
    public function getPluginsNamespace()
    {
    	return $this->pluginsNamespace;
    }
    
    /**
     * @return string
     */
    public function getTempDir()
    {
    	return $this->tempDir;
    }
    
    /**
     * @return multitype:string
     */
    public function getTablesMapping()
    {
    	return $this->tablesMapping;
    }
    
    /**
     * @return boolean
     */
    public function getSkipNullServerGroups()
    {
    	return $this->skipNullServerGroups;
    }

    /**
	 * @return boolean
	 */
	public function getDownloadMetadataFile()
	{
		return $this->downloadMetadataFile;
	}
	
	/**
	 * @return boolean
	 */
	public function getUpdateDownloadedFileTimestamp()
	{
		return $this->updateDownloadedFileTimestamp;
	}
	
	
	/**
	 * @return string
	 */
	public function getDiscoveryWorkerServer()
	{
		return $this->discoveryWorkerServer;
	}
	
	public function getDiscoveryWorkerDownloadServer() {
		return $this->discoveryWorkerDownloadServer;
	}
	
	/**
	 * @return string
	 */
	public function getLoggerClass()
	{
		return $this->loggerClass;
	}
	
	/**
	 * @return array
	 */
	public function getDownloadWorkerNameByRepo()
	{
		return $this->downloadWorkerNameByRepo;
	}

	/**
	 * @return string
	 */
	public function getRepoRuntimeDataDirectory() {
		return $this->repoRuntimeDataDirectory;
	}

	/**
	 * @return mixed
	 */
	public function getDownloadHighPriorityProductNameRegex() {
		return $this->downloadHighPriorityProductNameRegex;
	}

	/**
	 * @return mixed
	 */
	public function getDownloadNormalPriorityProductNameRegex() {
		return $this->downloadNormalPriorityProductNameRegex;
	}

	/**
	 * @return mixed
	 */
	public function getDownloadLowPriorityProductNameRegex() {
		return $this->downloadLowPriorityProductNameRegex;
	}
}

?>