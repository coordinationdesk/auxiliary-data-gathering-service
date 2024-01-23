// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
*/


#include <dbLTAPutOnCacheInventoryFile.h>

#include <LTAInventoryObjectOperatorIF.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConnectionPool.h>
#include <dbFileActionFactoryChain.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>
#include <LTAStat.h>

_ACS_BEGIN_NAMESPACE(acs)

using acs::math::StatNode ;

namespace {
    StatNode &statN() { return lta::stat()["IsOnCache"] ; }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLTAPutOnCacheInventoryFile)

using acs::dbConnectionWrapper ;

// constructor
dbLTAPutOnCacheInventoryFile::dbLTAPutOnCacheInventoryFile(dbConnectionPool &pool, db::LTAInventoryObject &s, int storageid) :
	dbPutOnCacheFile(pool, s, storageid) ,  
	_inventory(s),
	_inventoryName( s.getName( dbConnectionWrapper(pool) ) ),
    _status(dbFileAction::NOT_AVAILABLE)
{
	statN()+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileName: " << _inventoryName)  ;
}

using std::string ; 


void dbLTAPutOnCacheInventoryFile::start() 
{
    try {
        statN()["started"] += 1 ;
        Timer timer ;
        timer.start() ;

	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start putting file \"" <<_inventoryName << "\" on cache") ;

	    // It returns synchronous SMs where file is, ordered by retrieve cost ASC
	    const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory);
	    SQLString sqlString = getSyncSMsForPutOnCacheQuery(getStorageId());
	    dbSet smset;
	    {
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050

		    dbQuery query(theConnection,sqlString, "FAGUPSEL prior, cost") ;
		    query.execForSet(smset) ;
        }
        
        
	    const size_t smsetsize = smset.size() ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file \"" << _inventoryName << "\" can be cached by " << smsetsize << " synchronous SMs.") ;

        // if no SM has been found and the storageid was not 0, 
        // the SM cannot retrieve for the specific file or doesn't know about it 
        // so the SM is not enabled to download the file.
        if ((0 == smsetsize) && (getStorageId() != 0)) {
            ACS_THROW(OnCacheException() << "Cannot put " << _inventoryName << " on cache for SM id: " << getStorageId()) ;
        }

	    for (size_t ri=0; (ri < smsetsize); ++ri) {

		    dbPersistent::IdType storageManId = 0 ;
		    try {

			    dbRow row(getPool().getKey()) ;
			    smset.getNextRow(row);

 			    unsigned short ci = 0;
			    row.column(ci++,storageManId);
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying smid: " << storageManId) ;

			    dbPersistent::IdType id = 0 ;
			    row.column(ci++,id);

			    string smtype;
			    row.column(ci++,smtype);

			    SMInterface* storage= SMInterfaceFactoryMethod::getInterface(smtype, storageManId, &_inventory, getPool()); // PRQA S 3000
			    SMInterfaceScopeGuard guard(storage);
			    if(!storage)
			    {	
				    ACS_LOG_WARNING("No SMInterface got for smid: " << storageManId << " type : " << smtype);
			    }
                else {
			        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got SMInterface for smid: " << storage->SMId()
					    << " type: " << storage->SMType()) ;

                    storage->setClientName(getClientName()) ; // set the client name for the current action
				    LTAInventoryObjectOperatorIF oper;

				    dbFileAction::CacheStatus ast = storage->putOnCache(id,oper);
                    if (_status < ast) { // if response has higher priority, store the highest
                        _status = ast ;
                    }
                    // the highest possible - useless to continue to ask to other SMs
                    if (_status == dbFileAction::ON_CACHE) { 
                        break ;
                    }
                }
		    }
		    catch (std::exception &ex)
		    {
                ACS_LOG_NOTIFY_EX(ex) ;
		    }

	    } // for (size_t i=0; i < smsetsize; ++i)

	    setCompleted(true) ;

	    timer.stop() ;

        statN()["success"] += 1 ;
        (statN()["elapsed [ms]"])+=double(timer.elapsed()); // PRQA S 3081
    }
    catch (std::exception &) {
        statN()["error"] += 1 ;
        
        throw ; 
    }
}


SQLString dbLTAPutOnCacheInventoryFile::getSyncSMsForPutOnCacheQuery(int storageid) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "storageid: " << storageid) ;

// SELECT StorageManId, id, type
// FROM (
//    SELECT ism.StorageManId, ism.id, sm.type, p.retrievecost,  
//    (select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
//    sm.allowdefaultcost as adc  
//    FROM t_inventorysm ism 
//    INNER JOIN t_storagemanager sm on sm.id = ism.StorageManId AND sm.syncdownload = 't'
//    LEFT OUTER JOIN t_storagepriority p on p.id=ism.StorageManId AND p.filetype = 'MSI_L1C_DS' 
//    WHERE ism.inv_id = 1356 
//    AND ism.StorageManId = storageid (only if storageid is not 0)
// ) AS inner_table 
// WHERE inner_table.policycount = 0 
// OR (retrievecost IS NOT NULL AND retrievecost >= 0) 
// OR (retrievecost IS NULL AND adc = 't')
// order by CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END ASC
    
    const string &dbKey = getPool().getKey() ; 
	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	sqlString.addSelectList("StorageManId");
	sqlString.addSelectList("id");
	sqlString.addSelectList("type");

	const string runTimeTable("inner_table");
	{
		SQLString nestedSelect( dbKey, SQLString::SELECT ) ;

		nestedSelect.addSelectList("ism.StorageManId");
		nestedSelect.addSelectList("ism.id");
		nestedSelect.addSelectList("sm.type");
		nestedSelect.addSelectList("p.retrievecost");

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
        nestedSelect.addSelectList("allowdefaultcost", "adc"); 

	    const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory);
		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "sm","syncdownload" , dbParam::EQUAL_TO, db::toSQL(dbKey, true) )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
		}
		{
			LTAInventoryObjectOperatorIF oper;
			string filetype = oper.getFileType(_inventory);
			ACS_COND_THROW( filetype.empty(), exIllegalValueException( "Filetype not defined for t_srv_inventory with name \""+_inventoryName+'\"') );
			
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "p","id" , dbParam::EQUAL_TO , "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "p","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, filetype) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority p",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId()) )) ;
			nestedSelect.addANDWhereParameter(pp) ;
		}
        // AND ism.StorageManId = storageid (only if storageid is not 0)
		if (0 != storageid) {
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"StorageManId" , dbParam::EQUAL_TO , db::toSQL(dbKey, storageid) )) ;
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
		pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
		sqlString.addORWhereParameter(pp) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlString.getSQL() );
	
	return sqlString;

}

_ACS_END_NAMESPACE
