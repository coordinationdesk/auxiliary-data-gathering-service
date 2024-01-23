// PRQA S 1050 EOF
/*

    Copyright 1995-2021, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    libAwsS3Wrapper  version file

*/ 

#include <auto_version_file>
#include <s3V.h>

using namespace acs ;

const char* s3V::_name = auto_version_file::name;
const char* s3V::_tag = auto_version_file::tag;


s3V::s3V() throw() {}
s3V::~s3V() throw() {}


const char* s3V::tag() throw() { return _tag ; }
const char* s3V::name() throw() { return _name ; }

