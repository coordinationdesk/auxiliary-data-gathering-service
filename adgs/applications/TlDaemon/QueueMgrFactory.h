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
	
	Revision 2.2  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.7  2003/05/21 15:02:39  marfav
	Removed some dead code
	
	Revision 1.6  2003/04/18 16:13:58  fracar
	align to new compiler version
	
	Revision 1.5  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.4  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.3  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _QUEUE_MGR_FACTORY_H
#define _QUEUE_MGR_FACTORY_H

#include <acs_c++config.hpp>
#include <string>

#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


class tlDaemon;
class OrderQueueMgr;


class QueueMgrFactory : public pattern::Singleton<QueueMgrFactory> // PRQA S 2109, 2153
{
  friend QueueMgrFactory* pattern::Singleton<QueueMgrFactory>::instance(); // PRQA S 2107

  public:
    OrderQueueMgr* getQueueMgr(const std::string& type, tlDaemon& aDaemon) const;
    ~QueueMgrFactory() throw() ;

  protected:
    QueueMgrFactory();

  private:
    // Disabled because of the singleton implementation
    QueueMgrFactory& operator = (const QueueMgrFactory& d);
    QueueMgrFactory (const QueueMgrFactory& d);
};


_ACS_END_NAMESPACE

#endif // _QUEUE_MGR_FACTORY_H
