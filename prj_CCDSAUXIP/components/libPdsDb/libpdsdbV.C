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

	$Prod: LTA File Inventory Library $

*/

#include <auto_version_file>
#include <libpdsdbV.h>

using namespace acs ;

const char* libpdsdbV::_name = auto_version_file::name;
const char* libpdsdbV::_tag = auto_version_file::tag;


libpdsdbV::libpdsdbV() throw() {}
libpdsdbV::~libpdsdbV() throw() {}

const char* libpdsdbV::tag() throw() { return _tag ; }
const char* libpdsdbV::name() throw() { return _name ; }

