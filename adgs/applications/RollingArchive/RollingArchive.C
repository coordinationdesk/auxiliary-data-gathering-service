/*
    Copyright 1995-2021, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libRollingArchive$

*/

#include <RollingArchive.h>
#include <raV.h>

#include <RollingSpecialPolicies.h>
#include <RollingActionsCommit.h>

#include <RollingReport.h>
#include <RollingReportGeneratorSingleton.h>

#include <dbPersistentScopeGuard.h>
#include <dbPersistentFactoryChain.h>
#include <dbApplicationStatus.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <ApplicationLock.h>
#include <Filterables.h>

#include <ftmpstream.h>

#include <queue>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingArchive)

#ifndef ROLLINGARCHIVE_SUBSYSTEM  // PRQA S 1020 2
#define ROLLINGARCHIVE_SUBSYSTEM "SDF"
#endif

#ifndef ROLLINGARCHIVE_APPNAME // PRQA S 1020 2
#define ROLLINGARCHIVE_APPNAME "RollingArchive"
#endif

namespace {
    // version management
    raV version ;

    Application::GenericBuilder<RollingArchive> RollingArchiveBuilder(ROLLINGARCHIVE_APPNAME, ROLLINGARCHIVE_SUBSYSTEM); // PRQA S 1021
}


RollingArchive::RollingArchive( const string &appName, const string &subSystem ):
        PROJECT_APP(appName, subSystem),
        _confReloadConfTime( _defaultReloadConfTime ),
        _confRestartTime   ( _defaultRestartTime ),
        _confPoliciesRunMax( _defaultPoliciesRunMax ),
        _timerReloadConf(),
        _configurationToBeReloaded(false),
        _rollingPolicyManagerSptr()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RollingArchive::~RollingArchive() ACS_NOEXCEPT {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void RollingArchive::customSigIntHandler() {
    ACS_LOG_INFO ("SIGINT handler called in user space");
}

void RollingArchive::customSigTermHandler() {
    ACS_LOG_INFO ("SIGTERM handler called in user space");
}


void RollingArchive::updateStatusOnDb(size_t const& numRolledAtThisLoop) {

    //Update the timestamp of the last performed rolling actions.
    if ( numRolledAtThisLoop > 0 ) {

        try
        {
            dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
            dbConnectionWrapper pw(*cp); dbConnection &conn = pw ;
            const string &dbKey = conn.getKey() ;

            dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", dbKey)); // PRQA S 3081
            ACS_COND_THROW(!statusRecord,exIllegalValueException("dbApplicationStatus dynamic cast failed")); // PRQA S 3081

            dbPersistentScopeGuard guard(statusRecord);

            oftmpstream statusFile("/tmp/RollingArchive","status");

            statusRecord->setApplication("RollingArchive");

            string instance("Rolling");
            statusRecord->setInstance(instance);

            if( !statusRecord->load(conn) ) {
                ACS_LOG_INFO("Creating the first entry for the application status record on the database.");

                statusRecord->setStatusUploadFileName(statusFile.path()); //First time upload an empty file.
                statusRecord->save(conn);
            }

            //Get timestamp from DB
            DateTime timeStamp;

            {

                string dialect = SQLDialectFactory::dialectName(dbKey) ;

                SQLString sqlString( dbKey, SQLString::SELECT );
                sqlString.addSelectList( SQLDialectFactory::sqlDialect(dbKey).currentDateTime() );

                dbQuery query(conn, sqlString, "GetCurrentDateTime");
                dbSet set; query.execForSet( set );

                ACS_COND_THROW( set.size() == 0, // PRQA S 3081
                        exIllegalValueException(string("Cannot retrieve timestamp from db. Query is [")+sqlString.getSQL()+"]"));

                dbRow row(dbKey); set.getRow(1, row);
                row.column(0, timeStamp);
            }

            statusFile()
                << "<RollingArchiveStatus>\n"
                << "   <!-- DateTime of the last inventory items rolling. -->\n"
                << "   <lastRolledNumItems>"<<numRolledAtThisLoop<<"</lastRolledNumItems>\n"
                << "   <lastRollingDateTime>"<<timeStamp.asISO8601(0)<<"</lastRollingDateTime>\n"
                << "</RollingArchiveStatus>\n";

            statusFile.close();

            statusRecord->replaceStatusFile(statusFile.path(), conn);

            ACS_LOG_INFO("Rolling Notification: Rolled "<<numRolledAtThisLoop<<" items at "<<timeStamp.asISO8601(0));

        } catch(exception &e) {

            ACS_LOG_TRYCATCHNOTIFY_EX(
                    exSystemException( e, "Unable to save the application status on database") );
        }

    }
}


bool RollingArchive::manageWorkers () {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // Collects exit code of the managed workers and reports TRUE if any worker is running

    bool someoneRunning(false);
    try {

        _timerReloadConf.stop(); //not really stop, just to allow elapsed to take the time
        if ( not _configurationToBeReloaded && _timerReloadConf.elapsed()/1000 >= _confReloadConfTime ) { // PRQA S 3084, 4400
            ACS_LOG_PRIVINFO( applicationName()<<": Configuration Reload QUEUED (configuration timer expired).");
            _configurationToBeReloaded = true;
        }

        // canReloadConfiguration is true when there are no worker running
        bool canReloadConfiguration = ( 0 == _rollingPolicyManagerSptr->getNumOfRunningRequests() + _rollingPolicyManagerSptr->getNumOfPendingRequests() );
        if ( _configurationToBeReloaded && canReloadConfiguration ) {
            _configurationToBeReloaded = false;
            this->getConfiguration();
        }

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName()<<": Filling queue of actions");
        deque< RollingPolicyOutput::Sptr > actionsQueue;

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName()<<": Get actions collected by Rolling Special Policies (No Keep actions)");

        RollingPolicyOutput::Sptr actionsSpecialNoKeep = RollingSpecialPolicies::instance()->getListRollingActionsNoKeep_and_clear();
        if ( not actionsSpecialNoKeep->getListRollingActions().empty() ) {
            actionsQueue . push_back( actionsSpecialNoKeep );
        }

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName()<<": Get Keep actions collected by Rolling Special Policies");

        RollingPolicyOutput::Sptr actionsSpecialKeep = RollingSpecialPolicies::instance()->getListRollingActionsKeep();

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName()<<": Get actions collected by Rolling Policies. Apply to them the Keep filter.");

        vector<RollingPolicyOutput::Sptr> listOfPolicyOutputs;
        vector<RollingPolicyOutput::Sptr> listOfPolicyErrors;

        // Get and apply actions collected by Rolling Policies
        _rollingPolicyManagerSptr->fetchRollingPoliciesOutputs( listOfPolicyOutputs, listOfPolicyErrors );

        if ( not listOfPolicyErrors.empty() ) {
            ostringstream msg; msg<<"Got "<<listOfPolicyErrors.size()<<" errors from Rolling Policies ids [";
            string comma("");
            for ( vector<RollingPolicyOutput::Sptr>::iterator it = listOfPolicyErrors.begin(); it!=listOfPolicyErrors.end(); ++it ) {
                msg<<comma<< (*it)->getPolicyId();
                comma = ", ";
            }
            msg<<"]";
            ACS_LOG_WARNING( applicationName()<<": "<<msg.str() );
        }

        //Queue the actions
        std::copy( listOfPolicyOutputs.begin(), listOfPolicyOutputs.end(), std::back_inserter( actionsQueue ) );


        //Rolling report setup
        rsResourceSet& conf = ConfigurationSingleton::instance()->get();
        string itemFilterIncludeRe ;
        string itemFilterExcludeRe ;
        conf.getValue("RollingArchive.Report.ItemFilter.IncludeRe", itemFilterIncludeRe, false); //Load, if any
        conf.getValue("RollingArchive.Report.ItemFilter.ExcludeRe", itemFilterExcludeRe, false); //Load, if any

        //Dump reports (max n items per report)
        size_t maxItemsPerReport(35000);
        conf.getValue("RollingArchive.Report.MaxItemsPerReport", maxItemsPerReport, false); //Load, if any

        // if reportWorkingDir = "" ==> null report mode
        RollingReport rollingReport(
                RollingReportGeneratorSingleton::instance()->getAgent().getPrivateWorkingDir(),
                maxItemsPerReport, itemFilterIncludeRe, itemFilterExcludeRe);

        //Generate Report - If there is a report from previous running not yet consumed, it is consumed here and cleared
        rollingReport.recordCommit();

        //Commit Actions
        // Apply actions
        // Do not apply actions to items to be preserved, computed by Special Policies (manual keep)
        RollingActionsCommit::instance()->execute( actionsQueue, actionsSpecialKeep, rollingReport, _rollingPolicyManagerSptr );

        //Update the my status app to DB.
        this->updateStatusOnDb( rollingReport.getSummaryCurrentLoopNumRecords() );
        rollingReport.resetSummaryCurrentLoopNumRecords();

        //Generate Report
        rollingReport.recordCommit();

        //Check error on report generator
        RollingReportGeneratorSingleton::instance()->getAgent().ifErrorThrowException();


    } catch ( exception &e ) {
        ACS_LOG_NOTIFY_EX(e);
    }

    //Return if RollingPolicyManagerSptr is still up
    someoneRunning = _rollingPolicyManagerSptr->running();

    return someoneRunning;

}

void RollingArchive::createWorkers() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // Creates new workers, according to available works to do and available workers slots

    //Check if no new worker can be created because the app status
    bool pauseCreation =	_configurationToBeReloaded ||
                            ( _rollingPolicyManagerSptr->getNumOfRunningRequests() + _rollingPolicyManagerSptr->getNumOfPendingRequests() > _confPoliciesRunMax ) ;
    if ( pauseCreation ) {
        return;
    }

    //HERE IF NEW WORKERS CAN BE CREATED

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName()<<": start query t_InvRollingPolicies");

    dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
    const string &dbKey = cp->getKey();

    // Queries for InvRollingPolicies
    vector<dbPersistent::IdType> rollPolicyIds;
    try {
        dbConnectionWrapper pw(*cp); dbConnection &conn = pw ;

        ostringstream ossSql; ossSql<<
            "select id"
            " from t_invrollingpolicies"
            " where triggertime <= "<< SQLDialectFactory::sqlDialect(dbKey).currentDateTime();

        SQLString sqlString(dbKey, ossSql.str());
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

        dbSet set;
        dbQuery query( conn, sqlString, "SELECT_t_invrollingpolicies" );
        query.execForSet( set );

        dbRow row(dbKey);
        while ( set.getNextRow(row) ) {
            dbPersistent::IdType rollPolicyId = 0;
            row.column(0, rollPolicyId);
            rollPolicyIds.push_back( rollPolicyId );
        }

    } catch ( exception& ex ) {
        ACS_LOG_TRYCATCHNOTIFY_EX( exSystemException( ex, "Unable to query Rolling Policies. Retry planned on next loop.") );
    }

    //Queue the rolling policy processors
    vector<dbPersistent::IdType> failedPolicyIds;
    size_t newProcessing = _rollingPolicyManagerSptr->addRollingPoliciesToProcess(rollPolicyIds, failedPolicyIds );

    //Perform the Special Rolling Policies (Configured Actions on Specified inventory items)
    bool forceRunSpecialPolicies = (newProcessing > 0);
    RollingSpecialPolicies::instance()->execute( forceRunSpecialPolicies );

    for ( auto it = failedPolicyIds.begin(); it!=failedPolicyIds.end(); ++it ) {
        ACS_LOG_WARNING( "Errors occurred setting up a Rolling Policy executor for rolling policy id ["<<*it<<"]. Retry planned on next loop." );
    }

}

void RollingArchive::terminateWorkers() {
    // Shoot them in the head... RIP

    //Stop the RollingPolicyManager: calling stop (instead of lazyStop) the running rolling policies are stopped.
    _rollingPolicyManagerSptr->stop(false);

    ACS_LOG_INFO("Abort request sent to the Rolling Policy Manager.");

}

bool RollingArchive::getConfiguration() {
    try {
        // add configuration spaces
        addSpace(ConfigurationSingleton::ConfSpace("StationConf", "", false), ConfigurationSingleton::NoLoad);
        addSpace(ConfigurationSingleton::ConfSpace("RollingArchive", "", false), ConfigurationSingleton::NoLoad);
        loadConfiguration();
        rotateLog();
        // Get all configured resources
        rsResourceSet & conf = ConfigurationSingleton::instance()->get();

        // get form conf space time between to RollingArchive activities
        if ( not conf.getValue("RollingArchive.RestartTime", _confRestartTime, false) ) {
            _confRestartTime = _defaultRestartTime;
            ACS_LOG_WARNING ("Cannot read RollingArchive.RestartTime. Using default " << _confRestartTime);
        }

        if ( not conf.getValue("RollingArchive.ReloadConfPeriod", _confReloadConfTime, false) ) {
            _confReloadConfTime = _defaultReloadConfTime;
            ACS_LOG_WARNING("No RollingArchive.ReloadConfPeriod in configuration. Default period is: " << _confReloadConfTime << " [s].");
        }

        if ( not conf.getValue("RollingArchive.PoliciesRunMax", _confPoliciesRunMax, false) ) {
            _confPoliciesRunMax = _defaultPoliciesRunMax;
            ACS_LOG_WARNING("No RollingArchive.PoliciesRunMax in configuration. Default is: " << _confPoliciesRunMax<<".");
        }

        ACS_LOG_PRIVINFO(applicationName()<<": Configuration: Configuration reloading time: " << _confReloadConfTime << " [s]");
        ACS_LOG_PRIVINFO(applicationName()<<": Configuration: Autorestart time: " << _confRestartTime << " [s]");
        ACS_LOG_PRIVINFO(applicationName()<<": Configuration: Max number of policies can be queued to be run: " << _confPoliciesRunMax);

        { //This configuration is handled by RollingSpecialPolicy class.
            int val(-1);
            conf.getValue("RollingArchive.SpecialPoliciesTimerSec", val, false);
            ostringstream valOss; if ( -1 == val ) { valOss << "default"; } else { valOss << val; }
            ACS_LOG_PRIVINFO(applicationName()<<": Configuration: Special/Manual Rolling policy scan time: " << valOss.str() << " [s]");
        }

        //Apply new configuration - other conf settings used in other sections of code
        if ( this->restartInterval() != _confRestartTime ) {
            this->wantToRestart( _confRestartTime );
        }


    } catch(exception &e) {
        ACS_LOG_NOTIFY_EX(e);
        return false ;
    }

    // Restart Reload configuration timer
    _timerReloadConf.start();

    return true ;
}


int RollingArchive::main(int argc, char const * const * argv, char const * const * env) {

    //Print argv
    for (int i=1; i < argc; ++i) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, applicationName() << "[" << i << "]: " << argv[i])
    }

    // Lock the application.
    ApplicationLock appLock;
    appLock.lock(true); //exception is raised if lock fails

    // Force timestamps on filterable messages
    exMsgFmtFactory::instance()->emitTime(true);

    ACS_LOG_PRIVINFO("start");

    try{
        daemonize();
    } catch(exception &e) {
        ACS_LOG_NOTIFY_EX(e);
    }

    //Load Configuration for the first time
    if ( not this->getConfiguration() ) {
        Application::setStatus (Application::Critical, "Cannot load configuration: see log");
        //exit with error.
        ACS_THROW(exCriticalException("Cannot load configuration")) ; // PRQA S 3081
    }

    //Set restart time
    this->wantToRestart (_confRestartTime);
    ACS_LOG_PRIVINFO("Restarting every " << _confRestartTime << " [sec]");

    //Number of connections is known. It depends on implementation flow.
    Application::instance()->getDefaultConnectionPool()->setMaxConnection(4);
    Application::instance()->getDefaultConnectionPool()->setMaxConnectionAttempts(3);

    //Rolling report generator: setup and start
    {
        rsResourceSet& conf = ConfigurationSingleton::instance()->get();
        bool generateReport(false);
        string reportWorkingDir;
        string reportPrivateWorkingDir;
        if ( conf.hasKey("RollingArchive.Report") ) {
            conf.getValue("RollingArchive.Report.enabled.value", generateReport );
            conf.getValue("RollingArchive.Report.OutBasket", reportWorkingDir );
            if ( conf.hasKey("RollingArchive.Report.PrivateWorkingDir") ) {
                conf.getValue("RollingArchive.Report.PrivateWorkingDir", reportPrivateWorkingDir );
            } //default is null

            if ( ! generateReport ) {
                reportWorkingDir="";
                reportPrivateWorkingDir="";
            }
        }
        if ( generateReport ) {
            //Start the Async report generator
            RollingReportGeneratorSingleton::instance()->getAgent().setPaths( reportWorkingDir, reportPrivateWorkingDir );
            RollingReportGeneratorSingleton::instance()->getAgent().start();
        }
    }

    //Rolling policies manager: setup and start
    _rollingPolicyManagerSptr.reset( new RollingPolicyManager );
    _rollingPolicyManagerSptr->start();

    //MAIN LOOP
    while ( manageWorkers () || ( NoStop == getCurrentStopMode() ) ) {

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current Stop Mode ["<< static_cast<int>(getCurrentStopMode())<<"]");

        if ( hasStopModeChanged() ) {

            if ( ImmediateStop == getCurrentStopMode() ) {
                ACS_LOG_INFO ("SIGTERM detected");
                terminateWorkers();

            } else if (CleanStop == getCurrentStopMode() ) {
                ACS_LOG_INFO ("SIGINT detected");

                //Tell the Policy manager to end current running policy and to not start queued or paused policies.
                ACS_LOG_INFO("Nominal stop request sent to the Rolling Policy Manager.");
                _rollingPolicyManagerSptr->lazyStop(false);

            } else if (PhoenixRestart == getCurrentStopMode() ) {
                ACS_LOG_INFO ("Entering restart phase");

                //Tell the Policy manager to end current running policy and to not start queued or paused policies.
                ACS_LOG_INFO("Nominal stop request sent to the Rolling Policy Manager.");
                _rollingPolicyManagerSptr->lazyStop(false);
            }
        }

        // rotate log if needed
        rotateLog() ;

        if ( NoStop == getCurrentStopMode() ) {

            //Queue new Jobs
            createWorkers();
        }
        waitStopOrTimeout( 5000 ); // PRQA S 4400
    }

    if (PhoenixRestart == getCurrentStopMode()) {
        ACS_LOG_INFO ("PHOENIX TIME");
    }

    //Report Generation Join and error check
    ACS_LOG_PRIVINFO("Joining report generator exit.");
    RollingReportGeneratorSingleton::instance()->getAgent().lazyStop(false);

    while ( RollingReportGeneratorSingleton::instance()->getAgent().running() ) {
        waitStopOrTimeout(1000);
    }
    //Check error on report generator
    RollingReportGeneratorSingleton::instance()->getAgent().ifErrorThrowException();
    //-

    ACS_LOG_INFO ("End with Exit code 0");
    return 0;
}

_ACS_END_NAMESPACE

