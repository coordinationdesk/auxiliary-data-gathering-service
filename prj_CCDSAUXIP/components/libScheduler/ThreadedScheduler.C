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

	$Prod: A.C.S. ThinLayer Scheduler Library $

	$Id$

	$Author$

	$Log$
	Revision 2.13  2016/05/10 20:25:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.12  2016/04/14 09:57:00  marpas
	Thread does not inheriths any longer from ThreadSafe
	
	Revision 2.11  2015/12/22 12:37:08  marpas
	qa warnings fixed
	
	Revision 2.10  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.9  2015/01/23 11:49:16  marfav
	S2PDGS-1054 exit status thread safety management improved
	
	Revision 2.8  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.7  2013/10/18 11:33:29  marpas
	coding best practices applying
	
	Revision 2.6  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.5  2013/07/07 18:33:02  marpas
	application of coding best practices in progress
	some qa warnings and compilation ones removed
	adoption of libException 5.x standards in progress
	
	Revision 2.4  2013/04/03 13:51:22  marfav
	Rolling back to previous thread::run()
	
	Revision 2.3  2013/04/03 12:54:16  marpas
	Thread::run gets a void*
	
	Revision 2.2  2012/02/13 17:03:43  marpas
	removing compiler warnings
	
	Revision 2.1  2009/06/26 10:48:56  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.4  2004/11/19 14:58:21  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.3  2004/04/09 10:11:03  marfav
	Disk quota reservation support added
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/

#include <ThreadedScheduler.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ThreadedScheduler)




ThreadedScheduler::ThreadedScheduler () :
    Thread(),
    SchedulerIF(),
    _engine (0),
#ifdef SCHEDULER_CHECKS_DISK_SPACE
	_exitStatus (ssNOTSTARTED),
	_reservedDiskQuota(NoReservedQuota)
#else // SCHEDULER_CHECKS_DISK_SPACE
	_exitStatus (ssNOTSTARTED)
#endif // SCHEDULER_CHECKS_DISK_SPACE
{
}


ThreadedScheduler::~ThreadedScheduler () throw()
{
	try {
		Thread::stop(); // PRQA S 4631
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
	}
    delete _engine;
}

bool ThreadedScheduler::isSchedulerRunning()
{
	return Thread::running();
}

void ThreadedScheduler::stop()
{
	checkForRunning();
	_engine -> stop();
}

void ThreadedScheduler::abort()
{
	checkForRunning();
	_engine -> abort();
}

void ThreadedScheduler::abortBySigTerm()
{
	checkForRunning();
	_engine -> abortBySigTerm();
}


void ThreadedScheduler::suspend()
{
	checkForRunning();
	_engine -> suspend();
}

void ThreadedScheduler::resume()
{
	checkForRunning();
	_engine -> resume();
}

void ThreadedScheduler::doKill ()
{
	checkForRunning();
	_engine -> kill ();
}

void ThreadedScheduler::sendCmd (const string &cmdValue, const string & paramValue)
{
	checkForRunning();
	_engine -> sendCmd (cmdValue, paramValue);
}

bool ThreadedScheduler::getExitStatus(SchedulerExitStatus& code) // PRQA S 4020
{
	// _exitStatus can be
	// - ssNOTSTARTED => the scheduler never started
	// - ssUNDEFINED => the scheduler was started but didn't terminated its execution or exited unexpectedly
	// - the real exit code of the scheduler engine
	code = internalGetExitStatus();
	if (code == ssNOTSTARTED) {
		return false; // not a valid exit status
    }
	else if (isSchedulerRunning()) {
		return false; // execution not yet completed
    }
	else { 
        // pay attention... at this point the _exitStatus could not be the same as before
        // in the case the execution terminated in the meantime
        // when this is not running the exit status is updated and stable
        code = internalGetExitStatus();        
        return true; }// the value of "code" is meaningful
}


void ThreadedScheduler::checkForRunning()
{
	if (!isSchedulerRunning())
	{
		ACS_THROW (ThreadedSchedulerException("Cannot perform operation. Scheduler is not running.")); // PRQA S 3081
	}
}

void ThreadedScheduler::execute (const scOrder& workOrder)
{
	if (internalGetExitStatus() != ssNOTSTARTED)
	{
		ACS_THROW (ThreadedSchedulerException("Cannot execute. The Scheduler has been already started."));  // PRQA S 3081
	}
	// here have to create the SchedulerEngine in order to
	// allow the "run" to operate on it
	_engine = new SchedulerEngine(workOrder);

#ifdef SCHEDULER_CHECKS_DISK_SPACE
	size_t quota = getReservedDiskSpaceQuota();
	if (quota == NoReservedQuota) {
	    _engine->setReservedDiskSpaceQuota(SchedulerEngine::NoReservedQuota);
    }
	else {
	    _engine->setReservedDiskSpaceQuota(quota);
    }
#endif // SCHEDULER_CHECKS_DISK_SPACE


	// First set the exit status to ssUNDEFINED.
	// Using ssUNDEFINED allows to know that the scheduler has been started
	internalSetExitStatus (ssUNDEFINED);

	Thread::start();
}

// The thread runs here!
// This is "protected" so nobody can activate the scheduler out of control.
void ThreadedScheduler::run()
{
	// If the thread is not running and the exitStatus is still set to ssUNDEFINED
	// means that the Activate method has exited unexpectedly (i.e. due to uncaught exceptions)
    SchedulerExitStatus engineStatus = _engine->Activate();
	internalSetExitStatus (engineStatus);
	// At this point the exit status is valid and can be returned by the getExitStatus method
}

bool ThreadedScheduler::timedWaitExit (unsigned long timeout)
{
	return Thread::waitExitOrTimeout (timeout);
}


    // Internal helpers to improve thread safety of the _exitStatus
void ThreadedScheduler::internalSetExitStatus (const SchedulerExitStatus& s)
{
    Lock lock(mutex()) ;
    _exitStatus = s;
}

SchedulerExitStatus ThreadedScheduler::internalGetExitStatus() const
{
    Lock lock(mutex()) ;
    return _exitStatus;
}


_ACS_END_NAMESPACE
