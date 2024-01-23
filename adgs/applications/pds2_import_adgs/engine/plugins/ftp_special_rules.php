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

require_once 'ACSPhpLib/acs_mdb2.php';

class ftp_special_rules {

// Builds a rules array from a single rule if a specific behaviour is needed.	
	public static function getRule_ar($rule,$filetype,&$dbHandler){
		if ($rule != null) return array($rule);
		$rule_ar=self::getChildrensRules($filetype,$dbHandler);
		return $rule_ar;
	}

	public static function getChildrensRules($filetype,&$dbHandler) {
		
		$sql = "SELECT nameregularexpression FROM t_filetypes WHERE id_filetype IN
				(SELECT cfiletype FROM t_filetypescomps ftc
					INNER JOIN t_filetypes ft ON ftc.pfiletype = ft.id_filetype WHERE ft.filetype =
					".$dbHandler->getSQLObj()->sql_quote_str($filetype).")";
		
		$rulesRows = $dbHandler->execQuery($sql, acs_idb::_SQL_GETALL);
		if (empty($rulesRows))
			throw new Exception ("$filetype has a nameregularexpression null, but no children!");
			
			$rule_ar=array();
			foreach($rulesRows as $ruleRow){
				$rule_ar[]=$ruleRow['nameregularexpression'];
			}
			return $rule_ar;
	}
	
}

?>