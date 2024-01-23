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
	Revision 3.1  2017/01/13 15:18:47  marfav
	Enforcing best practices to remove warnings
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.23  2016/05/11 11:53:57  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.22  2016/04/11 13:45:45  marpas
	adopting new db interfaces
	
	Revision 2.21  2016/03/29 17:16:35  marpas
	adopting new db I/F
	
	Revision 2.20  2016/01/27 14:33:13  marfav
	APF-271
	jobid is a string field and not a number
	
	Revision 2.19  2015/12/23 10:40:12  marfav
	Splunk messages updated
	
	Revision 2.18  2015/12/01 15:49:53  marfav
	Fixing metric types
	
	Revision 2.17  2015/11/26 15:14:25  marfav
	mc_params are now lowercase
	
	Revision 2.16  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.15  2015/08/12 14:21:43  marfav
	APF-271 work in progress
	
	Revision 2.14  2015/08/10 09:29:47  marfav
	PDSEV-8: checking schema before enabling blacklist management
	
	Revision 2.13  2015/08/07 14:40:18  marfav
	PDSEV-8 interaction with the db queue optimized and blacklist table support added
	
	Revision 2.12  2015/05/14 08:20:49  damdec
	Compilation warning fixed.
	
	Revision 2.11  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.10  2013/03/14 15:05:39  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.9  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.8  2013/03/08 18:33:28  marfav
	Using connection pool
	
	Revision 2.7  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.6  2010/10/15 07:32:46  marfav
	exception type fixed in run method
	
	Revision 2.5  2010/10/14 10:49:33  marfav
	Added try catch block in the connection management section
	
	Revision 2.4  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.3  2008/11/14 11:42:43  marfav
	Using ConfigurationSingleton instead of MergeConfiguration to load confspaces
	
	Revision 2.2  2008/04/22 13:35:28  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.1  2006/11/10 13:48:28  fracar
	handling parameters file loading from the order table clob
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.41  2005/05/02 16:03:40  marfav
	preHandling is boolean now
	preHandling and postHandling can use a dbConnection to perform checks
	the connection is managed by OrderQueueMgr
	
	Revision 1.40  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.39  2005/03/09 09:30:43  marfav
	Order queue management changed
	now the orders are put in a waitlist when precondition fails
	the database will resume the waiting orders when the right input is inventoried
	
	Revision 1.38  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.37  2004/10/19 14:19:36  marfav
	Old code removed
	
	Revision 1.36  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.35  2004/05/06 16:31:10  marfav
	CHECKING_QUEUE status extended to preconditionCheck phase
	PROCESSING_ORDER status begins when preparing order (downloading)
	
	Revision 1.34  2004/05/05 14:21:08  marfav
	Removed order state transition message to Patrol in tryBookOrder method
	
	Revision 1.33  2004/05/04 16:23:44  marfav
	Using Thread::nanosleep instead of ::nanosleep
	
	Revision 1.32  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.31  2004/03/22 17:40:34  marfav
	BOOKED_ORDER message moved from PreconditionCheck to just before Downloading
	
	Revision 1.30  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.28  2003/09/25 09:38:09  marfav
	Removed support for internal order queue
	
	Revision 1.27  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.26  2003/09/18 15:42:30  marfav
	Fixing the last log message about Phoenix Support
	All system calls for filesystem managing removed. Using libFile instead
	
	Revision 1.25  2003/09/18 15:36:15  marfav
	Phoenix support fixed
	
	Revision 1.24  2003/09/15 10:27:22  marfav
	postHandling method added
	
	Revision 1.23  2003/08/01 11:03:30  marfav
	Added support for thread Randez-Vous and InternalOrderQueue
	
	Revision 1.22  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time
	
	Revision 1.21  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.20  2003/07/16 16:37:30  marfav
	Align to new version of libCryosatApp
	
	Revision 1.19  2003/07/10 14:18:32  marfav
	clone method added to exceptions
	
	Revision 1.18  2003/07/04 15:06:33  marfav
	Using Sigalarm Emulator and implementing restart
	
	Revision 1.17  2003/06/06 10:31:04  marfav
	Dead code removed
	
	Revision 1.15  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
	Revision 1.14  2003/05/13 08:43:15  marfav
	Align to new DbPersistentFactory constraints for DbCryosat objects
	
	Revision 1.13  2003/04/18 16:26:06  fracar
	align to new compiler version
	
	Revision 1.12  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.11  2003/03/27 15:33:22  marfav
	Added plugin support and some logging improvements
	
	Revision 1.10  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support
	
	Revision 1.9  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.8  2003/02/18 09:09:22  fracar
	improved logging
	
	Revision 1.7  2003/02/10 18:50:19  fracar
	prepared for multi-interval management
	
	Revision 1.6  2003/02/10 09:17:48  fracar
	improved logging
	
	Revision 1.5  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.4  2003/02/04 17:17:58  fracar
	updating job responsible field
	
	Revision 1.3  2003/02/04 14:04:39  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/

#include <OrderQueueMgr.h>

#include <SatelliteName.h>
#include <dbPersistentFactoryChain.h>
#include <dbConnectionPool.h>
#include <MCFilterables.h>
#include <exStream.h>

#include <tlDaemon.h>
#include <TlStatusMgrSingleton.h>
#include <TlOrderQueueSingleton.h>

#include <algorithm>
#include <memory>
#include <signal.h>
#include <time.h>

#include <sys/time.h>

#include <Application.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;
using namespace acs::mc;


bool OrderQueueMgr::_gotSigAlrm = false;
bool OrderQueueMgr::_gotSigUsr2 = false;

namespace {
// Class FlagGuard
// Sets a bool to true at CTOR and restore its original value at DTOR
  class FlagGuard
  {
	  public:
		  explicit FlagGuard (bool& v) throw() : _flag(v), _oldValue(v) { _flag = true; }
		  ~FlagGuard () throw() { _flag = _oldValue; }
	  private:
        FlagGuard() ; // not implemented 
        FlagGuard(const FlagGuard &) ; // not implemented 
        FlagGuard& operator=(const FlagGuard &) ; // not implemented 
      private:
		  bool& _flag;
		  bool _oldValue;
  };

}


bool
OrderQueueMgr::isHandling ()
{
	return _isHandling;
}


// --------------------------------------------------

string priorityOrder2String (PriorityOrder t)
{
  switch (t)
  {
    case PriorityOrderAscending:		return "Ascending";		break;
    case PriorityOrderDescending:		return "Descending";	break;
    default: return "unknown";
  }
}

PriorityOrder string2PriorityOrder (const std::string& s)
{
    if (s == "ascending"  || s == "Ascending" || s == "ASCENDING")
      return PriorityOrderAscending;
    else if (s == "descending" || s == "Descending" || s == "DESCENDING")
      return PriorityOrderDescending;
    else
      return PriorityOrderUnknown;
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderQueueMgr)

 
OrderQueueMgr::OrderQueueMgr(tlDaemon& aDaemon) : 
    Thread ("OrderQueueMgr"),
    _parent(aDaemon),
    _priorityOrder(PriorityOrderAscending),
    _timerFreq(0),
    _maxRetries(2),
    _busy(false),
    _verbose(false),
    _alrmHdlrInstalled(false),
    _timerEmulator(),
    _isHandling(false),
    _mutex()
{

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		_verbose = true;
    ACS_CLASS_END_DEBUG
}


OrderQueueMgr::~OrderQueueMgr() throw() {}

// -------------------------------------------------------------------------------------
//                                    RETRY - CATCH
// -------------------------------------------------------------------------------------


string
OrderQueueMgr::op_type2string (op_type operation)
{
  switch (operation)
  {
    case LOAD:
      return "LOAD";
      break;
    case UPDATE:
      return "UPDATE";
      break;
  }
  return "";
}


namespace {
	std::string getSatelliteName( const dbOrder& adbOrder  ) {
		string satelliteName("");
		bool isSatelliteSet(false);
		int satId = adbOrder.getSatelliteId(&isSatelliteSet);
		try {
			if ( isSatelliteSet ) {
				satelliteName = SatelliteName::instance(Application::instance()->getDefaultConnectionPool()->getKey())->name( satId );
			}
		} catch (exception& exc) {
			ACS_LOG_WARNING ("ExecutingOrder CTOR Unable to retrieve the satelliteName from the Id in the database. Defaulting to empty string. Got exception: " << exc.what());
		}
		return satelliteName;
	}
}


bool
OrderQueueMgr::retry_catch (op_type operation, dbOrder& adbOrder, bool notifyOnError, unsigned int orderId)
{
	bool flag=false;
	int counter=0;
	dbConnectionWrapper connWrp ( *( Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);
	while (flag==false && counter<_maxRetries)
	{
		try
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"retry_catch: executing command "<< op_type2string(operation) << "... try #" << counter+1);

			switch (operation)
			{
				case LOAD:
					adbOrder.load(conn, orderId);
					break;
				case UPDATE:
					adbOrder.update(conn);
					tlOrderStatus status = string2OrderStatus(adbOrder.getStatus());

					// jobid must be a string
					ostringstream osjobid;
					osjobid << adbOrder.getOrderId();

					//At processing start (downloading status), send a RUNNING message metric.
					// This allows to show processing info about the processing order.
					// Note: a second RUNNING message will be send when the actual processing starts.
					if ( OrderStatusDOWNLOADING == status ) {
						ACS_LOG_MC_INFO ( mc_param("jobid", osjobid.str()) <<
								mc_param("satellite", getSatelliteName( adbOrder ) ) <<
								mc_param("missionnumber", adbOrder.getMission()) <<
								mc_param("processorname", adbOrder.getProcessorName()) << mc_param("processorversion", adbOrder.getProcessorVersion()) <<
								mc_param ("sensingstarttime", adbOrder.getStartTime().asISO8601(3)) <<
								mc_param ("sensingstoptime", adbOrder.getStopTime().asISO8601(3)) <<
								mc_metric(Metric_RUNNING));
					}

					{
						// This will last until exits from the current scope
						MCScopedParams sp ;
						sp << mc_param("jobid", osjobid.str());
						sp << mc_metric(Metric_PROGRESS);

						// APF-271
						// Issue the MC message for the status update
						switch (status)
						{
							case OrderStatusDOWNLOADING:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 0) << mc_param ("status", "DOWNLOADING")) ; }
								break;
							case OrderStatusPROCESSING:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 0) << mc_param ("status", "PROCESSING")) ; }
								break;
							case OrderStatusELIGIBLE:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 0) << mc_param ("status", "IDLE")) ; }
								break;
							case OrderStatusCHECKING:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 0) << mc_param ("status", "CHECKING")) ; }
								break;
							case OrderStatusDONE:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 100) << mc_param ("status", "COMPLETED")) ; }
								break;
							case OrderStatusFATAL_FAILURE:
							case OrderStatusRECOVERABLE_FAILURE:
								{ ACS_LOG_MC_INFO( mc_param("percentagedone", 100) << mc_param ("status", "FAILED")) ; }
								break;
							default:
								break;
						}
					}
					break;
			}

			flag=true;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"retry_catch: executing command " << op_type2string(operation) << "... OK");
		}
		catch(exception& e)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"retry_catch: exception caught in attempt #" << counter+1 <<
					" executing " << op_type2string(operation) << "...");


			counter++;

			if ((counter == _maxRetries) && notifyOnError)
			{
				ACS_LOG_ERROR("ERROR cannot execute " << op_type2string(operation) );
				ACS_LOG_NOTIFY_EX(e);
			}
		}
	}
	return flag;
}


// -------------------------------------------------------------------------------------
//                       UPDATE ORDER STATUS
// -------------------------------------------------------------------------------------

bool
OrderQueueMgr::updateOrderStatus(const tlOrder& aTlOrder, const tlOrderStatus status, bool notifyOnError)
{
	ACS_SMARTPTR<dbOrder> aDbOrder ( dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", _parent.getDefaultConnectionPool()->getKey())) );

 	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"updateOrderStatus: updating order " << aTlOrder.getOrderSignature() << " status to " << OrderStatus2String(status));


// order retrieval and update on database
// every DB operation is redounded in the catch block
// no exception is handled

	bool retValue=true;

// Load
	if (!retry_catch(LOAD, *aDbOrder, notifyOnError, aTlOrder.getOrderId()))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"updateOrderStatus: - LOAD failed - order "<< aTlOrder.getOrderSignature());

		// return false;
		retValue=false;
	}
	else
	{
		aDbOrder->setStatus(OrderStatus2String(status));

		// Update
		if (!retry_catch(UPDATE, *aDbOrder, notifyOnError))
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"updateOrderStatus: - UPDATE failed - order "<< aTlOrder.getOrderSignature());

			// return false;
			retValue=false;
		}
	}


	return retValue;
}


// -------------------------------------------------------------------------------------
//                       REGISTERPROCESSINGSTART
// -------------------------------------------------------------------------------------
bool
OrderQueueMgr::registerProcessingStart(const tlOrder& aTlOrder)
{
	ACS_SMARTPTR<dbOrder> aDbOrder ( dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", _parent.getDefaultConnectionPool()->getKey())) );

	DateTime tstart;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStart: registering start order " << aTlOrder.getOrderSignature());


// retrieve and update the database related order

	bool retVal=true;

// Load
	if (!retry_catch(LOAD, *aDbOrder, true, aTlOrder.getOrderId()))
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStart: - LOAD failed - order "<< aTlOrder.getOrderSignature());

	// return false;
		retVal=false;
	}
	else
	{
		// Fields update
		aDbOrder->setStatus(OrderStatus2String(OrderStatusPROCESSING));
		aDbOrder->setProductionStopTime(tstart);

		// Update
		retVal = retry_catch(UPDATE, *aDbOrder, true);

		// do some logging before returning
		if (retVal)
		{
			tlOrder updatedOrder;
			Order2Order (*aDbOrder, updatedOrder);

			// log state transition
			ACS_LOG_INFO("OrderQueueMgr: order status for order " <<
			        updatedOrder.getOrderSignature() <<
			        " set to " <<
			        updatedOrder.getStatus());
		}
		else
		{
			// this is definitely an error, but let's have the caller log
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStart: couldn't set order status to " <<
				OrderStatus2String(OrderStatusPROCESSING) <<
				" for order " <<
				aTlOrder.getOrderSignature());
		}
	}
	
	// return rc;
	// delete aDbOrder;
	return retVal;
}

// -------------------------------------------------------------------------------------
//                       REGISTER_DOWNLOAD_START
// -------------------------------------------------------------------------------------
bool
OrderQueueMgr::registerDownloadStart(const tlOrder& aTlOrder)
{
	ACS_SMARTPTR<dbOrder> aDbOrder ( dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", _parent.getDefaultConnectionPool()->getKey())) );

	DateTime tstart;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerDownloadStart: registering download order " << aTlOrder.getOrderSignature() );
	
// retrieve and update the database related order

	bool retVal = true;

// Load
	if (!retry_catch(LOAD, *aDbOrder, true, aTlOrder.getOrderId()) )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerDownloadStart: - LOAD failed - order "<< aTlOrder.getOrderSignature() );

		//return false;
		retVal=false;
	}
	else
	{
		// Fields update
		aDbOrder->setStatus(OrderStatus2String(OrderStatusDOWNLOADING));
		aDbOrder->setProductionStartTime(tstart);
		aDbOrder->setProductionStopTime(tstart);

		string machinename (Application::hostname());
		aDbOrder->setJobResponsible(machinename);


		// Update
		retVal = retry_catch(UPDATE, *aDbOrder, true);

		// do some logging before returning
		if (retVal)
		{
			tlOrder updatedOrder;
			Order2Order (*aDbOrder, updatedOrder);

			// log state transition
			ACS_LOG_INFO("Order status for order " <<
				updatedOrder.getOrderSignature() <<
				" set to " <<
			updatedOrder.getStatus());
		}
		else
		{
			// this is definitely an error, but let's have the caller log
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerDownloadStart: couldn't set order status to " <<
					OrderStatus2String(OrderStatusDOWNLOADING) <<
					" for order " <<
				aTlOrder.getOrderSignature());
		}
	}

	// delete aDbOrder;

	return retVal;
}



// -------------------------------------------------------------------------------------
//                       REGISTERPROCESSINGSTOP
// -------------------------------------------------------------------------------------
bool
OrderQueueMgr::registerProcessingStop(const tlOrder& aTlOrder, const tlOrderStatus status)
{
    ACS_SMARTPTR<dbOrder> aDbOrder ( dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", _parent.getDefaultConnectionPool()->getKey())) );

    DateTime tstop;

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStop: registering stop order " << aTlOrder.getOrderSignature());

    bool retVal = true;

// Load
    if (!retry_catch(LOAD, *aDbOrder, true, aTlOrder.getOrderId()) )
    {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStop: - LOAD failed - order "<< aTlOrder.getOrderSignature());

	// return false;
	retVal=false;
    }
    else
    {

	// fields update
	aDbOrder->setStatus(OrderStatus2String(status));
	aDbOrder->setProductionStopTime(tstop);

	// Update
	retVal = retry_catch(UPDATE, *aDbOrder, true);

	// do some logging before returning
	if (retVal)
	{
	    tlOrder updatedOrder;
	    Order2Order (*aDbOrder, updatedOrder);

	    // log state transition
	    ACS_LOG_INFO("Order status for order " <<
		    updatedOrder.getOrderSignature() <<
		    " set to " <<
		    updatedOrder.getStatus());
	}
	else
	{
	    // this is definitely an error, but let's have the caller log
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registerProcessingStop: couldn't set order status to " <<
			    OrderStatus2String(status) <<
			    " for order " <<
			    aTlOrder.getOrderSignature());
	}
    }

    //delete aDbOrder;
    return retVal;
}

void
OrderQueueMgr::setPriorityOrder(PriorityOrder o)
{
  _priorityOrder = o;
}

PriorityOrder
OrderQueueMgr::getPriorityOrder() const
{
  return _priorityOrder;
}

int 
OrderQueueMgr::getMaxRetries () const 
{ 
    return _maxRetries; 
}

const tlDaemon& 
OrderQueueMgr::getParent () const
{
    return _parent;
}

tlDaemon& 
OrderQueueMgr::getParent ()
{
    return _parent;
}


void
OrderQueueMgr::verbose(bool v)
{
    _verbose = v;
}

bool
OrderQueueMgr::verbose() const
{
    return _verbose;
}

void
OrderQueueMgr::pause()
{
   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"pause: pausing");


   if (!requestedCancel()) // avoid pausing if in the process of being shut down
   {
    _busy = true;
   }
   else
   {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"pause: ignoring pause request because of pending cancellation request");
   }
}

void
OrderQueueMgr::resume()
{
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"resume: resuming");

  _busy = false;
}

void
OrderQueueMgr::wakeUp()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"wakeUp: waking up the Queue Manager and simulating a Broadcase msg reception");

    resume();

#define THIS_PARAMETER_IS_IGNORED 0
    sigUsr2Hdlr (THIS_PARAMETER_IS_IGNORED);
}


void
OrderQueueMgr::startQueueMgr()
{
    try
    {
	// initialisation
	_init();
	ACS_LOG_INFO("Order queue manager initialisation completed");
	start();
    }
    catch (exException& e)
    {
	// we just notify exception at this point without re-throwing
	// because in that case the thread is not running

	ACS_LOG_ERROR("ERROR: Order Queue Manager start failed");
	ACS_LOG_NOTIFY_EX(e);

    }

}

void
OrderQueueMgr::setTimerFrequency(int period_in_sec)
{
  {
    acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
    _timerFreq = period_in_sec;
  }

  ACS_LOG_INFO("OrderQueueMgr: timer frequency set to " << _timerFreq << " seconds");

}

int
OrderQueueMgr::getTimerFrequency()
{
  int period_in_sec;
  {
      acs::ThreadSafe::Lock guard(_mutex); //automaticaly acquire the lock
      period_in_sec = _timerFreq;
  }
  return period_in_sec;
}

void
OrderQueueMgr::restartTimer(bool reset)
{
     // now setup interval timer
     struct itimerval value;

     if (reset)
     {
          // avoid initial sigalrm if _timerFreq was set to zero
          value.it_value.tv_sec = (getTimerFrequency() ? 5:0);   // seconds (start reasonably soon with first attempt)
          value.it_value.tv_usec = 0;		// microseconds
     }
     else
     {
         // the first parameter will be ignored by getitimer
          _timerEmulator.getitimer (0, &value);

	 ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "restartTimer: current countdown is [ " << value.it_value.tv_sec << " seconds"
                            << " and " << value.it_value.tv_usec << " microsecond]");

     }

     value.it_interval.tv_sec = getTimerFrequency();	// seconds
     value.it_interval.tv_usec = 0;		// microseconds

     armTimer (value);
}

void
OrderQueueMgr::armTimer (const struct itimerval& value)
{
    if (_timerEmulator.setitimer(ITIMER_REAL, &value, NULL) != 0) // note that _timerFreq has been set to 0, this is NOP
    {
	ACS_LOG_WARNING("WARNING: cannot set the interval timer; errno=" << errno);

	// Here we set _timerFreq = 0 so the getStatus method can have knowledge of no timer activity
	setTimerFrequency(0);
    }

    if (getTimerFrequency() == 0)
    {
	// This is a warning. Specialized QueueManager will handle if continue running or not
	// when getStatus method is called
	// This happens also wher the timer cration fails
	ACS_LOG_WARNING("WARNING: no active timer for orders polling. Relying on BroadcastListener (if any)");

    }
}

void
OrderQueueMgr::restartQueueMgr()
{
    restartOQM();
    _restartOQM();
}

void
OrderQueueMgr::_restartOQM()
{
    restartTimer();
}

void
OrderQueueMgr::stopQueueMgr()
{
    // WARNING: this func is supposed to be called from the parent thread!

    ACS_LOG_WARNING("Stopping Queue Manager");


    // stop the thread
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"stopQueueMgr: calling Thread::stop");

    // let's wake up, in case we were in busy status
    resume();
    stop();

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"stopQueueMgr: Thread::stop called");


    // clean up
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"stopQueueMgr: calling _cleanup");

    _cleanup();

    ACS_LOG_INFO("Queue Manager stopped");

}

void
OrderQueueMgr::_init()
{
    try
    {

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_init: calling specific initialisation");

		init(); // specific derived initialisation

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_init: installing signal handlers");

		_gotSigAlrm = _gotSigUsr2 = false;
		installHandlers();

		ACS_LOG_INFO("Signal handlers installed");

		// restart timer and prepare it for the first checking queue attempt
		restartTimer (true);

		// Cleanup the blacklist when starting the queue
		cleanupBlacklistItems();
    }
    catch (exception& e)
    {
	ACS_LOG_ERROR("ERROR: initialisation failed");
	ACS_LOG_NOTIFY_EX(e);

	// we notify exception at this point, then re-throw
	// in order to make the thread to stop and be
	// picked-up by the parent monitoring status
        throw;
    }
}

void
OrderQueueMgr::sigUsr2Hdlr(int)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"sigUsr2Hdlr: SIGUSR2 received");

    _gotSigUsr2 = true;
}


void
OrderQueueMgr::sigAlrmHdlr(int)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"sigAlrmHdlr: SIGALRM received");

     _gotSigAlrm = true;
}

// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"
void
OrderQueueMgr::installHandlers()
{
    bool willIEverBeWokenUp = false; // will be true if at least one of the handlers has been correctly installed



    // install SIGUSR2 handler
    //
    if (signal(SIGUSR2, sigUsr2Hdlr) == SIG_ERR)
    {
	ACS_LOG_WARNING("WARNING: cannot install SIGUSR2 signal handler; only relying on periodic orders queue polling by timer interrupt" <<
	       " (errno=" << errno << ")");
    }
    else
	willIEverBeWokenUp = true;

    // install SIGALRM handler
    //

    if (_timerEmulator.signal (sigAlrmHdlr) != 0)  // Impossible
    {
	ACS_LOG_WARNING("WARNING: cannot install SIGALRM signal handler; only relying on DB broadcast interrupt for queue polling" <<
	       " (errno=" << errno << ")");
    }
    else
    {
	willIEverBeWokenUp = true;
	_alrmHdlrInstalled = true;
    }


    // ACS_THROW will stop the thread and the parent will get informations using the getStatus method
    if (!willIEverBeWokenUp)
    {
	ACS_THROW (OrderQueueMgrException("ERROR cannot install signal handlers; will never be woken up for queue polling"));
    }
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

void
OrderQueueMgr::run()
{
    ACS_LOG_INFO("Thread started");

    // main loop; quitted only by a stop call
    while (!requestedCancel())
    {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"run: pausing");


	do
	{
	      // in this point is safe to change the configuration parameters of the whole daemon
	      // if some executing order threads are running... they have their own settings and will continue without
	      // any variation

	      // the rendezVous call means that the OQM is ready to accept the new configuration
	      // the method will exit immediatly if there is no need to change the parameters
	      _parent.rendezVous();
	   
	      nanosleep(100U * 1000000U); // 1 tenth of a second

	      ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"run: nanosleep completed; requestedCancel()=" << int(requestedCancel()) <<
			    " _busy=" << int(_busy) << " _gotSigAlrm=" << int(_gotSigAlrm) << " _gotSigUsr2=" << int(_gotSigUsr2) <<
			    " while conditions evaluates to " << int(!requestedCancel() && (_busy || (!_gotSigAlrm && !_gotSigUsr2))));
	}
	while (!requestedCancel() && (_busy || (!_gotSigAlrm && !_gotSigUsr2)));


		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"run: woken up");


	// ... ok: let's see why we woke up...

	      // Entering in and then exiting from the status "CHECKING_QUEUE"
	      // The checking queue status will be active until the orderqueuemanager goes to sleep !
	      TlStatusMgrSingleton::StatusGuard _g(*TlStatusMgrSingleton::instance()->getMgr(), TL_CHECKING_QUEUE);


	// before going on with processing, let's check if in the meantime
	// the parent has stopped us
	if (!requestedCancel())
	{
	  // woken up by (_gotSigAlrm || _gotSigUsr2)


   	  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,":run: not busy, loading");

	  _gotSigAlrm = _gotSigUsr2 = false; //reset flags...
	  vector<tlOrder> Orders;

         try
         {

	  {
	      // next call handles internally errors from database: empty vector is returned in this case
	      retrieveOrders(Orders);
	      TlOrderQueueSingleton::instance() -> load (Orders);
	  }


	  // as soon as no order is processable, or max in progress orders limit is reached
	  // the parent daemon will return false (== cannot handle more)
	  // note that it is assumed that if an order cannot be processed the daemon will return true anyway
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"run: retrieved " << Orders.size() << " orders");


          {
	      // Use the same connection for the handling operations
	      // This connection is kept open until the while loop exits
	      // and will allow to execution of the preHandling and postHandling operations
	      // without having the need to create a new connection for each order

	      
	      while (!TlOrderQueueSingleton::instance() -> isEmpty() )
	      {
		try
		{
			// check if we've been stopped in the meantime
			if (!requestedCancel())
			{
			   _parent.rendezVous();
			   tlOrder order = TlOrderQueueSingleton::instance() -> getNext();

			   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"run: going to make parent handle order " << order.getOrderSignature() );

			  if (!handleOrder(order))
			     break;

			} // if (!requestedCancel())
			else
			{
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"run: cancellation requested");

			  break;
			}
		}
		catch (TlOrderQueue::TlOrderQueueException& ex)
		{
			// Ooops... someone deleted the last order in the meantime...
			// Just continue... the OrdersQueue now is surely empty
		}

	      } // while (!OrdersQueueSingleton::instance() -> isEmpty() )
	  } // connection guard block

          } // main try ends here
          catch (exception& ex)
	  {
		// An untrapped exception... There is a problem with the queue manager
		// notify the problem and continue working
//		exFError::notify ("OrderQueueManager is not able to access the order queue", excmcf);
		ACS_LOG_ERROR("OrderQueueManager is not able to access the order queue");
		ACS_LOG_NOTIFY_EX(ex);


		// Pulse the Warning status on the Thin Layer status manager
		(TlStatusMgrSingleton::instance()->getMgr())->pulseStatus (TL_WARNING);

	  }


		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"run: finished with this order set");

	} // if (!requestedCancel())
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"run: cancellation requested");
	}
    } // while (!requestedCancel())

    ACS_LOG_INFO("Quitting the run");

}

bool
OrderQueueMgr::handleOrder (tlOrder& aTlOrder)
{
    FlagGuard handleGuard (_isHandling);
    if (!_preHandling (aTlOrder))
    {
        // this order must not be executed for some NOMINAL reason...
	// return true and go for the next one
	// note: this means that the order has not even to be put in precondition check status
	//       smthg different from a failed precondition check!

	return true;
    }

    bool result = _parent.handleOrder (aTlOrder);

    _postHandling (aTlOrder);

    return result;
}

bool
OrderQueueMgr::_preHandling (tlOrder& o)
{
    return preHandling (o);
}

void
OrderQueueMgr::_postHandling (tlOrder& o)
{
    postHandling (o);
}

bool
OrderQueueMgr::tryBookOrder(const tlOrder& anOrder)
try
{

  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"tryBookOrder: booking order " << anOrder.getOrderSignature());

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"tryBookOrder: setting order status to " <<
		OrderStatus2String(OrderStatusCHECKING) << 
                " for order " <<
                anOrder.getOrderSignature());



  bool rc = updateOrderStatus(anOrder, OrderStatusCHECKING, false);

  // do some logging before returning
  if (rc)
  {
      // log state transition
      ACS_LOG_INFO("Order status for order " <<
		anOrder.getOrderSignature() <<
		" set to " <<
		OrderStatus2String(OrderStatusCHECKING));
  }
  else
  {
    // not necessarily an error...
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"tryBookOrder: couldn't set order status to " <<
		      OrderStatus2String(OrderStatusCHECKING) << 
		      " for order " <<
		      anOrder.getOrderSignature());

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"tryBookOrder: couldn't book order " << anOrder.getOrderSignature());

  }

  return rc;
}
catch (exception& ex)
{
   ACS_LOG_WARNING("Exception got while executing TryBookOrder method - skipping current order");
   ACS_LOG_NOTIFY_EX(ex);
   return false;
}

// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"
void
OrderQueueMgr::_cleanup()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"_cleanup: calling specific derived cleanup");

  cleanup();


  // stop the RTC alarm
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"_cleanup: disabling the timer");

	// using the _timerEmulatot.setitimer
	// has the same effect but stopTimer simply joins
	// the thread befor exiting the main loop of the TL
	_timerEmulator.stopTimer ();


  // reset the signal handlers to their default value
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"_cleanup: resetting signal handlers");

	// no need to unset the timer handler for the timerEmulator
  signal(SIGUSR2, SIG_DFL);
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"



_ACS_END_NAMESPACE
