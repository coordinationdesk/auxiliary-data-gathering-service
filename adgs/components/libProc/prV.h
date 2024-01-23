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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.1  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#ifndef _prV_H_
#define _prV_H_

namespace acs {

// support class for versioning

class prV { // PRQA S 2109
public:
	prV() throw() ;
	~prV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not defined to prevent their use
	prV(const prV &) ;
	prV & operator=(const prV &) ;
	
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _prV_H_



