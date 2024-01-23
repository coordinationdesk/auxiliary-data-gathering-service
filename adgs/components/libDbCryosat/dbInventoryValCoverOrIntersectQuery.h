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
	
	Revision 5.5  2016/07/04 14:39:41  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.4  2016/04/15 10:13:44  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.3  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
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
	
	Revision 2.6  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.5  2008/12/17 14:41:29  paoscu
	Attributes are now protected.
	
	Revision 2.4  2008/12/03 18:08:41  paoscu
	Added incremental support
	Improved additional parameters management
	
	Revision 2.3  2008/12/02 16:56:49  paoscu
	Added extra condition
	
	Revision 2.2  2008/12/01 20:06:38  paoscu
	Join clause added
	
	Revision 2.1  2008/11/04 17:03:03  paoscu
	Added class to dbInventoryValCoverOrIntersectQuery to be inherited by other special queries
	
	
		

*/

#ifndef _dbInventoryValCoverOrIntersectQuery_H_
#define _dbInventoryValCoverOrIntersectQuery_H_

#include <dbInventoryObjectSpecialQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class DateTime;
class dbConnection;

class dbInventoryValCoverOrIntersectQuery : public dbInventoryObjectSpecialQuery // PRQA S 2109, 2153
{
public:
	friend class dbCryosatFactory ; // PRQA S 2107

	dbInventoryValCoverOrIntersectQuery(dbInventoryValCoverOrIntersectQuery const &) ;
	dbInventoryValCoverOrIntersectQuery &operator=(dbInventoryValCoverOrIntersectQuery const &) ;

	virtual dbGeneralQueryIF *clone() const ;	// PRQA S 2502


	static  std::string staticClassName() ;

	virtual ~dbInventoryValCoverOrIntersectQuery() throw() ;

    virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	// returns a suitable trigger type for waiting orders it should return "INTERSECT" or  "COVER"
	virtual std::string getTriggerType() const ;

	//virtual void setJoinClause ( std::string const & ) ;

	virtual void setJoinTable(const std::string & table) ;
	virtual void setJoinToken(const SQLDialect::SQLJoinToken& ) ;
	virtual void setQueryJoinParam(const dbQueryParameters & );

	virtual void setExtraCondition ( std::string const & ) ;

	virtual void resetParameters() ; 
	virtual const std::string &getParameters() const ;



protected:
	dbInventoryValCoverOrIntersectQuery(bool latest, bool intersect, const std::string &dbKey);

private:
    dbInventoryValCoverOrIntersectQuery() ; // not implemented

protected:
	bool _latest;// PRQA S 2101 5
	bool _intersect ;

	SQLDialect::SQLJoinToken _joinToken;
 	std::string _jtable;
	dbQueryParameters _qparam;

	std::string _extraCondition ;
	mutable std::string _tempParameters ;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryValCoverOrIntersectQuery)

};


_ACS_END_NAMESPACE

#endif // _dbInventoryValCoverOrIntersectQuery_H_
