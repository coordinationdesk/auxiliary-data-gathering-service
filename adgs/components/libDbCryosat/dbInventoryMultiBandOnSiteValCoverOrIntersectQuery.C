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
	
	Revision 5.4  2016/07/04 15:02:59  damdec
	CSGACQ-82: code aligned to the base class changes.
	
	Revision 5.3  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
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
	
	Revision 3.0  2013/09/19 14:57:25  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.9  2013/04/30 13:31:42  marpas
	dbPersistent interface changed
	
	Revision 2.8  2013/04/17 17:37:24  marpas
	dbPersistent interface changed
	
	Revision 2.7  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.6  2012/11/19 22:51:15  marpas
	compile with PDS_OLD_SCHEMA to use old pds schema.
	dbReceptionRule and related Query class introduced
	
	Revision 2.5  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2009/02/04 15:48:23  marfav
	Addedd doesOverlap method
	
	Revision 2.3  2008/12/16 18:48:43  paoscu
	 Implemented dbInventoryTDIMultiBandOnSiteValCoverOrIntersectQuery
	
	Revision 2.2  2008/12/16 15:59:33  marpas
	 work in progress
	
	Revision 2.1  2008/12/03 18:09:03  paoscu
	dbInventoryMultiBandOnSiteValCoverOrIntersectQuery Added
	
	Revision 2.1  2008/12/01 20:07:05  paoscu
	Added dbInventoryMultiBandOnSiteValCoverOrIntersectQuery
	
	

*/

#include <dbInventoryMultiBandOnSiteValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>
#include <dbSmartQuery.h>
#include <dbSmartPersistent.h>

#include <XMLIstringstream.h>


#include <sstream>


using namespace acs;
using namespace std;


dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey)
: dbInventoryOnSiteValCoverOrIntersectQuery(latest , intersect, dbKey)
{

}


dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::dbInventoryMultiBandOnSiteValCoverOrIntersectQuery(dbInventoryMultiBandOnSiteValCoverOrIntersectQuery const &r)
: dbInventoryOnSiteValCoverOrIntersectQuery(r) 
{
}

dbInventoryMultiBandOnSiteValCoverOrIntersectQuery &dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::operator=(dbInventoryMultiBandOnSiteValCoverOrIntersectQuery const &r)
{
    if (this != &r) {
	    dbInventoryOnSiteValCoverOrIntersectQuery:: operator= (r );
    }
	return *this ;
}

dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::~dbInventoryMultiBandOnSiteValCoverOrIntersectQuery() throw()
{
}


unsigned long dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{
	if( ! incremental) {
		clear() ;
    }

	SQLString originalQuery = _sqlString;

	try
	{
		dbSmartQuery bandQuery("t_bands", c.getKey()) ;
		bandQuery.doQuery(c) ;
		
		size_t storedRecords = 0 ;

        size_t bq_size = bandQuery.size() ;
		for(size_t b=0 ; b<bq_size ; ++b)
		{
			unsigned int bandId = bandQuery.at(b)->getId() ;
			_sqlString = originalQuery;
			
			dbQueryParameters par(c.getKey()) ;
			par.addParameter( dbParam(c.getKey(), "t_inventory","band" , dbParam::EQUAL_TO , db::toSQL(c.getKey(), bandId)) ) ;
			addParameter(par) ;
			
			dbInventoryOnSiteValCoverOrIntersectQuery::doQuery( c, true, enabled ) ; //force incremental
			
			if( size() == storedRecords) // no files for this band
			{
				clear() ;
				break ;
			}
			storedRecords =  size();
		
		}
		
	}
	catch(...)
	{
		_sqlString = originalQuery;
		throw ;
	}

	_sqlString = originalQuery;

	return size() ;

}

bool dbInventoryMultiBandOnSiteValCoverOrIntersectQuery::doesOverlap() const
{
	// The result of this query has not to be filtered with overlap strategies
	return false;
}
