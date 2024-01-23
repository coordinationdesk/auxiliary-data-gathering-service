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

class pds2_imp_pl_archiver extends pds2_imp_pl_base {

    public function execute($userid){
    	$this->_logHandler->debug(__METHOD__ . ": " . $this->getName());
    	
    	//print_r($this->_opVars['run_rule_config']);
		$args = '';
		foreach ($this->pool_ref->output_files as $filename) {
			if (isset($this->_opVars['run_rule_config']['select_files_regex']) &&
				preg_match($this->_opVars['run_rule_config']['select_files_regex'], $filename))
				$args .= ' ' . escapeshellarg(basename($filename));
		}
		
		if (!isset($this->_opVars['run_rule_config']['archive_name_regex']))
			throw new Exception(__METHOD__ . ": wrong run_rule configuration: define archive_name_regex");

		$format = isset($this->_opVars['run_rule_config']['format']) ? $this->_opVars['run_rule_config']['format'] : 'tar';
		
		$matches = array();
		if (!preg_match($this->_opVars['run_rule_config']['archive_name_regex'], $this->pool_ref->output_files[0], $matches))
			throw new Exception(__METHOD__ . ": file {$this->pool_ref->output_files[0]} doesn't matches regex {$this->_opVars['run_rule_config']['archive_name_regex']}");
		
		$archive = $matches[1] . '.' . $format;

		switch ($format) {
			case 'tgz':
			case 'tar':
				$opts = 'cf';
				if ($format == 'tgz') $opts .= 'z';
				
				$cmd = "cd '{$this->pool_ref->pds_temporary_dir}' && tar $opts '$archive' $args";
				break;
			default:
				throw new Exception(__METHOD__ . ": Unrecognized archive format: $format");
		}
		
		$this->_logHandler->debug("Executing command: '$cmd'");

        $output = null; $retval = null;
        exec($cmd . " 2>&1", $output, $retval);

        // check exit code
        if ($retval != 0)
            throw new Exception("Error executing '$cmd', command output was " . join("\n", $output));
        
        $this->pool_ref->output_files = array($archive);
    }

}

?>