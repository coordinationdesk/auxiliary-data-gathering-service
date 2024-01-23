/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
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
	Revision 2.3  2015/05/04 15:30:27  marfav
	Removing compiler warnings
	
	Revision 2.2  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.1  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/03/18 10:27:01  marfav
	excmcf support added
	
	Revision 1.3  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.2  2004/04/07 13:24:35  marfav
	Does not use ACE anymore
	Unsing exStream.h
	
	Revision 1.1  2003/10/03 11:39:22  marfav
	Import SignalGuard
	


*/


#include <SignalGuard.h>

#include <Filterables.h>
#include <exStream.h>

using namespace std;
using namespace acs;


ACS_CLASS_DEFINE_DEBUG_LEVEL(SignalGuard)

SignalGuard::SignalGuard () : 
    _oldSet(),
    _newSet(),
    _masked(false)
{
	// create a new set of signal mask for signals
	// SIGINT
	// SIGTERM
	sigemptyset (&_newSet);
	if (sigaddset (&_newSet, SIGINT))
	    ACS_LOG_WARNING ("Cannot prepare signal mask for signal SIGINT");
	if (sigaddset (&_newSet, SIGTERM))
	    ACS_LOG_WARNING ("Cannot prepare signal mask for signal SIGTERM");
	mask();
}

void
SignalGuard::mask()
{
    if (!_masked)
    {
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Setting signal mask to block SIGINT and SIGTERM");

	// appy the mask and store the old mask set in the private attribute
	if (sigprocmask (SIG_BLOCK, &_newSet, &_oldSet))
	{
	    ACS_LOG_WARNING ("Cannot set sigprocmask");
	}
	else
	{
	    _masked = true;
	    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Signal mask set");
	}
    }
    else
    {
	    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "The mask was already set");
    }
}

void
SignalGuard::unmask()
{
    if (_masked)
    {
		// restore the old mask ad raise the pending signals
		sigset_t pendingSig;

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Retrieving pending signals")

		sigpending (&pendingSig);

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Restoring signal mask to old value")

		if (sigprocmask (SIG_SETMASK, &_oldSet, NULL))
		{
			ACS_LOG_WARNING ("Cannot set sigprocmask");
		}
		    else
		{
			_masked = false;
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Signal mask unset")
		}

		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Raising the pending signals")

		raiseSignals (pendingSig);
    }
    else
    {
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Signal mask was already unset")
    }
}

void
SignalGuard::flush()
{
	unmask();
	mask();
}

void
SignalGuard::raiseSignals(sigset_t pendingSig)
{
	if (sigismember (&pendingSig, SIGTERM))
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Raising SIGTERM")
		raise (SIGTERM);
	}
	else if (sigismember (&pendingSig, SIGINT))
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Raising SIGINT")
		raise (SIGINT);
	}
}

SignalGuard::~SignalGuard() throw() 
{
    try {
	    unmask();
    }
    catch(exception &x) {
        excerr.notify(x) ;
    } 
}

