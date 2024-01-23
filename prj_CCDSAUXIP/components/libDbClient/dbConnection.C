// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
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
	Revision 5.5  2017/11/21 18:47:08  marpas
	deprecated call removed in favour of execForSet
	
	Revision 5.4  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.3  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
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
	
	Revision 4.21  2013/06/03 12:38:41  marpas
	last serial is a long long instead of an unsigned long long
	coding best practices partially applied
	
	Revision 4.20  2013/05/30 11:19:01  marpas
	dbDriver is now a Singleton
	
	Revision 4.19  2013/05/14 18:10:48  marpas
	useless methods removed
	master/slave support removed due to remotion of useless classes in library
	fix for bad behaviour when closing connections after plugins have been downloaded
	
	Revision 4.18  2013/04/17 17:34:44  marpas
	dbStateManager & dbStateWrapper interface changed
	
	Revision 4.17  2013/04/09 07:53:08  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.16  2013/04/05 12:45:53  marpas
	Statistics are now grouped for db key
	
	Revision 4.15  2013/04/04 15:35:07  marpas
	statistics improved
	
	Revision 4.14  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.13  2013/02/12 12:38:14  marpas
	master db management functions rationalized
	
	Revision 4.12  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.11  2013/01/22 17:32:24  marpas
	minor changes
	
	Revision 4.10  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.9  2012/11/21 09:55:13  marpas
	re-adding file from attic for realignement
	
	Revision 4.7  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.6  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.4  2011/12/23 09:45:45  marpas
	- memento was reinitialized in case it is the very first connection and defaults need to be changed
	  according the "db.conf" file read
	- Debug improved
	
	Revision 4.3  2011/10/20 13:45:00  lucio.pulvirenti
	Statistics improved
	
	Revision 4.2  2011/10/18 17:23:46  marpas
	statistics improved
	
	Revision 4.1  2011/10/18 14:56:43  enrcar
	EC::LP:: Status ERROR introduced & handled
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.14  2011/10/14 07:49:45  marpas
	FORREADONLY token added
	
	Revision 3.13  2011/05/31 10:15:07  lucio.pulvirenti
	Introduced counter to inhibit check of connection goodness to avoid both endless loops
	and perturbation of db state when asking for last serial after insert (getlastSerial method)
	mutex introduced on new counter
	
	Revision 3.12  2011/05/25 08:34:17  lucio.pulvirenti
	Added public isGood method to test if an open connection is valid
	isOpen: added bool parameter (default true) enabling the test of connection validity
	in dbConnection methods, isOpen is always called so to disable the test of connection validity
	not to enter in infinite loop
	
	Revision 3.11  2011/03/02 17:03:35  marpas
	getting last serial implementation completed and tested with informix
	
	Revision 3.10  2010/11/22 21:26:02  marpas
	setFastConnect used during open (useful only for Informix based db)
	
	Revision 3.9  2010/10/14 18:01:38  marpas
	Statistics for queries for connection added
	they are total good and total errors, and then subdivided into
	execForSet and execForStatus groups along with min, max and average
	
	Revision 3.8  2010/09/11 18:07:36  marpas
	libDbExpress 3.2 interface adopted, libDbException 3.3 too
	
	Revision 3.7  2010/09/08 17:50:10  marpas
	debug improved, other general code improvements
	
	Revision 3.6  2010/08/10 11:20:53  marpas
	debug improved
	
	Revision 3.5  2010/08/10 11:18:36  marpas
	debug improved
	
	Revision 3.4  2010/08/10 11:05:29  marpas
	debug improved again
	
	Revision 3.3  2010/08/10 10:58:00  marpas
	debug improved
	
	Revision 3.2  2010/03/17 14:06:27  enrcar
	MP:: Finalize methods called. Transaction labels handled
	
	Revision 3.1  2009/10/08 10:50:12  marpas
	dbStat class removed and substituted by new libException.Statistics
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.17  2009/07/14 10:20:18  enrcar
	EC:: c++rules
	
	Revision 2.16  2009/07/01 11:35:45  marpas
	Null connection will cause an exception from ctor of dbConnectionGuard instead of a crash
	dtor of dbConnection will not raise exceptions by default
	
	Revision 2.15  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.14  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.13  2009/06/17 13:31:35  enrcar
	EC:: ecrypted passwords (and parameter passwordEncryptionAlgo) handled.
	
	Revision 2.12  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.11  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.10  2009/03/02 14:03:30  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.9  2009/01/27 13:16:19  marpas
	db statistics improved, some other minor changes with no regression
	
	Revision 2.8  2008/12/05 17:06:02  marpas
	getLastSerial method added
	
	Revision 2.7  2008/11/26 16:10:37  marpas
	accounting for transactions improveaccounting for transactions improved
	
	Revision 2.6  2008/11/26 16:08:12  marpas
	SQL for transaction is accounted too for queries
	
	Revision 2.5  2008/11/26 12:38:15  marpas
	transactions amanagement improved and accounting statistics
	
	Revision 2.4  2008/11/26 10:58:09  marpas
	exception blocked from dtor
	
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
	
*/


#include <dbConnection.h>
#include <dbAppResSet.h>
#include <dbAggregateTransactionState.h>
#include <SQLDialectFactory.h>
#include <SQLString.h>

#include <Timer.h>

#include <dbQuery.h>
#include <dbSet.h>
#include <dbStat.h>
#include <dbDriver.h>
#include <Statistics.h>
#include <Filterables.h>

#include <sstream>
#include <unistd.h>

#ifdef USE_DBCONNECTION_EXTERNAL_SECRET
#include <DbConnectionSecret.h>
#endif

_ACS_BEGIN_NAMESPACE(acs)

using acs::math::StatNode ;
using namespace std ;

//////////////////////////
//              		// 
// class dbConnection	//
//              		//
//////////////////////////

StatNode &dbConnection::statN(const std::string &key) { return db::stat(key)["dbConnection"] ; }

// Who change the following values MUST know what he's doing

//dbConnection::LockMode dbConnection::_defaultLockMode  = dbConnection::Wait;
dbConnection::LockMode dbConnection::_defaultLockMode  = dbConnection::WaitForSeconds;
int dbConnection::_defaultSingleStepSecondsToWait = 10 ; // PRQA S 4401
int dbConnection::_defaultSecondsToWait = 30 ; // PRQA S 4401
// Who changes the previous values MUST know what he's doing
bool dbConnection::_debugMsgEmitted = false ; 
ThreadSafe dbConnection::_preventConnCheckMutex;

dbConnection::Memento::Memento() :
	_lockMode(_defaultLockMode),
	_singleStepSecondsToWait(_defaultSingleStepSecondsToWait),
	_totalWaitSeconds(_defaultSecondsToWait)
{
}


dbConnection::Memento::Memento(const Memento &rhs) :
	_lockMode(rhs._lockMode),
	_singleStepSecondsToWait(rhs._singleStepSecondsToWait),
	_totalWaitSeconds(rhs._totalWaitSeconds)
{
	// empty
}

dbConnection::Memento &dbConnection::Memento::operator=(const Memento &rhs)
{
	// Copy ctr
	if (this != &rhs) {
		_lockMode = rhs._lockMode;
		_singleStepSecondsToWait = rhs._singleStepSecondsToWait ;
		_totalWaitSeconds = rhs._totalWaitSeconds ;
	}
	return * this ;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConnection)

dbConnection::dbConnection(const std::string& key)  :
	_aggregateTransactionState(0),
   	_connectionParameters(),
    _connection(),
	_connectionInternal(0),
	_transactionState(None),
	_preventConnCheckCounter(0),
    _connectionOpen(false),
	_isGoodInProgress(false),
	_lastError(),
    _lastErrorText(),
	_status(),
    _openTimer(),
    _transTimer(),
    _transStatLabel(),
	_key(key),
    _maxQueryDurationSecs(default_maxQueryDurationSecs)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "key: \"" << _key << "\"  this: " << this << "  _connectionParameters: " << (_connectionParameters.get())) ;
	dbAppResSetFactory::instance("", key) ;

	_status = Memento() ; // reinitialize the memento in case it was initialized *BEFORE* the Singleton above
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
    _connection = dbDriver::instance(_key)->getSqlConnection() ;
}


dbConnection::dbConnection(const string& key, const dbConnectionParameters &cp) :
	_aggregateTransactionState(0) ,
	_connectionParameters(cp),
    _connection(),
	_connectionInternal(0),
	_transactionState(None),
	_preventConnCheckCounter(0),
    _connectionOpen(false),
	_isGoodInProgress(false),
	_lastError(),
    _lastErrorText(),
	_status(),
    _openTimer(),
    _transTimer(),
    _transStatLabel(),
	_key(key),
    _maxQueryDurationSecs(default_maxQueryDurationSecs)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "key: \"" << _key << "\"  ptr: " << this) ;
// Attenzione: per avere un dbConnectionParameters valido, bisogna aver chiamato SQLDialectFactory.
// Questo puo' essere stato fatto sia dall'inizializzazione di AppResSetSingleton che manualmente 
// es. da un test. La chiamata esplicita initAppResSetSingleton non va quindi fatta
//	initAppResSetSingleton() ;
    
  	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "getDriverFunc: " << _connectionParameters->getDriverFunc()
        << "\tdriverLibrary: " << _connectionParameters->driverLibrary()
        << "\tvendorLibrary: " << _connectionParameters->vendorLibrary()
        << "\tkey: \"" << _key << "\"") ;
   
    _connection = dbDriver::instance(_key)->getSqlConnection() ;
 
    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_connection driver ptr:" << _connection.get()) ; 
}

dbConnection::~dbConnection() noexcept 
{
// PRQA S 4631 L1
	try {
  		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "closing and deleting connection. key: \"" << _key << "\"  ptr: " << this) ;
		close() ;	// close connection before self-destroying

		deleteConnection() ;
		
	}
	catch( exception &e )
	{
		ACS_LOG_WARNING("exception caught " << e) ;
	}
// PRQA L:L1
}

dbSQLConnection &dbConnection::getConnection()  // PRQA S 4120
{
	return _connection ; // PRQA S 4024
}



bool dbConnection::close()
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_key: \"" << _key << "\"  " // PRQA S 3050, 3380
			<< "_connection:" << (_connection ? "full" : "empty" )
			<< " isOpen:" << boolalpha << isOpen(false)) ;
    
	if (_connection.get() && isOpen(false)) {

		try {
			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "disconnecting " << _connection.get() ) ;
            SQLResult ret = SQL_SUCCESS ;
            try {
        	    ret = SQLDialect::SQLReturn(_key, _connection->disconnect()) ;	// Close connection // PRQA S 3010
			}
            catch(SQLDialectFactory::WrongSQLDialect &) { // the plugin was unregistered
                ;
            }
            ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "disconnected " << _connection.get() << " ret: " << ret )
			if (ret != SQL_SUCCESS) {
            	ostringstream tmp ;
            	tmp << "dbConnection close error(" << setErrorCode(ret) << ") ";
            	ACS_THROW(ConnectionException(errorText(_connection,tmp.str()))) ; // PRQA S 3081
 			}
		}
		catch(exException &e) {
			ACS_LOG_NOTIFY_EX(e) ;
		}
		
		// Update statistics
		statN(_key)["close #"]+=1 ;
		_openTimer.stop() ;
		statN(_key)["connection lapse [ms]"]+=double(_openTimer.elapsed()); // PRQA S 3081

		// reset transaction state 
		_transactionState = None ;
        clearAggregateTransactionState() ;
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "releasing connection" ) ; ;
        deleteConnection() ;
	}
	
	return true ;   
}


void dbConnection::checkWaitingIntervals() 
{
	// Just to avoid the some monkey hitting randomly the keyboard causes some troubles
	if (_status._singleStepSecondsToWait < 1) {
		_status._singleStepSecondsToWait = 1 ;
    }
}



ssize_t dbConnection::inc_preventConnCheckCounter()
{	
	ThreadSafe::Lock lock(_preventConnCheckMutex) ;
	if (++_preventConnCheckCounter < 1) { _preventConnCheckCounter = 1 ; }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << _preventConnCheckCounter) ;
	return _preventConnCheckCounter ;
}


ssize_t dbConnection::dec_preventConnCheckCounter()
{
	ThreadSafe::Lock lock(_preventConnCheckMutex) ;
	if (_preventConnCheckCounter>0) { _preventConnCheckCounter -- ;	}

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Returning: " << _preventConnCheckCounter) ;
	return _preventConnCheckCounter ;
}


bool dbConnection::open()
{
// EnrCar:
// Since this single function handles all the issues related to opening a connection 
// this method is longer and complex 
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_key: \"" << _key << "\"  this: " << this << "  _connectionParameters: " << (_connectionParameters.get())) ;

    if (!_connection.get()) {
        _connection = dbDriver::instance(_key)->getSqlConnection() ;	// Return a new connection
    }
	ACS_COND_THROW(isOpen(false),// Trying to open an already-opened connection // PRQA S 3081
		ConnectionException(string("Connection is already opened."))) ;	

	bool openedSucessfully = false ;
	
	try {
	
		inc_preventConnCheckCounter() ;

		Timer openStatTimer ;
		openStatTimer.start() ;
		checkWaitingIntervals() ;

        if (not _connectionParameters.get()) {
            _connectionParameters = dbAppResSetFactory::instance("",_key)->connectionParameters() ;
        }
        
 		string tmp = _connectionParameters->password() ;
		const char* encPassword = tmp.c_str() ;
		char decPassword [1024] ; memset( decPassword, 0, sizeof(decPassword) ) ;	/* Allocate buffer */ // PRQA S 4403
		ACS_SSL::EncryptionAlgo algo = ACS_SSL::EncryptionAlgo( _connectionParameters->pwdEncryptionAlgo()) ; // PRQA S 3081

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\n"  // PRQA S 3081
			<< "\tHostname: " << _connectionParameters->hostname() << "\n" 
        	<< "\tServer: " << _connectionParameters->server() << "\n" 
        	<< "\tDatabase: " << _connectionParameters->db() << "\n" 
        	<< "\tUser: " << _connectionParameters->user() << "\n" 
        	<< "\tPassword: " << encPassword << "\n" 
       		<< "\tPwdEncryptionAlgo: 0x" << hex << int(algo) << dec << '\n'
			<< "\tSalt string: " << _connectionParameters->salt() << '\n'	// uchar* -- NULL means that a random buffer should be created (if saltSize() != 0 )
       		<< "\tSalt random: " << _connectionParameters->saltSize() );
		
		// Get decPassword from encPassword  
		if ( ( encPassword != 0 ) && ( encPassword[0] != '\0') )
		{
			
			const string saltString ( _connectionParameters->salt() ) ;
			const size_t saltRandom ( _connectionParameters->saltSize() ) ;
			ACS_COND_THROW( (not saltString.empty() && (0 != saltRandom) ) , ConnectionException("MISCONFIGURATION: both salt string and salt size valorized, just one expected!") ) ; // PRQA S 3081
			
			const ACS_SSL::EncryptionAlgo encAlgo ( ACS_SSL::EncryptionAlgo_Algo(algo) ) ; 		// Extract the encryption algo (ignoring the flags)
			const ACS_SSL::EncryptionAlgo encAlgoFlags ( ACS_SSL::EncryptionAlgo_Flags(algo) ) ;// Extract the encryption flags (regardless the algo) 

			unsigned char salt_buffer [ 4096 ] ; memset( salt_buffer, 0, sizeof(salt_buffer) ) ; 
			vector <void*> vin ;
            vector <void*> vout ;
			BF_KEY bfkey ; // PRQA S 4102

			if (encAlgo == ACS_SSL::ENC_NONE) {}	/* Do nothing */
			else if (encAlgo == ACS_SSL::ENC_BLOWFISH)
			{
#ifdef USE_DBCONNECTION_EXTERNAL_SECRET
				DB_CONNECTION_KEY_IN_DEFINITION
#else
				unsigned char key_in[] = { 'm', 'a', 'n', 'u', 'e', 'l' } ;	/* Blowfish passphrase. Provided by RobRic */
#endif
				int key_len = sizeof(key_in) ;

				ACS_SSL::ACS_BLOWFISH_setkey( key_in, key_len, &bfkey) ;	/* Get Actual key */
				vin.push_back ( &bfkey ) ;

				ACS_COND_THROW( (strlen(encPassword)>sizeof(decPassword)), ConnectionException("Password too long!") ) ; // PRQA S 3081
   			}
			else
			{
				ostringstream os ;	// algo/encAlgo not handled !
				os << "Unable to handle algo: " << int(algo) ; // PRQA S 3081
				ACS_THROW(ConnectionException(os.str())) ;	// PRQA S 3081
			}

			if ( 0 != (encAlgoFlags & ACS_SSL::ENC_PREPEND_SALT) )
			{
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Salty algorithm: ENC_PREPEND_SALT flag is on");
				// Add the salt to the list of input parameters for the decode algo
				size_t saltSize(saltRandom) ;

				if (0 != saltSize)
				{
					ACS_COND_THROW( (! saltString.empty()), ConnectionException("MISCONFIGURATION: both salt string and salt size valorized, just one expected!") ) ; // PRQA S 3081

					// WHEN HERE: saltSize != 0 and empty salt string --> the salt will be randomized to size: saltSize
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "saltSize != 0 and empty salt string: the salt will be randomized to size: " << saltSize);
				}
				else if (0 == saltSize)
				{
					ACS_COND_THROW( saltString.empty(), ConnectionException("MISCONFIGURATION: numeric algo expects a configured salt, but neither the salt string" \
						" nor the random size has been provided !" )) ; 

					// WHEN HERE: saltSize == 0 and a valid salt string has been passed -> the configured salt (with its own size) will be used
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "saltSize == 0 and a valid salt string has been passed: \"" << saltString << '\"');
					
					// We have to compute the FINAL dimension of the salt (just the size of the printable string)
					saltSize = (saltString.length()) ; // The expected salt size is the number of bytes
				}

				// Compute the salt_buffer and its size, in order to pre-allocate the salt_buffer
				const size_t salt_buffer_size ( ACS_SSL::encode_salt(saltSize) ) ; 
				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Salt buffer size=" << salt_buffer_size );

				ACS_COND_THROW( (salt_buffer_size > sizeof(salt_buffer)), ConnectionException("MISCONFIGURATION: misleading salt (too long)")) ;

				ACS_SSL::encode_salt_buffer (  reinterpret_cast<const unsigned char*>(saltString.c_str()), saltString.length(), saltSize, salt_buffer, salt_buffer_size ) ; 

				vin.push_back( &(salt_buffer[0]) ) ; 	
			}
			else {
				ACS_COND_THROW( (not saltString.empty() || (0 != saltRandom) ) , ConnectionException("MISCONFIGURATION: either salt string or salt size valorized, but algo is saltless!") ) ; // PRQA S 3081
			}

			size_t s = 0 ;

			s = ACS_SSL::ACS_ALGO_decode( algo, 
											 reinterpret_cast<const unsigned char*>(encPassword),  // PRQA S 3030, 3081
											 strlen(encPassword), // NOSONAR - if here encPassword is a valid buffer
											 reinterpret_cast<unsigned char*>(decPassword),   // PRQA S 3030, 3081
											 vin, 
											 vout ) ;

			ACS_COND_THROW( (s < 0), ConnectionException("Password decoding failed") ) ; // PRQA S 3081

    		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Password decrypted successfully.") ;
#if(0)
        	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Decrypted password: !" << decPassword << "!") ;
#endif
			
		}

	//	else: DO Nothing, since decPassword is zero-filled
	
	_connectionParameters->setOptions(_connection) ; 

   	// FIXME If password is "" (empty string), a NULL pointer (i.e. 0) is sent, instead of a pointer to ("").

    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Running sqlDialect().installConnectionCallback") ;
		SQLDialectFactory::sqlDialect(_key).installConnectionCallback(this) ;

		// Open connection to DB
		long commandStatus = SQLDialect::SQLReturn(_key, 
								_connection->connect(const_cast<char *>(_connectionParameters->db().c_str()),   // PRQA S 3081 3
                            	        			 const_cast<char *>(_connectionParameters->user().c_str()),
													 ((decPassword[0] != 0) ? const_cast<char *>(decPassword): 0) ) ) ; // PRQA S 3380

    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Running sqlDialect().uninstallConnectionCallback") ;
		SQLDialectFactory::sqlDialect(_key).uninstallConnectionCallback() ;

    	openedSucessfully = (commandStatus ==  SQL_SUCCESS ) ;
		_connectionOpen = openedSucessfully ; /* Set connection status */

		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "this: " << hex << this << dec << "  _connection->connect() returned " << openedSucessfully 
				<< " status: " << commandStatus) ;

    	if (!openedSucessfully) {
			_connectionInternal = 0 ;	// Update statistics (Connection error)
        	openStatTimer.stop() ;
			StatNode &tw = statN(_key)["wrong #"]+=1 ; // PRQA S 4412
			tw["opening time [ms]"]+= double(openStatTimer.elapsed()) ; // PRQA S 3081

			setErrorCode(commandStatus) ;
            ostringstream cp ; cp << '\"' << _connectionParameters << '\"' ;
    		ACS_THROW(ConnectionException("Connection Error:" + cp.str() + " " +  errorText(_connection))) ; // PRQA S 3081
		}

		// set transaction state
		_transactionState = None ;

		// start the connection timer 
		_openTimer.start() ;


		// here the connection is correctly opened
		openStatTimer.stop() ;
		StatNode &tg = statN(_key)["good #"]+=1 ;	// Update statistics (Connection successfully) // PRQA S 4412
		statN(_key)["open #"]+=1 ; // PRQA S 4412
		tg["opening time [ms]"] += double(openStatTimer.elapsed()); // PRQA S 3081

		string sqlDesc ;

    	SQLString sqlString(_key, SQLString::COMMAND);	// Prepare command
		switch (_status._lockMode) {

			case NoWait :
                {
				    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "conn ptr: " << hex << this << dec << " NoWait mode") ;
				    sqlDesc ="No Wait On Locks" ;
				    sqlString = SQLString::getLockModeWait(_key, -1) ;
				    break ;
                }
			case Wait :
                {
				    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "conn ptr: " << hex << this << dec << " Wait mode") ; ;
				    sqlDesc ="Wait On Locks" ;
				    sqlString = SQLString::getLockModeWait(_key, 0) ;
				    break ;
                }
			case WaitForSeconds :
                {
				    ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "conn ptr: " << hex << this << dec << " WaitForSeconds mode" ) ;
				    sqlString = SQLString::getLockModeWait(_key, _status._singleStepSecondsToWait) ;
				    sqlDesc ="Timeout On Locks" ;
				    break ;
                }
		}
		
		if(!sqlString.sqlForcedStringEmpty()) {


			ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Executing: \"" << sqlString.getSQL() << "\"" ) ;
			string errorString ;
			dbQuery q(*this, sqlString, sqlDesc) ;	// Execute command LockModeSetQuery
			ACS_COND_THROW(!q.execForStatus(), // If query unsuccessfull, an exception is raised  // PRQA S 3081
				ConnectionException(string("dbConnection::open error setting the lock wait mode  ") + sqlString.getSQL() + " Error : " + getLastErrorText())) ;
		}   // end if(!sqlString.empty())

		{
			sqlString = SQLString::getIsolationCommittedRead(_key) ; 
        	if(!sqlString.sqlForcedStringEmpty()) {

		    	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Executing: \"" << sqlString.getSQL() << "\"") ;
 		    	string errorString ;
		    	dbQuery q(*this, sqlString, "isol committed read") ;	// Execute command IsolationModeSetQuery
		    	if(!q.execForStatus()) { // If query unsuccessfull, an exception is raised 
			    	ACS_THROW(ConnectionException("dbConnection::open error setting the isolation mode  " + sqlString.getSQL() + " Error : " + getLastErrorText())) ; // PRQA S 3081
		    	}
    	   }   // end if(!sqlString.empty())
		}

	}
	catch( exception &e )
	{
		dec_preventConnCheckCounter() ;
		throw;	// re-throw exception !
	}
	
	dec_preventConnCheckCounter() ;	// IT SHOULD BE EXECUTED AS _VERY_ LAST STATEMENT !

	return openedSucessfully ;
}

// PRQA S 4120, 4211 EOF
dbConnection::dbTransactionState dbConnection::transactionState()
{
	return _transactionState ;
}

void dbConnection::transactionState(const dbConnection::dbTransactionState& t) // PRQA S 4121
{
	_transactionState = t;
}

void dbConnection::setDefaultLockMode(LockMode mode)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "LockMode: " << mode) ;
	_defaultLockMode = mode ;
}


void dbConnection::setLockMode(LockMode mode)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "LockMode: " << mode) ;
	_status._lockMode = mode ;
}

dbConnection::LockMode dbConnection::getLockMode() const
{
	return _status._lockMode ;
}

void dbConnection::setLockWaitSeconds(int nsec)
{
	_status._totalWaitSeconds = min(nsec,1) ;
}

int dbConnection::getLockWaitSeconds() const
{
	return _status._totalWaitSeconds ;
}

void dbConnection::setDefaultLockWaitSeconds(int nsec)
{
	_defaultSecondsToWait = min(nsec,1) ;
}

int dbConnection::getDefaultLockWaitSeconds()
{
	return _defaultSecondsToWait ;
}

void dbConnection::setSingleStepWaitSeconds(int nsec)
{
	_status._singleStepSecondsToWait = min(nsec,1) ;
}

int dbConnection::getSingleStepWaitSeconds() const
{
	return _status._singleStepSecondsToWait ;
}

string dbConnection::getKey() const
{
	return _key ;
}

void dbConnection::setDefaultSingleStepWaitSeconds(int nsec)
{
	_defaultSingleStepSecondsToWait = min(nsec,1) ;
}

int dbConnection::getDefaultSingleStepWaitSeconds()
{
	return _defaultSingleStepSecondsToWait ;
}


const dbConnectionParameters &  dbConnection::connectionParameters() const 
{ 
	return _connectionParameters ; 
}

void dbConnection::connectionParameters(const dbConnectionParameters &cp) 
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;

	_connectionParameters = cp ;
}

const dbConnection::Memento &dbConnection::getStatus() const 
{
	return _status ;
}

void dbConnection::setStatus(const Memento &m) 
{
	if (isOpen(false)) {
		ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "dbConnection status change while connection is open") ;
	}
	
	_status = m ; 
}


bool dbConnection::isOpen(bool checkIsGood) //const 
{ 
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
	
	ssize_t internalPreventConnCheckCounter = 0 ;
	
	{
		ThreadSafe::Lock lock(_preventConnCheckMutex) ;
		internalPreventConnCheckCounter = _preventConnCheckCounter ;
	}	
	

	ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "_key: \"" << _key << "\"  checkIsGood: " << boolalpha << checkIsGood 
			<< " _preventConnCheckCounter: " << internalPreventConnCheckCounter
			<< " _connectionOpen: " << boolalpha << _connectionOpen ) ;
	
	
	if ( (0 == internalPreventConnCheckCounter) && checkIsGood && _connectionOpen )
	{
		try
		{
			if (! isGood())
			{
				// No further try/catch is needed since isGood() cannot throw exception

				close() ;
			}
		}
		catch (exception& e)
		{
			ACS_LOG_WARNING(SimpleDebugSignature
				<< " When closing connection got the following exception: " << e) ;
			deleteConnection() ;
			_connection = dbDriver::instance(_key)->getSqlConnection() ;	// Return a new connection
		}		
	}
	
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning: " << boolalpha << _connectionOpen) ;

    return _connectionOpen ;
}


void dbConnection::addStateWrapper(dbStateWrapper &sw)
{
	ACS_COND_THROW(!_aggregateTransactionState, // PRQA S 3081
		StateException("_aggregateTransactionState == 0; A transaction must be started before using this method"))
	_aggregateTransactionState -> add( sw, *this) ;
}	



void dbConnection::beginWork(const string &transStatLabel)
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "\"" << transStatLabel << "\" ThreadId:" << pthread_self()) ;
		
	ACS_COND_THROW(transactionState() != None,CommitException("Transaction is already started or cannot be started now")) ; 
	ACS_COND_THROW(_aggregateTransactionState,CommitException("TransactionState is not a nullptr as expected")) ; 
	try {

		dbQuery query(*this) ;
		query.sqlString(SQLString::beginTransaction(_key, transStatLabel), "BeginWork") ;

		_transTimer.start() ; // start transaction timer 
		ACS_COND_THROW(!query.execForStatus(), 
			CommitException("Query Execution error: Command : "
							  + query.sqlString() + " Error : " + query.errorString())) ;


    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "calling beginFinalize" ) ;
		SQLDialectFactory::sqlDialect(_key).beginFinalize(_connection) ;
    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "beginFinalize called") ;  

        // set transaction state
		_transactionState = Begin ;

		initAggregateTransactionState() ;
	}
	catch(const std::exception &) { // NOSONAR - any exception
		_transactionState = None ;
		(statN(_key)["Transactions #"]+=1)["not started #"]+=1 ;	/* Update statistics (unable to start transaction) */
		if( !transStatLabel.empty() ) {
			statN(_key)["Transactions #"]["named"][transStatLabel]["error #"]+=1;
			_transStatLabel.clear() ; // empty: no commit or rollback stat accounting
		}
		throw ;
	}

	statN(_key)["Transactions #"]+=1 ;	/* Update statistics (starting the transaction was successfully) */
	if( !transStatLabel.empty() ) {
		statN(_key)["Transactions #"]["named"][transStatLabel]+=1;
		_transStatLabel = transStatLabel;
	}
}


void dbConnection::commitWork()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_VLO_VERB) ;
		
	ACS_COND_THROW( (transactionState() != Begin) && (transactionState() != Error), CommitException("Transaction has never started - no commit invocation allowed")) ; // PRQA S 3081

	dbTransactionState old = transactionState() ;
	_transactionState = Commit ;
	dbQuery query(*this) ;
	
    
    query.sqlString(SQLString::commitTransaction(_key),"Commit");	/* Prepare command */
    
    if (ACS_CLASS_GET_DEBUG() == 7) {
        ACS_THROW(dbQuery::QueryExecuteException("Simulated commit exception")) ;
    }  
      
    if(!query.execForStatus())	/* Execute command */
	{
		_transactionState = old ;	/* Error during execution */
		ACS_THROW(CommitException("Query Execution error: Command : " // PRQA S 3081
						  + query.sqlString() + " Error : " + query.errorString())) ;
	}

	if (old == Error) { dec_preventConnCheckCounter() ;}
 
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "calling commitFinalize") ;
	SQLDialectFactory::sqlDialect(_key).commitFinalize(_connection) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "commitFinalize called" ) ;

	_transactionState = None ;

	updateNamedTransStat("commit [ms]", _transTimer) ;
	_transStatLabel.clear();
	clearAggregateTransactionState() ;
}



void dbConnection:: rollBack()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
		
	ACS_COND_THROW( (transactionState() != Begin) && (transactionState() != Error),CommitException("Transaction has never started - no rollback invocation allowed")) ; // PRQA S 3081

	dbTransactionState old = transactionState() ;
	_transactionState = Abort ;
	dbQuery query(*this) ;

	query.sqlString(SQLString::rollbackTransaction(_key),"Rollback") ;	/* Prepare command */
	if(!query.execForStatus())	/* Execute command */
	{
		_transactionState = old ;	/* Error during execution */
		ACS_THROW(CommitException("Query Execution error: Command : " // PRQA S 3081
						  + query.sqlString() + " Error : " + query.errorString())) ;
	}
	
	if (old == Error) { dec_preventConnCheckCounter() ; }
    
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "calling rollbackFinalize") ;
	SQLDialectFactory::sqlDialect(_key).rollbackFinalize(_connection) ;
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "rollbackFinalize called") ;

	_transactionState = None ;

	updateNamedTransStat("rollback [ms]", _transTimer) ;
	_transStatLabel.clear();
	
	if(_aggregateTransactionState) {
		_aggregateTransactionState->undo() ;
    }
	else {
		ACS_THROW(StateException("_aggregateTransactionState == 0")) ;  // PRQA S 3081
	}
    
	clearAggregateTransactionState() ;
}


bool dbConnection::hasAggregateTransactionState() const noexcept
{
    return _aggregateTransactionState != nullptr ;
}
    


void dbConnection::initAggregateTransactionState()
{
	
	ACS_COND_THROW(_aggregateTransactionState,StateException("_aggregateTransactionState != 0")) ; // PRQA S 3081
	
	_aggregateTransactionState= new dbAggregateTransactionState;
}


void dbConnection::clearAggregateTransactionState() noexcept
{
	delete _aggregateTransactionState ;
	_aggregateTransactionState=nullptr ;
}

string dbConnection::errorText(const std::string &s) 
{
	return errorText(_connection, s) ;
}


long dbConnection::setErrorCode(long errorCode)
{
    /* Set Error Code */ 
    return _lastError = errorCode ; // PRQA S 3010
}

void dbConnection::setConnectionInternal(pthread_t t, void *ci) 
{
	if (pthread_self() == t) {
		_connectionInternal = ci ;
	}
}

void * dbConnection::getConnectionInternal() 
{
	return _connectionInternal ;
}

long long dbConnection::getLastSerial(const string &table, const string &stat)
{
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method called for table: \"" << table << "\"") ;
	
	long long int serial = -1 ;
		
	try {
		
		ssize_t prev_counter = inc_preventConnCheckCounter() ; 
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Current value of _preventConnCheckCounter: " << prev_counter) ;
			
		dbQuery serialQuery(*this) ;

    	SQLString sql (_key, SQLString::COMMAND);
    	string c = SQLDialectFactory::sqlDialect(_key).getLastSerial(table, *this) ;	/* Obtain the proper command */
    	sql.forceSQLString(c);
    	serialQuery.sqlString(sql, stat + table + " get serial");

        dbSet set ;
        serialQuery.execForSet(set) ; // shall return one and only one row
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ;
    	dbRow serialRow(_key) ;
        set.getNextRow(serialRow) ; // first and only 
    	serialRow.column(0,serial) ;

		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "method finished for table: \"" << table << "\"  returning: " << serial ) ;
	}
	catch( exception &e )
	{
		dec_preventConnCheckCounter() ;
		throw;
	}
	
	{
		ssize_t prev_counter = dec_preventConnCheckCounter() ;
		
		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished. Current value of _preventConnCheckCounter: " << prev_counter
				<< "  returning: " << serial ) ;
	}

	return serial ;
}


void dbConnection::deleteConnection() 
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "resetting internal dbSQLConnection") ;
	
	{ ThreadSafe::Lock lock(_preventConnCheckMutex) ; _preventConnCheckCounter = 0 ; }
    _connectionOpen = false ;

	_connection.reset() ;
	_connectionInternal = 0 ;
}



long dbConnection::getLastError() const noexcept 
{
    /* Return Error Code */ 
    return _lastError ;
}

const string &dbConnection::getLastErrorText() const noexcept 
{
    /* Return Error Code */ 
    return _lastErrorText ;
}


bool dbConnection::isGood()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB) ;
	
	bool isGood = false ;
	ssize_t internalPreventConnCheckCounter = 0 ;
	
	{
		ThreadSafe::Lock lock(_preventConnCheckMutex) ;
		internalPreventConnCheckCounter = _preventConnCheckCounter ;
	}	
	
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "_preventConnCheckCounter: " << internalPreventConnCheckCounter 
			<< " _connectionOpen: " << boolalpha << _connectionOpen 
			<< " _isGoodInProgress: " << boolalpha << _isGoodInProgress) ;
	
	
	if ( (0 == internalPreventConnCheckCounter) && _connectionOpen && !_isGoodInProgress)
	{
    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "About to run the check") ;
		_isGoodInProgress = true ;
		isGood = false ;
		string cmd;
		try
		{
    	    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "creating goodQuery") ;
			dbQuery goodQuery(*this) ;

    		SQLString sql (_key, SQLString::COMMAND);
    	    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "asking getIsGoodDummyQuery") ;  
			cmd = SQLDialectFactory::sqlDialect(_key).getIsGoodDummyQuery() ;	/* Obtain a dummy, test command */
    		ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "About execute the command \"" << cmd << "\"") ;
			sql.forceSQLString(cmd);
    		goodQuery.sqlString(sql, "CONCK");

            dbSet set ;
            goodQuery.execForSet(set) ; // execute command

			isGood = true ;	

		}
		catch(exception& e)
		{
    		ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "The following exception got when executing the command \"" << cmd << "\""
				    << e ) ;
			isGood = false ;	
		}
	
		_isGoodInProgress = false ;	
		
	}
	else {
		isGood = true ; // A closed connection is ALWAYS good
    }

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Method finished.  Connection is good: " << boolalpha << isGood ) ;  
	
	return isGood ;

}


void dbConnection::updateNamedTransStat(const string &action, Timer &t)
{
	t.stop() ;
	StatNode &s = statN(_key)["Transactions #"] ;   // PRQA S 4412
	s[action] += double(t.elapsed()); // Update statistics (commit successfully) // PRQA S 3081
	if (not _transStatLabel.empty()) {
		s["named"][_transStatLabel][action] += double(t.elapsed()) ; // PRQA S 3081
    }
}


string toString(dbConnection::LockMode t)  // PRQA S 4020
{
    switch(t) {
    case dbConnection::NoWait: { return "NoWait" ; }  
    case dbConnection::Wait: { return "Wait" ; }
    case dbConnection::WaitForSeconds: { return "WaitForSeconds" ; }
    default:
        {
            ostringstream os ; 
            os << "unrecognized LockMode <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, dbConnection::LockMode t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, dbConnection::LockMode t) {// PRQA S 4222
    return os << toString(t) ;
}



_ACS_END_NAMESPACE
