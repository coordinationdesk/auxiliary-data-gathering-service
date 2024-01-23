// PRQA S 1050 EOF
/*
   Copyright 1995-2019, Exprivia
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   libMQTT  version file
*/ 
#include <libmqttV.h>
#include <auto_version_file> // this is the inclusion of the auto generated file with injected info

namespace acs {

const char* libmqttV::_name = auto_version_file::name;
const char* libmqttV::_tag = auto_version_file::tag;

libmqttV::libmqttV() ACS_NOEXCEPT {}

libmqttV::~libmqttV() ACS_NOEXCEPT {}

const char*  libmqttV::tag() { return _tag ; }
const char*  libmqttV::name() { return _name ; }

} // namespace acs
