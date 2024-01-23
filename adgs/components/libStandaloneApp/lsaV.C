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

    libStandaloneApp  version file

*/ 

#include <auto_version_file>
#include <lsaV.h>

using namespace acs ;

const char* lsaV::_name = auto_version_file::name;
const char* lsaV::_tag = auto_version_file::tag;


lsaV::lsaV() throw() {}
lsaV::~lsaV() throw() {}

const char* lsaV::tag() throw() { return _tag ; }
const char* lsaV::name() throw() { return _name ; }

