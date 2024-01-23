// PRQA S 1050 EOF
/*

	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SPA

*/

#ifndef _StopController_H_
#define _StopController_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <exException.h>
#include <exStream.h>
#include <ThreadSafe.h>

#include <sys/time.h> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief Manages the stop requests to a thread
 * It is the container of the stop requests
 * It derives from virtual base class ThreadSafe: it is harmless 
 **/

class StopController { 

public:

	/** StopController Exceptions */
	exDECLARE_EXCEPTION(StopControllerException, exException) ; // PRQA S 2131, 2502

    /**
       * \brief It is the "strength" of the halt request 
	   *
	   * The following relation is always true: NONE < LAZY < IMMEDIATE
       **/
	enum HaltLevel{
		NONE=1,
		LAZY=2,
		IMMEDIATE=3
	} ;



	/**
	   * When created the halt level is set to NONE
	   **/
	StopController();

	virtual ~StopController() ACS_NOEXCEPT ;


	/**
	 * Does not copy the halt level: new StopController will be created with halt level to NONE
	 **/
	StopController(const StopController & );

	/**
	 * Does not copy the halt level: the halt level is left unchanged
	 **/
	StopController &operator=(const StopController &);

	virtual HaltLevel getHaltLevel() const ;

	/**
	   * The halt level is changed only if the new level is higher than current one.
	   * i.e. only the following transitions are performed
	   * NONE -> LAZY
	   * NONE -> IMMEDIATE
	   * LAZY -> IMMEDIATE
	   * In case of illegal transition the halt value is silently left unchanged.
	   **/
	virtual void setHaltLevel(HaltLevel) ;

	/**
	 *\brief Returns true if halt has been requested.
	 * it returns true if the halt level is LAZY or IMMEDIATE
	 */
	virtual bool requestedCancel() const ;
    
	/**
	   * Wait for the timeout or cancel. Returns true if cancel was requested, false if timeout expired
	   *
	   * \param msec the timeout.
	   */
	virtual bool timeoutOrCancel(size_t msec) const ; // PRQA S 2624, 4400, 4405

    bool haltImmediate() const ACS_NOEXCEPT ;
    bool haltLazy() const ACS_NOEXCEPT ;

    static unsigned long nanosleep(unsigned long nanosec) ; // PRQA S 2502

#ifndef TEST
protected:
#else
public:
#endif
	virtual void reset() ;

	/**
	   * \brief Yield to other thread (possibly)
	   */
	static void yield() ACS_NOEXCEPT ; 

private:
	Condition _condition ;
	HaltLevel volatile _haltLevel ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StopController) ;

};

std::ostream &operator << (std::ostream &, const StopController::HaltLevel &) ;
exostream &operator << (exostream &, const StopController::HaltLevel &) ; // PRQA S 2072

std::ostream &operator << (std::ostream &, const StopController &) ;
exostream &operator << (exostream &, const StopController &) ; // PRQA S 2072


using pattern::Singleton;

/**
 * \brief Application Stop Controller Singleton
 * Implements a unique and global scope accessible StopController 
 * via a Singleton < StopController >
 **/
class AppStopControllerSingleton : public Singleton < StopController >  // PRQA S 2153
{
};

/**
 * Convenience function, returns a reference to the global application stop controller instance
 **/

inline StopController& AppStopController() { return *(AppStopControllerSingleton::instance()); }

_ACS_END_NAMESPACE

#endif //_StopController_H_

