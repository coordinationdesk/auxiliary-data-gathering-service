/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Thread Library $

	$Id$

	$Author$

	$Log$
	Revision 5.13  2017/09/22 11:41:41  marpas
	QA warnings fixed
	
	Revision 5.12  2017/03/03 09:26:09  marfav
	S3MPM-874
	Conditions do not allow the use of external mutex anymore and provide its own embedded mutex
	Conditions embed also a counter of the broadcast/signal events
	
	Revision 5.11  2016/06/13 12:50:20  marpas
	removing qa warnings
	
	Revision 5.10  2016/03/29 12:36:46  marfav
	S1PDGS-31708 allow re init of mutex if needed like after a fork
	
	Revision 5.9  2015/06/23 15:52:02  marpas
	doc updated
	
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
	
	Revision 5.4  2014/03/20 15:30:09  marfav
	Absolute time interface for condition timedwait is now private
	
	Revision 5.3  2014/03/19 15:58:54  marfav
	Condition timedwait interface added. It now returns bool
	
	Revision 5.2  2013/12/16 18:31:18  marpas
	qa warning
	
	Revision 5.1  2013/06/05 15:57:39  marpas
	new debugging and logging macros adopted
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.11  2013/04/29 15:39:00  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.10  2012/11/15 11:00:54  marpas
	qa rules enforced
	
	Revision 4.9  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.8  2012/10/29 14:29:52  marpas
	robustness improved
	
	Revision 4.7  2012/06/25 17:42:05  marpas
	qacpp instrumentation
	
	Revision 4.6  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.4  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.3  2012/02/20 15:04:03  marpas
	refactoring
	
	Revision 4.2  2012/02/07 10:10:23  marpas
	convenience typedefs added
	
	Revision 4.1  2012/01/31 14:13:04  marpas
	getting rid of boost and compilation warning reduced
	
	Revision 4.0  2011/10/18 11:21:58  marpas
	new statistics
	
	Revision 3.3  2011/01/26 12:07:54  marpas
	ThreadSafe::Condition::timedwait can return ThreadSafe::Timeout exception (which has been defined)
	
	Revision 3.2  2011/01/25 16:06:19  marpas
	ThreadSafe::Condition temptative implementation
	
	Revision 3.1  2011/01/24 18:16:27  marpas
	Mutex class removed (was not enough safe to be used .. and evidently nobody used it)
	
	Revision 3.0  2009/10/08 10:48:08  marpas
	Statistics class added
	rules application improved
	
	Revision 2.4  2009/06/26 14:10:44  marpas
	robustness improved on mutex when process exits
	
	Revision 2.3  2009/06/26 10:41:54  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2008/05/06 16:15:23  marpas
	locking during the whole exLogStream ctor was needed (APF-218)
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.10  2005/09/20 17:31:24  marpas
	thread safety improved, test accuracy too
	
	Revision 1.9  2004/04/28 10:42:05  marpas
	Locking policy changed, only recursive thread
	
	Revision 1.8  2004/04/28 10:11:22  marpas
	Lock policy improved
	ThreadSafe::SignalAction implemented
	
	Revision 1.7  2004/04/14 14:09:26  marpas
	ThreadSafe::unlock yields other threads
	
	Revision 1.6  2004/04/14 13:51:42  marpas
	DEBUG removed --- loop
	
	Revision 1.5  2004/04/14 13:40:35  envisat
	ThreadSafe::Lock debug added
	
	Revision 1.4  2004/04/07 14:31:22  marpas
	copy ctor and assignement operator implemented
	
	Revision 1.3  2004/04/02 15:54:44  marpas
	Legacy code compilation fixed
	
	Revision 1.2  2004/04/02 14:15:14  marpas
	ThreadSafe class introduced and used inside
	
	Revision 1.1  2004/04/02 07:51:14  marpas
	class rearrangement for ThreadSafe and Mutex
	
	
*/


#ifndef _ThreadSafe_H_
#define _ThreadSafe_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <pthread.h>
#include <sys/time.h>

_ACS_BEGIN_NAMESPACE(acs)


/*! \class ThreadSafe

	\brief Base class for thread safety
	
	This class is the base class for all thread safe classes. 
	\see ThreadSafe.h for some specialized implementation 
    
    \note this class can be safely copy constructed and assigned: no-op in both cases
*/

class ThreadSafe { // PRQA S 2109
public:
	/** \brief declare ThreadSafe::Exception - base class for all ThreadSafe class exceptions */
	exDECLARE_EXCEPTION(Exception,exException) ;    // PRQA S 2131, 2502
	/** \brief declare ThreadSafe::InvalidMutex exception */
	exDECLARE_EXCEPTION(InvalidMutex,Exception) ;   // PRQA S 2131, 2153, 2502
	/** \brief declare ThreadSafe::NoPermission exception */
	exDECLARE_EXCEPTION(NoPermission,Exception) ;   // PRQA S 2131, 2153, 2502
	/** \brief declare ThreadSafe::Busy exception */
	exDECLARE_EXCEPTION(Busy,Exception) ;           // PRQA S 2131, 2153, 2502
	/** \brief declare ThreadSafe::Timeout exception */
	exDECLARE_EXCEPTION(Timeout,Exception) ;        // PRQA S 2131, 2153, 2502

	/**
	 *\brief A locking guard over a ThreadSafe class
	 *
	 */
	class Lock { // PRQA S 2109
	public:
		Lock &operator=(const Lock &) = delete ;
        Lock() = delete ;
		/** 
		 *\brief ctor, locks the given ThreadSafe instance
		 *
		 */
		explicit Lock (const ThreadSafe &) ;

		/** 
		 *\brief alternate ctor, gets the given ThreadSafe instance and locks it accordingly to the lock_now parameter
		 *
         * \param lock_now if false do not acquire the lock over the mutex:
		 \see ThreadSafe::Lock::lock()
		 \see ThreadSafe::Lock::unlock()
		 */
		Lock (const ThreadSafe &, bool lock_now) ;

		/** 
		 *\brief copy ctor
		 *
		 */
		Lock(const Lock &) ;

		/** 
		 *\brief dtor, unlocks the given ThreadSafe instance
		 *
		 */
		~Lock() noexcept ;
        
		/** 
		 \brief unlock manually, the dtor will no longer try to unlock
         \note this method is non const even if the mutex has been acquired by const ref.
         This is a design choice to force the Lock user to act over a non const Lock

		 \returns true if the lock has been released upon this call - it was previoulsy locked.
		 */
		bool unlock() ;

		/** 
		 \brief lock manually, if the class is already locked it does nothing
         \note this method is non const even if the mutex has been acquired by const ref.
         This is a design choice to force the Lock user to act over a non const Lock

		 \returns true if the lock is acquired, false if not 

		 */
		bool lock() ;

        /**
		 \brief a cast operator to see if the lock was acquired, 
		 \returns a bool useful in boolean expression

		 \see ThreadSafe::Lock::locked() const
		 \see ThreadSafe::Lock::operator()() const
		 */
		operator bool () const noexcept ; // NOSONAR -- if (lock) ...

		/**
		 \brief an operator to known if the lock was acquired.

		 \see ThreadSafe::Lock::locked() const
		 */
		bool operator()() const noexcept ; 

		/**
		 \brief ask if the lock was sucessfully acquired.

		 \see ThreadSafe::Lock::operator()() const
		 \see ThreadSafe::Lock::operator bool () const
		 */
		bool locked() const noexcept ;

	private:
		const ThreadSafe &_mutex ;
		bool _locked ;
	} ;


	/**
	 *\brief A try-locking guard over a ThreadSafe class
	 *
	 */
	class TryLock { // PRQA S 2109
	public:
		TryLock(const TryLock &) = delete ;
		TryLock &operator=(const TryLock &) = delete ;
		TryLock() = delete ;

		/** 
		 \brief ctor, locks the given ThreadSafe instance

		 The ctor will try to acquire the lock, using the given timeout.
		 If the emitException flag was true and the lock was not acquired, a Busy exception is thrown.
		 Note that on a recursive ThreadSafe instance, if the running thread owns the lock, it is still
		 possible for him to acquire new locks on it.
		 The resource will be locked from other threads only when all the recursive locks have been 
		 unlocked (see pthread_mutexattr_settype(3) man page)

		 */
		explicit TryLock (const ThreadSafe &, unsigned long msectimeout=0, bool emitException=false) ;
		/** 
		 *\brief dtor, unlocks the given ThreadSafe instance
		 *
		 */
		~TryLock() noexcept;

		/**
		 \brief a cast operator to see if the lock was acquired, 
		 \returns a bool useful in boolean expression

		 \see ThreadSafe::TryLock::locked() const
		 \see ThreadSafe::TryLock::operator()() const
		 */
		operator bool () const noexcept ; // NOSONAR -- if (trylock) ...

		/**
		 \brief an operator to known if the lock was acquired.

		 \see ThreadSafe::TryLock::locked() const
		 */
		bool operator()() const noexcept ;

		/**
		 \brief ask if the lock was sucessfully acquired.

		 \see ThreadSafe::TryLock::operator()() const
		 \see ThreadSafe::TryLock::operator void* () const
		 */
		bool locked() const noexcept ;

		/** 
		 \brief unlock manually, the dtor will no longer try to unlock
         \note this method is non const even if the mutex has been acquired by const ref.
         This is a design choice to force the TryLock user to act over a non const TryLock
		 */
		void unlock() const ;

		/** 
		 \brief lock manually, if the class is already locked it does nothing

		 \param msectimeout instructs the method to try until the timeout expires or the lock is acquired
		 \returns true if the lock is acquired, false if not 

		 */
		bool lock(unsigned long msectimeout=0) ;

	private:
		const ThreadSafe &_mutex ;
		bool _locked ;
	} ;


public: // NOSONAR - for clarity
	/**
		\brief ctor
		The mutex is recursive and will allow multiple locks from the same thread. The mutex is released when all the lock are released. 

		Note that on a recursive ThreadSafe instance, if the running thread owns the lock, it is still
		possible for him to acquire new locks on it.
		The resource will be locked from other threads only when all the recursive locks have been 
		unlocked (see pthread_mutexattr_settype(3) man page)
	 */
	ThreadSafe() ;
	/*! do not copy anything, is implemented only to allow assignement on derived classes 
	  */
	ThreadSafe(const ThreadSafe &) ;
	/*! do not copy anything, is implemented only to allow assignement on derived classes 
	  */
	ThreadSafe &operator=(const ThreadSafe &) ;
	virtual ~ThreadSafe() noexcept ; 

	/**
	 \brief lock the class

	 The method is const to allow any inherited class to be locked even in const state.
	 */
	void lock() const ;

	/**
	 \brief tries to lock the class with a timeout

	 The method is const to allow any inherited class to be locked even in const state.
	 The method will attempt to acquire the lock until the given timeout expires. 
	 If the timeout is 0 this method calls ThreadSafe::trylock() with no parameters defined below 
	 \note after the trylock executes and locks, user MUST call unlock to avoid asymmetrical status on the mutex. 
     see pthread_mutex_lock and ERRORCHECK mutexes

	 \param msectimeout instructs the method to try until the timeout expires or the lock is acquired
	 \returns true if the lock is acquired, false otherwise

	 */
	bool trylock(unsigned long msectimeout) const ;

	/**
	 \brief tries to lock the class

	 The method is const to allow any inherited class to be locked even in const state.
	 The method will attempt to acquire the lock.
	 \note after the trylock executes and locks, user MUST call unlock to avoid asymmetrical status on the mutex. see pthread_mutex_lock and ERRORCHECK mutexes

	 \returns true if the lock is acquired, false otherwise

	 */
	bool trylock() const ;

	/**
	 \brief unlock the class

	 The method is const to allow any inherited class to be locked even in const state.
	 As consequence of unlocking the mutex, a pthread_yield is issued explicitely in order
	 to leave other threads trying to lock the resource to get the control over it.
     \param noExIfNotOwner if true, in case EPERM is raised, the funztion does not raise an exception but returns false.
     \returns true if success, false if noExIfNotOwner == true and not owner, exception in all other cases

	 */
	bool unlock(bool noExIfNotOwner = false) const ; 

	/**
         \brief resets the mutex to the initial state

	 This is useful when after a fork the mutex must be re-initalized in the child process to avoid deadlocks
	*/

	void reInit (bool throwIfFailures = true) const;

private:

// the Condition class needs to access to the mutex
friend class Condition; // PRQA S 2107 

	::pthread_mutex_t& getMutex() const { return _mutex; } // PRQA S 4623 # to allow locking of const instances

private: // NOSONAR - for clarity

	::pthread_mutexattr_t _mutexAttribute ;
	mutable ::pthread_mutex_t  _mutex ;

} ;

/*! just a convenience typedef 
*/
using Mutex = ThreadSafe ; // PRQA S 2401 # alias wanted
using Lock = ThreadSafe::Lock ;
using TryLock = ThreadSafe::TryLock ;


/*! \class Condition

\brief Base class for thread conditions

	The condition helps in creating synchronization points.
	It embeds a Mutex that shall be used to protect the critial areas
	and the shared resources that are involved.

	For more details on conditions and mutex see https://en.wikipedia.org/wiki/Monitor_(synchronization)
	
	
	\code

	// ...

	Condition myCondition;
	
	// Critical area scope
	{
		ThreadSafe::Lock lockguard ( myCondition )
		while ( ! sharedResource.ready() && ! time_to_exit )
		{
			myCondition.timedwait (3000); // 3000 msecs
		}
		doSometingh ( sharedResource );
	}
	
	// ...
	
	\endcode

	The condition embeds also a basic data structure counting the number of signal() and broadcast() triggered sevents
	This structure can be used to monitor the occurrence of external events even outside the waiting calls

	\code

	// ...

	Condition myCondition;

	while ( ! time_to_exit )
	{
	
		// wait for some external event to occurr
		{
			ThreadSafe::Lock lockguard ( myCondition )
			while ( myCondition.getEventCounter() == 0 && ! time_to_exit )
			{
				myCondition.timedwait (3000); // 3000 msecs
			}
			// must reset the counter here, the condition is still locked
			myCondition.resetEventCounter();
		}

		// Process external events here
		doProcess();

		// New async events during the process phase will be collected by the condition event counter
	}
	
	// ...
	
	\endcode



\see ThreadSafe.h for some specialized implementation 
*/

class Condition {

public:
	/**
		\brief ctor
		pthread_condition wrapper class
	*/
	Condition() ;
	Condition(const Condition &) ;
	Condition &operator=(const Condition &) ;
	virtual ~Condition() noexcept ;

	/**
		\brief Waits a signal on the condition until interrupted by a signal

		This function stops the current thread and puts it into the waiting queue on the condition.
		It will be woken up by a signal or a broadcast on the condition.
		The internal mutex shall be locked before invoking this function and it will release it before entering the waiting.
		The mutex will be locked againg before returning.
		It throws exceptions in case of issues in the pthread condition internal management.

		\returns true if the signal has been received and false in case of timeouts
	*/
	void wait() const; // PRQA S 2504

	/**
		\brief Waits a signal on the condition up to a given timeout

		This function stops the current thread and puts it into the waiting queue on the condition.
		It will be woken up by a signal, a timeout or a broadcast on the condition.
		The internal mutex shall be locked before invoking this function and it will release it before entering the waiting.
		The mutex will be locked againg before returning.
		It throws exceptions in case of issues in the pthread condition internal management.

		\param msectimeout the max amount to wait in milliseconds for the signal
		\returns true if the signal has been received and false in case of timeouts
	*/
	bool timedwait(unsigned long msectimeout) const;

	/**
		\brief Signal a single waiter

		This function sends a signal to ONE of the actors waiting on the condition. 
		The other entities will continue to wait.
		There is no predictible rule to identify who will receive the signal in the waiting queue.
	*/
	void signal() const ; // PRQA S 2502

	/**
		\brief Wake up all the waiters

		This function sends a signal to ALL of the actors waiting on the condition. 
		There is no rule to identify the order of the exit from the waiting queue.
	*/
	void broadcast() const ;

	/**
		\brief Wake up all the waiters

		This function sends a signal to ALL of the actors waiting on the condition. 
		There is no rule to identify the order of the exit from the waiting queue.
		This signature allows the user to choose how to handle the internal counter and the internal timer.
	*/
	void broadcast(const bool& increase_counter, const bool& reset_counter, const bool& reset_timer) const ;

	/*! a convenience of pthread_cond_signal(POSIX) implementation in boost thread nomenclature 
	*/
	void notify_one() const { signal() ; } // PRQA S 4214

	/*! a convenience of pthread_cond_broadcast(POSIX) implementation boost thread nomenclature 
	*/
	void notify_all() const { broadcast() ; } // PRQA S 4214


	/**
		\brief Cast the Condition to its embedded Mutex object
	*/
	const Mutex& getMutex() const { return _condMutex; }
	/**
		\brief Returns the embedded Mutex object
	*/
	operator const Mutex& () const { return _condMutex; } // NOSONAR - implicit cast a Condition to its inner Mutex

	/**
		\brief increment the internal envent counter by 1 
	*/
	void incrementEventCounter() const;

	/**
		\brief returns the actual value of the internal event counter
	*/
	unsigned int getEventCounter () const;
	/**
		\brief returns the timer (as ms) from the last timer-reset (which occurs automatically at each signal/broadcast)
	*/
	uint64_t getTimer_ms () const;
	/**
		\brief returns if the last timer-reset (which occurs automatically at each signal/broadcast) occurred more than "ms" ago
	*/
	bool timerIsOlder_ms (const int64_t& value_ms) const;
	/**
		\brief resets the internal event counter to 0
	*/
	void resetEventCounter() const;
	/**
		\brief force timer reset (this occurs automatically at each signal/broadcast)
	*/
	void resetTimer() const;

	static struct timespec& normalize(struct timespec&) noexcept;
	static struct timespec normalize(const struct timespec&) noexcept;

private:
	/**
	\brief Waits a signal on the condition up to a given absolute time

	Absolute time apart. It implements the same interface of bool timedwait(ThreadSafe & mutex, unsigned long msectimeout)

	\see bool timedwait(unsigned long msectimeout)
	*/
	bool timedwait(const struct timespec&)  const ;

	// private attributes
	::pthread_condattr_t _condAttribute = {} ;
	mutable ::pthread_cond_t _cond = {} ;

	ThreadSafe _condMutex = {} ;

	// event counter
	mutable unsigned int _eventCounter = 0;
	
	// timer -- NOT USED INTERNALLY
	// just to allow the user to get the time elapsed from last signal/broadcast (it is automatically reset).
	mutable struct timeval _timerFromLastSignal = {} ; 
} ;


class RWMutex // PRQA S 2109
{

public:

    // Inner Classes

    class BaseRWLock{ // PRQA S 2109
    public:
        BaseRWLock &operator=(const BaseRWLock &) = delete ;
        BaseRWLock() = delete ; 
        /** 
        *\brief ctor, locks the given ThreadSafe instance
        *
        */
        explicit BaseRWLock (const RWMutex &, bool read=true) ;

        /** 
        *\brief copy ctor
        *
        */
        BaseRWLock(const BaseRWLock &) ;

        /** 
        *\brief dtor, unlocks the given ThreadSafe instance
        *
        */
        virtual ~BaseRWLock() noexcept ;

    private:
        const RWMutex &_mutex ;
        bool _locked ;
        bool _read;
    };


public: // NOSONAR -for clarity
    /**
     *\brief A locking guard (ReadLock) over a RWMutex class
     *
     */
    class ReadLock : public BaseRWLock { // PRQA S 2109, 2153
    public:
        ReadLock &operator=(const ReadLock &) = delete ;
        ReadLock() = delete ;
        /** 
        *\brief ctor, read locks the given RWMutex instance
        */
        explicit ReadLock (const RWMutex &) ;

        /** 
        *\brief copy ctor
        */
        ReadLock(const ReadLock &) = default ;

        /** 
        *\brief dtor, unlocks the given RWMutex instance
        */
        ~ReadLock() override = default ;
    } ;


    /**
     *\brief A locking guard (WriteLock) over a RWMutex class
     *
     */
    class WriteLock : public BaseRWLock { // PRQA S 2109, 2153
    public:
        WriteLock &operator=(const WriteLock &) = delete ;
        WriteLock() = delete ;
        /** 
        *\brief ctor, write locks the given RWMutex instance
        *
        */
        explicit WriteLock (const RWMutex &) ;

        /** 
        *\brief copy ctor
        *
        */
        WriteLock(const WriteLock &) = default ;

        /** 
        *\brief dtor, unlocks the given RWMutex instance
        *
        */
        ~WriteLock() override = default ;
    } ;


    class TryLockBase { // PRQA S 2109
    public:
	    TryLockBase(const TryLockBase &) = delete ;
	    TryLockBase &operator=(const TryLockBase &) = delete ;
	    TryLockBase() = delete ;
	    /** 
	     \brief ctor, locks the given RWMutex instance

	     The ctor will try to acquire the lock, in read or write mode according to the parameter rlock 
         using the given timeout.
	     If the emitException flag was true and the lock was not acquired, a Busy exception is thrown.
	     Note that on a recursive ThreadSafe instance, if the running thread owns the lock, it is still
	     possible for him to acquire new locks on it.
	     */

	    explicit TryLockBase (const RWMutex &, unsigned long msectimeout, bool emitException, bool rlock) ;

	    /** 
	     *\brief dtor, unlocks the given RWMutex instance
	     *
	     */
	     virtual ~TryLockBase() noexcept;

	    /**
	     \brief a cast operator to see if the lock was acquired, 
         \returns a bool useful in boolean expression

	     \see TryLockBase::locked() const
	     \see TryLockBase::operator()() const
	     */
	    operator bool () const ; // NOSONAR -- if (trylockbase) 

	    /**
	     \brief an operator to known if the lock was acquired.

	     \see TryLockBase::locked() const
	     */
	    bool operator()() const ;

	    /**
	     \brief ask if the lock was sucessfully acquired.

	     \see TryLockBase::operator()() const
	     \see TryLockBase::operator bool () const
	     */
	    bool locked() const ;


    protected:
	    /** 
	     \param msectimeout instructs the method to try until the timeout expires or the lock is acquired
         \param rlock if true acquire a read-lock else is a write-lock
	     \returns true if the lock is acquired, false if not 
	     */
	    bool lock(unsigned long msectimeout, bool rlock);

   	    /** 
	     \brief unlock the RWMutex
	     */
	    void unlock() const ;

    private: // NOSONAR
	    const RWMutex &_mutex ;
	    bool _locked ;
    } ;

    class TryLockRead : public TryLockBase // PRQA S 2153
    {
    public:
	    explicit TryLockRead (const RWMutex &, unsigned long msectimeout = 0, bool emitException = false) ;
	    ~TryLockRead() override = default ;
    };

    class TryLockWrite : public TryLockBase // PRQA S 2153
    {
    public:
	    explicit TryLockWrite (const RWMutex &, unsigned long msectimeout = 0, bool emitException = false) ;
	    ~TryLockWrite() override = default ;
    };

public: // NOSONAR - for clarity
	/**
		\brief ctor
		The RWMutex is not recursive.
        Multiple threads can lock it in Read mode, while only one can lock in in Write mode.
        Read and Write locks are mutuallly exclusive.
        If a thread owns a RWLock in read or write mode, it cannot lock it again neither for reading
        nor for writing. Deadlocks can occurr.
	 */
	RWMutex() ;
	/*! do not copy anything, is implemented only to allow assignement on derived classes 
	  */
	RWMutex(const RWMutex &) ;
	/*! do not copy anything, is implemented only to allow assignement on derived classes 
	  */
	RWMutex &operator=(const RWMutex &) ;
	virtual ~RWMutex() noexcept ; 

	/**
	 \brief read lock the class

	 The method is const to allow any inherited class to be locked even in const state.
	 */
	void readLock() const ;

	/**
	 \brief write lock the class

	 The method is const to allow any inherited class to be locked even in const state.
	 */
	void writeLock() const ;

	/**
	 \brief tries to lock the class for reading

	 The method is const to allow any inherited class to be locked even in const state.
	 The method will attempt to acquire the lock no more than 10 times until the given timeout
	 expires. The interval between the attempts is then computed to be no longer than timeout/10 [msec].
	 Only a single attempt will occur if the timeout is 0.
	 \note, after the trylock executes and locks, user MUST call unlock

	 \param msectimeout instructs the method to try until the timeout expires or the lock is acquired
	 \returns true if the lock is acquired, false otherwise

	 */
	bool tryReadLock(unsigned long msectimeout=0) const ;

	/**
	 \brief tries to lock the class for writing

	 The method is const to allow any inherited class to be locked even in const state.
	 The method will attempt to acquire the lock no more than 10 times until the given timeout
	 expires. The interval between the attempts is then computed to be no longer than timeout/10 [msec].
	 Only a single attempt will occur if the timeout is 0.
	 \note, after the trylock executes and locks, user MUST call unlock

	 \param msectimeout instructs the method to try until the timeout expires or the lock is acquired
	 \returns true if the lock is acquired, false otherwise

	 */
	bool tryWriteLock(unsigned long msectimeout=0) const ;

	/**
	 \brief release read/write lock 

	 The method is const to allow any inherited class to be locked even in const state.
	 As consequence of unlocking the mutex, a pthread_yield is issued explicitely in order
	 to leave other threads trying to lock the resource to get the control over it.

	 */
	void unlock() const ; 
  
private:

    bool baseTryLock (unsigned long msectimeout, bool rlock) const;

private: // NOSONAR - for clarity

	::pthread_rwlockattr_t _rwmutexAttribute ;
	mutable ::pthread_rwlock_t  _rwmutex ;
} ;

_ACS_END_NAMESPACE

#endif // _ThreadSafe_H_
