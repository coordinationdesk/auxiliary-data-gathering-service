<?php
/* vim: set expandtab tabstop=4 shiftwidth=4 softtabstop=4 fdm=marker: */ 
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
// $Author: serabb $
// $Id: acs_file.php,v 1.75 2016/09/06 11:17:03 serabb Exp $
// $Revision: 1.75 $
//
require_once 'ACSPhpLib/acs_exception.php';
require_once 'ACSPhpLib/acs_tools.php';
require_once 'Config.php';

define('DEFAULT_GLOBAL_CONFIG', '/usr/acs/conf/acs_global_config.ini');
define('ACSPHPLIB_CONF', '/etc/sysconfig/acsphplib');
define('ACSPHPLIB_GLOBAL_CONFIG_PATH', 'ACS_GLOBAL_CONFIG_PATH');
define('ACSPHPLIB_TEMPDIR', '/etc/sysconfig/acstempdir');
define('ACSPHPLIB_TEMPDIR_PATH', 'ACS_TEMPDIR_PATH');

/**
 * @package ACSPhpLib
 */
class acs_file {
	private static $_preliminaryHeaderSent=false;
	
    /**
     * return file extension
     * @param string $filename Filename whose extension is to be determined
     */
    static public function getFileExtension($filename) {
        return substr($filename, strrpos($filename, '.'));
    }
    /**
     * creates directory tree recursively
     *
     * @param string strPath path to be created
     * @param integer mode mode (dec) of the pat to be created
     * @return bool returns true if the path was created successfully
     */
    static public function makeDirs($strPath, $mode= 0777) {
        return is_dir($strPath) or (mkdir($strPath, $mode, true) and chmod($strPath, $mode));
    }
    
    
    static public function recursive_chgrp($dirname,$group){
    	if(!file_exists($dirname)) return false;
        chgrp($dirname,$group);
        if(is_dir($dirname)) {
        	$dir_contents=acs_directory::getContents($dirname,null,null,false);
            foreach($dir_contents as $item)
            	self::recursive_chgrp($item['full'],$group);
        }

	}

    static public function recursive_chmod($dirname,$mode='0775',$modefile='0664'){
    	if(!file_exists($dirname)) return false;
        $mode_8=octdec($mode);
        chmod($dirname,$mode_8);
        if(is_dir($dirname)) {
        	$dir_contents=acs_directory::getContents($dirname,null,null,false);
            foreach($dir_contents as $item)
            	if ($item['type']=='file') {
            		self::recursive_chmod($item['full'],$modefile);
            	} else {
            		self::recursive_chmod($item['full'],$mode);	
            	}            	
        }
	}
    
    /**
     * Checks if the configuration file exists then
     * Parses and returns the configuration file.
     * @return misc/array array containing configuration parameters
     */
    /**
    *
    * load a configuration file in different formats

    * @link http://pear.php.net/Config See config for further description.
    * @param string $source path of the file to be read
    * @param string $type one of inifile, IniCommented, xml, apache, array ...
    * @param array $options parameters for file reader, changes according to the choosen type.
    */
    public static function getINIFile($fname, $type= 'Inifile', $options= array (), $rootName= null, $withDbg=false) {
        if ($fname === null) {
            throw new acs_exINIError(__METHOD__ . " : INI File Name not set !");
        }
        if (!file_exists($fname)) {
            throw new acs_exINIError(__METHOD__ . " : Cannot read configuration file : $fname");
        }
        
        if ($type == 'Inifile') {
        	// significally improved performance by reading config file by php primitive function
        	$config = parse_ini_file($fname, true);
        	
        	// handle comma separated values as array, for compatibility with PEAR::Config
        	foreach ($config as $section => $keys) {
        		foreach ($keys as $key => $value) {
        			if (strpos($value, '"') === false && strpos($value, ", ") !== false) {
        				$values = preg_split('/\s*,\s+/', $value);
        				if (count($values) > 1)
        					$config[$section][$key] = $values;
        			}
        		}
        	}
        } else {
        	// old processing, through PEAR module
	        $conf= new Config;
	        $root= & $conf->parseConfig($fname, $type, $options, $rootName);
	        if (PEAR :: isError($root)) {
	            throw new acs_exINIError(__METHOD__ . " : " . $root->getMessage());
	        }
	        $config= $root->toArray();
	        if (!is_null($rootName)) {
	            $config= $config['root'][$rootName];
	            // echo"<pre>";print_r($config);echo"</pre>";
	        }
	        else {
	            $config= $config['root'];
	        }
        }
        
        if (array_key_exists('include', $config)) {
        	foreach ($config['include'] as $includeFile) {
        		try {
        			$includedConfig = self::getINIFile($includeFile, $type, $options, $rootName, $withDbg);
        		} catch (acs_exINIError $e) {
        			throw new acs_exINIError("Error parsing {$fname}: " . $e->getMessage());
        		}
        		$config = acs_tools::array_overlay($includedConfig, $config);
        	}
        }

        // Parse overlay
        $iniExtFileName= dirname($fname) . '/ext_' . basename($fname);
        if (file_exists($iniExtFileName)) {
            $extIniAr= self :: getINIFile($iniExtFileName, $type, $options, $rootName);
            $config= acs_tools :: array_overlay($config, $extIniAr);
        }

        if($withDbg) {
            $iniDbgFileName= dirname($fname) . '/dbg_' . basename($fname);
            if (file_exists($iniDbgFileName)) {
                $dbgIniAr= self :: getINIFile($iniDbgFileName, $type, $options, $rootName);
                $config= acs_tools :: array_overlay($config, $dbgIniAr);
            }
        }
        return $config;
    }
    
    public static function resolveTokens($iniVars, $tokensIniFileName = null) {
    	$newIniVars = $iniVars;
    	// now get tokens from acs_global_config.ini if exists...
   		$acsGlobalConfigVar = self::getGlobalConfig('tokens', $tokensIniFileName);
    	if(count($acsGlobalConfigVar)>0){
    		foreach($iniVars as $sectionname=>$section){
    			foreach($section as $k=>$v){
    				/*
    				foreach($acsGlobalConfigVar as $tokenKey=>$tokenValue){
    					$tok = '<'.$tokenKey.'>';
    					if ($v != null && is_string($v) && strpos($v, $tok) !== false)
    						$newIniVars[$sectionname][$k]=str_replace('<'.$tokenKey.'>',$tokenValue,$newIniVars[$sectionname][$k]);
    				}
    				/*
    				/**/
    				if (is_string($v) && strpos($v, '<') !== false && strpos($v, '>') !== false) {
    					$m = array();
    					if (preg_match_all('/<(.*)>/U', $v, $m)) {
    						$substitute = array();
    						foreach ($m[1] as $idx => $key) {
    							$substitute[] = array_key_exists($key, $acsGlobalConfigVar) ? $acsGlobalConfigVar[$key] : $m[0][$idx];
    						}
    						$newIniVars[$sectionname][$k] = str_replace($m[0], $substitute, $v);
    					}
    				}/**/
    			}
    		}
    	}
    	return $newIniVars;
    }
    
    static public function setConfigKeyValues($fname, $sectionname, $keyvalues, $type= 'Inifile') {
        $conf= new Config();
        $section= false;
        // load up dbg config file if exists
        if (file_exists($fname)) {
            $root= & $conf->parseConfig($fname, $type);
            if (PEAR :: isError($root)) {
                throw new acs_exINIError(__METHOD__ . " : " . $root->getMessage());
            }
            $section= & $root->getItem('section', $sectionname);
        }
        else
            $root= & $conf->getRoot();
        if ($section === false) {
            // if the section doesn't exist....'
            $section= & $root->createSection($sectionname);
        }
        foreach ($keyvalues as $k => $v)
            $section->setDirective($k, $v);
        // save the new config file
        $success= $conf->writeConfig($fname, $type);
        if (PEAR :: isError($success)) {
            throw new acs_exINIError(__METHOD__ . " : " . $root->getMessage());
        }
    }
    static public function getDirList($path, $withdir= false, $nohidden= true) {
        $filesAr= array ();
        if (is_dir($path)) {
            $handle= @ opendir($path);
            while (false !== ($filename= readdir($handle))) {
                if ($filename == '.' or $filename == '..') {
                    continue;
                }
                if (($nohidden) && ($filename[0] == '.')) {
                    continue;
                }
                if (!is_readable($path . "/" . $filename)) {
                    continue;
                    //throw new acs_exFileError("Cannot read $path/$filename");
                }
                if (is_dir("$path/$filename")) {
                    if ($withdir)
                        $filesAr[]= $filename;
                }
                else
                    $filesAr[]= $filename;
            }
            closedir($handle);
            return $filesAr;
        }
        else {
            throw new acs_exFileError("Path $path not found !");
        }
    }
    /**
     * Check if the file is being written by some other applications
     * @param string $fileName file to be checked
     * @param integer $timeout time spent observing if the file changes (milliseconds)
     * @param integer $num How many observation before the file is assumed "stable"
     * @return bool true if the file is "stable"
     */
    static public function isStable($fileName, $timeout= 100, $num= 3) {
        if (!is_file($fileName)) {
            throw new acs_exFileError("$fileName does not exist.");
        }
        $start= microtime(true);
        $isStable= false;
        $timedout= false;
        $lastStatus['size']= 0;
        /**
         * @internal The mtime--modify time--is the time when the actual contents of a file was last modified. This is the time displayed in a long directoring listing (ls -l).
         */
        $lastStatus['mtime']= 0;
        $now= microtime(true);
        $timedout= (($now - $start) > floatval($timeout));
        while (($isStable === false) and ($timedout === false) and ($num > 0)) {
            /**
             * @internal Get File Stats
             */
            $status= stat($fileName);
            // Compare with last run
            if (($status['size'] == $lastStatus['size']) and ($status['mtime'] == $lastStatus['mtime'])) {
                /**
                 * @internal Ok seems to be stable, decrement number of "successful observation"
                 */
                $lastStatus['size']= $status['size'];
                $lastStatus['mtime']= $status['mtime'];
                $num= $num -1; // Decrement counter of successful observation
                $isStable= true;
            }
            $lastStatus['mtime']= $status['mtime'];
            usleep(1);
            clearstatcache();
            $now= microtime(true);
            $timedout= (($now - $start) > floatval($timeout));
            //print "Now=$now, Started at $start will timeout at ".($start+$timeout)." Timedout=".($timedout?"true":"false")." HERE:".( $now-$start)." T=".floatval($timeout)."\n";
        }
        if ($timedout) {
            throw new acs_exFileError("$filename is being modified.");
        }
        return $isStable;
    }
    static public function _checkFile($file) {
        if (file_exists($file) == false) {
            throw new acs_exFileError(__METHOD__ . " : $file not found.");
        }
        if (!is_readable($file)) {
            throw new acs_exFileError(__METHOD__ . " : $file not readable.");
        }
    }
    /**
     * @TODO : fixme .. this doesn't seem to be working
     */
    static public function correctPath($path) {
        $path .= '/';
        $path= trim(preg_replace("/\\\\/", "/", (string) $path));
        return $path;
    }
    /**
     * @param string $directory directory to be removed or emptied
     * @param bool $empty Flag indicating if the directory tree is to be emptied or removed
     *
     * recursive_remove_directory( directory to delete, empty )
     * expects path to directory and optional TRUE / FALSE to empty
     * of course PHP has to have the rights to delete the directory
     * you specify and all files and folders inside the directory
     *
     * to use this function to totally remove a directory, write:
     * recursive_remove_directory('path/to/directory/to/delete');
     *
     * to use this function to empty a directory, write:
     * recursive_remove_directory('path/to/full_directory',TRUE);
     */
    static public function recursive_remove_directory($directory, $empty= FALSE) {
        // if the path has a slash at the end we remove it here
        if (substr($directory, -1) == '/') {
            $directory= substr($directory, 0, -1);
        }
        // if the path is not valid or is not a directory ...
        if (!file_exists($directory) || !is_dir($directory)) {
            // ... we return false and exit the function
            return FALSE;
            // ... if the path is not readable
        }
        elseif (!is_readable($directory)) {
            // ... we return false and exit the function
            return FALSE;
            // ... else if the path is readable
        }
        else {
            // we open the directory
            $handle= opendir($directory);
            // and scan through the items inside
            while (FALSE !== ($item= readdir($handle))) {
                // if the filepointer is not the current directory
                // or the parent directory
                if ($item != '.' && $item != '..') {
                    // we build the new path to delete
                    $path= $directory . '/' . $item;
                    // if the new path is a directory
                    if (is_dir($path)) {
                        // we call this function with the new path
                        self :: recursive_remove_directory($path, $empty);
                        // if the new path is a file
                    }
                    else {
                        // we remove the file
                        unlink($path);
                    }
                }
            }
            // close the directory
            closedir($handle);
            // if the option to empty is not set to true
            if ($empty == FALSE) {
                // try to delete the now empty directory
                if (!rmdir($directory)) {
                    // return false if not possible
                    return FALSE;
                }
            }
            // return success
            return TRUE;
        }
    }
    static public function getTempDir($setPath = null) {
    	static $tempDir = null;

		if (file_exists(ACSPHPLIB_TEMPDIR)) {
			$i = parse_ini_file(ACSPHPLIB_TEMPDIR);
			$tempDir = rtrim($i[ACSPHPLIB_TEMPDIR_PATH]);
			if ($tempDir && is_dir($tempDir))
				return $tempDir;
		}

		if ($setPath)
			$tempDir = rtrim($setPath, ' /\\');

    	if ($tempDir == null || $tempDir == '<GLOBAL_TEMPORARY_DIR>') {
	        $res = sys_get_temp_dir();

	        $acsGlobalConfigVar = self::getGlobalConfig();
	        if (count($acsGlobalConfigVar)>0) {
	            if(isset($acsGlobalConfigVar['GLOBAL_TEMPORARY_DIR']))
	                $res = $acsGlobalConfigVar['GLOBAL_TEMPORARY_DIR'];
	        }
	        $tempDir = rtrim($res, ' /\\');
    	}
    	return $tempDir;
    }
    static public function getUniqueFileName($withTempPath=false) {
        usleep(1);
        $fname = acs_tools :: microtime_float();
        if($withTempPath) $fname=acs_file::getTempDir().acs_os::fileSeparator().$fname;
        return $fname;
    }
    static public function buildFilenameFromID($id, $rootPath, $fileprefix= '', $filepostfix= '', $filesPerDirectory= 1000) {
        if (!is_numeric($id) or ($id == 0))
            throw new acs_exFailed(__METHOD__ . " : Got an invalid Id ($id)!");
        $path= sprintf('%s/%06d', $rootPath, floor(($id -1) / $filesPerDirectory));
        if (!self :: makeDirs($path))
            throw new acs_exFailed(__METHOD__ . " " . __LINE__ . " : Could not create $path !");
        $filename= sprintf('%s/%s%d%s', $path, $fileprefix, $id, $filepostfix);
        return $filename;
    }
    static public function extractFilesFromArchive($archiveName, $dest, $fileExt) {
        require_once "File/Archive.php";
        $result= @ File_Archive :: extract($src= "$archiveName/", $dest);
        if (PEAR :: isError($result)) {
            throw new acs_exFailed(__METHOD__ . " " . __LINE__ . " : Could not create {$result->getMessage()} !");
        }
    }

    /**
     * @param string $archive   archive file to extract
     * @param string $dest      path where to extrat archive's contents
     *
     * This function extracts files from the specified archive using
     * operating system tools such as tar, unzip, gunzip. It chooses
     * automatically the utility to use checking the archive extension.
     *
     */
    static public function extractFilesFromArchiveExt($archive, $dest, $alias = array())
    {
        $ext = pathinfo($archive, PATHINFO_EXTENSION);
        $real_ext='.'.$ext;
        if (array_key_exists($ext, $alias)) $ext = $alias[$ext];
        switch (strtolower($ext))
        {
            case 'gz':
                if (substr($archive, strlen($archive) - 7) == ".tar.gz"){
                    $real_ext = '.tar.gz';
                    $cmd = "tar zxf '$archive' -C '$dest' --overwrite";
                } else
                    $cmd = "gunzip -f -c '$archive' > '$dest/" . basename($archive, '.gz') . "'";
                break;
            case 'bz2':
                if (substr($archive, strlen($archive) - 7) == ".tar.bz2"){
                    $real_ext = '.tar.bz2';
                    $cmd = "tar yxf '$archive' -C '$dest' --overwrite";
                } else
                    $cmd = "bunzip2 -f -c '$archive' > '$dest/" . basename($archive, $real_ext) . "'";
                break;
            case 'tgz':
                $cmd = "tar zxf '$archive' -C '$dest' --overwrite";
                break;
            case 'tar':
                $cmd = "tar xf '$archive' -C '$dest' --overwrite";
                break;
            case 'zip':
                $cmd = "unzip -o '$archive' -d '$dest'";
                break;
            default:
                throw new acs_exUnknownFormat("Unknown archive format for file '$archive'");
        }
        // unzip, with stderr redirection
        $output = null; $retval = null;
        exec($cmd . " 2>&1", $output, $retval);

        // check exit code
        if ($retval != 0)
            throw new acs_exFailed("Error executing '$cmd', command output was " . join("\n", $output));
        
        // return archive real extension
        return $real_ext;
    }

    static public function archiveAndDumpToBrowser($archiveName, $source, &$dbHandler, &$logHandler){
		try {
			$deleteSource = false;
			if(!file_exists($source)) throw new Exception('Cannot find '.$source);
			$archiveName = basename($archiveName);
	  		$ext = pathinfo($archiveName, PATHINFO_EXTENSION);
			$archiveFullName = sys_get_temp_dir().acs_os::fileSeparator().acs_file::getUniqueFileName().'.'.$ext;
			
			if(is_dir($source)) $sourcePath=$source;
			else { // create a directory wrapper
				$sourcePath = sys_get_temp_dir().acs_os::fileSeparator().acs_file::getUniqueFileName();
				self::makeDirs($sourcePath);
				$logHandler->debug("copy $source, $sourcePath");
				copy($source, $sourcePath.acs_os::fileSeparator().basename($source));
				$deleteSource = true;
			}
			
			// send preliminary headers if not sent before: this is to avoid timeout should the archive procedure take too much
			acs_file::sendDumpFilePreliminaryHeader();
			
			// create the archive
			acs_file::createArchiveExt( $archiveFullName, $sourcePath);
			
			// send it to the browser
			acs_file::dumpFileTobrowser( $archiveFullName, $archiveName, $dbHandler, $logHandler, true);
			if($deleteSource) @unlink($sourcePath);
		} catch (Exception $e) {
		    $logHandler->err(__METHOD__ . ' : ' . $e->getMessage());
		    // turn the error into an HTML error
		    echo self::getHTMLException($e->getMessage());
		    die;
		}    
	}

	static public function createArchiveExt($archive, $source, $alias = array())
    {
        $ext = pathinfo($archive, PATHINFO_EXTENSION);
        if (array_key_exists($ext, $alias)) $ext = $alias[$ext];
        switch (strtolower($ext))
        {
            case 'gz':
                if (substr($archive, strlen($archive) - 7) == ".tar.gz")
                    $cmd = "cd '$source' && tar zcf '$archive' *";
                else
                    $cmd = "cd '$source' && gzip * > $archive";
                break;
            case 'tgz':
                $cmd = "cd '$source' && tar zcf '$archive' *";
                break;
            case 'tar':
                $cmd = "cd '$source' && tar cf '$archive' *";
                break;
            case 'kmz':
            case 'zip':
                $cmd = "cd '$source' && zip -r '$archive' *";
                break;
            default:
                throw new acs_exUnknownFormat("Unknown archive format for file '$archive'");
        }
        // unzip, with stderr redirection
        $output = null; $retval = null;
        exec($cmd . " 2>&1", $output, $retval);

        // check exit code
        if ($retval != 0)
            throw new acs_exFailed("Error executing '$cmd', command output was " . join("\n", $output));
    }

    static public function sendDumpFilePreliminaryHeader(){
        if(self::$_preliminaryHeaderSent) return;
        header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
        header('Cache-Control: no-store, no-cache, must-revalidate');     // HTTP/1.1
        header('Cache-Control: pre-check=0, post-check=0, max-age=0');    // HTTP/1.1 

        header("Cache-control: private", false);
        header('Pragma: private');
        self::$_preliminaryHeaderSent=true;
	}

    /**
   @param string $file file to be downloaded
   @param string $name name for the downloaded file. if null is the basename of $file
   @param mixed  $loghandler Handler of the log file, can be null meaning no logging will be performed
    */
    public function dumpFileTobrowser($fullpathname, $name, &$dbHandler, &$logHandler, $deleteFile=false, $withdie = true, $update_cb=null,$mime_type=null) {
        if (!is_null($logHandler)) {
            $t= date(DATE_RFC822, $_SERVER['REQUEST_TIME']);
            $msg= "Request from {$_SERVER['REMOTE_ADDR']} at {$_SERVER['REQUEST_TIME']} ({$t}) of $file";
            $logHandler->info(__METHOD__ . " - " . __LINE__ . ":" . $msg);
        }
        if (!file_exists($fullpathname)) {
            $msg= "Requested file $fullpathname does not exist";
            if (!is_null($logHandler)) {
                $logHandler->err(__METHOD__ . " - " . __LINE__ . ":" . $msg);
            }
            throw new acs_exFileError ($msg);
        }

        $size= acs_file::getFileSize($fullpathname);
        if (is_null($name)) {
            $name= basename($fullpathname);
        }
        $name= rawurldecode($name);

        if (isset($_SERVER['HTTP_USER_AGENT'])) {
            $user_agent = $_SERVER['HTTP_USER_AGENT'];
        }
        if (!is_null($logHandler)) $logHandler->debug(__METHOD__ . " - " . __LINE__ . ":" . $user_agent);

        // Close sessions and shutdowns output buffering
        @session_write_close();
        while(@ob_get_level()){ @ob_end_clean(); }
        ob_implicit_flush();
        set_time_limit(0); 

        switch($mime_type){
            case 'autopdf':
                $mime_type='application/pdf';
                header('Content-Disposition: filename="' . $name . '"');
                break;

            case null:
            case 'application/octetstream':
            case 'application/octet-stream':
               	$mime_type= ($user_agent == 'IE' || $user_agent == 'Opera') ? 'application/octetstream' : 'application/octet-stream';
                header('Content-Disposition: attachment; filename="' . $name . '"');
                header('Accept-Ranges: bytes');
                break;
            case 'application/xml':
            case 'application/zip':
                header('Content-Disposition: attachment; filename="' . $name . '"');
                break;
        }
        header('Content-Type: ' . $mime_type);
        acs_file::sendDumpFilePreliminaryHeader();
        self::$_preliminaryHeaderSent = false;
        
		$range=0;
        /////  multipart-download and resume-download
        if (isset ($_SERVER['HTTP_RANGE'])) {
            $ranges=preg_match('/^bytes=((\d*-\d*,? ?)+)$/', @$_SERVER['HTTP_RANGE'], $matches) ? $matches[1] : array();
            foreach (explode(',', $ranges) as $chunk){
                list($o, $e) = explode('-', $chunk);
                if ($e >= $this->size || (empty($e) && $e !== 0 && $e !== '0')) {
                    $e = $this->size - 1;
                }
                if (empty($o) && $o !== 0 && $o !== '0') {
                    $o = $this->size - $e;
                    $e = $this->size - 1;
                }
                $parts[] = array($o, $e);
            }

            if (!is_null($logHandler)) $logHandler->debug(__METHOD__ . " - " . __LINE__ . ">>>>>>>" . var_export(array($ranges,$parts[0]),true));
            $range = $parts[0][0];
            $size2= $size -1;
            $new_length= $size - $range;
            header("HTTP/1.1 206 Partial Content");
            header("Date:".	gmdate("D, d M Y H i s e",mktime()));
            header("Content-Length: $new_length");
            header("Content-Range: bytes $range-$size2/$size");
            $msg= "Requested a partial download of $name, starting over at $range";
            if (!is_null($logHandler)) {
                $logHandler->debug(__METHOD__ . " - " . __LINE__ . ":" . $msg);
            }
            if (!is_null($logHandler)) $logHandler->debug(__METHOD__ . " - " . __LINE__ . ":" . "Content-Length: $new_length");
            if (!is_null($logHandler)) $logHandler->debug(__METHOD__ . " - " . __LINE__ . ":" . "Content-Range: bytes $range-$size2/$size") ;
        }
        else {
            $size2= $size -1;
            header("Content-Length: " . $size);
            $msg= "Requested a complete download of $name";
            if (!is_null($logHandler)) {
                $logHandler->debug(__METHOD__ . " - " . __LINE__ . ":" . $msg);
            }
        }
        header("Connection: Keep-Alive");
        if (isset ($new_length)){ $size= $new_length; }
        $chunksize= 1*(1024 * 1024);
     
        $architecture=php_uname('m');
        if(	(acs_file::getFileSize($fullpathname)<(float)2147483648)  ){
            // This is the standard
            acs_file::outputChunks($fullpathname, $size, $range, $logHandler, $deleteFile, $update_cb, $chunksize );
        } else {
            // This is the new way for huge (>2Gb files)
            acs_file::outputHugeFileChunks($fullpathname, $size, $range, $logHandler, $deleteFile, $update_cb, $chunksize );
        }
        if ($withdie)
            die();
    }
    /**
     * Reads a file chunk by chunk using an external helper program and dumps the output 
     * This is for files &gt; 2gb (if running on a 32bit machine)
     * 
     * @param $fullpathname string Filename with complete path
     * @param $size integer Size, used for update callback and logging
     * @param $offset integer Initial offset when opening file for read
     * @param $logHandler object Log handler, can be null;
     * @param $deleteFile boolean whether to remove file after the dump
     * @param $update_cb callback Callback function to be invoked after processing each chunk
     * @param $chunksize integer Size of the chunk to be read. 
     * 
     */
    static public function outputHugeFileChunks($fullpathname, $size, $offset, $logHandler, $deleteFile=false, $update_cb=null, $chunksize=4096){        
		$bs=0;
		$transmission_size=abs($size-$offset);
		$logHandler->debug(__METHOD__ . " - " . __LINE__ . ": $bs $size $offset $transmission_size" );
		
		if( ($offset==0)&&($chunksize==0) ){return;}
		if( $chunksize==0 ){ return; }
        
        while( ($bs<$transmission_size) && (connection_status()==0) ){
	    $logHandler->debug(__METHOD__ . " - " . __LINE__ . ": $bs $size $offset $transmission_size" );
            $cmd =sprintf("python -c \"f=open('%s','rb',0);f.seek(%s);print f.read(%s)\" ", $fullpathname,$offset,$chunksize);

            ob_start();
            passthru( $cmd );
            $data = ob_get_clean();
            $read_bytes=strlen($data);

            if($read_bytes>$chunksize){
                $diff=$read_bytes-$chunksize;
                $data = substr($data,0,$read_bytes-$diff);
                $read_bytes=strlen($data);
            } 
            if(($bs+$read_bytes)>$transmission_size){
                // Get rid of garbage
                $data = substr($data, 0, ($transmission_size-$bs) );
                $read_bytes=($transmission_size-$bs);
            }
            $offset+=$read_bytes;
            $bs+=$read_bytes;
            echo $data;
            flush();

            if( ! is_null($update_cb) ){
                $percentage =100. * ($bs/$size);
                $update_cb[1]['percentage'] = $percentage;

                if (!is_null($logHandler)) $logHandler->debug(__METHOD__." ======>|$percentage $chunksize >> ". sprintf("%02d%%",$percentage));
                if (!is_null($logHandler)) $logHandler->debug(var_export($update_cb[1],true));
                call_user_func_array($update_cb[0], $update_cb[1] );
            }
        }
    }
    /**
     * Reads a file chunk by chunk and dumps the output 
     * This is safe ONLY for files &lt; 2gb (if running on a 32bit machine)
     * 
     * @param $fullpathname string Filename with complete path
     * @param $size integer Size, used for update callback and logging
     * @param $offset integer Initial offset when opening file for read
     * @param $logHandler object Log handler, can be null;
     * @param $deleteFile boolean whether to remove file after the dump
     * @param $update_cb callback Callback function to be invoked after processing each chunk
     * @param $chunksize integer Size of the chunk to be read. 
     * 
     */
    static public function outputChunks($fullpathname, $size, $offset, $logHandler, $deleteFile=false, $update_cb=null, $chunksize=4096){
        $bytes_send= 0;
        if(!is_null($logHandler)) $logHandler->debug(__METHOD__ . " - " . __LINE__ . ": $fullpathname $size $offset" );
        if ($file= fopen($fullpathname, 'r')) {
            if (isset ($_SERVER['HTTP_RANGE']))
                fseek($file, $offset);
            while (!feof($file) and (connection_status() == 0)) {
                $buffer= fread($file, $chunksize);
                print ($buffer);
                flush();
                $bytes_send += strlen($buffer);

                if( ! is_null($update_cb) ){
                    if($size==0){
                        $update_cb[1]['percentage'] =100;
                    } else {
                        $update_cb[1]['percentage'] = 100. * ($bytes_send/$size);
                    }
                    if(!is_null($logHandler))$logHandler->debug("======>|". $bytes_send."| |".$size."| |".( $bytes_send/$size ) . "  ".  100*(( $bytes_send/$size ))." >> ". sprintf("%03d",100*( $bytes_send/$size ))   );
                    if(!is_null($logHandler))$logHandler->debug(var_export($update_cb[1],true));
                    call_user_func_array($update_cb[0], $update_cb[1] );
                }

            }
            fclose($file);
            if($deleteFile && file_exists($fullpathname)) @unlink($fullpathname);
        } else {
            $msg= "Cannot open file: $fullpathname";
            if (!is_null($logHandler)) {
                $logHandler->err(__METHOD__ . " - " . __LINE__ . ":" . $msg);
            }
            throw new acs_exFileError ($msg) ;
        }

        if( ! is_null($update_cb) ){
            if($size==0){
                $update_cb[1]['percentage'] = 100. ;
            } else {
                $update_cb[1]['percentage'] = 100. * ($bytes_send/$size);
            }
            call_user_func_array($update_cb[0], $update_cb[1] );
        }
    }
    /**
     * Execute a command
     * @param $command string Command to be executed
     * @return $output array containing the output of the executed command
     */
    static public function execute($command){
    	$output=null;
    	$retval=0;
    	exec($command . " 2>&1", $output, $retval);
		if ($retval != 0){
			$msg =sprintf("Error executing '%s', command output was: %s",
					$command,
					join("\n", $output)
				     );
			throw new acs_exFailed("Error executing:".$command);
		}
		return $output;
    }
    /**
     * Compute file size in a (possibly) safe way.
     * To avoid limit of 2Gb on 32bit system relies upon
     * the linux "stat" utility and returns the size as a float
     * number.
     * 
     * @param $fullpathname string Full path of the file 
     * @return $size float Size of the file
     * 
     */
    static public function getFileSize($fullpathname){
		if (strtoupper(substr(PHP_OS, 0, 3)) !== 'WIN') {
			$cmd=sprintf("stat -L --print %%s %s",escapeshellarg($fullpathname));
	        $output=acs_file::execute($cmd);
	        $size = (float)($output[0]);
        } else {
        	$size = @filesize($fullpathname);
        }
        return $size;
	}


 
	static public function smartCopyFile($fname, $desturi, &$_logHandler=null, $overwrite=false, $simulated=false, $pubkeyfile=null, $privkeyfile=null, $passphrase=null, $dest_fname=null) {
        if(trim($desturi)=='')
            throw new acs_exBadParam("Empty URI passed");

        // Verify that desired files do exist
        if (!file_exists($fname))
            throw new acs_exBadParam("Source filename $fname does not exist");

        $extraDebugStr = ($simulated) ? "Simulation : " : "";

        if( is_null($dest_fname) )
            $dest_fname= basename($fname);

        $urlParseAr = parse_url($desturi);

        require_once "ACSPhpLib/acs_network.php";

        $copyType = $urlParseAr['scheme'];
        switch ($copyType) {

            case "ftp" :
                if(isset($_logHandler)) $_logHandler->notice($extraDebugStr."Sending $fname to $desturi via ftp");
                if(!$simulated) acs_network::ftp_send($desturi, $fname, $dest_fname);
                break;

            case "sftp" :
                if(isset($_logHandler)) $_logHandler->notice($extraDebugStr."Sending $fname to $desturi via sftp");
                if(!$simulated) acs_network::sftp_send($desturi, $fname, $dest_fname, $pubkeyfile, $privkeyfile, $passphrase);
                break;

            case "file" :
                $symlinkfilename= $urlParseAr['path'] . '/' . $dest_fname;
                if($overwrite && file_exists($symlinkfilename)) {
                    $res = @ unlink($symlinkfilename);
                    if($res===false) throw new acs_exFileError("Cannot delete old version of file $symlinkfilename");
                }
                $res= @ copy($fname, $symlinkfilename);
                if($res===false) throw new acs_exFileError("Cannot copy $fname to $symlinkfilename");
                break;

            case "link" :
                $symlinkfilename= $urlParseAr['path'] . '/' . $dest_fname;
                if($overwrite && file_exists($symlinkfilename)) {
                    $res = @ unlink($symlinkfilename);
                    if($res===false) throw new acs_exFileError("Cannot delete old version of link $symlinkfilename");
                }
                $res= @ symlink($fname, $symlinkfilename);
                if($res===false) throw new acs_exFileError("Cannot link $fname to $symlinkfilename");
                break;

            default :
                throw new acs_exBadParam("Unknown copy type $copyType");
        }
    }

    static public function addLabelToFileSize($size,$prec=0,$extendedLabel=false) {

        $units=array(   array("byte","B"),
                        array("kilobyte","kB"),
                        array("megabyte","MB"),
                        array("gigabyte","GB"),
                        array("terabyte","TB"),
                        array("petabyte","PB"),
                        array("exabyte","EB"),
                        array("zettabyte","ZB"),
                        array("yottabyte","YB")
                    );
        foreach($units as $unitAr){
            if($size<1024) break;
            $size=$size/1024.;
        }
        if($extendedLabel) return round($size,$prec).' '.$unitAr[0];
        else return round($size,$prec).' '.$unitAr[1];
    }
    
    static public function getGlobalConfigName() {
    	if (file_exists(ACSPHPLIB_CONF)) {
    		$i = parse_ini_file(ACSPHPLIB_CONF);
    		return $i[ACSPHPLIB_GLOBAL_CONFIG_PATH];
    	}

    	return DEFAULT_GLOBAL_CONFIG;
    }
    
    static public function getGlobalConfig($what = 'tokens', $tokensIniFileName = null) {
    	$values = array();
    	
    	try {
    		if ($tokensIniFileName == null)
    			$iniName = self::getGlobalConfigName();
    		else
    			$iniName = $tokensIniFileName;
    		$global_ini = self :: getINIFile($iniName);

			switch ($what) {
				case 'tokens':    		
		    		$values = $global_ini['tokens'];
		    		
		    		// Add machine specific tokens to the main array
		    		$machine_section = 'tokens_' . php_uname('n'); 
		    		if (array_key_exists($machine_section, $global_ini))
		    			$values = array_merge($values, $global_ini[$machine_section]);
		    		break;
		    	default:
		    		$values = $global_ini;
			}
    	} catch (Exception $e) {
    		// no global config exists
    	}
    	
    	return $values;
    }
    
    static public function getHTMLException($msg){
        $errBox = self::getHTMLUserMessageBox($msg);
        return  "<center><font color='red'><h1><hr>ERROR<hr></h1></font>$errBox</center><br><br><hr><center><h3>Contact the web site administrator</h3></center>";
    }
    
    static public function getHTMLUserMessageBox($msg){
        return "<table width=80%><tr><td class='usermessages'>".nl2br(trim($msg))."</td></tr></table>";
    }
}


if (!function_exists('sys_get_temp_dir')) {
    function sys_get_temp_dir() {
        // Try to get from environment variable
        if (!empty ($_ENV['TMP']) and is_readable($_ENV['TMP'])) {
            return realpath($_ENV['TMP']);
        }
        else
            if (!empty ($_ENV['TMPDIR']) and is_readable($_ENV['TMPDIR']) and is_writable($_ENV['TMPDIR'])) {
                return realpath($_ENV['TMPDIR']);
            }
            else
                if (!empty ($_ENV['TEMP']) and is_readable($_ENV['TEMP']) and is_writable($_ENV['TEMP'])) {
                    return realpath($_ENV['TEMP']);
                }
                else {
                    // Detect by creating a temporary file
                    // Try to use system's temporary directory
                    // as random name shouldn't exist
                    $temp_file= tempnam(md5(uniqid(rand(), true)), '');
                    if ($temp_file) {
                        $temp_dir= realpath(dirname($temp_file));
                        unlink($temp_file);
                        return $temp_dir;
                    }
                    else {
                        throw new acs_exFileError("Could not determine a temporary directory");
                        return false;
                    }
                }
    }
}

