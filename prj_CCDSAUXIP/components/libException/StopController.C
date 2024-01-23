// PRQA S 1050 EOF
/*

	Copyright 1995-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
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

ACS_CLASS_DEFINE_DEBUG_LEVEL(StopController)

StopController::StopController(const StopController& ) 
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
	Lock lock(_mutex) ;
	return _haltLevel ;
}


void StopController::setHaltLevel(StopController::HaltLevel l)
{
    bool shallBroadcast = false;
    { // NOSONAR
        Lock lock(_mutex) ;
        
        if(  _haltLevel < l) 
        {
             _haltLevel = l ;
             shallBroadcast = true;
        }
    }

    // _mutex is now released
    if(shallBroadcast) 
    {
        { // NOSONAR - condition lock scope
             // broadcast the condition.. the level has been updated
            Lock cond_lock(_condition) ;
            _condition.broadcast();
        }

        for (auto condition_ptr : _ext_conditions_S) { Lock cond_lock(*condition_ptr) ; condition_ptr->broadcast() ; } 
    }
}

void StopController::reset()
{
	Lock lock(_mutex) ;
	_haltLevel = StopController::NONE ;
}

bool StopController::requestedCancel() const
{
	bool res = ( getHaltLevel() > StopController::NONE ) ;
	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "returning: " << std::boolalpha << res) ; 
	return  res ;
}

bool StopController::timeoutOrCancel(size_t msec) const // PRQA S 4020, 4214
{
    // First of all get the number of events occurred in to the condition
    auto events = _condition.getEventCounter();

    // wait only if cancel has not been requested at the current time
    if (! requestedCancel())
    {
        // wait until timeout or cancel requested
        // the wait will release the lock until waken up
        Lock cond_lock(_condition);
        // Now the condition is locked, nobody can broadcast or signal
        // Actually sleep only in the case no events has been triggered after the 
        // requestedCancel check
        if (events == _condition.getEventCounter())
        {
            _condition.timedwait(msec);
        }
    }
    return requestedCancel() ;
}


bool StopController::haltImmediate() const noexcept { return getHaltLevel() == StopController::IMMEDIATE ; }
bool StopController::haltLazy() const noexcept  { return getHaltLevel() == StopController::LAZY ; }


void StopController::yield() noexcept 
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


std::shared_ptr<Condition> StopController::create_condition() const
{
	/**
	   * Obtain a "Condition" (as shared pointer), directly forged by the StopController.
	   * The rationale is that the user's application might need a Condition for its own use
	   * (typically, waiting for an event) and would like to link the Condition to the StopController:
	   * The result is that, if a stop is triggered, the custom Condition will receive a broadcast.
	   * Thus immediately interrupting the wait on the Condition .
	   * Each Condition generated by this method is internally stored in a set.
	   * To remove one Condition from the set, /see unregister_condition
	   */


	// Grab the lock on the status
	Lock lock(_mutex);
	
	// Generate and register a new Condition
	auto p = std::make_shared<Condition>();
	_ext_conditions_S.insert(p) ; 

	return p ;
}


bool StopController::unregister_condition(const std::shared_ptr<Condition>& p) const
{
	/**
	  * Unregister a user's Condition, obtained by method: /see create_condition()
	  * After the Unregister, the Condition is removed from the set (so, one occurence of the shared pointer is released)
	  * and it will receive no further broadcasts in case of stop.
	  * Returns true in case of successful erase.
	*/

	// Grab the lock on the status
	Lock lock(_mutex);

	// If already present in the set, remove the shared pointer 
	if (_ext_conditions_S.end() == _ext_conditions_S.find(p)) { return false ; }
	else { _ext_conditions_S.erase(p) ; }
	return true ; 
}


std::ostream &operator << (std::ostream &os, const StopController::HaltLevel &h) 
{
    switch(h) {
    case StopController::NONE: os << "NONE" ; break ; 
    case StopController::LAZY: os << "LAZY" ; break ; 
    case StopController::IMMEDIATE: os << "IMMEDIATE" ; break ; 
    default:
        os << "<invalid HaltLevel(" << int (h) << ")>" ; break ;
    }
    return os ; // PRQA S 4028
}

exostream &operator << (exostream &os, const StopController::HaltLevel &h)  // PRQA S 4222
{
    switch(h) {
    case StopController::NONE: os << "NONE" ; break ;
    case StopController::LAZY: os << "LAZY" ; break ;
    case StopController::IMMEDIATE: os << "IMMEDIATE" ; break ;
    default:
        os << "<invalid HaltLevel(" << int (h) << ")>" ; break ;
    }
    return os ; // PRQA S 4028
}


std::ostream &operator << (std::ostream &os, const StopController &s) 
{
    return os << "StopController[" << s.getHaltLevel() << "]" ; 
}
exostream &operator << (exostream &os, const StopController &s) // PRQA S 4222
{
    return os << "StopController[" << s.getHaltLevel() << "]" ; 
}

_ACS_END_NAMESPACE

