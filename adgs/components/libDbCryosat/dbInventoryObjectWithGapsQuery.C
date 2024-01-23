// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	
	Revision 5.5  2016/07/04 15:12:18  damdec
	CSGACQ-82: new getFields interface adopted.
	
	Revision 5.4  2016/04/15 10:13:44  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/14 16:57:28  marpas
	honor dbGeneralQuery::offset(...)
	coding best practices applied
	
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
	
	Revision 2.2  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/04 16:13:30  enrcar
	Updated to DBEXPRESS
	
	Revision 1.2  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.1  2005/06/20 10:36:11  marpas
	gaps management added
	
		

*/


#include <dbInventoryObjectWithGapsQuery.h>
#include <dbInventoryObjectWithGaps.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryObjectWithGapsQuery)


string dbInventoryObjectWithGapsQuery::staticClassName()
{
	return "dbInventoryObjectWithGapsQuery" ;
}


// constructor
dbInventoryObjectWithGapsQuery::dbInventoryObjectWithGapsQuery(const string &dbkey) :
    dbGeneralQuery(dbkey) 
{
}


dbInventoryObjectWithGapsQuery::dbInventoryObjectWithGapsQuery(dbInventoryObjectWithGapsQuery const &other) :
    dbGeneralQuery(other.getKey()) 
{
	*this=other ;
}

dbInventoryObjectWithGapsQuery &dbInventoryObjectWithGapsQuery::operator=(dbInventoryObjectWithGapsQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryObjectWithGapsQuery::clone() const
{
	return new dbInventoryObjectWithGapsQuery(*this) ;
}


dbInventoryObjectWithGapsQuery::~dbInventoryObjectWithGapsQuery() throw() {}


unsigned long dbInventoryObjectWithGapsQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	ACS_COND_THROW(incremental,InvalidUse("Incremental query not implemented.")) ; // PRQA S 3081
	
	clear() ; // erase previous result set
	if(!enabled) {
		return 0;
    }

    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );
    if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
        if (offset() > 0) {
 		    queryString.setOffset( offset() ) ; // PRQA S 3000
        }
    }
	
	const string &dbKey=conn.getKey();
	deque<string> fields = dbInventoryObjectWithGaps::getFieldArray(conn);
	for (uint i = 0; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] );
	}
    queryString.addTableList( dbInventoryObjectWithGaps::getTable() );

// where parameters and orderby have been copied from _sqlString above 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL()) ;

	dbQuery query(conn, queryString, "dbInventoryObjectWithGapsQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); ++i)  // PRQA S 4238
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryObject") ;
		// record = new dbInventoryObjectWithGaps ;
		ACS_SMARTPTR<dbInventoryObjectWithGaps> record(dynamic_cast<dbInventoryObjectWithGaps*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObjectWithGaps", conn.getKey()))) ; // PRQA S 3050, 3081
		record->loadRow(conn, row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}

dbInventoryObjectWithGaps *dbInventoryObjectWithGapsQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbInventoryObjectWithGaps *>(item(i)) ; // PRQA S 3081
}

const dbInventoryObjectWithGaps *dbInventoryObjectWithGapsQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbInventoryObjectWithGaps *>(item(i)) ;// PRQA S 3081
}

_ACS_END_NAMESPACE
