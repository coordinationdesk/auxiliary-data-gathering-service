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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/03/14 15:23:13  marpas
	interface rationalization
	coding best practices applied
	
	Revision 2.3  2012/12/01 18:47:10  marpas
	qa rules
	
	Revision 2.2  2012/02/13 11:07:01  marpas
	refactoring in progress
	
	Revision 2.1  2009/08/03 18:24:22  marpas
	less connection and faster design
	
	Revision 2.0  2006/02/28 08:37:08  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 17:44:36  marfav
	Header fixed
	
	Revision 1.2  2003/04/30 10:21:03  marpas
	using namespace std was removed from includes
	
	Revision 1.1.1.1  2002/10/10 12:06:14  marpas
	libConfiguration import
		

*/

 
#ifndef _cfv_H_
#define _cfv_H_

#include <exException.h>

namespace acs {

// support class for versioning

class cfv { // PRQA S 2109
public:
	cfv() throw() ;
	~cfv() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not defined to prevent their use
	cfv(const cfv &) ;
	cfv &operator=(const cfv &) ;

private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _cfv_H_



