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
	Revision 2.4  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.3  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.2  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.1  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.6  2003/06/06 10:08:21  marfav
	FileOrderQueueMgr.C
	
	Revision 1.5  2003/05/05 10:27:05  marfav
	got rid of extra ^M
	
	Revision 1.4  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.3  2003/02/04 16:45:18  marfav
	Added registerDownloadStart method
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _FILE_ORDER_QUEUE_MGR_H
#define _FILE_ORDER_QUEUE_MGR_H

#include <OrderQueueMgr.h>

#include <exException.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class FileOrderQueueMgr : public OrderQueueMgr // PRQA S 2109
{
  public:
    explicit FileOrderQueueMgr(tlDaemon& aDaemon);
    virtual ~FileOrderQueueMgr() throw() {} // PRQA S 2131

    virtual bool updateOrderStatus(const tlOrder& anOrder, tlOrderStatus status, bool notifyOnError=true);
    virtual bool registerProcessingStart(const tlOrder& anOrder);
    virtual bool registerProcessingStop(const tlOrder& anOrder, tlOrderStatus status);
    virtual bool registerDownloadStart (const tlOrder& anOrder);

  protected:
    virtual void init();
    virtual void retrieveOrders(std::vector<tlOrder>& orders);

  private: // methods
    static tlOrder line2order(std::string line);
    FileOrderQueueMgr() ; // not implemented
    FileOrderQueueMgr(const FileOrderQueueMgr &) ; // not implemented
    FileOrderQueueMgr& operator= (const FileOrderQueueMgr &) ; // not implemented
  private: // attributes
    std::string _path;
    ACS_CLASS_DECLARE_DEBUG_LEVEL (FileOrderQueueMgr)
};


_ACS_END_NAMESPACE

#endif // _FILE_ORDER_QUEUE_MGR_H
