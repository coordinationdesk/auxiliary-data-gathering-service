/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

    Revision 5.14  2017/11/21 18:46:22  marpas
    execForSet will add to the returned set an empty row \see dbSet::getEmptyRow. this can be used to know number, names and types of columns when no data is returned
    
    Revision 5.13  2017/10/03 14:28:42  lucio.pulvirenti
    S3MPM-885: dbTemporaryTable CTOR: dbConnection::isOpen method called with checkIsGood boolean parameter set to false.
    
    Revision 5.12  2016/10/20 10:18:14  marpas
    qa warning fixed
    
    Revision 5.11  2016/07/21 07:11:13  nicvac
    CSGACQ-86: can set column name in the temporary table.
    
    Revision 5.10  2016/04/21 14:05:05  marpas
    statistics updated in upload/download clob/blob
    
    Revision 5.9  2016/03/14 15:11:24  marpas
    removing deprecated class (dbConnectionGuard)
    
    Revision 5.8  2014/03/12 12:40:55  marpas
    interface rationalization
    
    Revision 5.7  2014/02/07 18:36:53  marpas
    ACS_THROW and ACS_COND_THROW macros adopted
    
    Revision 5.6  2014/02/06 15:03:16  clanas
    removed GLOBAL since useless for postgresql from version 8.1
    
    Revision 5.5  2013/12/19 12:24:34  marpas
    Transaction class added
    
    Revision 5.4  2013/07/09 12:04:30  marpas
    execForStatusRetry fixed and signature changed
    
    Revision 5.3  2013/07/09 11:53:45  marpas
    new methods: execForStatusRetry, execOneRowRetry, execForSetRetry added
    
    Revision 5.2  2013/06/19 18:12:08  marpas
    removing compilation warning over deprecated include files
    
    Revision 5.1  2013/06/13 18:10:54  marpas
    libException 5.x standards adoption finished
    interface rationalization
    removing useless code
    coding best practices added
    some performances improvements.
    test fixed
    
    Revision 5.0  2013/06/06 18:52:59  marpas
    adopting libException 5.x standards
    
    Revision 4.24  2013/06/03 12:39:44  marpas
    coding best practices partially applied
    qa rules
    
    Revision 4.23  2013/05/14 18:14:00  marpas
    some qa warning removed
    
    Revision 4.22  2013/04/09 07:53:08  marpas
    ./NameIdAssociator refactoring
    new statistics
    
    Revision 4.21  2013/04/05 12:45:53  marpas
    Statistics are now grouped for db key
    
    Revision 4.20  2013/04/04 15:35:07  marpas
    statistics improved
    
    Revision 4.19  2013/02/26 17:12:24  marpas
    coding best practice appplied
    interface rationalization
    
    Revision 4.18  2013/01/24 19:27:58  marpas
    query stat strings fixed
    exception notification improved
    useless debug macros removed
    some stats added
    
    Revision 4.17  2012/12/14 18:38:54  marpas
    coding best practices
    qa rules
    
    Revision 4.16  2012/12/11 17:30:12  marpas
    qa rules
    
    Revision 4.15  2012/11/19 13:30:20  marpas
    HEAD REALIGNEMENT
    
    Revision 4.12  2012/04/03 08:46:18  lucio.pulvirenti
    privExecForStatus: if query to repeat, warning turned into debug message
    
    Revision 4.11  2012/04/02 11:12:27  marpas
    attempt to repeat queries - execForStatus - if some conditions are met into the db
    
    Revision 4.10  2012/02/17 13:55:14  marpas
    refactoring
    multiple db design supported
    less compilation warning
    
    Revision 4.9  2012/02/13 17:37:18  marpas
    refactoring in progress
    
    Revision 4.8  2011/12/22 13:51:59  lucio.pulvirenti
    Debug improved
    
    Revision 4.7  2011/12/12 14:48:30  lucio.pulvirenti
    EC:: privExecForSet: In case of error when fetching the next cursor, the error text was asked to the connection (wrong). Now it will be obtained by the cursor itself.
    
    Revision 4.6  2011/12/07 15:32:32  lucio.pulvirenti
    Debug improved
    
    Revision 4.5  2011/10/25 09:17:47  marpas
    statistics improved
    
    Revision 4.4  2011/10/24 17:38:47  marpas
    temporary teble stats update
    
    Revision 4.3  2011/10/20 13:45:00  lucio.pulvirenti
    Statistics improved
    
    Revision 4.2  2011/10/18 17:23:46  marpas
    statistics improved
    
    Revision 4.1  2011/10/18 14:58:00  enrcar
    EC::LP:: If error found during a transaction, the transaction status of connection is set to ERROR
    
    Revision 4.0  2011/10/18 12:31:14  marpas
    new statistics adopted
    
    Revision 3.11  2011/05/31 10:04:50  lucio.pulvirenti
    debug improved
    
    Revision 3.10  2011/05/24 13:24:22  lucio.pulvirenti
    debug modified after addition of bool parameter to dbConnection::isOpen() method
    
    Revision 3.9  2011/05/04 12:34:16  lucio.pulvirenti
    DTOR cannot throw exception anymore
    removeTable: no more exc thrown if table not created
    
    Revision 3.8  2011/02/21 10:37:14  lucio.pulvirenti
    EC:: dbTemporaryTable. Bug fixed in ORACLE DROP TEMPORARY TABLE
    
    Revision 3.7  2011/02/03 16:46:56  enrcar
    EC:: randomName method improved (previous was a temporary draft)
    
    Revision 3.6  2011/02/02 14:37:17  enrcar
    EC:: dbTemporaryTable class added (first release, not heavily tested yet)
    
    Revision 3.5  2010/10/14 18:01:39  marpas
    Statistics for queries for connection added
    they are total good and total errors, and then subdivided into
    execForSet, execForStatus and execOneRow groups along with min, max and average
    
    Revision 3.4  2010/09/27 13:05:42  lucio.pulvirenti
    EC:: updated to handle new TableList private interface
    
    Revision 3.3  2010/09/11 18:04:07  marpas
    libDbExpress 3.2 interface adopted
    libException 3.3 interface adopted
    
    Revision 3.2  2010/08/19 09:43:35  marpas
    for each of the 3 main functions, implements a lock for the whole methods when a database
    cannot do a time wait for lock. uses specific dialect feature and an appropriate dbLockGuard ctor.
    
    Revision 3.1  2009/10/08 10:50:13  marpas
    dbStat class removed and substituted by new libException.Statistics
    
    Revision 3.0  2009/10/05 14:47:51  marpas
    getting rid of db_values table and all parameters name with one string (was deprecated)
    now dbParam can be constructed with tab/col pair (or just 2 strings)
    efficiency improved and some little bugs removed
    
    Revision 2.15  2009/06/22 12:36:35  marpas
    robustness improved
    got rid of useless ifdef and warning
    useless code removed
    test compilation fixed
    
    Revision 2.14  2009/06/19 17:47:57  marpas
    dbConnection used to speed up work and to avoid multiple connection when collecting data
    
    Revision 2.13  2008/11/20 14:19:12  marpas
    error messages fixed
    
    Revision 2.12  2008/11/10 16:21:59  marpas
    dbCursor implemented
    
    Revision 2.11  2008/10/08 10:45:52  marpas
    dbQuery::getAffectedRows implemented and tested (at least partially on Ifx)
    
    Revision 2.10  2008/05/08 16:37:53  enrcar
    EC:: COUNT(*): A function is used instead of a string
    
    Revision 2.9  2007/12/11 13:15:13  enrcar
    EC:: privExecForSet modified to allow empty sets (if a parameter is given). Used to handle MySQL commands (return an empty set even if successfully)
    
    Revision 2.8  2007/12/07 18:49:35  enrcar
    ENRCAR:: Private Query Methods: Will be referred as DBX (renamed with prefix: dbx_)
    Previous Public methods: Forced to share the same I/O interface. Will be called using a function pointer. Renamed with prefix: priv_
    Added new Public Query methods (wrap to private ones, the proper pointer will be returned from the dialect and executed).
    
    Revision 2.7  2007/12/04 10:16:36  crivig
    ENRCAR:: suggestedType handled in ExecForSet ExecOneRow
    
    Revision 2.6  2007/11/29 11:17:45  marpas
    stats improved again
    
    Revision 2.5  2007/11/29 11:09:40  marpas
    stats improved
    
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
    
    (... Older log messages removed ...)
    
*/

#include <acs_c++config.hpp>

#include <SQLDialect.h>
#include <dbQuery.h>
#include <dbStat.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbCursor.h>
#include <SQLString.h>
#include <dbConnectionPool.h>
#include <dbConnection.h>
#include <Timer.h>
#include <exStream.h>
#include <File.h>

#include <SQLDialectFactory.h>
#include <SQLDialect.h>

#include <dbQueryParameters.h>

#include <Filterables.h>

#include <cstdlib>
#include <ctime>
#include <sys/types.h> 
#include <unistd.h>

#include <signal.h>
#include <SigalrmEmulator.h>

#include <set>
#include <random>
#include <thread>
#include <chrono>

/*
 * SIGALARM WRAPPER
 * used to raise a signal to interrupt the database polls / selects on stale sockets
 */

// A convenience signal (user defined)
#define CONN_GUARD_SIGNAL (SIGRTMIN+6)

// Unnamed namespace
namespace {

class StaleConnectionSignalDispatcher : 
    public acs::pattern::Singleton <StaleConnectionSignalDispatcher>
{

public:
    // Will never be called. No at_exit is foreseen.
    ~StaleConnectionSignalDispatcher() {}
    
    // Do not copy, assign or move
    StaleConnectionSignalDispatcher& operator= (const StaleConnectionSignalDispatcher&) = delete;
    StaleConnectionSignalDispatcher& operator= (StaleConnectionSignalDispatcher&&) = delete;
    StaleConnectionSignalDispatcher (const StaleConnectionSignalDispatcher&) = delete;
    StaleConnectionSignalDispatcher (StaleConnectionSignalDispatcher&&) = delete;
    
    static void sigalarmEmulatorCallback (void* p)
    {
        reinterpret_cast<StaleConnectionSignalDispatcher*>(p)->internalTimerCallback();
    }
    
    void registerQuery (int threshold, const std::string& name, const std::string& dbKey, bool doStat)
    {
        pthread_t threadId = ::pthread_self(); 
        // Prepare the descriptor
        QueryDescriptor qd;
        qd.tickThreshold = threshold;
        qd.queryName = name;
        qd.dbKey = dbKey;
        qd.doStat = doStat;

        // Critical zone
        {
            acs::ThreadSafe::Lock lock (_mutex);
            // Register the new query in the internal structure
            _registeredQueries[threadId] = qd;
        }
    }
    
    void unregisterQuery ()
    {
        pthread_t threadId = ::pthread_self();
        // Critical zone
        {

            bool haveToNotify = false;
            QueryDescriptor qd;
            int longQueryCount = 0;

            {
                acs::ThreadSafe::Lock lock (_mutex);
                if (0 != _longQueries.count(threadId))
                {
                    haveToNotify=true;
                    qd = _registeredQueries[threadId];
                    _longQueries.erase(threadId);
                    longQueryCount = _longQueries.size();
                }
                // Unregister the query
                _registeredQueries.erase(threadId);
            }

            // Notify and manage long queries
            if (haveToNotify)
            {
                ACS_LOG_INFO ("The named query \"" << qd.queryName << "\" exceeding its timeout returned after " << 
                              (qd.tickCounter+1) << " seconds.");
                // Clear the longQueries structure now
                ACS_LOG_INFO ("Current number of queries exceeding the configured timeout is " << longQueryCount );
                if (qd.doStat) { acs::dbQuery::statN(qd.dbKey)["instances #"]["longquery #"]+=1 ;}
            }


        }
    }

    void internalTimerCallback()
    {
        // Accumulate messages to notify in the log streams without any locked mutex
        std::vector < std::string > messages;
        int longQueryCounter = 0;

        // Critical section.. do not talk from inside it
        {
            // send signals to all the registered threads 
            // the handlers are dummy, so no worries about raising signals with a locked mutex
            acs::ThreadSafe::Lock lock (_mutex);

            // Loop on all the registered threads
            for (auto it=_registeredQueries.begin(); it!=_registeredQueries.end(); ++it)
            {
                // Just keep track of the current managed thread id in a simpler way
                pthread_t currentThread = it->first;
                QueryDescriptor& query = it->second;

                // raise the signal
                ::pthread_kill(currentThread, CONN_GUARD_SIGNAL);

                // increment the tick counter
                ++(query.tickCounter);

                // manage the warning emission if needed
                if (( query.tickThreshold > 0) && ( 0 == _longQueries.count(currentThread) ))
                {
                    // A threshold exists and the warning has not been emitted yet
                    if (query.tickCounter > query.tickThreshold)
                    {
                        // Accumulate messages and notify without the mutex locked
                        // Time to emit the warning
                        _longQueries.insert(currentThread);
                        longQueryCounter = _longQueries.size();
                        std::ostringstream os;
                        os << "The named query \"" << query.queryName << "\" exceeded its timeout of " << query.tickThreshold << " seconds";
                        messages.push_back (os.str());
                    }
                }
            }
        }

        // Emit messages if needed
        if (0 != longQueryCounter)
        {
            for_each (messages.begin(), messages.end(), [](const std::string& s) { ACS_LOG_WARNING (s);} );
            ACS_LOG_INFO ("Current number of queries exceeding the configured timeout is " << longQueryCounter );
        }

    
    }

private:

    struct QueryDescriptor
    {
        QueryDescriptor() :
            tickCounter(0),
            tickThreshold(0),
            queryName(),
            doStat(false),
            dbKey()
        {}

        int tickCounter;
        int tickThreshold;
        std::string queryName;
        bool doStat;
        std::string dbKey;
    };

    StaleConnectionSignalDispatcher() :
        _mutex(),
        _alarmEmulator(),
        _registeredQueries(),
         _longQueries()
    {
        // At ctor time just activate the sigalarm emulator thread and forget about it
        _alarmEmulator.signal(sigalarmEmulatorCallback, this);
        // Set the callback frequency
        struct itimerval newtimer;
        newtimer.it_value.tv_sec = 1;
        newtimer.it_value.tv_usec = 0;
        newtimer.it_interval.tv_sec = 1;
        newtimer.it_interval.tv_usec = 0;
        _alarmEmulator.setitimer (0, &newtimer, 0);
        // Done.. just wait it now
    }
    
    // Signleton friendship
    friend StaleConnectionSignalDispatcher* acs::pattern::Singleton< StaleConnectionSignalDispatcher >::instance();

    // Attributes and encapsulation
    acs::Mutex _mutex;
    acs::SigalrmEmulator _alarmEmulator;
    std::map <pthread_t, QueryDescriptor> _registeredQueries;
    std::set <pthread_t> _longQueries;
};

class StaleConnectionGuard
{
public:
    StaleConnectionGuard(const StaleConnectionGuard&) = delete;
    StaleConnectionGuard& operator=(const StaleConnectionGuard&) = delete;
    
    StaleConnectionGuard(int timeout, const std::string& queryName, const std::string& dbKey, bool doStat):
        _handlerInstalled(false),
        _oldaction(),
        _newaction()
    {
        memset(&_oldaction, 0, sizeof(_oldaction)) ;
        memset(&_newaction, 0, sizeof(_newaction)) ;
        _newaction.sa_handler = dummySignalHandler;
        if (0 == sigaction (CONN_GUARD_SIGNAL, &_newaction, &_oldaction) )
        {
            _handlerInstalled = true;
            // At the signal action reset, do not restore the original handler that causes an abort if called
            // force the use of SIG_IGN handler to prevent the reception of buffered signals that can cause 
            // a crash if received after the sigaction in the destructor
            _oldaction.sa_handler = SIG_IGN;
            StaleConnectionSignalDispatcher::instance()->registerQuery(timeout, queryName, dbKey, doStat);
        }
        else
        {
            ACS_LOG_WARNING ("Unable to install the signal handler in StaleConnectionGuard. Guard is disabled.");
        }
    }
    
    virtual ~StaleConnectionGuard() noexcept
    {
        if (_handlerInstalled)
        {
            StaleConnectionSignalDispatcher::instance()->unregisterQuery();
            sigaction (CONN_GUARD_SIGNAL, &_oldaction, 0); // reset handler AFTER unregistering .. it will be SIG_IGN
        }
    }

    static void dummySignalHandler(int) 
    {
        // This function does nothing and we want it remains as it is
        // it is used just to cause the exit of interruptable system calls
    }

private:
    bool _handlerInstalled;
    struct sigaction _oldaction, _newaction;   
};

} // end of the unnamed namespace


//////////////////////
//                  //
// class dbQuery    //
//                  //
//////////////////////

_ACS_BEGIN_NAMESPACE(acs)

using acs::math::StatNode ;
using std::string ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbTemporaryTable)

StatNode &dbTemporaryTable::statN(const std::string &key) { return db::stat(key)["dbTemporaryTable"] ; }

dbTemporaryTable::dbTemporaryTable(dbConnection& conn, bool selfRemove) :
    _tableCreated(false),
    _selfRemove(selfRemove),
    _tableName(),
    _columnName(),
    _ctype(),
    _connection (conn) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    const bool checkIsGood(false) ;
    if (! _connection.isOpen(checkIsGood) )
    {
        (statN(_connection.getKey())["instances #"]+=1)["failed conn #"] += 1 ;
        ACS_THROW( exIllegalValueException( "dbTemporaryTable::dbTemporaryTable  Connection not open!" ) ); 
    }
    
    (statN(_connection.getKey())["instances #"]+=1)["created #"] += 1 ;
    
    
}


dbTemporaryTable::~dbTemporaryTable() noexcept
{

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "tableCreated: " << std::boolalpha << _tableCreated << "  selfRemove: " << _selfRemove) ;

    try
    {        
        if ( _tableCreated && _selfRemove ) { removeTable() ; }
        statN(_connection.getKey())["instances #"]["destroyed #"]+=1 ;
    }
    catch(const std::exception &x)
    {
        ACS_LOG_NOTIFY_EX(x);    // DO _NOT_ THROW EXCEPTIONS IN D'TOR !
        statN(_connection.getKey())["instances #"]["destroy error #"]+=1 ;
    }

}

void dbTemporaryTable::setColumnName( const string& columnName ) { 
    _columnName = columnName;
}

//
//  dbTemporaryTable::createTable (compose and execute the query)
//
string dbTemporaryTable::createTable() 
{
    Timer timer ;
    timer.start() ;
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
    StatNode &stat = statN(_connection.getKey())["createTable [ms]"] ; 

    if (_tableCreated)
    {
        (stat["error #"]+=1)["table exists #"]+=1 ;
        ACS_THROW( exIllegalValueException( "dbTemporaryTable::dbTemporaryTable  Table already created !" ) ); 
    }

    //
    //  Obtain DB-Dependand parameters :
    //
    bool retrySupported = false ;       // TRUE only if DB shares temporary tables between connections. So a temp.table might altready exist in another connection
    unsigned short maxAttempts = 0 ;    // Number of attemps (1-rel) to re-create a new temporary table, if the previous name was already used.
    
    const string &dialectName = SQLDialectFactory::dialectName(_connection.getKey()) ;
    
    // Database-specific: a temporary table might already exist in the same session [DBSPEC]
    if ( dialectName == "Oracle11" )  
    {
        retrySupported = true ;
        maxAttempts = 5 ;   
    }
    else
    {
        // Temporary tables are private for each session
        retrySupported = false ;
        maxAttempts = 0 ; // Not supported !
    }
    
    unsigned short attempts = 0;    
    
    do
    {
        _tableName = randomName(dialectName) ;    // Generate a random name for the table
        string sqlQuery = createTableSQL() ;

        SQLString s(_connection.getKey(), SQLString::COMMAND) ;
        s.forceSQLString(sqlQuery) ;
        dbQuery q(_connection, s, "tmp table creation") ;
     
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creating new query for SQLString: " << s.getSQL() ) 

        try
        {
            q.execForStatus () ;
            _tableCreated = true ;    // Success !
        }
        catch(dbQuery::QueryExecuteException &e) 
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation of table: " << _tableName << "  Obtained QueryExecuteException: " << e)  ;
       
            if (retrySupported)
            {
                 (stat["error #"]+=1)["retries #"]+=1 ;
                
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying again, attempt: " << attempts+1 << " / " << maxAttempts) ;
                
                // wait 10ms, in order to increase timer entropy 
                // (a new table name will be randomly generated)
                std::this_thread::sleep_for (std::chrono::milliseconds(10)) ;
            }
            else    
            {
                (stat["error #"]+=1)["fatal #"]+=1 ;
                throw ;
            }
        }
        catch(const std::exception &)
        {
            (stat["error #"]+=1)["fatal"]+=1 ;
            throw ;
        }

    }
    while (!_tableCreated && retrySupported && (++attempts < maxAttempts)) ; 
    
    if (!_tableCreated)
    {
        (stat["error #"]+=1)["retries exceeded #"]+=1 ;
        ACS_THROW( exIllegalValueException() << "Unable to create temporary table after: " << attempts << " attempts"); 
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Temporary table " << _tableName << " successfully created at attempt #" << attempts)  ;
    timer.stop() ;
    stat += double(timer.elapsed()) ; 
    stat["ok #"]+=1 ;
    return _tableName ;    // Success
}

namespace {
    // any db has the same separator (up to now) so the input dialect parameter is not checked
    void getSeparators(const string &dialect, dbTemporaryTable::ctypeEnum c, string &L, string &R) {
        switch(c)
        {
        case dbTemporaryTable::INTEGER_TYPE: { L = R = "" ; break ; }
        case dbTemporaryTable::STRING: { L = R = "'" ; break ; }
        default:
            {
                ACS_THROW( exIllegalValueException() << "CType not handled (" << int(c)<< ')');
            }
        } // switch
    }
    
    string getInsertQuery(const string &dialect, const string &tblname) 
    {
           // Prepare DB-specific query: [DBSPEC]
        if ( dialect == "Informix9" ) {
            return "INSERT INTO " + tblname + " SELECT * FROM TABLE(list{ "  ;
        }
        else if ( dialect == "Postgres8" ) {
            return "INSERT INTO " + tblname + " VALUES "  ;
        }
        else if ( dialect == "Oracle11" ) {
            return "INSERT ALL" ;
        }
        else {
            ACS_THROW( exIllegalValueException( "SQL Dialect not handled: " + dialect ) ); 
        }
    }

    string getInsertQueryTail(const string &dialect)  
    {
       //
        // Finish DB-specific query: [DBSPEC]
        if ( dialect == "Informix9" ) { 
            return " }) " ;
        }
        else if ( dialect == "Postgres8" )  {
            return " " ;
        }
        else if ( dialect == "Oracle11" )  {
            return " SELECT * FROM DUAL " ;
        }
        else {
            ACS_THROW( exIllegalValueException( "SQL Dialect not handled: " + dialect ) ); 
        }
    }
    
    string getInsertQueryElem(const string &dialect,  
                              size_t i, 
                              const string &L, 
                              const string &R,
                              const string &el,
                              const string &table)
    {
        // i is also a first time indicator: BOOL FIRST_TIME = (i == 0)

        //
        // Build DB-specific query: [DBSPEC]
        if ( dialect == "Informix9" ) {
            return ((i != 0) ? ", " : "") + L + el + R ; 
        }
        else if ( dialect == "Postgres8" ) {
            return ((i != 0)? ", " : "") + string("(") + L + el + R + ")"; 
        }
        else if ( dialect == "Oracle11" ) {
            return " INTO " + table + " VALUES (" + L + el + R + ")" ;
        }
        else {
            ACS_THROW( exIllegalValueException( "SQL Dialect not handled: " + dialect ) ); 
        }
    }

}
//
//  dbTemporaryTable::insertValues
//
void dbTemporaryTable::insertValues(const std::vector<string> & v) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    Timer timer ;
    timer.start() ;
    StatNode &stat = statN(_connection.getKey())["insertValues [ms]"] ; 

    if (!_tableCreated)
    {
        (stat["error #"]+=1)["no table #"]+=1 ;
        ACS_THROW( exIllegalValueException( "Table not created" ) ); 
    }


    if (v.empty())
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Nothing to do")  ;
        return ; 
    }

    size_t max_length = 0 ;    // bytes for the single INSERT query (approx.) E.g. use 4000 for 4kb
    string sepL ;
    string sepR ;
    ssize_t strLen = 0 ;
    
    const string &dialect = SQLDialectFactory::dialectName(_connection.getKey()) ;    

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_ctype: " << int(_ctype))  ;

    getDbType(_ctype, dialect, &strLen) ;    // Get value strLen (maximum lenght allowed for a string, or -1 if undefined)

    //
    // Set DB-specific limits and string formatters: [DBSPEC]
    if ( dialect == "Informix9" ) 
    {
        max_length = 60*1024;    /* 2011-02-01  -  Max: 64Kb */ 
        getSeparators(dialect,_ctype, sepL, sepR) ;
    }
    else if ( dialect == "Postgres8" ) 
    {
        // 2011-02-01 EC:: Actual limit is probably tens of MEGABYTES or more. 
        // I was able to successfully execute a 1.5 MB single query using our DbExpress/AcsDialects engine
        max_length = 128*1024 ;    
        getSeparators(dialect,_ctype, sepL, sepR) ;
    }
    else if ( dialect == "Oracle11" ) 
    {
        // 2011-02-01 - Max: 64Kb  EC:: It turned out that Oracle is very slow to 
        // execute multiple inserts, so limit is lowered to 8kb */
        max_length = 8*1024 ;    
        getSeparators(dialect,_ctype, sepL, sepR) ;
    }
    else {
        ACS_THROW( exIllegalValueException( "SQL Dialect not handled: " + dialect ) ); 
    }

    // Execute INSERT statements
    size_t n = 0; 
    size_t vsize = v.size() ;
    while (n<vsize) 
    {
    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inserting elements starting from position: " << n )
    
        string sqlQuery = getInsertQuery(dialect, _tableName) ;

        size_t i = 0 ; // used also as "first time" indicator
        while ( (n<vsize) && (i<max_length) && ((i+v[n].length())<max_length) ) 
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Size: " << i << " Inserting element number: " << n << " !" << v[n] << "!") ;

            // Check if string can be contained into the TEMP table. Ignored for non-string tables
            ACS_COND_THROW( (strLen > 0 ) && ( ssize_t(v[n].length()) > strLen), 
                                        exIllegalValueException( "dbTemporaryTable::dbTemporaryTable  INTERNAL ERROR  string too long" ) );

            sqlQuery += getInsertQueryElem(dialect, i, sepL, sepR, v[n], _tableName) ;

            n++ ; 
            i = sqlQuery.length() ;
            
        } ;  // while ( (n<v.size()) && (i<max_length) )
    
        sqlQuery += getInsertQueryTail(dialect) ;
       
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query of size: " << sqlQuery.length() << " bytes: " << "\t!" << sqlQuery << "!") ;
    
        SQLString s(_connection.getKey(), SQLString::COMMAND ) ;
        s.forceSQLString(sqlQuery) ;
        dbQuery q(_connection, s, "tmp table ins values") ;
        
        try {
            q.execForStatus() ;
        } catch (const std::exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
            (stat["error #"]+=1)["execForStatus #"]+=1 ;
            ACS_THROW(exIllegalValueException(x,"Error executing SQL command. Query string is: " +  s.getSQL()) ) ; 
        }
    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Insert was successfully. Items so far: " << n << " / " << v.size()) ;
    } ;    /* END while (n<v.size()) */


    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) debugDumpTableContent() ;    ACS_CLASS_END_DEBUG // DEBUG ONLY

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. Successfully inserted: " << n << " items") ;
    timer.stop() ;
    stat += double(timer.elapsed()) ; 
    stat["ok #"]+=1 ;
}


//
//  dbTemporaryTable::removeTable
//
void dbTemporaryTable::removeTable() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tableCreated: " << std::boolalpha << _tableCreated ) ;
    Timer timer ;
    timer.start() ;
    StatNode &stat = statN(_connection.getKey())["removeTable [ms]"] ; 

    if ( ! _tableCreated )
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Nothing to do") ;
                
        return ;
    }

    std::vector<string> sqlQuery ;

    //
    // DB-specific DROP TEMPORARY TABLE query: [DBSPEC]
    const string &dialect = SQLDialectFactory::dialectName(_connection.getKey()) ;

    if ( dialect == "Informix9" ) 
    {   sqlQuery.push_back("DROP TABLE " + _tableName) ; }
    else if ( dialect == "Postgres8" ) 
    {  sqlQuery.push_back("DROP TABLE " + _tableName) ; }
    else if ( dialect == "Oracle11" ) 
    {
        sqlQuery.push_back("TRUNCATE TABLE " + _tableName) ; 
        sqlQuery.push_back("DROP TABLE " + _tableName) ;    // O/w the command will fail (ORA-14452)
        // EC:: I found from other sources the command:
        // DROP TABLE  _tableName  CASCADE CONSTRAINTS  (...?)
    }
    else {
        ACS_THROW( exIllegalValueException( "dbTemporaryTable::insertValues  SQL Dialect not handled: " + dialect ) ); 
    }

                
    SQLString s( _connection.getKey(), SQLString::COMMAND ) ;
    try {
        size_t qsize = sqlQuery.size() ;
        for (size_t i=0; i<qsize; ++i)
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing sql command #" << (i+1) << "/" << sqlQuery.size() << ": !" << sqlQuery[i] << "!")  
            
            s.forceSQLString(sqlQuery[i]) ;
            dbQuery q(_connection, s, "tmp table drop") ;

            q.execForStatus () ;    // Execute DROP TEMPORARY TABLE
        }    // for

        _tableCreated = false ; // Prevent further removals
        
    } catch (const std::exception &x) { // NOSONAR - any exception
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,x) ;
        (stat["error #"]+=1)["execForStatus #"]+=1 ; 
        ACS_THROW(exIllegalValueException(x,"Error executing SQL command. Query string is: " +  s.getSQL()) ) ; 
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished. Table: " << _tableName << " removed successfully") ;
    timer.stop() ;
    stat += double(timer.elapsed()) ; 
    stat["ok #"]+=1 ;
}

 
//
//  dbTemporaryTable::createTableSQL
//
std::string dbTemporaryTable::createTableSQL() const 
{    
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    statN(_connection.getKey())["createTableSQL #"]+=1 ;


    string sqlQuery = "" ;
    string columnName = "e" ;    /* Short column name, "e" for entry */
    if ( ! _columnName.empty() ) {
        columnName = _columnName;
    }

    const string &dialect = SQLDialectFactory::dialectName(_connection.getKey()) ;
    
    // Compose Db-specific SQL code for creating the Temporary table  [DBSPEC]
    if ( dialect == "Informix9" ) 
    {
        sqlQuery += "CREATE TEMP TABLE " + _tableName + " ( " 
            + " " + columnName + " " + getDbType(_ctype, dialect)
            + " ) WITH NO LOG " ;
    }

    else if ( dialect == "Postgres8" ) 
    {
        sqlQuery += "CREATE TEMPORARY TABLE " + _tableName + " ( " 
            + " " + columnName + " " + getDbType(_ctype, dialect)
            + " ) ON COMMIT PRESERVE ROWS " ;
    }
    
    else if ( dialect == "Oracle11" ) 
    {
        sqlQuery += "CREATE GLOBAL TEMPORARY TABLE " + _tableName + " ( " 
            + " " + columnName + " " + getDbType(_ctype, dialect)
            + " ) ON COMMIT PRESERVE ROWS " ;
    }
    
    else {
        ACS_THROW( exIllegalValueException( "SQL Dialect not handled: " + dialect ) ); 
    }
 
     ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Method finished: !" << sqlQuery << '!') ;
   
    return sqlQuery ;
}

 
//
//  dbTemporaryTable::debugDumpTableContent
//
void dbTemporaryTable::debugDumpTableContent() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    ACS_COND_THROW( !_tableCreated, exIllegalValueException( "dbTemporaryTable::dbTemporaryTable  Table not created" ) ); 


    SQLString s(_connection.getKey(), SQLString::SELECT ) ;
    s.addSelectList("*");
    s.addTableList(_tableName);
    
    dbQuery q(_connection, s, "SEL tmp " + _tableName) ;

    dbSet set ;
    q.execForSet(set) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Number of rows: " << set.size()) ;
    
    size_t scount = set.size() ;
    for (size_t i=1; i<=scount; ++i)
    {
        dbRow row(_connection.getKey()) ;
        set.getRow(i, row) ;
        dbColumn col = row.column(0) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, i << ":\t" << col ) ; 
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished") ;
}


//
//  STATIC  dbTemporaryTable::randomName
//
string dbTemporaryTable::randomName(const string& )
{    
/*
    Table name random pattern is: TT_aaaaaabbbbbcccccc
        TT: Temporary Table
        aaaaaa: seconds from epoch, last 24 bit (resolution: 2^24 seconds, i.e. 194 days)
        bbbbb:  usec 
        cccccc:  24 bit random number
        Total: 3+(6+5+6) = 20 bytes
*/

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    struct timeval tv ; 

    unsigned int r[3] ; // NOSONAR 

    ACS_COND_THROW( 0 != gettimeofday(&tv, nullptr), exIllegalValueException( "dbTemporaryTable::randomName  Error calling gettimeofday") ) ; 
    
    // Initialize random number generator
    timeval tim; 
    gettimeofday(&tim, 0);

    std::ranlux24 generator ; // 24 bit random generator NOSONAR - not sec sensitive
    generator.seed(tim.tv_sec + tim.tv_usec) ; // generator seed

    
    r[0] = tv.tv_sec & 0x00ffffff ;    // Second from Epoch -- lower 24 bit are used (resolution: 194 days)
    r[1] = tv.tv_usec & 0x000fffff  ;    // usec: range is 0-1e6 i.e. <= 20 bit
    r[2] = generator() & 0x00ffffff; // 24 bit random number
    
    std::ostringstream os ; 
    os << std::uppercase << std::hex ;
    os << "TT_" << std::setw(6) <<std:: setfill('0') << std::right << r[0]
                << std::setw(5) << std::setfill('0') << std::right << r[1]
                << std::setw(6) << std::setfill('0') << std::right << r[2] ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished: !" << os.str() << "!") ;

    return os.str() ;
}


//
//  STATIC  dbTemporaryTable::getDbType
//
string 
    dbTemporaryTable::getDbType(enum ctypeEnum ctype, const string& dialectName, ssize_t* pTypeLength) 
{
    std::ostringstream t ; 
    ssize_t l = -1 ;
            
    // Compose Db-specific SQL datatype  [DBSPEC]
    switch (ctype)
    {
    case INTEGER_TYPE: 
        if ( dialectName == "Informix9" ) {
            t << "INT8" ; 
        }
        else if ( dialectName == "Postgres8" ) {
            t << "BIGINT" ; 
        }
        else if ( dialectName == "Oracle11" ) {
            t << "NUMBER" ; 
        }
        else {
            ACS_THROW( exIllegalValueException("dbTemporaryTable::getDbType:  Dialect not handled: " + dialectName) ); 
        }

        break ;
     case STRING: 
        if ( dialectName == "Informix9" )
        {    l = 1024; /* Max: 32739 */ t << "LVARCHAR(" << l << ")" ; } 
        else if ( dialectName == "Postgres8" )
        {    l = 1024; /* Max: <~ 1GB */ t << "VARCHAR(" << l << ")" ; } 
        else if ( dialectName == "Oracle11" )
        {    l = 1024; /* Max: 4000 */ t << "VARCHAR2(" << l << ")" ; } 
        else {
            ACS_THROW( exIllegalValueException("dbTemporaryTable::getDbType:  Dialect not handled: " + dialectName) ); 
        }

        break ;
    default:
        ACS_THROW( exIllegalValueException() << "ctype not handled: " << int(ctype)); 
    }

    if (pTypeLength) { *pTypeLength = l ; }
    return t.str() ;
}


        
ACS_CLASS_DEFINE_DEBUG_LEVEL(dbQuery)

StatNode &dbQuery::statN(const std::string &key) { return db::stat(key)["dbQuery"] ; }

dbQuery::dbQuery(dbConnection &conn, bool doStat) :
    _connection(conn), 
    _sqlString(conn.getKey(), SQLString::QUERY_UNDEFINED),
    _queryStatLabel(),
    _errorString(),
    _errorState(),
    _success(true),
    _affectedRows(),
    _stat(doStat)
{
    if (_stat) { statN(_connection.getKey())["instances #"]["created #"]+=1 ;}
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Conn ptr: " << std::hex << &_connection) ;
}


dbQuery::dbQuery(dbConnection &conn, const SQLString &q, const string &queryStatLabel, bool doStat) :
    _connection(conn), 
    _sqlString( q ),
    _queryStatLabel(queryStatLabel),
    _errorString(),
    _errorState(),
    _success(true),
    _affectedRows(),
    _stat(doStat)
{
    if (_stat) { statN(_connection.getKey())["instances #"]["created #"]+=1 ; }
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Conn ptr: " << std::hex << &_connection) ;
}



dbQuery::~dbQuery() noexcept
{

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
    try { // no exception out of dtor
        if (_stat) { statN(_connection.getKey())["instances #"]["destroyed #"]+=1 ; }
    }
    catch(const std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

}


dbQuery::dbQuery(const dbQuery &dbq) :
    _connection(dbq._connection), 
    _sqlString(dbq._sqlString),
    _queryStatLabel(dbq._queryStatLabel),
    _errorString(),
    _errorState(),
    _success(true),
    _affectedRows(),
    _stat(dbq._stat) 
{
    if (_stat) { statN(_connection.getKey())["instances #"]["created #"]+=1 ; }
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
}



dbQuery &dbQuery::operator=(const dbQuery &dbq)
{
    if (this != &dbq) { // note don't copy the connection

        // copy the query
        _sqlString = dbq._sqlString ;
        _queryStatLabel = dbq._queryStatLabel ;
        _errorString = dbq._errorString ;
        _errorState = dbq._errorState ;
        _success = dbq._success ;
        _affectedRows = dbq._affectedRows ;
        // do not coy stat filter - so if I created a query to emit statistics it will 
        // always do even if another silent one is assigned
        // _stat = dbq._stat ;

    }

    return *this ;
}


string dbQuery::getKey() const
{
    return _connection.getKey() ; 
} 


bool dbQuery::execForStatus() 
{
    dbSet fakeSet ;
    bool retVal = false ;    /* Will be set by the called method */   
    std::vector<dbSpecificColumnType::Type> fakeVector ;
    bool acceptEmptySet=true ;    /* No effect if privExecForStatus is actually called */

    SQLDialect::ptrExecSQL ptr = SQLDialectFactory::sqlDialect(getKey()).returnExecForStatus(_sqlString, &dbQuery::privExecForStatus, &dbQuery::privExecForSet) ;

    (*this.*ptr)(fakeSet, retVal, fakeVector, acceptEmptySet);

    return retVal ;
}


void dbQuery::execForSet(dbSet& dbset, const std::vector<dbSpecificColumnType::Type>& suggestedTypes ) 
{
    bool retVal = false ;    /* Will be set by the called method */     
    bool acceptEmptySet=false ;    /* Cause no regression if privExecForSet is called, and will be ignored by privExecForStatus */

    SQLDialect::ptrExecSQL ptr = SQLDialectFactory::sqlDialect(getKey()).returnExecForSet(_sqlString, &dbQuery::privExecForSet, &dbQuery::privExecForStatus) ;


    (*this.*ptr)(dbset, retVal, suggestedTypes, acceptEmptySet);

    return ;
}

size_t dbQuery::execForSet(dbSet & set,
                           dbConnectionPool& pool,
                           const SQLString& q,
                           const std::string &queryStatLabel,
                           bool doStat,
                           const std::vector<dbSpecificColumnType::Type>& suggestedTypes )
{
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
    return execForSet(set, conn, q, queryStatLabel, doStat, suggestedTypes);
}

size_t dbQuery::execForSet(dbSet & set,
                           dbConnection& conn,
                           const SQLString& q,
                           const std::string &queryStatLabel,
                           bool doStat,
                           const std::vector<dbSpecificColumnType::Type>& suggestedTypes )
{
    std::string label = queryStatLabel;
    if (doStat && queryStatLabel.empty())
    {
        ACS_LOG_WARNING("Empty query stat label with stat enabled, using 'unknown'");
        label = "unknown";
    }

    // Setup query
    dbQuery query(conn, q, label, doStat);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << q.getSQL());

    // Execute query
    query.execForSet(set);

    return set.size();
}

void dbQuery::execOneRow(    dbRow & dbrow, 
                            const std::vector<dbSpecificColumnType::Type>& suggestedTypes, 
                            bool acceptEmptyRow ) 
{
    dbSet dbset ;
    bool retVal = false ;    

    SQLDialect::ptrExecSQL ptr = SQLDialectFactory::sqlDialect(getKey()).returnExecForSet(_sqlString, &dbQuery::privExecOneRow, &dbQuery::privExecForStatus) ;

    // call the method returned
    (*this.*ptr)(dbset, retVal, suggestedTypes, acceptEmptyRow);

    if (dbset.size() == 1) 
    {
        dbset.getRow(1, dbrow) ;
    }

    return ;

}


void dbQuery::privExecOneRow(
    dbSet &dbset, 
    bool& retVal,
    const std::vector<dbSpecificColumnType::Type>& suggestedTypes, 
    bool acceptEmptyRow // If TRUE, no exception will be raised when query returns no row
)
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Conn ptr: " << std::hex << &_connection << std::dec 
            << " connection: " << (!_connection.isOpen(false) ? "not " : "") << "open"
            << "  About to execute query: \"" << _sqlString.getSQL() << "\"" );
    StatNode &stat = _stat ? (statN(_connection.getKey())+=1)["execOneRow"]+=1 : statN(_connection.getKey())["execOneRow"] ; 
    
    
    // check the waiting intervals against multithreads
    
    _connection.checkWaitingIntervals() ;

    dbSQLCursor itcrow ;
    bool downloadRow = true ;
    retVal = false ;    /* Will be set TRUE only in case of success */
    
     Timer timer ;

     for (timer.start();;Timer::randomDelay(_defaultMsecCycleDelay)) {
        
         Timer stepTimer ;
        try {

            stepTimer.start() ;
            ErrorType eType = dbxExecForSet(_sqlString.getSQL(),itcrow) ;
            if (eType != NoError) {


                // BEHAVIOUR IS UNCHANGED!!!!!!!!!!!!!

#if(0)
                //
                //  TODO: DIALECT MUST BE USED TO HANDLE THIS SITUATION
                //
                //
                // Warning: 
                // Behaviour is changed.
                // In order to handle INSERT into DB, mysql requires
                // ExecForSet instead of ExecForStatus
                // and NO CURSOR is returned.
                // So, no exception should be returned in this case:
                downloadRow = false ;
                break ; // exit for
                //
                //  TODO: DIALECT MUST BE USED TO HANDLE THIS SITUATION
                //
#endif

                // BEHAVIOUR IS UNCHANGED!!!!!!!!!!!!!


                _errorString = _success ? _connection.getLastErrorText() : string("") ; 
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, " query.execForSet: " << _errorString
                        << "Connection mode: " << _connection.getLockMode() ) ;

                if (_connection.transactionState() == dbConnection::Begin) { 
                    _connection.inc_preventConnCheckCounter() ;    // Increase the counter _preventConnCheckCounter
                    _connection.transactionState(dbConnection::Error) ;            // Set transaction state as Error
                }    

                if (eType == PrepareError) {
                    updateStat2(stat,"error #","db error #") ; 

                    ACS_THROW(QueryExecuteException(string("dbQuery: ") + " " + _errorString + " Query is : " + _sqlString.getSQL() )) ; 
                }
                else { // should be an ExecuteError or EmptyCursor
                    updateStat2(stat,"error #","lock #") ; 
                    ACS_THROW(QueryLockException() << "Conn ptr: " << std::hex << &_connection << " " <<"dbQuery: Lock Error " << _errorString << " Query is : " << _sqlString.getSQL() ) ; 
                }
            }
            /*
                Get Row
                */
            long commandStatus = SQLDialect::SQLReturn(getKey(), itcrow->next()) ;

            if (commandStatus == SQL_SUCCESS )
            {
               /* Nothing to do - command successfull. */
               break ; // end loop
            }
            else if (commandStatus == DBXERR_EOF )
            {
                // EOF Found - This is not an error, but there are no more data
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Scan finished. Got status: " << commandStatus 
                        << " ("  << _connection.errorText(itcrow) << ")") ;

                if (!acceptEmptyRow) {
                    itcrow.reset();     /* In order to raise exception below */
                }
                else {
                    downloadRow = false ;  /* No exception will be raised, but row won't be downloaded */
                }
                break ; // end loop
            }
            else
            {
                /*
                    HERE IF: (commandStatus != SQL_SUCCESS) AND (commandStatus != DBXERR_EOF).

                    When a table is locked, (generic) error SQL_ERROR is raised.

                    (TODO: check other databases for lock-error...
                    A switch can be introduced:
                    SQL_ERROR -> locked
                    OTHER -> not locked, so the error should be emitted).
                */
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Conn ptr: " << std::hex << &_connection << std::dec
                        << " CommandStatus: " << commandStatus
                        << " Error: " << _connection.errorText(itcrow)) ;
                updateStat2(stat,"error #","lock #") ; 
                ACS_THROW(QueryLockException() << "dbQuery: Lock Error Got status: " << commandStatus   
                                               << " Error: " << _connection.errorText(itcrow) 
                                               << " Query is : " + _sqlString.getSQL() ) ;
            }
            break ;
        }
        catch(QueryLockException &e) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Exception got " << e) ;
            timer.stop() ;
            stepTimer.stop() ;
            if (_connection.getLockMode() == dbConnection::WaitForSeconds) {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Elapsed: " << timer.elapsed() << " Step duration: " << stepTimer.elapsed())  ;

                 if (long(timer.elapsed()) > _connection.getLockWaitSeconds() * 1000) { 
                    updateStat2(stat,"error #","timeout (total steps) #") ; 
                     throw ;
                }
                else {
                    updateStat(stat,"retries #") ; 
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Still: " << _connection.getLockWaitSeconds() * 1000 -  timer.elapsed() << " [msec] remaining") ; 
                }
            }
            else {
                updateStat2(stat,"error #","other #") ; 
                throw ;
            }
        }
    } // FOR    

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "query.execOneRow: returned") ;
    

// Should be triggered using dialect (mysql should NOT emit this exception
// since several queries (for example: insert into) return
// an empty set, EVEN IF SUCCESSFULL
    if(!itcrow.get())
    {
        updateStat2(stat,"error #","no data #") ; 
        ACS_THROW(QueryNoDataFound(string("dbQuery: no data found. Query is: ") + _sqlString.getSQL())) ; 
    }

    try {
        dbRow row(getKey()) ;
    
        // If row is empty and acceptEmptyRow is set, no row will be downloaded
        // but the internal SQLCursor of row will be set anyway
        // This can be useful for metadata extraction, etc.
        row.row(itcrow, suggestedTypes, downloadRow) ; /* row is downloaded from DB if not-null */

        /* The obtained row is inserted into given set */
        std::vector <dbRow> vect ;    
        vect.push_back(row) ;    /* fake vector, just to insert the row into set */
        dbset.set(vect) ;
        
        retVal = true ;
    }
    catch (const std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        updateStat2(stat,"error #","row conversion #") ;
        ACS_THROW(QueryExecuteException(x,string("Conversion to dbRow error. Query string is: ") +  _sqlString.getSQL()) ) ; 
    }

    timer.stop() ;
    
    updateStat(stat, timer) ;

    return ;
}


void dbQuery::privExecForStatus(dbSet &fakeSet, 
                                bool& retVal,
                                const std::vector<dbSpecificColumnType::Type>& suggestedTypes, 
                                bool acceptEmptyRow) 
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "connection:  ptr " << &_connection << "  " << (!_connection.isOpen(false) ? "not " : "") << "open" 
           << "  About to execute query: \"" << _sqlString.getSQL() << "\"") ;
    
    StatNode &stat = _stat ? (statN(_connection.getKey())+=1)["execForStatus"]+=1 : (statN(_connection.getKey()))["execForStatus"] ; 

    // check the waiting intervals against multithreads
    _connection.checkWaitingIntervals() ;
    
    retVal = false ;    /* Will be set TRUE only in case of success */
     Timer timer ;

    Timer repeatTimer ; 
    bool shouldRepeat = false ;
    repeatTimer.start() ;
     for (timer.start();;timer.randomDelay(_defaultMsecCycleDelay)) {

         Timer stepTimer ;        
        try {
            stepTimer.start() ; // start the timer
            retVal = dbxExecForStatus(_sqlString.getSQL(), shouldRepeat) ;
            _success = retVal ;
            if (! _success) {
                _errorString = _connection.getLastErrorText() ;    
            }
            else {
                _errorString = "" ;
            }

            if (! _success) {

                ssize_t prev_counter = -1 ; 
                if (_connection.transactionState() == dbConnection::Begin) { 
                    prev_counter = _connection.inc_preventConnCheckCounter() ;    // Increase the counter _preventConnCheckCounter
                    _connection.transactionState(dbConnection::Error) ;            // Set transaction state as Error
                }
            
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "query.execForStatus: \"" <<  _connection.getLastErrorText() << "\"" 
                        << "_preventConnCheckCounter: " << ((_connection.transactionState() == dbConnection::Error) ? prev_counter : 0)) ;
                // throw an exception
                ACS_THROW(QueryExecuteException(string("dbQuery: ") + " " +  
                                        _connection.getLastErrorText() + " " + 
                                        string(" Query is : ") + _sqlString.getSQL() )) ;

            }
            break ;
        }
        catch(QueryExecuteException &e) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Exception got " << e) ;
            timer.stop() ;
            stepTimer.stop() ;
            repeatTimer.stop() ;
            if (shouldRepeat && (repeatTimer.elapsed() < 60000) ) { 
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Query repeat condition met. Since first " << repeatTimer.elapsed() << " [ms]. Exception was " << e) ;
                // nothing to do ... just repeat
                updateStat(stat,"repeat #") ;
            }
            else if (_connection.getLockMode() == dbConnection::WaitForSeconds) {
                // check if there was some timeout allowed
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Elapsed: " << timer.elapsed() 
                       << " Step duration: " << stepTimer.elapsed())  ;
                // how long ?? 
                if (_connection.getSingleStepWaitSeconds() * 500 > long(stepTimer.elapsed())) { 
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Step duration: " << stepTimer.elapsed() 
                           << " SSTep Wait: " << _connection.getSingleStepWaitSeconds()*1000 
                           << " throwing exception" ) ;
                    updateStat2(stat,"error #","timeout (short response time) #") ;
                    throw ;
                }
                // timeout ?
                 if (long(timer.elapsed()) > _connection.getLockWaitSeconds() * 1000) { 
                    updateStat2(stat,"error #","timeout (total steps) #") ;
                     throw ;
                }
                else { // still some time to retry
                    updateStat(stat,"retries #") ;
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Still: " << _connection.getLockWaitSeconds() * 1000 -  timer.elapsed() << " [msec] remaining" )  ; 
                }
            }
            else {
                updateStat2(stat,"error #","other #") ; // no lock mode .. other errors 
                throw ;
            }
        }
    } // FOR
   
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "query.execForStatus: returned")  ;

    // stat update 
    timer.stop() ;
    updateStat(stat, timer) ;
}


void dbQuery::privExecForSet(dbSet &set, 
                            bool& retVal,
                            const std::vector<dbSpecificColumnType::Type>& suggestedTypes,
                            bool acceptEmptySet) 
{
    ACS_COND_THROW(!&_connection, QueryException("Null connection")) ; 

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "acceptEmptySet: " << std::boolalpha << acceptEmptySet << std::boolalpha 
            << " connection: ptr " << &_connection << "  " << (!_connection.isOpen(false) ? "not " : "") << "open"
            << "  About to execute query: \"" << _sqlString.getSQL() << "\"" ) ;
    StatNode &stat = _stat ? (statN(_connection.getKey())+=1)["execForSet"]+=1 : (statN(_connection.getKey()))["execForSet"]; 

    // check the waiting intervals against multithreads
    _connection.checkWaitingIntervals() ;
    
    Timer timer ;

    dbSQLCursor itset ;
    std::vector <dbRow> rowVector ;
    retVal = false ;    /* Will be set TRUE only in case of success */

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "starting cycle") ;
     for (timer.start();;timer.randomDelay(_defaultMsecCycleDelay)) {
         Timer stepTimer ;
        try {
            stepTimer.start() ;
            ErrorType eType = dbxExecForSet(_sqlString.getSQL(), itset);

            if ( eType == NoError ) {
                _success = true ;
            }
            else if ( (eType == EmptyCursor) && (acceptEmptySet) ) {
                _success = true ;
            }
            else {
                _success = false ;
            }

            _errorString = !_success ? _connection.getLastErrorText() : string("") ; 

            if (!_success) {

                ssize_t prev_counter = -1 ; 
                if (_connection.transactionState() == dbConnection::Begin) { 
                    prev_counter = _connection.inc_preventConnCheckCounter() ;    // Increase the counter _preventConnCheckCounter
                    _connection.transactionState(dbConnection::Error) ;            // Set transaction state as Error
                }

                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, " query.execForSet: \"" << _errorString << "\"" 
                       << " _preventConnCheckCounter: " << ((_connection.transactionState() == dbConnection::Error) ? prev_counter : 0)) ;

                if (eType == PrepareError) {
                    updateStat2(stat,"error #","db error #") ;
                    ACS_THROW(QueryExecuteException(string("dbQuery: ") + " " + _errorString + " Query is : " + _sqlString.getSQL() )) ; 
                }
                else { // should be an ExecuteError or EmptyCursor
                    updateStat2(stat,"error #","lock #") ;
                    ACS_THROW(QueryLockException(string("dbQuery:") + " Lock Error " + _errorString + " Query is : " + _sqlString.getSQL() )) ; 
                }
            }


            /*
                Set is filled-up
            */

            long commandStatus = 0 ;
            rowVector.clear();

            bool done = (eType == EmptyCursor) ;    /* If Cursor is empty, no scan has to be performed. */

            while (!done)
            {
                commandStatus = SQLDialect::SQLReturn(getKey(), itset->next()) ;
                if (commandStatus == SQL_SUCCESS )
                {
                    dbRow row(getKey()) ;
                    row.row(itset, suggestedTypes, true);    /* row is downloaded */
                    rowVector.push_back(row);
                }
                else if (commandStatus == DBXERR_EOF )
                {
                    // EOF Found - This is not an error, but there are no more data
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Scan finished. Got status: DBXERR_EOF" 
                            << " (" << _connection.errorText(itset) << ")") ;

                    done = true ;

                    dbRow row(getKey()) ; 

                    // If row is empty and acceptEmptyRow is set, no row will be downloaded
                    // but the internal SQLCursor of row will be set anyway
                    // This can be useful for metadata extraction, etc.
                    row.row(itset, suggestedTypes, false) ; /* row is downloaded from DB if not-null */
                    set.setEmptyRow(row) ;
                }
                else
                {
                    /*
                        HERE IF: (commandStatus != SQL_SUCCESS) AND (commandStatus != DBXERR_EOF).

                        When a table is locked, (generic) error SQL_ERROR is raised.
                        So, flag locked is set TRUE below.

                        (TODO: check other databases for lock-error...
                        A switch can be introduced:
                        SQL_ERROR -> locked
                        OTHER -> not locked, so the error should be emitted).
                        */

                        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Conn ptr: " << std::hex << &_connection << std::dec
                                << " CommandStatus: " << commandStatus
                                << " Error: " << _connection.errorText(itset)) ;

                        // exception

                        updateStat2(stat,"error #","lock#") ;
                        ACS_THROW(QueryLockException() << "dbQuery: Lock Error Got status: " << commandStatus  
                                            << " Error: " << _connection.errorText(itset) 
                                            << " Query is : " << _sqlString.getSQL() ) ;        

                }    // END: if (commandStatus == SQL_SUCCESS )
            }   /* while (!done) */

            break ; // good query itset valid ;
        }   /* try */
        catch(QueryLockException &e) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Exception got " << e) ;
            timer.stop() ;
            stepTimer.stop() ;
            if (_connection.getLockMode() == dbConnection::WaitForSeconds) {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Elapsed: " << timer.elapsed() 
                       << " Step duration: " << stepTimer.elapsed() )  ;

                if (_connection.getSingleStepWaitSeconds() * 500 > long(stepTimer.elapsed())) { 
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Step duration: " << stepTimer.elapsed() 
                           << " SSTep Wait: " << _connection.getSingleStepWaitSeconds()*1000 
                           << " throwing exception") ;
                    updateStat2(stat,"error #","timeout (short response time) #") ;
                    throw ;
                }

                 if (int(timer.elapsed()) > _connection.getLockWaitSeconds() * 1000) { 

                    updateStat2(stat,"error #","timeout (total steps) #") ;
                     throw ;
                }
                else {
                    updateStat(stat,"retries #") ;
                    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Still: " << _connection.getLockWaitSeconds() * 1000 -  timer.elapsed() << " [msec] remaining" ) ; 
                }
            }
            else {
                updateStat2(stat,"error #","other #") ;
                throw ;
            }
        }   /* catch */
    } // FOR


    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "query.execForSet: returned" ) ;

    try {
        set.set(rowVector) ;
        updateStat(stat, rowVector.size()) ;
        retVal = true ;
    }
    catch (const std::exception &x) {

        /* itset->Release() ; */
        
        updateStat2(stat,"error #","set building #") ;
        ACS_THROW(QueryExecuteException(x,string("Query Execution error. Command is: ") +  _sqlString.getSQL())) ; 
    }

    timer.stop() ;
    updateStat(stat, timer) ;
}    /* privExecForSet */


void dbQuery::downloadBlobClob( 
            bool isBlob // TRUE: is a Blob. FALSE is a Clob
                              ) const
{
    using std::pair ;  
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;
    pair <SQLString, string> commandAndFileName = _sqlString.getCommandAndFileName() ;
     
    string tableName ;
    string whereCondition ;
    string columnName ;
    string fileName ;
    SQLString sqlCommand(getKey(), SQLString::QUERY_UNDEFINED) ;

        
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for " << (isBlob? "BLOB": "CLOB" )) ; 

    if ( columnAndFileName.first.empty() && (commandAndFileName.first.getType() == SQLString::QUERY_UNDEFINED) )
    {
        ACS_THROW(QueryException( "Required field unset")) ; 
    }

    if ( not columnAndFileName.first.empty() && ( commandAndFileName.first.getType() != SQLString::QUERY_UNDEFINED) )
    {
        ACS_THROW(QueryException( "Both column and command set" )) ; 
    }

    if ( not columnAndFileName.first.empty() )
    {
        // Column is set
        std::vector < pair<string, string> > tableList = _sqlString.getTableList() ;

        // check sqlstring type: must be LOBJ_DOWNLOAD
        ACS_COND_THROW( _sqlString.getType() != SQLString::LOBJ_DOWNLOAD, 
                                    QueryException( "Wrong SQLString type" )
                                  ) ;
        if ( tableList.size() != 1 ) // only one table allowed
        {
            ACS_THROW(QueryException() << "ONE and ONLY ONE table must be provided for blob ops. (current size: " << tableList.size() << "should be: 1)") ; 
        }

        ACS_COND_THROW( columnAndFileName.first.empty() || columnAndFileName.second.empty(), 
                                   QueryException( "Column and FileName both required for blob ops and not set" )
                                 ) ;
 
         // prepare params
        tableName = tableList[0].first ;
        whereCondition = _sqlString.getWhereParameters() ;
        columnName = columnAndFileName.first ;
        fileName = columnAndFileName.second ;


        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n" 
            << "\ttableName: " << tableName << "\n" 
            << "\tcolumnName: " << columnName << "\n"
            << "\tfileName: " << fileName << "\n" 
            << "\twhereCondition: " << whereCondition ) ;
    
    }
    else
    {
        // SQL Command is set
        
        // fileName can be null. In this case, pre-check will be skipped
        sqlCommand = commandAndFileName.first ;
        fileName = commandAndFileName.second ;

        ACS_COND_THROW( sqlCommand.getType() == SQLString::QUERY_UNDEFINED, 
                                    QueryException( "Command not set" )
                                  ) ;
         ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "fileName: " << fileName ) ;
    
    }
    
    // PRE-CHECK
    // Check if filename already exists
    ACS_COND_THROW( not fileName.empty() && File::exists( fileName ), 
                                QueryException("PRE-CHECK FAILED: File " + fileName + " already exists!") 
                              ) ;
    
    if ( not fileName.empty() )
    {
        // OK -- File doesn't exist yet
        
        // Write test
        try 
        {
            File::touch( fileName );
            
            // If still here: OK -- remove file after touch:
            File::remove( fileName );
        }
        catch(const std::exception &x)
        {
            ACS_THROW(QueryException(x, string("PRE-CHECK FAILED: File ") + fileName + " cannot be written!" )) ; 
        }
    }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "PRE-CHECK SUCCESSFULL for file: " << fileName) ;
  
    StatNode &stat = _stat ? (statN(_connection.getKey())+=1)["LO download"]+=1 :(statN(_connection.getKey()))["LO download"] ; 
    Timer timer ;
    timer.start() ;
    try {
        if ( not columnAndFileName.first.empty() )
        {   
            // Column set

            if (isBlob)
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).downloadBlob(5 params)") ;
                SQLDialectFactory::sqlDialect(getKey()).downloadBlob(&_connection, tableName, columnName, whereCondition, fileName) ;
            }
            else
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).downloadBlob(5 params)") ;
                SQLDialectFactory::sqlDialect(getKey()).downloadClob(&_connection, tableName, columnName, whereCondition, fileName) ;
            }
        }
        else
        {
            // Command set

            if (isBlob)
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).downloadBlob(3 params)") ;
                SQLDialectFactory::sqlDialect(getKey()).downloadBlob(&_connection, sqlCommand, fileName ) ;
            }
            else
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).downloadClob(3 params)") ;
                SQLDialectFactory::sqlDialect(getKey()).downloadClob(&_connection, sqlCommand, fileName  ) ;
            }
        }
    }
    catch(const std::exception &) {
        updateStat2(stat,"error #","") ;
        throw ;
    }
    timer.stop() ;
    updateStat(stat, timer) ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;
    
    return ;
}


void dbQuery::downloadBlob() const
{   
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;

    return downloadBlobClob( true );
}


void dbQuery::downloadClob() const
{   
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ; 

    return downloadBlobClob( false );
}


void dbQuery::uploadBlobClob( 
    bool isBlob // TRUE: is a Blob. FALSE is a Clob
                             )
{  
    using std::pair ;  
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;
    pair <SQLString, string> commandAndFileName = _sqlString.getCommandAndFileName() ;
    string tableName ;
    string whereCondition ;
    string columnName ;
    string fileName ;
    SQLString sqlCommand(getKey(), SQLString::QUERY_UNDEFINED) ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method called for " << (isBlob? "BLOB": "CLOB" )) ; 
  
    // error check on precondition
    if ( columnAndFileName.first.empty() && (commandAndFileName.first.getType() == SQLString::QUERY_UNDEFINED) )
    {
        ACS_THROW(QueryException("Required field not set")) ; 
    }

    if ( not columnAndFileName.first.empty() && (commandAndFileName.first.getType() != SQLString::QUERY_UNDEFINED) )
    {
         ACS_THROW(QueryException("Both column and command set")) ; 
    }

    if ( not columnAndFileName.first.empty() )
    {    
        // Column is set
        std::vector < pair<string, string> > tableList = _sqlString.getTableList() ;
        
        // check SQLString type: MUST be LOBJ_UPLOAD
        ACS_COND_THROW( _sqlString.getType() != SQLString::LOBJ_UPLOAD, 
                                    QueryException( "Wrong SQLString type" )
                                  ) ;
 
        if ( tableList.size() != 1 ) // only oe table allowed
        {
            ACS_THROW(QueryException() << "ONE and ONLY ONE table must be provided for blob ops. (current size: " << tableList.size() << "should be: 1)") ; 
        }

        // column and file name MUST be set
        ACS_COND_THROW( columnAndFileName.first.empty() || columnAndFileName.second.empty(), 
                                    QueryException( "Column and FileName both required for blob ops and not set" )
                                   ) ;

        // prepare parameter
        tableName = tableList[0].first ;
        whereCondition = _sqlString.getWhereParameters() ;
        columnName = columnAndFileName.first ;
        fileName = columnAndFileName.second ;


        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n"
            << "\ttableName: " << tableName << "\n"
            << "\tcolumnName: " << columnName << "\n" 
            << "\tfileName: " << fileName << "\n"
            << "\twhereCondition: " << whereCondition ) ;
    }
    else
    {
        // SQL Command is set
        
        // fileName can be null. In this case, pre-check will be skipped
        sqlCommand = commandAndFileName.first ;
        fileName = commandAndFileName.second ;

        ACS_COND_THROW( sqlCommand.getType() == SQLString::QUERY_UNDEFINED, 
                                   QueryException("Command not set")
                                 ) ;
         ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "fileName: " << fileName )
    
    }
        
    // PRE-CHECK
    // Check if filename already exists
    ACS_COND_THROW( not fileName.empty() && (! File::exists( fileName )), 
                                QueryException("PRE-CHECK FAILED: File " + fileName + " doesn't exist!")
                              ) ;
    if( not fileName.empty() )
    {
        // OK -- File exists.
        
        // Read test
        {
            std::ifstream file_op( fileName.c_str(), std::ios::in );
            ACS_COND_THROW( ! file_op.is_open(), 
                                       QueryException("PRE-CHECK FAILED: File " + fileName + " not readable !")
                                     ) ;
            file_op.close() ;
        }           
    }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "PRE-CHECK SUCCESSFULL for file: " << fileName) ;
  
    StatNode &stat = _stat ? (statN(_connection.getKey())+=1)["LO upload"]+=1 : (statN(_connection.getKey()))["LO upload"]; 
    Timer timer ;
    timer.start() ;
    
    _affectedRows = 0 ;
    try {
        if ( not columnAndFileName.first.empty() )
        {   
            // Column set

            if (isBlob)
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).uploadBlob(5 params)") ;
                _affectedRows = SQLDialectFactory::sqlDialect(getKey()).uploadBlob(&_connection, tableName, columnName, whereCondition, fileName) ;
            }
            else
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).uploadClob(5 params)") ;
                _affectedRows = SQLDialectFactory::sqlDialect(getKey()).uploadClob(&_connection, tableName, columnName, whereCondition, fileName) ;
            }
        }
        else
        {
            // Command set        

            if (isBlob)
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).uploadBlob(3 params)") ;
                _affectedRows = SQLDialectFactory::sqlDialect(getKey()).uploadBlob(&_connection, sqlCommand, fileName) ;
            }
            else
            {
                ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to call : SQLDialectFactory::sqlDialect(getKey()).uploadClob(3 params)") ;
                _affectedRows = SQLDialectFactory::sqlDialect(getKey()).uploadClob(&_connection, sqlCommand, fileName) ;
            }

        }
    }
    catch(const std::exception &) {
        updateStat2(stat,"error #","") ;
        throw ;
    }
    
       // stat update 
    timer.stop() ;
    updateStat(stat, timer) ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;
}


void dbQuery::uploadBlob()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 

    return uploadBlobClob(true);
}


void dbQuery::uploadClob()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 

    return uploadBlobClob(false);
}


void dbQuery::zeroBlob() 
{
    using std::pair ;
    std::vector < pair<string, string> > tableList = _sqlString.getTableList() ;
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;


    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 
    

    if ( _sqlString.getType() != SQLString::LOBJ_UPLOAD )
    {
         ACS_THROW(QueryException( "dbQuery::zeroBlob() Wrong SQLString type" )) ; 
    }

    if ( tableList.size() != 1 ) // only one table
    {
        ACS_THROW(QueryException() << " ONE and ONLY ONE table must be provided for blob ops. (current size: " << tableList.size() << "should be: 1)") ; 
    }
    
    // no column set: error
    ACS_COND_THROW( columnAndFileName.first.empty(), 
                               QueryException( "Column is required for setting blob to zero and not set" )
                             ) ;
                             
     // prepare parameters
    string tableName = tableList[0].first ;
    string whereCondition = _sqlString.getWhereParameters() ;
    string columnName = columnAndFileName.first ;

    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n" 
        << "\ttableName: " << tableName << "\n" 
        << "\tcolumnName: " << columnName << "\n" 
        << "\twhereCondition: " << whereCondition) 
    
      // execute
    if ( !SQLDialectFactory::sqlDialect(getKey()).zeroBlob(&_connection, tableName, columnName, whereCondition)) {
        zeroBlobClob(tableName, columnName, whereCondition) ;
    }
    

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.") ;
}


void dbQuery::zeroClob() 
{
    using std::pair ;
    std::vector < pair<string,string> > tableList = _sqlString.getTableList() ;
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;


    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 

    if ( _sqlString.getType() != SQLString::LOBJ_UPLOAD )
    {
         ACS_THROW(QueryException( "dbQuery::zeroClob() Wrong SQLString type" )) ; 
    }

    if ( tableList.size() != 1 ) // only one table
    {
        ACS_THROW(QueryException() << " ONE and ONLY ONE table must be provided for blob ops. (current size: " << tableList.size() << "should be: 1)") ; 
    }

    // no column set: error
    ACS_COND_THROW( columnAndFileName.first.empty(),  
                               QueryException("Column is required for setting blob to zero and not set") 
                             ) ;

     // prepare parameters
    string tableName = tableList[0].first ;
    string whereCondition = _sqlString.getWhereParameters() ;
    string columnName = columnAndFileName.first ;
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, 
        "\n" << "\ttableName: " << tableName << "\n" 
        << "\tcolumnName: " << columnName << "\n" 
        << "\twhereCondition: " << whereCondition ) ;
  
      // execute
    if (!SQLDialectFactory::sqlDialect(getKey()).zeroClob(&_connection, tableName, columnName, whereCondition)) {
        zeroBlobClob(tableName, columnName, whereCondition) ;
    }
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished" );
}


void dbQuery::zeroBlobClob(const string &tableName, const string& columnName, const string& whereCondition ) 
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 

    SQLString sqlString (getKey(), SQLString::UPDATE);

    sqlString.addSetList(columnName, "NULL" ) ;
   
    sqlString.addTableList(tableName);

    sqlString.addANDWhereParameter(whereCondition) ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "QUERY: " << sqlString.getSQL() ) ;

    // prepare sql and stat label
    _sqlString = sqlString  ;
    _queryStatLabel = "setLargeObjNull_" + tableName ;

    bool status=false ;

    try
    {
        status = execForStatus() ; // execute
        ACS_COND_THROW(!status,QueryException("dbQuery::zeroBlobClob: Invalid status after execForStatus" )) ; 
    }
    catch (const std::exception& e)
    {
        ACS_THROW(QueryException( e, "dbQuery::zeroBlobClob: Cannot set to NULL on table \"" + tableName + "\"" )) ; 
    }
  
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "execForStatus successfully") ;
}





bool dbQuery::blobIsNull()  
{
    using std::pair ;
    std::vector < pair<string,string> > tableList = _sqlString.getTableList() ;
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 
    

    if ( _sqlString.getType() != SQLString::LOBJ_ISNULL )
    {
         ACS_THROW(QueryException( "dbQuery::blobIsNull() Wrong SQLString type" )) ; 
    }

    if ( tableList.size() != 1 ) // only one table
    {
        ACS_THROW(QueryException() << "ONE and ONLY ONE table must be provided for blob ops. (current size: " << tableList.size() << "should be: 1)") ; 
    }

    // no column: error
    ACS_COND_THROW( columnAndFileName.first.empty(), 
                               QueryException("dbQuery::blobIsNull() Column is required for getting blob size and not set") 
                             ) ;

     // prepare parameters
    string tableName = tableList[0].first ;
    string whereCondition = _sqlString.getWhereParameters() ;
    string columnName = columnAndFileName.first ;

    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n" 
        << "\ttableName: " << tableName << "\n" 
        << "\tcolumnName: " << columnName << "\n" 
        << "\twhereCondition: " << whereCondition) ;
    
  
      bool retValue = false ;
    if (!SQLDialectFactory::sqlDialect(getKey()).blobIsNull(&_connection, tableName, columnName, whereCondition, retValue)) {
        blobClobIsNull( tableName, columnName, whereCondition, retValue ) ;
    }
    
  
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished with status: " << std::boolalpha << retValue ) ;

    return retValue;
}


bool dbQuery::clobIsNull()  
{
    using std::pair ;
    std::vector < pair<string, string> > tableList = _sqlString.getTableList() ;
    pair <string, string> columnAndFileName = _sqlString.getColumnAndFileName() ;


    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 
    

    if ( _sqlString.getType() != SQLString::LOBJ_ISNULL )
    {
         ACS_THROW(QueryException( "dbQuery::clobIsNull() Wrong SQLString type" )) ; 
    }

    if ( tableList.size() != 1 ) // only one table in the query
    {
        ACS_THROW(QueryException() << "ONE and ONLY ONE table must be provided for clob ops. (current size: " << tableList.size() << "should be: 1)") ; 
    }
    // no column: exception 
    ACS_COND_THROW( columnAndFileName.first.empty(), 
                                QueryException("Column is required for getting clob size and not set")
                               )  ;


    // prepare query parameters ... 
    string tableName = tableList[0].first ;
    string whereCondition = _sqlString.getWhereParameters() ;
    string columnName = columnAndFileName.first ;

    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n"  
         << "\ttableName: " << tableName << "\n" 
         << "\tcolumnName: " << columnName << "\n" 
         << "\twhereCondition: " << whereCondition) ;
  
      bool retValue = false ;
    if (!SQLDialectFactory::sqlDialect(getKey()).clobIsNull(&_connection, tableName, columnName, whereCondition, retValue)) {
        blobClobIsNull( tableName, columnName, whereCondition, retValue ) ;
    }
    
  
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished with status: " << std::boolalpha << retValue) ;

    return retValue;
}


void dbQuery::blobClobIsNull( const string &tableName, const string& columnName, const string& whereCondition, bool& retValue) 
{

    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) 


    SQLString countFun (getKey(), SQLString::FUNCTION);
    countFun.setFunctionName("COUNT") ;
    countFun.addFunctionParameter("*") ;
    

    SQLString sqlString (getKey(), SQLString::SELECT);

    sqlString.addSelectList( countFun ) ;

    sqlString.addTableList(tableName);

    sqlString.addANDWhereParameter(whereCondition) ;
    
     {
        dbQueryParameters pp(getKey()) ;
        
        pp.addParameter(dbParam( getKey(), tableName , columnName , dbParam::ISNOTNULL )) ;
        sqlString.addANDWhereParameter(pp) ; // where largeobj col is not null ... 
    }
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "QUERY: " << sqlString.getSQL()) ;
    
    _sqlString = sqlString  ;
    _queryStatLabel = "isLargeObjNull_" + tableName ;
    int cret = 0 ;
    try
    {
        dbRow row(getKey()) ;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        execOneRow(row) ; // execute
#pragma GCC diagnostic pop
        row.column(0, cret) ; // load the returned value (count)
    }
    catch (const std::exception& e)
    {
        ACS_THROW(QueryException( e, "dbQuery::blobClobIsNull : Cannot execute query" )) ; 
    }
  
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Query returned: " << cret)  ;

    retValue = (cret==0) ;    /* set return value */
}



/*

    !!!!!!!!!!!!!!    THIS IS NOT THE EXTERNAL INTERFACE    !!!!!!!!!!!!!!
    
    Private method. For external interface see:
    bool dbQuery::execForStatus() above
    \ret: true for success
*/
bool dbQuery::dbxExecForStatus(const string& sqlString, bool &shouldRepeat) 
{
    
    /*
        Note: Lock error is also checked, by executeImmediate
    */
    shouldRepeat = false ;

    if (sqlString.empty()) { return true ; }

    //Gets a new SQLCommand object 
    dbSQLCommand command ;
    long commandStatus = SQLDialect::SQLReturn(getKey(), dbSQLCommandCreator(_connection.getConnection(), command)) ;
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "dbxExecForStatus. Cannot run getSQLCommand. Got status: " << commandStatus << " ";
        _connection.errorText(os.str());  
        _connection.setErrorCode(commandStatus) ;
        return false ;
    }
        
    dbSQLCursor sqlCursor ;
    _affectedRows = 0 ;

    // Install a signal handler guard
    {
        // This connection guard will raise a signal each second to prevent 
        // never ending poll/select in the database driver
        StaleConnectionGuard theGuard(_connection.getMaxQueryDurationSecs(), _queryStatLabel, getKey(), _stat);
        commandStatus = SQLDialect::SQLReturn(getKey(), dbSQLCommandExecuteImmediate(command,sqlString.c_str(), sqlCursor)) ;
    }    

 
    if (commandStatus != SQL_SUCCESS )
    {
        shouldRepeat = SQLDialect::shouldRepeatCommand(getKey(), _connection.getConnection(), command) ;
        std::ostringstream os ; 
        os << "dbxExecForStatus. Cannot run executeImmediate. Got status: " << commandStatus << " " ;
        _connection.errorText(command, os.str()) ; 
        _connection.setErrorCode(commandStatus) ;
        return false ;
    }
    command->getRowsAffected(&_affectedRows) ; ;

    return true ;  /* dbxExecForStatus successfully */
}


/*

    !!!!!!!!!!!!!!    THIS IS NOT THE EXTERNAL INTERFACE    !!!!!!!!!!!!!!
    
    Private method. For external interface see:
    void dbQuery::execForSet(dbSet &set) above
    \ret: NoError: Success else could be PrepareError or ExecuteError or EmptyCursor
*/
dbQuery::ErrorType dbQuery::dbxExecForSet(const string &sqlString, dbSQLCursor &cursor) 
{
    /*
        If there was an error during query: false is returned, and errorCode is Set.
        If query is successfully, true is returned.
        LOCK IS NOT CHECKED
    */

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "sql: " << sqlString) ;
    
    dbSQLCommand sqlcomm ;
    long commandStatus = SQLDialect::SQLReturn(getKey(), dbSQLCommandCreator(_connection.getConnection(), sqlcomm)) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "command got, status: " << commandStatus) ;
    _connection.setErrorCode(commandStatus) ;
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "dbxExecForSet. Cannot run getSQLCommand. Got status: " << commandStatus << " " ; 
        _connection.errorText(os.str()) ;  
        _connection.setErrorCode(commandStatus) ;
        return PrepareError ;
    }
    
    sqlcomm->setOption(eCommRowsetSize,20) ;    // FIXME ENRICO SERVE ??? 
    
    commandStatus = SQLDialect::SQLReturn(getKey(), sqlcomm->prepare (const_cast<char *>(sqlString.c_str()),0) ) ; // NOSONAR 
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "command prepared, status: " << commandStatus) ;
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "dbxExecForSet. Cannot run prepare. Got status: " << commandStatus << " " ; 
        _connection.errorText(sqlcomm, os.str()) ; _connection.setErrorCode(commandStatus) ;
        return PrepareError ;
    }
    


    dbSQLCursor sqlCursor ;
    _affectedRows= 0;

    {
        // This connection guard will raise a signal each second to prevent 
        // never ending poll/select in the database driver
        StaleConnectionGuard theGuard(_connection.getMaxQueryDurationSecs(), _queryStatLabel, getKey(), _stat);
        commandStatus = SQLDialect::SQLReturn(getKey(), dbSQLCommandExecute(sqlcomm,sqlCursor) );
    }
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "command executed, status: " << commandStatus) ;
    
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "dbxExecForSet. Cannot run execute. Got status: " << commandStatus << " " ;
        _connection.errorText(sqlcomm, os.str()) ; 
        _connection.setErrorCode(commandStatus) ;
        return ExecuteError ;
    }
    
    sqlcomm->getRowsAffected(&_affectedRows) ; ;

    cursor = sqlCursor ;  /* execForSet successfully */
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning cursor." << (!cursor ? " !!! EMPTY !!!": "")) ; 

    return !cursor ? EmptyCursor : NoError ; 
}


/*

    !!!!!!!!!!!!!!    THIS IS NOT THE EXTERNAL INTERFACE    !!!!!!!!!!!!!!
    
    Private method. For external interface see:
    void dbQuery::execForTablesMetaData(dbSet &set) above
    \ret: true success.
*/
bool dbQuery::execForTablesMetaData(const string &tableName, dbSQLCursor &cursor) 
{
    /*
        If there was an error during query: false is returned, and errorCode is Set.
        If query is successfully, true is returned.
        LOCK IS NOT CHECKED
    */

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "tableName: " << tableName) ;
        
    dbSQLMetaData sqlMetaData ;
    
    long commandStatus = SQLDialect::SQLReturn(getKey(), dbSQLMetaDataCreator(_connection.getConnection(), sqlMetaData)) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "command got, status: " << commandStatus) ;
    _connection.setErrorCode(commandStatus) ;
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "execForTablesMetaData. Cannot run getSQLMetaData. Got status: " << commandStatus << " " ; 
        _connection.errorText(os.str()) ;  
        _connection.setErrorCode(commandStatus) ;
        return false ;
    }

    dbSQLCursor sqlCursor ;
    
    
    //PAOLO this is the only line specific for getTables command
    commandStatus = SQLDialect::SQLReturn(getKey(), 
            // 0x0001 sql table
            dbSQLMetaDataGetTables(sqlMetaData, tableName.c_str(), 0x0001, sqlCursor) 
                ) ;
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "command executed, status: " << commandStatus) ;
    if (commandStatus != SQL_SUCCESS )
    {
        std::ostringstream os ; 
        os << "execForTablesMetaData. Cannot run execute. Got status: " << commandStatus << " " ;
        _connection.errorText(sqlMetaData, os.str()) ; 
        _connection.setErrorCode(commandStatus) ;
        return false ;
    }
    
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning cursor.") ;
    cursor = sqlCursor ;  /* execForTablesMetaData successfully */
    return true ;
}


void dbQuery::updateStat(StatNode &stat, const std::string &s) const 
{
    if (not _stat) { return ; } // no statistics required
    // this is a method to update the statistic accumulator passed 
    // increments values
    stat[s]+=1 ;
    if (!_queryStatLabel.empty()) {
        stat["Queries"][_queryStatLabel+ " [ms]"][s]+=1 ;
    }    
}

void dbQuery::updateStat2(StatNode &stat, const std::string &s1, const string & s2) const 
{    
    if (not _stat) { return ; } // no statistics required
    // this is a method to update the statistic accumulator passed 
    // increments values
    (stat[s1]+=1)[s2]+=1 ;
    if (!_queryStatLabel.empty()) {
        (stat["Queries"][_queryStatLabel+ " [ms]"][s1]+=1)[s2]+=1 ;
    }
    return ;
}


void dbQuery::updateStat(StatNode &stat, Timer &timer) const 
{
    if (not _stat) { return ; } // no statistics required
    // this is a method to update the statistic accumulator passed 
    // computes average and set min and max
    stat["lapse [ms]"]+=double(timer.elapsed()); 
    
    if (!_queryStatLabel.empty()) {
        stat["Queries"][_queryStatLabel + " [ms]"] +=double(timer.elapsed()); 
    }
}        


void dbQuery::updateStat(StatNode &stat, size_t rows) const 
{
    if (not _stat) { return ; } // no statistics required
    // this is a method to update the statistic accumulator passed 
    // computes average and set min and max
    stat["rows #"]+=rows;
    
    if (!_queryStatLabel.empty()) {
        stat["Queries"][_queryStatLabel + " [ms]"]["rows #"]+=rows ;
    }
}        


void dbQuery::execForStatusRetry(StopController &sc, size_t msec, unsigned short retriesMax) 
{
    for (unsigned int attempt = 0 ; ; ++attempt ) {
        try {
            if (!execForStatus())
            {
                ACS_THROW(QueryExecuteException("error executing query: " 
                    + _sqlString.getSQL() + " Error : " + errorString())) ;
            }
            return ;
        }
        catch (const std::exception &e) // NOSONAR - any exception
        {
            // note: if the max attempts has been reached exits immediately else
            // wait for msecs before trying again
            if ((attempt >= retriesMax) || sc.timeoutOrCancel(msec) ) {
                if (attempt >= retriesMax) {
                    ACS_THROW(QueryExecuteException(e) <<"maximum number of db connection retries (" << retriesMax << ") performed" ); 
                }
                else { // stop requested
                    ACS_THROW(QueryExecuteException(e, "stop requested")) ;
                }
            }
        }
    }
}



void dbQuery::execOneRowRetry(StopController &sc, 
                                size_t msec, 
                                unsigned short retriesMax,
                                dbRow &row , 
                                const std::vector<dbSpecificColumnType::Type>& suggestedTypes, 
                                bool acceptEmptyRow)
{
    for (unsigned int attempt = 0 ; ; ++attempt ) {
        try {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
            execOneRow(row, suggestedTypes, acceptEmptyRow);
#pragma GCC diagnostic pop
            return ;
        }
        catch (const std::exception &e)  // NOSONAR - any exception
        {
            // note: if the max attempts has been reached exits immediately else
            // wait for msecs before trying again
            if ((attempt >= retriesMax) || sc.timeoutOrCancel(msec) ) {
                if (attempt >= retriesMax) {
                    ACS_THROW(QueryExecuteException(e) <<"maximum number of db connection retries (" << retriesMax << ") performed" ) ;
                }
                else { // stop requested
                    ACS_THROW(QueryExecuteException(e, "stop requested")) ;
                }
            }
        }
    }
}

void dbQuery::execForSetRetry(StopController &sc,   
                              size_t msec, 
                              unsigned short retriesMax, 
                              dbSet& set, 
                              const std::vector<dbSpecificColumnType::Type>& suggestedTypes )
{
    for (unsigned int attempt = 0 ; ; ++attempt ) {
        try {
            execForSet(set, suggestedTypes);
            return ;
        }
        catch (const std::exception &e)  // NOSONAR - any exception
        {
            // note: if the max attempts has been reached exits immediately else
            // wait for msecs before trying again
            if ((attempt >= retriesMax) || sc.timeoutOrCancel(msec) ) {
                if (attempt >= retriesMax) {
                    ACS_THROW(QueryExecuteException(e) <<"maximum number of db connection retries (" << retriesMax << ") performed" ) ;
                }
                else { // stop requested
                    ACS_THROW(QueryExecuteException(e, "stop requested")) ;
                }
            }
        }
    }
}


string dbQuery::sqlString() const { return _sqlString.getSQL() ; }
const string &dbQuery::queryStatLabel() const { return _queryStatLabel ; } 

void dbQuery::sqlString(const SQLString &s, const string &statLabel) { _sqlString = s ; _queryStatLabel = statLabel ; }

string dbQuery::errorString() const { return _errorString ; }
string dbQuery::errorState() const { return _errorState ; }
bool dbQuery::error() const { return !_success ; }
size_t dbQuery::getAffectedRows() const { return _affectedRows ; } 

dbConnection &dbQuery::connection() const {return _connection ; } ;

_ACS_END_NAMESPACE
