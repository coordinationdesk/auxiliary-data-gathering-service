// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.12  2017/11/06 18:55:56  nicvac
	ECICMF-94: Resource reserved for an order and automatic resources release.
	
	Revision 3.11  2017/11/06 17:15:39  nicvac
	ECICMF-94: resource computation. Reservation before precondition check
	
	Revision 3.10  2017/11/02 18:50:20  nicvac
	ECICMF-94: Set Res Manager params only in isService case.

	Revision 3.9  2017/11/02 18:25:07  nicvac
	ECICMF-94: Resource Manager parameters.

	Revision 3.8  2017/10/24 15:37:44  marfav
	ECICMF-94 Preintegrazione servizi - Caso SLOT based coperto

	Revision 3.7  2017/10/23 15:46:33  marfav
	Resource management service integration in progress

	Revision 3.6  2017/10/19 15:52:05  marfav
	Integration of service based resource manager

	Revision 3.5  2017/03/21 16:08:05  marfav
	APF-411
	Added support for scheduler default exit code interpretation (posix / Gen-Proc-ICD)
	According to what stated in the WSConfig XML file

	Revision 3.4  2017/02/02 16:14:28  marfav
	Slot class is not a vector anymore

	Revision 3.3  2017/01/12 11:20:34  marfav
	Checking the dbParameters table to detect if direct transition
	from precondition to error is correctly supported

	Revision 3.2  2017/01/11 16:58:45  marfav
	S3MPM-865
	In case the insert in the t_waitingorders fails the TL order is put in error

	Revision 3.1  2016/11/04 15:23:17  marfav
	APF-271
	Adding slot id to splunk messages

	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications

	Revision 2.12  2016/10/11 07:53:59  marfav
	S3MPM-842
	Cache Manager is passed using a pointer instead of using a copy

	Revision 2.11  2015/11/19 17:23:03  marpas
	coding best practices application in progress

	Revision 2.10  2015/07/17 14:25:28  marfav
	Reverting changes made in APF-285

	Revision 2.8  2015/05/14 08:20:49  damdec
	Compilation warning fixed.

	Revision 2.7  2015/04/30 15:49:17  marfav
	Compiler warning removed

	Revision 2.6  2012/03/19 13:37:18  marpas
	fixing bad casts

	Revision 2.5  2012/02/09 17:26:24  marpas
	refactoring in progress

	Revision 2.4  2011/03/14 09:56:00  marpas
	aligned to libThread Rev 2.14

	Revision 2.3  2008/08/01 15:38:29  marfav
	PreConditionChecker plugins framework added

	Revision 2.2  2008/04/22 13:35:28  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved

	Revision 2.1  2006/06/12 11:09:50  marfav
	StdErr and StdOut Log Level support added

	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted

	Revision 1.37  2005/06/28 08:33:46  marfav
	Removed OrderType translation to enumerative types
	Only plain text strings are handled

	Revision 1.36  2005/03/31 18:13:22  fracar
	messages to mcf improved

	Revision 1.35  2005/03/18 10:27:01  marfav
	excmcf support added

	Revision 1.34  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files

	Revision 1.33  2004/12/23 18:34:26  marfav
	Multislot Environement now active
	WSConfig dump changed using WSConfigMessage class

	Revision 1.32  2004/12/03 17:24:47  marfav
	Header Fixed

	Revision 1.31  2004/11/19 14:57:45  fracar
	fixed problems with disk space check and open pipes dangling

	Revision 1.30  2004/10/19 14:19:36  marfav
	Old code removed

	Revision 1.29  2004/08/24 14:49:04  marfav
	Fixed a problem in the purging strategy.

	Revision 1.28  2004/05/17 08:18:28  marfav
	ordersMutex moved in threadsafe OrderSlot class

	Revision 1.27  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement

	Revision 1.26  2004/05/06 16:31:10  marfav
	CHECKING_QUEUE status extended to preconditionCheck phase
	PROCESSING_ORDER status begins when preparing order (downloading)

	Revision 1.25  2004/05/05 14:22:05  marfav
	Patch for SUSPENDED order state management added (CalVal environement only)

	Revision 1.24  2004/05/04 16:21:37  marfav
	Aligned to new interface of ExecutingOrder for order slot number management

	Revision 1.23  2004/04/22 16:05:07  integrator
	Executing validity check performed during commands management

	Revision 1.22  2004/04/07 13:19:56  marfav
	Added support for IPC from ThinLayer Clients and to running Schedulers
	Does not use ACE anymore
	Using exStream.h

	Revision 1.21  2004/03/22 17:40:33  marfav
	BOOKED_ORDER message moved from PreconditionCheck to just before Downloading

	Revision 1.20  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)

	Revision 1.19  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations

	Revision 1.18  2003/09/17 09:31:56  fracar
	handling hostname and missionwide valid files

	Revision 1.17  2003/08/01 11:02:46  marfav
	Log message improving

	Revision 1.16  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time

	Revision 1.15  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError

	Revision 1.14  2003/04/30 16:20:16  marfav
	Align to ICD v2.0 and some logging improvement

	Revision 1.13  2003/04/18 16:26:06  fracar
	align to new compiler version

	Revision 1.12  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h

	Revision 1.11  2003/03/27 15:33:22  marfav
	Added plugin support and some logging improvements

	Revision 1.10  2003/03/26 09:56:11  marfav
	Robustness improved

	Revision 1.9  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support

	Revision 1.8  2003/03/11 18:25:50  marfav
	Patrol message handling added

	Revision 1.7  2003/02/18 09:09:22  fracar
	improved logging

	Revision 1.6  2003/02/10 18:50:19  fracar
	prepared for multi-interval management

	Revision 1.5  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging

	Revision 1.4  2003/02/04 11:56:17  marfav
	Using ConfigurationSingleton instead of parent config

	Revision 1.3  2003/02/04 09:58:34  marfav
	Added LOG field to headers



 */



#include <OrderExecutionMgr.h>

#include <TaskTableFactoryChain.h>

#include <ExecutingOrder.h>
#include <tlDaemon.h>
#include <tlOrder.h>
#include <TlStatusMgrSingleton.h>
#include <WSConfig.h>
#include <Filterables.h>
#include <MCFilterables.h>

#include <exStream.h>

#include <ConfigurationSingleton.h>

#include <dbSchemaMetaDataProvider.h>
#include <dbConnectionPool.h>
#include <SQLString.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbQuery.h>

#include <memory>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;
using namespace acs::mc;


/**
 *	STL HELPER FUNCTOR AREA - BEGIN
 **/

void
delete_order(ExecutingOrder *p)
{
	delete p;
}

/**
 *	STL HELPER FUNCTOR AREA - END
 **/


namespace  // unnamed namespace
{
static const string cacheDirFromBase = "/.Cache";
static const string allowedTransitionParam = "order_transition_table_enabled";

static const string serviceResourceKeyKey = "TlDaemon.ResourceServiceFactoryKey";
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderExecutionMgr)

OrderExecutionMgr::OrderExecutionMgr(tlDaemon& aDaemon, const WSConfig& aConfig) : 
_parent(aDaemon),
_config(aConfig),
_verbose(false),
_baseDir("."),
_orders(),
_cacheManager(),
_mutex(),
_pathMutex(),
_serviceResManagerKey(),
_serviceResManager(0),
_stopMode (NO_STOP)
{
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				_verbose = true;
	ACS_CLASS_END_DEBUG

	init();
}



OrderExecutionMgr::~OrderExecutionMgr() throw() 
{
	// Get the internal orders slot vector
	vector <ExecutingOrder*> & ordersList = _orders.getVector();
	try 
	{
		for_each(ordersList.begin(), ordersList.end(), delete_order);
	}
	catch(exception &x) 
	{
		ACS_LOG_NOTIFY_EX(x) ;
	}

	// Destroy also the resource manager (if any)
	ThreadSafe::Lock lock (_mutex);

	if (0 != _serviceResManager)
	{
		delete _serviceResManager;
		_serviceResManager = 0;
	}
}

void
OrderExecutionMgr::init()
{
	// The key is not mandatory in the conf space
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	rs.getValue (serviceResourceKeyKey, _serviceResManagerKey, false);
	ACS_LOG_INFO("The configure resource manager service key is [" << _serviceResManagerKey << "]");

	// Create the resource manager service and start it (if exists)
	StringKey key = StringKeyUtils::makeKey( "", "", "", "", _serviceResManagerKey );
	_serviceResManager = TLServiceResourceSrvFactoryChain::instance()->newObject(key);
	ACS_COND_THROW (0 == _serviceResManager, InitException ("Unable to create a suitable resource service manager. Key was [" + _serviceResManagerKey + "]"));

	_serviceResManager->reloadConfiguration();
	ACS_LOG_INFO("Starting the resource management service...");
	try {
		_serviceResManager->startService();
	} catch (exception& ex) {
		ACS_THROW( InitException (ex, "Unable to start the Resource Service.") );
	}
	ACS_LOG_INFO("Resource management succesfully started");
}

void 
OrderExecutionMgr::reloadConfiguration()
{
	ThreadSafe::Lock lock (_mutex);
	if (_serviceResManager)
	{
		ACS_LOG_INFO ("Forcing reload configuration of the Resource Manager Service");
		// The service shall apply the new configuration without disruptions
		_serviceResManager->reloadConfiguration();
	}
	else
	{
		ACS_LOG_WARNING ("Reload configuration called but no Resource Manager Service exists");
	}
}

void
OrderExecutionMgr::setWSConfig (const WSConfig& a)
{
	_config=a;
}

void
OrderExecutionMgr::verbose(bool v)
{
	_verbose = v;
}

bool
OrderExecutionMgr::verbose() const
{
	return _verbose;
}

void
OrderExecutionMgr::stop(stopType stopMode)
{
	// do not handle any more orders
	// (this is needed to avoid race conditions during cleanup with other threads)
	{
		ThreadSafe::Lock lock(_mutex);
		_stopMode = stopMode;
	}

	// actions needed only by brute-force stopping process
	if (stopMode == IMMEDIATE_STOP)
	{
		// stop currently running orders

		// Get the internal orders slot vector
		vector <ExecutingOrder*> & ordersList = _orders.getVector();


		ExecutingOrder::stopType childStopMode = ExecutingOrder::IMMEDIATE_STOP;
		for_each(ordersList.begin(), ordersList.end(), bind2nd(mem_fun(&ExecutingOrder::haltOrder), childStopMode));

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"stop: halted all Executing Orders");
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"stop: CLEAN STOP received; not forwarding it to Executing Orders");
	}
}

OrderExecutionMgr::stopType OrderExecutionMgr::getStop() const
{
	ThreadSafe::Lock lock(_mutex);
	return _stopMode;
}

void
OrderExecutionMgr::cleanup()
{
	ACS_LOG_INFO("Order Execution Manager: cleaning up .... ");


	// called by tlDaemon upon exiting
	// wait for all running orders to exit
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"cleanup: calling specific ExecutingOrder cleanups");

	// Get the internal orders slot vector
	vector <ExecutingOrder*> & ordersList = _orders.getVector();

	for_each(ordersList.begin(), ordersList.end(), mem_fun(&ExecutingOrder::stopAndJoin));

	// finally, do actual cleanup and housekeeping
	purge();

	ACS_LOG_INFO("cleaning up...stopped");
}


inline ExecutingOrder *
purge_order(ExecutingOrder *p)
{
	if (p->isValid() || p->running())
	{
		return p;
	}
	else
	{
		delete p;
		return NULL;
	}
}

bool
OrderExecutionMgr::purge()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"purge: performing internal queue purge");

	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	ExecutingOrder *nullExecutingOrder = NULL ;

	// Get the internal orders slot vector
	vector <ExecutingOrder*> & ordersList = _orders.getVector();


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"purge: found " <<
			count(ordersList.begin(), ordersList.end(), nullExecutingOrder) <<
			" not purged before transform");


	// delete orders for which isValid() returns false, and set their value to NULL ...
	transform(ordersList.begin(), ordersList.end(), ordersList.begin(), purge_order);

	// if any completed orders were found, set return value to true
	bool retval = (count(ordersList.begin(), ordersList.end(), nullExecutingOrder) > 0);

	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
	int n = count(ordersList.begin(), ordersList.end(), nullExecutingOrder);
	if (n)
	{
		ACS_LOG_DEBUG("purge: found " <<
				count(ordersList.begin(), ordersList.end(), nullExecutingOrder) <<
				" to purge");
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"purge: found " <<
				count(ordersList.begin(), ordersList.end(), nullExecutingOrder) <<
				" to purge");
	}
	ACS_CLASS_END_DEBUG


	vector<ExecutingOrder *>::iterator j;

	// cannot use plain remove() algorithm, since either gcc or insure complain
	// for pointer usage
	j = remove_if(ordersList.begin(), ordersList.end(), bind2nd(equal_to<ExecutingOrder *>(), nullExecutingOrder));

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "purge: remove found "
			<< ordersList.end() - j << " orders to erase");


	ordersList.erase(j, ordersList.end());


	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "purge: found " 
			<< count(ordersList.begin(), ordersList.end(), nullExecutingOrder)
			<< " still to purge after remove_if") ;

	return retval;
}

bool
OrderExecutionMgr::haveResources()
{
	// verify if there are still resources available

	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	bool retval = false;
	if (getStop() == NO_STOP)
	{
		// The TL is still running and can accept new requests
		// verify if the service resource manager has free resources or not
		retval =  (_serviceResManager->getFreeResources() > 0);
	}

	return retval;
}

bool
OrderExecutionMgr::isProcessing()
{
	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	return (_orders.getVector().size() != 0);
}

bool
OrderExecutionMgr::canProcessOrder(const tlOrder& loadedOrder)
try
{
	string jobResp (loadedOrder.getJobResponsible ());
	string hostName (Application::hostname());

	if ((jobResp != string("")) && (jobResp != hostName))
	{
		if (_verbose)
		{
			ACS_LOG_INFO("Cannot process order. JobResponsible ["
					<< jobResp <<"] differs from hostname [" << hostName <<"]");
		}

		return false;
	}

	string p(loadedOrder.getProcessorName());
	string v(loadedOrder.getProcessorVersion());

	if (_config.isOrderProcessable(p, v, loadedOrder.getOrderType()) )
	{
		// it wouldn't be strictly necessary to load the task table at this point...
		// we are just trying to anticipate any possible errors in next call
		string tt;
		if (!_config.getTaskTable(p, v, tt))
		{
			ACS_LOG_WARNING("Cannot find task table for processor " <<
					p << " version " << v <<
					" in ws configuration file " << _config.getPath());

			return false;
		}
		else
			return true;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "canProcessOrder: " << loadedOrder.getOrderType()
				<< " order for processor " << p << " version " << v << " is not processable");
		return false;
	}
}
catch (exception& ex)
{
	ACS_LOG_WARNING("Exception caught while executing CanProcessOrder method - skipping current order (id #" << loadedOrder.getOrderId() << ")");
	// Add all the details only in verbose mode, otherwise just print the exception message
	if (_verbose)
	{
		ACS_LOG_NOTIFY_EX(ex);
	}
	else
	{
		ACS_LOG_ERROR (ex.what());
	}
	return false;
}

OrderReservedResourcesSptr
OrderExecutionMgr::reserveResourcesForOrder(const tlOrder& loadedOrder, bool& gotExceptions)
try
{
	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
	gotExceptions = false;

	OrderReservedResourcesSptr orderReservedResourcesSptr;

	string p(loadedOrder.getProcessorName());
	string v(loadedOrder.getProcessorVersion());

	string tt;
	if (!_config.getTaskTable(p, v, tt))
	{
		// should not happen
		ACS_LOG_WARNING("Cannot find task table for processor " <<
				p << " version " << v <<
				" in ws configuration file " << _config.getPath());
	}
	else
	{
		//Get Task Table from Factory
		StringKey key = ExecutingOrder::getTaskTableKeyFromOrder( loadedOrder );

		// Delete Task table at scope exit
		ACS_SMARTPTR<TaskTable> ttAPtr ( TaskTableFactoryChain::instance()->newObject( key ) );

		ttAPtr->readFromFile(tt);

		// 1- define how many resources are needed for this order (Resources required by current task table)
		// 2- generate a client id and interact with the resource manager to obtain the resources
		// 2a - if not possible return false and go ahead with the next order

		vector<Task> taskTableTasks;
		ttAPtr->getList (taskTableTasks, always_true_pred(taskTableTasks), true);

		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Got " << taskTableTasks.size() << " tasks from the task table "<<tt);

		size_t resourcesToReserve(1); // At least one
		vector<Task>::iterator it;
		for (vector<Task>::iterator it = taskTableTasks.begin(); it!=taskTableTasks.end(); ++it)
		{
			size_t taskRequiredResources = it->getNumberOfCPUs();
			resourcesToReserve = std::max( resourcesToReserve, taskRequiredResources );

			ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Task [" << it->getName() << "/" <<  it->getVersion() << "] - Needed CPUs " << taskRequiredResources);
		}

		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Max required Resources: " << resourcesToReserve);

		//Generate clientId
		string clientId = computeClientId (loadedOrder);

		if ( ! _serviceResManager->reserveResources(clientId, resourcesToReserve)  )
		{
			// No resources available. Return false to skip this order.
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "No resources available for starting the order " << loadedOrder.getOrderId() );

		} else {
			string resManagerUrl;
			bool isService = _serviceResManager->isService( resManagerUrl );
			orderReservedResourcesSptr . reset( new OrderReservedResources(
					*this, isService, _serviceResManagerKey, clientId, resManagerUrl ) );

			// From now the resource has been reserved and needs to be cleaned up.
			// They will be cleaned at orderReservedResources destructor.
		}

	}

	return orderReservedResourcesSptr;
}
catch(exception& ex)
{
	ACS_LOG_WARNING("Exception caught while executing reserveResourcesForOrder method - skipping current order (id #" << loadedOrder.getOrderId() << ")");
	// Add all the details only in verbose mode, otherwise just print the exception message
	if (_verbose)
	{
		ACS_LOG_NOTIFY_EX(ex);
	}
	else
	{
		ACS_LOG_ERROR (ex.what());
	}

	// Store the information that an exception has been caught inside
	gotExceptions = true;
	
	// And return an empty pointer to notify the parent the impossibility to complete the reservation
	OrderReservedResourcesSptr orderReservedResourcesSptr;
	return orderReservedResourcesSptr;
}

ExecutingOrder*
OrderExecutionMgr::createExecOrder (const tlOrder& loadedOrder, const OrderReservedResourcesSptr& orderReservedResourcesSptr)

try
{
	// this function assumes that a previous call to OrderExecutionMgr::canProcessOrder has been made
	acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock

	string p(loadedOrder.getProcessorName());
	string v(loadedOrder.getProcessorVersion());

	string tt;
	if (!_config.getTaskTable(p, v, tt))
	{
		// should not happen
		ACS_LOG_WARNING("Cannot find task table for processor " <<
				p << " version " << v <<
				" in ws configuration file " << _config.getPath());

		if (!_parent.updateOrderStatus(loadedOrder, OrderStatusELIGIBLE))
		{
			ACS_LOG_ERROR("Cannot update order status to " <<
					OrderStatus2String(OrderStatusELIGIBLE) <<
					" in the database for order " << loadedOrder.getOrderSignature());
			//	    exFError::notify (os.str(), excmcf);
		}
		else
		{
			// do some logging
			ACS_LOG_INFO("Order status for order " <<
					loadedOrder.getOrderSignature() <<
					" set to " <<
					OrderStatus2String(OrderStatusELIGIBLE));
		}

		return NULL;
	}

	// now create the executing copy of the order
	//
	try // need this, since the ExecutingOrder constructor loads the task table
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"getExecOrder: creating executing order " <<
				loadedOrder.getOrderSignature());

		string ll, outl, errl;

		if (!_config.getLogLevel(p, v, ll) || !_config.getErrLogLevel(p, v, errl) || !_config.getOutLogLevel(p, v, outl) )
		{
			ACS_LOG_WARNING("Cannot find log level for processor " <<
					p << " version " << v <<
					" in ws configuration file " << _config.getPath());

			if (!_parent.updateOrderStatus(loadedOrder, OrderStatusELIGIBLE))
			{
				ACS_LOG_ERROR("Cannot update order status to " <<
						OrderStatus2String(OrderStatusELIGIBLE) <<
						" in the database for order " << loadedOrder.getOrderSignature());
				//		 exFError::notify (os.str(), excmcf);
			}
			else
			{
				// do some logging
				ACS_LOG_INFO("Order status for order " <<
						loadedOrder.getOrderSignature() <<
						" set to " <<
						OrderStatus2String(OrderStatusELIGIBLE));
			}

			return NULL;
		}

		// create a new executing order
		string baseDir;
		{
			acs::ThreadSafe::Lock guard(_pathMutex); //automaticaly acquire the lock
			baseDir = _baseDir;
		}

		// APF-411 getting exit code
		bool posixExitCode;
		if ( !_config.hasPosixExitCode(p, v, posixExitCode) )
		{
			ACS_LOG_WARNING("Cannot find posix exit code definition for processor " <<
					p << " version " << v <<
					" in ws configuration file " << _config.getPath() << " - assuming Posix");
			posixExitCode = true;
		}

		// - create the executing order (and set the client id, the same used in reserveResourcesForOrder() )
		// -- the executing order must be able to release the resources in the dtor (callback in the OrderExecutionManager)
		// - in case of failures reset the resource manager

		ExecutingOrder *p = NULL;
		try
		{
			// cannot happen that getSlotNumber returns ZERO
			// because the slot availability is checked before creating a new order !!

			p = new ExecutingOrder(_orders.getSlotNumber(), *this, loadedOrder, tt, baseDir, ll, errl, outl, &_cacheManager, posixExitCode);

			//Set the resource manager parameters
			p->setResManager( orderReservedResourcesSptr );

		}
		catch (exception&)
		{
			//Note: Resurces will be released automatically at orderReservedResourcesSptr destruction
			throw;
		}

		// _clientIdMap[p->getAssignedSlot] = clientId;

		// store the created order...
		_orders.getVector().push_back(p);

		// ... and return it to caller
		return  p;
	}
	catch(exception& ex)
	{
		ACS_LOG_WARNING( "Cannot load task table " << tt <<
				" for processor " << p << " version " << v);
		ACS_LOG_NOTIFY_EX(ex);

		if (!_parent.updateOrderStatus(loadedOrder, OrderStatusELIGIBLE))
		{
			ACS_LOG_ERROR("Cannot update order status to " <<
					OrderStatus2String(OrderStatusELIGIBLE) <<
					" in the database for order " << loadedOrder.getOrderSignature());
			//	      exFError::notify (os.str(), excmcf);
		}
		else
		{
			// do some logging
			ACS_LOG_INFO("Order status for order " <<
					loadedOrder.getOrderSignature() <<
					" set to " <<
					OrderStatus2String(OrderStatusELIGIBLE));
		}

		return NULL;
	}
}
catch(exception& ex)
{
	ACS_LOG_WARNING("Exception got while executing CreateExecOrder method - skipping current order");
	ACS_LOG_NOTIFY_EX(ex);
	return NULL;
}

void OrderExecutionMgr::releaseClientResources (const string& clientId) throw()
{
	try
	{
		if (_serviceResManager != 0)
		{
			_serviceResManager->resetClient (clientId);
		}
		else
		{
			ACS_LOG_WARNING ("No resource manager allocated to release resources for client " << clientId);
		}
	}
	catch (exception& ex)
	{
		// do not re-throw. just notify as debug
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Release resources for client [" << clientId << "] got an exception: " << ex.what());
	}
}

string OrderExecutionMgr::computeClientId (const tlOrder& o) const
{
	ostringstream os;
	os << Application::hostname() << "_" << o.getOrderId();
	string clientId = os.str();
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Returning client id [" << clientId << "]");
	return clientId;
}


bool
OrderExecutionMgr::checkPreconditions(ExecutingOrder *execOrder)
try
{
	// check if inputs for this order are available in the database
	// and enough disk space is available
	if (!execOrder->check(_verbose))
	{
		// here we make logging and update order status
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checkPreconditions: check preconditions failed for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature() <<
				": setting status back to ELIGIBLE");

		if (!_parent.updateOrderStatus(*execOrder, OrderStatusELIGIBLE))
		{
			ACS_LOG_ERROR("Error occured during check preconditions: cannot update order status to " <<
					OrderStatus2String(OrderStatusELIGIBLE) <<
					" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());
			//	    exFError::notify (os.str(), excmcf);
		}
		else
		{
			// do some logging
			ACS_LOG_INFO("Order status for order " <<
					static_cast<const tlOrder&>(*execOrder).getOrderSignature() << " set to " <<
					OrderStatus2String(OrderStatusELIGIBLE));
		}

		// and then we abort the order
		abortOrder (execOrder);
		return false;
	}

	return true;
}
catch(InputResourceMgr::WaitingOrderException & ex)
{
	ACS_LOG_PRIVINFO (ex);
	ACS_LOG_WARNING ("WaitingList insertion failed for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature() << " setting to " << OrderStatus2String(OrderStatusFATAL_FAILURE));

	bool updateOk = true;

	if ( hasAllowedTransitionTable() )
	{
		// Update directly to ERROR. The transition PRECONDITIONCHECK->ERROR is allowed
		updateOk = _parent.updateOrderStatus(*execOrder, OrderStatusFATAL_FAILURE) ;
	}
	else
	{
		// Old scehma legacy code
		// double jump first to downloading and then to error because PRECONDITIONCHECK -> ERROR is not allowed
		// if update to downloading fails the update to error will not be attempted
		updateOk = _parent.updateOrderStatus(*execOrder, OrderStatusDOWNLOADING) && _parent.updateOrderStatus(*execOrder, OrderStatusFATAL_FAILURE) ;
	}

	if (not updateOk)
	{
		ACS_LOG_ERROR("Cannot update order status to " <<
				OrderStatus2String(OrderStatusFATAL_FAILURE) <<
				" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());
	}

	// multiple calls to abortOrder will not cause side effects
	abortOrder (execOrder);
	return false;

}
catch(exception& ex)
{
	ACS_LOG_WARNING("Exception got while executing CheckPreconditions method - skipping current order");
	ACS_LOG_NOTIFY_EX(ex);

	// have to abort current order since the parent will not do it
	if (!_parent.updateOrderStatus(*execOrder, OrderStatusELIGIBLE))
	{
		ACS_LOG_ERROR("Cannot update order status to " <<
				OrderStatus2String(OrderStatusELIGIBLE) <<
				" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());
		//              exFError::notify (os.str(), excmcf);
	}
	else
	{
		// do some logging
		ACS_LOG_INFO("Order status for order " <<
				static_cast<const tlOrder&>(*execOrder).getOrderSignature() << " set to " <<
				OrderStatus2String(OrderStatusELIGIBLE));
	}

	// multiple calls to abortOrder will not cause side effects
	abortOrder (execOrder);
	return false;
}


bool
OrderExecutionMgr::hasAllowedTransitionTable () const
{

	static bool checkPerformed = false;
	static bool retValue = false;

	// Double checked lock.. avoid lock if check already performed
	if (not checkPerformed)
	{

		// Get the lock here
		ThreadSafe::Lock lock (_mutex);

		// is the check still not performed here?
		if (not checkPerformed)
		{
			// This query will be executed only once
			dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
			dbConnection& conn (connWrp);
			string dbKey = conn.getKey();

			SQLString sqlString (dbKey, SQLString::SELECT);
			sqlString.addTableList ("t_dbparameters", "dbp");
			sqlString.addSelectList ("dbp.value_bool");

			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "dbp","parameter" , dbParam::EQUAL_TO, dbConv (dbKey,  allowedTransitionParam))) ;
			sqlString.addANDWhereParameter(pp) ;

			try
			{
				dbQuery query(conn, sqlString, "ChkAllowedTransition");
				dbSet set;
				query.execForSet(set);
				if (not set.empty() )
				{
					dbRow row(dbKey);
					set.getNextRow(row);
					row.column(0, retValue);
				}
			}
			catch (exception& ex)
			{
				ACS_LOG_WARNING ("Unable to detect if order state transitions are managed by transition table assuming it is not the case");
				ACS_LOG_PRIVINFO (ex);
			}

			checkPerformed = true;
		}
	}

	return retValue;
}


bool
OrderExecutionMgr::prepareOrder(ExecutingOrder *execOrder)
try
{
	// sending order information to Patrol
	ostringstream orderInformations;
	string s1, s2;
	const tlOrder& anOrder = (*execOrder);
	orderInformations << anOrder.getOrderId() 		 << "_" <<
			anOrder.getOrderType()		 << "_" <<
			anOrder.getStartTime().utcs(s1) << "_" <<
			anOrder.getStopTime().utcs(s2)  << "_" <<
			anOrder.getProcessorName();

	ostringstream boString;
	boString << "BOOKED_ORDER";
	boString << "#" << execOrder->getAssignedSlot();

	ACS_LOG_EVENT(boString.str(), orderInformations.str());
	//excmcf << event;

	//log that order was succesfully booked
	ACS_LOG_INFO( "Booked order " << anOrder.getOrderSignature() );

	// register the start of downloading updating database
	// state transition log is created by registerDownloadStart method

	if (!_parent.registerDownloadStart (*execOrder))
	{
		ACS_LOG_ERROR("Cannot update order status to " <<
				OrderStatus2String(OrderStatusDOWNLOADING) <<
				" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());

		// Have an error in the database. Cannot continue with this order.
		// Do the cleanup
		abortOrder (execOrder);
		return false;
	}

	if (!execOrder->download())
	{
		ACS_LOG_ERROR("ERROR: can't download inputs for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());

		if (!_parent.updateOrderStatus(*execOrder, OrderStatusFATAL_FAILURE))
		{
			// shouldn't happen: we have an error on the DB
			ACS_LOG_ERROR("Cannot update order status to " <<
					OrderStatus2String(OrderStatusFATAL_FAILURE) <<
					" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature() );
			//	    excmcf << msg;
		}
		else
		{
			// do some logging
			ACS_LOG_INFO("Order status for order " <<
					static_cast<const tlOrder&>(*execOrder).getOrderSignature() <<
					" set to " <<
					OrderStatus2String(OrderStatusFATAL_FAILURE) );
		}

		// do the cleanup
		abortOrder(execOrder);
		return false;
	}

	return true;
}
catch(exception& ex)
{
	ACS_LOG_WARNING("Exception got while executing PrepareOrder method - skipping current order");
	ACS_LOG_NOTIFY_EX(ex);

	// have to abort current order since the parent will not do it

	// multiple calls to abortOrder will not cause side effects
	abortOrder (execOrder);
	return false;
}


void
OrderExecutionMgr::abortOrder(ExecutingOrder *execOrder)
{
	if (execOrder)
	{
		acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "abortOrder: aborting the order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature() );

		// Get the internal orders slot vector
		vector <ExecutingOrder*> & ordersList = _orders.getVector();

		// delete the order from the vector...
		ordersList.erase(find(ordersList.begin(), ordersList.end(), execOrder));

		// ... and free its memory
		delete execOrder;
		execOrder = NULL;

	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "abortOrder: cannot abort a NULL executing order - multiple calls to abortOrder can cause this");
	}

}

bool
OrderExecutionMgr::processOrder(ExecutingOrder *execOrder)
{
	// Call the registerProcessingStart to update database entry
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "processOrder: going to process order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());

	if (!_parent.registerProcessingStart (*execOrder))
	{
		// shouldn't happen: we have an error on the DB
		ACS_LOG_ERROR("An error occured during processing the order: cannot update start information in the database for order " <<
				static_cast<const tlOrder&>(*execOrder).getOrderSignature());
		//	 excmcf << msg;

		// do the cleanup
		abortOrder(execOrder);
		return false;
	}

	// triggers if there is any exception during thread start
	bool gotException=false;

	// Try starting thread
	try
	{
		execOrder->exec();
	}
	catch (exception& e)
	{
		ACS_LOG_WARNING("Exception got while processing order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature() );
		ACS_LOG_NOTIFY_EX(e);

		gotException=true;
	}

	// do cleanup in case of exceptions
	if (gotException)
	{
		ACS_LOG_ERROR("ERROR: can't start ExecutingOrder for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());

		if (!_parent.updateOrderStatus(*execOrder, OrderStatusFATAL_FAILURE))
		{
			// shouldn't happen: we have an error on the DB
			ACS_LOG_ERROR("Cannot update order status to " <<
					OrderStatus2String(OrderStatusFATAL_FAILURE) <<
					" in the database for order " << static_cast<const tlOrder&>(*execOrder).getOrderSignature());
			//		  exFError::notify (os.str(), excmcf);
		}
		else
		{
			// do some logging
			ACS_LOG_INFO("Order status for order " <<
					static_cast<const tlOrder&>(*execOrder).getOrderSignature() << " set to " <<
					OrderStatus2String(OrderStatusFATAL_FAILURE) );
		}
		abortOrder(execOrder);
		return false;
	}

	// all is ok. thread started successfully
	return true;
}

void
OrderExecutionMgr::setBaseDir(const string& dir)
{
	acs::ThreadSafe::Lock guard(_pathMutex); //automaticaly acquire the lock
	_baseDir = dir;
	// Create a cache manager for the new base dir
	_cacheManager = CacheManager (_baseDir + cacheDirFromBase);
}

void
OrderExecutionMgr::notifySetupFailure (const ExecutingOrder& execOrder) const
{
	// ExecutingOrder calls this if there was un enexpected exception causing thread stop
	// exception is assumed to be notified by the underlying thread
	// we only have to register the processing stop and set the status order to error

	// APF-271
	MCScopedParams mcSlotId;

	// slotid must be a string
	ostringstream osslotid;
	osslotid << execOrder.getAssignedSlot();
	mcSlotId << mc_param("slotid", osslotid.str());

	ACS_LOG_ERROR("Setup failure while starting process for order " << static_cast<const tlOrder &>(execOrder).getOrderSignature());
	//   excmcf << msg;

	_parent.orderIsDone (execOrder, OrderStatusFATAL_FAILURE);


	// An order process was completed... Decrease PROCESSING
	(TlStatusMgrSingleton::instance()->getMgr())->exitStatus (TL_PROCESSING_ORDER);
}


bool
OrderExecutionMgr::updateOrderStatus(const ExecutingOrder& execOrder, const tlOrderStatus status, bool notifyOnError)
{
	return _parent.updateOrderStatus (execOrder.operator tlOrder(), status, notifyOnError);
}


void
OrderExecutionMgr::notifySchedulerCompletion(const ExecutingOrder& execOrder, SchedulerExitStatus status) const
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "notifySchedulerCompletion: handling order completion");


	tlOrderStatus s;

	// map the scheduler exit status to an order status ...
	switch(status)
	{
	case ssOK:
		s = OrderStatusDONE; break;

	case ssNO_ORDER:
	case ssNO_SPACE:
	case ssNO_WDIR:
		s = OrderStatusRECOVERABLE_FAILURE; break;

	case ssINTERNAL_ERR:
	case ssKILLED:
	case ssTASK_FAILED:
	case ssTASK_CRASHED:
	case ssABORTED:
	case ssSTOPPED:
		s = OrderStatusFATAL_FAILURE; break;

	default:
		//s = OrderStatusUNKNOWN;
		//.... should be UNKNOWN, but the database will get upset if we give him an illegal transition...
		// let's handle this as an internal failure
		ACS_LOG_WARNING("WARNING: unknown order status upon completion; forcing it to " <<
				OrderStatus2String(OrderStatusFATAL_FAILURE));
		s = OrderStatusFATAL_FAILURE;
	}


	// ... then make the parent update the order status

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "notifySchedulerCompletion: notifying completion to parent");


	_parent.orderIsDone(execOrder, s);

	// An order process was completed... Decrease PROCESSING
	(TlStatusMgrSingleton::instance()->getMgr())->exitStatus (TL_PROCESSING_ORDER);

}


class executingOrderFinder
{
private:
	unsigned int _orderId;

public:
	executingOrderFinder (unsigned int id) : _orderId (id)
{}

	bool operator() (ExecutingOrder*& exOrder)
	{
		const tlOrder& anOrder = (*exOrder);
		return ( (anOrder.getOrderId() == _orderId) && exOrder->isValid());
	}

};


ExecutingOrder*
OrderExecutionMgr::getExecutingOrderById (unsigned int orderId)
{
	ExecutingOrder* result = NULL;

	// Get the internal orders slot vector
	vector <ExecutingOrder*> & ordersList = _orders.getVector();

	vector<ExecutingOrder*>::iterator it = find_if (ordersList.begin(), ordersList.end(), executingOrderFinder(orderId));
	if (it != ordersList.end())
	{
		result = *it;
	}
	return result;
}

_ACS_END_NAMESPACE
