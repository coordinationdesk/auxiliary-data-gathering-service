/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Default Log Plugin library $

	$Id$

	$Author$

	$Log$
	Revision 1.11  2014/05/09 12:32:58  marfav
	S2PDGS-784 Using named filterables to avoid mix of
	messages coming from different threaded schedulers
	
	Revision 1.10  2014/05/09 09:33:31  marfav
	Thread safety improved protecting the log stream before creation and after deletion
	
	Revision 1.9  2013/12/16 19:13:18  marpas
	no exceptions from dtors
	
	Revision 1.8  2013/10/18 11:19:52  marpas
	adopting libException 5.16 interfaces
	
	Revision 1.7  2013/09/27 14:15:06  marpas
	streams have name
	
	Revision 1.6  2009/06/26 10:48:38  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.5  2004/12/23 18:32:17  marfav
	Added PROCESSORLOG tag to MCF messages
	
	Revision 1.4  2004/12/03 17:54:43  marfav
	Header fixed
	
	Revision 1.3  2004/10/13 13:21:36  marfav
	Declaration mispell fixed
	
	Revision 1.2  2004/09/23 15:22:18  marfav
	Log handlers derived from ScLogHandlerBase
	
	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
	Import libScDefaultLogGroup
	

*/



#include <ScDefaultLogHandlerGroup.h>

#include <ScLogManager.h>
#include <ScOutFilterable.h>
#include <ScErrFilterable.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;



SchedulerLogFileHandler::SchedulerLogFileHandler (const scOrder& theWorkOrder) :
	ScLogHandlerBase("SchedulerLogFileHandler"),
	_log(0),
	_theMsgSender(),
	_orderId(),
	_logOrderSignature (theWorkOrder.getLogFileName())

{
	// If no scheduler filename is set
	// a default log name will be used
	string logname = theWorkOrder.getLogFileName();
	if (logname.empty())
	{
		logname = "./ProcessingLOG";
	}
	exostream *parent = 0 ;
	_log = new exLogStream ("p_log", logname, parent);

#warning OrderId derived from workingDir path!! Update WorkOrder adding OrderId informations!!
	string path = theWorkOrder.getBaseDir();
	string res ;
	string::size_type slash = path.rfind('/') ;
	if (slash != string::npos)
	{
		res.append(path, slash+1,path.length()-slash );
	}
	else res=path;
	_orderId = res;
}

SchedulerLogFileHandler::~SchedulerLogFileHandler () throw()
{
	ThreadSafe::Lock lock (globalMutex());
	destroying();
	// Critical section here
	if (0 != _log)
	{
		delete _log;
		_log=0;
	}
}

bool 
SchedulerLogFileHandler::computeTaskCompletionPercentage (exFilterable&, float&)
{
	// Never try to compute percentage completion value
	return false;
}



void
SchedulerLogFileHandler::userFilterableNotification(exFilterable &f)
{
	// The scheduler log file will trap every message coming from
	// standard err and standard out
	// It will ignore TRK messages that will be logged in the TrkFile (if any)
	// and in the Application log file
	//
	// The logFileName set in the work order is used as unique id
	// to discriminate log messages to be forwared in this log
	
	// Avoid notification on dead log streams
	ThreadSafe::Lock lock (globalMutex());
	if (!isDestroying() && (0 != _log))
	{
		ScOutFilterable* outmsg = dynamic_cast<ScOutFilterable*> (&f);
		if (outmsg != NULL)
		{
			if ( (outmsg->getOrderSignature() == "") || (outmsg->getOrderSignature() == _logOrderSignature) )
			{
				*_log << *outmsg;
			}
		}
		ScErrFilterable* errmsg = dynamic_cast<ScErrFilterable*> (&f);
		if (errmsg != NULL)
		{
			if ( (errmsg->getOrderSignature() == "") || (errmsg->getOrderSignature() == _logOrderSignature) )
			{
				*_log << *errmsg;
			}
			// Send error messages to patrol
			ostringstream message;
			message << "PROCESSORLOG:" << _orderId << "#" << errmsg->text();
			_theMsgSender.sendMessage( message.str() );
		}
	}
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(ScDefaultLogHandlerGroup);

ScDefaultLogHandlerGroup::ScDefaultLogHandlerGroup (const scOrder& theWorkOrder) :
    _scLogHandler (theWorkOrder)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


ScDefaultLogHandlerGroup::~ScDefaultLogHandlerGroup() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}

_ACS_END_NAMESPACE

