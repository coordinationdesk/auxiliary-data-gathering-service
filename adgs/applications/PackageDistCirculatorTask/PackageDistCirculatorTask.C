/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageDistCirculatorTask$

	$Id$

	$Author$

	$Log$
	Revision 1.34  2017/05/02 10:22:22  tergem
	S2PDGS-1641 log armonization
	
	Revision 1.33  2017/03/27 14:27:55  nicvac
	APF-273: mc messages refactoring.
	
	Revision 1.32  2017/03/27 13:37:49  nicvac
	APF-273: Dissemination mc messages refactoring.
	
	Revision 1.31  2017/03/24 17:08:14  nicvac
	APF-273: Dissemination MC.
	
	Revision 1.30  2016/10/05 09:03:20  nicvac
	S2PDGS-1544: Log handling revised.
	
	Revision 1.29  2016/09/16 08:54:57  nicvac
	S2PDGS-1544: Log handling revision (PRIVINFO messages).
	
	Revision 1.28  2016/05/11 14:31:55  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.27  2014/03/12 16:45:28  nicvac
	S2PDGS-735: app private log file generation disabled. Log is collected by Scheduler.
	
	Revision 1.26  2013/10/04 14:00:33  nicvac
	acs sdk warnings processed.
	
	Revision 1.25  2013/09/13 07:34:17  chicas
	loadConfiguration notifyReplacement set to false
	
	Revision 1.24  2013/06/18 16:40:44  nicvac
	S2PDGS-405: Support for multiple item circulations in one datapackage.
	
	Revision 1.23  2013/06/14 16:30:29  nicvac
	ADC-182: Temp transder strategy implemented.
	
	Revision 1.22  2013/05/17 15:34:00  nicvac
	S2PDGS-300: priority handling.
	
	Revision 1.21  2013/05/03 16:55:26  nicvac
	Check on Soap responce.
	
	Revision 1.20  2013/05/03 15:26:34  nicvac
	_defaultDbKey init in load configuration.
	
	Revision 1.19  2013/04/22 11:04:33  marpas
	NameIdAssociator and related classes are now using Singleton template pattern
	
	Revision 1.18  2013/04/19 09:50:14  nicvac
	Soap error message fixed
	
	Revision 1.17  2013/04/17 16:21:14  nicvac
	Log message with queued circulation id.
	
	Revision 1.16  2013/04/16 18:18:42  nicvac
	Soap Circulation if implemented.
	
	Revision 1.15  2013/04/15 17:21:51  nicvac
	Circulation interface changed.
	
	Revision 1.14  2013/04/11 17:25:30  nicvac
	From url settings changed: network source if destination is not filesystem
	
	Revision 1.13  2013/03/28 14:35:47  nicvac
	Circulation DB from configuration.
	
	Revision 1.12  2013/03/15 17:27:32  nicvac
	ADC-83: Synch and Asynch distribution.
	
	Revision 1.11  2013/03/15 16:27:22  nicvac
	ConnectionPool
	
	Revision 1.10  2013/03/11 17:34:27  nicvac
	ExpProcInfo supports more than one circulation id
	
	Revision 1.9  2013/02/28 18:50:00  nicvac
	Test
	
	Revision 1.8  2013/02/07 16:04:46  nicvac
	load and dump for serializable objects.
	
	Revision 1.7  2013/01/22 16:51:05  nicvac
	Exit fixed
	
	Revision 1.6  2013/01/18 16:45:51  nicvac
	Error if package is empty.
	
	Revision 1.5  2012/12/20 16:06:33  nicvac
	If one item in DP, circulate only it and not the whole DP.
	
	Revision 1.4  2012/12/14 17:00:58  nicvac
	Recoverable error stay in loop
	
	Revision 1.3  2012/12/12 16:31:41  nicvac
	No JobOrder, new Distribution.
	
	Revision 1.2  2012/11/27 14:45:48  nicvac
	Implementation.
	
	Revision 1.1.1.1  2012/11/27 11:44:37  nicvac
	Import PackageDistCirculatorTask
	
        
*/ 

#include <PackageDistCirculatorTask.h>
#include "PackageDistCirculatorTaskV.h"

#include <Application.h>
#include <dbConnectionPool.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <Dir.h>

#include <Filterables.h>

#include <MCFilterables.h>


using namespace std;

_ACS_BEGIN_NAMESPACE(acs)
using namespace dataprovider;
using namespace mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageDistCirculatorTask)

static PackageDistCirculatorTaskV version;

#define MY_APP_NAME "PackageDistCirculatorTask"

#ifndef PACKAGEEXPORTER_SUBSYSTEM
#define PACKAGEEXPORTER_SUBSYSTEM "SDF"
#endif

#ifdef HAS_APPNAME_DEFINITIONS
PROJECT_APP::GenericBuilder<PackageDistCirculatorTask> theApplicationBuilder(MY_APP_NAME, PACKAGEEXPORTER_SUBSYSTEM);
#else
PROJECT_APP::GenericBuilder<PackageDistCirculatorTask> theApplicationBuilder(MY_APP_NAME, "DataCirculation");
#endif

void PackageDistCirculatorTask::customSigTermHandler() {

	ACS_LOG_ERROR("PackageDistCirculatorTask: SIGTERM received - exiting...");

	_stop = true;
}

void PackageDistCirculatorTask::customSigIntHandler() {

	ACS_LOG_ERROR("PackageDistCirculatorTask: SIGINT received - exiting...");

	_stop = true;
}


PackageDistCirculatorTask::PackageDistCirculatorTask(const std::string& app, const std::string& subsys):
	PROJECT_APP(app, subsys),
	PackageCirculatorTask()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageDistCirculatorTask::~PackageDistCirculatorTask() throw() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}



int PackageDistCirculatorTask::execute(string const& expProcInfoFilename) {

	//Load ExpProcInfo
	_expProcInfo.load( expProcInfoFilename );

	//Load Distribution
	_distribution.load( _expProcInfo.getDistributionFilename() );

	//Load Datapackage
	_datapackage.load( _expProcInfo.getDatapackageFilename() );

	//If the received datapackage dir to transfer is empty, the datapackage is not transferred 
	// and the task exit with Error. An empty datapackage can be caused by previous pachage processing.
	Dir dataPackageDir( _expProcInfo.getDatapackageAbsPath() );
	if ( dataPackageDir.size() == 0 ) {

		ACS_LOG_ERROR("PackageDistCirculatorTask::execute: Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] is empty. Exiting with error.");
		//Return Error
		return 0xFF;
	}

	//Circulate each file / dir in datapackage.
	// If a single tranfer needed for the whole datapackage, it is expected the previous formatter task
	// created a single dir/file inside datapackage dir.
	ACS_LOG_PRIVINFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"]. Queuing Circulation requests for ["<<dataPackageDir.size()<<"] items.");
	for ( size_t i(0); i<dataPackageDir.size(); ++i ) {
		string circulationId = this->requestCirculation( dataPackageDir[i] );

		//Circulation entry set in DB - Update the PackageExport interface
		_expProcInfo.addCirculationId( circulationId );

		ACS_LOG_INFO("PackageDistCirculatorTask::execute: Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<", "<<dataPackageDir[i]<<"]. Queued Circulation request with id: "<<circulationId);

	    //APF 273
        ACS_LOG_MC_INFO( MCCode("DDP") <<
        				 mc_metric (mc::Metric_LINK) <<
        				 mc_param("packageid", _datapackage.getPackageId() ) <<
                         mc_param("queueid", circulationId)
        );

	}

	//If the transfer is asynch Do not monitor the Circulation 
	int returnValue(0x00);
	if ( _isTransferAsynch ) {

		//Tell the Package Exporter the Transfer has been accepted and 
		// ask a Check Acknowledge Processing later on
		_expProcInfo.setDistributionStatus(ExpProcInfoDistributionAccepted);
		returnValue=0x00;

	} else {
		//If it is synch ==> Monitor the transfer

		bool allAreCompleted(false), someError(false), stillWorking(false);
		this->monitorCirculations( _expProcInfo.getCirculationIds(),
								   true, 
								   allAreCompleted, someError, stillWorking );
		//stillWorking always false here (joinFinalStatus=true)
		
		//If Circulation error, exit immediately with error
        if ( allAreCompleted ) {
			//Return Ok
			_expProcInfo.setDistributionStatus(ExpProcInfoDistributionCompleted);
			returnValue=0x00;

			ACS_LOG_INFO("PackageDistCirculatorTask::execute: Circulation completed for processing: "<<expProcInfoFilename);
		} else {

			this->abortCirculationsAndJoin( _expProcInfo.getCirculationIds() );

			//Return Error
			returnValue=0xFF;

			ACS_LOG_ERROR("PackageDistCirculatorTask::execute: Circulation Error for processing: "<<expProcInfoFilename);
		}

	}

	/*/ Test for mail notification
	//Dummy Delivery Slip
	string dsFilename( _expProcInfo.getWorkingDir() + "/testDeliverySlip" );
	ofstream dsFile(dsFilename.c_str());
	dsFile << "Test delivery slip"<<endl<<"Files: "<<endl<<" - File1"<<endl<<" - File2"<<endl;
	dsFile.close(); 
	_expProcInfo.setNotificationDeliverySlipFilename( dsFilename );

	//Dummy List
	vector<string> listOfFiles; listOfFiles.push_back("File1"); listOfFiles.push_back("File2");
	_expProcInfo.setNotificationListOfFiles( listOfFiles );
	
	//Dummy mail content
	PackageEmailContent mail;
	mail.setSubject( "Test mail subject content" );
	mail.setBody("Test mail body content");
	mail.dump( _expProcInfo.getWorkingDir() + "/mailContent" );
	_expProcInfo.setNotificationContentFilename( _expProcInfo.getWorkingDir() + "/mailContent" );
	//*/

	//Dump the updated I/O interface with PackageProcessing
	ACS_LOG_PRIVINFO("updating "<<expProcInfoFilename);
	_expProcInfo.dump( expProcInfoFilename );

	return returnValue;

}

void PackageDistCirculatorTask::loadConf()  {

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
		exPackageDistCirculatorTaskCriticalException ex2(ex, "PackageDistCirculatorTask::loadConf Cannot load configuration");
		ACS_THROW(ex2);
	}

	//Init superclass conf;
	this->initFromConf();

	_defaultDbKey = getDefaultConnectionPool()->getKey();
}



int PackageDistCirculatorTask::main(int argc, char const * const * argv, char const * const * env) {

	try {

		//Command Line printing - Useful for log inspection
		{
			ostringstream msg; msg<<"PackageDistCirculatorTask::main: Executing: ";
			for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
			msg<<endl;
			ACS_LOG_PRIVINFO(msg.str());
		}

		const vector<string> &parsedArgs = getParsedArgs();
		ACS_COND_THROW( parsedArgs.size() < 3, exPackageDistCirculatorTaskCriticalException("Command line parameters missing. Expected: <IF export Filename> <--asynch|--synch> <--soap|--db>") );

		string expProcInfoFile(parsedArgs[0]);
		string synchParam(parsedArgs[1]);
		string circIfParam(parsedArgs[2]);

		_isTransferAsynch = ( synchParam=="--asynch" );
		_useSoap = ( circIfParam=="--soap" );

	    //APF 273
		MCParameters::instance()->setDefaultFunction ("Data Dissemination");
		MCParameters::instance()->setDefaultCode ("DD");

		//No Log rotation. A new log file is created in the working dir.
		// Note: privinfo messages will be collected in the log file. Project App could filter them out.
		string logFileName = File::tempname( File::getDirPath( expProcInfoFile ) +"/"+ applicationName() +"_" );
		logFileName += ".log";

		LogRotationPolicy lrp(logFileName, LogRotationPolicy::FirstApplying, 0, 0, 1, false) ;
		Application::setLRPolicy (lrp);

		Application::instance()->setRotateLogParametersFromConf(false);
//		Application::rotateLog();

		ACS_LOG_PRIVINFO( "Detailed log messages written in ["<<logFileName<<"]" );
		//

		this->loadConf();
		Application::rotateLog();

		int status = this->execute(expProcInfoFile);

		ACS_COND_THROW( status!=0, exPackageDistCirculatorTaskCriticalException(string("Failed executing task on ")+expProcInfoFile));

		ACS_LOG_INFO(applicationName()<<": Exiting with SUCCESS.");

		return 0x00;

	} catch(exception& ex) {

		ACS_LOG_ERROR(applicationName()<<": Exiting with ERROR. Error message: "<<ex.what());

		// APF-273
		ACS_LOG_MC_INFO(mc_param("packageid", _datapackage.getPackageId() ) <<
						mc_param("filename", "N/A" ) <<
						mc_param("message", string("Occurred error: ")+ex.what() ) <<
						mc_metric(Metric_EXP_FAIL)) ;

		return 0xFF;

	}  catch(...) {
		ACS_LOG_ERROR(applicationName()<<": Exiting with ERROR. Unknown Exception catched.");

		// APF-273
		ACS_LOG_MC_INFO(mc_param("packageid", _datapackage.getPackageId() ) <<
						mc_param("filename", "N/A" ) <<
						mc_param("message", "Unknown error") <<
						mc_metric(Metric_EXP_FAIL)) ;

		return 0xFF;
	}
}

_ACS_END_NAMESPACE
