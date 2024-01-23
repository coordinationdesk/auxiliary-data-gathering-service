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

    $Prod: A.C.S Inventory application running as Daemon $

    $Id$

    $Author$

    $Log$
    Revision 5.5  2016/07/08 13:02:09  marfav
    builder adopted

    Revision 5.4  2016/07/08 12:33:02  marfav
    CSGACQ-84
    New project/application based log rotation management strategy enforced

    Revision 5.3  2016/05/11 11:45:56  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 5.2  2015/10/23 10:32:29  marpas
    moving some filterables from info class to private info one

    Revision 5.1  2014/09/19 10:21:11  marfav
    Added support to configurable restart time

    Revision 5.0  2013/06/20 07:51:40  marpas
    implementing SENT-37 about messaging
    rationalization, streams redirection and chaining fixed
    adopting libException 5.x standards
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 1.12  2013/05/24 18:34:28  nicvac
    S2PDGS-308: Inventory notifications implemented.

    Revision 1.11  2013/03/14 15:29:54  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.10  2013/03/12 18:00:25  marpas
    FileInventory class now uses dbConnectionPool

    Revision 1.9  2013/02/21 19:20:30  marpas
    dbConnectionPool used

    Revision 1.8  2012/03/01 13:06:43  marpas
    base class deal with -dbconf

    Revision 1.7  2012/02/09 17:45:40  marpas
    refactoring in progress

    Revision 1.6  2010/06/14 15:27:40  marpas
    HAS_MISSION_INCLUDES management added

    Revision 1.5  2009/12/03 13:45:59  marpas
    minor changes in diagnostics and log messages

    Revision 1.4  2008/11/13 12:40:55  marpas
    new ConfigurationSingleton approach

    Revision 1.3  2008/09/04 14:18:25  davcas
    Addded condition on execution time (> 5 seconds) to send message to MCF

    Revision 1.2  2008/08/28 08:07:26  giucas
    Problem with application builder fixed

    Revision 1.1  2008/08/27 14:24:59  davcas
    Added, derived from InventoryApp


*/

#include <InventoryDaemon.h>
#include <DateTime.h>
#include <FileInventory.h>
#include <ConfigurationSingleton.h>
#include <ApplicationLock.h>
#include <File.h>
#include <ChDir.h>

#include <Filterables.h>
#include <sstream>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef INVENTORYDAEMON_SUBSYSTEM
#define INVENTORYDAEMON_SUBSYSTEM "SDF"
#endif

#ifndef INVENTORYDAEMON_APPNAME
#define INVENTORYDAEMON_APPNAME "InventoryDaemon"
#endif



_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//static InventoryV vers;



ACS_CLASS_DEFINE_DEBUG_LEVEL(InventoryDaemon)

namespace {
    Application::GenericBuilder<InventoryDaemon> theApplicationBuilder(INVENTORYDAEMON_APPNAME, INVENTORYDAEMON_SUBSYSTEM) ;
}

/*
 * Class constructor
 */
InventoryDaemon::InventoryDaemon(const string &appName, const string &subSystem):
    PROJECT_APP (appName, subSystem),

    _mainThread(pthread_self()),
    _failedOrders(""),
    _targetDir(""),
    _backupDir(""),
    _restartTime(86400) // PRQA S 4400
{
}



/*
 * Class destructor
 */
InventoryDaemon::~InventoryDaemon()  throw() { }

void InventoryDaemon::customSigTermHandler() // PRQA S 4020
{
    if (pthread_self() != _mainThread) {
        return ;
    }

    ACS_LOG_INFO("InventoryDaemon: SIGTERM caught") ;
    ACS_LOG_INFO("InventoryDaemon: Stop scheduled") ;
}

void InventoryDaemon::customSigIntHandler()  // PRQA S 4020
{
    if (pthread_self() != _mainThread) {
        return ;
    }

    ACS_LOG_INFO("InventoryDaemon: SIGINT caught") ;
    ACS_LOG_INFO("InventoryDaemon: Stop scheduled") ;

}

int InventoryDaemon::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
    // only one Inventory daemon for host
    ApplicationLock appLock(1) ;
    appLock.lock();

    if (&exostream::getDefaultOutLogger() != &excmcf) {
        exostream::getDefaultOutLogger().appendChild(&excmcf) ;
    }

    // open the first log
//	rotateLog() ;

       DateTime curDateTime ;
    string tmpCurDate ;
    ACS_LOG_INFO("***********  Logfile for Application " << applicationName() << ". Created on: " << curDateTime.utcs(tmpCurDate) << "  ***********") ;
    ACS_LOG_INFO("Filename: " << getLRPolicy().getCurrentLogName() ) ;

    // DEMONIZE
    daemonize();


    // Start getting the configuration (ONLY ONCE) but retry until success
    unsigned int loadRetry = 0;
    while( ! requestedStop() )
    {
        try
        {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"in starting loop, loading configuration") ;
             // Get configuration once
            getConfiguration();
            rotateLog() ;
            break;
        }
        catch (exception &e)
        {
            ACS_LOG_WARNING("Problem loading configuration. Caused by:" << e << "\nKeep Trying") ;
        }
        // Try again
        loadRetry++;
        if(loadRetry >= 3) // PRQA S 4400
        {
            ACS_THROW(exCriticalException("Problem loading configuration. Maximum number of retries reached.")) ; // PRQA S 3081
        }

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"in starting loop, waiting: " << 30+(30*loadRetry)%600 // PRQA S 3084
                   << " [s] before retrying.") ;
        waitStopOrTimeout(30000+(30000*loadRetry)%600000);	// PRQA S 3084, 4400
    }

      // program phoenix
    wantToRestart(_restartTime);
    ACS_LOG_INFO ("Application will restart in " << _restartTime << " seconds");


    // Create temporary EMPTY working directory and go there
    string dirName;
    {
        char templateName[] = "/tmp/InventoryDeamon.XXXXXX" ;

        if(mkdtemp(templateName) == 0)
        {
            ACS_THROW(exCriticalException( "Problem creating temporary working dir under /tmp")); // PRQA S 3081
        }
        dirName = templateName ;
    }
    ChDir* wkDir = new ChDir(dirName, true);

    // MAIN LOOP
    while( !requestedStop() )
    {
        // rotate log if needed
        rotateLog() ;

        // Notify to MCF that process is started
        ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_BUSY);
        try
        {
            startInventory();
        }
        catch(exception& e)
        {
            ACS_LOG_NOTIFY_EX(e) ;

            // Notify to MCF that process is ended in error
            ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_ERROR);
            ostringstream os ;
            os << "ERROR." << _failedOrders <<  " SEE LOG " << getLRPolicy().getCurrentLogName() ;
            try {
                ACS_THROW(exCriticalException(e, os.str())); // PRQA S 3081
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX(x);
            }
        }

        // Notify to MCF that process is ended
        ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_IDLE);

        // wait main loop polling time
        waitStopOrTimeout(10000);
    } // end main loop

    delete wkDir ; 	/* pop to previous directory */
    File::rmdir(dirName) ;	/* remove temporary directory */

    if (PhoenixRestart == getCurrentStopMode())
    {
        ACS_LOG_INFO("Time to restart! Application will restart in 5 [sec]") ;
        waitStopOrTimeout(5000); // prevent race conditions // PRQA S 4400
        return 0;
    }

    if(! requestedStop())
    {
        // HERE IF: No restart, neither user wants to quit -- SO, AN ERROR OCCURRED!!!!!!!!!!
        ACS_LOG_INFO("Application will exit in 5 [sec]")  ;
        waitStopOrTimeout(5); // prevent race conditions // PRQA S 4400
        return -1;
    }

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods
/////////////////////////////////////////////////////////////////////////////////////////


void InventoryDaemon::startInventory()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called ...") ;

    time_t startProcessing = time(0);

    FileInventory inv(*getDefaultConnectionPool()) ;
    try {
        inv.makeInventory(_targetDir,_backupDir, "");
    }
    catch(...) {
        // note, this is to return the failed orders list.
        _failedOrders = inv.failedOrders() ;
        throw ;
    }
    time_t stopProcessing = time(0);

    if(stopProcessing - startProcessing > 5) // PRQA S 4400
    {
        int minExecution = (stopProcessing - startProcessing)/60; // PRQA S 3010
        ostringstream msg;
        ACS_LOG_PRIVINFO("Inventory executed in " << minExecution << " minutes: "
            << ((stopProcessing - startProcessing) - minExecution*60) << " seconds") ;
    }
}


void InventoryDaemon::getConfiguration()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called...") ;


    addSpace(ConfigurationSingleton::ConfSpace("Inventory")); // PRQA S 3081 3
    addSpace(ConfigurationSingleton::ConfSpace("InventoryEmailNotification"));
    addSpace(ConfigurationSingleton::ConfSpace("EmailUserNotification"));
    loadConfiguration() ;

    ConfigurationSingleton::instance()->get().getValue("Inventory.WorkingDir" , _targetDir);
    ConfigurationSingleton::instance()->get().getValue("Inventory.BackupDir" , _backupDir);

    if ( !ConfigurationSingleton::instance()->get().getValue("InventoryDaemon.RestartTime", _restartTime, false))
    {
        ACS_LOG_WARNING ("Unable to read configuration InventoryDaemon.RestartTime using default " << _restartTime << " [sec]");
    }


    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed") ;
}

_ACS_END_NAMESPACE
