<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2012 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
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
// $Prod: pds2_import $  
// $Author$
// $Id$
// $Revision$
//

require_once 'pds2_imp_pl_base.php';
require_once dirname(__FILE__) . '/../pds2_file_transfer_tools.php';

define("DELETE_MODE_RECURSIVE",	"recursive");	// delete files and folders contained in the repo_uri
define("DELETE_MODE_PRUNE",		"prune");		// delete the repo_uri path and all its contents
define("DELETE_MODE_FLAT",		"flat");		// delete only the contents of the master repo_uri path

define("DELETE_TARGET_DIRS",	"dirs");		// delete matching directories (and their contents)
define("DELETE_TARGET_FILES",	"files");		// delete matching files only

class pds2_imp_pl_cleaner extends pds2_imp_pl_base {
	protected $parsedURI, $tr;

	public function initialize($userid) {
		parent::initialize($userid);
		
		$this->pool_ref->transaction_ready = true;
		$this->parsedURI = parse_url($this->_opVars['repo_uri']);
		$this->tr = new preimp_wrapper(	$this->parsedURI['scheme'],
										$this->parsedURI['host'],
										$this->parsedURI['user'],
										$this->parsedURI['pass'],
										$this->parsedURI['port']);
	}

	public function precondition($userid) {
		try {
			$this->tr->getDirList($this->parsedURI['path']);
		} catch (Exception $e) {
			$this->_logHandler->warning("Repo uri {$this->_opVars['repo_uri']} does not exist: " . $e->getMessage());
			return false;
		}
		return true;
	}
	
    public function execute($userid) {
    	//var_dump($this->_opVars);
//    	var_dump($this->_opVars['run_rule_config']);
    	//echo "DIR: {$this->_opVars['repo_uri']}\n";
		$path = $this->parsedURI['path'];
		$delete_mode = $this->_opVars['run_rule_config']['delete_mode'];
		$delete_pattern = '/' . $this->_opVars['run_rule_config']['delete_pattern'] . '/';
		$delete_target = $this->_opVars['run_rule_config']['delete_target'];
		
		$this->_logHandler->notice("Proceding with {$this->_opVars['repo_uri']} deletion");
		if ($delete_mode == DELETE_MODE_PRUNE) {
			$this->deleteFiles($path, true, true, true, '/.*/');
			$this->tr->deleteEmptyDir($path);
		} else {
			$this->deleteFiles(	$path, 
								$delete_mode == 'recursive',
								$delete_target == 'files' || $delete_target == 'all',
								$delete_target == 'dirs' || $delete_target == 'all',
								$delete_pattern);
		}
    }
    
    public function deleteFiles($start_path, $recursive, $delete_files, $delete_dirs, $delete_mask) {
    	$contents = $this->tr->getDirList($start_path, 'a');
    	
    	if ($delete_files)
	    	foreach ($contents['files'] as $file)
	    		if (preg_match($delete_mask, $file))
	    			$this->tr->deleteFile($file);

	    if ($delete_dirs || $recursive)
	    	foreach ($contents['dirs'] as $dir) {
				if ($delete_dirs && preg_match($delete_mask, $dir)) {
					// this directory matches the pattern and must be deleted with all its children
					$this->deleteFiles($dir, true, true, true, '/.*/');
					$this->tr->deleteEmptyDir($dir);
				} else
					$this->deleteFiles($dir, $recursive, $delete_files, $delete_dirs, $delete_mask);
	    	}
    }
}

?>