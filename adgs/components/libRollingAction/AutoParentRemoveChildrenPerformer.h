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

	$Id$

	$Author$

	$Log$
	Revision 2.6  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.5  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.4  2014/11/28 18:39:00  nicvac
	S2PDGS-897
	
	Revision 2.3  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/11/26 17:14:50  nicvac
	S2PDGS-897
	
	Revision 2.1  2014/11/26 11:18:39  nicvac
	AutoParentRemoveChildrenPerformer added
	
        
*/

#ifndef _AutoParentRemoveChildrenPerformer_H_
#define _AutoParentRemoveChildrenPerformer_H_

#include <ActionPerformer.h>



_ACS_BEGIN_NAMESPACE(acs)

class MigrationReport ;

/**
* \brief
* This class implements the InvPolicyPerformer class in order to:
* Given an inventory item,
* - If the item has been migrated on Asynch Storage Manager
* -- Clean the item itself: remove the physical file from the specified storage and leave the inventory validity flag to TRUE (i.e. the item is valid, and offline).
*
* - Else, if not migrated on Asynch Storage Maganer
* -- Remove the item itself: remove the physical file from all the storages and remove the item from the inventory
*
* - In any case:
* -- remove the children if any (remove the physical files from the specified storage and remove the items from the inventory)
*
*
* This action policy is mainly intended for the scenario:
* - Rolling policy: roll item after its migration OR item that will not be migrated on LTA.
* For an item to be rolled:
* - if migrated ==> clean storage action (remove from synch, but you can download by asynch)
* - if not migrated (Migration decided to not save the item) ==> remove action (bye bye item)
*
**/

class AutoParentRemoveChildrenPerformer: public ActionPerformer { // PRQA S 2109
public:

	/** Default Class constructor */
	AutoParentRemoveChildrenPerformer();
	/** Destructor */
	virtual ~AutoParentRemoveChildrenPerformer() throw();

	/** Interface implementation */
	virtual void dowork( const RollingAction & actionToProcess, bool & errorOccurred, RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) const;

    static void getReportFromPkg (dbConnection &, dbPersistent::IdType pkgid, MigrationReport&);
    static std::string getClobFromPkg (dbConnection &, dbPersistent::IdType pkgid);
    static dbPersistent::IdType getPackageFromInv (dbConnection &, dbPersistent::IdType invid, dbPersistent::IdType smid);
    static void deleteClobFromPkg (dbConnection &, dbPersistent::IdType pkgid, dbPersistent::IdType smid = 0 ) ;

private:  
	AutoParentRemoveChildrenPerformer(const AutoParentRemoveChildrenPerformer & ); // not implemented 
	AutoParentRemoveChildrenPerformer &operator=(const AutoParentRemoveChildrenPerformer &);  // not implemented 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AutoParentRemoveChildrenPerformer)

};

_ACS_END_NAMESPACE

#endif //_AutoParentRemoveChildrenPerformer_H_

