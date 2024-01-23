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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2017/02/23 11:08:03  lucio.pulvirenti
	Offset now considered.
	
	Revision 5.5  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.4  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.3  2016/03/14 13:16:53  marpas
	getting rid of deprecvated classes
	
	Revision 5.2  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.14  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.13  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.12  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.11  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.10  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.7  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.6  2011/10/20 13:51:10  lucio.pulvirenti
	Statistics improved
	
	Revision 2.5  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.4  2009/06/19 18:36:29  marpas
	still working on dbConnection speedup
	
	Revision 2.3  2009/06/19 18:11:35  marpas
	dbConnection used to speed up work and to avoid issuing multiple connections and queries when it is useless
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2008/04/28 21:49:26  paoscu
	Added dbSmartAggregateQuery
	
	
	
	
*/



#include <dbSmartAggregateQuery.h>

#include <dbSmartAggregatePersistent.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSmartAggregateQuery)

string dbSmartAggregateQuery::staticClassName()
{
	return "dbSmartAggregateQuery" ;
}



// constructor
dbSmartAggregateQuery::dbSmartAggregateQuery(string const & parentTable , 
                                             vector<string> const & childTables,
                                             const string &dbKey ) :
    dbGeneralQuery(dbKey),
	_parentTable(parentTable),
    _childTables(childTables)
{
}

dbSmartAggregateQuery::dbSmartAggregateQuery(dbSmartAggregateQuery const &r) :
    dbGeneralQuery(r),
	_parentTable(r._parentTable),
    _childTables(r._childTables)
{
}

dbSmartAggregateQuery &dbSmartAggregateQuery::operator=(dbSmartAggregateQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
		_parentTable = other._parentTable ;
		_childTables = other._childTables ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbSmartAggregateQuery::clone() const
{
	return new dbSmartAggregateQuery(*this) ;
}


dbSmartAggregateQuery::~dbSmartAggregateQuery() throw() {}


unsigned long dbSmartAggregateQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}
	

	clear() ; // erase previous result set
	if(!enabled) { return 0; }

    const string &dbKey = conn.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT, true) ;
	dbSmartAggregatePersistent helper(_parentTable, _childTables, conn) ;
	helper.getSelectSQLString(queryString, conn) ;

	if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }
	if (offset() > 0) {
		queryString.setOffset( offset() ) ; // PRQA S 3000
    }


	if ( not getParameters().empty() ) {
		queryString.addANDWhereParameter( getParameters() ) ;
    }

	if ( not getOrderBySqlClause().empty()) { 
		queryString.setOrderByString( getOrderBySqlClause() ) ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(conn, queryString, _parentTable + " smart agg. query " ) ;

	dbSet set ;
	query.execForSet(set,helper.getColumnTypes(conn) ) ;

	dbRow row(dbKey) ;
	for (size_t i=0; set.getNextRow(row); i++)  // PRQA S 4238
	{
		ACS_SMARTPTR<dbSmartAggregatePersistent> record(new dbSmartAggregatePersistent(_parentTable,  _childTables, conn)) ;
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new record") ;

	}
	return size() ;
}



dbSmartAggregatePersistent *dbSmartAggregateQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbSmartAggregatePersistent *>(item(i)) ; // PRQA S 3081
}

const dbSmartAggregatePersistent *dbSmartAggregateQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbSmartAggregatePersistent *>(item(i)) ; // PRQA S 3081
}





_ACS_END_NAMESPACE
