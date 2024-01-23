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

require_once 'pds2_imp_pl_base.php';
require_once 'ACSPhpLib/acs_tools.php';

class pds2_imp_pl_wscall extends pds2_imp_pl_base {
	
    public function execute($userid){
    	$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());

		$curl = new acs_curl();
		
		if (!isset($this->_opVars['run_rule_config']['url']))
			throw new Exception(__METHOD__ . ": missing run_rule configuration for 'url' item");
		
		$method = isset($this->_opVars['run_rule_config']['method']) ? $this->_opVars['run_rule_config']['method'] : 'GET';
		$check_success_code = isset($this->_opVars['run_rule_config']['check_success_code']) ? $this->_opVars['run_rule_config']['check_success_code'] : 200;
		
		switch ($method) {
			case 'GET':
				$curl->get($this->_opVars['run_rule_config']['url']);
				break;
			case 'POST':
				$curl->post($this->_opVars['run_rule_config']['url']);
				break;
			default:
				throw new Exception(__METHOD__ . ": unknown http method: $method");
		}
		
		if ($curl->last_http_response['http_code'] != $check_success_code)
			throw new Exception(__METHOD__ . ": call to {$this->_opVars['run_rule_config']['url']} returned wrong http code: {$curl->last_http_response['http_code']}, expected: $check_success_code");
    }
	
}

?>