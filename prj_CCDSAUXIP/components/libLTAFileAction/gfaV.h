// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA File Action library $	



*/

#ifndef _gfaV_H_
#define _gfaV_H_

namespace acs {

// support class for versioning

class gfaV { // PRQA S 2109
public:
	gfaV() noexcept ;
	~gfaV() noexcept ;
	gfaV(const gfaV &) = default ; 
	gfaV& operator= (const gfaV &) = default ; 
	gfaV(gfaV &&) = default ; 
	gfaV& operator= (gfaV &&) = default ; 

	static const char* tag() noexcept ;
	static const char* name() noexcept ;
    
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _gfaV_H_

