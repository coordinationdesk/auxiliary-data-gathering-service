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

	$Prod: A.C.S. ThinLayer Scheduler Library $

	$Id$

	$Author$

	$Log$
	Revision 2.10  2015/04/30 13:29:52  marfav
	MP warnings removed
	
	Revision 2.9  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.8  2015/01/23 11:49:16  marfav
	S2PDGS-1054 exit status thread safety management improved
	
	Revision 2.7  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.6  2013/10/18 11:33:29  marpas
	coding best practices applying
	
	Revision 2.5  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.4  2013/04/03 13:51:22  marfav
	Rolling back to previous thread::run()
	
	Revision 2.3  2013/04/03 12:54:16  marpas
	Thread::run gets a void*
	
	Revision 2.2  2013/02/26 09:00:50  marfav
	Compiler warnings removed
	
	Revision 2.1  2009/06/26 10:48:57  marpas
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

#ifndef _THREADED_SCHEDULER_H_
#define _THREADED_SCHEDULER_H_

#include <SchedulerIF.h>
#include <SchedulerEngine.h>

#include <Thread.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

// This class wraps a SchedulerEngine and allow to control it
// and to retrieve its exit status

class ThreadedScheduler : private Thread, public SchedulerIF // PRQA S 2109, 2193
{

public:
	/*! class ThreadedSchedulerException declaration */
	exDECLARE_EXCEPTION(ThreadedSchedulerException,SchedulerIFException) ; // PRQA S 2131, 2153, 2502

public:
	ThreadedScheduler ();
	virtual ~ThreadedScheduler() throw() ;

	// Operations referred to running tasks
	virtual void stop();
	virtual void abort(); // PRQA S 2502
	virtual void suspend();
	virtual void resume();
	virtual void abortBySigTerm();
	virtual void doKill ();
	virtual void sendCmd (const std::string &cmdValue, const std::string &paramValue);

	// Synchronous join on exit
	virtual bool timedWaitExit (unsigned long);

	// Operations referred to the Scheduler
	virtual void execute (const scOrder& workOrder);
	virtual bool getExitStatus(SchedulerExitStatus&);
	virtual bool isSchedulerRunning();
#ifdef SCHEDULER_CHECKS_DISK_SPACE
	virtual size_t getReservedDiskSpaceQuota() const { return _reservedDiskQuota; } // PRQA S 2131 2
	virtual void setReservedDiskSpaceQuota(size_t MB) { _reservedDiskQuota = MB; }
#endif // SCHEDULER_CHECKS_DISK_SPACE

protected: //methods
	virtual void run();

private:
	ThreadedScheduler (const ThreadedScheduler&); // not implemented
	ThreadedScheduler &operator = (const ThreadedScheduler&); // not implemented

	// Just to avoid a warning saying that the stop(bool) was hidden
	virtual void stop (bool) { this->stop(); } // PRQA S 2131

    // Internal helpers to improve thread safety of the _exitStatus
    void internalSetExitStatus (const SchedulerExitStatus&);
    SchedulerExitStatus internalGetExitStatus() const;


private: //methods
	void checkForRunning();

private: //attribute
	SchedulerEngine*_engine;
	SchedulerExitStatus _exitStatus;
	size_t _reservedDiskQuota;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ThreadedScheduler) 
};


_ACS_END_NAMESPACE



#endif //_THREADED_SCHEDULER_H_
