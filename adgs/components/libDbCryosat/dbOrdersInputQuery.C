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
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:02  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:30  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2012/02/14 12:52:50  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/09/17 09:32:27  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2006/01/04 16:27:56  enrcar
	Updated to DBEXPRESS
	
	Revision 1.3  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.2  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.1  2004/09/24 12:21:14  ivafam
	Added dbOrdersInput class
	
	
*/

#include <dbOrdersInputQuery.h>
#include <dbOrdersInput.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOrdersInputQuery)

string dbOrdersInputQuery::staticClassName()
{
	return "dbOrdersInputQuery" ;
}


// constructor
dbOrdersInputQuery::dbOrdersInputQuery(const string &dbkey) :
    dbGeneralQuery(dbkey)
{
}


dbOrdersInputQuery::dbOrdersInputQuery(dbOrdersInputQuery const &other):
    dbGeneralQuery(other.getKey()) 
{
}

dbOrdersInputQuery &dbOrdersInputQuery::operator=(dbOrdersInputQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbOrdersInputQuery::clone() const
{
	return new dbOrdersInputQuery(*this) ;
}


dbOrdersInputQuery::~dbOrdersInputQuery() throw() {}



unsigned long dbOrdersInputQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
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

    queryString.addSelectList( dbOrdersInput::getFields() );
    queryString.addTableList( dbOrdersInput::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"" ) ;

	dbQuery query(conn, queryString, "dbOrdersInputQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		ACS_SMARTPTR<dbOrdersInput> record(dynamic_cast<dbOrdersInput*>(dbPersistentFactoryChain::instance()->newObject(dbOrdersInput::staticClassName(), conn.getKey()))) ; // PRQA S 3081
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
dbOrdersInput *dbOrdersInputQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbOrdersInput *>(item(i)) ; // PRQA S 3081
}

const dbOrdersInput *dbOrdersInputQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbOrdersInput *>(item(i)) ; // PRQA S 3081
}

_ACS_END_NAMESPACE
