// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Attribute Extractor $


*/
#include <auto_version_file>
#include <SRVExtractorV.h>

using namespace acs ;

const char* SRVExtractorV::_name = auto_version_file::name;
const char* SRVExtractorV::_tag = auto_version_file::tag;


SRVExtractorV::SRVExtractorV() ACS_NOEXCEPT {}
SRVExtractorV::~SRVExtractorV() ACS_NOEXCEPT {}

const char* SRVExtractorV::tag() { return _tag ; }
const char* SRVExtractorV::name() { return _name ; }
