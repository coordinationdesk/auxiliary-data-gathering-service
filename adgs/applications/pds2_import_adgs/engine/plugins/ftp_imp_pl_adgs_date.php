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
 * handles date/time placeholder like %Y, %d in remote paths
 * 
 * @author andrea
 *
 */
class ftp_imp_pl_adgs_date extends ftp_imp_pl_adgs {
	
	static $tmRegex = '/:(?P<function>\w+)\((?P<context>\w+),(?P<arg1>[+-]?\d+)(,(?P<arg2>[+-]?\d+))?\)$/';
	
	public function initialize($userid) {
		parent::initialize($userid);
		
		$this->current_remote_path = self::substituteDateComponents($this->current_remote_path);
		
		$newFilename = self::substituteDateComponents($this->_opVars['run_rule']);
		
		if (is_array($newFilename)) {
			throw new Exception(":range time modifier is available only for paths, not filenames");
		}
		
		$this->_opVars['run_rule'] = $newFilename;
	}
	
	static protected function computeTimeModifier($str) {
		$matches = array();
		if (preg_match(self::$tmRegex, $str, $matches)) {
			if ($matches['function'] == 'offset') {
				if ($matches['context'] != 'now') {
					throw new Exception("In date path function {$matches['function']} only context 'now' is supported");
				}
				
				// format standard date components in the input string
				$offset = doubleval($matches['arg1']);
				return time() + $offset * 86400;
			} else if ($matches['function'] == 'range') {
				if (!array_key_exists('arg2', $matches)) {
					throw new Exception("Time modifier {$matches['function']} requires 2 arguments");
				}
				$start = doubleval($matches['arg1']);
				$stop = doubleval($matches['arg2']);
				
				$ret = array();
				for ($a = $start; $a <= $stop; $a++) {
					$ret[] = time() + $a * 86400;
				}
				return $ret;
			} else {
				throw new Exception("Unidentified time modifier: {$matches['function']}");
			}
		}
		// in all other cases, return the current timestamp
		return time();
	}
	
	static protected function removeTimeModifier($str) {
		return preg_replace(self::$tmRegex, '', $str);
	}
	
	static protected function substituteDateComponents($str) {
		
		$epoch = self::computeTimeModifier($str);
		$startStr = self::removeTimeModifier($str);

		// replace custom advanced date components
		$doyToday = gmstrftime("%j");
		$doyYesterday = sprintf("%03d", intval($doyToday) - 1);
		$doyBeforeYesterday = sprintf("%03d", intval($doyToday) - 2);
		$doyTomorrow = sprintf("%03d", intval($doyToday) + 1);
		
		/*
		 * Absolute time keywords substitutions
		 */
		$startStr = str_replace(
			array('%{doyToday}', '%{doyYesterday}', '%{doyBeforeYesterday}', '%{doyTomorrow}'),
			array($doyToday, $doyYesterday, $doyBeforeYesterday, $doyTomorrow),
			$startStr);
			
		/*
		 * Dynamic (range/offset) time placeholders substitutions
		 */
		if (is_array($epoch)) {
			$ret = array();
			foreach ($epoch as $tm) {
				$ret[] = self::formatTimeString($startStr, $tm);
			}
			/*
			 *  array_unique avoids possible repetitions when the different
			 *  epochs produce the same output string
			 */
			return array_unique($ret);
		} else {
			return self::formatTimeString($startStr, $epoch);
		}
	}
	
	static protected function formatTimeString($formatString, $epoch) {
		return gmstrftime(
			str_replace(
				array('%{gpsWeek}'),
				array(self::fromEpochToGPSWeek($epoch)),
				$formatString),
			$epoch);
	}
	
	static protected function fromEpochToGPSWeek($epoch) {
		$gpsStartEpoch = strtotime('1980-01-06 00:00:00');
		$gpsTime = ($epoch - $gpsStartEpoch) / (86400 * 7);
		return intval($gpsTime);
	}

}

?>