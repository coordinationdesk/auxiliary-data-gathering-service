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

#ifndef _RollingPolicyProcessor_H_
#define _RollingPolicyProcessor_H_

#include <RollingPolicyOutput.h>

#include <Thread.h>
#include <exException.h>

#include <InvPolicySelector.h>

#include <memory>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Process a single Rolling policy
*
* For the received rolling policy
* - select items according to the policy (handling partial processing with limit clause)
* - return actions related to the policy
*
* Return to the client the execution status
**/

class RollingPolicyProcessor: public Thread  {
public:

	/** RollingPolicyProcessor Exceptions */
	exDECLARE_EXCEPTION(exRollingPolicyProcessorException, exException); // RollingPolicyProcessor Exception.
	exDECLARE_EXCEPTION(exRollingPolicyProcessorCriticalException, exCriticalException); // RollingPolicyProcessor Critical Exception.


	/** Default Class constructor */
	RollingPolicyProcessor();
	/** Destructor */
	virtual ~RollingPolicyProcessor() ACS_NOEXCEPT ;

	/** Shared Pointer */
	typedef std::shared_ptr< RollingPolicyProcessor > Sptr;

	ACS_DECLARE_NO_COPY_CLASS(RollingPolicyProcessor);
	
public:

	typedef enum {
		StatusUndefined=-1, 	//NOT DEFINED
		StatusCompleted,	    //The Policy returned items to be rolled. There are no further items involved by my policy: can delete me.
		StatusPaused,	        //The Policy returned items to be rolled. There are further items involved by my policy: call run again to get them.
		StatusError				//The Policy execution was not ok
	} ProcStatus;

	/** Init the object */
	virtual void init( dbPersistent::IdType const& policyId );

	/** Set the available slot condition attribute. */
	void setAvailableSlotCondition( Condition* availableSlotConditionPtr );

	/** Return the status of the rolling policy processing: COMPLETED, NOT COMPLETED. ProcStatus is also returned (Undefined, Success, ...) */
	bool getStatus(ProcStatus& code);

	/** true if the processing has been performed */
	bool isPerformed() const;

	/** Getters and setters. Cf. attributes' description */
	dbPersistent::IdType            getPolicyId() const;
	const RollingPolicyOutput::Sptr getRollingPolicyOutputSptr() const;

	/** Update the rolling policy */
	static bool updatePolicy( const dbPersistent::IdType & rollPolicyId,
	                          const bool & updateTriggerTime,
	                          const bool & updateStatus, const std::string & policyStatusToUpdate,
	                          DateTime& nextTriggerTime );

protected:
	/** Thread interface implementation */
	virtual void run();
	virtual void halt(StopController::HaltLevel l, bool joinAlso=true);

private:

	/** Extends the limit query to prevent select-skip loop */
	void extendSelectLimit(InvPolicy& invPolicy) const;

private:

	/** To notify when a slot has been freed. */
	Condition* _availableSlotConditionPtr;

	/** Run called and performed */
	bool _isPerformed;

	/** The processing status */
	ProcStatus _status;

	/** The policy id */
	dbPersistent::IdType _policyId;

	/** My Ouput */
	RollingPolicyOutput::Sptr _rollingPolicyOutputSptr;

	/** Last select session info **/
	InvPolicySelector::SessionInfo _lastSelectSessionInfo;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingPolicyProcessor)

};

_ACS_END_NAMESPACE

#endif //_RollingPolicyProcessor_H_

