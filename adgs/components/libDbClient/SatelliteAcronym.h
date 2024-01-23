// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Catalog Library $

	$Id$

	$Author$

	$Log$
	

*/

#ifndef _SatelliteAcronym_H_
#define _SatelliteAcronym_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
// class Satellite Acronym
//
//

class SatelliteAcronym: // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<SatelliteAcronym, std::string>
{
    friend SatelliteAcronym* ParamConstructibleSingletonMap<SatelliteAcronym, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~SatelliteAcronym() throw() {}  // PRQA S 2131

protected:
	explicit SatelliteAcronym(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Satellites", "satelliteid", "satelliteacronym", key),
        ParamConstructibleSingletonMap<SatelliteAcronym, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	SatelliteAcronym &operator=(const SatelliteAcronym &) ; 	// declared but not implemented
	SatelliteAcronym (const SatelliteAcronym &) ; 			// declared but not implemented
	SatelliteAcronym () ; 									// declared but not implemented
} ; 


_ACS_END_NAMESPACE




#endif // _SatelliteAcronym_H_
