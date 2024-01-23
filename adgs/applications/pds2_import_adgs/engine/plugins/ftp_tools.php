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

require_once dirname(__FILE__) .'/ftp_defines.php';
require_once 'ACSPhpLib/acs_mdb2.php';
require_once 'ACSPhpLib/acs_exception.php';

class ftp_tools {
	
	public static function recursive_array_search($needle,$haystack) {
	    foreach($haystack as $key=>$value) {
	        $current_key=$key;
	        if($needle===$value || (is_array($value) && self::recursive_array_search($needle,$value) !== false)) {
	            return $current_key;
	        }
	    }
	    return false;
	}

	public static function orderFilesbyAge($contents) {
		$ordered_contents=array();
		foreach($contents as $item){
			//if age is greater than limit for a array index it will be the limit: 
			//N.B. it could be happen for ages > 18-07-2036
			$index=(($item['age'])>LIMIT_ARRAY_INDEX)?LIMIT_ARRAY_INDEX:($item['age']);
			$ordered_contents[$index][]=$item['path'];
			//echo "\n".date("ymdHms", $item['age']);
		}
		ksort($ordered_contents);
		return $ordered_contents;
	}

	public static function recursive_remove_directory($directory, &$tr) {
       
            $contents = $tr->getDirList($directory, 'all');
                      
            while (list (, $val) = each ($contents['files']) ) 
            	 $tr->deleteFile($val);
            	 
           	while (list (, $val) = each ($contents['dirs']) ) 
            	self::recursive_remove_directory($val,$tr);
            
            $tr->deleteEmptyDir($directory);
        
    }

	public static function lockTable(&$dbHandler,$runid,$plClass,$ruleslock_max_time){
		// try to lock:
		// if it's already locked and the time of lock is inside the configured max time of lock throws an exception
		// if it's already locked and the time of lock is greater than configured max time of lock remove lock's record 
		try{
			//If tstamp > configured max time delete before lock
						$sql=sprintf('DELETE FROM t_receptionruleslock WHERE tstamp < %s - %s::interval AND receptionrule_id = %d AND responsible = %s', 
		    		$dbHandler->getSQLObj()->sql_auto_datetime(),
		    		$dbHandler->getSQLObj()->sql_quote_str($ruleslock_max_time),
		    		$runid,
		    		$dbHandler->getSQLObj()->sql_quote_str($plClass));
		    $dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);
			
			
			$sql=sprintf('INSERT INTO t_receptionruleslock (receptionrule_id,responsible,tstamp) VALUES (%d,%s,%s)', 
		    		$runid,
		    		$dbHandler->getSQLObj()->sql_quote_str($plClass),
			    	$dbHandler->getSQLObj()->sql_auto_datetime());
		    $dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);
		}catch(Exception $e){
	       	throw new ex_notime_to_unlock("No time to unlock the table. " . $e->getMessage()); 
		}
							
	}	
	
	public static function unLockTable(&$dbHandler,$runid,$plClass){
		//with db connection problem try to unlock for 3 times with a waiting of 10 seconds
		//If it's no locked throws an exception
        $unlock_attempts=1;
        while(true){
			try{
				$sql=sprintf('DELETE from t_receptionruleslock WHERE receptionrule_id=%d and responsible=%s', 
			    		$runid,
			    		$dbHandler->getSQLObj()->sql_quote_str($plClass));
			    $dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);
			    break;
			}catch(acs_exDBErrorConnection $e){
		       	if($unlock_attempts>3) throw $e;
				sleep(10); 
				$unlock_attempts++;
			}catch(Exception $e){
		       	throw $e;
			}
        }
		
	}

	public static function checkAvailableSpace($tmpPath,$destPath,$minTmpSpace,$minDestSpace,&$logHandler) {
		//convert the minimum disk space configured from MB to Byte
		$min_tmp_free_disk = $minTmpSpace * 1024 * 1024;
		$min_des_free_disk = $minDestSpace * 1024 * 1024;
		
		$logHandler->debug("Free disk Space reserved for the temporary directory: $tmpPath is: $minTmpSpace MB");
		$logHandler->debug("Free disk Space reserved for the destination directory: $destPath is: $minDestSpace MB");
		
		//calculate the available disk space
		$tmp_fds = disk_free_space(dirname($tmpPath));
		$des_fds = disk_free_space($destPath);
		$tmp_fdsMB = $tmp_fds/(1024 * 1024);
		$des_fdsMB = $des_fds/(1024 * 1024);
		
		$logHandler->debug("Disk free space available on the corresponding filesystem or disk partition for the temporary directory is: {$tmp_fdsMB} (MB)");
		$logHandler->debug("Disk free space available on the corresponding filesystem or disk partition for the destination directory is: {$des_fdsMB} (MB)");
		
		//calculate the available disk space preserving the configured minimum disk space
		$tmp_avail_space = disk_free_space(dirname($tmpPath)) - $min_tmp_free_disk;
		$des_avail_space = disk_free_space($destPath) - $min_des_free_disk;
		if ( $tmp_avail_space <= 0 || $des_avail_space <= 0 ){
			return false;
		} 
	}

	public static function rollDownloadHistory(&$dbHandler, $ruleId, $timeThreshold) {
	    try{
    	    $sql = "DELETE FROM t_receptionruleshist
                    WHERE tstamp < now() - '$timeThreshold days'::interval
                    AND receptionrule_id = $ruleId
    				AND status_id IN (SELECT id FROM t_stati WHERE status = 'COMPLETED')";
    	    $dbHandler->execQuery($sql, acs_idb::_SQL_EXEC);
    	}catch(Exception $e){
    	    throw new Exception("Cannot delete from t_receptionruleshist table. " . $e->getMessage());
    	}
	}
}



class ex_notime_to_unlock extends acs_exception {}



?>