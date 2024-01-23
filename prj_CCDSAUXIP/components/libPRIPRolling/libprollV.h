// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: PRIP Rolling Library $


*/


#ifndef _libprollV_H_
#define _libprollV_H_

namespace acs {

// support class for versioning

class libprollV { // PRQA S 2109
public:
	libprollV() throw() ;
	~libprollV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	libprollV(const libprollV &) ; // not implemented 
	libprollV& operator= (const libprollV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _libprollV_H_

