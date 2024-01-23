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
	
	Revision 5.6  2016/07/04 14:52:46  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.5  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.4  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.3  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.2  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
	Revision 5.1  2015/03/17 13:59:29  marfav
	APF-254 Added join clause due to APF special queries
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:00  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:28  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.8  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.7  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.6  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.5  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.4  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.3  2008/05/27 14:21:10  paoscu
	Fixed
	
	Revision 2.2  2008/04/23 18:52:32  paoscu
	Query fixed to check for gaps at beginning and end of interval
	
	Revision 2.1  2008/03/21 18:09:16  paoscu
	dbInventoryValFullIntersectQuery added
	

*/

#include <dbInventoryValFullIntersectQuery.h>
#include <dbInventoryObject.h>
#include <dbLastGeneratedOverlapStrategy.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryValFullIntersectQuery)




string dbInventoryValFullIntersectQuery::staticClassName()
{
	return "dbInventoryValFullIntersectQuery" ;
}




dbInventoryValFullIntersectQuery::dbInventoryValFullIntersectQuery(bool latest, const string &dbKey) :
    dbInventoryObjectSpecialQuery(dbKey),
    _latest(latest),
	_joinToken(),
	_jtable(),
	_qparam(dbKey)
{
}


dbInventoryValFullIntersectQuery::dbInventoryValFullIntersectQuery(dbInventoryValFullIntersectQuery const &rhs) :
	dbInventoryObjectSpecialQuery(rhs),
    _latest(rhs._latest),
	_joinToken(rhs._joinToken),
	_jtable(rhs._jtable),
	_qparam(rhs._qparam)

{
	// empty
}

dbInventoryValFullIntersectQuery &dbInventoryValFullIntersectQuery::operator=(dbInventoryValFullIntersectQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ;
		Lock lock2(other) ;
		
		dbInventoryObjectSpecialQuery::operator=(other) ;
		
		_latest = other._latest ;
		_joinToken = other._joinToken;
		_jtable = other._jtable;
		_qparam = other._qparam;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryValFullIntersectQuery::clone() const
{
	return new dbInventoryValFullIntersectQuery(*this) ;
}


dbInventoryValFullIntersectQuery::~dbInventoryValFullIntersectQuery() throw() {}

unsigned long dbInventoryValFullIntersectQuery::doQuery(dbConnection &conn, bool incremental, bool enabled)
{
	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}
	
	if(_latest) {//if _latest get only first record
		maxItems(1);
    }

	clear() ; // erase previous result set
	if(!enabled) {
		return 0;
    }

	SQLString queryString = _sqlString ;
	queryString.setType(SQLString::SELECT);
	if (maxItems() > 0 )
	{
		queryString.setLimit(maxItems());
	}
	queryString.addTableList(dbInventoryObject::getTable());	
	deque<string> fields = dbInventoryObject::getFieldArray(conn);
	for (uint i = 0; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] );
	}
    // Added the join clause 
	if (!_jtable.empty())
	{
		queryString.addJoinList(_joinToken, _jtable, _qparam) ;
	}
	
	const string &dbKey = conn.getKey() ;
	// where clause
	{
	    dbQueryParameters pp1(dbKey);
	    pp1.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystart", dbParam::MAJOR_OR_EQUAL_OF, startTimeString()) ); 
	    pp1.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystart", dbParam::MINOR_OR_EQUAL_OF, endTimeString()) ); 
	
	    dbQueryParameters pp2(dbKey);
	    pp2.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystop", dbParam::MAJOR_OR_EQUAL_OF, startTimeString()) ); 
	    pp2.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystop", dbParam::MINOR_OR_EQUAL_OF, endTimeString()) ); 

	    dbQueryParameters pp3(dbKey);
	    pp3.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystart", dbParam::MINOR_OR_EQUAL_OF, startTimeString()) ); 
	    pp3.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystop", dbParam::MAJOR_OR_EQUAL_OF, endTimeString()) ); 
		
		dbQueryParameters merged(dbKey) ;
		merged.addQueryParameter(pp1) ;
		merged.setParamOperator( dbQueryParameters::OR ) ;
		merged.addQueryParameter(pp2) ;
		merged.addQueryParameter(pp3) ;
		queryString.addANDWhereParameter(merged);
	
	}
	
	if(_latest) {//if _latest result record are ordered.
		queryString.clearOrderBy();
		queryString.addOrderByList(getOrderByField(), SQLString::DESC) ;
	}

	dbQuery query(conn, queryString, "dbInventoryValFullIntersectQuery::doQuery") ;

	dbRow row(dbKey) ;
	dbSet set ;

	query.execForSet(set) ;


	for (size_t i=0; set.getNextRow(row); i++) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Added a new InventoryObject") ;
		ACS_SMARTPTR<dbInventoryObject>record(dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey()))) ;
		record->loadRow(conn, row,0) ;
		_objects.push_back(record.release()) ;
	}



	dbLastGeneratedOverlapStrategy ovSt1(*this);
	vector< pair <int, dbGeoTimeRange> > res1;
	ovSt1.computeOverlap(res1);
	for(size_t i=0; i< res1.size(); i++)
	{
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, " Slot[" << i << "] : [ " << res1[i].second.start().asISO8601(6) << " : " << res1[i].second.stop().asISO8601(6)) ;
			if (res1[i].first >=0) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "] , file is \""<< ((*this)[res1[i].first])->getFileName() << "\" .") ;
            }
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "] . *** NO FILE *** .");
            }
		ACS_CLASS_END_DEBUG


		if (res1[i].first < 0 ) // there is a hole
		{
			clear() ; // erase previous result set
			return 0 ;
		}

		if( i==0 )
		{
			if( res1[i].second.start() >  startTime() )
			{
				clear() ; // erase previous result set
				return 0 ;
			}
		}

		if( i== ( res1.size() - 1)  )
		{
			if( res1[i].second.stop() <  endTime() )
			{
				clear() ; // erase previous result set
				return 0 ;
			}
		}

	}

	return size() ;
}

string dbInventoryValFullIntersectQuery::getTriggerType() const 
{
	return "INTERSECT" ;
}
void dbInventoryValFullIntersectQuery::setJoinTable(const std::string & table)
{
	_jtable = table;
} ;
void dbInventoryValFullIntersectQuery::setJoinToken(const SQLDialect::SQLJoinToken& token)
{
	_joinToken = token;
} ;
void dbInventoryValFullIntersectQuery::setQueryJoinParam(const dbQueryParameters & params)
{
	_qparam = params;
};



_ACS_END_NAMESPACE
