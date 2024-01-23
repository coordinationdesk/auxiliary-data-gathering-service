// PRQA S 1050 EOF
/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.34  2018/10/01 17:18:49  nicvac
	S2PDGS-2050: Graceful stop considers also the queued processing.
	
	Revision 1.33  2018/01/31 17:37:02  nicvac
	S3MPM: Check if package is also in the processed list.
	
	Revision 1.32  2017/05/31 16:36:23  nicvac
	OPMAN-260: Fixed check for existing processing.
	
	Revision 1.31  2017/05/29 17:53:24  nicvac
	OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)
	
	Revision 1.30  2017/03/03 09:43:41  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 Condition Interface
	
	Revision 1.29  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.28  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.27  2016/09/07 08:30:53  nicvac
	CSGACQ-86: Oracle porting.
	
	Revision 1.26  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.25  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.24  2014/10/03 12:16:59  nicvac
	S2PDGS-931 implementation.
	
	Revision 1.23  2014/09/18 16:44:27  nicvac
	S1PDGS-31550 implementation.
	
	Revision 1.22  2014/03/20 15:31:10  marfav
	Condition::timedwait interface changed
	
	Revision 1.21  2013/11/06 13:07:02  nicvac
	Simplified Scheduler and processing status handling.
	
	Revision 1.20  2013/11/05 19:16:09  nicvac
	Log Messages improved.
	
	Revision 1.19  2013/11/04 10:00:33  marpas
	avoiding useless macros
	
	Revision 1.18  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.17  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.16  2013/05/21 09:00:41  nicvac
	Threads synchronization improved.
	
	Revision 1.15  2013/05/20 14:01:46  nicvac
	Threads synchronization improved.
	
	Revision 1.14  2013/05/17 15:33:39  nicvac
	S2PDGS-300: priority handling.
	
	Revision 1.13  2013/03/20 14:00:02  nicvac
	Improved Log messages during shutdown
	
	Revision 1.12  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.11  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.10  2013/01/16 17:34:05  nicvac
	PackageProcessingManager waits on PackageProcessor only if this notifies.
	
	Revision 1.9  2012/12/12 16:18:26  nicvac
	PackageTaskTableManager integration
	
	Revision 1.8  2012/12/07 17:01:43  nicvac
	Condition and Threads stops.
	
	Revision 1.7  2012/12/06 18:35:17  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.6  2012/11/27 14:44:17  nicvac
	Implementation.
	
	Revision 1.5  2012/11/22 18:06:09  nicvac
	Implementation.
	
	Revision 1.4  2012/11/19 17:23:50  nicvac
	Implementing.
	
	Revision 1.3  2012/11/16 15:26:27  nicvac
	Implementation
	
	Revision 1.2  2012/11/12 17:43:56  nicvac
	Interface updated.
	
	Revision 1.1.1.1  2012/11/12 17:03:12  nicvac
	Import libPackageProcessing
	
        
*/ 

#include <PackageProcessingManager.h>

#include <DataProviderConfigurer.h>

#include <PackageTaskTableManager.h>

#include <PackageProcessorFmtDist.h>
#include <PackageProcessorCheck.h>

#include <ConfigurationSingleton.h>

#include <Filterables.h>

#include <Functional.h>

#include <algorithm>

#include <PackageProcessingV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageProcessingManager)

namespace {
    PackageProcessingV version;
}

PackageProcessingManager::PackageProcessingManager():
	_standbyCondition(),
	_pollingTimerMsec(0),
	_emptyProcessingSlots(0), 
	_runningPackageProcessors(0), 
	_queuedPackageProcessors(0),
	_distributedPackages(0), 
	_failedPackages(0),
	_waitingAckPackages(0)
{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);

	//Init by configuration
	ConfigurationSingleton::instance()->get().getValue("PackageProcessing.PollingTimer", _pollingTimerMsec);
	ConfigurationSingleton::instance()->get().getValue("PackageProcessing.AvailableStots", _emptyProcessingSlots);

	//Download the Task Table in cache
	PackageTaskTableManager::instance()->downloadTaskTables();

}

PackageProcessingManager::~PackageProcessingManager() throw() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");

	// Immediate Stop of the thread and join
	halt(StopController::IMMEDIATE, true);
	try {
		//Release the Task Table in cache
		PackageTaskTableManager::instance()->releaseTaskTables();
	} catch(exception& ex) {
		ACS_LOG_NOTIFY_EX(ex);
    }
}

void PackageProcessingManager::halt(StopController::HaltLevel l, bool joinAlso) {

	if ( StopController::IMMEDIATE == l ) {
		ACS_LOG_INFO("PackageProcessingManager: Abort request received. Aborting Pending Package Processors...");
	} else {
		ACS_LOG_INFO("PackageProcessingManager: Nominal Stop request received. Waiting for Pending Package Processors completion...");
	}

	ACS_LOG_PRIVINFO("PackageProcessingManager: Wake up if waiting...");

	{ //Notify the stop event. Wake up if sleeping.
    	ThreadSafe::Lock conditionLock(_standbyCondition);
        // Do not join with the mutex locked just set the halt level at this point
		Thread::halt(l,false);
		_standbyCondition.notify_all();
	}
    // Here the lock is free, can join if needed
    Thread::halt (l,joinAlso);
}


//STL helper: is PackageProcessor in current processing list?
struct searchProcessor: public func::binary_function<PackageProcessorSptr, dbPersistent::IdType, bool> {
	bool operator()(const PackageProcessorSptr& pp, const dbPersistent::IdType& packageId) const {
		return ( pp->getDataPackage()->getPackageId() == packageId );
	}
};

//STL helper: is PackageProcessor in executed processing list?
struct searchProcessorExecuted: public func::binary_function<DataPackageSptr, dbPersistent::IdType, bool> {
	bool operator()(const DataPackageSptr& pp, const dbPersistent::IdType& packageId) const {
		return ( pp->getPackageId() == packageId );
	}
};


bool PackageProcessingManager::isInCurrentProcessingList( dbPersistent::IdType const& packageId ) const {
	bool queued(false);
	bool running(false);
	bool completed(false);
	{
		std::deque<PackageProcessorSptr>::const_iterator rt =
				std::find_if( _runningPackageProcessors.begin(), _runningPackageProcessors.end(), std::bind2nd(searchProcessor(), packageId ) ) ;
		running = ( rt != _runningPackageProcessors.end() );
	}
	if (! running ) {
		std::deque<PackageProcessorSptr>::const_iterator qt =
				std::find_if( _queuedPackageProcessors.begin(), _queuedPackageProcessors.end(), std::bind2nd(searchProcessor(), packageId ) ) ;
		queued = ( qt != _queuedPackageProcessors.end() );


		std::deque<DataPackageSptr>::const_iterator dt =
				std::find_if( _distributedPackages.begin(), _distributedPackages.end(), std::bind2nd(searchProcessorExecuted(), packageId ) ) ;

		std::deque<DataPackageSptr>::const_iterator ft =
				std::find_if( _failedPackages.begin(),      _failedPackages.end(),      std::bind2nd(searchProcessorExecuted(), packageId ) ) ;

		std::deque<DataPackageSptr>::const_iterator wt =
				std::find_if( _waitingAckPackages.begin(),  _waitingAckPackages.end(),  std::bind2nd(searchProcessorExecuted(), packageId ) ) ;

		completed = (
				dt != _distributedPackages.end() ||
				ft != _failedPackages.end()      ||
				wt != _waitingAckPackages.end()     );
	}

	return ( queued || running || completed );
}

bool PackageProcessingManager::isInCurrentProcessingList( DataPackageSptr const& package ) const{
	return this->isInCurrentProcessingList( package->getPackageId() );
}


void PackageProcessingManager::addPackagesToProcess( vector<DataPackageSptr> const& packages, vector<DataPackageSptr>& failedPackages) {

	this->addPackagesToRun<PackageProcessorFmtDist>( packages, failedPackages );
}


void PackageProcessingManager::addPackagesToCheck( vector<dbPersistent::IdType> const& packagesIds, vector<dbPersistent::IdType>& failedPackagesIds) {

	this->addPackagesToRun<PackageProcessorCheck>( packagesIds, failedPackagesIds );
}

void PackageProcessingManager::getPackages( vector<DataPackageSptr>& distributedPackages, 
											vector<DataPackageSptr>& failedPackages,
											vector<DataPackageSptr>& waitingAckPackages ) {

	ThreadSafe::Lock lock( _standbyCondition );

	//Copy datapackages (shared pointers are retained); deque to vector
	distributedPackages = vector<DataPackageSptr>(_distributedPackages.begin(), _distributedPackages.end());

	failedPackages = vector<DataPackageSptr>(_failedPackages.begin(), _failedPackages.end());

	waitingAckPackages = vector<DataPackageSptr>( _waitingAckPackages.begin(), _waitingAckPackages.end() );

	//Forget about them (shared pointers are released)
	_distributedPackages.clear();
	_failedPackages.clear();
	_waitingAckPackages.clear();

}

size_t PackageProcessingManager::getNumOfPendingRequests() const {
	ThreadSafe::Lock lock( _standbyCondition );
	return _queuedPackageProcessors.size();
}

//STL helper: is PackageProcessor running?
struct isRunning: protected func::unary_function<PackageProcessorSptr, bool> {
	bool operator()(PackageProcessorSptr& e) const {
		return ( e->running() );
	}
};

//STL helper: stop PackageProcessor
struct stopRunning: public func::unary_function<PackageProcessorSptr, void> {
	void operator()(PackageProcessorSptr& e) const {
		e->stop(false);
	}
};

bool PackageProcessingManager::endMyJobNow() {

	bool result(false);

	//End my job in case of:
	// - immediate stop request
	// - graceful stop request, but processors end.
	
	// Note: after my client sent me stop, it do not call addPackagesToProcess anymore.
	//  And even if would call it (but it is not the case), added packages will be processed. I will exit when no processing is running or queued.

	if        ( this->haltLazy() ) {
		//graceful stop ==> exit at the end of pending processing
		result = ( 0 == _runningPackageProcessors.size() + _queuedPackageProcessors.size() );

	} else if ( this->haltImmediate() ) {
		result = true;

	} else {
		result = false;
	}

	return result;
}

void PackageProcessingManager::run() {

	ACS_LOG_PRIVINFO("PackageProcessingManager: started.");

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
        ThreadSafe::Lock lock ( _standbyCondition) ;

		//Update my status -- change the boolean_predicate value
		this->updateFromPackageProcessorsStatus();

		//You can rest if there's nothing to do -- while (!(boolean_predicate)) {
		while ( (_queuedPackageProcessors.size()==0 || _emptyProcessingSlots==0 ) && 
				! this->endMyJobNow()                                                 ) {

			try {
				//Sleep and wake me up on _standbyCondition notification or on timer elapses.
				if (!hideWaitLog) { ACS_LOG_PRIVINFO( "Waiting. Data packages to be processed: "<<_queuedPackageProcessors.size()<<". Available Processing Slots: "<<_emptyProcessingSlots<<"." ); }
				hideWaitLog=false;
				//release mutex and block on condition
				if (false == _standbyCondition.timedwait(_pollingTimerMsec )) //-- wait on condition
                {
                    hideWaitLog=true; 
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Time check expired. Checking status. Data packages to be processed: "<<_queuedPackageProcessors.size()<<". Available Processing Slots: "<<_emptyProcessingSlots<<".");
                }
				//The mutex is locked by me here (as per timedwait implementation)

				ACS_LOG_PRIVINFO("PackageProcessingManager: Woken up.");

			} catch (exception& ex) {
				ACS_LOG_WARNING("PackageProcessingManager::run: Got exception in the wakeup condition. Try on next loop. Exception:" << ex.what() );
			}

			//The mutex is locked by me here (as per pthread_cond_wait ans timedwait)
			//Notification received OR No notification received for a while. Check the status
			this->updateFromPackageProcessorsStatus();
		}

		//-- do protected work. The mutex is locked by me here
		//Something to do at last...
		while ( _queuedPackageProcessors.size() > 0 && _emptyProcessingSlots > 0 && 
				! this->endMyJobNow()               								) {

			//Consume the queued package processors

			ACS_LOG_INFO("Setting up a new Package Processing for datapackage id ["<<_queuedPackageProcessors.front()->getDataPackage()->getPackageId()<<"].");

			//Run queued processor
			PackageProcessorSptr packageProcessorSptr = _queuedPackageProcessors.front();
			_runningPackageProcessors.push_back( packageProcessorSptr );
			packageProcessorSptr->start();

			_emptyProcessingSlots--;
			_queuedPackageProcessors.pop_front();

		}

		//-- unlock the condition automatically
	}

	//Wait until all the package processors end
	{
		size_t count(0);
		bool abortSent(false);
		while ( _runningPackageProcessors.end() !=
				find_if( _runningPackageProcessors.begin(), _runningPackageProcessors.end(), isRunning() ) ) {

			if ( count % 60 == 0 ) {
				ACS_LOG_INFO("Waiting for Package Processors completion.");
				count=0;
			}
			++count;
			Timer::delay( 1000 );

			//Check if the abort request has to be fulfilled
			HaltLevel haltLevel = this->getHaltLevel();
			if ( StopController::IMMEDIATE == haltLevel && !abortSent) {
				abortSent = true;
				ACS_LOG_INFO("Sending Abort to all the Pending Package Processors... ");
				for_each( _runningPackageProcessors.begin(), _runningPackageProcessors.end(), stopRunning() );
			}
		}

		//Update my status - removes also Package Processing calling the destructors (through shared pointers)
		this->updateFromPackageProcessorsStatus();

		ACS_LOG_INFO( "PackageProcessingManager: "<<(abortSent?"Aborted":"Stopped")<<" [" << threadId() << "]");
	}

}

bool PackageProcessingManager_priorityProcessor(PackageProcessorSptr p1, PackageProcessorSptr p2) {

	bool res(false);

	if ( p1->getDataPackage()->getPriority() == p2->getDataPackage()->getPriority() ) {
		res = ( p1->getDataPackage()->getDueDate() < p2->getDataPackage()->getDueDate() );
	} else {
		res = (acs::conf::DPConfigurerSingleton::instance()->getLowerPriorityFirst())? 
			p1->getDataPackage()->getPriority() < p2->getDataPackage()->getPriority()
			:
			p1->getDataPackage()->getPriority() > p2->getDataPackage()->getPriority();
	}

	return res;
}

void PackageProcessingManager::sortPackageProcessors() {

	ThreadSafe::Lock lock( _standbyCondition );
	
	sort( _queuedPackageProcessors.begin(), _queuedPackageProcessors.end(), PackageProcessingManager_priorityProcessor );
}
	

//for_each support class: remove a PackageProcessorSptr item from deque
struct RemovePackageProcessor {
	RemovePackageProcessor( deque<PackageProcessorSptr>* dq ): _dq(dq) {}
	void operator() (PackageProcessorConstSptr pp) {
		_dq->erase (	
			remove( _dq->begin(), _dq->end(), pp ),
			_dq->end() );
	}
	private: deque<PackageProcessorSptr>* _dq;
};


void PackageProcessingManager::updateFromPackageProcessorsStatus() {

	ThreadSafe::Lock lock( _standbyCondition );

	//Package Processors to be removed from the processing queue
	std::deque<PackageProcessorConstSptr> completedProcessors(0);

	//Check the status of each running package processor
	for ( std::deque<PackageProcessorSptr>::iterator it = _runningPackageProcessors.begin();
		  it != _runningPackageProcessors.end(); ++it ) {

		//(*it) notifies the end of run(), but it could be still in running status and setException could not be still called on it (cf. Thread::svc).
		if ( (*it)->isPerformed() ) {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Waiting for Performed PackageProcessor.");

			(*it)->waitExitOrTimeout( 5000 ); //Nominally will return immediatelly because not running
			//Now for sure the exception (in the case) is set on *it. 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Performed PackageProcessor ended.");
		}

		//Check exceptions on Handled Processor

		exception* ex(NULL);
		bool gotException = (*it)->runException(ex);

		//Get the processing status
		PackageProcessor::ProcStatus exitStatus( PackageProcessor::Undefined );
		bool completed = gotException || (*it)->getStatus(exitStatus);

       
		//Check the status
		if ( completed ) {

			//Remember the processor to remove
			completedProcessors.push_back( *it );

			//Get the datapackage
			DataPackageSptr dataPackageSptr( (*it)->getDataPackage() );

			//At this step the execution is completed
			//do update the clob (expProcInfo) on the database
			try {
				(*it)->updateParametersOnDb();

			} catch (exception& ex) {
				// If the controlling thread failed due to an internal exception do not scream
				// the package will go in error and the operation will be re-started later
				if (!gotException) {
					ACS_LOG_ERROR ("Unable to update the database info for the package " << dataPackageSptr->getPackageId() << ". Exception dump follows.");
					ACS_LOG_NOTIFY_EX (ex);
				}
			}

			
			if ( !gotException && exitStatus == PackageProcessor::Success ) {

				if ( (*it)->isWaitingAcknoledge() ) {

					//Remember also the parameter that will be used to check the datapackage
					_waitingAckPackages.push_back(dataPackageSptr);

					ACS_LOG_INFO("Data package id "<<dataPackageSptr->getPackageId()<<" is waiting acknowledge for distribution");

				} else {
					//Package processed and distributed
					_distributedPackages.push_back( dataPackageSptr );

					ACS_LOG_INFO("Data package id "<<dataPackageSptr->getPackageId()<<" successfully processed and distributed");
				}


			} else {
				//Some error during processing / distribution
				_failedPackages.push_back( dataPackageSptr );

				ostringstream msg; msg<<"Error Processing/Distributing Data package id "<<dataPackageSptr->getPackageId() << '\n';
				if ( 0 != ex ) { msg<<"Occurred Error: "<<ex->what() ; }
				ACS_LOG_WARNING(msg.str());
			}
		}
		if ( 0 != ex ) { delete ex;}
	}

	//Remove the completed (success and/or error) package processors from the processing queue
	for_each( completedProcessors.begin(), completedProcessors.end(), RemovePackageProcessor(&_runningPackageProcessors) );

	//Freed processing slots
	_emptyProcessingSlots += completedProcessors.size();

}

_ACS_END_NAMESPACE
