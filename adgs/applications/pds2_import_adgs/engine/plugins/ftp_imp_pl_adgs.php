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

require_once dirname(__FILE__) .'/../../../pds2_import/engine/plugins/pds2_imp_pl_simple_dir_harvester.php';
require_once dirname(__FILE__) .'/ftp_defines.php';
require_once dirname(__FILE__) .'/ftp_special_rules.php';
require_once dirname(__FILE__) .'/ftp_tools.php';
require_once dirname(__FILE__) .'/../tools/preimp_ftp_curl_new.php';

/**
 * This class handles files and directories discovery and download from external repositories
 */

class ftp_imp_pl_adgs extends pds2_imp_pl_simple_dir_harvester{

	protected	$found_dirs = array();
	protected	$remoteFilesInfo;
	protected	$handleRemoteFilesMTime = true;
	protected	$copied_files = array();
				
	public function __construct($dbHandler,$logHandler,$storageHandler, $iniVars, $opVars) {
		parent::__construct($dbHandler,$logHandler,$storageHandler, $iniVars, $opVars);	
		$this->_sysLogHandler = & Log :: singleton('syslog','', '', $iniVars['log_to_syslog']);	
		$mask= '';
		eval ("\$mask=" . $this->_iniVars['log_to_syslog']['log_level'] . ";");
		$this->_sysLogHandler->setMask($mask);
		
		if (array_key_exists('sync_timestamp', $iniVars['import'])) {
			$this->handleRemoteFilesMTime = $iniVars['import']['sync_timestamp'];
		}
	}
	

	/** This method initializes the remote connection and try to connect, for success lock t_receptionruleslock table for failure call 
	 * incrementConnectionCounter 
     * @parameter string $userId
     */
	public function initialize($userid){
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		pds2_imp_pl_base::initialize($userid);
		
		$uri=$this->_opVars['repo_protocol'].'://'.$this->_opVars['repo_username'].':***@'.$this->_opVars['repo_hostname'].$this->_opVars['repo_remotepath'];
		printf("%-10s %-80s %-10s %-10s\n", $this->_opVars['repo_name'], $uri, $this->_opVars['run_rule_name'], $this->_opVars['run_rule']);
		$this->_logHandler->info("connecting to {$this->_opVars['repo_name']}...");

        // connect to the URI
		$scheme				= strtolower(trim($this->_opVars['repo_protocol']));
		if($scheme == '') {
			$this->_logHandler->warning("Scheme empty, assuming it is 'file'");
			$scheme = 'file';	
		}

		$this->current_account	= $this->_opVars['repo_username'];
		$this->current_host		= $this->_opVars['repo_hostname'];
		$this->current_port		= $this->_opVars['repo_port'];
		$decrypt_command=str_replace('<PWD>',$this->_opVars['repo_password'],$this->_iniVars['import']['decrypt_tool_path']);
		$this->current_password	=  ($this->_opVars['repo_password'])?exec($decrypt_command):'';
		$this->current_remote_path=  $this->_opVars['repo_remotepath']; 
		$this->_logHandler->debug("Using the '$scheme' connector");
		$this->_logHandler->debug("PDS TMP DIR: {$this->pds_temporary_dir}, PDS FINAL DIR: {$this->pds_final_dir}");
		
		// try to connect, for failure increment retrycounter on db
        try{
        	$this->_logHandler->debug("Attempt {$this->_opVars['retrycounter']}: connecting  {$this->current_remote_path}");
	        $this->_sysLogHandler->notice("LOG|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Connecting to host ".$this->_opVars['repo_hostname']."...\"|");	
			$ftp_active_mode=(isset($this->_iniVars['application']['ftp_active_mode']))?$this->_iniVars['application']['ftp_active_mode']:false;

			$this->preimp_obj = $this->createPreImpObj($scheme, $ftp_active_mode);
			
        	$this->_logHandler->info("Connected as $scheme://{$this->current_account}@{$this->current_host}{$this->current_remote_path}");
	        $this->_sysLogHandler->notice("LOG|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Connected to host ".$this->_opVars['repo_hostname']." in $scheme mode\"|");	
			ftp_tools::lockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class'],$this->_opVars['ruleslock_max_time']);
			$this->_logHandler->info("table t_receptionruleslock locked");
			return $this->preimp_obj;       
        }catch(Exception $e) {
		    $this->_logHandler->err($e->getMessage());
	       	$this->incrementConnectionCounter();
        	ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
	        $this->_sysLogHandler->err("FAIL|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Cannot connect to remote host. {$e->getMessage()}\"|");
			$this->_logHandler->info("table t_receptionruleslock unlocked");
	       	throw $e;  
        }
	}
	
	protected function createPreImpObj($scheme, $ftp_active_mode) {
		$options = array();
		$port = 0;
		
		switch ($scheme) {
			case 'ftp':
				$port = 21;
				break;
			case 'ftps':
				$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_CURL_PRESET] = 'curl_preset_ftps_explicit';
				$port = 21;
				break;
			case 'sftp':
				$port = 22;
				break;
			default:
				throw new Exception("unsupported protocol scheme: {$scheme}");
		}
		if (isset($this->current_port)) {
			$port = $this->current_port;
		}
		
		if (array_key_exists($scheme . '_timeout', $this->_iniVars['import'])) {
			$timeout = $this->_iniVars['import'][$scheme . '_timeout'];
		} else {
			$timeout = 90;
		}
		
		if (array_key_exists($scheme . '_ls_timeout', $this->_iniVars['import'])) {
			$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_LS_TIMEOUT] = $this->_iniVars['import'][$scheme . '_ls_timeout'];
		}
		if (array_key_exists($scheme . '_reset_connection', $this->_iniVars['import'])) {
			$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_CURL_RESET_CONNECTION] = $this->_iniVars['import'][$scheme . '_reset_connection'];
		}
		if (array_key_exists($scheme . '_max_cache_items', $this->_iniVars['import'])) {
			$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_DIR_CACHE_ITEMS] = $this->_iniVars['import'][$scheme . '_max_cache_items'];
		}
		
		/*
		 * Added custom CURL options from t_repositories "attributes" column, specified
		 * as a json object like the following:
		 * {
  		 *   "curl_options": {
    	 *     "CURLOPT_PROXY": "adgs-dns.adgs.int:3128",
    	 *     "CURLOPT_VERBOSE": 1
    	 *   }
    	 * }
    	 * 
    	 * @see
    	 * http://projectit.exprivia.it/browse/CCDS-53?focusedCommentId=84519&page=com.atlassian.jira.plugin.system.issuetabpanels:comment-tabpanel#comment-84519
		 */
		if (array_key_exists('attributes', $this->_opVars) && $this->_opVars['attributes'] !== null) {
			$obj = @json_decode($this->_opVars['attributes']);
			if ($obj === null) {
				$this->_logHandler->warning("invalid json repository attributes: " . $this->_opVars['attributes']);
			} else {
				$curlOptions = array();
				if (property_exists($obj, 'curl_options')) {
					foreach (get_object_vars($obj->curl_options) as $optionName => $optionValue) {
						$curlOptions[constant($optionName)] = $optionValue;
						$this->_logHandler->debug("setting CURL_OPTION {$optionName} with value {$optionValue}");
					}
				}
				$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_CURL_OPTIONS] = $curlOptions;
				
				if (property_exists($obj, 'remote_timezone')) {
					$options[preimp_ftp_curl_new::FTP_CURL_NEW_OPTIONS_REMOTE_TIMEZONE] = $obj->remote_timezone;
				}
			}
		}
		
		return new preimp_ftp_curl_new(
			$scheme, 
			$this->current_host, 
			$this->current_account, 
			$this->current_password,
			$port,
			$timeout,
			$ftp_active_mode,
			$options);
	}

	/** This method check if a couple (or a set of files) has to be downloaded if it's true sets $this->_opVars['slave_ext']*/
	public function check_headers(){
		if($this->_opVars['generateheader']==trim($this->getDBHandler()->getSQLObj()->sql_boolean(false),'\'"')
				&& $this->_opVars['headerseparation']==trim($this->getDBHandler()->getSQLObj()->sql_boolean(true),'\'"')){
					$this->_opVars['slave_ext']=array($this->_opVars['dblext']);
				}		
	}
	
	protected function fetchRemoteContents() {

		$paths = is_array($this->current_remote_path) ? $this->current_remote_path : array($this->current_remote_path);
		
		$contents = array('dirs' => array(), 'files' => array());
		
		foreach ($paths as $path) {
			$this->_logHandler->debug("Attempt {$this->_opVars['retrycounter']}: connecting  {$path}");
			$newContents = $this->preimp_obj->getDirList($path,'all');
			
			$contents['dirs'] = array_merge($contents['dirs'], $newContents['dirs']);
			$contents['files'] = array_merge($contents['files'], $newContents['files']);
			
			if ($this->handleRemoteFilesMTime) {
				$this->_logHandler->debug("Downloading files timestamps");
				foreach ($this->preimp_obj->getItemListInfo($path) as $tmpItem) {
					$tmpItemClean = $tmpItem;
					unset($tmpItemClean['path']);	// reduces memory footprint for large directories
					$this->remoteFilesInfo[$tmpItem['path']] = $tmpItemClean;
				}
				$this->_logHandler->debug("Done downloading files timestamps");
			}
		}
		return $contents;
	}

	/** This method verifies all preconditions (regexp and partners presence) to download files from remote repository */
	public function precondition($userid) {
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		$this->check_headers();
		$ret = false;
        $contents=array();
		
		// try to connect, for failure increment retrycounter on db
        try{
        	$contents = $this->fetchRemoteContents();
        }catch(Exception $e) {
		    $this->_logHandler->err($e->getMessage());
	       	$this->incrementConnectionCounter();
        	ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
	        $this->_sysLogHandler->err("FAIL|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Cannot list files. {$e->getMessage()}\"|");
			$this->_logHandler->info("table t_receptionruleslock unlocked");
	       	throw $e;
	    }

	    $this->_sysLogHandler->notice("LOG|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Checking remote path: ".$this->_opVars['repo_remotepath']." on host: ".$this->_opVars['repo_hostname']."...\"|");			
		$file_rule_ar=ftp_special_rules::getRule_ar($this->_opVars['run_rule'],$this->_opVars['filetype'],$this->_dbHandler);
		
		//put availables partners into $partners_ar (master filename as key and partners filenames as value). 
		//If file has to be downloaded as a single file $partners_ar is null.
		$partners_ar=$this->checkPartners($contents['files']);
		//finding matches
		foreach ($contents['files'] as $filename){
			$name = basename($filename);
   			if($this->checkIfDownloaded($filename) && $this->_opVars['delete_on_remote']=='f') continue;
			foreach ($file_rule_ar as $file_rule){
				if (preg_match("/$file_rule/", $name) && (empty($partners_ar) || isset($partners_ar[$filename]))) {
					$this->pool_ref->found_files[] = $name;
					$this->found_files[] = $filename;
					$ret = true;
				}			
			}				
		}
		
		//insert partners in found files if they're defined:
		$this->addPartnersToListToDownload($partners_ar);

		foreach ($contents['dirs'] as $filename){
			$name = basename($filename);
   			if($this->checkIfDownloaded($filename) && $this->_opVars['delete_on_remote']=='f') continue;
			foreach ($file_rule_ar as $file_rule){
				if (preg_match("/$file_rule/", $name)) {
					$this->pool_ref->found_dirs[] = $name;
					$this->found_dirs[] = $filename;
					$ret = true;
				}			
			}				
		}
		
		//check if file belongs to another receptionrule with the same host and remote path 
		//before marking it as unexpected (if it has to be notified). 
		$this->checkFilestoMark($contents['files']);
		$this->checkFilestoMark($contents['dirs']);
		
		$this->_logHandler->debug(" found files: ".print_r($this->found_files,true));
		
		if (!$ret) {
			$this->_logHandler->notice("No data found in '{$this->_opVars['repo_name']}' matching the defined rules for filetype: '{$this->_opVars['filetype']}'");
        	ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
	    	$this->_sysLogHandler->notice("LOG|jobid=\"{$this->_opVars['repo_id']}\"|message=\"No files found for this rule on host: ".$this->_opVars['repo_hostname']."\"|");
			$this->_logHandler->info("table t_receptionruleslock unlocked");
		} else
			$this->_logHandler->notice(count($this->found_files)+count($this->found_dirs). " products found in '{$this->_opVars['repo_name']}' matching the defined rules for filetype: '{$this->_opVars['filetype']}'");
        	
		return $ret;
	}

	/** This method download remote repository files and save them to localpath */
    public function execute($userid){
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
        $founditems=array_merge($this->found_files,$this->found_dirs);
        $errors = false;
        
        $beginTime = microtime(true);
        foreach($founditems as $filename){
            $temp_output_file = $this->pds_temporary_dir.'/'.basename($filename);
            try{
	           	$this->_logHandler->debug("downloading $filename ...");
	            $start_time = acs_tools :: microtime_float();
	            $this->preimp_obj->getBinaryItem($temp_output_file, $filename);
	            $URL=$this->_opVars['repo_hostname'].$this->_opVars['repo_remotepath'];
	            $this->_sysLogHandler->notice("IN|jobid=\"{$this->_opVars['repo_id']}\"|fromURL=\"$URL\"|filename=\"$filename\"|");	  
	            $transfer_time = acs_tools :: microtime_float() - $start_time;
	            $file_size = filesize($temp_output_file) / (1024 * 1024);
		        $this->_logHandler->info("$filename downloaded to $temp_output_file ($file_size MB, $transfer_time sec, " . ($file_size / $transfer_time) . " MB/sec)");
		        
		        if ($this->handleRemoteFilesMTime) {
			        // set current mtime as the remote file mtime
			        $remoteMTime = $this->remoteFilesInfo[$filename]['age'];
			        if (!touch($temp_output_file, $remoteMTime)) {
			        	throw new Exception("Cannot set {$temp_output_file} mtime to {$remoteMTime}");
			        }
			        $this->_logHandler->info("{$temp_output_file} mtime set to {$remoteMTime} as remote file {$filename}");
		        }
		        
	            //replacement of the work usually  done by the do_import method of pds_import_child class 
	            //replacement done to guarantee a continous flux of data into pds final_dir
				$output_file = $this->_opVars['pds_final_dir'] . '/' . basename($temp_output_file);
				$this->_logHandler->debug("Moving {$temp_output_file} to {$output_file}");
				if (rename($temp_output_file, $output_file) === false) {
					$this->_logHandler->err("Cannot move file {$temp_output_file} to {$output_file}");
					throw new Exception("Cannot move file {$temp_output_file} to {$output_file}");	
				}		            
				//items written in the property copied_files of the object pool_ref are not strictly files in this case
	            //anyway they are written and used for the  delete         
	            $this->copied_files[] = $filename;							
			    if($this->_opVars['delete_on_remote']=='f') $this->markAsAlreadyDownloaded($filename);
	        } catch(Exception $e) {
		        $this->_logHandler->err($e->getMessage());
	            $this->_sysLogHandler->err("FAIL|jobid=\"{$this->_opVars['repo_id']}\"|message=\"Cannot download $filename.{$e->getMessage()}\"|");	 
				$this->markAsToNotify($filename,_DOWNLOADERROR_);
	            $errors = true; 
	        }
            
        }
        $endTime = microtime(true);
        $this->_logHandler->info("total execution time: " . sprintf("%0.2f sec", $endTime - $beginTime));
        
		if (is_array($this->copied_files)) $this->_logHandler->notice(count($this->copied_files). " products downloaded from '{$this->_opVars['repo_name']}' matching the defined rules for filetype: '{$this->_opVars['filetype']}'");        
        // mark lastdate for given provider if no errors else increment Connection Counter
        if(!$errors) $this->resetConnectionCounterAndLastVisit();
        else $this->incrementConnectionCounter();
        ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
		$this->_logHandler->info("table t_receptionruleslock unlocked");
	}
	
	//this method delete downloaded files from remote repositories if it has to be done
	public function cleanup($userid) {
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		if($this->_opVars['delete_on_remote']=='f')
			return;		
		if(is_array($this->copied_files)){
			foreach ($this->copied_files as $remote_file){
				try{ $this->preimp_obj->deleteFile($remote_file);}
				catch(Exception $e){
					ftp_tools::recursive_remove_directory($remote_file,$this->preimp_obj);
				}
    		}
		}
		
	}

    //this method constructs the array of founded partners for founded files if it has to be done
    private function checkPartners($contents){    	
    	//$this->_opVars['slave_ext']) is setted just if a couple has to be downloaded
    	if(!isset($this->_opVars['slave_ext'])) return null;
    	$partners_ar=array();
    	foreach($contents as $filename){
    		$path_parts = pathinfo($filename);	
			if ($path_parts['extension']!=$this->_opVars['hdrext']) continue;	
			foreach ($this->_opVars['slave_ext'] as $slave_ext){
				if(in_array($path_parts['dirname'].'/'.$path_parts['filename'].'.'.$slave_ext,$contents)){
	    			    	$partners_ar[$filename][]=$path_parts['dirname'].'/'.$path_parts['filename'].'.'.$slave_ext;
	 			}
				elseif(preg_match("/{$this->_opVars['run_rule']}/", $path_parts['basename'])){
	    				$this->markAsToNotify($filename,_WAITPARTNER_);
	    		}
	    	}
		}
    	return $partners_ar;
    }
 	
 	//this method adds partners filenames in found files
 	public function addPartnersToListToDownload($partners_ar){
		if(is_null($partners_ar)) return;
		$founded_count=count($this->pool_ref->found_files);
		for($i = 0; $i <= $founded_count; $i++){
			//$partners_ar[$this->found_files[$i]] exists all the time because it's in the if 
			//condition to add files to found_files array
			foreach($partners_ar[$this->found_files[$i]] as $partner){
				$this->found_files[]=$partner;
				$this->pool_ref->found_files[]=basename($partner);					
			}
			
		} 		
 	}
 	
 	//this method checks if a file is already been downloaded
    protected function checkIfDownloaded($name){
    	
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());		
		$this->_logHandler->info("Checking if file $name has already been downloaded");
		$sql = sprintf('SELECT * FROM t_receptionruleshist WHERE filename=%s AND receptionrule_id=%d 
						and status_id = (SELECT id FROM t_stati WHERE status = %s)', 
	     			$this->_dbHandler->getSQLObj()->sql_quote_str($name),
	     			$this->_opVars['repo_id'],
		    	$this->_dbHandler->getSQLObj()->sql_quote_str(_COMPLETED_));
	    $row = $this->_dbHandler->execQuery($sql, acs_idb::_SQL_GETROW);
		$IsDownloaded=!is_null($row);
		if($IsDownloaded) 
	     	$this->_logHandler->info("File $name already acquired on {$row['filearrival']}. Skipped!");
	   
	    return 	$IsDownloaded;     		
	}
	
	//this method checks if files that don't follow the regexp have to be marked
	private function checkFilestoMark($contents){
		foreach($contents as $filename){
			if($this->checkIfDownloaded($filename)) continue;
			if($this->_opVars['onbadnamingconvention'] && !in_array($filename,$this->found_files)){
				if(!$this->isRemotePathAllowedForFile($filename)) $this->markAsToNotify($filename,_BADNAMCONV_);		
			}
		}
	}

	//this method checks if a file exists into recetionruleshist table
	private function checkFileName($filename){
		$this->_logHandler->info("Checking if file $filename has already been notified");
		$sql = sprintf('SELECT st.status FROM t_receptionruleshist rulh
						INNER JOIN t_stati st on st.id = rulh.status_id
						WHERE rulh.filename=%s AND rulh.receptionrule_id=%d', 
	     			$this->_dbHandler->getSQLObj()->sql_quote_str($filename),
	     			$this->_opVars['repo_id']);
	    $row = $this->_dbHandler->execQuery($sql, acs_idb::_SQL_GETROW);
		return $row;
	}

	//this method marks a file as already downloaded
	private function markAsAlreadyDownloaded($filename, $status = _COMPLETED_){
			//check if insert or update
			$row = $this->checkFileName($filename);
			
			if ($this->handleRemoteFilesMTime && array_key_exists($filename, $this->remoteFilesInfo)) {
				$fileMTime = $this->remoteFilesInfo[$filename]['age'];
			} else {
				$fileMTime = null;
			}
			
			if (is_null($row)) {
				$sql = <<<SQL
INSERT INTO
	t_receptionruleshist (filename, receptionrule_id, tstamp, status_id, filecreationtstamp)
	VALUES
	(?, ?, now(), (SELECT id FROM t_stati WHERE status = ?), to_timestamp(?))
SQL;
				$this->_dbHandler->execQuery($sql, acs_idb::_SQL_EXEC, MDB2_FETCHMODE_ASSOC,
					array(
						$filename,
						$this->_opVars['repo_id'],
						$status,
						$fileMTime
					),
					array(
						_ACS_DBTYPE_TEXT,
						_ACS_DBTYPE_INT,
						_ACS_DBTYPE_TEXT,
						_ACS_DBTYPE_INT
					));
			} else {
				$sql = <<<SQL
UPDATE
	t_receptionruleshist
	SET filename = ?, tstamp = now(), status_id = (SELECT id FROM t_stati WHERE status = ?),
		filecreationtstamp = to_timestamp(?)
	WHERE filename = ? and receptionrule_id = ?
SQL;
				$this->_dbHandler->execQuery($sql, acs_idb::_SQL_EXEC, MDB2_FETCHMODE_ASSOC,
					array(
						$filename,
						$status,
						$fileMTime,
						$filename,
						$this->_opVars['repo_id']
					),
					array(
						_ACS_DBTYPE_TEXT,
						_ACS_DBTYPE_TEXT,
						_ACS_DBTYPE_INT,
						_ACS_DBTYPE_TEXT,
						_ACS_DBTYPE_INT
					));
			}
		
	}
	
	//this method marks a file as to notify
	private function markAsToNotify($filename, $notificationType) {
		//check if insert or update
		$row = $this->checkFileName($filename);
	    //if status is error leave it as it is
	    if ($row['status']==_ERROR_) return;
	    
		$this->markAsAlreadyDownloaded($filename, $notificationType);
	}

	//this method checks if there is another rule on the same repository that admits the given filename. 
	private function isRemotePathAllowedForFile($filename){
		$sql = sprintf('SELECT nameregularexpression FROM t_receptionrules rul
						INNER JOIN t_filetypes as ftyp on rul.filetype_id=ftyp.id_filetype 
						INNER JOIN t_repositories as rep on rul.repository_id=rep.id_repository
						INNER JOIN t_knownhosts hosts on hosts.id=rep.host_id
						WHERE hosts.host_name = %s and rep.remotepath = %s', 
		   	$this->_dbHandler->getSQLObj()->sql_quote_str($this->_opVars['repo_hostname']),
		    $this->_dbHandler->getSQLObj()->sql_quote_str($this->_opVars['repo_remotepath']));
		$rows=$this->_dbHandler->execQuery($sql, acs_idb::_SQL_GETALL);
		$isAllowed=false;
		foreach($rows as $row) {
			if (preg_match("/{$row['nameregularexpression']}/", basename($filename))) {
				$isAllowed=true;
				break;
			}
		}
		return $isAllowed;
	}
	
	//this method increments connections counter
	private function incrementConnectionCounter(){
		try{
			$sql = sprintf('UPDATE t_receptionrules set retrycounter = %d WHERE id_receptionrule = %d', 
		   		(int)$this->_opVars['retrycounter']+1,
		   	$this->_opVars['repo_id']);
			$this->_dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);				
		}catch(Exception $e){
		    $this->_logHandler->err($e->getMessage());
        	ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
	        $this->_sysLogHandler->err("FAIL|jobid=\"{$this->_opVars['repo_id']}\"|message=\" {$e->getMessage()}\"|");
			$this->_logHandler->info("table t_receptionruleslock unlocked");
	       	throw $e;  
        }		
	}

	//this method resets connections counter
	private function resetConnectionCounterAndLastVisit(){
		try{
			$sql = sprintf('UPDATE t_receptionrules set retrycounter = %d,lastgood = %s, lastvisit = %s  WHERE id_receptionrule = %d', 
		   	0,
		   	$this->_dbHandler->getSQLObj()->sql_auto_datetime(),
		   	$this->_dbHandler->getSQLObj()->sql_auto_datetime(),
		   	$this->_opVars['repo_id']);
			$this->_dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);			
		}catch(Exception $e){
		    $this->_logHandler->err($e->getMessage());
        	ftp_tools::unLockTable($this->_dbHandler,$this->_opVars['repo_id'],$this->_opVars['plugin_class']);
	        $this->_sysLogHandler->err("FAIL|jobid=\"{$this->_opVars['repo_id']}\"|message=\" {$e->getMessage()}\"|");
			$this->_logHandler->info("table t_receptionruleslock unlocked");
	       	throw $e;  
        }		
		$this->_logHandler->notice("Discovery on {$this->_opVars['repo_name']} done.");			
	}

}
?>