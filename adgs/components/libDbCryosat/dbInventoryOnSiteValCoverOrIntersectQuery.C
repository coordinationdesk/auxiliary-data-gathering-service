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
	
	Revision 5.4  2016/07/04 14:44:27  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
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
	
	Revision 2.5  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.4  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2009/01/29 13:22:47  ivafam
	SiteId --> SiteID
	
	Revision 2.2  2009/01/25 09:05:08  marfav
	Addedd null site coverage clause
	
	Revision 2.1  2008/12/01 20:07:05  paoscu
	Added dbInventoryOnSiteValCoverOrIntersectQuery
	
	

*/

#include <dbInventoryOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <XMLIstringstream.h>
#include <dbConnectionPool.h>
#include <SiteName.h>


#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryOnSiteValCoverOrIntersectQuery::dbInventoryOnSiteValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryValCoverOrIntersectQuery(latest , intersect, dbKey)
{

}


dbInventoryOnSiteValCoverOrIntersectQuery::dbInventoryOnSiteValCoverOrIntersectQuery(dbInventoryOnSiteValCoverOrIntersectQuery const &r)
: dbInventoryValCoverOrIntersectQuery(r) // these will be override by assignement
{
}

dbInventoryOnSiteValCoverOrIntersectQuery &dbInventoryOnSiteValCoverOrIntersectQuery::operator=(dbInventoryOnSiteValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryOnSiteValCoverOrIntersectQuery::~dbInventoryOnSiteValCoverOrIntersectQuery() throw() {}


class SQLStringChangeGuard
{
public:
	SQLStringChangeGuard (SQLString& s) :
		_currentString (s),
		_oldString (s)
	{}

	virtual ~SQLStringChangeGuard()
	{
		_currentString = _oldString;
	}

private:
	SQLString& _currentString;
	SQLString _oldString;
};

unsigned long dbInventoryOnSiteValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{

	SQLStringChangeGuard myGuard (_sqlString);

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
	string siteNameString ;
	clobParameters.getValue( "SiteID" , siteNameString ) ;
	
	SiteName *siteName = SiteName::instance(c.getKey()) ;
	unsigned int siteId = siteName -> id(siteNameString) ;

	setJoinTable("t_invsiterelation") ;
	setJoinToken(SQLDialect::LEFT ) ;
	dbQueryParameters pp(c.getKey());
	pp.addParameter( dbParam(c.getKey(), "t_invsiterelation","filename" , dbParam::EQUAL_TO , "t_inventory.filename"))  ;
	pp.addParameter( dbParam(c.getKey(), "t_invsiterelation","fileversion" , dbParam::EQUAL_TO , "t_inventory.fileversion") ) ;
	setQueryJoinParam(pp);
	
	// the query will report all the files covering the given siteId
	// or not associated to any site that means the file covers all the sites 	

	dbQueryParameters par(c.getKey()) ;
	par.addParameter( dbParam(c.getKey(), "t_invsiterelation","site" , dbParam::EQUAL_TO , db::toSQL(c.getKey(), siteId)) ) ;
	par.addParameter( dbParam(c.getKey(), "t_invsiterelation","site" , dbParam::ISNULL));
	par.setParamOperator (dbQueryParameters::OR);
	addParameter(par) ; // remember I'm a query, so I can have parameters  
	
	return dbInventoryValCoverOrIntersectQuery::doQuery( c, incremental, enabled ) ;
}


_ACS_END_NAMESPACE
