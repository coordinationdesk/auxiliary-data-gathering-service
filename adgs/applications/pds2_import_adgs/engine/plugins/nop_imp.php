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

require_once dirname(__FILE__) .'/../../../pds2_import/engine/plugins/pds2_imp_pl_base.php';
class nop_imp extends pds2_imp_pl_base {

	public function execute($userid){
		$this->_logHandler->notice("nop_imp running...Do nothing.");
	}	
}

?>