/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel-3 ADF reformatter library $

	$Id$

	$Author$

	$Log$

*/

#ifndef _s3afV
#define _s3afV

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class s3afV { // PRQA S 2109
public:
	s3afV() ACS_NOEXCEPT ;
	~s3afV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
	//declared but not defined to prevent their use
	s3afV(const s3afV &) ;
	s3afV & operator=(const s3afV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _s3afV
