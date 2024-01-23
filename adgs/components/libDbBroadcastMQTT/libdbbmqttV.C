// PRQA S 1050 EOF
/*
   Copyright 2019-, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   libDbBroadcastMQTT version file header
*/ 
#include <libdbbmqttV.h>
#include <auto_version_file> // this is the inclusion of the auto generated file with injected info

namespace acs {

const char* libdbbmqttV::_name = auto_version_file::name;
const char* libdbbmqttV::_tag = auto_version_file::tag;

libdbbmqttV::libdbbmqttV() ACS_NOEXCEPT {}

libdbbmqttV::~libdbbmqttV() ACS_NOEXCEPT {}

const char*  libdbbmqttV::tag() { return _tag ; }
const char*  libdbbmqttV::name() { return _name ; }

} // namespace acs
