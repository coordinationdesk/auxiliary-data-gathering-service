// PRQA S 1050 EOF
/*

    Copyright 2020, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

*/ 

#include <auto_version_file>
#include <ObjSmV.h>

using namespace acs ;

const char* ObjSmV::_name = auto_version_file::name;
const char* ObjSmV::_tag = auto_version_file::tag;


ObjSmV::ObjSmV() throw() {}
ObjSmV::~ObjSmV() throw() {}

const char* ObjSmV::tag() throw() { return _tag ; }
const char* ObjSmV::name() throw() { return _name ; }
