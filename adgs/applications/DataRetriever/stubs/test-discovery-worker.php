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
require_once(__DIR__.'/PHPImportLoggerTest.php');

use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\PHPImportLoggerTest;
use Acs\PHPImport\Service\Gearman\TaskPayload;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginOData;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginInterface;
use Acs\PHPImport\Service\Tools\PluginTools;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginMpip;

/*
 * Plugins override
 */
function printTestWorkLoad(TaskPayload $payload) {
	$repoAttributes = array_key_exists('repoAttributes', $payload->rulePayload) ? 
		"'" . json_encode($payload->rulePayload['repoAttributes'], JSON_PRETTY_PRINT) . "'" : null;
	
	// use the real plugin class
	$pluginClass = 'Acs\PHPImport\Service\Plugin\\' . preg_replace('/Test$/', '', $payload->pluginClass);
	echo <<<EOF

\$wl = \$tdw->createWorkLoad(
	'{$pluginClass}',
	{$payload->rulePayload['idRule']},
	'{$payload->rulePayload['finalDir']}',
	'{$payload->rulePayload['baseTempDir']}',  
	'{$payload->rulePayload['product']->getName()}',
	'{$payload->rulePayload['product']->getId()}',
	'{$payload->rulePayload['identifier']}', 
	'{$payload->rulePayload['credentials']}', 
	'{$payload->rulePayload['url']}',
	'{$payload->rulePayload['product']->getPublicationDate()}',
	'{$payload->rulePayload['product']->getEvictionDate()}',
	{$repoAttributes});

EOF;
}

class DataRetrieverPluginODataTest extends DataRetrieverPluginOData {
	protected function downloadProducts($productsToDownload) {
		foreach ($productsToDownload as $product) {
			$url = PluginTools::getRepositoryUrlFromPayload($this->rulePayload);
			$payload = $this->createGearmanPayload($url, $product);
			printTestWorkLoad($payload);
		}
	}
}
class DataRetrieverPluginMpipTest extends DataRetrieverPluginMpip {
	protected function downloadProducts($productsToDownload) {
		foreach ($productsToDownload as $product) {
			$url = PluginTools::getRepositoryUrlFromPayload($this->rulePayload);
			$payload = $this->createGearmanPayload($url, $product);
			printTestWorkLoad($payload);
		}
	}
}

class TestImportWorker {
	protected $config;
	protected $internalLogger;
	
	public function __construct(Config $configuration) {
		$this->config = $configuration;
		
		// setup internal logger
		PHPImportLoggerTest::create($this->config);
		$this->internalLogger = PHPImportLoggerTest::get();
	}

	public function run(TaskPayload $payload) {
		$this->internalLogger->debug("received download task");
		
		try {
			$classname = $payload->pluginClass;
			// use the plugin test overrides
			$classname = end(explode('\\', $classname)) . 'Test';

			$plugin = new $classname(
				$payload->config,
				$payload->logger,
				$payload->rulePayload);

			if (! $plugin instanceof DataRetrieverPluginInterface) {
				throw new RuntimeException("class {$classname} is not an instance of DataRetrieverPluginInterface");
			}
			
			$plugin->discovery();
			
			$this->internalLogger->debug("done import task");
		} catch(Exception $e) {
			$this->internalLogger->error($e->getMessage());
			throw $e;
		}
	}
	
	public function createWorkLoad($receptionRuleId, $pdsFinalDir, $tempDir, $namespace, $pluginClass, $repoProtocol, $repoHostname, $repoPort, $repoRemotePath, $repoUsername, $repoPassword, $attributes = null) {
		$class = $namespace . '\\' . $pluginClass;
		$taskPayload = new TaskPayload(
			$this->config, 
			$this->internalLogger, 
			$class,
			array(
				'pds_final_dir' => $pdsFinalDir,
				'base_temp_dir' => $tempDir,
				'id_rule' => $receptionRuleId,
				'plugin' => $pluginClass,
				'retrycounter' => 0,
				'repo_protocol' => $repoProtocol,
				'repo_hostname' => $repoHostname,
				'repo_port' => $repoPort,
				'repo_remotepath' => $repoRemotePath,
				'repo_username' => $repoUsername,
				'repo_password' => $repoPassword,
				'repo_name' => 'test-repo',
				'timeout' => 30,
				'filetype' => 'TEST',
				'regexp' => '.*',
				'id_filetype' => 0
			)
		);
		if ($attributes !== null)
			$taskPayload->rulePayload['repo_attributes'] = $attributes;
		return $taskPayload;
	}
}

date_default_timezone_set('UTC');

$tiw = new TestImportWorker(new Config());

/**
$wl = $tiw->createWorkLoad(
	4,
	'/var/tmp/S1A',
	'/tmp',
	'Acs\PHPImport\Service\Plugin',
	'DataRetrieverPluginOData',
	'http',
// 	'172.17.0.1',
	1080,
	'/prip-php-mock.php/Products',
	'lta',
	'12qwas',
	null
	);
/**/
/**
$json = <<<JSON
{
	"odata_filter": "PublicationDate gt 2021-12-05T00:00:00Z and PublicationDate lt 2021-12-06T00:00:00Z",
	"omit_format": true,
	"propagate_headers_redirect": true,
	"propagate_headers_redirect_download": true,
	"curl_options": {
		"CURLOPT_VERBOSE": 1,
		"CURLOPT_MAX_RECV_SPEED_LARGE": 2048,
		"CURLOPT_BAD_CONSTANT": "foo"
	}
}
JSON;
$wl = $tiw->createWorkLoad(
	4,
	'/var/tmp/S1A',
	'/tmp',
	'Acs\PHPImport\Service\Plugin',
	'DataRetrieverPluginOData',
	'http',
	'172.17.0.1',
	1080,
	'/prip-php-mock.php/Products',
	'lta',
	'12qwas',
	$json
	);
/**/
/**
// ssh -L 172.17.0.1:9443:c1.ops.s1pdgs.eu:443 root@172.18.2.20
$json = <<<JSON
{
	"authentication_type": "OAuth2", 
	"client_id": "s1pdgs_client", 
	"client_secret": "9680267b-c068-4e79-a690-368c1d8a5cae", 
	"token_url": "https://172.17.0.1:9443/auth/realms/s1pdgs/protocol/openid-connect/token", 
	"oauth2_header": "Authorization: Bearer ",
	"odata_filter": "PublicationDate gt 2022-01-05T00:00:00Z and PublicationDate lt 2022-01-06T00:00:00Z",
	"curl_options": {
		"CURLOPT_VERBOSE": 1
	}
}
JSON;
$wl = $tiw->createWorkLoad(
	4,
	'/var/tmp/S1A',
	'/tmp',
	'Acs\PHPImport\Service\Plugin',
	'DataRetrieverPluginOData',
	'https',
	'172.17.0.1',
	9443,
	'/odata/v1/Products',
	'lta_expr',
	'gbjoxCNHwUWhw3p',
	$json
	);
/**
$json = <<<JSON
{
	"authentication_type": "OAuth2",
	"client_id": "FzZkBEXv9LBey7Fr1FMwvUXTqUka",
	"client_secret": "iyQFoFVdxzfBWNQPsuJ_5jJPJnsa",
	"token_url": "https://prip.s2a.atos.copernicus.eu/token",
	"oauth2_header": "Authorization: Bearer ",
	"odata_filter": "PublicationDate gt 2023-05-15T00:00:00Z",
	"propagate_headers_redirect": true,
	"propagate_headers_redirect_download": false,
	"curl_options": {
		"CURLOPT_VERBOSE": 1
	}
}
JSON;
$wl = $tiw->createWorkLoad(
	4,
	'/var/tmp/S2A',
	'/tmp',
	'Acs\PHPImport\Service\Plugin',
	'DataRetrieverPluginOData',
	'https',
	'prip.s2a.atos.copernicus.eu',
	443,
	'/odata/v1/Products',
	'ltaExprivia',
	'HEjbaxXJ',
	$json
	);
/**/
/**/
$json = <<<JSON
{
	"authentication_type": "OAuth2",
	"client_id": "mpip-api",
	"client_secret": "3ufepo4rIriPU1IPZkxLMm4F8sVZRFCW",
	"token_url": "http://172.17.0.1:1080/mpip-php-mock.php/token",
	"oauth2_header": "Authorization: Bearer ",
	"propagate_headers_redirect": true,
	"propagate_headers_redirect_download": false,
	"curl_options": {
		"CURLOPT_VERBOSE": 1
	}
}
JSON;
$wl = $tiw->createWorkLoad(
	4,
	'/var/tmp/MPIP',
	'/tmp',
	'Acs\PHPImport\Service\Plugin',
	'DataRetrieverPluginMpip',
	'http',
	'172.17.0.1',
	1080,
	'/mpip-php-mock.php',
	'adgs-user',
	'9xZP6diZ14&o',
	$json
	);
$wl->rulePayload['filetype'] = 'MPL_TIMELINE';
/**/
if (mkdir($wl->rulePayload['pds_final_dir'], 0775, true) === false)
	throw new RuntimeException("error creating {$wl->rulePayload['pds_final_dir']}");
$tiw->run($wl);

?>