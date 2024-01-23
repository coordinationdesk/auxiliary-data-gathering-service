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
	Revision 2.9  2015/05/13 14:57:23  marfav
	abortBySigterm is now part of the standard interfaces
	
	Revision 2.8  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.7  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.6  2013/11/11 11:24:18  marfav
	S2PDGS-516: flushing the work order file before spawning the scheduler
	
	Revision 2.5  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.4  2013/07/07 18:33:02  marpas
	application of coding best practices in progress
	some qa warnings and compilation ones removed
	adoption of libException 5.x standards in progress
	
	Revision 2.3  2013/02/26 09:00:50  marfav
	Compiler warnings removed
	
	Revision 2.2  2012/02/13 17:03:43  marpas
	removing compiler warnings
	
	Revision 2.1  2012/02/01 15:59:50  matteo.airoldi
	destructor compilation error fixed
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.3  2004/11/19 14:58:21  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/


#include <SpawnedScheduler.h>

#include <unistd.h>
#include <fcntl.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SpawnedScheduler)

SpawnedScheduler::SpawnedScheduler (const string & thePath, const string & woPath) :
	SpawnProc (thePath),
    SchedulerIF(),
    _filedes(),
	_schedBinPath (thePath),
	_workOrderPath (woPath),
	_exitStatus (ssNOTSTARTED),
	_ipcClient (0)
{
	_filedes[0] = _filedes[1] = -1;

}

SpawnedScheduler::~SpawnedScheduler () throw () 
{
    // PRQA S 4631 L1
	try {
		delete _ipcClient;

		if (_filedes[0] != -1) { close(_filedes[0]); }
		if (_filedes[1] != -1) { close(_filedes[1]); }
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
    // PRQA L:L1
}

bool SpawnedScheduler::isSchedulerRunning()
{
	return SpawnProc::isRunning();
}

void SpawnedScheduler::stop()
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::stopType);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending stop command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send stop command. No IPC Client instance.")); // PRQA S 3081
	}
}

void SpawnedScheduler::abort()
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::abortType);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending abort command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send abort command. No IPC Client instance.")); // PRQA S 3081
	}
}

void SpawnedScheduler::suspend()
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::suspendType);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending suspend command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send suspend command. No IPC Client instance.")); // PRQA S 3081
	}
}

void SpawnedScheduler::resume()
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::resumeType);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending resume command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send resume command. No IPC Client instance.")); // PRQA S 3081
	}
}

void SpawnedScheduler::doKill ()
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::killType);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending kill command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send kill command. No IPC Client instance.")); // PRQA S 3081
	}
}


void SpawnedScheduler::sendCmd (const string& cmdType, const string& cmdValue)
{
	checkForRunning();
	if (_ipcClient)
	{
		try
		{
			TlMessage msg (TlMessage::userType, cmdType, cmdValue);
			_ipcClient->sendMessage (msg);
		}
		catch (exception& ex)
		{
			ACS_THROW (SpawnedSchedulerException(ex, "Exception caught wile sending user command")); // PRQA S 3081
		}
	}
	else
	{
		ACS_THROW (SpawnedSchedulerException("Cannot send user command. No IPC Client instance.")); // PRQA S 3081
	}
}

void SpawnedScheduler::abortBySigTerm ()
{
	sendKillSignal (SIGTERM);
}

int SpawnedScheduler::sendKillSignal (int signal) // PRQA S 4211
{
	return SpawnProc::kill (signal);
}


bool SpawnedScheduler::getExitStatus(SchedulerExitStatus& status) // PRQA S 4020
{
	status = _exitStatus;
	if (status == ssNOTSTARTED) {
		return false; // The scheduler never started
    }
	else
	{
		int procExitCode = 0;
		bool result = SpawnProc::exitCode(procExitCode);

		if (!result)
		{
			// result == false => the Scheduler doesn't have finished its run
			// so the exit code shall be ssUNDEFINED
			return false;
		}
		else
		{
			// have a valid exit code stored in procExitCode
			// update the internal status and return true
			_exitStatus = status = SchedulerExitStatus (procExitCode); // PRQA S 3013, 3081
			return true;
		}
	}
}

void SpawnedScheduler::checkForRunning()
{
	if (!isSchedulerRunning())
	{
		ACS_THROW (SpawnedSchedulerException("Cannot perform operation. Scheduler is not running.")); // PRQA S 3081
	}
}

void SpawnedScheduler::execute (const scOrder& workOrder)
{
	if (_exitStatus != ssNOTSTARTED)
	{
		ACS_THROW (SpawnedSchedulerException("Cannot execute. The Scheduler was already spawned.")); // PRQA S 3081
	}
	// From now the exit code is Undefined
	// This allows to mark the scheduler as started
	_exitStatus = ssUNDEFINED;

	// create the pipe for communications with spawned process
	pipe(_filedes);
	_ipcClient = new ScIpcClient (_filedes[1]);
	this->setInDescriptor (_filedes[0]);

	// Have to write the scOrder to the requested file and pass it to the scheduler bin command line
	ofstream of(_workOrderPath.c_str());
	workOrder >> of;
	// S2PDGS-516 Flush it before spawning
	of.close();

	
	this->args()[0] = _workOrderPath;
	this->spawn();
}

bool SpawnedScheduler::timedWaitExit (unsigned long timeout)
{
	return SpawnProc::waitExitOrTimeout (timeout);
}


pid_t SpawnedScheduler::getSchedulerPid() const 
{
    return SpawnProc::pid();
}

_ACS_END_NAMESPACE
