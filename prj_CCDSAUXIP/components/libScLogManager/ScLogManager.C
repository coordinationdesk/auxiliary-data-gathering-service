// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Log Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.7  2015/08/28 10:16:13  matteo.airoldi
	Message notification on syslog can be switched off
	
	Revision 5.6  2015/08/12 15:58:48  marfav
	APF-271 work in progress
	
	Revision 5.5  2015/08/06 13:51:30  marfav
	Messages from tasks encapsulated in INFO filterables and notified on the default log stream
	
	Revision 5.4  2014/07/16 09:04:56  nicvac
	Global mutex use and destroying logic enforced
	
	Revision 5.3  2014/01/31 15:36:33  marfav
	S2PDGS-603 Processor messages can be filtered by configuration
	
	Revision 5.2  2013/09/27 15:27:30  marpas
	streams have name
	
	Revision 5.1  2013/09/26 19:34:36  marpas
	adopting libException Rev_5_16 interfaces
	test improved
	still doubles messages
	
	Revision 5.0  2013/07/07 18:27:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.4  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	
	Revision 1.1.1.1  2004/09/14 16:57:30  marfav
	Import libScLogManager
	


*/

#include <ScLogManager.h>
#include <ScOutFilterable.h>
#include <ScErrFilterable.h>
#include <TrkFilterable.h>

#include <MCFilterables.h>

#include <ScLogHandlerGroup.h>

#include <sclmV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc;

namespace {
 sclmV version;
 ScLogHandlerGroup myGroup;
}


ScLogManager sclog;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ScLogManager) 

ScLogManager::ScLogManager() :
	exostream ("ScLogManager",NullStream),
	_defaultHandler()
{
	// do not allow notification on standard streams
	this->silent();
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

ScLogManager::~ScLogManager() throw() 
{
}

void ScLogManager::notifyStdStreams (bool b)
{
    _defaultHandler.notifyStdStreams (b);
}

bool ScLogManager::notifyStdStreams() const
{
    return _defaultHandler.notifyStdStreams ();
}

void ScLogManager::notifyOnSyslog (bool b)
{
    _defaultHandler.notifyOnSyslog (b);
}

bool ScLogManager::notifyOnSyslog() const
{
    return _defaultHandler.notifyOnSyslog ();
}

void ScLogManager::setSyslogTaskMessageLimit (size_t n)
{
	_defaultHandler.setSyslogTaskMessageLimit (n);
}

size_t ScLogManager::getSyslogTaskMessageLimit () const
{
	return _defaultHandler.getSyslogTaskMessageLimit ();
}

size_t ScLogManager::getSyslogSkippedMessageCount() const
{
	return _defaultHandler.getSyslogSkippedMessageCount ();
}

size_t ScLogManager::getSyslogSkippedMessageCount(const string& t) const
{
	return _defaultHandler.getSyslogSkippedMessageCount (t);
}


// Default handler implementation
// it simple reply all messages to the default log stream
// and forwards messages to the syslog for M&C purposes
// notification on ex std streams can be configured

ACS_CLASS_DEFINE_DEBUG_LEVEL(ScDefaultHandler) 

ScDefaultHandler::ScDefaultHandler() :
	exHandler ("ScDefaultHandler", &sclog),
    _notifyStdStreams (true),
	_notifyOnSyslog (true),
	_issuedMessages(),
	_skippedMessages(),
	_overallSkippedMessages(0),
	_messageLimit(100)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

ScDefaultHandler::~ScDefaultHandler() throw()
{
    Lock lock (globalMutex());
    destroying();
}

void ScDefaultHandler::notifyStdStreams (bool b)
{
    Lock lock(globalMutex());
    _notifyStdStreams = b;
}

bool ScDefaultHandler::notifyStdStreams() const
{
    Lock lock(globalMutex());
    return _notifyStdStreams;
}

void ScDefaultHandler::notifyOnSyslog (bool b)
{
	Lock lock(globalMutex());
	_notifyOnSyslog = b;
}

bool ScDefaultHandler::notifyOnSyslog () const
{    
	Lock lock(globalMutex());
	return _notifyOnSyslog;
}

void ScDefaultHandler::setSyslogTaskMessageLimit (size_t n)
{
	Lock lock(globalMutex());
	_messageLimit = n;
}

size_t ScDefaultHandler::getSyslogTaskMessageLimit () const
{
	Lock lock(globalMutex());
	return _messageLimit;
}

size_t ScDefaultHandler::getSyslogSkippedMessageCount() const
{
	Lock lock(globalMutex());
	return _overallSkippedMessages;
}

size_t ScDefaultHandler::getSyslogSkippedMessageCount(const string& t) const
{
	Lock lock(globalMutex());

	size_t retval (0);
	map<string,size_t>::const_iterator it = _skippedMessages.find(t);
	if (it != _skippedMessages.end())
	{
		retval = it->second;
	}

	return retval;
}

void ScDefaultHandler::notifyFilterable (exFilterable &f)
{
    Lock lock (globalMutex());
    if (notifyStdStreams() && !isDestroying())
    {

		ScFilterable* theMsg (dynamic_cast<ScFilterable*> (&f));

		// All the messages coming from the processors are placed in the main log
		if ( theMsg != 0)
		{

			acs::exostream::getDefaultOutLogger() << f.message() << "\n";

			// Check if need to notify it also on syslog
			if (notifyOnSyslog())
			{
				// but avoid trk messages
				if (dynamic_cast<TrkFilterable*> (&f) == 0)
				{
					// And apply decimation rules if any 
					// messageLimit set to zero means no limit at all
					if  ( (0 == _messageLimit) || (_issuedMessages[theMsg->getOriginatorTaskName()] <= _messageLimit) )
					{
						// APF-271
						ACS_LOG_MC_INFO ( mc_param("message", f.message() ) << mc_metric (Metric_LOG) );
						++_issuedMessages[theMsg->getOriginatorTaskName()];
					}
					else
					{
						// message quota exceeded
						++_skippedMessages[theMsg->getOriginatorTaskName()];
						++_overallSkippedMessages;
					}
				}
			}

		}
    }   
}


_ACS_END_NAMESPACE
