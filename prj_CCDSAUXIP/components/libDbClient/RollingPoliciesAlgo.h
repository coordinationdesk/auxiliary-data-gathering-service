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
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/15 10:26:05  marpas
	typo fixed
	
	Revision 5.1  2016/04/15 10:13:01  marpas
	rewritten as standard NameIdAssociator with dbKey
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2013/04/05 09:36:19  marpas
	NameIdAssociator partial refactoring
	
	Revision 2.2  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2009/05/13 09:11:21  marpas
	rules fixing
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2004/09/03 15:29:49  ivafam
	Added new classes
	
	
*/

#ifndef _RollingPoliciesAlgo_H_
#define _RollingPoliciesAlgo_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
// class RollingPolicy Algorithm
//
//
class RollingPoliciesAlgo: // PRQA S 2153
    public NameIdAssociator<unsigned int>,
    public ParamConstructibleSingletonMap<RollingPoliciesAlgo, std::string>
{
    friend RollingPoliciesAlgo* ParamConstructibleSingletonMap<RollingPoliciesAlgo, std::string>::instance(const std::string &); // PRQA S 2107

public:
	RollingPoliciesAlgo &operator=(RollingPoliciesAlgo &&) = delete ;
	RollingPoliciesAlgo (RollingPoliciesAlgo &&) = delete ;
	RollingPoliciesAlgo &operator=(const RollingPoliciesAlgo &) = delete ;
	RollingPoliciesAlgo (const RollingPoliciesAlgo &) = delete ;
	RollingPoliciesAlgo () = delete ;
	virtual ~RollingPoliciesAlgo() = default ;
	
protected:
	explicit RollingPoliciesAlgo(const std::string &key) :

        NameIdAssociator<unsigned int>("T_RollingPoliciesAlgo", "Id", "Name", key),
        ParamConstructibleSingletonMap<RollingPoliciesAlgo, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }
} ; 


_ACS_END_NAMESPACE

#endif // _RollingPoliciesAlgo_H_
