#!/usr/bin/php
<?php
// --------------------------------------------------------------------------
// Copyright 1995-2021, Exprivia SpA - DADF
// Via Della Bufalotta, 378 - 00139 Roma - Italy
// http://www.exprivia.com
// 
// All Rights Reserved.
// 
// --------------------------------------------------------------------------
// This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
// the contents of this file may not be disclosed to third parties, copied or
// duplicated in any form, in whole or in part, without the prior written
// permission of Exprivia, SpA
// --------------------------------------------------------------------------

require_once(__DIR__.'/../vendor/autoload.php');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Log\PHPImportLogger;
use Acs\PHPImport\Service\DB\PHPImportDB;
use Monolog\Logger;

class Repository {
	public $symbolicName;
	public $transferProcotol;
	public $host;
	public $username;
	public $password;
	public $remotePath;
	public $port;
	public $attributes;
	
	public function __construct($symbolicName, $transferProtocol, $host, $username, $password, $remotePath, $port, $attributes) {
		$this->symbolicName = $symbolicName;
		$this->transferProcotol = $transferProtocol;
		$this->host = $host;
		$this->username = $username;
		$this->password = $password;
		$this->remotePath = $remotePath;
		$this->port = $port;
		$this->attributes = $attributes;
	}
}

class ReceptionRule {
	public $fileType;
	public $repository;
	public $active;
	public $hostname;
	public $localPath;
	public $pollingPeriod;
	public $retryPeriod;
	public $phpTimeoutSec;
	public $timeWindowSec;
	
	public function __construct($fileType, $repository, $active, $hostname, $localPath, $pollingPeriod, $retryPeriod, $phpTimeoutSec, $timeWindowSec) {
		$this->fileType = $fileType;
		$this->repository = $repository;
		$this->active = $active;
		$this->hostname = $hostname;
		$this->localPath = $localPath;
		$this->pollingPeriod = $pollingPeriod;
		$this->retryPeriod = $retryPeriod;
		$this->phpTimeoutSec = $phpTimeoutSec;
		$this->timeWindowSec = $timeWindowSec;
	}
}

class ReceptionRulesConfigurator {
	protected $iniFile, $outputBasePath;
	/**
	 * @var Config
	 */
	protected $config;
	/**
	 * @var Logger
	 */
	protected $logger;
	/**
	 * @var PDO
	 */
	protected $db;
	/**
	 * @var array
	 */
	protected $ini;
	/**
	 * @var array
	 */
	protected $knownHosts;
	/**
	 * @var array
	 */
	protected $fileTypes;
	/**
	 * @var array
	 */
	protected $outputPaths;
	/**
	 * @var array[Repository]
	 */
	protected $repositories;
	/**
	 * @var array[ReceptionRule]
	 */
	protected $receptionRules;
	
	/**
	 * @var boolean
	 */
	protected $createLocalOutputPaths;
	
	protected $jsonAttributesMapping = array(
		'oauth2_clientid' => 'client_id',
		'oauth2_clientsecret' => 'client_secret',
		'oauth2_tokenurl' => 'token_url',
		'oauth2_header' => 'oauth2_header',
		'odata_condition' => 'odata_filter',
		'odata_omit_format' => 'omit_format',
		'odata_curl_options' => 'curl_options',
		'odata_propagate_headers_redirect' => 'propagate_headers_redirect',
		'odata_propagate_headers_redirect_download' => 'propagate_headers_redirect_download',
		'auth' => 'authentication_type'
	);
	
	public function __construct(Config $config, $iniFile, $outputBasePath, $createLocalOutputPaths) {
		$this->config = $config;
		$this->iniFile = $iniFile;
		$this->outputBasePath = $outputBasePath;
		$this->createLocalOutputPaths = $createLocalOutputPaths;
		
		$this->logger = PHPImportLogger::get();
		$this->db = PHPImportDB::get();
		$this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	}
	
	public function configure() {
		$this->readConfiguration();
		
		try {
			$this->db->beginTransaction();
			
			$this->logger->info("cleaning up unused db configuration");
			$this->cleanUpUnusedConfiguration();
			
			$this->logger->info("configuring known hosts");
			$this->configureKnownHosts();
			
			$this->logger->info("configuring file types");
			$this->configureFileTypes();
			
			$this->logger->info("configuring repositories");
			$this->configureRepositories();
			
			$this->logger->info("configuring reception rules");
			$this->configureReceptionRules();
			
			$this->db->commit();
			
			if ($this->createLocalOutputPaths) {
				$this->logger->info("configuring output paths");
				$this->configureOutputPaths();
			}
			
		} catch (PDOException $e) {
			$this->logger->err("Error configuring reception rules: " . $e->getMessage());
			$this->db->rollBack();
		}
	}
	
	/*
	 * Read configuration methods
	 */
	
	protected function getJsonAttributesMapping() {
		return $this->jsonAttributesMapping;
	}
	
	protected function getJsonAttributes($section) {
		$jm = $this->getJsonAttributesMapping();
		
		$o = new stdClass();
		foreach ($jm as $keyName => $jsonAttr) {
			if (array_key_exists($keyName, $section)) {
				$value = $section[$keyName];
				
				if (is_array($value)) $value = (object)$value;
				
				$o->$jsonAttr = $value;
			}
		}
		
		if (count(get_object_vars($o)) > 0) {
			return $o;
		} else {
			return null;
		}
	}
	
	protected function readConfiguration() {
		$this->ini = @parse_ini_file($this->iniFile, true);
		if ($this->ini === false) {
			throw new Exception("Cannot parse ini file {$this->iniFile}");
		}
		
		foreach ($this->ini as $name => $section) {
			/*
			 * knownhosts
			 */
			if (!array_key_exists('odata_hostname', $section)) {
				throw new Exception("Missing odata_hostname from {$name} section");
			}
			$this->knownHosts[] = $section['odata_hostname'];
			
			/*
			 * filetypes
			 */
			if (!array_key_exists('filetype_regex', $section)) {
				throw new Exception("Missing filetype_regex from {$name} section");
			}
			$this->fileTypes[] = $section['filetype_regex'];
			
			/*
			 * outputpaths
			 */
			if (!array_key_exists('relative_output_folder', $section)) {
				throw new Exception("Missing relative_output_folder from {$name} section");
			}
			$this->outputPaths[] = $section['relative_output_folder'];
			
			/*
			 * repositories
			 */
			if (!array_key_exists('name', $section) ||
				!array_key_exists('protocol', $section) ||
				!array_key_exists('odata_hostname', $section) ||
				!array_key_exists('odata_path', $section) ||
				!array_key_exists('odata_port', $section)) {
				throw new Exception("missing required field 'name', 'protocol', 'odata_hostname', 'odata_path' or 'odata_port' from {$name} section");
			}
			$attributes = $this->getJsonAttributes($section);
			
			$this->repositories[] = new Repository(
				$section['name'], 
				$section['protocol'], 
				$section['odata_hostname'], 
				@$section['username'], 
				@$section['password'], 
				$section['odata_path'],
				$section['odata_port'], 
				$attributes);
			
			/*
			 * reception rules
			 */
			if (!array_key_exists('polling_period', $section) ||
				!array_key_exists('retry_period', $section) ||
				!array_key_exists('php_timeout', $section)) {
				throw new Exception("missing required field 'polling_period', 'retry_period' or 'php_timeout' from {$name} section");
			}
			$this->receptionRules[] = new ReceptionRule(
				$section['filetype_regex'], 
				$section['name'], 
				true,
				@$section['group_hostname'],
				$section['relative_output_folder'], 
				$section['polling_period'], 
				$section['retry_period'], 
				$section['php_timeout'], 
				null);
		}
		
		$this->knownHosts = array_unique($this->knownHosts);
		$this->fileTypes = array_unique($this->fileTypes);
	}
	
	/*
	 * DB update methods
	 */
	
	protected function cleanUpUnusedConfiguration() {
		$tm = $this->config->getTablesMapping();
		$t_srv_repositories = $tm['t_srv_repositories'];
		$t_srv_receptionrules = $tm['t_srv_receptionrules'];
		$t_srv_receptionruleshist = $tm['t_srv_receptionruleshist'];
		$t_filetypes = $tm['t_filetypes'];
		$t_knownhosts = $tm['t_knownhosts'];
		
		$ret = $this->db->exec("DELETE FROM $t_srv_receptionrules WHERE id_receptionrule NOT IN (SELECT receptionrule_id FROM $t_srv_receptionruleshist)");
		$this->logger->notice("removed {$ret} receptionrules");
		
		$ret = $this->db->exec("DELETE FROM $t_srv_repositories WHERE id_repository NOT IN (SELECT repository_id FROM $t_srv_receptionrules)");
		$this->logger->notice("removed {$ret} repositories");
		
		$ret = $this->db->exec("DELETE FROM $t_filetypes WHERE id_filetype NOT IN (SELECT filetype_id FROM $t_srv_receptionrules)");
		$this->logger->notice("removed {$ret} filetypes");
		
		$ret = $this->db->exec("DELETE FROM $t_knownhosts WHERE id NOT IN (SELECT host_id FROM $t_srv_repositories)");
		$this->logger->notice("removed {$ret} knownhosts");
		
		$ret = $this->db->exec("UPDATE $t_srv_receptionrules SET active='f'");
		$this->logger->notice("deactivated {$ret} receptionrules");
	}
	
	protected function configureKnownHosts() {
		$tm = $this->config->getTablesMapping();
		$t_knownhosts = $tm['t_knownhosts'];
		
		$sel = $this->db->prepare("SELECT count(*) cnt FROM $t_knownhosts WHERE host_name = :host");
		$ins = $this->db->prepare("INSERT INTO $t_knownhosts (host_name) VALUES (:host)");
		
		foreach ($this->knownHosts as $host) {
			$sel->bindValue(':host', $host);
			if (!$sel->execute()) {
				throw new Exception("cannot fetch host {$host} count from $t_knownhosts");
			}
			$r = $sel->fetch(PDO::FETCH_ASSOC);
			if ($r['cnt'] > 0) {
				$this->logger->info("no need to add host {$host}");
			} else {
				$ins->bindValue(':host', $host);
				if (!$ins->execute()) {
					throw new Exception("cannot insert {$host} into $t_knownhosts");
				}
				$this->logger->info("added host {$host}");
			}
		}
	}
	
	protected function configureFileTypes() {
		$tm = $this->config->getTablesMapping();
		$t_filetypes = $tm['t_filetypes'];
		
		$sel = $this->db->prepare("SELECT count(*) cnt FROM $t_filetypes WHERE nameregularexpression = :regex");
		$ins = $this->db->prepare("INSERT INTO $t_filetypes (filetype, nameregularexpression, headerseparation, \"group\", mngsngext) VALUES (:filetype, :regex, 'f', 'group', 't')");
		
		foreach ($this->fileTypes as $regex) {
			$sel->bindValue(':regex', $regex);
			if (!$sel->execute()) {
				throw new Exception("cannot fetch regex {$regex} count from $t_filetypes");
			}
			$r = $sel->fetch(PDO::FETCH_ASSOC);
			if ($r['cnt'] > 0) {
				$this->logger->info("no need to add filetype {$regex}");
			} else {
				$ins->bindValue(':filetype', $regex);
				$ins->bindValue(':regex', $regex);
				if (!$ins->execute()) {
					throw new Exception("cannot insert {$regex} into $t_filetypes");
				}
				$this->logger->info("added filetype {$regex}");
			}
		}
	}
	
	protected function configureRepositories() {
		$tm = $this->config->getTablesMapping();
		$t_srv_repositories = $tm['t_srv_repositories'];
		
		$sel = $this->db->prepare("SELECT count(*) cnt FROM $t_srv_repositories WHERE symbolicname = :symbolicname");
		$ins = $this->db->prepare("INSERT INTO $t_srv_repositories (symbolicname, transferprotocol_id, host_id, username, \"password\", remotepath, tempname_id, tempname_value, port, \"attributes\") VALUES (:symbolicname, :transferprotocol_id, :host_id, :username, :password, :remotepath, 1, 'tmp', :port, :attributes)");
		$upd = $this->db->prepare("UPDATE $t_srv_repositories SET transferprotocol_id = :transferprotocol_id, host_id = :host_id, username = :username, \"password\" = :password, remotepath = :remotepath, port = :port, \"attributes\" = :attributes WHERE symbolicname = :symbolicname");
		
		foreach ($this->repositories as $repo) {
			$sel->bindValue(':symbolicname', $repo->symbolicName);
			if (!$sel->execute()) {
				throw new Exception("cannot fetch repo {$repo->symbolicName} count from $t_srv_repositories");
			}
			$r = $sel->fetch(PDO::FETCH_ASSOC);
			if ($r['cnt'] > 0) {
				$upd->bindValue(':transferprotocol_id', $this->getTransferProtocolId($repo->transferProcotol));
				$upd->bindValue(':host_id', $this->getHostId($repo->host));
				$upd->bindValue(':username', $repo->username);
				$upd->bindValue(':password', $repo->password);
				$upd->bindValue(':remotepath', $repo->remotePath);
				$upd->bindValue(':port', $repo->port);
				$upd->bindValue(':attributes', $repo->attributes ? json_encode($repo->attributes) : null);
				$upd->bindValue(':symbolicname', $repo->symbolicName);
				if (!$upd->execute()) {
					throw new Exception("cannot update {$repo->symbolicName} with values " . print_r($repo, true));
				}
				$this->logger->info("updated repo {$repo->symbolicName} with values " . print_r($repo, true));
			} else {
				$ins->bindValue(':transferprotocol_id', $this->getTransferProtocolId($repo->transferProcotol));
				$ins->bindValue(':host_id', $this->getHostId($repo->host));
				$ins->bindValue(':username', $repo->username);
				$ins->bindValue(':password', $repo->password);
				$ins->bindValue(':remotepath', $repo->remotePath);
				$ins->bindValue(':port', $repo->port);
				$ins->bindValue(':attributes', $repo->attributes ? json_encode($repo->attributes) : null);
				$ins->bindValue(':symbolicname', $repo->symbolicName);
				if (!$ins->execute()) {
					throw new Exception("cannot insert repo {$repo->symbolicName} with values " . print_r($repo, true));
				}
				$this->logger->info("added repo {$repo->symbolicName}");
			}
		}
	}
	
	protected function configureReceptionRules() {
		$tm = $this->config->getTablesMapping();
		$t_srv_receptionrules = $tm['t_srv_receptionrules'];
		
		$sel = $this->db->prepare("SELECT count(*) cnt FROM $t_srv_receptionrules WHERE filetype_id = :filetype_id AND repository_id = :repository_id");
		$ins = $this->db->prepare("INSERT INTO $t_srv_receptionrules (filetype_id, repository_id, active, hostname, localpath, pollingperiod_sec, retryperiod_sec, discoveryplugin_id, php_timeout_sec, reception_tt_id, lastvisit) VALUES (:filetype_id, :repository_id, :active, :hostname, :localpath, :pollingperiod_sec, :retryperiod_sec, :discoveryplugin_id, :php_timeout_sec, :reception_tt_id, '1970-01-01 00:00:00')");
		$upd = $this->db->prepare("UPDATE $t_srv_receptionrules SET active = :active, hostname = :hostname, localpath = :localpath, pollingperiod_sec = :pollingperiod_sec, retryperiod_sec = :retryperiod_sec, discoveryplugin_id = :discoveryplugin_id, php_timeout_sec = :php_timeout_sec, reception_tt_id = :reception_tt_id WHERE filetype_id = :filetype_id AND repository_id = :repository_id");
		
		// default values
		$discoveryPlugin = $this->getDefaultDiscoveryPlugin();
		$reception_tt_id = $this->getDefaultReceptionTaskTableId();
		
		foreach ($this->receptionRules as $recrule) {
			$filetype_id = $this->getFiletypeId($recrule->fileType);
			$repository_id = $this->getRepositoryId($recrule->repository);
			$discoveryplugin_id = $this->getDiscoveryPluginId($discoveryPlugin);
			
			$sel->bindValue(':filetype_id', $filetype_id);
			$sel->bindValue(':repository_id', $repository_id);
			if (!$sel->execute()) {
				throw new Exception("cannot fetch reception rule for filetype {$recrule->fileType} and repository {$recrule->repository}");
			}
			$r = $sel->fetch(PDO::FETCH_ASSOC);
			if ($r['cnt'] > 0) {
				$upd->bindValue(':filetype_id', $filetype_id);
				$upd->bindValue(':repository_id', $repository_id);
				$upd->bindValue(':active', $recrule->active ? 't' : 'f');
				$upd->bindValue(':hostname', $recrule->hostname);
				$upd->bindValue(':localpath', $this->outputBasePath . '/' . $recrule->localPath);
				$upd->bindValue(':pollingperiod_sec', $recrule->pollingPeriod);
				$upd->bindValue(':retryperiod_sec', $recrule->retryPeriod);
				$upd->bindValue(':discoveryplugin_id', $discoveryplugin_id);
				$upd->bindValue(':php_timeout_sec', $recrule->phpTimeoutSec);
				$upd->bindValue(':reception_tt_id', $reception_tt_id);
				if (!$upd->execute()) {
					throw new Exception("cannot update reception rule for filetype {$recrule->fileType} and repository {$recrule->repository} with values " . print_r($recrule, true));
				}
				$this->logger->info("added reception rule for filetype {$recrule->fileType} and repository {$recrule->repository} with values " . print_r($recrule, true));
			} else {
				$ins->bindValue(':filetype_id', $filetype_id);
				$ins->bindValue(':repository_id', $repository_id);
				$ins->bindValue(':active', $recrule->active ? 't' : 'f');
				$ins->bindValue(':hostname', $recrule->hostname);
				$ins->bindValue(':localpath', $this->outputBasePath . '/' . $recrule->localPath);
				$ins->bindValue(':pollingperiod_sec', $recrule->pollingPeriod);
				$ins->bindValue(':retryperiod_sec', $recrule->retryPeriod);
				$ins->bindValue(':discoveryplugin_id', $discoveryplugin_id);
				$ins->bindValue(':php_timeout_sec', $recrule->phpTimeoutSec);
				$ins->bindValue(':reception_tt_id', $reception_tt_id);
				if (!$ins->execute()) {
					throw new Exception("cannot insert reception rule for filetype {$recrule->fileType} and repository {$recrule->repository} with values " . print_r($recrule, true));
				}
				$this->logger->info("added reception rule for filetype {$recrule->fileType} and repository {$recrule->repository} with values " . print_r($recrule, true));
			}
		}
	}
	
	protected function configureOutputPaths() {
		foreach ($this->outputPaths as $relativePath) {
			$pathname = $this->outputBasePath . '/' . $relativePath;
			if (!file_exists($pathname)) {
				if (!mkdir($pathname, null, true)) {
					throw new Exception("Cannot create output directory {$pathname}");
				}
				$this->logger->info("path $pathname created");
			} else {
				$this->logger->info("path $pathname exists");
			}
		}
	}
	
	/*
	 * DB utility methods
	 */
	
	protected function getTransferProtocolId($protocol) {
		$tm = $this->config->getTablesMapping();
		$t_transferprotocols = $tm['t_transferprotocols'];
		
		$sel = $this->db->prepare("SELECT id FROM $t_transferprotocols WHERE upper(protocol) = upper(:protocol)");
		$sel->bindValue(':protocol', $protocol);
		if (!$sel->execute()) {
			throw new Exception("cannot fetch protocol {$protocol} from $t_transferprotocols");
		}
		$r = $sel->fetch(PDO::FETCH_ASSOC);
		if ($r == false) {
			throw new Exception("cannot find protocol {$protocol}");
		}
		return $r['id'];
	}
	
	protected function getHostId($host) {
		$tm = $this->config->getTablesMapping();
		$t_knownhosts = $tm['t_knownhosts'];
		
		$sel = $this->db->prepare("SELECT id FROM $t_knownhosts WHERE host_name = :host");
		$sel->bindValue(':host', $host);
		if (!$sel->execute()) {
			throw new Exception("cannot fetch host {$host} from $t_knownhosts");
		}
		$r = $sel->fetch(PDO::FETCH_ASSOC);
		if ($r == false) {
			throw new Exception("cannot find host {$host}");
		}
		return $r['id'];
	}

	protected function getFiletypeId($filetype) {
		$tm = $this->config->getTablesMapping();
		$t_filetypes = $tm['t_filetypes'];
		
		$sel = $this->db->prepare("SELECT id_filetype FROM $t_filetypes WHERE filetype = :filetype");
		$sel->bindValue(':filetype', $filetype);
		if (!$sel->execute()) {
			throw new Exception("cannot fetch filetype {$filetype} from $t_filetypes");
		}
		$r = $sel->fetch(PDO::FETCH_ASSOC);
		if ($r == false) {
			throw new Exception("cannot find filetype {$filetype}");
		}
		return $r['id_filetype'];
	}
	
	protected function getRepositoryId($symbolicName) {
		$tm = $this->config->getTablesMapping();
		$t_srv_repositories = $tm['t_srv_repositories'];
		
		$sel = $this->db->prepare("SELECT id_repository FROM $t_srv_repositories WHERE symbolicname = :symbolicname");
		$sel->bindValue(':symbolicname', $symbolicName);
		if (!$sel->execute()) {
			throw new Exception("cannot fetch repository {$symbolicName} from $t_srv_repositories");
		}
		$r = $sel->fetch(PDO::FETCH_ASSOC);
		if ($r == false) {
			throw new Exception("cannot find repository {$symbolicName}");
		}
		return $r['id_repository'];
	}
	
	protected function getDiscoveryPluginId($name) {
		$tm = $this->config->getTablesMapping();
		$t_discoveryplugins = $tm['t_discoveryplugins'];
		
		$sel = $this->db->prepare("SELECT id_discoveryplugin FROM $t_discoveryplugins WHERE name = :name");
		$sel->bindValue(':name', $name);
		if (!$sel->execute()) {
			throw new Exception("cannot fetch discovery plugin {$name} from $t_discoveryplugins");
		}
		$r = $sel->fetch(PDO::FETCH_ASSOC);
		if ($r == false) {
			throw new Exception("cannot find discovery plugin {$name}");
		}
		return $r['id_discoveryplugin'];
	}
	
	/*
	 * 
	 */
	
	protected function getDefaultDiscoveryPlugin() {
		$value = getenv("DATA_RETRIEVER_PLUGIN");
		if ($value !== false) {
			return $value;
		} else {
			return 'DataRetrieverPluginOData';
		}
	}
	
	protected function getDefaultReceptionTaskTableId() {
		$value = getenv("RECEPTION_TASK_TABLE_ID");
		if ($value !== false) {
			return $value;
		} else {
			return 1;
		}
	}
}

$config = Config::get();

/**
 * get logger
 */
PHPImportLogger::create($config);
PHPImportLogger::get()->info('Launching DataRetriever Configurator...');
PHPImportLogger::get()->info('ARGS: ' . join(',', $_SERVER['argv']));

/**
 * run
 */
PHPImportDB::create($config);

$envCreateLocalPaths = getenv('PHP_DATARETRIEVER_CONFIGURATOR_CREATE_LOCAL_PATHS');
$createLocalPaths = $envCreateLocalPaths !== false ? $envCreateLocalPaths : true;
		
$configurator = new ReceptionRulesConfigurator($config, $_SERVER['argv'][1], $_SERVER['argv'][2], $createLocalPaths);
$configurator->configure();

PHPImportLogger::get()->info('done');

?>