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
	
	Revision 5.6  2016/07/04 14:52:46  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.5  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.4  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2015/03/17 13:59:29  marfav
	APF-254 Added join clause due to APF special queries
	
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
	
	Revision 2.3  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/05/13 15:12:13  marpas
	rules fixing and comments makeup
	
	Revision 2.1  2008/03/21 18:09:16  paoscu
	dbInventoryValFullIntersectQuery added
	

*/

#ifndef _dbInventoryValFullIntersectQuery_H_
#define _dbInventoryValFullIntersectQuery_H_

#include <dbInventoryObjectSpecialQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class dbInventoryValFullIntersectQuery : // PRQA S 2109, 2153
	public dbInventoryObjectSpecialQuery
{
	friend class dbCryosatFactory ; // PRQA S  2107
public:

	dbInventoryValFullIntersectQuery(dbInventoryValFullIntersectQuery const &) ;
	dbInventoryValFullIntersectQuery &operator=(dbInventoryValFullIntersectQuery const &) ;

	virtual dbGeneralQueryIF *clone() const ;	// PRQA S 2502


	static  std::string staticClassName() ;

	virtual ~dbInventoryValFullIntersectQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual void setJoinTable(const std::string & table) ;
	virtual void setJoinToken(const SQLDialect::SQLJoinToken& ) ;
	virtual void setQueryJoinParam(const dbQueryParameters & );

	// returns a suitable trigger type for waiting orders it should return "INTERSECT"
	virtual std::string getTriggerType() const ;

protected:
	dbInventoryValFullIntersectQuery(bool latest, const std::string &dbKey);

private:
	dbInventoryValFullIntersectQuery() ; // declared but not implemented

protected:
	bool _latest; // PRQA S 2101 2
	
	SQLDialect::SQLJoinToken _joinToken;
 	std::string _jtable;
	dbQueryParameters _qparam;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryValFullIntersectQuery) ;

};


_ACS_END_NAMESPACE

#endif // _dbInventoryValFullIntersectQuery_H_
