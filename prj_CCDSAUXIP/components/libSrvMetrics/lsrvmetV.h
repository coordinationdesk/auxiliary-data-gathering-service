// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/


#ifndef _lsrvmetV_H_
#define _lsrvmetV_H_

namespace acs {

// support class for versioning

class lsrvmetV { // PRQA S 2109
public:
	lsrvmetV() throw() ;
	~lsrvmetV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	lsrvmetV(const lsrvmetV &) ; // not implemented 
	lsrvmetV& operator= (const lsrvmetV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lsrvmetV_H_

