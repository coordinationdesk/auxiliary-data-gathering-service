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


	$Prod: A.C.S. Packager Optimiser Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2016/03/26 21:36:45  marpas
	using new db I/F
	
	Revision 2.7  2014/06/25 16:24:12  marpas
	coding best practices applied
	
	Revision 2.6  2014/03/10 15:00:33  francesco.avanzi
	better code practices applied
	
	Revision 2.5  2013/04/17 17:13:23  marpas
	new dbPersistent interface
	
	Revision 2.4  2012/02/16 18:18:21  marpas
	refactoring in progress
	
	Revision 2.3  2009/07/13 13:11:17  enrcar
	EC:: c++rules
	
	Revision 2.2  2009/03/24 14:33:14  enrcar
	EC:: c++rules
	
	Revision 2.1  2008/10/08 15:08:29  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 10:05:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/22 13:15:14  enrcar
	Interface updated
	
	Revision 1.4  2005/07/19 16:12:02  enrcar
	DBEXPRESS handled
	
	Revision 1.3  2005/06/20 15:23:39  enrcar
	Added handling for namespace std
	
	Revision 1.2  2004/09/14 16:42:22  marpas
	query i/f changed now its direct base is dbGeneralQuery
	
	Revision 1.1  2004/09/14 16:28:37  marpas
	object extended to limit query number
	


*/


#include <dbDistributionItemExtendedQuery.h>
#include <dbDistributionItemExtended.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>
#include <dbQueryParameters.h>

#include <SQLString.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDistributionItemExtendedQuery)

string dbDistributionItemExtendedQuery::staticClassName()
{
	return "dbDistributionItemExtendedQuery" ;
}


// constructor
dbDistributionItemExtendedQuery::dbDistributionItemExtendedQuery(const string &dbKey) :
    dbGeneralQuery(dbKey)
{
}


dbDistributionItemExtendedQuery::dbDistributionItemExtendedQuery(dbDistributionItemExtendedQuery const &r) : dbGeneralQuery(r)
{
}

dbDistributionItemExtendedQuery &dbDistributionItemExtendedQuery::operator=(dbDistributionItemExtendedQuery const &r)
{
	if(this != &r)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(r) ;
		
		dbGeneralQuery::operator=(r) ;	// call the base class =operator
	}
	
	return *this ;
}


dbGeneralQueryIF * dbDistributionItemExtendedQuery::clone() const
{
	return new dbDistributionItemExtendedQuery(*this) ;
}


dbDistributionItemExtendedQuery::~dbDistributionItemExtendedQuery() throw() 
{
}


unsigned long dbDistributionItemExtendedQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
#warning incremental mode not implemented

	// EnrCar::
	// W8115_OFF
	// This method has multiple returns in order to keep its complexity
	// as low as possible


	if(incremental)
	{
		// warning incremental mode not implemented
		
		ACS_THROW(exNotImplemented("not implemented") ) ; // PRQA S 3081
	}
	
	clear() ; // erase previous result set
	if(!enabled) {
		return 0;
    }


    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );	// prepare a SELECT query
    if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }

    queryString.addSelectList( dbDistributionItemExtended::getFields() );
    queryString.addTableList( dbDistributionItemExtended::getTable() );

// where parameters and orderby have been copied from _sqlString above 
    queryString.addANDWhereParameter( dbDistributionItemExtended::whereJoin(conn) ) ;


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() );


    dbQuery query(conn, queryString, "bDistributionItemExtendedQuery::doQuery") ;	// define query

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;	// execute query

	for (size_t i=0; set.getNextRow(row); ++i) // PRQA S 4238
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new DistributionItemExtended : " );
		// record = new dbDistributionItemExtended ;
		dbDistributionItemExtended *record=dynamic_cast<dbDistributionItemExtended*>( dbPersistentFactoryChain::instance()->newObject("dbDistributionItemExtended", conn.getKey())) ; // PRQA S 3081, 4412
		try{
			record->loadRow(row,0) ;
		}
		catch(...){
			// exception got 
			delete record;
			throw;
		}
		_objects.push_back(record) ;	// save results
	}

	return size() ;	// number of items 

}

dbDistributionItemExtended *dbDistributionItemExtendedQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbDistributionItemExtended *>(item(i)) ; // PRQA S 3081
}

const dbDistributionItemExtended *dbDistributionItemExtendedQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbDistributionItemExtended *>(item(i)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
