/*

    Copyright 1995-2015, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ThinLayer Daemon $

    $Id$

    $Author$

    $Log$
    Revision 3.9  2017/12/14 11:38:17  marfav
    ECICMF-75
    S2PDGS-1900
    Removed default task table factory

    Revision 3.8  2017/11/06 18:55:56  nicvac
    ECICMF-94: Resource reserved for an order and automatic resources release.

    Revision 3.7  2017/11/06 17:15:39  nicvac
    ECICMF-94: resource computation. Reservation before precondition check

    Revision 3.6  2017/10/24 15:37:44  marfav
    ECICMF-94 Preintegrazione servizi - Caso SLOT based coperto

    Revision 3.5  2017/10/23 15:46:33  marfav
    Resource management service integration in progress

    Revision 3.4  2017/10/19 15:52:05  marfav
    Integration of service based resource manager

    Revision 3.3  2017/03/01 13:48:09  tergem
    rotateLog call moved after configuration is loaded. Messages before log file opening to be handled

    Revision 3.2  2017/02/01 09:43:40  marfav
    Default added for appnames e subsname

    Revision 3.1  2016/11/04 15:23:17  marfav
    APF-271
    Adding slot id to splunk messages

    Revision 3.0  2016/10/11 08:27:16  marfav
    Major release due to cache management modifications

    Revision 2.30  2016/07/05 13:11:54  marfav
    CSGACQ-84
    Using the application based log rotation policy

    Revision 2.29  2016/05/11 11:53:57  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 2.28  2015/11/27 17:38:06  marpas
    coding best practice application in progress

    Revision 2.27  2015/11/19 17:23:03  marpas
    coding best practices application in progress

    Revision 2.26  2015/08/13 15:26:33  marfav
    Adding default fuction for MC messages

    Revision 2.25  2015/08/12 14:21:43  marfav
    APF-271 work in progress

    Revision 2.24  2015/08/07 14:40:18  marfav
    PDSEV-8 interaction with the db queue optimized and blacklist table support added

    Revision 2.23  2015/05/14 08:20:49  damdec
    Compilation warning fixed.

    Revision 2.22  2015/04/30 15:49:17  marfav
    Compiler warning removed

    Revision 2.21  2013/03/14 15:05:39  marpas
    connection pool concept is left to class inheriting from Application

    Revision 2.20  2013/03/08 18:33:28  marfav
    Using connection pool

    Revision 2.19  2013/02/05 11:30:28  danalt
    status file name aligned as per OME SPR CMC Integration for Resource Monitoring

    Revision 2.18  2013/01/11 11:40:49  marfav
    Addedd support to external appname definition

    Revision 2.17  2012/07/27 15:04:12  marfav
    using updateResourceSet instead of mergePrivate with ConfigurationSingleton

    Revision 2.16  2012/05/07 11:00:57  danalt
    Temporary Patch for S1PDGS on status filename

    Revision 2.15  2012/02/09 17:26:24  marpas
    refactoring in progress

    Revision 2.14  2011/10/13 13:02:53  marfav
    Implementing status messages as per S1PDGS-1902

    Revision 2.13  2010/08/03 14:39:34  nicvac
    Task Table plugins management

    Revision 2.12  2008/11/14 14:07:03  marfav
    ConfigurablePluginLoader interface changed
    Load configuration exception management improved

    Revision 2.11  2008/11/14 11:42:45  marfav
    Using ConfigurationSingleton instead of MergeConfiguration to load confspaces

    Revision 2.10  2008/09/18 13:25:00  marfav
    Added support to HAS_MISSION_INCLUDES flag

    Revision 2.9  2008/08/01 15:38:29  marfav
    PreConditionChecker plugins framework added

    Revision 2.8  2008/04/23 08:42:45  marfav
    SRC_APP fixed in SRV_APP

    Revision 2.7  2008/04/22 13:35:28  marfav
    Aligned to the task table conditioned visitor design pattern
    Exception management improved

    Revision 2.6  2008/04/10 16:04:12  integrator
    Added ifdef for SRV project

    Revision 2.5  2007/04/19 08:41:30  giucas
    Removed dependencies from StationApp for ENVISAT

    Revision 2.4  2006/06/27 09:06:54  marfav
    Fixed a pushed namespace when loading WSConfiguration file

    Revision 2.3  2006/06/12 11:12:28  marfav
    Code for NO_SLOTS support removed

    Revision 2.2  2006/03/29 13:24:15  giucas
    Added math.h include to compile

    Revision 2.1  2006/03/06 15:59:19  marfav
    Using Task Tables having AtExit sequence support

    Revision 2.0  2006/02/28 17:02:07  paoscu
    Exception 2.1 I/F adopted

    Revision 1.65  2005/09/28 14:13:42  marpas
    Getting rid of ArfConfiguration

    Revision 1.64  2005/06/28 08:33:46  marfav
    Removed OrderType translation to enumerative types
    Only plain text strings are handled

    Revision 1.63  2005/05/20 08:02:00  marfav
    Plugin regular expression fixed

    Revision 1.62  2005/05/03 16:35:53  marfav
    Order status updated according to scheduler exit code even if immediate stop in progress

    Revision 1.61  2005/04/04 15:20:27  ivafam
    Commented "add Patrol"

    Revision 1.60  2005/03/31 18:13:22  fracar
    messages to mcf improved

    Revision 1.59  2005/03/18 11:22:03  marfav
    Calval environment merged to standard one

    Revision 1.58  2005/03/18 10:27:01  marfav
    excmcf support added

    Revision 1.57  2005/03/02 14:06:18  marfav
    Added cache support for downloaded files

    Revision 1.56  2004/12/23 18:34:26  marfav
    Multislot Environment now active
    WSConfig dump changed using WSConfigMessage class

    Revision 1.55  2004/12/03 17:24:47  marfav
    Header Fixed

    Revision 1.54  2004/10/19 14:19:36  marfav
    Old code removed

    Revision 1.53  2004/10/13 13:44:16  marfav
    Patrol configuration spaces loading ifdeffed

    Revision 1.52  2004/09/16 17:32:19  marpas
    XMLResourceHandler include removed

    Revision 1.51  2004/08/30 12:54:10  marfav
    Support for Cosmo environment added

    Revision 1.50  2004/08/24 14:50:31  marfav
    Added support for DirPluginSet

    Revision 1.49  2004/06/21 10:03:11  marfav
    Stopping the IPC server during cleanup

    Revision 1.48  2004/05/12 08:19:53  marfav
    Preparing for multislot processing status environment

    Revision 1.47  2004/05/06 16:31:10  marfav
    CHECKING_QUEUE status extended to preconditionCheck phase
    PROCESSING_ORDER status begins when preparing order (downloading)

    Revision 1.46  2004/04/22 16:04:04  integrator
    Debug messaging improved

    Revision 1.45  2004/04/22 07:24:55  integrator
    CalVal Environment support added

    Revision 1.44  2004/04/16 10:06:03  marfav
    Timestamp on filterable enabled

    Revision 1.43  2004/04/07 13:19:56  marfav
    Added support for IPC from ThinLayer Clients and to running Schedulers
    Does not use ACE anymore
    Using exStream.h

    Revision 1.42  2004/03/12 14:33:25  marfav
    Send workstation configuration messages to patrol using a single event

    Revision 1.41  2004/03/11 19:10:29  marfav
    Preparing to manage external commands
    Align to new version of Scheduler Engine (libScheduler)

    Revision 1.37  2003/10/03 11:35:21  marfav
    Now all threads have SIGINT and SIGTERM blocked (from libThread)
    No need to block the signals during all the run to avoid threads problems

    Revision 1.36  2003/09/26 13:34:14  marfav
 *** empty log message *** (30 logs, max 3 mega per log)

    Revision 1.35  2003/09/25 09:33:37  marfav
    Removed support for internal order queue
    Using references from ConfigurationSingleton

    Revision 1.34  2003/09/23 14:02:42  fracar
    handling verbose execution mode in support to Thin Layer MMI interactive operations

    Revision 1.33  2003/09/18 15:36:15  marfav
    Phoenix support fixed

    Revision 1.32  2003/09/15 10:24:54  marfav
    Sort of orders by priority feature added

    Revision 1.31  2003/09/08 15:50:55  marfav
    Using ApplicationLock class
    Added running mode messages to the log

    Revision 1.30  2003/09/05 16:08:25  marfav
    Added support for ENVISAT environment

    Revision 1.29  2003/08/29 12:00:46  fracar
    fixed policy for cleaning up existing working directories
    reading file extensions from config file for downloaded inputs
    issuing a critical exception message to Patrol if cannot update order queue

    Revision 1.28  2003/08/07 08:37:23  marfav
    LogRotation policy added

    Revision 1.27  2003/08/01 11:03:30  marfav
    Added support for thread Randez-Vous and InternalOrderQueue

    Revision 1.26  2003/07/29 10:45:00  marfav
    Reload configuration support at run-time

    Revision 1.25  2003/07/18 14:50:45  marfav
    tlDaemon configuration space fixed

    Revision 1.24  2003/07/17 17:18:43  marfav
    Removed notify of exception never thrown

    Revision 1.23  2003/07/17 13:44:06  marfav
    Log messages using exFWarning and exFError

    Revision 1.22  2003/07/16 16:37:30  marfav
    Align to new version of libCryosatApp

    Revision 1.21  2003/07/10 14:18:32  marfav
    clone method added to exceptions

    Revision 1.20  2003/07/04 15:06:33  marfav
    Using Sigalarm Emulator and implementing restart

    Revision 1.19  2003/06/06 10:31:04  marfav
    Dead code removed

    Revision 1.17  2003/05/30 14:44:48  marfav
    Added TLD_Clean_Working_Dir key in configuration space

    Revision 1.16  2003/05/21 15:02:39  marfav
    Removed some dead code

    Revision 1.15  2003/04/30 16:20:16  marfav
    Align to ICD v2.0 and some logging improvement

    Revision 1.14  2003/04/18 16:26:06  fracar
    align to new compiler version

    Revision 1.13  2003/04/18 12:59:33  marfav
    Using namespaces removed from .h

    Revision 1.12  2003/03/27 15:33:22  marfav
    Added plugin support and some logging improvements

    Revision 1.11  2003/03/24 16:35:26  marfav
    Included JobOrderFactory support

    Revision 1.10  2003/03/11 18:25:50  marfav
    Patrol message handling added

    Revision 1.9  2003/02/18 09:09:22  fracar
    improved logging

    Revision 1.8  2003/02/10 18:50:19  fracar
    prepared for multi-interval management

    Revision 1.7  2003/02/10 09:17:48  fracar
    improved logging

    Revision 1.6  2003/02/07 08:30:48  fracar
    handling downloaded file split and improved messaging

    Revision 1.5  2003/02/04 17:17:58  fracar
    updating job responsible field

    Revision 1.4  2003/02/04 17:07:25  fracar
    got rid of ^M

    Revision 1.3  2003/02/04 16:45:09  fracar
    improved messaging

    Revision 1.2  2003/02/04 09:58:34  marfav
    Added LOG field to headers



 */


#include <tldV.h>

#include <tlDaemon.h>

#include <WSConfig.h>
#include <WSConfigMessage.h>
#include <TaskTable.h>
#include <TaskTableFactoryChain.h>
#include <TLServiceResourceDefaultFactory.h>
#include <QueueMgrFactory.h>
#include <OrderQueueMgr.h>
#include <OrderExecutionMgr.h>
#include <TlStatusMgrSingleton.h>
#include <SignalGuard.h>
#include <ExecutingOrder.h>

#include <ApplicationLock.h>
#include <ConfigurationSingleton.h>
#include <exMsgFmtFactory.h>
#include <exException.h>

#include <Filterables.h>
#include <MCFilterables.h>

#include <exStream.h>
#include <DirPluginSet.h>
#include <rsPushNSpace.h>
#include <Timer.h>
#include <XMLIstream.h>
#include <XMLValidator.h>

#include <signal.h>
#include <unistd.h>
#include <math.h>

#include <string>
#include <vector>
#include <iostream>
#include <sstream>



#ifdef CAL_VAL_APP
#include <cvfLogFilter.h>
#endif // CAL_VAL_APP

#ifdef SRV_APP
#include <srvLogFilter.h>
#endif // SRV_APP


static const char *_tldRootKey = "TlDaemon";
static const char *_cfgPathKey   	= "TLD_WS_Config_Path";
static const char *_queueFrequencyKey	= "TLD_Queue_Polling_Frequency";
static const char *_queueMgrTypeKey	= "TLD_QueueMgrType";
static const char *_basePathKey		= "TLD_Orders_Root_Path";
static const char *_restartTimerKey	= "TLD_Full_Cycle_Period";
static const char *_reloadTimerKey	= "TLD_Cfg_Reload_Period";
static const char *_leftTagKey = "TLD_LeftTag";
static const char *_rightTagKey = "TLD_RightTag";
static const char *_prioritySortOrderKey = "TLD_Priority_Sort_Order";
static const char *_logSizeKey = "TLD_Log_Rotation_Size";
static const char *_logAgeKey = "TLD_Log_Rotation_Age";
static const char *_ipcPortKey = "TLD_Ipc_Port";
static const char *_tlPluginsPathKey = "TLD_Plugins_Path";
static const char *_tlListOfPluginsPathKey = "TLD_List_of_Plugins_Path.TLD_Plugins_Path";
static const char *_tlDoTaskTableValidation = "TLD_Do_TaskTable_Validation";

static const char *_pluginRegExp = "\\.so$";


static const unsigned int _numberOfThreadToWait = 1;


// Default values are used if the keys are not found in configuration spaces
static const unsigned int _defaultReloadTimer = 10;  // Reload configuration files every 10 minutes
static const unsigned int _defaultRestartTimer = 24;  // Restart every 24 hours
static const unsigned int _defaultLogSize = 3; // Megabytes
static const unsigned int _defaultLogAge = 24; //Hours
static const unsigned int _defaultLogNumber = 15; //Logs
static const unsigned int _defaultIpcPort = 2048; // Tcp/Ip IPC port
static const char *_leftTagDefault = "RUNNING";
static const char *_rightTagDefault = "MODE";


using namespace std;
using namespace acs;
using namespace acs::mc;

namespace {
tldV version;
//Embedded Factory containing only the default TaskTable (retrievable by wildcard key).
TLServiceResourceDefaultFactory embeddedDefaultResourceMgrFactory;
}


// Do not touch the database in case of immediate stop
#define DO_NOT_UPDATE_STATUS_ON_IMMEDIATE_STOP


// Temporary Patch for S1PDGS
#ifdef HAS_APPNAME_DEFINITIONS

#ifndef TL_APPNAME
#define TL_APPNAME "tlDaemon"
#endif

#ifndef TL_SUBSYSTEM
#define TL_SUBSYSTEM "ThinLayer"
#endif

Application::GenericBuilder<tlDaemon> theApplicationBuilder(TL_APPNAME, TL_SUBSYSTEM) ;
#elif defined DEFAULT_ACS_STATUS_PATH
Application::GenericBuilder<tlDaemon> theApplicationBuilder("NRTAPThinLayer", "ACQ") ;
#else
Application::GenericBuilder<tlDaemon> theApplicationBuilder("tlDaemon", "ThinLayer") ;
#endif

bool tlDaemon::_haveToReloadCfg = false;


ACS_CLASS_DEFINE_DEBUG_LEVEL(tlDaemon)


string tlDaemon::getDefaultLogName() const
{
    string logFullPath = defaultLogPath + "/ThinLayerDaemon.log" ; // create the log path name
    return logFullPath;
}

int tlDaemon::main(int argc, char const * const * argv, char const * const * env)
{
    // APF-271
    MCParameters::instance()->setDefaultFunction ("Processing Chain");
    MCParameters::instance()->setDefaultCode ("DPR");

    // use this to have only one instance of the daemon per host and to write the pid
    ApplicationLock tlAppLock(1);
    string lockFileName = tlAppLock.lock();

    // turn application into a daemon
    daemonize();
    tlAppLock.writePid();

    // Force timestamps on filterable messages
    exMsgFmtFactory::instance()->emitTime(true);


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon main : lock file is [" << lockFileName << "]");

    string cfg("");

    if (argc >1)
    {
        if (argv[1][0] != '-')
        {
            ACS_LOG_WARNING("Thin Layer Daemon: WARNING: overriding configuration settings with " << argv[1]);
            cfg = argv[1];
        }
    }

    _myCfgPath = cfg;

    if (cfg != "")
    {
        ACS_LOG_INFO("Thin Layer Daemon: creating application; the configuration file is " << _myCfgPath);
    }


    try
    {
        run ();
    }

    catch(exception &e)
    {
        ACS_LOG_NOTIFY_EX(e) ;

        ACS_LOG_INFO ("Exiting return code -1");

        Application::setStatus (Application::Exiting, "Exiting return code -1");
        return -1;
    }

    int exitCode=0;

    if (!mustRestart())
    {
        // this is not a phoenix restart !!
        exitCode = exitStatus();
        ostringstream msg;
        msg << "Exiting return code " << exitCode;
        ACS_LOG_INFO(msg.str());
        //        excmcf << msg;

        Application::setStatus (Application::Exiting, msg.str());

    }

    return exitCode;
}



tlDaemon::tlDaemon (const string &appName, const string &subSystem) :
#ifdef HAS_MISSION_INCLUDES
            PROJECT_APP (appName, subSystem),
#elif COSMO
            CosmoApp(appName, subSystem),
#elif CAL_VAL_APP
            CalValApp(appName, subSystem),
#elif SRV_APP
            SrvApp(appName, subSystem),
#else
            CryosatApp(appName, subSystem),
#endif
            _countMutex(),
            _cfgMutex(),
            _rendezVousThreads(0),
            _reloadTimer(),
            _myCfgPath(),
            _wsConfiguration(0),
            _queueMgr(0),
            _executionMgr(0),
            _ipcServer(0),
            _verbose(false),
            _pluginLoader (), //_pluginLoader (ConfigurationSingleton::instance()->get()),
            _sigTermReceived (false),
            _sigIntReceived (false),
            _sigPhoenixReceived (false)
{

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
                    _verbose = true;
    ACS_CLASS_END_DEBUG

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon CTOR : executed");

}

tlDaemon::~tlDaemon() throw()
        {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon DTOR : entered");

    delete _wsConfiguration;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon DTOR : _wsConfiguration deleted");


    delete _queueMgr;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon DTOR : _queueMgr deleted");


    delete _executionMgr;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon DTOR : _executionMgr deleted");

    delete _ipcServer;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon DTOR : _ipcServer deleted");

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon DTOR : executed");

        }


void
tlDaemon::logRunningMode ()
{
    string leftTag (_leftTagDefault);
    string rightTag (_rightTagDefault);
    string queueMgrType("");

    {
        rsResourceSet& rs (ConfigurationSingleton::instance() -> get() );
        rsPushNSpace nspace_tld (rs, _tldRootKey);

        if (rs.hasKey (_leftTagKey) )
        {
            rs.getValue(_leftTagKey, leftTag);
        }
        if (rs.hasKey (_rightTagKey) )
        {
            rs.getValue(_rightTagKey, rightTag);
        }
        if (rs.hasKey (_queueMgrTypeKey) )
        {
            rs.getValue(_queueMgrTypeKey, queueMgrType);
        }
    }

    ACS_LOG_INFO(leftTag << " " << queueMgrType << " " << rightTag);

}


void
tlDaemon::verbose(bool v)
{
    _verbose = v;

    // managers verbose mode must follow the one of the daemon
    // (need to check their existence, since this can be called from within managers constructors)
    if (_queueMgr)
        _queueMgr->verbose(v);
    if (_executionMgr)
        _executionMgr->verbose(v);
}

bool
tlDaemon::verbose() const
{
    return _verbose;
}

bool
tlDaemon::updateOrderStatus(const tlOrder& aTlOrder, const tlOrderStatus status, bool notifyOnError)
{
    if (_queueMgr)
    {
        return _queueMgr->updateOrderStatus(aTlOrder, status, notifyOnError);
    }
    else
    {
        return false;
    }
}

bool
tlDaemon::registerDownloadStart(const tlOrder& anOrder)
{
    if (_queueMgr)
        return _queueMgr->registerDownloadStart (anOrder);
    else return false;
}

bool
tlDaemon::registerProcessingStart(const tlOrder& anOrder)
{
    if (_queueMgr)
        return _queueMgr->registerProcessingStart (anOrder);
    else return false;
}

bool
tlDaemon::registerProcessingStop(const tlOrder& anOrder, tlOrderStatus status)
{
    if (_queueMgr)
        return _queueMgr->registerProcessingStop (anOrder,status);
    else return false;
}


void
tlDaemon::reloadConfiguration(bool* warning)
{

    // keep track of the problems recorded during the configuration reload phase
    // in order to generate the correct overall status of the daemon
    bool emitWarning = false;


    bool configurationChanged = false;

    // wait for threads rendez-vous
    // this will cause all the threads calling the rendezVous method
    // to wait until the resource is unlocked

    Lock guard(_cfgMutex);  //acquire the lock and begin threads synchronization

    unsigned int count = 0;

    // check if there was a reloadConfiguration request concurrent with an exit or restart request
    // if there is an exit/restart request, have no need to reload the cfg
    bool haveToExit =  (getCurrentStopMode() != NoStop);


    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: waiting " <<
            _numberOfThreadToWait <<
            " threads before reloading configuration");


    while ( (count < _numberOfThreadToWait) && !haveToExit)
    {
        // sleep until all the threads are waiting for the _cfgMutex unlock

        //_signalGuard.flush();
        waitStopOrTimeout (1000);
        {
            Lock guard(_countMutex);
            count = _rendezVousThreads;
        }


        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon::reloadConfiguration: " <<
                count <<
                " threads joined to rendez vous");

        bool running = false;

        if (_queueMgr)
        {
            running=_queueMgr->running();

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon::reloadConfiguration: _queueMgr " <<
                    (running ? "is" : "is not") <<
                    " running");

        }

        if (( getCurrentStopMode() != NoStop) ||  !running)
        {
            haveToExit=true;

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon::reloadConfiguration: (( getStopMode() != NO_STOP) ||  !running) evaluates to true");
        }
    }

    if (!haveToExit)
    {
        // all the threads are synchronized... let's go with the configuration reload
        try
        {

            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: going to load configuration");


            {
                configurationChanged |= loadTLConfiguration(_myCfgPath, &emitWarning);
            }


            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: configuration loaded");

        }
        catch (const exException& toCatch)
        {
            ACS_LOG_WARNING ("Cannot load configuration spaces - going on with the previous settings");
            // this is an issue (warning)
            emitWarning = true;
        }
        try
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: going to load WS configuration");

            configurationChanged |= loadWSConfiguration();

            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: WS configuration loaded");
        }
        catch (const exException& toCatch)
        {
            ACS_LOG_NOTIFY_EX("Cannot load WS configuration file - available processors list inconsistency expected");
            ACS_LOG_NOTIFY_EX(toCatch);
            // this is an issue (warning)
            emitWarning = true;
        }

        if (configurationChanged)
        {
            // apply the new settings
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: going to configure facilities");
            configureFacilities();
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: facilities configured");


            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: going to restart facilities");
            restartFacilities();
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: facilities restart");
        }


        // now it is safe to rotate the log
        // rotate the log here to avoid race conditions with others threads
        // and to assure the mode logged on every fresh-rotated file
        rotateLog();

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: going to log running mode");
        logRunningMode();
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon::reloadConfiguration: running mode logged");
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Thin Layer Daemon::reloadConfiguration: haveToExit is true: configurations not reloaded");
    }

    _haveToReloadCfg=false;
    // now the _cfgMutex is released and all the threads will run back again

    if (warning)
        *warning = emitWarning;
}

void
tlDaemon::configureLog()
{
    rsResourceSet& rs (ConfigurationSingleton::instance() -> get() );
    rsPushNSpace nspace_tld (rs, _tldRootKey);

    // get the max log size
    float rotationSize;
    if (rs.hasKey (_logSizeKey) )
    {
        rs.getValue(_logSizeKey, rotationSize);
    }
    else // no resource defined for this key
    {
        ACS_LOG_WARNING("Thin Layer Daemon : resource [" << _logSizeKey << "] not defined. Using default value : "
                << _defaultLogSize << " Megabytes");
        rotationSize = _defaultLogSize;
    }
    ACS_LOG_INFO("Thin Layer Daemon : setting max log size to " << rotationSize << " Megabytes.");
    unsigned int newRotationSize = rint (rotationSize * 1024 * 1024);

    // get the max log age
    float rotationAge;
    if (rs.hasKey (_logAgeKey) )
    {
        rs.getValue(_logAgeKey, rotationAge);
    }
    else // no resource defined for this key
    {
        ACS_LOG_WARNING("Thin Layer Daemon : resource [" << _logAgeKey << "] not defined. Using default value : "
                << _defaultLogAge << " hours");
        rotationAge = _defaultLogAge;
    }

    ACS_LOG_INFO("Thin Layer Daemon : setting max log age to " << rotationAge << " hours.");
    unsigned int newRotationAge = rint (rotationAge * 3600);

    getLRPolicy().aging (newRotationAge);
    getLRPolicy().size (newRotationSize);

}

void
tlDaemon::configureTlDaemon()
{
    configureLog();
    configureTimers();
}

void
tlDaemon::configureFacilities()
{
    configureTlDaemon();
    configureQueueMgr();
    configureExecutionMgr();
    configureIpcServer();
}

void
tlDaemon::restartFacilities()
{
    _queueMgr->restartQueueMgr();
}

void
tlDaemon::_init()
{
    // Initialization starts here
    Application::setStatus (Application::Initializing);


    // first, set up configuration stuff

    ACS_LOG_INFO("Thin Layer Daemon: loading TL Configuration");

    // keep track of load configuration warnings
    bool loadTLConfWarning = false;

    try
    {
        loadTLConfiguration(_myCfgPath, &loadTLConfWarning);
    }
    catch (const exException& toCatch)
    {
        // This is an unrecoverable error. Update the status.
        Application::setStatus (Application::Critical, "Initialization failed - cannot load configuration");
        tlDaemonException e(toCatch, "Cannot load configuration settings: initialisation failed ") ;
        ACS_THROW (e) ;
    }

    // The configuration has been loaded.. it is safe now to rotate the log
    rotateLog();

    // do some logging
    ACS_LOG_INFO("Thin Layer Daemon: loading WS Configuration");

    try
    {
        loadWSConfiguration();
    }
    catch (const exException& toCatch)
    {
        // This is an unrecoverable error. Update the status.
        Application::setStatus (Application::Critical, "Initialization failed - cannot load WSConfig file");
        tlDaemonException e(toCatch, "Cannot load WS Configuration file: initialisation failed ") ;
        ACS_THROW (e) ;
    }



    // load plug-ins

    ACS_LOG_INFO("Thin Layer Daemon: loading plug ins");

    try
    {
        loadPlugIns();
    }
    catch (const exException& toCatch)
    {
        // This is an unrecoverable error. Update the status.
        Application::setStatus (Application::Critical, "Unable to load plugins");
        tlDaemonException e(toCatch, "Cannot load plug-ins: initialisation failed ") ;
        ACS_THROW (e) ;
    }



    // then, create order managers

    ACS_LOG_INFO("Thin Layer Daemon: instantiating order queue manager");
    createQueueMgr();

    ACS_LOG_INFO("Thin Layer Daemon: instantiating order execution manager");
    createExecutionMgr();

    // create the listener for incoming tcp/ip connections
    ACS_LOG_INFO("Thin Layer Daemon: instantiating IPC Server");
    createIpcServer();


    _reloadTimer.signal (sigReloadRequestHdlr);

    initRestartTimer();
    configureTlDaemon();

    // Setting the IDLE status
    {
        (TlStatusMgrSingleton::instance()->getMgr())->enterStatus (TL_IDLE);

        (DownloadStatusMgrSingleton::instance()->getMgr())->enterStatus (DL_IDLE);
    }



    // finally, perform any specific initialisation
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Thin Layer Daemon_init: calling specific initialisation");

    init();

    if (loadTLConfWarning)
    {
        // Warning in the configuration
        Application::setStatus (Application::Warning, "Configuration update failed, using old configuration");
    }
    else
    {
        // Initialization completed successfully
        Application::setStatus (Application::Ok);
    }

}


void
tlDaemon::rendezVous()
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Thin Layer Daemon::rendezVous: entered function");

    {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: going to acquire _countMutex");

        Lock guard(_countMutex);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: acquired _countMutex");

        ++_rendezVousThreads;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: released _countMutex");




    // acquire the lock and syncronize the thread
    // all the threads calling this method will be blocked attempting
    // the lock on _cfgMutex.
    // This will happen only if the main thread is waiting for a rendezVous
    // that is the main thread is inside the reloadConfiguration method
    //
    {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: going to acquire _cfgMutex");

        Lock guard(_cfgMutex);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: acquired _cfgMutex");



        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: going to acquire _countMutex");
            Lock guard(_countMutex);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: acquired _countMutex");
            --_rendezVousThreads;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: released _countMutex");
    }


    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: released _cfgMutex");
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Thin Layer Daemon::rendezVous: exiting function");
}

float
tlDaemon::retrieveWantedRestartTime ()
{
    rsResourceSet& rs (ConfigurationSingleton::instance() -> get() );
    rsPushNSpace nspace_tld (rs, _tldRootKey);

    // get the restartTime in hours from resources
    float wantedRestartTime;
    if (rs.hasKey (_restartTimerKey) )
    {
        rs.getValue(_restartTimerKey, wantedRestartTime);
    }
    else // no resource defined for this key
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::retrieveWantedRestartTime :  [" << _restartTimerKey << "] not defined. Using default value "
                << _defaultRestartTimer << " hours");

        wantedRestartTime = _defaultRestartTimer;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::retrieveWantedRestartTime : wanted restart time is " << wantedRestartTime << " hours");

    return wantedRestartTime;
}

void
tlDaemon::initRestartTimer()
{
    // This is used to set the wantToRestart timer the first time
    // At every configuration reload, the configureTimers method
    // will apply the right policy to the new settings

    float wantedRestartTime = retrieveWantedRestartTime();
    unsigned int wantedRestartTimeSec = rint (wantedRestartTime * 3600);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::initRestartTimer : restart timer set to " << wantedRestartTime << " hours");
    wantToRestart (wantedRestartTimeSec);
}

void
tlDaemon::configureTimers ()
{
    float wantedRestartTime = retrieveWantedRestartTime();

    unsigned int wantedRestartTimeSec = rint (wantedRestartTime*3600);
    unsigned int timeToPhoenix;

    // if have to wait less than the wanted time do nothing
    // set the restartTimer only if the time to the next restart
    // is greater than the wanted one
    if (timeToRestart (timeToPhoenix) > wantedRestartTimeSec)
    {
        wantToRestart (wantedRestartTimeSec);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::configureTimers : restart timer set to " << wantedRestartTime << " hours");
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::configureTimers : restart timer remainder is " << timeToPhoenix << " seconds");
    }

    // now setup interval timer for configuration reload
    rsResourceSet& rs (ConfigurationSingleton::instance() -> get() );
    rsPushNSpace nspace_tld (rs, _tldRootKey);

    float wantedReloadPeriod;
    if (rs.hasKey (_reloadTimerKey) )
    {
        rs.getValue(_reloadTimerKey, wantedReloadPeriod);
    }
    else
    {
        ACS_LOG_WARNING("Configuration reload timer resource [" << _reloadTimerKey
                << "] not defined. Using default value [" << _defaultReloadTimer << "]");
        wantedReloadPeriod = _defaultReloadTimer;
    }

    int timerFreq = rint (wantedReloadPeriod) ;
    if (timerFreq>0)
    {
        ACS_LOG_INFO("Reload configuration timer set to " << timerFreq << " minutes");
    }
    else
    {
        ACS_LOG_INFO("Thin Layer Daemon: no configuration reload timer set");
        timerFreq=0;
    }

    // the period of the timer is in seconds
    timerFreq=timerFreq*60;

    struct itimerval value;
    value.it_value.tv_sec = timerFreq;   // seconds - the first attempt (zero means no activity)
    value.it_value.tv_usec = 0;		// microseconds
    value.it_interval.tv_sec = timerFreq;	// seconds
    value.it_interval.tv_usec = 0;		// microseconds

    if (_reloadTimer.setitimer(ITIMER_REAL, &value, NULL) != 0)
    {
        ACS_LOG_WARNING("Cannot set the interval timer for configuration reload; errno=" << errno);
    }
}

static const char* namespaces[] =
{
        "tlDaemon",
        "OrderTypeManager",
        "dbBroadcast",
        "StationConf"
};




bool
tlDaemon::loadTLConfiguration(const string& aPath, bool* warning)
{

    bool warningEvent = false;

    exFMessage msg;

    // copy the content of the configuration singleton in order to detect the
    // configuration spaces that have been changed in the meanwhile
    rsResourceSet oldRs;
    oldRs = ConfigurationSingleton::instance()->get();

    bool configurationChanged = false;

    for (size_t i=0; i<sizeof(namespaces)/sizeof(namespaces[0]); i++)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::loadTLConfiguration: adding configuration space " << namespaces[i]);

        // adding the configuration file to the singleton
        if (! ConfigurationSingleton::instance()->addSpace (ConfigurationSingleton::ConfSpace (namespaces[i]), ConfigurationSingleton::NoLoad))
        {
            // log if and only if the configuration has never been loaded (it is the true add action)
            ACS_LOG_INFO("Thin Layer Daemon: adding configuration space " << namespaces[i]);

        }
    }

    // Force reload of any configuration space added to the singleton
    try
    {
        Application::loadConfiguration ();
        if (ConfigurationSingleton::instance()->getReplacements().size())
        {
            // some replacements occurred
            // notify it in the log with the list of the replacements
            exFWarning wmsg;

            for (unsigned long i =0; i < ConfigurationSingleton::instance()->getReplacements().size(); i++)
            {
                ACS_LOG_WARNING("Configuration space replacement: \"" << ConfigurationSingleton::instance()->getReplacements()[i].first
                        << "\" with \""
                        << ConfigurationSingleton::instance()->getReplacements()[i].second << "\"");
            }

            // this is a warning for the status
            warningEvent = true;
        }
    }
    catch (exException& ex)
    {
        ACS_LOG_WARNING("Thin Layer Daemon: WARNING: cannot load configuration space files. Resetting original configurations.");
        ACS_LOG_NOTIFY_EX(ex);
        ConfigurationSingleton::instance()->get() = oldRs;
        warningEvent = true;
    }


    // load now any settings passed via command line
    // and merge them in the private configuration

    if (aPath != "" && aPath.substr(0,1) != "-" )
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon: loading command line configuration file " << aPath);

        try
        {
            rsResourceSet cmdLineSet ;
            XMLIstream  xmlIstream(aPath);
            xmlIstream >> cmdLineSet;

            ConfigurationSingleton::instance()->getPrivate().mergeMode(rsResourceSet::ModeChange) ;
            ConfigurationSingleton::instance()->getPrivate().merge(cmdLineSet) ;
            ConfigurationSingleton::instance()->patchResourceSet();
        }
        catch(exception& ex)
        {
            ACS_LOG_WARNING("Thin Layer Daemon: WARNING: cannot load configuration file " << aPath << " ; going with existing settings");
            ACS_LOG_NOTIFY_EX(ex);

            // this is a warning for the status
            warningEvent = true;
        }

    }



    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::loadTLConfiguration: configuration spaces loaded");


    if ( !oldRs.isEqual(ConfigurationSingleton::instance()->get()))
    {
        ACS_LOG_INFO("Thin Layer Daemon: Configuration updated");
        configurationChanged=true;
    }


    if (warning)
    {
        *warning = warningEvent;
    }

    return configurationChanged;


}

bool
tlDaemon::loadWSConfiguration()
{
    rsResourceSet& rs( ConfigurationSingleton::instance() -> get() );

    // retrieve WS cfg file pathname from the configuration and create the WSConfig object
    {
        string s;
        {
            rsPushNSpace nspace_tld (rs, _tldRootKey);
            rs.getValue(_cfgPathKey, s);
        }

        // avoid creating a new WSConfig object during the reload of the configuration spaces
        if (_wsConfiguration)
            _wsConfiguration->load(s);
        else
            _wsConfiguration = new WSConfig(s);

        if (!_wsConfiguration->hasChanged())
        {
            return false;
        }

        ACS_LOG_INFO("Thin Layer Daemon: loading ws configuration file " << s );

    }




    // Prepare message for MCF showing configured processors

    //XML Validation settings are in conf space -hostname- and tlDaemon
    bool doTaskTableValidation(false);
    const string doTaskTableValidationKey(string(_tldRootKey)+"."+_tlDoTaskTableValidation);
    if ( rs.hasKey(doTaskTableValidationKey) ) rs.getValue(doTaskTableValidationKey, doTaskTableValidation);
    {
        ACS_LOG_INFO("Thin Layer Daemon: schema validation: "<<boolalpha<<doTaskTableValidation);
    }


    WSConfigMessage wsCfgMsg (*_wsConfiguration);
    for (vector<WSConfigMessageElement>::iterator it = wsCfgMsg.begin(); it!=wsCfgMsg.end(); ++it)
    {
        string tasktable = it->taskTable;
        it->status = WSConfigMessageElement::OK;
        if (!File::exists (tasktable))
        {
            it->status = WSConfigMessageElement::NO_TT;
        }
        else
        {
            try
            {
                //Key to query TT factory can be built after Db Orders retrieval.
                //Here satellite and mission are unknown!!! ==> cannot query the TT Factory
                // So, to validate each task table, instead of trying a real TT construction,
                // use the xml schema / dtd validation

                XMLIstream xmlTT( tasktable );

                xmlTT.setDoExternalSchemaValidation(doTaskTableValidation);

                rsResourceSet rsTT; xmlTT>>rsTT;

                //Task Table is valid - try to get ConfigFile and DefaultConfigVers

                const string defConfIdxKey("Private_Config.Default");
                if ( rsTT.hasKey( defConfIdxKey ) ) {
                    int defConfIdx(0); 		rsTT.getValue(defConfIdxKey, defConfIdx);

                    ostringstream configFileKey;configFileKey<<"Private_Config.List_of_Cfg_Files.Cfg_File["<<defConfIdx<<"].File_Name";
                    ostringstream configVersKey;configVersKey<<"Private_Config.List_of_Cfg_Files.Cfg_File["<<defConfIdx<<"].Version";

                    string configFile("");
                    if ( rsTT.hasKey( configFileKey.str() ) ) 		rsTT.getValue(configFileKey.str(), configFile);

                    string configVers("");
                    if ( rsTT.hasKey( configVersKey.str() ) ) 		rsTT.getValue(configVersKey.str(), configVers);

                    it->defConfigFile = 	configFile;
                    it->defConfigVersion = 	configVers;
                }

                vector<string> orders;
                _wsConfiguration->getOrders(it->name , it->version, orders);
                if (orders.size() == 0)
                {
                    it->status = WSConfigMessageElement::DISABLED;
                }
            }
            catch (std::exception& ex)
            {
                it->status = WSConfigMessageElement::BAD_TT;
                it->message = ex.what();
            }
        }
    }


    // This section will dump the WSConfigMessage content to the MCF
    {
        ostringstream wstext;
        wstext << wsCfgMsg ;
        ACS_LOG_EVENT("WSCONFIG", wstext.str() );
        //excmcf << patrol_msg ;
    }

    return true;
}



void
tlDaemon::loadPlugIns()
{
    rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
    try
    {
        vector<string> pluginsPaths;
        {
            rsPushNSpace nspace_tld (rs, _tldRootKey);
            //Just one dir where all plugins are located. Means each plugins is independent by each others - (retro-compatible)
            if ( rs.hasKey(_tlPluginsPathKey) )
            {
                string pluginsPath;
                rs.getValue (_tlPluginsPathKey, pluginsPath);
                pluginsPaths.push_back(pluginsPath);
            }
            //More than one dir where plugins are located. Means plugins in dir P2 depend by plugins in dir P1. P1 must be loaded before P2.
            else
            {
                rs.getArray(_tlListOfPluginsPathKey, pluginsPaths);
            }
        }

        DirPluginSet myPluginSet ( pluginsPaths, _pluginRegExp);
        myPluginSet.load (_pluginLoader);
        if (!_pluginLoader.load())
        {
            ACS_LOG_WARNING("Thin Layer Daemon : WARNING Plugin loader failed. Some plugins have not been installed.");
        }

    }
    catch (exception& e)
    {
        string err("Error while loading plugins");
        exIOException ee(e, err);
        ACS_THROW(ee);
    }
}



void
tlDaemon::createQueueMgr()
{
    string queueMgrType;
    {
        rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
        rsPushNSpace nspace_tld (rs, _tldRootKey);
        rs.getValue(_queueMgrTypeKey, queueMgrType);
    }


    ACS_LOG_INFO("Thin Layer Daemon: queue manager type is: " << queueMgrType);

    _queueMgr = QueueMgrFactory::instance()->getQueueMgr(queueMgrType, *this);
    logRunningMode();

    configureQueueMgr();
}

void
tlDaemon::configureQueueMgr()
{
    ACS_LOG_INFO("Thin Layer Daemon: configuring queue manager");

    if (_queueMgr)
    {
        // to avoid namespace overlap, first read all parameters, then set them in the queue mgr
        int i;
        string sortCriterium;
        PriorityOrder po;

        {
            rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
            rsPushNSpace nspace_tld (rs, _tldRootKey);

            rs.getValue(_queueFrequencyKey, i);

            if (rs.hasKey (_prioritySortOrderKey) )
            {
                rs.getValue (_prioritySortOrderKey, sortCriterium);
            }
            else
            {
                ACS_LOG_WARNING("Thin Layer Daemon: could not find key [" << _prioritySortOrderKey << "]. Using default [Ascending]");
                sortCriterium = "Ascending";
            }
            po = string2PriorityOrder (sortCriterium);
            if (po == PriorityOrderUnknown)
            {
                ACS_LOG_WARNING("Thin Layer Daemon: Unknown priority order criterium [" << sortCriterium <<"]. Using default [Ascending]");
                po = PriorityOrderAscending;
            }
        }

        ACS_LOG_INFO("Thin Layer Daemon: setting queue manager timer frequency to " << i << " seconds");
        _queueMgr->setTimerFrequency(i);

        ACS_LOG_INFO("Thin Layer Daemon: setting queue manager order sorting criterium to [" << priorityOrder2String (po) << "]");
        _queueMgr -> setPriorityOrder (po);
    }
    else
    {
        ACS_LOG_WARNING("Queue manager not created yet");
    }
}


void
tlDaemon::createExecutionMgr()
{
    _executionMgr = new OrderExecutionMgr(*this, *_wsConfiguration);
    configureExecutionMgr();
}


void
tlDaemon::configureExecutionMgr()
{
    string s;
    rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
    {
        rsPushNSpace nspace_tld (rs, _tldRootKey);
        rs.getValue(_basePathKey, s);
    }

    ACS_LOG_INFO("Thin Layer Daemon: configuring execution manager");

    if (_executionMgr)
    {
        ACS_LOG_INFO("Root directory for orders working dir is " << s );
        _executionMgr->setBaseDir(s);

        _executionMgr->setWSConfig (*_wsConfiguration);

        ACS_LOG_INFO("Reload Execution Manager internal configuration" );
        _executionMgr->reloadConfiguration();

    }
    else
    {
        ACS_LOG_WARNING("Execution manager not yet created but configure called!");
    }
}


void
tlDaemon::createIpcServer()
{
    if (!_ipcServer)
    {
        _ipcServer = new TlIpcServer();
        configureIpcServer();
    }
}

void
tlDaemon::configureIpcServer()
{
    rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
    rsPushNSpace nspace_tld (rs, _tldRootKey);
    int i = _defaultIpcPort;
    if (rs.hasKey (_ipcPortKey))
    {
        rs.getValue(_ipcPortKey, i);
    }
    else
    {
        ACS_LOG_WARNING("Thin Layer Deamon: WARNING key not found [" << _ipcPortKey << "] using default value [" << _defaultIpcPort << "]");
    }

    ACS_LOG_INFO("Thin Layer Daemon: configuring IPC server");

    if (_ipcServer)
    {
        // stop the server and join - if running
        ACS_LOG_INFO("Thin Layer Daemon: stopping IPC server");
        _ipcServer->stopListening();

        // manage all pending messages
        while (_ipcServer->messagesNumber() != 0)
        {
            manageMessages();
        }

        // configure
        ACS_LOG_INFO("Thin Layer Daemon: setting IPC port to [" << i << "]");
        _ipcServer->setPort (i);

        // restart
        ACS_LOG_INFO("Thin Layer Daemon: starting IPC server");
        _ipcServer->startListening();
    }
    else
    {
        ACS_LOG_WARNING("IPC server not yet created but configure called!");
    }

}

void
tlDaemon::manageMessages()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::manageCommands: entering function");

    // WARNING - WARNING - WARNING
    // The management of the commands MUST be performed by the same
    // thread that is in charge of calling PURGE on executing order list
    // (that is the daemon main thread)
    // This will assure that the executing order referred to an orderId will
    // not be deleted during the commands processing

    // manage one command per cycle
    TlMessage theMessage;
    try
    {
        if (_ipcServer && _executionMgr && _ipcServer->popMessage(theMessage))
        {
            ACS_LOG_INFO("Thin Layer Daemon: received command [" << theMessage.cmdType() << "] for orderid# " << theMessage.orderId());

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "manageCommands - processing command [" << theMessage.cmdType() <<
                    ", " << theMessage.cmdValue() << ", " << theMessage.paramValue() << ", " << theMessage.orderId() << "]");

            ExecutingOrder* theExOrder = _executionMgr->getExecutingOrderById (theMessage.orderId());

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tlDaemon::manageCommands - [" << theMessage.cmdType() << ", " << theMessage.orderId() <<
                    "] executing order address is " << static_cast<void*>(theExOrder));

            if (theExOrder)
            {
                theExOrder->manageCommand (theMessage);
            }
            else
            {
                ACS_LOG_WARNING("Ignoring command [" << theMessage.cmdType() << "]. No ExecutingOrder found for orderId [" << theMessage.orderId() << "]");
            }
        }
    } // try
    catch (exception& ex)
    {
        // cannot process the command. Drop the message and notify the event
        ACS_LOG_WARNING("Thin Layer Daemon: WARNING exception caught while processing command [" << theMessage.cmdType() << "]");
        ACS_LOG_NOTIFY_EX(ex);
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::manageCommands: exiting function");
}


static const int polling_freq=250; // milliseconds
void
tlDaemon::run()
{
    ACS_LOG_INFO("Thin Layer Daemon: Starting");

    // The _init() call will also do the first rotateLog to open it
    // after the configuration space loading
    _init();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: executed initialisazion");

    // start queue manager (no need to start execution manager)
    _queueMgr->startQueueMgr();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: executed _queueMgr->run()");

    // now just wait
    while (getCurrentStopMode() == NoStop)
    {

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: going wait");

        Timer tperiod;
        tperiod.delay(polling_freq);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: woken up; checking Reload Configuration Flag");

        bool emitWarning = false;
        string statusReason ("");

        // EmitStatus is used to avoid too many status messages
        // and is used to emit warning messages when they occurs
        // or OK messages after a configuration reload
        bool emitStatus = false;


        if (_haveToReloadCfg)
        {
            emitStatus = true;
            reloadConfiguration( &emitWarning );
            if (emitWarning)
                statusReason = "Configuration update failed, using old configuration";
        }

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: checking OrderQueueMgr Status");

        OrderQueueMgr::OQM_Status oqmStatus = (_queueMgr->getStatus());

        if ((oqmStatus == OrderQueueMgr::OQM_ERROR1)||(oqmStatus==OrderQueueMgr::OQM_ERROR2))
        {
            // Sending ERROR status to PATROL
            // The QueueManager has provided information about the exception caused the crash in logfile.
            (TlStatusMgrSingleton::instance()->getMgr())->enterStatus (TL_ERROR);

            // Forcing TlDaemon shutdown....
            // _quitRequested=true;
            setCurrentStopMode (CleanStop);

            // Have to log the event
            //            exFError::notify ("OrderQueueManager is not running.... - shutting down", excmcf);
            ACS_LOG_ERROR("OrderQueueManager is not running.... - shutting down");

            // Update the status to critical
            Application::setStatus (Application::Critical, "OrderQueueManager not running - Shutting down");

        }
        else if (PhoenixRestart == getCurrentStopMode())
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "run : Restarting process!!");

            // restart timer has timed out
            _sigPhoenixReceived = true;
        }
        else
        {
            if (oqmStatus == OrderQueueMgr::OQM_WARNING)
            {
                // Sending the WARNING status to PATROL and continue with the clock-driven QueueManager only
                // The QueueManager has already created the log message about the warning event...
                (TlStatusMgrSingleton::instance()->getMgr())->enterStatus (TL_WARNING);

                // There is a warning... keep track of the event
                emitWarning = true;
                emitStatus = true;
                statusReason = "BroadcastReceiver not running - using clock-driven queue polling";
            }
            if (canPurge())
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: checking for completed orders");

                if (_executionMgr->purge())
                {
                    _queueMgr->wakeUp();
                }
            }
        }

        // Manage external events
        manageSignals();
        manageMessages();

        if (emitStatus)
        {
            // Update the application status flag
            Application::setStatus ( emitWarning ? Application::Warning : Application::Ok , statusReason);
        }

    } // MAIN LOOP

    // External signals might have caused exit from the main loop, but without performing the correct procedure
    manageSignals();
    manageMessages();


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: entering Wait for processing completion loop");

    while (anyActiveProcess()) //Wait for processing completion loop
    {
        Timer tperiod;
        tperiod.delay(polling_freq);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::run: purging order execution manager");

        if (canPurge())
            _executionMgr->purge();

        manageSignals();
        manageMessages();
    } // Wait for processing completion loop


    //exFMessage::notify("Thin Layer Daemon: ... exiting", excmcf);
    ACS_LOG_INFO("Thin Layer Daemon: ... exiting");
    _cleanup();

    // The last opportunity to stop the Daemon during a Phoenix
    manageSignals();

    if (getCurrentStopMode() == PhoenixRestart)
    {
        // Exit caused by the restart timer
        // Have to restart the daemon
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon: ... restarting");

        // The application will restart as soon as the main exits
    }
    else
    {
        ACS_LOG_INFO("Thin Layer Daemon: exiting the run. Exit status is " << exitStatus());
        Application::setStatus (Application::Exiting);
    }
}



bool
tlDaemon::handleOrder(const tlOrder& anOrder)
{
    ExecutingOrder* execOrder = NULL;

    if (_verbose)
    {
        ACS_LOG_INFO("Thin Layer Daemon::handleOrder: handling order " << anOrder.getOrderSignature());
    }

    // No method called from here can throw
    // all exceptions must be handled inside the calls
    // first: check if max number of parallel executing orders has been reached
    //
    if ( !_executionMgr->haveResources() )
    {
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: order " << anOrder.getOrderSignature() <<
                    " cannot be executed: max number of parallel executing order reached");
        }

        // Max number of parallel order reached - logging done by haveSlotTime method

        // ok, pause the order queue manager...
        _queueMgr->pause();

        // ... and don't do useless effort with remaining orders
        return false;
    }

    // check if we were stopped in the meanwhile
    if (getCurrentStopMode() == ImmediateStop)
    {
        return false;
    }

    // then: control if this workstation is configured to handle this type of order for this processor
    //
    if (!_executionMgr->canProcessOrder(anOrder))
    {
        // cannot process it
        // do nothing, but return true to the queue mgr to continue with other orders
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: can't process order " << anOrder.getOrderSignature() <<
                    " on node " << getNodeName());
        }

        // put it in the black list (if managed by the order queue manager)
        _queueMgr->addBlackListItem (anOrder.getOrderId());

        return true;
    }

    // check if we were stopped in the meanwhile
    if (getCurrentStopMode() == ImmediateStop)
    {
        return false;
    }

    //Check if this workstation has enough resources for this order. In the case, reserve resources
    //To allow the computation of resources this operation might fail due to exception in several points
    //In case of exception the involved order will be blacklisted and notified into the log so
    //At the next restart it will be evaluated again and started if possible
    bool gotExceptions = false;
    OrderReservedResourcesSptr orderReservedResourcesSptr = _executionMgr->reserveResourcesForOrder( anOrder, gotExceptions );
    if ( ! orderReservedResourcesSptr ) {
        // cannot reserve Resources for the order
        // do nothing, but return true to the queue mgr to continue with other orders
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: can't Reserve Resources for order " << anOrder.getOrderSignature());
        }
        if (gotExceptions)
        {
            ACS_LOG_WARNING ("Internal error managing order id #" << anOrder.getOrderId() << ". Blacklisted." );
            // put it in the black list (if managed by the order queue manager)
            _queueMgr->addBlackListItem (anOrder.getOrderId());
        }
        return true;
    }

    // From now the resources has been reserved and needs to be cleaned up.
    // Resources will be cleaned up at orderReservedResourcesSptr destructor, i.e. when its reference counter = 0;
    // In particular, the object will be owned and retained by this scope and by the ExecutingOrder (being created).
    // When ExecutingOrder and this scope ends, the resources will be automatically released (cf. OrderReservedResources destructor).

    // check if we were stopped in the meanwhile
    if (getCurrentStopMode() == ImmediateStop)
    {
        return false;
    }

    // Try to book the order and avoid other workstations to process the same order
    if (!_queueMgr->tryBookOrder(anOrder))
    {
        // cannot book it
        // probably some other workstation did before this one
        // do nothing, but return true to the queue mgr to continue with other orders
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: can't book order " << anOrder.getOrderSignature());
        }

        return true;
    }

    // check if we were stopped in the meanwhile
    if (getCurrentStopMode() == ImmediateStop)
    {
        return false;
    }

    // Request an instance of executinOrder from the executionManager
    // From now the execOrder has to be cleaned in case of failures inside the _executinionMgr called methods
    // we will only do order cleanup in case of IMMEDIATE_STOP in between calls
    if (! (execOrder = _executionMgr->createExecOrder(anOrder, orderReservedResourcesSptr)))
    {
        // cannot create it
        // do nothing, but return true to the queue mgr to continue with other orders
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: can't create ExecutingOrder for order " << anOrder.getOrderSignature());
        }

        return true;
    }

    // Check Preconditions for the order
    if ( ! (_executionMgr->checkPreconditions(execOrder)) ) // error during call
    {
        if (_verbose)
        {
            ACS_LOG_INFO("Thin Layer Daemon::handleOrder: " <<
                    "checkPreconditions" <<
                    " failed for order " << anOrder.getOrderSignature());
        }
        return true;
    }

    // APF-271
    MCScopedParams mcSlotId;

    // slotid must be a string
    ostringstream osslotid;
    osslotid << execOrder->getAssignedSlot();

    mcSlotId << mc_param("slotid", osslotid.str());


    // From now the TL is assumed to be in PROCESSING
    // The exitStatus will be called either from
    // OrderExecutionManager if the scheduler is spawned (in notifySetupFailure or notifySchedulerCompletion)
    // or
    // here if the Scheduler cannot be spawned (i.e. due to dowload failures)
    TlStatusMgrSingleton::StatusGuard processingGuard (*TlStatusMgrSingleton::instance()->getMgr(), TL_PROCESSING_ORDER);



    // Here starts roadmap for process order
    // have to complete with array of pointer to functions and iterator


    typedef bool (OrderExecutionMgr::*OEMgrMethod)(ExecutingOrder *);
    vector<OEMgrMethod> roadmap;
    vector<string> roadmap_names;
    roadmap.push_back(&OrderExecutionMgr::prepareOrder);
    roadmap_names.push_back("input download");
    roadmap.push_back(&OrderExecutionMgr::processOrder);
    roadmap_names.push_back("processing");

    for (vector<OEMgrMethod>::iterator i = roadmap.begin();
            i != roadmap.end();
            ++i)
    {
        // check if we've been stopped in the meantime
        if (getCurrentStopMode() == ImmediateStop)
        {
            _executionMgr->abortOrder(execOrder);
            return false;
            break;
        }

        // call the method pointed by the iterator
        if ( ! ((_executionMgr->*(*i))(execOrder)) ) // error during call
        {
            if (_verbose)
            {
                ACS_LOG_INFO("Thin Layer Daemon::handleOrder: " <<
                        roadmap_names[i - roadmap.begin()] <<
                        " failed for order " << anOrder.getOrderSignature());
            }

            // interrupt the loop, but return true to the caller to try going on with next order
            return true;
            break;
        }
    }

    // The Scheduler was spawned
    // the exitStatus(TL_PROCESSING) is now in charge of the OrderExecutionMgr
    processingGuard.release (false);

    return true;
}

void
tlDaemon::orderIsDone(const tlOrder& order, tlOrderStatus s) const
{
#ifdef DO_NOT_UPDATE_STATUS_ON_IMMEDIATE_STOP
    // check if we've been stopped in the meantime
    if (getCurrentStopMode() == ImmediateStop)
        return;
#endif //DO_NOT_UPDATE_STATUS_ON_IMMEDIATE_STOP

    if (!_queueMgr->registerProcessingStop(order, s))
    {
        // shouldn't happen: we have an error on the DB
        ACS_LOG_ERROR("Cannot update stop information in the database for order ID#" <<
                order.getOrderId());
        //	excmcf << msg;
    }

    // ... cleanup will be made by the parent thread during purge
}

int
tlDaemon::exitStatus() const
{
    tl_status currentStatus = ((TlStatusMgrSingleton::instance()->getMgr())-> getStatus ());
    int returnCode = 0;
    if (currentStatus == TL_ERROR) returnCode = -1;

    return returnCode;
}


void
tlDaemon::sigReloadRequestHdlr(int) throw()
{
    // debug message
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::sigReloadRequestHdlr : reload configuration files flag set");

    _haveToReloadCfg = true;
}


void
tlDaemon::catchSigTerm (int) throw()
{
    _sigTermReceived = true;
}

void
tlDaemon::catchSigInt (int) throw()
{
    _sigIntReceived = true;
}

bool
tlDaemon::canPurge()
{
    bool result = true;

    // If any of the following condition is "false" the canPurge must return "false"
    // result &= condition;

    // Purging an order while QueueManager is handling can cause the deletion of
    // the query stored in the InputDownloader while the InputDownloader is still using it
    result &= !_queueMgr->isHandling();

    return result;
}

void
tlDaemon::manageSignals()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon:: entering manageSignals");
    if (hasStopModeChanged())
    {
        switch (getCurrentStopMode())
        {
            case Application::PhoenixRestart:
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::manageSignals. New state is RESTART (sigPhoenix)");
                stopOrderExecutionMgr();
                break;
            }
            case Application::CleanStop:
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::manageSignals. New state is CLEAN_STOP (sigInt)");
                stopOrderExecutionMgr();
                break;
            }
            case Application::ImmediateStop:
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thin Layer Daemon::manageSignals. New state is IMMEDIATE_STOP (sigTerm)");
                stopOrderExecutionMgr();
                break;
            }
            case Application::NoStop:
            default:
            {
                break;
            }
        }
    }
}

void
tlDaemon::stopOrderExecutionMgr()
{
    // prepare the right stop signal for execution manager
    // mapping daemon stop mode to OrderExecutionMgr stopType value

    OrderExecutionMgr::stopType stopMode;

    // Once started, the stopping sequence is not sensible
    // to other stop signals... no filter is needed to avoid interferences
    // during the shutdown.

    switch (getCurrentStopMode())
    {
    case (ImmediateStop):
                 stopMode = OrderExecutionMgr::IMMEDIATE_STOP;
    break;
    default:
        stopMode = OrderExecutionMgr::CLEAN_STOP;
    }


    // stop the execution manager
    //
    ACS_LOG_INFO("Thin Layer Daemon: stopping execution manager");
    if (_executionMgr)
        _executionMgr->stop(stopMode);
}

bool
tlDaemon::anyActiveProcess()
{
    bool result = false;
    if (_queueMgr)
        result = _queueMgr->isHandling();
    if (_executionMgr)
        result |= _executionMgr->isProcessing();
    return result;
}


void
tlDaemon::_cleanup()
{
    // stop the timer and join its thread
    _reloadTimer.stopTimer();

    // stop the order queue manager
    //
    ACS_LOG_INFO("Thin Layer Daemon: stopping queue manager");
    _queueMgr->stopQueueMgr();


    // stop the execution manager
    //
    ACS_LOG_INFO("Thin Layer Daemon: cleaning up execution manager");
    _executionMgr->cleanup();

    // stop the IPC server
    ACS_LOG_INFO("Thin Layer Daemon: stopping IPC server");
    _ipcServer->stopListening();

    // finally, perform any specific cleanup
    cleanup();
}

string tlDaemon::getNodeName()
{
    return hostname();
}

