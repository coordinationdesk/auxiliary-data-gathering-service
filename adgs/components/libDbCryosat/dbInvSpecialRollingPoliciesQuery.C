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
	
	Revision 5.5  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.4  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:56  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2006/01/04 16:18:25  enrcar
	Updated to DBEXPRESS
	
	Revision 1.5  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.4  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.3  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
	Revision 1.2  2004/09/22 13:20:07  ivafam
	Bug Fixed in do query: Now the SQLCLAUSE "ORDER BY" is added if _orderByString
	is not empty.
	
	Revision 1.1  2004/09/06 12:53:04  ivafam
	Class added
	
	
*/

#include <dbInvSpecialRollingPoliciesQuery.h>
#include <dbInvSpecialRollingPolicies.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInvSpecialRollingPoliciesQuery)

string dbInvSpecialRollingPoliciesQuery::staticClassName()
{
	return "dbInvSpecialRollingPoliciesQuery" ;
}


// constructor
dbInvSpecialRollingPoliciesQuery::dbInvSpecialRollingPoliciesQuery(const string &dbkey) :
    dbGeneralQuery(dbkey)
{
}


dbInvSpecialRollingPoliciesQuery::dbInvSpecialRollingPoliciesQuery(dbInvSpecialRollingPoliciesQuery const &other):
    dbGeneralQuery(other.getKey()) 
{
	*this=other ;
}

dbInvSpecialRollingPoliciesQuery &dbInvSpecialRollingPoliciesQuery::operator=(dbInvSpecialRollingPoliciesQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInvSpecialRollingPoliciesQuery::clone() const
{
	return new dbInvSpecialRollingPoliciesQuery(*this) ;
}


dbInvSpecialRollingPoliciesQuery::~dbInvSpecialRollingPoliciesQuery() throw() {}

unsigned long dbInvSpecialRollingPoliciesQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
# warning incremental not implemented

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

    queryString.addSelectList( dbInvSpecialRollingPolicies::getFields() );
    queryString.addTableList( dbInvSpecialRollingPolicies::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;


	dbQuery query(conn, queryString, "dbInvSpecialRollingPoliciesQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		// record = new dbInvSpecialRollingPolicies ;
		ACS_SMARTPTR<dbInvSpecialRollingPolicies> record(dynamic_cast<dbInvSpecialRollingPolicies*>(dbPersistentFactoryChain::instance()->newObject(dbInvSpecialRollingPolicies::staticClassName(), conn.getKey()))) ; // PRQA S 3081
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
dbInvSpecialRollingPolicies *dbInvSpecialRollingPoliciesQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbInvSpecialRollingPolicies *>(item(i)) ; // PRQA S 3081
}

const dbInvSpecialRollingPolicies *dbInvSpecialRollingPoliciesQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbInvSpecialRollingPolicies *>(item(i)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
