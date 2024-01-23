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
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
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
	
	Revision 2.5  2013/03/11 12:09:04  marfav
	dbConnectionPool use enforced
	
	Revision 2.4  2013/03/08 18:33:28  marfav
	Using connection pool
	
	Revision 2.3  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.2  2011/01/24 16:21:06  marpas
	Mutex.h does not longer exixts
	
	Revision 2.1  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.26  2005/06/22 12:36:20  enrcar
	Added handling for namespace std
	
	Revision 1.25  2005/05/02 16:03:40  marfav
	preHandling is boolean now
	preHandling and postHandling can use a dbConnection to perform checks
	the connection is managed by OrderQueueMgr
	
	Revision 1.24  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.23  2004/10/19 08:17:07  marfav
	Support for orders satellite and mission fields added
	
	Revision 1.22  2004/09/21 08:16:04  marfav
	SigalrmEmulator moved in namespace acs
	
	Revision 1.21  2004/08/24 14:50:17  marfav
	Added mission support in Order2Order function
	
	Revision 1.20  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.19  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.18  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.17  2003/09/15 10:27:22  marfav
	postHandling method added
	
	Revision 1.16  2003/08/01 11:03:30  marfav
	Added support for thread Randez-Vous and InternalOrderQueue
	
	Revision 1.15  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time
	
	Revision 1.14  2003/07/10 14:18:32  marfav
	clone method added to exceptions
	
	Revision 1.13  2003/07/04 15:06:33  marfav
	Using Sigalarm Emulator and implementing restart
	
	Revision 1.12  2003/06/06 10:31:04  marfav
	Dead code removed
	
	Revision 1.10  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
	Revision 1.9  2003/05/13 08:43:15  marfav
	Align to new DbPersistentFactory constraints for DbCryosat objects
	
	Revision 1.8  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling
	
	Revision 1.7  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.6  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.5  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.4  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.3  2003/02/04 14:04:39  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _ORDER_QUEUE_MGR_H
#define _ORDER_QUEUE_MGR_H

#include <SigalrmEmulator.h>
#include <Thread.h>
#include <exException.h>
#include <dbOrder.h>

#include <tlOrder.h>
#include <StatusMgr.h>
#include <ThreadSafe.h>


#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class tlDaemon;


enum PriorityOrder {PriorityOrderAscending, PriorityOrderDescending, PriorityOrderUnknown};
PriorityOrder string2PriorityOrder (const std::string&);
std::string priorityOrder2String (PriorityOrder);


class OrderQueueMgr : public Thread // PRQA S 2109
{
public:


  /*! class OrderQueueMgrException declaration */
  exDECLARE_EXCEPTION(OrderQueueMgrException, exException) ; // PRQA S 2131, 2502

  enum OQM_Status {OQM_ERROR,OQM_ERROR1,OQM_ERROR2, OQM_WARNING, OQM_OK};

  explicit OrderQueueMgr(tlDaemon& aDaemon);
  virtual ~OrderQueueMgr() throw() ;

  virtual void run();
  void pause(); // PRQA S 2502
  void resume();
  void wakeUp();
  void startQueueMgr();
  void stopQueueMgr();
  void restartQueueMgr();
  bool isHandling();

  void setTimerFrequency(int period_in_sec);
  int getTimerFrequency();
  void setPriorityOrder(PriorityOrder o);
  PriorityOrder getPriorityOrder() const;
  void verbose(bool v);
  bool verbose() const;

  bool tryBookOrder(const tlOrder& anOrder);

  virtual OQM_Status getStatus () {return OQM_OK;} // PRQA S 2131
  virtual bool updateOrderStatus(const tlOrder& anOrder, tlOrderStatus status, bool notifyOnError=true);
  virtual bool registerProcessingStart(const tlOrder& anOrder);
  virtual bool registerProcessingStop(const tlOrder& anOrder, tlOrderStatus status);
  virtual bool registerDownloadStart (const tlOrder& anOrder);
  virtual void cleanupBlacklistItems() const {} // PRQA S 2131
  virtual void addBlackListItem (dbPersistent::IdType id) const {} // PRQA S 2131

protected: // methods
  virtual void init() {} // derived specific initialisation // PRQA S 2131
  virtual void cleanup() {} // derived specific initialisation // PRQA S 2131
  virtual void retrieveOrders(std::vector<tlOrder>& orders) = 0;
  virtual void restartOQM() {} // PRQA S 2131
  virtual void postHandling (tlOrder&) {} // PRQA S 2131
  virtual bool preHandling (tlOrder&) {return true;} // PRQA S 2131

  template <class X,class Y>
      void Order2Order (const X &  , Y & );

// Method atdded to fix attributes scope and set them private
  const tlDaemon& getParent () const;
  tlDaemon& getParent();
  int getMaxRetries () const;

protected:
  enum op_type {LOAD,UPDATE};

private: // methods
  void installHandlers();
  void _init();
  void _cleanup();
  void _restartOQM();

  bool _preHandling (tlOrder&);
  void _postHandling (tlOrder&);
  bool handleOrder (tlOrder&);

  std::string op_type2string (op_type);
  bool retry_catch (op_type, dbOrder&, bool notifyOnError, unsigned int orderId=0);
  void armTimer (const struct itimerval&);
  void restartTimer (bool reset=false);

  // prevent the copy of this threaded class
  OrderQueueMgr& operator = (const OrderQueueMgr& ); // not implemented
  OrderQueueMgr (const OrderQueueMgr& ); // not implemented
  OrderQueueMgr (); // not implemented

  static void sigUsr2Hdlr(int);
  static void sigAlrmHdlr(int);

private: // attributes
  tlDaemon& _parent;
  PriorityOrder _priorityOrder;
  int _timerFreq; // if 0, no timer
  int _maxRetries;

  bool _busy;
  bool _verbose; // instructs the queue mgr to log extra messages about order execution
                 // set by the Interactive Order Manager, which needs some messages that
                 // normally are debug
  bool _alrmHdlrInstalled;
  SigalrmEmulator _timerEmulator;
  bool _isHandling;
  ThreadSafe _mutex; // thread-safe implementation

private: // static stuff...
  static bool _gotSigAlrm;
  static bool _gotSigUsr2;

  ACS_CLASS_DECLARE_DEBUG_LEVEL (OrderQueueMgr)
};


// Implementation of template method
template <class X,class Y>
void OrderQueueMgr::Order2Order (const X & src  , Y & dst)
{
  dst.reset();
  dst.setOrderId(src.getOrderId());
  dst.setCreationDate(src.getCreationDate());
  dst.setPriority(src.getPriority());
  dst.setStatus(src.getStatus());
  dst.setProcessorName(src.getProcessorName());
  dst.setProcessorVersion(src.getProcessorVersion());
  dst.setProcessorConfiguration(src.getProcessorConfiguration());
  dst.setTroubleshooting(src.getTroubleshooting());
  dst.setStartTime(src.getStartTime());
  dst.setStopTime(src.getStopTime());
  dst.setJobResponsible(src.getJobResponsible());
  dst.setProductionStartTime(src.getProductionStartTime());
  dst.setProcessingTimeWindow(src.getProcessingTimeWindow());
  dst.setProcessorId(src.getProcessorId());
  dst.setOrderType(src.getOrderType());
  dst.setExternalOrderId(src.getExternalOrderId());
  
  bool isSet = false ;
  int parameter = src.getSatelliteId (&isSet);
  if (isSet) 
  {
	  dst.setSatelliteId (parameter);
  }
  parameter = src.getMission (&isSet);
  if (isSet) 
  {
	  dst.setMission (parameter);
  }
}

_ACS_END_NAMESPACE

#endif // _ORDER_QUEUE_MGR_H
