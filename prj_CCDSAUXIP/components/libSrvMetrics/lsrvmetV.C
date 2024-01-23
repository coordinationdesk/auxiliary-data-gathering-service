// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/

#include <auto_version_file>
#include <lsrvmetV.h>

using namespace acs ;

const char* lsrvmetV::_name = auto_version_file::name;
const char* lsrvmetV::_tag = auto_version_file::tag;


lsrvmetV::lsrvmetV() throw() {}
lsrvmetV::~lsrvmetV() throw() {}

const char* lsrvmetV::tag() throw() { return _tag ; }
const char* lsrvmetV::name() throw() { return _name ; }

