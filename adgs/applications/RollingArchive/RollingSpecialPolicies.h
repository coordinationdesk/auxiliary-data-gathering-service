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

#ifndef _RollingSpecialPolicies_H_
#define _RollingSpecialPolicies_H_

#include <exException.h>
#include <Singleton.hpp>

#include <ThreadSafe.h>

#include <RollingPolicyOutput.h>
#include <set>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Queries for InvSpecialRollingPolicies.
* Performs configured Action on selected items.
* "KEEP" actions are honored (only) by this singleton
*
* Manual rolling has the expiration date:
* after the expiration date, the manual rolling rule is automatically deleted.
* So, for example,
* - If you want invalidate item x now:
* -- set a manual rolling on x, invalidate, no expiration date
*
* - If you want invalidate item x tomorrow:
* -- set a manual rolling on x, keep, expiration date to tomorrow (keep x wins on invalidate x up to tomorrow)
* -- set a manual rolling on x, invalidate, no expiration date    (keep x wins on invalidate x)
*
* - If you want keep item x forever:
* -- set a manual rolling on x, keep, no expiration date
*
**/

class RollingSpecialPolicies : public Singleton <RollingSpecialPolicies> { 

public:
	/** Entry point for Clients requesting the service
	 * - forceExecution: ignore the timer and execute now */
	void execute(bool forceExecution);

	/** Get the items to keep */
	RollingPolicyOutput::Sptr getListRollingActionsKeep() const;
	/** Get the items roll */
	RollingPolicyOutput::Sptr getListRollingActionsNoKeep_and_clear();

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	RollingSpecialPolicies();

	ACS_DECLARE_NO_COPY_CLASS(RollingSpecialPolicies);
	friend RollingSpecialPolicies* Singleton<RollingSpecialPolicies>::instance(); //SINGLETON

private:
	/** Refresh configuration */
	void loadConfiguration();

	/** Perform the special rolling actions */
	void doWork();

	/** Update Inv special rolling policies (manual rolling) to remove:
	 * - rules for items not found anymore on SM.
	 * - expired rules */
	void updateInvSpecialRollingPolicies();


private:
	/** Concurrent Threads call my services. Manage requests by mutex */
	Mutex _mutex;

	/** Time between two runs [sec] */
	float _confTimeRunSec;

	/** First time do not consider timer */
	bool _firstRequest;

	/** Timer to execute run */
	Timer _timerRun;

	/** Conventional Value to represent a special rolling policy */
	const dbPersistent::IdType _specialPolicyId;

	/** The Keep actions to apply on the set of items */
	std::set<RollingAction> _listRollingActionsKeep;

	/** The No-Keep actions to apply on the set of items */
	std::set<RollingAction> _listRollingActionsNoKeep;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingSpecialPolicies)

};

_ACS_END_NAMESPACE

#endif //_RollingSpecialPolicies_H_

