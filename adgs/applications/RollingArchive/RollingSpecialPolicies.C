/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/ 

#include <RollingSpecialPolicies.h>

#include <Application.h>

#include <RollingAlgoUtils.h>
#include <RollingPoliciesActions.h>

#include <InvPolicyPerformerFactory.h>

#include <dbInvSpecialRollingPolicies.h>
#include <dbInvSpecialRollingPoliciesQuery.h>

#include <dbInventoryObject.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbPersistentScopeGuard.h>
#include <dbPersistentFactoryChain.h>

#include <Filterables.h>

#include <set>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingSpecialPolicies)



RollingSpecialPolicies::RollingSpecialPolicies():
	_mutex(),
	_confTimeRunSec(60.0),
	_firstRequest(true),
	_timerRun(),
	_specialPolicyId( 0 ),
	_listRollingActionsKeep(),
	_listRollingActionsNoKeep()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	this->loadConfiguration();

	//Start timer for run
	_timerRun.start();
}

void RollingSpecialPolicies::loadConfiguration() {

	static bool logOnce(false);

	// Get all configured resources
	rsResourceSet & conf = ConfigurationSingleton::instance()->get();

	// get form conf space time beetwen to RollingArchive activities
	if ( not conf.getValue("RollingArchive.SpecialPoliciesTimerSec", _confTimeRunSec, false) ) {
		if ( not logOnce ) {
			ACS_LOG_WARNING("No RollingArchive.SpecialPoliciesTimerSec in configuration. Default period is: " << _confTimeRunSec << " [s].");
			logOnce = true;
		}
	}

}


void RollingSpecialPolicies::execute( bool forceExecution ) {

	Lock lock(_mutex);

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	this->loadConfiguration();

	_timerRun.stop(); //not really stop, just to allow elapsed to take the time
	if ( _firstRequest || forceExecution || _timerRun.elapsed()/1000.0 >= _confTimeRunSec ) {

		if (_firstRequest || forceExecution) { ACS_LOG_PRIVINFO( "RollingSpecialPolicies: Querying Special Policies"); }
		else                                 { ACS_LOG_PRIVINFO( "RollingSpecialPolicies: Querying Special Policies (elapsed "<<_confTimeRunSec<<" [sec])"); }

		_firstRequest = false;

		//Do the actual work
		this->doWork();

		_timerRun.start();
	} else {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "RollingSpecialPolicies: time "<<_confTimeRunSec<<" [sec] not elapsed. Query skipped");
	}

}

void RollingSpecialPolicies::doWork() {

	Lock lock(_mutex);

	//Update the manual rolling requests on t_invspecialrollingpolicies (set it up for next loop)
	this->updateInvSpecialRollingPolicies();

	// Allocates list of InventoryObject to perform action on
	_listRollingActionsKeep.clear();
	_listRollingActionsNoKeep.clear();

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, Application::appName()<<": start query to t_InvSpecialRollingPolicies");

	// Get the default connection Pool
	dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
	const string dbKey = cp->getKey();
	dbConnectionWrapper pw(*cp) ; //Keep this connection for the current scope
	dbConnection &theConnection = pw;

	RollingPoliciesActions* actions = RollingPoliciesActions::instance(dbKey);

	// Queries for InvSpecialRollingPolicies and fills list with InventoryObject and related Action
	dbInvSpecialRollingPoliciesQuery * rollingSpecialPolicies = dynamic_cast<dbInvSpecialRollingPoliciesQuery*>(dbQueryFactoryChain::instance()->newObject(dbInvSpecialRollingPoliciesQuery::staticClassName(), dbKey));
	dbGenQueryScopeGuard rollingSpecialPoliciesSG(rollingSpecialPolicies) ;
	if (!rollingSpecialPolicies) {
		ACS_THROW(exCriticalException("Cannot create objects of type dbInvSpecialRollingPoliciesQuery")) ;  // PRQA S 3081
	}
	// Perform query
	rollingSpecialPolicies->setOrderBy("action ASC ");
	rollingSpecialPolicies->doQuery(theConnection);

	//Get list of inventory items to be preserved (because on-going operations on them)
	set< dbPersistent::IdType > invIdsToPreserve;
	bool foundAction_noKeep(false);
	for (size_t i=0; not foundAction_noKeep && i < rollingSpecialPolicies->size(); ++i) {
		foundAction_noKeep = ( "KEEP" != actions->name( (*rollingSpecialPolicies)[i]->getAction() ) );
	}
	if ( foundAction_noKeep ) {
		RollingAlgoUtils::instance()->getInventoryItemsToBePreserved(theConnection, invIdsToPreserve);
		if ( ! invIdsToPreserve.empty() ) {
		    ACS_LOG_INFO( "There are ["<<invIdsToPreserve.size()<<"] inventory items to be preserved "
		    		"because involved by on-going operations (LTA upload or distribution) or by a configured retain rule. "
		    		"They will be preserved from rolling." );
		}
	}

	size_t manualRollingForcedToKeep(0);
	for (size_t i=0; i < rollingSpecialPolicies->size(); ++i) {

		dbPersistent::IdType invobjId(0);
		{
			dbInventoryObject* invObject=dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject(dbInventoryObject::staticClassName(), dbKey));
			dbPersistentScopeGuard scopeG(invObject);
			ACS_COND_THROW( !invObject, // PRQA S 3081
					exCriticalException("Cannot create objects of type dbInventoryObject"));

			invObject->setFileName(    (*rollingSpecialPolicies)[i]->getInventoryName() );
			invObject->setFileVersion( (*rollingSpecialPolicies)[i]->getInventoryVersion() );
			invObject->load(theConnection);
			invobjId = invObject->getId();
		}

		bool toBePreserved = ( invIdsToPreserve.find(invobjId) != invIdsToPreserve.end() );
		if( toBePreserved ) {
			// If the the item is to be preserved, insert a KEEP action on this specific file

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Manual rolling id ["<<(*rollingSpecialPolicies)[i]->getId()<<"] "
					"item ["<<(*rollingSpecialPolicies)[i]->getInventoryName()<<"] "
					"version ["<<(*rollingSpecialPolicies)[i]->getInventoryVersion()<<"] is in the list of items to be preserved." );

			if ( "KEEP" != actions->name( (*rollingSpecialPolicies)[i]->getAction() ) ) {
				++manualRollingForcedToKeep;
			}

			RollingAction currAction(invobjId, actions->id("KEEP", true), (*rollingSpecialPolicies)[i]->getStorage());
			InvPolicyPerformerFactory::populateActionToPerfom(theConnection, currAction, _listRollingActionsKeep);

		} else {

			RollingAction currAction(invobjId, (*rollingSpecialPolicies)[i]->getAction(), (*rollingSpecialPolicies)[i]->getStorage());

			if ( "KEEP" == actions->name( (*rollingSpecialPolicies)[i]->getAction() ) ) {
				InvPolicyPerformerFactory::populateActionToPerfom(theConnection, currAction, _listRollingActionsKeep);
			} else {
				InvPolicyPerformerFactory::populateActionToPerfom(theConnection, currAction, _listRollingActionsNoKeep);
			}
		}
	}

	if ( manualRollingForcedToKeep > 0 ) {
		ACS_LOG_INFO( "Manual Rolling: there are ["<<manualRollingForcedToKeep<<"] manual rolling actions forced to KEEP action (because items in the list to be preserved)." );
	}
	if ( _listRollingActionsNoKeep.size() > 0 ) {
		ACS_LOG_INFO( "Manual Rolling: there are ["<<_listRollingActionsNoKeep.size()<<"] manual rolling actions to be performed." );
	}
	if ( _listRollingActionsKeep.size() > 0 ) {
		ACS_LOG_INFO( "Manual Rolling: there are ["<<_listRollingActionsKeep.size()<<"] manual KEEP rolling actions." );
	}

}


void RollingSpecialPolicies::updateInvSpecialRollingPolicies() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Clean requests from the "manual rolling" queue (t_invspecialrollingpolicies).
	// Preconditions:
	// - You add a manual rolling request on t_invspecialrollingpolicies to a specific inventory item that is:
	// -- referenced in t_inventory (because referential integrity)
	// -- stored on some storage manager (i.e. referenced by t_inventorysm)
	//
	// 1. If you set a KEEP action, after the action is performed the inventory item is still on SM (item is still referenced by t_inventorysm)
	// 2. If you set a destructive action, after the action is performed the inventory item is not anymore on SM
	//    (invalidate, remove, clean always remove from the requested SM)
	//
	// So, in the general case:
	// If the manual rolling operation is on SM x
	// - if the inventory item is still on SM x ==> the action still has not been performed or it was a KEEP action ==> keep the manual rolling request
	// - if the inventory item in not on SM x ==> the manual rolling has been performed and can be removed from the manual rolling queue.
	//
	// Note: consider that an action that removes the item also from t_inventory (like remove action),
	//       causes the delete on cascade on t_invspecialrollingpolicies (because referential integrity).
	//       So in that case the cleaning is automatically performed by dbms.
	//       So this code is useful for an action (like invalidate) that remove item from SM but keeps reference in t_inventory

	try {

		dbConnectionPool* cp = Application::instance()->getDefaultConnectionPool();
		dbConnectionWrapper pw(*cp); dbConnection &conn = pw ;
        const string &dbKey = conn.getKey() ;

		ostringstream ossSqlTime; ossSqlTime<<
		"Delete from t_invspecialrollingpolicies where id in                                             \n"
		"(                                                                                               \n"
		"	With                                                                                         \n"
		"	rollManual as (                                                                              \n"
		"		SELECT invroll.id as rollId, invroll.expirationdate as expDate,                          \n"
		"              invroll.storageid as rollStorageId, invsm.storagemanid as storageId               \n"
		"		from                                                                                     \n"
		"		t_invspecialrollingpolicies invroll inner join (                                         \n"
		"		   t_inventory inv left join t_inventorysm invsm on inv.id_inv = invsm.inv_id            \n"
		"		) on invroll.inventoryname = inv.filename and invroll.inventoryversion = inv.fileversion \n"
		"	),                                                                                           \n"
		"	rollManualToProcess as (                                                                     \n"
		"		select rollId from rollManual                                                            \n"
		"       where                                                                                    \n"
		"       (rollStorageId = storageId or (rollStorageId is null and storageId is not null) ) and    \n"
		"       (expDate is null or "<<SQLDialectFactory::sqlDialect(dbKey).currentDateTime() << " <= expDate ) \n"
		"	)                                                                                            \n"
		"	select rollId                                                                                \n"
		"	from rollManual                                                                              \n"
		"   where                                                                                        \n"
		"   rollId not in (select rollId from rollManualToProcess)                                       \n"
		");";

		SQLString sqlString(dbKey, ossSqlTime.str() );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<"]");

		dbQuery sqlDelete( conn, sqlString, "updateInvSpecialRollingPolicies" );
		sqlDelete.execForStatus();

	} catch(exception &e) {
		ACS_LOG_TRYCATCHNOTIFY_EX(
				exSystemException( e, "Unable to update the Special Rolling Policies. Will retry on next loop.") );
	}

}


RollingPolicyOutput::Sptr RollingSpecialPolicies::getListRollingActionsKeep() const {
	Lock lock(_mutex);

	RollingPolicyOutput::Sptr actionsKeepSptr( new RollingPolicyOutput );

	actionsKeepSptr->setPolicyId( _specialPolicyId );
	actionsKeepSptr->setIsSpecialPolicy(true);
	actionsKeepSptr->getListRollingActions() = _listRollingActionsKeep;

	//Do not clear _listRollingActionsKeep. To be returned each time this method is called.
	// This because items to keep will be never consumed.

	return actionsKeepSptr;
}

RollingPolicyOutput::Sptr RollingSpecialPolicies::getListRollingActionsNoKeep_and_clear() {
	Lock lock(_mutex);

	RollingPolicyOutput::Sptr actionsNoKeepSptr( new RollingPolicyOutput );

	actionsNoKeepSptr->setPolicyId( _specialPolicyId );
	actionsNoKeepSptr->setIsSpecialPolicy(true);
	actionsNoKeepSptr->getListRollingActions() = _listRollingActionsNoKeep;

	//Caller consumes _listRollingActionsNoKeep ==> _listRollingActionsNoKeep can be clered.
	_listRollingActionsNoKeep.clear();

	return actionsNoKeepSptr;
}


_ACS_END_NAMESPACE



