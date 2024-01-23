// PRQA S 1050 EOF
/*

	Copyright 1995-2009, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.1  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.0  2006/02/28 09:09:04  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/03/17 12:42:56  marpas
	headers fixed
	
	Revision 1.3  2003/04/30 14:03:27  marpas
	using namespace std removed
	
	Revision 1.2  2003/01/14 15:41:48  marpas
	versioning ...
	
	Revision 1.1.1.1  2003/01/14 15:34:55  marpas
	libDbConfiguration Import
		

*/

#ifndef _dbcf_H_
#define _dbcf_H_

#include <exException.h>

namespace acs {

// support class for versioning

class dbcf { // PRQA S 2109
public:
	dbcf() throw() ;
	~dbcf() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not implemented to prevent their use
	dbcf(const dbcf &) ;
	dbcf &operator=(const dbcf &) ;
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _dbcf_H_



