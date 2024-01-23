<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2010 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
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
// $Prod: webcat_feeder_sdfbroker_csndc $
// $Author: andema $
// $Id: acs_geoserver_tools.php,v 1.31 2017/03/16 10:25:33 andema Exp $
// $Revision: 1.31 $
//

require_once 'ACSPhpLib/acs_tools.php';

class acs_geoserver_rest {
	protected	$_logHandler,
				$rest,
				$type,
				$rest_type,
				$curl_extra_options,
				$reload_after_changes = false,
				$reload_object = null;

	public function __construct($url, &$loghandler = null, $type = 'json', array $curl_extra_options = array(),$auto_close_connection=true)
	{
		$this->_logHandler = $loghandler;
		$this->rest = new acs_rest_geoserver_customized($url, $loghandler);
        $this->rest->auto_close_connection = $auto_close_connection;
        $this->setProtocolType($type);
		$this->setCurlExtraOptions($curl_extra_options);
	}

	public function enableAutoReload($status, $object = null)
	{
		$this->reload_after_changes = $status;
		$this->reload_object = $object == null ? $this : $object;
	}
	
	public function setProtocolType($type)
	{
		$this->type = $type;
		$this->rest_type = $type;
		if ($type == 'xmlobj')
			$this->rest_type = 'xml';

		switch ($this->rest_type)
		{
			case 'json':
				$this->rest->content_type = 'application/json';
				break;
			case 'xml':
				$this->rest->content_type = 'text/xml';
				break;
			default:
				throw new Exception("Unknown REST protocol type: {$this->rest_type}");
		}
	}

	public function setCurlExtraOptions(array $options)
	{
		$this->curl_extra_options = $options;
	}

	public function authorization($enabled, $username = null, $password = null,$type=null)
	{
		if ($enabled)
		{
			$this->curl_extra_options[CURLOPT_UNRESTRICTED_AUTH] = true;
	        $this->curl_extra_options[CURLOPT_HTTPAUTH] = ($type==null)?CURLAUTH_ANY:$type;
	        $this->curl_extra_options[CURLOPT_USERPWD] = "$username:$password";
		}
		else
		{
			unset($this->curl_extra_options[CURLOPT_UNRESTRICTED_AUTH]);
	        unset($this->curl_extra_options[CURLOPT_HTTPAUTH]);
	        unset($this->curl_extra_options[CURLOPT_USERPWD]);
		}
	}

	protected function decodeResponse($contents) //, $headers)
	{
		switch ($this->type)
		{
			case 'json':
				return json_decode($contents, true);
			case 'xml':
				return $contents;
			case 'xmlobj':
				return simplexml_load_string($contents);
			default:
				throw new Exception("Unknown REST protocol type response: {$this->type}");
		}
	}

	protected function encodeRequest($data)
	{
		switch ($this->type)
		{
			case 'json':
				return json_encode($data);
			case 'xml':
				return $data;
			case 'objxml':
				return $data->asXML();
			default:
				throw new Exception("Unknown REST protocol type response: {$this->type}");
		}
	}

	protected function doCall($type, $url, $data = null, $contentType = 'text/xml')
	{
		$this->rest->curl_extra_options = $this->curl_extra_options;
        if(isset($this->_logHandler)) $this->_logHandler->debug("REST $type @ url = " . $this->rest->getBaseUrl() . $url);
		switch ($type)
		{
			case 'GET':
				$ret = $this->rest->get($url);
				$ret['data'] = $this->decodeResponse($ret['data']);
				if(isset($this->_logHandler)) $this->_logHandler->debug('REST GET received: '.$ret['data']);
                break;
			case 'POST':
                if(isset($this->_logHandler)) $this->_logHandler->debug('REST POST data: '.var_export($data,true));
				$data = $this->encodeRequest($data);
				$ret = $this->rest->post($url, $data, $contentType, true);
				break;
			case 'DELETE':
				$ret = $this->rest->delete($url);
				break;
			case 'PUT':				
				$ret = $this->rest->put($url, $data, $contentType, true);
				break;
			default:
				throw new Exception("geoserver rest request $type not yet implemented");
		}
		return $ret;
	}

    public function closeConnection(){
    	if(isset($this->rest)) {
           if(isset($this->_logHandler)) $this->_logHandler->debug('REST: closing connection');
    	   $this->rest->closeConnection();
    	}
    }

	public function getVersion()
    {
    	$url = "/about/version";
    	return $this->doCall('GET', $url);
    }

    public function addWorkspace($workspace_def)
    {
    	$url = "/workspaces";
    	return $this->doCall('POST', $url, $workspace_def);
    }
    
	public function getWorkspaces()
	{
		$url = sprintf("/workspaces.%s", $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getWorkspace($workspace)
	{
		$url = sprintf("/workspaces/%s.%s", $workspace, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	//
	// VECTOR
	//
	public function getDataStores($workspace)
	{
		$url = sprintf("/workspaces/%s/datastores.%s", $workspace, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getDataStore($workspace, $datastore)
	{
		$url = sprintf("/workspaces/%s/datastores/%s.%s", $workspace, $datastore, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getFeatureTypes($workspace, $datastore)
	{
		$url = sprintf("/workspaces/%s/datastores/%s/featuretypes.%s", $workspace, $datastore, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getFeatureType($workspace, $datastore, $featuretype)
	{
		$url = sprintf("/workspaces/%s/datastores/%s/featuretypes/%s.%s", $workspace, $datastore, $featuretype, $this->rest_type);
		return $this->doCall('GET', $url);
	}


	//
	// RASTER
	//
	public function getCoverageStores($workspace)
	{
		$url = sprintf("/workspaces/%s/coveragestores.%s", $workspace, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getCoverageStore($workspace, $store)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s.%s", $workspace, $store, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getCoverages($workspace, $store)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/coverages.%s", $workspace, $store, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getCoverage($workspace, $store, $coverage)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/coverages/%s.%s", $workspace, $store, $coverage, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getLayers()
	{
		$url = sprintf("/layers.%s", $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getLayer($layer_name)
	{
		$url = sprintf("/layers/%s.%s", $layer_name, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getLayerGroup($name,$workspace_name='')
	{	
		$workspace_name=($workspace_name!=='')?'workspaces/'.$workspace_name:$workspace_name;
		$url = sprintf("/%s/layergroups/%s.%s", $workspace_name,$name, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getStyles()
	{
		$url = sprintf("/styles.%s", $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function getStyle($style_name)
	{
		$url = sprintf("/style/%s.%s", $style_name, $this->rest_type);
		return $this->doCall('GET', $url);
	}

	public function modifyLayer($layer_name, $layer_data, $contentType='text/xml')
	{
		$url = sprintf("/layers/%s.%s", $layer_name, $this->rest_type);
		$ret =  $this->doCall('PUT', $url, $layer_data, $contentType);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
				
		return $ret;
	}	

	public function modifyLayerDefaultStyle($layer_name, $layer_data, $contentType='text/xml')
	{
		$url = sprintf("/layers/%s.%s", $layer_name, $this->rest_type);
		$ret =  $this->doCall('PUT', $url, $layer_data, $contentType);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
				
		return $ret;
	}	
	
	public function modifyLayerGroup($name, $data, $workspace_name='',$contentType='text/xml')
	{
		$workspace_name=($workspace_name!=='')?'workspaces/'.$workspace_name:$workspace_name;
		$url = sprintf("/%s/layergroups/%s.%s", $workspace_name, $name, $this->rest_type);
		$ret =  $this->doCall('PUT', $url, $data, $contentType);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
				
		return $ret;
	}
	
	public function addCoverageFromFile($workspace, $store, $filepath, $contentType='text/plain', $layerName=null)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/external.geotiff", $workspace, $store);
		if ($layerName != null) {
			$url = $url . "?configure=first&coverageName=" . $layerName;
		}
		$ret =  $this->doCall('PUT', $url, $filepath, $contentType);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
				
		return $ret;
	}
	
	public function addDataFromShapefile($workspace, $store, $data, $contentType='application/zip', $layerName=null)
	{
		$url = sprintf("/workspaces/%s/datastores/%s/file.shp", $workspace, $store);
		$url = $url . "?configure=all";
// to test
//		if ($layerName != null) {
//			$url = $url . "?configure=first&layerName=" . $layerName;
//		}
		$ret =  $this->doCall('PUT', $url, $data, $contentType);
				
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		return $ret;
	}
	
	public function addDataStore($workspace, $store_data)
	{
		$url = sprintf("/workspaces/%s/datastores.%s", $workspace, $this->rest_type);
		$ret =  $this->doCall('POST', $url, $store_data);
	
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
	
		return $ret;
	}
	
	public function addFeatureType($workspace, $store_name, $feature_data) {
		$url = sprintf("/workspaces/%s/datastores/%s/featuretypes.%s", $workspace, $store_name, $this->rest_type);
		$ret =  $this->doCall('POST', $url, $feature_data);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}
	
	public function addStyleFromFile($data, $contentType='application/vnd.ogc.sld+xml', $styleName=null)
	{
		$url = "/styles.SLD";
		if ($styleName != null) {
			$url = $url . "?name=" . $styleName;
		}
		$ret =  $this->doCall('POST', $url, $data, $contentType);
				
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		return $ret;
	}

	public function modifyStyleFromFile($data, $contentType='application/vnd.ogc.sld+xml', $styleName=null)
	{
		$url = sprintf("/styles/%s.SLD", $styleName);

		$ret =  $this->doCall('PUT', $url, $data, $contentType);
				
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		return $ret;
	}

	public function addNewStyleToLayer($data, $layer, $contentType='application/vnd.ogc.sld+xml')
	{		
		$url = sprintf("/layers/%s/styles.XML", $layer);
		
		$ret =  $this->doCall('POST', $url, $data, $contentType);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();		
		return $ret;
	}
			
	public function addCoverageFromData($workspace, $store, $data, $contentType='image/tiff', $layerName=null, $fileName = 'file.geotiff', $configure = null)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/" . $fileName, $workspace, $store);
		if ($layerName != null) {
			if ($configure == null)
				$configure = 'first';
			$url = $url . "?configure={$configure}&coverageName=" . $layerName;
		} else if ($configure != null) {
			$url = $url . "?configure={$configure}";
		}
		$ret =  $this->doCall('PUT', $url, $data, $contentType);
				
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
				
		return $ret;
	}

	public function addCoverageStore($workspace, $store)
	{
		$url = sprintf("/workspaces/%s/coveragestores", $workspace);
		$ret = $this->doCall('POST', $url, $store);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}

	public function modifyCoverageStore($workspace, $store_name, $store_data)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s.%s", $workspace, $store_name, $this->rest_type);
		$ret = $this->doCall('PUT', $url, $store_data, 'text/xml');
	
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
	
		return $ret;
	}
	
	public function addCoverage($workspace, $store, $layer)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/coverages", $workspace, $store);
		$ret =  $this->doCall('POST', $url, $layer);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}
	
	public function addGranuleToCoverage($workspace, $store, $data, $contentType='text/plain', $granuleType = 'external.imagemosaic')
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/%s", $workspace, $store, $granuleType);
		$ret =  $this->doCall('POST', $url, $data, $contentType);
	
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
	
		return $ret;
	}

	public function addLayerGroup($layergroup,$workspace_name='')
	{
		$workspace_name=($workspace_name!=='')?'workspaces/'.$workspace_name:$workspace_name;
		$url = sprintf("/%s/layergroups", $workspace_name,$this->rest_type);
		$ret =  $this->doCall('POST', $url, $layergroup);
	
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
	
		return $ret;
	}

	public function deleteWorkSpace($workspace,$recursive=false)
	{
		$url = sprintf("/workspaces/%s", $workspace);
		if ($recursive) $url.= '?recurse=true';
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}

	public function deleteCoverageStore($workspace, $store,$recursive=false)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s", $workspace, $store);
		if ($recursive) $url.= '?recurse=true';
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}

	public function deleteCoverage($workspace, $store, $layer,$recursive=false)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/coverages/%s", $workspace, $store, $layer);
		if ($recursive) $url.= '?recurse=true';
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}
	
	public function deleteGranuleFromCoverage($workspace, $store, $filter = null)
	{
		$url = sprintf("/workspaces/%s/coveragestores/%s/coverages/%s/index/granules.xml", $workspace, $store, $store);
		
		if ($filter)
			$url .= "?filter=" . urlencode($filter);
		$ret =  $this->doCall('DELETE', $url);
	
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
	
		return $ret;
	}

	public function deleteLayer($layer)
	{
		$url = sprintf("/layers/%s/", $layer);
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}
	
	public function deleteDataStore($workspace, $store, $recurse = false)
	{
		$url = sprintf("/workspaces/%s/datastores/%s", $workspace, $store);
		if ($recurse) $url.= '?recurse=true';
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}

	public function deleteLayerGroup($name,$workspace_name='')
	{
		$workspace_name=($workspace_name!=='')?'workspaces/'.$workspace_name:$workspace_name;
		$url = sprintf("/%s/layergroups/%s/", $workspace_name, $name);
		$ret = $this->doCall('DELETE', $url);
		
		if ($this->reload_after_changes) $this->reload_object->reloadConfiguration();
		
		return $ret;
	}
	
	public function reloadConfiguration()
	{
		$url = "/reload";
		return $this->doCall('POST', $url);
	}

}

class acs_geoserver_cluster_nodes_reload {
	protected $rest_clients;

	public function __construct(array $server_urls, $log_handler = null, $type = 'json', $curl_extra_opts = array())
	{
		foreach ($server_urls as $url)
			$this->rest_clients[] = new acs_geoserver_rest($url, $log_handler, $type, $curl_extra_opts);
	}
	
	public function reloadConfiguration() {
		foreach ($this->rest_clients as $client)
			$client->reloadConfiguration();
	}
}

class acs_geoserver_rest_json_utils {
	static public function buildWorkspace($workspace_name) {
		return array('workspace' => array('name' => $workspace_name));
	}
	
	static public function buildDataStore($workspace_name, $store_name, $file_url, $enabled = true) {
		return array (
				'dataStore' =>
				array (
						'name' => $store_name,
						'enabled' => $enabled,
						'workspace' =>
						array (
								'name' => $workspace_name
						),
						'connectionParameters' =>
						array (
								'entry' =>
								array (
										0 =>
										array (
												'@key' => 'url',
												'$' => $file_url,
										),
										1 =>
										array (
												'@key' => 'namespace',
												'$' => 'http://' . $workspace_name,
										),
								),
						),
				)
		);
	}
	
	static public function buildFeatureType($layer_name, $store_name, $feature_name, $enabled = true) {
		return array (
				'featureType' =>
				array (
						'name' => $layer_name,
						'nativeName' => $feature_name,
						'title' => $layer_name,
						'enabled' => $enabled,
						'store' =>
						array (
								'@class' => 'dataStore',
								'name' => $store_name,
						),
						'maxFeatures' => 0,
						'numDecimals' => 0,
				)
		);
	}

	static public function buildCoverageStore($name, $workspace_name, $relative_filename, $type = 'GeoTIFF', $enabled = true)
	{
		return array('coverageStore' =>
					array('name' => $name,
						'type' => $type,
						'enabled' => $enabled,
						'workspace' =>
							array('name' => $workspace_name),
						'url' => 'file:' . $relative_filename,
						)
		);
	}

	static public function buildCoverage($name, $workspace_name, $coverage_store_name, array $options = array())
	{
		return array ('coverage' =>
				    array (
				      'name' => $name,
				      'namespace' =>
					      array (
					        'name' => 'acs',
//					        'href' => 'http://emsa-web-develop.acs:7001/geoserver/rest/namespaces/acs.json',
					      ),
				      'title' => $name,
				      'description' => $options['description'],
				      'nativeCRS' => $options['crs'],
				      'srs' => $options['srs'],
				      'nativeBoundingBox' =>
				      array (
				        'minx' => $options['minx'],
				        'maxx' => $options['maxx'],
				        'miny' => $options['miny'],
				        'maxy' => $options['maxy'],
				        'crs' => $options['srs'],
				      ),
				      'latLonBoundingBox' =>
				      array (
				        'minx' => $options['minx'],
				        'maxx' => $options['maxx'],
				        'miny' => $options['miny'],
				        'maxy' => $options['maxy'],
				        'crs' => $options['srs'],
				      ),
				      'projectionPolicy' => 'REPROJECT_TO_DECLARED',
				      'enabled' => true,
/*				      'metadata' =>
				      array (
				        'entry' =>
				        array (
				          0 =>
				          array (
				            '@key' => 'cachingEnabled',
				            '$' => 'false',
				          ),
				          1 =>
				          array (
				            '@key' => 'dirName',
				            '$' => 'TEST_MICA_PYRAMIDGEO_PyramidGEO',
				          ),
				        ),
				      ),*/
				      'store' =>
				      array (
				        '@class' => 'coverageStore',
				        'name' => $coverage_store_name,
				        //'href' => 'http://emsa-web-develop.acs:7001/geoserver/rest/workspaces/acs/coveragestores/TEST_MICA_PYRAMIDGEO.json',
				      ),
				      'nativeFormat' => $options['format'],
				      'grid' =>
				      array (
				        '@dimension' => '2',
				        'range' =>
				        array (
				          'low' => '0 0',
				          'high' => "{$options['size_x']} {$options['size_y']}",
				        ),
				        'transform' =>
				        array (
				          'scaleX' => 1,
				          'scaleY' => 1,
				          'shearX' =>
				          array (
				            0 => 0,
				            1 => 0,
				            2 => $options['maxx'],
				            3 => $options['maxy'],
				          ),
				        ),
				        'crs' => $options['srs'],
				      ),
				      'supportedFormats' =>
				      array (
				        'string' =>
				        array (
				          0 => 'GIF',
				          1 => 'PNG',
				          2 => 'JPEG',
				          3 => 'TIFF',
				          4 => 'GEOTIFF',
				        ),
				      ),
				      'interpolationMethods' =>
				      array (
				        'string' =>
				        array (
				          0 => 'bilinear',
				          1 => 'bicubic',
				        ),
				      ),
				      'dimensions' =>
				      array (
				        'coverageDimension' =>
				        array (
				          0 =>
				          array (
				            'name' => 'GRAY_INDEX',
				            'description' => 'GridSampleDimension[-Infinity,Infinity]',
				          ),
				        ),
				      ),
				      'requestSRS' =>
				      array (
				        'string' =>
				        array (
				          0 => $options['srs'],
				        ),
				      ),
				      'responseSRS' =>
				      array (
				        'string' =>
				        array (
				          0 => $options['srs'],
				        ),
				      ),
				    ),
				  );
	}
}

class acs_geoserver_rest_xml_utils {
	static public function buildWorkspace($workspace_name) {
		return str_replace('{workspace_name}', $workspace_name, '<workspace><name>{workspace_name}</name></workspace>');
	}
	
	static public function buildDataStore($workspace_name, $store_name, $file_url, $enabled = true) {
		$search = array('{workspace_name}', '{store_name}', '{file_url}', '{enabled}');
		$replace = array(htmlspecialchars($workspace_name),
						htmlspecialchars($store_name),
						htmlspecialchars($file_url),
						$enabled ? 'true' : 'false');
		return str_replace($search, $replace, '<dataStore>
		<name>{store_name}</name>
		<enabled>true</enabled>
			<workspace>
				<name>{workspace_name}</name>
			</workspace>
			<connectionParameters>
				<entry key="url">{file_url}</entry>
				<entry key="namespace">http://{workspace_name}</entry>
			</connectionParameters>
		</dataStore>');
	}
	
	static public function buildFeatureType($layer_name, $store_name, $feature_name, $enabled = true) {
		$search = array('{layer_name}', '{feature_name}', '{store_name}', '{enabled}');
		$replace = array(htmlspecialchars($layer_name),
						htmlspecialchars($feature_name),
						htmlspecialchars($store_name),
						$enabled ? 'true' : 'false');
		return str_replace($search, $replace, '<featureType>
  <name>{layer_name}</name>
  <nativeName>{feature_name}</nativeName>
  <title>{layer_name}</title>
  <enabled>{enabled}</enabled>
  <store class="dataStore">
    <name>{store_name}</name>
  </store>
</featureType>');
	}
	
	static public function buildCoverageStore($name, $workspace_name, $relative_filename, $type = 'GeoTIFF', $enabled = true)
	{
		$search = array('{name}', '{workspace_name}', '{relative_filename}', '{type}', '{enabled}');
		$replace = array(htmlspecialchars($name),
						htmlspecialchars($workspace_name),
						htmlspecialchars($relative_filename),
						htmlspecialchars($type),
						$enabled ? 'true' : 'false');
		return str_replace($search, $replace, '<coverageStore>
  <name>{name}</name>
  <type>{type}</type>
  <enabled>{enabled}</enabled>
  <workspace>
    <name>{workspace_name}</name>
  </workspace>
  <url>file:{relative_filename}</url>
</coverageStore>');
	}

	static public function buildCoverage($name, $workspace_name, $coverage_store_name, array $options = array(),$parameters=null,$dimensions=null)
	{
		$search = array('{name}', '{workspace_name}', '{coverage_store_name}');
		$replace = array(htmlspecialchars($name), htmlspecialchars($workspace_name), htmlspecialchars($coverage_store_name));

		foreach ($options as $k => $v)
		{
			$search[] = '{' . $k . '}';
			$replace[] = htmlspecialchars($v);
		}

		//Add optional parameters if defined:
		$parameters_xml='';
		if (is_array($parameters) && count($parameters)>0){
			$parameters_xml.='<parameters>';
			foreach ($parameters as $entry_name => $entry_value)
				$parameters_xml.="<entry>
	<string>$entry_name</string>
	<string>$entry_value</string>
</entry>";
			$parameters_xml.='</parameters>';			
		}
		
		//Add dimensions if defined:
		$metadata_xml='';
		if (is_array($dimensions) && count($dimensions)>0){
			foreach ($dimensions as $dimension_name => $dimension_value)
				$metadata_xml='<metadata>
		    <entry key="'.$dimension_name.'">
		      <dimensionInfo>
		        <enabled>true</enabled>
		        <presentation>'.$dimension_value['presentation'].'</presentation>
		        <units>'.$dimension_value['units'].'</units>
		       <defaultValue>
		          <strategy>'.$dimension_value['strategy'].'</strategy>
		        </defaultValue>
		      </dimensionInfo>
		    </entry>
		  </metadata>';
  		}
		
		return str_replace($search, $replace, '<coverage>
  <name>{name}</name>
  <namespace>
    <name>{workspace_name}</name>
  </namespace>
  <title>{name}</title>
  <description>{description}</description>
  <nativeCRS>{crs}</nativeCRS>
  <srs>{srs}</srs>
  <nativeBoundingBox>
    <minx>{minx}</minx>
    <maxx>{maxx}</maxx>
    <miny>{miny}</miny>
    <maxy>{maxy}</maxy>
    <crs>{srs}</crs>
  </nativeBoundingBox>
  <latLonBoundingBox>
    <minx>{minx}</minx>
    <maxx>{maxx}</maxx>
    <miny>{miny}</miny>
    <maxy>{maxy}</maxy>
    <crs>{srs}</crs>
  </latLonBoundingBox>
  <projectionPolicy>REPROJECT_TO_DECLARED</projectionPolicy>
  <enabled>true</enabled>'.
	$metadata_xml
  .'<store class="coverageStore">
    <name>{coverage_store_name}</name>
  </store>
  <nativeFormat>{format}</nativeFormat>
  <grid dimension="2">
    <range>
      <low>0 0</low>
      <high>{size_x} {size_y}</high>
    </range>
    <transform>
      <scaleX>1.0</scaleX>
      <scaleY>1.0</scaleY>
      <shearX>0.0</shearX>
      <shearX>0.0</shearX>
      <translateX>{maxx}</translateX>
      <translateY>{maxy}</translateY>
    </transform>
    <crs>{srs}</crs>
  </grid>
  <supportedFormats>
    <string>GIF</string>
    <string>PNG</string>
    <string>JPEG</string>
    <string>TIFF</string>
    <string>GEOTIFF</string>
  </supportedFormats>
  <interpolationMethods>
    <string>bilinear</string>
    <string>bicubic</string>
  </interpolationMethods>
  <dimensions>
    <coverageDimension>
      <name>GRAY_INDEX</name>
      <description>GridSampleDimension[-Infinity,Infinity]</description>
    </coverageDimension>
  </dimensions>
  <requestSRS>
    <string>{srs}</string>
  </requestSRS>
  <responseSRS>
    <string>{srs}</string>
  </responseSRS>
  '.$parameters_xml.'
</coverage>');
	}

	static public function buildCoverageMosaic($name, $workspace_name, $coverage_store_name, $parameters=null,$dimensions=null)
	{
		//Add optional parameters if defined:
		$parameters_xml='';
		if (is_array($parameters) && count($parameters)>0){
			$parameters_xml.='<parameters>';
			foreach ($parameters as $entry_name => $entry_value)
				$parameters_xml.="<entry>
				<string>$entry_name</string>
				<string>$entry_value</string>
				</entry>";
				$parameters_xml.='</parameters>';
		}
		
		$metadata_xml='';
		if (is_array($dimensions) && count($dimensions)>0){
			foreach ($dimensions as $dimension_name => $dimension_value)
				$metadata_xml='<metadata>
		    <entry key="'.$dimension_name.'">
		      <dimensionInfo>
		        <enabled>true</enabled>
		        <presentation>'.$dimension_value['presentation'].'</presentation>
		        <units>'.$dimension_value['units'].'</units>
		       <defaultValue>
		          <strategy>'.$dimension_value['strategy'].'</strategy>
		        </defaultValue>
		      </dimensionInfo>
		    </entry>
		  </metadata>';
		}
		return "<coverage>
  <name>{$name}</name>
  <enabled>true</enabled>
  {$metadata_xml}
  {$parameters_xml}
</coverage>";
	}
	
    static public function buildLayerGroup($name, $layers) {
                $search = array('{name}');
                $replace = array(htmlspecialchars($name));
                if(!is_array($layers)) {
                        $layers=array($layers);
                }
                foreach($layers as $layer){
                        $search_layer = array('{layername}');
                        $replace_layer = array(htmlspecialchars($layer));
                        $layerpart.=str_replace($search_layer, $replace_layer,'<layer>{layername}</layer>');
                        $stylepart.='<style/>';
                }
                return str_replace($search, $replace, "<layerGroup>
                        <name>{name}</name>
						<mode>NAMED</mode>
                        <layers>
                                $layerpart
                        </layers>
                        <styles>
                        $stylepart
                        </styles>
                </layerGroup>");
	}


}

class acs_rest_geoserver_customized extends acs_rest{
	
	protected function checkResponse($response, $ret)
	{
		
		if ($response['http_code'] == 0)
			throw new acs_exFailed("Cannot contact url {$response['url']}: ".print_r($ret,true));
		$ret_category = (int)($response['http_code'] / 100);
		if ($ret_category == 5 && strstr($ret,'already exists'))
			throw new acs_geoAlreadyExists("REST error {$response['http_code']} for url {$response['url']}: ".print_r($ret,true));
		if ($ret_category == 4 || $ret_category == 5)
			throw new acs_exFailed("REST error {$response['http_code']} for url {$response['url']}: ".print_r($ret,true));
		if ($ret_category == 2 && $this->_logHandler)
			$this->_logHandler->debug("REST: ".print_r($ret,true));
	}	
	
}

class acs_geoAlreadyExists extends acs_exception{}
?>
