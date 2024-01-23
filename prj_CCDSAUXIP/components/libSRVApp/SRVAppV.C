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

	$Prod: SRV App $


*/


#include <auto_version_file>
#include <SRVAppV.h>

using namespace acs;

const char* SRVAppV::_name = auto_version_file::name;
const char* SRVAppV::_tag = auto_version_file::tag;


SRVAppV::SRVAppV() ACS_NOEXCEPT {}
SRVAppV::~SRVAppV() ACS_NOEXCEPT {}

const char* SRVAppV::tag() ACS_NOEXCEPT { return _tag; }
const char* SRVAppV::name() ACS_NOEXCEPT { return _name; }

