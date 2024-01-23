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

    libSerialization  version file

*/ 

#include <auto_version_file>
#include <SerializableV.h>

using namespace acs;

const char* SerializableV::_name = auto_version_file::name;
const char* SerializableV::_tag = auto_version_file::tag;


SerializableV::SerializableV() ACS_NOEXCEPT {}
SerializableV::~SerializableV() ACS_NOEXCEPT {}

const char* SerializableV::tag() ACS_NOEXCEPT { return _tag; }
const char* SerializableV::name() ACS_NOEXCEPT { return _name; }
