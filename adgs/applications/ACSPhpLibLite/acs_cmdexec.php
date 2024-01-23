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
// $Author: seravo $
// $Id: acs_cmdexec.php,v 1.3 2012/06/07 15:22:16 seravo Exp $
// $Revision: 1.3 $
//

require_once 'ACSPhpLib/acs_file.php';

/**
 * Class used to run a command as a sub-process
 * 
 * @author andrea
 *
 */
class acs_cmdexec {
	protected $h, $cmd, $stdout, $stderr, $stdout_offs, $stderr_offs;
	
	/**
	 * Constructor tells the object which command must be executed
	 * 
	 * @param string $cmd		the command to be launched
	 */
	public function __construct($cmd) {
		$this->stdout = acs_file::getUniqueFileName(true) . ".out";
		$this->stderr = acs_file::getUniqueFileName(true) . ".err";
		
		$stdout = escapeshellarg($this->stdout);
		$stderr = escapeshellarg($this->stderr);
		
		$this->cmd = "$cmd 1>> {$stdout} 2>> {$stderr}";
	}
	
	public function __destruct() {
		if ($this->h)
			pclose($this->h);
		@unlink($this->stdout);
		@unlink($this->stderr);
	}
	
	/**
	 * Execute the command in foreground or background
	 * 
	 * @param boolean $background	true if the command must be launched as a background process
	 * 
	 * @return number	if launched in foreground, the return code of the command
	 */
	public function execute($background = false) {
		if ($background) {
			$this->h = popen($this->cmd, 'w');
			return null;
		} else {
			$null = array(); $retval = 0;
			exec($this->cmd, $null, $retval);
			return $retval;
		}
	}
	
	/**
	 * Sends a string to the sub-process (if launched in background mode)
	 * 
	 * @param string $string	string to be sent to the background process
	 */
	public function putStdin($string) {
		if ($this->h)
			fwrite($this->h, $string);
	}
	
	protected function getOutput($f, &$offset) {
		$out = file_get_contents($f, false, null, $offset);
		$offset += strlen($out);
		return $out;
	}
	
	/**
	 * Get the standard output of the launched process
	 * 
	 * @return string	the last not-read trunk of the stdout
	 */
	public function getStdout() {
		return $this->getOutput($this->stdout, $this->stdout_offs);
	}

	/**
	 * Get the standard error of the launched process
	 *
	 * @return string	the last not-read trunk of the stderr
	 */
	public function getStderr() {
		return $this->getOutput($this->stderr, $this->stderr_offs);
	}
	
	/**
	 * Rewinds the stdout to the beginning
	 */
	public function rewindStdout() {
		$this->stdout_offs = 0;
	}
	
	/**
	 * Rewinds the stderr to the beginning
	 */
	public function rewindStderr() {
		$this->stderr_offs = 0;
	}
}

?>