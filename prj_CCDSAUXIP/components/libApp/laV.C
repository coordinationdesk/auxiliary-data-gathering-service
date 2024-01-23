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

    libApp  version file

*/ 

#include <auto_version_file>
#include <laV.h>

using namespace acs ;

const char* laV::_name = auto_version_file::name;
const char* laV::_tag = auto_version_file::tag;


laV::laV() ACS_NOEXCEPT {}
laV::~laV() ACS_NOEXCEPT {}

const char* laV::tag() ACS_NOEXCEPT { return _tag ; }
const char* laV::name() ACS_NOEXCEPT { return _name ; }

