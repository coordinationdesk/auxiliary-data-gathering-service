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

	$Prod: A.C.S. Rolling Archive $
	
*/


#include <CleanStoragePerformer.h>
#include <RollingAction.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CleanStoragePerformer)

CleanStoragePerformer::CleanStoragePerformer() : ActionPerformer()
{
}

CleanStoragePerformer::~CleanStoragePerformer() throw()
{
}

void CleanStoragePerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	try {

		RollingReportAutoSession rollingReportAutoSession( rollingReport );

		string uniqueId;
		RollingReport::InfoRecord currentItemInfo;

		//Storage managers where the item is stored.
		map<dbPersistent::IdType, bool>  map_smId_sync;

		if ( this->cleanItem( conn, actionToProcess.getObject(), actionToProcess.getStorage(),
		                      map_smId_sync, uniqueId, currentItemInfo ) ) {

			//Record offline in rolling report if the item now is only retained by asynch storages.
			bool foundSync(false);
			for (auto it = map_smId_sync.begin(); not foundSync && it != map_smId_sync.end(); ++it) {
			    foundSync = (it->second == true);
			}

			bool offline = not foundSync;
			if ( offline ) {
				rollingReport.record( uniqueId, RollingReport::offline, currentItemInfo );
			}

			ostringstream msg;
			msg<<"Cleaned Inventory Id ["<<actionToProcess.getObject()<<"] "<<uniqueId<<" "
			     "on storage id ["<<actionToProcess.getStorage()<<"]"
			     "offline status ["<<boolalpha<<offline<<"]";
			ACS_LOG_INFO( msg.str() );

		} else {

			const bool & isSync = map_smId_sync.at( actionToProcess.getStorage() );

			ostringstream msg;
			msg<<"Inventoy Id ["<< actionToProcess.getObject() << "] is stored only on the selected "<<(isSync?"synch":"asynch")<<" storage id ["<< actionToProcess.getStorage()<<"]. "
				"If you want to remove it use invalidate action or "<<( isSync ? "wait for the inventory item migration" : "save it to another storage" );
			ACS_LOG_INFO(msg.str());
		}


	} catch(exception & ex) {

		errorOccurred = true;
		ACS_LOG_WARNING("Problem occurred Cleaning Inventory Id ["<< actionToProcess.getObject()<<"] from  storage id ["<<actionToProcess.getStorage()<<"]. Will retry on next loop. Problem occurred: "<<ex.what());
	}

}


_ACS_END_NAMESPACE
