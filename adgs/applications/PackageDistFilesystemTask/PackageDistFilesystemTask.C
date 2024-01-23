/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageDistFilesystemTask$

	$Id$

	$Author$

	$Log$
	Revision 1.9  2018/02/20 14:02:28  chicas
	message to ACS_LOG_PRIVINFO
	
	Revision 1.8  2017/11/08 10:54:56  fabrizio.tringali
	added defaults for subsystem
	
	Revision 1.7  2017/05/02 10:42:44  tergem
	S2PDGS-1641 log harmonization
	
	Revision 1.6  2016/10/05 09:04:01  nicvac
	S2PDGS-1544: Log handling revised.
	
	Revision 1.5  2016/09/16 08:59:32  nicvac
	S2PDGS-1544: Log handling revision (PRIVINFO messages).
	
	Revision 1.4  2014/03/12 17:04:39  nicvac
	S2PDGS-735: app private log file generation disabled. Log is collected by Scheduler.
	
	Revision 1.3  2013/07/25 13:45:42  nicvac
	Test completed.
	
	Revision 1.2  2013/07/25 12:35:16  nicvac
	S2PDGS-428: implementation.
	
	Revision 1.1.1.1  2013/07/25 12:28:01  nicvac
	Import PackageDistFilesystemTask
	
        
*/ 

#include <PackageDistFilesystemTask.h>
#include "PackageDistFilesystemTaskV.h"

#include <Application.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <RegExp.h>
#include <File.h>
#include <Dir.h>
#include <Filterables.h>

using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageDistFilesystemTask)

static PackageDistFilesystemTaskV version;


#define MY_APP_NAME "PackageDistFilesystemTask"

#ifndef PACKAGEEXPORTER_SUBSYSTEM
#define PACKAGEEXPORTER_SUBSYSTEM "SDF"
#endif

#ifdef HAS_APPNAME_DEFINITIONS
PROJECT_APP::GenericBuilder<PackageDistFilesystemTask> theApplicationBuilder(MY_APP_NAME, PACKAGEEXPORTER_SUBSYSTEM);
#else
PROJECT_APP::GenericBuilder<PackageDistFilesystemTask> theApplicationBuilder(MY_APP_NAME, "DataCirculation");
#endif

PackageDistFilesystemTask::PackageDistFilesystemTask(const std::string& app, const std::string& subsys):
	PROJECT_APP(app, subsys),
	_expProcInfo(),
	_distribution(),
	_confProtocolRe()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageDistFilesystemTask::~PackageDistFilesystemTask() throw() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}



void PackageDistFilesystemTask::loadConf()  {

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

		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		conf.getValue( "PackageDistFilesystemTask.protocolRe", _confProtocolRe );

	} catch(exception &ex) {

		//exit with error.
		Application::setStatus(Application::Critical, "Initialization failed - cannot load configuration");
		exPackageDistFilesystemTaskCriticalException ex2(ex, "PackageDistCirculatorTask::loadConf Cannot load configuration");
		throwException(ex2);
	}

}

void PackageDistFilesystemTask::resolveAndLink(string const& src, string const& dest) const {

    string newsrc = src ;
	if ( File::isSymbolicLink(src, true) ) {
		newsrc = File::readlink(src, true); //Get the actal FileName
	}
    try {
        File::unlink(dest) ;
    } catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x);
    }
    
	File::hardlink(newsrc, dest);
}


int PackageDistFilesystemTask::execute(string const& expProcInfoFilename) {


	ACS_LOG_PRIVINFO(__FUNCTION__<<": loading Exporter IF "<<expProcInfoFilename);

	//Load ExpProcInfo
	_expProcInfo.load( expProcInfoFilename );

	Application::rotateLog();
	ACS_LOG_PRIVINFO(__FUNCTION__<<": loading Exporter IF "<<_expProcInfo.getDistributionFilename());
	//Load Distribution
	_distribution.load( _expProcInfo.getDistributionFilename() );

	//If the received datapackage dir to transfer is empty, the datapackage is not transferred 
	// and the task exit with Error. An empty datapackage can be caused by previous pachage processing.
	Dir dataPackageDir( _expProcInfo.getDatapackageAbsPath() );
	if ( dataPackageDir.size() == 0 ) {
		ACS_LOG_ERROR(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] is empty. Exiting with error.");
		//Return Error
		return 0xFF;
	}

	//Allowed protocols
	RegExp re; re.setReg( _confProtocolRe );
	if ( ! re.match( _distribution.getProtocol() ) ) {
		ACS_LOG_ERROR(__FUNCTION__<<": For Datapackage distribution ["<<_expProcInfo.getDatapackageAbsPath()<<"] the protocol ["<<_distribution.getProtocol()<<"] is not of filesystem type. Exiting with error.");
		//Return Error
		return 0xFF;
	}

	//Distribute each file / dir in datapackage.
	// If a single tranfer needed for the whole datapackage, it is expected the previous formatter task
	// created a single dir/file inside datapackage dir.
	ACS_LOG_INFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] contains ["<<dataPackageDir.size()<<"] items to be Hard Linked to "<<_distribution.getRemotepath());
	for ( size_t i(0); i<dataPackageDir.size(); ++i ) {

		this->resolveAndLink( _expProcInfo.getDatapackageAbsPath()+"/"+dataPackageDir[i],
							  _distribution.getRemotepath()+"/"+dataPackageDir[i] );

		ACS_LOG_PRIVINFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<", "<<dataPackageDir[i]<<"]. Hard Linked to "<<_distribution.getRemotepath());
	}


	_expProcInfo.setDistributionStatus(ExpProcInfoDistributionCompleted);

	ACS_LOG_PRIVINFO(__FUNCTION__<<": Local Distribution completed for processing: "<<expProcInfoFilename);

	//Dump the updated I/O interface with PackageProcessing
	ACS_LOG_PRIVINFO("PackageDistFileSystemTask::execute: updating "<<expProcInfoFilename);
	_expProcInfo.dump( expProcInfoFilename );

	return 0x00;

}


int PackageDistFilesystemTask::main(int argc, char const * const * argv, char const * const * env) {

	try {

		//Command Line printing - Useful for log inspection
		{
			ostringstream msg; msg<<__FUNCTION__<<": Executing: ";
			for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
			msg<<endl;
			ACS_LOG_PRIVINFO(msg.str());
		}

		const vector<string> &parsedArgs = getParsedArgs();

		ACS_COND_THROW( parsedArgs.size() < 1, exPackageDistFilesystemTaskCriticalException("Command line parameters missing. Expected: <IF export Filename>") );

		string expProcInfoFile(parsedArgs[0]);

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

		throwConditionedException( status!=0x00, exPackageDistFilesystemTaskCriticalException(string("Failed executing task on ")+expProcInfoFile));

		ACS_LOG_INFO(applicationName()<<": Exiting with SUCCESS.");

		return 0x00;

	} catch(exception& ex) {

		ACS_LOG_ERROR(applicationName()<<": Exiting with ERROR. Error message: "<<ex.what());

		return 0xFF;
	}
}

