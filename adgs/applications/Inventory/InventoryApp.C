// PRQA S 1050 EOF
/*

    Copyright 1995-2020, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod:

    $Id$

    $Author$

    $Log$
    Revision 5.10  2016/07/08 12:33:02  marfav
    CSGACQ-84
    New project/application based log rotation management strategy enforced

    Revision 5.9  2015/10/23 10:32:29  marpas
    moving some filterables from info class to private info one

    Revision 5.8  2015/02/04 14:58:58  marfav
    S2PDGS-1072 adding support to options noactions and restoreonsync

    Revision 5.7  2015/02/02 16:08:50  marfav
    S2PDGS-1072 upload without triggers implemented

    Revision 5.6  2015/01/30 17:23:06  marfav
    S2PDGS-976 managing restore on sync storage managers

    Revision 5.5  2014/03/13 11:15:16  nicvac
    S2PDGS-737: disabled log rotation from configuration.

    Revision 5.4  2013/09/27 14:45:53  marpas
    streams have name

    Revision 5.3  2013/09/13 07:29:38  chicas
    loadConfiguration notifyReplacement set to false

    Revision 5.2  2013/07/09 14:19:52  chicas
    Message improved

    Revision 5.1  2013/07/09 14:04:05  chicas
    Added -logdir option

    Revision 5.0  2013/06/20 07:51:40  marpas
    implementing SENT-37 about messaging
    rationalization, streams redirection and chaining fixed
    adopting libException 5.x standards
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 2.23  2013/05/24 18:34:28  nicvac
    S2PDGS-308: Inventory notifications implemented.

    Revision 2.22  2013/03/14 15:29:54  marpas
    connection pool concept is left to class inheriting from Application

    Revision 2.21  2013/03/12 18:00:25  marpas
    FileInventory class now uses dbConnectionPool

    Revision 2.20  2013/02/21 19:20:30  marpas
    dbConnectionPool used

    Revision 2.19  2012/03/06 13:30:25  marpas
    usage fixed

    Revision 2.18  2012/02/09 17:45:40  marpas
    refactoring in progress

    Revision 2.17  2011/07/14 13:21:18  lucia.galli
    removed small bug in relaxfncheck option

    Revision 2.16  2011/06/24 11:52:34  chicas
    bug fixed for  _relaxFnCheck setting

    Revision 2.15  2011/06/20 10:26:13  marpas
    -relaxfncheck && --relaxfncheck arguments can be used to relax the check on files number for directory products.

    Revision 2.14  2011/03/21 18:36:00  marpas
    on duplicate files returns 123

    Revision 2.13  2011/03/21 18:28:55  marpas
    VEN-1993 management in progress: no critical exception if a duplicate was found and -noCriticalOnDuplicates arg passed

    Revision 2.12  2010/06/14 15:27:40  marpas
    HAS_MISSION_INCLUDES management added

    Revision 2.11  2009/12/04 14:18:43  marpas
    exit status changed into 255 in case of error.
    Message about exit status added

    Revision 2.10  2009/12/03 13:45:59  marpas
    minor changes in diagnostics and log messages

    Revision 2.9  2009/09/04 11:11:13  marpas
    no .log added if the name is given with -logname option

    Revision 2.8  2009/09/04 11:00:03  marpas
    Inventory takes [[-]-logname <logname>] arguments to force a log filename without path

    Revision 2.7  2009/09/04 10:52:05  marpas
    log filename changed: now is in the form Inventory_YYYYMMDDThhmmss.uuuuuu.log

    Revision 2.6  2009/02/12 18:14:40  marfav
    Argument recognition fixed

    Revision 2.5  2009/02/12 11:04:11  marpas
    facility fixed

    Revision 2.4  2009/02/12 11:00:53  marpas
    arguments recognition improved

    Revision 2.3  2008/11/13 12:40:55  marpas
    new ConfigurationSingleton approach

    Revision 2.2  2006/05/05 08:27:22  giucas
    final version

    Revision 2.1  2006/05/04 17:59:54  marpas
    builder used instead of global variable

    Revision 2.0  2006/02/28 11:52:50  paoscu
    Exception 2.1 I/F adopted

    Revision 1.29  2006/01/10 19:58:49  marpas
    conf space replacement check in correct position: AFTER the getResources call

    Revision 1.28  2005/11/22 11:21:23  marpas
    return bug fixed

    Revision 1.27  2005/11/22 10:36:09  enrcar
    Bug Fixed. Exit code forced to be !=0 in case of exception

    Revision 1.26  2005/09/29 19:23:42  marpas
    stream management improved

    Revision 1.25  2005/09/29 16:33:39  marpas
    make up and list of failed orders (if any)

    Revision 1.24  2005/09/28 14:17:43  marpas
    Getting rid of ArfConfiguration

    Revision 1.23  2005/09/13 15:05:59  davcas
    Added CriticalException notification to excmcf if main exits in error

    Revision 1.22  2005/09/13 15:02:58  davcas
    Added CriticalException notification to excmcf if main exits in error

    Revision 1.21  2005/09/13 14:19:33  davcas
    Added CriticalException thrown if main exits in error

    Revision 1.20  2005/04/04 13:57:25  ivafam
    Commented add Patrol

    Revision 1.19  2005/03/17 16:16:08  giucas
    Modified to send messages to excmcf

    Revision 1.18  2004/12/03 11:18:45  giucas
    Header fixed

    Revision 1.17  2004/10/15 15:08:13  seralb
    Bug solved in -overwrite option

    Revision 1.16  2004/10/14 15:02:01  seralb
    overwrite option added

    Revision 1.15  2004/09/07 13:49:09  marpas
    Makefile.Station removed
    supported projects Cosmo, Cryosat, Envisat

    Revision 1.14  2004/09/06 08:40:50  danalt
    Cosmo MultiMission Environment Support Added

    Revision 1.13  2004/08/06 13:39:07  envisat
    Added print of Exception in main

    Revision 1.12  2004/05/07 12:19:22  seralb
    File catalog gestion

    Revision 1.11  2003/09/04 09:02:06  seralb
    Bug on log name solved

    Revision 1.10  2003/09/03 15:05:44  seralb
    Bug solved

    Revision 1.9  2003/09/03 14:02:11  seralb
    Upgrade

    Revision 1.8  2003/08/28 15:38:34  giucas
    Added message on use of Message Queue

    Revision 1.7  2003/08/22 14:21:38  giucas
    Moved execution of getConfiguration method befor sending messages to Patrol

    Revision 1.6  2003/07/17 15:09:41  seralb
    Improvement

    Revision 1.5  2003/07/09 12:26:31  seralb
    Improvement

    Revision 1.4  2003/05/22 11:50:07  seralb
    Patrol upgrade

    Revision 1.3  2003/03/14 14:49:19  giucas
    Updated stop time

    Revision 1.2  2003/03/03 15:04:26  marpas
    boh (makefile fixed)

    Revision 1.1.1.1  2003/01/20 13:57:10  seralb
    Import Inventory



*/

#include <InventoryApp.h>
#include <FileInventory.h>
#include <DateTime.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <File.h>

#include <Filterables.h>
#include <sstream>
#include <InventoryV.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace { // unnamed
    InventoryV vers;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(InventoryApp)

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef INVENTORY_SUBSYSTEM
#define INVENTORY_SUBSYSTEM "SDF"
#endif

#ifndef INVENTORY_APPNAME
#define INVENTORY_APPNAME "InventoryApp"
#endif

namespace {
    Application::GenericBuilder<InventoryApp> theApplicationBuilder(INVENTORY_APPNAME, INVENTORY_SUBSYSTEM) ;
}

/*
 * Class constructor
 */
InventoryApp::InventoryApp(const string &appName, const string &subSystem):
    PROJECT_APP (appName, subSystem),
    _option(""),
    _failedOrders(),
    _noCriticalOnDuplicates(false) ,
    _relaxFnCheck(false),
    _restoreOnSync(false),
    _noActions(false),
    _noBackup(false)
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB," Id: " << vers.name() ) ;

}


int InventoryApp::usage ( const std::string & exe) const
{
    excerr << "Use: \n" ;
    excerr << File::getFileName(exe) << FileInventory::usage()
            << "\t--nobackup will disable backup even if backup directory is configured\n"
            << '\n';
    return 1 ;
}


/*
 * Class destructor
 */
InventoryApp::~InventoryApp() throw() {}


string InventoryApp::getDefaultLogName() const
{
    string logName ;
    {
        DateTime now ;
        ostringstream os ;
        now.asISO8601(os,6) ; // PRQA S 4400
        logName = os.str() ;
        // remove ':'
        string::iterator new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),':')) ;
        logName.erase(new_end, logName.end());

        // remove '-'
        new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),'-')) ;
        logName.erase(new_end, logName.end());
    }

    ostringstream theName;
    theName << defaultLogPath << "/Inventory_" << logName << ".log";

    return theName.str();
}

int InventoryApp::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
    string overWrite1Arg = "-overwrite" ;
    string overWrite2Arg = "--overwrite" ;
    string replaceFile1Arg = "-replacefile" ;
    string replaceFile2Arg = "--replacefile" ;
    string noCriticalOnDuplicates1 = "-noCriticalOnDuplicates" ;
    string noCriticalOnDuplicates2 = "--noCriticalOnDuplicates" ;
    string logname1Arg = "-logname" ;
    string logname2Arg = "--logname" ;
    string relaxfncheck = "--relaxfncheck" ;
    string relaxfncheck2 = "-relaxfncheck" ;
    string logdir1Arg = "-logdir" ;
    string logdir2Arg = "--logdir" ;

    string restoreSync1Arg = "-restoreonsync";
    string restoreSync2Arg = "--restoreonsync";

    string noTriggers1Arg = "-noactions";
    string noTriggers2Arg = "--noactions";

    const string noBackup1Arg = "-nobackup";
    const string noBackup2Arg = "--nobackup";

    string externalLogName ;
    string externalLogDir;

    for (int i=1; i < argc; i++) { // PRQA S 4235, 4236

        if (StringUtils::equalsNoCase(overWrite1Arg,argv[i]) || StringUtils::equalsNoCase(overWrite2Arg,argv[i]) ) {
            _option = "-overwrite" ;
        }
        else if (StringUtils::equalsNoCase(replaceFile1Arg,argv[i]) || StringUtils::equalsNoCase(replaceFile2Arg,argv[i]) ) {
            _option = "-replacefile" ;
        }
        else if (StringUtils::equalsNoCase(noCriticalOnDuplicates1,argv[i]) || StringUtils::equalsNoCase(noCriticalOnDuplicates2,argv[i]) ) {
            _option = noCriticalOnDuplicates1 ;
            _noCriticalOnDuplicates = true ;
        }
        else if ((StringUtils::equalsNoCase(logname1Arg,argv[i]) || StringUtils::equalsNoCase(logname2Arg,argv[i])) && (i < (argc-1))) {
            externalLogName = argv[++i] ;
        }
        else if (StringUtils::equalsNoCase(relaxfncheck,argv[i]) || StringUtils::equalsNoCase(relaxfncheck2,argv[i]))  {
            _relaxFnCheck = true ;
            _option = "-relaxfncheck" ;
        }
        else if (StringUtils::equalsNoCase(restoreSync1Arg,argv[i]) || StringUtils::equalsNoCase(restoreSync2Arg,argv[i]))  {
            _restoreOnSync = true ;
            _option = "-restoreonsync";
        }
        else if (StringUtils::equalsNoCase(noTriggers1Arg,argv[i]) || StringUtils::equalsNoCase(noTriggers2Arg,argv[i]))  {
            _noActions = true ;
        }
        else if (StringUtils::equalsNoCase(noBackup1Arg,argv[i]) || StringUtils::equalsNoCase(noBackup2Arg,argv[i]))  {
            _noBackup = true ;
        }
        else if (StringUtils::equalsNoCase(logdir1Arg,argv[i]) || StringUtils::equalsNoCase(logdir2Arg,argv[i])) {
            externalLogDir = argv[++i] ;
        }

        else {
            // nothing
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "nobackup=" << boolalpha <<  _noBackup );

    if (!_option.empty()) {
        ACS_LOG_PRIVINFO("Inventory starting with option " << _option) ;
    }



    if ( (_option == "-replacefile") && (_noActions) )
    {
        // This is not allowed
        // Replace file is based on the assumption that all the automatic
        // post inventory actions will be re-executed on the new file
        ACS_LOG_ERROR("--replacefile and --noactions options are not allowed together." ) ;
        ACS_LOG_PRIVINFO("Application exit status: " << 123 ) ;
        return 123 ;
    }


#ifdef CSG
    // Complete the log filename setup in CSG environment
    putThreadInLogName(true);
#endif


    if ( (! externalLogDir.empty()) || (! externalLogName.empty()) )
    {
        // Must override using app specific naming convention and user requested
        // Any project naming convention cannot be applied at the current stage
        string fullLogName = getDefaultLogName();

        string logname = File::getFileName(fullLogName) ;
        string logdir = File::getDirPath(fullLogName) ;

        fullLogName = (externalLogDir.empty() ? logdir : externalLogDir) + "/" + ( externalLogName.empty() ? logname : externalLogName);

        // do the override in the LRP
        getLRPolicy().forceLogName ( fullLogName );
    }

/**
    string logName ;
    {
        DateTime now ;
        ostringstream os ;
        now.asISO8601(os,6) ; // PRQA S 4400
        logName = os.str() ;
        // remove ':'
        string::iterator new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),':')) ;
        logName.erase(new_end, logName.end());

        // remove '-'
        new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),'-')) ;
        logName.erase(new_end, logName.end());
    }

    ostringstream logFileName;
    string logDir(defaultLogPath);
    if (! externalLogDir.empty())
    {
        ACS_LOG_INFO("Requested Log dir " << externalLogDir) ;

        if  ( File::exists(externalLogDir) )
        { logDir = externalLogDir; }
        else
        { ACS_LOG_WARNING( "Requested log dir: " << externalLogDir << "not present; using default " << logDir) ;			}
    }

    if (externalLogName.empty()) {
        logFileName << logDir << "/Inventory_" << logName << ".log";
    }
    else {
        logFileName << logDir << "/" << externalLogName ;
    }

    if (&exostream::getDefaultOutLogger() != &excmcf) {
        exostream::getDefaultOutLogger().appendChild(&excmcf) ;
    }

    // Merge all streams into the log
    vector<exostream *> streams ;
    streams.push_back(&exostream::getDefaultOutLogger()) ;
    streams.push_back(&excout) ;

    exLogStream log("log",logFileName.str(), streams, true);

    ACS_LOG_PRIVINFO(applicationName() << " START") ;
    ACS_LOG_INFO("logname is: " << logFileName.str()) ;
**/

    Application::instance()->setRotateLogParametersFromConf(false);
//	Application::rotateLog();

    getConfiguration();
    Application::rotateLog();

    // Notify to Patrol that process is started
    ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_BUSY);
    try
    {
        startInventory();
    }
    catch(FileInventory::DuplicateInventoryException & e)
    {
        ACS_LOG_NOTIFY_EX(e);
        if (!_noCriticalOnDuplicates) {
            // Notify to MCF that process is ended in error
            ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_ERROR);
            ostringstream os ;
            os << "Inventory::main: EXIT IN ERROR." << _failedOrders <<  " SEE LOG " << getLRPolicy().getCurrentLogName() ;
            ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException(e,os.str())) ;


            ACS_LOG_ERROR("Application exit status: " << 128);
            return 128 ; // returns different from 0 by design !!! It is an exception:  // PRQA S 4400
        }
        else {
            ostringstream os ;
            os << "Inventory::main: EXIT IN ERROR." << _failedOrders <<  " SEE LOG " << getLRPolicy().getCurrentLogName() ;
            ACS_LOG_TRYCATCHNOTIFY_EX(FileInventory::DuplicateInventoryException(e,os.str())) ;

            ACS_LOG_ERROR("Application exit status: " << 123 ) ;
            return 123 ; // returns different from 0 by design !!! It is an exception: // PRQA S 4400
        }
    }
    catch(exception & e)
    {
        ACS_LOG_NOTIFY_EX(e);

        // Notify to MCF that process is ended in error
        ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_ERROR);
        ostringstream os ;
        os << "Inventory::main: EXIT IN ERROR." << _failedOrders <<  " SEE LOG " << getLRPolicy().getCurrentLogName() ;
        ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException(e,os.str()));

        ACS_LOG_ERROR("Application exit status: " << 255);
        return 255 ; // returns different from 0 by design !!! It is an exception:   // PRQA S 4400
                   // means the application did not do its job !!!
    }

    // Notify to Patrol that process is ended
    ACS_LOG_EVENT("INVENTORY_STATUS",EXFE_IDLE);
    ACS_LOG_PRIVINFO("Application exit status: " << 0 );
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods
/////////////////////////////////////////////////////////////////////////////////////////


void InventoryApp::startInventory()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called with option " << _option) ;

    time_t startProcessing = time(0);


    string 	targetDir;
    ConfigurationSingleton::instance()->get().getValue("Inventory.WorkingDir" , targetDir);
    string backupDir;
    if( not _noBackup ) {
        ConfigurationSingleton::instance()->get().getValue("Inventory.BackupDir" , backupDir);
    }
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Working dir=\"" << targetDir << "\", backup dir=\"" << backupDir << '\"');

    FileInventory inv(*getDefaultConnectionPool());
    inv.relaxFNumberCheck(_relaxFnCheck) ;
    inv.restoreOnSync (_restoreOnSync);
    inv.noActions (_noActions);
    try {
        inv.makeInventory(targetDir,backupDir, _option);
    }
    catch(...) {
        // note, this is to return the failed orders list.
        _failedOrders = inv.failedOrders() ;
        throw ;
    }
    time_t stopProcessing = time(0);

    int minExecution = (stopProcessing - startProcessing)/60; // PRQA S 3010
    ACS_LOG_PRIVINFO("Inventory executed in " << minExecution	<< " minutes: "
        << ((stopProcessing - startProcessing) - minExecution*60) << " seconds") ;
}


void InventoryApp::getConfiguration() // PRQA S 4211
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called...") ;


    addSpace(ConfigurationSingleton::ConfSpace("Inventory")); // PRQA S 3081 3
    addSpace(ConfigurationSingleton::ConfSpace("InventoryEmailNotification"));
    addSpace(ConfigurationSingleton::ConfSpace("EmailUserNotification"));


    loadConfiguration(false) ;


    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed") ;
}

_ACS_END_NAMESPACE
