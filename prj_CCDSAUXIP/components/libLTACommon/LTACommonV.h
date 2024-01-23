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

	$Prod: LTA common functions $


*/

#ifndef _LTACommonV_H_
#define _LTACommonV_H_

namespace acs {

// support class for versioning

class LTACommonV { // PRQA S 2109
public:
	LTACommonV() ;
	~LTACommonV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	// declared but not defined to prevent their use
	LTACommonV(const LTACommonV &) ;
	LTACommonV & operator=(const LTACommonV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _LTACommonV_H_



