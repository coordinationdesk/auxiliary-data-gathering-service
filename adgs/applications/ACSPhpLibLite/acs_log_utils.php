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
// $Prod: ACSPhpLib $  
// $Author: andema $
// $Id: acs_log_utils.php,v 1.3 2012/09/07 07:42:19 andema Exp $
// $Revision: 1.3 $
//

require_once 'ACSPhpLib/acs_tools.php';

//
// This class is automatically used by acs_log to store multiple instances
// of logger objects. It implements the method state to communicate
// system variables status change to external systems (like the mcf).
//
class log_acs_composite extends Log_composite {
	
	protected $_additional_info = array();
	
	public function setAdditionalInfo(array $additional_info) {
		$this->_additional_info = $additional_info;
	}
	
	public function updateAdditionalInfo(array $additional_info) {
		$this->_additional_info = array_merge($this->_additional_info, $additional_info);
	}
	
	public function log($message, $priority = null) {
		if (count($this->_additional_info) > 0) {
			$str = array();
			foreach ($this->_additional_info as $k => $v)
				$str[] = "$k=>$v";
			$message = '(' . join(',', $str) . ') ' . $message;
		}
		parent::log($message, $priority);
	}
	
	public function state(array $registers)
    {
        if (!$this->_opened)
            $this->open();
        $success = true;
        foreach ($this->_children as $id => $child) {
        	if (method_exists($this->_children[$id], 'state'))
        		$success &= $this->_children[$id]->state($registers);
        	else
            	$success &= $this->_children[$id]->log("Monitoring system variable change: " . print_r($registers, true), PEAR_LOG_DEBUG);
        }

        //$this->_announce(array('priority' => $priority, 'message' => $message));

        /* Return success if all of the children logged the event. */
        return $success;
	}
}

//
// This is the base class for the log drivers
//
class acs_log_driver extends Log {

	function __construct($name, $ident= '', $conf= array (), $level= PEAR_LOG_DEBUG) {
		$this->_conf = $conf;
		$this->_ident = $ident;
		$this->_appname = $name;
		$this->_priority = $level;
		
		$this->_id = md5(microtime());
	}

	/**
	 * Destructor. Calls close().
	 *
	 * @access private
	 */
	function __destruct() {
		$this->close();
	}
	/**
	* Starts a new mail message.
	* This is implicitly called by log(), if necessary.
	*
	* @access public
	*/
	function open() {
		$this->_opened= true;
		return true;
	}
	/**
	* Closes the display handler.
	*
	* @access  public
	* @since   Log 1.9.6
	*/
	function close() {
		$this->_opened= false;
		return true;
	}
	
	// Key method to reimplement. This method is called when a
	// log message must be traced
	function log($message, $priority= null) {
		throw new Exception("Override " . __METHOD__ . " in your base class");
	}
	
	function wrapMessage($message)
	{
		if(is_array($this->_conf) &&
           array_key_exists('userErrorList',$this->_conf) &&
           $this->_conf['userErrorList']){
            require_once 'ACSPhpLib/acs_tools.php';
            $erroListFile = $this->_conf['userErrorList'];

            $message = acs_tools::replaceStringFromResourceFile($message,$erroListFile,$this->_conf['application']['language']);
        }
        return $message;
	}
}

//
// General utilities
//
class acs_log_utils {
	static public function getLogDriversDir() {
		return dirname(__FILE__) . '/drivers/log/';
	}
	
	static public function getLogDrivers() {
		if (!is_dir(self::getLogDriversDir()))
			return array();
		
		$srcs = acs_directory::getContents(self::getLogDriversDir(), '/^.*.php$/');
		if (is_array($srcs))
			return $srcs;
		else
			return array();
	}

	static public function includeLogDrivers() {
		foreach (self::getLogDrivers() as $src)
			require_once($src['full']);
	}
}

?>
