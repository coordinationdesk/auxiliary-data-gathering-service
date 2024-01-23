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
	Revision 5.7  2016/03/21 19:46:48  marpas
	qa warning fixed
	
	Revision 5.6  2016/03/21 19:20:56  marpas
	dbConnection ctors and open and close methods cannot be longer called from normal code.
	connection creation, opening can only be performed from a pool and temporarily from dbConnectionMgr(obsolete) untile the latter exists
	
	Revision 5.5  2014/06/27 09:16:51  marpas
	qa warning
	
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/19 12:13:55  marpas
	Transaction class introduced
	
	Revision 5.2  2013/06/13 18:10:54  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.12  2013/06/03 12:38:01  marpas
	last serial is a long long instead of unsigned long long
	
	Revision 4.11  2013/05/14 18:10:49  marpas
	useless methods removed
	master/slave support removed due to remotion of useless classes in library
	fix for bad behaviour when closing connections after plugins have been downloaded
	
	Revision 4.10  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.9  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.8  2013/02/12 12:38:14  marpas
	master db management functions rationalized
	
	Revision 4.7  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.6  2012/11/21 09:55:13  marpas
	re-adding file from attic for realignement
	
	Revision 4.4  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.3  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.2  2011/10/20 13:45:00  lucio.pulvirenti
	Statistics improved
	
	Revision 4.1  2011/10/18 14:56:43  enrcar
	EC::LP:: Status ERROR introduced & handled
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.6  2011/05/31 10:06:23  lucio.pulvirenti
	mutex and prevent conn check counter added
	
	Revision 3.5  2011/05/24 13:20:05  lucio.pulvirenti
	public isGood method added to check if an open connection is good via a dummy query
	optional parameter added to isOpen method (default true) in order to call isGood
	when connection is open
	
	Revision 3.4  2011/03/02 17:03:35  marpas
	getting last serial implementation completed and tested with informix
	
	Revision 3.3  2010/10/14 18:01:38  marpas
	Statistics for queries for connection added
	they are total good and total errors, and then subdivided into
	execForSet, execForStatus and execOneRow groups along with min, max and average
	
	Revision 3.2  2010/09/11 18:07:36  marpas
	libDbExpress 3.2 interface adopted, libDbException 3.3 too
	
	Revision 3.1  2009/10/08 10:50:12  marpas
	dbStat class removed and substituted by new libException.Statistics
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.8  2009/07/14 10:20:18  enrcar
	EC:: c++rules
	
	Revision 2.7  2009/07/01 11:35:45  marpas
	Null connection will cause an exception from ctor of dbConnectionGuard instead of a crash
	dtor of dbConnection will not raise exceptions by default
	
	Revision 2.6  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.5  2009/03/02 14:03:30  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.4  2008/12/05 17:06:02  marpas
	getLastSerial method added
	
	Revision 2.3  2008/01/10 16:03:48  lucio.pulvirenti
	added labels for transactions statistics
	
	Revision 2.2  2008/01/10 11:41:41  paoscu
	It is now possible by default to save more times the same persistent wrapper during a transaction.
	Only the first one is actully stored while others are silently ignored.
	The method dbConnection::newTransactionPhase has been removed.
	
	Revision 2.1  2007/03/21 16:05:18  marpas
	forceLocalHostIsMaster static method added to dbConnection and dbConnectionArbiter to support application development and integration
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/10/19 08:39:40  enrcar
	Updated to dbClient version: Memento added.
	SQLDialect register and unregister methods called
	
	Revision 1.11  2005/08/11 07:56:10  marpas
	statistics almost fully tested
	
	Revision 1.10  2005/05/05 13:43:18  enrcar
	Added sql-command filter on returned value
	removed query methods (execforstatus execforset) and moved to dbQuery.C
	
	Revision 1.9  2005/04/27 10:04:00  enrcar
	exec-query functions moved to dbQuery
	
	Revision 1.8  2005/04/22 20:29:31  marpas
	work in progress
	
	Revision 1.7  2005/04/22 10:43:18  marpas
	work in progress
	
	Revision 1.6  2005/04/21 17:11:15  marpas
	work in progress
	
	Revision 1.5  2005/04/19 09:19:12  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/04/15 10:35:04  enrcar
	*** empty log message ***
	
	Revision 1.3  2005/03/15 13:08:14  enrcar
	Add connection() method to return the _connection
	
	Revision 1.2  2005/03/11 13:34:10  enrcar
	*** empty log message ***
	
	Revision 1.30  2005/02/23 08:47:03  marpas
	Improved way to check multithreading
	
	Revision 1.29  2005/02/16 15:38:21  marpas
	Every time type into the dbConnection interface has been changed from size_t to int.
	
	Revision 1.28  2005/02/16 14:48:22  marpas
	dbConnection::getStatistics patched.
	dbQuery statistics added
	dbConnection total wait time is now 30 sec, single step is 10 sec (5 if multithread).
	dbConnection: the env variable dbConnection_SingleStepWaitSecond can be used to override the default
	dbConnection: the env variable dbConnection_TotalWaitSecond can be used to override the default
	
	Revision 1.27  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.26  2004/08/30 17:42:18  marpas
	dbConnection I/F changed to take into account the need to set each single step WAIT FOR SECONDS time
	and a total wait time
	dbQuery behaviour now uses the new dbConnection I/F for all three main methods
	dbQuery now implements a policy to avoid repeating the query if the returned error was not due to the timeout on a lock
	because single step lock granularity is 1 sec., if a single step returns with an error when less than half a second is elapsed, 
	the query is not longer repeated, but the exception is thrown.
	
	Revision 1.25  2004/05/31 16:30:37  marpas
	dbConnection::setDefaultLockWaitSeconds method created
	
	Revision 1.24  2004/05/31 16:15:17  marpas
	dbQuery execution method policy changed when the connection is in LockWaitSeconds mode
	
	Revision 1.23  2004/05/13 17:26:48  marpas
	newTransactionPhase method added to support multiphase transaction on the same object instances
	
	Revision 1.22  2004/04/14 15:39:00  marpas
	NameIdAssociator is a template
	
	Revision 1.21  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.20  2004/02/16 13:34:22  paoscu
	setResource and getResource fixed.
	
	Revision 1.19  2004/02/03 17:00:59  paoscu
	Work on transaction state.
	Better doxygen comments.
	
	Revision 1.18  2004/01/28 16:39:26  paoscu
	Begin work, commit and rollback moved in dbConnection
	
	Revision 1.17  2004/01/27 19:28:44  paoscu
	Transaction state management added.
	
	Revision 1.16  2003/08/01 16:12:47  marpas
	is possible to ignore the MASTER/SLAVE status and perform the connection
	on the first database available iff any
	
	Revision 1.15  2003/07/30 09:44:22  marpas
	dbConnection::localMaster method added
	
	Revision 1.14  2003/07/14 19:24:19  marpas
	dbConnection::hostname moved in libApp.Application
	
	Revision 1.13  2003/07/14 17:02:11  marpas
	dbConnection::hostname static method implemented
	
	Revision 1.12  2003/07/11 13:08:02  marpas
	exException::clone implemented
	
	Revision 1.11  2003/04/30 13:36:03  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.10  2003/03/20 17:53:43  marpas
	local host master detection implemented
	
	Revision 1.9  2003/03/17 17:53:40  paoscu
	dbConnection ctor initializes the AppResSet singleton
	
	Revision 1.8  2003/03/14 20:21:52  paoscu
	new policy to establish who is the arbiter.
	
	Revision 1.7  2003/03/14 19:16:30  paoscu
	Master/Slave policy implemented
	
	Revision 1.6  2003/03/13 17:56:34  paoscu
	work in progress
	
	Revision 1.5  2003/03/07 18:33:12  paoscu
	seems working now ...
	
	Revision 1.4  2003/03/06 18:30:50  paoscu
	work in progress
	
	Revision 1.3  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.2  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#ifndef _dbConnection_H_
#define _dbConnection_H_

#include <acs_c++config.hpp>
#include <dbAggregateTransactionState.h>
#include <dbConnectionParameters.h>
#include <dbException.h>

#include <Timer.h>


class SQLConnection ;
class SQLCommand ;
class SQLMetaData ;
class SQLCursor ;

_ACS_BEGIN_NAMESPACE(acs)

class dbStateWrapper ;

_ACS_BEGIN_NAMESPACE(math) class StatNode ; _ACS_END_NAMESPACE 
_ACS_BEGIN_NAMESPACE(db) class Transaction ; _ACS_END_NAMESPACE 


/**
 *\brief Used to manage connection
 *	
 *This class is used to manage the connection with a generic db. <BR>
 *
 */

class dbConnection // PRQA S 2109
{
public:
	friend class acs::db::Transaction ; // PRQA S 2107
	
	/**
	 *The Lock Mode.
	 *Some database operations require a row, table, or entire database lock.
	 *When another user try to obtain a locked resource, the lock mode tells
	 *the query behaviour.
	 *
	 *Default is WaitForSeconds(30)
	 *
	 */
	enum LockMode {
		//! If the database is in lock mode, the query fails
		NoWait,
		//! Wait until database is available
		Wait,
		//! Wait for the specified number of seconds see also dbQuery::execOneRow, dbQuery::execForStatus, dbQuery::execForSet
		WaitForSeconds
	}  ;

	struct Memento { // PRQA S 2173, 2175
		Memento() ;
		~Memento() = default ;
		Memento(const Memento &) ;
		Memento & operator=(const Memento &) ;
		
		LockMode _lockMode ;                     // PRQA S 2100 5  
		int		_singleStepSecondsToWait ;
		int		_totalWaitSeconds ;
	} ;
	

	
	/**
	 *The state of current transaction
	 */
	enum dbTransactionState
    {
        None,
        Auto,
        Begin,
        Commit,
        Abort,
		Error
    } ;

	/*! class ConnectionException declaration */
	exDECLARE_EXCEPTION(ConnectionException,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class StateException declaration */
	exDECLARE_EXCEPTION(StateException,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class CommitException declaration */
	exDECLARE_EXCEPTION(CommitException,dbException) ; // PRQA S 2131, 2153, 2502
private:
	explicit dbConnection(const std::string& key) ;	
	dbConnection(const std::string &key, const dbConnectionParameters &cp);
    friend class dbConnectionPoolElem ; // PRQA S 2107
public:

	virtual ~dbConnection() noexcept ;
	
	dbSQLConnection &getConnection() ;

	//! \returns The connectionParameters
	const dbConnectionParameters & connectionParameters() const ;
	void connectionParameters(const dbConnectionParameters &) ;

	const Memento &getStatus() const ;

    //! \return the transaction state \see dbTransactionState enum
	dbTransactionState transactionState();
	
	void transactionState(const dbTransactionState&) ;

	//! \return True if the connection is open
	bool isOpen(bool checkIsGood=true) ;
private:
    // only the dbConnectionPool and dbConnectionMgr can open and close
	//! Open the connection with the database
	bool open() ;      // PRQA S 2502

	//! Close the connection with the database
	bool close() ;      // PRQA S 2502
public:
	/** 
	 *Returns the text associated with last error number  \see getLastError()
	 */
    const std::string &getLastErrorText() const noexcept ;
	
	/** 
	 *Return true if connection is good
	 */
	bool isGood();
	
	/** 
	 * Increase counter _preventConnCheckCounter
	 */
	ssize_t inc_preventConnCheckCounter() ;

	/** 
	 * Decrease counter _preventConnCheckCounter
	 */
	ssize_t dec_preventConnCheckCounter() ;

	
private:
    static math::StatNode &statN(const std::string &key) ;
	/** 
	 *Returns the last error number \see getLastErrorText()
	 */
    long getLastError() const noexcept ;
public:    
	//! Set the lock mode. See the enum dbConnection::LockMode description for more details
	void setLockMode(LockMode) ;

	//! \return The lock mode
	LockMode getLockMode() const ;

	/** Set the default lock mode. Any dbConnection instance will get this value. 
	 *See the enum dbConnection::LockMode description for more details
	 */
	static void setDefaultLockMode(LockMode) ;

	/**
	 *Set the number of second to wait
	 *Used only if the lock mode is set to WaitForSeconds see dbConnection::LockMode
	 */
	void setLockWaitSeconds(int) ;

	/**
	 *Get the number of second to wait
	 *Used only if the lock mode is set to WaitForSeconds see dbConnection::LockMode
	 */
	int getLockWaitSeconds() const ;

	/**
	 *Set the number of second to wait for each step until the number returned from getLockWaitSeconds() is reached
	 *Used only if the lock mode is set to WaitForSeconds see dbConnection::LockMode
	 */
	void setSingleStepWaitSeconds(int) ;

	/**
	 *Get the number of second to wait for each step until the number returned from getLockWaitSeconds() is reached
	 *Used only if the lock mode is set to WaitForSeconds see dbConnection::LockMode
	 */
	int getSingleStepWaitSeconds() const ;

	/**
	 *Return the current key
	 */
	std::string getKey() const ;


	/**
	 * Set the number of second to wait. Every dbConnection will get this value
	 *
	 */
	static void setDefaultLockWaitSeconds(int) ;

	/**
	 * Get the number of second to wait. Every dbConnection will get this value
	 *
	 */
	static int getDefaultLockWaitSeconds() ;

	
	/**
	 * Set the number of second to wait for each step \see setSingleStepWaitSeconds(). Every dbConnection will get this value
	 *
	 */
	static void setDefaultSingleStepWaitSeconds(int) ;

	/**
	 * Get the number of second to wait for each step \see getSingleStepWaitSeconds().. Every dbConnection will get this value
	 *
	 */
	static int getDefaultSingleStepWaitSeconds() ;

	/**
	 *Add a state wrapper link to the connection.
	 *When a state wrapper is added its state is recorded.
	 *If the transaction is rolled-back the state will be restored.
	 *For example an object can save its state before operations like save or update.
	 *\see dbStateWrapper
	 *\see dbConnectionGuard::beginWork()
	 *\exception Throw a StateException if no transaction has been started.
	 */
	void addStateWrapper(dbStateWrapper &) ;


	/**
	 * set a resource. A transaction must be started.
	 * The resources can be used to excange information between
	 * objects during a complex transaction.
	 * \see dbConnectionGuard::beginWork()
	 * \exception Throw a StateException if no transaction has been started.
	 */
	template<class _Tp>
	void setResource(const std::string &, const _Tp &value) ;

	/**
	 *get a resource. A transaction must be started.
	 *\see dbConnectionGuard::beginWork()
	 *\exception Throw a StateException if no transaction has been started.
	 */
	template<class _Tp>
	void getResource(const std::string &, _Tp &value) const;


	/**
	 * Check and sets the WAIT FOR LOCK intervals and the number of retries that will be 
	 * performed in case a lock is found.
	 */
	void checkWaitingIntervals() ;

	/**
	 * sets the error text raised from an SQLConnection \see getLastError(), getLastErrorText()
	 */
	template <class T>
    std::string errorText(T &, const std::string &message="") ;

    std::string errorText(const std::string &s="") ;
	
    long setErrorCode(long errorCode) ;
	void setConnectionInternal(pthread_t, void *) ;
	void * getConnectionInternal() ;
	long long getLastSerial(const std::string &table, const std::string &stat="" ) ;
    
    static const int default_maxQueryDurationSecs = 10 ; // [secs]
	/**
	 * sets the interval that should cause a warning if any query should last more than the given interval
     * 
     * default is dbConnection::default_maxQueryDurationSecs 
     * any number <= 0 will cause no check on duration.
     *
     * \note the query is left unchanged - just a message will be notified
     * \param s the interval in secs 
	 */
    void setMaxQueryDurationSecs(int s) noexcept { _maxQueryDurationSecs = s ; } 
	/**
	 * returns the interval in secs that would cause a warning if any query should last longer.
	 */
    int  getMaxQueryDurationSecs() const noexcept { return _maxQueryDurationSecs ; } 

    bool hasAggregateTransactionState() const noexcept ; 
    

private:
    void deleteConnection() ;
	void setStatus(const Memento &) ;

	void beginWork( const std::string &transStatLabel = "" ) ;
	void commitWork() ;
	void rollBack() ;

	/*!
	 * Build the internal AggregateTransactionState
	 * This should be done at the beginning of a transaction
	 */
	void initAggregateTransactionState() ;

	/*!
	 * Delete the internal AggregateTransactionState
	 * This should be done at the ens of a transaction
	 */
	void clearAggregateTransactionState() noexcept ;

	/*!
	 * Update statistics of a named transaction
	 * This should be done at the end of a transaction
	 */
	void updateNamedTransStat(const std::string &, Timer &);
	
	dbConnection() = delete ;
	dbConnection(const dbConnection &) = delete ;
	dbConnection &operator=(const dbConnection &)  = delete ;
	dbConnection(dbConnection &&) = delete ;
	dbConnection &operator=(dbConnection &&)  = delete ;

private:
	dbAggregateTransactionState *_aggregateTransactionState ;
	dbConnectionParameters _connectionParameters ;

    dbSQLConnection _connection ; 
	void *_connectionInternal ;
	dbTransactionState _transactionState ;
    ssize_t _preventConnCheckCounter ;
	bool _connectionOpen ;
	bool _isGoodInProgress ;
	long _lastError ;
    std::string _lastErrorText ;
	static LockMode	_defaultLockMode ;

	Memento _status ;

	Timer	_openTimer ;
	Timer	_transTimer ;
	
	// Who change the following values MUST know what he's doing
	static int _defaultSingleStepSecondsToWait ;
	static int _defaultSecondsToWait ;
	static int _overrideDefaultSecondsToWait ;
	// Who change the previoues values MUST know what he's doing
	static bool _debugMsgEmitted ;

	static ThreadSafe _preventConnCheckMutex;
	
	// label of transaction for statistics
	std::string _transStatLabel;
	
	std::string _key ;
    
    int _maxQueryDurationSecs ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConnection) ;
} ;




template<class _Tp>
inline
void dbConnection::setResource(const std::string &s, const _Tp &value)
{
	if(!_aggregateTransactionState)
	{
		StateException e("_aggregateTransactionState == 0; A transaction must be started before using this method") ;
		ACS_THROW(e) ;
	}
	_aggregateTransactionState -> setResource( s, value) ;
}


template<class _Tp>
inline
void dbConnection::getResource(const std::string &s, _Tp &value) const
{
	if(!_aggregateTransactionState)
	{
		StateException e("_aggregateTransactionState == 0; A transaction must be started before using this method") ;
		ACS_THROW(e) ;
	}
	_aggregateTransactionState -> getResource( s, value) ;
}

template <class T>
inline
std::string dbConnection::errorText(T &t, const std::string &message)
{	
	std::string es = dbSQLGetError(t) ;

	_lastErrorText = message + es ; // store message 
 
    return es ;
}

std::string toString(dbConnection::LockMode) ;

std::ostream & operator << (std::ostream &, dbConnection::LockMode) ;
exostream & operator << (exostream &, dbConnection::LockMode) ; // PRQA S 2072


_ACS_END_NAMESPACE

#endif // _dbConnection_H_
