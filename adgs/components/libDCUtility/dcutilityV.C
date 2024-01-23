// PRQA S 1050 EOF
/*

    Copyright 1995-2018, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    libDCUtility  version file

*/ 

#include <auto_version_file>
#include "dcutilityV.h"

using namespace acs ;

const char* dcutilityV::_name = auto_version_file::name;
const char* dcutilityV::_tag = auto_version_file::tag;


dcutilityV::dcutilityV() throw() {}
dcutilityV::~dcutilityV() throw() {}


const char* dcutilityV::tag() throw() { return _tag ; }
const char* dcutilityV::name() throw() { return _name ; }

