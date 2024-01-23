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

    libFtpCurl  version file

*/ 

#include <auto_version_file>
#include <lfcV.h>

using namespace acs ;

const char* lfcV::_name = auto_version_file::name;
const char* lfcV::_tag = auto_version_file::tag;


lfcV::lfcV() {}
lfcV::~lfcV() {}

const char* lfcV::tag() { return _tag ; }
const char* lfcV::name() { return _name ; }
