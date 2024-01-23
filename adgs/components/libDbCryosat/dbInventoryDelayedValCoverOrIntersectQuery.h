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
	Revision 6.1  2017/02/02 16:07:36  marfav
	CSGACQ-110
	Explicit declaration of expiration dates in case of waiting order insertion
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:57  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2010/03/23 17:38:46  marfav
	String fixed
	
	Revision 2.3  2010/03/23 17:36:28  marfav
	Added DELAY trigger type
	
	Revision 2.2  2010/03/23 17:29:04  marfav
	Added expiration date computation from available inventory data
	
	Revision 2.1  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
*/

#ifndef _dbInventoryDelayedValCoverOrIntersectQuery_H_
#define _dbInventoryDelayedValCoverOrIntersectQuery_H_

#include <dbInventoryValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryDelayedValCoverOrIntersectQuery : // PRQA S 2109, 2153
	public dbInventoryValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryDelayedValCoverOrIntersectQuery(dbInventoryDelayedValCoverOrIntersectQuery const &) ;
	dbInventoryDelayedValCoverOrIntersectQuery &operator=(dbInventoryDelayedValCoverOrIntersectQuery const &) ;
	virtual ~dbInventoryDelayedValCoverOrIntersectQuery() throw() ;
	
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true) ;
	
	// Inherited interface
	virtual bool hasExpirationDate() const;
	virtual DateTime getComputedExpirationDate() const;
	
	virtual std::string getTriggerType() const;

protected:
	dbInventoryDelayedValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey) ;
private:
	dbInventoryDelayedValCoverOrIntersectQuery() ; // declared but not implemented
	
private:
	// attributes
	bool _somethingFound;
	DateTime _oldestDate;	

};


_ACS_END_NAMESPACE

#endif // _dbInventoryDelayedValCoverOrIntersectQuery_H_
