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

    libIsmSMClient  version file

*/ 

#include <auto_version_file>
#include <ismclientV.h>

using namespace acs ;

const char* ismclientV::_name = auto_version_file::name;
const char* ismclientV::_tag = auto_version_file::tag;


ismclientV::ismclientV() throw() {}
ismclientV::~ismclientV() throw() {}

const char* ismclientV::tag() throw() { return _tag ; }
const char* ismclientV::name() throw() { return _name ; }

