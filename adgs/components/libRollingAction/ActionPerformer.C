// PRQA S 1050 EOF
/*
	Copyright 1995-2022, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SpA

	$Prod: libRollingAction$

	$Id$

	$Author$

	$Log$
	Revision 2.10  2016/07/26 16:39:59  nicvac
	CSGACQ-86: Oracle porting.
	
	Revision 2.9  2016/05/10 20:34:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.8  2016/04/11 13:30:15  marpas
	adopting new db interfaces
	
	Revision 2.7  2016/03/26 21:41:11  marpas
	using new db I/F
	
	Revision 2.6  2016/01/21 17:05:31  nicvac
	S2PDGS-1394: Report configurable on item id
	
	Revision 2.5  2015/10/28 13:41:01  damdec
	PDSEV-16: handling of blob in t_inventory removed.
	
	Revision 2.4  2014/12/02 14:49:07  nicvac
	S2PDGS-897
	
	Revision 2.3  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/11/26 17:14:50  nicvac
	S2PDGS-897
	
	Revision 2.1  2014/11/26 11:02:52  nicvac
	Refactoring
	
	Revision 2.1  2014/11/25 18:35:33  nicvac
	Refactoring
	
        
*/ 

#include <ActionPerformer.h>

#include <RollingPoliciesActions.h>
#include <RollingAction.h>

#include <dbPersistentScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbEraseFile.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbConnectionPool.h>
#include <dbSchemaMetaDataProvider.h>
#include <Transaction.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ActionPerformer)



ActionPerformer::ActionPerformer() : 
    InvPolicyPerformer() 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

ActionPerformer::~ActionPerformer() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}


bool ActionPerformer::invalidateItem( 	dbConnection &conn, // PRQA S 4214
										dbPersistent::IdType const& invId, dbPersistent::IdType const& storageId,
										string& uniqueId, RollingReport::InfoRecord& infoRecord, bool allowTempOrderRemoval) const {

	bool invalidated(false);

	dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), conn.getKey())); // PRQA S 3081
	dbPersistentScopeGuard scopeG(invObject);
	ACS_COND_THROW( !invObject, // PRQA S 3081
			exCriticalException("Cannot create objects of type dbInventoryObject"));

	invObject->setId( invId );

	invObject->load(conn);

	uniqueId = invObject->getUniqueId();

	infoRecord[ RollingReport::rFilename ] = invObject->getFileName();
	infoRecord[ RollingReport::rFileversion ] = invObject->getFileVersion();

	if(invObject->getValidityFlag()) {
	    	dbConnectionPool &pool = db::ConnPools::instance()->getPool(conn.getKey()) ;
		dbEraseFile* eraser = dynamic_cast<dbEraseFile*> (dbFileActionFactoryChain::instance()->newEraseAction(pool, *invObject, storageId )); // PRQA S 3081
		ACS_COND_THROW( !eraser, // PRQA S 3081
				exIllegalValueException("Dynamic_cast failure on dbEraseFile or Can't get instance of the eraser"));

		dbFileActionScopeGuard guard(eraser);
		// Start erasing product
		eraser->start();

		// conf value is reloaded for each item because the entire configuration is reloaded periodically
		bool purgeTempOrdersOnInvalidate = false;
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		conf.getValue("RollingArchive.PurgeTempOrdersOnInvalidate", purgeTempOrdersOnInvalidate, false );
		db::Transaction transaction(conn ) ;
	    	transaction.begin_work("") ;

		if (allowTempOrderRemoval && purgeTempOrdersOnInvalidate && dbSchemaMetaDataProvider::existTable("t_temporaryorders", conn))
		{
			// remove the t_temporary_orders row corresponding to invObject->getFileName()/invObject->getFileVersion()
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing t_temporaryorders row for \"" << invObject->getFileName() << "\", \"" << invObject->getFileVersion() << "\"") ;
			ostringstream ossSqlTime; 
			ossSqlTime<< "Delete from t_temporaryorders where (filename = \'" << invObject->getFileName() << "\' and fileversion = \'" << invObject->getFileVersion() << "\')";
			SQLString sqlString(conn.getKey(), ossSqlTime.str() );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");
			dbQuery sqlDelete( conn, sqlString, "DeleteTempOrdersOnInvalidate" );

			try
			{
				sqlDelete.execForStatus();
			}
			catch(exException & e)
			{
				ACS_LOG_WARNING("Error deleting t_temporary_orders row for \"" << invObject->getFileName() << "\", \"" << invObject->getFileVersion() << "\"");
				ACS_LOG_NOTIFY_EX(e);
			}
			
		}

		// Invalidate record
		invObject->setValidityFlag(false);
		invObject->update(conn);
		
		transaction.commit() ;
		
		invalidated = true;

	}

	return invalidated;

}

bool ActionPerformer::removeItem( 	dbConnection &conn, // PRQA S 4214
									dbPersistent::IdType const& invId,
									string& uniqueId, RollingReport::InfoRecord& infoRecord) const {

	dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), conn.getKey()));  // PRQA S 3081
	dbPersistentScopeGuard scopeG(invObject) ;
	ACS_COND_THROW( !invObject, // PRQA S 3081
			exCriticalException("Cannot create objects of type dbInventoryObject"));

	invObject->setId( invId );
	invObject->load(conn);

	uniqueId = invObject->getUniqueId();
	infoRecord[ RollingReport::rFilename ] = invObject->getFileName();
	infoRecord[ RollingReport::rFileversion ] = invObject->getFileVersion();

    dbConnectionPool &pool = db::ConnPools::instance()->getPool(conn.getKey()) ;
	dbEraseFile* eraser = dynamic_cast<dbEraseFile*> (dbFileActionFactoryChain::instance()->newEraseAction(pool,*invObject)); // PRQA S 3081
	ACS_COND_THROW( !eraser, // PRQA S 3081
			exIllegalValueException("Dynamic_cast failure on dbEraseFile or Can't get instance of the eraser"));

	dbFileActionScopeGuard guard(eraser);
	// Start removing from storage
	eraser->start();

	// Start removing from database
	invObject->remove(conn);

	return true;

}

bool ActionPerformer::cleanItem(dbConnection & conn,
				dbPersistent::IdType const& invId,
				dbPersistent::IdType const& storageId,
				map<dbPersistent::IdType, bool>& map_smId_sync,
				string& uniqueId, RollingReport::InfoRecord& infoRecord) const {

	bool cleanPerformed(false);

	this->getListOfSM( conn, invId, map_smId_sync );

	bool theStorageRetainsItem = ( map_smId_sync.count( storageId ) > 0 );

	if ( theStorageRetainsItem && map_smId_sync.size() >= 2 ) {

		dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), conn.getKey())); // PRQA S 3081
		dbPersistentScopeGuard scopeG(invObject);
		ACS_COND_THROW( !invObject, // PRQA S 3081
				exCriticalException("Cannot create objects of type dbInventoryObject"));

		invObject->setId( invId );
		invObject->load(conn);

		uniqueId = invObject->getUniqueId();
		infoRecord[ RollingReport::rFilename ] = invObject->getFileName();
		infoRecord[ RollingReport::rFileversion ] = invObject->getFileVersion();

		//Erase from storage
		{
		    dbConnectionPool &pool = db::ConnPools::instance()->getPool(conn.getKey()) ;
			dbEraseFile* eraser = dynamic_cast<dbEraseFile*> (dbFileActionFactoryChain::instance()->newEraseAction(pool, *invObject, storageId )); // PRQA S 3081
			ACS_COND_THROW( !eraser, // PRQA S 3081
					exIllegalValueException("Dynamic_cast failure on dbEraseFile or Can't get instance of the eraser"));

			dbFileActionScopeGuard guard(eraser);
			eraser->start();
		}

		map_smId_sync.erase( storageId );

		cleanPerformed = true;

	} else {

		cleanPerformed = false;
	}

	return cleanPerformed;

}



void ActionPerformer::invEntrySetValid( // PRQA S 4214
		dbConnection &conn, dbPersistent::IdType const& invId, bool valid,
		string& uniqueId, RollingReport::InfoRecord& infoRecord) const {

	dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), conn.getKey())); // PRQA S 3081
	dbPersistentScopeGuard scopeG(invObject);
	ACS_COND_THROW( !invObject, // PRQA S 3081
			exCriticalException("Cannot create objects of type dbInventoryObject"));

	invObject->setId( invId );

	invObject->load(conn);

	uniqueId = invObject->getUniqueId();
	infoRecord[ RollingReport::rFilename ] = invObject->getFileName();
	infoRecord[ RollingReport::rFileversion ] = invObject->getFileVersion();

	if ( invObject->getValidityFlag() != valid ) {
		invObject->setValidityFlag( valid );
		invObject->update(conn);
	}

}

void ActionPerformer::invEntryRemove( // PRQA S 4214
		dbConnection &conn, dbPersistent::IdType const& invId,
		string& uniqueId, RollingReport::InfoRecord& infoRecord) const {

	dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), conn.getKey())); // PRQA S 3081
	dbPersistentScopeGuard scopeG(invObject);
	ACS_COND_THROW( !invObject, // PRQA S 3081
			exCriticalException("Cannot create objects of type dbInventoryObject"));

	invObject->setId( invId );

	invObject->load(conn);

	uniqueId = invObject->getUniqueId();
	infoRecord[ RollingReport::rFilename ] = invObject->getFileName();
	infoRecord[ RollingReport::rFileversion ] = invObject->getFileVersion();

	invObject->remove(conn);

}


bool ActionPerformer::isOnAsynchSM( dbConnection &conn, dbPersistent::IdType const& invId,
		vector<dbPersistent::IdType>& asyncSMids) const {

	asyncSMids.clear();

	//Load Inventory ids of my children
    const string &dbKey = conn.getKey() ;
	ostringstream ossSql;
	ossSql<<
		"select sm.id \
		from t_storagemanager sm, t_inventorysm invsm \
		where \
		invsm.storagemanid = sm.id and \
		invsm.inv_id = "<<db::toSQL(dbKey, invId)<<" and \
		(sm.syncupload = "<<db::toSQL(dbKey, false)<<" or sm.syncdownload = "<<db::toSQL(dbKey, false)<<")";

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<< ossSql.str() );

	SQLString sqlString( conn.getKey(), ossSql.str() );
	dbSet res; dbQuery query( conn, sqlString, "SEL t_storagemanager(asynch)" );
	query.execForSet( res );

	dbRow row(dbKey);
	while ( res.getNextRow(row) ) {
		dbPersistent::IdType smId = 0; 
        bool isNull = false ;
		row.column(0, smId, &isNull);
		if (!isNull) {
			asyncSMids.push_back( smId );
		}
	}

	return (! asyncSMids.empty() );

}


void ActionPerformer::getListOfSM( dbConnection & conn, dbPersistent::IdType const& invId, map<dbPersistent::IdType, bool>& map_smId_sync ) const {

	map_smId_sync.clear();

	//Load Inventory ids of my children
    const string &dbKey = conn.getKey() ;
	ostringstream ossSql;
	ossSql<<
			"select sm.id, (sm.syncupload = "<<db::toSQL(dbKey, true)<<" and sm.syncdownload = "<<db::toSQL(dbKey, true)<<") as synch "
			"from t_storagemanager sm, t_inventorysm invsm "
			"where "
			"invsm.storagemanid = sm.id and "
			"invsm.inv_id = "<<db::toSQL(dbKey, invId);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<< ossSql.str() );

	SQLString sqlString( conn.getKey(), ossSql.str() );
	dbSet res; dbQuery query( conn, sqlString, "SEL t_storagemanager(sync)" );
	query.execForSet( res );

	dbRow row(dbKey);
	while ( res.getNextRow(row) ) {
		dbPersistent::IdType smId = 0;
		bool isSynch(false);
        bool isNull = false ;
		row.column(0, smId   , &isNull);
		row.column(1, isSynch, &isNull);
		map_smId_sync[ smId ] = isSynch;
	}

}

void ActionPerformer::getChildrenIds(dbConnection &conn,
                                     dbPersistent::IdType const& parentId,
                                     vector<dbPersistent::IdType>& children) const
{

    const string &dbKey = conn.getKey() ;
	//Load Inventory ids of my children
	ostringstream ossSql;
	ossSql<<
		"select inv_id1 \
		from 	t_inv_relations 				invR \
				inner join t_inv_relation_types invRt on invRt.id = invR.rel_type_id \
		where \
		invR.inv_id2 = " 		<<db::toSQL(dbKey, parentId)<<" and \
		invR.inv_id1 != "		<<db::toSQL(dbKey, parentId)<<" and \
		invRt.relation_type = "	<<db::toSQL(dbKey, "CHILD_TO_PARENT");

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query: "<< ossSql.str() );

	SQLString sqlString( conn.getKey(), ossSql.str() );
	dbSet res; 
    dbQuery query( conn, sqlString, "SEL t_inv_relations(CHILDREN)" );
	query.execForSet( res );

	dbRow row(dbKey);
	while ( res.getNextRow(row) ) {
		dbPersistent::IdType invId = 0 ;
        bool isNull = false ;
		row.column(0, invId, &isNull);
		if (!isNull) {
			children.push_back(invId);
		}
	}
}


_ACS_END_NAMESPACE

