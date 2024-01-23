/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libSRVImporterApp$
        
*/

#ifndef _SRVImporterAppV_H_
#define _SRVImporterAppV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class SRVImporterAppV {
public:
	SRVImporterAppV()  ACS_NOEXCEPT ;
	~SRVImporterAppV() ACS_NOEXCEPT ;
	static const char*  tag()   ACS_NOEXCEPT ;
	static const char*  name()  ACS_NOEXCEPT ; 
private:
	static const char* 	_tag;
	static const char* 	_name;
};

_ACS_END_NAMESPACE

#endif 


