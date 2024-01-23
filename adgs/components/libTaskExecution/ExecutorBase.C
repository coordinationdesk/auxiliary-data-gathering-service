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

	$Prod: A.C.S. TaskTableExecution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.9  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.8  2016/05/10 14:44:12  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.7  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.6  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.5  2013/07/07 18:29:17  marpas
	applying coding best practices in progress
	some qa and compilation warnings removed
	
	Revision 2.4  2013/02/21 14:44:19  marfav
	Compiler warnings removed
	
	Revision 2.3  2012/02/20 15:09:57  marpas
	refactoring
	
	Revision 2.2  2012/02/13 17:06:34  marpas
	removing compiler warnings
	
	Revision 2.1  2008/04/22 11:13:21  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/


#include <ExecutorBase.h>
#include <ttexecV.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {
    ttexecV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ExecutorBase)

ExecutorBase::ExecutorBase() :
	_exitCode (0),
	_isRunning (false),
	_hasExitCode (false),
	_defaultPosixExitCode(true),
	_flagsStack(),
	_internalStatus (exeRUN),
	_eventQueue()

{
	// Fill the stack using default values
	Flags defFlags (false, 9, 0);
	pushFlags (defFlags);
}

ExecutorBase::~ExecutorBase() throw() 
{
	// Empty DTOR
}

ExecutorBase::ExecutorBase (const ExecutorBase& b) :
	ThreadSafe (*this),
	_exitCode (b._exitCode),
	_isRunning (b._isRunning),
	_hasExitCode (b._hasExitCode),
	_defaultPosixExitCode(b._defaultPosixExitCode),
	_flagsStack(b._flagsStack),
	_internalStatus (b._internalStatus),
	_eventQueue (b._eventQueue)
{
}

ExecutorBase& ExecutorBase::operator= (const ExecutorBase& b)
{
	if (this != &b) 
	{
		Lock lock1 (*this);
		Lock lock2 (b);

		_exitCode       = b._exitCode;
		_isRunning      = b._isRunning;
		_hasExitCode    = b._hasExitCode;
		_flagsStack     = b._flagsStack;
		_internalStatus = b._internalStatus;
		_eventQueue     = b._eventQueue;
		_defaultPosixExitCode = b._defaultPosixExitCode;
		ThreadSafe::operator= (b);
	}
	return *this;
}

ExecutorBase::ExecutorStatus ExecutorBase::getStatus() const
{
	Lock lock(*this);
	return _internalStatus;
}

void ExecutorBase::resetStatus ()
{
	Lock lock(*this);
	_internalStatus=exeRUN;
}

bool ExecutorBase::setStatus (ExecutorStatus s)
{
	Lock lock(*this);
	// The status transition map is :
	// from RUN to: FAILED, EXIT, ABORT
	// from FAILED to: EXIT, ABORT
	// from EXIT to: ABORT

	bool retval = false;
	
	switch (_internalStatus)
	{
	case exeRUN:
        {
		    // all status are allowed
		    _internalStatus = s;
		    retval = true;
		    break;
        }
	case exeFAILED:
        {
		    // EXIT or ABORT allowed
		    if ( (s == exeEXIT) || ( s == exeABORT) ) 
		    {
			    _internalStatus = s;
			    retval = true;
		    }
		    break;
        }
	case exeEXIT:
        {
		    // only abort is allowed
		    if (s == exeABORT) 
		    {
			    _internalStatus = s;
			    retval = true;
		    }
		    break;
        }
	default: // exeABORT
        {
		    // only ABORT on ABORT is allowed
		    if (s == exeABORT) 
		    {
			    _internalStatus = s;
			    retval = true;
		    }
		    break;
        }
	}

	return retval;
}

void ExecutorBase::pushFlags (const Flags& f)
{
	Lock lock(*this);
	_flagsStack.push (f);
}

ExecutorBase::Flags ExecutorBase::topFlags () const
{
	Lock lock(*this);
	if (_flagsStack.empty()) 
	{
		ACS_THROW (FlagsStackException("Empty flag stack. Top is not allowed")); // PRQA S 3081
	}
	return _flagsStack.top();
}

void ExecutorBase::popFlags ()
{
	Lock lock(*this);
	if (_flagsStack.empty()) 
	{
		ACS_THROW (FlagsStackException("Empty flag stack. Pop is not allowed")); // PRQA S 3081
	}
	_flagsStack.pop();
}

size_t ExecutorBase::flagsStackSize() const
{
	Lock lock(*this);
	return _flagsStack.size();
}


int ExecutorBase::getExitCode() const
{
	Lock lock(*this);
	return _exitCode;
}

bool ExecutorBase::isRunning() const
{
	Lock lock(*this);
	return _isRunning;
}

bool ExecutorBase::hasExitCode() const
{
	Lock lock(*this);
	return _hasExitCode;
}

void ExecutorBase::setExitCode (int theCode)
{
	Lock lock(*this);
	_exitCode = theCode;
}

void ExecutorBase::setIsRunning(bool b)
{
	Lock lock(*this);
	_isRunning = b;
}

void ExecutorBase::setHasExitCode(bool b)
{
	Lock lock(*this);
	_hasExitCode = b;
}
    
bool ExecutorBase::getDetached() const
{
	Lock lock(*this);
	return topFlags().detached;
}

void ExecutorBase::setDetached(bool b)
{
	Lock lock(*this);
	
	Flags f = topFlags();
	popFlags();
	
	f.detached = b;
	pushFlags(f);
}

int ExecutorBase::getDefKillSignal() const
{
	Lock lock(*this);
	return topFlags().killSignal;
}

void ExecutorBase::setDefKillSignal(int s)
{
	Lock lock(*this);
	
	Flags f = topFlags();
	popFlags();

	f.killSignal = s;
	pushFlags(f);
}
	
int ExecutorBase::getDefQuitSignal() const
{
	Lock lock(*this);
	return topFlags().quitSignal;
}

void ExecutorBase::setDefQuitSignal(int s)
{
	Lock lock(*this);
	Flags f = topFlags();
	popFlags();

	f.quitSignal = s;
	pushFlags(f);
}

void 
ExecutorBase::setDefaultPosixExitCode (bool p)
{
	Lock lock(*this);
	_defaultPosixExitCode = p;
}

bool 
ExecutorBase::isDefaultPosixExitCode () const
{
	Lock lock(*this);
	return 	_defaultPosixExitCode;
}


void ExecutorBase::insertEventInQueue (const std::pair < ExecutorEventType, int> &theEvent)
{
	Lock lock (*this);
	
		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inserting element (" 
			   << eventTypeToString (theEvent.first)<< ", " << theEvent.second << ")") ;
	_eventQueue.push (theEvent);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "queue size is (" << _eventQueue.size() << ")") ;
	
}

bool ExecutorBase::haveEventsInQueue () const
{
	Lock lock (*this);
	bool retval = ( !_eventQueue.empty() );

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "queue size is (" << _eventQueue.size() << ") - " 
		       << "Returning : " << boolalpha << retval) ;

	return retval;		
}

bool 
ExecutorBase::getEventFromQueue (pair < ExecutorEventType, int>& theEvent)
{
	Lock lock (*this);
	bool haveElements = ( !_eventQueue.empty() );

	if (haveElements) 
	{
		theEvent = _eventQueue.front();
		_eventQueue.pop();

		
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning element (" 
				   << eventTypeToString (theEvent.first) << ", " << theEvent.second << ")") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "new queue size is (" << _eventQueue.size() << ")") ;

	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "queue is EMPTY!") ;
	}

	return haveElements;		
}

// Static Method
string 
ExecutorBase::eventTypeToString (ExecutorEventType event)
{
	string theEventType;
	switch (event) 
	{
	case eeAbort:
        {
		    theEventType="Abort";
		    break;
        }
	case eeAbortLevel:
        {
		    theEventType="AbortLevel";
		    break;
        }
	case eeQuit:
        {
		    theEventType="Quit";
		    break;
        }
	case eeKill:
        {
		    theEventType="Kill";
		    break;
        }
	case eeKillGroup:
        {
		    theEventType="KillGroup";
		    break;
        }
	case eeArmageddon:
        {
		    theEventType="Armageddon";
		    break;
        }
	default:
        {
		    theEventType="UNKNOWN";
            break ;
        }
	}
	return theEventType;
	
}


void 
ExecutorBase::abort()
{
	// Insert an abort event in the queue
	// if the status update is succesfull
	if (setStatus (exeABORT)) {
		insertEventInQueue (make_pair (eeAbort, 0));
    }
}
	
void 
ExecutorBase::abortLevel(int theLevel)
{
	// Insert an abortLevel event in the queue
	// if the status update is succesfull
	if (setStatus (exeFAILED)) {
		insertEventInQueue (make_pair (eeAbortLevel, theLevel));
    }
}

void 
ExecutorBase::quit()
{
	// Insert a quit event in the queue
	// if the status update is succesfull
	if (setStatus (exeEXIT)) {
		insertEventInQueue (make_pair (eeQuit, 0));
    }
}

void 
ExecutorBase::signal(int signal)
{
	// Insert a kill event in the queue
	insertEventInQueue (make_pair (eeKill, signal));
}

void 
ExecutorBase::signalGroup (int signal)
{
	// Insert a killGroup event in the queue
	insertEventInQueue (make_pair (eeKillGroup, signal));
}

void
ExecutorBase::armageddon (int signal)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called [" << signal << "]") ;

	setStatus (exeABORT);

	// armageddon is not maskable in any way
	insertEventInQueue (make_pair (eeArmageddon, signal));
	// force asynchronous events management
	manageEvents();
}


_ACS_END_NAMESPACE

