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

    libTlDefaultPCC  version file

*/ 

#include <auto_version_file>
#include <tldpccfV.h>

using namespace acs ;

const char* tldpccfV::_name = auto_version_file::name;
const char* tldpccfV::_tag = auto_version_file::tag;



tldpccfV::tldpccfV() throw() {}
tldpccfV::~tldpccfV() throw() {}
const char* tldpccfV::tag() throw() { return _tag ; }
const char* tldpccfV::name() throw() { return _name ; }
