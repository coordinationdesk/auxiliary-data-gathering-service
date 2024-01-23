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

	$Prod: A.C.S. ThinLayer IPC Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 18:26:23  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	
	Revision 1.3  2013/01/25 12:09:56  marpas
	using pattern::Singleton from libException
	
	Revision 1.2  2012/06/05 13:28:41  marfav
	compiler warnings removed
	
	Revision 1.1.1.1  2007/02/19 11:05:17  marant
	Import libIpcBase
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/


#include <SigPipeManagerSingleton.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SigPipeManagerSingleton)


SigPipeManagerSingleton::SigPipeManagerSingleton() :
	Singleton<SigPipeManagerSingleton> (),
    _mutex(),
    _guardCount(),
    _oldHandlers()
{
	_guardCount.clear();
	_oldHandlers.clear();
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


// Deactivate old sytle cast warning because of
// SIG_IGN
// macros using it
// PRQA S 1040 2
#pragma GCC diagnostic ignored "-Wold-style-cast" 

void
SigPipeManagerSingleton::addGuard()
{

	acs::ThreadSafe::Lock guard(_mutex); // acquire the lock

	pid_t pid = getpid();

	if ( _guardCount.find(pid) == _guardCount.end() )
	{
		// the first guard for this pid. Need to initialize the _guardCount map
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "first guard for pid [" << pid << "]. Creating structures.") ;

		_guardCount[pid]=0;
	}

	if (_guardCount[pid] == 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "disabling SIGPIPE for pid [" << pid << "].") ;

		struct sigaction oldact; // PRQA S 4102 2 
		struct sigaction ignoreact;
		ignoreact.sa_handler = SIG_IGN; // PRQA S 3080, 3081
		int result = 0 ;

		do
		{
			// retry in case of EINTR
			result = sigaction(SIGPIPE, &ignoreact, &oldact);
		}
		while ((result != 0) && (errno == EINTR));
		if (result != 0)
		{
			ACS_THROW (SigPipeManagerException( "Unable to set SIGPIPE action.", errno )); // PRQA S 3081
		}

		// Store old action handler (only if the change was succesful)
		_oldHandlers[pid]=oldact;
	}

	_guardCount[pid]++;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current guards for pid [" << pid << "] = " << _guardCount[pid]) ;

	// release the lock

}

// PRQA S 1040 2
#pragma GCC diagnostic warning "-Wold-style-cast"  

void
SigPipeManagerSingleton::removeGuard()
{
	acs::ThreadSafe::Lock guard(_mutex); // acquire the lock

	pid_t pid = getpid();
	_guardCount[pid]--;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current guards for pid [" << pid << "] = " << _guardCount[pid]) ;

	if (_guardCount[pid] == 0)
	{
		// The last guard was removed
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "restoring SIGPIPE handler for pid [" << pid << "].");

		int result = 0 ;

		// restore SIGPIPE
		do
		{
			// retry in case of EINTR
			result = sigaction(SIGPIPE, &_oldHandlers[pid], 0);
		}
		while ((result != 0) && (errno == EINTR));
		if (result != 0)
		{
			ACS_THROW (SigPipeManagerException( "Unable to set SIGPIPE action.", errno )); // PRQA S 3081
		}
	}


	// release the lock
}


_ACS_END_NAMESPACE

