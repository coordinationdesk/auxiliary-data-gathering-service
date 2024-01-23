/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/

#ifndef _RollingActionsCommit_H_
#define _RollingActionsCommit_H_


#include <RollingPolicyManager.h>
#include <RollingPolicyOutput.h>

#include <RollingReport.h>

#include <dbPersistent.h>

#include <Singleton.hpp>
#include <exException.h>

#include <deque>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Apply Rolling Actions
**/

class RollingActionsCommit : public Singleton <RollingActionsCommit> { 
public:

	/** Apply actions.
	 * - Filter out Keep actions
	 * - Resolve conflicts (same item with different action / storage manager)
	 * - Apply actions
	 *  */
	void execute( 	std::deque< RollingPolicyOutput::Sptr >& actionsQueue, RollingPolicyOutput::Sptr & actionsSpecialKeepSptr,
					RollingReport& rollingReport,
					RollingPolicyManager::Sptr & rollingPolicyManagerSptr);

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	RollingActionsCommit();

	ACS_DECLARE_NO_COPY_CLASS(RollingActionsCommit);
	friend RollingActionsCommit* Singleton<RollingActionsCommit>::instance(); //SINGLETON


private:
	/** Clean rollingPolicyOutput_A and rollingPolicyOutput_B, resolving conflicts among them */
	void resolveConflicts( RollingPolicyOutput::Sptr & rollingPolicyOutput_A, RollingPolicyOutput::Sptr & rollingPolicyOutput_B );

private:
	/**
	 * Handle a safe policy:
	 * in case of conflicts (same inventory items, different actions) the action with higher severity level is not performed.
	 * Eg. on item I we have Action Keep and Action Remove. Action Keep is performed (severity level is 0). Action Remove not performed (severiti level is > 0).
	 * map is ActionId --> Severity level
	 * */
	std::map< dbPersistent::IdType , size_t > _severityLevel;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingActionsCommit)

};

_ACS_END_NAMESPACE

#endif //_RollingActionsCommit_H_

