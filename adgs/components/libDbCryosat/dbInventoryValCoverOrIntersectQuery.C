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
	
	Revision 5.6  2016/07/04 14:39:41  damdec
	CSGACQ-82: compatibility with Oracle introduced.
	
	Revision 5.5  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.4  2016/04/14 10:50:21  marpas
	coding best practices applicationin progress
	somw qa warnings removed
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
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
	
	Revision 2.10  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.9  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.8  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.7  2010/03/05 15:09:11  enrcar
	EC:: Oracle11 dialect supported whenever no specific code was required (was: Postgres8 || Informix9)
	
	Revision 2.6  2008/12/16 18:48:16  paoscu
	Fixed incremental query
	
	Revision 2.5  2008/12/15 16:39:36  marpas
	incremental mode returns the number o data retrieved, regardless the total number of object in the query result
	
	Revision 2.4  2008/12/03 18:08:41  paoscu
	Added incremental support
	Improved additional parameters management
	
	Revision 2.3  2008/12/02 16:56:49  paoscu
	Added extra condition
	
	Revision 2.2  2008/12/01 20:06:38  paoscu
	Join clause added
	
	Revision 2.1  2008/11/04 17:03:03  paoscu
	Added class to dbInventoryValCoverOrIntersectQuery to be inherited by other special queries
	

*/

#include <dbInventoryValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryValCoverOrIntersectQuery)




string dbInventoryValCoverOrIntersectQuery::staticClassName()
{
	return "dbInventoryValCoverOrIntersectQuery" ;
}

dbInventoryValCoverOrIntersectQuery::dbInventoryValCoverOrIntersectQuery(bool latest, bool intersect, const string &dbKey) :
    dbInventoryObjectSpecialQuery(dbKey) ,
    _latest(latest),
	_intersect(intersect),
	_joinToken(),
	_jtable(),
	_qparam(dbKey),
    _extraCondition(),
    _tempParameters()
{
}


dbInventoryValCoverOrIntersectQuery::dbInventoryValCoverOrIntersectQuery(dbInventoryValCoverOrIntersectQuery const &rhs) :
	dbInventoryObjectSpecialQuery(rhs),
	_latest(rhs._latest),
	_intersect(rhs._intersect),
	_joinToken(rhs._joinToken),
	_jtable(rhs._jtable),
	_qparam(rhs._qparam),
	_extraCondition(rhs._extraCondition),
	_tempParameters(rhs._tempParameters)

{
	// empty
}

dbInventoryValCoverOrIntersectQuery &dbInventoryValCoverOrIntersectQuery::operator=(dbInventoryValCoverOrIntersectQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ;
		Lock lock2(other) ;
		
		dbInventoryObjectSpecialQuery::operator=(other) ;
		
		_latest = other._latest ;
		_intersect = other._intersect ;
		_joinToken = other._joinToken;
		_jtable = other._jtable;
		_qparam = other._qparam;
		_extraCondition = other._extraCondition ;
		_tempParameters = other._tempParameters ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryValCoverOrIntersectQuery::clone() const
{
	return new dbInventoryValCoverOrIntersectQuery(*this) ;
}


dbInventoryValCoverOrIntersectQuery::~dbInventoryValCoverOrIntersectQuery() throw() {}



unsigned long dbInventoryValCoverOrIntersectQuery::doQuery(dbConnection &conn, bool incremental, bool enabled)
{

	if(_latest) { //if _latest get only first record
		maxItems(1);
    }


	if(!incremental) {
		clear() ; // erase previous result set
    }

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
    // Added the join clause needed by special query
	if (!_jtable.empty())
	{
		queryString.addJoinList(_joinToken, _jtable, _qparam) ;
	}
	
	const string &dbKey = conn.getKey() ;
	// where clause
	if ( _intersect )
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
	else
	{
	    dbQueryParameters pp(dbKey);
	    pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystart", dbParam::MINOR_OR_EQUAL_OF, startTimeString()) ); 
	    pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "validitystop", dbParam::MAJOR_OR_EQUAL_OF, endTimeString()) ); 
		queryString.addANDWhereParameter(pp);
	}
	
	if(!_extraCondition.empty() )
	{
		queryString.addANDWhereParameter(_extraCondition);
	}
	if(_latest) {//if _latest result record are ordered.
		queryString.clearOrderBy();
		queryString.addOrderByList(getOrderByField(), SQLString::DESC) ;
	}

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query\n" << queryString.getSQL() );
	dbQuery query(conn, queryString, "dbInventoryValCoverOrIntersectQuery::doQuery") ;

	dbRow row(dbKey) ;
	dbSet set ;

	query.execForSet(set) ;
    long rows = set.size() ;
	vector<dbInventoryObject *> newObjects ;
	for (size_t i=0; set.getNextRow(row); i++) 
	{
		ACS_SMARTPTR<dbInventoryObject> record(dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", conn.getKey()))) ;
		record->loadRow(conn, row,0) ;

		if(!incremental)
		{
			_objects.push_back(record.release()) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryObject") ;
		}
	    else
		{
 	        // Check if the new record has already been loaded by a previous query
			// in case delete it
			for (size_t ne=0; ne<_objects.size();ne++)
			{
 	            dbInventoryObject *oe = dynamic_cast<dbInventoryObject *>(_objects[ne]) ;
 	            if (oe && record->getFileName() == oe->getFileName() && record->getFileVersion() == oe->getFileVersion()) {
 	                record.reset() ;
 	                break ;
                }
 	        }
 	        if (record.get()) {
                newObjects.push_back(record.release()) ;
 	        }
 	    }
	}

    if (incremental) {
        size_t no_size =  newObjects.size() ;
        for (size_t i=0; i < no_size; ++i)
        {
            _objects.insert(_objects.begin(),newObjects[i]) ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryObject") ;
        }
    }

	return rows ;
}

string dbInventoryValCoverOrIntersectQuery::getTriggerType() const 
{
	if(_intersect) {
		return "INTERSECT" ;
    }
	else {
		return "COVER" ;
    }
}


void dbInventoryValCoverOrIntersectQuery::setExtraCondition ( std::string const & s)
{
	_extraCondition= s ;
} 


void dbInventoryValCoverOrIntersectQuery::resetParameters()
{
	_extraCondition = "" ;
	dbInventoryObjectSpecialQuery::resetParameters() ;
}


const string &dbInventoryValCoverOrIntersectQuery::getParameters() const
{
	_tempParameters =  dbInventoryObjectSpecialQuery::getParameters() ;
	if ( !_tempParameters.empty() && !_extraCondition.empty() ) {
		_tempParameters += " AND " + _extraCondition + " " ; 
    }
	return _tempParameters ;
}

void dbInventoryValCoverOrIntersectQuery::setJoinTable(const std::string & table)
{
	_jtable = table;
} ;
void dbInventoryValCoverOrIntersectQuery::setJoinToken(const SQLDialect::SQLJoinToken& token)
{
	_joinToken = token;
} ;
void dbInventoryValCoverOrIntersectQuery::setQueryJoinParam(const dbQueryParameters & params)
{
	_qparam = params;
};

_ACS_END_NAMESPACE
