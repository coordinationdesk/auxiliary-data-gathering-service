// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.7  2017/02/23 11:08:03  lucio.pulvirenti
	Offset now considered.
	
	Revision 5.6  2016/04/29 15:59:41  marpas
	getting rid of deprecated methods
	
	Revision 5.5  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.4  2016/03/26 21:45:49  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.3  2016/03/14 13:16:53  marpas
	getting rid of deprecvated classes
	
	Revision 5.2  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/22 17:46:09  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:03  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.10  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.9  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.8  2013/01/22 17:37:06  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.7  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.4  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.3  2011/10/20 13:51:10  lucio.pulvirenti
	Statistics improved
	
	Revision 2.2  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.1  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/08/08 11:48:16  enrcar
	DBEXPRESS handling added
	
	Revision 1.11  2005/06/17 14:14:25  marpas
	std namespace use fixed
	
	Revision 1.10  2004/08/30 17:47:45  marpas
	dbGeneralQueryIF::doQuery method has a new parameter to enable/disable the query.
	The default behaviour should be:
	1) if the incremental is not set, the query should clear the previous result if any
	2) if the enable is false, the query should not be performed and the size of the result should be returned
	
	Because many of the class inheriting this interface do not implement the incremental feature, for the moment a clear of the result and returning 0 is then accepted
	
	Revision 1.9  2004/04/28 13:09:22  paoscu
	Incremental query support.
	
	Revision 1.8  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.7  2004/02/04 10:11:27  paoscu
	dbPersistent interface chanched.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbApplicationStatusQuery.h>
#include <dbApplicationStatus.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbApplicationStatusQuery)

std::string dbApplicationStatusQuery::staticClassName()
{
	return "dbApplicationStatusQuery" ;
}


// constructor
dbApplicationStatusQuery::dbApplicationStatusQuery(const string &dbKey) :  
    dbGeneralQuery(dbKey)
{
}

dbApplicationStatusQuery::dbApplicationStatusQuery(const dbApplicationStatusQuery &as) : dbGeneralQuery(as)
{
}

dbApplicationStatusQuery &dbApplicationStatusQuery::operator=(const dbApplicationStatusQuery &as)
{
	if (this != &as) {
		dbGeneralQuery::operator=(as) ;
	}
	
	return *this ;
}

dbApplicationStatusQuery::~dbApplicationStatusQuery() noexcept {}

dbGeneralQueryIF* dbApplicationStatusQuery::clone() const 
{
	return new dbApplicationStatusQuery(*this) ;
}

unsigned long dbApplicationStatusQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}

	clear() ; // erase previous result set
	if (!enabled) {	return 0 ; }
    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT, true );
    if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }
	if (offset() > 0) {
		queryString.setOffset( offset() ) ; // PRQA S 3000
    }
	

    queryString.addSelectList( dbApplicationStatus::getFields() );
    queryString.addTableList( dbApplicationStatus::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(conn, queryString, dbApplicationStatus::getTable() + " query") ;

	dbSet set ;
	query.execForSet(set) ;

	dbRow row(conn.getKey()) ;
	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding a new ApplicationStatus") ;
		// record = new dbApplicationStatus ;
		ACS_SMARTPTR<dbApplicationStatus> record(dynamic_cast<dbApplicationStatus*>( dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", conn.getKey()))) ; // PRQA S 3081
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
dbApplicationStatus *dbApplicationStatusQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbApplicationStatus *>(item(i)) ; // PRQA S 3081
}

const dbApplicationStatus *dbApplicationStatusQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbApplicationStatus *>(item(i)) ; // PRQA S 3081
}


_ACS_END_NAMESPACE
