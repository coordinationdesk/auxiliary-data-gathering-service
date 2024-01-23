<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2011 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
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

interface pds2_file_transfer {
	public function getDirList($dirname, $what = 'd');
	public function getBinaryFile($local_file, $remote_file);
	public function putBinaryFile($local_file, $remote_file);
	public function deleteFile($remote_file);
	public function deleteEmptyDir($remote_dir);
	public function closeConnection();
}

class preimp_wrapper implements pds2_file_transfer {
	protected $context;
	protected $write_context;
	protected $block_size = 8192;
	public    $udm_filesize = array( 
	        'B' => 1, 
	        'KB' => 1024, 
	        'MB' => 1048576, 
	        'GB' => 1073741824, 
	        'TB' => 1099511627776,
	        'PB' => 1125899906842624 
    ); 
	
	public function __construct($protocol, $host, $username, $password, $port = null) {
		$this->context = "{$protocol}://{$username}";
		if (isset($password))
			$this->context .= ":{$password}";
		if (isset($host))
			$this->context .= "@{$host}";
		if (isset($port))
			$this->context .= ":{$port}";
		
		$scheme_list = stream_get_wrappers();
		if (!in_array($protocol, $scheme_list))
			throw new Exception("Unsupported scheme: $protocol");
		
		$this->write_context = stream_context_create(array('ftp' => array('overwrite' => true)));
	}
	
	public function __destruct() {
		$this->closeConnection();
	}

	public function getDirList($dirname, $what = 'd') {
		$dir = $this->context . $dirname;  
		$files = array();
		$dirs = array();
		try{
			$flag=FilesystemIterator::SKIP_DOTS ;
			$i = new  RecursiveDirectoryIterator($dir,$flag);	
		}catch(Exception $e){
			throw new Exception($e->getMessage().". Cannot read directory $dirname");
		}
		//list all files/dirs
		while($i->valid()) {
			if ($i->isDir()) $dirs[]=$dirname.'/'.$i->getFilename();
			else $files[]= $dirname.'/'.$i->getFilename();
			$i->next();			
		}

		switch ($what) {
			case 'd':	return $dirs;
			case 'f':	return $files;
			default:	return array('dirs' => $dirs, 'files' => $files);
		}
	}

	public function getDirSize($dirname,$udm='B') { 
    	$dir = $this->context . $dirname;
		try{    	 
			$flag=FilesystemIterator::SKIP_DOTS ;
			$i = new  RecursiveDirectoryIterator($dir,$flag);
		}catch(Exception $e){
			throw new Exception($e->getMessage().". Cannot read directory $dirname");
		}		
		
		$size=0;
		while($i->valid()) {		
			if ($i->isDir()) $size+=$this->getDirSize($dirname.'/'.$i->getFilename());
			else $size+=$i->getSize();
			$i->next();			
		}
    	return $size/$this->udm_filesize[$udm]; 
	}

	public function getItemListInfo($dirname) {
		$dir = $this->context . $dirname; 
		$items=array();
		try{
			$flag=FilesystemIterator::SKIP_DOTS ;
			$i = new  RecursiveDirectoryIterator($dir,$flag);	
		}catch(Exception $e){
			throw new Exception($e->getMessage().". Cannot read directory $dirname");
		}
		//list all files/dirs
		while($i->valid()) {
			if ($i->isDir()) {
				$items[]=array(
					'path'=>$dirname.'/'.$i->getFilename(),
					'size'=>$this->getDirSize($dirname.'/'.$i->getFilename()),
					'age'=>$this->getDirAge($dirname.'/'.$i->getFilename()),
					'dir'=>true);
			}
			else {
				$items[]= array(
					'path'=>$dirname.'/'.$i->getFilename(),
					'size'=>$i->getSize(),
					'age'=>$i->getMTime(),
					'dir'=>false);
			}
			
			$i->next();			
		}
		return $items;
	}

	public function getDirAge($dirname) { 
    	$dir = $this->context . $dirname;
		try{    	 
			$flag=FilesystemIterator::SKIP_DOTS ;
			$i = new  RecursiveDirectoryIterator($dir,$flag);
		}catch(Exception $e){
			throw new Exception($e->getMessage().". Cannot read directory $dirname");
		}		
		
		$age=0;
		while($i->valid()) {		
			if ($i->isDir()) $age=max($age,$this->getDirAge($dirname.'/'.$i->getFilename()));
			else $age=$i->getMTime();
			$i->next();			
		}
    	return $age; 
	}

	public function copyFile($input_file, $output_file) {
		$fi = fopen($input_file, 'r');
		if ($fi === false)
			throw new Exception("Cannot open input file $input_file");

		$fo = fopen($output_file, 'w', false, $this->write_context);
		if ($fo === false) {
			fclose($fi);
			throw new Exception("Cannot open output file $output_file");
		}
		
		try {
			do {
				$str = fread($fi, $this->block_size);
				if ($str === false)
					throw new Exception("Cannot read input file $input_file");
				$write_ret = fwrite($fo, $str);
				if ($write_ret === false || $write_ret != strlen($str))
					throw new Exception("Cannot write to output file $output_file");
			} while (strlen($str) == $this->block_size);
		} catch (Exception $e) {
			fclose($fi);
			fclose($fo);
			throw $e;
		}
		
		fclose($fi);
		fclose($fo);
	}
	
	public function getBinaryFile($local_file, $remote_file) {
		$this->copyFile($this->context . $remote_file, $local_file);
	}
	
	public function putBinaryFile($local_file, $remote_file) {
		$this->copyFile($local_file, $this->context . $remote_file);
	}
	
	public function deleteFile($remote_file) {
		if (unlink($this->context . $remote_file) === false)
			throw new Exception("Cannot unlink file $remote_file");
	}

	public function deleteEmptyDir($remote_dir) {
		$context = $this->context == 'file://' ? '' : $this->context;	// this is to avoid a bug in rmdir for file: wrapper
		if (rmdir($context . $remote_dir) === false)
			throw new Exception("Cannot remove directory $remote_dir");
	}
	
	public function closeConnection() {
	}
}

class preimp_ftp implements pds2_file_transfer {
	
	protected $conn_id=null;
	public    $udm_filesize = array( 
	        'B' => 1, 
	        'KB' => 1024, 
	        'MB' => 1048576, 
	        'GB' => 1073741824, 
	        'TB' => 1099511627776,
	        'PB' => 1125899906842624 
    ); 
	
	public function __construct($current_host, $username, $password, $port=21, $timeout=90, $ftp_active_mode=false){
        $this->conn_id = @ftp_connect($current_host, $port, $timeout);
        if($this->conn_id == false) throw new Exception("Cannot connect via FTP to $current_host on port $port.");

        // try to login
        if (@ftp_login($this->conn_id, $username, $password)==false) {
        	throw new Exception("Cannot authenticate with username ***** and password *****");
        }
        
        if($ftp_active_mode==false){
        	ftp_pasv($this->conn_id, true);
        }        		
	}
	
	public function getDirList($dirname, $what = 'd') {
		$dirs = array();
		$files = array();
		$items = @ftp_nlist($this->conn_id, $dirname);
		foreach ($items as $item)
			if (ftp_size($this->conn_id, $item) == -1)
				$dirs[] = $item;
			else
				$files[] = $item;
				
		switch ($what) {
			case 'd':	return $dirs;
			case 'f':	return $files;
			default:	return array('dirs' => $dirs, 'files' => $files);
		}
	}

	public function getDirSize($dirname,$udm='B') { 
		$size=0;
		$items = @ftp_nlist($this->conn_id, $dirname);
		foreach ($items as $item)
			if (ftp_size($this->conn_id, $item) == -1)
				$size+=$this->getDirSize($item);
			else{
				//temporary solution for file with size > 2G
				$size+=(ftp_size($this->conn_id, $item)<0)?sprintf ("%u", ftp_size($this->conn_id, $item)):ftp_size($this->conn_id, $item);
				}
    	return $size/$this->udm_filesize[$udm]; 
    	
    }

	public function getItemListInfo($dirname) {

		$items = @ftp_nlist($this->conn_id, $dirname);
		$items_info=array();
		foreach ($items as $item)
			if (ftp_size($this->conn_id, $item) == -1)
				$items_info[] = array(
					'path'=>$item,
					'size'=>$this->getDirSize($item),
					'age'=>$this->getDirAge($item),
					'dir'=>true);
			else{
				//temporary solution for file with size > 2G
				$realsize=(ftp_size($this->conn_id, $item)<0)?sprintf ("%u", ftp_size($this->conn_id, $item)):ftp_size($this->conn_id, $item);
				$items_info[] = array(
					'path'=> $item,
					'size'=>$realsize,
					'age'=> ftp_mdtm($this->conn_id, $item),
					'dir'=>false);	
			}

		
		return $items_info;
	}

	public function getDirAge($dirname) { 
		$items = @ftp_nlist($this->conn_id, $dirname);
		$age=0;
		foreach ($items as $item) {		
			if (ftp_size($this->conn_id, $item) == -1) 
				$age=max($age,$this->getDirAge($item));
			else $age=ftp_mdtm($this->conn_id, $item);
		}
    	return $age; 
	}

	public function getBinaryItem($local_itemname, $remote_dirname){
		
		$item = '';
		
		if (ftp_size($this->conn_id, $remote_dirname) == -1){ 
			// SRO log
			//$this->getBinaryDir($local_itemname, $remote_dirname);
			$item.= $this->getBinaryDir($local_itemname, $remote_dirname);
		} else {
			// SRO log
			//$this->getBinaryFile($local_itemname, $remote_dirname);		
			$item = $this->getBinaryFile($local_itemname, $remote_dirname);
		}
		
		return $item;
	}

	public function getBinaryDir($local_dir, $dirname){
       
       if (ftp_chdir($this->conn_id, $dirname) == false) { 
        	throw new Exception("Remote change dir Failed: $dirname");	
            return; 
        } 
        //create local dir if doesn't exists
        if (!(is_dir($local_dir))){
        	if(!acs_file::makeDirs ($local_dir)) throw new Exception("Cannot create $local_dir");
        }
        //change local dir
        if(!chdir($local_dir)) throw new Exception("Cannot change to $local_dir");
        
        //recursive copy of structure and files
        $items = @ftp_nlist($this->conn_id, $dirname);	
        
        $downloaded_items = '';
        $items_count = 0;
        
    	foreach ($items as $file) { 
    		if ($file == '.' || $file == '..') 
            continue; 
	        if (@ftp_chdir($this->conn_id, $file)) { 
            	ftp_chdir ($this->conn_id, ".."); 
            	$this->getBinaryDir($local_dir.'/'.basename($file),$file); 
        	} 
        	else 
            	if (@ftp_get($this->conn_id, $local_dir.'/'.basename($file), $file, FTP_BINARY)==false) {
            		throw new Exception("Cannot download file $file to $local_dir./".basename($file));
            	}
        		$downloaded_items.="    > ".$file."\n";
        		$items_count++;
    	} 
        
    	ftp_chdir ($this->conn_id, ".."); 
    	chdir ("..");     	

    	// SRO return the the directory name so it can be logged outside
    	return "DIRECTORY ($items_count files): ".$dirname."\n".$downloaded_items;
    	
    }
	
	public function getBinaryFile($local_file, $filename){
        if (@ftp_get($this->conn_id, $local_file, $filename, FTP_BINARY)==false) {
            throw new Exception("Cannot download file $filename to $local_file");
        }	
        
        // SRO return the the filename name so it can be logged outside
        return 'File: '.$filename."\n";
    }
    
    public function putBinaryFile($local_file, $remote_file) {
        if (@ftp_put($this->conn_id, $remote_file, $local_file, FTP_BINARY)==false) {
            throw new Exception("Cannot upload file $local_file to $remote_file");
        }	
    }

	public function deleteFile($filename){
        if (@ftp_delete($this->conn_id, $filename)==false) {
            throw new Exception("Cannot delete $filename");
        }	
    }
    
    public function deleteEmptyDir($dirname) {
        if (@ftp_rmdir($this->conn_id, $dirname)==false) {
            throw new Exception("!!!!! Cannot delete directory $dirname");
        }	
    }
	
	public function closeConnection() {
		ftp_close($this->conn_id);	
	}
	
	// SRO check if the connection is still alive
	public function checkConnection() {
		$isAlive = @ftp_nlist($this->conn_id, ".");
		if ($isAlive != false) {
			$isAlive =  true;
		}
		return $isAlive;
	
	}
	
}

class preimp_ftps extends preimp_ftp implements pds2_file_transfer {
	public function __construct($current_host, $username, $password, $port=21, $timeout=90,$ftp_active_mode=false){
        $this->conn_id =  ftp_ssl_connect($current_host, $port);
        if($this->conn_id == false) throw new Exception("Cannot connect via FTP to $current_host on port $port.");

        // try to login
        if (ftp_login($this->conn_id, $username, $password)==false) {
        	throw new Exception("Cannot authenticate with username ***** and password *****");
        }
        
        if($ftp_active_mode==false){
        	ftp_pasv($this->conn_id, true);
        }
		
	}
}

class preimp_sftp implements pds2_file_transfer {
    private $connection;
    private $sftp;
    private $ssh2_version;
	protected $block_size = 8192;
	protected $timeout = 90;
	public  $udm_filesize = array( 
	        'B' => 1, 
	        'KB' => 1024, 
	        'MB' => 1048576, 
	        'GB' => 1073741824, 
	        'TB' => 1099511627776,
	        'PB' => 1125899906842624 
    ); 

    public function __construct($current_host, $username, $password, $port=22, $timeout=90, $public_key='.ssh/id_rsa.pub', $private_key='.ssh/id_rsa' ) {    	
    	$ssh2_extension = new ReflectionExtension('ssh2');
    	$this->ssh2_version = $ssh2_extension->getVersion();
    	$this->timeout = $timeout;
    	
    	//if password is set check if the server is responsive:
		/* TO UNDERSTAND WHY THIS CURL CONFLICT WITH GEARMAN IMPLEMENTATION
			if($password!=''){
	    	$before = time();
	    	$ch = curl_init();
	    	curl_setopt($ch, CURLOPT_URL, 'sftp://'.$username.':'.$password.'@'.$current_host);
	    	curl_setopt($ch,CURLOPT_CONNECTTIMEOUT,$timeout);
		    if (curl_exec($ch)){
		    	curl_close($ch);
		        $sec= (time()-$before);
		    }else{
		       curl_close($ch);
		       $sec= 9999;
		    }
	
			if($sec > (int)$this->timeout)
			        throw new Exception("Cannot connect via SFTP to $current_host server is not responsive!");
		}
		*/
        
		$this->connection = ssh2_connect($current_host, $port);
		if (! $this->connection)
            throw new Exception("Cannot connect via SFTP to $current_host on port $port.");
		$this->login($username, $password, $public_key, $private_key);
    }

    public function login($username, $password='', $public_key, $private_key) {
        
		if ($password!='' && !ssh2_auth_password($this->connection, $username, $password)){
	        	throw new Exception("Cannot authenticate with username ***** and password *****");	
		}
		if ($password==''){
			//it should be done reading config file in user home to choose correct files for keys 
			//if there are more than one or if their name are different from id_rsa.pub e id_rsa
			
			$passphrase=$password;
			
			if (!ssh2_auth_pubkey_file($this->connection, $username, $public_key, $private_key, $passphrase))
	        	throw new Exception("Cannot authenticate with public key.");	      
	  	}
        $this->sftp = @ssh2_sftp($this->connection);
        if (! $this->sftp)
            throw new Exception("Cannot initialize SFTP subsystem.".print_r(error_get_last(),true));
    }

    function ssh_disconnect($reason, $message, $language) 
	{ 
	  throw new Exception("Server disconnected with reason code [%d] and message: %s\n", $reason, $message); 
	} 

    public function uploadFile($local_file, $remote_file)
    {
        $sftp = $this->sftp;
        $stream = @fopen("ssh2.sftp://$sftp$remote_file", 'w');
        if (! $stream)
            throw new Exception("Cannot open file: $remote_file");
        $data_to_send = @file_get_contents($local_file);
        if ($data_to_send === false)
            throw new Exception("Cannot open local file: $local_file.");
        if (@fwrite($stream, $data_to_send) === false)
            throw new Exception("Cannot send data from file: $local_file.");
        @fclose($stream);
    }
    
    public function getDirList($dirname, $what = 'd') {
		$sftp = $this->sftp;
		$dirname=ltrim($dirname);
		$dir = "ssh2.sftp://".$sftp."/".$dirname;  
		$dirs = array();
		$files = array();
		$handle = opendir($dir);
        if ($handle === false)
            throw new Exception("Cannot read directory $dirname");
		// List all the files
		while (false !== ($file = @readdir($handle))) {
			if (substr("$file", 0, 1) != "."){
				$cpath = $dir . '/' . $file;
				if (is_file($cpath))
					$files[] = $dirname.'/'.$file;
               	if (is_dir($cpath))
               		$dirs[] = $dirname.'/'.$file;
			}
		}
		closedir($handle);
		switch ($what) {
			case 'd':	return $dirs;
			case 'f':	return $files;
			default:	return array('dirs' => $dirs, 'files' => $files);
		}
	}    

	public function getDirSize($dirname,$udm='B') { 
		$sftp = $this->sftp;
		$dir = "ssh2.sftp://$sftp$dirname";  
		$handle = @opendir($dir);
        if ($handle === false)
            throw new Exception("Cannot read directory $dirname");
            
		while (false !== ($file = @readdir($handle))) {
			if (substr("$file", 0, 1) != "."){
				$cpath = $dir . '/' . $file;
               	if (is_dir($cpath))
               		$size+=$this->getDirSize($dirname.'/'.$file);
               	else
               		$size+=filesize( $cpath);
			}
		}
		closedir($handle);
 				
    	return $size/$this->udm_filesize[$udm]; 
    	
    }

	public function getItemListInfo($dirname) {
		$sftp = $this->sftp;
		$dir = "ssh2.sftp://$sftp$dirname";  
		$items_info = array();
		$handle = @opendir($dir);
        if ($handle === false)
            throw new Exception("Cannot read directory $dirname");
		// List all the files
		while (false !== ($file = @readdir($handle))) {
			if (substr("$file", 0, 1) != "."){
				$cpath = $dir . '/' . $file;
				if (is_file($cpath))
					$items_info[] = array(
					'path'=> $dirname.'/'.$file,
					'size'=>filesize( $cpath),
					'age'=> filemtime($cpath),
					'dir'=>false);	
					
               	if (is_dir($cpath))
               		$items_info[] = array(
					'path'=>$dirname.'/'.$file,
					'size'=>$this->getDirSize($dirname.'/'.$file),
					'age'=>$this->getDirAge($dirname.'/'.$file),
					'dir'=>true);
			}
		}
		closedir($handle);	
		return $items_info;
	}

	public function getDirAge($dirname) { 
		$sftp = $this->sftp;
		$dir = "ssh2.sftp://$sftp$dirname";  
		$handle = @opendir($dir);
        if ($handle === false)
            throw new Exception("Cannot read directory $dirname");
		$age=0;
		while (false !== ($file = @readdir($handle))) {
			if (substr("$file", 0, 1) != "."){
				$cpath = $dir . '/' . $file;
				if (is_file($cpath)) $age=filemtime($cpath);	
               	if (is_dir($cpath)) $age=max($age,$this->getDirAge($dirname.'/'.$file));
			}
		}
		closedir($handle);	
    	return $age; 
	}

	public function getBinaryItem($local_itemname, $remote_path){
		$item = '';
		$cpath = "ssh2.sftp://".$this->sftp. '/' . $remote_path;
		if (is_dir($cpath)) {
			// SRO log
			//$this->getBinaryDir($local_itemname, $remote_path);
			$item.= $this->getBinaryDir($local_itemname, $remote_path);
		}
		else{
			// SRO log			
			//$this->getBinaryFile($local_itemname, $remote_path);
			$item = $this->getBinaryFile($local_itemname, $remote_path);	
		}	
		
		return $item;
	}

	public function getBinaryDir($local_dir, $dirname){
       	$sftp = $this->sftp;
		$dir = "ssh2.sftp://$sftp$dirname";  
		$handle = @opendir($dir);
        if ($handle === false)
            throw new Exception("Cannot read directory $dirname");
            
        //create local dir if doesn't exists
        if (!(is_dir($local_dir))){
        	if(!acs_file::makeDirs ($local_dir)) throw new Exception("Cannot create $local_dir");
        }
        //change local dir
        if(!chdir($local_dir)) throw new Exception("Cannot change to $local_dir");

        $downloaded_items = '';
        $items_count = 0;
        
		while (false !== ($file = @readdir($handle))) {
			if (substr("$file", 0, 1) != ".") {
				$cpath = $dir . '/' . $file;
				if (is_file($cpath)) {
					$file = $this->getBinaryFile($local_dir.'/'.basename($file), $dirname.'/'.$file);
					
					$downloaded_items.="    > ".$file."\n";
					$items_count++;
					
				}
               	if (is_dir($cpath)) {
               		$this->getBinaryDir($local_dir.'/'.basename($file),$dirname.'/'.$file);
               	}
			}
		}
     	chdir ("..");   
     	
     	// SRO return the the directory name so it can be logged outside
     	return "DIRECTORY ($items_count files): ".$dirname."\n".$downloaded_items;
     	
    }

	public function copyFile(&$fi, &$fo, $file_size ) {
		$len=0;
		try {
			do {
				$str = fread($fi, $this->block_size);
				if ($str === false)
					throw new Exception("Cannot read input file $input_file");
				$len+=strlen($str);
				$write_ret = fwrite($fo, $str);
				
				if ($write_ret === false || $write_ret != strlen($str))
					throw new Exception("Cannot write to output file $output_file");
			} while (strlen($str) == $this->block_size || $len < $file_size);
		} catch (Exception $e) {
			fclose($fi);
			fclose($fo);
			throw $e;
		}
		
		fclose($fi);
		fclose($fo);
	}

    public function getBinaryFile($local_file, $remote_file) {
		$time_start = time();
        // Remote stream
	    if (!$remoteStream = @fopen("ssh2.sftp://$this->sftp/$remote_file", 'r')) {
	    	throw new Exception("Unable to open remote file: $remote_file");
	    }
		// Local stream
	    if (!$localStream = @fopen($local_file, 'w')) {
	    	throw new Exception("Unable to open local file for writing: $local_file");
	    }

		$len=0;
 		$file_size = filesize("ssh2.sftp://$this->sftp/$remote_file");
        try {
 			$this->copyFile($remoteStream,$localStream , $file_size);
         }catch (Exception $e) {
         	unlink($local_file);                       
            throw $e;
         }
         
         fclose($remoteStream);
         fclose($localStream);
         
         // SRO return the the filename just uploaded so it can be logged outside
         return 'File: '.$remote_file."\n";         

    }

    public function putBinaryFile($local_file, $remote_file) {  	
    	try{
			// Remote stream
			if (!$remoteStream = @fopen("ssh2.sftp://$this->sftp/$remote_file", 'w')) {
				throw new Exception("Unable to open remote file: $remote_file");
			}
			// Local stream
			if (!$localStream = @fopen($local_file, 'r')) {
			  	throw new Exception("Unable to open local file for writing: $local_file");
			}
		 	$file_size = filesize($local_file); 
		 	$this->copyFile($localStream, $remoteStream, $file_size );
    	}catch(Exception $e){
    		throw new Exception( $e->getMessage()." Cannot upload file $local_file to $remote_file");
    	}			
    }
        
    public function deleteFile($remote_file){
      $sftp = $this->sftp;
      $ret = @unlink("ssh2.sftp://$sftp$remote_file");
      if ($this->ssh2_version == '0.10')	// unlink return value is wrong for this version of ssh2
      	$ret = ! $ret;
      if(!$ret)
      	throw new Exception("Cannot delete $remote_file");
    }
    
    public function deleteEmptyDir($remote_dir){
      $sftp = $this->sftp;
      $ret = @rmdir("ssh2.sftp://$sftp$remote_dir");
      if ($this->ssh2_version == '0.10')	// unlink return value is wrong for this version of ssh2
      	$ret = ! $ret;
      if(!$ret)
      	throw new Exception("Cannot delete $remote_dir");
    }
	
	public function closeConnection() {
		$this->sftp = null;	
		$this->connection = null;	
	}
	
	// SRO check if the connection is still alive
	public function checkConnection() {
		$sftp = $this->sftp;
		$dir = "ssh2.sftp://$sftp";
		$handle = @opendir($dir);
		if ($handle === false){
			return $handle;
		}
	
		return true;
	
	}
	
}

?>
