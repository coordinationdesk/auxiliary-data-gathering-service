// PRQA S 1050 EOF
/*

	Copyright 2012-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: Importer $

*/


#ifndef _ImporterVersion_h_
#define _ImporterVersion_h_

#include <acs_c++pers.hpp>

namespace acs {

//support class for versioning

class ImporterVersion { // PRQA S 2109
public:
	ImporterVersion() ACS_NOEXCEPT ;
	~ImporterVersion() ACS_NOEXCEPT ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	ImporterVersion(const ImporterVersion&) ;
	ImporterVersion& operator=(const ImporterVersion&) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;
}

#endif // _ImporterVersion_h_
