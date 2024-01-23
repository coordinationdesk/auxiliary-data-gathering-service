/*

  Copyright 1995-2022, Exprivia SpA.
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
  Revision 5.9  2017/10/25 13:57:04  marpas
  range functions added

  Revision 5.8  2017/03/03 09:27:43  marfav
  Aligned to libException Rev_5_3 Condition interface change

  Revision 5.7  2016/06/27 12:23:29  davide.tiriticco
  Implemented setCpuAffinity() and getCpuAffinity()

  Revision 5.6  2016/04/14 09:54:22  marpas
  Thread does not inherits from ThreadSafe anymore - instead own a mutex variable and offer a cast operator to a
  reference to if in order to lock anybody would inherit from Thread.
  a new template static method added to control execution of multiple threads and, in case one of them should have an
  exception - call a predicate over them.

  Revision 5.5  2013/11/21 10:17:26  marpas
  enumerative cases name changed

  Revision 5.4  2013/11/21 10:00:14  marfav
  Added support to externally provided condition to broadcast on exit
  Running assures to return false when the condition(s) has been broadcasted

  Revision 5.3  2013/10/25 10:43:35  marpas
  doc & warnings

  Revision 5.2  2013/07/09 11:27:44  marpas
  yield, timeoutOrCancel and nanosleep method moved from Thread to StopController

  Revision 5.1  2013/06/10 16:11:41  marpas
  deprecated macros substituted

  Revision 5.0  2013/06/10 16:08:26  marpas
  adopting libException 5.x standards

  Revision 2.17  2013/04/03 13:38:41  marpas
  run does not longer gets a void *

  Revision 2.16  2013/04/03 12:32:09  marpas
  run method gets a void *

  Revision 2.15  2013/04/02 12:34:18  marpas
  qa warning fixed

  Revision 2.14  2013/02/22 10:28:55  marpas
  coding best practices applied

  Revision 2.13  2012/12/02 12:43:12  marpas
  qa warning

  Revision 2.12  2012/11/27 13:14:40  marpas
  qa rules

  Revision 2.11  2012/11/27 11:07:17  marpas
  qa rules

  Revision 2.10  2012/10/03 16:18:54  marfav
  Added support to ThreadSafe::Condition to signal thread completion to external entities

  Revision 2.9  2012/09/27 16:41:38  marpas
  Quality rules implementation
  wrong catch in dtor fixed

  Revision 2.8  2012/02/09 14:19:20  marpas
  refactoring

  Revision 2.7  2011/03/14 09:38:43  marpas
  ThreadSafe is a *virtual* base class for Thread

  Revision 2.6  2011/03/14 09:28:36  marpas
  Thread::stop, Thread::lazyStop and Thread::halt have an additional parameter (defaulted to true)to ask to wait for the
  join.
  Is a sort of asynchronous request, setting the HaltLevel and returning immediately.
  This allow a calling thread to perform some other tasks and periodically check for the sub-thread status.

  Revision 2.5  2011/03/11 09:08:56  marpas
  robustness improved
  Thread::join now returns a boolean (see documentation - make doc)
  test changed accordingly

  Revision 2.4  2009/06/26 10:50:41  marpas
  some classes do not throw any exception from their dtors

  Revision 2.3  2009/03/12 09:56:53  marpas
  rules violations fixed and or justified

  Revision 2.2  2007/11/22 11:34:44  lucio.pulvirenti
  Stop Control feature added

  Revision 2.1  2006/03/16 10:49:50  marpas

  CVS ----------------------------------------------------------------------

  Revision 2.0  2006/02/28 08:27:50  marpas
  Exception 2.1 I/F adopted

  Revision 1.23  2005/05/30 20:27:09  marpas
  new method timeoutOrCancel, see documentation

  Revision 1.22  2005/05/30 15:18:17  marpas
  documentation comments changed

  Revision 1.21  2004/09/10 17:19:18  paoscu
  pthread::join(unsigned long msectimeout = 0) implemented and tested

  Revision 1.20  2004/09/10 13:32:53  danalt
  join method added

  Revision 1.19  2004/05/04 14:16:34  paoscu
  minor change

  Revision 1.18  2004/05/04 14:15:36  paoscu
  thread start speedup

  Revision 1.17  2004/04/13 14:08:39  paoscu
  ThreadSafe is virtually inherited

  Revision 1.16  2004/04/13 13:21:09  paoscu
  ACE dependencies removed

  Revision 1.15  2004/04/07 16:08:51  paoscu
  copy ctor and assignment operator declared protected and given with fake implementation

  Revision 1.14  2003/10/10 16:02:34  paoscu
  static maskedSignals method added

  Revision 1.13  2003/10/10 15:37:46  paoscu
  Example improved

  Revision 1.12  2003/07/28 22:19:46  paoscu
  deleteRunException private method changed into setRunException

  Revision 1.11  2003/07/17 14:15:49  paoscu
  runException method changed i/f too see doxygen documentation

  Revision 1.10  2003/07/17 10:38:39  paoscu
  thread name implemented

  Revision 1.9  2003/07/11 12:55:22  paoscu
  thread exceptions management

  Revision 1.8  2003/04/30 10:00:48  paoscu
  using namespace std was removed from includes

  Revision 1.7  2003/04/15 17:31:48  paoscu
  GCC3.2

  Revision 1.6  2003/03/18 13:01:59  marpas
  sync bug again

  Revision 1.5  2003/03/11 17:41:55  marpas
  New cancellation policy

  Revision 1.4  2003/02/26 10:37:10  marpas
  threadIdUnguarded now is private

  Revision 1.3  2003/02/24 16:01:15  paoscu
  Sync problem at thread startup fixed

  Revision 1.2  2003/01/09 10:54:09  marpas
  Doxygen doc fixed.

  Revision 1.1.1.1  2002/10/21 18:07:37  paoscu
  Import libThread


*/

#ifndef _Thread_H_
#define _Thread_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <StopController.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////
//                      //
//     Thread class     //
//                      //
//////////////////////////

/**
 *\brief A Class for managing a single thread
 *
 *This class uses POSIX threads to run threads. Users must reimplement the run() method which
 *is the thread entry point.
 */
class Thread : public StopController
{
public:
  /*! class ThreadException declaration */
  exDECLARE_EXCEPTION(ThreadException, exException);

  /*! class TimeoutException declaration */
  exDECLARE_EXCEPTION(TimeoutException, ThreadException);

  enum class SvcStatuses
  {
    NotStarted,
    Started,
    Exiting
  };

  enum SvcStatus
  {
    SvcNotStarted __attribute__((deprecated("Use new SvcStatuses values"))) = static_cast<int>(SvcStatuses::NotStarted),
    SvcStarted __attribute__((deprecated("Use new SvcStatuses values"))) = static_cast<int>(SvcStatuses::Started),
    SvcExiting __attribute__((deprecated("Use new SvcStatuses values"))) = static_cast<int>(SvcStatuses::Exiting)
  };

  /**
   *\brief Constructor
   *
   *\param threadName is the name of the thread: default the empty string.
   */
  explicit Thread(const std::string& threadName = "unnamedThread");

  /**
   *\brief Destructor
   *
   *If the thread is still running it will be asynchronously stopped (be careful).
   */
  ~Thread() noexcept override;

  const Mutex& mutex() const noexcept { return _mutex; }

  /**
   *\brief Starts the thread.
   */
  void start();

  /**
   *\brief Starts the thread avoiding to touch other feature of the system
   * maybe some italian guys can recognize the FICT signature
   * unless you know exactly what you are doing, please prefer the simple start function and FICT
   */
  void startFICT();

  /**
   *\brief Tells the thread to stop.
   *
   *  setHaltLevel(l) and joins the thread.
   *
   * \param l the HaltLevel
   * \param joinAlso (default true) wait for the thread to join the current one
   *
   * It's up to the running thread to check if halt has been called.
   * Some threads could no check (and hence will not honor) the request
   * \see StopController class
   */
  virtual void halt(StopController::HaltLevel l, bool joinAlso = true);

  /**
   * equivalent to halt (StopController::IMMEDIATE)
   * \param joinAlso (default true) wait for the thread to join the current one
   */
  virtual void stop(bool joinAlso = true);

  /**
   * equivalent to halt (StopController::LAZY)
   * \param joinAlso (default true) wait for the thread to join the current one
   */
  virtual void lazyStop(bool joinAlso = true);

  /**
   * Stop thread immediately.
   *
   * Thread will is asynchronously stopped (be carefull).
   */
  virtual void exit();

  bool running() const;

  /**
   * \brief Yield to other thread (possibly)
   */
  static void yield() noexcept { StopController::yield(); }

  /**
   *\brief Get Thread priority
   */
  int getPrio() const;

  /**
   *\brief Set Thread priority
   */
  void setPrio(int prio);

  /**
   * \brief Returns the CPU mask representing the CPU affinity of this thread
   */
  cpu_set_t getCpuAffinity() const;

  /**
   * \brief Sets the CPU affinity of this thread
   */
  void setCpuAffinity(const cpu_set_t& cpuset);

  /**
   *\brief Returns the pointer of an ThreadException generated by thread.
   *
   * If The run() method ends throwing an exException then it is cloned and stored..
   * runException(exException *&e) fill the given pointer with a copy of the exception.
   * It is responsibilty of the caller to release the given exception.
   * If the thread is restarted, the internal pointer (if any) is automatically deleted.
   * \return The same reference to pointer passed to.
   *
   * \deprecated Use the new interface with shared_ptr.
   *
   * \b Example
   * \code
   * class myThread::public Thread
   * .....
   * myThread t;
   * .....
   * t.start();
   * .....
   * std::exception *e ;
   * t.runException(e)
   * if(e)
   * {
   *  ACS_LOG_NOTIFY_EX(*e);
   *  delete e ;
   * }
   * .....
   * \endcode
   *
   */
  std::exception*& runException(std::exception*&) const noexcept
    __attribute__((deprecated("Use alternative that return shared_ptr")));

  /**
   *\brief Returns a shared pointer of an Exception generated by thread.
   *
   * If The run() method ends throwing an exception then it is cloned and stored.
   * \return The a shared pointer filled with a copy of the throwed exception.
   *
   * \b Example
   * \code
   * class myThread::public Thread
   * .....
   * myThread t;
   * .....
   * t.start();
   * .....
   * std::shared_ptr<const std::exception> e = t.runException();
   * if(e)
   * {
   *  ACS_LOG_NOTIFY_EX(*e);
   * }
   * .....
   * \endcode
   *
   */
  std::shared_ptr<const std::exception> runException() const noexcept;

  /**
   *\brief Check if an ThreadException is generated by thread.
   *
   * \return If The run() method ends throwing an exception then return true. Otherwise return false.
   *
   * \b Example
   * \code
   * class myThread::public Thread
   * .....
   * myThread t;
   * .....
   * t.start();
   * .....
   * if(hasException())
   * {
   *  ACS_LOG_NOTIFY_EX(*t.runException());
   * }
   * .....
   * \endcode
   *
   */
  bool hasException() const noexcept;

  /**
   *\brief \return thread ID
   */
  pthread_t threadId() const;

  /**
   *\brief Joins the thread.
   *
   * \param msectimeout if the thread is not joined into tis time the TimeoutException is thrown. If it is left to
   * default (0), there is no check on the timeout and the method could last forever.
   * \returns true if the thread was run at least once, false if never started or already joined
   */
  bool join(unsigned long msectimeout = 0);

  /**
   * \brief Sends a signal to thread
   *
   * \param sigNum Signal to send
   */
  void kill(int sigNum);

  /**
   * \brief Return the signals explicitely masked when a thread is started.
   *
   * \param mask will be filled with all the signals explicitely masked when a thread is started.
   */
  static void maskedSignals(sigset_t& mask) noexcept;

  /**
   *\brief  Tries to join the thread using the Condition or exit after the given timeout
   *\return true if thread is not running
   */
  bool waitExitOrTimeout(unsigned long msecs) const;

  /**
   *\brief  Adds an external condition to be broadcasted on thread exit
   *\param c a pointer to the Condition instance
   */
  void setExternalCondition(Condition*);

  const std::string& threadName() const noexcept { return _threadName; }

protected:
  /**
   *\brief Thread entry point
   *
   *Must be reimplemented by user. This is the method which actually does work.
   *
   */
  virtual void run() = 0;

  /**
   *\brief \returns parent thread ID
   */
  pthread_t parentThreadId() const noexcept;

  /**
   *\brief  Does nothing (to be reimplemented if the user selects specials signals)
   */
  virtual void signalFilter(sigset_t&);

  static unsigned long nanosleep(unsigned long nanosec) { return StopController::nanosleep(nanosec); }

  static bool not_running_but_good(const Thread* thread) { return !thread->running() && !thread->hasException(); }

  static bool not_running_ex(const Thread* thread) { return !thread->running() && thread->hasException(); }

  // They do not copy anything
  Thread(const Thread&);
  Thread& operator=(const Thread&);

private:
  void setRunException(const std::shared_ptr<std::exception>& ex) noexcept;

#ifdef TEST_THREAD
public:
#endif
  void internalJoin();

  /**
   *\brief \returns thread ID
   */
  pthread_t threadIdUnguarded() const noexcept;

#ifdef TEST_THREAD
private:
#endif
  /**
   *\brief returns a pointer to an exException (if any).
   *
   *That pointer shall not be deleted because the class itself manages the memory.
   */
  static void* svc(void*);

public:
  static const unsigned int _joinTimeoutSlicingMsec = 250;
  template<typename Iterator, typename Predicate>
  class not_ex_th
  {
  public:
    explicit not_ex_th(Predicate p) :
      pred__(p) {}

    void operator()(Iterator i)
    {
      if (!i->hasException())
      {
        pred__(i);
      }
    }

    Predicate pred__;
  };

  /**
   * Checks all threads in the range [first, last).
   * waits until all of them are no longer running or one has an exception.
   * In the latter case, call the predicate on every thread (it is meant to stop them) and throws
   * an exception.
   *
   */
  template<typename Iterator, typename Predicate>
  static void check_threads(Iterator ifirst, Iterator ilast, Predicate predicate)
  {
    Iterator first = ifirst;
    Iterator last = ilast;
    while (first != last)
    {
      Iterator shortened = std::partition(first, last, not_running_but_good);
      if (shortened == last)
      {  // no more to check - everyone has finished
        return;
      }
      else
      {
        Iterator nrt = std::find_if(shortened, last, not_running_ex);
        if (nrt != last)
        {
          std::for_each(shortened, last, not_ex_th<typename Iterator::value_type, Predicate>(predicate));

          ACS_COND_THROW((*nrt)->hasException(),
                         Thread::ThreadException(*(*nrt)->runException(), "thread exception found"));
        }
        else
        {
          // no thread in error
          first = shortened;
        }
      }
    }
  }

  /** wait until every Thread in the interval [first, last) is not running */
  template<typename Iterator>
  static void wait_stop(Iterator first, Iterator last)
  {
    for (Iterator i = first; i != last; ++i)
    {
      while ((*i)->running())
      {
        Thread::yield();
        nanosleep(100 * 1000000U);  // 100 ms
      }
    }
  }

  /** starts every Thread in the interval [first, last) */
  template<typename Iterator>
  static void start(Iterator first, Iterator last)
  {
    for (Iterator i = first; i != last; ++i)
    {
      try
      {
        (*i)->start();
      }
      catch (std::exception const& ex)
      {
        ACS_LOG_NOTIFY_EX(ex);
      }
    }
  }

  /** halts every Thread in the interval [first, last) according with the given HaltLevel */
  template<typename Iterator>
  static void halt(Iterator first, Iterator last, HaltLevel l)
  {
    for (Iterator i = first; i != last; ++i)
    {
      (*i)->halt(l);
    }
  }

  /** returns the number of running threads in the in the interval [first, last) */
  template<typename Iterator>
  static size_t run_count(Iterator first, Iterator last)
  {
    size_t r_acc = 0;
    for (Iterator i = first; i != last; ++i)
    {
      if ((*i)->running())
      {
        ++r_acc;
      }
    }
    return r_acc;
  }

  // The attributes _started and _threadId are declared now as volatile following the issues
  // http://projectit.exprivia.it/browse/CPS-69
  // http://projectit.exprivia.it/browse/CPS-102
  //
  // The _threadId attribute is the most critical because it is used
  // in the svc function and also in the running function in true concurrency
  // and, without any barrier, this can lead to the function running return false
  // even if the thread is actually running due to a cached value of the attribute

private:
  volatile bool _started = false;  // NOSONAR - Needed to be volatile

#ifdef TEST_THREAD
public:
#endif
  volatile pthread_t _threadId = 0UL;  // NOSONAR - Needed to be volatile

#ifdef TEST_THREAD
private:
#endif
  Mutex _mutex = Mutex();
  pthread_t _threadHd = 0UL;
  pthread_t _parentThreadId = 0UL;
  std::shared_ptr<std::exception> _runException = nullptr;
  struct itimerval _itimer;
  std::string _threadName;
  Condition _condition = Condition();
  Condition* _extCondition = nullptr;
  volatile mutable SvcStatuses _svcStatus = SvcStatuses::NotStarted;  // NOSONAR - Needed to be volatile
  bool _fireRegisteredInitialization = true;
  ACS_CLASS_DECLARE_DEBUG_LEVEL(Thread);
};

_ACS_END_NAMESPACE

#endif
