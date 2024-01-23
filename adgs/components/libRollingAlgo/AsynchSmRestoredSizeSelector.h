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

#ifndef _AsynchSmRestoredSizeSelector_H_
#define _AsynchSmRestoredSizeSelector_H_

#include <InvPolicyCommonSelector.h>
#include <exException.h>

#include <stdint.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Implement the InvPolicySelector class selecting inventory items.
 * Select Inventory items (parents or independents) that have been restored from an asynch storage manager.
 * Selected items are the oldest ones can be deleted to free up space in ISM, according to configured threshold.
 *
 **/

class AsynchSmRestoredSizeSelector: public InvPolicyCommonSelector  { // PRQA S 2109
public:

	/** Class constructor */
	explicit AsynchSmRestoredSizeSelector( const InvPolicy &);
	/** Destructor */
	virtual ~AsynchSmRestoredSizeSelector() ACS_NOEXCEPT;

	ACS_DECLARE_NO_COPY_CLASS( AsynchSmRestoredSizeSelector );

public:

	/** IF implementation. cf. base class */
	virtual void selectInit( dbConnection & conn );

protected:
	/** cf. base class*/
	virtual void getSessionResult( const dbSet& queryResult, const size_t & distinctCounter,
			std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const;

	virtual void getSessionResult( std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const;

private:
	/** Return this policy's query: cf. S2PDGS-1967.
	 * HiQuota: do selection if total occupied size is greater than HiQuota
	 * LowQuota: select items while their total occupying size is less than LowQuota */
	std::string getRestoredSizeQuery(dbConnection &conn, uint64_t const& hiQuota, uint64_t const& lowQuota ) const;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AsynchSmRestoredSizeSelector)

};

_ACS_END_NAMESPACE

#endif //_AsynchSmRestoredSizeSelector_H_

