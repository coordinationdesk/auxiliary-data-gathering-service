/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageDistCurlTask$

	$Id$

	$Author$

	$Log$
	Revision 1.16  2017/03/27 14:27:49  nicvac
	APF-273: mc messages refactoring.
	
	Revision 1.15  2017/03/27 13:37:44  nicvac
	APF-273: Dissemination mc messages refactoring.
	
	Revision 1.14  2017/03/24 17:08:08  nicvac
	APF-273: Dissemination MC.
	
	Revision 1.13  2016/10/05 09:03:37  nicvac
	S2PDGS-1544: Log handling revised.
	
	Revision 1.12  2016/09/15 15:19:30  nicvac
	S2PDGS-1544: Revised log handling.
	
	Revision 1.11  2016/03/23 14:53:49  nicvac
	PDSEV-29: aligned to last api curl. Fixed Dir case.
	
	Revision 1.10  2016/03/21 13:34:10  nicvac
	PDSEV-29: New FtpCurl put interface
	
	Revision 1.9  2016/03/21 11:30:23  nicvac
	PDSEV-29: Fixed api interface.
	
	Revision 1.8  2016/03/18 15:49:22  nicvac
	FtpCurl put new interface
	
	Revision 1.7  2016/03/18 15:42:34  nicvac
	PDSEV-29: implementation and test loop
	
	Revision 1.6  2016/03/18 13:16:33  nicvac
	Work in progress
	
	Revision 1.5  2016/03/17 14:47:13  nicvac
	PDSEV-29: libFtpCurl usage.
	
	Revision 1.4  2016/01/15 16:19:58  nicvac
	Remote path fixed
	
	Revision 1.3  2016/01/15 15:03:01  nicvac
	SNAV-41: test & fix loop.
	
	Revision 1.2  2016/01/15 14:14:16  nicvac
	SNAV-41: implementation
	
	Revision 1.1.1.1  2016/01/15 09:47:48  nicvac
	Import PackageDistCurlTask
	
        
*/ 

#include <PackageDistCurlTask.h>
#include "PackageDistCurlTaskV.h"
#include <Filterables.h>

#include <Application.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <RemoteSiteUtility.h>
#include <TemporaryFileService.h>
//#include <SpawnProc.h>

#include <CurlConfigurer.h>
#include <RemoteSiteFactory.h>
#include <RemoteSite.h>

#include <RegExp.h>
#include <File.h>
#include <Dir.h>
#include <Filterables.h>

#include <MCFilterables.h>

#include <fstream>
#include <streambuf>

#include <stdio.h>


using namespace std;

_ACS_BEGIN_NAMESPACE(acs)
using namespace mc;


ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageDistCurlTask)

static PackageDistCurlTaskV version;

#define MY_APP_NAME "PackageDistCurlTask"

#ifdef HAS_APPNAME_DEFINITIONS
PROJECT_APP::GenericBuilder<PackageDistCurlTask> theApplicationBuilder(MY_APP_NAME, PACKAGEEXPORTER_SUBSYSTEM);
#else
PROJECT_APP::GenericBuilder<PackageDistCurlTask> theApplicationBuilder(MY_APP_NAME, "DataCirculation");
#endif

PackageDistCurlTask::PackageDistCurlTask(const std::string& app, const std::string& subsys):
			PROJECT_APP(app, subsys),
			_expProcInfo(),
			_distribution()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

PackageDistCurlTask::~PackageDistCurlTask() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void PackageDistCurlTask::loadConf()  {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Call base class loadConfiguration.
	// This allows used libraries to download automatically their configuration
	// (for libraries implementing auto-conf download, like libFtpCurl).

	this->loadConfiguration();

}

void PackageDistCurlTask::doCurl( vector<string> const& relativePathLocals ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string datapackageAbsPath = StringUtils::pathNormalize( File::getAbsolutePath( _expProcInfo.getDatapackageAbsPath() ) );

	curl::RemoteSite remoteSite;

	/* _TEST_
	remoteSite.setProtocol( "sftp" );
	remoteSite.setHost( "sentinel-sl6" );
	remoteSite.setUser( "nicvac" );
	remoteSite.setPassword( "12qwas" );
	remoteSite.setIsPasswordEncrypted(false);
	remoteSite.setRenameStrategy( curl::temp::prefix, "." );
	*/
	remoteSite.setProtocol( _distribution.getProtocol() );
	remoteSite.setHost( _distribution.getHostname() );
	remoteSite.setUser( _distribution.getUsername() );
	remoteSite.setPassword( _distribution.getPassword() );
	remoteSite.setIsPasswordEncrypted(true);
	remoteSite.setRenameStrategy( curl::temp::TemporaryFileService::makeStrategy( _distribution.getTempStrategyName() ), _distribution.getTempStrategyParam() );

	curl::BandPriority no_priority ;
	FtpCurl ftpCurl( no_priority, remoteSite) ;

	ftpCurl.setLocalPath( datapackageAbsPath );

	ftpCurl.openConnection();

	/* _TEST_
	ftpCurl.setLocalPath( datapackageAbsPath );
	ftpCurl.setLocalFile(  "prova01.txt" );
    ftpCurl.setRempath("/tmp/") ;
    ftpCurl.setRemoteFile( "prova01.txt" );
	ftpCurl.put(false);

	ftpCurl.setLocalPath( datapackageAbsPath );
	ftpCurl.setLocalFile(  "prova02.txt" );
    ftpCurl.setRempath("/tmp/") ;
	ftpCurl.setRemoteFile( "prova02.txt" );
	ftpCurl.put(false);

	ftpCurl.setLocalPath( datapackageAbsPath );
	ftpCurl.putDir( datapackageAbsPath+"/provadir", "/tmp/provadir" );
	*/

	ACS_LOG_INFO( "Uploading ["<< relativePathLocals.size() <<"] items." );

	for ( vector<string>::const_iterator it=relativePathLocals.begin(); it!=relativePathLocals.end(); ++it ) {

		try {

			string localItemNameAbsPath = datapackageAbsPath+"/"+ *it;

			if ( File::isDir( localItemNameAbsPath ) ) {

				ACS_LOG_PRIVINFO( "Uploading Dir ["<< *it <<"]" );

				ftpCurl.setLocalPath( datapackageAbsPath );
				ftpCurl.putDir( localItemNameAbsPath, _distribution.getRemotepath()+"/"+*it );

			} else {

				ACS_LOG_PRIVINFO( "Uploading File ["<< *it <<"]" );
				ftpCurl.setLocalPath( datapackageAbsPath );
			    ftpCurl.setLocalFile( *it );
			    ftpCurl.setRempath( _distribution.getRemotepath() );
			    ftpCurl.setRemoteFile( *it );
			    ftpCurl.put( false );
			}

			// APF-273
			string mc_url = _distribution.getProtocol()+"://"+_distribution.getHostname()+"/"+_distribution.getRemotepath()+"/"+*it;
			size_t mc_size(0);
			if ( File::isDir( localItemNameAbsPath ) ) {
				Dir d( localItemNameAbsPath );
				mc_size = d.size();
			} else {
				mc_size = File::size(localItemNameAbsPath);
			}

			// APF-273
			ACS_LOG_MC_INFO(mc_param("packageid", _expProcInfo.getDatapackageContent().getPackageId() ) <<
							mc_param("filename", *it ) <<
							mc_param("tourl", mc_url ) <<
							mc_param("filesize", mc_size ) <<
							mc_metric(Metric_EXP_OUT)) ;

		} catch ( exception& ex ) {

			// APF-273
			ACS_LOG_MC_INFO(mc_param("packageid", _expProcInfo.getDatapackageContent().getPackageId() ) <<
							mc_param("filename", *it ) <<
							mc_param("message", string("Error uploading file: ")+ex.what() ) <<
							mc_metric(Metric_EXP_FAIL)) ;

			throw;
		}

	}

	ftpCurl.closeConnection();
}


int PackageDistCurlTask::execute(string const& expProcInfoFilename) {


	ACS_LOG_INFO(__FUNCTION__<<": loading Exporter IF ["<<expProcInfoFilename<<"]");

	Application::rotateLog();
	
	//Load ExpProcInfo
	_expProcInfo.load( expProcInfoFilename );

	ACS_LOG_INFO(__FUNCTION__<<": loading Exporter IF ["<<_expProcInfo.getDistributionFilename()<<"]");
	//Load Distribution
	_distribution.load( _expProcInfo.getDistributionFilename() );

	//If the received datapackage dir to transfer is empty, the datapackage is not transferred
	// and the task exit with Error. An empty datapackage can be caused by previous package processing.
	Dir dataPackageDir( _expProcInfo.getDatapackageAbsPath() );
	if ( dataPackageDir.size() == 0 ) {
		ACS_LOG_ERROR(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] is empty. Exiting with error.");
		//Return Error
		return 0xFF;
	}

	//Distribute each file / dir in datapackage.
	// If a single tranfer needed for the whole datapackage, it is expected the previous formatter task
	// created a single dir/file inside datapackage dir.

	string remotePath = _distribution.getProtocol()+"://"+_distribution.getHostname()+"/"+_distribution.getRemotepath()+"/";

	vector<string> srcs;
	ACS_LOG_INFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"]. Uploading scheduled to login ["<<_distribution.getUsername()<<"], remote path ["<<remotePath<<"]");
	for ( size_t i(0); i<dataPackageDir.size(); ++i ) {
		ACS_LOG_PRIVINFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<", "<<dataPackageDir[i]<<"]. Uploading scheduled to login ["<<_distribution.getUsername()<<"], remote path ["<<remotePath<<"]");
		srcs.push_back( dataPackageDir[i] );
	}

	this->doCurl( srcs );

	ACS_LOG_INFO(__FUNCTION__<<": Datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"] uploaded to login ["<<_distribution.getUsername()<<"], remote path ["<<remotePath<<"]");

	_expProcInfo.setDistributionStatus(ExpProcInfoDistributionCompleted);

	ACS_LOG_INFO(__FUNCTION__<<": Local Distribution completed for processing: ["<<expProcInfoFilename<<"]");

	//Dump the updated I/O interface with PackageProcessing
	ACS_LOG_INFO(__FUNCTION__<<": updating ["<<expProcInfoFilename<<"]");
	_expProcInfo.dump( expProcInfoFilename );

	return 0x00;
}


int PackageDistCurlTask::main(int argc, char const * const * argv, char const * const * env) {

	try {

		//Command Line printing - Useful for log inspection
		{
			ostringstream msg; msg<<__FUNCTION__<<": Executing: ";
			for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
			msg<<endl;
			ACS_LOG_INFO(msg.str());
		}

		const vector<string> &parsedArgs = getParsedArgs();

		ACS_COND_THROW( parsedArgs.size() < 1, exPackageDistCurlTaskCriticalException("Command line parameters missing. Expected: <IF export Filename>") );

		string expProcInfoFile(parsedArgs[0]);

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

		ACS_LOG_INFO( "Detailed log messages written in ["<<logFileName<<"]" );
		//

		this->loadConf();
		Application::rotateLog();

		int status = this->execute(expProcInfoFile);

		ACS_COND_THROW( status!=0x00, exPackageDistCurlTaskCriticalException(string("Failed executing task on [")+expProcInfoFile+"]"));

		ACS_LOG_INFO(applicationName()<<": Exiting with SUCCESS.");

		return 0x00;

	} catch(exception& ex) {

		ACS_LOG_ERROR(applicationName()<<": Exiting with ERROR. Error message: "<<ex.what() );

		// APF-273
		ACS_LOG_MC_INFO(mc_param("packageid", _expProcInfo.getDatapackageContent().getPackageId() ) <<
						mc_param("filename", "N/A" ) <<
						mc_param("message", string("Occurred error: ")+ex.what() ) <<
						mc_metric(Metric_EXP_FAIL)) ;


		return 0xFF;
	}
}


_ACS_END_NAMESPACE

