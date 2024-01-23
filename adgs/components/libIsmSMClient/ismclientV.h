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

	$Prod: A.C.S. ISM SM client library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/20 13:37:44  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa rules
	
	Revision 1.2  2012/02/13 17:09:49  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/06/16 15:14:58  marpas
	Importing libIsmSMClient
	

*/

#ifndef _ismclientV_H_
#define _ismclientV_H_

namespace acs {

// support class for versioning

class ismclientV { // PRQA S 2109
public:
	ismclientV() throw() ;
	~ismclientV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    ismclientV(const ismclientV &) ; // not implemented
    ismclientV& operator= (const ismclientV &) ; // not implemented
    
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _ismclientV_H_

