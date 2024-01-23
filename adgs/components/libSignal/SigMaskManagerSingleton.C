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

	$Prod: A.C.S. Signal Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/05/10 14:51:33  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.1  2013/12/04 11:50:36  marpas
	simplified
	
	Revision 5.0  2013/06/17 11:32:01  marpas
	adopting libException 5.x standards
	
	Revision 2.5  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.4  2012/11/06 17:44:33  marpas
	Quality improved, coverage test done
	
	Revision 2.3  2012/02/09 14:30:28  marpas
	refactoring
	
	Revision 2.2  2012/02/07 10:33:42  marpas
	removing compiler warnings
	
	Revision 2.1  2011/04/29 11:39:01  marpas
	debug improved
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/14 12:13:38  marfav
	Added SigMask classes to library
	

*/


#include <SigMaskManagerSingleton.h>

#include <Filterables.h>
#include <cstring>


_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(SigMaskManagerSingleton)


SigMaskManagerSingleton::SigMaskManagerSingleton() :
	pattern::Singleton<SigMaskManagerSingleton> (),
    _guardCount(),
    _oldHandlers()
{
	_guardCount.clear();
	_oldHandlers.clear();

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "instancing now") ;
}


SigMaskManagerSingleton::~SigMaskManagerSingleton() ACS_NOEXCEPT 
{
}


// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"

void
SigMaskManagerSingleton::addGuard(int signum)
{
    Lock lock(_M_mutex()) ;

	pid_t pid = getpid();

	if ( _guardCount.find(signum) == _guardCount.end() ) // PRQA S 3223 # the find algo is harmless
	{
		// the first guard for this signal. Need to initialize the _guardCount map
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "first guard for signal [" << signum << "] and pid [" << pid << "]. Creating structures.") ;

		_guardCount[signum][pid] = 0;
	}

	if (_guardCount[signum][pid] == 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "disabling signal [" << signum << "] for pid [" << pid << "].") ;

		struct sigaction oldact;    // PRQA S 4102
		memset(&oldact, 0, sizeof(oldact)) ;
        struct sigaction ignoreact;    // PRQA S 4102
		memset(&ignoreact, 0, sizeof(ignoreact)) ;
        
		ignoreact.sa_handler = SIG_IGN;  // PRQA S 3039, 3048, 3080, 3081
		int result = 0;

		do // PRQA S 4680, 4682
		{
			// retry in case of EINTR
			result = sigaction(signum, &ignoreact, &oldact);
		}
		while ((result != 0) && (errno == EINTR)); //PRQA S 3224, 3231
		
		ACS_COND_THROW(0 != result,SigMaskManagerException("Unable to set signal action", errno)) ;  //PRQA S 3224

		// Store old action handler (only if the change was succesful)
		_oldHandlers[signum][pid]=oldact;
	}

	++(_guardCount[signum][pid]);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current guards for signal " << signum << "] and pid [" << pid << "] = " << 
				_guardCount[signum][pid]) ;


	// release the lock

}

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

void
SigMaskManagerSingleton::removeGuard(int signum)
{
    Lock lock(_M_mutex()) ;

	pid_t pid = getpid();

	if ( (_guardCount.find(signum) == _guardCount.end()) || ( 0 == _guardCount[signum][pid])) // PRQA S 3223, 3231 # the find algo is harmless
	{
		// This guard has never been created
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no guards for signal [" << signum << "] and pid [" << pid << "]") ;
	}
	else
	{
	    // This guard exists. Check if the guard has to be removed or just decrease the counter
	    --(_guardCount[signum][pid]);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current guards for signal " << signum << "] and pid [" << pid << "] = " << 
					_guardCount[signum][pid]) ;

	    if (0 ==_guardCount[signum][pid])
	    {
		    // The last guard has been removed
				    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "restoring handler for signal [" << signum << "] and pid [" << pid << "]") ;

		    int result = 0 ;

		    // restore old action
		    do // PRQA S 4680, 4682
		    {
			    // retry in case of EINTR
			    result = sigaction(signum, &_oldHandlers[signum][pid], 0);
		    }
		    while ((result != 0) && (errno == EINTR)); //PRQA S 3224, 3231
			
			ACS_COND_THROW(0 != result,SigMaskManagerException("Unable to reset signal action", errno )) ; //PRQA S 3224
	    }
	}

	// release the lock
}


_ACS_END_NAMESPACE

