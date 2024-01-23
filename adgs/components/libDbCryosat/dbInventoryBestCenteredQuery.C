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
	Revision 6.1  2016/07/28 09:43:27  damdec
	CSGACQ-87: new SQLString intervalToSeconds and SQLCase integrated
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.5  2016/07/04 15:05:59  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.4  2016/04/18 15:22:52  marpas
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
	
	Revision 2.5  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.4  2012/03/19 10:38:52  marpas
	no warning from forceSQLString
	
	Revision 2.3  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2010/03/09 09:13:51  crivig
	porting to Oracle11
	
	Revision 2.1  2008/09/09 15:31:48  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.11  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.10  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.9  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.8  2004/08/30 17:51:13  paoscu
	doQuery abstract I/F changed
	
	Revision 1.7  2004/04/28 13:08:40  paoscu
	Incremental query support.
	
	Revision 1.6  2004/04/13 16:52:56  paoscu
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.5  2003/10/21 13:05:24  paoscu
	staticClassName method added.
	
	Revision 1.4  2003/07/29 12:22:50  paoscu
	progReset() moved.
	
	Revision 1.3  2003/07/25 15:17:53  paoscu
	potential memroy leaks removed
	order by management improved
	
	Revision 1.2  2003/04/24 09:29:08  paoscu
	Bug fixed in query.
	
	Revision 1.1  2003/04/17 15:43:33  paoscu
	dbInventoryBestCenteredQuery added.
	
		

*/

#include <dbInventoryBestCenteredQuery.h>
#include <dbInventoryObject.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>


#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryBestCenteredQuery)


string dbInventoryBestCenteredQuery::staticClassName()
{
	return "dbInventoryBestCenteredQuery" ;
}




dbInventoryBestCenteredQuery::dbInventoryBestCenteredQuery(const string &dbkey) :
    dbInventoryObjectSpecialQuery(dbkey)
{
}


dbInventoryBestCenteredQuery::dbInventoryBestCenteredQuery(dbInventoryBestCenteredQuery const &r) :
    dbInventoryObjectSpecialQuery(r)
{
}

dbInventoryBestCenteredQuery &dbInventoryBestCenteredQuery::operator=(dbInventoryBestCenteredQuery const &r)
{
	if(this != &r)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(r) ;
		
		dbInventoryObjectSpecialQuery::operator=(r) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryBestCenteredQuery::clone() const
{
	return new dbInventoryBestCenteredQuery(*this) ;
}







dbInventoryBestCenteredQuery::~dbInventoryBestCenteredQuery() throw() {}

unsigned long dbInventoryBestCenteredQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}

	maxItems(1);


	clear() ; // erase previous result set
	if(!enabled) {
		return 0;
    }

	SQLString queryString = _sqlString ;
	queryString.setType(SQLString::SELECT);
	queryString.setLimit(1);
	queryString.addTableList(dbInventoryObject::getTable());	
	deque<string> fields = dbInventoryObject::getFieldArray(conn);
	for (uint i = 0; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] );
	}
	const string &dbKey = conn.getKey() ;

	SQLString::SQLCase sql_case ;
	sql_case.addWhen
	(
	/* WHEN */
		SQLString::intervalToSeconds(dbKey, endTimeString(), getValidityStopField()) +
		SQLDialectFactory::sqlDialect(dbKey).translateOperator(SQLDialect::MAJOR_OF) +
	    SQLString::intervalToSeconds(dbKey, getValidityStartField(), startTimeString()), 
	/* THEN */
		SQLString::intervalToSeconds(dbKey, getValidityStartField(), startTimeString()) 
	) ;
	sql_case.setElse( 
		SQLString::intervalToSeconds(dbKey, endTimeString(), getValidityStopField())
	) ;

	queryString.addSelectList( sql_case, "DISTANCE" ) ; 

	// where clause
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystart", dbParam::MINOR_OR_EQUAL_OF, startTimeString()) ); 
	    pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystop", dbParam::MAJOR_OR_EQUAL_OF, endTimeString()) ); 
		queryString.addANDWhereParameter(pp);
	
	}
	
	queryString.clearOrderBy();
	queryString.addOrderByList("DISTANCE", SQLString::DESC) ;
	dbQuery query(conn, queryString, "dbInventoryBestCenteredQuery::doQuery") ;

	dbRow row(dbKey) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Added a new InventoryObject") ;
		ACS_SMARTPTR<dbInventoryObject> record(dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey()))) ; // PRQA S 3081
		record->loadRow(conn, row,0) ;
		_objects.push_back(record.release()) ;
	}

	return size() ;
}
std::string dbInventoryBestCenteredQuery::sql_extract(const std::string & what, const std::string & fromValue)
{
	ostringstream retValue;
	int delta(1);
	
	if (what == "day")
	{
		delta = 86400;
	}
	else if (what == "hour")
	{
		delta = 3600;
	}
	else if (what == "minute")
	{
		delta = 60;
	}
	else if (what == "second")
	{
		delta = 1;
	}
	else
	{
		// nothing
	}
	
	retValue << "(extract (" << what << " from " << fromValue << "))*" << delta << "";
	return retValue.str();
}

_ACS_END_NAMESPACE
