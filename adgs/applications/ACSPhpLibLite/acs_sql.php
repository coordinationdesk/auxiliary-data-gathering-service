<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions  5                                                           |
// +---------------------------------------------------------------------------+
// | Copyright   1995-2006 Advanced Computer Systems S.p.A. (A.C.S. S.p.A.) |
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
// $Author: emiliano.merlin $
// $Id: acs_sql.php,v 1.50 2012/11/29 11:06:24 emiliano.merlin Exp $
// $Revision: 1.50 $
//

class acs_sql
{
    protected $DBType='';


    public function __construct($dbtype) {
        $this->DBType=$dbtype;
    }

    public function sql_if($condition, $then_clause, $else_clause){
        switch($this->DBType){
            case 'mysql':
                return "IF($condition, $then_clause, $else_clause)";
                break;

            case 'pgsql':
            case 'oci8':
            case 'mssql':
            case 'ifx':
                return "(CASE WHEN $condition THEN $then_clause ELSE $else_clause END)";
                break;
            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_ifnull($value, $replace_with){
        switch($this->DBType){
            case 'mysql':
                return "IFNULL($value, $replace_with)";
                break;

            case 'oci8':
                return "NVL($value, $replace_with)";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_var($name){
        switch($this->DBType){
            case 'mysql':
                return "@$name";
                break;
            default:
                return $name;
        }
    }

    public function sql_concat($params){
        if(!is_array($params)) throw new acs_exBadParam('sql_concat needs an array as parameter');
        
        switch($this->DBType){
            case 'mysql':
            	if(!count($params)) return '';
            	return 'concat('.join(',',$params).')';

            case 'oci8':
            	return join(' || ',$params);
            
            case 'pgsql':
            	return join(' || ',$params);
            	
            default:
                return join(' + ',$params);
        }
    }

    public function sql_length($field){
        switch($this->DBType){
            case 'mysql':
            	return "strlen($field)";

            case 'oci8':
            	return "length($field)";
            	
            default:
                throw new Exception("sql_length not implemented for $this->DBType");
        }
    }

    public function sql_groupBy($fields, $withrollup=false){
        if(!is_array($fields)) $fields=array($fields);
        if($withrollup){
	        switch($this->DBType){
	            case 'mysql':
	            	return 'GROUP BY '.join(',',$fields).' WITH ROLLUP';
	
	            case 'pgsql':
	            	return 'GROUP BY rollup('.join(',',$fields).')';
	
	            default:
	                throw new Exception("Roll UP not implemented for $this->DBType");
	        }
        } else {
        	return 	'GROUP BY '.join(',',$fields);
        }
    }

    public function sql_set_var($name, $value, $type=''){
        switch($this->DBType){
            case 'mysql':
                if(stripos ($value,'select')!==false) return str_replace('SELECT ',"SELECT ".self::sql_var($name)." := ",$value);
                else return self::sql_var($name)." := ".$value;
                break;

            case 'ifx':
                if($type=='') throw new acs_exBadParam("TYPE is required for ".__METHOD__.", DB=".$this->DBType);
                return "DECLARE $name $type;\n LET $name = $value";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_autoinc($table_name=''){
        switch($this->DBType){
            case 'mysql':
                return "LAST_INSERT_ID()";
                break;

            case 'ifx':
                if($table_name=='') throw new acs_exBadParam("TABLE_NAME is required for ".__METHOD__.", DB=".$this->DBType);
                return "(SELECT FIRST 1 dbinfo('sqlca.sqlerrd1') FROM $table_name)";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_auto_date($format=''){
        switch($this->DBType){
            case 'mysql':
                if($format=='') $format='%Y-%m-%d';
                return "DATE_FORMAT(now(), '$format')";
                break;

            case 'mssql':
                return "getdate()";
                break;

            case 'ifx':
                return "TODAY";
                break;

            case 'pgsql':
                return "current_date";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_auto_datetime(){
        switch($this->DBType){
            case 'mysql':
            case 'monetdb':
            	return "now()";
                break;

            case 'mssql':
                return "getdate()";
                break;

            case 'ifx':
                return "CURRENT";
                break;

            case 'oci8':
                return "CURRENT_DATE";
                break;
            
			case 'pgsql':
                return "current_timestamp";
                break;
                
            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_format_date($year,$month,$day,$quoted=false) {
        switch($this->DBType){
            case 'mysql':
            case 'pgsql':
            case 'monetdb':
                $sql = sprintf('%04d-%02d-%02d',$year,$month,$day);
                if($quoted) $sql = $this->sql_quote_str($sql);
                return $sql;
                break;

            case 'mssql':
                $sql = sprintf('%04d%02d%02d',$year,$month,$day);
                if($quoted) $this->sql_quote_str($sql);
                return $sql;
                break;

            case 'ifx':
                return "EXTEND(MDY($month,$day,$year))";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_format_time($hour,$minute,$second) {
        switch($this->DBType){
            case 'pgsql':
            case 'mysql':
            case 'mssql':
            case 'monetdb':
                return sprintf("%02d:%02d:%02d",$hour,$minute,$second);
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_format_datetime($year,$month,$day,$hour,$minute,$second) {
        switch($this->DBType){
            case 'ifx':
            case 'monetdb':
                return sprintf("%04d-%02d-%02d %02d:%02d:%02d",$year,$month,$day,$hour,$minute,$second);
                break;

            case 'mysql':
            case 'mssql':
            case 'pgsql':
                return $this->sql_format_date($year,$month,$day).' '.$this->sql_format_time($hour,$minute,$second);
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_unix_epoch_to_date($uepoch,$dateType='NORMAL',$aftermsec='',$quoteMarker="'") {
        switch($this->DBType){
            case 'mysql':
            case 'mssql':
                return $quoteMarker.date("Ymd",$uepoch/1000).$quoteMarker;
                break;

            case 'pgsql':
            case 'ifx':
            case 'monetdb':
                switch($dateType){
                    case 'NORMAL':
                        $timestamp = (int)($uepoch / 1000);
                        $timestamp_nr = number_format(($uepoch /1000), 3); 
                        //$ms = substr((string)$uepoch, strlen((string)$uepoch) - 3, 3).$aftermsec;
                        $ms = substr((string)$timestamp_nr, strlen((string)$timestamp_nr) - 3, 3).$aftermsec;
                        return $quoteMarker.gmstrftime('%Y-%m-%d %H:%M:%S', $timestamp) . "." . $ms.$quoteMarker;

                    case 'DATEONLY':
                        $timestamp = (int)($uepoch / 1000);
                        return $quoteMarker.gmstrftime('%Y-%m-%d', $timestamp).$quoteMarker;

                    case 'START': return $quoteMarker.date("Y-m-d 00:00:00.000",$uepoch/1000).str_repeat('0',strlen($aftermsec)) .$quoteMarker;

                    case 'END': return $quoteMarker.date("Y-m-d 23:59:59.999",$uepoch/1000).str_repeat('9',strlen($aftermsec)) .$quoteMarker;

                }
                break;

            case 'oci8':
            	$timestamp = (int)($uepoch / 1000);
                switch($dateType){
                    case 'NORMAL':
                        //$ms = substr((string)$uepoch, strlen((string)$uepoch) - 3, 3).$aftermsec;
                        $timestamp_nr = number_format(($uepoch /1000), 3); 
                        $ms = substr((string)$timestamp_nr, strlen((string)$timestamp_nr) - 3, 3).$aftermsec;
                        return "to_timestamp('".gmdate("Y/m/d:H:i:s",$timestamp).sprintf('.%03d', $ms)."', 'YYYY/MM/DD:HH24:MI:SS.FF')";

                    case 'DATEONLY':
                    	return "to_date('".gmdate("Y/m/d",$timestamp)."', 'YYYY/MM/DD')";

                    case 'START': return "to_timestamp('".gmdate("Y/m/d",$timestamp).":00:00:00.0', 'YYYY/MM/DD:HH24:MI:SS.FF')";

                    case 'END': return "to_timestamp('".gmdate("Y/m/d",$timestamp).":23:59:59.999', 'YYYY/MM/DD:HH24:MI:SS.FF')";;

                }
                break;

            case 'ogc':
                switch($dateType){
                    case 'NORMAL':
                        $timestamp = (int)($uepoch / 1000);
                        //$ms = substr((string)$uepoch, strlen((string)$uepoch) - 3, 3).$aftermsec;
                        $timestamp_nr = number_format(($uepoch /1000), 3); 
                        $ms = substr((string)$timestamp_nr, strlen((string)$timestamp_nr) - 3, 3).$aftermsec;
                        return gmstrftime('%Y-%m-%dT%H:%M:%S', $timestamp) . '.' . $ms.'Z';

                    case 'DATEONLY':
                        $timestamp = (int)($uepoch / 1000);
                        return gmstrftime('%Y-%m-%d', $timestamp);

                    case 'START': return date("Y-m-dT00:00:00.000Z",$uepoch/1000).str_repeat('0',strlen($aftermsec));

                    case 'END': return date("Y-m-dT23:59:59.999Z",$uepoch/1000).str_repeat('9',strlen($aftermsec));

                }
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_unix_epoch_to_date_time($uepoch,$aftermsec='',$quoteMarker="'") {
        switch($this->DBType){
            case 'mysql':
            case 'mssql':
                return $quoteMarker.gmdate("YmdHis",$uepoch).$quoteMarker;
                break;

            case 'oci8':
                return "to_date('".gmdate("Y/m/d:H:i:s",$uepoch)."', 'yyyy/mm/dd:hh24:mi:ss')";

            default:
                return $this->sql_unix_epoch_to_date($uepoch,'NORMAL',$aftermsec,$quoteMarker);
        }
    }

    public function sql_iso8601_to_date_time($isoVal) {
		require_once 'acs_satellite_tools.php';
		$uts = acs_time_conversion::ISO8601toUnixTimeStamp($isoVal);
        return $this->sql_unix_epoch_to_date_time($uts);
    }

    public function sql_format_date_time($fieldname) {
        switch($this->DBType){
            case 'mysql':
                return "DATE_FORMAT($fieldname,GET_FORMAT(DATETIME,'ISO'))";

            case 'oci8':
                return "to_char($fieldname, 'DD-Mon-YYYY HH24:MI:SS')";

            default:
                return $fieldname;
        }
    }
    
    public function sql_quote_str($val) {
        switch($this->DBType){
            case 'oci8':
                $val=str_replace("'","''",$val);
                break;
            case 'monetdb':
                # ??? should I use monetdb_escape_string() ???
                $val=strtr($val, array('\n'=>'\\n', '\t'=>'\\t', '?'=>'\\?', '\\'=>'\\\\', '\''=>'\\\'', '"'=>'\\"', '\0'=>'\\0'));
                break;
            case 'ifx':
                $val = str_replace($this->sql_get_quote_char(), $this->sql_get_quote_char() . $this->sql_get_quote_char(), $val);
                break; 
            default:
                $val=addslashes($val);
        }

        return $this->sql_get_quote_char() . $val . $this->sql_get_quote_char();
    }

    public function sql_get_quote_char() {
        switch($this->DBType){
            case 'oci8':
            case 'pgsql':
            case 'monetdb':
                return "'";
            default:
                return '"';
        }
    }

    public function sql_get_identifier_quote_char() {
        switch($this->DBType){
            case 'ifx': // for ifx, only if the server environment variable DELIMIDENT is set
            case 'pgsql':
            case 'oci8':
            case 'monetdb':
                return '"';
            case 'mysql':
                return "`";
            default:
                return '';
        }
    }

    public function sql_geom2wkt($field_name, $alias=''){
        switch($this->DBType){
            case 'mysql':
                $res = "AsText($field_name)";
                break;

            case 'oci8':
                $res = "SDO_UTIL.TO_WKTGEOMETRY($field_name)";
                break;

            case 'monetdb':
                $res = $field_name;
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
        if($alias!='') $res.=' as '.$alias;
        return $res;
    }

    public function sql_geom_intersect($field_a, $field_b){
        switch($this->DBType){
            case 'mysql':
                $res = "Intersects($field_a, $field_b)";
                break;

            case 'monetdb':
                $res = "\"Intersect\"($field_a, $field_b)";
                break;

            case 'oci8':
                $res = "(SDO_RELATE($field_a, $field_b,'mask=ANYINTERACT')='TRUE')";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
        return $res;
    }

	public function sql_get_table_columns(&$dbHandler, $tbname){
        $mapAr=array();
        switch($this->DBType){
			case 'mysql': break;
			
			case 'pgsql':
				$mapAr = array(
					'column_name'=>'Field',
					'data_type'=>'Type',
					'is_nullable'=>'Null',
					'column_default'=>'Default',
				);
				break;
			
			case 'ifx':
				$mapAr = array(
					'col_name'=>'Field',
					'col_type'=>'Type',
					'not_null'=>'Null',
					'is_pk'=>'Key',
				);
				break;
				
			default:
				throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }

		$sql = $this->sql_show_columns($tbname);
		$retRows=array();
		$rows = $dbHandler->execQuery($sql, acs_idb :: _SQL_GETALL);

		foreach($rows as $row){

			$tmpRow=array();
			foreach($row as $field=>$value){

				if(isset($mapAr[$field])) $field = $mapAr[$field];
				$tmpRow[$field]	= $value;
			}
			$retRows[]=$tmpRow;
		}
		return $retRows;
	}

    public function sql_show_columns($tbname) {
        switch($this->DBType){
            case 'mysql':
                return "show columns from $tbname";
                break;
            case 'pgsql':
                return "SELECT column_name,data_type,is_nullable,column_default FROM information_schema.columns WHERE table_name='$tbname'";
                break;
            case 'ifx':
                return "
            SELECT
            Col.colno -1                           col_pos,
            Col.colname                            col_name,
            Col.coltype                            col_type,
            Col.collength                           Length,

            (CASE
                WHEN MOD(col.collength/16,16) =  0 THEN 'YEAR'
                WHEN MOD(col.collength/16,16) =  2 THEN 'MONTH'
                WHEN MOD(col.collength/16,16) =  4 THEN 'DAY'
                WHEN MOD(col.collength/16,16) =  6 THEN 'HOUR'
                WHEN MOD(col.collength/16,16) =  8 THEN 'MINUTE'
                WHEN MOD(col.collength/16,16) = 10 THEN 'SECONDS'
                WHEN MOD(col.collength/16,16) = 11 THEN 'FRACTION(1)'
                WHEN MOD(col.collength/16,16) = 12 THEN 'FRACTION(2)'
                WHEN MOD(col.collength/16,16) = 13 THEN 'FRACTION(3)'
                WHEN MOD(col.collength/16,16) = 14 THEN 'FRACTION(4)'
                WHEN MOD(col.collength/16,16) = 15 THEN 'FRACTION(5)'
            END )                           DateFrom,

            (CASE
                WHEN MOD(col.collength,16) =  0 THEN 'YEAR'
                WHEN MOD(col.collength,16) =  2 THEN 'MONTH'
                WHEN MOD(col.collength,16) =  4 THEN 'DAY'
                WHEN MOD(col.collength,16) =  6 THEN 'HOUR'
                WHEN MOD(col.collength,16) =  8 THEN 'MINUTE'
                WHEN MOD(col.collength,16) = 10 THEN 'SECONDS'
                WHEN MOD(col.collength,16) = 11 THEN 'FRACTION(1)'
                WHEN MOD(col.collength,16) = 12 THEN 'FRACTION(2)'
                WHEN MOD(col.collength,16) = 13 THEN 'FRACTION(3)'
                WHEN MOD(col.collength,16) = 14 THEN 'FRACTION(4)'
                WHEN MOD(col.collength,16) = 15 THEN 'FRACTION(5)'
            END )                           DateTo,


            (CASE WHEN
               PrimaryKeyCons.constrtype IS NULL THEN
                   'F'
               ELSE
                   'T'
            END )                                   is_pk ,

            (CASE WHEN
               (Col.coltype > 256
                   AND Col.coltype < 512) THEN
                   'T'
               ELSE
                   'F'
            END )                                  not_null ,

            (CASE WHEN
               (
                Col.coltype = 6
                OR Col.coltype = 262
                OR Col.coltype = 18
                OR Col.coltype = 274) THEN
                   'T'
               ELSE
                   'F'
            END )                                   is_serial ,

            ParentTable.tabname                     Foreign_Table_Name,
            ParentColumn.colno -1                   Foreign_Col_NO,
            ParentColumn.colname                    Foreign_Col_Name,
            ParentTable.tabid                       Foreign_Table_ID,

            Tp.name                                 Extended_Type_Name

        FROM
           syscolumns Col

            INNER JOIN systables Tab
               ON(
                   Col.tabid = Tab.tabid
                   AND Tab.tabname = '$tbname'
               )

            LEFT JOIN sysxtdtypes Tp
                ON Col.extended_id = Tp.extended_id

            LEFT JOIN
            (
                sysindexes PrimaryKeyIndex
                INNER JOIN sysconstraints PrimaryKeyCons
                    ON
                    (
                        PrimaryKeyCons.idxname = PrimaryKeyIndex.idxname
                        AND PrimaryKeyCons.constrtype = 'P'
                    )
            )
            ON
            (
               Col.tabid = PrimaryKeyIndex.tabid
               AND (
                           Col.colno = ABS( PrimaryKeyIndex.part1)
                       OR  Col.colno = ABS( PrimaryKeyIndex.part2 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part3 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part4 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part5 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part6 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part7 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part8 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part9 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part10 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part11 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part12 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part13 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part14 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part15 )
                       OR  Col.colno = ABS( PrimaryKeyIndex.part16 )
               )
            )




            LEFT JOIN
            (
                sysindexes ChildFieldIndex
                INNER JOIN sysconstraints ChildFieldCons
                   ON
                   (
                       ChildFieldCons.idxname = ChildFieldIndex.idxname
                       AND ChildFieldCons.constrtype = 'R'
                   )

                INNER JOIN sysreferences refer
                   ON refer.constrid = ChildFieldCons.constrid

                INNER JOIN systables ParentTable
                   ON ParentTable.tabid = refer.ptabid

                INNER JOIN sysconstraints ParentKeyCons
                   ON ParentKeyCons.constrid = refer.primary

                INNER JOIN sysindexes ParentIndex
                   ON ParentIndex.idxname = ParentKeyCons.idxname

                INNER JOIN syscolumns ParentColumn
                   ON ParentColumn.tabid = ParentTable.tabid
            )
            ON
            (
               Col.tabid = ChildFieldIndex.tabid
                AND (
                            Col.colno = ABS(ChildFieldIndex.part1 ) AND ParentColumn.colno = ABS(ParentIndex.part1 )
                        OR  Col.colno = ABS(ChildFieldIndex.part2 ) AND ParentColumn.colno = ABS(ParentIndex.part2 )
                        OR  Col.colno = ABS(ChildFieldIndex.part3 ) AND ParentColumn.colno = ABS(ParentIndex.part3 )
                        OR  Col.colno = ABS(ChildFieldIndex.part4 ) AND ParentColumn.colno = ABS(ParentIndex.part4 )
                        OR  Col.colno = ABS(ChildFieldIndex.part5 ) AND ParentColumn.colno = ABS(ParentIndex.part5 )
                        OR  Col.colno = ABS(ChildFieldIndex.part6 ) AND ParentColumn.colno = ABS(ParentIndex.part6 )
                        OR  Col.colno = ABS(ChildFieldIndex.part7 ) AND ParentColumn.colno = ABS(ParentIndex.part7 )
                        OR  Col.colno = ABS(ChildFieldIndex.part8 ) AND ParentColumn.colno = ABS(ParentIndex.part8 )
                        OR  Col.colno = ABS(ChildFieldIndex.part9 ) AND ParentColumn.colno = ABS(ParentIndex.part9 )
                        OR  Col.colno = ABS(ChildFieldIndex.part10) AND ParentColumn.colno = ABS(ParentIndex.part10)
                        OR  Col.colno = ABS(ChildFieldIndex.part11) AND ParentColumn.colno = ABS(ParentIndex.part11)
                        OR  Col.colno = ABS(ChildFieldIndex.part12) AND ParentColumn.colno = ABS(ParentIndex.part12)
                        OR  Col.colno = ABS(ChildFieldIndex.part13) AND ParentColumn.colno = ABS(ParentIndex.part13)
                        OR  Col.colno = ABS(ChildFieldIndex.part14) AND ParentColumn.colno = ABS(ParentIndex.part14)
                        OR  Col.colno = ABS(ChildFieldIndex.part15) AND ParentColumn.colno = ABS(ParentIndex.part15)
                        OR  Col.colno = ABS(ChildFieldIndex.part16) AND ParentColumn.colno = ABS(ParentIndex.part16)
               )
            )
        order by col_pos ";
                break;

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_upper($str)
    {
        switch ($this->DBType){
            case 'mysql':
            case 'mssql':
            case 'pgsql':
            case 'ifx':
            case 'oci8':
                return "upper($str)";

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_boolean($value)
    {
        if (is_string($value))
            $value = strtolower($value);
//throw new Exception(var_Export($value, true));

        if ($value === 1 || $value === true || $value === 'true' || $value === 't')
            $value = true;
        elseif ($value === 0 || $value === false || $value === 'false' || $value === 'f')
            $value = false;
        elseif ($value == 'null') return $value;
        else
            throw new acs_exBadParam(__METHOD__. " wrong boolean value ($value)");

        switch ($this->DBType)
        {
            case 'mysql':
                return ($value ? 'TRUE' : 'FALSE');
            case 'mssql':
                return ($value ? 1 : 0);
            case 'monetdb':
                return ($value ? 'true' : 'false');
            case 'pgsql':
            case 'ifx':
                return ($value ? "'t'" : "'f'");
            case 'oci8':
            	return 'BOOL(' . ($value ? "'t'" : "'f'") . ')';

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_get_wkt_geometry_collection($wkt, $srid=null, $geomType=2007, $elementType=1003) {
        switch($this->DBType){
            case 'mysql':
                $wkt=$this->sql_quote_str($wkt);
                return "GeometryCollectionFromText($wkt)";

            case 'monetdb':
                $wkt=$this->sql_quote_str($wkt);
                return "GeomCollectionFromText($wkt, 0)";

            case 'oci8':
                if(!isset($srid)) $srid='NULL';
                $coordCnt=1;
                if(!is_array($wkt)) $wkt=array($wkt);
                foreach($wkt as $polygonStr){
                    $elemInfoAr[]="$coordCnt,$elementType,1";
                    $ordinateArray[]=$polygonStr;
                    $coordCnt+=count(explode(',',$polygonStr));
                }
                $elemInfoStr = join(', ',$elemInfoAr);
                $ordinateArrayStr=join(', ',$ordinateArray);
                $res = "SDO_GEOMETRY( $geomType, $srid, NULL, SDO_ELEM_INFO_ARRAY($elemInfoStr), SDO_ORDINATE_ARRAY($ordinateArrayStr))";

                return $res;
            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_get_wkt_geometry_collection_items($wkt, $elementType=1003) {
        switch($this->DBType){
            case 'oci8':
                if(!isset($srid)) $srid='NULL';
                $coordCnt=1;
                if(!is_array($wkt)) $wkt=array($wkt);
                foreach($wkt as $polygonStr){
                    $elemInfoAr[]="$coordCnt,$elementType,1";
                    $ordinateArray[]=$polygonStr;
                    $coordCnt+=count(explode(',',$polygonStr));
                }
                $elemInfoStr = join(', ',$elemInfoAr);
                $ordinateArrayStr=join(', ',$ordinateArray);

                return array('ELEM_INFO'=>$elemInfoStr, 'ORDINATE'=>$ordinateArrayStr );

            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

    public function sql_get_wkt_geometry($wkt,$srid=null) {
        $wkt=$this->sql_quote_str($wkt);
        switch($this->DBType){
            case 'mysql':
                return "GeometryFromText($wkt)";
            case 'monetdb':
                return "GeomFromText($wkt, 0)";
            default:
                throw new acs_exNotCapable(__function__.' not implemented for '.$this->DBType);
        }
    }

}
?>
