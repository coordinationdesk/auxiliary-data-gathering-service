// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. MQTT Statistics Library
*/

#ifndef _mqttstatV_H_
#define _mqttstatV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class mqttstatV { // PRQA S 2109
public:
	mqttstatV() ACS_NOEXCEPT ;
	~mqttstatV() ACS_NOEXCEPT ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	mqttstatV(const mqttstatV &) ;
	const mqttstatV &operator=(const mqttstatV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;
} // Close namespace

#endif // _mqttstatV_H_

