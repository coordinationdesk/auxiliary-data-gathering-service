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

    libOrderCreator  version file

*/ 

#include <auto_version_file>
#include <ogenV.h>

using namespace acs ;

const char* ogenV::_name = auto_version_file::name;
const char* ogenV::_tag = auto_version_file::tag;


ogenV::ogenV() throw() {}
ogenV::~ogenV() throw() {}

const char* ogenV::tag() throw() { return _tag ; }
const char* ogenV::name() throw() { return _name ; }
