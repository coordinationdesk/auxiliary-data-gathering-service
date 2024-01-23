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
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2011/09/28 13:29:40  lucia.galli
	New special query added
	
	
	
*/

#ifndef _dbInventoryMultiBandValCoverOrIntersectQuery_H_
#define _dbInventoryMultiBandValCoverOrIntersectQuery_H_

#include <dbInventoryValCoverOrIntersectQuery.h>


namespace acs {

class DateTime;
class dbConnection;

class dbInventoryMultiBandValCoverOrIntersectQuery : // PRQA S 2153
	public dbInventoryValCoverOrIntersectQuery
{
public:
	friend class dbCryosatFactory ; // PRQA S 2107

	dbInventoryMultiBandValCoverOrIntersectQuery(dbInventoryMultiBandValCoverOrIntersectQuery const &) ;
	dbInventoryMultiBandValCoverOrIntersectQuery &operator=(dbInventoryMultiBandValCoverOrIntersectQuery const &) ;
	virtual ~dbInventoryMultiBandValCoverOrIntersectQuery() throw() ;
	
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual bool doesOverlap() const;

protected:
	dbInventoryMultiBandValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey) ;
private:
	dbInventoryMultiBandValCoverOrIntersectQuery() ; // declared but not implemented
};


} // namespace

#endif // _dbInventoryMultiBandValCoverOrIntersectQuery_H_
