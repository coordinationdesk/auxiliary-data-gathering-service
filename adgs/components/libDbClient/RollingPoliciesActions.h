// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/18 15:22:01  marpas
	rewritten with standard NameIdAssociator with db key
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:51  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2013/04/05 09:36:19  marpas
	NameIdAssociator partial refactoring
	
	Revision 2.1  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2004/09/03 15:29:49  ivafam
	Added new classes
	
	
*/

#ifndef _RollingPoliciesActions_H_
#define _RollingPoliciesActions_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;
//
//
// class RollingPolicies Actions
//
//
class RollingPoliciesActions: // PRQA S 2153
    public NameIdAssociator<unsigned int>,
    public ParamConstructibleSingletonMap<RollingPoliciesActions, std::string>
{
    friend RollingPoliciesActions* ParamConstructibleSingletonMap<RollingPoliciesActions, std::string>::instance(const std::string &); // PRQA S 2107

public:
	virtual ~RollingPoliciesActions() throw() {}  // PRQA S 2131
	
protected:
	explicit RollingPoliciesActions(const std::string &key) :

        NameIdAssociator<unsigned int>("T_RollingPolicyActions", "Id", "Action", key),
        ParamConstructibleSingletonMap<RollingPoliciesActions, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	RollingPoliciesActions &operator=(const RollingPoliciesActions &) ; 	// declared but not implemented
	RollingPoliciesActions (const RollingPoliciesActions &) ; 			// declared but not implemented
	RollingPoliciesActions () ; 									// declared but not implemented
} ; 



_ACS_END_NAMESPACE


#endif // _RollingPoliciesActions_H_
