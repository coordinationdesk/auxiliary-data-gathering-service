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

	$Prod: SRV Standalone App $


*/


#ifndef _SRVStandaloneAppV_H_
#define _SRVStandaloneAppV_H_

namespace acs {

// support class for versioning

class SRVStandaloneAppV { // PRQA S 2109
public:
	SRVStandaloneAppV() throw() ;
	~SRVStandaloneAppV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	SRVStandaloneAppV(const SRVStandaloneAppV &) ; // not implemented 
	SRVStandaloneAppV& operator= (const SRVStandaloneAppV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _SRVStandaloneAppV_H_



