// PRQA S 1050 EOF
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
    Revision 3.0  2016/10/11 08:27:16  marfav
    Major release due to cache management modifications

    Revision 2.15  2016/07/05 13:11:54  marfav
    CSGACQ-84
    Using the application based log rotation policy

    Revision 2.14  2015/12/01 15:49:53  marfav
    Fixing metric types

    Revision 2.13  2015/11/27 17:38:07  marpas
    coding best practice application in progress

    Revision 2.12  2015/11/19 17:23:03  marpas
    coding best practices application in progress

    Revision 2.11  2015/04/30 15:49:17  marfav
    Compiler warning removed

    Revision 2.10  2013/03/14 15:05:39  marpas
    connection pool concept is left to class inheriting from Application

    Revision 2.9  2013/03/08 18:33:28  marfav
    Using connection pool

    Revision 2.8  2013/01/11 11:40:49  marfav
    Addedd support to external appname definition

    Revision 2.7  2012/02/09 17:26:24  marpas
    refactoring in progress

    Revision 2.6  2011/10/13 13:02:53  marfav
    Implementing status messages as per S1PDGS-1902

    Revision 2.5  2011/01/24 16:21:06  marpas
    Mutex.h does not longer exists

    Revision 2.4  2008/09/18 13:25:00  marfav
    Added support to HAS_MISSION_INCLUDES flag

    Revision 2.3  2008/04/22 13:35:28  marfav
    Aligned to the task table conditioned visitor design pattern
    Exception management improved

    Revision 2.2  2008/04/10 16:04:12  integrator
    Added ifdef for SRV project

    Revision 2.1  2007/04/19 08:41:30  giucas
    Removed dependencies from StationApp for ENVISAT

    Revision 2.0  2006/02/28 17:02:07  paoscu
    Exception 2.1 I/F adopted

    Revision 1.29  2005/03/18 10:27:01  marfav
    excmcf support added

    Revision 1.28  2004/12/03 17:24:47  marfav
    Header Fixed

    Revision 1.27  2004/09/21 08:16:04  marfav
    SigalrmEmulator moved in namespace acs

    Revision 1.26  2004/08/30 12:54:10  marfav
    Support for Cosmo environment added

    Revision 1.25  2004/08/24 14:50:31  marfav
    Added support for DirPluginSet

    Revision 1.24  2004/04/22 07:24:55  integrator
    CalVal Environment support added

    Revision 1.23  2004/04/07 13:19:56  marfav
    Added support for IPC from ThinLayer Clients and to running Schedulers
    Does not use ACE anymore
    Using exStream.h

    Revision 1.22  2004/03/11 19:10:29  marfav
    Preparing to manage external commands
    Align to new version of Scheduler Engine (libScheduler)

    Revision 1.20  2003/10/03 11:35:21  marfav
    Now all threads have SIGINT and SIGTERM blocked (from libThread)
    No need to block the signals during all the run to avoid threads problems

    Revision 1.19  2003/09/25 09:33:37  marfav
    Removed support for internal order queue
    Using references from ConfigurationSingleton

    Revision 1.18  2003/09/18 15:36:15  marfav
    Phoenix support fixed

    Revision 1.17  2003/09/08 15:50:55  marfav
    Using ApplicationLock class
    Added running mode messages to the log

    Revision 1.16  2003/09/05 16:08:25  marfav
    Added support for ENVISAT environment

    Revision 1.15  2003/08/01 11:03:30  marfav
    Added support for thread Randez-Vous and InternalOrderQueue

    Revision 1.14  2003/07/29 10:45:00  marfav
    Reload configuration support at run-time

    Revision 1.13  2003/07/10 14:18:32  marfav
    clone method added to exceptions

    Revision 1.12  2003/07/04 15:06:33  marfav
    Using Sigalarm Emulator and implementing restart

    Revision 1.11  2003/06/06 10:31:04  marfav
    Dead code removed

    Revision 1.9  2003/04/18 12:59:33  marfav
    Using namespaces removed from .h

    Revision 1.8  2003/03/27 15:33:22  marfav
    Added plugin support and some logging improvements

    Revision 1.7  2003/03/24 16:35:26  marfav
    Included JobOrderFactory support

    Revision 1.6  2003/03/11 18:25:50  marfav
    Patrol message handling added

    Revision 1.5  2003/02/18 09:09:22  fracar
    improved logging

    Revision 1.4  2003/02/10 09:17:48  fracar
    improved logging

    Revision 1.3  2003/02/04 16:45:09  fracar
    improved messaging

    Revision 1.2  2003/02/04 09:58:34  marfav
    Added LOG field to headers



*/


#ifndef _TL_DAEMON_H
#define _TL_DAEMON_H

#include <SigalrmEmulator.h>
#include <TlIpcServer.h>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifdef HAS_MISSION_INCLUDES
#include <ProjectAppIncludes>
#elif COSMO
#include <CosmoApp.h>
#elif CAL_VAL_APP
#include <CalValApp.h>
#elif SRV_APP
#include <SrvApp.h>
#else
#include <CryosatApp.h>
#endif

#include <ConfigurablePluginLoader.h>
#include <tlOrder.h>
#include <rsResourceSet.h>
#include <exException.h>
#include <exStream.h>
#include <exFilterable.h>

#include <ThreadSafe.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)


class OrderQueueMgr;
class OrderExecutionMgr;
class WSConfig;

class tlDaemon : public // PRQA S 2109, 2153
#ifdef HAS_MISSION_INCLUDES
    PROJECT_APP
#elif COSMO
    CosmoApp
#elif CAL_VAL_APP
    CalValApp
#elif SRV_APP
    SrvApp
#else
    CryosatApp
#endif

{
  public:
    /*! class tlDaemonException declaration */
    exDECLARE_EXCEPTION(tlDaemonException, exException) ; // PRQA S 2131, 2502

  public:

    tlDaemon (const std::string &appName, const std::string &subSystem);
    virtual ~tlDaemon() throw() ;

    virtual int main(int argc, char const * const * argv, char const * const * env);

    void verbose(bool v);
    bool verbose() const;

    void performBookedOrder();
    void inputFileIsReady();
    void run();
    bool handleOrder(const tlOrder& anOrder);

    void orderIsDone(const tlOrder& order, tlOrderStatus s) const;
    int exitStatus() const;

    bool updateOrderStatus(const tlOrder&, const tlOrderStatus, bool notifyOnError=true);
    bool registerProcessingStart(const tlOrder& anOrder);
    bool registerProcessingStop(const tlOrder& anOrder, tlOrderStatus status);
    bool registerDownloadStart (const tlOrder& anOrder);
    void rendezVous();

    // Derived from Application
    virtual std::string getDefaultLogName() const;

  protected:
    void init() {}	// specific initialisation for derived classes
    void cleanup() {}	// specific cleanup for derived classes

  private: // methods
    void _init();
    void _cleanup();
    bool loadTLConfiguration(const std::string& aPath, bool* warning);
    bool loadWSConfiguration();
    void loadPlugIns();
    float retrieveWantedRestartTime();
    void createQueueMgr();
    void createExecutionMgr();
    void configureQueueMgr();
    void initRestartTimer();
    void configureTlDaemon();
    void configureLog();
    void configureTimers();
    void reloadConfiguration(bool* warning);
    void configureExecutionMgr();
    void configureFacilities();
    void restartFacilities();
    void logRunningMode();
    void createIpcServer();
    void configureIpcServer();
    void manageMessages();

    virtual void catchSigTerm(int) throw() ;
    virtual void catchSigInt(int) throw() ;
    void manageSignals();
    void stopOrderExecutionMgr();
    bool anyActiveProcess();
    bool canPurge();


  tlDaemon& operator = (const tlDaemon&); // not implemented
  tlDaemon (const tlDaemon&); // not implemented
  tlDaemon (); // not implemented


  private: // attributes
    ThreadSafe _countMutex; // this will ensure consistency of the _rendezVousThreads counter
    ThreadSafe _cfgMutex; // needs this to handle configuration changes on-the-fly
    unsigned int _rendezVousThreads;  // counts the number of threads that are waiting the shared resource (_cfgMutex)
    SigalrmEmulator _reloadTimer;  // will signal the need to reload the configuration files

    std::string _myCfgPath;
    WSConfig *_wsConfiguration;
    OrderQueueMgr *_queueMgr;
    OrderExecutionMgr *_executionMgr;
    TlIpcServer* _ipcServer;

    bool _verbose; // instructs the daemon to log extra messages about order execution
                   // set by the Interactive Order Manager, which needs some messages that
                   // normally are debug

    // The loader must have the same lifetime of the application
    ConfigurablePluginLoader _pluginLoader;
    bool _sigTermReceived;
    bool _sigIntReceived;
    bool _sigPhoenixReceived;  // Not set by a signal handler. The sigPhoenix is not a "physical" signal
                               // but it is checked by manageSignals() just like sigInt and sigTerm


private: // static stuff...
    static std::string getNodeName();
    static void sigReloadRequestHdlr(int) throw() ;
    static bool _haveToReloadCfg;
    ACS_CLASS_DECLARE_DEBUG_LEVEL (tlDaemon)
};

_ACS_END_NAMESPACE


#endif // _TL_DAEMON_H
