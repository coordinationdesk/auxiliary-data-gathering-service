// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA
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
	Revision 5.8  2017/11/21 18:43:45  marpas
	doc improved
	
	Revision 5.7  2016/07/21 07:11:14  nicvac
	CSGACQ-86: can set column name in the temporary table.
	
	Revision 5.6  2016/05/27 10:06:14  marfav
	Exec One Row is now deprecated
	
	Revision 5.5  2016/04/21 14:05:05  marpas
	statistics updated in upload/download clob/blob
	
	Revision 5.4  2014/03/12 12:40:55  marpas
	interface rationalization
	
	Revision 5.3  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.2  2013/07/09 12:04:30  marpas
	execForStatusRetry fixed and signature changed
	
	Revision 5.1  2013/07/09 11:53:45  marpas
	new methods: execForStatusRetry, execOneRowRetry, execForSetRetry added
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.11  2013/05/14 18:14:00  marpas
	default query statisticl label is not longer the emoty string but 'Unnamed'
	
	Revision 4.10  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.9  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.8  2013/04/04 15:35:08  marpas
	statistics improved
	
	Revision 4.7  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.6  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/04/02 11:12:27  marpas
	attempt to repeat queries - execForStatus - if some conditions are met into the db
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/10/18 17:23:46  marpas
	statistics improved
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.5  2011/05/04 12:21:23  lucio.pulvirenti
	removeTable method now public; DTOR cannot throw exception anymore
	
	Revision 3.4  2011/02/02 14:37:17  enrcar
	EC:: dbTemporaryTable class added (first release, not heavily tested yet)
	
	Revision 3.3  2010/09/11 17:48:01  marpas
	adoption of libDbExpress 3.2 interface
	
	Revision 3.2  2009/11/25 22:08:04  marpas
	useless forward declaration removed
	
	Revision 3.1  2009/10/08 10:50:13  marpas
	dbStat class removed and substituted by new libException.Statistics
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.9  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.8  2008/11/10 16:21:59  marpas
	dbCursor implemented
	
	Revision 2.7  2008/10/08 10:45:52  marpas
	dbQuery::getAffectedRows implemented and tested (at least partially on Ifx)
	
	Revision 2.6  2007/12/07 18:49:35  enrcar
	ENRCAR:: Private Query Methods: Will be referred as DBX (renamed with prefix: dbx_)
	Previous Public methods: Forced to share the same I/O interface. Will be called using a function pointer. Renamed with prefix: priv_
	Added new Public Query methods (wrap to private ones, the proper pointer will be returned from the dialect and executed).
	
	Revision 2.5  2007/12/04 10:16:36  crivig
	ENRCAR:: suggestedType handled in ExecForSet ExecOneRow
	
	Revision 2.4  2007/11/29 10:54:30  marpas
	statitistics improved, now queries can be labelled
	
	Revision 2.3  2007/11/28 12:06:26  marpas
	regression bug fixed
	
	Revision 2.2  2007/11/28 11:44:03  marpas
	private execForSet moved to privExecForSet. Its retrun code is not longer a boolean
	
	Revision 2.1  2006/12/13 18:13:10  paoscu
	Added methods to understand if a LO field is NULL.
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.17  2005/11/30 15:46:53  enrcar
	uploadBlobClob( bool ) , downloadBlobClob( bool ) methods added.
	Blob handling rewritten. Is also possible to upload/download LOBJs using an explicit SQL query (function call,etc).
	
	Revision 1.16  2005/10/28 08:27:38  enrcar
	updated to new interface for SQLDialect
	zeroBlobClob standard implementation added
	
	Revision 1.15  2005/10/26 14:09:56  enrcar
	CLOB handling added
	
	Revision 1.14  2005/10/14 12:23:58  enrcar
	unuseful method removed
	
	Revision 1.13  2005/09/29 16:38:18  enrcar
	QueryException added
	uploadBlob / downloadBlob methods added
	sqlstring stored as SQLString instead of std::string, in order to use it as container (for blob ops.)
	
	Revision 1.12  2005/09/12 12:00:09  enrcar
	row member receives parameter acceptEmptyRow in order to handle empty sets (for metadata extraction, etc)
	
	Revision 1.11  2005/09/08 12:09:49  paoscu
	member variable _error renamed into _success.
	CLOB download method under development (still not working)
	
	Revision 1.10  2005/08/11 07:54:06  marpas
	multiset patch removed due the fact the multiset is no longer supported by dbExpress approach
	
	Revision 1.9  2005/08/09 14:12:33  marpas
	getStatistics styatic method moved to dbStat class
	
	Revision 1.8  2005/08/08 19:15:42  marpas
	statistics simplemented but not yet fully tested.
	CLOB/BLOB management in progress
	
	Revision 1.7  2005/07/29 18:37:57  paoscu
	execForTablesMetaData methods added.
	
	Revision 1.6  2005/07/14 13:15:03  enrcar
	Added DBEXPRESS_COMPAT handling
	
	Revision 1.5  2005/07/08 16:23:47  enrcar
	Interface changed to handle SQLString
	
	Revision 1.4  2005/07/06 14:53:26  enrcar
	Added bool method --unimplemented
	
	Revision 1.3  2005/04/27 10:08:23  enrcar
	exec-query funtions moved from dbConnection
	
	Revision 1.2  2005/04/12 09:45:24  enrcar
	Merged with last version
	
	Revision 1.12  2005/02/16 14:48:22  marpas
	dbConnection::getStatistics patched.
	dbQuery statistics added
	dbConnection total wait time is now 30 sec, single step is 10 sec (5 if multithread).
	dbConnection: the env variable dbConnection_SingleStepWaitSecond can be used to override the default
	dbConnection: the env variable dbConnection_TotalWaitSecond can be used to override the default
	
	Revision 1.11  2004/10/25 08:30:47  marpas
	another patch for multiset queries
	
	Revision 1.10  2004/08/30 17:42:18  marpas
	dbConnection I/F changed to take into account the need to set each single step WAIT FOR SECONDS time
	and a total wait time
	dbQuery behaviour now uses the new dbConnection I/F for all three main methods
	dbQuery now implements a policy to avoid repeating the query if the returned error was not due to the timeout on a lock
	because single step lock granularity is 1 sec., if a single step returns with an error when less than half a second is elapsed, 
	the query is not longer repeated, but the exception is thrown.
	
	Revision 1.9  2004/07/29 09:37:53  integrator
	Retry query execution for N times and sleep between tries
	when in wait for seconds connection mode.
	Sleeping period grows at every cycle using random values.
	
	Revision 1.8  2004/05/31 16:15:17  marpas
	dbQuery execution method policy changed when the connection is in LockWaitSeconds mode
	
	Revision 1.7  2003/09/23 14:12:12  paoscu
	reset method removed.
	
	Revision 1.6  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.5  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.4  2003/03/07 10:16:05  achval
	working in progress
	
	Revision 1.3  2003/03/06 18:30:50  paoscu
	work in progress
	
	Revision 1.2  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#ifndef _dbQuery_H_
#define _dbQuery_H_
#include <acs_c++config.hpp>
#include <SQLString.h>
#include <dbException.h>
#include <dbStat.h>

#include <StopController.h>

#include <sys/types.h> // PRQA S 1013

//////////////////////////////////////////////
//              							// 
// class dbQuery							// 
//              							//
// this is the base class for all queries	//
//////////////////////////////////////////////

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbRow ;
class dbSet ;
class dbQueryParameters ;
class StopController ;

_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

class Timer ;
/*
 \class dbQuery : The class used to perform a query in Informix Database
 */		 

/*! 
 * \class dbTemporaryTable
 * Create a temporary table containing one column 
 */
class dbTemporaryTable // PRQA S 2109
{
public :
    enum ctypeEnum {
        INTEGER_TYPE,
        STRING
    } ;

	explicit dbTemporaryTable(dbConnection& conn, bool selfRemove=true) ;
	~dbTemporaryTable() throw() ;

	/* Set the column name - useful for example to join it with other queries */
	void setColumnName(const std::string& columnName);

    template<typename T>
    static std::string T2s_(const T & t) { std::ostringstream ss ;ss  << t ; return ss.str() ; }

    template<typename T>
    static ctypeEnum getCtype() { return INTEGER_TYPE ; }

    template<typename T>
    static std::string elTypeName() ;

    /*!
        vector<T> v ; ... createTable(v) ;
        set<T> s ; ... createTable(s) ;
        
    */
    template <class Container> 
	std::string createTable(const Container &c) {
        Timer timer ;
        timer.start() ;

	    acs::math::StatNode &stat = statN(_connection.getKey())["create <" + elTypeName<typename Container::value_type>() + "> [ms]"] ; // PRQA S 4412
	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
        _ctype = getCtype<typename Container::value_type>() ;
        try {
            std::string tableName = createTable() ;
            std::vector <std::string> v ; 
            std::back_insert_iterator< std::vector<std::string> > back_itv (v);
            std::transform(c.begin(), c.end(), back_itv, T2s_< typename Container::value_type > ) ;
            insertValues(v) ; 
            timer.stop() ;
            stat += double(timer.elapsed()) ; // PRQA S 3081 // PRQA S 3081
            stat["ok #"] +=1 ;   
	        return tableName ;
        }
        catch(std::exception &) {
            stat["error #"] +=1 ;   
            throw ;
        } 
    }

    void removeTable() ;	// Drop table

private :
    dbTemporaryTable() ;	// not implemented
    dbTemporaryTable(const dbTemporaryTable &) ;	// not implemented
    dbTemporaryTable& operator= (const dbTemporaryTable &) ;	// not implemented
    std::string createTableSQL () const ;
    void  debugDumpTableContent () ;    // DEBUG ONLY
    void insertValues(const std::vector<std::string>&) ;

	std::string createTable() ;	// Create actual table

    static std::string randomName (const std::string&) ;
    static std::string getDbType(enum ctypeEnum, const std::string&, ssize_t* pTypeLength=0) ;
    static math::StatNode &statN(const std::string &key) ;

private :		
	bool _tableCreated ;
    bool _selfRemove ;
	std::string _tableName ;
	std::string _columnName;
    ctypeEnum _ctype ;
	dbConnection &_connection ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(dbTemporaryTable) ;

} ;

template<>
inline
dbTemporaryTable::ctypeEnum dbTemporaryTable::getCtype<std::string>() { return STRING ; }

template<>
inline 
std::string dbTemporaryTable::T2s_(const signed char & t) { std::ostringstream ss ;ss << int(t) ; return ss.str() ; } // PRQA S 3081

template<> inline std::string dbTemporaryTable::elTypeName<std::string>() { return "string" ; }
template<> inline std::string dbTemporaryTable::elTypeName<signed char>() { return "signed char" ; }
template<> inline std::string dbTemporaryTable::elTypeName<short>() { return "short" ; }
template<> inline std::string dbTemporaryTable::elTypeName<int>() { return "int" ; }
template<> inline std::string dbTemporaryTable::elTypeName<long>() { return "long" ; }
template<> inline std::string dbTemporaryTable::elTypeName<long long>() { return "long long" ; }
template<> inline std::string dbTemporaryTable::elTypeName<unsigned char>() { return "unsigned char" ; }
template<> inline std::string dbTemporaryTable::elTypeName<unsigned short>() { return "unsigned short" ; }
template<> inline std::string dbTemporaryTable::elTypeName<unsigned int>() { return "unsigned int" ; }
template<> inline std::string dbTemporaryTable::elTypeName<unsigned long>() { return "unsigned long" ; }
template<> inline std::string dbTemporaryTable::elTypeName<unsigned long long>() { return "unsigned long long" ; }



class dbQuery // PRQA S 2109
{
	friend class dbCursor ; // PRQA S 2107
public:	


	/*! class QueryLockException declaration */
	exDECLARE_EXCEPTION(QueryLockException,dbException) ;   // PRQA S 2131, 2153, 2502

	/*! class QueryExecuteException declaration */
	exDECLARE_EXCEPTION(QueryExecuteException,dbException) ;   // PRQA S 2131, 2153, 2502

	/*! class QueryException declaration */
	exDECLARE_EXCEPTION(QueryException,dbException) ;   // PRQA S 2131, 2153, 2502

	/*! class QueryNoDataFound declaration */
	exDECLARE_EXCEPTION(QueryNoDataFound,dbException) ;   // PRQA S 2131, 2153, 2502
  
	/*!
		Constructor
		\param conn a dbConnection used to manage the connection
		\param doStat would perform statistics 
	 */
	explicit dbQuery(dbConnection &conn, bool doStat = true) ;

	/*!
		Constructor
		\param conn a dbConnection used to manage the connection
		\param q an std::string containing the SQL command 
		\param queryStatLabel the name of the query for statistics
        \param doStat would perform statistics 
        
	 */
    dbQuery(dbConnection &conn, 
			const SQLString& q, 
			const std::string &queryStatLabel, 
            bool doStat = true) ;

	//! Destructor
	virtual	~dbQuery() throw() ;
	//! Copy constructor
	dbQuery(const dbQuery &) ;

	dbQuery &operator=(const dbQuery &) ;

	//! \return The SQL std::string
	virtual std::string sqlString() const ;

	//! Set the SQL String
	virtual void sqlString(const SQLString &, const std::string &statLabel) ;

	const std::string &queryStatLabel() const ;

	dbConnection &connection() const ;

	//! \return The error std::string (see Informix Documentation for codes)
	std::string errorString() const ;
	//! \return The Error Status (see Informix Documentation for codes)
	std::string errorState() const ;
	//! \return True if an error has occurred
	bool error() const ;


    /*! In case the connection (see dbConnection) was a WaitForSeconds one, the method will perform a number of 
		attempts until the timeout expires (see dbConnection::setLockMode and dbConnection::setLockWaitSeconds)
		\brief Perform a query with no result
		This function has to be used to perform query that have no result.
		Example : DELETE FROM table WHERE id = 1
    */
    virtual bool execForStatus() ;
    virtual void execForStatusRetry(StopController &sc, 
                                size_t msec, 
                                unsigned short retriesMax) ;


	/*! \brief Perform a query with a single row result \deprecated
		This function has to be used to perform query that have only one result row.
		Example : SELECT COUNT(*) FROM table
    */
    virtual void  __attribute__((deprecated))  execOneRow(dbRow & , 
        					const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(), 
                            bool acceptEmptyRow=false ) ;

    virtual void __attribute__((deprecated))  execOneRowRetry(StopController &sc, 
                                size_t msec, 
                                unsigned short retriesMax,
                                dbRow & , 
        					    const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(), 
                                bool acceptEmptyRow=false ) ;

	/*! In case the connection (see dbConnection) was a WaitForSeconds one, the method will perform a number of 
		attempts until the timeout expires (see dbConnection::setLockMode and dbConnection::setLockWaitSeconds)
		\brief Perform a query with a result set
		This function has to be used to perform query that have a result set.
		Example : SELECT * FROM table
        If the set is empty, it has an empty row attribute (\see dbSet::getEmptyRow) to be used to know column types it would have returned.
    */
    virtual void execForSet(dbSet &, 
                            const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>() ) ;

    
    virtual void execForSetRetry(StopController &sc, 
                                size_t msec, 
                                unsigned short retriesMax, 
                                dbSet& set, 
                                const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>()) ; 

     /*! In case the connection (see dbConnection) was a WaitForSeconds one, the method will perform a number of 
		attempts until the timeout expires (see dbConnection::setLockMode and dbConnection::setLockWaitSeconds)
		\brief Perform a query with no result
		This function has to be used to perform query that have no result.
		Example : DELETE FROM table WHERE id = 1
	 */
	void privExecForStatus(dbSet &, 
            			   bool&,
                           const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(), 
                           bool acceptEmptyRow=false ) ;

	/*! In case the connection (see dbConnection) was a WaitForSeconds one, the method will perform a number of 
		attempts until the timeout expires (see dbConnection::setLockMode and dbConnection::setLockWaitSeconds)
		\brief Perform a query with a single row result
		This function has to be used to perform query that have only one result row.
		Example : SELECT COUNT(*) FROM table
	 */
	void privExecOneRow(dbSet &, 
            			bool&,
                        const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(), 
                        bool acceptEmptyRow=false ) ;

	/*! In case the connection (see dbConnection) was a WaitForSeconds one, the method will perform a number of 
		attempts until the timeout expires (see dbConnection::setLockMode and dbConnection::setLockWaitSeconds)
		\brief Perform a query with a result set
		This function has to be used to perform query that have a result set.
		Example : SELECT * FROM table
	 */

	void privExecForSet(dbSet &, 
            			bool&,
                        const std::vector<dbSpecificColumnType::Type>& suggestedTypes=std::vector<dbSpecificColumnType::Type>(), 
                        bool acceptEmptySet=false ) ;



	virtual bool blobIsNull() ;
	virtual bool clobIsNull() ;
	virtual void downloadBlob() const ;
	virtual void downloadBlobClob( bool ) const ;
	virtual void downloadClob() const ;
    
    /*! sets the affected rows */
	virtual void uploadBlob() ;
    /*! sets the affected rows */
	virtual void uploadBlobClob( bool ) ;
    /*! sets the affected rows */
	virtual void uploadClob() ;
    /*! sets the affected rows */
	virtual void zeroBlob() ;
    /*! sets the affected rows */
	virtual void zeroClob() ;
	size_t getAffectedRows() const ;
    static math::StatNode &statN(const std::string &key) ;

protected:
	std::string getKey() const ;

private:
	enum ErrorType {
		NoError = 0,
		PrepareError = 1,
		ExecuteError = 2,
		EmptyCursor = 3
	} ;



	void blobClobIsNull(const std::string&, const std::string&, const std::string&, bool& ) ; 
    bool dbxExecForStatus(const std::string &, bool &shouldRepeat) ;
 	ErrorType dbxExecForSet(const std::string &, dbSQLCursor& ) ;
	bool execForTablesMetaData(const std::string &tableName , dbSQLCursor& ) ;
    void zeroBlobClob(const std::string&, const std::string&, const std::string& ) ; 
    void updateStat(math::StatNode &, const std::string &) const ;
    void updateStat2(math::StatNode &, const std::string &, const std::string &) const ;
	void updateStat(math::StatNode &, Timer &) const ;
	void updateStat(math::StatNode &, size_t) const ;
	dbQuery() ; // default - not implmented
private:

	dbConnection &_connection ;
	SQLString   _sqlString ;    /* Default ctor will set type to: QUERY_UNDEFINED */  
    std::string _queryStatLabel ;
    std::string _errorString ;
	std::string _errorState ;
	bool	    _success ;
	int         _affectedRows ;
    bool        _stat ;

	static const size_t _defaultMsecCycleDelay = 100 ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbQuery) ;
} ;

_ACS_END_NAMESPACE

#endif // _dbQuery_H_
