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

    $Prod:  $

    $Id$

    $Author$

    $Log$
    Revision 1.42  2018/02/20 14:11:00  chicas
    message to ACS_LOG_PRIVINFO

    Revision 1.41  2017/11/06 15:43:25  marfav
    Using defaults for application name definitions if not defined

    Revision 1.40  2017/05/02 10:03:44  tergem
    S2PDGS-1641: log armonization

    Revision 1.39  2016/07/06 08:15:24  marfav
    CSGACQ-84
    Log rotation management aligned to new strategy
    Using standard application builder

    Revision 1.38  2016/05/11 14:44:15  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 1.37  2014/09/19 09:29:14  marfav
    Added configuration support to restart time

    Revision 1.36  2013/12/19 12:50:17  marpas
    coding best practices applied

    Revision 1.35  2013/09/30 07:40:17  clanas
    removed exception notification for confspace replacement

    Revision 1.34  2013/05/12 14:37:10  marpas
    no longer master/slave check

    Revision 1.33  2013/03/14 13:22:42  marpas
    connection pool concept is demanded to class inheriting from Application

    Revision 1.32  2013/02/19 16:22:33  marpas
    configuration reloading fixed
    uses PluginLoaderNotifier through derived class AppPluginLoaderNotifier to detect broken links

    Revision 1.31  2013/02/12 17:57:16  marpas
    minor changes, messages improved

    Revision 1.30  2013/02/12 12:36:59  marpas
    no application in warning state if a parameter has been set to default because not present in configuration
    master db concept managemente improved

    Revision 1.29  2013/02/07 17:58:24  marpas
    DistributorExecutor & CompletedOrderFinalizer now do most of the work (they are in libDistributorCreator)
    They are also aware about connection pools and stop predicates to maximize efficiency
    setting DistributorGenerator_DEBUG to 1357 allow the application to run even in case of problem loading configuration files for testing purposes

    Revision 1.28  2013/02/05 18:17:24  marpas
    listener re-added on t_temporarydistribution & t_completedorders
    logical flow with delay rewritten in main loop

    Revision 1.27  2013/01/24 19:21:29  marpas
    interface rationalization
    control flow simplified for easy maintenance
    coding best practices
    qa rules

    Revision 1.26  2013/01/11 13:54:56  marfav
    Addedd support to external appname definition

    Revision 1.25  2012/03/08 10:52:55  marpas
    task flow and messages revisited

    Revision 1.24  2012/03/01 12:58:05  marpas
    dbconf in in charge to the base class

    Revision 1.23  2012/02/29 15:19:09  chicas
    Changed all factories with DefaultDistributorFacotries class

    Revision 1.22  2012/02/20 15:20:05  marpas
    refactoring

    Revision 1.21  2012/02/09 15:12:44  marpas
    refactoring

    Revision 1.20  2011/11/15 11:19:03  marpas
    Oracle need LOWER function in uppercase

    Revision 1.19  2011/10/20 10:08:41  crivig
    software status notification introduced

    Revision 1.18  2010/12/20 10:30:03  crivig
    limit to 50 the distribution of completed orders

    Revision 1.17  2010/07/29 12:54:20  marpas
    _mainThread initialization after daemonize

    Revision 1.16  2010/06/15 09:35:28  marpas
    HAS_MISSION_INCLUDES management added

    Revision 1.15  2010/04/14 13:04:08  enrcar
    EC:: Oracle11 compatibility added

    Revision 1.14  2010/03/09 10:24:39  enrcar
    EC:: Oracle DB compatibility added

    Revision 1.13  2009/07/01 17:08:05  marpas
    should keep trying to connect to the db until stopped.

    Revision 1.12  2009/06/11 12:52:21  crivig
    partialcvrg and totalcvrg algos added

    Revision 1.11  2009/02/19 12:01:29  marpas
    debug improved and useless libraries removed

    Revision 1.10  2008/12/11 13:17:17  marpas
    improvements

    Revision 1.9  2008/11/26 13:23:42  danalt
    Using ConfigurationSingleton instead of MergeConfiguration to load confspaces

    Revision 1.8  2008/10/08 13:23:07  crivig
    porting to postgres8

    Revision 1.7  2008/05/20 13:27:29  crivig
    create temporarydistribution from completed orders

    Revision 1.6  2008/04/28 14:05:11  crivig
    20 log files saved

    Revision 1.5  2008/02/01 14:24:22  crivig
    qualifyreport algo added

    Revision 1.4  2007/04/02 14:56:12  crivig
    Redistribution algo added

    Revision 1.3  2007/03/21 16:50:50  crivig
    force local host as master introduced

    Revision 1.2  2007/03/16 16:33:29  crivig
    coding and testing

    Revision 1.1.1.1  2007/03/15 15:01:03  crivig
    Imported source

*/
#include <DistributorGenerator.h>
#include <DGListener.h>
#include <DGDBSimulationSingleton.h>
#include <CompletedOrderFinalizer.h>
#include <DistributorExecutor.h>
#include <DistributorCreatorFactoryChain.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <DirPluginSet.h>
#include <Plugin.h>
#include <ConfigurationSingleton.h>
#include <dbConnectionPool.h>

#include <ApplicationLock.h>

#include <DefaultDistributorFactories.h>

#include <ChDir.h>

#include <Timer.h>
#include <Filterables.h>
#include <vector>


#include <dgV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace { // unnamed
    // version management
    dgV version ;
}

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
    StopPred & operator= (const StopPred &r) ; // not implemented
};


ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributorGenerator)

const unsigned int DistributorGenerator::_defaultRestartTime = 86400 ; // One day in seconds

// Root key for configuration spaces navigation
const std::string DistributorGenerator::_distGenRootKey = "DistributionGeneration";

// Configuration spaces keys for values
const std::string DistributorGenerator::_pluginsPathKey = "PluginsPath";
const std::string DistributorGenerator::_pluginRegExp   = "\\.so$";
const std::string DistributorGenerator::_restartTimeKey = "RestartTime";


// PRQA S 1020 L1


#ifndef DISTRIBUTIONGENERATOR_APPNAME
#define DISTRIBUTIONGENERATOR_APPNAME "DistributorGenerator"
#endif

#ifndef DISTRIBUTIONGENERATOR_SUBSYSTEM
#define DISTRIBUTIONGENERATOR_SUBSYSTEM "SDF"
#endif

#define APPNAME DISTRIBUTIONGENERATOR_APPNAME
#define APPSUBS DISTRIBUTIONGENERATOR_SUBSYSTEM

namespace { // unnamed

    // The application builder
    Application::GenericBuilder<DistributorGenerator> theApplicationBuilder(APPNAME, APPSUBS) ;

}

// PRQA L:L1
DistributorGenerator::DistributorGenerator(const string &appName, const string &subSystem) :
#if defined (HAS_MISSION_INCLUDES)
    PROJECT_APP(appName, subSystem),  // PRQA S 1021
#elif GENERIC_STATION_APP
    StationApp(appName, subSystem),
#elif COSMO
    CosmoApp(appName, subSystem),
#else
    CryosatApp(appName, subSystem),
#endif
    _pluginLoader(),
    _pollingTime(default_pollingTime_),
    completed_orders_num_(default_completed_orders_num_),
    _warning(false),
    _restartTime (_defaultRestartTime),
     _mainThread(pthread_self())
{
}

DistributorGenerator::~DistributorGenerator() throw() {
// delete _connection;
}


void DistributorGenerator::customSigTermHandler()
{
    if (pthread_self() == _mainThread) {
        ACS_LOG_INFO("SigTerm caught: Stop scheduled") ;
    }
}

void DistributorGenerator::customSigIntHandler()
{
    if (pthread_self() == _mainThread) {
        ACS_LOG_INFO("SigInt caught: Stop scheduled") ;
    }
}

int DistributorGenerator::usage ( const std::string & ) const
{
    excerr << "Use: \n" ;
    excerr << applicationName() << " [--cap] [--simul]\n" ;

    excerr << "\n\tApplication options:\n" ;
    excerr << "\t--cap    : prints capabilities and exits\n" ;
    excerr << "\t--simul  : simulation mode: no DB changes\n" ;
    excerr << "\n" ;
    return 1 ;
}

string DistributorGenerator::loadPlugins() // PRQA S 4020
{
    ostringstream pluginLoadMessages;
    try
    {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"loading configuration") ;
        // The configuration is needed for the plugin path keys
        // No retry of configuration loading will be performed
        // This is the only point where plugin loading is allowed
        loadConf();

        rsResourceSet& rs = ConfigurationSingleton::instance() -> get() ;

        try
        {
            const string rsKey = _distGenRootKey+"."+_pluginsPathKey ;

            if (!rs.hasKey (rsKey))
            {
                // No configuration found for plugin loader
                // exit and don't emit any error message

                return "" ;
            }

            string pluginsPath;
            {
                rsPushNSpace nspace_tld (rs, _distGenRootKey);
                rs.getValue (_pluginsPathKey, pluginsPath);
            }

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Looking for plugin(s) with regexp: \"" << _pluginRegExp << "\"  from path: \"" << pluginsPath << "\"") ;

            DirPluginSet myPluginSet (pluginsPath, _pluginRegExp);
            myPluginSet.load (_pluginLoader);

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Loading plugin(s) with regexp: \"" << _pluginRegExp << "\"  from path: \"" << pluginsPath << "\"") ;

            AppPluginLoaderNotifier pln(this) ;
              if (!_pluginLoader.load(pln))
            {
                Plugin& p = _pluginLoader.failed(true);
                ACS_THROW(exIOException ("Plugin loader failed. " + p.openError())) ; // PRQA S 3081
            }

             if (pln.warning()) {
                ACS_LOG_WARNING("Problems loading plugins - see previous messages") ;
                _warning = true ;
            }
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Successfully loaded plugin(s) with regexp: \"" << _pluginRegExp << "\"  from path: \"" << pluginsPath << "\"") ;
        }
        catch (exception& e)
        {
            ACS_THROW(exIOException(e, "Error while loading plugins")) ; // PRQA S 3081
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

void DistributorGenerator::waitLoop(unsigned long secs, DGListener *listener)
{
    // waiting loop
    Timer pollingTimer ;
    pollingTimer.start() ;

    while (not requestedStop()) {

        pollingTimer.stop();
        if (pollingTimer.elapsed()/1000 > secs) { // PRQA S 3084, 4400
            break ;
        }

        if (0 != listener) {
		size_t messages_in_queue = 0; 
		const bool reset_queue (true) ;
		listener->gotMessages(messages_in_queue, reset_queue) ; // Queue is cleared

            if (!messages_in_queue) {

                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"no broadcast received till now") ;
                waitStopOrTimeout(1000) ; // wait a second or stop received // PRQA S 4400
                continue ;
            }
            else {
                ACS_LOG_INFO(messages_in_queue << " broadcast(s) received.") ;
                break ;
            }
        }
        else {
            waitStopOrTimeout(1000) ; // wait a second or stop received // PRQA S 4400
        }
    }
}

void DistributorGenerator::mainLoop()
{
    string wrkPath = "/tmp" ;
    ACS_LOG_PRIVINFO("Changing working directory to " << wrkPath) ;
    ChDir wdir(wrkPath) ;

    //******************** Set restart Time ***********************************

    wantToRestart(_restartTime);
    ACS_LOG_PRIVINFO("Restarting every " << _restartTime << " [s]") ;

    debugException() ; // throw exception if debug == 13579

    DGListener *listener = 0 ;

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"starting broadcast listener") ;

    try {
        listener = new DGListener((*Application::instance()->getDefaultConnectionPool())) ;
        listener->startListening() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _pollingTime << "[s]") ;
    }
    ACS_SMARTPTR<DGListener> listenerGuard(listener) ;

    Timer confReloadTimer ;
    confReloadTimer.start() ;
    while(not requestedStop())
    {
        try
        {

            // rotate log if needed
            Application::rotateLog() ;

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
            waitLoop(5) ; // allow 5 sec or stop before retry - avoid race // PRQA S 4400
        }
    }
    if (listener)
    {
	listener->stopListening();
    }
    
}

int DistributorGenerator::loadFirstConf() // PRQA S 4020
{
    unsigned loadTries=0;
    unsigned loadTriesMax=3;

    //start main loop here
    while(not requestedStop())
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
            ACS_LOG_INFO("Cannot load configuration. Keep trying. ") ;
        }
        loadTries++;
        waitStopOrTimeout(30000) ;
    }

    return 0 ;
}

void DistributorGenerator::debugException()
{
    if (ACS_CLASS_GET_DEBUG() == 13579) { // PRQA S 4400
        ACS_LOG_WARNING("about to generate an exception") ;
        ACS_THROW(exCriticalException("Debug generated exception")) ; // PRQA S 3081
    }
}

void DistributorGenerator::writeMsgIfStopped()
{
    switch (getCurrentStopMode())
    {
    case PhoenixRestart:
        ACS_LOG_INFO("Phoenix received ... restaring") ;
        break;
    case CleanStop:
        ACS_LOG_INFO("Stop received ... exiting with code 0") ;
        break;
    case ImmediateStop:
        ACS_LOG_INFO("Abort received ... exiting with code 0") ;
        break;
    default:
        break;
    }
}

int DistributorGenerator::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
    //This allocates all factories for default distribution algo
    DefaultDistributorFactories defaultDistr;

    // Store the plugin loader error messages (if any)
    // and put it on logfile later on, when a logfile exists
    string pluginLoadMessages=loadPlugins();

    bool printCapabilities = false ;
    for (int i=0; i < argc; i++) {
        if (string("-cap") == argv[i] || string("--cap") == argv[i]) { // PRQA S 3081
            printCapabilities = true ;
        }
        if (string("--simul") == argv[i]) { // PRQA S 3081
            DGDBSimulationSingleton::instance()->simulation(true) ;
        }
    }

    if (printCapabilities) {
        excerr << applicationName() << " capabilities\n" ;
        excerr << "******** Distribution Algo ********\n" ;
        excerr << DistributorCreatorFactoryChain::instance()->capabilities() << "\n" ;
        excerr << "use --simul or \n" ;
        excerr << "export DGDBSimulationSingleton_DEBUG=1 to set the application in simul mode\n" ;
        allowPostLog(false) ;
        return 0 ;
    }
    //Lock the application.
    ApplicationLock appLock;
    appLock.lock(true); //exception is raised if lock fails

//	Application::rotateLog() ; // open the first log

    ACS_LOG_INFO("start") ;

    daemonize() ;

    _mainThread = pthread_self() ;

    if (not pluginLoadMessages.empty())
    {
        ACS_LOG_WARNING(pluginLoadMessages) ;
    }

    if (DGDBSimulationSingleton::instance()->simulation()) {
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
        ACS_LOG_ERROR("Exiting due to following exception: " << e.what()) ;
           Application::setStatus (Application::Critical, "Error due to exception");
        return 1 ;
    }
    Application::setStatus (Application::Exiting);
    return 0;

}




void DistributorGenerator::doWork() // PRQA S 4020, 4211
{
    StopPred stop_pred;

    // First create temporary distribution from the completed orders
    try {
        CompletedOrderFinalizer cof ;
        cof.distributeCompletedOrders(*getDefaultConnectionPool(), stop_pred, completed_orders_num_) ;
    }
    catch(exception &ex) {
        ACS_LOG_NOTIFY_EX(ex) ;
    }

    if (stop_pred()) { return ; }

    // than distribute temporaries
    DistributorExecutor executor ;
    executor.distribute(*getDefaultConnectionPool(), stop_pred) ;
}


bool DistributorGenerator::loadConf() // PRQA S 4020
{
    try {
        addSpace(ConfigurationSingleton::ConfSpace("DistributionGeneration"), ConfigurationSingleton::NoLoad); // PRQA S 3081

        StopPred stop_pred;

        try {
            loadConfiguration(false,stop_pred) ;
        }
        catch(...) {
            if (ACS_CLASS_GET_DEBUG() == 1357) { // PRQA S 4400
                ACS_LOG_WARNING("Cannot load configuration. proceed anyway ") ;
            }
            else { throw ; }
        }

        rsResourceSet& rs = ConfigurationSingleton::instance()->get();
        rsPushNSpace pusher (rs, _distGenRootKey);

        if (!rs.getValue ("CompletedOrdersNum", completed_orders_num_, false))
        {
            ACS_LOG_WARNING("cannot find DistributionGeneration.CompletedOrdersNum. Default set to " << completed_orders_num_ ) ;
        }

        if (!rs.getValue ("DatabasePollingTime", _pollingTime, false))
        {
            _warning = true ;
            ACS_LOG_WARNING("cannot find DistributionGeneration.DatabasePollingTime. Default set to " << _pollingTime << " [s]") ;
        }

        if(_pollingTime < 1 ) {
            _pollingTime=default_pollingTime_;
            ACS_LOG_WARNING("Invalid DistributionGeneration.DatabasePollingTime. Default set to " << _pollingTime << " [s]") ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DatabasePollingTime is " << _pollingTime << " [s]") ;

        if (!rs.getValue (_restartTimeKey, _restartTime, false))
        {
            ACS_LOG_WARNING("cannot find DistributionGeneration." << _restartTimeKey << ". Default set to " << _restartTime ) ;
        }

    }
    catch(exception &e){
        ACS_LOG_NOTIFY_EX( e ) ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning false") ;
        return false ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning true") ;
    return true ;
}


_ACS_END_NAMESPACE


