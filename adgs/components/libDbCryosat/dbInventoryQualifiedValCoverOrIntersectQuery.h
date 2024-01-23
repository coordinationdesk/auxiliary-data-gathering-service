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
	
	Revision 5.3  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:26  marfav
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
	
	Revision 2.3  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.1  2008/11/05 18:16:18  marpas
	dbInventoryQualified(Latest)Val(Cover/intersect)Query implemented
	
*/

#ifndef _dbInventoryQualifiedValCoverOrIntersectQuery_H_
#define _dbInventoryQualifiedValCoverOrIntersectQuery_H_

#include <dbInventoryValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryQualifiedValCoverOrIntersectQuery : // PRQA S 2153
	public dbInventoryValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryQualifiedValCoverOrIntersectQuery(dbInventoryQualifiedValCoverOrIntersectQuery const &) ;
	dbInventoryQualifiedValCoverOrIntersectQuery &operator=(dbInventoryQualifiedValCoverOrIntersectQuery const &) ;
	virtual ~dbInventoryQualifiedValCoverOrIntersectQuery() throw() ;

protected:
	dbInventoryQualifiedValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey) ;
private:
	dbInventoryQualifiedValCoverOrIntersectQuery() ; // declared but not implemented

};


_ACS_END_NAMESPACE

#endif // _dbInventoryQualifiedValCoverOrIntersectQuery_H_
