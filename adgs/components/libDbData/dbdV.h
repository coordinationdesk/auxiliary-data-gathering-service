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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.2  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.1  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

 
#ifndef _dbdV_H_
#define _dbdV_H_

namespace acs { 

// support class for versioning

class dbdV { // PRQA S 2109
public:
	dbdV() throw() ;
	~dbdV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	dbdV(const dbdV &) ; // not implemented 
	dbdV &operator=(const dbdV &) ; // not implemented 
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _dbdV_H_

