/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageCheckCirculatorTask$

	$Id$

	$Author$

	$Log$
	Revision 1.20  2017/11/08 10:51:25  fabrizio.tringali
	added defaults for subsystem and appname
	
	Revision 1.19  2017/05/02 10:18:31  tergem
	S2PDGS-1641 log armonization
	
	Revision 1.18  2016/10/05 09:02:51  nicvac
	S2PDGS-1544: Log handling revised.
	
	Revision 1.17  2016/09/16 08:56:47  nicvac
	S2PDGS-1544: Log handling revision (PRIVINFO messages).
	
	Revision 1.16  2016/05/11 14:30:02  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.15  2014/03/12 16:49:53  nicvac
	S2PDGS-735: app private log file generation disabled. Log is collected by Scheduler.
	
	Revision 1.14  2013/10/04 14:08:35  nicvac
	acs sdk warnings processed.
	
	Revision 1.13  2013/09/13 07:32:47  chicas
	loadConfiguration notifyReplacement set to false
	
	Revision 1.12  2013/06/18 16:44:27  nicvac
	S2PDGS-405: Support for multiple item circulations in one datapackage.
	
	Revision 1.11  2013/05/03 15:25:39  nicvac
	_defaultDbKey init in load configuration.
	
	Revision 1.10  2013/04/22 11:04:21  marpas
	NameIdAssociator and related classes are now using Singleton template pattern
	
	Revision 1.9  2013/04/19 09:49:28  nicvac
	Soap Error message fixed
	
	Revision 1.8  2013/04/16 18:26:00  nicvac
	Error case improved
	
	Revision 1.7  2013/04/16 18:19:05  nicvac
	Soap Circulation If used.
	
	Revision 1.6  2013/04/15 17:23:08  nicvac
	Circulation interface changed.
	
	Revision 1.5  2013/03/28 14:35:31  nicvac
	Circulation DB from configuration.
	
	Revision 1.4  2013/03/21 11:16:27  nicvac
	ADC-83: WaitingAcknowledge Step: if some circulation in error then set the related ones to abort.
	
	Revision 1.3  2013/03/20 14:04:56  nicvac
	Synch case fixed.
	
	Revision 1.2  2013/03/15 19:44:44  nicvac
	ADC-83: WaitingAcknowledge Step implemented
	
	Revision 1.1.1.1  2013/03/15 19:34:05  nicvac
	Import PackageCheckCirculatorTask
	
        
*/ 

#include <PackageCheckCirculatorTask.h>
#include "PackageCheckCirculatorTaskV.h"


#include <Application.h>
#include <dbConnectionPool.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <Filterables.h>

#include <File.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageCheckCirculatorTask)

static PackageCheckCirculatorTaskV version;

#define MY_APP_NAME "PackageCheckCirculatorTask"

#ifndef PACKAGEEXPORTER_SUBSYSTEM
#define PACKAGEEXPORTER_SUBSYSTEM "SDF"
#endif

#ifndef PACKAGEEXPORTER_APPNAME
#define PACKAGEEXPORTER_APPNAME "PackageExporter"
#endif 

#ifdef HAS_APPNAME_DEFINITIONS
PROJECT_APP::GenericBuilder<PackageCheckCirculatorTask> theApplicationBuilder(MY_APP_NAME, PACKAGEEXPORTER_SUBSYSTEM);
#else
PROJECT_APP::GenericBuilder<PackageCheckCirculatorTask> theApplicationBuilder(MY_APP_NAME, "DataCirculation");
#endif

PackageCheckCirculatorTask::PackageCheckCirculatorTask(const std::string& app, const std::string& subsys):
	PROJECT_APP(app, subsys),
	PackageCirculatorTask()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageCheckCirculatorTask::~PackageCheckCirculatorTask() throw() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}

void PackageCheckCirculatorTask::customSigIntHandler() 
{
	ACS_LOG_INFO ("SIGINT received. Exiting...");
	_stop = true;
}

void PackageCheckCirculatorTask::customSigTermHandler() 
{
	ACS_LOG_INFO ("SIGINT received. Exiting...");
	_stop = true;
}


int PackageCheckCirculatorTask::execute(string const& expProcInfoFilename) {

	int returnValue(0x00);

	//Load ExpProcInfo
	_expProcInfo.load( expProcInfoFilename );

	//Check all the Circulations passed in the parameter file
	const vector<string>& circIds = _expProcInfo.getCirculationIds();

	bool allAreCompleted(false), someError(false), stillWorking(false);
	bool joinFinalStatus( !_isTransferAsynch );
	this->monitorCirculations( circIds, joinFinalStatus, allAreCompleted, someError, stillWorking );

	//Final checks
	if ( allAreCompleted ) {
		//Return Ok
		_expProcInfo.setDistributionStatus(ExpProcInfoDistributionCompleted);
		returnValue=0x00;

		ACS_LOG_INFO("PackageCheckCirculatorTask::execute: Circulation completed for processing: "<<expProcInfoFilename);

	} else if ( someError ) {

		this->abortCirculationsAndJoin( circIds );

		//Return Error
		returnValue=0xFF;

		ACS_LOG_ERROR("PackageCheckCirculatorTask::execute: Circulation Error for processing: "<<expProcInfoFilename);

	} else { //Asynch case 
	
		//Tell the Package Exporter the Transfer is still running and
		// ask a Check Acknowledge Processing later again
		_expProcInfo.setDistributionStatus(ExpProcInfoDistributionAccepted);
		returnValue=0x00;

		ACS_LOG_INFO("PackageCheckCirculatorTask::execute: Circulation Still Working for processing: "<<expProcInfoFilename);
	}


	//Dump the updated I/O interface with PackageProcessing
	ACS_LOG_PRIVINFO("PackageCheckCirculatorTask::execute: updating "<<expProcInfoFilename);
	_expProcInfo.dump( expProcInfoFilename );

	return returnValue;
}


void PackageCheckCirculatorTask::loadConf()  {

	try {
		// Create a static list of conf spaces
		vector<string> confFileToLoad(0);
		confFileToLoad.push_back (MY_APP_NAME);
		confFileToLoad.push_back ("PackageExporter");
		confFileToLoad.push_back ("PackageTask");

		for ( vector<string>::iterator elem= confFileToLoad.begin(); elem != confFileToLoad.end(); ++elem ) { // PRQA S 4238
			Application::instance()->addSpace(ConfigurationSingleton::ConfSpace( *elem )); // PRQA S 3081 

			ACS_LOG_INFO(MY_APP_NAME<<"::loadConf: loaded configuration space "<<*elem) ;
		}

		Application::instance()->loadConfiguration(false) ;
	} catch(exception &ex) {

		//exit with error.
		Application::setStatus(Application::Critical, "Initialization failed - cannot load configuration");
		exPackageCheckCirculatorTaskCriticalException ex2(ex, "PackageCheckCirculatorTask::loadConf Cannot load configuration");
		ACS_THROW(ex2);
	}

	//Init superclass conf;
	this->initFromConf();

	_defaultDbKey = getDefaultConnectionPool()->getKey();

}


int PackageCheckCirculatorTask::main(int argc, char const * const * argv, char const * const * env) {

	try {

		//Command Line printing - Useful for log inspection
		{
			ostringstream msg; msg<<"PackageCheckCirculatorTask::main: Executing: ";
			for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
			msg<<endl;
			ACS_LOG_INFO(msg.str());
		}

		const vector<string> &parsedArgs = getParsedArgs();
		ACS_COND_THROW( parsedArgs.size() < 3, exPackageCheckCirculatorTaskCriticalException("Command line parameters missing. Expected: <IF export Filename> <--asynch|--synch> <--soap|--db>") );

		string expProcInfoFile(parsedArgs[0]);
		string synchParam(parsedArgs[1]);
		string circIfParam(parsedArgs[2]);

		_isTransferAsynch = ( synchParam=="--asynch" );
		_useSoap = ( circIfParam=="--soap" );

		//No Log rotation. A new log file is created in the working dir.
		// Note: privinfo messages will be collected in the log file. Project App could filter them out.
		string logFileName = File::tempname( File::getDirPath( expProcInfoFile ) +"/"+ applicationName() +"_" );
		logFileName += ".log";

		LogRotationPolicy lrp(logFileName, LogRotationPolicy::FirstApplying, 0, 0, 1, false) ;
		Application::setLRPolicy (lrp);

		Application::instance()->setRotateLogParametersFromConf(false);
//		Application::rotateLog();

		ACS_LOG_INFO( "Detailed log messages written in ["<<logFileName<<"]" );
		//

		this->loadConf();

		Application::rotateLog();
		int status = this->execute(expProcInfoFile);

		ACS_COND_THROW( status!=0, exPackageCheckCirculatorTaskCriticalException(string("Failed executing task on ")+expProcInfoFile));

		ACS_LOG_INFO(applicationName()<<": Exiting with SUCCESS.");

		return 0x00;

	} catch(exception& ex) {

		ACS_LOG_INFO(applicationName()<<": Exiting with ERROR. Error message: "<<ex.what());

		return 0xFF;

	}  catch(...) {
		ACS_LOG_INFO(applicationName()<<": Exiting with ERROR. Unknown Exception catched.");
		return 0xFF;
	}
}

_ACS_END_NAMESPACE
