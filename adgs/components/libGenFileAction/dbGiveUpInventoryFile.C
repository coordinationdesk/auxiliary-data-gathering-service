// PRQA S 1050 EOF
/*

	Copyright 2021-, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $	

*/


#include <dbGiveUpInventoryFile.h>
#include <dbFileActionFactoryChain.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <dbQuery.h>
#include <dbInventoryObject.h>
#include <dbInventoryObjectOperatorIF.h>
#include <SMInterface.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbSet.h>
#include <dbRow.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGiveUpInventoryFile)

// constructor
dbGiveUpInventoryFile::dbGiveUpInventoryFile(dbConnectionPool &pool, dbInventoryObject &s) :
	dbGiveUpFile(pool, s) ,  
    _inventory(s)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileName: " << _inventory.getFileName() 
		    << " FileVersion: " << _inventory.getFileVersion()) ;
}


dbGiveUpInventoryFile::~dbGiveUpInventoryFile() ACS_NOEXCEPT {}

using std::string ;

void dbGiveUpInventoryFile::start() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "clientName: " << getClientName()) ;
	
	ACS_LOG_PRIVINFO("Start giving up on file \"" <<_inventory.getFileName() << "\"") ;

    const string &dbKey = getPool().getKey() ;
	

    // Every connection is gathered from the pool and released ASAP

    { // Connectionwrapper scope
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050

	    ACS_COND_THROW( // PRQA S 3081
            ( (not _inventory.isFromDatabase()) and ( not _inventory.load(theConnection) ) ),
            PreconditionException("Error downloading file: inventory \"" + _inventory.getFileName() + "\" can't be found in database.")
        ) ;
    } // Connectionwrapper scope
	
	dbInventoryObjectOperatorIF oper;
	
	{ //the file could be on Storage manager(s)

		// be sure that the t_storagemanager table has the field allowdefaultcost
        bool hasADCfield (false);
        {
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050
            hasADCfield = dbSchemaMetaDataProvider::hasField ( "t_storagemanager", "allowdefaultcost", theConnection); // PRQA S 4412
        }

		// It returns synchronous SMs where file is, ordered by retrieve cost ASC
		SQLString sqlString = getSyncSMsForDownloadQuery(hasADCfield, dbKey);

		dbSet smset;
		{
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050

		    dbQuery query(theConnection,sqlString, "FAGUPSEL prior, cost") ;
		    query.execForSet(smset) ;
        }
			
		size_t smsetsize = smset.size() ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file \"" + _inventory.getFileName() + "\" can be givedup from " << smsetsize << " synchronous SMs.") ;

		for (size_t ri=0; ri < smsetsize; ++ri) {

			dbPersistent::IdType storageManId = 0 ;
			try {

				dbRow row(dbKey) ;
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
					ACS_LOG_WARNING("No SMInterface got for smid: " << storageManId << " type : " << smtype) ;
				}
                else {
				    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got SMInterface for smid: " << storage->SMId()
						    << " type: " << storage->SMType()) ;


                    storage->setClientName(getClientName()) ; // set the client name for the current action

				    storage->giveup(id,oper);
                }
			}
            catch (std::exception &ex)
			{
                ACS_LOG_NOTIFY_EX(ex) ;
			}

		} // for (size_t i=0; i < smsetsize; ++i)
	}
    
	ACS_LOG_PRIVINFO("Finished to givingup on file \"" <<_inventory.getFileName() << "\"") ;
	
	setCompleted(true) ;
}



SQLString dbGiveUpInventoryFile::getSyncSMsForDownloadQuery(bool hasADCfield, const std::string& dbKey) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

/* SELECT StorageManId, id, type
// FROM (
//    SELECT ism.StorageManId, ism.id, sm.type, 
             CASE WHEN p.parameters IS NULL THEN 1 ELSE 0 END as paramNull, 
             p.retrievecost,  
//           (select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
//           sm.allowdefaultcost as adc  
// FROM t_inventorysm ism 
//      INNER JOIN t_storagemanager sm on sm.id = ism.StorageManId AND sm.syncdownload = 't'
//      LEFT OUTER JOIN t_storagepriority p on p.id=ism.StorageManId AND p.filetype = 'MSI_L1C_DS' 
//      WHERE ism.inv_id = 1356 
// ) AS inner_table 
// WHERE inner_table.policycount = 0 
// OR (retrievecost IS NOT NULL AND retrievecost >= 0) 
// OR (retrievecost IS NULL AND adc = 't')
*/
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
		nestedSelect.addSelectList("CASE WHEN p.parameters IS NULL THEN 1 ELSE 0 END", "paramNull");
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
    	if (hasADCfield) {            
        	nestedSelect.addSelectList("allowdefaultcost", "adc"); 
    	}

        std::string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory) ;
	    ACS_COND_THROW( invSmTableName.empty(), exIllegalValueException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );

		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "sm","syncdownload" , dbParam::EQUAL_TO, db::toSQL(dbKey, true) )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
		}
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "p","id" , dbParam::EQUAL_TO , "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "p","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getFileType()) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority p",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId()) )) ;
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
	
	return sqlString;

}


_ACS_END_NAMESPACE
