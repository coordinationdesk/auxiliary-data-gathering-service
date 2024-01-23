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
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:59  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:27  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.2  2009/01/25 09:08:05  marfav
	Query derives from OnSite family
	
	Revision 2.1  2008/12/02 16:57:29  paoscu
	dbInventoryOnOrbitValCoverOrIntersectQuery added
	
	
	
*/

#ifndef _dbInventoryOnOrbitValCoverOrIntersectQuery_H_
#define _dbInventoryOnOrbitValCoverOrIntersectQuery_H_

#include <dbInventoryOnSiteValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryOnOrbitValCoverOrIntersectQuery : // PRQA S 2153
	public dbInventoryOnSiteValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryOnOrbitValCoverOrIntersectQuery(dbInventoryOnOrbitValCoverOrIntersectQuery const &) ;
	dbInventoryOnOrbitValCoverOrIntersectQuery &operator=(dbInventoryOnOrbitValCoverOrIntersectQuery const &) ;
	virtual ~dbInventoryOnOrbitValCoverOrIntersectQuery() throw() ;
	
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true) ;

protected:
	dbInventoryOnOrbitValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey) ;
private:
	dbInventoryOnOrbitValCoverOrIntersectQuery() ; // declared but not implmented
};


_ACS_END_NAMESPACE

#endif // _dbInventoryOnOrbitValCoverOrIntersectQuery_H_
