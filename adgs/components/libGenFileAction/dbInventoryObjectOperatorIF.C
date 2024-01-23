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

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 7.9  2017/11/17 10:45:38  marpas
	qa warnings fixed
	code best practice applied
	
	Revision 7.8  2016/12/05 15:42:32  francesco.avanzi
	tested new isOnAsync
	
	Revision 7.7  2016/10/18 09:03:11  lucio.pulvirenti
	S1PDGS-31826: insertDownloadRequest: t_smactions insert performed with SQLString to skip insertion if record already occurs on synch SM.
	 getRequestId private method added.
	
	Revision 7.6  2016/10/17 15:19:02  lucio.pulvirenti
	S1PDGS-31826: work in progress.
	
	Revision 7.5  2016/10/12 16:43:05  francesco.avanzi
	S2PDGS-1556: Work In Progress
	
	Revision 7.4  2016/04/29 16:03:17  marpas
	ids are of dbPersistent::IdType type - to be overall consistent
	coding best practices applied
	
	Revision 7.3  2016/04/11 13:29:59  marpas
	adopting new db interfaces
	
	Revision 7.2  2016/03/26 21:42:14  marpas
	using new db I/F
	
	Revision 7.1  2015/06/17 10:51:50  lucio.pulvirenti
	S2PDGS-1234: willBeRestoredOnSynchSM IF added to check if product can be restored on synchronous SM with no errors. Common code with
	restoreOnSynchSM method put into new private canBeRestoredOnSynchSM method.
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.10  2015/01/09 16:12:51  lucio.pulvirenti
	S2PDGS-1010: restoreOnSynchSM public IF added.
	
	Revision 6.9  2015/01/08 17:17:46  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.8  2014/12/18 18:21:08  lucio.pulvirenti
	S2PDGS-1010: work in progress.
	
	Revision 6.7  2014/12/17 10:12:15  lucio.pulvirenti
	S2PDGS-1010: canGetFromSynchSM IF added.
	
	Revision 6.6  2014/12/16 17:05:01  lucio.pulvirenti
	S2PDGS-1010: work in progress.
	
	Revision 6.5  2014/12/15 17:11:57  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.4  2014/12/11 12:20:23  marpas
	qa warnings
	
	Revision 6.3  2014/12/11 11:59:36  marpas
	follows interface changes on libDbData 5.14
	rewritten to improve readability
	
	Revision 6.2  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.1  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.1  2013/09/16 16:59:16  marpas
	getId() method implemented
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.6  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.5  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.4  2012/11/20 09:04:04  marpas
	define PDS_OLD_SCHEMA to use old schema features
	
	Revision 1.3  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.2  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	

	
	

*/


#include <dbInventoryObjectOperatorIF.h>

#include <dbFileActionFactoryChain.h>
#include <dbInventoryObject.h>
#include <dbFileType.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbSMactions.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SQLString.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

const int dbInventoryObjectOperatorIF::defaultSMErrorRecoveryPeriod = 60;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectOperatorIF)

// constructor
dbInventoryObjectOperatorIF::dbInventoryObjectOperatorIF() :
    dbPersistentOperatorIF()
{
}



dbInventoryObjectOperatorIF::~dbInventoryObjectOperatorIF() throw() {}


dbInventoryObjectOperatorIF::dbInventoryObjectOperatorIF(const dbInventoryObjectOperatorIF &rhs ) :
	dbPersistentOperatorIF(rhs)
{
}

dbInventoryObjectOperatorIF &dbInventoryObjectOperatorIF::operator=(const dbInventoryObjectOperatorIF &rhs )
{
	if (this != &rhs) {
		dbPersistentOperatorIF::operator=(rhs) ;
	}
	return *this ;
}

dbPersistentOperatorIF *dbInventoryObjectOperatorIF::clone() const 
{
	return new dbInventoryObjectOperatorIF(*this) ;
}


namespace { // unnamed 
    const dbInventoryObject*  toInv(const dbPersistent &p)
    {
	    const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	    if(!obj)
	    {
		    ACS_THROW(dbInventoryObjectOperatorIF::dbInventoryObjectOperatorException("dbPersistent is not an Inventory Object")); // PRQA S 3081
	    }
        return obj ;
    }
}


dbPersistent::IdType dbInventoryObjectOperatorIF::getId(const dbPersistent &p) const 
{	
	return toInv(p)->getId();
}


const std::string dbInventoryObjectOperatorIF::getFileName(const dbPersistent &p ) const // PRQA S 4020 
{
	return toInv(p)->getFileName();
}

const std::string dbInventoryObjectOperatorIF::getFileVersion(const dbPersistent & p ) const 
{
	return toInv(p)->getFileVersion();
}

const std::string dbInventoryObjectOperatorIF::getGroup(const dbPersistent &p, dbConnection &conn) const 
{
	const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	if(!obj)
	{
		ACS_THROW(dbInventoryObjectOperatorException("dynamic_cast fails")); // PRQA S 3081
	}
	
	dbFileType* filetype = dynamic_cast<dbFileType*> (dbPersistentFactoryChain::instance()->newObject("dbFileType", conn.getKey())); // PRQA S 3081, 4412
	dbPersistentScopeGuard guard2(filetype) ;
	
	if(!filetype)
	{
		ACS_THROW(dbInventoryObjectOperatorException("dbFileType dynamic_cast fails")); // PRQA S 3081
	}
	
	filetype->setFileType(obj->getFileType());
	filetype->load(conn);
	
	string ret =  filetype->getGroup();
	return ret ;
}

const std::string dbInventoryObjectOperatorIF::getFileType(const dbPersistent & p ) const 
{
	return toInv(p)->getFileType();
}



bool dbInventoryObjectOperatorIF::canGetFromSynchSM(const dbPersistent &p, dbConnection &conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	ACS_COND_THROW(!obj, dbInventoryObjectOperatorException("dynamic_cast fails")); // PRQA S 3081
	
	const string &dbKey = conn.getKey() ;

    // be sure that the t_storagemanager table has the field allowdefaultcost
    bool hasADCfield = dbSchemaMetaDataProvider::hasField ("t_storagemanager", "allowdefaultcost", conn); // PRQA S 4412
	
	const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(p);
	ACS_COND_THROW( invSmTableName.empty(), dbInventoryObjectOperatorException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );  // PRQA S 3081

// SELECT COUNT(*) FROM (
// SELECT ism.id, p.retrievecost,  
// (select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
// sm.allowdefaultcost as adc  
// FROM t_inventorysm ism 
// INNER JOIN t_storagemanager sm on sm.id = ism.StorageManId 
// LEFT OUTER JOIN t_storagepriority p on p.id=ism.StorageManId AND p.filetype = 'MSI_L1C_DS' 
// WHERE ism.inv_id = 1356 AND sm.syncdownload = 't
// ') AS inner_table 
// WHERE inner_table.policycount = 0 
// OR (retrievecost IS NOT NULL AND retrievecost >= 0) 
// OR (retrievecost IS NULL AND adc = 't');
		
	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	{
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("*");
		sqlString.addSelectList(tmp);
	}

	const string runTimeTable("inner_table");
	{
		SQLString nestedSelect( dbKey, SQLString::SELECT ) ;
		
		nestedSelect.addSelectList("ism.id");
		nestedSelect.addSelectList("retrievecost");
		
		SQLString innerSelect( dbKey, SQLString::SELECT ) ;
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("*");
		innerSelect.addSelectList(tmp);
		innerSelect.addTableList("t_storagepriority");
		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "t_storagepriority" ,"id" , dbParam::EQUAL_TO , "ism.StorageManID" )) ;
			innerSelect.addANDWhereParameter(pp) ;
		}
		
		nestedSelect.addSelectList(innerSelect, "policycount");
    	if (hasADCfield) {            
        	nestedSelect.addSelectList("allowdefaultcost", "adc"); 
    	}

		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "ism","StorageManId" , dbParam::EQUAL_TO , "t_storagemanager.id" )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager",pp) ;
		}
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "ism","StorageManId" , dbParam::EQUAL_TO , "t_storagepriority.id" )) ;
 			pp.addParameter(dbParam( dbKey, "t_storagepriority","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getFileType()) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getId()) )) ;
			pp.addParameter(dbParam(dbKey, "t_storagemanager" ,"syncdownload" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
			nestedSelect.addANDWhereParameter(pp) ;
		}
		
		sqlString.addTableList(nestedSelect.getSQL(true), runTimeTable );

 	}

	// Add Where
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"policycount" , dbParam::EQUAL_TO , db::toSQL(dbKey, 0) )) ;
		sqlString.addANDWhereParameter(pp) ;
	}
	
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNOTNULL )) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, 0) )) ;
		sqlString.addORWhereParameter(pp) ;
	}
	
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNULL )) ;
		if (hasADCfield ) {
			pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
		}
		sqlString.addORWhereParameter(pp) ;
	}
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlString.getSQL() );
	

	dbQuery query(conn, sqlString, "SM Sync") ;
	dbSet set ;
    query.execForSet(set) ; // no need to check tuple number is a count

    dbRow smRow(dbKey) ;   
    set.getNextRow(smRow) ; // first and only 
	int count = 0;
    smRow.column(0,count);
	
	return (count > 0) ;
	
}



pair<bool, dbPersistent::IdType> dbInventoryObjectOperatorIF::restoreOnSynchSM(const dbPersistent &p, dbConnection &conn) const
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	ACS_COND_THROW(!obj, dbInventoryObjectOperatorException("dynamic_cast fails")); // PRQA S 3081
	
	bool insertDownloadRequest = true;
	return canBeRestoredOnSynchSM (obj, insertDownloadRequest, conn);

}

pair<bool, dbPersistent::IdType> dbInventoryObjectOperatorIF::canBeRestoredOnSynchSM (const dbInventoryObject* obj, bool insertDownlRequest, dbConnection& conn) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ACS_COND_THROW(!obj, dbInventoryObjectOperatorException("Input pointer is null!")); // PRQA S 3081
	
	ACS_COND_THROW( not obj->getValidityFlag(), NotMigrableException("Validity flag of \""+obj->getFileName()+"\" file with version "+obj->getFileVersion()+" is false.") ); // PRQA S 3081

	// be sure that the t_storagemanager table has the field error_recovery_period
    bool hasErrorRecoveryPeriod = dbSchemaMetaDataProvider::hasField ("t_storagemanager", "error_recovery_period", conn); // PRQA S 4412

	// It returns SMs where file is, ordered by synchronous (as first) and by retrieve cost ASC
	SQLString sqlString = getSMsForDownloadQuery(obj, hasErrorRecoveryPeriod, conn);

	dbSet smset;

	try {
		dbQuery query(conn, sqlString, "SMs for download") ;
		query.execForSet(smset);
	}
	catch( exception &e )
	{
		ACS_THROW( dbInteractionException(e, "Error in query on SMs for download") );
	}

	ACS_COND_THROW( smset.empty(), // PRQA S 3081
			NotMigrableException('\"'+obj->getFileName()+"\" file with version "+obj->getFileVersion()+" occurs on no Storage Manager or its retrievecost is negative for each Storage Manager having the file.") ) ; // PRQA S 3081

	size_t smsetsize = smset.size() ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to probe SMs having \"" << obj->getFileName() << "\" file with version " << obj->getFileVersion() << ": " << smsetsize << " found" );
	
	dbPersistent::IdType requestId = 0;
	bool canBeRestored = false; // if false, it means that already occurs on synch SM, otherwise it means that can be restored and it can be possibly associated to a download request already in progress or just inserted. 

	dbPersistent::IdType theBestCandidateSMId = 0;
	dbPersistent::IdType inErrorCandidateSMId = 0;
	size_t tryAgainLaterSM = 0;
	bool syncdownload = false ;
	for (size_t ri=0; ri < smsetsize; ++ri) {
		
		dbRow row(conn.getKey()) ;
		smset.getNextRow(row);

		unsigned short c = 0;
		dbPersistent::IdType storageManId = 0 ;
		row.column(c++,storageManId);
		
		row.column(c++,syncdownload);
		if( syncdownload ) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << obj->getFileName() << "\" file already occurs on synchronous Storage Manager with id=" << storageManId );
			break;
		}
		
		bool isNull = false;
		dbPersistent::IdType actionId = 0 ;
		row.column(c++,actionId, &isNull);
		if( isNull ) {
			if (0 == theBestCandidateSMId) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No download action still performed on \"" << obj->getFileName() << "\" file by SM with id=" << storageManId << ": the best candidate" );
				theBestCandidateSMId = storageManId;
			}
			continue;
		}
		
		isNull = false;
		string status;
		row.column(c++,status, &isNull);
		if( isNull || ( "COMPLETED" != status && "ERROR" != status ) ) { // either not yet started or still in processing
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Download from SM with id " << storageManId << " of \"" << obj->getFileName() << "\" file " << ( status.empty() ? "not started yet" : ("still in progress: status is "+status) ) // PRQA S 3380, 3385
								<< "; t_smactions id=" << actionId );
			canBeRestored = true;
			requestId = actionId;
			break;
		}
		else if( ("COMPLETED" == status) && (0 == theBestCandidateSMId )) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Download from SM with id " << storageManId << " of \"" << obj->getFileName() << "\" file has been completed (t_smactions id=" << actionId << "): the best candidate" );
			theBestCandidateSMId = storageManId;
		}
		else if( ("ERROR" == status) && (theBestCandidateSMId == 0) && (inErrorCandidateSMId == 0 )) {
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Download from SM with id " << storageManId << " of \"" << obj->getFileName() << "\" file has been ended in ERROR: t_smactions id=" << actionId );

			DateTime stopTime;
			row.column(c++,stopTime, &isNull); // it should be valorized, if not stopTime = now()
			
			int errorRecoveryPeriod = defaultSMErrorRecoveryPeriod;
			if( hasErrorRecoveryPeriod ) {
				row.column(c++,errorRecoveryPeriod, &isNull);
			}

			if( errorRecoveryPeriod > 0 ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Download from SM with id " << storageManId << " of \"" << obj->getFileName() << "\" file has been set in ERROR on " << stopTime.getSqlString()
									<< " and error recovery period is " << errorRecoveryPeriod << " seconds" );
				if( (stopTime + static_cast<DateTime::JD50>(errorRecoveryPeriod/86400.0)) < DateTime() ) { // PRQA S 3011, 3050, 3081, 4400
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SM with id " << storageManId << " is a candidate to download \"" << obj->getFileName() << "\" file ");
					inErrorCandidateSMId = storageManId;
				}
				else {
					tryAgainLaterSM++;
				}
			}
		}
        else { /* nothing */ }

	} // for (size_t i=0; i < smsetsize; ++i)
	
	if( not syncdownload && not canBeRestored ) {
// if here no eligible/in processing download occurs for the file
		dbPersistent::IdType suitableSMId = 0;
		if( theBestCandidateSMId != 0 ) {
			suitableSMId = theBestCandidateSMId;
		}
		else if( inErrorCandidateSMId != 0 ) {
			suitableSMId = inErrorCandidateSMId;
		}
		else {
			
			ACS_COND_THROW( tryAgainLaterSM > 0, TryAgainLaterToMigrateException("Migration of \""+obj->getFileName()+"\" file on synchronous SM failed but it will be resumed later") ); // PRQA S 3081
						
			ACS_THROW(MigrationFailedException("Migration of \""+obj->getFileName()+"\" file failed by all asynchronous SMs where the file occurs and it cannot be restored") ); // PRQA S 3081
		}
		
		if( insertDownlRequest ) {	
		// insert record into queue.
			requestId = insertDownloadRequest(obj, suitableSMId, conn);
		}
		canBeRestored = true;

	}
	
	return make_pair(canBeRestored, requestId);
	
	
}


SQLString dbInventoryObjectOperatorIF::getSMsForDownloadQuery(const dbInventoryObject* obj, bool hasErrorRecoveryPeriod, dbConnection& conn) const
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	const string &dbKey = conn.getKey() ;
	
   // be sure that the t_storagemanager table has the field allowdefaultcost
    bool hasADCfield = dbSchemaMetaDataProvider::hasField ("t_storagemanager", "allowdefaultcost", conn); // PRQA S 4412

	const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(*obj);
	ACS_COND_THROW( invSmTableName.empty(), dbInventoryObjectOperatorException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) ); // PRQA S 3081

// SELECT StorageManId, 
// syncdownload, 
// actionId, 
// status, stopdate--, error_recovery_period
// FROM (
// 
// 	SELECT ism.StorageManId, p.retrievecost, sm.syncdownload ,-- sm.error_recovery_period, 
// 	(select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
// 	sm.allowdefaultcost as adc,
// 	act.id AS actionId,
// 	q.status, 
// 	q.stopdate
// 	FROM t_inventorysm ism 
// 	INNER JOIN t_storagemanager sm on sm.id = ism.StorageManId 
// 	LEFT OUTER JOIN t_storagepriority p on p.id=ism.StorageManId AND p.filetype = 'MSI_L1C_DS' 
// 	LEFT OUTER JOIN (
// 		SELECT ism.id AS ismId, 
// 		MAX(act.id) AS maxId
// 		FROM t_inventorysm ism 
// 		LEFT OUTER JOIN t_smactions act ON act.inv_id = ism.inv_id AND act.smid = ism.StorageManId AND act.operation = 'DOWNLOAD'
// 		WHERE ism.inv_id = 1356
// 		group by ism.id
// 	) AS tt ON tt.ismId = ism.id
// 	LEFT OUTER JOIN t_smactions act ON act.id = tt.maxId
// 	LEFT OUTER JOIN t_smqueue q ON q.packageid = act.packageid
// 	WHERE ism.inv_id = 1356
// 
// ) AS inner_table
// WHERE inner_table.policycount = 0 
// OR (retrievecost IS NOT NULL AND retrievecost >= 0) 
// OR (retrievecost IS NULL AND adc = 't')
// order by syncdownload DESC, CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END ASC



	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	sqlString.addSelectList("StorageManId");
	sqlString.addSelectList("syncdownload");
	sqlString.addSelectList("actionId");
	sqlString.addSelectList("status");
	sqlString.addSelectList("stopdate");
	if( hasErrorRecoveryPeriod ) {
		sqlString.addSelectList("error_recovery_period");
	}
	
	const string runTimeTable("t1");
	{
		SQLString nestedSelect( dbKey, SQLString::SELECT ) ;
		
		nestedSelect.addSelectList("ism.StorageManId");
		nestedSelect.addSelectList("p.retrievecost");
		nestedSelect.addSelectList("sm.syncdownload");
		if( hasErrorRecoveryPeriod ) {
			nestedSelect.addSelectList("sm.error_recovery_period");
		}
		{
			SQLString innerSelect( dbKey, SQLString::SELECT ) ;
			SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        	tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("*");
			innerSelect.addSelectList(tmp);
			innerSelect.addTableList("t_storagepriority");
			// Add Where
			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter(dbParam(dbKey, "t_storagepriority" ,"id" , dbParam::EQUAL_TO , "ism.StorageManID" )) ;
				innerSelect.addANDWhereParameter(pp) ;
			}
			nestedSelect.addSelectList(innerSelect, "policycount");
		}
    	if (hasADCfield) {            
        	nestedSelect.addSelectList("sm.allowdefaultcost", "adc"); 
    	}
		nestedSelect.addSelectList("act.id", "actionId");
		nestedSelect.addSelectList("q.status");
		nestedSelect.addSelectList("q.stopdate");

		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "ism.StorageManId")) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
		}
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "p","id" , dbParam::EQUAL_TO , "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "p","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getFileType()) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority p",pp) ;
		}
		
		const string joinTable("t2");
		{
			SQLString innerSelect( dbKey, SQLString::SELECT ) ;
			innerSelect.addSelectList("ism.id", "ismId");
			SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        	tmp.setFunctionName("MAX"); tmp.addFunctionParameter("act.id");
			innerSelect.addSelectList(tmp, "maxId");
			innerSelect.addTableList(invSmTableName, "ism");
			
			{
   				dbQueryParameters pp( dbKey);
 				pp.addParameter(dbParam( dbKey, "act","inv_id" , dbParam::EQUAL_TO , "ism.inv_id" )) ;
  				pp.addParameter(dbParam( dbKey, "act", "smid", dbParam::EQUAL_TO , "ism.StorageManId"  )) ;
				pp.addParameter(dbParam( dbKey, "act","operation" , dbParam::EQUAL_TO , db::toSQL(dbKey, "DOWNLOAD") )) ;
				innerSelect.addJoinList(SQLDialect::LEFT,"t_smactions act",pp) ;
			}

			// Add Where
			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getId()) )) ;
				innerSelect.addANDWhereParameter(pp) ;
			}
			
			innerSelect.addGroupByList("ism.id");
			
			// Join with outer select
			{
   				dbQueryParameters pp( dbKey);
 				pp.addParameter(dbParam( dbKey, joinTable, "ismId" , dbParam::EQUAL_TO , "ism.id" )) ;
				nestedSelect.addJoinList(SQLDialect::LEFT, innerSelect.getSQL(true)+" "+joinTable, pp) ;
			}
		}
		
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, joinTable, "maxId" , dbParam::EQUAL_TO , "act.id" )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_smactions act",pp) ;
		}
		
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "q", "packageid" , dbParam::EQUAL_TO , "act.packageid" )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_smqueue q",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getId()) )) ;
			nestedSelect.addANDWhereParameter(pp) ;
		}
		
		sqlString.addTableList(nestedSelect.getSQL(true), runTimeTable );

 	}

	// Add Where
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"policycount" , dbParam::EQUAL_TO , db::toSQL(dbKey, 0) )) ;
		sqlString.addANDWhereParameter(pp) ;
	}
	
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNOTNULL )) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, 0) )) ;
		sqlString.addORWhereParameter(pp) ;
	}
	
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNULL )) ;
		if (hasADCfield ) {
			pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
		}
		sqlString.addORWhereParameter(pp) ;
	}
	
	sqlString.addOrderByList("syncdownload", SQLString::DESC) ;
	sqlString.addOrderByList("CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END", SQLString::ASC) ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlString.getSQL() );
  
	return sqlString;
}


dbPersistent::IdType dbInventoryObjectOperatorIF::insertDownloadRequest(const dbInventoryObject* obj, dbPersistent::IdType smId, dbConnection& conn) const // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
// 	INSERT INTO  t_smactions  (smid, filename, fileversion, operation, inv_id, unique_id)   (
// 		SELECT  2, 'AB_SNM_20140203120000_99999999999999_0001.xml', '0002', 'DOWNLOAD', 3741, 'AB_SNM_20140203120000_99999999999999_0001.xml0002' FROM now()  
// 		WHERE NOT EXISTS (
// 			SELECT  * FROM  t_inventorysm invsm  
// 			INNER JOIN t_storagemanager sm ON  (( (sm.id = invsm.storagemanid) ))  
// 			WHERE  (( (invsm.inv_id = 3741) ) AND (  (sm.syncdownload = 't') ))  
// 		)   
// 	)
	
	const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(*obj);
	ACS_COND_THROW( invSmTableName.empty(), dbInventoryObjectOperatorException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) ); // PRQA S 3081
		
	const string &dbKey = conn.getKey() ;
	SQLString sqlInsert(dbKey, SQLString::INSERT ) ;

	sqlInsert.addTableList("t_smactions");

	vector <string> columns ;
	columns.push_back("smid") ; 
	columns.push_back("filename") ; 
	columns.push_back("fileversion") ; 
	columns.push_back("operation") ; 
	columns.push_back("inv_id") ; 
	columns.push_back("unique_id") ; 

	{
		SQLString sqlSelect(dbKey, SQLString::SELECT ) ;

		sqlSelect.addSelectList( db::toSQL(dbKey, smId ) ) ;
		sqlSelect.addSelectList( db::toSQL(dbKey, obj->getFileName() ) ) ;
		sqlSelect.addSelectList( db::toSQL(dbKey, obj->getFileVersion() ) ) ;
		sqlSelect.addSelectList( db::toSQL(dbKey, "DOWNLOAD") ) ;
		sqlSelect.addSelectList( db::toSQL(dbKey, obj->getId() ) ) ;
		sqlSelect.addSelectList( db::toSQL(dbKey, obj->getUniqueId() ) ) ;


		{
			SQLString nestedSelect(dbKey, SQLString::SELECT ) ;
        	// Add Table list
 			nestedSelect.addTableList(invSmTableName, "invsm");
			nestedSelect.addSelectList("*") ;

			{
   				dbQueryParameters pp( dbKey);
 				pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "invsm.storagemanid")) ;
				nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
			}

			{
    			dbQueryParameters pp(dbKey) ;
				pp.addParameter(dbParam(dbKey, "invsm","inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, obj->getId()) )) ;
				pp.addParameter(dbParam(dbKey, "sm","syncdownload" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
				nestedSelect.addANDWhereParameter(pp) ;
  			}
			sqlSelect.addANDWhereParameter("NOT EXISTS" + nestedSelect.getSQL(true)) ;
		}

		sqlInsert.setValuesList(columns, sqlSelect);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlInsert.getSQL() );

	
	dbQuery dbInsert(conn, sqlInsert, "INS t_smactions") ;
	bool statementExecuted = false;
	dbPersistent::IdType requestId = 0;
	try {
		statementExecuted = dbInsert.execForStatus();
	}
	catch( exception &e ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Cannot insert t_smactions record");
		requestId = getRequestId(smId, obj->getId(), false, conn);
		ACS_COND_THROW( requestId == 0, dbInteractionException(e, "Error is not a problem of unique constraint on t_smactions") );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_smactions record for \"" << obj->getFileName() << "\" filename, (inv id=" << obj->getId() << ") already occurring with id=" << requestId );
	}
	
	if( statementExecuted ) {
		
		size_t insertedRows = dbInsert.getAffectedRows();
		ACS_COND_THROW( insertedRows > 1, dbInteractionException("UNEXPECTED: more than one t_smactions record inserted!")); // PRQA S 3081 
		bool occurringOnSynchSM = false;
		if( insertedRows == 0 ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "No t_smactions record inserted: already occurring on synchronous SM");
			occurringOnSynchSM = true;
		}
		requestId = getRequestId(smId, obj->getId(), occurringOnSynchSM, conn);
		if( requestId == 0 ) {
			ostringstream exMsg;
			exMsg << "UNEXPECTED: t_smactions record related to inventory id=" << obj->getId() << " neither just inserted, nor already existing!";
			ACS_THROW( dbInteractionException(exMsg.str()) ); // PRQA S 3081
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "t_smactions record for \"" << obj->getFileName() << "\" filename (inv id=" << obj->getId() << ") " // PRQA S 3380
					<< ( occurringOnSynchSM ? "already occurring" : "just inserted" ) << " with id=" << requestId );

	}
	else {
		ACS_COND_THROW( requestId == 0, dbInteractionException("UNEXPECTED: netiher t_smactions record inserted nor exception got!") ); // PRQA S 3081
	}
	
	return requestId;
}

bool dbInventoryObjectOperatorIF::willBeRestoredOnSynchSM (const dbPersistent& p, dbConnection& conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	ACS_COND_THROW(!obj, dbInventoryObjectOperatorException("dynamic_cast fails")); // PRQA S 3081
	
	bool insertDownloadRequest = false;
	pair<bool, dbPersistent::IdType> req = canBeRestoredOnSynchSM (obj, insertDownloadRequest, conn);
	return req.first;
	
}
bool dbInventoryObjectOperatorIF::isOnAsyncSM(const dbPersistent&p, dbConnection&conn) const  //still to be named
{
    const string &dbKey = conn.getKey() ;
    const dbInventoryObject* obj = dynamic_cast<const dbInventoryObject*> (&p); // PRQA S 3081
	ACS_COND_THROW(!obj, dbInventoryObjectOperatorException("dynamic_cast fails")); // PRQA S 3081
	
	const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(p);
	ACS_COND_THROW( invSmTableName.empty(), dbInventoryObjectOperatorException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );  // PRQA S 3081
	
	
//  SELECT count syncdownload 
//  FROM t_inventorysm INNER JOIN t_storagemanager ON t_storagemanager.id =t_inventorysm.storagemanid 
//  WHERE inv_id = ( p.id() ) AND syncdownload = false
    SQLString select( dbKey, SQLString::SELECT ) ;
    select.addTableList( invSmTableName ) ;
	{
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("syncdownload");
		select.addSelectList(tmp);
	}
    {
        dbQueryParameters pp( dbKey);
 		pp.addParameter( dbParam( dbKey, invSmTableName,"storagemanid" , dbParam::EQUAL_TO, "t_storagemanager.id")) ;
		select.addJoinList(SQLDialect::INNER,"t_storagemanager",pp) ;
	}
    {
        dbQueryParameters pp( dbKey);
 		pp.addParameter( dbParam( dbKey, invSmTableName,"inv_id" , dbParam::EQUAL_TO, db::toSQL(dbKey, obj->getId() ))) ;
		select.addANDWhereParameter(pp) ;
	}
    {
        dbQueryParameters pp( dbKey);
 		pp.addParameter( dbParam( dbKey, "t_storagemanager","syncdownload" , dbParam::EQUAL_TO, db::toSQL(dbKey,false ))) ;
		select.addANDWhereParameter(pp) ;
	}
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUERY: " << select.getSQL())
    dbQuery query(conn, select, "COUNT syncdownload") ;
    dbSet set ;
    query.execForSet(set) ;
    dbRow row(dbKey) ;
    set.getNextRow(row) ;
    long num = 0 ;
    row.column(0,num) ;
    return 0 != num ; //still to be implemented: this function must return whether the file has ever been on LTA or NOT
}


dbPersistent::IdType dbInventoryObjectOperatorIF::getRequestId(dbPersistent::IdType smId, dbPersistent::IdType invId, bool occurringOnSynchSM, dbConnection& conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	const string &dbKey = conn.getKey() ;

	SQLString selectString(dbKey, SQLString::SELECT) ;
	string table = "T_SMactions" ;
    selectString.addTableList( table ) ;
	selectString.addSelectList("id");


	{
    	dbQueryParameters storageparameters(dbKey) ; 
    	storageparameters.addParameter(dbParam(dbKey, table,"smid", dbParam::EQUAL_TO, db::toSQL(dbKey, smId))) ; // PRQA S 3050, 3081
    	selectString.addANDWhereParameter(storageparameters) ;
	}

	{
    	dbQueryParameters inventoryIdparam(dbKey) ; 
		inventoryIdparam.addParameter(dbParam(dbKey, table,"inv_id", dbParam::EQUAL_TO, db::toSQL(dbKey, invId))) ; // PRQA S 3050, 3081
		selectString.addANDWhereParameter(inventoryIdparam) ;
	}

	{
    	dbQueryParameters operparameters(dbKey) ; 
    	operparameters.addParameter(dbParam(dbKey, table,"operation", dbParam::EQUAL_TO, db::toSQL(dbKey, "DOWNLOAD"))) ; // PRQA S 3050, 3081
    	selectString.addANDWhereParameter(operparameters) ;
	}

	if(occurringOnSynchSM) {
		selectString.addOrderByList("id", SQLString::DESC) ;
	}
	else {
		DateTime currdate;
    	dbQueryParameters endparameters(dbKey) ; 
    	endparameters.addParameter(dbParam(dbKey, table,"enddate", dbParam::MAJOR_OF,db::toSQL(dbKey, currdate,dbConv::YearToFractionExtended))) ; // PRQA S 3050, 3081
    	selectString.addANDWhereParameter(endparameters) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << selectString.getSQL() );

	dbQuery selectquery(conn,selectString, "SMactions id") ;
	
	dbSet set;
	selectquery.execForSet(set);
	dbPersistent::IdType requestId = 0;
	if( not set.empty() ) {
		dbRow selectrow(dbKey) ;
		set.getNextRow(selectrow);
		selectrow.column(0, requestId);
	}
	return requestId;
}

_ACS_END_NAMESPACE
