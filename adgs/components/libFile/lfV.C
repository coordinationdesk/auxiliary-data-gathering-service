// PRQA S 1050 EOF
/*

        Copyright 1995-2099, Advanced Computer Systems , Inc.
        Via Della Bufalotta, 378 - 00139 Roma - Italy
        http://www.acsys.it

        All Rights Reserved.

        This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
        the contents of this file may not be disclosed to third parties, copied or
        duplicated in any form, in whole or in part, without the prior written
        permission of Advanced Computer Systems, Inc.

        Prod: A.C.S. Version File

*/ 

#include <auto_version_file>
#include <lfV.h>

using namespace acs ;

const char* lfV::_name = auto_version_file::name;
const char* lfV::_tag = auto_version_file::tag;


lfV::lfV() throw() {}
lfV::~lfV() throw() {}


const char* lfV::tag() throw() { return _tag ; }
const char* lfV::name() throw() { return _name ; }
