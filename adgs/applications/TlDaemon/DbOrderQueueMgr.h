// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SPA - DADF
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
	Revision 2.11  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.10  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.9  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.8  2015/08/10 09:29:47  marfav
	PDSEV-8: checking schema before enabling blacklist management
	
	Revision 2.7  2015/08/07 14:40:18  marfav
	PDSEV-8 interaction with the db queue optimized and blacklist table support added
	
	Revision 2.6  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.5  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.4  2013/03/08 18:33:28  marfav
	Using connection pool
	
	Revision 2.3  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.2  2009/06/26 10:31:53  marpas
	no message from some class dtors
	
	Revision 2.1  2006/06/12 11:01:25  marfav
	Code for NO_SLOTS support removed
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/05/02 16:04:41  marfav
	preHandling takes care of checking the orders in the black list
	before starting the precondition phase
	
	Revision 1.11  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.10  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.9  2003/08/01 11:03:30  marfav
	Added support for thread Randez-Vous and InternalOrderQueue
	
	Revision 1.8  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
	Revision 1.7  2003/05/13 08:43:15  marfav
	Align to new DbPersistentFactory constraints for DbCryosat objects
	
	Revision 1.6  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.5  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.4  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.3  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _DB_ORDER_QUEUE_MGR_H
#define _DB_ORDER_QUEUE_MGR_H


#include <OrderQueueMgr.h>
#include <tlOrder.h>
#include <MQTTDbBroadcastListenerBase.h>

#include <dbOrder.h>
#include <exException.h>

#include <dbConnectionPool.h>
#include <signal.h>

#include <string>
#include <vector>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class tlDaemon;

class TlBroadcastListener : public MQTTDbBroadcastListenerBase
{
	public:
		TlBroadcastListener(dbConnectionPool& cp) ;
		virtual ~TlBroadcastListener() ACS_NOEXCEPT {}

#ifdef ENABLE_TEST
	public:
#else
	protected:
#endif
		virtual bool filterMessage(const MQTTMessageHandler & m);
		virtual void init() ;

	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL (TlBroadcastListener)
};


class DbOrderQueueMgr : public OrderQueueMgr // PRQA S 2109
{
  public:

    explicit DbOrderQueueMgr(tlDaemon& aDaemon);
    virtual ~DbOrderQueueMgr() throw() ;

    virtual OQM_Status getStatus();

    virtual void cleanupBlacklistItems() const;
    virtual void addBlackListItem (dbPersistent::IdType id) const;


  protected:
    virtual void init();
    virtual void retrieveOrders(std::vector<tlOrder>& orders);
    virtual void cleanup();
    virtual void restartOQM();
    virtual bool preHandling (tlOrder& o);

  private:
    bool schemaHasBlacklistTable() const;
    void putOrdersFromStatusToError (const std::string& status) const;


  private: // Dispatcher Class
	class DbOQM_Dispatcher // PRQA S 2109
	{
	public:
		DbOQM_Dispatcher (const TlBroadcastListener* theListener, const DbOrderQueueMgr* theParent) :
		    _listener (theListener),
		    _status (OQM_OK),
		    _parent (theParent)
		{}
        ~DbOQM_Dispatcher() throw() {} ;
		DbOQM_Dispatcher (const DbOQM_Dispatcher&);
		DbOQM_Dispatcher& operator = (const DbOQM_Dispatcher&);
		void operator () (const OQM_Status&, unsigned int slot = 0);

	private:
		DbOQM_Dispatcher () ; // not implemented
	private:
		const TlBroadcastListener* _listener;
		OQM_Status _status;
		const DbOrderQueueMgr* _parent;
	};

  private:
      DbOrderQueueMgr (); // not implemented
      DbOrderQueueMgr (const DbOrderQueueMgr&); // not implemented
      DbOrderQueueMgr& operator = (const DbOrderQueueMgr&); // not implemented


  private:
    TlBroadcastListener* _myListener;
    StatusMgr <OQM_Status , DbOQM_Dispatcher>* _statusMgr;
    
    ACS_CLASS_DECLARE_DEBUG_LEVEL (DbOrderQueueMgr)
};

_ACS_END_NAMESPACE

#endif // _DB_ORDER_QUEUE_MGR_H
