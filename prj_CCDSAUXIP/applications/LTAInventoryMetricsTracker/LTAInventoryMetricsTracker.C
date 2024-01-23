// PRQA S 1050 EOF
/*

    Copyright 1995-2021, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    LTAInventoryMetricsTracker app

*/ 

#include <ApplicationLock.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <exMacros.h>
#include <dbAppResSet.h>
#include <SMPluginLoader.h>
#include <ltainvmetV.h>
#include <csignal>

#include <LTAInventoryMetricsTracker.h>
#include <LTAInventoryMetricsTrackerConfig.h>
#include <InventoryTotalMetrics.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
	ltainvmetV vers;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAInventoryMetricsTracker);

PROJECT_APP::GenericBuilder<LTAInventoryMetricsTracker> theApplicationBuilder("LTAInventoryMetricsTracker", "LTA"); // PRQA S 2311

/*
 * Default constructor
 */
LTAInventoryMetricsTracker::LTAInventoryMetricsTracker(const std::string& appname, const std::string& appsubsys) :
	PROJECT_APP(appname, appsubsys),	// PRQA S 4052
	metrics_(),
	appInstance_(),
	waitInterval_(defaultWaitInterval),
	waitIntervalAfterTailReached_(defaultWaitIntervalAfterTailReached),
	mainThread_(pthread_self())
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/*
 * Destructor
 */
LTAInventoryMetricsTracker::~LTAInventoryMetricsTracker() ACS_NOEXCEPT { }

std::string LTAInventoryMetricsTracker::getDefaultLogName() const {
	return defaultLogPath + "/LTAInventoryMetricsTracker.log";
}

void LTAInventoryMetricsTracker::init(const LTAInventoryMetricsTrackerConfig& config) {
	ACS_LOG_INFO("Initializing from configuration.");
	appInstance_ = config.getInstance(); // throws exception if not exists
	
	bool found = false;
	
	std::string wd = config.getWorkingDir(&found);
	if (found) {
		if (!File::exists(wd)) {
			File::mkdir(wd);
		}
		metrics_.setWorkingDir(wd);
	}
	
	size_t sval = 0;
	unsigned long lval = 0;
	sval = config.getQueryBlockSize(&found);
	if (found) {
		if (sval == 0) {
			ACS_LOG_WARNING("Query block size configured with invalid value 0. Default will be used.");
		}
		metrics_.setBlockSize(sval);
	}
	
	lval = config.getWaitInterval(&found);
	if (found) {
		if (lval == 0) {
			ACS_LOG_WARNING("Wait interval configured with invalid value 0. Default will be used.");
		}
		waitInterval_ = lval;
	}
	
	lval = config.getInventoryTailReachedWaitInterval(&found);
	if (found) {
		if (lval == 0) {
			ACS_LOG_WARNING("Inventory tail wait interval configured with invalid value 0. Default will be used.");
		}
		waitIntervalAfterTailReached_ = lval;
	}
}

/*
 * This is the main function
 */
int LTAInventoryMetricsTracker::main(int argc, char const * const * argv, char const * const * env) { // PRQA S 4020
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	int retValue = 0;

	//Lock the application.
	ApplicationLock appLock;
	appLock.lock(true);

	// no deamonize required

	try {
		addSpace(ConfigurationSingleton::ConfSpace("LTAInventoryMetricsTracker", "", false)); // PRQA S 3081
		loadConfiguration();
	} catch (dbConnection::ConnectionException& e) {
		ACS_LOG_NOTIFY_EX(e);
		Application::setStatus(Application::Exiting);
		ACS_LOG_INFO("Application will restart in 30 sec.");
		
		waitStopOrTimeout(30000);
		
		if (getCurrentStopMode() == ImmediateStop) {
			Application::setStatus(Application::Exiting);
		} 
		else if (getCurrentStopMode() == CleanStop) {
			Application::setStatus(Application::Exiting);
		}

		return retValue;
	}

	Application::rotateLog();

	// load configuration
	LTAInventoryMetricsTrackerConfig config(ConfigurationSingleton::instance()->get());
	config.show();

	if (config.isRestartEnabled()) {
		wantToRestart(config.getRestartTime());
	}
	
	// Initialize
	init(config);
	
	ACS_LOG_INFO("Application successfully started.");
	
	dbConnectionPool& pool = *Application::instance()->getDefaultConnectionPool();
	while (getCurrentStopMode() == NoStop) {
		Application::rotateLog();

		if (hasStopModeChanged()) {
			// Immediate stop
			if (getCurrentStopMode() == ImmediateStop) {
				Application::setStatus(Application::Exiting);
				ACS_LOG_INFO ("SIGTERM received");
			}
			
			// Clean stop
			else if (getCurrentStopMode() == CleanStop) {
				Application::setStatus(Application::Exiting);
				ACS_LOG_INFO ("SIGINT received");
			}
			
			// Phoenix restart
			else if (getCurrentStopMode() == PhoenixRestart) {
				ACS_LOG_INFO("Phoenix detected. Restarting...");
			}
		}
		
		// Dispatch new orders only without any stop request
		if (getCurrentStopMode() == NoStop) {
			try {
				dbConnectionWrapper cw(pool);
				dbConnection& conn(cw);

				// start total inventory process here
				dbPersistent::IdType lastId = metrics_.loadLastInventoryId("LTAInventoryMetricsTracker", appInstance_, conn);
				size_t processedEntries = metrics_.processNextInventoryBlock("LTAInventoryMetricsTracker", appInstance_, lastId, conn);
				
				bool stopped = false;
				if (processedEntries < metrics_.getBlockSize()) {
					if (processedEntries > 0) {
						ACS_LOG_INFO("Reached the end of the inventory table. Waiting " << std::to_string(waitIntervalAfterTailReached_) << " seconds...");
					}
					// Reached the end of the inventory table
					stopped = waitStopOrTimeout(waitIntervalAfterTailReached_ * 1000);
				}
				else {
					// There are still records to be processed in the inventory table
					ACS_LOG_INFO("There are still record to be processed in the inventory table. Waiting " << std::to_string(waitInterval_) << " seconds...");
					stopped = waitStopOrTimeout(waitInterval_ * 1000);
				}
				
				if (stopped) {
					ACS_LOG_INFO("Application stopped as requested.");
				}
			} catch (dbConnection::ConnectionException& e) {
				ACS_LOG_NOTIFY_EX(e);
				Application::setStatus (Application::Exiting);
				retValue = 255;
				break;
			} catch (std::exception& e) {
				ACS_LOG_NOTIFY_EX(e);
				ACS_LOG_ERROR("Urecoverable error. Exiting.");
				Application::setStatus (Application::Critical);
				retValue = 255;
				break;
			}
		}
	}
	
	// Phoenix restart
	if (getCurrentStopMode() == PhoenixRestart) {
		ACS_LOG_INFO("Phoenix now.");
	}

	ACS_LOG_PRIVINFO("Exiting with exit value " << retValue);
	return retValue;
}

_ACS_END_NAMESPACE

