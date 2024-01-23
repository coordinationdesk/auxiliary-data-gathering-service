/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libRollingAlgo$

 */

#ifndef _InvPolicyCommonSelector_H_
#define _InvPolicyCommonSelector_H_

#include <InvPolicySelector.h>
#include <dbSet.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
 * \brief
 * Common implementation for the InvPolicy classes.
 * IMPORTANT: In derived classes DO NOT use DISTINCT clause to select Inventory Id. Otherwise performance strongly decreases!
 *            Distinct operation is done by software, at RollingAction creation time.
 **/

class InvPolicyCommonSelector: public InvPolicySelector {
public:

	/** Default Class constructor */
	explicit InvPolicyCommonSelector(const InvPolicy &);
	/** Destructor */
	virtual ~InvPolicyCommonSelector() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(InvPolicyCommonSelector);

public:

	/** IF implementation. cf. base class */

	virtual void selectInit( dbConnection & conn ) = 0;

	virtual void selectRun(dbConnection &,
			InvPolicySelector::SessionInfo& sessionInfo,
			std::string& newPolicyStatus,
			std::set<RollingAction> & listOfActions,
			ConfigurationSingleton::StopPredicate &) const;

protected:

	/** Set results:
	 * In:
	 *  - queryResult: the result of current session
	 *  - distinctCounter: distinct ids in queryResult
	 * Out:
	 *  - status:  the Policy status (to store as clob in db)
	 *  - partialResult: there are more items to be returned. Run this policy again (because limit on number of selected items on db).  */
	virtual void getSessionResult( const dbSet& queryResult, const size_t & distinctCounter,
			std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const;

	/** As above, but it's the result in case of no query has been set in the selectInit method */
	virtual void getSessionResult( std::string& newPolicyStatus, InvPolicySelector::SessionInfo & sessionInfo ) const;

protected:
	/** Coming from Policy configured in DB */
	InvPolicy::NumId      _polId;
	InvPolicy::NumId      _polAction;
	InvPolicy::NumId      _polStorage;
	std::string           _polFileType;
	std::string           _polFileClass;
	InvPolicy::NumInteger _polFetchRowsLimit ;
	InvPolicy::NumInteger _polIdleNumItems   ;
	std::string           _polParameters     ;
	std::string           _polStatus         ;

	/** CONVENTION: Each query Must return inventory id with this attribute name. */
	const std::string _invIdAttributeName;

	/** EACH DERIVED POLICY MUST SET THE FOLLOWING FIELDS IN THE selectInit method. */

	/** Operator among all the configured sql selection: 'and', 'or' */
	std::string _operatorSelections;

	/** Configured Sql selection */
	std::vector<std::string> _sqlSelections;

	/** Append to the query the "items to be preserverd" part */
	bool _preserveItems;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvPolicyCommonSelector)

};

_ACS_END_NAMESPACE

#endif //_InvPolicyCommonSelector_H_

