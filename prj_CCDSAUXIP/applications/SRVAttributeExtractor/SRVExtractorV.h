// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Attribute Extractor $


*/


#ifndef _SRVExtractorV_H_
#define _SRVExtractorV_H_

#include <acs_c++pers.hpp>

namespace acs {

// support class for versioning

class SRVExtractorV { // PRQA S 2109
public:
	SRVExtractorV() ACS_NOEXCEPT ;
	~SRVExtractorV() ACS_NOEXCEPT ;
	static const char* tag() ;
	static const char* name() ;
private:
	SRVExtractorV(const SRVExtractorV &) ;
	SRVExtractorV &operator =(const SRVExtractorV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _SRVExtractorV_H_
