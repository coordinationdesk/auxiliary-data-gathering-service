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

	$Prod: A.C.S. libDefaultDistributorCreator Library $

	$Id$

	$Author$

	$Log$
	Revision 2.15  2016/07/13 15:12:46  marfav
	CSGACQ-86 oracle sql code reviewed and fixed
	
	Revision 2.14  2016/04/11 13:37:35  marpas
	adopting new db interfaces
	
	Revision 2.13  2016/03/26 21:36:55  marpas
	using new db I/F
	
	Revision 2.12  2016/03/21 18:51:12  marpas
	dbConnection can be got from pool only
	
	Revision 2.11  2015/12/10 10:54:25  matteo.airoldi
	Bug on ifdef PDS_OLD_SCHEMA for Postgres8 Dialect fixed
	
	Revision 2.10  2014/07/29 16:02:19  marpas
	qa & gcc warnings fixed
	
	Revision 2.9  2014/02/05 15:39:15  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.8  2013/05/14 18:50:49  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.7  2013/04/12 12:19:58  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.6  2013/04/09 08:22:03  marpas
	NameIdAssociator refactoring
	
	Revision 2.5  2013/03/12 17:55:22  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.4  2013/02/26 18:30:47  marpas
	new dbSet I/F adopted
	
	Revision 2.3  2013/02/11 09:46:06  marpas
	using  dbConnection::localMaster method
	
	Revision 2.2  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.1  2013/02/04 15:41:22  marpas
	disseminationpackingalgo_id transferred from temporarydistribution to distributionitems
	
	Revision 2.0  2013/01/21 18:02:23  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.23  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.22  2010/08/26 09:25:14  marpas
	Oracle support added
	
	Revision 1.21  2010/07/29 12:32:41  marpas
	bug fixed in distribution w/ due-days
	
	Revision 1.20  2010/05/06 09:18:23  marpas
	procedure p_distribute_item_wdate integrated. Its call is conditioned to the presence of the tag <Due_Days_Interval> into the data block section.
	
	Revision 1.19  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.18  2010/04/08 09:58:24  crivig
	Oracle db handled
	
	Revision 1.17  2009/09/28 17:02:34  marpas
	dbParam interface obsolecence
	
	Revision 1.16  2009/06/25 09:19:09  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.15  2009/03/24 09:57:46  crivig
	modified in order to handle new fields in t_distributionitems for carts
	
	Revision 1.14  2009/02/03 10:09:47  crivig
	timestamp for informix, tstamp for postgres
	
	Revision 1.13  2009/01/22 13:57:54  marpas
	was not deleted
	
	Revision 1.12  2009/01/22 13:37:34  ivafam
	Bug Fixed
	
	Revision 1.11  2009/01/22 13:20:32  marpas
	use new stored procedure
	
	Revision 1.10  2009/01/21 18:59:12  paoscu
	Postgres dialect supported.
	
	Revision 1.9  2008/12/11 13:13:11  marpas
	Added Venus Redistribution
	
	Revision 1.8  2008/11/17 11:24:00  crivig
	timestamp switched to tstamp
	
	Revision 1.7  2008/09/17 08:29:02  crivig
	bug fixed in porting
	
	Revision 1.6  2008/09/16 10:28:31  crivig
	porting to postgres8
	
	Revision 1.5  2008/02/15 12:55:47  crivig
	tag modified to User-ID
	
	Revision 1.4  2007/04/24 09:57:55  enrcar
	bug on connection solved; bug on NULL orderid solved
	
	Revision 1.3  2007/04/12 08:10:53  crivig
	tested
	
	Revision 1.2  2007/04/11 09:55:33  crivig
	delete from temporary distribution
	
	Revision 1.1  2007/04/02 14:46:39  crivig
	*** empty log message ***
	
*/
 

#include <RedistributionDistributorCreator.h>
#include <DistributionAlgo.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbDownloadFile.h>
#include <XMLIstream.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <dbInventoryObjectSpecialQuery.h>
#include <dbInventoryValIntersectQuery.h>
#include <Transaction.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbGenQueryScopeGuard.h>
#include <dbFileActionScopeGuard.h>
#include <dbFileActionFactoryChain.h>
#include <SatelliteName.h>
#include <rsPushNSpace.h>

#include <ConfigurationSingleton.h>
#include <Filterables.h>
#include <DateTime.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RedistributionDistributorCreator);

RedistributionDistributorCreator::RedistributionDistributorCreator(const string &algo) :
	DistributorCreator(algo),
    _tmpDownloadPath("/tmp/")
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); 
	// get from configuration the ageLimit after which files have to be deleted
	string key ;
	try 
	{
		getConf("TemporaryDownloadPath", key,  _tmpDownloadPath, "") ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key << " set to default value " << _tmpDownloadPath);
	}
}

RedistributionDistributorCreator::~RedistributionDistributorCreator() throw() 
{
}

void RedistributionDistributorCreator::emitVenusDistribution(
    dbConnectionPool &pool,
    rsResourceSet &resSetDBlock, 
    const TemporaryInfo& tmpInfo, 
    dbConnection &theConnection) const // PRQA S 4214
{
	// get start and stop time
	rsPushNSpace dblockns(resSetDBlock,"Data_Block") ;
	
	// read the number of products
	size_t productsNum = resSetDBlock.getArraySize ("List_of_Products.Product"); // PRQA S 4412

	// and the whole users array
	vector<string> users ;
    resSetDBlock.getArray ("List_of_UserIDs.UserID", users);
	
	// now read the array of products
	vector< pair<string, string> > products ;

	for (size_t i=0; i < productsNum; i++) {
		ostringstream key ;
		key << "List_of_Products.Product[" << i << "]" ;
		rsPushNSpace productIns(resSetDBlock,key.str()) ;
		string name ;
		resSetDBlock.getValue("File_Name", name) ;
        string version ;
		resSetDBlock.getValue("File_Version", version) ;
		products.push_back(pair<string, string>(name, version)) ;
	}
	
	// we need also the filetypes for each file
	// let's load them
	vector<string> filetypes ;
    size_t size =  products.size() ;

	const string &dbKey = theConnection.getKey();

	for (size_t i=0; i < size; ++i) {
		SQLString fTypeString(dbKey, SQLString::SELECT) ;
		fTypeString.addTableList("t_inventory") ;
		fTypeString.addSelectList("filetype") ;
		dbQueryParameters pftype(dbKey) ;
		pftype.addParameter( dbParam(dbKey, "t_inventory","filename" , dbParam::EQUAL_TO, db::toSQL(dbKey, products[i].first)) ) ; // PRQA S 3050, 3081
		pftype.addParameter( dbParam(dbKey, "t_inventory","fileversion" , dbParam::EQUAL_TO, db::toSQL(dbKey, products[i].second)) ) ; // PRQA S 3050, 3081
		fTypeString.addANDWhereParameter(pftype) ;	
        dbConnectionWrapper pw(pool) ;
        dbConnection &conn = pw ; // PRQA S 3050 
		dbQuery queryFtype(conn, fTypeString, "FileTypeRetrieval") ;
        dbSet set ;
		queryFtype.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081
		dbRow row(dbKey) ;
        set.getNextRow(row) ; // first and only 

		string ftype ;
		row.column(0,ftype) ;
		filetypes.push_back(ftype) ;
	}				
					
					


	// and now ... let's emit distribution
	string dialectName = SQLDialectFactory::dialectName(theConnection.getKey());
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ;
	try {
		// for each product ... 
        size_t productSize = products.size() ;
		for (size_t i=0; i < productSize; ++i) {
			// for each user .. 
            size_t userSize = users.size() ;
			for (size_t j=0; j < userSize; ++j) {
				string execObj;
                
				if( dialectName == "Informix9"  )
				{
					execObj =  "EXECUTE PROCEDURE p_distribute_item_on_demand( ";
				} 
				else if(  dialectName == "Postgres8" )
				{
					execObj =  "SELECT p_distribute_item_on_demand( ";
				} 
				else if(  dialectName == "Oracle11" ){
					execObj =  "EXECUTE p_distribute_item_on_demand( ";
				}
                else {
                    ACS_THROW( exIllegalValueException("Unknown db: " + dialectName )); // PRQA S 3081
                }
				execObj+= db::toSQL(dbKey,products[i].first); // PRQA S 3050, 3081
				execObj+=  ", ";
				execObj+= db::toSQL(dbKey,products[i].second); // PRQA S 3050, 3081
				execObj+= ", ";
				execObj+= db::toSQL(dbKey,filetypes[i]); // PRQA S 3050, 3081
				execObj+= ", ";
				execObj+= db::toSQL(dbKey,users[j]); // PRQA S 3050, 3081
				execObj+= ", NULL, NULL, NULL )" ;

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << execObj << "\"") ;
                   
				SQLString qryStr(dbKey, SQLString::COMMAND);
				qryStr.forceSQLString(execObj) ;
				dbQuery qry(theConnection, qryStr, "p_distribute_item_on_demand") ;
				if (!simulation()) {
					qry.execForStatus() ;
				}
				else {
					ACS_LOG_INFO("SIMUL MODE no PROCEDURE p_distribute_item_on_demand executed!");
				}
			}
            
            vector<dbPersistent::IdType> ids ;
            ids.push_back(tmpInfo.id) ;
            deleteFromTempDist(ids, theConnection) ;
            
		}
		
		transaction.commit() ;
	}
	catch(exception &x)  {
		transaction.roll_back() ;
		throw;
	}
}

void RedistributionDistributorCreator::emitDistribution(const rsResourceSet &resSetDBlock, const TemporaryInfo& tmpInfo, dbConnection &theConnection) const // PRQA S 4214
{	
	// get start and stop time
	string startTimeKey = "Data_Block.Time_Interval.Start" ;
	string startTime;
	resSetDBlock.getValue(startTimeKey,startTime) ;
	
	string stopTimeKey = "Data_Block.Time_Interval.Stop" ;
	string stopTime;
	resSetDBlock.getValue(stopTimeKey, stopTime) ;
	
	int duedays = 0 ; 
	try {
		resSetDBlock.getValue("Data_Block.Due_Days_Interval", duedays) ;
        ACS_LOG_INFO("Due Interval [days] for distribution is: " << duedays );
  	}
	catch(exception &) {	
		ACS_LOG_INFO("No <Data_Block><Due_Days_Interval> tag found. Proceeding with standard distribution" );
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "start time: " << startTime << " stop Time" << stopTime );

	// get file type number and users number
	string fileTypeKey = "Data_Block.List_of_File_Types.File_Type" ;
	size_t numFileTypes = resSetDBlock.getArraySize(fileTypeKey) ;

	string userKey = "Data_Block.List_of_Users.User-ID" ;
	size_t numUsers = resSetDBlock.getArraySize(userKey) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "temporary distribution file " << 
            							"contains " << numFileTypes << 
										" file types and " << numUsers << 
										" users." );

	// get all the file types and users
	vector<string> fileTypes;
	vector<string> usersList;
	const string &dbKey = theConnection.getKey();
	for (size_t i=0; i < numFileTypes; ++i) 
	{
		ostringstream basekey ;
		basekey << fileTypeKey << "[" << i << "]" ;
		// read each file type
		string tmpFileType;
		resSetDBlock.getValue(basekey.str(), tmpFileType);
		fileTypes.push_back(db::toSQL(dbKey,tmpFileType)); // PRQA S 3050, 3081
	}
	for (size_t i=0; i < numUsers; ++i) 
	{
		ostringstream basekey ;
		basekey << userKey << "[" << i << "]" ;
		// read each file type
		string tmpUser;
		resSetDBlock.getValue(basekey.str(), tmpUser);
		usersList.push_back(tmpUser);
	}




	// get all inventory object with validity-start-stop that intersect (startTime, stopTime)
	// and with filetype in fileTypes  and user in usersList
	dbInventoryObjectSpecialQuery* Invquery=dynamic_cast<dbInventoryValIntersectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryValIntersectQuery", theConnection.getKey())); // PRQA S 3081, 4412
	if(!Invquery)
	{
		
		ACS_THROW(exIllegalValueException("Dynamic_cast failure.")); // PRQA S 3081
	}
	dbGenQueryScopeGuard theQuesryScopeGuard(Invquery) ;

	//Set start time and stop time for intersect
	Invquery->t0( DateTime(startTime) ); // PRQA S 3081 2
	Invquery->t1( DateTime(stopTime) );

	dbQueryParameters parameters(dbKey) ; 
	parameters.addParameter(dbParam(dbKey, "t_inventory","filetype", dbParam::IN, fileTypes)) ;
	Invquery->addParameter(parameters) ;
	Invquery->doQuery(theConnection) ;

    ACS_LOG_INFO(Invquery->size() << " records loaded." );
	
	string dialectName = SQLDialectFactory::dialectName(theConnection.getKey());
	if (not Invquery->empty())
	{
		db::Transaction transaction(theConnection) ;
		transaction.begin_work("") ;
		try {
            size_t usersSize = usersList.size() ;
			for(size_t j=0; j< usersSize; ++j) { // loop over users
                size_t invSize = Invquery->size() ;
				for (size_t i=0; i< invSize; ++i) { // loop over inventories
					SQLString orderTypeString(dbKey, SQLString::SELECT) ;
					orderTypeString.addTableList("t_ordersqueue") ;
					orderTypeString.addSelectList("ordertype") ;
					dbQueryParameters pOrd(dbKey) ;

					/* Check if orderId is NULL -- if NOT NULL, orderIdSet is true */
					bool orderIdSet ;  // PRQA S 4101
					((*Invquery)[i])->getOrderId(&orderIdSet) ;

					

       				if(orderIdSet) {
						pOrd.addParameter( dbParam(dbKey, "t_ordersqueue","orderid" , dbParam::EQUAL_TO, db::toSQL(dbKey,((*Invquery)[i])->getOrderId())) ) ;  // PRQA S 3050, 3081
					}
                    else {
						pOrd.addParameter( dbParam(dbKey, "t_ordersqueue","orderid" , dbParam::ISNULL) );
                    }
					
					orderTypeString.addANDWhereParameter(pOrd) ;

					dbQuery queryOrder(theConnection, orderTypeString, "t_ordersqueue w/oid") ;	
					dbSet set ;
					queryOrder.execForSet(set) ;
					string ordertype = "NULL";
                    size_t setSize = set.size() ;
					for (size_t k=0; k < setSize; ++k)
					{
						dbRow row (dbKey);
						set.getNextRow(row) ;
						row.column(0, ordertype) ;
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ordertype =  " <<ordertype );
					}
				
				
					if(ordertype.length() == 0) { ordertype="NULL"; }
					string execObj;
					if ( dialectName == "Informix9" )
					{
						execObj =  "EXECUTE PROCEDURE ";
					}
					else if ( dialectName == "Postgres8" )
					{
						execObj =  "SELECT ";
					}
					else if (dialectName == "Oracle11")
					{
						execObj =  "EXECUTE ";
					} 
					else
					{
						ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
					}
					
						if (duedays != 0) {
							execObj +=  " p_distribute_item_wdate( ";
                        }
						else {
							execObj +=  " p_distribute_item( ";
                        }
			
						execObj+= db::toSQL(dbKey,((*Invquery)[i])->getFileName()); // PRQA S 3050, 3081
						execObj+=  ", ";
						execObj+= db::toSQL(dbKey,((*Invquery)[i])->getFileVersion()); // PRQA S 3050, 3081
						execObj+= ", ";
						execObj+= db::toSQL(dbKey,((*Invquery)[i])->getFileType()); // PRQA S 3050, 3081
						execObj+= ", ";
						execObj+= db::toSQL(dbKey,usersList[j]); // PRQA S 3050, 3081
						execObj+=  ", ";
						execObj+= db::toSQL(dbKey,ordertype) ; // PRQA S 3050, 3081
						execObj+= ", NULL, NULL" ;
						if (0 != duedays) {
							execObj+= string(",") + string(db::toSQL(dbKey,duedays)) ; // PRQA S 3050, 3081
                        }
						execObj+= " )" ;
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << execObj << "\"" );

					SQLString qryStr(dbKey, SQLString::COMMAND);
					qryStr.forceSQLString(execObj) ;
					dbQuery qry(theConnection, qryStr, (duedays != 0) ? "p_distribute_item_wdate": "p_distribute_item") ; // PRQA S 3380
					if (!simulation()) {
						qry.execForStatus() ;
					}
					else {
                        ACS_LOG_INFO("SIMUL MODE no PROCEDURE p_distribute_item_wdate executed!");
					}

				}
			}
			
		    vector<dbPersistent::IdType> ids ;
            ids.push_back(tmpInfo.id) ;
            deleteFromTempDist(ids, theConnection) ;
			
			transaction.commit() ;
		}
		catch(exception &x)  {
			transaction.roll_back() ;
			throw;
		}
	}

}


void RedistributionDistributorCreator::downloadAndEmitDistribution( // PRQA S 4020, 4214
    const TemporaryInfo& tmpInfo, 
    dbInventoryObject *obj, 
    dbConnection &theConnection,
    dbConnectionPool &pool) const 
{
	obj->load(theConnection) ;
	
	// now download the file in a temporary path
	string tmpName = File::tempname(_tmpDownloadPath, "xml") ; // PRQA S 3050
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading:" << obj->getFileName() << " as:" << tmpName );
	dbDownloadFile* dloadObj = dynamic_cast<dbDownloadFile*>(dbFileActionFactoryChain::instance()->newDownloadAction(pool,*obj,tmpName)); // PRQA S 3081
		     	
	if(!dloadObj)
	{
		ACS_THROW(exIllegalValueException("Dynamic_cast failure.")); // PRQA S 3081
	}
	dbFileActionScopeGuard guard(dloadObj);
			
	dloadObj->start() ; // start the downloading thread
	do {
		usleep(100000) ; // PRQA S 4400
	} while (dloadObj->downloading()) ;
	
	
	exception *te = 0;
	dloadObj->runException(te) ;
	
	if (te) {
		try {
			File::unlink(tmpName) ;
		}
		catch(exception &) {
			// don't care about unlink exceptions if any
		}
		ostringstream os ;
		os << "Cannot download " << obj->getFileName() << " version " << obj->getFileVersion() << " as " << tmpName ;
		ACS_THROW(DistributorCreator::DistributorException(*te,os.str())) ;
		delete te ;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Parsing:" << tmpName );
	rsResourceSet tmpDistFile ;
	try {
		XMLIstream xml(tmpName) ;

		xml >> tmpDistFile ;
	}
	catch(...) {
		// just unlink and propagate
		try {
			File::unlink(tmpName) ;
		}
		catch(exception &) {
			// don't care about unlink exceptions if any
		}
		throw ;
	}
	
	try {
		if (!simulation()) { File::unlink(tmpName) ; }
	}
	catch(exception &) {
		// don't care about unlink exceptions if any
	}

	int venusid = SatelliteName::instance("")->id("Venus",true); // PRQA S 3000, 4412

	try {
		if (venusid == obj->getSatelliteId()) {
			emitVenusDistribution(pool, tmpDistFile, tmpInfo, theConnection) ;
		}
        else {
			emitDistribution(tmpDistFile, tmpInfo, theConnection) ;
        }
		appendSummary(DistributionIdSuccesfullyCreated, tmpInfo.id);
	}
	catch(...) {
		appendSummary(TemporaryDistributionIdHasProblem,tmpInfo.id) ;
		throw ;
	}

}



void RedistributionDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate & stop_pred) const // PRQA S 4020
{

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050

    int algoid = DistributionAlgo::instance(theConnection.getKey())->id(_algo); // PRQA S 3000

	
	// now select all the records from t_temporarydistribution with algo = algoid
	// but correctly qualified
	string sqlCommand;
	// get the algoId from t_distributionalgo table
	string dbKey = theConnection.getKey();
	const string dialectName = SQLDialectFactory::dialectName(dbKey);
	
	if( dialectName == "Postgres8" || dialectName == "Oracle11" ) {
		sqlCommand ="SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END) " ;
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", CAST (' ' as varchar (5) ) , t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand += " FROM t_temporarydistribution  t WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
	}
	else if( dialectName == "Informix9") {
		sqlCommand ="SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp,\
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END) ";
            
#ifndef PDS_OLD_SCHEMA
        sqlCommand += ", \" \", t.policy, t.disseminationpackingalgo_id" ; 
#endif
		sqlCommand += " FROM t_temporarydistribution  t WHERE t.preselection = ";
		sqlCommand += db::toSQL(dbKey, true);      // PRQA S 3050, 3081
		sqlCommand += " AND t.algo = ";
		sqlCommand += db::toSQL(dbKey, algoid);    // PRQA S 3050, 3081
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
	}
	vector <TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand, "t_temporarydistribution preselected", TmpInfo, theConnection);

    size_t tmpSize = TmpInfo.size() ;

	for (size_t i=0; i < tmpSize; ++i) {
		
        if (stop_pred()) { return ; }

		try {
			ACS_LOG_INFO("Parsing temporary distribution id: " << TmpInfo[i].id );
			dbInventoryObject *theObj = 0;
			try {
				theObj = dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", theConnection.getKey())); // PRQA S 3081
				if (!theObj) {
					ACS_THROW(DistributorCreator::DistributorException("Cannot allocate a dbInventoryObject")) ; // PRQA S 3081
				}
				dbPersistentScopeGuard theObjScopeGuard(theObj) ;
				theObj->setFileName(TmpInfo[i].filename) ;
				theObj->setFileVersion(TmpInfo[i].fileversion) ;
 				ACS_LOG_INFO("Downloading Temporary distribution file: \"" << TmpInfo[i].filename << 
							 ' ' << TmpInfo[i].fileversion );
				downloadAndEmitDistribution(TmpInfo[i],theObj,theConnection, pool) ;
			}
			catch(exception &e) {
				ostringstream os ;
				os << "Cannot download file " << TmpInfo[i].filename << " version " << TmpInfo[i].fileversion ;
				ACS_THROW(DistributorCreator::DistributorException(e,os.str())) ;
			}			
		}
		catch(exception &e) { // notify now and continue with next if any.
			ACS_LOG_NOTIFY_EX(e) ; 
		}
	}	
}

_ACS_END_NAMESPACE
