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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/07/24 10:30:22  marfav
	Added ThreadedProcWrapper class
	

*/

#include <ThreadedProcWrapper.h>
#include <SpawnProc.h>

#include <sys/wait.h> // PRQA S 1013 

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ThreadedProcWrapper)

ThreadedProcWrapper::ThreadedProcWrapper(SpawnProc* s) :
	Thread(),
	_spawnProc (s)
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "SpawnProc address is [" << _spawnProc << "]") ;
}

ThreadedProcWrapper::~ThreadedProcWrapper() throw()
{}

	// Copy CTOR and Assignment
ThreadedProcWrapper::ThreadedProcWrapper (const ThreadedProcWrapper& t) :
	Thread (t),
	_spawnProc (t._spawnProc)
{}

ThreadedProcWrapper& 
ThreadedProcWrapper::operator= (const ThreadedProcWrapper& t)
{
	if (this != &t)
	{
		// Base class assignment
		Thread::operator= (t);

		// Any additional assignment here
		_spawnProc = t._spawnProc;
	}
	return *this;
}

void 
ThreadedProcWrapper::run()
{
	// The spawnProc is assumed to be correctly started and is property of the caller
	// this class will spawn the process and then wait for its completion
	// no diagnostic codes will be returned or trapped

	ACS_CLASS_WRITE_DEBUG_TH (ACS_VLO_VERB, "Starting the monitoring thread") ;

	bool procCompleted (false);
	while (!requestedCancel() && !procCompleted)
	{
		procCompleted = _spawnProc->waitProcCompletion();
		ACS_CLASS_WRITE_DEBUG_TH (ACS_LOW_VERB, "waitProcCompletion returned [" << boolalpha << procCompleted << "]") ;
	}

	ACS_CLASS_WRITE_DEBUG_TH (ACS_VLO_VERB, "Exiting the monitoring thread") ;

}



_ACS_END_NAMESPACE
