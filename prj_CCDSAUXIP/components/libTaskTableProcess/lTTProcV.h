// PRQA S 1050 EOF
/*

	Copyright 2012-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libTaskTableProcess$

	$Id$

	$Author$
    
    $Log$

*/

 
#ifndef _lTTProcV_H_
#define _lTTProcV_H_

namespace acs {

// support class for versioning

class lTTProcV { // PRQA S 2109
public:
	lTTProcV() ;
	~lTTProcV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	lTTProcV(const lTTProcV&) ; // not implemented
	lTTProcV&operator=(const lTTProcV&) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // 


