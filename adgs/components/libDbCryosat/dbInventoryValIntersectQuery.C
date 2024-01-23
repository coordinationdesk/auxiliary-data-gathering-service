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
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
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
	
	Revision 2.2  2008/11/04 17:03:03  paoscu
	Added class to dbInventoryValCoverOrIntersectQuery to be inherited by other special queries
	
	Revision 2.1  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.17  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.16  2005/06/17 10:30:22  marpas
	using namespace std added
	
	Revision 1.15  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.14  2005/03/08 10:17:35  marpas
	getTriggerType method interface fixed
	
	Revision 1.13  2005/03/04 23:11:06  marpas
	getTriggerType method added to special queries interface
	By default it returns always true, it has been, hence, reimplemented for some cases
	
	Revision 1.12  2004/12/30 22:48:00  paoscu
	headers fixed
	
	Revision 1.11  2004/10/19 11:59:30  ivafam
	Control if clause "Order BY" is setted
	
	Revision 1.10  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.9  2004/04/28 13:08:41  paoscu
	Incremental query support.
	
	Revision 1.8  2004/04/13 16:52:56  paoscu
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.7  2003/11/18 17:35:49  paoscu
	Order by configurable by user.
	
	Revision 1.6  2003/10/21 13:05:24  paoscu
	staticClassName method added.
	
	Revision 1.5  2003/07/29 12:22:50  paoscu
	progReset() moved.
	
	Revision 1.4  2003/07/25 15:17:53  paoscu
	potential memroy leaks removed
	order by management improved
	
	Revision 1.3  2002/12/27 12:14:51  paoscu
	Now queries should be faster.
	
	Revision 1.2  2002/12/17 17:45:57  paoscu
	A few changes in protected methods.
	
	Revision 1.1  2002/12/13 18:36:24  paoscu
	Imported
	
		

*/

#include <dbInventoryValIntersectQuery.h>
#include <dbInventoryObject.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>


#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryValIntersectQuery::dbInventoryValIntersectQuery(bool latest, const string &dbKey)
: dbInventoryValCoverOrIntersectQuery(latest , true, dbKey)
{
}


dbInventoryValIntersectQuery::dbInventoryValIntersectQuery(dbInventoryValIntersectQuery const &r)
: dbInventoryValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryValIntersectQuery &dbInventoryValIntersectQuery::operator=(dbInventoryValIntersectQuery const &r)
{
    if(this != &r) {
	    dbInventoryValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}



dbInventoryValIntersectQuery::~dbInventoryValIntersectQuery() throw() {}



_ACS_END_NAMESPACE

