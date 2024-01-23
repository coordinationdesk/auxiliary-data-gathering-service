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
	
	Revision 5.2  2016/04/18 15:22:52  marpas
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
	
	Revision 2.4  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.3  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/01/25 09:08:05  marfav
	Query derives from OnSite family
	
	Revision 2.1  2008/12/02 16:57:29  paoscu
	dbInventoryOnOrbitValCoverOrIntersectQuery added
	
	

*/

#include <dbInventoryOnOrbitValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <XMLIstringstream.h>


#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryOnOrbitValCoverOrIntersectQuery::dbInventoryOnOrbitValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryOnSiteValCoverOrIntersectQuery(latest , intersect, dbKey)
{

}


dbInventoryOnOrbitValCoverOrIntersectQuery::dbInventoryOnOrbitValCoverOrIntersectQuery(dbInventoryOnOrbitValCoverOrIntersectQuery const &r)
: dbInventoryOnSiteValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryOnOrbitValCoverOrIntersectQuery &dbInventoryOnOrbitValCoverOrIntersectQuery::operator=(dbInventoryOnOrbitValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryOnSiteValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryOnOrbitValCoverOrIntersectQuery::~dbInventoryOnOrbitValCoverOrIntersectQuery() throw() 
{
}


unsigned long dbInventoryOnOrbitValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{

	//Example of parameters clob
	/*
	<?xml version ="1.0" encoding="UTF-8"?>
	<Parameter_File>
        	<AbsolutOrbitNumber>1</AbsolutOrbitNumber>
        	<SiteId>Site2</SiteId>
        	<Extended>f</Extended>
        	<MNT_Option>questo ? un dem</MNT_Option>
	</Parameter_File>
	*/
	
	
	if( getParametersCLOB().empty() )
	{
		ACS_THROW(exIllegalValueException( "Empty CLOB parameters." ) ) ; // PRQA S 3081
	}
	
	
	XMLIstringstream clobStream (  getParametersCLOB ()  ) ;
	rsResourceSet clobParameters;
	clobStream >> clobParameters ;
	int absolutOrbitNumber = 0 ;
	clobParameters.getValue( "AbsolutOrbitNumber" , absolutOrbitNumber ) ;
	
	//excerr << "dbInventoryOnOrbitValCoverOrIntersectQuery::doQuery " << absolutOrbitNumber ;
	

	dbQueryParameters par(c.getKey()) ;
	par.addParameter( dbParam(c.getKey(), "t_inventory","startorbitnumber" , dbParam::MINOR_OR_EQUAL_OF , db::toSQL(c.getKey(), absolutOrbitNumber)) ) ;
	par.addParameter( dbParam(c.getKey(), "t_inventory","stoporbitnumber" ,  dbParam::MAJOR_OR_EQUAL_OF , db::toSQL(c.getKey(), absolutOrbitNumber)) ) ;
	addParameter(par) ; // remember I'm a query, so I can have parameters  

	// The site coverage is managed by the the superclass OnSiteQuery

	//setExtraCondition( " NOT EXISTS ( SELECT t_invsiterelation.site "
	//            	   " FROM t_invsiterelation  "
	//				   " WHERE t_invsiterelation.filename    = t_inventory.filename     "
	//				   " AND   t_invsiterelation.fileversion = t_inventory.fileversion )" )  ;
	
	return dbInventoryOnSiteValCoverOrIntersectQuery::doQuery( c, incremental, enabled ) ;
}


_ACS_END_NAMESPACE
