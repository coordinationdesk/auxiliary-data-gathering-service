<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2010 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.)    |
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
// $Prod:  $
// $Author$
// $Id$
// $Revision$
//

require_once 'ACSPhpLib/acs_servlet.php';

class acs_servlet_ext_exception extends Exception {}
class acs_servlet_ext_ex_bad_configuration extends Exception {}
class acs_servlet_ext_ex_bad_request extends acs_servlet_ext_exception {}

abstract class acs_servlet_ext extends acs_servlet {
	
	public function __construct($iniFileName, $ident = __METHOD__) {
		parent :: __construct($iniFileName, $ident, true);
		
		set_error_handler(array($this, 'onError'), E_ALL & ~E_NOTICE & ~E_STRICT & ~E_DEPRECATED);
	}
	
	public function getRequestIniSectionName() {
		return 'requests';
	}
	
	public function getRequestConfiguration() {
		$ini = $this->getIniVars();
		$sectionName = $this->getRequestIniSectionName();
		if (!array_key_exists($sectionName, $ini)) {
			throw new acs_servlet_ext_ex_bad_configuration("Section {$sectionName} not found in config");
		}
		return @$ini[$sectionName];
	}
	
	/**
	 * @param string $request
	 * @return acs_servlet_ext_request
	 * @throws acs_servlet_ext_ex_bad_request
	 */
	public function createRequestHandler($request) {
		$requestConf = $this->getRequestConfiguration();
	
		if (!array_key_exists($request, $requestConf))
			throw new acs_servlet_ext_ex_bad_request("Unknown request: " . $request);
	
		$requestClass = $requestConf[$request];
		if (! $requestClass) {
			throw new acs_servlet_ext_ex_bad_request("Cannot access to disabled request: " . $request);
		} else {
			list($class, $source) = explode('@', $requestClass);

			if (!class_exists($class)) {
				if (!$source)
					throw new acs_servlet_ext_ex_bad_configuration("Specify class file for {$class}");
	
				require_once($source);
			}

			return new $class($this);
		}
	}
	
	/**
	 * @return array strings of configured and enabled requests
	 */
	public function getConfiguredRequests() {
		$reqs = array();
		foreach ($this->getRequestConfiguration() as $request => $conf) {
			if ($conf)
				$reqs[] = $request;
		}
		return $reqs;
	}
	
	public function getRequiredParams() {
		return array('request');
	}

	public function getRequestName() {
		return $_REQUEST['request'];
	}
	
	public function getRequestVars() {
		return $_REQUEST;
	}
	
	public function getPostPayload() {
		return file_get_contents('php://input');
	}
	
	public function parse() {
		try {
			$rh = $this->createRequestHandler($this->getRequestName());
			$resp = $rh->perform($this->getRequestVars());
			
			$resp->output();
		} catch (Exception $e) {
			$this->onException($e);
		}
	}

	protected function getErrorLevel($errlevel) {
		static $levelMap = array(
				E_ERROR => 'err',
				E_USER_ERROR => 'err',
				E_WARNING => 'warning',
				E_USER_WARNING => 'warning',
		);
		$method = @$levelMap[$errno];
		if (!$method) {
			$method = 'err';
		}
		return $method;
	}
	
	protected function logFromErrorCallback($errlevel, $errstr) {
		$method = $this->getErrorLevel($errlevel);
		$this->_logHandler->$method(strtoupper($method) . ': ' . $errstr);
	}
	
	protected function logFromExceptionCallback(Exception $e) {
		$method = $this->getErrorLevel(E_ERROR);
		$this->_logHandler->$method(
				sprintf("%s, %s in %s:%d\n%s",
						$e->getMessage(),
						get_class($e), 
						$e->getFile(), 
						$e->getLine(), 
						$e->getTraceAsString()
						));
	}
	
	abstract protected function getErrorResponse($errno, $errstr, $errfile, $errline, array $errcontext = array());
	
	protected function getExceptionResponse(Exception $e) {
		return $this->getErrorResponse(E_ERROR, $e->getMessage(), $e->getFile(), $e->getLine());
	}
	
	public function onError($errno, $errstr, $errfile, $errline, array $errcontext = array()) {
		$this->logFromErrorCallback($errno, $errstr);

		$response = $this->getErrorResponse($errno, $errstr, $errfile, $errline);
		$response->output();
		die;
	}
	
	public function onException(Exception $e) {
		$this->logFromExceptionCallback($e);
	
		$response = $this->getExceptionResponse($e);
		$response->output();
		die;
	}
}

class acs_servlet_ext_response {

	/**
	 * @var mixed
	 */
	protected $payload;
	
	/**
	 * @var array
	 */
	protected $extraHeaders;
	
	/**
	 * @var array
	 */
	protected $headers = array('Content-Type: text/html');

	public function __construct($payload, array $extraHeaders = array()) {
		$this->payload = $payload;
		$this->extraHeaders = $extraHeaders;
	}

	public function getOutput() {
		return $this->payload;
	}

	public function getHeaders() {
		return array_merge($this->headers, $this->extraHeaders);
	}

	public function output() {
		foreach ($this->getHeaders() as $header) {
			header($header);
		}
		echo $this->getOutput();
	}
}

class acs_servlet_ext_response_json extends acs_servlet_ext_response {
	protected $headers = array('Content-Type: application/json');
	
	public function getOutput() {
		return json_encode($this->payload);
	}
}

class acs_servlet_ext_response_xml extends acs_servlet_ext_response {
	protected $headers = array('Content-Type: application/xml');

	public function getOutput() {
		return $this->payload->saveXml();
	}
}

class acs_servlet_ext_usage {
	public $sampleCall;
	public $arguments;
	public $description;
	public $notes;
}

abstract class acs_servlet_ext_request {
	protected $body = false;
	
	/**
	 * 
	 * @var acs_servlet_ext $theServlet
	 */
	protected $theServlet;
	
	public function __construct(acs_servlet_ext $theServlet) {
		$this->theServlet = $theServlet;
	}
	
	/**
	 * @param array $requestVars
	 * @return acs_servlet_ext_response
	 */
	abstract public function perform($requestVars);

	/**
	 * Used in the "help" request
	 * @return acs_servlet_ext_usage
	 */
	abstract public function getUsage();
	
	public function getHttpBody() {
		if ($this->body === false) {
			$this->body = $this->theServlet->getPostPayload();
		}
		return $this->body;
	}
}

abstract class acs_servlet_ext_request_help extends acs_servlet_ext_request {
	
	public function getRequestsUsage() {
		$usages = array();
		foreach ($this->theServlet->getConfiguredRequests() as $reqName) {
			$request = $this->theServlet->createRequestHandler($reqName);
			$usages[$reqName] = $request->getUsage();
		}
		return $usages;
	}
}

class acs_servlet_ext_rest_method_not_implemented extends acs_servlet_ext_exception {}

abstract class acs_servlet_ext_request_rest extends acs_servlet_ext_request {
	public function perform($requestVars) {
		switch ($_SERVER['REQUEST_METHOD']) {
			case 'GET':
				return $this->handleGet($requestVars);
			case 'POST':
				return $this->handlePost($requestVars);
			case 'PUT':
				return $this->handlePut($requestVars);
			case 'DELETE':
				return $this->handleDelete($requestVars);
			case 'HEAD':
				return $this->handleHead($requestVars);
			case 'PATCH':
				return $this->handlePatch($requestVars);
			case 'OPTIONS':
				return $this->handleOptions($requestVars);
			default:
				throw new acs_servlet_ext_ex_bad_request("Unknown request method: {$_SERVER['REQUEST_METHOD']}");
		}
	}
	
	public function handleGet($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handlePost($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handlePut($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handleDelete($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handleHead($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handlePatch($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
	
	public function handleOptions($requestVars) {
		throw new acs_servlet_ext_rest_method_not_implemented("method not implemented");
	}
}

?>