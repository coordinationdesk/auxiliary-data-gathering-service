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
	Revision 6.2  2016/10/07 12:39:47  marpas
	compilation fixed
	
	Revision 6.1  2016/10/06 17:01:58  marpas
	implementing VEN-2163 on HEAD - ready to be reported on venus branch - test to be fixed
	
	

*/

#include <dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery.h>
#include <QualifiedFlag.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery::dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryOnOrbitValCoverOrIntersectQuery(latest , intersect, dbKey)
{

}


dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery::dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery(dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery const &r)
: dbInventoryOnOrbitValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery &dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery::operator=(dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryOnOrbitValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery::~dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery() throw() 
{
}


unsigned long dbInventoryOnOrbitQualifiedValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{
    const string qdesc = "QUALIFIED_PRODUCT" ;
    try {
        const string &dbKey = c.getKey() ;
        int qflag = QualifiedFlag::instance(dbKey)->id(qdesc) ;

	    dbQueryParameters par(dbKey) ;
	    par.addParameter( dbParam(dbKey, "t_inventory","qualified" , dbParam::EQUAL_TO , db::toSQL(dbKey, qflag)) ) ;
	    addParameter(par) ; // remember I'm a query, so I can have parameters  
    }
    catch(exception &x ) {
        ACS_THROW(exIllegalValueException(x,"Cannot find id for " + qdesc ) ) ; // PRQA S 3081
    }
    
    return dbInventoryOnOrbitValCoverOrIntersectQuery::doQuery( c, incremental, enabled ) ;
}


_ACS_END_NAMESPACE
