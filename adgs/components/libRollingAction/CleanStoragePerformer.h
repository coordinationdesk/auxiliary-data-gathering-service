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
	Revision 2.6  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.5  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
	Revision 2.4  2016/03/21 18:37:05  marpas
	no dbConnection ctor - they must be obtained from pool
	using db::Transaction
	
	Revision 2.3  2014/11/27 20:35:00  nicvac
	S2PDGS-897
	
	Revision 2.2  2014/04/16 09:32:24  nicvac
	S2PDGS-774: Rolling notification in application status.
	
	Revision 2.1  2013/10/01 16:21:23  nicvac
	Removing compilation warning.
	
	Revision 2.0  2013/09/23 12:19:33  nicvac
	S2PDGS-465: t_inventorysm.inv_id references t_inventory.id_inv.
	
	Revision 1.5  2013/03/12 17:46:50  marpas
	dbConnectionPool is mandatory now in file actions interfaces
	
	Revision 1.4  2009/03/04 18:10:54  marpas
	CODECHECK parsing strategy improved: no regressions
	
	Revision 1.3  2009/03/03 14:37:01  marpas
	CODECHECK parsing - no regressions
	
	Revision 1.2  2008/10/01 13:53:59  ivafam
	Comment improved
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	Revision 2.1  2006/06/21 13:19:10  ivafam
	Method compareNoCase replaced with StringUtil::compareNoCase
	
	Revision 2.0  2006/02/28 16:45:47  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/09/20 16:55:45  ivafam
	Work in progress
	
	Revision 1.3  2004/09/14 17:05:17  ivafam
	KeepPerformer class added
	
	Revision 1.2  2004/09/14 15:33:37  ivafam
	Work in progress
	
	Revision 1.1  2004/09/10 17:59:24  ivafam
	Work in progress
	
*/



#ifndef _CleanStoragePerformer_H_
#define _CleanStoragePerformer_H_ 

#include <ActionPerformer.h>

_ACS_BEGIN_NAMESPACE(acs)

/*
* This class implements the InvPolicyPerformer class in order to clean an inventory item:
* - if the specified store is the only one that retain the item ==> action skipped
* - if the item in retained by other storage (beside the specified one) ==> remove from the specified storage.
* - the item in inventory is kept as valid.
* Usually is used for example to clean the synch storage manager:
*  delete an item from synch storage manager that can be still retrieved from an asynch storage manager.
* Consider that the action itself does not discriminate among synch and asynch storage manager. It works in general,
* removing the item from the specified storage manager.
**/
class CleanStoragePerformer : public ActionPerformer // PRQA S 2109
{		
public:
 	CleanStoragePerformer();
	virtual ~CleanStoragePerformer() throw() ;

	virtual void dowork( const RollingAction & actionToProcess,bool & errorOccurred, RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) const;

private:
    CleanStoragePerformer(const CleanStoragePerformer&);
 	CleanStoragePerformer & operator=(const CleanStoragePerformer&);	

private:
 	ACS_CLASS_DECLARE_DEBUG_LEVEL(CleanStoragePerformer)
};

_ACS_END_NAMESPACE

#endif /* _CleanStoragePerformer_H_ */
