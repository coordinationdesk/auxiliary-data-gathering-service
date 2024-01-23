/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV App $


*/

#ifndef _SRVAppV_h_
#define _SRVAppV_h_

#include <acs_c++config.hpp>

namespace acs {

// support class for versioning

class SRVAppV { // PRQA S 2109
public:
	SRVAppV() ACS_NOEXCEPT ;
	~SRVAppV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
	SRVAppV(const SRVAppV&) ; // not implemented
	SRVAppV & operator=(const SRVAppV&) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _SRVAppV_h_
