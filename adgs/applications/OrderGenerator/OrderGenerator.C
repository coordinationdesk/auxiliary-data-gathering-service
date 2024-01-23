// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SpA

	$Prod: A.C.S. OrderGenerator $

	$Id$

	$Author$

	$Log$
	Revision 5.10  2017/05/02 10:17:11  tergem
	S2PDGS-1641 log armonization
	
	Revision 5.9  2017/02/06 16:20:36  marfav
	CSGACQ-112
	Perform periodically the waiting order cleanup to
	purge expired records
	
	Revision 5.8  2016/07/06 15:27:17  marfav
	CSGACQ-84
	New project/application based log rotation management strategy enforced
	
	Revision 5.7  2016/05/11 13:48:56  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.6  2016/04/11 12:32:34  marpas
	useless headers removed
	
	Revision 5.5  2016/03/21 10:10:02  marpas
	adapting to last dev environment.
	now relies on HAS_MISSION_INCLUDE
	
	Revision 5.4  2014/09/19 12:11:05  marfav
	Added support to configurable restart time
	
	Revision 5.3  2013/10/11 09:45:42  chicas
	 removed AuxDownloaderDefaultFactory.h include for HAS_MISSION_INCLUDE cases
	
	Revision 5.2  2013/06/20 14:19:00  marpas
	fixing stream chains
	
	Revision 5.1  2013/06/20 07:55:03  marpas
	stream chaining fixed
	
	Revision 5.0  2013/06/19 14:33:35  marpas
	adoption of libException 5.x standards
	
	Revision 2.29  2013/05/12 14:39:44  marpas
	no longer master/slave check
	
	Revision 2.28  2013/03/14 12:36:59  marpas
	connection pool concept moved in classes derived from Application
	
	Revision 2.27  2013/02/19 16:25:12  marpas
	configuration reloading fixed
	uses PluginLoaderNotifier through derived class AppPluginLoaderNotifier to detect broken links
	
	Revision 2.26  2013/02/13 18:23:25  marpas
	Connection Pool concept adopted
	application flow completely rewritten
	some methods moved into libOrderCreator (new class OrderExecutor)
	coding best practices applied
	qa rules
	
	Revision 2.25  2013/02/05 11:29:22  danalt
	status file name aligned as per OME SPR CMC Integration for Resource Monitoring
	
	Revision 2.24  2013/01/11 13:32:12  marfav
	Addedd support to external appname definition
	
	Revision 2.23  2012/05/07 11:04:47  danalt
	*** empty log message ***
	
	Revision 2.22  2012/03/16 08:26:41  marpas
	alignement for std app
	
	Revision 2.21  2012/03/01 13:06:58  marpas
	base class deal with -dbconf
	
	Revision 2.20  2012/02/09 17:32:14  marpas
	refactogring in progress
	
	Revision 2.19  2010/06/24 10:27:25  marpas
	improved message when non factory is found (sat added)
	
	Revision 2.18  2010/06/23 14:15:36  marpas
	SatelliteName::nullSatName() invoked
	
	Revision 2.17  2010/06/22 17:00:50  marpas
	satId set to -1 in case of null satellite
	
	Revision 2.16  2010/03/09 10:24:39  enrcar
	EC:: Oracle DB compatibility added
	
	Revision 2.15  2009/07/01 17:03:55  marpas
	should keep trying to establish a connection with the db without exiting
	
	Revision 2.14  2009/06/05 11:54:45  marpas
	new message for no plugin to load
	conf space for Cnm updated
	
	Revision 2.13  2009/02/19 11:52:04  marpas
	minor changes
	
	Revision 2.12  2008/12/02 11:25:21  marpas
	bProc not longer directly referenced
	
	Revision 2.11  2008/11/24 11:28:05  marpas
	log messages improved
	
	Revision 2.10  2008/11/21 18:56:00  marpas
	ConfigurationSingleton new mode added
	
	Revision 2.9  2008/11/05 12:53:34  marpas
	work in progress
	
	Revision 2.8  2008/10/07 12:46:25  crivig
	porting to postgres8
	
	Revision 2.7  2008/09/22 09:08:51  danalt
	added HAS_MISSION includes
	
	Revision 2.6  2007/04/20 10:31:58  ivafam
	Patch to remove libProc dependence of libGfeSatStore
	
	Revision 2.5  2007/04/18 14:00:56  marpas
	arguments improved, now supports -simul -help
	
	Revision 2.4  2007/04/11 15:05:59  clanas
	implemented Orbit propagator with Db separation
	
	Revision 2.3  2007/03/30 13:40:14  danalt
	added ConfigurationSingleton instance to save Configuration Spaces loaded by CosmoApp
	
	Revision 2.2  2007/01/10 11:44:42  giucas
	Updated for new CFI 3.5 integration
	
	Revision 2.1  2006/06/09 08:40:39  giucas
	Because plugin, the very first loadConfiguration is invoked BEFORE the daemonize call. This hangs the process
	if no master database is found. So, the very first loadConfiguration should be smoother as for the SDFArbiter.
	
	Revision 2.0  2006/02/28 15:49:11  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.43  2006/02/09 14:13:18  giucas
	semicolon removed from SQL statement
	
	Revision 1.42  2005/11/24 18:03:58  giapas
	Fixed to compile in CompactStation environment.
	
	Revision 1.41  2005/10/24 18:23:23  marpas
	dbexpress compatibility
	
	Revision 1.40  2005/10/10 15:26:59  marfav
	StationConf space added
	
	Revision 1.39  2005/10/06 15:43:25  marfav
	Plugin support added
	Orders creator moved to specialized libraries
	
	Revision 1.38  2005/09/14 09:33:51  marpas
	special debug to cause the application exiting with an exception
	
	Revision 1.37  2005/08/05 15:33:09  marpas
	New statistics used. Now COSMO can use them
	
	Revision 1.36  2005/08/01 11:04:11  marfav
	allowFinalStatistics is not used in COSMO application environment
	
	Revision 1.35  2005/06/22 09:28:17  enrcar
	namespace std handling added
	
	Revision 1.34  2005/06/21 12:25:52  marpas
	GroupedCollection algorithm implemented
	
	Revision 1.33  2005/06/13 16:25:48  marpas
	capabilities report improved
	
	Revision 1.32  2005/06/13 08:40:16  marpas
	COSMO now uses ACS converters libraries
	
	Revision 1.31  2005/05/31 11:55:48  marpas
	-dbconf argument supported
	MergeConfiguration predicate introduced
	some log messages improved
	
	Revision 1.30  2005/04/19 12:02:36  marpas
	Header modified
	
	Revision 1.29  2005/04/04 14:13:39  ivafam
	Commented add Patrol
	
	Revision 1.28  2005/04/01 08:32:34  marpas
	Summary messages sent to MCF, conf files updated
	
	Revision 1.27  2004/10/07 19:21:02  marpas
	new EXTERNAL algo for RPRO orders under develop
	
	Revision 1.26  2004/10/07 19:03:45  marpas
	new files for external RPRO orders NRT like
	
	Revision 1.25  2004/09/07 14:09:58  marpas
	Makefile.Station removed
	supported projects Cosmo, Cryosat, Envisat
	
	Revision 1.24  2004/09/06 09:10:20  danalt
	Cosmo MultiMission Environment Support Added
	
	Revision 1.23  2004/07/13 09:24:52  marpas
	capabilities string changed
	checking enforced
	
	Revision 1.22  2004/07/05 14:45:29  marpas
	No Patrol in Station Mode
	
	Revision 1.21  2004/04/08 16:31:58  paoscu
	StationApp supported
	
	Revision 1.20  2004/03/23 08:59:18  marpas
	Orbit wrapper not initialized nor linked
	
	Revision 1.19  2004/03/22 17:03:58  marpas
	L2Anx2Anx added
	satellite id management added
	
	Revision 1.18  2004/03/08 23:31:00  marpas
	Simulation mode introduced
	
	Revision 1.17  2003/11/25 13:12:52  marpas
	New initialization policy for explorer wrapper ...
	and new ConfigurationSpace added
	
	Revision 1.16  2003/08/01 12:41:32  marpas
	messages upgraded
	
	Revision 1.15  2003/08/01 11:47:32  marpas
	restart implemented
	reload configuration implemented
	-cap argument to know order processing capabilities
	
	Revision 1.14  2003/07/30 20:08:45  marpas
	DEBUG improvements
	MASTER/SLAVE check improved during processing.
	
	Revision 1.13  2003/07/17 22:42:23  marpas
	Optimization flag overridden
	compilation warning removed
	
	Revision 1.12  2003/07/17 15:15:21  marpas
	configuration spaces fixed
	Thread::runException i/f changed
	
	Revision 1.11  2003/07/14 19:45:16  marpas
	hostname mehtod moved in Application class
	
	Revision 1.10  2003/07/14 17:13:58  marpas
	loading of local configuration space added
	
	Revision 1.9  2003/07/14 13:59:45  marpas
	log timeout changed
	
	Revision 1.8  2003/07/12 02:14:17  marpas
	Log rotation policy implemented, Patrol space loaded
	
	Revision 1.7  2003/06/10 08:10:13  marpas
	new OfflineLevel2OrderCreator class
	master/slave policy implemented
	
	Revision 1.6  2003/06/04 19:33:53  marpas
	V1 test.
	
	Revision 1.5  2003/03/13 17:32:01  marpas
	some improvements
	
	Revision 1.4  2003/02/20 08:48:41  marpas
	NRT test
	
	Revision 1.3  2003/02/18 16:57:23  marpas
	test in progress
	
	Revision 1.2  2003/02/07 10:36:55  marpas
	work in progress
	
	Revision 1.1.1.1  2003/02/04 18:47:30  marpas
	Import Order Generator
	

*/



#include <OrderGenerator.h>
#include <OrderExecutor.h>
#include <OGDBSimulationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <dbConnectionPool.h>
#include <DirPluginSet.h>
#include <Plugin.h>
#include <OGListener.h>
#include <ConfigurationSingleton.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <OrderCreatorFactoryChain.h>

#include <ProjectPropagatorIncludes>
#include <ProjectTimeConverterIncludes>

#include <ApplicationLock.h>
#include <StringUtils.h>

// factories
#include <OfflineCreatorFactory.h>
#include <NRTCreatorFactory.h>
#include <RPROCreatorFactory.h>



#include <Timer.h>
#include <Filterables.h>

#include <sstream>
#include <vector>


#include <ogV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace { // unnamed
    // version management
    ogV version ;
}

// Root key for configuration spaces navigation
const std::string OrderGenerator::_ordGenRootKey = "OrderGeneration";

// Configuration spaces keys for values 
const std::string OrderGenerator::_pluginsPathKey = "PluginsPath";
const std::string OrderGenerator::_pluginRegExp   = "\\.so$";

class StopPred: public ConfigurationSingleton::StopPredicate { // PRQA S 2109, 2153
public:
    StopPred() : StopPredicate() {} // PRQA S 2528
    
    virtual ~StopPred() throw () {} // PRQA S 2131
    
    StopPred(const StopPred &r) : 
      ConfigurationSingleton::StopPredicate(r)
    {}
    
    virtual bool evalCondition() const throw () // PRQA S 2131 
    {
        return AppStopController().requestedCancel() ;
    }
private:
    StopPred & operator= (const StopPred &r) ; // not implemented because the reference 
};


ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderGenerator) 


// PRQA S 1020 END_APP_MACRO
#ifndef ORDERGENERATOR_SUBSYSTEM
#define ORDERGENERATOR_SUBSYSTEM "SDF"
#endif

#ifndef ORDERGENERATOR_APPNAME
#define ORDERGENERATOR_APPNAME "OrderGenerator"
#endif 

#ifdef HAS_APPNAME_DEFINITIONS
#define APPNAME ORDERGENERATOR_APPNAME
#define APPSUBS ORDERGENERATOR_SUBSYSTEM
//Sentinel1 temporary patch
#elif defined DEFAULT_ACS_STATUS_PATH 
#warning MP: temporary became definitive ???
#define APPNAME "NRTAPOrderGenerator"
#define APPSUBS "ACQ"
#else
#define APPNAME "OrderGenerator"
#define APPSUBS "SDF"
#endif
// PRQA L:END_APP_MACRO

namespace { // unnamed

	PROJECT_APP::GenericBuilder<OrderGenerator> theApplicationBuilder(APPNAME, APPSUBS); // PRQA S 1021

    // instantiate the order factories
    OfflineCreatorFactory theOfflineCreatorFactory ;
    NRTCreatorFactory theNRTCreatorFactory ;
    RPROCreatorFactory theRPROCreatorFactory ;
}


OrderGenerator::OrderGenerator(const string& appname, const string& appsubs) :
	PROJECT_APP (appname, appsubs), // PRQA S 1021
    _pluginLoader(),
    _pollingTime(default_pollingTime_),
 	_mainThread(pthread_self()),
    _warning(false),
    _restartTime (_defaultRestartTime),
	_dbMaintenancePeriod (_defaultMaintenancePeriod)
{
}

OrderGenerator::~OrderGenerator() throw() 
{
}

void OrderGenerator::customSigIntHandler() 
{
    ACS_LOG_INFO("SigInt caught: Stop scheduled") ;
}

void OrderGenerator::customSigTermHandler() 
{
    ACS_LOG_INFO("SigTerm caught: Stop scheduled") ;
}

int OrderGenerator::usage(const std::string &) const
{
	excerr << "Use: \n" ;
	excerr << "\t" << applicationName() << "[--simul] [--cap]\n" ;
    
	excerr << "\n\tApplication options:\n" ;
	excerr << "\t[--simul] : to do a simulation (rollbacks all DB modification)\n" ;
	excerr << "\t[--cap]   : to list capabilities\n" ; 
	excerr << "\t__________________________________\n" ;
	excerr << "\tto achieve simulation:\n" ;
	excerr << "\texport OGDBSimulationSingleton_DEBUG=1\n" ;
	excerr << "\t__________________________________\n" ;

    return 1 ;
}



string OrderGenerator::loadPlugins() // PRQA S 4020
{
    ostringstream pluginLoadMessages;
	try
	{
		// The configuration is needed for the plugin path keys
		// No retry of configuration loading will be performed
		// This is the only point where plugin loading is allowed
		loadConf();
            
        rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
        try
        {
    	    if (!rs.hasKey (_ordGenRootKey+"."+_pluginsPathKey))
		    {
			    // No configuration found for plugin loader
			    // exit and don't emit any error message
			    return "" ;
		    }
	
		    string pluginsPath;
		    {
	    	    rsPushNSpace nspace_tld (rs, _ordGenRootKey);
	    	    rs.getValue (_pluginsPathKey, pluginsPath);
		    }

		    DirPluginSet myPluginSet (pluginsPath, _pluginRegExp);
		    myPluginSet.load (_pluginLoader);
            AppPluginLoaderNotifier pln(this) ;
  			if (!_pluginLoader.load(pln))
		    {
	    	    Plugin& p = _pluginLoader.failed(true);
	    	    ostringstream os;
	    	    os << "Plugin loader failed. " << p.openError();
	    	    exIOException ex (os.str()); 
	    	    ACS_THROW(ex);
		    }
 			if (pln.warning()) { 
                ACS_LOG_WARNING("Problems loading plugins - see previous messages") ;
                _warning = true ; 
            }
        }
        catch (exception& e)
        {
		    ACS_THROW(exIOException(e, "Error while loading plugins")) ;
        }
	}
	catch (exception& ex)
	{
        ACS_LOG_WARNING("Warning: Plugin loader exception: " << ex) ;
		pluginLoadMessages << "Plugin loader error: " << ex.what();
		_warning = true;
	}
    return pluginLoadMessages.str() ;
}


void OrderGenerator::waitLoop(unsigned long secs, OGListener *listener)
{
	// waiting loop
	Timer pollingTimer ;
	pollingTimer.start() ;

	while (not AppStopController().requestedCancel() ) {

		pollingTimer.stop();
		if (pollingTimer.elapsed()/1000 > secs) { // PRQA S 3084, 4400
			break ;
		}

        if (0 != listener) {

  		    size_t messages_in_queue ; 
		    const bool reset_queue (true) ;
		    listener->gotMessages(messages_in_queue, reset_queue) ; // Queue is cleared

		    if (!messages_in_queue) {
			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "no broadcast received till now") ;
			    AppStopController().timeoutOrCancel (1000);
			    continue ;
		    }
		    else {
                	ACS_LOG_INFO(messages_in_queue << " broadcast(s) received.") ;
			    break ;
		    }
        }
        else {
	    AppStopController().timeoutOrCancel (1000);
        }
    }
}

void OrderGenerator::mainLoop()
{
    string wrkPath = "/tmp" ;
    ACS_LOG_INFO("Changing working directory to " << wrkPath) ;
	if (0 != ::chdir(wrkPath.c_str())) {
		ACS_THROW(exIOException("Cannot chdir to \"" + wrkPath + "\"",errno)) ; // PRQA S 3081
	}

	//******************** Set restart Time ***********************************
	wantToRestart(_restartTime); 
	ACS_LOG_INFO("Restarting every " << _restartTime << " [s]") ;

    debugException() ; // throw exception if debug == 13579    
    
    StopPred stop_pred ;

	OGListener *listener = 0 ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "starting broadcast listener") ;
    try {
        	listener = new OGListener((*Application::instance()->getDefaultConnectionPool())) ;
		listener->startListening() ;
	}
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _pollingTime << "[s]") ;
    }
    ACS_SMARTPTR<OGListener> listenerGuard(listener) ;
    
    Timer confReloadTimer ;
    confReloadTimer.start() ;

	Timer dbMaintenanceTimer ;
	dbMaintenanceTimer.start() ;

	// Start with the db maintenance
	performDbMaintenance();

	while(not stop_pred())
	{
		try
		{

			// rotate log if needed
			Application::rotateLog() ;

			dbMaintenanceTimer.stop();
			if (dbMaintenanceTimer.elapsed()/1000 > _dbMaintenancePeriod) // PRQA S 3084, 4400
			{
				// Perform the maintenance
				performDbMaintenance();

				// restart the timer
				dbMaintenanceTimer.start();
			}


            confReloadTimer.stop() ;

			if (confReloadTimer.elapsed()/1000 > _pollingTime * 5) { // 5 times polling time to reload conf // PRQA S 3084, 4400
				loadConf(); //Reload Configuration
                confReloadTimer.start() ; // restart
                try {
	                if (listener) {
                        listener->reloadConfiguration() ;
                    }
	            }
                catch(exception &x) {
                    ACS_LOG_NOTIFY_EX(x) ;
                    ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _pollingTime << "[s]") ;
                }
            }

			doWork() ;
            waitLoop(_pollingTime, listener) ;

		}//try
		catch(exception &e)
		{
			ACS_LOG_NOTIFY_EX(e) ;
            waitLoop(5) ; // allow 5 [s] or stop before retry - avoid race // PRQA S 4400
		}
	}
	if (listener)
	{
		listener->stopListening();
	}
}

int OrderGenerator::loadFirstConf() // PRQA S 4020
{
	unsigned loadTries=0;
	unsigned loadTriesMax=3;

    StopPred stop_pred ;
	//start main loop here
	while(not stop_pred())
	{
		if (loadConf()) {
			break ; 
		}
		else {
			if(loadTries == (loadTriesMax - 1)) // PRQA S 3084
			{
				//exit with error.
                Application::setStatus (Application::Critical, "Initialization failed - cannot load configuration");
				return -1;
			}
            ACS_LOG_WARNING("Cannot load configuration. Keep trying. ") ;
		}			
		loadTries++;
		AppStopController().timeoutOrCancel (30000);
	}
    
    return 0 ;
}

void OrderGenerator::debugException() 
{
	if (ACS_CLASS_GET_DEBUG() == 13579) { // PRQA S 4400
        ACS_LOG_WARNING("about to generate an exception") ;
		ACS_THROW(exCriticalException("Debug generated exception")) ; // PRQA S 3081
	}
}

void OrderGenerator::writeMsgIfStopped() 
{
	if ( CleanStop == getCurrentStopMode() ) {
        ACS_LOG_INFO("Stop received ... exiting with code 0") ;
	}
	if ( ImmediateStop == getCurrentStopMode() ) {
        ACS_LOG_INFO("Abort received ... exiting with code 0") ;
	}
}




void OrderGenerator::performDbMaintenance () const // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Starting db maintenance procedurs" );

	// Get a DB connection to perform the maintenance
    dbConnectionWrapper pw( *getDefaultConnectionPool() ) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	string theKey = theConnection.getKey();

	// DataBase maintenance activities here

	// Purge waiting orders
	try
	{
		SQLString sqlCommand(theKey,  SQLString::CUSTPROCEDURE ) ; // PRQA S 2502

		// Execute the procedure now
		sqlCommand.setProcedureName("p_purgeexpiredwaitingorders");
		dbQuery purgeQuery (theConnection, sqlCommand, "p_purge_expired_orders") ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand.getSQL() << '\"' );
		if (!OGDBSimulationSingleton::instance()->simulation()) 
		{
				// purge waiting orders table removing expired records
				purgeQuery.execForStatus() ;
		}
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("Error during purge expired waiting orders stored procedure execution. Excpetion notify follows.");
		// No need to propagate the exception just log it and go ahead
		ACS_LOG_NOTIFY_EX (ex);
	}

	// Add other periodicals here...
}


int OrderGenerator::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
	// Store the plugin loader error messages (if any)
	// and put it on logfile later on, when a logfile exists
    string pluginLoadMessages=loadPlugins();
    
    bool printCapabilities = false ;
	for (int i=0; i < argc; i++) {
		if (string("-cap") == argv[i] || string("--cap") == argv[i]) { // PRQA S 3081
            printCapabilities = true ;
		}
        if (string("--simul") == argv[i]) { // PRQA S 3081
            OGDBSimulationSingleton::instance()->simulation(true) ;
        }
	}


    if (printCapabilities) {
		excerr << applicationName() << " capabilities\n" ;
        excerr << "Sat\n" << " " << "Type Algo\n" ;
		excerr << OrderCreatorFactoryChain::instance()->capabilities() << "\n" ;
		excerr << "use --simul or \n" ;
		excerr << "export OGDBSimulationSingleton_DEBUG=1 to set the application in simul mode\n" ;
		allowPostLog(false) ;
		return 0 ;
    }

	//Lock the application.
	ApplicationLock appLock;
	appLock.lock(true); //exception is raised if lock fails

    if (&exostream::getDefaultOutLogger() != &excmcf) {
	    exostream::getDefaultOutLogger().appendChild(&excmcf) ;
    }
    
//	Application::rotateLog() ; // open the first log
	ACS_LOG_INFO("start") ;

	daemonize();

	_mainThread = pthread_self() ;

	if (not pluginLoadMessages.empty())
	{
		// Flush the plugin error messages to excerr/excmcf
		// using a warning filterable (the DistributorGenerator will go on anyway)
		ACS_LOG_WARNING(pluginLoadMessages) ;
	}

	if (OGDBSimulationSingleton::instance()->simulation()) {
		ACS_LOG_INFO("SIMUL MODE: no change to any database table will be done.") ;
	}
	
	//******************** Load Configuration for the first time **************
	int fcr = loadFirstConf() ;
	Application::rotateLog() ; // open the first log
    if (0 != fcr) { return fcr ; }
	
	writeMsgIfStopped() ;

	try {
		if(_warning) { Application::setStatus (Application::Warning, "Initialization error - problem loading configuration or plugin"); }
		else { Application::setStatus (Application::Ok); }

        mainLoop() ; // loop and do nominal work

        writeMsgIfStopped() ; 

	}
	catch(exception &e)
	{
		ACS_LOG_ERROR("Exiting due to following exception: " << e) ;
  		Application::setStatus (Application::Critical, "Error due to exception");
		return 1 ;
	}

    // Application will restart automatically if PhoenixRestart is the current stop mode
    Application::setStatus (Application::Exiting);
	return 0;
}


void OrderGenerator::doWork() const
{
	OrderExecutor executor ;
    StopPred stop_pred;
    executor.process(*getDefaultConnectionPool(), stop_pred) ; 
}



bool OrderGenerator::loadConf() // PRQA S 4020
{
	try {
		addSpace(ConfigurationSingleton::ConfSpace("OrdersGeneration"), ConfigurationSingleton::NoLoad) ; // PRQA S 3081 3
		addSpace(ConfigurationSingleton::ConfSpace("ExplorerWrapper"), ConfigurationSingleton::NoLoad) ;
		addSpace(ConfigurationSingleton::ConfSpace("StationConf","", false), ConfigurationSingleton::NoLoad) ;
				
        StopPred stop_pred;

        try {
            loadConfiguration(true,stop_pred) ;
        }
        catch(...) {
            if (ACS_CLASS_GET_DEBUG() == 1357) { // PRQA S 4400
                ACS_LOG_WARNING("Cannot load configuration. proceed anyway ") ;
            }
            else { throw ; }
        }
        
 
        if ( !ConfigurationSingleton::instance()->get().getValue("OrderGeneration.RestartTime", _restartTime, false) )
        {
            ACS_LOG_WARNING ("cannot find OrderGeneration.RestartTime. Default set to " << _restartTime);
        }
 
		try
		{
			ConfigurationSingleton::instance()->get().getValue("OrderGeneration.DatabasePollingTime", _pollingTime);
		}
		catch(exception &e)
		{
			_warning = true ;
            ACS_LOG_WARNING("cannot find OrderGeneration.DatabasePollingTime. Default set to " << _pollingTime << " [s]") ;
		}
		if(_pollingTime < 1 ) {
			_pollingTime=default_pollingTime_;  
            ACS_LOG_WARNING("Invalid OrderGeneration.DatabasePollingTime. Default set to " << _pollingTime << " [s]") ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DatabasePollingTime is " << _pollingTime << " [s]") ;

		ConfigurationSingleton::instance()->get().getValue ("OrderGeneration.DbMaintenancePeriod", _dbMaintenancePeriod, false);
		if (_dbMaintenancePeriod < 1) 
		{
			_dbMaintenancePeriod = _defaultMaintenancePeriod;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Database Maintenance Period is " << _dbMaintenancePeriod << " [s]") ;

	}
	catch(exception &e){
		ACS_LOG_NOTIFY_EX(e) ;
		return false ;
	}
	return true ;
}



_ACS_END_NAMESPACE

