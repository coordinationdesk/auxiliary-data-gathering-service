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
	Revision 2.4  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.3  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.2  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced
	
	Revision 2.1  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/06/22 12:36:20  enrcar
	Added handling for namespace std
	
	Revision 1.10  2005/05/02 16:22:03  marfav
	aligned to new preHandling and postHandling methods using dbConnections
	
	Revision 1.9  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.8  2003/09/15 10:59:25  marfav
	Support for MMI integration added
	
	Revision 1.7  2003/05/14 14:57:51  marfav
	Using auto_ptr and common code moved to OrderQueueMgr
	
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


#ifndef _INTERACTIVE_DB_ORDER_QUEUE_MGR_H
#define _INTERACTIVE_DB_ORDER_QUEUE_MGR_H


#include <OrderQueueMgr.h>
#include <tlOrder.h>

#include <exException.h>
#include <dbOrder.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class tlDaemon;

class InteractiveDbOrderQueueMgr : public OrderQueueMgr // PRQA S 2109
{
public:
  explicit InteractiveDbOrderQueueMgr(tlDaemon& aDaemon);
  virtual ~InteractiveDbOrderQueueMgr() throw() ;

  virtual OQM_Status getStatus();

protected:
  // No cleanup() method needed by this object....
  virtual void init();
  virtual void retrieveOrders(std::vector<tlOrder>& orders);
  virtual void postHandling (tlOrder&);

private:

  InteractiveDbOrderQueueMgr (); // not implemented
  InteractiveDbOrderQueueMgr (const InteractiveDbOrderQueueMgr& d); // not implemented
  InteractiveDbOrderQueueMgr& operator = (const InteractiveDbOrderQueueMgr& d); // not implemented
  void prepareOrdersDir();
  bool ordersDirExists();

private:
  std::string _path;

  ACS_CLASS_DECLARE_DEBUG_LEVEL (InteractiveDbOrderQueueMgr)

};


_ACS_END_NAMESPACE


#endif // _INTERACTIVE_DB_ORDER_QUEUE_MGR_H
