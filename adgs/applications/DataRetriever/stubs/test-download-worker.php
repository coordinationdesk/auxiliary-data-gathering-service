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
use Acs\PHPImport\Service\Plugin\Data\Product;
use Acs\PHPImport\Service\Plugin\DataRetrieverPluginInterface;


class TestDownloadWorker {
	protected $config;
	protected $internalLogger;
	
	public function __construct(Config $configuration) {
		$this->config = $configuration;
		
		// setup internal logger
		PHPImportLoggerTest::create($this->config);
		$this->internalLogger = PHPImportLoggerTest::get();
	}

	public function run(TaskPayload $payload) {
		$logger = $payload->logger;
		$logger->debug("received download task");
		
		$classname = $payload->pluginClass;
		
		try {
			$plugin = new $classname(
				$payload->config,
				$payload->logger,
				$payload->rulePayload);

			if (! $plugin instanceof DataRetrieverPluginInterface) {
				throw new RuntimeException("class {$classname} is not an instance of DataRetrieverPluginInterface");
			}
			
			$plugin->download();
			
			$logger->debug("done download task");
		} catch(Exception $e) {
			$logger->error($e);
			throw $e;
		}
	}
	
	public function createWorkLoad($pluginClass, $idRule, $finalDir, $tempDir, $productName, $productUuid, $identifier, $credentials, $url, $publicationDate, $evictionDate, $ruleAttributes = null) {
		
		$payloadArray = array(
			'product' => new Product(
				$productUuid,
				$productName,
				$publicationDate,
				$evictionDate
				),
			'finalDir' => $finalDir,
			'baseTempDir' => $tempDir,
			'identifier' => $identifier,
			'url' => $url,
			'credentials' => $credentials,
			'host' => 'localhost',
			'timeout' => 30,
			'idRule' => $idRule,
			'repoName' => $identifier
		);
		if ($ruleAttributes !== null) {
			$payloadArray['repoAttributes'] = json_decode($ruleAttributes);
		}
		
		$taskPayload = new TaskPayload(
			$this->config, 
			$this->internalLogger, 
			$pluginClass,
			$payloadArray
			);
		
		return $taskPayload;
	}
}

date_default_timezone_set('UTC');

$tdw = new TestDownloadWorker(new Config());

/* mock, with timeout product
$wl = $tdw->createWorkLoad(
	'Acs\PHPImport\Service\Plugin\DataRetrieverPluginOData',
	4,
	'/var/tmp/S1A',
	'/tmp',
	'S1A_IW_RAW__0SDV_20201116T143651_20201116T143724_035275_041EB6_3853.SAFE.zip',
	'f3a31de8-2db6-11eb-a607-bc97e19fed56',
	'4-test-repo-TEST',
	'lta:12qwas',
	'http://172.17.0.1:1080/prip-php-mock.php/Products',
	'2021-08-04T10:37:28.000000Z',
	'2021-08-04T11:39:28.000000Z',
	NULL);
/**/
/* mock, with redirect product *
$json = <<<JSON
{
	"omit_format": true,
	"propagate_headers_redirect_download": true
}
JSON;
$wl = $tdw->createWorkLoad(
	'Acs\PHPImport\Service\Plugin\DataRetrieverPluginOData',
	4,
	'/var/tmp/S1A',
	'/tmp',
	'S1A_EW_RAW__0SSH_20160206T090038_20160206T090146_009823_00E612_365C.SAFE.zip',
	'0395d0d6-41d2-11eb-a3bc-bc97e19fed56',
	'4-test-repo-TEST',
	'lta:12qwas',
	'http://172.17.0.1:1080/prip-php-mock.php/Products',
	'2021-08-05T10:28:09.987654Z',
	'2021-08-05T11:30:09.123456Z',
	$json);
/**/
/* S2A product from ATOS *
$wl = $tdw->createWorkLoad(
	'Acs\PHPImport\Service\Plugin\DataRetrieverPluginOData',
	4,
	'/var/tmp/S2A',
	'/tmp',
	'S2A_OPER_MSI_L1B_GR_2APS_20230514T225800_S20230514T194256_D11_N05.09.tar',
	'2ca1c3b0-ad10-4ccb-8a5a-a82387b88996',
	'4-test-repo-TEST',
	'ltaExprivia:HEjbaxXJ',
	'https://prip.s2a.atos.copernicus.eu:443/odata/v1/Products',
	'2023-05-15T00:04:02.913Z',
	'2023-05-22T00:04:02.913Z',
	'{
	    "authentication_type": "OAuth2",
	    "client_id": "FzZkBEXv9LBey7Fr1FMwvUXTqUka",
	    "client_secret": "iyQFoFVdxzfBWNQPsuJ_5jJPJnsa",
	    "token_url": "https:\/\/prip.s2a.atos.copernicus.eu\/token",
	    "oauth2_header": "Authorization: Bearer ",
	    "odata_filter": "PublicationDate gt 2023-05-15T00:00:00Z",
	    "propagate_headers_redirect": true,
	    "propagate_headers_redirect_download": false,
	    "curl_options": {
	        "CURLOPT_VERBOSE": 1
	    }
	}');
/**/
/* MPIP-mock product */
$wl = $tdw->createWorkLoad(
	'Acs\PHPImport\Service\Plugin\DataRetrieverPluginMpip',
	4,
	'/var/tmp/MPIP',
	'/tmp',
	'S2B_OPER_MPL_TIMELINE_20230516T221043_20230528T221043.tgz',
	'S2B_OPER_MPL_TIMELINE_20230516T221043_20230528T221043.tgz',
	'4-test-repo-MPL_TIMELINE',
	'adgs-user:9xZP6diZ14&o',
	'http://172.17.0.1:1080/mpip-php-mock.php',
	'2023-05-16T14:10:43.123+00:00',
	'',
	'{
	    "authentication_type": "OAuth2",
	    "client_id": "mpip-api",
	    "client_secret": "3ufepo4rIriPU1IPZkxLMm4F8sVZRFCW",
	    "token_url": "http:\/\/172.17.0.1:1080\/mpip-php-mock.php\/token",
	    "oauth2_header": "Authorization: Bearer ",
	    "propagate_headers_redirect": true,
	    "propagate_headers_redirect_download": false,
	    "curl_options": {
	        "CURLOPT_VERBOSE": 1
	    }
	}');
/**/
$finalDir = $wl->rulePayload['finalDir'];
if (mkdir($finalDir, 0775, true) === false)
	throw new RuntimeException("error creating {$finalDir}");
	
$tdw->run($wl);

?>