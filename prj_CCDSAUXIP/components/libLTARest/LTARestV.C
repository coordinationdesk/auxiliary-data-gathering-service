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

	$Prod: LTA Rest Interface $


*/

#include <auto_version_file>
#include <LTARestV.h>

using namespace acs ;

const char* LTARestV::_name = auto_version_file::name;
const char* LTARestV::_tag = auto_version_file::tag;


LTARestV::LTARestV() ACS_NOEXCEPT {}
LTARestV::~LTARestV() ACS_NOEXCEPT {}

const char* LTARestV::tag() { return _tag ; }
const char* LTARestV::name() { return _name ; }
