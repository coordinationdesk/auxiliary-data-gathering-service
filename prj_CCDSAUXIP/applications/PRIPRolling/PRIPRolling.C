// PRQA S 1050 EOF
/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    PRIPRolling

*/ 

#include <ApplicationLock.h>
#include <Transaction.h>
#include <PRIPRolling.h>
#include <dbConnection.h>
#include <exMacros.h>
#include <dbAppResSet.h>
#include <SMPluginLoader.h>
#include <StringUtils.h>
#include <EvictionTask.h>
#include <PRIPRollingConfigurer.h>
#include <priprollV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
	priprollV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(PRIPRolling);

PROJECT_APP::GenericBuilder<PRIPRolling> theApplicationBuilder("PRIPRolling", "PRIP"); // PRQA S 2311

/*
 * Default constructor
 */
PRIPRolling::PRIPRolling(const std::string& appname, const std::string& appsubsys) :
	PROJECT_APP(appname, appsubsys),	// PRQA S 4052
	_mainThread(pthread_self())
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/*
 * Destructor
 */
PRIPRolling::~PRIPRolling() ACS_NOEXCEPT { }

std::string PRIPRolling::getDefaultLogName() const {
	return StringUtils::pathJoin(defaultLogPath , File::getFileName(Application::executableName()) + ".log");
}

int PRIPRolling::main(int argc, char const * const * argv, char const * const * env) { // PRQA S 4020
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	int retValue = 0;

	//Lock the application.
	ApplicationLock appLock;
	appLock.lock(true);

	daemonize();
	
	// Reassign pid after daemonize
	_mainThread = pthread_self();

	ConfigurationSingleton* configSingl = 0;
	try {
		addSpace(ConfigurationSingleton::ConfSpace("PRIPRolling", "", false)); // PRQA S 3081
		loadConfiguration();
		configSingl = ConfigurationSingleton::instance();
	} catch (dbConnection::ConnectionException& e) {
		ACS_LOG_NOTIFY_EX(e);
		Application::setStatus(Application::Exiting);
		ACS_LOG_INFO("Application will restart in 30 sec.");
		
		size_t loops = 0;
		while (getCurrentStopMode() == NoStop || loops++ < 30) {
			sleep(1);
		}
		
		if (getCurrentStopMode() == ImmediateStop) {
			retValue = 255;
			Application::setStatus(Application::Exiting);
		} 
		else if (getCurrentStopMode() == CleanStop) {
			Application::setStatus(Application::Exiting);
		}
		else { // Phoenix or no stop
			doRestart();
		}
		return retValue;
	}
	
	Application::rotateLog();
	
	PRIPRollingConfigurer configurer(configSingl->get());
	configurer.show();
	
	wantToRestart(configurer.getRestartTime());
	
	dbConnectionPool& pool = *Application::instance()->getDefaultConnectionPool();
	
	size_t maxItems = configurer.getMaxItems();

	size_t totDbPoll = configurer.getDatabasePollingTime();
	size_t currDelay = totDbPoll;
				
	// Main loop
	EvictionTask evictionTask(pool, maxItems, configurer.getRemoveRecord());
	while (getCurrentStopMode() == NoStop) {
		Application::rotateLog();
		
		if (currDelay >= totDbPoll) {
			evictionTask.execTask(AppStopController());
			currDelay = 0;
		}
		
		Timer::delay(1000);
		currDelay += 1000;
	}
	
	// Immediate stop
	if (getCurrentStopMode() == ImmediateStop) {
		ACS_LOG_INFO ("SIGTERM received");
		retValue = 255;
	}
	
	// Clean stop
	else if (getCurrentStopMode() == CleanStop) {
		ACS_LOG_INFO ("SIGINT received");
	}
		
	// Phoenix restart
	else if (getCurrentStopMode() == PhoenixRestart) {
		ACS_LOG_INFO("Phoenix detected. Restarting...");
		doRestart();
	}

	if (Application::getCurrentStatus() != Application::Critical) {
		retValue = 255;
	}
	
	return retValue;

}

_ACS_END_NAMESPACE
