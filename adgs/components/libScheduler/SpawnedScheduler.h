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
	Revision 2.6  2015/05/13 14:57:23  marfav
	abortBySigterm is now part of the standard interfaces
	
	Revision 2.5  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.4  2013/12/17 12:10:07  marpas
	qa warning
	
	Revision 2.3  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.2  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.1  2012/02/01 15:59:50  matteo.airoldi
	destructor compilation error fixed
	
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

#ifndef _SPAWNEDSCHEDULER_H_
#define _SPAWNEDSCHEDULER_H_

#include <SchedulerIF.h>
#include <SchedulerEngine.h>

#include <SpawnProc.h>
#include <exException.h>
#include <ScIpcClient.h>


_ACS_BEGIN_NAMESPACE(acs)

class SpawnedScheduler : private SpawnProc, public SchedulerIF // PRQA S 2109, 2153, 2193
{

public:
	/*! class SpawnedSchedulerException declaration */
	exDECLARE_EXCEPTION(SpawnedSchedulerException,SchedulerIFException) ; // PRQA S 2131, 2153, 2502

public:
	SpawnedScheduler (const std::string &thePath, const std::string & woPath);
	virtual ~SpawnedScheduler() throw () ;

	// Commands referred to running tasks
	virtual void stop();
	virtual void abort(); // PRQA S 2502
	virtual void suspend();
	virtual void resume();
	virtual void doKill ();
	virtual void sendCmd (const std::string &cmdType, const std::string &cmdValue);
	virtual void abortBySigTerm();

	// Commands referred to the Scheduler
	virtual void execute (const scOrder& workOrder);
	virtual bool getExitStatus(SchedulerExitStatus&);
	virtual bool isSchedulerRunning();
	int sendKillSignal(int signal=9); // PRQA S 4400

	// Synchronous join on exit
	virtual bool timedWaitExit (unsigned long);

    // Get the PID of the spawned scheduler
    pid_t getSchedulerPid() const ;

#ifdef SCHEDULER_CHECKS_DISK_SPACE
	virtual size_t getReservedDiskSpaceQuota() const { return NoReservedQuota;} // PRQA S 2131 2
	virtual void setReservedDiskSpaceQuota(size_t MB) {} // NOP
#endif // SCHEDULER_CHECKS_DISK_SPACE

private: 
	SpawnedScheduler (); // not implemented
	SpawnedScheduler (const SpawnedScheduler&); // not implemented
	SpawnedScheduler &operator = (const SpawnedScheduler&); // not implemented


private: //methods
	void checkForRunning();

private: //attribute
	int _filedes[2]; // for pipe() // PRQA S 4403
	std::string _schedBinPath;
	std::string _workOrderPath;
	SchedulerExitStatus _exitStatus;
	ScIpcClient* _ipcClient;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpawnedScheduler)
};

_ACS_END_NAMESPACE

#endif //_SPAWNEDSCHEDULER_H_

