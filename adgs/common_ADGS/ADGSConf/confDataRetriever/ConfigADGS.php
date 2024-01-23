<?php
namespace Acs\PHPImport\Config;

use Monolog\Logger;


class ConfigADGS extends Config {  
	
	protected $memoryLimit = '2048M';
	protected $pluginsNamespace = 'Acs\PHPImport\Service\Plugin';
    
    protected $logName = 'php://stdout';
    protected $logLevel = Logger::INFO;
    protected $loopDelay = 30;
    protected $serverGroupName = 'ODATA-SERV';
    protected $skipNullServerGroups = true;
    protected $phpImportIdentifier = 'data-retriever-adgs';
    protected $importWorkerName = 'discovery-worker-adgs';
    protected $downloadWorkerName = 'download-worker-adgs';
    protected $downloadWorkerServers=array('127.0.0.1');
    protected $maxParallelJobs = 10;
    protected $timeWindowFilter = true;
    protected $noPlugin = 'nop_imp';
    protected $deleteHistoryTimeWindowThresholdSec = 3600;
    protected $pripRequestTop = 1000;
    protected $discoveryTimeThresholdMSec = 2000;
    protected $deleteFromHistoryEnabled = false;
    
    protected $dbConfPath = '/data/ADGS/conf/database/db.conf';
    protected $dbConfHostIndex = 0;
    protected $decrypterCmdFullPath = '/usr/local/components/ADGS/bin/Decrypter.sh';
    protected $ruleLockMaxTime = '2 hours';
    
    protected $maxDiscoveryQueuedTasks = 100000;

    protected $downloadMetadataFile = false;
	
	protected $updateDownloadedFileTimestamp = true;
    
    protected $tempDir = '/data/ADGS/so_notifier/tmp';
    
    protected $tablesMapping = array(
    	't_filetypes' => 't_filetypes',
    	't_filetypescomps' => 't_filetypescomps',
    	't_transferprotocols' => 't_transferprotocols',
    	't_knownhosts' => 't_knownhosts',
    	't_discoveryplugins' => 't_discoveryplugins',
    	't_srv_statuses' => 't_stati',
    	't_srv_repositories' => 't_repositories',
    	't_srv_receptionrules' => 't_receptionrules',
    	't_srv_receptionruleslock' => 't_receptionruleslock',
    	't_srv_receptionruleshist' => 't_receptionruleshist',
    );
}

?>
