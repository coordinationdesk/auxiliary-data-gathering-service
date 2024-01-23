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

namespace Acs\PHPImport\Service\Tools;

use Acs\PhpTools\CmdExec;
use Acs\PHPImport\Config\Config;
use Acs\PHPImport\Service\Log\PHPImportLogger;

define('_ERRORWORK_', 'ERROR');

class PluginTools {
	
	public static function recursive_array_search($needle,$haystack) {
	    foreach($haystack as $key=>$value) {
	        $current_key=$key;
	        if($needle===$value OR (is_array($value) && self::recursive_array_search($needle,$value) !== false)) {
	            return $current_key;
	        }
	    }
	    return false;
	}

	public static function createTempDir($baseTempDir, $identifier) {
	    $attempts = 1;
	    do {
	    	$nanoTime = microtime(true);
	    	$tmpDir = $baseTempDir . '/' . $identifier . $nanoTime;
	    	$dirAlreadyExists = file_exists($tmpDir);
	        $attempts++;
	        if ($attempts == 10) {
	            break;
	        }
	        if ($dirAlreadyExists) {
	        	// try to avoid future collisions, sleep a random
	        	// time from 0 to 30 milliseconds
	        	usleep(rand(0, 30000));
	        }
	    } while ($dirAlreadyExists);
	    
	    if (mkdir($tmpDir) === false) {
	        $error = error_get_last();
	        throw new \Exception($identifier . " : Cannot make temporary directory: {$tmpDir}. " . $error['message']);
	    }
	    
	    return $tmpDir;
	}

	public static function removeTempDir($tmpDir,$identifier){
	    
        if (is_dir($tmpDir)) {
            $objects = scandir($tmpDir);
                foreach ($objects as $object) {
                    if ($object != "." && $object != "..") {
                        if (is_dir($tmpDir. DIRECTORY_SEPARATOR .$object) && !is_link($tmpDir."/".$object)){
                            self::removeTempDir($tmpDir. DIRECTORY_SEPARATOR .$object,$identifier);
                        } else{
                            unlink($tmpDir. DIRECTORY_SEPARATOR .$object);
                        }
                    }
                }
                if (rmdir($tmpDir)===false){
                    $error=error_get_last();
                    throw new \Exception($identifier. " : Cannot remove temporary directory: {$tmpDir}. ".$error['message']);
                }	   
        }
	}
	
	public static function cleanHistoryFromEvictedCompletedProducts($dbHandler, $ruleId, $timeThreshold) {
		$tm = Config::get()->getTablesMapping();
		
        $sql = "DELETE FROM {$tm['t_srv_receptionruleshist']}
                WHERE evictiondate < now() - '$timeThreshold seconds'::interval
                AND receptionrule_id = $ruleId
				AND status_id IN (SELECT id FROM {$tm['t_srv_statuses']} WHERE status = 'COMPLETED')";
        $stmt = $dbHandler->prepare($sql);
        $stmt->execute();
        $error = $stmt->errorInfo();
        if ($error[0] !== '00000') {
            throw new \Exception(print_r($error,true));
        }
	}
	
	public static function lockTable($dbHandler,$ruleId,$plClass,$ruleslock_max_time,$identifier){
		// try to lock:
		// if it's already locked and the time of lock is greater than configured max time of lock remove lock's record 
		try{
			$tm = Config::get()->getTablesMapping();
			
			//If tstamp > configured max time delete before lock
		    $sql="DELETE from {$tm['t_srv_receptionruleslock']}
                    WHERE tstamp < now() - '$ruleslock_max_time'::interval 
                    AND eceptionrule_id = $ruleId AND responsible='$plClass'";
		    $stmt=$dbHandler->prepare($sql);
		    $stmt->execute();
		    $error=$stmt->errorInfo();
		    if($error[0]!== '00000'){
		        throw new \Exception(print_r($error,true));
		    }
			
			$sql="INSERT INTO {$tm['t_srv_receptionruleslock']} (receptionrule_id,responsible,tstamp) 
                    VALUES ($ruleId,'$plClass',now()";
			$stmt=$dbHandler->prepare($sql);
			$stmt->execute();
			$error=$stmt->errorInfo();
			if($error[0]!== '00000'){
			    throw new \Exception(print_r($error,true));
			}
		}catch(\Exception $e){
	       	throw $e; 
		}
							
	}	

	public static function unLockTable($dbHandler,$ruleId,$plClass,$identifier){
		//with db connection problem try to unlock for 3 times with a waiting of 10 seconds
		//If it's no locked throws an exception
        $unlock_attempts=1;
        while(true){
			try{
				$tm = Config::get()->getTablesMapping();
				
			    $sql="DELETE from {$tm['t_srv_receptionruleslock']} 
                    WHERE receptionrule_id=$ruleId AND responsible='$plClass'";
			    $stmt=$dbHandler->prepare($sql);
			    $stmt->execute();
			    $error=$stmt->errorInfo();
			    if($error[0]!== '00000'){
			        throw new \Exception(print_r($error,true));
			    }
			    $stmt->closeCursor();
			    $stmt = null; 
			    break;
			}catch(\Exception $e){
		       	if($unlock_attempts>3) throw $e;
				sleep(10); 
				$unlock_attempts++;
			}catch(\Exception $e){
		       	throw $e;
			}
        }
		
	}
	
	public static function extendReceptionRuleLock($dbHandler, $ruleId) {
		$tm = Config::get()->getTablesMapping();
		
		$sql = "UPDATE {$tm['t_srv_receptionruleslock']} SET tstamp = now() WHERE receptionrule_id=$ruleId";
		$stmt = $dbHandler->prepare($sql);
		$stmt->execute();
		$error = $stmt->errorInfo();
		if($error[0] !== '00000'){
			throw new \Exception(print_r($error, true));
		}
	}

	public static function setConnectionCounterAndLastTimes($dbHandler,$ruleId,$retryCounter,
	    $insertVisitTstamp,$insertGoodTstamp,$identifier){  
	    try{
	    	$tm = Config::get()->getTablesMapping();
	    	
	        $visitTstamp=($insertVisitTstamp===true)?', lastvisit=now() ':'';
	        $goodTstamp=($insertGoodTstamp===true)?', lastgood=now() ':'';
	        $sql="UPDATE {$tm['t_srv_receptionrules']}
              SET retrycounter= :retryCounter
              $visitTstamp $goodTstamp
            WHERE id_receptionrule =:ruleId";
	        
	        
	        $stmt=$dbHandler->prepare($sql);
	        $stmt->bindValue(':ruleId', $ruleId);
	        $stmt->bindValue(':retryCounter', $retryCounter);
	        $stmt->execute();
	        $error=$stmt->errorInfo();
	        if($error[0]!== '00000'){
	            throw new \Exception($identifier.": {$tm['t_srv_receptionrules']} update failed.".print_r($error,true));
	        }
	        $stmt->closeCursor();
	        $stmt = null; 
	    }catch(\Exception $e){
	        throw new $e;
	    }

	}	

	public static function getRuleAr($dbHandler,$rule,$fileTypeId,$identifier){
	    if ($rule != null) return array($rule);
	    try{
	        $ruleAr=self::getChildrensRules($dbHandler,$fileTypeId,$identifier);
	    }catch(\Exception $e){
	        throw $e;
	    }
	    
	    return $ruleAr;
	}
	
	public static function getChildrensRules($dbHandler,$fileTypeId,$identifier){
	    
		$tm = Config::get()->getTablesMapping();
		
	    $rowData=$dbHandler->query("SELECT nameregularexpression FROM t_filetypes 
                            WHERE id_filetype IN
				(SELECT cfiletype FROM {$tm['t_filetypescomps']} where pfiletype = '$fileTypeId')");
	    
	    $ruleAr=null;
	    foreach($rowData as $row) {
	        $ruleAr[]=$row['nameregularexpression'];
	    }
	   
	    if ($ruleAr==null){
	        throw new \Exception ("$identifier : File tipe id: $fileTypeId has a 
nameregularexpression null, but no children in t_filetypescomp!");	        
	    }
	        
	    return $ruleAr;	
	}	

	/** this method update the t_srv_receptionruleshist table in order to mark that a file is in a certain status
	*/
	public static function markAs($dbHandler, $fileName, $status, $ruleId, $creationTstamp, $identifier, $remoteUuid = null, $evictionDate = null) {
		$row = self::checkFileName($dbHandler,$ruleId,$fileName);
		if (isset($row) && $row[0]['status'] === _ERRORWORK_) return;
	        
		$tm = Config::get()->getTablesMapping();

		$sql = is_null($row) ? 
				"INSERT INTO {$tm['t_srv_receptionruleshist']} (filename, receptionrule_id,
    				tstamp, filecreationtstamp, status_id, remoteuuid, evictiondate) VALUES (:fileName, :ruleId, now(), :creationTstamp,
                    (SELECT id FROM {$tm['t_srv_statuses']} WHERE status = :status), :remoteuuid, :evictiondate)" :
	            "UPDATE {$tm['t_srv_receptionruleshist']} 
					SET filename =:fileName, tstamp = now(),
                    	filecreationtstamp = :creationTstamp, status_id = (SELECT id FROM {$tm['t_srv_statuses']} WHERE status = :status),
						remoteuuid = :remoteuuid, evictiondate = :evictiondate
   					WHERE filename = :fileName and receptionrule_id = :ruleId ";

		$stmt = $dbHandler->prepare($sql);
		$stmt->bindValue(':ruleId', $ruleId);
		$stmt->bindValue(':fileName', $fileName, \PDO::PARAM_STR);
		$stmt->bindValue(':status', $status, \PDO::PARAM_STR);
		$stmt->bindValue(':creationTstamp', $creationTstamp, \PDO::PARAM_STR);
		$stmt->bindValue(':evictiondate', empty($evictionDate) ? null : $evictionDate, \PDO::PARAM_STR);
		$stmt->bindValue(':remoteuuid', $remoteUuid, \PDO::PARAM_STR);
		$stmt->execute();
		$error=$stmt->errorInfo();
		if ($error[0]!== '00000') {
			throw new \Exception("{$tm['t_srv_receptionruleshist']} update failed.".print_r($error,true));
		}
		$stmt->closeCursor();
		$stmt = null;
	}

	/** this method checks if a file exists in the t_srv_receptionruleshist table and return its status
	 */
	public static function checkFileName($dbHandler,$ruleId,$filename) {
		$tm = Config::get()->getTablesMapping();
		
	    $rowData = $dbHandler->query("SELECT st.status FROM {$tm['t_srv_receptionruleshist']} rulh
							INNER JOIN {$tm['t_srv_statuses']} st on st.id = rulh.status_id
							WHERE rulh.filename='$filename' AND rulh.receptionrule_id=$ruleId");
		
	    $rowStatus=null;
		foreach($rowData as $row) {
		    $rowStatus[]=array_diff_key($row, range(1, 2));
		}
		
		return $rowStatus;
	}
	
	public static function getMaxFileCreationTimeStampForRule(\PDO $dbHandler, $idRule, $msec) {
		$tm = Config::get()->getTablesMapping();
		
		$sql = <<<SQL
SELECT 
	to_char(max(filecreationtstamp) - interval '{$msec} millisecond', 'YYYY-MM-DD"T"HH24:MI:SS.MS"Z"')
FROM {$tm['t_srv_receptionruleshist']} WHERE receptionrule_id = {$idRule}
SQL;
		$rows = $dbHandler->query($sql);
		return $rows->fetchColumn(0);
	}
	
	public static function decryptPassword($password, $cdl) {
		// Decrypt pwd
		$decrypterCdl = sprintf('%s %s', $cdl, escapeshellarg($password));
		$decrypterCmd = new CmdExec($decrypterCdl);
		$return_var = $decrypterCmd->execute(false);
		if ($return_var !=0 ) {
			throw new \Exception("$decrypterCdl exited with $return_var ");
		}
		return $decrypterCmd->getStdout();
	}
	
	public static function getRepoAttributesCurlOptions($repoAttributes) {
		/*
		 * Handle additional curl options specified in json repo attributes as
		 *	{
		 * 		"curl_options": {
		 * 			"CURLOPT_VERBOSE": 1,
		 * 			"CURLOPT_PROXY": "proxy.project.int:1234"
		 * 		}
		 * 	}
		 *
		 */
		$repoCurlOptions = array();
		if ($repoAttributes != null && property_exists($repoAttributes, 'curl_options')) {
			if (is_object($repoAttributes->curl_options)) {
				foreach (get_object_vars($repoAttributes->curl_options) as $curlOption => $value) {
					$curlOptionConstant = @constant($curlOption);
					if ($curlOptionConstant == null) {
						PHPImportLogger::get()->warn("Unknown CURL constant {$curlOption} in repo attributes");
					} else {
						$repoCurlOptions[$curlOptionConstant] = $value;
					}
				}
			}
		}
		return $repoCurlOptions;
	}
	
	public static function mixCurlOptions($defaultCurlOptions, $repoCurlOptions) {
		$outputArray = $defaultCurlOptions;
		foreach ($repoCurlOptions as $k => $v) {
			$outputArray[$k] = $v;
		}
		
		return $outputArray;
	}
	
	public static function getRepositoryUrlFromPayload(array $rulePayload) {
		if (!array_key_exists('repo_protocol', $rulePayload))
			throw new \RuntimeException("missing repo_protocol in rule payload");
		if (!array_key_exists('repo_hostname', $rulePayload))
			throw new \RuntimeException("missing repo_hostname in rule payload");
		if (!array_key_exists('repo_port', $rulePayload))
			throw new \RuntimeException("missing repo_port in rule payload");
		if (!array_key_exists('repo_remotepath', $rulePayload))
			throw new \RuntimeException("missing repo_remotepath in rule payload");
		
		$protocol = $rulePayload['repo_protocol'];
		$hostName = $rulePayload['repo_hostname'];
		$port = $rulePayload['repo_port'];
		$remotePath = $rulePayload['repo_remotepath'];

		//configuration check
		if (stripos($protocol, 'http') === false) {
			throw new \Exception("protocol: {$protocol} not managed");
		}
		if (empty($hostName)) {
			throw new \Exception("no PRIP hostname configured!");
		}
		
		$url = (empty($port)) ? $protocol . '://' . $hostName . $remotePath : $protocol . '://' . $hostName . ':' . $port . $remotePath;
		return $url;
	}
	
	public static function getRepositoryCredentialsFromPayload(array $rulePayload, Config $config) {
		if (!array_key_exists('repo_password', $rulePayload))
			throw new \RuntimeException("missing repo_password in rule payload");
		if (!array_key_exists('repo_username', $rulePayload))
			throw new \RuntimeException("missing repo_username in rule payload");
				
		$password = PluginTools::decryptPassword(
			$rulePayload['repo_password'], 
			$config->getDecrypterCmdFullPath());

		return rtrim($rulePayload['repo_username'] . ':' . $password, "\n"); // remove trailing new lines
	}
}

?>