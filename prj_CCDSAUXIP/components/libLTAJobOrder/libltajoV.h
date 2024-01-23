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

	$Prod: LTA JobOrder Library $

*/



#ifndef _libltajoV_H_
#define _libltajoV_H_

namespace acs {

// support class for versioning

class libltajoV { // PRQA S 2109
public:
	libltajoV() throw() ;
	~libltajoV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	libltajoV(const libltajoV &) ; // not implemented 
	libltajoV& operator= (const libltajoV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _libltajoV_H_

