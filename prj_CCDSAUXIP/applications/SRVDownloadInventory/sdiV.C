// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: SRVDownloadInventory $
	

*/

#include <auto_version_file>
#include <sdiV.h>

using namespace acs ;

const char* sdiV::_name = auto_version_file::name;
const char* sdiV::_tag = auto_version_file::tag;


sdiV::sdiV() throw() {}
sdiV::~sdiV() throw() {}

const char* sdiV::tag() throw() { return _tag ; }
const char* sdiV::name() throw() { return _name ; }
