// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libThread$

	$Id$

	$Author$

	$Log$
	Revision 5.3  2013/10/07 14:32:53  marpas
	timeoutOrCancel interface doc fixed
	
	Revision 5.2  2013/07/09 11:27:44  marpas
	yield, timeoutOrCancel and nanosleep method moved from Thread to StopController
	
	Revision 5.1  2013/06/18 12:27:25  marpas
	deprecated include changed
	
	Revision 5.0  2013/06/10 16:08:25  marpas
	adopting libException 5.x standards
	
	Revision 1.10  2013/04/02 17:26:33  marpas
	haltImmediate & haltLazy methods
	
	Revision 1.9  2013/04/02 17:06:20  marpas
	inserters operator added and tested
	
	Revision 1.8  2013/02/22 10:28:54  marpas
	coding best practices applied
	
	Revision 1.7  2012/11/06 18:01:57  marpas
	Quality improved
	
	Revision 1.6  2012/09/27 16:41:56  marpas
	quality rules implementation
	
	Revision 1.5  2012/02/09 14:19:20  marpas
	refactoring
	
	Revision 1.4  2012/01/31 15:53:40  marpas
	removing compiler warnings
	
	Revision 1.3  2011/03/14 09:24:46  marpas
	StopController does not longer inherits from ThreadSafe, but contain its own private mutex instead
	
	Revision 1.2  2009/06/26 10:50:40  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.1  2007/11/28 10:01:04  lucio.pulvirenti
	missing files added
	
        
*/ 

#include <StopController.h>

#include <Filterables.h>

#include <unistd.h>
#include <cmath>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(StopController)


StopController::StopController() : _condition(), _haltLevel(StopController::NONE)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

StopController::~StopController() ACS_NOEXCEPT 
{ // PRQA S 4631 L1 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
} // PRQA L:L1

StopController::StopController(const StopController& aStopController) : _condition(), _haltLevel(StopController::NONE)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

StopController& StopController::operator = (const StopController&) // PRQA S 4073
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	// The _haltLevel level *MUST* not be copied.
	return *this;
}


StopController::HaltLevel StopController::getHaltLevel() const
{
	Lock lock(_condition) ;
	return _haltLevel ;
}


void StopController::setHaltLevel(StopController::HaltLevel l)
{
	Lock lock(_condition) ;
	if(  _haltLevel < l) {
		 _haltLevel = l ;
		 // broadcast the condition.. the level has been updated
		 _condition.broadcast();
    }
}

void StopController::reset()
{
	Lock lock(_condition) ;
	_haltLevel = StopController::NONE ;
}

bool StopController::requestedCancel() const
{
	bool res = ( getHaltLevel() > StopController::NONE ) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning: " << boolalpha << res) ; 
	return  res ;
}




bool StopController::timeoutOrCancel(size_t msec) const // PRQA S 4020, 4214
{
	// Grab the lock on the status
	Lock lock(_condition);
	// wait only if cancel has not been requested at the current time
	if (not requestedCancel())
	{
		// wait until timeout or cancel requested
		// the wait will release the lock until waken up
		_condition.timedwait(msec);
	}
	// keep in mind the lock is still active at this point
	return requestedCancel() ;
}


bool StopController::haltImmediate() const ACS_NOEXCEPT { return getHaltLevel() == StopController::IMMEDIATE ; }
bool StopController::haltLazy() const ACS_NOEXCEPT  { return getHaltLevel() == StopController::LAZY ; }


void StopController::yield() ACS_NOEXCEPT 
{
	::pthread_yield();
}


unsigned long StopController::nanosleep(unsigned long nanosec)
{
	struct timespec req ;    // PRQA S 4102
	req.tv_sec  = nanosec / 1000000000 ; // PRQA S 3000, 3084, 4400
	req.tv_nsec = nanosec % 1000000000 ; // PRQA S 3000, 3084, 4400
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "tv_sec: " << req.tv_sec
			   << " tv_nsec: " << req.tv_nsec) ;
	struct timespec rem ;    // PRQA S 4102
	::nanosleep(&req,&rem) ;
	return rem.tv_sec*1000000000+rem.tv_nsec ; // PRQA S 3000, 4400
}

ostream &operator << (ostream &os, const StopController::HaltLevel &h) 
{
    switch(h) {
    case StopController::NONE: { os << "NONE" ; break ; }
    case StopController::LAZY: { os << "LAZY" ; break ; }
    case StopController::IMMEDIATE: { os << "IMMEDIATE" ; break ; }
    default:
        { os << "<invalid HaltLevel(" << int (h) << ")>" ; break ; } // PRQA S 3081
    }
    return os ; // PRQA S 4028
}

exostream &operator << (exostream &os, const StopController::HaltLevel &h)  // PRQA S 4222
{
    switch(h) {
    case StopController::NONE: { os << "NONE" ; break ; }
    case StopController::LAZY: { os << "LAZY" ; break ; }
    case StopController::IMMEDIATE: { os << "IMMEDIATE" ; break ; }
    default:
        { os << "<invalid HaltLevel(" << int (h) << ")>" ; break ; }// PRQA S 3081
    }
    return os ; // PRQA S 4028
}


ostream &operator << (ostream &os, const StopController &s) 
{
    return os << "StopController[" << s.getHaltLevel() << "]" ; 
}
exostream &operator << (exostream &os, const StopController &s) // PRQA S 4222
{
    return os << "StopController[" << s.getHaltLevel() << "]" ; 
}

_ACS_END_NAMESPACE

