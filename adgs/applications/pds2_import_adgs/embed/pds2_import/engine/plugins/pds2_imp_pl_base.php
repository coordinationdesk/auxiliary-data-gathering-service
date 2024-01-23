<?php
// vim: set et ts=4 sw=4 fdm=marker:
// +---------------------------------------------------------------------------+
// | PHP versions 4 and 5                                                      |
// +---------------------------------------------------------------------------+
// | Copyright (c) 1995-2006 - Advanced Computer Systems S.p.A.                |
// | Copyright 1995-2007, Advanced Computer Systems ,Inc.                      |
// | Via Della Bufalotta, 378 - 00139 Roma - Italy                             |
// | http://www.acsys.it                                                       |
// | All rights reserved.                                                      |
// +---------------------------------------------------------------------------+
// | This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems; |
// | the contents of this file may not be disclosed to third parties, copied or|
// | duplicated in any form, in whole or in part, without the prior written    |
// | permission of Advanced Computer Systems, Inc.                             |
// +---------------------------------------------------------------------------+
//
// $Prod:  $
// $Author$
// $Id$
// $Revision$
//
require_once 'ACSPhpLib/acs_wfoperation.php';

class pds2_imp_pl_base extends acs_wf_operation {

	protected	$simulation = false,
				$pool_ref = null,
				$name = '';
	
	public function setName($name) {
		$this->name = $name;
	}
	
	public function getName() {
		return $this->name;
	}
	
	public function setTransactionPool(& $pool) {
		$this->pool_ref = $pool;
	}

	public function setSimulation($flag) {
		$this->simulation = $flag;
	}

	// Overridables
	
	/**
	 * @SuppressWarnings(PHPMD.EvalExpression)
	 */
	public function initialize($userid) {
		$this->pds_temporary_dir  = $this->pool_ref->pds_temporary_dir;
        $this->pds_final_dir      = $this->pool_ref->pds_final_dir;
        $this->pds_bad_files_dir  = $this->pool_ref->pds_bad_files_dir;
        
        // check rule
		$this->ruleName = $this->_opVars['run_rule_name'];

		if ($this->_opVars['repo_run_rule_operation_type'] == _OPTYPE_DISABLED ||
			$this->_opVars['run_rule_operation_type'] == _OPTYPE_DISABLED) {
			$this->_logHandler->warning("Rule {$this->name} is turned off !");
			exit;
		}

		
		$this->simulation |= $this->_opVars['repo_run_rule_operation_type'] == _OPTYPE_SIMULATED ||
							$this->_opVars['run_rule_operation_type'] == _OPTYPE_SIMULATED;
		if ($this->simulation)
			$this->_logHandler->warning("Rule {$this->name} will be simulated !");
		
		// get basic configuration from ini
		$config = array();
		$config_key = get_class($this);
//		$this->_logHandler->debug("Checking for configutation from ini section: $config_key");
		if (array_key_exists($config_key, $this->_iniVars))
			$config = $this->_iniVars[$config_key];
		
		// parse run_rule external
		if ($this->_opVars['run_rule_type_code'] == _RUNRULE_TYPE_CODE_CONFIG) {
			$str = $this->_opVars['run_rule'];
			$dbconfig = array();
			
			if (substr($str, 0, 2) == 'a:')
				$dbconfig = unserialize($str);
			elseif (!eval('$dbconfig = array(' . $str . '); return true;'))
				throw new Exception(__METHOD__ . ": run_rule configuration error");
			$config = array_merge($config, $dbconfig);
		}
		$this->_opVars['run_rule_config'] = $config;
	}
	
	public function precondition($userid) {
		return true;
	}

    public function execute($userid){
    }
    
    public function cleanup($userid) {
    }
    
    public function cleanupTemp($userid) {
    }
	
}
?>
