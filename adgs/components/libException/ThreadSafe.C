// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.13  2018/08/29 15:56:48  marpas
	qa warnings fixed
	
	Revision 5.12  2017/07/10 11:41:04  davide.tiriticco
	compiler warnings removed
	
	Revision 5.11  2017/03/03 09:26:09  marfav
	S3MPM-874
	Conditions do not allow the use of external mutex anymore and provide its own embedded mutex
	Conditions embed also a counter of the broadcast/signal events
	
	Revision 5.10  2016/06/13 12:50:20  marpas
	removing qa warnings
	
	Revision 5.9  2016/03/29 12:36:46  marfav
	S1PDGS-31708 allow re init of mutex if needed like after a fork
	
	Revision 5.8  2014/06/04 17:45:17  marpas
	ThreadSafe::unlock signature changed
	it returns a boolean to signal if the unlock was succesful
	it has also a boolean param that, if true, does not cause exception in case the mutex was not owned by the unlocking thread and the methods will return false in this case.
	If the passed param is false (default) any error causes an exception as before.
	
	Revision 5.7  2014/03/25 14:49:28  marfav
	Completed RWMutex coding. OnExitDetector introduced
	
	Revision 5.6  2014/03/24 18:41:13  marpas
	qa warning
	
	Revision 5.5  2014/03/24 15:35:15  marfav
	Introducing RWMutex - work in progress
	
	Revision 5.4  2014/03/19 15:58:54  marfav
	Condition timedwait interface added. It now returns bool
	
	Revision 5.3  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.2  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.1  2013/06/05 15:57:39  marpas
	new debugging and logging macros adopted
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.11  2012/12/20 13:25:44  marpas
	coding best practices
	explain used instead of what()
	tests improved
	
	Revision 4.10  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.9  2012/10/29 14:29:52  marpas
	robustness improved
	
	Revision 4.8  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.7  2012/06/22 13:41:46  marpas
	pr qa instrumentation
	
	Revision 4.6  2012/06/14 17:16:40  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.4  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.3  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.2  2012/02/20 15:04:03  marpas
	refactoring
	
	Revision 4.1  2012/01/31 14:13:04  marpas
	getting rid of boost and compilation warning reduced
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.6  2011/03/01 17:34:21  micmaz
	removed abort() used only for test.
	
	Revision 3.5  2011/03/01 11:16:37  marpas
	Thread::Condition dtor notifies in broadcast
	
	Revision 3.4  2011/01/26 12:07:54  marpas
	ThreadSafe::Condition::timedwait can return ThreadSafe::Timeout exception (which has been defined)
	
	Revision 3.3  2011/01/25 16:06:19  marpas
	ThreadSafe::Condition temptative implementation
	
	Revision 3.2  2010/09/13 13:26:00  marpas
	aligned to new StackTrace interface
	
	Revision 3.1  2009/12/02 11:46:10  marpas
	order of static variable dtor calls improved. No exeption from exstream::notify() reimplementation and better management into ThreadSafe dtro method.
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	
	Revision 2.7  2009/06/26 14:10:44  marpas
	robustness improved on mutex when process exits
	
	Revision 2.6  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.5  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.4  2008/05/06 16:15:23  marpas
	locking during the whole exLogStream ctor was needed (APF-218)
	
	Revision 2.3  2007/06/26 15:36:51  giucas
	exceptions caught in dtors
	
	Revision 2.2  2007/04/11 17:13:27  giucas
	Destructor does not throw any exception
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.11  2005/10/12 22:21:03  ivafam
	initialization improved
	
	Revision 1.10  2005/09/20 17:31:24  marpas
	thread safety improved, test accuracy too
	
	Revision 1.9  2004/04/28 10:42:05  marpas
	Locking policy changed, only recursive thread
	
	Revision 1.8  2004/04/28 10:11:22  marpas
	Lock policy improved
	ThreadSafe::SignalAction implemented
	
	Revision 1.7  2004/04/14 14:09:26  marpas
	ThreadSafe::unlock yields other threads
	
	Revision 1.6  2004/04/14 13:53:04  marpas
	useless include removed
	
	Revision 1.5  2004/04/14 13:51:42  marpas
	DEBUG removed --- loop
	
	Revision 1.4  2004/04/14 13:40:35  envisat
	ThreadSafe::Lock debug added
	
	Revision 1.3  2004/04/07 14:31:22  marpas
	copy ctor and assignement operator implemented
	
	Revision 1.2  2004/04/02 14:15:14  marpas
	ThreadSafe class introduced and used inside
	
	Revision 1.1  2004/04/02 07:51:14  marpas
	class rearrangement for ThreadSafe and Mutex
	
	Revision 1.1  2004/04/01 17:13:16  marpas
	ThreadSafe class being introduced, there is still work to be done
	
*/

#include <ThreadSafe.h>
#include <exStackTrace.h>
#include <exStream.h>
#include <iostream>

#include <OnExitDetector.h>

_ACS_BEGIN_NAMESPACE(acs)

using std::exception ;

namespace {
	/** \brief declare ThreadSafe::DeadLock exception */
	exDECLARE_EXCEPTION(DeadLock,ThreadSafe::Exception) ;       // PRQA S 2131, 2153, 2502
}


ThreadSafe::ThreadSafe() : _mutexAttribute(), _mutex()
{
	int ret = ::pthread_mutexattr_init(&_mutexAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Mutex attributes",ret)) ; // PRQA S 3081
    
	ret = ::pthread_mutexattr_settype(&_mutexAttribute,PTHREAD_MUTEX_RECURSIVE) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to set Mutex attributes",ret)) ; // PRQA S 3081
    
	ret = ::pthread_mutex_init(&_mutex, &_mutexAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init the Mutex",ret)) ; // PRQA S 3081

}

ThreadSafe::ThreadSafe(const ThreadSafe &t) : _mutexAttribute(), _mutex()
{
	int ret = ::pthread_mutexattr_init(&_mutexAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Mutex attributes",ret)) ; // PRQA S 3081
    
	int kind = 0 ;
	ret = ::pthread_mutexattr_gettype(&t._mutexAttribute, &kind) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to get Mutex attributes",ret)) ; // PRQA S 3081
    
	ret = ::pthread_mutexattr_settype(&_mutexAttribute,kind) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to set Mutex attributes",ret)) ; // PRQA S 3081
	
 	ret = ::pthread_mutex_init(&_mutex, &_mutexAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init the Mutex",ret)) ; // PRQA S 3081
}


ThreadSafe & ThreadSafe::operator=(const ThreadSafe &) // PRQA S 4072, 4250
{
    // No self assignement check
	return *this ;
}

ThreadSafe::~ThreadSafe() ACS_NOEXCEPT 
{
    // cerr << SimpleDebugSignature << hex << this << dec << '\n' ;
	// attention, the exception handling is tricky
	int ret = ::pthread_mutex_destroy(&_mutex) ;
	bool oldTrace = exStackTrace::allow() ; // store previous trace mode 
	try { // PRQA S 4631 L1
		if (!OnExitDetector::instance()->exiting()) {
			if (ret != 0) {
				exStackTrace::allow(true) ; // trace mode true
				ACS_THROW(Exception("Unexpected errno from pthread_mutex_destroy",ret)) ; // PRQA S 3081
			}
		}
	}
	catch(exception& e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
        if (ret !=0) { // reset old trace mode 
			exStackTrace::allow(oldTrace) ; // restore previous trace mode 
		}
	} // PRQA L:L1
	ret =::pthread_mutexattr_destroy(&_mutexAttribute) ;
	try { // PRQA S 4631 L2
		if (!OnExitDetector::instance()->exiting()) {
			if (ret != 0) {
				exStackTrace::allow(true) ; // trace mode true
				ACS_THROW(Exception("Unexpected errno from pthread_mutexattr_destroy",ret)) ; // PRQA S 3081
			}
		}
	}
	catch(exception& e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
		if (ret !=0) { // reset old trace mode 
			exStackTrace::allow(oldTrace) ; // restore previous trace mode 
		}
	} // PRQA L:L2
}

void ThreadSafe::reInit (bool throwIfFailures) const // PRQA S 4020, 4214
{
	int ret = ::pthread_mutex_init(&_mutex, &_mutexAttribute) ;
	ACS_COND_THROW(throwIfFailures &&throwIfFailures &&  (ret != 0), ThreadSafe::Exception("Unable to re-init the Mutex",ret)) ; // PRQA S 3081
}

void ThreadSafe::lock() const                           // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ? 
		return ; // do not perform any lock ... just return
    }
	int ret = ::pthread_mutex_lock(&_mutex) ;
    // cerr << SimpleDebugSignature << hex << this << dec << ' ' << ret << '\n' ;
	switch(ret) { // according with the kind of error 
	case 0:                                             // PRQA S 4066
        // ok
		return ;
	case EINVAL: // not a valid mutex // PRQA S 4011 # throws
        {
		    ACS_THROW(InvalidMutex("lock",ret)) ; // PRQA S 3081
        }
	case EDEADLK: // a deadlock - will never occur because mutexes are created with PTHREAD_MUTEX_RECURSIVE // PRQA S 4011 # throws 
		ACS_THROW(DeadLock("lock",ret)) ; // PRQA S 3081
	default: // any other // PRQA S 4612 # throws 
		ACS_THROW(Exception("Unexpected errno while locking",ret)) ; // PRQA S 3081
	}
}

bool ThreadSafe::unlock(bool noExIfNotOwner) const                         // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ? 
		return true ; // do not perform any unlock ... just return
    }
	int ret = ::pthread_mutex_unlock(&_mutex) ;
	switch(ret) {
	case 0:                                             // PRQA S 4066
        // ok, yeld to other threads if any
		::pthread_yield() ;
		return  true;
	case EINVAL: // not a valid mutex // PRQA S 4011 # throws 
		ACS_THROW(InvalidMutex("unlock",ret)) ; // PRQA S 3081
	case EPERM: // no permissions on this one
        {
		    ACS_COND_THROW(not noExIfNotOwner, NoPermission("unlock",ret)) ; // PRQA S 3081
            return false ;
        }
	default: // any other // PRQA S 4612 # throws 
		ACS_THROW(Exception("Unexpected errno while unlocking",ret)) ; // PRQA S 3081
	}
}
		
bool ThreadSafe::trylock() const // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ?
		return true ; // just return ... 
    }

    int callret = ::pthread_mutex_trylock (&_mutex);

	switch(callret) {
	case 0: { return true ; } // ok locked
    case EBUSY: { return false ; } // ok but busy
    case EAGAIN: // PRQA S 4011 # throws 
   		ACS_THROW(ThreadSafe::Exception("Too many locks active",callret)) ; // PRQA S 3081
	case EDEADLK: // a deadlock would never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE // PRQA S 4011 # throws 
		ACS_THROW(DeadLock("Deadlock detected",callret)) ; // PRQA S 3081
	case EINVAL:    // PRQA S 4011 # throws                                 
        // invalid mutex: error
		ACS_THROW(ThreadSafe::InvalidMutex("Invalid lock instance",callret)) ; // PRQA S 3081
	default: // PRQA S 4612 # throws 
        // any other error: error
		ACS_THROW(ThreadSafe::Exception("Unexpected code while trying to get a timed read lock",callret)) ; // PRQA S 3081
    }
}

bool ThreadSafe::trylock(unsigned long msectimeout) const // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ?
		return true ; // just return ... 
    }
  
    if (0 == msectimeout) { return this->trylock() ; }
    
    // Get the current time
    struct timespec tp; // PRQA S 4102
    int callret = ::clock_gettime (CLOCK_REALTIME, &tp);

    // In case of issues gathering the system time throw an exception
    ACS_COND_THROW(callret != 0, ThreadSafe::Exception("Unable to get system time", errno)) ; // PRQA S 3081

    // Add the delay to the current time
    unsigned long secs = msectimeout / 1000;                  // PRQA S 3084
    unsigned long const nsecs = (msectimeout % 1000) * 1000000;     // PRQA S 3084

    tp.tv_sec += secs;                                  // PRQA S 3000, 3084
    tp.tv_nsec += nsecs;                                // PRQA S 3000, 3084

    Condition::normalize(tp) ;
    
    // try to get the wrlock in the given time  
    callret = ::pthread_mutex_timedlock(&_mutex, &tp);
    
    // The return value will be changed if timeout is detected
    bool retvalue = true ;

	switch(callret) {
	case 0:                                         // PRQA S 4066
        // ok, done
        retvalue = true;
        break;
	case ETIMEDOUT:                                 // PRQA S 4066
        // timed out. Just return false
        retvalue = false;
		break ;
    case EAGAIN: // PRQA S 4011 # throws 
   		ACS_THROW(ThreadSafe::Exception("Too many locks active",callret)) ; // PRQA S 3081
	case EDEADLK: // a deadlock would never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE // PRQA S 4011 # throws 
		ACS_THROW(DeadLock("Deadlock detected",callret)) ; // PRQA S 3081
	case EINVAL:   // PRQA S 4011 # throws                                 
        // invalid mutex: error
		ACS_THROW(ThreadSafe::InvalidMutex("Invalid lock instance",callret)) ; // PRQA S 3081
	default:  // PRQA S 4612 # throws 
        // any other error: error
		ACS_THROW(ThreadSafe::Exception("Unexpected code while trying to get a timed read lock",callret)) ; // PRQA S 3081
    }
		
	return retvalue ;     
}
		




// ThreadSafe::Lock class

ThreadSafe::Lock::Lock(const ThreadSafe &m) : 
    _mutex(m), // PRQA S 2528
    _locked(false)
{
    try {
	    _mutex.lock() ;
	    // if here the lock executes correctly
	    _locked = true ;
    }
    catch(DeadLock &) {
        // will never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE
    }
}

// ThreadSafe::Lock class

ThreadSafe::Lock::Lock(const ThreadSafe &m, bool lock_now) : 
    _mutex(m), // PRQA S 2528
    _locked(false)
{
    if (lock_now) {
        try {
	        _mutex.lock() ;
	        // if here the lock executes correctly
	        _locked = true ;
        }
        catch(DeadLock &) {
            // will never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE
        }
    }
}

ThreadSafe::Lock::Lock(const Lock &l) : 
    _mutex(l._mutex),  // PRQA S 2528
    _locked(false)
{
    try {
	    _mutex.lock() ;
	    // if here the lock executes correctly
	    _locked = true ;
    }
    catch(DeadLock &) {
        // will never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE
    }
}

ThreadSafe::Lock::~Lock() ACS_NOEXCEPT
{
    // PRQA S 4631 L3
	try { // no exception out of this method
		if (_locked) {
			_mutex.unlock() ;
        }
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ; // we can safely use the exostream framework
	}
    // PRQA L:L3
}

bool ThreadSafe::Lock::unlock()
{
    if (true == _locked) {
        try {
	        _mutex.unlock() ;
	        // if here the lock executes correctly
	        _locked = false ;
            return true ;
        }
        catch(DeadLock &) {
            // will never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE
        }
    }
    
    return false  ;
}

bool ThreadSafe::Lock::lock()
{
    if (false == _locked) {
        try {
	        _mutex.lock() ;
	        // if here the lock executes correctly
	        _locked = true ;
            return true ;
        }
        catch(DeadLock &) {
            // will never occur because ThreadSafe's mutex are PTHREAD_MUTEX_RECURSIVE
        }
    }
    
    return false  ;
}



ThreadSafe::Lock::operator void *() const ACS_NOEXCEPT
{
	return reinterpret_cast<void *>(locked() ? 1 : 0) ; // PRQA S 3041, 3081,3383
}


bool ThreadSafe::Lock::operator()() const ACS_NOEXCEPT
{
	return locked() ;
}


bool ThreadSafe::Lock::locked() const ACS_NOEXCEPT // PRQA S 4120
{
	return _locked ;
}












// ThreadSafe::TryLock class

ThreadSafe::TryLock::TryLock(const ThreadSafe &m, unsigned long msectimeout, bool emitException) :
	_mutex(m),  // PRQA S 2528
	_locked(false)
{
	if (!lock(msectimeout))
		ACS_COND_THROW(emitException,Busy("Resource is busy")) ;  // PRQA S 3081
}

ThreadSafe::TryLock::~TryLock() ACS_NOEXCEPT
{
	try {
		unlock() ; // PRQA S 4631
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ; // PRQA S 4631
	}
}


ThreadSafe::TryLock::operator void *() const ACS_NOEXCEPT
{
	return reinterpret_cast<void *>(locked() ? 1 : 0) ; // PRQA S 3041, 3081,3383
}


bool ThreadSafe::TryLock::operator()() const ACS_NOEXCEPT
{
	return locked() ;
}


bool ThreadSafe::TryLock::locked() const ACS_NOEXCEPT // PRQA S 4120
{
	return _locked ;
}

void ThreadSafe::TryLock::unlock() // PRQA S 4211
{
	if (_locked) {
		_mutex.unlock() ;
    }
}


bool ThreadSafe::TryLock::lock(unsigned long msectimeout) // PRQA S 4020
{
	if (_locked) {
		return true ;
    }
	return _locked = _mutex.trylock(msectimeout) ;
}


Condition::Condition() : _condAttribute(), _cond(), _condMutex(), _eventCounter(0)
{
	int ret = ::pthread_condattr_init(&_condAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Condition attributes",ret)) ; // PRQA S 3081
    
	ret = ::pthread_cond_init(&_cond, &_condAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Condition",ret)) ; // PRQA S 3081
}

Condition::Condition(const Condition &t) : _condAttribute(), _cond(), _condMutex(t._condMutex), _eventCounter(0)
{
	int ret = ::pthread_condattr_init(&_condAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Condition attributes",ret)) ; // PRQA S 3081

	ret = ::pthread_cond_init(&_cond, &_condAttribute) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init Condition",ret)) ; // PRQA S 3081
}

Condition & Condition::operator=(const Condition &) // PRQA S 4072, 4250
{
    // no check for self assignement needed
	return *this ;
}

Condition::~Condition() ACS_NOEXCEPT 
{
	try { // PRQA S 4631 L4
		try {
			broadcast() ;
		}
		catch(exception &x) {
			ACS_LOG_NOTIFY_EX(x) ;
		}
		// attention, the exception handling is tricky
		int ret = ::pthread_cond_destroy(&_cond) ;
		ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_cond_destroy",ret)) ;  // PRQA S 3081
		ret =::pthread_condattr_destroy(&_condAttribute) ;
		ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_condattr_destroy",ret)) ;  // PRQA S 3081
	}
	catch(exception& e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
	} // PRQA L:L4
}

void Condition::wait() const
{
	int ret = ::pthread_cond_wait(&_cond, & _condMutex.getMutex() ) ;

	// Here the mutex is locked
	ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_cond_wait",ret)) ;  // PRQA S 3081
}

struct timespec& Condition::normalize(struct timespec& ts) ACS_NOEXCEPT
{
    if (ts.tv_nsec >= 1000000000)
    {
        // nanoseconds overflow
        ts.tv_sec += ts.tv_nsec/1000000000 ;
        ts.tv_nsec = ts.tv_nsec%1000000000;
    }


    return ts ;
}

struct timespec Condition::normalize(const struct timespec& ts) ACS_NOEXCEPT
{
    struct timespec rt = ts ;
    Condition::normalize(rt) ;

    return rt ;
}


bool Condition::timedwait(unsigned long msectimeout) const
{
    // Get the current time
    struct timespec tp; // PRQA S 4102
    int callret = ::clock_gettime (CLOCK_REALTIME, &tp);

    // In case of issues gathering the system time throw an exception
    ACS_COND_THROW(callret != 0, ThreadSafe::Exception("Unable to get system time", errno)) ; // PRQA S 3081

    // Add the delay to the current time
    unsigned long secs = msectimeout / 1000;                  // PRQA S 3084
    unsigned long nsecs = (msectimeout % 1000) * 1000000;     // PRQA S 3084

    tp.tv_sec += secs;                                  // PRQA S 3000, 3084
    tp.tv_nsec += nsecs;                                // PRQA S 3000, 3084

    return timedwait (tp);
}

bool Condition::timedwait(const struct timespec& tp) const
{
    bool result=true;
    struct timespec rt = normalize(tp) ;
   
	int ret = ::pthread_cond_timedwait(&_cond, & _condMutex.getMutex(), &rt) ;

    if (ETIMEDOUT == ret)
    {
        // Tis is a timeout. Shall return false and not throw any exception
        result = false;
    }
    else
    {
    	ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_cond_timedwait",ret)) ;  // PRQA S 3081
    }
    
    return result;
}



void Condition::signal() const // PRQA S 4214
{
	ThreadSafe::Lock lock (*this); // PRQA S 3050
	incrementEventCounter();
	int ret = ::pthread_cond_signal(&_cond);
	ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_cond_signal",ret)) ;  // PRQA S 3081
}


void Condition::broadcast() const // PRQA S 4214
{
	ThreadSafe::Lock lock (*this); // PRQA S 3050
	incrementEventCounter();
	int ret = ::pthread_cond_broadcast(&_cond);
	ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unexpected errno from pthread_cond_broadcast",ret)) ;  // PRQA S 3081
}


void Condition::incrementEventCounter() const // PRQA S 4214
{
	ThreadSafe::Lock lock (*this); // PRQA S 3050
	++_eventCounter;
}

unsigned int Condition::getEventCounter () const // PRQA S 4214
{
	ThreadSafe::Lock lock (*this); // PRQA S 3050
	return _eventCounter;
}

void Condition::resetEventCounter() const // PRQA S 4214
{
	ThreadSafe::Lock lock (*this); // PRQA S 3050
	_eventCounter = 0;
}


RWMutex::RWMutex() : _rwmutexAttribute(), _rwmutex()
{
    // Any of the following steps may fail. In the case throw an exception
    int ret = ::pthread_rwlockattr_init (&_rwmutexAttribute);
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init RWMutex attributes",ret)) ; // PRQA S 3081

    ret = ::pthread_rwlockattr_setpshared (&_rwmutexAttribute, PTHREAD_PROCESS_PRIVATE);
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to set Process Sharing attribute to RWMutex",ret)) ; // PRQA S 3081

    ret = ::pthread_rwlock_init (&_rwmutex, &_rwmutexAttribute);
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to create the RWMutex",ret)) ; // PRQA S 3081
}

RWMutex::RWMutex(const RWMutex &t) : _rwmutexAttribute(), _rwmutex()
{
    int ret = ::pthread_rwlockattr_init (&_rwmutexAttribute);
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to init RWMutex attributes",ret)) ; // PRQA S 3081
    
	int pshrd = 0 ;
	ret = ::pthread_rwlockattr_getpshared(&t._rwmutexAttribute, &pshrd) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to get Process Sharing attribute from RWMutex",ret)) ; // PRQA S 3081
    
	ret = ::pthread_rwlockattr_setpshared(&_rwmutexAttribute,pshrd) ;
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to set Process Sharing attribute to RWMutex",ret)) ; // PRQA S 3081

    ret = ::pthread_rwlock_init (&_rwmutex, &_rwmutexAttribute);
    ACS_COND_THROW(ret != 0, ThreadSafe::Exception("Unable to create the RWMutex",ret)) ; // PRQA S 3081
}

RWMutex & RWMutex::operator=(const RWMutex &) // PRQA S 4072, 4250
{
    // No self assignement check
	return *this ;
}

RWMutex::~RWMutex() ACS_NOEXCEPT 
{
	// attention, the exception handling is tricky
	int ret = ::pthread_rwlock_destroy(&_rwmutex) ;
    
	bool oldTrace = exStackTrace::allow() ; // store previous trace mode 
	try { // PRQA S 4631 L1
		if (!OnExitDetector::instance()->exiting()) {
			if (ret != 0) {
				exStackTrace::allow(true) ; // trace mode true
				ACS_THROW(ThreadSafe::Exception("Unexpected errno from pthread_mutex_destroy",ret)) ; // PRQA S 3081
			}
		}
	}
	catch(exception& e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
        if (ret !=0) { // reset old trace mode 
			exStackTrace::allow(oldTrace) ; // restore previous trace mode 
		}
	} // PRQA L:L1
	ret =::pthread_rwlockattr_destroy(&_rwmutexAttribute) ;
	try { // PRQA S 4631 L2
		if (!OnExitDetector::instance()->exiting()) {
			if (ret != 0) {
				exStackTrace::allow(true) ; // trace mode true
				ACS_THROW(ThreadSafe::Exception("Unexpected errno from pthread_mutexattr_destroy",ret)) ; // PRQA S 3081
			}
		}
	}
	catch(exception& e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
		if (ret !=0) { // reset old trace mode 
			exStackTrace::allow(oldTrace) ; // restore previous trace mode 
		}
	} // PRQA L:L2
}

void RWMutex::readLock() const                           // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ? 
		return ; // do not perform any lock ... just return
    }
	int ret = ::pthread_rwlock_rdlock(&_rwmutex) ;

	switch(ret) { // according with the kind of error 
	case 0:                                             // PRQA S 4066
        // ok
		return ;
	case EINVAL: // not a valid mutex // PRQA S 4011 # throws
        ACS_THROW(ThreadSafe::InvalidMutex("Invalid rwlock instance",ret)) ; // PRQA S 3081
	case EDEADLK: // a deadlock // PRQA S 4011 # throws 
		ACS_THROW(DeadLock("Deadlock detected",ret)) ; // PRQA S 3081
	case EAGAIN: // a deadlock // PRQA S 4011 # throws
		ACS_THROW(ThreadSafe::Exception("Too many locks active",ret)) ; // PRQA S 3081
	default: // any other // PRQA S 4612 # fall through
		ACS_THROW(ThreadSafe::Exception("Unexpected errno while locking",ret)) ; // PRQA S 3081
	}
}

void RWMutex::writeLock() const                           // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ? 
		return ; // do not perform any lock ... just return
    }
	int ret = ::pthread_rwlock_wrlock(&_rwmutex) ;

	switch(ret) { // according with the kind of error 
	case 0:                                             // PRQA S 4066
        // ok
		return ;
	case EINVAL: // not a valid mutex // PRQA S 4011 # throws 
        ACS_THROW(ThreadSafe::InvalidMutex("Invalid rwlock instance",ret)) ; // PRQA S 3081
	case EDEADLK: // a deadlock // PRQA S 4011 # throws
		ACS_THROW(DeadLock("Write lock already owned. Deadlock detected.",ret)) ; // PRQA S 3081
	default: // any other // PRQA S 4612 # fall through
		ACS_THROW(ThreadSafe::Exception("Unexpected errno while locking",ret)) ; // PRQA S 3081
	}
}

void RWMutex::unlock() const                         // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ? 
		return ; // do not perform any unlock ... just return
    }
	int ret = ::pthread_rwlock_unlock(&_rwmutex) ;
	switch(ret) {
	case 0:                                             // PRQA S 4066
        // ok, yeld to other threads if any
		::pthread_yield() ;
		return ;
	case EINVAL: // not a valid mutex // PRQA S 4011 # throws 
		ACS_THROW(ThreadSafe::InvalidMutex("Invalid rwlock instance",ret)) ; // PRQA S 3081
	case EPERM: // no permissions on this one // PRQA S 4011 # throws 
		ACS_THROW(ThreadSafe::NoPermission("Thread does not own any lock",ret)) ; // PRQA S 3081
	default: // any other // PRQA S 4612 # throws 
		ACS_THROW(ThreadSafe::Exception("Unexpected errno while unlocking",ret)) ; // PRQA S 3081
	}
}

bool RWMutex::baseTryLock (unsigned long msectimeout, bool rlock) const // PRQA S 4020, 4214
{
	if (OnExitDetector::instance()->exiting()) { // am i exiting ?
		return true ; // just return ... 
    }
    
    // Get the current time
    struct timespec tp; // PRQA S 4102
    int callret = ::clock_gettime (CLOCK_REALTIME, &tp);

    // In case of issues gathering the system time throw an exception
    ACS_COND_THROW(callret != 0, ThreadSafe::Exception("Unable to get system time", errno)) ; // PRQA S 3081

    // Add the delay to the current time
    unsigned long secs = msectimeout / 1000;                  // PRQA S 3084
    unsigned long nsecs = (msectimeout % 1000) * 1000000;     // PRQA S 3084

    tp.tv_sec += secs;                                  // PRQA S 3000, 3084
    tp.tv_nsec += nsecs;                                // PRQA S 3000, 3084

    Condition::normalize(tp) ;
    
    if (rlock)
    {
        // try to get the rdlock in the given time  
        callret = ::pthread_rwlock_timedrdlock(&_rwmutex, &tp);
    }
    else
    {
        // try to get the wrlock in the given time  
        callret = ::pthread_rwlock_timedwrlock(&_rwmutex, &tp);
    }
    
    // The return value will be changed if timeout is detected
    bool retvalue = true;

	switch(callret) {
	case 0:                                         // PRQA S 4066
        // ok, done
        retvalue = true;
        break;
	case ETIMEDOUT:                                 // PRQA S 4066
        // timed out. Just return false
        retvalue = false;
		break ;
    case EAGAIN: // PRQA S 4011 # throws 
   		ACS_THROW(ThreadSafe::Exception("Too many locks active",callret)) ; // PRQA S 3081
	case EDEADLK: // a deadlock // PRQA S 4011 # throws 
		ACS_THROW(DeadLock("Deadlock detected",callret)) ; // PRQA S 3081
	case EINVAL:      // PRQA S 4011 # throws                               
        // invalid mutex: error
		ACS_THROW(ThreadSafe::InvalidMutex("Invalid rwlock instance",callret)) ; // PRQA S 3081
	default:  // PRQA S 4612 # throws 
        // any other error: error
		ACS_THROW(ThreadSafe::Exception("Unexpected code while trying to get a timed read lock",callret)) ; // PRQA S 3081
    }
		
	return retvalue ; 
}

bool RWMutex::tryReadLock(unsigned long msectimeout) const // PRQA S 4020, 4214
{
    return baseTryLock (msectimeout, true); // read lock
}

bool RWMutex::tryWriteLock(unsigned long msectimeout) const // PRQA S 4020, 4214
{
    return baseTryLock (msectimeout, false); // write lock
}



// RWMutex::BaseRWLock class

RWMutex::BaseRWLock::BaseRWLock(const RWMutex &m, bool read) : 
    _mutex(m), // PRQA S 2528
    _locked(false),
    _read(read)
{
    try {
        if (_read)   
        {   
    	    _mutex.readLock() ;
        }
        else
        {
            _mutex.writeLock() ;
        }
        
	    // if here the lock executes correctly
	    _locked = true ;
    }
    catch(DeadLock &x) {
        ; // no warning it is allowed
	}
}

RWMutex::BaseRWLock::BaseRWLock(const BaseRWLock &l) : 
    _mutex(l._mutex),  // PRQA S 2528
    _locked(false),
    _read(l._read)
{
    try {
        if (_read) 
        {     
    	    _mutex.readLock() ;
        }
        else
        {
            _mutex.writeLock() ;
        }
        
	    // if here the lock executes correctly
	    _locked = true ;
    }
    catch(DeadLock &x) {
        ; // no warning it is allowed
	}
}

RWMutex::BaseRWLock::~BaseRWLock() ACS_NOEXCEPT
{
    // PRQA S 4631 L3
	try { // no exception out of this method
		if (_locked) {
			_mutex.unlock() ;
        }
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ;
	}
    // PRQA L:L3
}


// RWMutex::ReadLock class

RWMutex::ReadLock::ReadLock(const RWMutex &m) : 
    BaseRWLock(m, true)
{
}

RWMutex::ReadLock::ReadLock(const ReadLock &l) : 
    BaseRWLock (l)
{
}

RWMutex::ReadLock::~ReadLock() ACS_NOEXCEPT
{
}

// RWMutex::WriteLock class

RWMutex::WriteLock::WriteLock(const RWMutex &m) : 
    BaseRWLock(m, false)
{
}

RWMutex::WriteLock::WriteLock(const WriteLock &l) : 
    BaseRWLock (l)
{
}

RWMutex::WriteLock::~WriteLock() ACS_NOEXCEPT
{
}


// RWMutex::TryLockBase class

RWMutex::TryLockBase::TryLockBase(const RWMutex &m, unsigned long msectimeout, bool emitException, bool rlock) :
	_mutex(m),  // PRQA S 2528
	_locked(false)
{
	if (!lock(msectimeout, rlock))
		ACS_COND_THROW(emitException,ThreadSafe::Busy("Resource is busy")) ;  // PRQA S 3081
}

RWMutex::TryLockBase::~TryLockBase() ACS_NOEXCEPT
{
	try {
		unlock() ; // PRQA S 4631
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ; // PRQA S 4631
	}
}


RWMutex::TryLockBase::operator void *() const 
{
	return reinterpret_cast<void *>(locked() ? 1 : 0) ; // PRQA S 3041, 3049, 3081,3383
}


bool RWMutex::TryLockBase::operator()() const 
{
	return locked() ;
}


bool RWMutex::TryLockBase::locked() const  // PRQA S 4120
{
	return _locked ;
}

void RWMutex::TryLockBase::unlock() // PRQA S 4211
{
	if (_locked) {
		_mutex.unlock() ;
    }
}


bool RWMutex::TryLockBase::lock(unsigned long msectimeout, bool rlock) // PRQA S 4020
{
	if (_locked) {
		return true ;
    }
    if (rlock)
    {
    	_locked = _mutex.tryReadLock(msectimeout) ;
    }
    else
    {
    	_locked = _mutex.tryWriteLock(msectimeout) ;
    }
	return _locked;
}

// RWMutex::TryLockRead class
RWMutex::TryLockRead::TryLockRead (const RWMutex &m, unsigned long msectimeout, bool emitException) :
    RWMutex::TryLockBase (m, msectimeout, emitException, true)
{
    // Just construct using the base class
}    

RWMutex::TryLockRead::~TryLockRead() ACS_NOEXCEPT
{}

// RWMutex::TryLockRead class
RWMutex::TryLockWrite::TryLockWrite (const RWMutex &m, unsigned long msectimeout, bool emitException) :
    RWMutex::TryLockBase (m, msectimeout, emitException, false)
{
    // Just construct using the base class
}    

RWMutex::TryLockWrite::~TryLockWrite() ACS_NOEXCEPT
{}

_ACS_END_NAMESPACE
