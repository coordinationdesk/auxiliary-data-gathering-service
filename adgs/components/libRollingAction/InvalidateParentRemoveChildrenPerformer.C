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

#include <InvalidateParentRemoveChildrenPerformer.h>

#include <RollingPoliciesActions.h>
#include <RollingAction.h>

#include <dbPersistentScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbEraseFile.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbConnectionPool.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvalidateParentRemoveChildrenPerformer)



InvalidateParentRemoveChildrenPerformer::InvalidateParentRemoveChildrenPerformer() :
    ActionPerformer() 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

InvalidateParentRemoveChildrenPerformer::~InvalidateParentRemoveChildrenPerformer() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}



void InvalidateParentRemoveChildrenPerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string currentItem;
	RollingReport::InfoRecord currentItemInfo;

	//... invalidate parent: remove the physical file from the specified storage and set the validity flag to FALSE. And ...
	try {

		RollingReportAutoSession rollingReportAutoSession( rollingReport );

		if ( this->invalidateItem( conn, actionToProcess.getObject(), actionToProcess.getStorage(), currentItem, currentItemInfo ) ) {
			rollingReport.record( currentItem, RollingReport::invalidated, currentItemInfo );
		}

		//... remove the children (both physical file from the specified storage and entry from inventory).
		try {
			vector<dbPersistent::IdType> childrenIds;
			this->getChildrenIds( conn, actionToProcess.getObject(), childrenIds );

			string childItem;
			RollingReport::InfoRecord childItemInfo;
			for (vector<dbPersistent::IdType>::iterator it=childrenIds.begin(); it!=childrenIds.end(); ++it) {

				if ( this->removeItem( conn, *it, childItem, childItemInfo ) ) {
					rollingReport.record( childItem, RollingReport::removed, childItemInfo );
				}
			}

		} catch(exception& ex) {
			//Notify the DB contains orphans. A specific and controlled Process/Procedure is in charge of clean orphans.
			ACS_LOG_WARNING("Problem occurred removing Children of Invalidated Parent Inventory item id ["<< actionToProcess.getObject()<<"]. DB contains Orphan items.");
		}

		ACS_LOG_INFO("Invalidated Inventory Id ["<<actionToProcess.getObject()<<"] "<<currentItem<<"  and removed children (if any)" );

	} catch(exception & ex) {

		errorOccurred = true;
		ACS_LOG_WARNING("Problem occurred invalidating Inventory Id ["<< actionToProcess.getObject()<<"]. Will retry on next loop. Problem occurred: "<<ex.what());
	}


}

_ACS_END_NAMESPACE

