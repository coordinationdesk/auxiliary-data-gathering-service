// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDbSpace Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2012/02/13 16:50:12  marpas
	refactoring in progress
	
	Revision 1.2  2005/06/08 16:20:05  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/10/08 12:36:28  marpas
	libDbSpace import
	
		

*/

#ifndef _dbspV_H_
#define _dbspV_H_

namespace acs {

// support class for versioning

class dbspV { // PRQA S 2109
public:
	dbspV() throw()  ;
	~dbspV() throw()  ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	dbspV(const dbspV&) ; // not implemented	
    dbspV& operator=(const dbspV&) ; // not implemented

private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dbspV_H_




