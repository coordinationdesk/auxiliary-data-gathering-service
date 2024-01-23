// PRQA S 1050 EOF
/*
	Copyright 2020-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $

*/

#include <dbLTAEraseInventoryFile.h>

#include <LTASMTmp.h>
#include <LTAInventoryObjectOperatorIF.h>
#include <FileType.h>
#include <dbFileActionFactoryChain.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SMInterface.h>
#include <SMInterfaceScopeGuard.h>
#include <SMInterfaceFactoryMethod.h>
#include <dbConnectionPool.h>
#include <MCFilterables.h>
#include <File.h>
#include <LTAStat.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc ;


using acs::math::StatNode ;

namespace {
    StatNode &statN() { return lta::stat()["Erase"] ; }
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLTAEraseInventoryFile)

// constructor
dbLTAEraseInventoryFile::dbLTAEraseInventoryFile(dbConnectionPool &pool, db::LTAInventoryObject &s, int storageid) :
	dbEraseFile(pool, s,storageid) ,
	_inventory(s),
	_inventoryName( s.getName( dbConnectionWrapper(pool) ) )  // PRQA S 2528
{
	statN()+=1 ;
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

void dbLTAEraseInventoryFile::start() // PRQA S 4020
{
    resetRemainingSyncSM();
    
    try {
        statN()["started"] += 1 ;
        Timer timer ;
        timer.start() ;
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	    ACS_COND_THROW(isComplete(),dbPersistent::InvalidUse("File has been already erased.")) ; // PRQA S 3081 

	    {
		    ostringstream os;
		    os << "Starting to erase file \"" << _inventoryName << "\". ";
		    if(0 != _storageid) {
			    os << "On storage " << _storageid;
		    }
    	    else {
			    os << "On All storages" ;
    	    }
		    ACS_LOG_PRIVINFO(os.str());
	    }

        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
        const string &dbKey = theConnection.getKey() ;

	    //check if isFromDatabase.
	    if(!_inventory.isFromDatabase())
	    {
		    if(!_inventory.load(theConnection))
		    {
			    ACS_THROW(PreconditionException("Error erasing file \"" + _inventoryName + "\". Inventory object has not been saved in database.")) ; // PRQA S 3081
		    }
	    }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "preconditions passed.") ;

	    const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory);
	    ACS_COND_THROW( invSmTableName.empty(), EraseException("Inventory sm table name not valorized!") );

    //  SELECT  
    // 	ism.id, 
    // 	ism.StorageManId, 
    // 	sm."type", 
    // 	p.erasecost,  
    // 	(SELECT   (   COUNT  (  * ) )  FROM  t_storagepriority   WHERE  (( (t_storagepriority.id = ism.StorageManID) ))  )  AS policycount, 
    // 	sm.allowdefaultcost AS adc 
    // 	FROM  t_inventorysm ism  
    // 	LEFT OUTER JOIN  t_storagemanager sm ON  (( (sm.id = ism.StorageManId) )) 
    // 	LEFT OUTER JOIN  t_storagepriority p ON  (( (p.id = ism.StorageManId) ) AND (  (p.filetype = 'fileType_ECS_17') ))  
    // 	WHERE  (( (ism.inv_id = 3247) ))  
    // 	ORDER BY  p.erasecost DESC 

	    // subquery for the select count
	    SQLString subSql (dbKey, SQLString::SELECT);
	    {
    	    SQLString countStar(dbKey, SQLString::FUNCTION ) ;
    	    countStar.setFunctionName("COUNT");
    	    countStar.addFunctionParameter("*");
		    subSql.addSelectList (countStar);
	    }
	    subSql.addTableList ("t_storagepriority");

	    {
		    dbQueryParameters pp(dbKey) ;
		    pp.addParameter(dbParam(dbKey, "t_storagepriority", "id", dbParam::EQUAL_TO , "ism.StorageManID")) ;
		    subSql.addANDWhereParameter (pp);
	    }

	    SQLString sqlString (dbKey, SQLString::SELECT );

   	    sqlString.addSelectList( "ism.id" );	
   	    sqlString.addSelectList( "ism.StorageManId" );	
   	    sqlString.addSelectList( "sm.type" );	
   	    sqlString.addSelectList( "p.erasecost" );	
   	    sqlString.addSelectList( subSql, "policycount" );	// the select count subquery
	    sqlString.addSelectList( "sm.allowdefaultcost", "adc" );	

        sqlString.addTableList( invSmTableName, "ism" );

	    // Add Join on t_storagemanager
	    {
    	    dbQueryParameters pp(dbKey) ;
		    pp.addParameter(dbParam(dbKey, "sm", "id", dbParam::EQUAL_TO , "ism.StorageManId")) ;
    	    sqlString.addJoinList(SQLDialect::LEFT, " t_storagemanager", "sm", pp) ;
	    }

	    // Add Join on t_storagepriority
	    {
		    LTAInventoryObjectOperatorIF oper;
		    string filetype = oper.getFileType(_inventory);
		    ACS_COND_THROW( filetype.empty(), EraseException( "Filetype not defined for t_srv_inventory with name \""+_inventoryName+'\"') );

    	    dbQueryParameters pp(dbKey) ;
		    pp.addParameter(dbParam(dbKey, "p", "id", dbParam::EQUAL_TO , "ism.StorageManId")) ;
		    pp.addParameter(dbParam(dbKey, "p", "filetype", dbParam::EQUAL_TO , db::toSQL(dbKey, filetype) )) ;
    	    sqlString.addJoinList(SQLDialect::LEFT, " t_storagepriority", "p", pp) ;
	    }

	    // Where parameters
	    {
    	    dbQueryParameters pp(dbKey) ;
		    pp.addParameter(dbParam(dbKey, "ism", "inv_id", dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId()) )) ;
		    sqlString.addANDWhereParameter (pp);
	    }

	    if(_storageid != 0 )
	    {
    	    dbQueryParameters pp(dbKey) ;
		    pp.addParameter(dbParam(dbKey, "ism", "StorageManId", dbParam::EQUAL_TO , db::toSQL(dbKey, _storageid) )) ;
		    sqlString.addANDWhereParameter (pp);	
	    }

        sqlString.addOrderByList("p.erasecost", SQLString::DESC);

	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following query:\n" << sqlString.getSQL() );

	    dbQuery query(theConnection,sqlString, "FAERASEL SM") ;
	    dbSet smset;
	    query.execForSet(smset) ;

	    vector <LTASMTmp> SMList ;

	    int notAllowed = 0 ;
        const size_t smsetsize = smset.size() ;
	    for (size_t i=0; i < smsetsize; ++i) {
		    dbRow row(dbKey) ;
		    smset.getNextRow(row);

		    string smtype;
		    int storageManId = 0 ;
            int id = 0 ;
		    int cost = 0 ;
		    bool isNull = false ;
            bool adc = true; 
            unsigned int policycounter = 0;            

		    row.column(0,id);
		    row.column(1,storageManId);
		    row.column(2,smtype);   // PRQA S 4400 6
		    row.column(3,cost,&isNull);            
            row.column(4,policycounter);
            row.column(5,adc);

            // Algo is:
            // 1 - if no policies (policycounter = 0) allow the operation (cost set to 1)
            // 2 - if some policies
            //     2.1 - if cost is set use the cost
            //     2.2 - if cost is not set use +1 if ADC is true or -1 if ADC is false

            if (policycounter == 0)
            {
                cost = 1;
            }
            else
            {
                if (isNull)
                {
                    cost = ( adc ? 1 : -1 ); // allow default cost // PRQA S 3380
                }
            }

		    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, ( cost >=0 ? "Positive" : "Negative" ) << " cost for storage manager " << storageManId << " of \"" << smtype << "\" type" );
		    if (cost >=0) { // ignore negative cost
			    LTASMTmp sm(storageManId,smtype,id) ; // PRQA S 3000
			    sm.cost = cost ;
			    SMList.push_back(sm);
		    }
		    else {
			    notAllowed++ ;
            }
	    }

	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, SMList.size() << " SMs retrieved with positive cost" );

	    // There are the following cases:
	    // 1) there is no SM on which the file is stored
	    // 2) there are some SMs which do not allow the erasure

	    if(SMList.empty()) {

		    bool validityFlag = _inventory.getValidityFlag(theConnection);
		    if (notAllowed > 0) {
			    ; // nothing to do with SMs ... continue with standard erasure from DB
		    }
		    else if (validityFlag) {
			    ACS_LOG_WARNING("The file " << _inventoryName << " is not stored on any storage manager but its validity flag is true. Continuing erasure") ;
		    }
		    else if( not validityFlag ) 
		    {
			    setCompleted(true) ;
			    return;
		    }
            else {
			    // empty	
		    }
	    }

	    // note 1, if erasure is not completed the exception is thrown just after the following loop
	    // see note 2 
        const size_t smsize = SMList.size() ;
	    for(size_t i=0; i< smsize; ++i) {
		    try
		    {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMList[i].type 
                        << " Smid:" << SMList[i].id) ;
			    SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMList[i].type, SMList[i].id, &_inventory, getPool()); // PRQA S 3000
			    SMInterfaceScopeGuard guard(storage);
			    if(0 == storage)
			    {
				    ostringstream os ;
				    os << "No storage manager interface for SMid: " << SMList[i].id<< " type: " << SMList[i].type ;
				    ACS_THROW(EraseException(os.str())); // PRQA S 3081
			    }

                storage->setClientName(getClientName()) ; // set the client name for the current action

			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType() 
					    << " Smid:" << storage->SMId()) ;

			    try
			    {
				    storage->erase(SMList[i].sminvrecordid, _inventoryName, "") ;
			    }
			    catch (exception& ex)
			    {
				    ostringstream os;
				    os << "An error occured during the erase of invsm record with id #" << SMList[i].sminvrecordid << " on sm " << storage->SMId() << '\n';
				    // only synchronuos storage can throw an exception
				    ACS_COND_THROW( storage->isEraseSynchronous(), SyncEraseException( ex, os.str() ) );  
				    os << "Asynchronous SM no managed: no async action will be emitted.";
				    ACS_LOG_WARNING(os.str());
				    ACS_LOG_NOTIFY_EX(ex);
			    }

			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _inventoryName << "\" file Erased" ) ;

			    // APF-270 delete from _storageid
			    ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
							     mc_metric(Metric_DELETE) << 
							     mc_param("equipmentid", SMList[i].id) << 
							     MCCode ("ARC") << MCFunction ("Archive")) ;

		    }
		    catch(exception &ex)
		    {
			    ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
							     mc_param("equipmentid", SMList[i].id) <<
							     mc_param("message", string("Unable to remove from the storage manager: ") + ex.what()) <<
							     mc_metric(Metric_FAIL) << 
							     MCCode ("ARC") << MCFunction ("Archive")) ;		        

				if (dbSchemaMetaDataProvider::existTable("t_srv_sm_sync_missingactions", theConnection)) {
					try {
						// APF-335: handling of erase failure on synchronous SM
						db::Transaction transaction(theConnection) ;
						transaction.begin_work("handleErase") ;

						// add a record on the t_sm_pendingerase to trace the erase failure
						dbSmartPersistent therecordE ("t_srv_sm_sync_missingactions", theConnection);
						therecordE.set("sm_id", SMList[i].id, theConnection);
						therecordE.set("invsm_id", SMList[i].sminvrecordid, theConnection);
						string operation("ERASE");
						therecordE.set("operation", operation, theConnection);
						therecordE.save(theConnection);
						ACS_LOG_PRIVINFO("Added " << invSmTableName << " id# " << SMList[i].sminvrecordid << " in t_srv_sm_sync_missingactions");

						// Set to NULL inv_id on the record on the database
						dbSmartPersistent therecord (invSmTableName, theConnection);
						therecord.load( theConnection, SMList[i].sminvrecordid); 
						therecord.unset("inv_id",  theConnection);
						therecord.update(theConnection);
						ACS_LOG_PRIVINFO("Set to null inv_id for record id# " << SMList[i].sminvrecordid << " in " << invSmTableName);

						if (dbSchemaMetaDataProvider::existTable("t_srv_deleted_inventory", theConnection)) {
							dbPersistent::IdType ftypeId = _inventory.getFileTypeId(theConnection);
							db::FileType ftype(theConnection);
							ftype.load(theConnection, ftypeId);

							dbSmartPersistent delInvSp("t_srv_deleted_inventory", theConnection);
							delInvSp.set<std::string>("filename", _inventoryName, theConnection);
							delInvSp.set<std::string>("group", ftype.getGroup(theConnection), theConnection);
							delInvSp.set<DateTime>("creationdate", _inventory.getCreationDate(theConnection), theConnection);
							delInvSp.set<dbPersistent::IdType>("filetype_id", ftypeId, theConnection);
							delInvSp.set<dbPersistent::IdType>("action_id", therecordE.getId(), theConnection);
							delInvSp.save(theConnection);

							ACS_LOG_PRIVINFO("Added record id# " << delInvSp.getId() << " in t_srv_deleted_inventory");
						}

						transaction.commit() ;
					}
					catch(exception &x)
					{
						ACS_LOG_ERROR("An error occurred during updating of t_srv_sm_sync_missingactions and " << invSmTableName << ". No update performed.");
						ACS_LOG_NOTIFY_EX(x);
					}
				}

			    ACS_LOG_NOTIFY_EX(ex) ;
			    ACS_LOG_WARNING("Continuing erasure on next SM if any.") ;
		    }
	    } // for(size_t i=0; i< smsize; ++i)


        SQLString sqlStringSM (dbKey, SQLString::SELECT);
	    {
		    SQLString countFun (dbKey, SQLString::FUNCTION);
		    countFun.setFunctionName("COUNT") ;
    	    countFun.addFunctionParameter("*") ;
		    sqlStringSM.addSelectList( countFun ) ;
	    }

        sqlStringSM.addTableList(invSmTableName);

 	    {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, invSmTableName,"inv_id" ,  dbParam::EQUAL_TO ,  db::toSQL(dbKey, _inventory.getId()) )) ; // PRQA S 3050, 3081
            sqlStringSM.addANDWhereParameter(pp) ;
        }

	    int numberofcurrentStorage=0 ;
        try
        {
		    dbQuery querySM(theConnection,sqlStringSM, "FAERASEL "+invSmTableName) ;
            dbSet set ;
            querySM.execForSet(set) ; // no need to check tuples number: is a count
            dbRow row(dbKey);
            set.getNextRow(row) ; // first and only 
            row.column(0, numberofcurrentStorage) ;
   	    }
        catch (exception& e)
        {
            ACS_THROW(EraseException( e, "EraseInventoryFile: cannot query for inventory count" )) ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query returned the number of storage where \"" << _inventoryName << "\" file results: " << numberofcurrentStorage) ;
		setRemainingSyncSM(numberofcurrentStorage);
		
	    if(0 == numberofcurrentStorage) {

		    _inventory.setValidityFlag(false, theConnection) ;

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to update database.") ;
		    try
		    {
			    _inventory.update(theConnection);
		    }
		    catch(exception &ex)
		    {
			    ACS_THROW(EraseException(ex, "Error erasing file \"" + _inventoryName + "\". Error invalidating"));
		    }
		    ACS_LOG_INFO("Erased file \""  << _inventoryName << '\"') ;
	    }
	    else {
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << _inventoryName << "\" file not erased");
	    }

	    timer.stop() ;

        statN()["success"] += 1 ;
        (statN()["elapsed [ms]"])+=double(timer.elapsed()); // PRQA S 3081
    }
    catch (exception &) {
        statN()["error"] += 1 ;
        throw ;
    }
}

_ACS_END_NAMESPACE
