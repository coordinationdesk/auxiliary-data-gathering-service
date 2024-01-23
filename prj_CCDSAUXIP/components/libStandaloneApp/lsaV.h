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

	$Prod: A.C.S. Standalone Application Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2012/02/14 14:46:52  marpas
	refactoring
	
	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
	imported source code
	
	

*/
 
#ifndef _lsaV_H_
#define _lsaV_H_

namespace acs {

// support class for versioning

class lsaV { // PRQA S 2109
public:
	lsaV() throw() ;
	~lsaV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    lsaV(const lsaV &) ; // not implemented 
    lsaV& operator= (const lsaV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lsaV_H_



