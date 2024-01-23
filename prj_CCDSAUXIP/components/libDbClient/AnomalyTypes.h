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

#ifndef _AnomalyTypes_H_
#define _AnomalyTypes_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
// class Anomaly Types
//
//

class AnomalyTypes: // PRQA S 2153
    public NameIdAssociator<short>, 
    public ParamConstructibleSingletonMap<AnomalyTypes, std::string>
{
    friend AnomalyTypes* ParamConstructibleSingletonMap<AnomalyTypes, std::string>::instance(const std::string &); // PRQA S 2107
public:
	AnomalyTypes &operator=(AnomalyTypes &&) = delete ;
	AnomalyTypes (AnomalyTypes &&) = delete ;
	AnomalyTypes &operator=(const AnomalyTypes &) = delete ;
	AnomalyTypes (const AnomalyTypes &) = delete ;
	AnomalyTypes () = delete ;
	virtual ~AnomalyTypes() = default ;

protected:
	explicit AnomalyTypes(const std::string &key) :
        NameIdAssociator<short>("T_Anomaly_Types", "Id", "Anomaly_Type", key),
        ParamConstructibleSingletonMap<AnomalyTypes, std::string>()
        {
            NameIdAssociator<short>::load() ;
        }
} ; 


_ACS_END_NAMESPACE




#endif // _AnomalyTypes_H_
