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
	Revision 6.1  2016/09/26 12:59:06  damdec
	OPMAN-127: order by string cleared before the reset.
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.5  2016/07/04 15:00:31  damdec
	CSGACQ-82: new getFields interface adopted.
	
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
	
	Revision 4.1  2014/04/08 16:39:55  marpas
	adopting libDateTime 5.13
	
	Revision 4.0  2013/09/30 12:30:57  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:25  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.4.2.1  2012/09/13 09:45:23  marpas
	Special eury MUST NOT use any order by previously set by TL, instead it MUST uses its own sorting criteria for left and right queries
	
	Revision 2.4  2012/03/22 17:32:01  marpas
	fixing left query - parameters were not inherited
	
	Revision 2.3  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/11/25 17:43:34  marpas
	dbInventoryObjectSpecialQuery::setOptionalFilter virtual method implemented
	
	Revision 2.1  2008/09/09 15:35:25  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.10  2005/06/17 10:31:54  marpas
	std namespaces took into consideration
	
	Revision 1.9  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
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
	
	Revision 1.2  2002/12/18 14:57:49  paoscu
	dbInventoryLatestValidityClosestQuery fixed
	
	Revision 1.1  2002/12/17 17:42:48  paoscu
	Imported .
	
	
		

*/

#include <dbInventoryLatestValidityClosestQuery.h>
#include <dbInventoryObject.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <dbConv.h>

#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryLatestValidityClosestQuery)


string dbInventoryLatestValidityClosestQuery::staticClassName()
{
	return "dbInventoryLatestValidityClosestQuery" ;
}



dbInventoryLatestValidityClosestQuery::dbInventoryLatestValidityClosestQuery(const string &dbkey) :
    dbInventoryObjectSpecialQuery(dbkey)
{
}


dbInventoryLatestValidityClosestQuery::dbInventoryLatestValidityClosestQuery(dbInventoryLatestValidityClosestQuery const &r) :
    dbInventoryObjectSpecialQuery(r)
{
}

dbInventoryLatestValidityClosestQuery &dbInventoryLatestValidityClosestQuery::operator=(dbInventoryLatestValidityClosestQuery const &r)
{
	if(this != &r)
	{
		Lock lock1(*this) ;
		Lock lock2(r) ;
		
		dbInventoryObjectSpecialQuery::operator=(r) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryLatestValidityClosestQuery::clone() const
{
	return new dbInventoryLatestValidityClosestQuery(*this) ;
}


dbInventoryLatestValidityClosestQuery::~dbInventoryLatestValidityClosestQuery() throw() {}

unsigned long dbInventoryLatestValidityClosestQuery::doQuery(dbConnection &conn, bool incremental, bool enabled)
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.") ); // PRQA S 3081
	}
	

	DateTime::JD50 middleTime( startTime() );
	middleTime += DateTime::JD50( endTime() );
	middleTime = double(middleTime) / 2.0 ;
	dbRow row(conn.getKey()); 
	dbSet set ;
	dbQuery query(conn) ;
	dbInventoryObject *recordLeft(0) ;
	dbInventoryObject *recordRight(0) ;

	clear() ; // erase previous result set
	if(!enabled)
		return 0;


	//**************  Right  *****************
        // attenzione l'assegnazione implicitamente assegna tutti i parametri che sono gia nella 
        // sqlstring della query intrinsecamente nella classe (guardare l'inheritance
    SQLString querySQL = _sqlString ;
	querySQL.setType(SQLString::SELECT ) ;
	querySQL.setLimit(1) ;
	deque<string> fields = dbInventoryObject::getFieldArray(conn);
	for (uint i = 0; i < fields.size(); ++i)
	{
		querySQL.addSelectList( fields[i] );
	}
	
	querySQL.addTableList(dbInventoryObject::getTable());
	{
        dbQueryParameters pp(conn.getKey()) ;
		pp.addParameter(dbParam(conn.getKey(), dbInventoryObject::getTable(),"validitystart", dbParam::MAJOR_OR_EQUAL_OF , db::toSQL(conn.getKey(), DateTime(middleTime) , dbConv::YearToFraction)));
		querySQL.addANDWhereParameter(pp) ;
 	}
	querySQL.clearOrderBy();
	querySQL.addOrderByList(getValidityStartField(),SQLString::ASC);
	querySQL.addOrderByList(getOrderByField(),SQLString::DESC);
	query.sqlString(querySQL, "LatestValidityClosest_right") ;


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Right query: " <<query.sqlString()) ; 
	query.execForSet(set) ;


	if( not set.empty() )
	{
		set.getNextRow(row);
		recordRight=dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey())) ;

		try {
			recordRight->loadRow(conn, row,0) ;
		}
		catch(...) {
			delete recordRight ; 
			throw ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Right record filename is " <<recordRight->getFileName()) ;

	}


	//**************  Left  *****************


        // attenzione l'assegnazione implicitamente assegna tutti i parametri che sono gia nella 
        // sqlstring della query intrinsecamente nella classe (guardare l'inheritance
    SQLString querySQLStop = _sqlString ;
	querySQLStop.setType(SQLString::SELECT ) ;
	querySQLStop.setLimit(1) ;
	for (uint i = 0; i < fields.size(); ++i)
	{
		querySQLStop.addSelectList( fields[i] );
	}
	querySQLStop.addTableList(dbInventoryObject::getTable());
	{
		dbQueryParameters pp(conn.getKey()) ;
		pp.addParameter(dbParam(conn.getKey(),dbInventoryObject::getTable(),"validitystop", dbParam::MINOR_OR_EQUAL_OF , db::toSQL(conn.getKey(),DateTime(middleTime) , dbConv::YearToFraction)));
		querySQLStop.addANDWhereParameter(pp) ;
 	}
	querySQLStop.clearOrderBy();
	querySQLStop.addOrderByList(getValidityStopField(),SQLString::DESC);
	querySQLStop.addOrderByList(getOrderByField(),SQLString::DESC);
	query.sqlString(querySQLStop, "LatestValidityClosest_left") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Left query: " <<query.sqlString()) ;
	query.execForSet(set) ;
	if( not set.empty() )
	{
		set.getNextRow(row);
		recordLeft=dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey())) ;
		try {
			recordLeft->loadRow(conn, row,0) ;
		}
		catch(...) {
			delete recordRight ;
			delete recordLeft ; 
			throw ;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Left record filename is " <<recordLeft->getFileName()) ;
	}



	//************ Decision **************************

	if( recordLeft && recordRight)
	{
		//There will be only one!
		DateTime leftDistance(middleTime) ;


		leftDistance -=  recordLeft-> getValidityStop().jd50() ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Left Interval distance is  " 
                   << double(DateTime::JD50(leftDistance))) ;

		DateTime rightDistance( recordRight->getValidityStart()  )  ;
		rightDistance-=middleTime;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Right Interval distance is  " 
                   << double(DateTime::JD50(rightDistance))) ;



		if(leftDistance < rightDistance)
		{
			delete recordRight;
			recordRight=0;
		}
		else
		{
			delete recordLeft;
			recordLeft=0;
		}
	}  

	//Now there is at most one record. 

	if( recordLeft)
	{
		_objects.push_back(recordLeft) ;
	}


	if( recordRight)
	{
		_objects.push_back(recordRight) ;
	}  



	return size() ;
}

_ACS_END_NAMESPACE
