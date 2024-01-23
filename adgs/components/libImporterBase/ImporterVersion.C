// PRQA S 1050 EOF
/*

	Copyright 2012-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: Importer $

*/


#include <auto_version_file>
#include <ImporterVersion.h>

using namespace acs ;

const char* ImporterVersion::_name = auto_version_file::name;
const char* ImporterVersion::_tag = auto_version_file::tag;

ImporterVersion::ImporterVersion() ACS_NOEXCEPT {}
ImporterVersion::~ImporterVersion() ACS_NOEXCEPT {}

const char* ImporterVersion::tag() throw() { return _tag ; }
const char* ImporterVersion::name() throw() { return _name ; }

