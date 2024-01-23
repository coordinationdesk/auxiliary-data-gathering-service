// PRQA S 5600, 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV App $


*/


#include <SRVApp.h>
#include <StandaloneResourceSpaceFactory.h>
#include <dbDataFactory.h>
#include <dbConnectionPool.h>
#include <SMPluginLoader.h>
#include <dbAppResSet.h>
#include <SRVAppV.h>

#include <NameIdAssociator.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
	SRVAppV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVApp);

/*
 *  SRVApp definition
 */
SRVApp::SRVApp(const std::string& name, const std::string& subSystem) :
	Application(name, subSystem),	// PRQA S 4052
	_logFormatter(), 
    _syslog(),
    _logFilter()

{
	_logFormatter.emitPid(true);
	_logFormatter.emitTime(true);

	std::vector<exostream*> parents;
	parents.push_back(&excout);
	parents.push_back(&excerr);
	
	exMsgFmtFactory::instance()->appName(name) ;
	// create syslog handler.. will be re-created after the daemonize in the case
	createSyslogHandler() ;


	// Disable map files on disk for nameid associators
	NameIdAssociatorConfiguration::instance()->setEnableDiskMaps(false);
}

SRVApp::~SRVApp() ACS_NOEXCEPT {
}

int SRVApp::internalusage(const std::string &exename) const 
{
    int ret = Application::internalusage(exename) ;
	excerr << "\n\tSRVApp options:\n" ;
	excerr << "\t[--dbconf file]: uses a specific db configuration file.conf\n" ;
	excerr << "\n" ;
	return ret ;
}


int SRVApp::start(int argc, char const * const * argv, char const * const * env) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

	std::string dbConf;
	const std::vector<std::string>& args = getParsedArgs();

	size_t argsize = args.size();
	if (argsize > 0) {
		std::vector<std::string> newParsedArgs;

		const std::string dbConfKey = "--dbconf";

		// Loop over each argument
		for (size_t idx = 0; idx < argsize; ++idx) {	// PRQA S 4235, 4236
			const std::string inkey = args[idx];

			// db configuration arguments
			if ((dbConfKey == inkey) && (idx < argsize - 1)) {	// PRQA S 3084
				dbConf = args[++idx]; // i advanced here
			}

			else {
				// Parameter not recognized
				newParsedArgs.push_back(inkey);
			}
		}

		setParsedArgs(newParsedArgs);
	}

	dbAppResSet::setDefaultDbConfName(dbConf);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbconf:\"" << dbConf << "\"");

	// Insert here factories and handler
	dbAppResSetFactory::instance(dbConf);
	dbDataFactory dataFactory ;
	StandaloneResourceSpaceFactory resourcesFactory ;
    setDefaultConnectionPool(&db::ConnPools::instance()->getPool("")) ;


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to call Application::start method.");

	int res = -1;
	try {
		initPlugins() ;
		try {
			res = Application::start(argc, argv, env);
			justBeforeStartEnd();
		} catch (std::exception& e) {
			try {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Releasing connection pool");
				db::ConnPools::instance()->release();
			} catch (std::exception& e) {	// PRQA S 2502
				ACS_LOG_NOTIFY_EX(e);
			}

			throw;
		}

		try {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Releasing connection pool");
			db::ConnPools::instance()->release();
		} catch (std::exception& e) {
			ACS_LOG_NOTIFY_EX(e);
		}

		// exostream::getDefaultOutLogger.flush(); // Not needed at all since stdout should be disabled.
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
		// exostream::getDefaultOutLogger.flush(); // Not needed at all since stdout should be disabled.
		res = 255;
	} catch (...) {
		ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException("Unknown exception caught")) ; // PRQA S 3081
		// exostream::getDefaultOutLogger.flush(); // Not needed at all since stdout should be disabled.
		res = 255;
	}
	
	return res;
}

void SRVApp::initPlugins() 
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path: " << dbAppResSetFactory::instance(dbAppResSet::getDefaultDbConfName(),"")->getSMPluginPath()) ;

	SMPluginLoader::init(dbAppResSetFactory::instance(dbAppResSet::getDefaultDbConfName(),"")->getSMPluginPath()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "done") ;
}

void SRVApp::justBeforeStartEnd() {

}

void SRVApp::preDaemonize() {

}

void SRVApp::postDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	createSyslogHandler() ;
}

void SRVApp::createSyslogHandler() 
{
	_syslog.reset(new exSyslog("syslog")) ;
	_logFilter.reset(new exAdaptiveFilter(_syslog.get())) ;
	_logFilter->allowPrivateInfo(false) ;
	_logFilter->allowDebug(false) ;
	_logFilter->allowError(false) ;
	_logFilter->allowEvent(false) ;
	_logFilter->allowMessage(false) ;
	_logFilter->allowWarning(false) ;
	_logFilter->allowOpAlert(false) ;
	_logFilter->allowMCFilterable(true);
	_logFilter->allowAnyFilterable(false);
}


// Returns the default log size when rotation occur
std::streampos SRVApp::getDefaultRotationLogSize() const { return 1 << 20; } // 1 MegaByte // PRQA S 4400

// Returns the default log age when rotation occur
size_t SRVApp::getDefaultRotationLogAgeSec() const { return defaultLogAge; }

// Returns the default log num to save
unsigned int SRVApp::getDefaultRotationLogNumToSave() const { return 10; } // PRQA S 4400

// Returns the default log num to save
bool SRVApp::getDefaultRotationLogCompressOld() const { return true; }

// Returns if logs appends by default
bool SRVApp::getDefaultRotationAppend() const { return true; }

// Returns if logs appends by default
Application::LogRotationPolicy::Mode SRVApp::getDefaultRotationMode() const { return LogRotationPolicy::FirstApplying; }

_ACS_END_NAMESPACE
