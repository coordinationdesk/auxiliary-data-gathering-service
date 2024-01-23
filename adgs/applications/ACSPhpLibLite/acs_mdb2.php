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
// $Author: seravo $
// $Id: acs_mdb2.php,v 1.87 2016/01/05 17:37:42 seravo Exp $
// $Revision: 1.87 $
//

require_once 'ACSPhpLib/acs_exception.php';

// PEAR includes
require_once 'MDB2.php';
require_once 'MDB2/Extended.php';
require_once 'acs_sql.php';
require_once 'acs_file.php';

// Interface include
require_once 'interfaces/acs_idb.php';

// Common mdb2 types
define('_ACS_DBTYPE_INT',		'integer');
define('_ACS_DBTYPE_TEXT',		'text');
define('_ACS_DBTYPE_TIMESTAMP',	'timestamp');
define('_ACS_DBTYPE_BOOLEAN',	'boolean');
define('_ACS_DBTYPE_DECIMAL',	'decimal');
define('_ACS_DBTYPE_FLOAT',		'float');
define('_ACS_DBTYPE_TIME',		'time');
define('_ACS_DBTYPE_DATE',		'date');
define('_ACS_DBTYPE_CLOB',		'clob');
define('_ACS_DBTYPE_BLOB',		'blob');

class acs_mdb2 implements acs_idb {
	private $_dbConn = null;
	private $_inivars = null;
	private $_logHandler = null;
    private $_debugSQL = false;
    public $_dbType = '';
    public $autocommit=true;
    private $_acsSqlObj=null;
    private static $_blobFieldsAsFile=null;
    public $columns = array();	// This array stores the column names after a call to execQuery
    public $nativecolumntypes = array();
    public $nativecolumnsizes = array();
    public $columntypes = array();
    protected $allKeysLowercase=false;

    private $_historyListeners = array();

	public function __construct(& $inivars, $logHandler = null, $debugSQL = false, $db_section='db') {
		if (!is_array($inivars) or !array_key_exists($db_section, $inivars)) {
			throw new acs_exBadParam('Bad db inivars');
		}
		$this->_logHandler = $logHandler;
		$this->_inivars = $inivars;
		$this->_debugSQL = $debugSQL;
		$dsn = null;

        // handle pwd decryption
        if(array_key_exists('crypt_password',$inivars[$db_section])){
            require_once('ACSPhpLib/acs_tools.php');
            if($inivars[$db_section]['crypt_password']=='DOIT')
                throw new Exception('Crypt Password ('.$inivars[$db_section]['password'].') = '.acs_tools::bf_pwd_crypt($inivars[$db_section]['password']));
            $password = acs_tools::bf_pwd_decrypt($inivars[$db_section]['crypt_password']);
            $inivars[$db_section]['password'] = trim($password, chr(0));
            $this->_inivars[$db_section]['password'] = $inivars[$db_section]['password'];
        }

        if(array_key_exists('all_keys_lowercase',$inivars[$db_section])){
            $this->allKeysLowercase=$inivars[$db_section]['all_keys_lowercase'];
        }

		if (!array_key_exists('type', $inivars[$db_section])) {
			// Default type is mysql
			$inivars[$db_section]['type'] = 'mysql';
		}

		if (!array_key_exists('debug', $inivars[$db_section])) {
			$inivars[$db_section]['debug'] = 2;
		}

		$this->_dbType=$inivars[$db_section]['type'];

        // create istance-dbtype-dependent acssql
        $this->_acsSqlObj = new acs_sql($this->_dbType);

        $options = array (
            'debug' => $inivars[$db_section]['debug'],
            'portability' => "DB_PORTABILITY_ALL ^ DB_PORTABILITY_LOWERCASE"
        );

        /**
         * If exists a key "dsn" in "db" section of inivars use this one
         * else assume there are the different pieces needd to build a valid dsn
         */
        if (!array_key_exists('dsn', $inivars[$db_section])) {
            // wrap up PDO Lib if Informix
            switch ($this->_dbType){
                case 'ifx':
    				$dsn = "informix:database=" . $inivars[$db_section]['db'] . ";" .
    				"server=" . $inivars[$db_section]['server'] . ";" .
    				"service=" . $inivars[$db_section]['port'] . ";" .
    				"host=" . $inivars[$db_section]['host'] . ";" .
                    "protocol=onsoctcp;" .
                    "EnableScrollableCursors=1;".
                    "DB_LOCALE=" . $inivars[$db_section]['dblocale'];
    //                die($dsn);
    				$user = $inivars[$db_section]['user'];
    				$password = $inivars[$db_section]['password'];
                    break;

                case 'oci8':
    				$dsn = array (
    					'new_link' => 'true',
    					'phptype' => 'oci8',
    					'username' => $inivars[$db_section]['user'],
    					'password' => $inivars[$db_section]['password'],
    					'hostspec' => $inivars[$db_section]['host'],
    					'service' => $inivars[$db_section]['service'],
    					'database' => $inivars[$db_section]['db'],
    					'port' => (array_key_exists('port', // Default port assumed to be 1521
    	                $inivars[$db_section]
    				) ? $inivars[$db_section]['port'] : 1521));

    				if (array_key_exists('new_link', $inivars[$db_section]))
    					$dsn['new_link'] = $inivars[$db_section]['new_link'] ? 'true' : 'false'; 

                    $seqformat='SQ_%s';
                    if(array_key_exists('sequence_format',$inivars[$db_section]))
                        $seqformat=$inivars[$db_section]['sequence_format'];
                    $options = array (
                        'debug' => $inivars[$db_section]['debug'],
                        'portability' => "MDB2_PORTABILITY_ALL",
                        'emulate_database' => false,
                        'seqname_format' => $seqformat,
                    );
                    break;

                case 'ogc':
                    require_once 'ACSPhpLib/acs_ogc_tools.php';

                    $this->ogc_with_curl = array_key_exists('with_curl', $inivars[$db_section]) ? $inivars[$db_section]['with_curl'] : false;
                    $this->ogc_call_type = array_key_exists('call_type', $inivars[$db_section]) ? $inivars[$db_section]['call_type'] : 'post';
                    $this->ogc_possible_errors = array_key_exists('exception_xpath', $inivars[$db_section]) ? $inivars[$db_section]['exception_xpath'] : null;
                    $this->ogc_url = $inivars[$db_section]['service_url'];
                    $this->ogc_RSDecoder = array_key_exists('res_set_decoder', $inivars[$db_section]) ? $inivars[$db_section]['res_set_decoder'] : _ACS_OGC_DEC_XML;
                    break;

                case 'monetdb':
                    require_once 'ACSPhpLib/extras/db/monetdb/php_monetdb.php';

                    $dsn = array (
                        'phptype' => 'monetdb',
                        'lang' => (array_key_exists('lang', // Default lang assumed to be 'sql'
                        $inivars[$db_section]
                    ) ? $inivars[$db_section]['lang'] : 'sql'),
                        'host' => $inivars[$db_section]['host'],
                        'port' => (array_key_exists('port', // Default port assumed to be 50000
                        $inivars[$db_section]
                    ) ? $inivars[$db_section]['port'] : 50000),
                        'username' => $inivars[$db_section]['user'],
                        'password' => $inivars[$db_section]['password'],
                        'database' => $inivars[$db_section]['db'],
                        'hashfunc' => (array_key_exists('hashfunc', // Default hashfunc assumed to be 'sha1'
                        $inivars[$db_section]
                    ) ? $inivars[$db_section]['hashfunc'] : 'sha1'),
                    );
                    break;

                default:
    				$dsn = array (
    					'new_link' => 'true',
    					'phptype' => $inivars[$db_section]['type'],
    					'username' => $inivars[$db_section]['user'],
    					'password' => $inivars[$db_section]['password'],
    					'hostspec' => $inivars[$db_section]['host'],
    					'database' => $inivars[$db_section]['db'],
    					'port' => (array_key_exists('port', // Default port assumed to be 3306
    	                $inivars[$db_section]
    				) ? $inivars[$db_section]['port'] : 3306));
                    break;
			}

		} else {
			// Use provided dsn
			$dsn = $inivars[$db_section]['dsn'];
		}

		// DON'T CHANGE THIS TO FACTORY OR SINGLETON BECAUSE
		// ME MUST ALWAYS CHECK THE CONNECTION
		switch($this->_dbType){
            case 'ifx':
			try {
                $this->_dbConn = new PDO($dsn, $user, $password);
                $this->_dbConn->setAttribute ( PDO::ATTR_CASE, PDO::CASE_NATURAL );

					// If specified, set a timeout to handle table locks.
					// Informix default behavior is to return an error if a transaction
					// locks a table and a select query is performed
					if (array_key_exists('lock_mode', $inivars[$db_section]))
						$this->execQuery('SET LOCK MODE TO ' .$inivars[$db_section]['lock_mode'], acs_idb::_SQL_EXEC);

			} catch (PDOException $e) {
				throw new acs_exDBErrorConnection(__METHOD__ . " : ($db_section) " . $e->getMessage());
			}
                break;

            case 'ogc':
                break;

            case 'callback':
    			$this->_dbConn = null;
    			if ($this->_logHandler !== null)
    				$this->_logHandler->debug("MDB2, using callback 'connection'");
                    break;

            case 'monetdb':
    			$this->_dbConn = monetdb_connect($dsn['lang'], $dsn['host'], $dsn['port'],
                               $dsn['username'], $dsn['password'], $dsn['database'], $dsn['hashfunc']);
                if ($this->_dbConn === FALSE)
					throw new acs_exDBErrorConnection(__METHOD__ . " : ($db_section) " . monetdb_last_error());
				break;

			case 'pdo':
				$this->_dbConn = new PDO($dsn);
				break;
				
            case 'cassandra':
            	$this->_dbConn = new PDO($dsn);
            	$this->_dbConn->exec("USE {$inivars[$db_section]['keyspace']};");
            	break;

            default:
    			$this->_dbConn = MDB2 :: connect($dsn, $options);

    			if (PEAR :: isError($this->_dbConn))
    				throw new acs_exDBErrorConnection(__METHOD__ . " : ($db_section) " . $this->_dbConn->getMessage()." => ".$this->_dbConn->userinfo);

    			$this->_dbConn->loadModule('Extended');
		}
		
		// Initialization sql; use a key in your db section like 'initsql_1 = "ALTER SESSION SET......"'
		foreach ($inivars[$db_section] as $k => $sql)
			if (strpos($k, 'initsql_') === 0) $this->execQuery($sql, acs_idb::_SQL_EXEC);

        // check for history tables
        if(array_key_exists('history_tables',$inivars[$db_section])) {
            require_once "ACSPhpLib/acs_history.php";
            $histSections=$inivars[$db_section]['history_tables'];
            if(!is_array($histSections)) $histSections=array($histSections);
            foreach($histSections as $histSection){
                if(!array_key_exists($histSection,$inivars))
                    throw new acs_exFailed("Section $histSection declared in history_tables is not declared in the ini file");
                    $histObj = acs_history::createFromIniSection($this,$inivars[$histSection],$this->applicationUserId);
                    $this->addHistoryListener($histObj);
            }

        }
	}
	/**
	 * @return misc dbconnection
	 * @access public
	 */
	public function & getDBConnection() {
		if ($this->_dbType == 'ifx') return $this;
        else return $this->_dbConn;
	}

 	public function __destruct() {
        switch($this->_dbType){
            case 'oci8':
            	if( ! PEAR::isError($this->_dbConn) )
            		$this->_dbConn->disconnect();
            	// do not break
            case 'ifx':
                $this->_dbConn=null;
                break;

            case 'callback':
            case 'ogc':
                break;

            case 'monetdb':
                monetdb_disconnect($this->_dbConn);
                $this->_dbConn=null;
                break;
                
            case 'cassandra':
            case 'pdo':
            	unset($this->_dbConn);
            	break;

            default:
                  if( ! PEAR::isError($this->_dbConn) ) $this->_dbConn->disconnect();
          }
    }

	static public function & getCallbackConnection($ini = array(), $logHandler = null) {
		$ini = array_merge($ini, array('callback' => array('type' => 'callback')));
 		return new acs_mdb2($ini, $logHandler, false, 'callback');
	}

	private function getSqlOperation($sql) {
		$matches = array();
		if (!preg_match('/\s*(\w+)\s.*/', $sql, $matches))
			throw new Exception("Cannot find sql operation in command: $sql");
		 return strtoupper($matches[1]);
	}

	/**
	 * Wraps a query
	 * @access public
	 */
	public function execQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
		$this->columns = array();
		$this->columntypes = array();
		$this->nativecolumntypes = array();
		$this->nativecolumnsizes = array();

		$dump = false;

        if ($this->_debugSQL) {
        	
        	$sql_str = $sql;
        	$sql_op = array();
        	
        	if (array_key_exists('sql_dump_operations', $this->_inivars['log'])) {
        		if (is_array($this->_inivars['log']['sql_dump_operations']))
        			$sql_op = $this->_inivars['log']['sql_dump_operations'];
        		else
        			$sql_op = array($this->_inivars['log']['sql_dump_operations']);
        	} else 
        		$sql_op = array('SELECT', 'UPDATE', 'INSERT', 'DELETE');
        	if (is_array($sql_str)) {
        		foreach ($sql_str as $k => $v) {
        			if (!in_array($this->getSqlOperation($v), $sql_op))
        				unset($sql_str[$k]);
        		}
        	} else 
        		if (!in_array($this->getSqlOperation($sql), $sql_op))
        			$sql_str = array();
// throw new Exception(print_r($this->_inivars['log']['sql_dump_operations'], true));
        	if (!empty($sql_str) || count($sql_str) > 0) {
	            $msg = 'SQL DUMP: ' . var_export($sql_str,true);
	            if ($this->_logHandler !== null)
	                $this->_logHandler->notice($msg);
	            else
	                echo $msg . "\n";
	            $start_time = acs_tools::microtime_float();
	            $dump = true;
        	}
        }

		switch ($this->_dbType) {
			case 'ifx':
				$res = $this->_ifxExecQuery($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
				break;
        	case 'ogc':

	            if($this->ogc_with_curl){
	                $cUrlObj=new acs_curl(false);
	                switch($this->ogc_call_type){
	                	case 'post':
	                		$res = $cUrlObj->post($this->ogc_url,$sql);
	                		break;
	
	                	case 'get':
	                		if ($this->_logHandler !== null)
	                			$this->_logHandler->debug("URL for server: " . print_r($this->ogc_url.'?'.$sql, true));
	                		$res = $cUrlObj->get($this->ogc_url.'?'.$sql);
	                		break;
	
	                	default:
	                		throw new Exception("Unknown call type for ogc curl call ({$this->ogc_call_type})");
	                }
	
	            } else {
	                switch($this->ogc_call_type){
	                	case 'post':
	                		$res = acs_callurl::post($this->ogc_url,$sql);
	                		break;
	
	                	case 'get':
	                		$res = acs_callurl::get($this->ogc_url,$sql);
	                		break;
	
	                	default:
	                		throw new Exception("Unknown call type for ogc call ({$this->ogc_call_type})");
	                }
	            }
	            if ($this->_logHandler !== null)
	            	$this->_logHandler->debug("Response from server: " . print_r($res, true));
	            // Check for errors
	            $xmlres = @simplexml_load_string($res);
	            if($xmlres===false) throw new acs_exDBErrorConnection('No valid response from OGC Service');
	            if(isset($this->ogc_possible_errors)){
	                $possible_errors = explode(",", $this->ogc_possible_errors);
	
	                $ns = $xmlres->getDocNamespaces(true);
	                if(is_array($ns)){
	                    foreach ($ns as $prefix => $namespace)
	                        $xmlres->registerXPathNamespace($prefix, $namespace);
	                }
	                foreach ($possible_errors as $error_xpath) {
	//throw new Exception(var_export($error_xpath,true));
	                    $node = @$xmlres->xpath(trim($error_xpath));
	
	                    if ($node!=false) {
	                        // search by node 0
	                        $err = (string)$node[0];
	                        if (trim($err) != '')
	                            throw new Exception("Error calling OGC Service at '{$this->ogc_url}' of type '$error_node': $err, xml file is $xmlname");
	
	                        // search by attribute
	                        $attrObjName = $node[0]->attributes();
	                        $err = $attrObjName['exceptionCode'];
	                        if (trim($err) != '')
	                            throw new Exception("Error calling OGC Service at '{$this->ogc_url}' of type '$error_node': $err, xml file is $xmlname");
	                    }
	                }
	            }
	            $res = acs_ogc_decoders::ogcToResultSet($this->ogc_RSDecoder,$xmlres);
	            break;
			case 'callback':
				$res = $this->callbackExecQuery($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
				break;
			case 'monetdb':
				$res = $this->_monetdbExecQuery($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
				break;
			
			case 'cassandra':
			case 'pdo':
				$res = $this->_pdoExecQuery($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
				break;
			
			default:
				$res = $this->_execQuery($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
			break;
		}
		if ($this->_debugSQL && $dump)
		{
			$msg = 'SQL DUMP: last query took ' . ((acs_tools::microtime_float() - $start_time) * 1000) . ' ms';
			if ($this->_logHandler !== null)
				$this->_logHandler->notice($msg);
		}
//        throw new Exception(var_export($sql,true));
        if($this->allKeysLowercase && is_array($res)) {
             if(array_key_exists(0,$res)){
             	foreach($res as $k=>$row) $res[$k]=array_change_key_case($row,CASE_LOWER);
             } else
                $res=array_change_key_case($res,CASE_LOWER);

//throw new Exception(var_export($res,true));
        }
        return $res;
	}

	protected function callbackExecQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {

		if ($type != acs_idb :: _SQL_GETALL)
			throw new Exception("With {$this->_dbConn} connections only acs_idb::_SQL_GETALL is implemented");

		if ($this->_logHandler !== null)
			$this->_logHandler->debug("MDB2 callback: calling method " . (isset($sql['callback'][1]) ? $sql['callback'][1] : $sql['callback'][0]));
		$keys = $sql;
		unset($keys['_callback']);

		$results = call_user_func($sql['_callback'], $keys, $type, $mode, $prepareParam, $paramtypes, $types, $column);

		if (is_array($results) && count($results))
		{
			$firstrow = reset($results);
			$this->columns = array_keys($firstrow);
			foreach ($firstrow as $colname => $colvalue)
				$this->nativecolumntypes[$colname] = gettype($colvalue);
			$this->columntypes = $this->mapTypes($this->nativecolumntypes);
		}

		return $results;
	}
	
	protected function patchQuotedStrings($str)
	{
		return preg_replace(array("/('.*')/Ue"), array("str_repeat('#', strlen('\\0'))"), $str);
	}

	protected function _execQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
		switch ($type) {
			case acs_idb :: _SQL_GETROW :
				$res = $this->_dbConn->extended->getRow($sql, $types, $prepareParam, $paramtypes, $mode);
				break;

			case acs_idb :: _SQL_GETALL :
				//$res = $this->_dbConn->queryAll($sql, $types, $mode);
				$res = $this->mdb2QueryAll($sql, $types, $mode, false, false, false, $prepareParam, $paramtypes);
				break;

			case acs_idb :: _SQL_EXEC :
                if(!is_array($sql)) $sql = array($sql);

                require_once 'ACSPhpLib/extras/sqltokenizer/SqlParser.php';
                foreach($sql as $sqlStatement){


                    $handle = SqlParser::ParseString($sqlStatement);

                    $historyPrimaryKeysRows=null;
                    $hashistoryListener=array_key_exists($handle->query['tablename'],$this->_historyListeners);

                    // if there's a listener associated with this table
                    if($hashistoryListener){
                        if(isset($handle->query['update'])){ // if it is un update
                            // build select query in order to get list of involved primary keys
                            $tablename=$handle->query['tablename'];
                            $histObj=$this->getHistoryListener($tablename);
                            $primaryKeys=$histObj->getMonitoredTablePrimaryKeysAsString();
                            $sql = "SELECT $primaryKeys FROM $tablename {$handle->query['where']}";
                            $historyPrimaryKeysRows = $this->execQuery($sql,acs_idb :: _SQL_GETALL);
                        }
                    }

					$skip_std_exec = false;

                    // now execute the query
                	if (sizeof($prepareParam) > 0)
                	{
               			$has_oids = false;
                		if ($this->_dbType == 'pgsql')
                		{
	                		foreach ($paramtypes as $k => $tp)
	                			if ($tp == 'oid')
	                			{
	                				$has_oids = true;

	                				$db = $this->_dbConn->connection;
	                				$oid = pg_lo_create($db);
	                				if ($oid === false)
										throw new Exception("Cannot create large object for query '$sql'");
									if ($this->_logHandler !== null)
										$this->_logHandler->debug("Created large object with id = $oid");
									$handle = pg_lo_open($db, $oid, "w");
									if ($handle === false)
										throw new Exception("Cannot open large object for query '$sql'");
									if (pg_lo_write($handle, $prepareParam[$k]) === false)
										throw new Exception("Cannot write large object for query '$sql'");
									pg_lo_close($handle);
									// substitute carried param with the oid number
									$prepareParam[$k] = $oid;
									$paramtypes[$k] = 'integer';
	                			}
                		} elseif ($this->_dbType == 'oci8') {
							$lobs = array();

							// Remove literal strings from incoming sql query
							// and substitute them with same length 
							$clean_sql = $this->patchQuotedStrings($sqlStatement);
//var_dump($sqlStatement);
//var_dump($clean_sql);
							if (strpos($clean_sql, '?') !== false)	// check for a query placeholder
							{
								// First step: get all field names to check which field has a ? placeholder
								if (isset($handle->query['update']))
								{
									// take real fields from the sqlparsed structure
									$real_fields = $handle->query['set_params'];
									$where = $this->patchQuotedStrings($handle->query['where']);
									if (strpos($where, '?') !== false)
									{
										$where_fields = array();
										// additional placeholders also in the where clause, try guessing names
										$tokens = preg_split('/[-=+\/\s]+/i', $where);

										foreach ($tokens as $idx => $t)
											if ($t == '?')
												$where_fields[$tokens[$idx - 1]] = $t;
										$real_fields = array_merge($real_fields, $where_fields);
									}
								}
								elseif (isset($handle->query['insert']))
								{
									// parse fields from the clean_sql string
									preg_match('/\s*INSERT\s+INTO\s+(\w+)\s*\((.*)\)\s*VALUES\s*\((.*)\)\s*/iU', $clean_sql, $matches);
									
									$splitter = '/\s*,\s*/';
									$real_fields = array_combine(preg_split($splitter, $matches[2]), preg_split($splitter, $matches[3]));
								}
								else
									throw new acs_exFailed("Unforeseen query type while looking for prepared statement placeholders"); 
								
								// Second step: replace all ? placeholders with oracle-like placeholders (:fieldname)
								$ph_pos = 0;
								foreach ($real_fields as $fieldname => $temp_value)
								{
									$fieldname = str_replace('"', '', $fieldname);	// remove quote from special identifier column
									if ($temp_value == '?')
									{
										// search ? placeholder in the clean sql string
										$ph_pos = strpos($clean_sql, '?', $ph_pos + 1);
										// replace in the real sql the ? with :field placeholder
										$sqlStatement = substr($sqlStatement, 0, $ph_pos) . ":$fieldname" . substr($sqlStatement, $ph_pos + 1);
										$clean_sql = substr($clean_sql, 0, $ph_pos) . ":$fieldname" . substr($clean_sql, $ph_pos + 1);
									}
								}
//echo "after replacements\n";
//var_dump($sqlStatement);
//var_dump($clean_sql);
//var_dump($real_fields); die;
							}
							
							// Extract :field placeholders
							if (!preg_match_all('/:[a-z_[0-9]+/i', $clean_sql, $matches))
								throw new acs_exFailed("Cannot find ORACLE placeholders (like :field) in sql: $sqlStatement");
							foreach ($matches[0] as $placeholder)
								$query_params_fields_mapping[$placeholder] = substr($placeholder, 1);
							$query_params = array_values($query_params_fields_mapping);
							if (count($query_params) != count($prepareParam))
								throw new acs_exFailed("Wrong query placeholders count " . count($query_params) . ", expected " . count($prepareParam) .
														"\nPlaceholders are: " . print_r($query_params, true) .
														"\nClean sql is: $clean_sql");

							$ret_flds = array();
							foreach ($paramtypes as $k => $tp) {
								$name = $query_params[$k];
								if ($tp == _ACS_DBTYPE_BLOB || $tp == _ACS_DBTYPE_CLOB)
								{
									$prepared_vars[] = array('name' => $name,
															'value' => & $prepareParam[$k],
															'lob_func' => $tp == _ACS_DBTYPE_BLOB ? 'EMPTY_BLOB()' : 'EMPTY_CLOB()',
															'placeholder' => ":$name",
															'type' => $tp,
															);
									$curr_var = end($prepared_vars);
									$sqlStatement = str_replace($curr_var['placeholder'], $curr_var['lob_func'], $sqlStatement);
									$ret_flds[] = $query_params_fields_mapping[$curr_var['placeholder']];
									$ret_ph[] = $curr_var['placeholder'];
								}
								else
									$prepared_vars[] = array('name' => $name,
															'value' => & $prepareParam[$k],
															'placeholder' => ":$name",
															);
							}
							
							if (sizeof($ret_flds))
							{
								$sqlStatement .= ' RETURNING ';
								$sqlStatement .= join(', ', $ret_flds) . ' INTO ' . join(', ', $ret_ph);
							}
							try {
								$s = oci_parse($this->_dbConn->connection, $sqlStatement);

								foreach ($prepared_vars as $k => $var)
									if ($var['type'] == _ACS_DBTYPE_BLOB || $var['type'] == _ACS_DBTYPE_CLOB)
									{
										$lobs[$k] = oci_new_descriptor($this->_dbConn->connection, OCI_D_LOB);
										oci_bind_by_name($s, $var['placeholder'], $lobs[$k], -1,
															$var['type'] == _ACS_DBTYPE_BLOB ? OCI_B_BLOB : OCI_B_CLOB);
									}
									else
										oci_bind_by_name($s, $var['placeholder'], $var['value']);
								
								ob_start();
								$ex_ret = oci_execute($s, OCI_DEFAULT);
								$ex_output = ob_get_flush();
								if (!$ex_ret || $ex_output)
									throw new acs_exFailed("ORACLE ERROR: " . print_r(oci_error($this->_dbConn->connection), true) . $ex_output);
								foreach ($lobs as $k => $lob_obj)
								{
									if (!$lob_obj->save($prepareParam[$k]))
										throw new acs_exFailed("Cannot save ORACLE LOB");
								}
								oci_commit($this->_dbConn->connection);

								oci_free_statement($s);
							} catch (Exception $e) {
								throw new acs_exFailed(__METHOD__ . ": ORACLE acs_mdb2 error: " . $e->getMessage() . " sql: $sqlStatement");
							}

							// Skip further processing
							$skip_std_exec = true;
                		}

                		if ($skip_std_exec == false) {
	                		$stm = $this->_dbConn->prepare($sqlStatement, $paramtypes, MDB2_PREPARE_MANIP);
	                		if (PEAR::isError($stm))
	                		{
	                			$res = $stm;
	                			break;
	                		}
	                		$res = $stm->execute($prepareParam);
	                		$stm->free();
                		}
                	}
                	else {
                    	$res= $this->_dbConn->exec($sqlStatement);
                	}
                    if (PEAR :: isError($res)) break;

                    // if here, query was ok, so update history table if necessary
                    if($hashistoryListener){
                        $historyTableName=$histObj->getHistoryTableName();

                        $tablename=$handle->query['tablename'];
                        $monitoredTablePrimaryKeys=$histObj->getMonitoredTablePrimaryKeys();
                        $whereAr='';
                        foreach($historyPrimaryKeysRows as $histRow){
                            foreach($histRow as $pkField=>$pkValue){
                                if($monitoredTablePrimaryKeys[$pkField]=='quote') $pkValue=$this->getSQLObj()->sql_quote_str($pkValue);
                                $whereAr[]="$pkField=$pkValue";
                            }
                        }
                        $monitoredFields=array_merge($histObj->getMonitoredTablePrimaryKeys(),$histObj->getMonitoredFields());
                        $monitoredFieldsString = join(', ', array_merge($monitoredFields,$histObj->getExtraFieldsValues()));
                        $sqltpl="INSERT INTO $historyTableName SELECT $monitoredFieldsString FROM $tablename WHERE {WHERE}";
                        $sql = str_replace('{WHERE}',join('AND',$whereAr),$sqltpl);
                        $this->execQuery($sql,acs_idb :: _SQL_EXEC);
                        unset($historyPrimaryKeysRows);
                        unset($histObj);
                    }
                }
				break;

			case acs_idb :: _SQL_GETASSOC :
				$res = & $this->_dbConn->extended->getAssoc($sql, $types, $prepareParam, $paramtypes, $mode);
				break;

			case acs_idb :: _SQL_GETCOL :
				$res = & $this->_dbConn->extended->getCol($sql, $types, $prepareParam, $paramtypes, $column);
				break;

			default :
				throw new Exception(__METHOD__ . " : Unknown query type => $type!");
		}
		if (PEAR :: isError($res)) {
			$msg = $res->message . "  " . $res->userinfo;
			// [nativecode=1406 ** Data too long for column 'processor_version' at row 1]
			if (preg_match_all("|\[nativecode.*\]|", $msg, $out, PREG_PATTERN_ORDER)) {
				$msg = join('', $out[0]);
			}
			if ($this->_logHandler !== null)
				$this->_logHandler->err(__METHOD__ . " " . $res->message . "  " . $res->userinfo);
			throw new acs_exDBErrorQuery($msg);
		}
		return $res;
	}

    public function getNativeMessage($e) {
    	$msg = $e->getMessage();
   		switch ($this->_dbType) {
			case 'pgsql':
				$m = preg_split('/Native message: ERROR: /', $msg, -1, PREG_SPLIT_OFFSET_CAPTURE);
				$msg = $m[1][0];
				$m = preg_split('/\]/', $msg, -1, PREG_SPLIT_OFFSET_CAPTURE);
				$msg = $m[0][0];
				break;
			case 'ifx':
				$m = preg_replace("|\[Informix\]\[Informix ODBC Driver\]\[Informix\]|", "", $msg);
				$m = preg_split('/\(/', $m, -1, PREG_SPLIT_OFFSET_CAPTURE);
				$msg = $m[0][0];
				break;
			case 'oci8':
				$m = preg_split('/Native message: /', $msg, -1, PREG_SPLIT_OFFSET_CAPTURE);
				$msg = $m[1][0];
				$m = preg_split('/\]/', $msg, -1, PREG_SPLIT_OFFSET_CAPTURE);
				$msg = $m[0][0];
				break;
			default:
				break;
		}
		return rtrim($msg);
    }

	protected function _ifxExecQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
        switch ($type) {
			case acs_idb :: _SQL_EXEC :
                if(!is_array($sql)) $sql=array($sql);
                foreach($sql as $sqlStatement){
                    $stmt = $this->_dbConn->prepare($sqlStatement);
                    if($stmt===false) $this->raiseIFXError($this->_dbConn,__METHOD__,$sql);
                    if (!empty($prepareParam) && !empty($paramtypes))
                    {
                      for ($i = 0; $i < count($prepareParam); $i++)
                      {
                        if ($paramtypes[$i] == 'CLOB' || $paramtypes[$i] == 'BLOB')
                        {
                          $stmt->bindParam($i+1, $prepareParam[$i], PDO::PARAM_LOB);
                        }
                        else
                        {
                        	$stmt->bindParam($i+1, $prepareParam[$i]) ;
                        }
                      }
                    }

                    if($this->autocommit) $this->beginTransaction();
                    $res = $stmt->execute();
                    $resAr=$stmt->errorInfo();
                    if(($res===false)||($resAr[1]!=0)) $this->raiseIFXError($stmt,__METHOD__,$sql);
                }
                if($this->autocommit) $this->commit();

                break;

			case acs_idb :: _SQL_GETROW :
			case acs_idb :: _SQL_GETALL :
			case acs_idb :: _SQL_GETASSOC :
			case acs_idb :: _SQL_GETCOL :
				$res = $this->_executeIfxSelect($sql, $type, $mode, $prepareParam, $paramtypes, $types, $column);
				break;

			default :
				throw new acs_exBadParam(__METHOD__ . " : Unknown query type => $type");
		}
		return $res;
	}

    protected function _monetdbExecQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
        if ($mode != MDB2_FETCHMODE_ASSOC)
            throw new acs_exNotCapable(__function__ . ' not implemented for mode != MDB2_FETCHMODE_ASSOC');
        if (($prepareParam !== array()) || ($paramtypes !== null))
            throw new acs_exNotCapable(__function__ . ' lacks of support for prepared statements');
        if ($types !== null)
            throw new acs_exNotCapable(__function__ . ' lacks of support for setting data types when fetching results');
        switch ($type) {
            case acs_idb::_SQL_GETROW :
            case acs_idb::_SQL_GETALL :
            case acs_idb::_SQL_GETCOL :
                $rh = monetdb_query($this->_dbConn, $sql);
                if ($rh === FALSE) {
                    $msg = monetdb_last_error();
                    if ($this->_logHandler !== null)
                        $this->_logHandler->err(__METHOD__ . ' ' . $msg);
                    throw new acs_exDBErrorQuery($msg);
                }
                break;
            case acs_idb::_SQL_EXEC :
                break;

            case acs_idb::_SQL_GETASSOC :
                throw new acs_exNotCapable(__function__ . ' not implemented for type == acs_idb::_SQL_GETASSOC');
            default :
                throw new Exception(__METHOD__ . " : Unknown query type => $type!");
        }
        switch ($type) {
            case acs_idb::_SQL_GETROW :
                $res = monetdb_fetch_assoc($rh);
                if ($res === FALSE) $res = null;
                monetdb_free_result($rh);
                break;

            case acs_idb::_SQL_GETALL :
                $res = array();
                $i = 0;
                $row = monetdb_fetch_assoc($rh);
                while ($row !== FALSE) {
                    $res[$i++] = $row;
                    $row = monetdb_fetch_assoc($rh);
                }
                monetdb_free_result($rh);
                break;

            case acs_idb::_SQL_EXEC :
                if (!is_array($sql)) $sql = array($sql);
                foreach($sql as $sqlStatement) {
                    $rh = monetdb_query($this->_dbConn, $sqlStatement);
                    if ($rh === FALSE) {
                        $msg = monetdb_last_error();
                        if ($this->_logHandler !== null)
                            $this->_logHandler->err(__METHOD__ . ' ' . $msg);
                        throw new acs_exDBErrorQuery($msg);
                    }
                    $res = monetdb_affected_rows($rh);
                    if ($res === FALSE) $res = 0;
                    else $res = intval($res);
                    monetdb_free_result($rh);
                }
                break;

            case acs_idb::_SQL_GETCOL :
                $res = array();
                $i = 0;
                $row = monetdb_fetch_row($rh);
                while ($row !== FALSE) {
                    $res[$i++] = $row[$column];
                    $row = monetdb_fetch_row($rh);
                }
                monetdb_free_result($rh);
        }
        return $res;
    }
    
    protected function _pdoExecQuery($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
    	$typeMap = array(_ACS_DBTYPE_INT		=> PDO::PARAM_INT,
						_ACS_DBTYPE_TEXT		=> PDO::PARAM_STR,		
						_ACS_DBTYPE_TIMESTAMP	=> null,
						_ACS_DBTYPE_BOOLEAN		=> PDO::PARAM_BOOL,
						_ACS_DBTYPE_DECIMAL		=> null,
						_ACS_DBTYPE_FLOAT		=> null,
						_ACS_DBTYPE_TIME		=> null,	
						_ACS_DBTYPE_DATE		=> null,	
						_ACS_DBTYPE_CLOB		=> PDO::PARAM_LOB,	
						_ACS_DBTYPE_BLOB		=> PDO::PARAM_LOB);
    	
    	if ($mode != MDB2_FETCHMODE_ASSOC)
    		throw new acs_exNotCapable(__METHOD__ . " only MDB2_FETCHMODE_ASSOC is supported");
    	if ($types !== null)
    		throw new acs_exNotCapable(__METHOD__ . ' lacks of support for setting data types when fetching results');
    	
    	$stmt = $this->_dbConn->prepare($sql);
    	if ($stmt == false)
    		throw new acs_exDBErrorQuery(join(' ', $this->_dbConn->errorInfo()));
    	
    	for ($c = 0; $c < sizeof($prepareParam); $c++) {
    		$stmt->bindValue($c + 1, $prepareParam[$c], $typeMap[$paramtypes[$c]]);
    	}
    	
    	switch ($type) {
    		case acs_idb::_SQL_GETROW :
    		case acs_idb::_SQL_GETALL :
    		case acs_idb::_SQL_GETCOL :
    		case acs_idb::_SQL_EXEC :
    			if (!$stmt->execute())
    				throw new acs_exDBErrorQuery(join(' ', $this->_dbConn->errorInfo()));
    			break;
    	
    		case acs_idb::_SQL_GETASSOC :
    			throw new acs_exNotCapable(__METHOD__ . ' not implemented for type == acs_idb::_SQL_GETASSOC');
    		default :
    			throw new Exception(__METHOD__ . " : Unknown query type => $type!");
    	}
    	
    	switch ($type) {
    		case acs_idb::_SQL_GETROW :
    			$res = $stmt->fetch();
    			break;
    		case acs_idb::_SQL_GETALL :
    			$res = $stmt->fetchAll(PDO::FETCH_ASSOC);
    			break;
    		case acs_idb::_SQL_GETCOL :
    			$res = array();
    			foreach ($stmt->fetchAll() as $row)
    				$res[] = $row[$column];
    			break;
    		default:
    			$res = null;
    	}
    	return $res;
    }

	public function lastId($table = null, $field = null, $id = null) {
		switch ($this->_dbType) {
			case 'cassandra':
				throw new Exception(__METHOD__ . " not implemented by the driver");

			case 'pdo':
			case 'ifx':
				// returns the autoincrement ID if supported or $id
				$lid = $this->_dbConn->lastInsertID();
				break;

			default:
	            if (($this->_dbType == 'pgsql' && $this->autocommit === false)
	                || ($this->_dbType == 'monetdb'))
	        	{
	        		$row = $this->execQuery("SELECT max($field) as lid FROM $table", acs_idb :: _SQL_GETROW);
	        		$lid = intval($row['lid']);
	        	}
	        	else
	        		$lid = $this->_dbConn->lastInsertID($table, $field, $id);
        }

		if (PEAR::isError($lid))
			throw new Exception("Error getting last inserted id (table=$table,field=$field,id=$id: " . $lid->userinfo);

        if (($this->_debugSQL) && ($this->_logHandler !== null)) $this->_logHandler->notice(__METHOD__." = $lid");

        return $lid;
	}

	function _executeIfxSelect($sql, $type, $mode = MDB2_FETCHMODE_ASSOC, $prepareParam = array (), $paramtypes = null, $types = null, $column = 0) {
		// handle blob as file
        if(is_array(self::$_blobFieldsAsFile)){
        	foreach(self::$_blobFieldsAsFile as $bfield) {
        		$tmpFname=acs_file::getTempDir().'/'.acs_file::getUniqueFileName().'ifx';
                $sql=str_replace("<$bfield>",'LOToFile('.$bfield.',"'.$tmpFname.'","client") as '.$bfield.'_blob_',$sql);
        	}
        }
        $stmt = $this->_dbConn->prepare($sql);
        if($stmt===false) $this->raiseIFXError($this->_dbConn,__METHOD__,$sql);

        $err=$stmt->execute();
        if($err===false) $this->raiseIFXError($stmt,__METHOD__,$sql);

		$clobcol = array ();
		for ($i = 0; $i < $stmt->columnCount(); $i++) {
			$metadata = $stmt->getColumnMeta($i);

			if ($i == $column)
				$colname = $metadata['name'];

			if ($i == 0)
				$assoccol = $metadata['name'];

			if ($metadata['pdo_type'] == PDO :: PARAM_LOB) {
				$stmt->bindColumn(($i +1), $clobcol[$metadata['name']]['value'], PDO :: PARAM_LOB);
			}

			$this->columns[] = $metadata['name'];
			$this->nativecolumntypes[$metadata['name']] = $metadata['native_type'];
		}

		$this->columntypes = $this->mapTypes($this->nativecolumntypes);

		$res = array ();
		if($mode==MDB2_FETCHMODE_ORDERED) $ifxmode=PDO::FETCH_NUM;
        else $ifxmode=PDO::FETCH_ASSOC;

        while ($a = $stmt->fetch($ifxmode)) {
			foreach ($clobcol as $k => $v) {
			//	ob_start();
			//	fpassthru($clobcol[$k]['value']);
			//	$x = ob_get_contents();
			//	ob_end_clean();
                $a[$k] = stream_get_contents($clobcol[$k]['value']);
			}
            if(is_array(self::$_blobFieldsAsFile)){
                foreach(self::$_blobFieldsAsFile as $bfield) {
                    $tmpFname=$a[$bfield.'_blob_'];
                    if(empty($tmpFname)) continue;
                    $a[$bfield]=file_get_contents($tmpFname);
                    unlink($tmpFname);
                }
            }
			$res[] = $a;
		}

		switch ($type) {
			case acs_idb :: _SQL_GETROW :
				return $res[0];
				break;
			case acs_idb :: _SQL_GETALL :
				return $res;
				break;
			case acs_idb :: _SQL_GETASSOC :
				foreach ($res as $k => $v) {
					$newres[$v[$assoccol]] = '';
					foreach ($v as $key => $value) {
						if ($key != $assoccol)
							$newres[$v[$assoccol]][$key] = $value;
					}
				}
				return $newres;

				break;
			case acs_idb :: _SQL_GETCOL :
				foreach ($res as $k => $v)
					foreach ($v as $key => $value)
						if ($key == $colname)
							$newres[] = $value;
				return $newres;
				break;
		}
	}

    public function raiseIFXError(&$errObj, $methodName, $extraText=''){
        $errmsgAr=$errObj->errorInfo();
        if(is_array($extraText)) $extraText=join(', ',$extraText);

        // Case of error on a table lock. No error code is returned due to a driver bug.
		if (is_array($errmsgAr) && array_key_exists(1, $errmsgAr) && $errmsgAr[1] == 0)
			throw new acs_exDBErrorQuery('Database is busy'.($extraText<>''?' ('.$extraText.')':''));

        throw new acs_exDBErrorQuery(is_array($errmsgAr) && array_key_exists(2,$errmsgAr) ? $errmsgAr[2].($extraText<>''?' ('.$extraText.')':''):'Undefined error in '.$methodName.' : '.$extraText);
    }

    public function beginTransaction(){
        if ($this->_logHandler !== null) $this->_logHandler->debug(__METHOD__);
        $this->_dbConn->beginTransaction();
    }

    public function rollback(){
        if ($this->_logHandler !== null) $this->_logHandler->err(__METHOD__);
        $this->_dbConn->rollBack();
    }

    public function commit(){
        if ($this->_logHandler !== null) $this->_logHandler->debug(__METHOD__);
        $this->_dbConn->commit();
    }

    public function getDBType(){
        return $this->_dbType;
    }
    public function setLimit($pageSize,$from){
	  		$lres=$this->_dbConn->setLimit($pageSize,$from);
				if (PEAR::isError($lres)) {
								$msg="'Limit' claues is not supported by current backend";
								if ($this->_logHandler !== null)
									$this->_logHandler->err($msg);
								throw new Exception(__METHOD__." $msg");
					}
	  }
    public function getSQLObj(){
        return $this->_acsSqlObj;
    }

    function quote($value, $type = null, $quote = true, $escape_wildcards = false)
    {
        switch($this->_dbType){
        	case 'ifx': return '"' . str_replace('"', '""', $value) . '"';
            case 'mssql': return $this->_dbConn->quote(str_replace("'","''",$value), $type, $quote, $escape_wildcards);
            default: return $this->_dbConn->quote($value, $type, $quote, $escape_wildcards);
        }
    }

    public function registerBlobFields(){
        self::$_blobFieldsAsFile=func_get_args();
    }

    public function queryOne($sql){
        $row=$this->execQuery($sql, acs_idb::_SQL_GETROW);
        list($k,$v)=each($row);
        return $v; //$this->_dbConn->queryOne($sql);
    }

    public function queryAll($sql, $type, $mode = MDB2_FETCHMODE_ASSOC){
        return $this->execQuery($sql, acs_idb::_SQL_GETALL, $mode);
    }

	public function mapType($native_type, $native_size = null)
	{
		switch ($this->_dbType)
		{
			case 'mysql':
				return preg_replace(
							array(	'/^int$/',
									'/^date$/',
									'/^time$/',
									'/^datetime$/',
									'/^bool$/',
									'/^real/',
									'/^real$/',
									'/^clob$/',
									'/^blob$/',
									'/^string$/'),
							array(	_ACS_DBTYPE_INT,
									_ACS_DBTYPE_DATE,
									_ACS_DBTYPE_TIME,
									_ACS_DBTYPE_TIMESTAMP,
									_ACS_DBTYPE_BOOLEAN,
									_ACS_DBTYPE_DECIMAL,
									_ACS_DBTYPE_FLOAT,
									_ACS_DBTYPE_CLOB,
									_ACS_DBTYPE_BLOB,
									_ACS_DBTYPE_TEXT),
							$native_type);
			case 'pgsql':
				return preg_replace(
							array(	'/^int[0-9]*$/',
									'/^date$/',
									'/^time$/',
									'/^timestamp$/',
									'/^bool$/',
									'/^numeric/',
									'/^float[0-9]*$/',
									'/^text$/',
									'/^bytea$/',
									'/^varchar$/'),
							array(	_ACS_DBTYPE_INT,
									_ACS_DBTYPE_DATE,
									_ACS_DBTYPE_TIME,
									_ACS_DBTYPE_TIMESTAMP,
									_ACS_DBTYPE_BOOLEAN,
									_ACS_DBTYPE_DECIMAL,
									_ACS_DBTYPE_FLOAT,
									_ACS_DBTYPE_CLOB,
									_ACS_DBTYPE_BLOB,
									_ACS_DBTYPE_TEXT),
							$native_type);
			case 'ifx':
				return preg_replace(
							array(	'/^SERIAL$|^INTEGER$|^INT[0-9]*$|^SMALLINT$/',
									'/^DATETIME YEAR TO DAY$/',
									'/^DATETIME HOUR TO SECOND$/',
									'/^DATETIME YEAR TO FRACTION\([0-9]\)$/',
									'/^BOOLEAN$/',
									'/^DECIMAL$/',
									'/^FLOAT/',
									'/^CLOB$/',
									'/^BLOB$/',
									'/^VARCHAR$|^CHAR$/'),
							array(	_ACS_DBTYPE_INT,
									_ACS_DBTYPE_DATE,
									_ACS_DBTYPE_TIME,
									_ACS_DBTYPE_TIMESTAMP,
									_ACS_DBTYPE_BOOLEAN,
									_ACS_DBTYPE_DECIMAL,
									_ACS_DBTYPE_FLOAT,
									_ACS_DBTYPE_CLOB,
									_ACS_DBTYPE_BLOB,
									_ACS_DBTYPE_TEXT),
							$native_type);
			case 'callback':
				return  preg_replace(
							array(	'/^integer$/',
									//'/^DATETIME YEAR TO DAY$/',
									//'/^DATETIME HOUR TO SECOND$/',
									//'/^DATETIME YEAR TO FRACTION\([0-9]\)$/',
									'/^boolean$/',
									'/^double/',
									'/^double/',
									//'/^CLOB$/',
									//'/^BLOB$/',
									'/^string$/'),
							array(	_ACS_DBTYPE_INT,
									//_ACS_DBTYPE_DATE,
									//_ACS_DBTYPE_TIME,
									//_ACS_DBTYPE_TIMESTAMP,
									_ACS_DBTYPE_BOOLEAN,
									_ACS_DBTYPE_DECIMAL,
									_ACS_DBTYPE_FLOAT,
									//_ACS_DBTYPE_CLOB,
									//_ACS_DBTYPE_BLOB,
									_ACS_DBTYPE_TEXT),
							$native_type);
			case 'oci8':
				$type = preg_replace(
							array(	'/^NUMBER$/',
									'/^DATE$/',
									'/^time$/',		// tbd
									'/^TIMESTAMP$/',
									'/^bool$/',		// tbd
									'/^101$/',
									'/^100*$/',
									'/^CLOB|LONG$/',
									'/^BLOB$/',
									'/^CHAR|VARCHAR2$/'),
							array(	_ACS_DBTYPE_INT,
									_ACS_DBTYPE_DATE,
									_ACS_DBTYPE_TIME,
									_ACS_DBTYPE_TIMESTAMP,
									_ACS_DBTYPE_BOOLEAN,
									_ACS_DBTYPE_DECIMAL,
									_ACS_DBTYPE_FLOAT,
									_ACS_DBTYPE_CLOB,
									_ACS_DBTYPE_BLOB,
									_ACS_DBTYPE_TEXT),
							$native_type);

				return $type;
			default:
				throw new Exception("No type mapping defined for db connection type {$this->_dbType}");
		}
	}

	public function mapTypes($native_types, $native_sizes = array())
	{
		foreach ($native_types as $col_name => $col_type)
			$mdb2_types[$col_name] = $this->mapType($col_type, @$native_sizes[$col_name]);

		return $mdb2_types;
	}

    public function mdb2QueryAll($query, $types = null, $fetchmode = MDB2_FETCHMODE_ASSOC,
        $rekey = false, $force_array = false, $group = false, $prepareParam = array(), $paramtypes = array())
    {
    	if (sizeof($prepareParam) == 0) {
    		$stmt = null;
	        $result = $this->_dbConn->query($query, $types);
	        if (!MDB2::isResultCommon($result)) {
	            return $result;
	        }
    	} else {
    		$stmt = $this->_dbConn->prepare($query, $paramtypes, $types);
    		if (PEAR::isError($stmt)) {
    			return $stmt;
    		}
    		$result = $stmt->execute($prepareParam);
    		if (!MDB2::isResultCommon($result)) {
    			return $result;
    		}
    	}

        $all = $result->fetchAll($fetchmode, $rekey, $force_array, $group);
        // Save column names in the columns property
        $cols = $result->getColumnNames();
        if ($this->allKeysLowercase)
        	$cols = array_change_key_case($cols, CASE_LOWER);
		$this->columns = array_keys($cols);

		switch ($this->_dbType)
		{
			case 'mysql':
				foreach ($cols as $colname => $colidx)
				{
					$tp = mysql_field_type($result->result, $colidx);
					if ($tp == 'int' && mysql_field_len($result->result, $colidx) == 1)
						$this->nativecolumntypes[$colname] = 'boolean';
					else
						$this->nativecolumntypes[$colname] = $tp;
				}
				break;
			case 'pgsql':
				foreach ($cols as $colname => $colidx)
					$this->nativecolumntypes[$colname] = pg_field_type($result->result, $colidx);
				break;

			case 'oci8':
				foreach ($cols as $colname => $colidx)
				{
						$this->nativecolumntypes[$colname] = oci_field_type($result->result, $colidx + 1); // field idx is 1-based
						$this->nativecolumnsizes[$colname] = oci_field_size($result->result, $colidx + 1); // field idx is 1-based
				}
				break;
		}

		$this->columntypes = $this->mapTypes($this->nativecolumntypes, $this->nativecolumnsizes);

        $result->free();
        if ($stmt)
        	$stmt->free();
        return $all;
    }

    public function getPgOid($oid)
    {
    	$this->beginTransaction();
		$db = $this->_dbConn->connection;
		$handle = pg_lo_open($db, $oid, "r");
		if ($handle === FALSE)
		{
			$this->rollBack();
			throw new Exception("Cannot oper OID $oid");
		}

		do
		{
			$buf = pg_lo_read($handle, 50000);
			$read = strlen($buf);
			$data .= $buf;
		} while ($read > 0);
		$this->commit();

		return $data;
    }

    public function addHistoryListener(&$histListenerObj) {
    	require_once "ACSPhpLib/acs_history.php";

        if(!($histListenerObj instanceof acs_history)) throw new acs_exBadParam('The given History Listener does not derive from acs_history');
        $this->_historyListeners[$histListenerObj->getMonitoredTableName()]=$histListenerObj;
    }

    public function registerApplicationUserId($uid) {
    	foreach($this->_historyListeners as $histObj){
    	   $histObj->registerApplicationUserId($uid);
    	}
    }

    public function getHistoryListener($tablename) {
    	if(!array_key_exists($tablename,$this->_historyListeners))
            throw new acs_exBadParam("No History Listener available for table $tablename");
        return $this->_historyListeners[$tablename];

    }


}
?>