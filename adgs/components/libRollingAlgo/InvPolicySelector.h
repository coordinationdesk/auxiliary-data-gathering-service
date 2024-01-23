// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $

 */



#ifndef _InvPolicySelector_H_
#define _InvPolicySelector_H_ 

#include <acs_c++config.hpp>

#include <ConfigurationSingleton.h>
#include <dbConnection.h>

#include <InvPolicy.h>
#include <RollingAction.h>
#include <set>


_ACS_BEGIN_NAMESPACE(acs)

/**
 *  This class is an abstract interface, in charge to fill a list of pair containing the
 *  reference to the inventoryfile and a reference to the action to apply
 */
class InvPolicySelector {
public:

	/** S2ParametersBasedSelector Exceptions */
	exDECLARE_EXCEPTION(exInvPolicySelectorException, exException); // PRQA S 2131, 2502 2

	/** Info about the Executed Selector */
	class SessionInfo {
	public:
		SessionInfo(): _numItems(0), _numItemsDistinct(0), _partialResult(false) {}
		SessionInfo(bool partialResult, size_t numItems, size_t numItemsDistinct):
			_numItems(numItems), _numItemsDistinct(numItemsDistinct), _partialResult(partialResult) {}

		size_t getNumItems()         const { return _numItems; }
		size_t getNumItemsDistinct() const { return _numItemsDistinct; }
		bool   isPartialResult()     const { return _partialResult; }

	private:
		/** number of items returned */
		size_t _numItems;
		/** distict number of items returned */
		size_t _numItemsDistinct;
		/** partialResult: there are more items to be returned. Run this policy again (because limit on number of selected items on db). */
		bool _partialResult;
	};

	/** Costructor */
	explicit InvPolicySelector(const InvPolicy &);

	/** Destructor */
	virtual ~InvPolicySelector() ACS_NOEXCEPT;

	/** Init the selections to be done */
	virtual void selectInit( dbConnection & conn ) = 0;

	/**
	 * Abstract Method for select object
	 *
	 * newPolicyStatus: the new clob to write in the policy table.
	 * listOfActions: list of rolling actions to be performed.
	 * */
	virtual void selectRun(dbConnection &,
			SessionInfo& stats,
			std::string& newPolicyStatus,
			std::set<RollingAction> & listOfActions,
			ConfigurationSingleton::StopPredicate &) const = 0;

	ACS_DECLARE_NO_COPY_CLASS( InvPolicySelector );

};

_ACS_END_NAMESPACE


#endif /* _InvPolicySelector_H_ */
