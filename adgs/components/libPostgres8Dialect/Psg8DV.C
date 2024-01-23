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

    libPostgres8Dialect  version file

*/ 

#include <auto_version_file>
#include <Psg8DV.h>

using namespace acs ;

const char* Psg8DV::_name = auto_version_file::name;
const char* Psg8DV::_tag = auto_version_file::tag;

Psg8DV::Psg8DV() throw() {}
Psg8DV::~Psg8DV() throw() {}

const char* Psg8DV::tag() throw() { return _tag ; }
const char* Psg8DV::name() throw() { return _name ; }

