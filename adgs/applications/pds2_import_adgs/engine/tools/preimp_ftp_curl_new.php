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

require_once dirname(__FILE__) . '/../../../pds2_import/engine/pds2_file_transfer_tools.php';

class preimp_ftp_curl_new implements pds2_file_transfer {
	protected $protocol, $host, $username, $password, $port, $timeout, $ftp_active_mode, $options;
	protected $removeCommand;
	
	protected $ch;
	
	protected $remoteTimezone = 'UTC';
	
	protected $ls_timeout;
	
	protected $curl_reset_connection = false;
	
	protected $dirCacheItemsCount;
	protected $dirCacheItems;
	protected $dirCacheMaxItems = 5000;
	
	const FTP_CURL_NEW_OPTIONS_CURL_PRESET = 'curl_preset';
	const FTP_CURL_NEW_OPTIONS_CURL_OPTIONS = 'curl_options';
	const FTP_CURL_NEW_OPTIONS_RETURN_FILENAMES_ONLY = 'return_filenames_only';
	const FTP_CURL_NEW_OPTIONS_REMOTE_TIMEZONE = 'remote_timezone';
	const FTP_CURL_NEW_OPTIONS_LS_TIMEOUT = 'ftp_ls_timeout';
	const FTP_CURL_NEW_OPTIONS_CURL_RESET_CONNECTION = 'curl_reset_connection';
	const FTP_CURL_NEW_OPTIONS_DIR_CACHE_ITEMS = 'dir_cache_items';
	
	const FTP_ACTION_TRANSFER = 'transfer';
	const FTP_ACTION_LS = 'ls';

    public function __construct($protocol, $host, $username, $password, $port = 21, $timeout = 90, $ftp_active_mode = false, $options = array()) {
    	$this->protocol = $protocol;
    	$this->host = $host;
    	$this->username = $username;
    	$this->password = $password;
    	$this->port = $port;
    	$this->timeout = $timeout;
    	$this->ftp_active_mode = $ftp_active_mode;
    	$this->options = $options;
    	
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_REMOTE_TIMEZONE, $this->options)) {
    		$this->remoteTimezone = $this->options[self::FTP_CURL_NEW_OPTIONS_REMOTE_TIMEZONE];
    	}
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_LS_TIMEOUT, $this->options)) {
    		$this->ls_timeout = $this->options[self::FTP_CURL_NEW_OPTIONS_LS_TIMEOUT];
    	}
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_CURL_RESET_CONNECTION, $this->options)) {
    		$this->curl_reset_connection = $this->options[self::FTP_CURL_NEW_OPTIONS_CURL_RESET_CONNECTION];
    	}
    	
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_DIR_CACHE_ITEMS, $this->options)) {
    		$this->dirCacheMaxItems = $this->options[self::FTP_CURL_NEW_OPTIONS_DIR_CACHE_ITEMS];
    	}
    	
    	$this->dirCacheInit();

    	$this->connect();
    }
    
    protected function connect() {
    	$this->ch = curl_init();
    	
    	$this->setCurlOptions();
		
		// try to connect
		$this->getDirList('/');
    }
    
    protected function setCurlOptions() {
    	/*
    	 * options valid for all protocols
    	 */
    	curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, false);
    	curl_setopt($this->ch, CURLOPT_TIMEOUT, $this->timeout);
    	curl_setopt($this->ch, CURLOPT_FTPLISTONLY, false);
    	curl_setopt($this->ch, CURLOPT_PORT, $this->port);
    	curl_setopt($this->ch, CURLOPT_USERPWD, "{$this->username}:{$this->password}");
    	
    	$this->callPresetMethod();
    	$this->setAdditionalCurlOptions();
    }
    
    protected function setCurlTimeout($what) {
    	switch ($what) {
    		case self::FTP_ACTION_LS:
    			curl_setopt($this->ch, CURLOPT_TIMEOUT, $this->ls_timeout ? $this->ls_timeout : $this->timeout);
    			break;
    		default:
    			curl_setopt($this->ch, CURLOPT_TIMEOUT, $this->timeout);
    			break;
    	}
    }
    
    protected function callPresetMethod() {
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_CURL_PRESET, $this->options)) {
    		$presetMethod = $this->options[self::FTP_CURL_NEW_OPTIONS_CURL_PRESET];
    		if (method_exists($this, $presetMethod)) {
    			$this->$presetMethod();
    		}
    	}
    }
    
    protected function curl_preset_ftps_implicit() {
    	// TODO: verify if it is needed with an implicit ftps connection
    	curl_setopt($this->ch, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
    	curl_setopt($this->ch, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
    	curl_setopt($this->ch, CURLOPT_FTP_SSL, CURLOPT_FTPSSLAUTH);
    	curl_setopt($this->ch, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    }
    
    protected function curl_preset_ftps_explicit() {
    	// use the connection as a simple ftp connection
    	$this->protocol = 'ftp';
    	
    	// switch to AUTH TLS after the ftp connection
    	curl_setopt($this->ch, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_TLS);
    	curl_setopt($this->ch, CURLOPT_FTPSSLAUTH, CURLFTPAUTH_DEFAULT);
    	curl_setopt($this->ch, CURLOPT_FTP_SSL, CURLOPT_FTPSSLAUTH);
    	curl_setopt($this->ch, CURLOPT_SSLVERSION, CURL_SSLVERSION_DEFAULT);
    }
    
    protected function setAdditionalCurlOptions() {
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_CURL_OPTIONS, $this->options)) {
    		$curlOptions = $this->options[self::FTP_CURL_NEW_OPTIONS_CURL_OPTIONS];
    		if (is_array($curlOptions)) {
    			foreach ($curlOptions as $option => $value) {
    				curl_setopt($this->ch, $option, $value);
    			}
    		}
    	}
    }
    
    protected function getFilenameOnly() {
    	$nameOnly = false;
    	if (array_key_exists(self::FTP_CURL_NEW_OPTIONS_RETURN_FILENAMES_ONLY, $this->options)) {
    		$nameOnly = $this->options[self::FTP_CURL_NEW_OPTIONS_RETURN_FILENAMES_ONLY];
    	}
    	return $nameOnly;
    }
    
    protected function getBaseUrl() {
    	return "{$this->protocol}://{$this->host}:{$this->port}";
    }
    
    protected function getUrl($path) {
    	$baseUrl = $this->getBaseUrl();
    	return $baseUrl . "/{$path}";
    }
    
    protected function getFileAgeFromListParts($fileParts) {
    	if (strpos($fileParts['yearorhour'], ':') !== false) {
    		$year = gmstrftime('%Y');
    		$hour = $fileParts['yearorhour'];
    	} else {
    		$year = $fileParts['yearorhour'];
    		$hour = '00:00:00';
    	}
    	return strtotime("{$fileParts['day']} {$fileParts['month']} {$year} {$hour}");
    }
    
    protected function getRemoveFileCommand() {
    	switch ($this->protocol) {
    		case 'ftp': return "dele";
    		case 'ftps': return "dele";
    		case 'sftp': return "rm";
    	}
    }
    
    protected function getRemoveDirCommand() {
    	switch ($this->protocol) {
    		case 'ftp': return "rmd";
    		case 'ftps': return "rmd";
    		case 'sftp': return "rmdir";
    	}
    }
    
    public function deleteFile($remoteFile) {
    	$this->setCurlTimeout(self::FTP_ACTION_LS);
    	
		curl_setopt($this->ch, CURLOPT_URL, $this->getBaseUrl());

		// escapeshellarg($remoteFile) does not work with old FTP sites
		// beware of files and directories containing spaces		
		curl_setopt($this->ch, CURLOPT_POSTQUOTE, array($this->getRemoveFileCommand() . " ". $remoteFile));
    	
		$response = @curl_exec($this->ch);
		
		curl_setopt($this->ch, CURLOPT_POSTQUOTE, array());
		
		// invalidate the cache
		$this->dirCacheInit();

		if ($response === false) {
			$error = curl_error($this->ch);
			throw new Exception("Error deleting file {$remoteFile}: {$error}");
		}
	}
	
	protected function fixDirectoryName($remoteDir) {
		if (!empty($remoteDir) && $remoteDir[strlen($remoteDir) - 1] != '/') {
			return $remoteDir .= '/';
		} else {
			return $remoteDir;
		}
	}

	public function deleteEmptyDir($remoteDir) {
		$this->setCurlTimeout(self::FTP_ACTION_LS);
		
		$remoteDir = $this->fixDirectoryName($remoteDir);
		
		curl_setopt($this->ch, CURLOPT_URL, $this->getBaseUrl());
		
		// escapeshellarg($remoteFile) does not work with old FTP sites
		// beware of files and directories containing spaces
		curl_setopt($this->ch, CURLOPT_POSTQUOTE, array($this->getRemoveDirCommand() . " ". $remoteDir));
		
		$response = @curl_exec($this->ch);
		
		curl_setopt($this->ch, CURLOPT_POSTQUOTE, array());
		
		// invalidate the cache
		$this->dirCacheInit();
		
		if ($response === false) {
			$error = curl_error($this->ch);
			throw new Exception("Error deleting empty dir {$remoteDir}: {$error}");
		}
	}

	protected function parseDirectoryItem($line) {
		// -rw-r--r--    1 anonymou ex         33201180 Jan 14 06:12 D1D01140000011718001
		$regex = '/' .
			'(?P<permissions>[-rwxds]+)\s+' .
			'(?P<blocks>[0-9]+)\s+' .
			'(?P<user>\w+)\s*' .
			'(?P<group>\w+)\s+' .
			'(?P<size>[0-9]+)\s+' .
			'(?P<month>\w+)\s+' .
			'(?P<day>[0-9]+)\s+' .
			'(?P<yearorhour>[:0-9]+)\s+' .
			'(?P<filename>.*)$' .
			'/';
		$matches = array();
		if (!preg_match($regex, $line, $matches)) {
			return null;
		}
		return $matches;
	}
	
	protected function getRawDirList($dirname) {
		
		$this->setCurlTimeout(self::FTP_ACTION_LS);
		
		$dirname = $this->fixDirectoryName($dirname);
		
		$url = $this->getUrl($dirname);
		curl_setopt($this->ch, CURLOPT_URL, $url);
		
		$fp = tmpfile();
		curl_setopt($this->ch, CURLOPT_FILE, $fp);
		
		$response = curl_exec($this->ch);
		fseek($fp, 0, SEEK_SET);
		$data = stream_get_contents($fp);
		fclose($fp);
		
		if ($response === false) {
			$error = curl_error($this->ch);
			throw new Exception("Error listing directory {$dirname}: {$error}");
		}
		return preg_split('/\r\n|\r|\n/', $data);
	}
	
	protected function dirCacheInit() {
		$this->dirCacheItems = array();
		$this->dirCacheItemsCount = 0;
	}
	
	protected function dirCacheSetItem($dirname, $filenameOnly, array $contents) {
		$itemsCount = count($contents);
		
		if ($this->dirCacheItemsCount + $itemsCount > $this->dirCacheMaxItems) {
			$this->dirCacheInit();
		}
		
		$this->dirCacheItems[$dirname][$filenameOnly ? 1 : 0] = $contents;
		$this->dirCacheItemsCount += $itemsCount;
	}
	
	protected function dirCacheGetItem($dirname, $filenameOnly) {
		if ($this->dirCacheItemExists($dirname, $filenameOnly)) {
			return $this->dirCacheItems[$dirname][$filenameOnly ? 1 : 0];
		}
		throw new Exception("missing dirCache item: $dirname," . ($filenameOnly ? 'true' : 'false'));
	}
	
	protected function dirCacheItemExists($dirname, $filenameOnly) {
		return array_key_exists($dirname, $this->dirCacheItems) &&
			array_key_exists($filenameOnly ? 1 : 0, $this->dirCacheItems[$dirname]);
	}
	
	protected function computeDirData($dirname) {
		$filenameOnly = $this->getFilenameOnly();
		$dirname = rtrim($dirname, '/');
		
		if ($this->dirCacheItemExists($dirname, $filenameOnly)) {
			return $this->dirCacheGetItem($dirname, $filenameOnly);
		}
		
		$directoryItems = $this->getRawDirList($dirname);
		
		$itemsInfo = array();
		
		/*
		 * https://projectit.exprivia.it/browse/CCDS-73
		 *
		 * Set the default timezone aligned with the
		 * remote system timezone, to calculate the correct
		 * epoch from file times textual representation
		 */
		$previousTz = date_default_timezone_get();
		date_default_timezone_set($this->remoteTimezone);
		
		foreach ($directoryItems as $item) {
			$fileParts = $this->parseDirectoryItem($item);
			if ($fileParts) {
				if ($fileParts['filename'] == '.' || $fileParts['filename'] == '..') {
					continue;
				}
				
				$name = $filenameOnly ? $fileParts['filename'] : $dirname . '/' . $fileParts['filename'];
				
				$itemsInfo[] = array(
					'path' => $name,
					'size' => $fileParts['size'],
					'age' => $this->getFileAgeFromListParts($fileParts),
					'dir' => $fileParts['permissions'][0] == 'd');
			}
		}
		
		/*
		 * Set back the original timezone
		 */
		date_default_timezone_set($previousTz);
		
		$this->dirCacheSetItem($dirname, $filenameOnly, $itemsInfo);
		
		return $itemsInfo;
	}

	public function getDirList($dirname, $what = 'd') {
		
		$dirs = array();
		$files = array();

		foreach ($this->computeDirData($dirname) as $fileParts) {
			if ($fileParts['dir']) {
				$dirs[] = $fileParts['path'];
			} else {
				$files[] = $fileParts['path'];
			}
		}
		
		switch ($what) {
			case 'd':	return $dirs;
			case 'f':	return $files;
			default:	return array('dirs' => $dirs, 'files' => $files);
		}   
	}
	
	public function getItemListInfo($dirname) {
		return $this->computeDirData($dirname);
	}
	
	public function getBinaryFile($localFile, $remoteFile) {
		
		$this->setCurlTimeout(self::FTP_ACTION_TRANSFER);
		
		$fp = fopen ($localFile, 'w+');
		
		curl_setopt($this->ch, CURLOPT_URL, $this->getUrl($remoteFile));
		curl_setopt($this->ch, CURLOPT_FILE, $fp);
		
		$response = @curl_exec($this->ch);
		
		fclose($fp);

		// This is required because using CURLOPT_FILE compromise the option
		// CURLOPT_RETURNTRANSFER for the next requests
		//
		// Workaround: always write curl_exec responses to a file (see getRawDirList)
		//
		if ($this->curl_reset_connection) {
			$this->resetConnection();
		} else {
			// set again the required curl options
			$this->setCurlOptions();
		}
		
		if ($response === false) {
			@unlink($localFile);
			$error = curl_error($this->ch);
			throw new Exception("Error downloading file {$remoteFile}: {$error}");
		}
	}
	
	protected function getDirectoryItem($dirListItems, $remoteItem) {
		foreach ($dirListItems as $item) {
			if ($item['path'] == $remoteItem) {
				return $item;
			}
		}
		
		throw new Exception("cannot find remote item {$remoteItem} in {$dirListItems}");
	}
	
	protected function isDirectory($remoteItem) {
		$baseDir = dirname($remoteItem) . '/';
		$baseListItems = $this->getItemListInfo($baseDir);

		$item = $this->getDirectoryItem($baseListItems, $remoteItem);
		return $item['dir'];
	}
	
	public function getBinaryItem($localFile, $remoteItem) {
		
		if ($this->isDirectory($remoteItem)) {
			if (!@mkdir($localFile)) {
				throw new Exception("Cannot make local directory {$localFile}");
			}
			
			foreach ($this->getItemListInfo($remoteItem . '/') as $item) {
				if ($item['dir'] == true) {
					$this->getBinaryItem($localFile . '/' . basename($item['path']), $item['path']);
				} else {
					$this->getBinaryFile($localFile . '/' . basename($item['path']), $item['path']);
				}
			}
		} else {
			$this->getBinaryFile($localFile, $remoteItem);
		}
	}
	
	protected function resetConnection() {
		$this->closeConnection();
		
		$this->ch = curl_init();
		$this->setCurlOptions();
	}

	public function putBinaryFile($local_file, $remote_file) {
		throw new RuntimeException(__METHOD__ . " not implemented");
	}

	public function closeConnection() {
		curl_close($this->ch);
	}
    
}

?>