// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	
	Revision 3.1  2013/09/26 19:32:52  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.0  2013/09/19 14:57:27  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.3  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/04/28 15:21:39  marpas
	OCADA ifdef: QUALIFIED_PRODUCT or'ed with VALID_PRODUCT
	
	Revision 2.1  2008/11/05 18:16:18  marpas
	dbInventoryQualified(Latest)Val(Cover/intersect)Query implemented
	

*/

#include <dbInventoryQualifiedValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <QualifiedFlag.h>


#include <sstream>


using namespace acs;
using namespace std;


dbInventoryQualifiedValCoverOrIntersectQuery::dbInventoryQualifiedValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryValCoverOrIntersectQuery(latest , intersect, dbKey)
{
	dbQueryParameters par(getKey()) ;
	par.addParameter( dbParam(getKey(),"t_inventory","qualified" , dbParam::EQUAL_TO , db::toSQL(getKey(),QualifiedFlag::instance("")->id("VALID_PRODUCT")))) ;
#if defined(OCADA) 
	par.setParamOperator(dbQueryParameters::OR) ;
	par.addParameter( dbParam(getKey(),"t_inventory","qualified" , dbParam::EQUAL_TO , db::toSQL(getKey(),QualifiedFlag::instance("")->id("QUALIFIED_PRODUCT")))) ;
#endif
	dbInventoryValCoverOrIntersectQuery::addParameter(par) ; // remember I'm a query, so I can have parameters  
}


dbInventoryQualifiedValCoverOrIntersectQuery::dbInventoryQualifiedValCoverOrIntersectQuery(dbInventoryQualifiedValCoverOrIntersectQuery const &r)
: dbInventoryValCoverOrIntersectQuery(r) 
{
}

dbInventoryQualifiedValCoverOrIntersectQuery &dbInventoryQualifiedValCoverOrIntersectQuery::operator=(dbInventoryQualifiedValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryQualifiedValCoverOrIntersectQuery::~dbInventoryQualifiedValCoverOrIntersectQuery() throw() 
{
}

