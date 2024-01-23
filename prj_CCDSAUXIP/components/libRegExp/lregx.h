// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Regular Expression Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/06/27 11:27:30  marpas
	qac++ instrumentation
	
	Revision 2.2  2012/02/08 16:26:16  marpas
	refactoring
	
	Revision 2.1  2009/03/11 19:10:43  marpas
	rule violations fixed
	
	Revision 2.0  2006/02/28 08:25:52  marpas
	Exception 2.1 I/ adopted
	
	Revision 1.2  2005/05/19 17:58:59  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/11/21 15:00:23  paoscu
	Import libRegExp
	
	

*/
 
#ifndef _lregx_H_
#define _lregx_H_
#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class lregx { // PRQA S 2109
public:
	static const char*  tag() noexcept ;
	static const char*  name() noexcept ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lregx_H_



