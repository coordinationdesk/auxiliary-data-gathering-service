// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $

 */


#include <InvPolicySelectorFactory.h>
#include <RollingPoliciesAlgo.h>
#include <InvPolicy.h>
#include <TimeElapsedSelector.h>
#include <OnDistributionSelector.h>
#include <UsedSelector.h>
#include <ParametersBasedSelector.h>
#include <S2ParametersBasedSelector.h>
#include <S1ParametersBasedSelector.h>
#include <AsynchSmRestoredSizeSelector.h>
#include <StorageSizeSelector.h>
#include <RollingAction.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvPolicySelectorFactory)

InvPolicySelectorFactory::InvPolicySelectorFactory() : 
Singleton<InvPolicySelectorFactory >()
{
}

InvPolicySelectorFactory::~InvPolicySelectorFactory() ACS_NOEXCEPT {
}

// Build the right selector depending on specified policy
InvPolicySelector * InvPolicySelectorFactory::newSelector(const InvPolicy& policy)  // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	RollingPoliciesAlgo* algo = RollingPoliciesAlgo::instance( policy.getDbKey() );

	dbPersistent::IdType algoId(0);
	policy.get_algorithm( algoId );
	string algoName = algo->name(algoId);

	if(StringUtils::equalsNoCase(algoName,"TimeBased")) {
		return new TimeElapsedSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"OnDistribution")) {
		return new OnDistributionSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"Used")) {
		return new UsedSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"S2ParametersBased")) {
		return new S2ParametersBasedSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"S1ParametersBased")) {
		return new S1ParametersBasedSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"AsynchSmRestoredSize")) {
		return new AsynchSmRestoredSizeSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"StorageSize")) {
		return new StorageSizeSelector(policy) ;
	}
	if(StringUtils::equalsNoCase(algoName,"ParametersBased")) {
		return new ParametersBasedSelector(policy) ;
	}

	return 0 ;
}


_ACS_END_NAMESPACE
