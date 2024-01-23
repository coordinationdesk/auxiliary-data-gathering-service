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
	Revision 2.5  2018/01/29 18:36:31  marpas
	work in progress to stop during application of rolling actions
	
	Revision 2.4  2017/11/28 16:50:30  marpas
	qa warnings partially fixed
	coding best practice application in progress
	
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
	
	Revision 1.2  2008/10/01 13:54:00  ivafam
	Comment improved
	
	Revision 1.1.1.1  2008/05/27 17:09:02  ivafam
	Import libRollingAction
	
	Revision 2.0  2006/02/28 16:45:47  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/09/20 16:55:45  ivafam
	Work in progress
	
	Revision 1.2  2004/09/14 15:33:37  ivafam
	Work in progress
	
	Revision 1.1  2004/09/13 10:27:38  ivafam
	Added new classes
	
	
*/



#ifndef _InvalidPerformer_H_
#define _InvalidPerformer_H_ 

#include <ActionPerformer.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* This class implements the InvPolicyPerformer class in order to invalidate an inventory item
* (that mean remove the physical file from the specified storage and set
* the validity flag to FALSE)
**/
class InvalidPerformer : public ActionPerformer // PRQA S 2109
{		
public:
	InvalidPerformer();
	virtual ~InvalidPerformer() throw() ;

	virtual void dowork( const RollingAction & actionToProcess, bool & errorOccurred, RollingReport& rollingReport, ConfigurationSingleton::StopPredicate &, dbConnection &) const;

private:
    InvalidPerformer(const InvalidPerformer&);
	InvalidPerformer & operator=(const InvalidPerformer&);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvalidPerformer)
};

_ACS_END_NAMESPACE


#endif /* _InvalidPerformer_H_ */
