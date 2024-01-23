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
	
	Revision 5.2  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.1  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.1  2013/09/26 19:32:51  marpas
	qa rules
	coding best practices
	compilation warnings removed
	using libException 5.x interface
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2010/11/03 14:35:37  andcol
	bug fixed
	
	Revision 2.1  2010/10/27 15:30:49  chicas
	added dbInventoryNamedValCoverOrIntersectQuery
	


*/

#include <dbInventoryNamedValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <XMLIstringstream.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryNamedValCoverOrIntersectQuery::dbInventoryNamedValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryValCoverOrIntersectQuery(latest , intersect, dbKey)
{
}


dbInventoryNamedValCoverOrIntersectQuery::dbInventoryNamedValCoverOrIntersectQuery(dbInventoryNamedValCoverOrIntersectQuery const &r)
: dbInventoryValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryNamedValCoverOrIntersectQuery &dbInventoryNamedValCoverOrIntersectQuery::operator=(dbInventoryNamedValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryNamedValCoverOrIntersectQuery::~dbInventoryNamedValCoverOrIntersectQuery() throw() {}

unsigned long dbInventoryNamedValCoverOrIntersectQuery::doQuery(dbConnection &c,bool incremental,bool enabled)
{
	if( getParametersCLOB().empty() )
	{
		ACS_THROW(exIllegalValueException( "Empty CLOB parameters." ) ) ; // PRQA S 3081
	}
	
	
	XMLIstringstream clobStream (  getParametersCLOB ()  ) ;
	rsResourceSet clobParameters;
	clobStream >> clobParameters ;
	string radixOfName ;
	clobParameters.getValue( "FileNameRadix" , radixOfName ) ;
	
	radixOfName+="%";
	dbQueryParameters par(getKey()) ;
	par.addParameter( dbParam(getKey(), "t_inventory","filename" , dbParam::LIKE , db::toSQL(getKey(), radixOfName))) ;
	
	addParameter(par) ; // remember I'm a query, so I can have parameters  

	return dbInventoryValCoverOrIntersectQuery::doQuery ( c, incremental, enabled);
}

_ACS_END_NAMESPACE
