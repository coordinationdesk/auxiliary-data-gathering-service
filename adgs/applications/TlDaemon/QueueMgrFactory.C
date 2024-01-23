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
	
	Revision 2.4  2015/11/20 11:10:55  marpas
	coding best practices application in progress
	
	Revision 2.3  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.2  2015/05/13 11:06:23  damdec
	Compilation warning fixed.
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.10  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.9  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.8  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.7  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError
	
	Revision 1.6  2003/05/21 15:02:39  marfav
	Removed some dead code
	
	Revision 1.5  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.4  2003/03/27 15:33:22  marfav
	Added plugin support and some logging improvements
	
	Revision 1.3  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/

#include <QueueMgrFactory.h>

#include <FileOrderQueueMgr.h>
#include <DbOrderQueueMgr.h>
#include <Filterables.h>
#include <exStream.h>

#include <InteractiveDbOrderQueueMgr.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

OrderQueueMgr* QueueMgrFactory::getQueueMgr(const string& type, tlDaemon& aDaemon) const // PRQA S 4020
{
    if (type == "FILE")
	    { return new FileOrderQueueMgr(aDaemon); }
    if (type == "DB")
	    { return new DbOrderQueueMgr(aDaemon); }
    if (type == "INTERACTIVE")
	    { return new InteractiveDbOrderQueueMgr(aDaemon); }
    else
    {
	    ACS_LOG_WARNING("QueueMgrFactory: unknown order queue manager type " << type << "; using DB");

	    return new DbOrderQueueMgr(aDaemon);
    }
}

QueueMgrFactory::QueueMgrFactory() {}
QueueMgrFactory::~QueueMgrFactory() throw() {}


_ACS_END_NAMESPACE
