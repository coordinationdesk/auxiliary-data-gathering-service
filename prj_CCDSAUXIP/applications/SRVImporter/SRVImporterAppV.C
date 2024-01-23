/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libSRVImporterApp$
        
*/

#include <auto_version_file>
#include <SRVImporterAppV.h>

_ACS_BEGIN_NAMESPACE(acs)

const char* SRVImporterAppV::_name = auto_version_file::name;
const char* SRVImporterAppV::_tag  = auto_version_file::tag ;

SRVImporterAppV::SRVImporterAppV()  ACS_NOEXCEPT {}
SRVImporterAppV::~SRVImporterAppV() ACS_NOEXCEPT {}

const char* SRVImporterAppV::tag()  ACS_NOEXCEPT { return _tag ; }
const char* SRVImporterAppV::name() ACS_NOEXCEPT { return _name ; }

_ACS_END_NAMESPACE

