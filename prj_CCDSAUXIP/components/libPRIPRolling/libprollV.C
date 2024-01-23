// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: PRIP Rolling Library $


*/

#include <auto_version_file>
#include <libprollV.h>

using namespace acs ;

const char* libprollV::_name = auto_version_file::name;
const char* libprollV::_tag = auto_version_file::tag;


libprollV::libprollV() throw() {}
libprollV::~libprollV() throw() {}

const char* libprollV::tag() throw() { return _tag ; }
const char* libprollV::name() throw() { return _name ; }

