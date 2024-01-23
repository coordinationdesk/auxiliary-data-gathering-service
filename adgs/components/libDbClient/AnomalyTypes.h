// PRQA S 1050 EOF
/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
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
	virtual ~AnomalyTypes() throw() {}  // PRQA S 2131

protected:
	explicit AnomalyTypes(const std::string &key) :
        NameIdAssociator<short>("T_Anomaly_Types", "Id", "Anomaly_Type", key),
        ParamConstructibleSingletonMap<AnomalyTypes, std::string>()
        {
            NameIdAssociator<short>::load() ;
        }

private:
	AnomalyTypes &operator=(const AnomalyTypes &) ; 	// declared but not implemented
	AnomalyTypes (const AnomalyTypes &) ; 			// declared but not implemented
	AnomalyTypes () ; 									// declared but not implemented
} ; 


_ACS_END_NAMESPACE




#endif // _AnomalyTypes_H_
