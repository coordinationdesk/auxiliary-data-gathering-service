# DataRetriever
Configurable distributed product downloader

## Dependencies

- /usr/bin/php
- php-pdo
- php-pgsql
- php-pecl-gearman
- gearmand
- supervisor >= 4.0.0

## Spec file customization

	%define		project			TEST
project prefix for the RPM package
	
	%define		installname		%{project}DataRetriever

name of the main application directory

	%define		installpath		/opt/exprivia
	
base installation path
	
	%define		daemonuser		operator

system user to run the daemon processed 

	%define		configclass		Config

configuration class with the project customizations
	
	%define		downloadworkers 12
	
number of parallel download workers to be used


## Configuration

A default configuration is provided in {appdir}/config/Config.php. If wanted, an override of this configuration
can be used creating a class file placed into {appdir}/config/Config{project}.php and the Config{project} class
shall be written in the spec file modifying the "configClass" define

	%define		configclass		ConfigMyProject
	
Also the %install and %files sections shall be updated to add the new configuration class file
	
An example of a custom configuration file is reported below

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
	
	namespace Acs\PHPImport\Config;
	
	use Monolog\Logger;
	
	class ConfigMyProject extends Config {
		protected $memoryLimit = '2048M';
		
		protected $logLevel = Logger::INFO;
		protected $loopDelay = 30;
		protected $serverGroupName = 'X';
		protected $skipNullServerGroups = false;
		protected $phpImportIdentifier = 'data-retriever-{project}';
		protected $importWorkerName = 'discovery-worker-{project}';
		protected $downloadWorkerName = 'download-worker-{project}';
		protected $downloadWorkerServers = array('127.0.0.1');
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
	}
	
	?>

### Configuration details

	$memoryLimit = '2048M';

maximum amount of memory each process can allocate
	
	$logLevel = Logger::INFO;
log level

	$loopDelay = 30;
number of seconds each reception rule check shall be performed

	$serverGroupName = 'X';
"hostname" of the reception rule to take into account (t\_srv\_receptionrules hostname column)

	$phpImportIdentifier = 'data-retriever-{project}';
identifier of the main process identifier used to lock the reception rule (t\_srv\_receptionruleslock processidentifier column) 

	$importWorkerName = 'discovery-worker-{project}';
gearman task name for the discovery task (do not touch is not necessary)

	$downloadWorkerName = 'download-worker-{project}';
gearman task name for the download task (do not touch is not necessary)

	$downloadWorkerServers = array('127.0.0.1');
list of servers where the download tasks can be dispatched

	$maxParallelJobs = 10;
maximum number of reception rules to be handled at a time

	$noPlugin = 'DummyPlugin';
name of the NopImport plugin

	$deleteFromHistoryEnabled = false;
enable delete of old completed products from history table t\_srv\_receptionruleshist

	$deleteHistoryTimeWindowThresholdSec = 3600;
period of time (in seconds) to purge old completed files from history

	$pripRequestTop = 1000;
maximum number of products to be discovered at a time (pagination)

	$discoveryTimeThresholdMSec = 2000;
threshold (in milliseconds) to be subtracted from start time filter during products discovery
	
	$dbConfPath = '/opt/exprivia/dbconf/db.conf';
path of the external file with db configuration

	$dbConfHostIndex = 0;
number of the db configuration to be used

	$decrypterCmdFullPath = '/opt/exprivia/bin/Decrypter.sh';
db password decrypter command
	
	$ruleLockMaxTime = '2 hours';
maximum time for a reception rule lock to be released
	
	$maxDiscoveryQueuedTasks = 100000;
maximum number of queued task to be forwarded to gearman (0 = disabled)
	
	$tempDir = '/tmp';
temporary directory where downloads are stored until completion
	
	$tablesMapping = ('t_srv_tablename' => 't_newschema_tablename', ...)
mapping of tables names

	$repoRuntimeDataDirectory = '/var/tmp/';
directory used to store transient repository access information, like OAuth2 access tokens; a per-repository file will be created storing the OAuth2 token and expiration epoch for a later use for subsequent repository accesses; the file name is composed by {client_id}-{username}.phps

    $downloadHighPriorityProductNameRegex = '_DS_';
    $downloadNormalPriorityProductNameRegex = '_GR_';
    $downloadLowPriorityProductNameRegex = null;
set as a regular expression string to change the download priority of a product if its name matches one of the three regexes; in the example S2 datastrips take precedence over granules download

## DB configuration

A sample of db configuration is reported for reference

- t\_discoveryplugins

	id_discoveryplugin|name                    |
	------------------+------------------------+
	                 1|DataRetrieverPluginOData|
- t\_filetypes

	id_filetype|filetype     |description|nameregularexpression|headerseparation|group|invperiod|mngsngext|sngext|hdrext|dblext|
	-----------+-------------+-----------+---------------------+----------------+-----+---------+---------+------+------+------+
	          2|S1_HISTORICAL|           |^S1.*                |false           |group|         |true     |      |      |      |
	          3|S2_HISTORICAL|           |^S2.*                |false           |group|         |true     |      |      |      |
	          4|S1_LOCALTEST |           |^S1A_IW_RAW.*        |false           |group|         |true     |      |      |      |
          
- t\_filetypescomps (shall be filled only if virtual filetypes are present)

	id|pfiletype|cfiletype|
	--+---------+---------+
	
- t\_transferprotocols

	id|protocol|
	--+--------+
	 1|FTP     |
	 2|FTPS    |
	 3|SFTP    |
	 4|FILE    |
	 5|MEDIA   |
	 6|HTTP    |
	 7|HTTPS   |

- t\_knownhosts

	id|ip_address    |host_name                  |
	--+--------------+---------------------------+
	 1|146.59.136.138|databridge.gael-systems.com|
	 2|172.17.0.1    |172.17.0.1                 |
	 3|127.0.0.1     |localhost                  |
	 4|172.30.18.66  |172.30.18.66               |

- t\_srv\_statuses

	id|status            |
	--+------------------+
	 1|BOOKED            |
	 2|CANCELLED         |
	 3|COMPLETED         |
	 4|CREATED           |
	 5|DISTRIBUTION      |
	 6|ELIGIBLE          |
	 7|ERROR             |
	 8|IN_PROGRESS       |
	 9|LTA_DOWNLOADING   |
	10|TO_BE_CANCELLED   |
	11|RUNNING           |
	12|PAUSED            |
	13|ONLINE            |
	14|ONGOING           |
	15|EVICTED           |
	16|QUEUED            |
	17|UNRECOVERABLEERROR|
	18|RECOVERABLEERROR  |
	19|TRANSFERRING      |
	20|BLOCKED           |
	21|PROCESSING        |
	22|ABORTED           |
	23|NOTFOUNDERROR     |
	24|INEVICTION        |
	25|VALID             |
	26|IN_USE            |
	27|LOST              |

- t\_srv\_repositories

	id_repository|symbolicname|transferprotocol_id|host_id|username |password       |remotepath                 |tempname_id|tempname_value|allowedciphers|port|attributes                                                                                                                                                                                                         |
	-------------+------------+-------------------+-------+---------+---------------+---------------------------+-----------+--------------+--------------+----+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
	            1|historical  |                  7|      1|lta3     |********       |/gss-catalogue/Products    |          1|tmp           |              |    |                                                                                                                                                                                                                   |
	            2|fresh-oauth2|                  7|      2|lta_expr |***************|/odata/v1/Products         |          1|tmp           |              |9443|{ "authentication_type": "OAuth2", "client_id": "s1pdgs_client", "client_secret": "*******-c068-4e79-a690-368c1d8a5cae", "token_url": "https://172.17.0.1:9443/auth/realms/s1pdgs/protocol/openid-connect/token" }|
	            3|local-mockup|                  6|      2|lta      |12qwas         |/prip-php-mock.php/Products|          1|tmp           |              |1080|                                                                                                                                                                                                                   |
	            4|s2-prip-test|                  7|      4|pripuser1|12qwas         |/odata/v1/Products         |          1|tmp           |              | 443|                                                                                                                                                                                                                   |

- t\_srv\_receptionrules

	id_receptionrule|filetype_id|repository_id|priority|active|generateheader|reception_tt_id|tt_parameters|hostname|localpath              |connretries|pollingperiod_sec|retryperiod_sec|connfailureaddress|procfailureaddress|lastvisit          |lastgood           |retrycounter|discoveryplugin_id|successnotificationaddress|php_timeout_sec|timewindow_sec|
	----------------+-----------+-------------+--------+------+--------------+---------------+-------------+--------+-----------------------+-----------+-----------------+---------------+------------------+------------------+-------------------+-------------------+------------+------------------+--------------------------+---------------+--------------+
	               1|          2|            1|       1|false |false         |              1|             |S1H     |/var/tmp/S1H           |          1|              300|             30|                  |                  |2020-11-26 16:22:28|2020-11-26 16:22:28|           0|                 3|                          |               |          3600|
	               2|          3|            1|       1|false |false         |              1|             |S2H     |/var/tmp/S2H           |          1|              300|             30|                  |                  |2020-11-26 16:40:08|2020-11-26 16:40:08|           0|                 3|                          |               |          3600|
	               3|          2|            2|       1|false |false         |              1|             |X       |/var/tmp/S1            |          1|              300|             30|                  |                  |2020-11-26 16:40:08|2020-11-26 16:40:08|           0|                 2|                          |             91|          3600|
	               4|          2|            3|       1|true  |false         |              1|             |X       |/var/tmp/MOCK_IN_BASKET|          1|              300|             30|                  |                  |2021-08-05 13:19:38|2021-08-05 13:19:38|           0|                 2|                          |             91|          3600|
	               5|          3|            4|       1|false |false         |              1|             |X       |/data/CDPDataRetriever |          1|              300|             30|                  |                  |2021-08-05 14:05:23|2021-08-05 14:05:23|           0|                 2|                          |             91|          3600|

## Repository extended attributes

Each repository can be configured with an extended json object stored in the t\_srv\_repositories "attributes" column.

Some attributes are dedicated to the authentication type, others to the http and OData protocol details.

Follows an example of the json repository configuration object

	{
	  "authentication_type": "OAuth2",
	  "client_id": "<oauth2-client-id>",
	  "client_secret": "<oauth2-client-secret",
	  "token_url": "<oauth2-token-url>",
	  "oauth2_header": "Authorization: Bearer ",
	  "odata_filter": "PublicationDate gt 2021-12-05T00:00:00Z and PublicationDate lt 2021-12-06T00:00:00Z",
	  "omit_format": false,
	  "propagate_headers_redirect": true, 
	  "propagate_headers_redirect_download": true,
	  "curl_options": {                            
	    "CURLOPT_VERBOSE": 1,                    
	    "CURLOPT_PROXY": "proxy.project.int:1234"
	  }
	}

In details:

- authentication_type can be one of the following:
    * None
    * Basic
    * OAuth2, if used, the following properties are required:
        - client_id
        - client_secret
        - token_url
	oauth2_header is optional, its default value is "OAUTH2-ACCESS-TOKEN: ", but the OAuth2 standard is "Authorization: Bearer "
- omit_format
    * false: discovery and metadata download requests will always have the $format=application/json argument
    * true: no $format argument is used
- propagate\_headers\_redirect
    * true: main request headers are redirected to the locations specified by the redirected url
    * false: the subsequent redirected url call are performed with fresh headers, masquerading the original ones
- propagate\_headers\_redirect\_download, same as above, for the download process
- odata_filter every valid OData filter, will be added to the automatic pagination filter with the "and" operator
- curl_options is an object representing curl options to be applied to every curl instance used to discovery, get metadata and product for this repository (see https://www.php.net/manual/en/function.curl-setopt.php)

## Service configuration

- configure tasks services autostart

	systemctl enable gearmand
	systemctl enable supervisord

- configure main reception rules service (only on the main node!)

	systemctl enable {project}DataRetriever


- start workers

	systemctl start gearmand supervisord


- start reception rules service

	systemctl start {project}DataRetriever


- stop workers and reception rules service

	systemctl stop supervisord {project}DataRetriever


- stop all services 

*This empties the gearman queue if persistance is not configured!!*

	systemctl stop gearmand supervisord {project}DataRetriever


## Logs

- see logs from all DataRetriever processes

	journalctl -f | grep DataRetriever


- see splunk messages for DataRetriever products activity

	journalctl -f | grep IMP
