// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Notification Dispatcher $


*/


#ifndef _SRVDispatcherV_H_
#define _SRVDispatcherV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class SRVDispatcherV { // PRQA S 2109
public:
	SRVDispatcherV() ACS_NOEXCEPT ;
	~SRVDispatcherV() ACS_NOEXCEPT ;
	static const char* tag() ;
	static const char* name() ;
private:
	SRVDispatcherV(const SRVDispatcherV &) ;
	SRVDispatcherV &operator =(const SRVDispatcherV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _SRVDispatcherV_H_
