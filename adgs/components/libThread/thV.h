// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Thread Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/02/22 10:28:55  marpas
	coding best practices applied
	
	Revision 2.4  2012/09/27 16:41:56  marpas
	quality rules implementation
	
	Revision 2.3  2012/02/09 14:19:20  marpas
	refactoring
	
	Revision 2.2  2009/06/26 10:50:41  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2009/03/12 09:56:53  marpas
	rules violations fixed and or justified
	
	Revision 2.0  2006/02/28 08:27:50  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/01 12:51:50  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/10/21 18:07:37  paoscu
	Import libThread
	

*/


#ifndef _thV_H_
#define _thV_H_

#include <acs_c++config.hpp>

namespace acs {

// support class for versioning

class thV { // PRQA S 2109
public:
	thV() ACS_NOEXCEPT ;
	~thV() ACS_NOEXCEPT ;
	static const char* tag() ACS_NOEXCEPT ;
	static const char* name() ACS_NOEXCEPT ;
private:
	// declared but not defined to prevent their use
	thV(const thV &) = delete ;
	thV &operator =(const thV &) = delete ;
	thV(thV &&) = delete ; // PRQA S 2038 2 
	thV &operator =( thV &&) = delete ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _thV_H_

