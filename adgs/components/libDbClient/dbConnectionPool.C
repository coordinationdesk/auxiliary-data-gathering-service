// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2018/02/08 16:15:07  marpas
	avoid crash when the ConnPools singleton is released after the unloading of all DB drivers
	
	Revision 5.5  2017/11/21 18:42:53  marpas
	qa warnings fixed
	
	Revision 5.4  2016/11/07 16:41:32  marpas
	porting OPMAN-219 changes to HEAD.
	if dbConnectionPool::setMaxConnectionAttempts is set to a number gt 0, and no good connection occurs since the call, after the same number of connection attempts, no connection will succeed any longer. This is to protect applications that would do a number of query to a db, but don't want to wait the timeout repeated more than a fixed number of times.
	
	Revision 5.3  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.2  2015/01/09 14:39:41  enrcar
	EC:: A check if dbConnection is good has been removed during the close. This would bring to severe inconsistencies (dialect already deallocated from memory by automatic d'tor)
	
	Revision 5.1  2013/06/19 18:12:08  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.18  2013/06/03 12:39:44  marpas
	coding best practices partially applied
	qa rules
	
	Revision 4.17  2013/05/14 18:13:59  marpas
	Singleton release at exit implementation
	
	Revision 4.16  2013/05/03 11:16:08  marpas
	adding dbConnPools::release and dbConnectionPool::release method to get rid of unused connections
	
	Revision 4.15  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.14  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.13  2013/04/04 15:35:07  marpas
	statistics improved
	
	Revision 4.12  2013/04/03 11:05:04  marpas
	getKey method added
	
	Revision 4.11  2013/04/03 06:33:49  marpas
	dbConnectionWrapper can cast to a dbConnection & or a dbConnection*
	
	Revision 4.10  2013/03/14 15:26:09  marpas
	ConnPools class added in db namespace
	dbConnectionPool has private ctor to be called only from the above class
	
	Revision 4.9  2013/03/12 13:40:49  marpas
	getMaxConnection method added
	
	Revision 4.8  2013/03/08 16:06:25  marpas
	fix cases where connection is not good and must be asked again to the pool
	
	Revision 4.7  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.2  2011/12/07 15:28:09  lucio.pulvirenti
	New statistics introduced to highlight if the same db conn is exchanged between different threads
	Debug improved
	
	Revision 4.1  2011/11/29 17:48:40  marpas
	Statistics improved
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.8  2011/05/25 12:51:34  lucio.pulvirenti
	try-catch blocks added to all DTORs
	dbConnectionWrapper::operator dbConnection &: connection is open either if null or not good
	(tested with dbConnection::isGood() method)
	
	Revision 3.7  2011/05/19 08:08:05  lucio.pulvirenti
	dbConnectionWrapper: if exception caught when opening connection, dbConnectionPoolElem obj is deleted and
	removed from vector
	
	Revision 3.6  2011/04/05 16:54:17  marpas
	debug improved
	
	Revision 3.5  2011/03/09 18:23:18  marpas
	multithread test improved, debug too.
	
	Revision 3.4  2010/09/08 17:49:31  marpas
	debug improved
	
	Revision 3.3  2010/09/08 08:41:54  marpas
	statistics improved
	
	Revision 3.2  2010/09/08 08:34:04  marpas
	class names revised, statistics added
	
	Revision 3.1  2010/09/07 17:12:17  marpas
	new class dbConnectionPool added
	
*/


#include <dbConnectionPool.h>
#include <dbStat.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using acs::math::StatNode ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConnectionPoolElem)
dbConnectionPoolElem::dbConnectionPoolElem(const string &key) : 
	_conn(key),
	_used(false), 
    _resetTime(),
	_aging(dbConnectionPool::defaultAging),
	_last_thread(pthread_self() )
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "creating conn: " << &_conn) ;
}

dbConnectionPoolElem::~dbConnectionPoolElem() ACS_NOEXCEPT 
{
// PRQA S 4631 L3
	try {
		close() ;
	}
	catch( exception &e )
	{
        ACS_LOG_NOTIFY_EX(SimpleDebugSignature << e) ;
	}
// PRQA L:L3
}
	

dbConnectionPoolElem::operator dbConnection *()  
{
	open() ;
	return &_conn ; // PRQA S 4024
} 

void dbConnectionPoolElem::reset() 
{
	close() ;
}

void dbConnectionPoolElem::open() 
{
	if (!_conn.isOpen()) { // isOpen() returns TRUE only if connection is both OPEN and GOOD -- o/w closes it and returns FALSE
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "opening conn: " << &_conn) ;
		_conn.open() ;
		dbConnectionPool::statN(_conn.getKey())["dbConnection"]["open #"]+=1 ;
		_resetTime = time(0) ;
	}
}

void dbConnectionPoolElem::close() 
{
	const bool checkIsGood = false ;	// Prevent any query to find out if the connection is still good (it's irrelevant, since we are closing...)
	if (_conn.isOpen(checkIsGood)) { // isOpen() returns TRUE only if connection is both OPEN and GOOD -- o/w closes it and returns FALSE
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "closing conn: " << &_conn) ;
		dbConnectionPool::statN(_conn.getKey())["dbConnection"]["close #"]+=1 ;
		_conn.close() ;
	}
}

void dbConnectionPoolElem::setUsed() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "conn: " << &_conn) ;
	_used = true ;
	pthread_t this_thread = pthread_self() ;
	if (_last_thread != this_thread) {
		_last_thread = this_thread ;
	}
}

void dbConnectionPoolElem::setUnused() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "conn: " << &_conn) ;
	_used = false ;
	checkAging() ;
}

// PRQA S 4120 2
bool dbConnectionPoolElem::isUsed() const 
{
	return _used ;
}

bool dbConnectionPoolElem::compare(dbConnection *c) // PRQA S 4211
{
	return c == &_conn ;
}

void dbConnectionPoolElem::checkAging() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	if (!_used && (time(0) >= _resetTime + _aging) ) {

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "aging - wasOpen: " << std::boolalpha << _conn.isOpen(false) << " conn: " << &_conn) ;

		close() ;
	}
}

void dbConnectionPoolElem::setAging(int sec ) 
{
	_aging = sec ;
	checkAging() ;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConnectionPool)
const size_t dbConnectionPool::maxConnDefault ;
const int dbConnectionPool::defaultAging ;

StatNode &dbConnectionPool::statN(const std::string &key) { return db::stat(key)["dbConnectionPool"] ; }

dbConnectionPool::dbConnectionPool(const string &key) :
	_key(key),
    _pool(),
	_maxConn(maxConnDefault), 
	_aging(defaultAging),
    _goodCounter(),
	_wrongCounter(),
	_maxConnectionAttempts(),
    _mutex()

{
	statN(_key)["pool instances"]["created #"]+=1 ;

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

dbConnectionPool::~dbConnectionPool() ACS_NOEXCEPT 
{
// PRQA S 4631 L2
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	statN(_key)["pool instances"]["destroyed #"]+=1 ;
	Lock lock(_mutex) ;
	for(size_t i=0; i < _pool.size(); i++) { // PRQA S 4238, 4244
		try {
			delete _pool[i] ;
			statN(_key)["dbConnection"]["deleted #"]+=1 ;
		}
		catch( exception &e )
		{
            ACS_LOG_NOTIFY_EX(SimpleDebugSignature << e) ;
		}
	}
// PRQA L:L2
}
	
const string &dbConnectionPool::getKey() const ACS_NOEXCEPT { return _key ; } // PRQA S 4120

dbConnectionPoolElem &dbConnectionPool::getConnection() // PRQA S 4020
{
	Lock lock(_mutex) ;
	for (size_t i=0; i < _pool.size(); i++) { // PRQA S 4238, 4244
		if (!_pool[i]->isUsed()) {
			_pool[i]->setUsed() ;
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "returning element # " << i ) ;
			return *_pool[i] ;
		}	
	}
	
	if ((_maxConnectionAttempts > 0) && 
		(_goodCounter == 0) && 
		(_wrongCounter >= _maxConnectionAttempts)) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_wrongCounter: " << _wrongCounter << " _maxConnectionAttempts: " << _maxConnectionAttempts ) ; // PRQA S 3084
		ACS_THROW(dbConnection::ConnectionException("Giving up on max attempt(s) failed")) ;  // PRQA S 3081
	
	}
	
	// add a new connection to the pool 
	_pool.push_back(new dbConnectionPoolElem(_key)) ;
	_pool.back()->setUsed() ;
	_pool.back()->setAging(_aging) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "new connection, pool has " << _pool.size() << " elements - returning element # " << _pool.size()-1 ) ; // PRQA S 3084
	statN(_key)["dbConnection"]["created #"]+=1 ;
	statN(_key)["dbConnection"]["concurrent #"]+=_pool.size(); 

	if (_pool.size() > _maxConn) {
        ACS_LOG_WARNING("Pool max size ("<< _maxConn << ") exceeded: " << _pool.size() << " consider to revise the application flow") ;
	}
	
	return *_pool.back() ;
} 

void dbConnectionPool::setMaxConnection(size_t m) 
{
	Lock lock(_mutex) ;
	_maxConn = m ;
	garbageCleaner() ;
}

size_t dbConnectionPool::getMaxConnection() const ACS_NOEXCEPT // PRQA S 4120
{
	return _maxConn ;
}

void dbConnectionPool::release()  
{ 
	Lock lock(_mutex) ;
    size_t bfore = _pool.size() ;
    garbageCleaner(true) ; 
    size_t after = _pool.size() ;
    ACS_LOG_PRIVINFO("Connection Pool[\"" << getKey() << "\"]: released " << bfore - after << " connections " << _pool.size() << " remaining" ) ; // PRQA S 3084
}

void dbConnectionPool::releaseConnection(dbConnection *c) // PRQA S 4020
{
	if (!c) { return ; }
    
	Lock lock(_mutex) ;
	for (size_t i=0; i < _pool.size(); i++) { // PRQA S 4238, 4244
		if (_pool[i]->compare(c)) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "releasing element # " << i) ;
			_pool[i]->setUnused() ;
			break ;
		}	
	}
	garbageCleaner() ;
}


void dbConnectionPool::destroyElement(dbConnectionPoolElem *c) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_MID_VERB) ;

	if (!c) { return ; }
	
	Lock lock(_mutex) ;
		
	garbageCleaner() ;
	size_t psize = _pool.size() ;
    for (size_t i=0; i < psize; ++i) {
		if (_pool[i] == c) {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "erasing element # " << i) ;
			statN(_key)["dbConnection"]["deleted #"] += -1 ;
			delete _pool[i] ;
			_pool.erase(_pool.begin()+i) ; // PRQA S 3000
			break ;
		}	
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished") ;
}


void dbConnectionPool::setAging(int sec) 
{
	Lock lock(_mutex) ;
	_aging = sec ;
	size_t psize = _pool.size() ;
    for (size_t i=0; i < psize; ++i) {
		_pool[i]->setAging(_aging) ; 
    }
}


void dbConnectionPool::garbageCleaner(bool forced) // PRQA S 4020
{
	Lock lock(_mutex) ;
	size_t psize = _pool.size() ;
	if (not forced) {
		for (size_t i=0; i < psize; ++i) {
			_pool[i]->checkAging();
		}
		
		if (psize < _maxConn) { return ; }
	}
	vector<dbConnectionPoolElem *> used ;
    for (size_t i=0; i < psize; ++i) {
		if (_pool[i]->isUsed()) {
			used.push_back(_pool[i]) ;
        }
		else {
			statN(_key)["dbConnection"]["deleted #"]+=1 ;
			delete _pool[i] ;
		}
	}
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, _pool.size() - used.size() << " element(s) freed") ; // PRQA S 3084
	
	_pool.swap(used) ;
}

void dbConnectionPool::wrongConn() ACS_NOEXCEPT { _wrongCounter++ ; }
void dbConnectionPool::goodConn() ACS_NOEXCEPT { _goodCounter++ ; }
void dbConnectionPool::setMaxConnectionAttempts(unsigned short c ) ACS_NOEXCEPT 
{
	_wrongCounter = 0; 
	_goodCounter =0 ;
	_maxConnectionAttempts = c ; // PRQA S 3000
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConnectionWrapper)

dbConnectionWrapper::dbConnectionWrapper(dbConnectionPool &p) :_pool(p), _conn(0) // PRQA S 2528
{
}

dbConnectionWrapper::~dbConnectionWrapper() ACS_NOEXCEPT 
{
// PRQA S 4631 L1
	try
	{
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Conn ptr: " << hex << _conn) ;
		if (0 != _conn)
		{
			// reset the default query timeout in case it has been modified by the user
			_conn->setMaxQueryDurationSecs (dbConnection::default_maxQueryDurationSecs);
		}
		// release it 
		_pool.releaseConnection(_conn) ;
	}
	catch( exception &e )
	{
        ACS_LOG_NOTIFY_EX(SimpleDebugSignature << e) ;
	}
// PRQA L:L1
}

dbConnection *dbConnectionWrapper::getConn() 
{
	if (!_conn || !_conn->isGood()) {
        if (_conn) {
            // if here, the connection has been declared notGood !!
            _pool.statN(_pool.getKey())["dbConnection"]["notGood #"]+=1 ;

            _pool.releaseConnection(_conn) ; 
        }
        _conn = 0 ;
		dbConnectionPoolElem &pe = _pool.getConnection() ;
		try {
			_conn = pe ; // PRQA S 3050
            _pool.goodConn() ; // add to good connections
        }
		catch(exception &) {
			_pool.wrongConn() ; // add to wrong connections
			try {
				_pool.destroyElement(&pe) ;
			}
			catch( exception &e )
			{
			    ACS_LOG_NOTIFY_EX(SimpleDebugSignature << e) ;
			}
			throw ;
		}
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Conn ptr: " << hex << _conn) ;
	
	return _conn ;
} 

dbConnectionWrapper::operator dbConnection &() 
{
	return *getConn() ;
}

_ACS_BEGIN_NAMESPACE(db)


ACS_CLASS_DEFINE_DEBUG_LEVEL(ConnPools)
StatNode &ConnPools::statN(const std::string &key) { return db::stat(key)["ConnPools"] ; }
ConnPools::ConnPools() :
    Singleton<ConnPools>(),
    ThreadSafe(),
    pools_() 
{
}

ConnPools::~ConnPools() ACS_NOEXCEPT 
{
    // PRQA S 4631 L4
    try {
        release() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L4
}

namespace {
    void release_pools() // the release invoker see the aexit invocation in the ConnPools::getPool method
    {
        ConnPools::instance()->release() ;
    }
}

dbConnectionPool &ConnPools::getPool(const std::string& key) 
{
    Lock l(*this) ;
    std::map<std::string, DBXSH_PTR_NSPACE::shared_ptr<dbConnectionPool> >::iterator p = pools_.find(key) ;
    if (p == pools_.end()) { // does not exists
        statN(key)["pools created #"]+=1 ;
        pools_[key] = DBXSH_PTR_NSPACE::shared_ptr<dbConnectionPool>(new dbConnectionPool(key)) ; // PRQA S 3081
        // when DB driver plugins should be unloaded from the memory, all the connections shall be already close
        // the release_pools (above) invokes the ConnPools::release ensuring this condition deleting all the connection pools 
        // hence all the connections they own
        ::atexit(release_pools) ;
    }
    else {
        statN(key)["hits #"]+=1 ;
    }
    
    return *pools_[key] ;
}

namespace {
    struct get_key { // PRQA S 2171, 2173, 2175, 2185
        explicit get_key(vector<string> &v) : v_(v) {} // PRQA S 2528
        void operator()(const pair<string, DBXSH_PTR_NSPACE::shared_ptr<dbConnectionPool> > &p) { 
            v_.push_back(p.second->getKey()) ;  
        }
    private:
        vector<string> &v_ ;
    } ;
}

void ConnPools::release()
{
    Lock l(*this) ;
    vector<string> poolKeys ;
    for_each(pools_.begin(), pools_.end(), get_key(poolKeys) ) ; // PRQA S 3081
    pools_.clear() ;
    size_t nsize = poolKeys.size() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "pools number: " << nsize) ;
    for (size_t i=0; i < nsize; ++i) { 
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Pool with key: \"" << poolKeys[i] << "\" released.") ;
        statN(poolKeys[i])["pools destroyed #"]+=1 ; 
    }
}
    
_ACS_END_NAMESPACE // db
    
_ACS_END_NAMESPACE // acs
