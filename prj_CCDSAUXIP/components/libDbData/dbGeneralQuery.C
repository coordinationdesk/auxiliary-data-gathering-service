// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.4  2016/03/14 16:56:37  marpas
	implementing getKey()
	
	Revision 5.3  2016/03/14 13:16:39  marpas
	introducing offset support in queries
	
	Revision 5.2  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.13  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.12  2013/02/25 16:50:16  marpas
	interface minor changes
	
	Revision 2.11  2013/02/11 12:57:19  marpas
	dbGeneralQuery::size() signature fixed
	
	Revision 2.10  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.9  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.8  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.5  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.4  2009/06/16 13:09:57  marpas
	join implemented on queries
	
	Revision 2.3  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2007/11/29 11:30:22  marpas
	statistical improvements on dbQuery follow-on
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.15  2006/01/11 13:53:28  aleric
	addOrderBy interface added
	
	Revision 1.14  2005/10/11 07:56:13  marpas
	getParameters I/F also for non-DBEXPRESS
	
	Revision 1.13  2005/08/08 11:40:49  enrcar
	Added DBEXPRESS handling
	
	Revision 1.11  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.10  2004/04/28 13:09:22  paoscu
	Incremental query support.
	
	Revision 1.9  2004/04/13 16:37:25  marpas
	copy ctor and assignement operator fixed
	
	Revision 1.8  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.7  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.6  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.5  2003/04/17 13:58:42  marpas
	GCC3.2
	
	Revision 1.4  2003/03/11 13:49:51  marpas
	still working
	
	Revision 1.3  2003/02/13 13:53:55  marpas
	*** empty log message ***
	
	Revision 1.2  2003/01/27 17:52:52  paoscu
	Bug fixed in _object vector initialization.
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#include <dbGeneralQuery.h>
#include <dbQueryParameters.h>
#include <dbPersistent.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeneralQuery) 


//
// dbGeneralQuery::dbGeneralQuery() 
//
dbGeneralQuery::dbGeneralQuery(const string &key) :
    dbGeneralQueryIF(),
    _objects(), 
    _sqlString(key, SQLString::QUERY_UNDEFINED),
	_maxItems (-1),
    _orderBy(),
    _mutex(),
	_offset (-1),
    _key(key)
{
	_objects.reserve(1024) ; // stores for 1024 passes, should optimize // PRQA S 4400
}






dbGeneralQuery &dbGeneralQuery::operator=(const dbGeneralQuery &other)
{
		
	if(this != &other)
	{
		ThreadSafe::Lock lock1(_mutex) ;
		ThreadSafe::Lock lock2(other._mutex) ;
		
		clear() ;
		
		for (size_t i=0 ; i< other.size() ; i++ ) {
			_objects.push_back( other.item(i) -> clone() ) ;
        }
			
        _sqlString = other._sqlString ;
		_maxItems = other._maxItems;
        _offset = other._offset ;
        _key = other._key ;
        // nb _oderBy do not need copy - is always rebuilded
	}

	return *this ;
}


dbGeneralQuery::dbGeneralQuery(const dbGeneralQuery &r) :
    dbGeneralQueryIF(r),
    _objects(), 
    _sqlString(r._sqlString),
	_maxItems (r._maxItems),
    _orderBy(),
    _mutex(),
    _offset(r._offset),
    _key(r._key) 
{
	for (size_t i=0 ; i< r.size() ; i++ ) {
		_objects.push_back( r.item(i) -> clone() ) ;
    }
}



//
// dbGeneralQuery::~dbGeneralQuery()
//
dbGeneralQuery::~dbGeneralQuery() noexcept 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB) ;
    try {
	    dbGeneralQuery::clear() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L1
}

void dbGeneralQuery::addJoin(SQLDialect::SQLJoinToken j, const std::string &table, const dbQueryParameters &p) 
{
	ThreadSafe::Lock lock(_mutex) ;
	_sqlString.addJoinList(j, table, p) ;
} 


//
// dbGeneralQuery::addParameter(const dbQueryParameters &p) 
//
void dbGeneralQuery::addParameter(const dbQueryParameters &p, ParamOperator po) 
{
	ThreadSafe::Lock lock(_mutex) ;

    switch(po) {
    case AND:  { _sqlString.addANDWhereParameter(p) ; break ; }
    case OR:   { _sqlString.addORWhereParameter(p) ; break ; }
    default:
        {
            ostringstream os ;
            os << "dbGeneralQuery::addParameter - invalid ParamOperator enum (" << int(po) << ")" ; // PRQA S 3081
            ACS_THROW(InvalidUse(os.str())) // PRQA S 3081
        }
    }
}

void dbGeneralQuery::addParameter(const string &p, ParamOperator po) 
{
	ThreadSafe::Lock lock(_mutex) ;

    switch(po) {
    case AND:   { _sqlString.addANDWhereParameter(p) ; break ; }
    case OR:    { _sqlString.addORWhereParameter(p) ; break ; }
    default:
        {
            ostringstream os ;
            os << "dbGeneralQuery::addParameter - invalid ParamOperator enum (" << int(po) << ")" ; // PRQA S 3081
            ACS_THROW(InvalidUse(os.str())) // PRQA S 3081
        }
    }
}

//
// dbGeneralQuery::resetParameters() 
//
void dbGeneralQuery::resetParameters() 
{
	ThreadSafe::Lock lock(_mutex) ;
		
    _sqlString.reset() ;
}


size_t dbGeneralQuery::size() const
{
	ThreadSafe::Lock lock(_mutex) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "size: " << _objects.size() );
	return _objects.size() ; 
}

bool dbGeneralQuery::empty() const
{
	ThreadSafe::Lock lock(_mutex) ;
		
	return _objects.empty() ; 
}

dbPersistent * dbGeneralQuery::item(unsigned long i) 
{
	ThreadSafe::Lock lock(_mutex) ;
		
	if (i >= _objects.size()) 
	{
		ostringstream os ;
		os << "Only " << _objects.size() << " elements: cannot find #" << i ;
		ACS_THROW(exBoundsException("dbGeneralQuery::item out of bound. " + os.str())) ;  // PRQA S 3081
	}
	return _objects[i] ; 
}

const dbPersistent * dbGeneralQuery::item(unsigned long i) const
{
	ThreadSafe::Lock lock(_mutex) ;
		
	if (i >= _objects.size()) 
	{
		ostringstream os ;
		os << "Only " << _objects.size() << " elements: cannot find #" << i ;
		ACS_THROW(exBoundsException("dbGeneralQuery::item out of bound. " + os.str())) ;  // PRQA S 3081
	}
	return _objects[i] ; 
}

void dbGeneralQuery::erase(unsigned long pos) 
{
	ThreadSafe::Lock lock(_mutex) ;
		
	if (pos >= _objects.size())
	{
		ostringstream os ;
		os << "Only " << _objects.size() << " elements: cannot remove #" << pos ;
		ACS_THROW(exBoundsException("dbGeneralQuery::erase out of bound. " + os.str())) ;  // PRQA S 3081
	}
	
	dbPersistent *tbd = _objects[pos] ;
	_objects.erase(_objects.begin()+pos) ; // PRQA S 3000
	delete tbd ;
}

void dbGeneralQuery::clear() 
{
	ThreadSafe::Lock lock(_mutex) ;
	
    size_t os = _objects.size() ;
	for (unsigned long i=0; i < os; i++) {
		delete _objects[i] ;
	}	
	_objects.clear() ;
}

ssize_t dbGeneralQuery::maxItems() const 
{ 
	ThreadSafe::Lock lock(_mutex) ;
		
	return _maxItems ; 
}

void dbGeneralQuery::maxItems(ssize_t v) 
{
	ThreadSafe::Lock lock(_mutex) ;
		
    _maxItems = v ; 
}


const string &dbGeneralQuery::getOrderBy() const 
{
	ThreadSafe::Lock lock(_mutex) ;
	_orderBy = _sqlString.getOrderBy() ;
	return _orderBy ;
}

const string &dbGeneralQuery::getOrderBySqlClause() const 
{
	ThreadSafe::Lock lock(_mutex) ;
		
	return getOrderBy() ;
}

void dbGeneralQuery::setOrderBy(const string &v)  
{
	ThreadSafe::Lock lock(_mutex) ;
		
	_sqlString.setOrderByString(v) ;
}

void dbGeneralQuery::addOrderBy(const string &v, SQLString::OrderBy const & o )  
{
	ThreadSafe::Lock lock(_mutex) ;
		
	_sqlString.addOrderByList(v, o) ;
}


const std::string &dbGeneralQuery::getParameters() const 
{
	ThreadSafe::Lock lock(_mutex) ;

    return _sqlString.getWhereParameters() ;
}

const vector<string> &dbGeneralQuery::getJoinParameters() const 
{
	ThreadSafe::Lock lock(_mutex) ;
	return _sqlString.getJoinParameters() ;
}

void dbGeneralQuery::copyJoinParameters(SQLString& sql) {
	ThreadSafe::Lock lock(_mutex) ;
	_sqlString.copyJoinParameters(sql);
}


_ACS_END_NAMESPACE
