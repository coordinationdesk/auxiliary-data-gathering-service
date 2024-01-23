// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
	Revision 3.6  2017/11/06 18:55:56  nicvac
	ECICMF-94: Resource reserved for an order and automatic resources release.
	
	Revision 3.5  2017/11/06 17:15:39  nicvac
	ECICMF-94: resource computation. Reservation before precondition check
	
	Revision 3.4  2017/10/24 15:37:44  marfav
	ECICMF-94 Preintegrazione servizi - Caso SLOT based coperto
	
	Revision 3.3  2017/10/23 15:46:33  marfav
	Resource management service integration in progress
	
	Revision 3.2  2017/10/19 15:52:05  marfav
	Integration of service based resource manager
	
	Revision 3.1  2017/01/12 11:20:34  marfav
	Checking the dbParameters table to detect if direct transition
	from precondition to error is correctly supported
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.6  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.5  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.4  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.3  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.2  2011/01/24 16:21:06  marpas
	Mutex.h does not longer exixts
	
	Revision 2.1  2008/04/22 13:35:28  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.18  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.17  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.16  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.15  2004/05/17 08:18:28  marfav
	ordersMutex moved in threadsafe OrderSlot class
	
	Revision 1.14  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.13  2004/05/05 14:22:05  marfav
	Patch for SUSPENDED order state management added (CalVal environement only)
	
	Revision 1.12  2004/04/07 13:19:56  marfav
	Added support for IPC from ThinLayer Clients and to running Schedulers
	Does not use ACE anymore
	Using exStream.h
	
	Revision 1.11  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.10  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.9  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time
	
	Revision 1.8  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.7  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support
	
	Revision 1.6  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.5  2003/02/18 09:09:22  fracar
	improved logging
	
	Revision 1.4  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.3  2003/02/04 11:56:17  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _ORDER_EXECUTION_MGR_H
#define _ORDER_EXECUTION_MGR_H

#include <exException.h>
#include <SchedulerExitStatus.h>
#include <WSConfig.h>
#include <tlOrder.h>
#include <OrderSlot.h>
#include <OrderReservedResources.h>
#include <CacheManager.h>

#include <ThreadSafe.h>

#include <string>
#include <vector>

#include <ThinLayerServicesFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

class tlOrder;
class tlDaemon;
class ExecutingOrder;


class OrderExecutionMgr // PRQA S 2109
{
public:
	/*! class OrderNumException declaration */
	exDECLARE_EXCEPTION(OrderNumException, exException) ; // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(InitException, exException) ; // PRQA S 2131, 2502

	enum stopType { NO_STOP, CLEAN_STOP, IMMEDIATE_STOP };

	OrderExecutionMgr(tlDaemon& aDaemon, const WSConfig& aConfig);
	virtual ~OrderExecutionMgr() throw() ;

	void setBaseDir(const std::string& dir);
	void setWSConfig (const WSConfig&);
	void verbose(bool v);
	bool verbose() const;

	void cleanup();
	bool purge();

	bool haveResources();
	bool canProcessOrder(const tlOrder& loadedOrder);

	/** Pre-allocate resources for the order.
	Return Reserved resources as OrderReservedResourcesSptr.
	OrderReservedResourcesSptr == true  ==> reservation is ok
	OrderReservedResourcesSptr == false ==> reservation failed
	*/
	OrderReservedResourcesSptr reserveResourcesForOrder(const tlOrder& loadedOrder, bool& gotExceptions);

	ExecutingOrder* createExecOrder (const tlOrder& loadedOrder, const OrderReservedResourcesSptr& orderReservedResourcesSptr);
	bool checkPreconditions(ExecutingOrder *execOrder);
	void abortOrder(ExecutingOrder *execOrder);
	bool prepareOrder(ExecutingOrder *execOrder);
	bool processOrder(ExecutingOrder *execOrder);

	void stop(stopType stopMode=IMMEDIATE_STOP);
	stopType getStop () const;

	void notifySchedulerCompletion(const ExecutingOrder& execOrder, SchedulerExitStatus status) const;
	void notifySetupFailure (const ExecutingOrder& execOrder) const;

	ExecutingOrder* getExecutingOrderById (unsigned int);
	bool isProcessing();
	bool updateOrderStatus(const ExecutingOrder&, const tlOrderStatus, bool notifyOnError=true);
  
	// New operations to allow run-time initialization and configuration
	void reloadConfiguration ();
	
	// Operation to release the resources allocated by a given clientId
	// it will never throw
	void releaseClientResources (const std::string&) throw();

protected:

private: // methods
	OrderExecutionMgr& operator = (const OrderExecutionMgr& d); // not implemented
	OrderExecutionMgr (const OrderExecutionMgr& d); // not implemented
	OrderExecutionMgr (); // not implemented

	// Private operations
	bool hasAllowedTransitionTable () const;
	void init();

	std::string computeClientId (const tlOrder&) const;

private: // attributes
	tlDaemon& _parent;
	WSConfig _config;
	bool _verbose; // instructs the mgr to log extra messages about order execution
	// size_t _max; // max orders handled in parallel; default 1
	std::string _baseDir;
	OrderSlot _orders;
	CacheManager _cacheManager;

	// will help syncing orders completion notification to parent thread
	// and order removal inside the _orders array
	ThreadSafe _mutex; 

	ThreadSafe _pathMutex; // thread-safe implementation for basePath parameter

	// Attributes to manage the service resource server plugin 
	std::string _serviceResManagerKey;
	TLServiceResourceSrvBase* _serviceResManager;

	stopType _stopMode;

	ACS_CLASS_DECLARE_DEBUG_LEVEL (OrderExecutionMgr)
};

_ACS_END_NAMESPACE


#endif // _ORDER_EXECUTION_MGR_H
