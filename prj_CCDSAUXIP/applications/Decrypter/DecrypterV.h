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
	

*/


#ifndef _DecrypterV_H_
#define _DecrypterV_H_

namespace acs {

// support class for versioning

class DecrypterV { // PRQA S 2109
public:
	DecrypterV() throw() ;
	~DecrypterV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not defined to prevent their use
	DecrypterV(const DecrypterV &) ;
	DecrypterV &operator =(const DecrypterV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _DecrypterV_H_

