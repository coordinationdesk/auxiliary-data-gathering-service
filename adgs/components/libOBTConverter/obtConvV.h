/*

	Copyright 2002, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$


*/

#ifndef _obtConvV_H_
#define _obtConvV_H_

#include <acs_c++config.hpp>
_ACS_BEGIN_NAMESPACE(acs)

// support class for versioning

class obtConvV {
public:
	obtConvV() ;
	~obtConvV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _obtConvV_H_

