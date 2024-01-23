/*

	Copyright 1995-2021, Exprivia Spa - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.1  2017/01/13 15:18:47  marfav
	Enforcing best practices to remove warnings
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.25  2016/07/14 13:56:04  marfav
	CSGACQ-86 using sqlstring
	
	Revision 2.24  2016/05/11 11:53:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.23  2016/04/11 13:45:45  marpas
	adopting new db interfaces
	
	Revision 2.22  2016/03/29 17:16:35  marpas
	adopting new db I/F
	
	Revision 2.21  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.20  2015/08/10 09:29:47  marfav
	PDSEV-8: checking schema before enabling blacklist management
	
	Revision 2.19  2015/08/07 14:40:18  marfav
	PDSEV-8 interaction with the db queue optimized and blacklist table support added
	
	Revision 2.18  2015/05/04 15:30:27  marfav
	Removing compiler warnings
	
	Revision 2.17  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.16  2014/04/17 08:08:23  marfav
	Flushing dbBroadcast message queue when retrieving new orders
	
	Revision 2.15  2013/04/12 12:16:44  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.14  2013/03/14 15:05:38  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.13  2013/03/12 17:57:34  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.12  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.11  2013/03/08 18:33:28  marfav
	Using connection pool
	
	Revision 2.10  2013/02/26 16:36:23  marfav
	Some compiler warning removed
	
	Revision 2.9  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.8  2011/10/13 13:01:01  marfav
	Compiler warning managed
	
	Revision 2.7  2010/03/09 10:24:39  enrcar
	EC:: Oracle DB compatibility added
	
	Revision 2.6  2009/09/28 17:00:30  marpas
	dbParam interface obsolecence
	
	Revision 2.5  2009/06/26 10:31:53  marpas
	no message from some class dtors
	
	Revision 2.4  2008/10/08 14:32:05  crivig
	porting to postgres8
	
	Revision 2.3  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.2  2006/11/10 13:48:27  fracar
	handling parameters file loading from the order table clob
	
	Revision 2.1  2006/06/12 11:01:58  marfav
	Status Manager creation moved into CTOR
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.44  2005/10/24 18:33:18  marpas
	dbexpress compatibility
	
	Revision 1.43  2005/05/03 10:37:33  marfav
	Waiting orders query updated. Debug messages improved.
	
	Revision 1.42  2005/05/02 16:04:41  marfav
	preHandling takes care of checking the orders in the black list
	before starting the precondition phase
	
	Revision 1.41  2005/03/31 18:13:22  fracar
	messages to mcf improved
	
	Revision 1.40  2005/03/18 10:27:01  marfav
	excmcf support added
	
        Revision 1.39  2005/03/15 09:02:32  marfav
        Fixed broadcast listener logic

	Revision 1.38  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.37  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.36  2004/10/19 08:16:17  marfav
	dbFactories removed from code
	
	Revision 1.35  2004/09/29 14:08:18  danalt
	Dependency from dbCryosatFactory removed
	
	Revision 1.34  2004/09/08 09:27:26  integrator
	cmp_nocase function renamed
	
	Revision 1.33  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.32  2004/04/22 07:24:55  integrator
	CalVal Environement support added
	
	Revision 1.31  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.30  2003/09/25 09:35:25  marfav
	Hang on exiting fixed - caused by TlBroadcastListener deletion
	
	Revision 1.29  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.28  2003/09/15 10:29:39  marfav
	stable_sort algorithm used for sorting orders by priority
	
	Revision 1.27  2003/08/07 08:36:26  marfav
	Error event when order queue unreachable
	
	Revision 1.26  2003/08/01 11:03:30  marfav
	Added support for thread Randez-Vous and InternalOrderQueue
	
	Revision 1.25  2003/07/17 17:18:43  marfav
	Removed notify of exception never thrown
	
	Revision 1.24  2003/07/17 16:47:02  marfav
	Thread::runException i/f changed: fixed
	
	Revision 1.23  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.22  2003/07/10 14:18:32  marfav
	clone method added to exceptions
	
	Revision 1.21  2003/06/06 10:31:04  marfav
	Dead code removed
	
	Revision 1.19  2003/05/30 09:04:21  marfav
	Broadcast messages format changed
	
	Revision 1.18  2003/05/21 15:02:39  marfav
	Removed some dead code
	
	Revision 1.17  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
	Revision 1.16  2003/05/13 08:43:15  marfav
	Align to new DbPersistentFactory constraints for DbCryosat objects
	
	Revision 1.15  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.14  2003/04/18 15:56:56  fracar
	aligning to new compiler version
	
	Revision 1.13  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.12  2003/03/27 15:33:22  marfav
	Added plugin support and some logging improvements
	
	Revision 1.11  2003/03/26 09:56:11  marfav
	Robustness improved
	
	Revision 1.10  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support
	
	Revision 1.9  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.8  2003/02/10 18:50:19  fracar
	prepared for multi-interval management
	
	Revision 1.7  2003/02/10 09:17:48  fracar
	improved logging
	
	Revision 1.6  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.5  2003/02/04 17:26:04  marfav
	Update JobResponsible field
	
	Revision 1.3  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/
#include <DbOrderQueueMgr.h>
#include <TlStatusMgrSingleton.h>
#include <TlOrderQueueSingleton.h>

#include <dbOrderQuery.h>
#include <dbOrder.h>
#include <dbQuery.h>
#include <dbQueryFactoryChain.h>
#include <dbPersistentFactoryChain.h>
#include <dbQueryParameters.h>
#include <DateTime.h>
#include <Filterables.h>
#include <StringUtils.h>
#include <exException.h>
#include <exStream.h>

#include <dbSmartPersistent.h>

#include <dbRow.h>
#include <dbSet.h>

#include <algorithm>
#include <ctype.h>

#include <Application.h>

using namespace std;
using namespace acs;


ACS_CLASS_DEFINE_DEBUG_LEVEL(TlBroadcastListener)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DbOrderQueueMgr)



// *******************************************
// Compares two string in case-insensitive way
int comp_nocase (const string& s, const string& s2)
{
	string::const_iterator p=s.begin();
	string::const_iterator p2=s2.begin();

	while (p!=s.end() && p2!=s2.end())
	{
		if (toupper(*p) != toupper(*p2))
			return (toupper(*p) < toupper(*p2)) ? -1 : 1;
		++p;
		++p2;
	}
	return (s2.size() == s.size()) ? 0 : ( (s.size() < s2.size()) ? -1 : 1 );
}
// *******************************************

// -------------------------------------------------------------------------------------
//                       CLASS TlBroadcastListener
// -------------------------------------------------------------------------------------

TlBroadcastListener::TlBroadcastListener(dbConnectionPool& cp) 
			: MQTTDbBroadcastListenerBase(cp) 
{ 
}

void TlBroadcastListener::init()
{
	// set the vector of DB events
	std::vector<MQTTDbEventType> dbEvents;
	// t_ordersqueue events
	dbEvents.push_back(MQTTDbEventType("t_ordersqueue", "INSERT"));
	// t_orderstatetransitions events
	dbEvents.push_back(MQTTDbEventType("t_orderstatetransitions", "ERR2ELI"));
	// t_waitingorders events
	MQTTDbEventType et1("t_waitingorders", "ORDNOWAI");
	et1.addEvent("ORDWAIT");
	dbEvents.push_back(et1);

	// set the DB events of interest
	setDbEventInterests(dbEvents);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Subscription for Db broadcast events performed.");
}

bool TlBroadcastListener::filterMessage(const MQTTMessageHandler & m)
{
  string action= m.get_action();
  string table= m.get_table();

  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Caught message: Action " << action << " table " << table);

  if ( ( (true == StringUtils::equalsNoCase(action,"INSERT")) && (true == StringUtils::equalsNoCase(table,"t_ordersqueue")) ) ||
       ( (true == StringUtils::equalsNoCase(action,"ERR2ELI")) && (true == StringUtils::equalsNoCase(table,"t_orderstatetransitions")) ) )
  {

  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "INS message on T_OrdersQueue accepted - returning true")

    // returning true here will trigger the queue check
    return true;
  }
  else if (true == StringUtils::equalsNoCase (action, "ORDNOWAI"))
  {
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Order removed from OrderWait table - returning true");

    // returning true here will trigger the queue check
    return true;
  }
  else if (true == StringUtils::equalsNoCase (action, "ORDWAIT"))
  {
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Order inserted into OrderWait table - returning false and purging the TlOrderQueue");

    // no need to check the queue again
    // but have to purge TlOrderQueueSingleton
    long orderid = atol(table.c_str()) ;
    TlOrderQueueSingleton::instance() -> purge (orderid);
    return false;
  }
  else
  {

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Message " << action << " on " << table << " rejected - returning false");

    // no need to check the queue again
    return false;
  }
}

// -------------------------------------------------------------------------------------
//                        DISPATCHER
// -------------------------------------------------------------------------------------

DbOrderQueueMgr::DbOQM_Dispatcher::DbOQM_Dispatcher (const DbOrderQueueMgr::DbOQM_Dispatcher& d) :
    _listener(d._listener),
    _status(d._status),
    _parent(d._parent)
{
}

DbOrderQueueMgr::DbOQM_Dispatcher& DbOrderQueueMgr::DbOQM_Dispatcher::operator = (const DbOrderQueueMgr::DbOQM_Dispatcher& d)
{
	if (this != &d)
	{
		_listener=d._listener;
		_status=d._status;
		_parent=d._parent;
	}
	return *this;
}

void DbOrderQueueMgr::DbOQM_Dispatcher::operator () (const  DbOrderQueueMgr::OQM_Status& status, unsigned int slot)
{
     // assume that since entered a status, we never will exit back from
     // so only one state transition to Warning  or Error is possible

	if ((_status != status))
	{

	    // There was a state transition - Initial status is assumed to be OK
	    // and it is setted by ctor (as external parameter)

	    switch (status)
	    {

	    case OQM_ERROR1:
		    // The Queue Manager is not running
		    // Cannot continue
			ACS_LOG_ERROR ("The order queue manager is not running");

		    std::exception* ex;
		    if (_parent->runException(ex))
		    {
			    ACS_LOG_ERROR ("Order Queue Manager Exception caugth : " << ex->what());
			    delete ex;
		    }
		    break;

	    case OQM_ERROR2:
		    // The Queue Manager is runnig
		    // The broadcastListener is not listening and no timer setted
		    // Cannot continue

			ACS_LOG_ERROR ("The broadcast listener is not running, cannot continue without any timer set");
		    // listener can not exist at all ....
		    if (_listener)
		    {
			    std::exception* ex;
			    if (_listener->runException(ex))
			    {
				    ACS_LOG_ERROR ( "BroadcastListener Thread: exception caugth : " << ex->what() );
			        delete ex;
			    }
		    }
		    break;

	    case OQM_WARNING:
		    // The Queue Manager is runnig
		    // The broadcastListener is not listening but a timer exists
		    // Can continue using timer-driven polling cycles
		    ACS_LOG_WARNING ("The broadcast listener is not running, continuing with timer signal only");

		    // listener can not exist at all ....
		    if (_listener)
		    {
			    std::exception* ex;
			    if (_listener->runException(ex))
			    {
				    ACS_LOG_ERROR ( "BroadcastListener Thread: exception caugth : " << ex->what() );
				    delete ex;
			    }
		    }
		    break;

	    default:
		    // nothing to do in case of OQM_ERROR and  OQM_OK
		    break;
	    }
	}
	_status=status;
}


// -------------------------------------------------------------------------------------
//                        CONSTRUCTOR
// -------------------------------------------------------------------------------------

DbOrderQueueMgr::DbOrderQueueMgr(tlDaemon& aDaemon) :
    OrderQueueMgr(aDaemon),
    _myListener(NULL),
    _statusMgr(NULL)
{

    {
	try
	{
	     // create the db broadcast listener...
	     ACS_LOG_INFO ("Creating TlBroadcastListener")
	     _myListener = new TlBroadcastListener( (*Application::instance()->getDefaultConnectionPool()));
	     ACS_LOG_INFO ("TlBroadcastListener creation completed")
	}

	// log message not reporting init() fails ... maybe we can continue without any BroadcastListener
	// we will get more informations calling the getStatus method
	catch (exception& e)
	{
	    ACS_LOG_WARNING ("Got exception during OrderQueueMgr creation... BroadcastListener could not be created")
	    ACS_LOG_NOTIFY_EX (e);
	    _myListener = NULL;
	}

       // Creating the status manager
       // here we can point to a valid listener address....
       _statusMgr = new StatusMgr<OQM_Status, DbOQM_Dispatcher> (DbOQM_Dispatcher(_myListener , this), OQM_OK );
       _statusMgr->enterStatus(OQM_OK);
    }
}


// -------------------------------------------------------------------------------------
//                         DESTRUCTOR
// -------------------------------------------------------------------------------------

DbOrderQueueMgr::~DbOrderQueueMgr() throw() 
{
  delete _statusMgr;
    _statusMgr = 0;
 
  if (_myListener)
  {
  	try {
    	_myListener->stopListening();
	}
	catch(exception &x) {
		excerr.notify(x) ;
	}
    delete _myListener;
    _myListener = 0;
  }
}


// -------------------------------------------------------------------------------------
//                       INIT
// -------------------------------------------------------------------------------------

void
DbOrderQueueMgr::init()
{
	try
	{
		// Reset all the interrupted status to ERROR
		putOrdersFromStatusToError ("PROCESSING");
		putOrdersFromStatusToError ("PRECONDITIONCHECK");
		putOrdersFromStatusToError ("DOWNLOADING");
		// if the db broadcast listener has been created...
		if (_myListener)
		{
			// ... configure it to raise the sigusr2 signal ...
			_myListener->sendSignal(true);

			// ... GO!
			ACS_LOG_INFO ("Starting TlBroadcastListener")
			_myListener->startListening();
			_myListener->sendSignal(true);
		}
	}

	// log message not reporting init() fails ... maybe we can continue without any BroadcastListener
	// we will get more informations calling the getStatus method
	catch (exception& e)
	{
		ACS_LOG_WARNING ("Got exception during OrderQueueMgr initialization... BroadcastListener could not be started");
		ACS_LOG_NOTIFY_EX (e);
	}
}


/**
 * Try to update the orders in a given status to ERROR, to reset interrupted actions
 */
void DbOrderQueueMgr::putOrdersFromStatusToError (const string & status) const
{
	try
	{
		dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (connWrp);
		string dbKey = conn.getKey();

		SQLString sqlString (dbKey, SQLString::UPDATE);
		sqlString.addTableList ("t_ordersqueue", "q");
		sqlString.addSetList( "status", dbConv(dbKey, "ERROR") ); // PRQA S 3050, 3081
		// Coming from the given status
		{
			dbQueryParameters pp(dbKey);
			pp.addParameter(dbParam(dbKey, "q","status", dbParam::EQUAL_TO, dbConv(dbKey, status))); // PRQA S 3050, 3081
			sqlString.addANDWhereParameter(pp);
		}
		// Belonging to me 
		// This filter may actually change in the future when clustering and UUID will be used
		{
			dbQueryParameters parameters (dbKey);
			parameters.addParameter(dbParam(dbKey, "q","jobresponsible", dbParam::EQUAL_TO, db::toSQL(dbKey, Application::hostname())) ) ;
			sqlString.addANDWhereParameter(parameters);
		}

		dbQuery query(conn, sqlString, "UpdOrdQueueToError");
		query.execForStatus();
		if (query.getAffectedRows() > 0)
		{
			ACS_LOG_INFO ("Forced " << query.getAffectedRows() << " orders from " << status << " to ERROR");
		}
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("Unable to force orders from status " << status << " to ERROR");
		ACS_LOG_NOTIFY_EX (ex);
	}
}



// -------------------------------------------------------------------------------------
//                       PRE ORDER HANDLING
// -------------------------------------------------------------------------------------
bool
DbOrderQueueMgr::preHandling (tlOrder& o)
{
    // last checks before handling order to parent...
    // if this returns false, the order will be skipped, and not put in precondition check

   ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "order#" << o.getOrderId());

    bool result = true;
    dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
    dbConnection& conn (connWrp);
	string dbKey = conn.getKey();

    // This query returns one row of one column that contains
    // 0 if the order is not blacklisted
    // 1(or more) if the order has entries in the waitingorders table

	SQLString sqlString (dbKey, SQLString::SELECT);
	sqlString.addTableList ("t_ordersqueue", "q");
	sqlString.addSelectList ("q.status");
	sqlString.addSelectList ("count (w.orderid)" , "waiting");

	// The Join on waiting orders
	{
		dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "q","orderid" , dbParam::EQUAL_TO , "w.orderid" )) ;
		sqlString.addJoinList(SQLDialect::LEFT,"t_waitingorders", "w", pp) ;
	}

	// Where condition
	{
		dbQueryParameters pp(dbKey) ;	
		pp.addParameter(dbParam(dbKey, "q","orderid" , dbParam::EQUAL_TO, dbConv (dbKey, o.getOrderId() ))) ;
		sqlString.addANDWhereParameter(pp) ;
	}
	
	// group by clause
	sqlString.addGroupByList ("q.status");

    try
    {
	dbQuery query(conn, sqlString, "SelOrdQueueByStatus");
	dbRow row(dbKey);
	dbSet set;
	query.execForSet(set);
	
	if (not set.empty())
	{
	    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Query on t_waitingorders returned " << set.size() << " rows")
		
	    // here is assumed that the query returned exactly one row... 
	    // anyway only the first row is used
	    set.getNextRow(row);
	    string status;
	    unsigned long count;
	    row.column(0, status);
	    row.column(1, count);
		 
	    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Order#" << o.getOrderId() << ": status [" << status << "] waiting count [" << count << "]")

	    if ((count == 0) && (status == "ELIGIBLE"))
	    {
	    	// this order is not present in the blacklist
		result = true;
	    }
	    else
	    {
	    	// this order is blacklisted
		result = false;
	    }
		
	}
	else
	{
	    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Query on t_waitingorders returned no rows. Assuming order not blacklisted");
	    // This is not strictly necessary... 
	    result = true;
	}
    }
    catch (...)
    {
		// Cannot execute the query... not a problem
		// Just go on and assume the order not present in the waitingOrders table
		// In the worst case the order will be re-checked during the precondition phase
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Exception caught while executing the preHandling query. Assuming order not blacklisted");
		// This is not strictly necessary... 
		result = true;
    }


		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Order#" << o.getOrderId() << (result?" is not ":" is ") << "blacklisted");

    // If green light for order execution, then fill it up with parameter CLOB
    {
	try
	{
	    ACS_SMARTPTR<dbOrder> adbOrder (dynamic_cast<dbOrder*> (dbPersistentFactoryChain::instance()->newObject("dbOrder", conn.getKey())));
	    adbOrder->load(conn, o.getOrderId() );
	    if (adbOrder->hasParameters())
	    {
		ostringstream os;
		adbOrder->saveParametersFile(os, conn);
		o.setParametersCLOB (os.str());

			    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Order#" << o.getOrderId() << ": clob refreshed.")
			    ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Order#" << o.getOrderId() << ": clob dump is \"" << os.str() << "\"");

	    }
	    else 
	    {
			    // Just debug messages here
			    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Order#" << o.getOrderId() << ": no parameters file")
	    }
	}
	catch (exception& ex)
	{
	    ACS_LOG_WARNING ("Failure downloading parameters file from database for order#" << o.getOrderId())
	    ACS_LOG_NOTIFY_EX (ex);
	}
    }

    return result;
}




// -------------------------------------------------------------------------------------
//                       RESTART Order Queue Mgr
// -------------------------------------------------------------------------------------
void
DbOrderQueueMgr::restartOQM()
{
    try
    {
	if (_myListener)
	{
		ACS_LOG_INFO ("Updating broadcast listener configuration settings")
	    _myListener->reloadConfiguration();
	}
    }
    catch (exException& e)
    {
		ACS_LOG_WARNING ("Got exception during broadcast listener configuration");
		ACS_LOG_NOTIFY_EX (e);
    }
    catch (...)
    {
		ACS_LOG_WARNING ("Unknown exception got during broadcast listener configuration");
    }
}


// -------------------------------------------------------------------------------------
//                       CLEANUP
// -------------------------------------------------------------------------------------
void DbOrderQueueMgr::cleanup()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

    if (_myListener)
    {
	_myListener->stopListening();
    }

    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Cleanup done");
}




// -------------------------------------------------------------------------------------
//                       RETRIEVEORDERS
// -------------------------------------------------------------------------------------
void
DbOrderQueueMgr::retrieveOrders(vector<tlOrder>& orders)
{

  // Cleanup also the listener message queue (if any)
  if (_myListener)
  {
	size_t messages_in_queue ; 
	const bool reset_queue (true) ;
	_myListener->gotMessages(messages_in_queue, reset_queue) ; // Queue is cleared
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, messages_in_queue << " messages got.");
  }

  orders.clear();

  ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Executing query ... ");

  dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
  dbConnection& theConnection (connWrp);  
  const string &dbKey =theConnection.getKey() ;


  // Retrieving from DB the first 100 "eligible" orders
  ACS_SMARTPTR<dbOrderQuery> query ( dynamic_cast<dbOrderQuery*>(dbQueryFactoryChain::instance()->newObject("dbOrderQuery", dbKey)) );

  query->resetParameters() ;
  {
    dbQueryParameters parameters (dbKey);
    parameters.addParameter(dbParam(dbKey, "t_ordersqueue","status", dbParam::EQUAL_TO, db::toSQL(dbKey, "ELIGIBLE")) ) ;
    query->addParameter(parameters) ;
  }

  {
    dbQueryParameters parameters (dbKey);
    parameters.addParameter(dbParam(dbKey, "t_ordersqueue","jobresponsible", dbParam::EQUAL_TO, db::toSQL(dbKey, Application::hostname())) ) ;
    parameters.addParameter(dbParam(dbKey, "t_ordersqueue","jobresponsible", dbParam::ISNULL) ) ;
    parameters.setParamOperator (dbQueryParameters::OR);
    query->addParameter(parameters) ;
  }

  // Using LEFT join instead of NOT-IN clause (WAITINGORDERS)
  {
    dbQueryParameters pp(dbKey);
    pp.addParameter(dbParam( dbKey,  "t_ordersqueue","orderid" , dbParam::EQUAL_TO , "t_waitingorders.orderid")) ;
    query->addJoin (SQLDialect::LEFT,"t_waitingorders",pp) ;

    // And adding selection criteria on the joined items
    dbQueryParameters parameters (dbKey);
    parameters.addParameter(dbParam(dbKey, "t_waitingorders","orderid", dbParam::ISNULL)) ;
    query->addParameter(parameters) ;
  }

  // In case of blacklisting allowed do join also that table and put a limit on the query results (BLACKLISTED)
  if (schemaHasBlacklistTable())
  {
    dbQueryParameters pp(dbKey);
    pp.addParameter(dbParam( dbKey,   "t_ordersqueue","orderid" , dbParam::EQUAL_TO , "t_ordersqueueblacklist.orderid")) ;
    pp.addParameter(dbParam( dbKey,   "t_ordersqueueblacklist","jobresponsible", dbParam::EQUAL_TO, db::toSQL(dbKey, Application::hostname())) ) ;
    query->addJoin (SQLDialect::LEFT, "t_ordersqueueblacklist",pp) ;

    // And adding selection criteria on the joined items
    dbQueryParameters parameters (dbKey);
    parameters.addParameter(dbParam(dbKey, "t_ordersqueueblacklist","orderid", dbParam::ISNULL)) ;
    query->addParameter(parameters) ;

    // in this case put also a limit on the query results
    query->maxItems(100);
  }

  // add order by clause so no post processing stable sort is needed
  ostringstream obs;
  if (getPriorityOrder() == PriorityOrderDescending)
  {
     query->addOrderBy("priority", SQLString::DESC) ;
  }
  else
  {
     query->addOrderBy("priority", SQLString::ASC) ;
  }

  // if same priority, order by insertion date (first the older orders)
  query->addOrderBy("creationdate", SQLString::ASC) ;

  // query
  bool flag=false;
  int counter=0;
  while (flag==false && counter<getMaxRetries())
  {
    try
    {
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Executing query ... try #"<< counter+1);

      query->doQuery(theConnection);
      flag=true;

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Executing query ... OK");
    }
    catch(exException& e)
    {
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Exception caught: " << e);
      counter++;
    }
    catch(...)
    {
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Unknown exception caught");

      counter++;
    }
  }

  if (flag)  // if query ok
  {
    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Flag was true; query size is " << query->size() )

    // filling up the orders vector

    tlOrder anOrder;
    for (unsigned int i=0; i<query->size(); ++i)
    {
      Order2Order (*( (*query)[i] ),  anOrder);

		  ACS_CLASS_BGN_DEBUG (ACS_MID_VERB)
		    string s1, s2;
		    excerr << "DbOrderQueueMgr::retrieveOrders: order(" << (*query)[i]->getOrderId() << "): [" <<
			      (*query)[i]->getStartTime().utcs(s1) << "\t" << (*query)[i]->getStopTime().utcs(s2) << "\n";
		    excerr << "DbOrderQueueMgr::retrieveOrders: order(" << anOrder.getOrderId() << "): [" <<
			      anOrder.getStartTime().utcs(s1) << "\t" << anOrder.getStopTime().utcs(s2) << "\n";
		  ACS_CLASS_END_DEBUG

      orders.push_back(anOrder);
    }

  }
  else
  {

	// This notify sends an error message to patrol console
	// The ThinLayer status will have a pulse to the warning status
	// in order to request the operator acknowledge of the occurent event
	ACS_LOG_ERROR ("The order queue is unreachable");
	(TlStatusMgrSingleton::instance()->getMgr())->pulseStatus (TL_WARNING);


    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Flag was false")

  }

  // Query deletion is managed by ACS_SMARTPTR destructor

}

// -------------------------------------------------------------------------------------
//                       GETSTATUS
// -------------------------------------------------------------------------------------
DbOrderQueueMgr::OQM_Status  DbOrderQueueMgr::getStatus()
{
	if (!(this->running()))
	{
		_statusMgr->enterStatus(OQM_ERROR1);
	}
	else
	{
		// first of all have to check if the listener exists
		if (!_myListener)
		{
			if (getTimerFrequency()) _statusMgr->enterStatus (OQM_WARNING);
			else _statusMgr->enterStatus (OQM_ERROR2);
		}
		else if (!((_myListener)->isListening()))
		{
			if (getTimerFrequency()) _statusMgr->enterStatus (OQM_WARNING);
			else _statusMgr->enterStatus (OQM_ERROR2);
		}
		else
		{
			// useless.... use only for sake of clearity
			_statusMgr->enterStatus (OQM_OK);
		}
	}
	OQM_Status currentStatus=_statusMgr->getStatus();
	return currentStatus;
}

// Blacklist queue management

namespace
{
	static const string blackListTable = "T_OrdersQueueBlackList";
}

void DbOrderQueueMgr::cleanupBlacklistItems() const
{
	// NOP if the table is not neded
	if (!schemaHasBlacklistTable()) 
	{
		return;
	}

	try
	{
		dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (connWrp);
		string dbKey = conn.getKey();

		SQLString sqls (dbKey, SQLString::DELETE);
		sqls.addTableList (blackListTable);
		{
			dbQueryParameters parameters (dbKey);
			parameters.addParameter(dbParam(dbKey, blackListTable,"jobresponsible", dbParam::EQUAL_TO, db::toSQL(dbKey, Application::hostname())) ) ;
			sqls.addANDWhereParameter(parameters) ;
		}

		dbQuery query(conn, sqls, "DEL "+blackListTable);
		query.execForStatus();

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Removed " << query.getAffectedRows() << " blacklisted order queue items for the host " << Application::hostname());

	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("Unable to cleanup the blacklist table");
		ACS_LOG_NOTIFY_EX (ex);
	}
}

void DbOrderQueueMgr::addBlackListItem (acs::dbPersistent::IdType id) const
{
	// NOP if the table is not neded
	if (!schemaHasBlacklistTable()) 
	{
		return;
	}

	try
	{
		dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (connWrp);

		dbSmartPersistent blackItem (blackListTable, conn);

		blackItem.set ("orderid", id, conn);
		blackItem.set ("jobresponsible", Application::hostname(), conn);

		blackItem.save(conn);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("Unable to insert the orderId [" << id << "] in the blacklist table");
		ACS_LOG_NOTIFY_EX (ex);
	}
}

bool DbOrderQueueMgr::schemaHasBlacklistTable () const
{
	dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);

	return  dbSchemaMetaDataProvider::existTable (blackListTable, conn);
}
