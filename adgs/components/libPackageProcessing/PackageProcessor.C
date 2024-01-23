// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 1.54  2017/03/03 09:43:41  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 Condition Interface
	
	Revision 1.53  2017/02/28 17:07:42  nicvac
	S3EVOL-13: DatapackageSerializable include file removed
	
	Revision 1.52  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.51  2016/12/19 18:32:37  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.50  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.49  2016/09/07 08:30:53  nicvac
	CSGACQ-86: Oracle porting.
	
	Revision 1.48  2016/08/03 14:57:06  damdec
	CSGACS-94: handling of clobs changed.
	
	Revision 1.47  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.46  2016/01/29 11:17:38  marpas
	typos fixed
	
	Revision 1.45  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.44  2015/05/14 10:12:30  marfav
	S2PDGS-1220 using configurable spawned scheduler
	
	Revision 1.43  2014/04/18 15:41:03  nicvac
	S2PDGS-603: Scheduler log filtering.
	
	Revision 1.42  2014/04/17 17:10:48  nicvac
	S2PDGS-603: Scheduler log filtering.
	
	Revision 1.41  2013/11/12 17:49:13  nicvac
	S2PDGS-517: Cache inter-process.
	
	Revision 1.40  2013/11/06 18:19:22  nicvac
	Log messages improved.
	
	Revision 1.39  2013/11/06 18:01:49  nicvac
	Log messages improved.
	
	Revision 1.38  2013/11/06 13:07:02  nicvac
	Simplified Scheduler and processing status handling.
	
	Revision 1.37  2013/11/05 19:16:09  nicvac
	Log Messages improved.
	
	Revision 1.36  2013/10/31 11:34:56  marpas
	avoiding useless macros
	
	Revision 1.35  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.34  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.33  2013/05/23 13:48:44  nicvac
	S2PDGS-308: Notification Refactoring.
	
	Revision 1.32  2013/05/20 14:01:46  nicvac
	Threads synchronization improved.
	
	Revision 1.31  2013/03/21 14:44:28  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.30  2013/03/18 10:24:14  nicvac
	ADC-83: WaitingAcknowledge Step implemented. Test fixing.
	
	Revision 1.29  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.28  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.27  2013/03/07 17:54:03  nicvac
	SQLString for portability.
	
	Revision 1.26  2013/03/07 12:01:17  nicvac
	null values support.
	
	Revision 1.25  2013/03/05 17:41:05  nicvac
	Retry policy implementation.
	
	Revision 1.24  2013/03/05 14:07:18  nicvac
	rowCount is deprecated
	
	Revision 1.23  2013/02/28 18:49:26  nicvac
	Fixes after testing
	
	Revision 1.22  2013/02/22 17:38:22  nicvac
	S2PDGS-186: mail notification implemented.
	
	Revision 1.21  2013/02/07 16:03:35  nicvac
	load and dump for serializable objects.
	
	Revision 1.20  2013/02/01 17:39:57  nicvac
	Distribution serialized in xml
	
	Revision 1.19  2013/01/24 17:57:56  nicvac
	Datapackage dump enabled.
	
	Revision 1.18  2013/01/16 17:34:05  nicvac
	PackageProcessingManager waits on PackageProcessor only if this notifies.
	
	Revision 1.17  2013/01/09 14:14:03  nicvac
	Fixed getStatus
	
	Revision 1.16  2012/12/20 16:05:16  nicvac
	Remote path fixed.
	
	Revision 1.15  2012/12/14 18:20:55  nicvac
	Default destination url
	
	Revision 1.14  2012/12/12 16:19:55  nicvac
	ProcInfo, no JobOrder, setUp from Db.
	
	Revision 1.13  2012/12/07 18:05:51  nicvac
	working dir setup.
	
	Revision 1.12  2012/12/07 17:01:43  nicvac
	Condition and Threads stops.
	
	Revision 1.11  2012/12/06 18:35:17  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.10  2012/12/04 17:42:49  nicvac
	fix
	
	Revision 1.9  2012/11/30 14:16:32  nicvac
	Implementation.
	
	Revision 1.8  2012/11/27 14:44:17  nicvac
	Implementation.
	
	Revision 1.7  2012/11/23 17:35:45  nicvac
	Implementation.
	
	Revision 1.6  2012/11/22 18:06:09  nicvac
	Implementation.
	
	Revision 1.5  2012/11/21 17:45:47  nicvac
	Implementation.
	
	Revision 1.4  2012/11/21 11:31:30  nicvac
	Serialization.
	
	Revision 1.3  2012/11/20 18:06:24  nicvac
	DataPackage and Distribution serialization.
	
	Revision 1.2  2012/11/19 17:23:50  nicvac
	Implementing.
	
	Revision 1.1.1.1  2012/11/12 17:03:12  nicvac
	Import libPackageProcessing
	
        
*/ 

#include <PackageProcessor.h>

#include <Application.h>

#include <ExporterServiceResolver.h>

#include <PackageTaskTableManager.h>

#include <PackageEmailNotification.h>
#include <UserNotifier.h>

#include <ConfigSpaceDescriptor.h>

#include <DataProviderConfigurer.h>
#include <DataProvider.h>

#include <SpawnedScheduler.h>

#include <dbPersistentFactoryChain.h>
#include <dbFileAction.h>
#include <dbFileActionScopeGuard.h>
#include <dbFileActionFactoryChain.h>
#include <Status.h>
#include <dbSmartPersistent.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <File.h>

#include <Filterables.h>
#include <ftmpstream.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)
using namespace dataprovider;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageProcessor)


PackageProcessor::PackageProcessor():

	_availableSlotConditionPtr(NULL),
	_isPerformed(false),
	_dataPackageSptr(),
	_status(Undefined),
	_retryCurrent(0),
	_retryThreshold(0),
	_parameters(""),
	_workingDir(""),
	_processingSteps(0),
	_ttMap(),
	_currentExpProcInfo(),
	_fileTypeExpProcInfo(    "EXP_PROCINFO"),
	_fileTypeExpDatapackage( "EXP_DATAPACKINFO"),
	_fileTypeExpDistribution("EXP_DATAPACKDISTRO"),
	_fileTypeExpParameters(	 "EXP_PARAMETERS"),
	_procFileIF(),
	_isWaitingAcknoledge(false),
	_packageId(0)
{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);

	//Set values from configuration

	//Load from configuration the processing steps to be performed on the data package.
	//rsResourceSet& conf = ConfigurationSingleton::instance()->get();


}

PackageProcessor::~PackageProcessor() throw() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void PackageProcessor::halt(StopController::HaltLevel l, bool joinAlso) {
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	Thread::halt(l,joinAlso);
}


DataPackageSptr PackageProcessor::getDataPackage() const {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	return _dataPackageSptr;
}
	

void PackageProcessor::setAvailableSlotCondition( Condition* availableSlotConditionPtr ) {

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	_availableSlotConditionPtr = availableSlotConditionPtr;
}

bool PackageProcessor::getStatus(ProcStatus& code)  {

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	code = _status;

	//Return completed or not
	return ( _isPerformed && !this->running() );

}

bool PackageProcessor::isPerformed() const {
	return _isPerformed;
}

bool PackageProcessor::isWaitingAcknoledge() const {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

    return _isWaitingAcknoledge;
}


void PackageProcessor::deleteWorkingDir() const {
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	acs::File::unlink( _workingDir );
}


void PackageProcessor::handleNotification(bool const& success) const {

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, 
			  "_retryCurrent  : "<<_retryCurrent   <<'\n'
			<<"_retryThreshold: "<<_retryThreshold <<'\n'
			<<"success        : "<<success );

	//Send failure notification only last attempt
	if ( !success && (_retryCurrent < _retryThreshold) ) {

		ACS_LOG_WARNING("failure notification skipped. Package ["
				<<_dataPackageSptr->getPackageId()<<"] exporting will be retried. Current attempt = ["
				<<_retryCurrent<<"/"<<_retryThreshold<<"]");

		return;
	} 

	try {
		PackageEmailNotificationSptr notification( new PackageEmailNotification );
		string status = (success) ? Status::completed : Status::error ;

		notification->setDataPackage( *_dataPackageSptr ).setStatus( status ).setSuccess( success );

		//List of files to include in default mail - If empty the Notifier lists the
		// downloaded files (from dataPackage object)
		notification->setListOfFiles( _currentExpProcInfo.getNotificationListOfFiles() );

		string deliverySlip( _currentExpProcInfo.getNotificationDeliverySlipFilename() );

		//Save the delivery slip in cache - the working dir is going to be deleted
		if ( !deliverySlip.empty() && File::exists(deliverySlip) ) {
			CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();

			string dsFilename(  File::getFileName( deliverySlip ) );
			std::stringstream ss;
			ss << cacheSptr->getPath() << "/dlvSlp" << _dataPackageSptr->getPackageId();
			File::mkdir(ss.str());
			File::link( deliverySlip, 
						ss.str() + "/" + dsFilename,
						true );

			ss.str("");
			ss << "dlvSlp" << _dataPackageSptr->getPackageId() << "/" << dsFilename;
			cacheSptr->use( ss.str() );

			notification->setDeliverySlipFilename( 
							ss.str(), 
							true );

		}

		//Check if tasks asked to override the mail content
		string mailContentFile( _currentExpProcInfo.getNotificationContentFilename() );
		if ( !mailContentFile.empty() && File::exists( mailContentFile ) ) {
			notification->setPackageEmailContentFromFile( mailContentFile );
		}

		UserNotifier::instance()->sendAsynch( notification );

	} catch (exception& ex) {

		ACS_LOG_WARNING("PackageProcessor::handleNotification:"<<"[DP "<<_dataPackageSptr->getPackageId()<<"] Error sending notification: "<<ex.what());
	}
}


//*/Helper class to notify my manager I've terminated the run (because finished or exception)
class PackageProcessorNotifyEnd {
public:
	PackageProcessorNotifyEnd(Condition* c, bool& isPerformed):
		_c(c), _isPerformed(isPerformed) { }
	~PackageProcessorNotifyEnd() {
		ThreadSafe::Lock conditionLock(*_c);
		_isPerformed=true;
		_c->notify_all();
	}
private:
	PackageProcessorNotifyEnd(const PackageProcessorNotifyEnd & );
	PackageProcessorNotifyEnd &operator=(const PackageProcessorNotifyEnd &);
	Condition* _c;

	bool& _isPerformed;
};


// Returns the actual expProcInfo path
string PackageProcessor::getExpProcInfoPath() const
{
	ACS_COND_THROW ( ("" == _workingDir), exPackageProcessorException("Working dir still not defined. Please execute the processor first.") );
	string retval = _workingDir + "/expProcInfo.xml";
	return retval;
}

dbPersistent::IdType PackageProcessor::getPackageId() const
{
	ACS_COND_THROW ( (0 == _packageId), exPackageProcessorException("PackageId still not defined. Please execute the processor first.") );
	return _packageId;
}

void PackageProcessor::updateParametersOnDb () const
{
	dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
	dbConnection& conn = pw;
	static const size_t maxConnRetry = 3;

	// Gather the packageId. This also can throw if no packageId is still available
	dbPersistent::IdType packageId = this->getPackageId();

	//Dump the content of the current ExpProcInfo file in a string.
	// ExpProcInfo can be updated by Tasks running in the TaskTables. Its content is read after TaskTable execution.
	ostringstream os;
	_currentExpProcInfo.dump(os);

	//APF-376 update on demand the delserviceparam 
	size_t connRetry(0);
	bool updated(false);

	while ( !updated && connRetry < maxConnRetry ) {

		try {

        	dbSmartPersistent item ("t_distributionqueue", conn);
        	item.set ( "packageid", packageId, conn );
			oftmpstream clobstream ("/tmp/DistributionQueueDelServiceParam",".clob") ;
			clobstream() << os.str();
			clobstream.close(); // flush the buffer
			item.setFilePath("delserviceparam", clobstream.path(), conn) ;
        	item.update (conn);
			
			// if update is ok exit loop
			updated = true;

		} catch(exception& ex) {

			if ( connRetry==0 ) {
				// send a warning the first time a connection problem has occured
				ACS_LOG_WARNING("Problem connecting db");
				ACS_LOG_NOTIFY_EX(ex) ;
			}

			if ( connRetry == ( maxConnRetry - 1 ) ) {
				ostringstream msg;
				msg << "Unable to update the clob. Maximum number of db connection retries (" << maxConnRetry << ") performed" ;
				ACS_THROW ( exPackageProcessorException ( ex, msg.str() ) );
			}
			Timer::randomDelay ( 1000, 3000 );
			++connRetry;
		}
	}
}


void PackageProcessor::run() {

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called.");

	_packageId = _dataPackageSptr->getPackageId();
	dbPersistent::IdType dpid = _dataPackageSptr->getPackageId();

	//Helper notifier: when run finishes (nominally or exception), notify my manager some slot have been freed;
	PackageProcessorNotifyEnd packageProcessorNotifyEnd( _availableSlotConditionPtr, _isPerformed );

	ACS_LOG_INFO( "datapackage id=["<<_dataPackageSptr->getPackageId()<<"] started id=["<<this->threadId()<<"]" );

	//Set up distribution and task tables
	this->setUp();

	//Create and setup working dir
	this->setUpWorkingDir();

	//Set Processor interface Files: Filetype - Filename
	_procFileIF[ _fileTypeExpProcInfo ]   = getExpProcInfoPath(); // _workingDir +"/expProcInfo.xml";   //The ExpProcInfo file used as I/O interface between processors and this

	//Run sequentially a Task Table for each configured exporter step
	bool errorExport(false);
	for ( vector<string>::const_iterator stepPtr=_processingSteps.begin(); 
		  stepPtr!=_processingSteps.end() && !errorExport;
		  ++stepPtr ) {

		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Getting TaskTable for [step, package id]: ["<<*stepPtr<<", "<<_packageId<<"]");
		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Selected TaskTable: "<<_ttMap[*stepPtr]);

		//Dump the Exp Proc Info just once
		if ( ! File::exists( _procFileIF[ _fileTypeExpProcInfo ] ) ) {

			_currentExpProcInfo.dump( _procFileIF[ _fileTypeExpProcInfo ] );

		}

		//LOG handling: Cf. comment on PackageExporter.KeepSchedulerLog in the configuration space.
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		bool keepSchedulerLog = false;
		try { conf.getValue("PackageExporter.KeepSchedulerLog", keepSchedulerLog); } catch(exception& ex) {};
		std::stringstream ss;
		ss << "PackageProcessorLOG_DP" << dpid << "_" << *stepPtr;
		string woLogname(ss.str());
		string woLogFileName = keepSchedulerLog?
				File::tempname(  Application::defaultLogPath +"/"+woLogname+"_" )  :
				File::tempname( _workingDir                  +"/"+woLogname+"_" )  ;
		string woSchedulerLogName( woLogFileName+"_Scheduler" ); //Actually not used because using Threaded scheduler

		//Set up the Scheduler and process the Task Table
		scOrder wo;
		wo.setLogProcMessages( false );
		wo.setLogFileName( woLogFileName );
		wo.setTaskTable( _ttMap[*stepPtr] );
		wo.setJobOrder( _procFileIF[ _fileTypeExpProcInfo ] );
		wo.setBaseDir( _workingDir );
		wo.setSchedulerLogName( woSchedulerLogName ); //Actually not used because using Threaded scheduler
		wo.setTrackerFileName( _workingDir+"/TrkFile" ); 
		wo.doICreateDir(false);
		wo.doIKillBackgroundTasks(true);
		wo.doICleanupOnExit(false);

		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Starting Scheduler for step ["<<*stepPtr<<"]...");

		// Try to auto-detect the Scheduler executable name
		// if configured override it
		string schedulerPathName = File::getDirPath (Application::executableName()) + "/Scheduler";
		conf.getValue ("PackageProcessing.SchedulerPath", schedulerPathName, false);

		ACS_SMARTPTR<SpawnedScheduler> schedulerAPtr( new SpawnedScheduler (schedulerPathName, _workingDir+"/WorkOrder.xml" ) );
		schedulerAPtr->execute( wo );

		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Scheduler started for step ["<<*stepPtr<<"]");

		bool schedStopSent(false);
		SchedulerExitStatus exitStatus(ssUNDEFINED);

		// getExitStatus is true when scheduler ends.
		while ( !schedulerAPtr->getExitStatus(exitStatus) ) {

			// perform a check every 2 secs and exit immediately if scheduler exits
			schedulerAPtr->timedWaitExit(2000);

			if ( this->requestedCancel() && !schedStopSent) {
				ACS_LOG_WARNING("[DP "<<_packageId<<"] Quitting Scheduler for step ["<<*stepPtr<<"]...");
				schedulerAPtr->abortBySigTerm();
				schedStopSent = true;
			}
		}

		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Scheduler ended for step ["<<*stepPtr<<"] with exitStatus: ["<<int(exitStatus)<<", "<< schedulerExitStatus2String(exitStatus)<<"]" ); // PRQA S 3081

		errorExport = (exitStatus != ssOK);
	}

	//TTs sequence finished
	_status = errorExport? Error : Success ;

	//Processing may have reserved some new items in cache.
	//At the end of processing these additional cache items must be released.
	//These information are exchanged between Processors and System through ExpProcInfo 

	//Load ExpProcInfo
	_currentExpProcInfo.load( _procFileIF[ _fileTypeExpProcInfo ] );

	const bool distributed( _currentExpProcInfo.getDistributionStatus() == ExpProcInfoDistributionCompleted && !errorExport );
	const bool waitingAck ( _currentExpProcInfo.getDistributionStatus() == ExpProcInfoDistributionAccepted  && !errorExport );

	{
		string msgStatus = errorExport? "error":"success";
		ACS_LOG_INFO("[DP "<<_packageId<<"] Package Processor execution status: ["<< msgStatus
				<<boolalpha
				<<"]; Package distributed: ["<<distributed<<"]; Waiting distribution acknowledge: ["<<waitingAck<<"]"
				<<noboolalpha );
	}

	//Manage Email Notifications
	if ( !waitingAck ) {
		this->handleNotification(distributed);
	}

	//Check the ExpProcInfo
	if ( distributed ) {
		//The Package has been distributed

		//Release (eventually) the additional cache files downloaded by TaskTables' Task
		ACS_LOG_PRIVINFO("[DP "<<_packageId<<"] Downloaded "<<_currentExpProcInfo.getCacheItemsToRelease().size()<<" cache items by Tasks in Task Table. Releasing them.");
		CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();
		for ( vector<string>::const_iterator it = _currentExpProcInfo.getCacheItemsToRelease().begin();
			  it != _currentExpProcInfo.getCacheItemsToRelease().end();
			  ++it ) {
	
			try {
				//Inter-process cache mechanism: use the item in this process before releasing it.
				cacheSptr->use( *it );
				cacheSptr->release( *it );
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Released cache item got from Task Table: "<<*it );
			} catch( exception& ex ) {
				ACS_LOG_WARNING("[DP "<<_packageId<<"] Cannot release cache item ["<<*it<<"]. Item was downloaded by Tasks in Task Table. Exception: "<<ex.what() );
			}
		}

		//Delete Working dir along with the addition files created to support the processing
		try {
			rsResourceSet& conf = ConfigurationSingleton::instance()->get();
			bool cleanupWorkingDirOnSuccess(true);
			try { conf.getValue("PackageExporter.CleanupWorkingDirOnSuccess", cleanupWorkingDirOnSuccess); } catch(exception& ex) {};
			if (cleanupWorkingDirOnSuccess) {
				this->deleteWorkingDir();
			}

		} catch (exception& ex) {
			ACS_LOG_WARNING("[DP "<<_packageId<<"] Cannot delete working dir ["<<_workingDir<<"]. Exception: "<<ex.what() );
		}

	} else if ( waitingAck ) {
		//The Package distribution has been accepted, but it's in progress (Asynch distribution)
		//I.e. Fmt and Dist Tasktables processed ok and ask the system to check later the distribution status
		//These datapackages are set as WAITINGACK in DB.

		//The _procFileIF[ _fileTypeExpProcInfo ] will be written as clob in DB and PackageProcessorCheck
		//will be runned with that clob (ADC-83).

		_isWaitingAcknoledge = true;

	}


	//Notify this processing slot is free
	//Done by the packageProcessorNotifyEnd
    

}

_ACS_END_NAMESPACE
