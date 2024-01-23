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

#include <RollingPolicyManager.h>

#include <RollingSpecialPolicies.h>
#include <RollingAlgoUtils.h>

#include <dbConnectionPool.h>

#include <Application.h>

#include <Filterables.h>

#include <DateTime.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingPolicyManager)



RollingPolicyManager::RollingPolicyManager():
	_standbyCondition(),
	_pollingTimerMsec(10000),
	_emptyProcessingSlots(1),
	_queuedRollingPolicyProcessors(0),
	_runningRollingPolicyProcessors(0),
	_pausedRollingPolicyProcessors(0),
	_listOfOutputs(0),
	_listOfErrors(0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Init by configuration

	rsResourceSet & conf = ConfigurationSingleton::instance()->get();

	conf.getValue("RollingArchive.RollingPolicy.AvailableStots", _emptyProcessingSlots, false);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Rolling policies execution parallelism: ["<<_emptyProcessingSlots<<"]" );

}

RollingPolicyManager::~RollingPolicyManager() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


void RollingPolicyManager::halt(StopController::HaltLevel l, bool joinAlso) {

	if ( StopController::IMMEDIATE == l ) {
		ACS_LOG_INFO("RollingPolicyManager: Abort request received. Aborting Pending Rolling Processors...");
	} else {
		ACS_LOG_INFO("RollingPolicyManager: Nominal Stop request received. Waiting for Pending Rolling Processors completion...");
	}

	ACS_LOG_PRIVINFO("RollingPolicyManager: Wake up if waiting...");

	{ //Notify the stop event. Wake up if sleeping.
    	ThreadSafe::Lock conditionLock(_standbyCondition);
        // Do not join with the mutex locked just set the halt level at this point
		Thread::halt(l,false);
		_standbyCondition.notify_all();
	}
    // Here the lock is free, can join if needed
    Thread::halt (l,joinAlso);
}


//STL helper: is RollingPolicyProcessor in current processing list?
struct searchProcessor: public func::binary_function<RollingPolicyProcessor::Sptr, dbPersistent::IdType, bool> {
	bool operator()(const RollingPolicyProcessor::Sptr& pp, const dbPersistent::IdType& policyId) const {
		return ( pp->getPolicyId() == policyId );
	}
};

//STL helper: is RollingPolicyOutput in current output list?
struct searchOutput: public func::binary_function<RollingPolicyOutput::Sptr, dbPersistent::IdType, bool> {
	bool operator()(const RollingPolicyOutput::Sptr& pp, const dbPersistent::IdType& policyId) const {
		return ( pp->getPolicyId() == policyId );
	}
};

//for_each support class: remove a RollingPolicyProcessor::Sptr item from deque
struct RemoveRollingPolicyProcessor {
	RemoveRollingPolicyProcessor( deque<RollingPolicyProcessor::Sptr>* dq ): _dq(dq) {}
	void operator() (RollingPolicyProcessor::Sptr pp) {
		_dq->erase (
			remove( _dq->begin(), _dq->end(), pp ),
			_dq->end() );
	}
	private: deque<RollingPolicyProcessor::Sptr>* _dq;
};


bool RollingPolicyManager::isInCurrentProcessingList( dbPersistent::IdType const& rollPolicyId ) const {
	bool running(false);
	bool paused(false);
	bool queued(false);
	bool executed(false);
 	{
		std::deque<RollingPolicyProcessor::Sptr>::const_iterator rt =
				std::find_if( _runningRollingPolicyProcessors.begin(), _runningRollingPolicyProcessors.end(), std::bind2nd(searchProcessor(), rollPolicyId ) ) ;

		running = ( rt != _runningRollingPolicyProcessors.end() );

		std::deque<RollingPolicyProcessor::Sptr>::const_iterator pt =
				std::find_if( _pausedRollingPolicyProcessors.begin(), _pausedRollingPolicyProcessors.end(), std::bind2nd(searchProcessor(), rollPolicyId ) ) ;
		paused = ( pt != _pausedRollingPolicyProcessors.end() );
	}

	if ( not running && not paused ) {

		std::deque<RollingPolicyProcessor::Sptr>::const_iterator qt =
				std::find_if( _queuedRollingPolicyProcessors.begin(), _queuedRollingPolicyProcessors.end(), std::bind2nd(searchProcessor(), rollPolicyId ) ) ;
		queued = ( qt != _queuedRollingPolicyProcessors.end() );

		std::deque<RollingPolicyOutput::Sptr>::const_iterator ot =
				std::find_if( _listOfOutputs.begin(), _listOfOutputs.end(), std::bind2nd(searchOutput(), rollPolicyId ) ) ;

		std::deque<RollingPolicyOutput::Sptr>::const_iterator et =
				std::find_if( _listOfErrors.begin(), _listOfErrors.end(), std::bind2nd(searchOutput(), rollPolicyId ) ) ;

		executed = (
				ot != _listOfOutputs.end() ||
				et != _listOfErrors.end()       );
	}

	return ( queued || running || paused || executed );
}


size_t RollingPolicyManager::addRollingPoliciesToProcess( const vector<dbPersistent::IdType> & policyIds, vector<dbPersistent::IdType>& failedPolicyIds ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	size_t countNewProcessing(0);
	if ( ! this->requestedCancel() ) {

		if ( policyIds.empty() ) {
			return 0;
		}

		{
			Lock lock( _standbyCondition );

			size_t countErrorInit(0);
			size_t countAlreadyRunning(0);
			for ( std::vector<dbPersistent::IdType>::const_iterator it = policyIds.begin(); it!=policyIds.end(); ++it ) {

				if ( not this->isInCurrentProcessingList( *it ) ) {

					//Set up the rolling  processing and queue it
					RollingPolicyProcessor::Sptr rollingPolicyProcessorSptr( new RollingPolicyProcessor );

					try {
						rollingPolicyProcessorSptr->init( *it );
						rollingPolicyProcessorSptr->setAvailableSlotCondition( &_standbyCondition );

						_queuedRollingPolicyProcessors.push_back( rollingPolicyProcessorSptr );
						++countNewProcessing;

					} catch (std::exception& ex) {

						failedPolicyIds.push_back( *it );
						++countErrorInit;

						ACS_LOG_NOTIFY_EX(ex);
					}

				} else {
					++countAlreadyRunning;
				}

			}

			if (countNewProcessing > 0) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RollingPolicyManager: received "<<countNewProcessing<<" new rolling policies to be processed.");
			}
			if (countErrorInit > 0) {
				ACS_LOG_WARNING("RollingPolicyManager: failed initialization of "<<countErrorInit<<" new rolling policies to be processed.");
			}
			if (countAlreadyRunning > 0) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RollingPolicyManager: ignored "<<countAlreadyRunning<<" rolling policies because already in current processing list.");
			}

		}

		{//Notify something to be processed
			ThreadSafe::Lock conditionLock(_standbyCondition);
			_standbyCondition.notify_all();
		}

	} else {
		ACS_LOG_WARNING("RollingPolicyManager: Request to run rolling policy rejected. Stop received on Rolling Archive.");
	}

	return countNewProcessing;

}


void RollingPolicyManager::fetchRollingPoliciesOutputs( vector<RollingPolicyOutput::Sptr> & listOfOutputs, vector<RollingPolicyOutput::Sptr> & listOfErrors) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG( "_queuedRollingPolicyProcessors : "<<_queuedRollingPolicyProcessors .size() );
		ACS_LOG_DEBUG( "_runningRollingPolicyProcessors: "<<_runningRollingPolicyProcessors.size() );
		ACS_LOG_DEBUG( "_pausedRollingPolicyProcessors : "<<_pausedRollingPolicyProcessors .size() );
		ACS_LOG_DEBUG( "_listOfOutputs                 : "<<_listOfOutputs                 .size() );
		ACS_LOG_DEBUG( "_listOfErrors                  : "<<_listOfErrors                  .size() );
	ACS_CLASS_END_DEBUG

	ThreadSafe::Lock lock( _standbyCondition );

	listOfOutputs = vector<RollingPolicyOutput::Sptr>( _listOfOutputs.begin(), _listOfOutputs.end() );
	listOfErrors  = vector<RollingPolicyOutput::Sptr>( _listOfErrors .begin(), _listOfErrors .end() );

	//Forget about them (shared pointers are released)
	_listOfOutputs.clear();
	_listOfErrors .clear();
}


void RollingPolicyManager::notifyConsumptions( const vector<RollingPolicyOutput::Sptr> & listOfConsumedOutputs ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( listOfConsumedOutputs.empty() ) {
		return;
	}

	if ( this->requestedCancel() ) {
		ACS_LOG_WARNING("RollingPolicyManager: Request to run rolling policy rejected. Stop received on Rolling Archive.");
		return;
	}

	ThreadSafe::Lock lock( _standbyCondition );

	//Move paused processing (if existing) to processing queue
	//Rolling Processors to be moved from the paused queue to the processing queue
	std::deque<RollingPolicyProcessor::Sptr> restoredProcessors(0);

	for ( vector<RollingPolicyOutput::Sptr>::const_iterator it = listOfConsumedOutputs.begin(); it != listOfConsumedOutputs.end(); ++it ) {

		const RollingPolicyOutput::Sptr & rollingPolicyOutputSptr = *it;

		const dbPersistent::IdType rollPolicyId = rollingPolicyOutputSptr->getPolicyId();

		std::deque<RollingPolicyProcessor::Sptr>::const_iterator pt =
				std::find_if( _pausedRollingPolicyProcessors.begin(), _pausedRollingPolicyProcessors.end(), std::bind2nd(searchProcessor(), rollPolicyId ) ) ;

		const bool policyIsPaused = ( pt != _pausedRollingPolicyProcessors.end() );

		if ( policyIsPaused ) {

			const RollingPolicyProcessor::Sptr & pausedRollingPolicyProcessor = (*pt);

			//Move the processor from paused processors to queued processors
			_queuedRollingPolicyProcessors.push_back( pausedRollingPolicyProcessor );

			restoredProcessors.push_back( pausedRollingPolicyProcessor );
		}

		//Update the rolling policy
		//Update the clob status of the policy
		if ( not rollingPolicyOutputSptr->isSpecialPolicy() ) {
			const string policyStatusToUpdate = rollingPolicyOutputSptr->getNewPolicyStatus();
			//Update also the trigger time if Rolling Policy has been completed.
			const bool updateTriggerTime = not policyIsPaused;
			DateTime nextTriggerTime;
			bool updated = RollingPolicyProcessor::updatePolicy( rollPolicyId, updateTriggerTime, true, policyStatusToUpdate, nextTriggerTime  );
			if ( updateTriggerTime && updated ) {
				ACS_LOG_WARNING("Policy id ["<< rollPolicyId <<"] next execution set to ["<<nextTriggerTime.asISO8601(0)<<"]" );
			}
		}
	}

	//Remove the restored rolling processors from the paused queue
	for_each( restoredProcessors.begin(), restoredProcessors.end(), RemoveRollingPolicyProcessor(&_pausedRollingPolicyProcessors) );

}


size_t RollingPolicyManager::getNumOfPendingRequests() const {
	ThreadSafe::Lock lock( _standbyCondition );
	return _queuedRollingPolicyProcessors.size() + _pausedRollingPolicyProcessors.size();
}

size_t RollingPolicyManager::getNumOfRunningRequests() const {
	ThreadSafe::Lock lock( _standbyCondition );
	return _runningRollingPolicyProcessors.size();
}


bool RollingPolicyManager::endMyJobNow() {

	bool result(false);

	//End my job in case of:
	// - immediate stop request
	// - graceful stop request, but processors end.

	// Note: after my client sent me stop, it do not call addRollingPoliciesToProcess anymore.
	//  And even if would call it (but it is not the case), added rolling processors will be processed. I will exit when no processing is running or queued.

	if        ( this->haltLazy() ) {
		//graceful stop ==> exit at the end of current running processing. Queued and paused policies will not be run.
		result = ( 0 == _runningRollingPolicyProcessors.size() );
		//result = ( 0 == _runningRollingPolicyProcessors.size() + _queuedRollingPolicyProcessors.size() );

	} else if ( this->haltImmediate() ) {
		result = true;

	} else {
		result = false;
	}

	return result;
}

//STL helper: is RollingPolicyProcessor running?
struct isRunning: protected func::unary_function<RollingPolicyProcessor::Sptr, bool> {
	bool operator()(RollingPolicyProcessor::Sptr& e) const {
		return ( e->running() );
	}
};

//STL helper: stop RollingPolicyProcessor
struct stopRunning: public func::unary_function<RollingPolicyProcessor::Sptr, void> {
	void operator()(RollingPolicyProcessor::Sptr& e) const {
		e->stop(false);
	}
};


void RollingPolicyManager::run() {

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RollingPolicyManager: started.");

	//Init
    const string dbKey = Application::instance()->getDefaultConnectionPool()->getKey();

    RollingAlgoUtils::instance()->init( dbKey );

	//Main loop.
	while( ! this->endMyJobNow() ) {

		/*/Pattern is:
		lock the condition
		while (!(boolean_predicate)) {
			wait on condition
		}
		do protected work
		(optionally, signal or broadcast the condition again or change a predicate value)
		unlock the condition
		//*/

		bool hideWaitLog(false); //To not log the wait message for each loop;

		//--lock the condition
        ThreadSafe::Lock lock( _standbyCondition ) ;

		//Update my status -- change the boolean_predicate value
		this->updateFromRollingPolicyProcessorsStatus();

		//You can rest if there's nothing to do -- while (!(boolean_predicate)) {
		while ( (_queuedRollingPolicyProcessors.size()==0 || _emptyProcessingSlots==0 ) &&
				! this->endMyJobNow()                                                 ) {

			try {
				//Sleep and wake me up on _standbyCondition notification or on timer elapses.
				if (!hideWaitLog) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RollingPolicyManager Waiting. Running rolling policies: "<<_runningRollingPolicyProcessors.size()+_pausedRollingPolicyProcessors.size()
							<<"; Queued rolling policies to be processed: "<<_queuedRollingPolicyProcessors.size()<<". Available Processing Slots: "<<_emptyProcessingSlots<<"." );
				}
				hideWaitLog=false;
				//release mutex and block on condition
				if (false == _standbyCondition.timedwait(_pollingTimerMsec )) { //-- wait on condition
                    hideWaitLog=true;
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Time check expired. Checking status. Running rolling policies: "<<_runningRollingPolicyProcessors.size()+_pausedRollingPolicyProcessors.size()
							<<"; Queued rolling policies to be processed: "<<_queuedRollingPolicyProcessors.size()<<". Available Processing Slots: "<<_emptyProcessingSlots<<"." );
                }
				//The mutex is locked by me here (as per timedwait implementation)

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RollingPolicyManager: Woken up.");

			} catch (exception& ex) {
				ACS_LOG_WARNING("RollingPolicyManager::run: Got exception in the wake up condition. Try on next loop. Exception:" << ex.what() );
			}

			//The mutex is locked by me here (as per pthread_cond_wait and timedwait)
			//Notification received OR No notification received for a while. Check the status
			this->updateFromRollingPolicyProcessorsStatus();
		}

		//-- do protected work. The mutex is locked by me here
		//Something to do at last...
		while ( _queuedRollingPolicyProcessors.size() > 0 && _emptyProcessingSlots > 0 &&
				! this->endMyJobNow()               								) {

			//Consume the queued rolling processors

			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "Setting up a new Rolling Processing for policy id ["<<_queuedRollingPolicyProcessors.front()->getPolicyId()<<"].");

			//Run queued processor
			RollingPolicyProcessor::Sptr rollingPolicyProcessorSptr = _queuedRollingPolicyProcessors.front();
			_runningRollingPolicyProcessors.push_back( rollingPolicyProcessorSptr );
			rollingPolicyProcessorSptr->start();

			--_emptyProcessingSlots;
			_queuedRollingPolicyProcessors.pop_front();
		}

		//-- unlock the condition automatically
	}

	//Wait until all the rolling processors end
	{
		size_t count(0);
		bool abortSent(false);
		while ( _runningRollingPolicyProcessors.end() !=
				find_if( _runningRollingPolicyProcessors.begin(), _runningRollingPolicyProcessors.end(), isRunning() ) ) {

			if ( count % 60 == 0 ) {
				ACS_LOG_INFO("Waiting for Rolling Processors completion.");
				count=0;
			}
			++count;
			Timer::delay( 1000 );

			//Check if the abort request has to be fulfilled
			HaltLevel haltLevel = this->getHaltLevel();
			if ( StopController::IMMEDIATE == haltLevel && !abortSent) {
				abortSent = true;
				ACS_LOG_INFO("Sending Abort to all the Pending Rolling Processors... ");
				for_each( _runningRollingPolicyProcessors.begin(), _runningRollingPolicyProcessors.end(), stopRunning() );
			}
		}

		//Update my status - removes also Rolling Processing calling the destructors (through shared pointers)
		this->updateFromRollingPolicyProcessorsStatus();

		ACS_LOG_INFO( "RollingPolicyManager: "<<(abortSent?"Aborted":"Stopped")<<" [" << threadId() << "]");
	}

}


void RollingPolicyManager::updateFromRollingPolicyProcessorsStatus() {

	ThreadSafe::Lock lock( _standbyCondition );

	//Rolling Processors to be removed from the processing queue
	std::deque<RollingPolicyProcessor::Sptr> completedProcessors(0);

	//Check the status of each running rolling processor
	for ( std::deque<RollingPolicyProcessor::Sptr>::iterator it = _runningRollingPolicyProcessors.begin();
		  it != _runningRollingPolicyProcessors.end(); ++it ) {

		//(*it) notifies the end of run(), but it could be still in running status and setException could not be still called on it (cf. Thread::svc).
		if ( (*it)->isPerformed() ) {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Waiting for Performed RollingPolicyProcessor.");

			(*it)->waitExitOrTimeout( 5000 ); //Nominally will return immediately because not running
			//Now for sure the exception (in the case) is set on *it.
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Performed RollingPolicyProcessor ended.");
		}

		//Check exceptions on Handled Processor

		exception* ex(NULL);
		bool gotException = (*it)->runException(ex);

		//Get the processing status
		RollingPolicyProcessor::ProcStatus exitStatus( RollingPolicyProcessor::StatusUndefined );
		bool completed = gotException || (*it)->getStatus(exitStatus);

		//Check the status
		if ( completed ) {

			//Remember the processor to remove
			completedProcessors.push_back( *it );

			//Get the policy id
			dbPersistent::IdType policyId( (*it)->getPolicyId() );

			if ( not gotException && exitStatus == RollingPolicyProcessor::StatusCompleted ) {

				//Rolling policy processed
				_listOfOutputs.push_back( (*it)->getRollingPolicyOutputSptr() );

				ACS_LOG_INFO("Policy id ["<<policyId<<"] successfully processed (THIS POLICY COMPLETED ITEMS RETRIEVAL ON CURRENT RUNNING LOOP).");

			} else if ( not gotException && exitStatus == RollingPolicyProcessor::StatusPaused ) {

				//Rolling policy processed
				_listOfOutputs.push_back( (*it)->getRollingPolicyOutputSptr() );

				//Keep this processor in the paused queue
				_pausedRollingPolicyProcessors.push_back( *it );

				ACS_LOG_INFO("Policy id ["<<policyId<<"] successfully processed (THIS POLICY WILL RETRIEVE MORE ITEMS ON NEXT RUNNING LOOP).");

			} else {

				//Some error during processing
				_listOfErrors.push_back( (*it)->getRollingPolicyOutputSptr() );

				ostringstream msg; msg<<"Policy id ["<< policyId <<"] processed with Error.\n";
				if ( 0 != ex ) { msg<<"Occurred Error: "<<ex->what() ; }
				ACS_LOG_WARNING(msg.str());

				//No exceptions allowed here. It's important to update completedProcessors, at the end of this method.
				try {
					DateTime nextTriggerTime;
					if ( RollingPolicyProcessor::updatePolicy( policyId, true, false, "", nextTriggerTime  ) ) {
						ACS_LOG_WARNING("Policy id ["<< policyId <<"] execution will be retried at ["<<nextTriggerTime.asISO8601(0)<<"]" );
					} else {
						ACS_LOG_WARNING("Policy id ["<< policyId <<"]. ERROR UPDATING POLICY. Execution will be retried on next running loop." );
					}
				} catch( exception& ex) {
					ACS_LOG_NOTIFY_EX(ex);
				}
			}
		}
		if ( 0 != ex ) { delete ex;}
	}

	//Remove the completed (success and/or error) rolling processors from the processing queue
	for_each( completedProcessors.begin(), completedProcessors.end(), RemoveRollingPolicyProcessor(&_runningRollingPolicyProcessors) );

	//Freed processing slots
	_emptyProcessingSlots += completedProcessors.size();

}



_ACS_END_NAMESPACE

