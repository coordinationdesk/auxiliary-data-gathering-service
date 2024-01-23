<?php
// vim: set et ts=4 sw=4 fdm=marker:
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
// $Author: stefano.rossi $
// $Id: acs_tools.php,v 1.116 2017/04/05 09:59:30 stefano.rossi Exp $
// $Revision: 1.116 $
//


/**
 * Common utilities
 *
 * @package ACSPhpLib
 */
require_once 'ACSPhpLib/acs_exception.php';

define('__GC_CURRENT_CONNECTION__', '__GC_CURRENT_CONNECTION__');

class acs_tools
{
	private static $bfc=null;
    const __ACS_CRYPT_KEY__ = 'manuel';
    const __GENERAL_CONFIGURATION__ = 'ACS_GENERAL_CONFIGURATION';
    private static $autoIncStringAr=array();


    /**
	 * Microtime float: returns a float containing microseconds
	 * @return integer
	 */
	static public function microtime_float()
	{
		list ($usec, $sec)= explode(" ", microtime());
		return ((float) $usec + (float) $sec);
	}
	
	/**
	 * Microtime string: returns a string containing seconds and microseconds 
	 * @return string
	 */
	static public function microtime_string()
	{
		list ($usec, $sec)= explode(" ", microtime());				
		return $sec.ltrim($usec, '0');
		
	}

  /**
   * Returns an array obtained merging the two passwd arrays. Keys
   * of the second array override keys of the first one.
   *
   * @param array $skel Array containing initial values
   * @param array $arr Array containing values that will override the ones of the first array
   * @return array Array obtained by the overlay of the two received array
   */
	public static function array_overlay($skel, $arr)
	{
		foreach ($skel as $key => $val)
		{
			if (!isset ($arr[$key]))
			{
				$arr[$key]= $val;
			} else
			{
				if (is_array($val))
				{
					$arr[$key]= self :: array_overlay($val, $arr[$key]);
				}
			}
		}
		return $arr;
	}

    public static function getDBConnections(&$iniVars){
        $availableConnection=array();
        if(array_key_exists('db',$iniVars)) {
        	if(array_key_exists('displayname',$iniVars['db'])) $availableConnection['db']=$iniVars['db']['displayname'];
            else $availableConnection['db']='default';
        }
        foreach($iniVars as $section=>$content){
        	if(substr($section,0,3)=='db_'){
                if(array_key_exists('displayname',$iniVars[$section])) $availableConnection[$section]=$iniVars[$section]['displayname'];
                else $availableConnection[$section]=substr($section,3);
        	}
        }
        return $availableConnection;
    }

    public static function getGeneralConfigurationParam($param,$defaultValue){
    	if(isset($_SESSION) && is_array($_SESSION)){
            if(array_key_exists(__GENERAL_CONFIGURATION__,$_SESSION)){
                if(array_key_exists($param,$_SESSION[__GENERAL_CONFIGURATION__])){
                    return $_SESSION[__GENERAL_CONFIGURATION__][$param];
                }
            }
        }
        return $defaultValue;
    }

    public static function setGeneralConfigurationParam($param,$value){
        $_SESSION[__GENERAL_CONFIGURATION__][$param]=$value;
    }

    public static function getCurrentConnection(){
        return self::getGeneralConfigurationParam(__GC_CURRENT_CONNECTION__,'db');
    }

    public static function setCurrentConnection($value){
        self::setGeneralConfigurationParam(__GC_CURRENT_CONNECTION__,$value);
    }

    public static function & getDBHandler($db_section,&$iniVars,&$logHandler){
        if($db_section=='db') $db_section=self::getCurrentConnection();
        if(array_key_exists($db_section,$iniVars)){
            if(array_key_exists('oldlibrary', $iniVars[$db_section]) && ($iniVars[$db_section]['oldlibrary']==true)){
                require_once 'ACSPhpLib/acs_db.php';
                $dbHandler = new acs_db($iniVars, $logHandler, $iniVars['log']['sql_dump'],$db_section);

                if ($logHandler)
                	$logHandler->debug(__METHOD__ . ":" . __LINE__ . " using old DB Class");
            } else {
                require_once 'ACSPhpLib/acs_mdb2.php';
                $dbHandler = new acs_mdb2($iniVars, $logHandler, $iniVars['log']['sql_dump'],$db_section);

                if ($logHandler)
                	$logHandler->debug(__METHOD__ . ":" . __LINE__ . " using new MDB2 Class");
            }
            return $dbHandler;
        }
        return null;
    }

    public function setArrayDefaults(& $varAr, & $defAr, $asVariables=false){
        if(!is_array($varAr)) throw new acs_exBadParam(__METHOD__ . " : varAr parameter is not a valid array");
        if(!is_array($defAr)) throw new acs_exBadParam(__METHOD__ . " : defAr parameter is not a valid array");

        if($asVariables) $varContainer=new acs_variablesContainer();
        foreach($defAr as $k=>$v){
            if((!array_key_exists($k,$varAr)) or (trim($varAr[$k])=='')) $varAr[$k]=$v;
            if($asVariables) $varContainer->$k=$varAr[$k];
        }
        if($asVariables) return $varContainer;
    }

    public function array_reindex(& $varAr, $index){
        if(!is_array($varAr)) throw new acs_exBadParam(__METHOD__ . " : varAr parameter is not a valid array");
        $reindexed_array=array();
        foreach($varAr as $k=>$v){
            if(!array_key_exists($index, $v)) throw new acs_exBadParam(__METHOD__ . " : index $index not found in given array");
            $reindexed_array[strtolower($v[$index])] = $v;
        }
        return $reindexed_array;
    }

    public function array_remove_element($val, & $ar) {
        $idx=array_search($val,$ar);
        if($idx!==false) unset($ar[$idx]);
    }


    public function extractTokens($str, $leftDelimiter='{', $rightDelimiter='}') {
        $matches=array();
        preg_match_all("/".$leftDelimiter."([\w]+)[^".$rightDelimiter."]*".$rightDelimiter."/", $str, $matches,PREG_PATTERN_ORDER);
        return $matches[1];
    }

    public function getMethodSignature($classname, $methodname) {
        $params=array();
        $reflect = new ReflectionMethod ($classname, $methodname);
        foreach ($reflect->getParameters() as $i => $param) {
           $params[]=$param->getName();
        }
        return $params;
    }

    public function getObjectMethods($obj) {
        $methods=array();
        $reflect = new ReflectionObject  ($obj);
        $classname=$reflect->getName();
        foreach ($reflect->getMethods() as $i => $metAr) {
            $methodname=$metAr->name;
            $methods[$methodname]=self::getMethodSignature($classname,$methodname);
        }
        return $methods;
    }

    public static function bf_pwd_crypt($pwd){
        require_once('Crypt/Blowfish.php');
        if(!isset(self::$bfc)){
            self::$bfc =& Crypt_Blowfish::factory('ecb', self::__ACS_CRYPT_KEY__, null, CRYPT_BLOWFISH_PHP);
            if (PEAR::isError($bfc))
                throw new acs_exNotCapable($b->getMessage());
        }

        return base64_encode(self::$bfc->encrypt((string)$pwd));
    }

    public static function bf_pwd_decrypt($pwd){
        require_once('Crypt/Blowfish.php');
        if(!isset(self::$bfc)){
            self::$bfc =& Crypt_Blowfish::factory('ecb', self::__ACS_CRYPT_KEY__, null, CRYPT_BLOWFISH_PHP);
            if (PEAR::isError(self::$bfc))
                throw new acs_exNotCapable($b->getMessage());
        }
        return self::$bfc->decrypt((string)base64_decode($pwd));
    }

    function hex2bin($data) {
        $len = strlen($data);
        return pack('H' . $len, $data);
    }

    public function simple_extracode_encode($str){
        $ar=str_split($str);
        $res='';
        foreach($ar as $digit) {
            if($digit=='#') $res.=$digit;
            else $res.=chr(65+(int)$digit);
        }
        return $res;
    }

    public function simple_extracode_decode($str){
        $ar=str_split($str);
        $res='';
        foreach($ar as $digit) {
            if($digit=='#') $res.=$digit;
            else $res.=ord($digit)-65;
        }
        return $res;
    }

    public function tokenCrypt(&$randomStr, $tokenLenght=40, $extraCode=null, $update=false, $blowcrypted=false) {

        // if the extraCode is given, let's encrypt it
        $tailstr='';
        if(isset($extraCode)) {
            if($blowcrypted){
                require_once('Crypt/Blowfish.php');
                if(!isset(self::$bfc)){
                    self::$bfc =& Crypt_Blowfish::factory('ecb', self::__ACS_CRYPT_KEY__, null, CRYPT_BLOWFISH_PHP);
                    if (PEAR::isError($bfc))
                        throw new acs_exNotCapable($b->getMessage());
                }
                $tailstr = self::$bfc->encrypt((string)$extraCode);
                $tailstr = base64_encode($tailstr).'_';
            } else {
            	$tailstr = self::simple_extracode_encode($extraCode).'_';
            }

            // at least 6 random generated numbers must be for the algo to be secure enough
            $tailstrlen=strlen($tailstr);

            if($tokenLenght-$tailstrlen<6)
                throw new acs_exBadParam("Token length ($tokenLenght) is too short for given extraCode ($extraCode).\nMinimum needed is ".($tailstrlen+6));
            $tokenLenght -= $tailstrlen;
            // let's reverse the tailstr to disguise the algo
            $tailstr=strrev($tailstr);
        }

        // now create the random sequence
        if(!$update) {
            $randomStr='';
            mt_srand(acs_tools::microtime_float());
            while(strlen($randomStr)<$tokenLenght){
                $randval = mt_rand();
                $randomStr .= str_shuffle(md5(microtime()));
            }
            $randomStr=substr($randomStr,0,$tokenLenght);
        }

        return $randomStr.$tailstr;
    }

    public function tokenDecrypt($token,& $randomPart, $blowcrypted=false) {

        // if the extraCode is given, let's decrypt it
        list($randomPart,$extraCode)=explode('_',$token);
        if(empty($extraCode))
            throw new acs_exBadParam("Token cannot be decoded (extraCode not found)");

        if($blowcrypted){
            require_once('Crypt/Blowfish.php');
            if(!isset(self::$bfc)){
                self::$bfc =& Crypt_Blowfish::factory('ecb', self::__ACS_CRYPT_KEY__, null, CRYPT_BLOWFISH_PHP);
                if (PEAR::isError(self::$bfc))
                    throw new acs_exNotCapable(self::$bfc->getMessage());
            }

            $tailstr = base64_decode(strrev($extraCode));
            return self::$bfc->decrypt((string)$tailstr);
        } else {
        	return self::simple_extracode_decode(strrev($extraCode));
        }
    }

    public function superCapitalise($str){
        return ucwords(strtolower(str_replace('_',' ',$str)));
    }

    public static function URLtoHTMLHRef($url){
        $url=trim($url);
        if(empty($url)) return '';
        $urllink=$url;
        if(!@preg_match("/http:\/\//",$url)) $urllink="http://".$url;
        return "<a href=\"$urllink\" target=\"_blank\"><u>$url</u></a>";
    }

    public static function hasHTMLTags($val) {
        return @preg_match("/<(.|\n)+?>/",$val);
    }

    public static function isHTMLLink($val) {
        return @preg_match("/http:\/\//",$val);
    }

    public static function isDateField($val,&$dateAr=array()) {
        return @preg_match("/(\d{4}).(\d{2}).(\d{2})/",$val,$dateAr);
    }

    /* Input format:
            col1    col2    col3    col4
       0    1       2       x1      y1
       1    3       4       x1      y2
       2    3       1       x1      y3
       3    5       3       x2      y1
       4    6       6       x2      y2
       5    8       1       x2      y3
       6    5       6       x3      y1
       7    9       4       x3      y2
       8    1       5       x3      y3

       Output format (with x = col3, y = col4, data = col2):

                    x1      x2      x3
       0    y1      2       3       6
       1    y2      4       6       4
       2    y3      1       1       5

       */
    static public function crossTable($x_index, $y_index, $data_index, $table, $y_as_index = true)
    {
        $out = array();

        foreach ($table as $row)
        {
            $x = $row[$x_index];
            $y = $row[$y_index];
            $data = $row[$data_index];

            $prev_row = $out[$y];
            $prev_row[$x] = $data;
            $out[$y] = $prev_row;
        }

        if (!$y_as_index)
        {
            $temp = array();
            foreach ($out as $k => $row)
                $temp[] = array_merge(array($y_index => $k), $row);
            return $temp;
        }
        else
            return $out;
    }

    // This function returns the length of the greater line inside
    // a string which contains newlines.
    static public function strlen_multiline($str, $sep = "\n")
    {
        $max_size = 0;
        foreach (explode($sep, $str) as $line)
        {
            $len = strlen($line);
            if ($len > $max_size)
                $max_size = $len;
        }
        return $max_size;
    }

    // This function splits a table in two or more tables
    // when the rows are greater than $number_of_chars
    static public function splitTableByChars(& $table, $number_of_chars, $use_column_headers = false, $repeat_columns = array(), $multilinesep = "\n")
    {
        // Compute the number of chars for each rows
        $maxrowlen = 0;
        $collens = array();
        if ($use_column_headers)
            // Preset the column lengths to the header column lengths
            foreach (array_keys(reset($table)) as $key)
            {
                $l = self::strlen_multiline($key, $multilinesep);
                $collens[$key] = $l;
                $maxrowlen += $l;
            }
        foreach ($table as $kr => $row)
        {
            $rowlen = 0;
            foreach ($row as $kc => $cell)
            {
                $l = self::strlen_multiline($cell, $multilinesep);
                $rowlen += $l;
                if ($collens[$kc] < $l)
                    $collens[$kc] = $l;
            }
            if ($maxrowlen < $rowlen) $maxrowlen = $rowlen;
        }
        //print_r($collens);

        if ($maxrowlen < $number_of_chars)
            return array($table);  // avoid table processing if not needed

        $tables = array(); // array of output tables
        foreach ($table as $kr => $row)
        {
            $kt = 0;
            $curr_row_chars = 0;

            foreach ($row as $kc => $cell)
            {
                $curr_row_chars += $collens[$kc];
                //echo "Current column: $kc, chars = $curr_row_chars on $number_of_chars";
                if ($curr_row_chars > $number_of_chars)
                {
                    //echo ": cutting";
                    $kt++;
                    $curr_row_chars = $collens[$kc];
                    foreach ($repeat_columns as $kh)
                    {
                        $tables[$kt][$kr][$kh] = $row[$kh];
                        $curr_row_chars += $collens[$kh];
                    }
                }
                //echo "<br>";

                $tables[$kt][$kr][$kc] = $cell;
            }
        }

        return $tables;
    }

    //truncates a string to a certain char length, stopping on a word if not specified otherwise.
    static public function checkEllipsis($string, $maxlength, $ellipsis='...', $stopanywhere=false) {
        if (strlen($string) > $maxlength) {
            //limit hit!
            $string = substr($string,0,($maxlength -strlen($ellipsis)));
            if ($stopanywhere) {
                //stop anywhere
                $string .= $ellipsis;
            } else{
                //stop on a word.
                $string = substr($string,0,strrpos($string,' ')).$ellipsis;
            }
        }
        return $string;
    }

    // Fetched data from a 2-dimensional array (table) like a select statement
    // Examples:
    // $a = array(	array('filename'=>'a.txt', 'size' => 94, 'path'=>'/etc'),
	//				array('filename'=>'b.txt', 'size' => 102, 'path'=>'/tmp'),
	//				array('filename'=>'c.txt', 'size' => 110, 'path'=>'/etc'));
	//
	// acs_tools::arraySelect($a, array('filename', 'path'), '$row["path"]>100')
	// returns =>
	//array(2) {
	//  [1]=>
	//  array(2) {
	//    ["filename"]=>
	//    string(5) "b.txt"
	//    ["path"]=>
	//    string(4) "/tmp"
	//  }
	//  [2]=>
	//  array(2) {
	//    ["filename"]=>
	//    string(5) "c.txt"
	//    ["path"]=>
	//    string(4) "/etc"
	//  }
	//}
    // acs_tools::arraySelect($a, array('filename'), '$row["path"] == "/etc"')
    // returns =>
	//array(2) {
	//  [0]=>
	//  string(5) "a.txt"
	//  [2]=>
	//  string(5) "c.txt"
	//}
	// acs_tools::arraySelect($a, array('filename'), '$row["path"] == "/etc"', false)
	// returns =>
	//array(2) {
	//  [0]=>
	//  array(1) {
	//    ["filename"]=>
	//    string(5) "a.txt"
	//  }
	//  [2]=>
	//  array(1) {
	//    ["filename"]=>
	//    string(5) "c.txt"
	//  }
	//}
    static public function arraySelect(array $dataset, array $fields = null, $condition = null, $flatten_if_one_field = true)
    {
    	$fields = array_combine($fields, $fields);

    	if ($flatten_if_one_field)
    		$useonefield = sizeof($fields) == 1;
    	else
    		$useonefield = false;

    	foreach ($dataset as $key => $row)
    	{
    		$resrow = null;
    		if ($condition)
    		{
    			$res = false;
    			eval("\$res = $condition;");
    			if ($res)
    				$resrow =  is_null($fields) ? $row : array_intersect_key($row, $fields);
    		}
    		else
    			$resrow =  is_null($fields) ? $row : array_intersect_key($row, $fields);
    		if (!is_null($resrow))
    			$out[$key] = $useonefield ? reset($resrow) : $resrow;
    	}
    	return $out;
    }
    /**
     * Returns the server url with the full protocol and port specification
     * If $prefix is not null it is appended as the first parameter of the reques
     * e.g. http://server:port/prefix/request
     *
     * If with_full_resource is true the full resource is returned instead of just
     * the address
     * e.g.
     * with_full_resource=false
     * http://server:port
     * wit_full_resource=true
     * http://server:port/request_script
     *
     */
    public static function getServerUrl($prefix=null,$with_full_resource=true){
        $proto   = $_SERVER['HTTPS']?'https':'http' ;
        $server  = $_SERVER['SERVER_NAME'];
        $port    = $_SERVER['SERVER_PORT'];
        $resource= $_SERVER['SCRIPT_NAME'];
        if( is_null($prefix) ){
        	$prefix="";
        }
        if($with_full_resource) $self = "{$proto}://{$server}:{$port}{$prefix}{$resource}";
        else $self = "{$proto}://{$server}:{$port}{$prefix}";

        return $self;
    }

    public static function degreesToFloat($strDeg,$defVal=0) {
        if ($strDeg<>''){
            list($decdeg, $decmin, $decsec)=explode('#',str_replace(array(chr(176),"'",'"'),array('#','#',''),$strDeg));
            
            if($decdeg<0){
            	return(abs($decdeg)+$decmin/60+$decsec/3600)*-1;
            }
            
            return $decdeg+$decmin/60+$decsec/3600;
        }
        return $defVal;
    }

	public static function floatToDegrees($floatValue,$coordType='UNKNOWN',$secondsSymbol='"',$asArray=false) {
        $res = array();
        $floatValue=(double)$floatValue;
        $isNegative=$floatValue<0;
        $floatValue=abs($floatValue);
        $res['deg']= floor($floatValue);
        $min = (double)($floatValue - $res['deg']) * 60;
        $res['min'] = floor($min);
        $sec = (double)($min - $res['min']) * 60;
        $res['sec'] = floor($sec);
        $res['dec'] = round(100*($sec-$res['sec']));
        $res['absdeg']=$res['deg'];
        if($isNegative) {
        	$res['deg']=-$res['deg'];
        	$res['sign_label'] = ($coordType=='LAT') ? 'S' : 'W';
        } else {
        	$res['sign_label'] = ($coordType=='LAT') ? 'N' : 'E';
        }

        if($asArray){
        	return $res;
        }
        
    	if($coordType=='UNKNOWN'){
    		$res = sprintf('%03d%s %02d\' %02d.%02d%s',$res['deg'],chr(176),$res['min'],$res['sec'],$res['dec'],$secondsSymbol);
    	} else {
    		$res = sprintf('%03d%s %02d\' %02d.%02d%s %s',$res['absdeg'],chr(176),$res['min'],$res['sec'],$res['dec'],$secondsSymbol,$res['sign_label']);
    	}

        return $res;
    }

    public static function LatLonPointUTMtoLatLon($northing, $easting, $j=32) {
        // $f = Northing
        // $f1 = Easting
        // $j: UTM ZONE (Danmark = 32)

        $d = 0.99960000000000004; // scale along long0
        $d1 = 6378137; // Polar Radius
        $d2 = 0.0066943799999999998;

        $d4 = (1 - sqrt(1 - $d2)) / (1 + sqrt(1 - $d2));
        $d15 = $easting - 500000;
        $d16 = $northing;
        $d11 = (($j - 1) * 6 - 180) + 3;
        $d3 = $d2 / (1 - $d2);
        $d10 = $d16 / $d;
        $d12 = $d10 / ($d1 * (1 - $d2 / 4 - (3 * $d2 * $d2) / 64 - (5 * pow($d2,3) ) / 256));
        $d14 = $d12 + ((3 * $d4) / 2 - (27 * pow($d4,3) ) / 32) * sin(2 * $d12) + ((21 * $d4 * $d4) / 16 - (55 * pow($d4,4) ) / 32) * sin(4 * $d12) + ((151 * pow($d4,3) ) / 96) * sin(6 * $d12);
        $d13 = rad2deg($d14);
        $d5 = $d1 / sqrt(1 - $d2 * sin($d14) * sin($d14));
        $d6 = tan($d14) * tan($d14);
        $d7 = $d3 * cos($d14) * cos($d14);
        $d8 = ($d1 * (1 - $d2)) / pow(1 - $d2 * sin($d14) * sin($d14), 1.5);
        $d9 = $d15 / ($d5 * $d);
        $d17 = $d14 - (($d5 * tan($d14)) / $d8) * ((($d9 * $d9) / 2 - (((5 + 3 * $d6 + 10 * $d7) - 4 * $d7 * $d7 - 9 * $d3) * pow($d9,4) ) / 24) + (((61 + 90 * $d6 + 298 * $d7 + 45 * $d6 * $d6) - 252 * $d3 - 3 * $d7 * $d7) * pow($d9,6) ) / 720);
        $d17 = rad2deg($d17);
        $d18 = (($d9 - ((1 + 2 * $d6 + $d7) * pow($d9,3) ) / 6) + (((((5 - 2 * $d7) + 28 * $d6) - 3 * $d7 * $d7) + 8 * $d3 + 24 * $d6 * $d6) * pow($d9,5) ) / 120) / cos($d14);
        $d18 = $d11 + rad2deg($d18);

        return array('lat'=>$d17,'lon'=>$d18);
    }

	public static function invokeCallbackArray($id, array $args = array())
    {
    	if (is_null($id))
    		return;

    	list($signature, $source) = explode("@", $id);
    	if (strpos($signature, "::") !== false)
    		$res = explode("::", $signature);
    	else
    		$res = $signature;

    	if (dirname($source) == '.')
    		$source = dirname(__FILE__) . "/$source";
   		if (!file_exists($source))
   			throw new Exception("Cannot find '$source'");

   		require_once $source;

   		if (!is_callable($res))
   			throw new Exception("Cannot execute method '{$res[0]} {$res[1]}'");

    	return call_user_func_array($res, $args);
    }

    public static function invokeCallback($id)
    {
    	$params = func_get_args();
    	array_shift($params);

    	return self::invokeCallbackArray($id, $params);
    }
	/**
	 * Given a configuration instance, check if an ip is allowed access.
	 * Access is grant on the basis of
	 * 1) match
	 * 2) belongs to a range
	 * 3) does not is blacklisted
	 * 4) does not belongs to a blacklisted range
	 *
	 * The configuration section is :
	 *
	 *  [allowed_ipaddress]
	 *	 ; values are ip,range
	 * 	 ;
	 *	 ; ip implies ipaddress and ignores allowed_range
	 *	 ; range implies allowed_range and ignores ipaddress
	 *	 ;
	 *	 ;
	 *	 ; if blacklist or blacklist_range are filled
	 *	 ; these ip will not be allowed to access
	 *	 ;
	 *	 type=ip
	 *	 ipaddress=172.30.5.147
	 *
	 *	 ; allowed_range=""
	 *	 ; blacklist=""
	 *	 ; blacklist_range=""
	 *
	 */
	public function allow_access_by_ip($conf, $ip=null, $throw=true){
		require_once 'acs_network.php';
		if( is_null ($ip) ){
			$ip = acs_IPTools::getRealIpAddress();
		}
		$errmsg = "";
		switch( $conf['type'] ){
			case 'ip':{
				if( array_key_exists('ipaddress',$conf) ){
					$valid_ip_addresses=explode(',',$conf['ipaddress']);
					$result = in_array($ip, $valid_ip_addresses );
				} else {
					throw new Exception("Incorrect config file. Expected an 'ipaddress' in section 'allowed_ipaddress' and 'type'='ip' ");
				}
				$errmsg = "not in  {$conf['ipaddress']}";
				break;
			}
			case 'range':{
				if( array_key_exists('allowed_range',$conf) ){
					$range = $conf['allowed_range'];
					$result = acs_IPTools::ip_in_range($ip, $range );
				} else {
					throw new Exception("Incorrect config file. Expected an 'allowed_range' in section 'allowed_ipaddress' and 'type'='range' ");
				}
				$errmsg = "not in  range {$conf['range']}";
				break;
			}
			default :{
				throw new Exception("Incorrect config file. Expected one of 'ip/range' for 'type' property in section  allowed_ipaddress");
			}
		}
		if( array_key_exists('blacklist',$conf) ){
			$blacklisted=explode(',',$conf['blacklist']);
			if(in_array($ip, $blacklisted )){
				$msg = sprintf("IP Address %s is in the blacklist  %s" , $ip, $conf['blacklist']);
				throw new Exception("$msg");
			}
		}
		if( array_key_exists('blacklist_range',$conf) ){
			$bl = acs_IPTools::ip_in_range($ip, $conf['blacklist_range'] );
			if($bl){
				$msg = sprintf("IP Address %s is in the blacklist range %s" , $ip, $conf['blacklist_range']);
				throw new Exception("$msg");
			}
		}
		if( $throw && ($result===false) ){
			$msg = sprintf("IP Address %s is not allowed. %s" , $ip, $errmsg);
			throw new Exception($msg);
		}
		return $result;
	}

	static public function buildHttpQuery($args)
	{
		$url = '';
		if (acs_os::isWindows())
    	{

			foreach ($args as $idx => $val)
			{
				if (is_array($val))
				{
					foreach($val as $k => $v)
						$url .= htmlspecialchars($idx).'['.$k.']=' . htmlspecialchars($v).'&';
				}
				else
					$url .= htmlspecialchars($idx).'='.htmlspecialchars($val).'&';
			}
    	}
		else
			$url .= http_build_query($args);
		return $url;
	}

    public static function getAutoIncFromString($str, $withTrim=true){
    	if($withTrim) $str=trim($str);
        if($str=='') return 0; // 0 is the default value for empty strings
        $keyPos = array_search($str, self::$autoIncStringAr);
        if($keyPos===false) { // add it to the array...
            self::$autoIncStringAr[]=$str;
            $keyPos = array_search($str, self::$autoIncStringAr);
        }
        return $keyPos+1;
    }

    public static function replaceStringFromResourceFile($str,$resourceFile='default',$lang='eng'){
        if($resourceFile=='debug') return $str;
        $replaceRules=array();
        $default_resource_path = dirname(__FILE__).'/components/resources/';
        if($lang!='none'){
            $langResourceFile = $default_resource_path."commonReplaceStrings_$lang.php";
            if(file_exists($langResourceFile))
                require_once $langResourceFile;
        }

        if($resourceFile=='default'){
            $resourceFile = $default_resource_path.'commonReplaceStrings_RegExp.php';
        }
        if(!file_exists($resourceFile)) throw new Exception("Error trapping resource file not found ($resourceFile)");
        require_once $resourceFile;

        return preg_replace(array_keys($replaceRules),array_values($replaceRules),$str);
    }
    
    // This is used to validate an ftp account, by connecting and trying to write a 1-byte file
    public static function checkURI($protocol, $host, $username, $password, $port, $path='/', $testWrite=false) {

    	// check input parameters
    	$errMsgAr= array();
    	if(!isset($protocol)) $errMsgAr[]='Protocol';
    	if(!isset($host)) $errMsgAr[]='Host';
    	if(!is_numeric($port)) $errMsgAr[]='Port';
    	if(!isset($path)) $errMsgAr[]='Path';
		if(count($errMsgAr)>0) throw new acs_exBadParam("The following parameters are invalid:\n".join(', ',$errMsgAr));
		
		$context = "{$protocol}://{$username}:{$password}@{$host}:{$port}";
		$write_context = stream_context_create(array($protocol => array('overwrite' => true)));
        $contextNoPwd = "{$protocol}://{$username}:xxxx@{$host}:{$port}";
        if($testWrite){
        	require_once 'ACSPhpLib/acs_file.php';

            $sourceFile='/tmp/'.acs_file::getUniqueFileName();
            file_put_contents($sourceFile,'test');
            $fo = fopen($sourceFile, 'w', false, $write_context);
            if ($fo === false) {
            	throw new Exception("Cannot access & write to given account $contextNoPwd");
            }
            fclose($fo);
            unlink($sourceFile) ;
        } else {
        	$fo = @fopen($context, 'r');
            if ($fo === false) throw new Exception("Cannot access given account $contextNoPwd");
            fclose($fo);
        }
                
    }

    public static function checkURIString($uri, $testWrite=false) {
		$defaultPortByScheme = array('http'=>80, 'ftp'=>21, 'sftp'=>22, 'tftp'=>69, 'https'=>443);
		
		$uri = trim($uri);
		if($uri=='') throw new acs_exBadParam("Passed URI cannot be empty");
		$params = parse_url($uri);
		if(!isset($params['scheme'])) $params['scheme'] = 'http';
		if(!isset($params['port'])) $params['port'] = $defaultPortByScheme[$params['scheme']];

		self::checkURI($params['scheme'], $params['host'], $params['user'], $params['pass'], $params['port'], $params['path'], $testWrite);
    }

    public static function array_change_key_case_rec(&$array, $case=CASE_LOWER, $flag_rec=false) 
    {
      $array = array_change_key_case($array, $case);
      if ( $flag_rec ) {
        foreach ($array as $key => $value) {
            if ( is_array($value) ) {
                acs_tools::array_change_key_case_rec($array[$key], $case, true);
            }
        }
      }
    } 
} // acs_tools

class acs_image_tools {
    public function hexColorToRGB($color) {
        $col=array();
        if( eregi( "[#]?([0-9a-f]{2})([0-9a-f]{2})([0-9a-f]{2})", $color, $ret ) )
        {
            $col['red']   = hexdec( $ret[1] );
            $col['green'] = hexdec( $ret[2] );
            $col['blue']  = hexdec( $ret[3] );
        }
        return $col;
    }

    public function getImagefromUrl($url,$width,$height) {
        // read binary stream
        $fp = fopen($url, 'rb');
        if($fp===false) throw new Exception("Unable to open file $url for reading");

        $data = '';
        while(!feof($fp)) $data .= fgets($fp, 4096);
        fclose($fp);

        //set new height
        $origImg = imagecreatefromstring($data);
        $origWidth=imagesx($origImg);
        $origHeight=imagesy($origImg);

        $resImg = imagecreatetruecolor($width,$height);

        imagecopyresampled($resImg,$origImg,0,0,0,0,$width,$height,$origWidth,$origHeight);
        imagedestroy($origImg);

        return  $resImg;
    }

    /**
   * Resize a given image. Supported format are GIF, JPEG, PNG. N.B. Transparency in GIF is
   * not preserved.
   *
   * @param string $img full path of image to resize
   * @param int $w new image width
   * @param int $h new image heigth
   * @param string $newfilename the full path of the new file
   * @return $newfilename the path of the svaled image
   */
	public static function resize($img, $w, $h, $newfilename) {

	 //Get Image size info
	 $imgInfo = getimagesize($img);
	 switch ($imgInfo[2]) {
	  case 1:
	  	$im = imagecreatefromgif($img);
	  	break;
	  case 2:
	  	$im = imagecreatefromjpeg($img);
	  	break;
	  case 3:
	  	$im = imagecreatefrompng($img);
	  	break;
	  default:
	  	trigger_error('Unsupported filetype!', E_USER_WARNING);
	  	break;
	 }

	 //If image dimension is smaller, do not resize
	 if ($imgInfo[0] <= $w && $imgInfo[1] <= $h) {
	  $nHeight = $imgInfo[1];
	  $nWidth = $imgInfo[0];
	 } else {
	      //yeah, resize it, but keep it proportional
		  if ($w/$imgInfo[0] > $h/$imgInfo[1]) {
		   $nWidth = $w;
		   $nHeight = $imgInfo[1]*($w/$imgInfo[0]);
		  }else{
		   $nWidth = $imgInfo[0]*($h/$imgInfo[1]);
		   $nHeight = $h;
		  }
	 }
	 $nWidth = round($nWidth);
	 $nHeight = round($nHeight);

	 $newImg = imagecreatetruecolor($nWidth, $nHeight);

	 /* Check if this image is PNG or GIF, then set if Transparent
	  * TRANSPARENCY DOES NOT WORK FOR GIF*/
	 if(($imgInfo[2] == 1) OR ($imgInfo[2]==3)){
	  imagealphablending($newImg, false);
	  imagesavealpha($newImg,true);
	  $transparent = imagecolorallocatealpha($newImg, 255, 255, 255, 127);
	  imagefilledrectangle($newImg, 0, 0, $nWidth, $nHeight, $transparent);

	 }
	 imagecopyresampled($newImg, $im, 0, 0, 0, 0, $nWidth, $nHeight, $imgInfo[0], $imgInfo[1]);

	 //Generate the file, and rename it to $newfilename
	 switch ($imgInfo[2]) {
		  case 1:
		  	imagegif($newImg,$newfilename);
		  	break;
		  case 2:
		  	imagejpeg($newImg,$newfilename);
		  	break;
		  case 3:
		  	imagepng($newImg,$newfilename);
		  	break;
  		  default:
  		  	trigger_error('Failed resize image!', E_USER_WARNING);
  		  	break;
	 }

	   return $newfilename;
	}
}

class acs_variablesContainer{} // used as dummy container in setArrayDefaults

if ( !function_exists('sys_get_temp_dir') ){
   function sys_get_temp_dir(){
     // Try to get from environment variable
     if ( !empty($_ENV['TMP']) and is_readable($_ENV['TMP']) ){
         return realpath( $_ENV['TMP']    );
     } else if (  !empty( $_ENV['TMPDIR']) and is_readable( $_ENV['TMPDIR']) and is_writable( $_ENV['TMPDIR'])){
         return realpath( $_ENV['TMPDIR'])  ;
     } else if (  !empty( $_ENV['TEMP'])   and is_readable( $_ENV['TEMP'] ) and is_writable( $_ENV['TEMP'] )) {
         return realpath( $_ENV['TEMP'])    ;
     }
     else{
       // Detect by creating a temporary file
       // Try to use system's temporary directory
       // as random name shouldn't exist
       $temp_file = tempnam( md5(uniqid(rand(), true)), '' );
       if( $temp_file ){
           $temp_dir = realpath( dirname($temp_file) );
           unlink( $temp_file );
           return $temp_dir;
       } else {
           throw new acs_exFileError("Could not determine a temporary directory");
           return false;
       }
     }
   }
}

class acs_counters {
    private $_counterAr=array();

    public function newCounter($counterID,$counterLabel){
        if(array_key_exists($counterID, $this->_counterAr)) throw new acs_exBadParam(__METHOD__ . " " . __LINE__ . " : counter ID $counterID already exists");
        $counterLabel=trim($counterLabel);
        if($counterLabel=='') throw new acs_exBadParam(__METHOD__ . " " . __LINE__ . " : empty counter label provided");

        $this->_counterAr[$counterID]=array('value'=>0,'label'=>$counterLabel);
    }

    public function updateCounter($counterID,$delta=1){
//       print_r($this->_counterAr);die;
        if(!array_key_exists($counterID, $this->_counterAr)) throw new acs_exBadParam(__METHOD__ . " " . __LINE__ . " : counter ID $counterID doesn't exists");
        if($delta==0) throw new acs_exBadParam(__METHOD__ . " " . __LINE__ . " : zero delta for counter $counterID");
        if(!is_numeric($delta)) throw new acs_exBadParam(__METHOD__ . " " . __LINE__ . " : invalid delta ($delta) for counter $counterName");
        $this->_counterAr[$counterID]['value']=$this->_counterAr[$counterID]['value']+$delta;
    }

    public function getReport($verbose){
        $rep='';
        foreach ($this->_counterAr as $k=>$v) {
            if($verbose or ($v['value']!=0))
                $rep .= $v['label'] . ' = ' . $v['value'] . "\n";
        }
        return $rep;
    }

}

class acs_boundingbox {
    private $minx, $miny, $maxx, $maxy;
    private $defminx, $defminy, $defmaxx, $defmaxy;
    private $delta_x=0, $delta_y=0,$delta_perc_x=0, $delta_perc_y=0;
    

    public function __construct($minx=-180, $miny=-90, $maxx=180, $maxy=90) {
        $this->defminx=$maxx;
        $this->defminy=$maxy;
        $this->defmaxx=$minx;
        $this->defmaxy=$miny;

        $this->reset();
    }

    public function reset() {
        $this->minx=$this->defminx;
        $this->miny=$this->defminy;
        $this->maxx=$this->defmaxx;
        $this->maxy=$this->defmaxy;
    }

    public function addPoint($x,$y) {
        $this->minx=min($this->minx,$x);
        $this->miny=min($this->miny,$y);
        $this->maxx=max($this->maxx,$x);
        $this->maxy=max($this->maxy,$y);
    }

	private function getBufferDeltas(){
        $dx = $this->delta_x + ($this->maxx-$this->minx) * $this->delta_perc_x;
        $dy = $this->delta_y + ($this->maxy-$this->miny) * $this->delta_perc_y;
		return array($dx, $dy);
	}

    public function getmaxmin() {
        list($dx, $dy) = $this->getBufferDeltas();
        return array('minx'=>$this->minx - $dx,
                    'maxx'=>$this->maxx + $dx,
                    'miny'=>$this->miny - $dy,
                    'maxy'=>$this->maxy + $dy
        );
    }

    public function getboundingbox() {
        list($dx, $dy) = $this->getBufferDeltas();
        return array('ul_x'=>$this->minx-$dx, 'ul_y'=>$this->maxy+$dy,
                     'ur_x'=>$this->maxx+$dx, 'ur_y'=>$this->maxy+$dy,
                     'lr_x'=>$this->maxx+$dx, 'lr_y'=>$this->miny-$dy,
                     'll_x'=>$this->minx-$dx, 'll_y'=>$this->miny-$dy
        );
    }

    public function getBoundingBoxAsPolygon($x='x',$y='y',$asClosed=false) {
        list($dx, $dy) = $this->getBufferDeltas();
        $poly = array(   array($x=>$this->minx-$dx, $y=>$this->maxy+$dy),
                         array($x=>$this->maxx+$dx, $y=>$this->maxy+$dy),
                         array($x=>$this->maxx+$dx, $y=>$this->miny-$dy),
                         array($x=>$this->minx-$dx, $y=>$this->miny-$dy)
                    );
        if($asClosed) $poly[]=array($x=>$this->minx-$dx, $y=>$this->maxy+$dy);
        return $poly;
    }

    public function addFixedBufferToBoundingBox($delta_x, $delta_y) {
        $this->delta_x = $delta_x / 2.0;
        $this->delta_y = $delta_y / 2.0;
        $this->delta_perc_x = 0;
        $this->delta_perc_y = 0;
    }

    public function addPercentageBufferToBoundingBox($delta_perc_x, $delta_perc_y) {
        $this->delta_perc_x = $delta_perc_x/200.0;
        $this->delta_perc_y = $delta_perc_y/200.0;
        $this->delta_x = 0;
        $this->delta_y = 0;
    }

}

class acs_geo_polygon {
    public static function polygonToArray($polygon, &$bbox, $x_coord='X', $y_coord='Y', $coupleSep=',',$lonLat=true){

        $bboxObj=new acs_boundingbox();
        $res=array();
	    if(trim($coupleSep)==''){
            $tmpAr=explode(' ',$polygon);
            for($i=0; $i<count($tmpAr)-1; $i+=2) {
                $geocouplesAr[]=$tmpAr[$i].' '.$tmpAr[$i+1];
            }
	    } else{
            $geocouplesAr=explode(',',$polygon);
	    }

        for($i=0; $i<count($geocouplesAr); $i++) {
            list($X,$Y)=explode(' ',trim($geocouplesAr[$i])); 
            $res[]=array($x_coord=>$X, $y_coord=>$Y);
            if($lonLat===true)	$bboxObj->addPoint($X,$Y);
            else $bboxObj->addPoint($Y,$X);
        }
        $bboxVals=$bboxObj->getmaxmin();
        $bbox=array(array($x_coord=>$bboxVals['minx'], $y_coord=>$bboxVals['miny']),array($x_coord=>$bboxVals['maxx'], $y_coord=>$bboxVals['maxy']));
        return $res;
    }

    public static function removeSignFromCoordinate($coord,$type){
    	if(!is_numeric($coord)) throw new acs_exBadParam("given coord is not numeric ($coord)");
        $type=strtoupper($type);
        if(($type<>'LAT') && ($type<>'LON')) throw new acs_exBadParam("type can be only LAT or LON");
        $labels=array('neg'=>array('LAT'=>'S','LON'=>'W'),'pos'=>array('LAT'=>'N','LON'=>'E'));
        if($coord<0) $coord=str_replace('-','',$coord).$labels['neg'][$type];
        else $coord=str_replace('+','',$coord).$labels['pos'][$type];
        return $coord;
    }

    public static function getGeometryType(&$geometry,$stripTypeLabel=false){
        if(stripos($geometry,'LINESTRING')!==false) $res= 'Linestring';
        else $res ='Polygon';
        $res= 'Linestring';
        if($stripTypeLabel) {
            $geometry=str_ireplace(array('POLYGON','LINESTRING','(',')'),array('','',''),$geometry);
        }

        return $res;
    }
}

class acs_directory
{
    /*  Fetches directory contents and returns an array with the details of each found file
        It is possible to use filters on filenames (as a regex) and filetype (dir, file, link, etc.)
        Example of a returned array item (for file '/tmp/file.txt'):
        'name' => 'file.txt'
        'full' => '/tmp/file.txt'
        'type' => 'file'
        'path' => '/tmp/'
        'base' => 'file'
        'ext' =>  'txt'
    */
	static public function getContents(	$path,
										$name_filter = null /* regex */,
										$type_filter = null,
										$recursive = true,
										& $ar = null)
	{
		$nf = null;
		if ($name_filter != null)
		{
			$nf = is_array($name_filter) ? join('|', $name_filter) : $name_filter;
			if ($nf[0] != '/')
				$nf = "/$nf/";
		}

		if ($dh = opendir($path))
		{
			while (($file = readdir($dh)) !== false)
			{
				$insert = true;

				$fullpathfile = $path . '/' . $file;
				$type = filetype(realpath($fullpathfile));

				if ($nf != null)
					$insert = preg_match($nf, $file) > 0;
				if ($type_filter != null)
					$insert = $insert && ($type == $type_filter);

				$pi = pathinfo($fullpathfile);

				//echo "processing $fullpathfile, $insert (nf='$nf', tp='$type_filter')\n";

				if (!in_array($file, array('.', '..')))
				{
					if ($insert)
						$ar[] = array(	'name' => $file,
										'full' => $fullpathfile,
										'type' => $type,
										'path' => $pi['dirname'],
										'base' => basename($pi['basename'], '.' . $pi['extension']),
										'ext' => $pi['extension'],
										'stat'=> stat($fullpathfile));

					if ($recursive && $type == 'dir')
						self::getContents($fullpathfile, $name_filter, $type_filter, true, $ar);
				}
			}

			closedir($dh);
		}
		else
			throw new Exception("Path '$path' not found");
		return $ar;
	}
	
	// Canonicalizes a path.
	// Works like realpath, but also on non-existing directories. Symlinks are not processed.
	static public function getAbsolutePath($path) {
		$path = str_replace(array('/', '\\'), DIRECTORY_SEPARATOR, $path);
		$abs_path_prefix = @($path[0] == DIRECTORY_SEPARATOR ? DIRECTORY_SEPARATOR : '');
		$parts = array_filter(explode(DIRECTORY_SEPARATOR, $path), 'strlen');
		$absolutes = array();
		foreach ($parts as $part) {
			if ('.' == $part) continue;
			if ('..' == $part) {
				array_pop($absolutes);
			} else {
				$absolutes[] = $part;
			}
		}
		return $abs_path_prefix . implode(DIRECTORY_SEPARATOR, $absolutes);
	}
}

class acs_curl {

    public $headers;
    public $user_agent;
    public $compression;
    public $cookie_file;
    public $proxy;
    public $timeout;
    public $last_http_response;
    public $auto_close_connection = true;
    public $connection = null;
    public $sendHeaders=false;

    public function __construct($cookies=TRUE,$cookie='cookies.txt',$compression='gzip',$proxy='', $timeout = 30) {
    	$this->setDefaultHeaders();
        $this->user_agent = 'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.0.3705; .NET CLR 1.1.4322; Media Center PC 4.0)';
        $this->compression=$compression;
        $this->proxy=$proxy;
        $this->cookies=$cookies;
        $this->timeout = $timeout;
        if ($this->cookies == TRUE) $this->cookie($cookie);
    }

	public function setDefaultHeaders() {
	        $this->headers = array();
	        $this->headers[] = 'Accept: */*';
	        $this->headers[] = 'Accept-Language: it-IT';
	        $this->headers[] = 'Pragma: no-cache';
	        $this->headers[] = 'Connection: Keep-Alive';
	}

    public function cookie($cookie_file) {
        if (file_exists($cookie_file)) {
            $this->cookie_file=$cookie_file;
        } else {
            if(@fopen($cookie_file,'w')){
            	$this->cookie_file=$cookie_file;
            	@fclose($this->cookie_file);
            }
        }
    }

    public function get($url, $contentType='application/xml', $withReturnTransfer=true, array $additional_options = array()) {
        if(!isset($this->connection)) $this->connection = curl_init();
        $this->headers[] = 'Content-Type: '.$contentType;

        curl_setopt($this->connection, CURLOPT_URL, $url);
        curl_setopt($this->connection, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_setopt($this->connection, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($this->connection, CURLOPT_HTTPHEADER, $this->headers);
        curl_setopt($this->connection, CURLOPT_HEADER, 0);
        curl_setopt($this->connection, CURLOPT_USERAGENT, $this->user_agent);
        if ($this->cookies == TRUE) curl_setopt($this->connection, CURLOPT_COOKIEFILE, $this->cookie_file);
        if ($this->cookies == TRUE) curl_setopt($this->connection, CURLOPT_COOKIEJAR, $this->cookie_file);
        curl_setopt($this->connection,CURLOPT_ENCODING , $this->compression);
        curl_setopt($this->connection, CURLOPT_TIMEOUT, $this->timeout);
        //if ($this->proxy) curl_setopt($cUrl, CURLOPT_PROXY, 'proxy_ip:proxy_port');
        if($withReturnTransfer) curl_setopt($this->connection, CURLOPT_RETURNTRANSFER, 1);

        foreach ($additional_options as $k => $v)
        	curl_setopt($this->connection, $k, $v);

        curl_setopt($this->connection, CURLOPT_FOLLOWLOCATION, 1);
        $return = curl_exec($this->connection);
        $this->last_http_response = curl_getinfo($this->connection);
        if($this->sendHeaders){
        	if ($headers = get_headers($this->last_http_response['url']))
	        	foreach ($headers as $header)
        			header($header);
        }
        if($this->auto_close_connection){
            $this->closeConnection();
        }
        return $return;
    }

	// Note:
	// in order to use array values as post data,
	// use contenttype = 'application/x-www-form-urlencoded'
    function post($url, $data, $contentType='application/xml', $withReturnTransfer=true, array $additional_options = array()) {
        if(!isset($this->connection)) $this->connection = curl_init();

		if (is_array($data))
			$data = http_build_query($data);

        $this->headers[] = 'Content-Type: '.$contentType;
        $this->headers[] = 'Content-Length: '.strlen($data);

        curl_setopt($this->connection, CURLOPT_URL, $url);
        curl_setopt($this->connection, CURLOPT_SSL_VERIFYPEER, FALSE);
        curl_setopt($this->connection, CURLOPT_SSL_VERIFYHOST, 2);
        curl_setopt($this->connection, CURLOPT_HTTPHEADER, $this->headers);
        curl_setopt($this->connection, CURLOPT_HEADER, 0);
        curl_setopt($this->connection, CURLOPT_USERAGENT, $this->user_agent);
        curl_setopt($this->connection, CURLOPT_ENCODING , $this->compression);
        curl_setopt($this->connection, CURLOPT_TIMEOUT, $this->timeout);
        curl_setopt($this->connection, CURLOPT_POSTFIELDS, $data);
        if($withReturnTransfer) curl_setopt($this->connection, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($this->connection, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($this->connection, CURLOPT_POST, 1);

        foreach ($additional_options as $k => $v)
        	curl_setopt($this->connection, $k, $v);

        $return = curl_exec($this->connection);
		$this->last_http_response = curl_getinfo($this->connection);
		
        if($this->sendHeaders){
        	if ($headers = get_headers($this->last_http_response['url'])){
	        	foreach ($headers as $header){
        			header($header);
        		}
	        }			
        }
        if($this->auto_close_connection){
            $this->closeConnection();
        }
        return $return;
    }

    public function closeConnection() {
        if(isset($this->connection)) {
            curl_close($this->connection);
            $this->connection=null;
        }
    }
}

class acs_callurl
{
    // Call this function to send post data to a generic url
    // returns the url response string
	static public function post($url, $data, $content_type = "text/xml")
	{
		require_once "HTTP/Request.php"; // pear install HTTP_Request
		$req = new HTTP_Request($url);
		$req->setMethod(HTTP_REQUEST_METHOD_POST);
		$req->addHeader("Content-Type", $content_type); //"application/x-www-form-urlencoded");
		if (is_array($data))
			foreach ($data as $k => $v)
				$req->addPostData($k, $v);
		else
		{
			$req->addHeader("Content-Length", strlen($data));
			//$req->addRawPostData($data, true);
			$req->setBody($data);
			//$req->setBody(urlencode($data));
		}

		if (!PEAR::isError($req->sendRequest())) {
			return $req->getResponseBody();
		} else {
			throw new acs_exFailed("Cannot post data to '$url'");
		}
	}

	static public function get($url, $data, $content_type = "text/xml")
	{
		require_once "HTTP/Request.php"; // pear install HTTP_Request
		$req = new HTTP_Request($url);
		$req->setMethod(HTTP_REQUEST_METHOD_GET);
		$req->addHeader("Content-Type", $content_type); //"application/x-www-form-urlencoded");
		if (is_array($data))
			foreach ($data as $k => $v)
				$req->addQueryString($k, $v);
		else
		{
            $req->addRawQueryString($data);
		}

		if (!PEAR::isError($req->sendRequest())) {
			return $req->getResponseBody();
		} else {
			throw new acs_exFailed("Cannot send get request to '$url'");
		}
	}
}

class acs_cvslogtools
{
	static public function getCvsRLogs($module, $date = null, $cvsroot = null)
	{
		if ($date)
			$date_opts = "-d'$date'";
		if ($cvsroot)
			$cvsroot_opts = "-d $cvsroot";
		$cmd = "/usr/bin/cvs $cvsroot_opts rlog -NS $date_opts $module 2> /dev/null";
		$output = null; $exit_code = null;
		exec($cmd, $output, $exit_code);

		if ($exit_code)
			throw new Exception("Cannot execute cvs command: '$cmd'");

		return join("\n", $output);
	}

	static protected function splitCvsLogHeader($header)
	{
		preg_match('/^RCS file: (.*),v$/mi', $header, $matches);
		$file = $matches[1];

		preg_match('/^head: (.*)/mi', $header, $matches);
		$revision = $matches[1];

		return array('file' => $file, 'revision' => $revision);
	}

	static protected function splitCvsLogRecords($records)
	{
		foreach ($records as $rec)
		{
			$lines = split("\n", preg_replace("/^\n/m", '', $rec));

			preg_match('/^revision[ ]+(.*)$/i', $lines[0], $matches);
			$revision = $matches[1];

			preg_match('/^date:[ ]+([0-9]{4}\/[0-9]{2}\/[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2});/i', $lines[1], $matches);
			$date = $matches[1];

			preg_match('/author: (.*);/iU', $lines[1], $matches);
			$author = $matches[1];

			$out[$revision] = array('revision' => $revision, 'date' => $date, 'author' => $author, 'comments' => join("\n", array_slice($lines, 2)));
		}

		return $out;
	}

	static public function getPerFileLogs($global_log)
	{
		$out = array();
		foreach (preg_split('/={10,}/', $global_log) as $file_block)
		{
			if (preg_match('/RCS file: (.*),v/', $file_block, $matches) && $matches[1] != '')
			{
				$paragraphs = preg_split('/-{10,}/', $file_block);

				$out[$matches[1]] = array(	'header' => self::splitCvsLogHeader(array_shift($paragraphs)),
											'revisions' => self::splitCvsLogRecords($paragraphs));
			}
		}

		return $out;
	}
}

class acs_os {
	static public function isWindows() {
		return substr( php_uname( ), 0, 7 ) == "Windows";
	}

	static public function isLinux() {
		return ! self::isWindows();
	}

	static public function fileSeparator(){
		if(self::isWindows()){
			return "\\";
		}else{
			return "/";
		}

	}

}


// An interface class to get info about an image file
// with gdalinfo
// Image driver, image resolution, CRS and BBOX are provided by getInfo
class acs_gdalinfo {
	protected $_gdalinfo;
	public function __construct($gdalinfo_bin = '/usr/bin/gdalinfo')
	{
		if (!file_exists($gdalinfo_bin))
			throw new acs_exFailed("gdalinfo executable does not exists ($gdalinfo_bin)");
		$this->_gdalinfo = $gdalinfo_bin;
	}

	protected function callGdalInfo($img_file)
	{
		$img = escapeshellarg($img_file);
		$retval = 0;
		$output = array();
		$cmd = "{$this->_gdalinfo} {$img} 2>&1";
		exec($cmd, $output, $retval);
		if ($retval != 0)
			throw new acs_exFailed("Error running \"$cmd\" ($retval): " . join("\n", $output));

		return $output;
	}

	public function getInfo($img_file)
	{
		$info = array();
		$matches = array();

		$output = $this->callGdalInfo($img_file);

		$str = join("\n", $output);

		if (preg_match('/^Driver: (.*)$/msU', $str, $matches))
			$info['driver'] = $matches[1];

		if (preg_match('/^Files: (.*)$/msU', $str, $matches))
			$info['files'] = $matches[1];

		if (preg_match('/^Size is ([0-9]+)[, ]*([0-9]+)$/msU', $str, $matches))
		{
			$info['size_x'] = $matches[1];
			$info['size_y'] = $matches[2];
		}

		if (preg_match('/^Coordinate System is:\n(.*)\n[A-Za-z]+/msU', $str, $matches))
			$info['CRS'] = $matches[1];

		$min_lat = 1e99; $max_lat = -1e99;
		$min_lon = 1e99; $max_lon = -1e99;
		$min_lat_lon = false;
		foreach (array('ul' => 'Upper Left', 'll' => 'Lower Left', 'ur' => 'Upper Right', 'lr' => 'Lower Right') as $key => $what)
		{
			//preg_match('/^' . $what . '[ (]+(.*),[ ]+(.*)[ )]+$/', $str, $matches);
			if (preg_match('/^' . $what . '[ (]+([ |-]+[0-9.]*),([ |-]+[0-9.]*)[ )]+(\s[(].*[)])*$/msU', $str, $matches)) 
			{
				$x = $matches[1];
				$y = $matches[2];
				$info[$key] = array('x' => $x, 'y' => $y);
				if (is_numeric($x) && is_numeric($y))
				{
					$x = (float)$x; $y = (float)$y;
					$min_lat_lon = true;
					if ($x < $min_lon) $min_lon = $x;
					if ($x > $max_lon) $max_lon = $x;
					if ($y < $min_lat) $min_lat = $y;
					if ($y > $max_lat) $max_lat = $y;
				}
			}
		}
		if ($min_lat_lon)
		{
			$info['bbox']['lat']['max'] = $max_lat;
			$info['bbox']['lat']['min'] = $min_lat;
			$info['bbox']['lon']['max'] = $max_lon;
			$info['bbox']['lon']['min'] = $min_lon;
		}

		if (preg_match_all('/^Band/msU', $str, $matches))
			$info['band_number'] = count($matches[0]);
		
		if (preg_match('/^Metadata:\n(.*)\n[A-Za-z]+/msU', $str, $matches))
			$info['metadata'] = $matches[1];
		if (preg_match('/^Image Structure Metadata:\n(.*)\n[A-Za-z]+/msU', $str, $matches))
			$info['imagestructuremetadata'] = $matches[1];

		return $info;
	}
}

// An interface class to the REST apis
class acs_rest {
	protected $base_url, $logHandler;
	public $curl_extra_options = array();
	public $content_type = 'text/xml';
    public $auto_close_connection = true;
    public $curl = null;
    public $reset_headers = true;
    public $check_response = true;

	public function __construct($base_url, &$logHandler = null) {
		$this->base_url = $base_url;
		$this->_logHandler = $logHandler;
        $this->curl = new acs_curl(false);
	}
	
	public function getBaseUrl() {
		return $this->base_url;
	}

	protected function checkResponse($response, $ret)
	{
		//var_dump($response);die;
		if ($response['http_code'] == 0)
			throw new acs_exFailed("Cannot contact url {$response['url']}: ".print_r($ret,true));
		$ret_category = (int)($response['http_code'] / 100);
		if ($ret_category == 4 || $ret_category == 5)
			throw new acs_exFailed("REST error {$response['http_code']} for url {$response['url']}: ".print_r($ret,true));
		if ($ret_category == 2 && $this->_logHandler)
			$this->_logHandler->debug("REST: ".print_r($ret,true));
	}

	public function get($add_url) {
		if ($this->reset_headers)
			$this->curl->setDefaultHeaders();
		$this->curl->auto_close_connection = $this->auto_close_connection;
        $ret = $this->curl->get($this->base_url . $add_url, $this->content_type, true, $this->curl_extra_options);

        if ($this->check_response)
			$this->checkResponse($this->curl->last_http_response, $ret);

		return array('data' => $ret, 'http' => $this->curl->last_http_response);
	}

	public function post($add_url, $data, $contentType=null) {
		if ($this->reset_headers)
			$this->curl->setDefaultHeaders();
		$currentContentType = $contentType != null ? $contentType : $this->content_type;
        $this->curl->auto_close_connection = $this->auto_close_connection;
		$ret = $this->curl->post($this->base_url . $add_url, $data, $currentContentType, true, $this->curl_extra_options);
		if ($this->check_response)
			$this->checkResponse($this->curl->last_http_response, $ret);

		return array('data' => $ret, 'http' => $this->curl->last_http_response);
	}

	public function put($add_url, $data, $contentType) {
		if ($this->reset_headers)
			$this->curl->setDefaultHeaders();
        $this->curl->auto_close_connection = $this->auto_close_connection;
		$options = $this->curl_extra_options;
		$options[CURLOPT_CUSTOMREQUEST] = 'PUT';	// do not use array_merge because reset keys values when integer
	    
		$ret = $this->curl->post($this->base_url . $add_url, $data, $contentType, true, $options);

		if ($this->check_response)
			$this->checkResponse($this->curl->last_http_response, $ret);

		return array('data' => $ret, 'http' => $this->curl->last_http_response);
	}
	
	public function custom($add_url, $data, $contentType, $request) {
		if ($this->reset_headers)
			$this->curl->setDefaultHeaders();
		$this->curl->auto_close_connection = $this->auto_close_connection;
		$options = $this->curl_extra_options;
		$options[CURLOPT_CUSTOMREQUEST] = $request;	// do not use array_merge because reset keys values when integer
	
		$ret = $this->curl->post($this->base_url . $add_url, $data, $contentType, true, $options);
	
		if ($this->check_response)
			$this->checkResponse($this->curl->last_http_response, $ret);
	
		return array('data' => $ret, 'http' => $this->curl->last_http_response);
	}

	public function delete($add_url) {
		if ($this->reset_headers)
			$this->curl->setDefaultHeaders();
        $this->curl->auto_close_connection = $this->auto_close_connection;
		$options = $this->curl_extra_options;
		$options[CURLOPT_CUSTOMREQUEST] = 'DELETE';	// do not use array_merge because reset keys values when integer

		$ret = $this->curl->get($this->base_url . $add_url, $this->content_type, true, $options);

		if ($this->check_response)
			$this->checkResponse($this->curl->last_http_response, $ret);

		return array('data' => $ret, 'http' => $this->curl->last_http_response);
	}

    public function closeConnection(){
        if(isset($this->curl)) $this->curl->closeConnection();
    }

}

?>
