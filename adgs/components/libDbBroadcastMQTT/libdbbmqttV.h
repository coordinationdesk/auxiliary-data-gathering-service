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

   libDbBroadcastMQTT version file
*/ 

#ifndef _libdbbmqttV_H_
#define _libdbbmqttV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class libdbbmqttV { // PRQA S 2109
public:
	libdbbmqttV() ACS_NOEXCEPT ;
	~libdbbmqttV() ACS_NOEXCEPT ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	libdbbmqttV(const libdbbmqttV &) ;
	const libdbbmqttV &operator=(const libdbbmqttV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;
} // Close namespace

#endif // _libdbbmqttV_H_
