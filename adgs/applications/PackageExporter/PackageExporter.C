// PRQA S 1050 EOF
/*
    Copyright 1995-2021, Expriva SpA - DADF
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    $Prod: A.C.S. libPackageExporter$

    $Id$

    $Author$

    $Log$
    Revision 2.0  2017/05/31 17:35:42  nicvac
    OPMAN-260: Need PDS DB schema PDS_12-04.

    Revision 1.71  2017/05/30 09:30:49  nicvac
    OPMAN-260: minor.

    Revision 1.70  2017/05/29 17:53:43  nicvac
    OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)

    Revision 1.69  2017/04/20 09:52:45  tergem
    S2PDGS-1641: logRotation moved after configuration is loaded.

    Revision 1.68  2017/03/27 14:28:03  nicvac
    APF-273: mc messages refactoring.

    Revision 1.67  2017/03/27 13:37:56  nicvac
    APF-273: Dissemination mc messages refactoring.

    Revision 1.66  2017/03/24 17:08:21  nicvac
    APF-273: Dissemination MC.

    Revision 1.65  2017/03/03 15:47:38  nicvac
    S2PDGS-1706: Waiting ack packages lock release.

    Revision 1.64  2017/02/23 16:37:49  nicvac
    S3EVOL-13: Distribution working area is not related anymore to download cache.

    Revision 1.63  2017/02/21 17:05:57  nicvac
    S3EVOL-13: Inter process cache lock mode; cache limit check

    Revision 1.62  2017/02/20 15:43:58  nicvac
    S2PDGS-1676: Warning related to already rolled items appears only in the app log.

    Revision 1.61  2017/02/02 10:31:25  davide.tiriticco
    S2PDGS-1655: Application aligned to packageId type update from string to dbPersistent::IdType

    Revision 1.60  2016/12/20 11:12:56  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.59  2016/12/19 18:32:44  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.58  2016/12/16 16:56:15  nicvac
    S3MPM-860: PackageExporter resources improvement.

    Revision 1.57  2016/12/13 16:39:57  nicvac
    OPMAN-199: Reset intermediate status improved.

    Revision 1.56  2016/09/23 14:16:03  nicvac
    OPMAN-138: Improved disk space check in the frame of cryo2 evolution.

    Revision 1.55  2016/09/07 08:35:48  nicvac
    CSGACQ-86: Oracle porting.

    Revision 1.54  2016/08/05 16:58:38  nicvac
    CSGACQ-86: Oracle porting

    Revision 1.53  2016/07/06 15:47:53  marfav
    CSGACQ-84
    New project/application based log rotation management strategy enforced

    Revision 1.52  2016/05/10 14:47:05  marfav
    APF-376 removing references to the ExpWaitingAckDataPackage structure
    the update of delserviceparameters is made by underlying PackageProcessing elements

    Revision 1.51  2016/03/29 17:13:16  marpas
    adopting new db I/F

    Revision 1.50  2016/01/13 17:01:55  marpas
    messaging strategy strongly revisited.

    Revision 1.49  2015/10/20 12:40:11  marfav
    PDSEV-15 using t_distributionruleslock instead of t_distributionpolicieslock

    Revision 1.48  2015/08/17 15:10:56  marfav
    Added support to MC messages

    Revision 1.47  2014/10/13 15:37:16  lucio.pulvirenti
    S2PDGS-760: application status set to OFF when stopped.

    Revision 1.46  2014/10/03 12:17:58  nicvac
    S2PDGS-931 implementation.

    Revision 1.45  2014/09/19 14:29:43  nicvac
    S1PDGS-31550: simplified configuration.

    Revision 1.44  2014/09/18 16:45:38  nicvac
    S1PDGS-31550 implementation.

    Revision 1.43  2014/05/28 12:40:51  nicvac
    Cache erase quota configuration.

    Revision 1.42  2014/05/14 11:27:17  nicvac
    S2PDGS-791: better exception printing.

    Revision 1.41  2014/04/18 15:42:23  nicvac
    S2PDGS-603: Scheduler log filtering.

    Revision 1.40  2014/04/17 17:11:07  nicvac
    S2PDGS-603: Scheduler log filtering.

    Revision 1.39  2013/11/12 17:52:09  nicvac
    Log rotation policy changed.

    Revision 1.38  2013/11/06 18:05:05  nicvac
    Plugins handling.

    Revision 1.37  2013/11/04 11:03:37  marpas
    avoiding useless macros

    Revision 1.36  2013/10/31 11:24:49  marpas
    avoiding useless macros

    Revision 1.35  2013/10/04 13:32:00  nicvac
    acs sdk warnings processed.

    Revision 1.34  2013/09/30 12:57:11  marpas
    useless includes and libraries removed

    Revision 1.33  2013/09/30 07:34:40  clanas
    removed notification exception for space replacement

    Revision 1.32  2013/06/26 11:08:35  nicvac
    S1PDGS-30982: handleWaitingAckPackages more robust.

    Revision 1.31  2013/05/23 13:47:23  nicvac
    S2PDGS-308: Notification Refactoring.

    Revision 1.30  2013/05/20 16:20:36  nicvac
    S2PDGS-300: Priority handling.

    Revision 1.29  2013/05/09 10:08:26  marpas
    minor improvements

    Revision 1.28  2013/05/08 15:37:56  marpas
    listener management in case of no bcasts

    Revision 1.27  2013/04/15 17:20:04  nicvac
    sqlDialect interface changed

    Revision 1.26  2013/04/11 17:24:28  nicvac
    Configuration loadStealth

    Revision 1.25  2013/03/20 14:01:27  nicvac
    Improved Application Restart.

    Revision 1.24  2013/03/19 17:52:08  nicvac
    Fixed phoenix case.

    Revision 1.23  2013/03/18 10:25:21  nicvac
    ADC-83: WaitingAcknowledge Step implemented. Test fixing.

    Revision 1.22  2013/03/15 16:26:37  nicvac
    ADC-83 (WaitingAcknowledge Step implemented)
    ConnectionPool

    Revision 1.21  2013/03/15 10:36:59  nicvac
    S2PDGS-186
    ADC-83
    WaitingAcknowledge Step implemented

    Revision 1.20  2013/03/11 17:33:27  nicvac
    ExporterBase interface changed.

    Revision 1.19  2013/03/07 17:54:49  nicvac
    SQLString for portability.

    Revision 1.18  2013/03/07 12:03:17  nicvac
    TERM exit fixed. ERRINT exit added.

    Revision 1.17  2013/03/05 17:42:16  nicvac
    Retry policy implementation.

    Revision 1.16  2013/02/21 16:16:18  nicvac
    PackageEmailNotifier integration

    Revision 1.15  2013/02/13 17:51:25  nicvac
    New mail system integration

    Revision 1.14  2013/02/08 11:41:57  nicvac
    Media types by configuration

    Revision 1.13  2013/01/25 15:29:40  nicvac
    DeliveryFactory removed (delivery slip moved to task in TT).

    Revision 1.12  2013/01/18 09:30:17  nicvac
    ADC-99 : downloading to error

    Revision 1.11  2013/01/11 13:19:25  marfav
    Addedd support to external appname definition

    Revision 1.10  2012/12/12 16:25:57  nicvac
    Default TT Factory

    Revision 1.9  2012/12/07 17:03:31  nicvac
    Stop does not join PackageProcessingManager.

    Revision 1.8  2012/12/06 18:36:11  nicvac
    Test fix

    Revision 1.7  2012/12/05 15:24:46  nicvac
    Test fix

    Revision 1.6  2012/12/04 17:40:40  nicvac
    Fix during test

    Revision 1.5  2012/11/27 14:45:10  nicvac
    All mediatypes.

    Revision 1.4  2012/11/23 17:37:02  nicvac
    ExporterServiceResolver integration.

    Revision 1.3  2012/11/16 15:25:39  nicvac
    Sptr to Data and viceversa (temporary solution)

    Revision 1.2  2012/11/12 17:44:25  nicvac
    PackageProcessingManager integrated.

    Revision 1.1.1.1  2012/11/09 13:28:07  nicvac
    Import PackageExporter


*/

#include <PackageExporter.h>
#include <PackageExporterV.h>

#include <ResetStatus.h>

#include <DataProviderConfigurer.h>

#include <ApplicationLock.h>
#include <Status.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistent.h>
#include <dbFileType.h>
#include <Transaction.h>

#include <DirPluginSet.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <PackageEmailNotification.h>
#include <UserNotifier.h>

#include <ScDefaultLogGroupFactory.h>

#include <ExporterServiceResolver.h>

#include <MCFilterables.h>
#include <DistributionListener.h>

using namespace std;
using namespace acs::conf;
using namespace acs::dataprovider;
using namespace acs::mc;

_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageExporter)

namespace {
    PackageExporterV version;
}

//Factories are instanced in main().

#define MY_APP_NAME "PackageExporter"

#ifndef PACKAGEEXPORTER_SUBSYSTEM
#define PACKAGEEXPORTER_SUBSYSTEM "SDF"
#endif

#ifndef PACKAGEEXPORTER_APPNAME
#define PACKAGEEXPORTER_APPNAME "PackageExporter"
#endif

PROJECT_APP::GenericBuilder<PackageExporter> theApplicationBuilder(PACKAGEEXPORTER_APPNAME, PACKAGEEXPORTER_SUBSYSTEM);


PackageExporter::PackageExporter(const string& app, const string& subsys):
    PROJECT_APP(app, subsys),
    _timeIntervals(),
    _productPath(), _downloadRepository(),
    _errorLog(), _logFullPath(),
    _availableDownloadSpaceSize(),
    _checkForNewProcessingOnNextMainLoop(true), //Check on the first main loop
    _pollingTime(30),
    _jobResponsible(),
    _restartTime( 3600 * 24 ), //[sec] (one day)
    _criticalErrorDelaySec(30), //[sec]
    _confPendingRequestsLimit( 15 ),
    _confWaitingAcknowledgeLimit( 1000 ),
    _confMinSpaceRequiredMb(), //[Mb] - default computed at load configuration time.
    _pluginLoader(),
    _packageProcessingManagerPtr(),
    _resourceStatsPtr()
{

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    // set job responsible from hostname
    //@@@ cf. DataProvider::DataProvider. Revise the approach!
    char tmp[1024]; memset(tmp, 0x00, 1024); ::gethostname ( tmp, sizeof ( tmp ) );
    _jobResponsible = tmp;

}

PackageExporter::~PackageExporter() throw() {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    //Unregister the objects owned in the Service Resolver
    ExporterServiceResolver::instance()->unregister();
}


/** Signal handlers */
void PackageExporter::customSigIntHandler()
{
    ACS_LOG_PRIVINFO ("SIGINT signal received");
    Application::setStatus (Application::Exiting);
}

void PackageExporter::customSigTermHandler()
{
    ACS_LOG_PRIVINFO ("SIGTERM signal received");
    Application::setStatus (Application::Exiting);
}


void vDp2vPid( vector<DataPackage> const& vDp, vector< dbPersistent::IdType >& vPid ) {

    for ( vector<DataPackage>::const_iterator it = vDp.begin(); it != vDp.end(); ++it ) {
        vPid.push_back( it->getPackageId() );
    }

}

void PackageExporter::sendMcfExitMessages() const {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "sendMcfExitMessages: start");
    // Send message to MCF VIEW
    ACS_LOG_EVENT("FTPVIEW", "SHOWUPLOADWARNING");
    ACS_LOG_EVENT("FTPVIEW", "DELETEALLUPLOADS");
    ACS_LOG_EVENT("FTPVIEW", "HIDEUPLOADWARNING");
}


void PackageExporter::convertTime(const string & timestring, double & timeDouble) {

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Exporter::convertTime start");
    int hour = -1;
    int minutes = -1;
    int seconds = -1;
    // convert time string HH:MM.SS in double format
    if (!sscanf(timestring.c_str(), "%d:%d.%d", &hour, &minutes, &seconds)) {
        exIOException e(" Exporter::convertTime Cannot read time string " + timestring);
        ACS_THROW(e);
    }

    timeDouble = (hour * 3600.0 + minutes * 60.0 + seconds);
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Exporter::convertTime timeDouble " << timeDouble);

}



void PackageExporter::handleFailedDwnlPackages(vector<DataPackage> const& failedPackages) {

    try {
        // for failed packages call the notifier for mail Warnings
        for ( vector<DataPackage>::const_iterator it=failedPackages.begin(); it!=failedPackages.end(); ++it ) {
            PackageEmailNotificationSptr packageEmailNotificationSptr(new PackageEmailNotification);
            packageEmailNotificationSptr->setDataPackage(*it).setStatus(acs::Status::error).setSuccess(false);
            UserNotifier::instance()->sendAsynch( packageEmailNotificationSptr );
        }

    } catch (exception &e) {
        ACS_LOG_ERROR("cannot send mail warning!") ;
        ACS_LOG_NOTIFY_EX(e);
    }
}

void PackageExporter::handleSuspendedPackages(vector<DataPackage> const& suspendedPackages) {

    try {
        // for suspended packages call the notifier for mail Warnings
        for ( vector<DataPackage>::const_iterator it=suspendedPackages.begin(); it!=suspendedPackages.end(); ++it ) {
            PackageEmailNotificationSptr packageEmailNotificationSptr(new PackageEmailNotification);
            packageEmailNotificationSptr->setDataPackage(*it).setStatus(acs::Status::suspended).setSuccess(false);
            UserNotifier::instance()->sendAsynch( packageEmailNotificationSptr );
        }

    } catch (exception &e) {
        ACS_LOG_ERROR("cannot send mail warning!") ;
        ACS_LOG_NOTIFY_EX(e);
    }
}

void PackageExporter::handleDownloadedPackages(vector<DataPackage> const& goodPackages, vector<DataPackage>& failedPackages) {
    if (goodPackages.size()) {
        //Add packages to the PackageProcessingManager for distribution

        //@@@ Data -> Sptr to be removed when handle*Packages revised with shared pointers

        vector<DataPackageSptr> goodPackagesParam;
        for ( vector<DataPackage>::const_iterator it=goodPackages.begin(); it!=goodPackages.end(); ++it ) {
            DataPackageSptr dataPackageSptr( new DataPackage );
            *dataPackageSptr = *it;
            goodPackagesParam.push_back( dataPackageSptr );
        }

        vector<DataPackageSptr> tmpFailedPackages;
        _packageProcessingManagerPtr->addPackagesToProcess( goodPackagesParam, tmpFailedPackages);

        //@@@ Sptr -> Data to be removed after refactoring
        for ( vector<DataPackageSptr>::iterator it=tmpFailedPackages.begin(); it!=tmpFailedPackages.end(); ++it ) {
            failedPackages.push_back( *(*it) );
        }

    }
}

bool PackageExporter::getDistributionStats(dbPersistent::IdType const& dataPackageId,
                                                 bool& isLastChance, int& retryCurrent, int& retryThreshold,
                                                 size_t& elapsedSec) throw() {

    bool itsOk(true);

    try {

        dbSet set;
        ostringstream sqlSStream;
        ostringstream idStr; idStr<<dataPackageId;
        const string &dbKey = Application::instance()->getDefaultConnectionPool()->getKey() ;
        try {
            dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
            dbConnection& conn = pw;
            /*/
            "SELECT disq.retrycounter, disr.connretries \
            FROM \
            t_distributionqueue as disq, t_distributionrules as disr, \
            t_distributionitems as disit \
            WHERE \
            disq.packageid = "<<string(dbConv(dataPackageId)<<" AND \
            disq.packageid = disit.packageid AND \
            disit.distributionrule_id = disr.id_distributionrule \
            LIMIT 1 ";
            //*/

            SQLString sqlString( dbKey, SQLString::SELECT );
            sqlString.setLimit(1);
            sqlString.addSelectList( "disq.retrycounter" );
            sqlString.addSelectList( "disr.connretries" );
            sqlString.addSelectList( "disq.creationdate" );
            sqlString.addSelectList( "disq.lastattempt" );
            sqlString.addTableList("t_distributionqueue", "disq");
            sqlString.addTableList("t_distributionrules", "disr");
            sqlString.addTableList("t_distributionitems", "disit");
            dbQueryParameters pp(dbKey);
            pp.addParameter( dbParam(dbKey, "disq","packageid", dbParam::EQUAL_TO, db::toSQL( dbKey, dataPackageId)) );
            pp.addParameter( dbParam(dbKey, "disq","packageid", dbParam::EQUAL_TO, "disit.packageid") );
            pp.addParameter( dbParam(dbKey, "disit","distributionrule_id", dbParam::EQUAL_TO, "disr.id_distributionrule") );
            sqlString.addANDWhereParameter( pp );

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL() );

            dbQuery query(conn, sqlString, "SelDistribution");
            query.execForSet( set );
        } catch (exception& ex) {
            ACS_THROW(exPackageExporterException( ex, "Database Error retrieving retry counters for data package id ["+idStr.str()+"]" ));
        }

        ACS_COND_THROW(
            set.size() == 0,
            exPackageExporterException(string("Cannot retrieve retry counters for datapackage id [")+idStr.str()+"]"));

        //Get row from the query result - Just the first
        dbRow row(dbKey); set.getRow(1, row);
        int i(0);

        DateTime dtCreation;
        DateTime dtLastAtt;
        bool isNull(false);
        //If Null on DB, the attribute is left with its default value
        row.column(i++, retryCurrent); //is not null by schema
        ++retryCurrent; //current retry = db retry + 1
        row.column(i++, retryThreshold, &isNull);
        row.column(i++, dtCreation, &isNull); //not null by schema.
        row.column(i++, dtLastAtt, &isNull); //if null, left to now.

        isLastChance = (retryCurrent >= retryThreshold);

        elapsedSec = static_cast<size_t> ( rintf( (dtLastAtt.jd50() - dtCreation.jd50()) * 86400 ) );

    } catch (exception& ex) {

        ACS_LOG_WARNING( ex.what() );
        ACS_LOG_NOTIFY_EX(ex);
        itsOk=false;
        isLastChance=true;
        retryCurrent=1;
        retryThreshold=1;
        elapsedSec=0;
    }

    return itsOk;

}

void PackageExporter::stopConsumer(bool const& abort) {

    if ( abort ) {
        ACS_LOG_INFO("Aborting the Package Processing Manager...");

        //Stop the PackageProcessingManager: calling stop (instead of lazyStop) the running
        // processors are stopped.
        // Related package is set as Error in DB because stopConsumer called from runExport
        _packageProcessingManagerPtr->stop(false);
    } else {
        ACS_LOG_INFO("Waiting the Package Processing Manager...");
        //Stop the PackageProcessingManager: calling lazyStop the running
        // processors are finished.
        // The related package is set accordingly in DB because stopConsumer called from runExport
        _packageProcessingManagerPtr->lazyStop(false);
    }


    bool abortForced(false);
    while ( _packageProcessingManagerPtr->running() ) {

        Timer::delay(1000);

        //It was a nominal stop, then I was waiting for the end of processing.
        // But in the meantime (perhaps because the processing did not exit soon) a TERM request come.
        // In this case abort the processing
        if ( !abort && (ImmediateStop == getCurrentStopMode()) && !abortForced) {

            abortForced = true;
            ACS_LOG_INFO("Aborting the Package Processing Manager...");
            _packageProcessingManagerPtr->stop(false);
        }

    }

    ACS_LOG_INFO("Package Processing Manager stopped.");
}



void PackageExporter::getProcessedPackages(vector<DataPackage>& goodPackages,
                                           vector<DataPackage>& failedPackages,
                                           vector<DataPackage>& waitingAckPackages) {

    //@@@ Sptr -> Data conversion to be removed when getPackages revised with shared pointers
    vector<DataPackageSptr> goodPackagesParam(0);
    vector<DataPackageSptr> failedPackagesParam(0);
    vector<DataPackageSptr> waitingAckPackagesParam(0);

    _packageProcessingManagerPtr->getPackages(goodPackagesParam, failedPackagesParam, waitingAckPackagesParam);

    //Do not clear goodPackages and failedPackages here!
    //failedPackages here could contain packages from dataprovider (e.g. download error)

    for ( vector<DataPackageSptr>::const_iterator it=goodPackagesParam.begin(); it!=goodPackagesParam.end(); ++it ) {
        goodPackages.push_back( *(*it) );
    }

    for ( vector<DataPackageSptr>::const_iterator it=failedPackagesParam.begin(); it!=failedPackagesParam.end(); ++it ) {
        failedPackages.push_back( *(*it) );
    }

    for ( vector<DataPackageSptr>::const_iterator it=waitingAckPackagesParam.begin(); it!=waitingAckPackagesParam.end(); ++it ) {
        waitingAckPackages.push_back( *(*it) );
    }

}


void PackageExporter::handleWaitingAckPackages() throw() {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    //The set of package ids to check
    vector<dbPersistent::IdType> packageIdsToCheck;
    try {

        //Retrieve datapackages that need acknowledge check
        dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
        dbConnection& conn = pw;
        const string &dbKey = conn.getKey() ;

        string dialectName = SQLDialectFactory::dialectName( dbKey );
        transform(dialectName.begin(), dialectName.end(), dialectName.begin(), ::tolower);

        string sqlIntervalOneSecond;

        if 			(dialectName.find( "postgres" ) != string::npos ) {
            sqlIntervalOneSecond = "INTERVAL '1 seconds'";

        } else if	(dialectName.find( "oracle" ) != string::npos ) {
            sqlIntervalOneSecond = "NUMTODSINTERVAL(1, 'second')";

        } else {
            ACS_THROW( exPackageExporterException("SQL query is not defined to support ["+dialectName+"].") );
        }

        DateTime nowTime;
        ostringstream sqlSStream;
        sqlSStream<<"\
            SELECT DISTINCT \
            disq.packageid as packageid, \
            (MIN (disit.duedate) )  as duedate, \
            disr.priority as priority \
            FROM \
            t_distributionqueue disq, \
            t_distributionitems disit, \
            t_distributionrules disr \
            WHERE \
            disq.status = "<<db::toSQL(dbKey, acs::Status::waitingack)<<" AND \
            disq.jobresponsible = "<<db::toSQL(dbKey,_jobResponsible)<<" AND \
            (disq.distributeddate + disr.ackcheckperiod_sec * "<<sqlIntervalOneSecond<<" ) < "<<db::toSQL(dbKey, nowTime, dbConv::YearToFraction)<<" AND \
            disq.packageid = disit.packageid  AND \
            disit.distributionrule_id = disr.id_distributionrule \
            GROUP BY \
            disq.packageid, \
            disr.priority \
            ORDER BY \
            disr.priority "<< ( (acs::conf::DPConfigurerSingleton::instance()->getLowerPriorityFirst())?"ASC":"DESC" ) <<", \
            duedate  ASC";

        SQLString querySQL ( dbKey, sqlSStream.str());

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing query: " <<querySQL.getSQL());

        dbSet set;
        dbQuery query( conn, querySQL, "SELWaitingAckPackages"  );
        query.execForSet( set );

        //Queue processing for Acknowledge check

        //Note: The above query can get packages that are in the current processing list (queued or running). Nominally this could happen very rarely
        // because usually the ackn. check processing is very fast and disr.ackcheckperiod_sec is bigger than processing time.
        // Anyway, even if this happens, it's not a problem because _packageProcessingManagerPtr->addPackagesToCheck filters out and ignore packages
        // in the current processing list. Ignored packaged will be considered on the next main loop.

        dbRow row(dbKey);
        while ( set.getNextRow(row) ) {

            dbPersistent::IdType packageid = 0;
            row.column(0, packageid);
            packageIdsToCheck.push_back( packageid );
        }

        if ( ! packageIdsToCheck.empty() ) {
            vector<dbPersistent::IdType> failedPackageIds;
            _packageProcessingManagerPtr->addPackagesToCheck( packageIdsToCheck, failedPackageIds );

            if ( ! failedPackageIds.empty() ) {
                //Cf. comments in handleDegradedWaitingAckPackages
                this->handleDegradedWaitingAckPackages( failedPackageIds );
            }

        }

    } catch(exception& ex) {

        //Not critical step. Will retry on the next loop.

        ACS_LOG_WARNING("Cannot Retrieve / Queue Packages for Acknowledge Check");
        ACS_LOG_NOTIFY_EX(ex) ;
    }

}

void PackageExporter::handleDegradedWaitingAckPackages( vector<dbPersistent::IdType> const& failedPackageIds ) const {

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    // Here we are in a very degraded case.
    // You can be here when a WaitingAck processing type was starting, but the init phase failed.
    // This means that for this WaitingAck order it has not been possible to create
    // the related Datapackage object (for example for a corrupted clob in the related distribution order).
    // Because Datapackage object and related infos are not available anymore for this order,
    // we cannot query for the distribution status to the asynch service (e.g. circulator).
    // This means that the datapackage could have been actually distributed by the asynch service, but we can't know.
    // So the only thing we can do here is to set these orders to Error and Warn the operator.

    ACS_LOG_WARNING("Degraded WaitingAcknowledge Orders detected. Setting them to error. Cannot check if packages have been actually distributed by asynch service. Degraded counter: "<<failedPackageIds.size());
    try {

        dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
        dbConnection& conn = pw;
        const string &dbKey = conn.getKey() ;

        for ( vector<dbPersistent::IdType>::const_iterator it=failedPackageIds.begin(); it!=failedPackageIds.end(); ++it ) {
            DateTime curdate;

            SQLString sqlString( dbKey, SQLString::UPDATE );
            sqlString.addTableList ( "t_distributionqueue" );
            sqlString.addSetList ( "status", db::toSQL(dbKey, acs::Status::error ) );
            sqlString.addSetList ( "distributeddate", db::toSQL(dbKey, curdate, dbConv::YearToFraction ) );

            dbQueryParameters pp(dbKey);
            pp.addParameter ( dbParam(dbKey, "t_distributionqueue", "packageid", dbParam::EQUAL_TO, db::toSQL(dbKey, *it) ) );
            sqlString.addANDWhereParameter( pp );

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<<sqlString.getSQL() );

            dbQuery update( conn, sqlString, "UpdDistQueueDegradeWaitAck" );
            update.execForStatus();
        }

    } catch(exception& ex) {
        //Not a critical operation. Will retry on the next main loop.
        ACS_LOG_WARNING("Some problem occurred while setting Degraded WaitingAcknowledge orders to error.");
    }


}

void PackageExporter::updateWaitingAckPackages( vector<DataPackage> const& waitingAckPackages ) {

    if ( waitingAckPackages.empty() ) {
        return;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Some Waitingack to update.");

    //Allowed retry
    size_t maxConnRetry(0);
    rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    conf.getValue ( "maxDbRetries", maxConnRetry );

    dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
    dbConnection& conn = pw;
    const string &dbKey = conn.getKey() ;

    //APF-376 the delserviceparam is filled after any task table execution
    //despite of the context and the execution exit code
    for ( vector<DataPackage>::const_iterator it=waitingAckPackages.begin(); it!=waitingAckPackages.end(); ++it ) {

        DateTime curdate;
        size_t connRetry(0);
        bool updated(false);
        while ( !updated && connRetry < maxConnRetry ) {

            try {
                SQLString sqlString( dbKey, SQLString::UPDATE );
                sqlString.addTableList ( "t_distributionqueue" );
                sqlString.addSetList ( "status", db::toSQL(dbKey, acs::Status::waitingack) );
                sqlString.addSetList ( "distributeddate", db::toSQL(dbKey, curdate, dbConv::YearToFraction ) );

                dbQueryParameters pp(dbKey);
                pp.addParameter ( dbParam(dbKey, "t_distributionqueue", "packageid", dbParam::EQUAL_TO, db::toSQL(dbKey, it->getPackageId() ) ) );
                sqlString.addANDWhereParameter( pp );

                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<<sqlString.getSQL() );

                dbQuery update( conn, sqlString , "SetDistributionWaitingAck");
                update.execForStatus();

                // if update is ok exit loop
                updated = true;

            } catch(exception& ex) {

                if ( connRetry==0 ) {
                    // send a warning the first time a connection problem has occured
                    ACS_LOG_WARNING("Problem connecting db");
                    ACS_LOG_NOTIFY_EX(ex) ;
                }
                if ( connRetry == ( maxConnRetry - 1 ) || ImmediateStop != getCurrentStopMode() ) {
                    ostringstream msg;
                    msg << "PackageExporter::updateWaitingAckPackages  " << __LINE__ << " maximum number of db connection retries ("
                        << maxConnRetry << ") performed!" << endl;
                    ACS_THROW ( exPackageExporterException ( ex, msg.str() ) );
                }
                Timer::delay ( 10000 );
                ++connRetry;
            }
        }
    }

}


void PackageExporter::runExport(bool checkForNewProcessing, bool exitFlag, DataProvider & dataProvider, Cache & cache) {

    ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,"Exporter Run Export");


    //NOTE: THIS METHOD IS EXECUTED CONTINUOUSLY! (Refactory needed, cf. warning on PackageExporter::main).
    // Take care of reduce DB accesses as much as possible when checkForNewProcessing is false!
    _checkForNewProcessingOnNextMainLoop = false;

    // distribute downloaded packages only if in time windows
    bool allowed = false;
    if (exitFlag || !(_timeIntervals.size())){
        allowed = true;
    } else {
        // if time windows configured => check if export allowed
        DateTime curtime;
        DateTime::JD50 DateJD50(curtime);
        double curDate = ((DateJD50.jd()) - floor(DateJD50.jd())) * 86400.0;
        ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Current Date time : " << curDate);
        // Loop on all configured download time interval

        for (unsigned int i = 0; i < _timeIntervals.size(); ++i) {
            // If current date is in between configured interval -> download actions are allowed
            if (curDate >= _timeIntervals[i].timeStart && curDate <= _timeIntervals[i].timeStop) {
                ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Exporter::runExport EXPORT allowed! curtime in time window " << i);
                allowed = true;
                break;
            }
        }
    }

    // if TERM signal received or not in time window exit
    if (!allowed || ImmediateStop == getCurrentStopMode()) {
        return;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,"check for downloaded packages");
    vector<DataPackage> goodPackages, failedPackages;
    dataProvider.checkDownloadThreads(&goodPackages, &failedPackages);
    //goodPackages: successfully downloaded in packageId path
    //failedPackages: error during downloading.

    //Failed package handling
    this->handleFailedDwnlPackages(failedPackages);

    //Check available Resources for processing
    bool checkForEligible   = true;
    bool checkForWaitingAck = true;
    {
        bool limitProcessing = _resourceStatsPtr->isPendingRequestsLimit();
        bool limitWaitingAck = false;
        bool limitDiskSpace = false;
        if ( checkForNewProcessing and !limitProcessing ) {
            limitWaitingAck = _resourceStatsPtr->isWaitingAcknowledgeLimit();
            if ( limitWaitingAck ) {
                ACS_LOG_WARNING( "Reached Limit ("<< _resourceStatsPtr->getWaitingAcknowledgeLimit() <<") for Waiting Acknowledge packages. Eligible packages distribution will be re-enabled when resources will be available again." );
            }

            limitDiskSpace = _resourceStatsPtr->isDiskSpaceLimit();
            if ( limitDiskSpace ) {
                ACS_LOG_WARNING( "Reached Limit ("<< _resourceStatsPtr->getMinSpaceRequiredMb() <<" Mb) for Disk space in ["<<_resourceStatsPtr->getSpaceRequiredPath()<<"]. Eligible packages distribution will be re-enabled when resources will be available again." );
            }
        }

        checkForEligible   = checkForNewProcessing && !limitProcessing && !limitWaitingAck && !limitDiskSpace;
        checkForWaitingAck = checkForNewProcessing && !limitProcessing;
    }

    // check for ELIGIBLE packages and start downloading
    // from storage managers...
    vector < DataPackage > suspendedPackages;
    size_t countNewEligibleThisLoop(0);
    if ( checkForEligible ) {
        //Number of queued Eligible packages are limited by configuration
        countNewEligibleThisLoop = dataProvider.checkEligiblePackages(&failedPackages, &suspendedPackages);
    }
    //failedPackages: downloading thread cannot start or error setting DOWNLOADING status on DB.
    //suspendedPackages: eligible packages containing items with validityFlag=false. Send warnings to M&C.

    vector< dbPersistent::IdType > suspendedPckIds; vDp2vPid(suspendedPackages, suspendedPckIds);
    dataProvider.updateDistrStatus(acs::Status::suspended, suspendedPckIds, false, false);

    //Suspended package handling
    this->handleSuspendedPackages(suspendedPackages);

    //Downloaded packages...
    vector<dbPersistent::IdType> goodPackagesIds; vDp2vPid(goodPackages, goodPackagesIds);
    dataProvider.updateDistrStatus(acs::Status::distribution, goodPackagesIds, false, false);

    //good downloaded package handling
    this->handleDownloadedPackages(goodPackages, failedPackages);

    goodPackages.clear();

    // if exit -> terminate the dataConsumer
    if (exitFlag) {
        //Consumer stop
        this->stopConsumer(false);
    }

    //Packages whose exporting has been queued (tipically taken in charge by other asych components).
    // To be checked after a while along their related parameter (clob) ...
    vector<DataPackage> waitingAckPackages;
    //Here goodPackages is reset. failedPackages are appended to the previous collected one (error downloading)
    // retreive all distributed, failed, waiting packages
    this->getProcessedPackages(goodPackages, failedPackages, waitingAckPackages);

    //update status to WAITINGACK for waitingAckPackages.
    this->updateWaitingAckPackages( waitingAckPackages );
    size_t countNewWaitingAckThisLoop = waitingAckPackages.size();

    dbConnectionWrapper cw(*Application::instance()->getDefaultConnectionPool());
    dbConnection &conn = cw;
    //update status to ERROR/RECOVERABLEERROR for failed packages
    vector< dbPersistent::IdType > errorPackageIds, recoverPackageIds;
    for (size_t i = 0; i < failedPackages.size(); ++i) {

        dbPersistent::IdType pck = failedPackages[i].getPackageId();

        //Check if this was the last attempt to try this package's export
        bool isLastChance(true); int retryCurrent(1); int retryThreshold(1); size_t elapsedSec(0);
        this->getDistributionStats(pck, isLastChance, retryCurrent, retryThreshold, elapsedSec);

        if ( isLastChance ) {
            errorPackageIds.push_back(pck);
        } else {
            recoverPackageIds.push_back(pck);
        }

        ACS_LOG_WARNING("Exporter::runExport: export retries on package ["<<failedPackages[i].getPackageId()<<"]: "<<retryCurrent<<"/"<<retryThreshold);

        // send Error message to MCF
        if ( isLastChance ) {
            std::stringstream ss;
            ss << _productPath << "/" << failedPackages[i].getPackageId() << "/" << _errorLog;
            string errorLogPath = ss.str();
            ACS_LOG_ERROR("Exporter::runExport Cannot export pkg: " << pck
                << ( File::exists(errorLogPath) ? (" See Logfile " + errorLogPath) : "")) ;

            // APF-273
            ACS_LOG_MC_INFO( mc_param("packageid", pck ) <<
                             mc_param("retriesnum", retryCurrent ) <<
                             mc_param("message", "Error distributing package. Refer to messages related to this package." ) <<
                             mc_metric(Metric_EXP_FAIL)) ;

            // get fileTypes list
            list < string > fTypes;
            failedPackages[i].getInvFilesFileTypes(&fTypes);
            // extract unique fileTypes
            fTypes.unique();

            for (list<string>::iterator it = fTypes.begin(); it != fTypes.end(); ++it) {
                try {
                    vector < string > missions;
                    dbFileType::getAssociatedSatellites(conn, *it, missions);
                    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"MISSION SIZE" << missions.size());
                    for (size_t k = 0; k < missions.size(); ++k) {
                        // OCD-405 + OCD-406 key: mission:filetype because MCF cannot use key value
                        ostringstream eventOpAlert ;
                        eventOpAlert << "Error processing package: " << failedPackages[i].getPackageId() << " See logfile ";
                        if (File::exists(errorLogPath)) {
                            eventOpAlert << errorLogPath ;
                        }else{
                            eventOpAlert << _logFullPath ;
                        }
                        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "OPALERT>>>>>>>>>>>>> " << missions[k] << ":" << *it);
                        ACS_LOG_OPEVENTALERTMSG(missions[k], *it,eventOpAlert.str());
                    }
                } catch (exception const& ex) {
                    std::stringstream ss;
                    ss <<  "Cannot notify OpAlerts for ftype:" << *it << " package was: " << failedPackages[i].getPackageId();
                    ACS_LOG_TRYCATCHNOTIFY_EX(exPackageExporterException(ex,ss.str()));
                }
            }
        }

        //Cache files released at dowload time. Items in distribution processing working area are not related to cache.
    }

    // set status to ERROR_STATUS for all failed Packages (no other retries possible)
    dataProvider.updateDistrStatus(acs::Status::error, errorPackageIds, true, false);

    // set status to RECOVERABLEERROR_STATUS for all failed Packages to retry
    dataProvider.updateDistrStatus(acs::Status::recoverableError, recoverPackageIds, true, false);


    //update status to COMPLETED for good packages
    vector<dbPersistent::IdType> completedPackageIds;
    for (size_t counter = 0; counter < goodPackages.size(); ++counter) {
        dbPersistent::IdType package = goodPackages[counter].getPackageId();
        completedPackageIds.push_back(package);

        //Check if this was the last attempt to try this package's export
        bool isLastChance(true); int retryCurrent(0); int retryThreshold(0); size_t elapsedSec(0);
        this->getDistributionStats(package, isLastChance, retryCurrent, retryThreshold, elapsedSec);

        // APF-273
        ACS_LOG_MC_INFO( mc_param("packageid", package ) <<
                         mc_param("retriesnum", retryCurrent ) <<
                         mc_param("jobtime", elapsedSec ) <<
                         mc_metric(Metric_EXP_OUT)) ;

        // unlink directory for package (where files have been downloaded from inventory)
        try {
            std::stringstream ss; ss << _productPath << "/" << package;
            File::unlink(ss.str());
        } catch (exception& ex) {
            ostringstream msg; msg <<"[DP "<<package<<"]: Cannot clean package path.";
            ACS_LOG_TRYCATCHNOTIFY_EX(exPackageExporterException(ex, msg.str()));
        }

        // notify to MCF
        {
            DateTime currentTime;
            string utcs; utcs = currentTime.utcs(utcs);
            ostringstream os; os<<utcs<<" - Uploaded Pkg: "<<package ;
            ACS_LOG_EVENT("SHIPPING_LIST_UPL", os.str());
        }

        //Cache files released at download time. Items in distribution processing working area are not related to cache.
    }
    // set status to COMPLETED for all goodPackages
    dataProvider.updateDistrStatus(acs::Status::completed, completedPackageIds, true, true);

    //Retrieve datapackages that need acknowledge check
    // Start a processor for each needed acknowledge check
    // IMPORTANT: Check for new waiting acknowledge packages here, i.e. after status in distribution queue have been updated.
    //@@@ RIVEDERE LA GESTIONE DEI POLLING!!! (v. PacakageExporter::main)
    if ( checkForWaitingAck ) {
        this->handleWaitingAckPackages();
    }

    //Decide in the next main loop if it's the case to check again the DB for new processing, without waiting the polling timer.
    _checkForNewProcessingOnNextMainLoop =
            ( checkForEligible && countNewEligibleThisLoop > 0 )
            ||
            ( checkForWaitingAck && countNewWaitingAckThisLoop > 0 );

}


void PackageExporter::loadPlugins() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    try	{
        rsResourceSet& conf ( ConfigurationSingleton::instance() -> get() );

        string regExp;
        vector<string> pluginsPaths;

        conf.getValue( "PackageExporter.Plugins.regExp.value", regExp );
        conf.getArray( "PackageExporter.Plugins.PluginsPathList.PluginsPath", pluginsPaths );

        ostringstream oss;
        std::copy(pluginsPaths.begin(), pluginsPaths.end(), ostream_iterator<string>(oss, "\n"));
        ACS_LOG_PRIVINFO("Loading plugins from configured paths: \n"<<oss.str());

        DirPluginSet myPluginSet (pluginsPaths, regExp);
        myPluginSet.load (_pluginLoader);
        if (!_pluginLoader.load()) {
            ACS_LOG_WARNING("Plugin loader failed. Some plugins have not been installed");
            const Plugin& failed = _pluginLoader.failed(true);
            ACS_LOG_WARNING(failed.openError());
        }
    } catch (exception& ex)	{
        ACS_LOG_WARNING("Cannot load plugin");
        ACS_LOG_NOTIFY_EX(ex) ;
    }
}

void PackageExporter::loadConf() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    try {

        vector<string> confFileToLoad(0);
        confFileToLoad.push_back( MY_APP_NAME );
        confFileToLoad.push_back( "StationConf" );
        confFileToLoad.push_back( "PackageProcessing" );
        confFileToLoad.push_back( "PackageEmailNotification" );
        confFileToLoad.push_back( "EmailUserNotification" );
        confFileToLoad.push_back( "libScDefaultLogGroup.so" ); //Needed for scheduler log handling


        //Main config file contains other resources to load
        for ( vector<string>::iterator elem= confFileToLoad.begin(); elem != confFileToLoad.end(); ++elem ) {
            Application::instance()->addSpace(ConfigurationSingleton::ConfSpace( *elem ));
            ACS_LOG_PRIVINFO("loaded configuration space " << *elem);
        }

        Application::instance()->loadConfiguration(false) ;
        Application::rotateLog();

    } catch(exception &ex) {

        //exit with error.
        Application::setStatus(Application::Critical, "Initialization failed - cannot load configuration");
        ACS_THROW(exPackageExporterCriticalException(ex, "PackageExporter::loadConf Cannot load configuration")) ;
    }

    //Set Attributes from configuration
    _downloadRepository= acs::conf::DPConfigurerSingleton::instance()->getDownloadRepository();
    _availableDownloadSpaceSize = acs::conf::DPConfigurerSingleton::instance()->getDownloadSpaceSize(); //returned as bytes - cache works with bytes.
    const size_t availableDownloadSpaceSizeMb = _availableDownloadSpaceSize / (1024*1024);

    rsResourceSet& conf = ConfigurationSingleton::instance()->get();

    //Application Restart Time (Phoenix)
    if ( ! conf.getValue("ReloadConfigurationTime", _restartTime, false) ) {
        ACS_LOG_PRIVINFO("Using default value ["<<_restartTime<<" sec] for configuration 'ReloadConfigurationTime'");
    }

    //Application Restart Time (Phoenix) on failed startup
    if ( ! conf.getValue("PackageExporter.CriticalErrorDelaySec", _criticalErrorDelaySec, false) ) {
        ACS_LOG_PRIVINFO("Using default value ["<<_criticalErrorDelaySec<<" sec] for configuration 'PackageExporter.CriticalErrorDelaySec'");
    }

    //Get Pending Requests Limit
    if ( ! conf.getValue("PackageExporter.PendingRequestsLimit", _confPendingRequestsLimit, false) ) {
        ACS_LOG_PRIVINFO("Using default value ["<<_confPendingRequestsLimit<<"] for configuration 'PackageExporter.PendingRequestsLimit'");
    }

    //Get Waiting Acknowledge Request Limit
    if ( ! conf.getValue("PackageExporter.WaitingAcknowledgeLimit", _confWaitingAcknowledgeLimit, false) ) {
        ACS_LOG_PRIVINFO("Using default value ["<<_confWaitingAcknowledgeLimit<<"] for configuration 'PackageExporter.WaitingAcknowledgeLimit'");
    }

    //Get Min disk space required
    if ( ! conf.getValue("PackageExporter.MinSpaceRequiredMb", _confMinSpaceRequiredMb, false) ) {

        _confMinSpaceRequiredMb = availableDownloadSpaceSizeMb * 0.001;

        ACS_LOG_PRIVINFO("Using default value ["<<_confMinSpaceRequiredMb<<"] for configuration 'PackageExporter.MinSpaceRequiredMb'");
    }

    //Set Connections configuration
    size_t maxConnections = this->getDefaultConnectionPool()->getMaxConnection();
    if ( ! conf.getValue("PackageExporter.Connections.MaxConnections", maxConnections, false) ) {

        //Estimate the needed connections according to configured concurrency
        size_t maxDownloadThreads = 3;
        conf.getValue("DataProvider.maxDownloadThreads", maxDownloadThreads, false);

        size_t availableStots = 10;
        conf.getValue("PackageProcessing.AvailableStots", availableStots, false);

        maxConnections = maxDownloadThreads + availableStots + 5;

        ACS_LOG_PRIVINFO("Using default value ["<<maxConnections<<"] for configuration 'PackageExporter.Connections.MaxConnections'");
    }
    maxConnections = std::min( maxConnections, static_cast<size_t>(250) );
    ACS_LOG_PRIVINFO("Setting Max Connections to ["<<maxConnections<<"]");
    this->getDefaultConnectionPool()->setMaxConnection( maxConnections );

    //Time windows configuration
    {
        _timeIntervals.clear();

        rsPushNSpace confNs( conf, "List_of_TimeWindows" );

        // get time window list for Download Actions
        int numSM = conf.getArraySize("TimeWindow");
        if (!numSM) {
            ACS_LOG_WARNING("No time windows configured!");

        } else {

            for (int nr = 0; nr < numSM; ++nr) {
                // set the TimeWindow push name
                ostringstream tw_nameSpace; tw_nameSpace << "TimeWindow[" << nr << "]";

                timeInterval window;
                string timeStart(""); string timeStop("");

                conf.getValue(tw_nameSpace.str()+".timestart", timeStart);
                conf.getValue(tw_nameSpace.str()+".timestop", timeStop);

                ACS_LOG_PRIVINFO("timeStart = "<<timeStart);
                ACS_LOG_PRIVINFO("timeStop = "<<timeStop);

                convertTime(timeStart, window.timeStart);
                convertTime(timeStop, window.timeStop);

                if (window.timeStart >= window.timeStop) {

                    ACS_LOG_WARNING("Check configuration for TimeWindow: TimeStart >=  TimeStop; start= "
                        << timeStart << " stop= " << timeStop << " time window skipped!");
                    continue;
                }
                _timeIntervals.push_back(window);
            }
        }
    }


    //Data Prodiver configuration
    conf.getValue("DataProvider.ProductPath", _productPath);
    conf.getValue("PackageExporter.errorLog", _errorLog);

    if ( ! conf.getValue("PackageExporter.PollingTimer", _pollingTime, false) ) {
        ACS_LOG_WARNING("No <PollingTimer> parameter found in configuration. Set to: "<<_pollingTime<<" [sec]");

        Application::setStatus(Application::Warning, "Initialization error - default pollingTime value used!");
    }

    if ( !File::exists ( _downloadRepository ) ) {
        ACS_LOG_PRIVINFO("Download repository: "<<_downloadRepository<<" does not exists! Going to make it.");
        File::mkdirtree ( _downloadRepository );
    }
    const size_t freeDiskMb = File::getFreeSpaceInMB ( _downloadRepository );

    ACS_LOG_PRIVINFO("Free Disk Space: "<<freeDiskMb<<" [Mb]");
    ACS_LOG_PRIVINFO("Minimum Disk Space Required: "<<_confMinSpaceRequiredMb<<" [Mb]");

    if ( freeDiskMb < availableDownloadSpaceSizeMb ) {

        if ( freeDiskMb >= _confMinSpaceRequiredMb ) {
            _availableDownloadSpaceSize = (freeDiskMb - _confMinSpaceRequiredMb) * (1024*1024);
        } else {
            ostringstream os;
            os << "Few available space on disk: "<<freeDiskMb<<" [Mb]. Required "<<_confMinSpaceRequiredMb<<" [Mb]. Please stop the Package Exporter, free some space on disk and restart.";
            ACS_THROW( exPackageExporterException(os.str()) );
        }

        ostringstream os;
        os << "The Disk Space is less than configured for the cache." <<"\n";
        os << " The Package Exporter will adjust this value from:"<<"\n";
        os << " Cache reserved space read from the configuration file(s): " << availableDownloadSpaceSizeMb <<" [Mb]\n";
        os << " free Disk: " << freeDiskMb <<" [Mb]\n";
        os << " to:" <<"\n";
        os << " Cache reserved download space: " << (_availableDownloadSpaceSize / (1024*1024) ) <<" [Mb]\n";
        os << " free Disk: " << freeDiskMb <<" [Mb]\n";

        ACS_LOG_WARNING( os.str() );
    }
}


int PackageExporter::main(int argc, char const * const * argv, char const * const * env) {

    //Critical startup phase: in this phase a failure causes the immediate exit.

    //Lock the application.
    ApplicationLock appLock;
    appLock.lock(true); //exception is raised if lock fails

    //APF 273
    MCParameters::instance()->setDefaultFunction ("Data Dissemination");
    MCParameters::instance()->setDefaultCode ("DDP");

    // send Idle status to MCF
    ACS_LOG_EVENT("UPLOAD", EXFE_IDLE);

    //Open the first log - Log rotation policies can change after configuration loading. This is handled by Application::loadConfiguration
    Application::rotateLog();

    // daemonize if allowed by the base app
    daemonize();

    //Non-critical startup phase: in this phase a failure causes the daemon restart after a while.
    ACS_SMARTPTR< ScDefaultLogGroupFactory > EmbeddedScDefaultLogGroupFactoryAptr;
    CacheSptr cacheSptr;
    ACS_SMARTPTR< DataProvider > dataProviderAptr;
    ACS_SMARTPTR<PackageProcessingManager> packageProcessingManagerAptr;
    ACS_SMARTPTR<ResourceStats> resourceStatsAptr;
    DistributionListener *listener = 0 ;
    ACS_SMARTPTR<DistributionListener> listenerGuard;

    // No failures at startup until now, but can happen in the try block
    // there is the need to keep track of it
    bool gotFailure = false;
    try {

        // load configuration
        this->loadConf();

        //Factories instances - To be done after load configuration (because some conf spaces needed)!
        EmbeddedScDefaultLogGroupFactoryAptr.reset( new ScDefaultLogGroupFactory );

        // load plugins
        if ( ConfigurationSingleton::instance()->get().hasKey("PackageExporter.Plugins") ) {
            this->loadPlugins();
        }

        // Check for my distribution queue's intermediate status and force them to initial status (if allowed by conf)
        {
            dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
            dbConnection& conn = pw;

            ResetStatus resetStatus( _jobResponsible, conn );
            resetStatus.resetDistributionQueueStatus();
        }

        //Initialize download Cache
        double cacheEraseQuotaPcg(0.20);
        ConfigurationSingleton::instance()->get().getValue("PackageExporter.CacheEraseQuotaPcg", cacheEraseQuotaPcg, false);

        ACS_LOG_PRIVINFO("Cache download repository: "<<_downloadRepository) ;
        ACS_LOG_PRIVINFO("Cache download space size: "<<_availableDownloadSpaceSize<<" bytes.") ;
        ACS_LOG_PRIVINFO("space size in Gigabytes: " << _availableDownloadSpaceSize/1073741824L << " GB.") ;
        ACS_LOG_PRIVINFO("Cache erase quota percentage [0.0 .. 1.0]: "<<cacheEraseQuotaPcg);


        cacheSptr.reset( new Cache(_downloadRepository, _availableDownloadSpaceSize, cacheEraseQuotaPcg) );
        ExporterServiceResolver::instance()->setDownloadCacheSptr( cacheSptr );

        //Get Mediatypes to monitor
        vector<string> mediaTypes(0); //Empty vector means: all the mediaTypes
        ConfigurationSingleton::instance()->get().getArray( "PackageExporter.List_of_MediaTypes.MediaType", mediaTypes);
        {
            string mediaTypesStr("");
            for ( vector<string>::const_iterator it=mediaTypes.begin(); it!=mediaTypes.end(); ++it ) {
                mediaTypesStr += (mediaTypesStr=="")?(*it):(", "+ *it);
            }
            if (mediaTypesStr == "") { mediaTypesStr="All mediatypes";}

            ACS_LOG_PRIVINFO("Monitoring distribution queue for mediatypes ["<<mediaTypesStr<<"]");
        }

        //Initialize the DataProvider
        dataProviderAptr.reset( new DataProvider(mediaTypes, *cacheSptr, *Application::instance()->getDefaultConnectionPool() ) );

        //Init and Start the PackageProcessingManager
        packageProcessingManagerAptr.reset( new PackageProcessingManager );
        _packageProcessingManagerPtr = packageProcessingManagerAptr.get(); //_packageProcessingManagerPtr accessed only in this method's scope. Referenced because called by runExport.
        packageProcessingManagerAptr->start();

        //Init Resource Stats
        resourceStatsAptr.reset(
                new ResourceStats(	_confPendingRequestsLimit, _confWaitingAcknowledgeLimit,
                                    _confMinSpaceRequiredMb, _productPath,
                                    _jobResponsible, _packageProcessingManagerPtr ) );
        _resourceStatsPtr = resourceStatsAptr.get(); // accessed only in this method's scope. Referenced because called by runExport.

        // initialization of broadcast listener
        try {
            listener = new DistributionListener((*Application::instance()->getDefaultConnectionPool())) ;
            listener->startListening();
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x);
            ACS_LOG_WARNING("No listening to DB broadcasts (if any), there could be a latency up to: " << _pollingTime << "[s]");
        }
        listenerGuard.reset(listener) ;

    } catch(exception& ex) {

        ACS_LOG_PRIVINFO("Current application status: STARTUPFAILED.");

        ACS_LOG_ERROR("Got exception during the start-up phase.");
        ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_EVENT("UPLOAD", EXFE_ERROR);

        // Startup auto recovery has been disabled.
        // This because if the process is always up, trying to start over and over, it is difficult to detect
        // the anomaly through the monitoring and control system.
        // While, if the process is down, the anomaly is detected easily.
        // Keep track of the failure at startup
        gotFailure = true;
    }
    DataProvider& dataProvider = *dataProviderAptr;

    // If the startup was ok, do the actual service start
    if ( ! gotFailure ) {

        Timer pollingTimer;

        //******************** Set restart Time ***********************************
        wantToRestart(_restartTime);

        ACS_LOG_PRIVINFO("Restarting every "<<_restartTime<<" [sec]");
        ACS_LOG_PRIVINFO("Polling timer:   "<<_pollingTime<<" [sec]");

        //start main loop here
        _checkForNewProcessingOnNextMainLoop = true; //Check for new processing at the first loop.
        while (NoStop == getCurrentStopMode()) {

            try {

                // check for Db BroadCasts
 	        size_t messages_in_queue = 0; 
                if (0 != listener) {
		    const bool reset_queue (true) ;
		    listener->gotMessages(messages_in_queue, reset_queue) ; // Queue is cleared
                }
#warning MP LA POLITICA DI POLLING VA COMPLETAMENTE RIVISTA!
#warning MP QUESTO CICLO VIENE ESEGUITO SOLO CON UN PICCOLO DELAY
#warning MP PASSANDO UN PARAMETRO A runExport PER FARLO ESEGUIRE IN MODO LIGHT

                //Check if new eligibles can be taken into account
                pollingTimer.stop();

                bool checkForNewProcessing =
                        ( messages_in_queue || (pollingTimer.elapsed() / 1000 > _pollingTime) || _checkForNewProcessingOnNextMainLoop > 0);
                ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "\n"
                    <<"checkForNewProcessing                             : "<<checkForNewProcessing<<"\n"
		    <<"broadcast messages received                                   : "<< messages_in_queue <<"\n"
                    <<"pollingTimer.elapsed() / 1000 > _pollingTime      : "<< (pollingTimer.elapsed() / 1000) <<" > "<<_pollingTime <<"\n"
                    <<"_checkForNewProcessingOnNextMainLoop              : "<< _checkForNewProcessingOnNextMainLoop
                );

                // rotate log if needed
                Application::rotateLog();
                // end of log rotation
                // exit immediately if a TERM signal received
                if (ImmediateStop == getCurrentStopMode() ) {
                    break;
                }

                runExport(checkForNewProcessing, false, dataProvider, *cacheSptr);
                if (checkForNewProcessing) {
                    pollingTimer.start();
                }
                Timer::delay(100);

            } catch (exception &e) {

                //Here a critical and unexpected error occurred: db is down, status updating error because db status inconsistent, etc.
                //The application status here is not coherent with DB status: for example when a package status update failed
                // (eg. from downloading to distribution, from waitingack to completed) the order status and the t_distribution_lock
                // status is not coherent with application status.

                ACS_LOG_NOTIFY_EX(e);
                ACS_LOG_EVENT("UPLOAD", EXFE_ERROR);
		gotFailure = true;
		// force immediate stop
		setCurrentStopMode(ImmediateStop);

            }
        }
    } //Startup was ok

    if (listener) {
        listener->stopListening();
    }


    // if a stop signal has been received send Mcf Exit Messages
    if (NoStop != getCurrentStopMode())
    {
        sendMcfExitMessages();
    }
    // it should be useless...done just to be sure to stop running downloading threads in the case of failure and AppStopController returns noStop
    if ( NULL != dataProviderAptr.get() ) {
        dataProviderAptr->cleanExit();
    }

    if ( AppStopController().haltLazy() ) {
        // in case of Restart or SIG INT signal
        // Finish pending work and left all packages in a final status...

        dataProvider.cleanExit();
        try {
            //The Processing are stopped (this->stopConsumer(false);) calling runExport(,true,,)
            runExport(false, true, dataProvider, *cacheSptr);

        } catch(exception& ex) {
            ACS_LOG_ERROR("Package Exporter exiting due to occurred exception.");
            ACS_LOG_NOTIFY_EX(ex);

            // Force the immediate stop handling phase and keep track of the failure
            setCurrentStopMode (ImmediateStop);
            gotFailure = true;
        }
    }

    //Close the UserNotifier
    UserNotifier::instance()->close();

    if (ImmediateStop == getCurrentStopMode() ) {

        //Stop provider and consumer...
        dataProvider.cleanExit();
        this->stopConsumer(true);

        //Abort UserNotifier pending work
        ACS_LOG_PRIVINFO("Aborting Package Email Notifier...");
        UserNotifier::instance()->abort();
    }


    //Wait until UserNotifier commits pending work
    while ( ! UserNotifier::instance()->isCommitted() ) {
        ACS_LOG_PRIVINFO("Waiting Package Email Notifier...");
        Timer::delay(1000);
    }

    int retCode= gotFailure ? 0xFF:0x00;

    if (PhoenixRestart == getCurrentStopMode())
    {
        ACS_LOG_PRIVINFO ("Restarting the service...");
    }
    else
    {
        ACS_LOG_PRIVINFO("Exiting main with "<<retCode);
    }

    // Exit from main
    return retCode;

}

_ACS_END_NAMESPACE
