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

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.13  2017/05/29 17:53:17  nicvac
 OPMAN-260: PackageExporter concurrent Agents new algo (t_distributionruleslock to state change method)

 Revision 5.12  2017/04/26 14:57:04  nicvac
 S3MPM-878: bad memory access fixed. (During getPackageId string to int implementation it has been missed a string concatenation fix).

 Revision 5.11  2017/03/03 15:48:50  nicvac
 S2PDGS-1705: stack trace log messages.

 Revision 5.10  2017/02/21 17:02:44  nicvac
 S3EVOL-13: Inter process cache mode

 Revision 5.9  2017/02/02 09:49:20  davide.tiriticco
 S2PDGS-1655: packageId type changed from string to dbPersistent::IdType

 Revision 5.8  2016/12/16 16:55:45  nicvac
 S3MPM-860: PackageExporter resources improvement.

 Revision 5.7  2016/08/05 16:57:29  nicvac
 CSGACQ-86: Oracle porting

 Revision 5.6  2016/05/10 21:46:39  danalt
 use of ACS_THROW and ACS_COND_THROW

 Revision 5.5  2014/01/30 13:44:29  francesco.avanzi
 coding best practices applied & new db::Transaction used

 Revision 5.4  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.3  2013/10/28 17:34:09  marpas
 work in progress

 Revision 5.2  2013/10/03 14:08:01  nicvac
 acs sdk warnings processed.

 Revision 5.1  2013/07/15 16:47:18  marpas
 compilation warnings fixed

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards

 Revision 1.34  2013/05/17 15:33:01  nicvac
 S2PDGS-300: priority handling.

 Revision 1.33  2013/04/12 16:49:36  marpas
 SQLDialectFactory::dialectName() get db connection key

 Revision 1.32  2013/03/11 17:29:50  nicvac
 Update interface revised.

 Revision 1.31  2013/03/07 16:32:57  nicvac
 dbConv used

 Revision 1.30  2013/03/07 11:59:16  nicvac
 Support for RECOVERABLEERROR status (retry policy implementation).

 Revision 1.29  2013/03/05 15:48:49  nicvac
 Support for Retry policy added

 Revision 1.28  2013/03/05 13:59:59  nicvac
 rowCount is deprecated

 Revision 1.27  2013/03/04 17:41:31  nicvac
 update modified to support the new schema (retry handling)

 Revision 1.26  2012/11/27 14:41:48  nicvac
 Query all mediatypes

 Revision 1.25  2012/11/09 17:10:29  nicvac
 Set of mediatype handling.

 Revision 1.24  2012/10/19 15:04:06  micmaz
 updated library

 Revision 1.23  2012/10/08 13:59:19  micmaz
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.22  2012/09/19 15:57:11  micmaz
 ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.21  2012/09/14 16:31:32  micmaz
 S2GPP-611
 Development of Distribution Formatter framework for Export

 Revision 1.19  2012/09/11 16:42:31  micmaz
 removed compiling warning

 Revision 1.18  2012/01/23 12:03:53  micmaz
 removed LOG(...)

 Revision 1.17  2011/05/12 13:27:59  crivig
 changed interface in order to fix a bug for suspended packages

 Revision 1.16  2011/05/06 10:46:46  crivig
 modified for quality check; all i++ modified into ++i

 Revision 1.15  2011/03/11 11:16:42  crivig
 Timer::delay used in db retries

 Revision 1.14  2011/03/04 11:39:47  crivig
 integrated with new libCache

 Revision 1.13  2011/02/25 14:53:05  crivig
 bug on cartId fixed

 Revision 1.12  2010/12/17 10:25:28  crivig
 Cache class used; connection pool passed

 Revision 1.11  2010/12/03 15:00:49  crivig
 cache cleaning handled with Cache class

 Revision 1.10  2010/11/29 11:04:21  crivig
 changed Package to DataPckage; get**Status methods added

 Revision 1.9  2010/11/22 11:37:38  crivig
 libDownloadOptimiser eliminated! downloadPackData class used instead

 Revision 1.8  2010/11/05 13:11:19  crivig
 testing..

 Revision 1.7  2010/10/28 15:37:35  marpas
 version file name (and class) changed

 Revision 1.6  2010/10/28 13:48:31  crivig
 testing

 Revision 1.5  2010/10/27 10:45:00  crivig
 new cleanExit method added

 Revision 1.4  2010/10/22 13:41:39  crivig
 compiling

 Revision 1.3  2010/10/13 14:06:49  crivig
 compiled

 Revision 1.2  2010/10/06 13:55:14  crivig
 coding

 Revision 1.1.1.1  2010/10/01 13:17:27  crivig
 Imported new library

 */
#include <DataProvider.h>
#include "DataProviderConfigurer.h"

#include <File.h>
#include <HWScan.h>
#include <Transaction.h>

#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbInventoryObjectOperatorIF.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <Status.h>
#include <Cache.h>


#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>
#include <sstream>
#include <vector>

#include <algorithm>

#include <DataProviderV.h>



using namespace std;

_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)

namespace {
// version management
DataProviderV version;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL ( DataProvider );

// instantiate the order factories


DataProvider::DataProvider ( vector <string> const& mediaTypes, Cache& cache, dbConnectionPool& connectionPool ) :
							_confMaxConnRetry ( 100 ),
							_confMaxDownloadThreads ( 0 ),
							_confSupportLTA(false),
    						_mediaTypes ( mediaTypes ),
    						_jobResponsible(),
    						_cache ( cache ), // PRQA S 2528
    						_downloadThreads(),
    						_connectionPool ( connectionPool ), // PRQA S 2528
    						_stop ( false )
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DataProvider::DataProvider start " );

	// set job responsible from hostname
	_jobResponsible = HWScan::hostname() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Job responsible name for this daemon: " << _jobResponsible );
	// get maximum number of downloading threads from conf space
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	if ( not conf.getValue ( "DataProvider.maxDownloadThreads", _confMaxDownloadThreads, false)  ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "maxDownloadThreads set to default value: "<<_confMaxDownloadThreads );
	}

	if ( not conf.getValue ( "maxDbRetries", _confMaxConnRetry, false ) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "maxDbRetries set to default value: "<<_confMaxConnRetry );
	}

	if ( not conf.getValue("PackageExporter.LTASupport", _confSupportLTA, false) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "LTASupport set to default value: "<<_confSupportLTA );
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DataProvider::DataProvider end " );
}

DataProvider::~DataProvider() throw () 
{
	// PRQA S 4631 L1

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Destroying DataProvider instance" );
	size_t dtsize = _downloadThreads.size() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "              Threads to destroy: " << dtsize );
	for ( size_t i = 0; i < dtsize; ++i ) {
		delete _downloadThreads[i];
	}
	// PRQA L:L1
}

void DataProvider::cleanExit() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DataProvider::cleanExit() " );
	_stop = true;
	// waits for all download threads to exit

	size_t dtsize = _downloadThreads.size() ;
	for ( size_t i = 0; i < dtsize; ++i ) {
		_downloadThreads[i]->exit();
		while ( _downloadThreads[i]->running() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DataProvider::cleanExit thread " << i << "is running: sleep!" );
			_downloadThreads[i]->waitExitOrTimeout(100) ; // PRQA S 4400
		}
	}
}


void DataProvider::updateDistrStatus(std::string const& status, // PRQA S 4020
		std::vector<dbPersistent::IdType> const& packagesId,
		bool incrementRetry, bool updateRelatedItems) {

	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "updateDistrStatus  called" );
	if ( packagesId.empty() ) {
		return;
	}


	//Set up for transaction
	dbConnectionWrapper pw ( _connectionPool );
	dbConnection &conn = pw; // PRQA S 3050
	db::Transaction transaction( conn );

	// update status for an array of packages
	DateTime curdate;
	size_t connRetry = 0;
	const string &dbKey = conn.getKey() ;
	while ( connRetry < _confMaxConnRetry ) {
		try {

			//Begin transaction
			transaction.begin_work("updateDistrStatus");

			{ //update t_distributionqueue
				SQLString sqlString ( dbKey, SQLString::UPDATE );
				sqlString.addTableList ( "t_distributionqueue" );
				sqlString.addSetList ( "status", db::toSQL (dbKey, status ) );
				sqlString.addSetList ( "distributeddate", db::toSQL (dbKey, curdate, dbConv::YearToFraction )  );
				//distributeddate is always updated (for each status updates). 
				//distributeddate it's meaningful when status is COMPLETED (e.g. for report generation)
				//distributeddate also used in WAITINGACK status (to compute elapsed time since lask check)
#ifndef PDS_OLD_SCHEMA
				if ( incrementRetry ) {
					sqlString.addSetList ( "retryCounter", "retryCounter + 1" );
					sqlString.addSetList ( "lastattempt", db::toSQL (dbKey,  curdate, dbConv::YearToFraction ) ) ;
				}
#endif
				dbQueryParameters pp(dbKey) ;
				pp.addParameter ( dbParam (dbKey, "t_distributionqueue", "packageid", dbParam::IN, packagesId ) );
				sqlString.addANDWhereParameter ( pp );
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Executing query " << sqlString.getSQL() );

				dbQuery updatePackage ( conn, sqlString, "UPD DistrStatus" );
				updatePackage.execForStatus();
			}

			//Updates infos on related package's items (t_distributionitems)
			if ( updateRelatedItems ) {
				SQLString sqlString ( dbKey, SQLString::UPDATE );
				sqlString.addTableList ( "t_distributionitems" );
				sqlString.addSetList ( "distributeddate", db::toSQL (dbKey, curdate, dbConv::YearToFraction ) );
				dbQueryParameters pp(dbKey);
				pp.addParameter ( dbParam (dbKey, "t_distributionitems", "packageid", dbParam::IN, packagesId ) );
				sqlString.addANDWhereParameter ( pp );
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Executing query " << sqlString.getSQL() );

				dbQuery updatePackage ( conn, sqlString, "UpdDistQueueDistributedDate");
				updatePackage.execForStatus();
			}

			// if update is ok break loop
			transaction.commit();

			break;

		} catch ( exception const& e ) {

			//Roll back 
			transaction.roll_back();

			if ( 0 == connRetry ) {
				// send a warning the first time a connection problem has occured
				ACS_LOG_WARNING( "A problem connecting to db: " << e.what() );
				ACS_LOG_NOTIFY_EX(e);
			}
			if ( (connRetry == ( _confMaxConnRetry - 1 )) || _stop ) { // PRQA S 3084
				ostringstream msg;
				msg << "maximum number of db connection retries ("<< _confMaxConnRetry << ") performed!" ;
				ACS_THROW ( DataProviderException ( e, msg.str() ) );
			}
			Timer::delay ( 100 ); // PRQA S 4400
			connRetry++;
		}

	} // end for
}


void DataProvider::checkDownloadThreads ( vector<DataPackage>* goodPackages, vector<DataPackage>* failedPackages ) {
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "Start DataProvider::checkDownloadThreads()" );
	// Loop on all _downloadThreads elements
	size_t dtsize = _downloadThreads.size() ;
	for ( size_t counter = 0; counter < dtsize; ++counter ) {
		if ( _downloadThreads[counter]->running() ) {
			continue;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "DataProvider::checkDownloadThreads thread " << counter << " not running!" );
		DataPackage pckInfo = _downloadThreads[counter]->getPackage();
		// check if thread exit in error
		std::exception* e = 0;
		_downloadThreads[counter]->runException ( e );
		if ( e != 0 ) {
			ACS_LOG_ERROR( "Error occurred in download thread for package ["<<pckInfo.getPackageId()<<"]" );

			failedPackages->push_back ( pckInfo );

			ostringstream msg; msg<<"Exception occurred during item downloading for package ["<<pckInfo.getPackageId()<<"]";
			ACS_LOG_TRYCATCHNOTIFY_EX( DataProviderException ( *e, msg.str() ) );

			delete e;
			ACS_LOG_ERROR( "Stopping the download thread...");
			_downloadThreads[counter]->stop();
			while ( _downloadThreads[counter]->running() ) {
				_downloadThreads[counter]->waitExitOrTimeout(100) ; // PRQA S 4400
			}
			ACS_LOG_ERROR( "Thread stopped!");
			delete _downloadThreads[counter];
			_downloadThreads[counter] = 0 ;
			continue;
		}
		// check if thread finished correctly
		if ( _downloadThreads[counter]->isDone() ) {
			goodPackages->push_back ( pckInfo );
			delete _downloadThreads[counter];
			_downloadThreads[counter] = 0 ;
			continue;
		}
	}

	size_t i = _downloadThreads.size();
	// remove from _downloadThreads all the elements set to NULL
	while ( i > 0 ) {
		if ( _downloadThreads[i - 1] == 0 ) { // PRQA S 3084
			_downloadThreads.erase ( _downloadThreads.begin() + i - 1 ); // PRQA S 3000
		}
		i--;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "_downloadThreads.size() = " << _downloadThreads.size() );
}

bool DataProvider::bookPackage( const dbPersistent::IdType& packageId, dbConnection& conn ) throw() {

	bool booked=false;
	try {
		const string &dbKey = conn.getKey() ;

		// update status to PRECONDITIONCHECK and set job responsible in t_distributionqueue
		SQLString sqlString ( dbKey, SQLString::UPDATE );
		sqlString.addTableList ( "t_distributionqueue" );
		sqlString.addSetList ( "jobresponsible", db::toSQL (dbKey, _jobResponsible ) );
		sqlString.addSetList ( "status", db::toSQL (dbKey,  Status::preconditioncheck ) );
		{
			dbQueryParameters pp(dbKey);
			pp.addParameter ( dbParam (dbKey,  "t_distributionqueue", "packageid", dbParam::EQUAL_TO, db::toSQL(dbKey, packageId) ) );
			sqlString.addANDWhereParameter ( pp );
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Executing query " << sqlString.getSQL() );
		dbQuery updatePackage ( conn, sqlString, "BookDistQueue" );

		// if query is good
		if ( updatePackage.execForStatus() ) {
			booked = true;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Pkg n." << packageId << " updated to "<<Status::preconditioncheck<<" and kept by " << _jobResponsible << "! " );
		}
	} catch ( std::exception& ex) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Received exception while booking package [" << packageId
				<< "]. It is nominal in case of multiple exporter agents because concurrency booking mechanism." << ex );
	}

	return booked;
}

bool DataProvider::checkShortTermAvailability( const dbPersistent::IdType& packageId, dbConnection& conn, bool & restoreRequestQueued ) throw() {

	bool check = false;
	restoreRequestQueued = false;

	// Status here is PRECONDITIONCHECK. So, only one instance is executing on current packageId.

	try {
		const string &dbKey = conn.getKey() ;
		//Get the items of this package

		ostringstream sqlSStream;
		sqlSStream <<
			"SELECT inv.id_inv,  disit.id "
			"FROM "
			"t_inventory inv INNER JOIN t_distributionitems disit ON disit.filename = inv.filename AND disit.fileversion = inv.fileversion "
			"WHERE "
			"disit.packageid = "<<db::toSQL(dbKey, packageId)
		;

		SQLString querySQL ( dbKey, sqlSStream.str() );

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkShortTermAvailability executing query " << querySQL.getSQL() );
		dbSet set;
		dbQuery query ( conn, querySQL, "Package Check Items availability" );
		query.execForSet ( set );

		//Prepare the inventory object
		dbInventoryObject *invPtr=dynamic_cast<dbInventoryObject*> (dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", dbKey));
		ACS_COND_THROW(0 == invPtr, exIllegalValueException("Cannot dynamic_cast for dbInventoryObject")); // PRQA S 3081
		dbPersistentScopeGuard sGuard(invPtr);

		//Compute the Inventory items' set that are not currently available on Synch Storage Manager
		//For each Inventory item of current package
		dbRow item(dbKey);
		while ( set.getNextRow(item) ) {
			dbPersistent::IdType invId ;
			dbPersistent::IdType distributionItemId ;

			size_t idxc(0);
			item.column ( idxc++, invId );
			item.column ( idxc++, distributionItemId );

			invPtr->load(conn, invId);
			dbInventoryObjectOperatorIF invIF;
			const bool isAvailableItem = invIF.canGetFromSynchSM( *invPtr, conn);

			if ( not isAvailableItem ) {

				ostringstream itemInfo; itemInfo<<"Package id ["<<packageId<<"]: Inventory item ["<<invPtr->getId()<<"] "<<"["<<invPtr->getFileName()<<"; "<<invPtr->getFileVersion()<<"] ";

				ACS_LOG_PRIVINFO( itemInfo.str() << " is not currently available on Synch storage manager.");

				const bool isOnAsyncSM = invIF.isOnAsyncSM( *invPtr, conn);
				ACS_COND_THROW( not isOnAsyncSM,  DataProviderException( itemInfo.str()+". is not available on any Synch or Asynch storage manager")); // PRQA S 3081

				{
					//Annotate the inventory item in the t_distributionWaitingItems.
					// t_distributionWaitingItems is purged by triggers at item's restoring from LTA.
					SQLString sql( dbKey, SQLString::INSERT );
					sql.addTableList("t_distributionWaitingItems");
					sql.addValuesList("distributionitem_id", db::toSQL(dbKey, distributionItemId));
					sql.addValuesList("package_id"         , db::toSQL(dbKey, packageId));
					sql.addValuesList("inventory_id"       , db::toSQL(dbKey, invId));

					dbQuery dbq(conn, sql, "Insert distribution waiting item");
					const bool insertOk = dbq.execForStatus();
					ACS_COND_THROW( not insertOk,  DataProviderException( itemInfo.str()+". Cannot insert entry in t_distributionWaitingItems")); // PRQA S 3081
				}

				//Queue the restore request from Asynch Storage manager
				pair<bool, dbPersistent::IdType> statusRestoreRequest = invIF.restoreOnSynchSM( *invPtr, conn );
				if( statusRestoreRequest.first ) {
					ACS_LOG_PRIVINFO( itemInfo.str() << " is going to be restored on synchronous SM: request id is ["<<statusRestoreRequest.second<<"]");

					//Restore Request queued. Entry in t_distributionWaitingItems added.
					restoreRequestQueued = true;

				} else {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, itemInfo.str() << " already available on synchronous SM");
					//Race condition. In the mean time the inventory item has been restored.
					// Delete the entry from t_distributionWaitingItems (if not already automatically done by trigger/stored procedure).
			        dbQueryParameters pp(dbKey) ;
				    pp.addParameter(dbParam(dbKey, "t_distributionWaitingItems","distributionitem_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, distributionItemId) )) ;

					SQLString sql( dbKey, SQLString::DELETE );
					sql.addTableList("t_distributionWaitingItems");
				    sql.addANDWhereParameter(pp) ;

					dbQuery dbq(conn, sql, "Delete distribution waiting item (race condition)");
					const bool deleteOk = dbq.execForStatus();
					ACS_COND_THROW( not deleteOk,  DataProviderException( itemInfo.str()+". Cannot delete entry from t_distributionWaitingItems")); // PRQA S 3081
				}
			}
		}

		check = true;

	} catch ( std::exception& ex ) {
		check = false;
		ACS_LOG_ERROR("Error during Short Term Availability check: " << ex );
	}

	return check;
}


size_t DataProvider::checkEligiblePackages ( vector<DataPackage>* failedPackages, // PRQA S 4020
		vector<DataPackage>* suspendedPackages ) {
	//if maximun threads number reached do nothing
	long availableThreadsNum = _confMaxDownloadThreads - _downloadThreads.size(); // PRQA S 3000, 3084
	if ( availableThreadsNum < 1 ) {
		return 1;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Check Eligible Packages in database for available threads number = " << availableThreadsNum );

	dbSet eligibleSet;
	dbConnectionWrapper pw ( _connectionPool );
	dbConnection& conn = pw; // PRQA S 3050
	conn.setSingleStepWaitSeconds ( 60 ); // PRQA S 4400
	conn.setLockWaitSeconds ( 120 ); // PRQA S 4400
	const string &dbKey = conn.getKey() ;
	string distItemTable = "t_distributionitems";
	string distQueueTable = "t_distributionqueue";
	// select availableThreadsNum ELIGIBLE packages
	try {

		// Reminder: so far it's not possible to use SQLString and dbQueryParameters (4.7) also because "a and ( b or (c and d) )" is not supported

		//So far Postres and Oracle are supported. Query contains non standard elements like "Interval 1 seconds"
		//If needed, rewrite this query to support other DBs.
		string dialectName = SQLDialectFactory::dialectName(dbKey);
		transform(dialectName.begin(), dialectName.end(), dialectName.begin(), ::tolower);

		string sqlIntervalOneSecond;
		string sqlLimitLead; string sqlLimitTrail;
		if 			(dialectName.find( "postgres" ) != string::npos ) {
			sqlIntervalOneSecond = "INTERVAL '1 seconds'";
			sqlLimitLead="";
			sqlLimitTrail="FETCH FIRST "+ db::toSQL(dbKey, availableThreadsNum) +" ROWS ONLY";

		} else if	(dialectName.find( "oracle" ) != string::npos ) {
			sqlIntervalOneSecond = "NUMTODSINTERVAL(1, 'second')";
			sqlLimitLead="SELECT * FROM (";
			sqlLimitTrail=") WHERE ROWNUM <= "+ db::toSQL(dbKey, availableThreadsNum);

		} else {
			ACS_THROW( DataProviderException("DataProvider::checkEligiblePackages: SQL query is not defined to support ["+dialectName+"]") ); // PRQA S 3081
		}

		//First step: RECOVERABLEERROR --> ELIGIBLE (concurrency is resolved by jobresponsible)
		{
			DateTime nowTime;

			ostringstream sqlSStream;
			sqlSStream <<
					"UPDATE t_distributionqueue "
					"SET status = "<< db::toSQL(dbKey, Status::eligible) <<" "
					"WHERE packageid IN ( "
						"SELECT  t_distributionqueue.packageid "
						"FROM "
						"   (( t_distributionitems "
						"      INNER JOIN t_distributionqueue ON t_distributionqueue.packageid = t_distributionitems.packageid ) "
						"      INNER JOIN t_distributionrules ON t_distributionrules.id_distributionrule = t_distributionitems.distributionrule_id ) "
						"WHERE "
						"t_distributionqueue.status = "<<db::toSQL(dbKey, Status::recoverableError)<<" "
						"AND t_distributionqueue.jobresponsible = "<<db::toSQL(dbKey, _jobResponsible )<<" "
						"AND (t_distributionqueue.lastattempt + t_distributionrules.retryperiod_sec * "<<sqlIntervalOneSecond<<" ) < "<<db::toSQL(dbKey, nowTime, dbConv::YearToFraction)<<" "
					")";

			SQLString querySQL ( dbKey, sqlSStream.str() );

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Executing query " << querySQL.getSQL() );
			dbQuery updatePackage ( conn, querySQL, "RecoverDistQueue" );

			if ( not updatePackage.execForStatus() ) {
				ACS_LOG_PRIVINFO("Error updating from "<<Status::recoverableError<<" to "<<Status::eligible<<". Retrying on the next working loop.");
			}

		}

		//Second step: BOOK ELIGIBLE (concurrently)
		ostringstream sqlSStream;
		sqlSStream << sqlLimitLead <<" ";
		sqlSStream <<
			"SELECT "
			"t_distributionqueue.packageid, t_distributionqueue.format, "
			"t_distributionrules.priority, "
			"( MIN  (  t_distributionitems.duedate ) )  AS duedate "
			"FROM  "
			"((( "
			"   t_distributionitems "
			"   INNER JOIN t_distributionqueue ON t_distributionqueue.packageid = t_distributionitems.packageid ) "
			"   INNER JOIN t_distributionrules ON t_distributionrules.id_distributionrule = t_distributionitems.distributionrule_id ) ";
		sqlSStream << ( _confSupportLTA ?
			"   LEFT  JOIN t_distributionWaitingItems ON t_distributionWaitingItems.package_id = t_distributionqueue.packageid ) " : ") " );
		sqlSStream <<
			"WHERE "
			"t_distributionqueue.status = "<< db::toSQL(dbKey, Status::eligible) <<" "
			"AND ( t_distributionqueue.jobresponsible = "<<db::toSQL(dbKey, _jobResponsible )<<" OR t_distributionqueue.jobresponsible IS NULL ) ";
		sqlSStream << ( _confSupportLTA ?
			"AND t_distributionWaitingItems.package_id IS NULL " : " " ) ; //i.e. all the items are available on synch SM
		if ( not _mediaTypes.empty() ) {
			sqlSStream <<
			"AND t_distributionitems.mediatype IN ( ";
			for ( vector<string>::iterator it=_mediaTypes.begin(); it!=_mediaTypes.end(); ++it ) { // PRQA S 4238
				if ( it !=_mediaTypes.begin() ) { sqlSStream<<", " ; }
				sqlSStream<< db::toSQL(dbKey, *it ) ;
			}
			sqlSStream << " ) ";
		}
		sqlSStream <<
			"GROUP BY "
			"t_distributionqueue.packageid, "
			"t_distributionqueue.format, "
			"t_distributionrules.priority "
			"ORDER BY "
			"t_distributionrules.priority "<< ( (acs::conf::DPConfigurerSingleton::instance()->getLowerPriorityFirst())?"ASC":"DESC" ) << // PRQA S 3380
			", duedate ASC ";
		sqlSStream << sqlLimitTrail;

		SQLString querySQL ( dbKey, sqlSStream.str() );

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkEligiblePackages  executing query " << querySQL.getSQL() );
		dbQuery query ( conn, querySQL, "SelDistQueue" );
		query.execForSet ( eligibleSet );

	} catch ( exception const& e ) {
		ACS_LOG_ERROR( "Cannot retrieve ELIGIBLE packages." );
		ACS_LOG_NOTIFY_EX(e);

		return 0;
	}

	// loop on all packages found
	size_t eligiblePackagesNumber = eligibleSet.size();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Number of ELIGIBLE packages found: " << eligiblePackagesNumber );

	for ( size_t counter = 0; counter < eligiblePackagesNumber; ++counter ) {
		if ( _stop ) { //skip if a signal was received.
			return counter;
		}

		dbRow pkg(dbKey);
		eligibleSet.getRow ( counter + 1, pkg ); // PRQA S 3084

		// get pkg Id and format
		dbPersistent::IdType packageId ;

		pkg.column ( 0, packageId );
		string format;
		pkg.column ( 1, format );


		//Book this distribution
		if ( ! this->bookPackage( packageId, conn ) ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Pkg n." << packageId << " has been booked by another export agent, thus continue with the next package." );
			continue;
		}
		// Booked a distribution
		// Here the distribution order is in PRECONDITIONCHECK status

		//Check if items of current distribution are available on Synch Storage Manager or they need to be restored from LTA
		if ( _confSupportLTA ) {
			bool restoreRequestQueued = false;
			if ( not this->checkShortTermAvailability( packageId, conn, restoreRequestQueued ) ) {
				//Error is logged in checkShortTermAvailability
				DataPackage package ( packageId );
				failedPackages->push_back( package  );
				continue;
			}
			if ( restoreRequestQueued ) {
				ACS_LOG_PRIVINFO( "Pkg n." << packageId << " cannot be exported right now. One or more items in the package need to be restored from Long Term Archive. "
						"Restore request from LTA has been queued. The Package will be exported after restoring has been fulfilled.");

				//Reset the package to eligible and continue with the next package
				std::vector<dbPersistent::IdType> packagesId(1, packageId );
				this->updateDistrStatus( Status::eligible,  packagesId, false, false );
				continue;
			}
		}

		// create a new Package
		DataPackage package ( packageId );
		try {
			// if package cannot be filled, suspend package
			if ( !package.fillPackage ( conn ) ) {
				ACS_LOG_PRIVINFO( "Pkg n." << packageId << " cannot be filled, thus will be SUSPENDED and continue with the next package." );
				suspendedPackages->push_back ( package );
				continue;
			}

		} catch ( exception& ex ) {
			// if Critical error while filling package ==>
			//  go from PRECONDITIONCHECK to ERROR/RECOVERABLEERROR
			ACS_LOG_ERROR( "Received exception for package [" << packageId << "]. " << ex );

			failedPackages->push_back ( package );
			continue;
		}

		try {
			// update status to DOWNLOADING and job responsible in t_distributionqueue
			SQLString sqlString ( dbKey, SQLString::UPDATE );
			sqlString.addTableList ( distQueueTable );
			sqlString.addSetList ( "jobresponsible", db::toSQL (dbKey, _jobResponsible ) );
			sqlString.addSetList ( "status", db::toSQL (dbKey, Status::downloading ) );
			{
				dbQueryParameters pp(dbKey);
				pp.addParameter ( dbParam (dbKey,  distQueueTable, "packageid", dbParam::EQUAL_TO, db::toSQL(dbKey, packageId) ));
				sqlString.addANDWhereParameter ( pp );
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Executing query " << sqlString.getSQL() );
			dbQuery updatePackage ( conn, sqlString, "UPD "+distQueueTable );

			ACS_COND_THROW( ! updatePackage.execForStatus(),
					DataProviderException( "Error updating order to DOWNLOADING status" ));

			// Here the distribution order is in DOWNLOADING status
			{
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Pkg n." << packageId << " updated to DOWNLOADING_STATUS and kept by " << _jobResponsible << "! " );

				//create new Thread and start it.
				DownloadPackData* downloadPackData = new DownloadPackData ( package, _connectionPool, _cache );
				_downloadThreads.push_back ( downloadPackData );
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Start downloading Pkg n." << packageId );
				downloadPackData->start();
				// sending to M&C the facility starting
				exFEvent mc ( "DISTRIBUTION", EXFE_BUSY );
				excmcf << mc;
			}
		} catch ( std::exception const& e ) {
			//If updating downloading went wrong or downloader cannot start ==>
			// go from PRECONDITIONCHECK/DOWNLOADING to ERROR/RECOVERABLEERROR

			failedPackages->push_back ( package );

			ACS_LOG_ERROR( " Received exception for package [" << packageId << "]");
			ACS_LOG_NOTIFY_EX(e);
		}

	} /* for */

	return eligiblePackagesNumber;

}

_ACS_END_NESTED_NAMESPACE
