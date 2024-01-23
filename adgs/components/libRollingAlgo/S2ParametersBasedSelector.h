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

#ifndef _S2ParametersBasedSelector_H_
#define _S2ParametersBasedSelector_H_

#include <InvPolicyCommonSelector.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * * Implement the InvPolicySelector class selecting inventory items
 * according to the S2 Rolling configuration (stored in Policy's parameters field).
 **/

class S2ParametersBasedSelector: public InvPolicyCommonSelector  { // PRQA S 2109, 2153
public:

	/** Class constructor */
	explicit S2ParametersBasedSelector(const InvPolicy & );
	/** Destructor */
	virtual ~S2ParametersBasedSelector() ACS_NOEXCEPT;

	ACS_DECLARE_NO_COPY_CLASS(S2ParametersBasedSelector);

public:

	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

protected:
	/** Return a TimeThreshold sql Query */
	std::string getTimeThresholdQuery( std::string const& parTimeEvent, std::string const& parTimeThreshold, dbConnection& conn ) const;
	/** Return a Expert sql Query */
	std::string getExpertQuery( std::string const& parExpertWhereStatement, dbConnection& conn ) const;
	/** Return a Cloudcover sql Query */
	std::string getCloudCoverQuery(std::string const& parCloudCovThreshold, dbConnection& conn) const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S2ParametersBasedSelector)
};

_ACS_END_NAMESPACE

#endif //_S2ParametersBasedSelector_H_
