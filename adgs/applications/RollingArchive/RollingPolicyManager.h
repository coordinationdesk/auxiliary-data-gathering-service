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

#ifndef _RollingPolicyManager_H_
#define _RollingPolicyManager_H_

#include <RollingPolicyProcessor.h>

#include <Thread.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Handle the rolling policies' queue
*
* Start each queued rolling policy, handling available slots
**/

class RollingPolicyManager: public Thread {
public:

	/** Default Class constructor */
	RollingPolicyManager();
	/** Destructor */
	virtual ~RollingPolicyManager() ACS_NOEXCEPT ;

	/** Shared Pointer */
	typedef std::shared_ptr< RollingPolicyManager > Sptr;

	ACS_DECLARE_NO_COPY_CLASS(RollingPolicyManager);

public:

	/** Queue Rolling Policy to process.
	 * - failedRolling: list of Rolling on which cannot initialize processing
	 * return the new queued rolling policies */
	size_t addRollingPoliciesToProcess( const std::vector<dbPersistent::IdType> & policyIds, std::vector<dbPersistent::IdType>& failedPolicyIds );

	/** Copy the outputs to the client. Reset internals. */
	void fetchRollingPoliciesOutputs( std::vector<RollingPolicyOutput::Sptr> & listOfOutputs, std::vector<RollingPolicyOutput::Sptr> & listOfErrors);

	/** Notify to me the consumption of a set of RollingPolicyOutput.
	 * This is used internally to restore the related (if existing) RollingPolicyProcessor (that was paused, waiting for its output's consumption) */
	void notifyConsumptions( const std::vector<RollingPolicyOutput::Sptr> & listOfConsumedOutputs );

	/** Return the number of pending processing (queued and paused) */
	size_t getNumOfPendingRequests() const;

	/** Return the number of running processing */
	size_t getNumOfRunningRequests() const;


protected:
	/** Thread interface implementation */
	virtual void run();
	virtual void halt(StopController::HaltLevel l, bool joinAlso=true);

private:
	/** The rolling policy is already in the current processing list */
	bool isInCurrentProcessingList( dbPersistent::IdType const& rollPolicyId ) const;

	/** Query the rolling processors to update:
	- the emptyProcessingSlots
	- the completedRolling
	- the failedRolling
	*/
	void updateFromRollingPolicyProcessorsStatus();

	/** Check the type of the cancel request Vs pending work.
	 * True: end my work as soon as possible */
	bool endMyJobNow();


private:

	/** Condition for the standby */
	Condition _standbyCondition;

	/** Wake up after this time interval. Even if nobody notifies you on the standby condition.  [msec]
	 * The processing manager is woken up by event (Rolling Policy Processor ends, request for a new rolling...).
	 * But, anyway wake up after this time interval to prevent hanging. */
	unsigned long _pollingTimerMsec;

	/** Current number of available slots for rolling processing (initialized by configuration). */
	int _emptyProcessingSlots;

	/** Set of rolling processors to run. */
	std::deque<RollingPolicyProcessor::Sptr> _queuedRollingPolicyProcessors;

	/** Running rolling processors. */
	std::deque<RollingPolicyProcessor::Sptr> _runningRollingPolicyProcessors;

	/** Paused rolling processors.
	 * A Rolling processor is paused when its last output (a burst of items) is being rolled.
	 * When its last output is processed, will be played again to return a new burst of items. */
	std::deque<RollingPolicyProcessor::Sptr> _pausedRollingPolicyProcessors;

	/** rolling policies' output queue (success) */
	std::deque<RollingPolicyOutput::Sptr> _listOfOutputs;

	/** rolling policies' output queue (error) */
	std::deque<RollingPolicyOutput::Sptr> _listOfErrors;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingPolicyManager)

};

_ACS_END_NAMESPACE

#endif //_RollingPolicyManager_H_

