<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions 4 and 5                                                      |
// +---------------------------------------------------------------------------+
// | Copyright (c) 1995-2006 - Advanced Computer Systems S.p.A.                |
// | Copyright 1995-2007, Advanced Computer Systems ,Inc.                      |
// | Via Della Bufalotta, 378 - 00139 Roma - Italy                             |
// | http://www.acsys.it                                                       |
// | All rights reserved.                                                      |
// +---------------------------------------------------------------------------+
// | This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems; |
// | the contents of this file may not be disclosed to third parties, copied or|
// | duplicated in any form, in whole or in part, without the prior written    |
// | permission of Advanced Computer Systems, Inc.                             |
// +---------------------------------------------------------------------------+
//
// $Prod:  $
// $Author$
// $Id$
// $Revision$
//

require_once 'pds2_imp_pl_base.php';
require_once dirname(__FILE__) . '/../pds2_file_transfer_tools.php';

class pds2_imp_pl_simple_dir_harvester extends pds2_imp_pl_base {
	protected	$preimp_obj = null,
				$current_remote_path,
				$current_account,
				$current_host,
				$current_password,
				$current_port,
				$found_files = array();

	public function initialize($userid) {
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		
		parent::initialize($userid);
		
		printf("%-10s %-80s %-10s %-10s\n", $this->_opVars['repo_name'], $this->_opVars['repo_uri'], $this->_opVars['run_rule_name'], $this->_opVars['run_rule']);

        // connect to the URI
        $uri 				= $this->_opVars['repo_uri'];
		$parsedURI 			= parse_url($uri);
		$scheme				= strtolower(trim($parsedURI['scheme']));
		if($scheme == '') {
			$this->_logHandler->warning("Scheme empty, assuming it is 'file'");
			$scheme = 'file';	
		}

		$this->current_account	= $parsedURI['user'];
		$this->current_host		= $parsedURI['host'];
		$this->current_password	= $parsedURI['pass'];
		$this->current_port		= $parsedURI['port'];
		$this->current_remote_path= $parsedURI['path']; 

		$this->_logHandler->debug("Using the '$scheme' connector");
		$this->_logHandler->debug("PDS TMP DIR: {$this->pds_temporary_dir}, PDS FINAL DIR: {$this->pds_final_dir}");

		$this->preimp_obj = new preimp_wrapper($scheme, $this->current_host, $this->current_account, $this->current_password, $this->current_port); 
        $this->_logHandler->debug("Connected as $scheme://{$this->current_account}@{$this->current_host}{$this->current_remote_path}");
	}

    public function execute($userid){
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		parent::execute($userid);

        // get list of files
        //$contents = $this->preimp_obj->getDirList($this->current_remote_path);
        $this->_logHandler->debug("Files on remote root directory {$this->current_remote_path} ({$this->current_account}):");

        // foreach single file
        foreach($this->found_files as $filename){
			$baseFilename = basename($filename);
            if($baseFilename=='.' || $baseFilename=='..' ||  $baseFilename=='_temporary_') continue;

            // try to download $server_file and save to $local_file
            $local_file = $this->pds_temporary_dir.'/'.basename($filename);
            try{
            	$start_time = acs_tools :: microtime_float();
            	$this->preimp_obj->getBinaryFile($local_file, $filename);
            	$transfer_time = acs_tools :: microtime_float() - $start_time;
            	$file_size = filesize($local_file) / (1024 * 1024);
	            $this->_logHandler->debug("Product $filename downloaded to $local_file ($file_size MB, $transfer_time sec, " . ($file_size / $transfer_time) . " MB/sec)");
            	$this->copied_files[] = $filename;
            	$this->pool_ref->transferred_files[] = $filename;
				$this->pool_ref->output_files[] = $local_file;
            } catch(Exception $e) {
                $this->_logHandler->err($e->getMessage());
            }

        }
    }
    
	public function precondition($userid) {
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());

		if ($this->simulation)
			return true;

//		print_r($this->_opVars);
		$ret = false;
		
		$contents = $this->preimp_obj->getDirList($this->current_remote_path);

		foreach ($contents as $filename) {
			$name = basename($filename);
			if (preg_match("/{$this->_opVars['run_rule']}/", $name)) {
				$this->pool_ref->found_files[] = $name;
				$this->found_files[] = $filename;
				$ret = true;
			}
		}
		
		if (!$ret)
			$this->_logHandler->warning("precondition failed: rule = {$this->_opVars['run_rule']}, files = " . print_r($this->pool_ref->found_files, true));
		return $ret;
	}

    public function cleanup($userid) {
    	$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
    	
    	foreach ($this->copied_files as $remote_file) {
    		// throws an exception if needed
    		$this->preimp_obj->deleteFile($remote_file);
    	}
    }
    
    public function cleanupTemp($userid) {
    	$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
        $this->preimp_obj->closeConnection();
    }
}

?>