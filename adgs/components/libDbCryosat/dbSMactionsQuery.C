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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:03  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:31  marfav
	dbSMactions now include inventory id pointers
	
	Revision 1.2  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 1.1  2008/10/20 09:38:07  crivig
	source code added
	
*/

// This file is automatically generated by dbGenerator tool

#include <dbSMactionsQuery.h>
#include <dbSMactions.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSMactionsQuery)

string dbSMactionsQuery::staticClassName()
{
	return "dbSMactionsQuery" ;
}


// constructor
dbSMactionsQuery::dbSMactionsQuery(const string &dbkey) :
    dbGeneralQuery(dbkey) 
{
}


dbSMactionsQuery::dbSMactionsQuery(dbSMactionsQuery const &other):
    dbGeneralQuery(other.getKey()) 
{
}

dbSMactionsQuery &dbSMactionsQuery::operator=(dbSMactionsQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbSMactionsQuery::clone() const
{
	return new dbSMactionsQuery(*this) ;
}


dbSMactionsQuery::~dbSMactionsQuery() throw() {}




unsigned long dbSMactionsQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}
	
	clear() ; // erase previous result set
	if(!enabled) {
		return 0;
    }

    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );
    if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }

	queryString.addSelectList( dbSMactions::getFields() );
	queryString.addTableList( dbSMactions::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL()  << "\"") ;

	dbQuery query(conn, queryString, "dbSMactionsQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		// record = new dbOrder ;
		ACS_SMARTPTR<dbSMactions> record(dynamic_cast<dbSMactions*>( dbPersistentFactoryChain::instance()->newObject("dbSMactions", conn.getKey()))) ; // PRQA S 3081
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
dbSMactions *dbSMactionsQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbSMactions *>(item(i)) ; // PRQA S 3081
}

const dbSMactions *dbSMactionsQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbSMactions *>(item(i)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
