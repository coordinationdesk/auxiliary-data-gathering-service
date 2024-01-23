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

	$Prod: A.C.S. libRollingAlgo$
 */

#ifndef _S1ParametersBasedSelector_H_
#define _S1ParametersBasedSelector_H_

#include <InvPolicyCommonSelector.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * Implement the InvPolicySelector class selecting inventory items
 * according to the S1 Rolling specific needs.
 **/

class S1ParametersBasedSelector: public InvPolicyCommonSelector  { // PRQA S 2109, 2153
public:

	/** Class constructor */
	explicit S1ParametersBasedSelector( const InvPolicy &);
	/** Destructor */
	virtual ~S1ParametersBasedSelector() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS( S1ParametersBasedSelector );

public:

	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

private:
	/** Return a S1 Specific Sql Query involving time thresholds and Centre
	 * Parameters: cf. S1PDGS-31618 */
	std::string getSpecificTimeAndCentreQuery( std::string const& localCentreID,
			double const& maxDeltaOldProducts_days, double const& rollingPolicyNotUsedProducts_days, dbConnection & conn ) const;

private:
	/** Latency configured on the policy */
	int64_t _latencyHours ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1ParametersBasedSelector)

};

_ACS_END_NAMESPACE

#endif //_S1ParametersBasedSelector_H_

