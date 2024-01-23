// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA. - DFDA-AS
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
	Revision 5.2  2016/11/07 16:41:32  marpas
	porting OPMAN-219 changes to HEAD.
	if dbConnectionPool::setMaxConnectionAttempts is set to a number gt 0, and no good connection occurs since the call, after the same number of connection attempts, no connection will succeed any longer. This is to protect applications that would do a number of query to a db, but don't want to wait the timeout repeated more than a fixed number of times.
	
	Revision 5.1  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.15  2013/05/14 18:13:59  marpas
	Singleton release at exit implementation
	
	Revision 4.14  2013/05/03 11:16:08  marpas
	adding dbConnPools::release and dbConnectionPool::release method to get rid of unused connections
	
	Revision 4.13  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.12  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.11  2013/04/03 11:05:04  marpas
	getKey method added
	
	Revision 4.10  2013/04/03 06:33:49  marpas
	dbConnectionWrapper can cast to a dbCOnnection & or a dbConnection*
	
	Revision 4.9  2013/03/14 15:26:09  marpas
	ConnPools class added in db namespace
	dbConnectionPool has private ctor to be called only from the above class
	
	Revision 4.8  2013/03/12 13:40:49  marpas
	getMaxConnection method added
	
	Revision 4.7  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.6  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/12/07 15:28:09  lucio.pulvirenti
	New statistics introduced to highlight if the same db conn is exchanged between different threads
	Debug improved
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.3  2011/05/19 08:08:05  lucio.pulvirenti
	dbConnectionWrapper: if exception caught when opening connection, dbConnectionPoolElem obj is deleted and
	removed from vector
	
	Revision 3.2  2010/09/08 08:34:04  marpas
	class names revised, statistics added
	
	Revision 3.1  2010/09/07 17:12:17  marpas
	new class dbConnectionPool added
	

*/

#ifndef _dbConnectionPool_H_
#define _dbConnectionPool_H_

#include <acs_c++config.hpp>
#include <dbConnection.h>
#include <Singleton.hpp>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ; // fwd decl

class dbConnectionPoolElem { // PRQA S 2109

public:
	explicit dbConnectionPoolElem(const std::string& key) ;
	~dbConnectionPoolElem() noexcept ;
	dbConnectionPoolElem() = delete ;
	dbConnectionPoolElem(const dbConnectionPoolElem &) = delete ;
	dbConnectionPoolElem &operator=(const dbConnectionPoolElem &) = delete ;
	dbConnectionPoolElem(dbConnectionPoolElem &&) = delete ;
	dbConnectionPoolElem &operator=(dbConnectionPoolElem &&) = delete ;

	/** a cast method to a dbConnection 
	*/
	operator dbConnection *() ; // PRQA S 2181
	/** reset the connection (closes and reopen )
	 */
	void reset() ;
	void setUsed() ;
	void setUnused() ;
	bool isUsed() const ;
	bool compare(dbConnection *) ;
	void setAging(int sec ) ;

private:

	void open() ;   // PRQA S 2502 2 
	void close() ;

	friend class dbConnectionPool;
	void checkAging() ;

private:
	dbConnection _conn ;
	bool _used ;
	time_t _resetTime ;
	int _aging ;
	pthread_t _last_thread ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConnectionPoolElem) ;
} ;



_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

_ACS_BEGIN_NAMESPACE(db)

using pattern::Singleton ;

class ConnPools : // PRQA S 2109, 2153
    public Singleton<ConnPools>, 
    public ThreadSafe 
{
    friend ConnPools* Singleton<ConnPools>::instance(); // PRQA S 2107

public:
	virtual ~ConnPools() noexcept ;
	ConnPools(const ConnPools &) = delete ;
	ConnPools & operator= (const ConnPools &) = delete ;
	ConnPools(ConnPools &&) = delete ;
	ConnPools & operator= (ConnPools &&) = delete ;

	dbConnectionPool &getPool(const std::string& key) ;
    /* this method should be called at exiting to close all existing (unused) connections */
    void release() ;
protected:
	ConnPools();
private:
    static math::StatNode &statN(const std::string &key) ;
private:  
    std::map<std::string, std::shared_ptr<dbConnectionPool> > pools_ ;  
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ConnPools) ;
} ;
    
_ACS_END_NAMESPACE // db

class dbConnectionWrapper { // PRQA S 2109
friend class dbConnectionPool ; // PRQA S 2107

public:
	explicit dbConnectionWrapper(dbConnectionPool &) ;
	~dbConnectionWrapper() noexcept ;
	dbConnectionWrapper() = delete ;
	dbConnectionWrapper(const dbConnectionWrapper &) = delete ;
	dbConnectionWrapper &operator=(const dbConnectionWrapper &) = delete ;
	dbConnectionWrapper(dbConnectionWrapper &&) = delete ;
	dbConnectionWrapper &operator=(dbConnectionWrapper &&) = delete ;
	
    const std::string &getKey() const noexcept ;

	/** the main method to get the connection
	*/
	operator dbConnection &() ; // PRQA S 2181
private:
    
    dbConnection *getConn() ; 
private:
	dbConnectionPool &_pool ;
	dbConnection *_conn ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConnectionWrapper) ;
} ;

class dbConnectionPool { // PRQA S 2109, 2153
    friend dbConnection *dbConnectionWrapper::getConn() ; // PRQA S 2107

public:
	virtual ~dbConnectionPool() noexcept ;

	/** returns the first dbConnection not used the returned connection is open.
	  */
	dbConnectionPoolElem &getConnection() ; 
	void releaseConnection(dbConnection *) ;
    
    /* this method should be called at exiting to close all existing (unused) connections */
    void release() ;

	void setMaxConnectionAttempts(unsigned short) noexcept ;
	void setMaxConnection(size_t) ;
	void setAging(int) ;
    size_t getMaxConnection() const noexcept ;
    const std::string &getKey() const noexcept ;
    static math::StatNode &statN(const std::string &key) ;
private:
	void destroyElement(dbConnectionPoolElem *) ;
	void wrongConn() noexcept ;
	void goodConn() noexcept ;

	// not implemented 
	dbConnectionPool() ;
	dbConnectionPool(const dbConnectionPool &) ;
	dbConnectionPool &operator=(const dbConnectionPool &) ;
	void garbageCleaner(bool forced = false) ;
#ifdef TEST_DBCONNECTIONPOOL
public:
#else
private:
#endif
	explicit dbConnectionPool(const std::string& key) ;
    friend dbConnectionPool& db::ConnPools::getPool(const std::string &) ; // PRQA S 2107

public:		
	static const size_t maxConnDefault = 4 ;
	static const int defaultAging = 300 ; // 5 min

private:
	std::string _key ; 
	std::vector< dbConnectionPoolElem *> _pool ;
	size_t _maxConn ;
	int _aging ;
	int _goodCounter ;
	int _wrongCounter ;
	int _maxConnectionAttempts ;
    Mutex _mutex ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConnectionPool) ;
} ;

    
_ACS_END_NAMESPACE // acs


#endif // _dbConnectionPool_H_
