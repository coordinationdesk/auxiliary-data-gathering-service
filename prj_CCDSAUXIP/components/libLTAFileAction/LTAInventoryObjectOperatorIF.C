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

	$Id$

	$Author$

	$Log$

*/


#include <LTAInventoryObjectOperatorIF.h>

#include <dbFileActionFactoryChain.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SQLString.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace { // unnamed 
    const db::LTAInventoryObject* toInv(const dbPersistent &p)
    {
	    const db::LTAInventoryObject* obj = dynamic_cast<const db::LTAInventoryObject*> (&p); // PRQA S 3081
	    if(!obj)
	    {
		    ACS_THROW(LTAInventoryObjectOperatorIF::LTAInventoryObjectOperatorException("dbPersistent is not a LTAInventoryObject")); // PRQA S 3081
	    }
        return obj ;
    }
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAInventoryObjectOperatorIF)

dbPersistentOperatorIF *LTAInventoryObjectOperatorIF::clone() const 
{
	return new LTAInventoryObjectOperatorIF(*this) ;
}



dbPersistent::IdType LTAInventoryObjectOperatorIF::getId(const dbPersistent &p) const 
{	
	return toInv(p)->getId();
}


const std::string LTAInventoryObjectOperatorIF::getFileName(const dbPersistent &p ) const // PRQA S 4020 
{
	return toInv(p)->getName( dbConnectionWrapper( db::ConnPools::instance()->getPool("") ) );
}

const std::string LTAInventoryObjectOperatorIF::getFileVersion(const dbPersistent & p ) const 
{
	return string("0001");
}

const std::string LTAInventoryObjectOperatorIF::getGroup( const dbPersistent &p, dbConnection &theConnection ) const 
{
	
	db::FileType fileType( theConnection );
	
	loadFileType(toInv(p), theConnection, fileType );

	std::string theGroup = fileType.getGroup(theConnection );

	// LTA-168: managing historical product mode to change last char of the group in "H"
	bool historicalMode (false);
	ConfigurationSingleton::instance()->get().getValue ("SRVInventory.historical", historicalMode, false);
	if (historicalMode && !theGroup.empty() )
	{
		// Replace last char with "H"
		theGroup.pop_back();
		theGroup.push_back('H');
		ACS_LOG_INFO ("Historical mode on. Changing group in " << theGroup);
	}

	return theGroup;
}

const std::string LTAInventoryObjectOperatorIF::getFileType( const dbPersistent & p ) const 
{
	dbConnectionWrapper pw(db::ConnPools::instance()->getPool("") ) ;
    dbConnection &theConnection = pw ; // PRQA S 3050

	db::FileType fileType( theConnection );
	
	loadFileType(toInv(p), theConnection, fileType );

	return fileType.getFileType(theConnection ) ;
}

void LTAInventoryObjectOperatorIF::loadFileType(const db::LTAInventoryObject* toInv, dbConnection &theConnection, db::FileType & fileType ) const 
{
	dbPersistent::IdType fileTypeId = toInv->getFileTypeId( theConnection );
	
	if( not fileType.load(theConnection , fileTypeId) ) {
		ostringstream exMsg ;
		exMsg << "Cannot upload t_filetypes record with id: " << fileTypeId ;
		ACS_THROW( LTAInventoryObjectOperatorException( exMsg.str() ) ); // PRQA S 3081
	}
}

bool LTAInventoryObjectOperatorIF::canGetFromSynchSM(const dbPersistent &p, dbConnection &conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	const string &dbKey = conn.getKey() ;

// SELECT COUNT(*) FROM (
// SELECT ism.id, p.retrievecost,  
// (select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
// sm.allowdefaultcost as adc  
// FROM t_srv_inventorysm ism 
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
        nestedSelect.addSelectList("allowdefaultcost", "adc"); 

		const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(p);
		ACS_COND_THROW( invSmTableName.empty(), LTAInventoryObjectOperatorException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );

		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "ism","StorageManId" , dbParam::EQUAL_TO , "t_storagemanager.id" )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager",pp) ;
		}
		
		{
			// get filetype for where condition
			const string filetype = getFileType( p ) ;
			ACS_COND_THROW( filetype.empty(), LTAInventoryObjectOperatorException( "Empty filetype for t_filetypes record linked to \""+getFileName(p)+ "\" inventory" ) ); // PRQA S 3081

   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "ism","StorageManId" , dbParam::EQUAL_TO , "t_storagepriority.id" )) ;
 			pp.addParameter(dbParam( dbKey, "t_storagepriority","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, filetype) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, getId(p)) )) ;
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
		pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
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

_ACS_END_NAMESPACE
