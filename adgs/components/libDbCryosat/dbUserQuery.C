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
	Revision 6.0  2016/07/07 13:44:05  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.5  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.4  2016/04/15 14:37:04  marpas
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
	
	Revision 4.0  2013/09/30 12:31:04  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:32  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:51  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/09/23 08:27:38  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.24  2005/12/23 12:37:13  marpas
	dbexpress introduced for DB2 compatibility
	
	Revision 1.23  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.22  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.21  2004/09/22 14:58:39  ivafam
	If in _orderByString clause "ORDER BY" is present do not add clause
	
	Revision 1.20  2004/09/22 13:20:07  ivafam
	Bug Fixed in do query: Now the SQLCLAUSE "ORDER BY" is added if _orderByString
	is not empty.
	
	Revision 1.19  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.18  2004/04/28 13:08:41  paoscu
	Incremental query support.
	
	Revision 1.17  2004/04/13 16:52:56  paoscu
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.16  2004/02/04 10:12:36  paoscu
	Library generated again. dbPersistent interface changed.
		

*/

// This file is automatically generated by dbGenerator tool

#include <dbUserQuery.h>
#include <dbUser.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


using namespace acs ;
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbUserQuery)

string dbUserQuery::staticClassName()
{
	return "dbUserQuery" ;
}


// constructor
dbUserQuery::dbUserQuery(const string &dbkey) :
    dbGeneralQuery(dbkey) 
{
}


dbUserQuery::dbUserQuery(dbUserQuery const &other) :
    dbGeneralQuery(other.getKey()) 
{
	*this=other ;
}

dbUserQuery &dbUserQuery::operator=(dbUserQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ;
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbUserQuery::clone() const
{
	return new dbUserQuery(*this) ;
}


dbUserQuery::~dbUserQuery() throw() {}


unsigned long dbUserQuery::doQuery(dbConnection &conn, bool incremental, bool enabled)
{
	if(incremental)
	{
		InvalidUse e("Incremental query not allowed.") ;
		ACS_THROW(e) ;
	}
	

	clear() ; // erase previous result set
	if(!enabled)
		return 0;

    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );
    if (maxItems() > 0)
		queryString.setLimit( maxItems() ) ;

    queryString.addSelectList( dbUser::getFields() );
    queryString.addTableList( dbUser::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(conn, queryString, "dbUserQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) 
	{
		dbUser *record ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new User : ") ;
        
		// record = new dbUser ;
		record=dynamic_cast<dbUser*>( dbPersistentFactoryChain::instance()->newObject("dbUser", conn.getKey())) ;
		try{
			record->loadRow(row,0) ;
		}
		catch(...){
			delete record;
			throw;
		}
		_objects.push_back(record) ;
	}

	return size() ;
}
dbUser *dbUserQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbUser *>(item(i)) ;
}

const dbUser *dbUserQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbUser *>(item(i)) ;
}

