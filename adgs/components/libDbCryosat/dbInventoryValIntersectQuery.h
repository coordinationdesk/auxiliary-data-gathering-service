// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:28  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2012/02/14 12:52:49  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.1  2008/11/04 17:03:03  paoscu
	Added class to dbInventoryValCoverOrIntersectQuery to be inherited by other special queries
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/03/08 10:17:35  marpas
	getTriggerType method interface fixed
	
	Revision 1.8  2005/03/04 23:11:06  marpas
	getTriggerType method added to special queries interface
	By default it returns always true, it has been, hence, reimplemented for some cases
	
	Revision 1.7  2004/12/30 22:48:00  paoscu
	headers fixed
	
	Revision 1.6  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.5  2004/04/28 13:08:41  paoscu
	Incremental query support.
	
	Revision 1.4  2004/04/13 16:51:54  paoscu
	_queriedType added.
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.3  2003/10/21 13:05:24  paoscu
	staticClassName method added.
	
	Revision 1.2  2002/12/27 12:14:51  paoscu
	Now queries should be faster.
	
	Revision 1.1  2002/12/13 18:36:24  paoscu
	Imported
	
		

*/

#ifndef _dbInventoryValIntersectQuery_H_
#define _dbInventoryValIntersectQuery_H_

#include <dbInventoryValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryValIntersectQuery : // PRQA S 2153
	public dbInventoryValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryValIntersectQuery(dbInventoryValIntersectQuery const &) ;
	dbInventoryValIntersectQuery &operator=(dbInventoryValIntersectQuery const &) ;

	virtual ~dbInventoryValIntersectQuery() throw() ;

protected:
	dbInventoryValIntersectQuery(bool latest, const std::string &dbKey);
private:
	dbInventoryValIntersectQuery() ; // declared but not implemented
	
};


_ACS_END_NAMESPACE

#endif // _dbInventoryValIntersectQuery_H_
