// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    SRV Notification Dispatcher main

*/ 

#include <SRVNotificationDispatcher.h>
#include <StopController.h>
#include <ApplicationLock.h>
#include <SRVDispatcherV.h>
#include <RestServerConfigurer.h>
#include <SRVNotifier.h>
#include <RestServerBase.h>
#include <SRVRestInterface.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace 
{ 
	// version management
	SRVDispatcherV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVNotificationDispatcher) 

namespace { // unnamed
	PROJECT_APP::GenericBuilder<SRVNotificationDispatcher> theApplicationBuilder("SRVNotificationDispatcher", "SRVQ");
}


SRVNotificationDispatcher::SRVNotificationDispatcher(const string& appname, const string& appsubs) :
	PROJECT_APP (appname, appsubs), // PRQA S 1021
	_restartTime (_defaultRestartTime)
{
}

SRVNotificationDispatcher::~SRVNotificationDispatcher() throw() 
{
}

/* void SRVNotificationDispatcher::customSigIntHandler()
{
	ACS_LOG_INFO ("SIGINT handler called in user space");
}

void SRVNotificationDispatcher::customSigTermHandler()
{
	ACS_LOG_INFO ("SIGTERM handler called in user space");
}
 */
int SRVNotificationDispatcher::usage(const std::string &) const
{
	excerr << "Use: \n" ;
	excerr << "\t" << applicationName() << "\n" ;
    
	excerr << "\n\tApplication options:\n" ;
	excerr << "\t__________________________________\n" ;

    return 1 ;
}


int SRVNotificationDispatcher::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{

	//Lock the application.
	ApplicationLock appLock;
	appLock.lock(true); //exception is raised if lock fails

	daemonize();

	// Is it OK, until now
	Application::setStatus (Application::Ok);


	//******************** Load Configuration and rotate log **************
	bool confOk = loadConf();
	Application::rotateLog();

	if ( ! confOk )
	{
		ACS_LOG_ERROR ("Unable to load the configuration. Exiting.");
  		Application::setStatus (Application::Critical, "Unable to start, configuration not available.");
  		return 1;
	}

	ACS_LOG_INFO ("Application restart timeout set to " << _restartTime << " seconds.");
	wantToRestart (_restartTime);

	try 
	{
		mainLoop() ; // loop and do nominal work
	}
	catch(exception &e)
	{
		ACS_LOG_ERROR("Exiting due to following exception: " << e) ;
  		Application::setStatus (Application::Critical, "Error due to exception");
		return 1 ;
	}

	if (mustRestart())
	{
		ACS_LOG_INFO ("Service is restarting");
	}
	else
	{
		ACS_LOG_INFO ("Service stop has been requested. Exiting.");
	}

	Application::setStatus (Application::Exiting);
	return 0;
}


void SRVNotificationDispatcher::mainLoop()
{
	// load configuration
	RestServerConfigurer config("/insertNotification", "SRVNotificationDispatcher");
	config.load();
 	// Create Rest Server
	RestServerBase theServer(config.host(), config.port());

	theServer.registerService(std::make_shared<SRVRestInterface>(config.url()));
	theServer.setMaxRequestSize(config.maxRequestSize());
	theServer.setMaxThreads(config.maxThreadNum());
	// start server
	theServer.startServer();

	// Create Notifier thread
	SRVNotifier theNotifier;
	// Start it
	theNotifier.start();
	
	
	// Do-nothing loop until something interesting happened
	bool exiting = false;
	while (( theServer.isServerOnline() || theNotifier.running()) /* || ( NoStop == getCurrentStopMode() ) */ )
	{
		Application::rotateLog() ; // do log rotation if needed
		if ( hasStopModeChanged() )
		{
			if ( ImmediateStop == getCurrentStopMode() )
			{
				ACS_LOG_INFO ("SIGTERM detected");
			}
			else if (CleanStop == getCurrentStopMode() )
			{
				ACS_LOG_INFO ("SIGINT detected");
			}
			else if (PhoenixRestart == getCurrentStopMode() )
			{
				ACS_LOG_INFO ("Entering restart phase");
			}
			
			exiting = true;
		}
		
		// Check if notifier or REST server are running. If at least one of them is not running start exiting
		if (!theNotifier.running() || !theServer.isServerOnline()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Notifier thread or REST server not running. Exiting.");
			exiting = true;
		}

		// Here we are exiting and the service is up. Must shut it down before quit.
		if (exiting) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stopping REST service and notifier thread.");
			theServer.stopServer();
			theNotifier.stop(false);
		}

		waitStopOrTimeout(100);
	}
	
	// Check for notifier thread errors
	std::shared_ptr<const std::exception> ex = theNotifier.runException();
	if (ex.get() != nullptr) {
		ACS_LOG_ERROR("Notification thread exited with the following error: " << ex->what());
	}
	
	if (PhoenixRestart == getCurrentStopMode())
	{
		// The application is restarting right now
		ACS_LOG_INFO ("PHOENIX TIME");
	}
	
 }

static const char* restartTimeKey = "SRVNotificationDispatcherConf.RestServer.RestartTime";

bool SRVNotificationDispatcher::loadConf() // PRQA S 4020
{
	try 
	{
		addSpace(ConfigurationSingleton::ConfSpace("SRVNotificationDispatcher"), ConfigurationSingleton::NoLoad) ; // PRQA S 3081 
		//addSpace(ConfigurationSingleton::ConfSpace("SRVNotificationServer"), ConfigurationSingleton::NoLoad) ; // PRQA S 3081 
		loadConfiguration(false) ;
	}
	catch(exception &e){
		// In case of exceptions return false
		ACS_LOG_NOTIFY_EX(e) ;
		return false ;
	}
	
	// Read the restart time for the phoenix (if set, otherwise there is the default)
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rs.getValue (restartTimeKey, _restartTime, false);
	
	return true ;
}



_ACS_END_NAMESPACE
