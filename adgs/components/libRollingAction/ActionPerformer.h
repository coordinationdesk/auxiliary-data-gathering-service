/*
	Copyright 1995-2022, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: libRollingAction$

	$Id$

	$Author$

	$Log$
	Revision 2.8  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.7  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.6  2016/01/21 17:05:31  nicvac
	S2PDGS-1394: Report configurable on item id
	
	Revision 2.5  2014/12/02 14:49:07  nicvac
	S2PDGS-897
	
	Revision 2.4  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.3  2014/11/26 17:14:50  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/11/26 11:18:39  nicvac
	AutoParentRemoveChildrenPerformer added
	
	Revision 2.1  2014/11/26 11:02:52  nicvac
	Refactoring
	
	Revision 2.1  2014/11/25 18:35:33  nicvac
	Refactoring
	
        
*/

#ifndef _ActionPerformer_H_
#define _ActionPerformer_H_

#include <RollingReport.h>

#include <InvPolicyPerformer.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Base class providing utilities for Action performers: E.g.:
* - invalidate the item: remove the physical file from     the storage(s) and set the inventory validity flag to FALSE
* - remove the item:     remove the physical file from ALL the storage(s) and remove the item from the inventory
* - clean the item:      remove the physical file from     the storage    and leave the inventory validity flag to TRUE. Done if it is not the last storage retaining the item.
* Other functions:
* - get the children if any
* - tell if an item has been migrated on asynch storage manager
* - get all the synch/asunch storages an item is retained
* - get the Migration report for an item migrated on asynch storage manager
*
* This Class has to be derived from classes that actually implements the Action.
*
**/

class ActionPerformer : public InvPolicyPerformer { // PRQA S 2109
public:

	/** Default Class constructor */
	ActionPerformer();
	/** Destructor */
	virtual ~ActionPerformer() throw();
#ifdef ENABLED_FOR_TEST
public:
#else
protected:
#endif

	/** Invalidate the inventory Item on storage:
	 * - remove the physical file from the specified storage
	 * - set the inventory validity flag to FALSE
	 * - in case allowTempOrderRemoval = true, according to the configuration, the temporary order item associated to the inventory record to invalidate is removed
	 * Options:
	 * - storageId=0 : remove the physical file from ALL the storages */
	bool invalidateItem( 	dbConnection &,
							dbPersistent::IdType const& invId, dbPersistent::IdType const& storageId,
							std::string& uniqueId, RollingReport::InfoRecord& infoRecord, bool allowTempOrderRemoval = false) const;

	/** Remove the inventory Item from storage:
	 * - remove the physical file from ALL the storages
	 * - remove the item from the inventory
	 * Note: because the item is removed from inventory, it is removed (by referential integrity) anywhere in db,
	 *       so it cannot be removed from just one storage manager, but from ALL of them.  */
	bool removeItem( 	dbConnection &,
						dbPersistent::IdType const& invId,
						std::string& uniqueId, RollingReport::InfoRecord& infoRecord) const;

	/** Clean the inventory item from storage:
	* - if the specified store is the only one that retain the item ==> do nothing
	* - if the item is retained by other storages (beside the specified one) ==> remove from the specified storage.
	* - the item in inventory is kept as valid.
	* Usually is used for example to clean the synch storage manager:
	*  delete an item from synch storage manager that can be retrieved from an asynch storage manager.
	* Consider that the action itself does not discriminate among synch and asynch storage manager. It works in general,
	* removing the item from the specified storage manager.
	* Return:
	* - true: the item has been cleaned from specified storage.
	* - false: the item is retained only the the specified storage. Operation cancelled.
	* - map_smId_sync: list of storage manager the item is still retained after the clean operation
	*   map_smId_sync[ x ] = true: the storage manager is of synch type
	*/
	bool cleanItem(	dbConnection &,
					dbPersistent::IdType const& invId,
					dbPersistent::IdType const& storageId,
					std::map<dbPersistent::IdType, bool>& map_smId_sync,
					std::string& uniqueId, RollingReport::InfoRecord& infoRecord) const;

	/** Set the validity flag on the inventory entry */
	void invEntrySetValid(	dbConnection &, dbPersistent::IdType const& invId, bool valid,
							std::string& uniqueId, RollingReport::InfoRecord& infoRecord) const;

	/** Just Remove the inventory entry from t_inventory */
	void invEntryRemove(	dbConnection &, dbPersistent::IdType const& invId,
							std::string& uniqueId, RollingReport::InfoRecord& infoRecord) const;

	/** True: the inventory id is stored off-line on some Asynch Storage manager
	 * asyncSMids: the asynch sm Ids where the invId is stored. */
	bool isOnAsynchSM( dbConnection &, dbPersistent::IdType const& invId, std::vector<dbPersistent::IdType>& asyncSMids) const;

	/** Get the list of storage managers that retains the inventory item.
	 * map_smId_sync[ x ] = true: the storage manager is of synch type
	 */
	void getListOfSM( dbConnection &, dbPersistent::IdType const& invId, std::map<dbPersistent::IdType, bool>& map_smId_sync ) const;

	/** Get Inventory Items Children */
	void getChildrenIds(dbConnection &, dbPersistent::IdType const& parentId, std::vector<dbPersistent::IdType>& children) const;


private:
	ActionPerformer(const ActionPerformer & ); // not implemented
	ActionPerformer &operator=(const ActionPerformer &); // not implemented
	

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ActionPerformer)

};

_ACS_END_NAMESPACE

#endif //_ActionPerformer_H_

