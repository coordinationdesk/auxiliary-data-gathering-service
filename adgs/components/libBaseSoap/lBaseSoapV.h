// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. BaseSoap Library $

	$Id$

	$Author$

	$Log$	

*/
 
#ifndef _lBaseSoapV_H_
#define _lBaseSoapV_H_

namespace acs {

// support class for versioning

class lBaseSoapV { // PRQA S 2109
public:
	lBaseSoapV() ;
	~lBaseSoapV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	lBaseSoapV(const lBaseSoapV &) ;
	lBaseSoapV &operator=(const lBaseSoapV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lBaseSoapV_H


