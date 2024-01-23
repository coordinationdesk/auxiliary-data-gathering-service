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

require_once dirname(__FILE__) .'/ftp_imp_pl_adgs.php';

/**
 * Extension of the base ftp_impl_pl_adgs class,
 * download same files only if modification timestamp is newer
 * than the t_receptionruleshist
 * 
 * @author andrea
 *
 */
class ftp_imp_pl_adgs_newer extends ftp_imp_pl_adgs {
	
	public function __construct($dbHandler, $logHandler, $storageHandler, $iniVars, $opVars) {
		parent :: __construct($dbHandler, $logHandler, $storageHandler, $iniVars, $opVars);

		if ($this->handleRemoteFilesMTime == false) {
			$this->_logHandler->warning("cannot set handleRemoteFilesMTime to 'false' with ftp_imp_pl_adgs_newer plugin");
		}
		$this->handleRemoteFilesMTime = true;
	}
	
	
	//this method checks if a file is already been downloaded
	protected function checkIfDownloaded($name){
		
		$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
		$this->_logHandler->info("Checking if file $name has already been downloaded");
		
		$sql = <<<SQL
SELECT *, extract(epoch from filecreationtstamp) mtime  
	FROM t_receptionruleshist 
	WHERE 
		filename = ? AND 
		receptionrule_id = ? AND 
		status_id = (SELECT id FROM t_stati WHERE status = ?)
SQL;
		$row = $this->_dbHandler->execQuery($sql, acs_idb::_SQL_GETROW, MDB2_FETCHMODE_ASSOC,
			array(
				$name,
				$this->_opVars['repo_id'],
				_COMPLETED_
			),
			array(
				_ACS_DBTYPE_TEXT,
				_ACS_DBTYPE_INT,
				_ACS_DBTYPE_TEXT
			));
		
		$isDownloaded = !is_null($row);
		
		if ($isDownloaded) {
			$remoteMTime = $this->remoteFilesInfo[$name]['age'];
			if ($remoteMTime <= $row['mtime']) {
				$this->_logHandler->info("File $name already acquired, remote mtime {$remoteMTime}, history mtime is {$row['mtime']}. Skipped!");
			} else {
				$this->_logHandler->info("File $name already acquired but remote mtime {$remoteMTime} is newer than history mtime {$row['mtime']}. Downloading again");
				$isDownloaded = false;
			}
		}
		
		return 	$isDownloaded;
	}
	
}

?>