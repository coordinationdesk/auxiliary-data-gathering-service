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

	$Id$

	$Author$

	$Log$
	Revision 2.12  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.11  2016/07/29 14:24:10  nicvac
	CSGACQ-86: Restored last version.
	
	Revision 2.10  2016/07/29 09:42:06  nicvac
	S2PDGS-1501: Restored the previous version of this file to release a stable version of the lib. Last version of this file implies the inclusions of another lib not yet stable at this time.
	
	Revision 2.9  2016/07/21 16:15:30  nicvac
	CSGACQ-86: Oracle porting
	
	Revision 2.8  2016/05/10 20:34:43  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.7  2016/04/18 15:25:17  marpas
	some NameIdAssociators adhere to the standards now - they need a db key
	
	Revision 2.6  2016/03/21 18:37:05  marpas
	no dbConnection ctor - they must be obtained from pool
	using db::Transaction
	
	Revision 2.5  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.4  2014/04/16 09:32:24  nicvac
	S2PDGS-774: Rolling notification in application status.
	
	Revision 2.3  2013/11/19 17:56:30  nicvac
	S2PDGS-531: Aggiunta action RemoveParent-RemoveChildren.
	
	Revision 2.2  2013/10/02 15:08:02  nicvac
	acs sdk warnings processed.
	
	Revision 2.1  2013/09/26 19:25:16  nicvac
	S2PDGS-303: Rolling actions can be ordered on inventory fields criteria.
	
	Revision 2.0  2013/09/23 12:19:33  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.7  2013/09/13 15:34:22  nicvac
	S2PDGS-303 implementing.
	
	Revision 1.6  2013/06/20 13:39:54  marpas
	direct use of excmcf is prohibited
	
	Revision 1.5  2013/03/12 17:46:50  marpas
	dbConnectionPool is mandatory now in file actions interfaces
	
	Revision 1.4  2012/06/06 16:25:51  marfav
	Code improved removing compiler warnings
	
	Revision 1.3  2008/10/09 17:18:18  ivafam
	Work in progress
	
	Revision 1.2  2008/10/01 13:53:59  ivafam
	Comment improved
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	Revision 2.0  2006/02/28 16:45:47  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/09/20 16:55:45  ivafam
	Work in progress
	
	Revision 1.4  2004/09/14 17:05:17  ivafam
	KeepPerformer class added
	
	Revision 1.3  2004/09/14 15:33:37  ivafam
	Work in progress
	
	Revision 1.2  2004/09/10 17:59:24  ivafam
	Work in progress
	
	Revision 1.1.1.1  2004/09/10 16:26:01  ivafam
	Import RollingArchive
	
*/ 


#include <InvPolicyPerformerFactory.h>
#include <Transaction.h>
#include <RemovePerformer.h>
#include <InvalidPerformer.h>
#include <KeepPerformer.h>
#include <CleanStoragePerformer.h>
#include <InvalidateParentRemoveChildrenPerformer.h>
#include <RemoveParentRemoveChildrenPerformer.h>
#include <AutoParentRemoveChildrenPerformer.h>
#include <RollingAction.h>
#include <RollingPoliciesActions.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <list>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvPolicyPerformerFactory)

// Costructor
InvPolicyPerformerFactory::InvPolicyPerformerFactory() :
    Singleton<InvPolicyPerformerFactory>()
{
}

// Destructor
InvPolicyPerformerFactory::~InvPolicyPerformerFactory() throw() 
{
}


// Build the right action performer and perform action
void InvPolicyPerformerFactory::performAction(	const RollingAction & actionToProcess, bool & errorOccurred,
												RollingReport& rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection &conn) { // PRQA S 4211
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	RollingPoliciesActions*  Actions = RollingPoliciesActions::instance(conn.getKey());
	const string & actionName = Actions->name( actionToProcess.getRollingAction() );

    if ( StringUtils::equalsNoCase( actionName, "KEEP") ) {
    	// Create Keep action
    	ACS_SMARTPTR<KeepPerformer> actionAptr( new KeepPerformer() ) ;
    	actionAptr->dowork(actionToProcess, errorOccurred, rollingReport, stop, conn);

    } else if ( StringUtils::equalsNoCase( actionName, "CLEANSTORAGE") ) {
    	// Create Clean Storage action
		ACS_SMARTPTR<CleanStoragePerformer> actionAptr( new CleanStoragePerformer() ) ;
		actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn );

    } else if ( StringUtils::equalsNoCase( actionName, "INVALIDATE") ) {
	    // Create Invalidate action
		ACS_SMARTPTR<InvalidPerformer> actionAptr( new InvalidPerformer() ) ;
		actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn );

    } else if ( StringUtils::equalsNoCase( actionName, "REMOVE") ) {
	    // Create Remove action
		ACS_SMARTPTR<RemovePerformer> actionAptr( new RemovePerformer() ) ;
		actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn );

    } else if ( StringUtils::equalsNoCase( actionName, "INVALIDATEPARENT-REMOVECHILDREN") ) {
    	// Create Invalidate Parent & Remove Children action
    	ACS_SMARTPTR<InvalidateParentRemoveChildrenPerformer> actionAptr( new InvalidateParentRemoveChildrenPerformer() ) ;
    	actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn );

    } else if ( StringUtils::equalsNoCase( actionName, "REMOVEPARENT-REMOVECHILDREN") ) {
    	// Create Remove Parent & Remove Children action
    	ACS_SMARTPTR<RemoveParentRemoveChildrenPerformer> actionAptr( new RemoveParentRemoveChildrenPerformer() ) ;
    	actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn);

    } else if ( StringUtils::equalsNoCase( actionName, "AUTOPARENT-REMOVECHILDREN") ) {
    	// Create Auto Parent & Remove Children action
    	ACS_SMARTPTR<AutoParentRemoveChildrenPerformer> actionAptr( new AutoParentRemoveChildrenPerformer() ) ;
    	actionAptr->dowork( actionToProcess, errorOccurred, rollingReport, stop, conn);

    } else {
    	ostringstream msg; msg<<"Misconfiguration detected. Action id ["<<actionToProcess.getRollingAction()<<"] not supported.";
    	ACS_LOG_WARNING( msg.str() );
    	errorOccurred = true;
    }

}

// Performs check on the possibility to add action and if it is possible add/replace the action
bool InvPolicyPerformerFactory::populateActionToPerfom(dbConnection &conn, RollingAction & action, std::set<RollingAction> & listSet) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// If the inventory object is not in the list insert it
	listSet.insert(action);

	RollingPoliciesActions*  actions = RollingPoliciesActions::instance(conn.getKey());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Policy  Added  for action [" << actions->name(action.getRollingAction())<< "] and storageid ["<<action.getStorage()<<"]");

	return true;

	/*
	set<RollingAction>::iterator iter ;
	RollingAction::sameInventoryObject compare;
	iter= find_if( listSet.begin(), listSet.end(), bind2nd(compare,action) );
	const bool inventoryItemsInList = ( iter != listSet.end() );
	*/
}


_ACS_END_NAMESPACE

