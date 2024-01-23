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
	Revision 2.7  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.6  2017/11/28 17:01:36  marpas
	qa silencer fixed
	
	Revision 2.5  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.4  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.3  2014/04/16 09:32:24  nicvac
	S2PDGS-774: Rolling notification in application status.
	
	Revision 2.2  2013/10/02 15:08:02  nicvac
	acs sdk warnings processed.
	
	Revision 2.1  2013/09/26 19:25:16  nicvac
	S2PDGS-303: Rolling actions can be ordered on inventory fields criteria.
	
	Revision 2.0  2013/09/23 12:19:33  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.6  2013/03/12 17:46:50  marpas
	dbConnectionPool is mandatory now in file actions interfaces
	
	Revision 1.5  2009/03/04 18:10:54  marpas
	CODECHECK parsing strategy improved: no regressions
	
	Revision 1.4  2009/03/03 14:37:01  marpas
	CODECHECK parsing - no regressions
	
	Revision 1.3  2008/10/09 17:18:18  ivafam
	Work in progress
	
	Revision 1.2  2008/10/01 13:53:59  ivafam
	Comment improved
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	Revision 2.1  2006/06/21 13:19:10  ivafam
	Method compareNoCase replaced with StringUtil::compareNoCase
	
	Revision 2.0  2006/02/28 16:45:47  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/09/14 15:33:37  ivafam
	Work in progress
	
	Revision 1.2  2004/09/10 17:59:24  ivafam
	Work in progress
	
	Revision 1.1.1.1  2004/09/10 16:26:01  ivafam
	Import RollingArchive
	

*/


#ifndef _InvPolicyPerformerFactory_H_
#define _InvPolicyPerformerFactory_H_ 

#include <RollingReport.h>
#include <ConfigurationSingleton.h>

#include <Singleton.hpp>

#include <string>

#include <set>


_ACS_BEGIN_NAMESPACE(acs)

class RollingAction;
class dbConnection ;

using pattern::Singleton ;


// Factory for the action performer
class InvPolicyPerformerFactory : public Singleton<InvPolicyPerformerFactory> // PRQA S 2109, 2153
{

public:
	// class NotFoundException declaration 
    exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502
	virtual ~InvPolicyPerformerFactory() throw() ;

	ACS_DECLARE_NO_COPY_CLASS(InvPolicyPerformerFactory);
	friend InvPolicyPerformerFactory* Singleton<InvPolicyPerformerFactory>::instance(); // PRQA S 2107

public:

   	/** Perform the action */
	void performAction(	const RollingAction & actionToProcess, bool & errorOccurred,
						RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) ;

	/**
	 * Add the action to listSet
	 * Return true if the action is added
	 * >>>>>> NOTE <<<<<<
	 * In the Legacy implementation the policies were run in sequence and on all over the Inventory (without a limit clause on select).
	 * In the overall result, that was the union of all the policies, you could have two different actions (type of operation / storage managers) on
	 * the same inventory item. This can happen because the same item can be selected by different policies.
	 * This method resolved the conflicts (took safest actions, according to the type of operation and store manager) and it was possible because policies were
	 * executed sequentially.
	 * As per S3MARINE-93, (and saying the truth even before, since when limit clause was introduced in the policy sql select to improve performance)
	 * policies returns limited numbers of items (a burst) and there are independent queues of actions to be executed. So there is no way to resolve conflicts.
	 * Conflicts resolution are demanded on configuration side: who define rolling policy must take care of avoid conflicts.
	 */
	static bool populateActionToPerfom(dbConnection &, RollingAction & action, std::set<RollingAction> & listSet) ;
	
protected:
	InvPolicyPerformerFactory();

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvPolicyPerformerFactory)
} ;


_ACS_END_NAMESPACE

#endif // _InvPolicyPerformerFactory_H_ 

