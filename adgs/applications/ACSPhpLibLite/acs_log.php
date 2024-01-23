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
// $Id: acs_log.php,v 1.22 2015/09/30 09:01:04 andema Exp $
// $Revision: 1.22 $
//
// PEAR includes
require_once 'Log.php';
require_once 'Log/composite.php';
require_once 'Log/file.php';
// Interface include
require_once 'interfaces/acs_ilog.php';

// Acs includes
require_once 'ACSPhpLib/acs_log_utils.php';

/**
 * ACS Log file object
 * @package ACSPhpLib
 */
class acs_log implements acs_ilog {
	private $_inivars= null;
	private $_logHandler= null;
	/**
	 * @package ACSPhpLib
	 * @param mixed $inivars array containing the configuration settings. Expects a section (sub array) named 'log'
	 * @param string $ident string that will be printed into the log, identifies the emitter
	 * @param bool $isWebInterface true only if called from a web page, format errors in html
	 */
	function __construct(& $inivars, $ident= '', $isWebInterface= false) {
		$this->_inivars= $inivars;
		$conf= array (
			'mode' => 0666,
			'timeFormat' => '%X %x',
			'lineFormat' => '%1$s [%2$s] [%3$s] %4$s'
		);
		if ($this->_inivars['log']['log_overwrite']){
			$conf['append']= false;
		}
		$log_prefix= $this->_inivars['log']['log_name_prefix'];
		if (is_null($log_prefix)){
			$log_prefix= '';
		}
		$LogFname= sprintf('%s/%s%s.log', $this->_inivars['log']['log_path'], $log_prefix, strftime("%Y-%m-%d", time()));
		$this->_logHandler= & Log :: singleton('acs_composite');
		$mask= '';
		eval ("\$mask=" . $this->_inivars['log']['log_level'] . ";");
		if (array_key_exists('log_to_file', $this->_inivars['log']) && 
			$this->_inivars['log']['log_to_file']) {
			if (array_key_exists('filepermissions', $this->_inivars['log']) && 
				$this->_inivars['log']['filepermissions']) $conf['mode'] = intval($this->_inivars['log']['filepermissions'],010);
			if (array_key_exists('dirpermissions', $this->_inivars['log']) &&
				$this->_inivars['log']['dirpermissions']) $conf['dirmode'] = intval($this->_inivars['log']['dirpermissions'],010);
			$file_log= & Log :: singleton('file', $LogFname, $ident, $conf);
			$file_log->setMask($mask);
			$this->_logHandler->addChild($file_log);
		}

		if ($this->_inivars['log']['log_to_screen']){
			if(!$isWebInterface || $this->_inivars['log']['log_for_web']) {
				$log_color=(array_key_exists('log_color',$this->_inivars['log']) ? $this->_inivars['log']['log_color'] : '#ff0000' );
				$conf= array (
					'error_prepend' => '<font color="'.$log_color.'"><tt>',
					'error_append' => '</tt></font>'
				);
			}
			
			$display_log= & Log :: singleton('display', '', $ident, $conf, PEAR_LOG_DEBUG);
			$display_log->setMask($mask);
			$this->_logHandler->addChild($display_log);
		}
		if( ( array_key_exists('log_to_mcf',$this->_inivars['log'] ) and  $this->_inivars['log']['log_to_mcf']  ) ) {
			if( array_key_exists('log_to_mcf_db_section',$this->_inivars['log']) and array_key_exists('log_to_mcf_db_section',$this->_inivars)) {
				$mcf_conf = $this->_inivars['log_to_mcf_db_section'];
				
			} else{
				$mcf_conf = array(
					'db'=> array(
						'type'    =>array_key_exists('log_to_mcf_dbtype',$this->_inivars['log'])? $this->_inivars['log']['log_to_mcf_dbtype'] : 'mysql',
								'user'    =>$this->_inivars['log']['log_to_mcf_user'],
								'password'=>$this->_inivars['log']['log_to_mcf_password'],
								'host'    =>$this->_inivars['log']['log_to_mcf_host'],
								'db'      =>array_key_exists('log_to_mcf_db', $this->_inivars['log'])   ? $this->_inivars['log']['log_to_mcf_db']   : 'mcflocal',
										'port'    =>array_key_exists('log_to_mcf_port', $this->_inivars['log']) ? $this->_inivars['log']['log_to_mcf_port'] : 3306,
					)
				);
			}
			
			$mcf_logger= & Log :: singleton('mcf', $appname=$ident, $ident, $mcf_conf);
			$mcf_logger->setMask($mask);
			$this->_logHandler->addChild($mcf_logger);
		}
		
		if (array_key_exists('extra_log', $this->_inivars))
		{
			// loggers can be specified in ini file using multiple values of the key loggers in log section
			// e.g.
			// [extra_log]
			//  log_err_email = On
			//  log_mcf = On
			//  log_info_email = On
			
			// include log drivers
			acs_log_utils::includeLogDrivers();
			
			foreach ($this->_inivars['extra_log'] as $logger => $logger_enabled)
			{
				if (! $logger_enabled)
					continue;
				
				$conf = $this->_inivars[$logger];
				if (!is_array($conf)) $conf = array();
				
				$conf['main_conf'] = $this->_inivars;
				
				// get the type of the new logger object
				if (!isset($conf['type']))
					throw new acs_exLogError("Define a 'type' for extra logger section '$logger'");
				
				if (isset($conf['source']))
					require_once $conf['source'];
				
				// create the logger instance
				$log_obj = & Log::singleton($conf['type'], $conf['name'], $ident, $conf);
				if (!is_object($log_obj))
					throw new acs_exFailed("Cannot create logger of type $type");
				
				// set the logger mask (inherit from [log] section if missing
				$mask_code = array_key_exists('log_level', $conf) ? 
						$conf['log_level'] : $this->_inivars['log']['log_level'];
						eval ("\$mask=$mask_code;");
						$log_obj->setMask($mask);
						
						// add the logger as a child of the main composite logger object
						if (!$this->_logHandler->addChild($log_obj))
							throw new acs_exLogError("Cannot add child of type $type to composite logger");
			}
		}
		
		if ($this->_logHandler === null){
			throw new acs_exLogError(__FUNCTION__ . " " . __LINE__ . " : Could not setup log!");
		}
	}
	/**
	 * returns the log handler that will be used to write into the log
	 */
	public function & getLoghandler() {
		return $this->_logHandler;
	}
}
?>
