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
	Revision 2.7  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.6  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.5  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.4  2014/11/26 11:02:52  nicvac
	Refactoring
	
	Revision 2.3  2014/11/25 18:35:33  nicvac
	Refactoring
	
	Revision 2.2  2014/04/16 09:32:24  nicvac
	S2PDGS-774: Rolling notification in application status.
	
	Revision 2.1  2013/11/19 17:56:30  nicvac
	S2PDGS-531: Aggiunta action RemoveParent-RemoveChildren.
	
	Revision 2.0  2013/09/23 12:19:33  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.2  2013/09/20 13:19:48  nicvac
	S2PDGS-303: coding completed.
	
	Revision 1.1  2013/09/19 17:33:52  nicvac
	S2PDGS-303: implementation.
	
        
*/

#ifndef _InvalidateParentRemoveChildrenPerformer_H_
#define _InvalidateParentRemoveChildrenPerformer_H_

#include <ActionPerformer.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* This class implements the InvPolicyPerformer class in order to:
* Given an inventory item,
* - invalidate the item itself: remove the physical file from the specified storage and set the inventory validity flag to FALSE
* - remove the children if any (remove the physical files from all the storage and remove the items from the inventory)
**/

class InvalidateParentRemoveChildrenPerformer: public ActionPerformer { // PRQA S 2109
public:

	/** Default Class constructor */
	InvalidateParentRemoveChildrenPerformer();
	/** Destructor */
	virtual ~InvalidateParentRemoveChildrenPerformer() throw();

	/** Interface implementation */
	virtual void dowork( const RollingAction & actionToProcess, bool & errorOccurred, RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) const;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	InvalidateParentRemoveChildrenPerformer(const InvalidateParentRemoveChildrenPerformer & );
	/** Operator = */
	InvalidateParentRemoveChildrenPerformer &operator=(const InvalidateParentRemoveChildrenPerformer &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvalidateParentRemoveChildrenPerformer)

};

_ACS_END_NAMESPACE

#endif //_InvalidateParentRemoveChildrenPerformer_H_
