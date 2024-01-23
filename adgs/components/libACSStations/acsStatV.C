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

    libACSStations  version file

*/ 

#include <auto_version_file>
#include <acsStatV.h>


_ACS_BEGIN_NAMESPACE(acs)

const char* acsStatV::_name = auto_version_file::name;

const char* acsStatV::_tag = auto_version_file::tag;


acsStatV::acsStatV() {}
acsStatV::~acsStatV() {}

const char* acsStatV::tag() { return _tag ; }
const char* acsStatV::name() { return _name ; }
_ACS_END_NAMESPACE
