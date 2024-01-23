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

	$Prod: CDP App $

*/

#ifndef _ladgsaV_H_
#define _ladgsaV_H_

namespace acs {

// support class for versioning

class ladgsaV { // PRQA S 2109
public:
	ladgsaV() ;
	~ladgsaV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	ladgsaV(const ladgsaV &) ; // not implemented 
	ladgsaV& operator= (const ladgsaV &) ; // not implemented 
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _ladgsaV_H_



