// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

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
	SatelliteAcronym &operator=(SatelliteAcronym &&) = delete ;
	SatelliteAcronym (SatelliteAcronym &&) = delete ;
	SatelliteAcronym &operator=(const SatelliteAcronym &) = delete ;
	SatelliteAcronym (const SatelliteAcronym &) = delete ;
	SatelliteAcronym () = delete ;
	virtual ~SatelliteAcronym() = default ;

protected:
	explicit SatelliteAcronym(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Satellites", "satelliteid", "satelliteacronym", key),
        ParamConstructibleSingletonMap<SatelliteAcronym, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }
} ; 


_ACS_END_NAMESPACE




#endif // _SatelliteAcronym_H_
