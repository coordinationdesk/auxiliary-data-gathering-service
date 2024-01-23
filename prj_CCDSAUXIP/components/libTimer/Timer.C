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

	$Prod: A.C.S. Timer Library $

	$Id$

	$Author$

	$Log$
	Revision 2.9  2013/11/21 19:33:35  marpas
	qa warnings and test coverage
	
	Revision 2.8  2013/11/21 12:16:02  marpas
	RTTimer class added
	
	Revision 2.7  2013/02/22 10:34:06  marpas
	Timer does not lonh inherits virtually form ThreadSafe
	Ita has an internal mutex instead.
	
	Revision 2.6  2012/11/27 12:24:53  marpas
	qa rules
	
	Revision 2.5  2012/02/09 14:14:16  marpas
	refactoring
	
	Revision 2.4  2012/01/31 15:38:06  marpas
	removing compiler warnings
	
	Revision 2.3  2011/03/01 11:27:28  marpas
	GCC 4.4.x support
	
	Revision 2.2  2009/10/06 12:13:40  marpas
	interface improved
	
	Revision 2.1  2009/05/15 15:14:34  marpas
	rules fixing
	
	Revision 2.0  2006/02/28 08:28:40  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/05/06 08:39:53  marpas
	headers fixed
	
	Revision 1.8  2004/08/30 14:13:54  marpas
	randomDelay method added
	Note that according to the nanosleep man, resolution is 10msec on Linux/i386
	
	Revision 1.7  2004/04/23 09:10:41  marpas
	delay method is static now
	
	Revision 1.6  2004/04/14 09:43:46  marpas
	Thread safe class used
	
	Revision 1.5  2003/07/11 12:57:02  marpas
	exException::clone implemented
	
	Revision 1.4  2003/06/11 14:39:52  marpas
	isOlder method added, architectural slightly changed
	
	Revision 1.3  2003/01/16 11:33:45  marpas
	lock & unlock exceptions added
	
	Revision 1.2  2003/01/15 17:08:37  marpas
	thread safe
	
	Revision 1.1.1.1  2002/10/04 16:26:05  paoscu
	Import libTimer
	

*/
 
#include <Timer.h>
#include <exException.h>
#include <unistd.h>
#include <cstring>
#include <tV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    tV version ;
}


bool Timer::_srandInitialized = false ;

namespace
{
    bool getTimeOfDay( struct timeval *a, struct timezone *z) 
    {
        struct timespec t ;
        if(clock_gettime(CLOCK_MONOTONIC, &t))
            return false ;  // unable to get time
        a->tv_sec = t.tv_sec ;
        a->tv_usec = t.tv_nsec /1000 ;
        return true ;
    }
}

Timer::Timer() :
    _start(),
    _stop(), 
    _life(),
    _zone(),
	_running(false),
    _mutex()
{
	getTimeOfDay(&_start,&_zone) ; // to avoid misuse of life...if never started

}


Timer::~Timer() throw() { }

Timer::Timer(const Timer &r) :
    _start(r._start),
    _stop(r._stop), 
    _life(r._life),
    _zone(r._zone),
    _running(r._running),
    _mutex()
{
}

Timer &Timer::operator=(const Timer &rhs) 
{
	if (this != &rhs) { // avoid auto_assignation
		Lock lock(_mutex) ;
		Lock lockr(rhs._mutex) ;
		
		// deep copy 
		memcpy(&_start, &rhs._start, sizeof(_start)) ;
		memcpy(&_stop, &rhs._stop, sizeof(_stop)) ;
		memcpy(&_life, &rhs._life, sizeof(_life)) ;
		memcpy(&_zone, &rhs._zone, sizeof(_zone)) ;
		_running = rhs._running ;
	}
	
	return *this ;
}


void Timer::start()
{
	Lock lock(_mutex) ;
	
	// store the actual time
	
	getTimeOfDay(&_start,&_zone) ; // to avoid misuse of life...if never started


	_stop = _start ;
	_running = true ;
}

void Timer::stop()
{
	Lock lock(_mutex) ;
	// store the actual time
	
	getTimeOfDay(&_stop,&_zone) ; // to avoid misuse of life...if never started

	
	if (_start.tv_usec > _stop.tv_usec) {
	    _stop.tv_usec += 1000000 ;  // 1 sec more (1e06 usecs)          // PRQA S 4400
	    _stop.tv_sec-- ;
	}
	_running = false ;
}

void Timer::stopLife()
{
	Lock lock(_mutex) ;
	// store the actual time
	
	getTimeOfDay(&_life,&_zone) ; // to avoid misuse of life...if never started
	
	if (_start.tv_usec > _life.tv_usec) {
	    _life.tv_usec += 1000000 ; // 1 sec more (1e06 usecs)          // PRQA S 4400
	    _life.tv_sec-- ;
	}
}

Timer::Delay Timer::elapsed()
{
	Lock lock(_mutex) ;
	if (_running) {	stop() ; }
	
	struct timeval lapsed ;                                     // PRQA S 4102
	
	// deal with secs and fraction separately
	lapsed.tv_usec = _stop.tv_usec - _start.tv_usec ;
	lapsed.tv_sec = _stop.tv_sec - _start.tv_sec ;

	// remember the return value is in msec
	return lapsed.tv_sec*1000 + lapsed.tv_usec/1000 ;          // PRQA S 3000, 4400
}

Timer::Delay  Timer::life()
{
	Lock lock(_mutex) ;
	struct timeval lapsed ;                                     // PRQA S 4102
	
	stopLife() ;
	
	// deal with secs and fraction separately	
	lapsed.tv_usec = _life.tv_usec - _start.tv_usec ;
	lapsed.tv_sec = _life.tv_sec - _start.tv_sec ;

	// remember the return value is in msec
	return lapsed.tv_sec*1000 + lapsed.tv_usec/1000 ;          // PRQA S 3000, 4400
}


bool Timer::isOlder(Delay d)         // PRQA S 4020
{
	Lock lock(_mutex) ;
	stop() ;
		
	if (_stop.tv_sec < _start.tv_sec) { // there was a date/time misalignement

		ACS_LOG_TRYCATCHNOTIFY_EX(DateTimeMisalignement("Probably the date was set in the past.")) ;        // PRQA S 3081

		_start = _stop ;
		return true ;
	}
	else {
		return elapsed() > d ;
    }
}



void Timer::delay(Delay msec)
{
    struct timeval delay ;                                   // PRQA S 4102

    delay.tv_sec = msec/1000 ;                              // PRQA S 3000, 3084, 4400
    delay.tv_usec = 1000 * (msec%1000) ;                    // PRQA S 3000, 3084, 4400

    select(0, 0, 0, 0,  &delay) ;
}



void Timer::randomDelay(Delay maxMsec, Delay minMsec)
{
	if (!_srandInitialized) {
		srand(getpid() + time (0));  // PRQA S 3000, 3010    
		_srandInitialized = true ;
	}

	delay(static_cast<unsigned>((maxMsec-minMsec) * float(rand())/RAND_MAX + minMsec)) ;    // PRQA S 3011, 3081, 3084
}


bool Timer::running() const throw() { return _running ; }   // PRQA S 4120




RTTimer::RTTimer() :
    _start(),
    _stop(), 
	_running(false),
    _mutex()
{
}


RTTimer::~RTTimer() throw() { }

RTTimer::RTTimer(const RTTimer &r) :
    _start(r._start),
    _stop(r._stop), 
    _running(r._running),
    _mutex()
{
}

RTTimer &RTTimer::operator=(const RTTimer &rhs) 
{
	if (this != &rhs) { // avoid auto_assignation
		Lock lock(_mutex) ;
		Lock lockr(rhs._mutex) ;
		
		// deep copy 
		memcpy(&_start, &rhs._start, sizeof(_start)) ;
		memcpy(&_stop, &rhs._stop, sizeof(_stop)) ;
		_running = rhs._running ;
	}
	
	return *this ;
}


void RTTimer::start()
{
	Lock lock(_mutex) ;
	
	// store the actual time
	::clock_gettime (CLOCK_REALTIME, &_start);

	_stop = _start ;
	_running = true ;
}

void RTTimer::stop()
{
	Lock lock(_mutex) ;
	// store the actual time
	
	::clock_gettime (CLOCK_REALTIME, &_stop);
	
	if (_start.tv_nsec > _stop.tv_nsec) {
	    _stop.tv_nsec += 1000000000 ;  // 1 sec more (1e09 nsecs)          // PRQA S 4400
	    _stop.tv_sec-- ;
	}
	_running = false ;
}

long double RTTimer::elapsed()
{
	Lock lock(_mutex) ;
	if (_running) {	stop() ; }
	
	struct timespec lapsed ;                                     // PRQA S 4102
	
	// deal with secs and fraction separately
	lapsed.tv_nsec = _stop.tv_nsec - _start.tv_nsec ;
	lapsed.tv_sec = _stop.tv_sec - _start.tv_sec ;

	// remember the return value is in sec
	return lapsed.tv_sec + lapsed.tv_nsec/1000000000.0L ;          // PRQA S 3011, 4400
}


_ACS_END_NAMESPACE
