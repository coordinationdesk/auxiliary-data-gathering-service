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

	$Prod: LTA File Inventory Library $

*/


#ifndef _libltafileinvV_H_
#define _libltafileinvV_H_

namespace acs {

// support class for versioning

class libltafileinvV { // PRQA S 2109
public:
	libltafileinvV() throw() ;
	~libltafileinvV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	libltafileinvV(const libltafileinvV &) ; // not implemented 
	libltafileinvV& operator= (const libltafileinvV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _libltafileinvV_H_

