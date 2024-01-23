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

#include <RollingPolicyProcessor.h>

#include <RollingStopPredicate.h>

#include <InvPolicy.h>
#include <InvPolicySelectorFactory.h>
#include <InvPolicySelector.h>

#include <dbConnectionPool.h>

#include <Application.h>
#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingPolicyProcessor)



RollingPolicyProcessor::RollingPolicyProcessor():
	_availableSlotConditionPtr(NULL),
	_isPerformed(false),
	_status(StatusUndefined),
	_policyId(0),
	_rollingPolicyOutputSptr(),
	_lastSelectSessionInfo()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RollingPolicyProcessor::~RollingPolicyProcessor() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RollingPolicyProcessor::init( dbPersistent::IdType const& policyId ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_policyId = policyId;

	_rollingPolicyOutputSptr.reset( new RollingPolicyOutput );
	_rollingPolicyOutputSptr->setPolicyId( _policyId );

}

void RollingPolicyProcessor::halt(StopController::HaltLevel l, bool joinAlso) {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	Thread::halt(l,joinAlso);
}

dbPersistent::IdType RollingPolicyProcessor::getPolicyId() const {
	return _policyId;
}

void RollingPolicyProcessor::setAvailableSlotCondition( Condition* availableSlotConditionPtr ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_availableSlotConditionPtr = availableSlotConditionPtr;
}

bool RollingPolicyProcessor::getStatus(ProcStatus& code)  {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	code = _status;

	//Return completed or not
	return ( _isPerformed && !this->running() );

}

bool RollingPolicyProcessor::isPerformed() const {
	return _isPerformed;
}

//Helper class to notify my manager I've terminated the run (because finished or exception)
class RollingPolicyProcessorNotifyEnd {
public:
	RollingPolicyProcessorNotifyEnd(Condition* c, bool& isPerformed): _c(c), _isPerformed(isPerformed) { }
	~RollingPolicyProcessorNotifyEnd() ACS_NOEXCEPT {
		ThreadSafe::Lock conditionLock(*_c);
		_isPerformed=true;
		_c->notify_all();
	}
	ACS_DECLARE_NO_COPY_CLASS(RollingPolicyProcessorNotifyEnd);
private:
	Condition* _c;
	bool& _isPerformed;
};

namespace {
	string status2str( const RollingPolicyProcessor::ProcStatus & status ) {
		if      ( status == RollingPolicyProcessor::StatusUndefined ) { return "StatusUndefined" ; }
		else if ( status == RollingPolicyProcessor::StatusCompleted ) { return "StatusCompleted" ; }
		else if ( status == RollingPolicyProcessor::StatusPaused    ) { return "StatusPaused"    ; }
		else if ( status == RollingPolicyProcessor::StatusError	    ) { return "StatusError"	 ; }
		return "";
	}
}


void RollingPolicyProcessor::extendSelectLimit( InvPolicy& invPolicy ) const {

	// For this policy, _rollingPolicyOutputSptr contains skipped actions FROM THE FIRST TO THE previous loop
	//  of this RollingPolicyProcessor instance (i.e. skipped actions are cumulative).
	// Because skips in the previous loop, the current select will retrieve again the same items that
	// can potentially be skipped again.
	// To prevent the select - skip loop, allow the current selection to get more items, extending
	// the limit of the select.
	// New selected items are filtered out if they were in skipped actions (blacklisted).
	// The entire process (consider again the items in errors) will be repeated when the policy restart from scratch
	// (i.e. the init method is called again).

	InvPolicy::NumInteger limitDelta = _rollingPolicyOutputSptr->getListRollingActionsSkipped().size();
	if ( 0 < limitDelta ) {
		ACS_LOG_WARNING("Policy id ["<<_policyId<<"]: skipped actions detected on this policy. Extending the query limit for current loop by ["<<limitDelta<<"]");
	}

	//Check the limitDelta is not too high (will stuck the agent on stressing DB)
	{
		InvPolicy::NumInteger rowLimit(0);
		bool isNull = invPolicy.get_fetchrowslimit(rowLimit);
		InvPolicy::NumInteger upperLimit( rowLimit * 10 );
		if ( not isNull && rowLimit != InvPolicy::Unlimited_fetchrowslimit && limitDelta > upperLimit ) {
			ostringstream msg;
			msg<<"Policy id ["<<_policyId<<"]: Too many skipped actions for this policy. "
				"Configured limit is ["<<rowLimit<<"]. Tried to extend the limit by further ["<<limitDelta<<"]. "
				"Will retry on the next triggerring time.";
			ACS_THROW( exRollingPolicyProcessorException( msg.str() ) );
		}
	}

	invPolicy.add_fetchrowslimit_idlenumitems( limitDelta );

	//Consider also the distinct case.
	//Example: limit=10. returned items= 10. distinct items = 2; The 2 distinct items will be skipped (for example because a manual keep policy).
	//          So limit will be 10+2, but distinct items are again the 2 that will be skipped ==> limit again 10+2 ==> loop
	//Extend the limit by the factor tot_items/distinct
	size_t tot  = _lastSelectSessionInfo.getNumItems();
	size_t dist = _lastSelectSessionInfo.getNumItemsDistinct();
	if ( dist > 0 && dist != tot) {
		float extendFactor = static_cast<float>(tot) / static_cast<float>(dist);

		InvPolicy::NumInteger rowLimit(0);
		bool isNull = invPolicy.get_fetchrowslimit(rowLimit);
		if ( not isNull && rowLimit != InvPolicy::Unlimited_fetchrowslimit ) {

			size_t limitDelta = rowLimit * extendFactor - rowLimit;

			ACS_LOG_WARNING("Policy id ["<<_policyId<<"]: Distinct factor is ["<<extendFactor<<"]. Extending the query limit for current loop by ["<<limitDelta<<"]");

			invPolicy.add_fetchrowslimit( limitDelta );
		}
	}
}

void RollingPolicyProcessor::run() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_status = StatusUndefined;

	//Helper notifier: when run finishes (nominally or exception), notify my manager some slot have been freed;
	RollingPolicyProcessorNotifyEnd rollingPolicyProcessorNotifyEnd( _availableSlotConditionPtr, _isPerformed );

	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Policy id ["<<_policyId<<"] execution started. Thread id ["<<this->threadId()<<"]" );

	//Reset my output for this running loop.
	// Keep the list of error actions to implement the blacklist.
	// i.e. _rollingPolicyOutputSptr->getListRollingActionsSkipped() must not be be cleared (it will be at destrucor time)
	_rollingPolicyOutputSptr->getListRollingActions().clear();

	bool errorRollSelect(false);

	//Load the policy from db
	dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
    const string dbKey = cp->getKey();

	try {
		RollingStopPredicate stop_pred;

		dbConnectionWrapper pw(*cp); dbConnection & conn = pw ;

		InvPolicy invPolicy (_policyId, conn);
		string checkReport;
		ACS_COND_THROW( not invPolicy.check(checkReport), exRollingPolicyProcessorException("Policy check error: "+checkReport) );

		//Prevent the select-skip loop
		this->extendSelectLimit( invPolicy );

		//Fills list with InventoryObject and related Action
		unique_ptr<InvPolicySelector> theSelector ( InvPolicySelectorFactory::instance()->newSelector(invPolicy ) );

		if ( not theSelector ) {
			ostringstream msg; msg<<"Policy id ["<<_policyId<<"]: Cannot find a Selector for policy";
			ACS_THROW( exRollingPolicyProcessorCriticalException( msg.str()) );
		}

		//Init the select
		theSelector->selectInit( conn );

		// Select the items ( do the query and append results to objectsToProcess)
		set<RollingAction> & listRollingActions = _rollingPolicyOutputSptr->getListRollingActions();
		string newPolicyStatus;
		theSelector->selectRun( conn, _lastSelectSessionInfo, newPolicyStatus, listRollingActions, stop_pred) ;

		_rollingPolicyOutputSptr->setNewPolicyStatus( newPolicyStatus );

		//Filter out new items that failed in the previous loop.
		_rollingPolicyOutputSptr->applyBlackList();

	} catch (exception& ex) {
		errorRollSelect = true;
		ostringstream msg; msg << "Policy id ["<<_policyId<<"]: Unable to run selection policy on DB." ;
		exRollingPolicyProcessorCriticalException exn(ex, msg.str());
		ACS_LOG_NOTIFY_EX( exn );
	}

	//Processing end
	_status = StatusCompleted;
	if        ( errorRollSelect )                          { _status = StatusError;
	} else if ( _lastSelectSessionInfo.isPartialResult() ) { _status = StatusPaused;
	}

	string msgStatus = status2str( _status );
	ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Policy id ["<<_policyId<<"] execution status: ["<< msgStatus<<"];");

	//Notify this processing slot is free
	//Done by the RollingPolicyProcessorNotifyEnd
}


const RollingPolicyOutput::Sptr RollingPolicyProcessor::getRollingPolicyOutputSptr() const {
	return _rollingPolicyOutputSptr;
}

bool RollingPolicyProcessor::updatePolicy(
		const dbPersistent::IdType & rollPolicyId,
        const bool & updateTriggerTime,
        const bool & updateStatus, const std::string & policyStatusToUpdate,
        DateTime& nextTriggerTime ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool done(false);
	//Load the policy from db
	dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
	{
		dbConnectionWrapper pw(*cp); dbConnection & conn = pw ;
		InvPolicy invPolicy ( rollPolicyId, conn);

		done = invPolicy.updateStatusAndTriggerTime( rollPolicyId,
				updateTriggerTime,
				updateStatus, policyStatusToUpdate, nextTriggerTime, conn );
	}

	return done;

}



_ACS_END_NAMESPACE

