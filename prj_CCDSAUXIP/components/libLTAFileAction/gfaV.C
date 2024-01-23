// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA File Action library $	


*/ 

#include <auto_version_file>
#include <gfaV.h>

using namespace acs ;

const char* gfaV::_name = auto_version_file::name;
const char* gfaV::_tag = auto_version_file::tag;


gfaV::gfaV() noexcept {}
gfaV::~gfaV() noexcept {}

const char* gfaV::tag() noexcept { return _tag ; }
const char* gfaV::name() noexcept { return _name ; }

