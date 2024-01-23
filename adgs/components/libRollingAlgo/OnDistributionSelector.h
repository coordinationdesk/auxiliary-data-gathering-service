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

 $Prod: A.C.S.Rolling Algo Library $

 */

#ifndef _OnDistributionSelector_H_
#define _OnDistributionSelector_H_ 

#include <S2ParametersBasedSelector.h>

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Implement the InvPolicySelector class selecting inventory items
 * that have been distributed more than x hours ago.
 *
 * This specialization of S2ParametersBasedSelector does not use the parameter file from the rolling policy.
 *
 **/

class OnDistributionSelector: public S2ParametersBasedSelector { // PRQA S 2109, 2153
public:

	explicit OnDistributionSelector(const InvPolicy &);

	virtual ~OnDistributionSelector() ACS_NOEXCEPT;

	ACS_DECLARE_NO_COPY_CLASS(OnDistributionSelector);

public:

	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

private:
	/** This is read from policy (policy does not have a parameter file) */
	int64_t _latency;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OnDistributionSelector)
};

_ACS_END_NAMESPACE

#endif /* _OnDistributionSelector_H_ */
