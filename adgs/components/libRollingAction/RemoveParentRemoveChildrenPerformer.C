// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libRollingAction$
        
*/ 

#include <RemoveParentRemoveChildrenPerformer.h>

#include <RollingPoliciesActions.h>
#include <RollingAction.h>
#include <dbConnectionPool.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
ACS_CLASS_DEFINE_DEBUG_LEVEL(RemoveParentRemoveChildrenPerformer)



RemoveParentRemoveChildrenPerformer::RemoveParentRemoveChildrenPerformer() : 
    ActionPerformer() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RemoveParentRemoveChildrenPerformer::~RemoveParentRemoveChildrenPerformer() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void RemoveParentRemoveChildrenPerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string currentItem;
	RollingReport::InfoRecord currentItemInfo;

	//... invalidate parent: remove the physical file from the specified storage and set the validity flag to FALSE. And ...
	try {

		RollingReportAutoSession rollingReportAutoSession( rollingReport );

		if ( this->invalidateItem(conn, actionToProcess.getObject(), actionToProcess.getStorage(), currentItem, currentItemInfo) ) {
			rollingReport.record( currentItem, RollingReport::invalidated, currentItemInfo );
		}

		//... remove the children (both physical file from the specified storage and entry from inventory). And ...
		try {
			vector<dbPersistent::IdType> childrenIds;
			this->getChildrenIds( conn, actionToProcess.getObject(), childrenIds );

			for (vector<dbPersistent::IdType>::iterator it=childrenIds.begin(); it!=childrenIds.end(); ++it) { // PRQA S 4238
				string childrenItem;
				RollingReport::InfoRecord childrenItemInfo;
				if ( this->removeItem( conn, *it, childrenItem, childrenItemInfo ) ) {
					rollingReport.record( childrenItem, RollingReport::removed, childrenItemInfo );
				}

			}

		} catch(exception& ex) {
			//Notify the DB contains orphans. A specific and controlled Process/Procedure is in charge of clean orphans.
			ACS_LOG_WARNING("Problem occurred removing Children of Removed Parent Inventory item id ["<< actionToProcess.getObject()<<"]. DB contains Orphan items.");
		}

		//... remove the invalidated Parent.
		// But why do you invalidate and then remove parent?
		// - Can't you just remove the parent first and then the children? No, because the delete on cascade, no children will be detected
		// - Can't you remove the children first and then the parent? No, because system level issues could be raised (e.g.: a valid parent with partial children)
		try {
			string x; RollingReport::InfoRecord y; //Not interesting here
			this->invEntryRemove( conn, actionToProcess.getObject(), x, y );
			rollingReport.record( currentItem, RollingReport::removed, currentItemInfo );

		} catch (exception& ex) {
			//Notify the DB contains invalidated items that should be removed.
			ACS_LOG_WARNING("Problem occurred removing Inventory item id ["<< actionToProcess.getObject()<<"]. Anyway the item is invalidated (removed from storage manager and invalidated in inventory).");
		}

		ACS_LOG_INFO("Removed Inventory Id ["<<actionToProcess.getObject()<<"] "<<currentItem<<"  and removed children (if any)" );

	} catch(exception& ex) {

		errorOccurred = true;

		ACS_LOG_WARNING("Problem occurred removing Inventory Id ["<< actionToProcess.getObject()<<"]. Will retry on next loop. Problem occurred: "<<ex.what());
	}


}

_ACS_END_NAMESPACE

