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
  Revision 5.20  2017/10/25 13:57:04  marpas
  range functions added

  Revision 5.19  2017/03/03 09:27:43  marfav
  Aligned to libException Rev_5_3 Condition interface change

  Revision 5.18  2016/06/27 12:23:29  davide.tiriticco
  Implemented setCpuAffinity() and getCpuAffinity()

  Revision 5.17  2016/04/14 09:54:22  marpas
  Thread does not inherits from ThreadSafe anymore - instead own a mutex variable and offer a cast operator to a
  reference to if in order to lock anybody would inherit from Thread.
  A new template static method added to control execution of multiple threads and, in case one of them should have an
  exception - call a predicate over them.

  Revision 5.16  2015/10/29 12:16:31  marfav
  PDSEV-17 printing the stack of all the running threads in case of crash

  Revision 5.15  2015/10/22 11:35:17  marpas
  InitCallbackManagerSingleton class improved: more simple, documented and more secure in terms of wrong use
  test changed accordingly

  Revision 5.14  2015/06/26 18:35:26  marpas
  dependency from new MC framework (libException Rev_5_61 or greater) added

  Revision 5.13  2015/05/26 15:23:19  damdec
  Introduced singleton creation for allowing the controlled initialization of not thread-safe libraries

  Revision 5.12  2014/07/04 16:20:48  marpas
  minor change

  Revision 5.11  2014/06/10 07:31:08  marfav
  Log a Warning when destroying a runnning thread

  Revision 5.10  2014/03/20 15:30:48  marfav
  Condition::timedwait interface changed

  Revision 5.9  2014/02/07 18:07:48  marpas
  adopting ACS_THROW and ACS_COND_THRO macros

  Revision 5.8  2013/11/21 10:17:26  marpas
  enumerative cases name changed

  Revision 5.7  2013/11/21 10:00:14  marfav
  Added support to externally provided condition to broadcast on exit
  Running assures to return false when the condition(s) has been broadcasted

  Revision 5.6  2013/10/25 10:43:35  marpas
  doc & warnings

  Revision 5.5  2013/08/26 09:48:26  marfav
  useless log removed

  Revision 5.4  2013/08/26 09:47:43  marfav
  waitExitOrTimeout microseconds management fixed

  Revision 5.3  2013/07/09 11:27:44  marpas
  yield, timeoutOrCancel and nanosleep method moved from Thread to StopController

  Revision 5.2  2013/06/18 12:27:25  marpas
  deprecated include changed

  Revision 5.1  2013/06/10 16:11:40  marpas
  deprecated macros substituted

  Revision 5.0  2013/06/10 16:08:25  marpas
  adopting libException 5.x standards

  Revision 2.17  2013/04/03 13:38:41  marpas
  run does not longer gets a void *

  Revision 2.16  2013/04/03 12:32:09  marpas
  run method gets a void *

  Revision 2.15  2013/02/22 10:28:55  marpas
  coding best practices applied

  Revision 2.14  2013/01/16 10:36:35  nicvac
  ConditionScopeManager instanced out of the try catch scope.

  Revision 2.13  2012/11/06 18:01:57  marpas
  Quality improved

  Revision 2.12  2012/10/04 09:45:59  marfav
  waitExitOrTimeout implementation and test completed

  Revision 2.11  2012/10/03 16:18:54  marfav
  Added support to ThreadSafe::Condition to signal thread completion to external entities

  Revision 2.10  2012/09/27 16:41:38  marpas
  Quality rules implementation
  wrong catch in dtor fixed

  Revision 2.9  2012/02/09 14:19:20  marpas
  refactoring

  Revision 2.8  2012/01/31 15:53:40  marpas
  removing compiler warnings

  Revision 2.7  2011/03/14 09:28:36  marpas
  Thread::stop, Thread::lazyStop and Thread::halt have an additional parameter (defaulted to true)to ask to wait for the
  join.
  Is a sort of asynchronous request, setting the HaltLevel and returning immediately.
  This allow a calling thread to perform some other tasks and periodically check for the sub-thread status.

  Revision 2.6  2011/03/11 09:08:56  marpas
  robustness improved
  Thread::join now returns a boolean (see documentation - make doc)
  test changed accordingly

  Revision 2.5  2011/03/07 09:46:51  marpas
  useless message removed

  Revision 2.4  2011/03/07 09:20:11  marpas
  after a pthread_join (successful) the pthread_is is reset in order to avoid a crash if the thread_id is an hidden
  pointer to some structures (as happen in some implementations)

  Revision 2.3  2009/06/26 10:50:41  marpas
  some classes do not throw any exception from their dtors

  Revision 2.2  2009/03/02 14:06:09  marpas
  CODECHECK parsing - no regressions

  Revision 2.1  2007/11/22 11:34:44  lucio.pulvirenti
  Stop Control feature added

  Revision 2.0  2006/02/28 08:27:50  marpas
  Exception 2.1 I/F adopted

  Revision 1.28  2006/02/21 18:12:29  marpas
  NTPL & kernel 2.6 fixes. test added

  Revision 1.27  2005/07/27 09:02:29  marpas
  math.h included for Advanced Server 4

  Revision 1.26  2005/05/30 20:27:09  marpas
  new method timeoutOrCancel, see documentation

  Revision 1.25  2005/05/30 12:18:58  marpas
  debug behaviour moved after a check

  Revision 1.24  2004/09/16 17:15:49  paoscu
  join will not raise an exception if the thread exited

  Revision 1.23  2004/09/10 17:19:18  paoscu
  pthread::join(unsigned long msectimeout = 0) implemented and tested

  Revision 1.22  2004/09/10 13:32:53  danalt
  join method added

  Revision 1.21  2004/05/04 14:15:36  paoscu
  thread start speedup

  Revision 1.20  2004/04/28 12:00:53  paoscu
  include missing

  Revision 1.19  2004/04/28 11:59:55  paoscu
  usleep changed into Timer::delay

  Revision 1.18  2004/04/13 16:41:39  paoscu
  comment modified

  Revision 1.17  2004/04/13 13:21:09  paoscu
  ACE dependencies removed

  Revision 1.16  2004/04/07 16:08:51  paoscu
  copy ctor and assignment operator declared protected and given with fake implementation

  Revision 1.15  2003/10/10 16:02:34  paoscu
  static maskedSignals method added

  Revision 1.14  2003/10/10 15:37:46  paoscu
  Example improved

  Revision 1.13  2003/10/02 10:41:50  paoscu
  Signals 2 and 15 are now blocked for each class inheriting Thread

  Revision 1.12  2003/07/28 22:19:46  paoscu
  deleteRunException private method changed into setRunException

  Revision 1.11  2003/07/17 14:15:49  paoscu
  runException method changed i/f too see doxygen documentation

  Revision 1.10  2003/07/17 10:38:39  paoscu
  thread name implemented

  Revision 1.9  2003/07/11 12:55:22  paoscu
  thread exceptions management

  Revision 1.8  2003/03/28 16:21:15  paoscu
  join in exit()

  Revision 1.7  2003/03/18 13:01:59  marpas
  sync bug again

  Revision 1.6  2003/03/11 17:41:55  marpas
  New cancellation policy

  Revision 1.5  2003/02/24 16:01:15  paoscu
  Sync problem at thread startup fixed

  Revision 1.4  2003/02/21 17:02:56  paoscu
  Bug fixed.

  Revision 1.3  2003/02/21 16:49:51  paoscu
  Minor changes.

  Revision 1.2  2003/01/20 11:06:24  marpas
  Get rid of memory used by terminated threads.

  Revision 1.1.1.1  2002/10/21 18:07:37  paoscu
  Import libThread


*/

#include <Thread.h>
#include <Filterables.h>
#include <MCFilterables.h>
#include <thV.h>
#include <sstream>
#include <unistd.h>
#include <csignal>
#include <algorithm>
#include <InitCallbackManagerSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace
{
const thV Version;  // NOSONAR - Force version's symbols linking
};

ACS_CLASS_DEFINE_DEBUG_LEVEL(Thread)

Thread::Thread(const std::string& threadName) :
  StopController(),
  _itimer(),
  _threadName(threadName)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "If DEBUG is greater than 1 the setitimer is called");
}

// Note: this is not a real copy, just the name
Thread::Thread(const Thread& t) :
  StopController(t),
  _itimer(),
  _threadName("copyOf" + t._threadName)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "If DEBUG is greater than 1 the setitimer is called");
}

Thread& Thread::operator=(const Thread& r)
{
  if (this != &r)
  {
    StopController::operator=(r);
    // just the name
    _threadName = "copyOf" + r._threadName;
  }

  return *this;
}

Thread::~Thread() noexcept
{
  try
  {
    if (running())
    {
      ACS_LOG_WARNING("Going to destroy the running thread named [" << threadName() << "]");
    }
  }
  catch (const std::exception& x)
  {
    ACS_LOG_NOTIFY_EX(x);
  }

  try
  {
    Thread::exit();
  }
  catch (const std::exception& x)
  {
    ACS_LOG_NOTIFY_EX(ThreadException(x, "Thread exception in dtor: logic error"));
  }
}

void Thread::startFICT()
{
  // Here comes the FICT implementation
  _fireRegisteredInitialization = false;
  start();
}

void Thread::start()
{
  ACS_COND_THROW(running(), ThreadException("Thread already running."));

  // the gprof ...
  ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
  setitimer(ITIMER_PROF, &_itimer, nullptr);
  ACS_CLASS_END_DEBUG

  setRunException(nullptr);
  ACS_COND_THROW(hasException(), ThreadException("Cannot clean run exception."));

  _svcStatus = SvcStatuses::NotStarted;
  _started = false;

  if (0 != _threadHd)
  {  // a previous thread ended
    internalJoin();
  }
  // philosophically, the parent of a thread is the one who
  // starts it - potentially different from the one instantiating it.
  _parentThreadId = ::pthread_self();

  // Just in case someone used FICT interface
  if (_fireRegisteredInitialization)
  {
    // Create the initialization singleton (it allows the controlled initialization of not thread-safe libraries)
    bool isInit = InitCallbackManagerSingleton::instance()->fire();
    ACS_COND_THROW(!isInit, ThreadException("Initialization singleton not created!"));
  }

  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "starting thread");

  int sterr = ::pthread_create(&_threadHd, nullptr, svc, this);
  ACS_COND_THROW(0 != sterr, ThreadException("Cannot spawn thread.", sterr));

  // wait until started
  do
  {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "waiting thread");
    // wait 500 usec
    nanosleep(500000);
  } while (!_started);

  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "waiting more");
  // allow 1 msec more to enter in the run method
  nanosleep(1000000);
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "started");
}

void Thread::setRunException(const std::shared_ptr<std::exception>& ex) noexcept
{
  try
  {
    Lock lock(_mutex);
    _runException = ex;
  }
  catch (const std::exception& x)
  {
    ACS_LOG_NOTIFY_EX(x);
  }
}

void Thread::halt(StopController::HaltLevel l, bool joinAlso)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  if (running())
  {
    setHaltLevel(l);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "cancel set to true");
  }
  if ((0 != _threadHd) && joinAlso)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "joining");
    internalJoin();
  }
}

void Thread::stop(bool joinAlso)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  halt(StopController::IMMEDIATE, joinAlso);
}

void Thread::lazyStop(bool joinAlso)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  halt(StopController::LAZY, joinAlso);
}

void Thread::exit()
{
  if (running())
  {
    setHaltLevel(StopController::IMMEDIATE);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "cancelling thread: " << _threadId);
    ::pthread_cancel(_threadId);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "joining thread: " << _threadId);
    internalJoin();
  }
  else if (0 != _threadHd)
  {  // a previous thread ended
    internalJoin();
  }
  else
  {
    ;  // nothing to do
  }
}

bool Thread::running() const
{
  if (0 == _threadId)
  {  // not yet started
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_threadId: " << _threadId);
    return false;
  }

  if (::pthread_self() == _threadId)
  {  // myself ... hence running (cogito ergo sum !)
    return true;
  }

  // check if the call to thew method has been done for a cond signal
  // in this case the _svcStatus pvt member has 'Exiting' value.
  if (_svcStatus == SvcStatuses::Exiting)
  {
    yield();
    if (0 != ::pthread_kill(_threadId, 0))
    {  // no longer running
      _svcStatus = SvcStatuses::NotStarted;
    }

    return false;
  }

  int err = ::pthread_kill(_threadId, 0);
  if (0 == err)
  {  // father: try to send a fake signal to see if the thread exists
    return true;
  }
  if (err == ESRCH)
  {  // no longer exists
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "pthread_kill returned ESRCH.");
    return false;
  }

  ACS_THROW(ThreadException("Kill with signal 0 to thread failed.", err));
}

int Thread::getPrio() const
{
  ACS_COND_THROW(!running(), ThreadException("Thread is not running."));

  struct sched_param param;
  int policy = 0;

  int err = ::pthread_getschedparam(_threadId, &policy, &param);
  ACS_COND_THROW(0 != err, ThreadException("Cannot get thread priority.", err));

  return param.sched_priority;
}

// The setPrio method do not technically modifies any member variable
// nevertheless it changes the running thread (if any) priority.
// A sort of privileges should be ensured to do this.
// So, any code having a non const Thread can set its priority as it can set other properties
void Thread::setPrio(int prio)
{
  ACS_COND_THROW(!running(), ThreadException("Thread is not running."));

  struct sched_param param;
  int policy = 0;

  int err = ::pthread_getschedparam(_threadId, &policy, &param);
  ACS_COND_THROW(0 != err, ThreadException("Cannot get thread priority.", err));

  param.sched_priority = prio;
  err = ::pthread_setschedparam(_threadId, policy, &param);
  ACS_COND_THROW(0 != err, ThreadException("Cannot set thread priority.", err));
}

// The setCpuAffinity method do not technically modifies any member variable
// nevertheless it changes the running thread (if any) priority.
// A sort of privileges should be ensured to do this.
// So, any code having a non const Thread can set its priority as it can set other properties
void Thread::setCpuAffinity(const cpu_set_t& cpuset)
{
  ACS_COND_THROW(!running(), ThreadException("Thread is not running."));

  int err = ::pthread_setaffinity_np(_threadId, sizeof(cpu_set_t), &cpuset);
  ACS_COND_THROW(err != 0, ThreadException("Cannot set CPU affinity for this thread.", err));
}

cpu_set_t Thread::getCpuAffinity() const
{
  cpu_set_t cpuset;

  CPU_ZERO(&cpuset);

  int err = ::pthread_getaffinity_np(_threadId, sizeof(cpu_set_t), &cpuset);
  ACS_COND_THROW(err != 0, ThreadException("Cannot get CPU affinity for this thread.", err));

  return cpuset;
}

// OLD DEPRECATED IMPLEMENTATION - TO BE REMOVED FROM HERE
std::exception*& Thread::runException(std::exception*& e) const noexcept
{
  try
  {
    Lock lock(_mutex);
    if (_runException)
    {
      auto x = dynamic_cast<exException*>(_runException.get());
      if (x)
      {
        return e = x->clone();
      }
      return e = new ThreadException(*_runException, "std::exception caught");
    }
  }
  catch (const std::exception& x)
  {
    ACS_LOG_NOTIFY_EX(x);
  }
  return e = nullptr;
}
// OLD DEPRECATED IMPLEMENTATION - TO BE REMOVED TO HERE

std::shared_ptr<const std::exception> Thread::runException() const noexcept
{
  Lock lock(_mutex);
  return std::const_pointer_cast<const std::exception>(_runException);
}

bool Thread::hasException() const noexcept
{
  Lock lock(_mutex);
  return nullptr != _runException;
}

pthread_t Thread::threadId() const
{
  ACS_COND_THROW(!running(), ThreadException("Thread is not running."));
  return _threadId;
}

bool Thread::join(unsigned long msectimeout)
{
  if (0 == _threadHd)
  {  // thread never run or already joined
    return false;
  }

  // the test is done on _threadId whose value is set into the children thread
  // this is a stronger constraint that checking _threadHd whose meaning the thread ran and finished, but was still not
  // joined the start method synchronize on _threadId ensuring it will not return until the thread started
  ACS_COND_THROW(!running() && (0 == _threadId), ThreadException("Thread is not running."));

  if (0 != msectimeout)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "waiting " << msectimeout << " msec");
    unsigned long wait = 0;
    // each cycle lasts 250 msec, do not lower this time to avoid the cycle is much longer than the nanosleep
    for (wait = 0; wait <= msectimeout; wait += _joinTimeoutSlicingMsec)
    {
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "waited " << wait << " msec");
      if (!running())
      {
        break;
      }
      nanosleep(250000000);
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "out of the wait loop after " << wait << " msec");

    ACS_COND_THROW(running(), TimeoutException() << "Thread: " << _threadId << " timeout expired joining.");
  }

  internalJoin();
  return true;
}

void Thread::internalJoin()
{
  if (0 == _threadId)
  {
    _threadHd = 0;
    return;  // useless join
  }

  const int ret = ::pthread_join(_threadId, nullptr);

  if (0 == ret)
  {
    _threadHd = 0;
    _threadId = 0;
    _svcStatus = SvcStatuses::NotStarted;
    return;
  }

  switch (ret)
  {
    case ESRCH:
      ACS_THROW(ThreadException(ret) << "Thread id: " << _threadId << " was not found.");
    case EINVAL:
      ACS_THROW(ThreadException(ret) << "Thread id: " << _threadId
                                     << " was detached or another thread is waiting its termination.");
    case EDEADLK:
      ACS_THROW(ThreadException("Deadlock on join (joining with myself ?).", ret));
    default:
      ACS_THROW(ThreadException("Unknown pthread_join errno.", ret));
  }
}

// The kill method do not technically modifies any member variable
// nevertheless it changes the running thread (if any) priority.
// A sort of privileges should be ensured to do this.
// So, any code having a non const Thread can set its priority as it can set other properties
void Thread::kill(int sigNum)
{
  ACS_COND_THROW(!running(), ThreadException("Thread is not running."));
  int err = ::pthread_kill(_threadId, sigNum);
  ACS_COND_THROW(0 != err, ThreadException("Cannot send signal to thread.", err));
}

pthread_t Thread::threadIdUnguarded() const noexcept
{
  return _threadId;
}

pthread_t Thread::parentThreadId() const noexcept
{
  return _parentThreadId;
}

void Thread::signalFilter(sigset_t& signalMask)
{
  // nothing (to be reimplemented if the user selects specials signals)
}

class ConditionScopeManager
{
public:
  explicit ConditionScopeManager(Condition& cond) :
    _cond(&cond) {}
  explicit ConditionScopeManager(Condition* cond) :
    _cond(cond) {}
  ConditionScopeManager(const ConditionScopeManager&) = delete;
  ConditionScopeManager& operator=(const ConditionScopeManager&) = delete;

  ~ConditionScopeManager() noexcept
  {
    try
    {
      if (_cond)
      {
        _cond->broadcast();
      }
    }
    catch (const std::exception& ex)
    {
      ACS_LOG_NOTIFY_EX(ex);
      // do nothing.. just die
    }
  }

private:
  Condition* _cond;
};

namespace
{
class SvcStatusGuard
{
public:
  explicit SvcStatusGuard(volatile Thread::SvcStatuses& s) :
    status_(s) { status_ = Thread::SvcStatuses::Started; }
  ~SvcStatusGuard() noexcept { status_ = Thread::SvcStatuses::Exiting; }
  SvcStatusGuard(const SvcStatusGuard&) = delete;
  SvcStatusGuard& operator=(const SvcStatusGuard&) = delete;

private:
  volatile Thread::SvcStatuses& status_;
};

class LiveThreadIdScopeGuard
{
public:
  LiveThreadIdScopeGuard(pthread_t id, const std::string& name) :
    _id(id)
  {
    exStackTrace::addLiveThreadId(_id, name);
  }
  LiveThreadIdScopeGuard(const LiveThreadIdScopeGuard&) = delete;
  LiveThreadIdScopeGuard& operator=(const LiveThreadIdScopeGuard&) = delete;

  ~LiveThreadIdScopeGuard() noexcept
  {
    exStackTrace::removeLiveThreadId(_id);
  }

private:
  pthread_t _id;
};
}  // namespace

void* Thread::svc(void* m)  // NOSONAR - Must be void* for pthread_create implementation
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  auto me = static_cast<Thread*>(m);
  me->_threadId = ::pthread_self();

  LiveThreadIdScopeGuard liveIdGuard(me->_threadId, me->_threadName);
  ConditionScopeManager theConditionManager(me->_condition);
  ConditionScopeManager theExtConditionManager(me->_extCondition);
  // do not move before the cond guard: it has to be destroyed BEFORE them
  SvcStatusGuard svcStatusGuard(me->_svcStatus);

  try
  {
    sigset_t newmask;
    maskedSignals(newmask);
    ::pthread_sigmask(SIG_BLOCK, &newmask, nullptr);

    int err = ::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    ACS_COND_THROW(0 != err, ThreadException("Error in thread set cancellation state.", err));

    err = ::pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
    ACS_COND_THROW(0 != err, ThreadException("Error in thread set cancellation type.", err));

    me->StopController::reset();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling run");
    me->_started = true;
    me->run();
  }
  catch (const exException& e)
  {
    me->setRunException(std::shared_ptr<std::exception>(e.clone(me->_threadName)));
  }
  catch (const std::exception& e)  // NOSONAR - Want to catch all exceptions here
  {
    me->setRunException(std::make_shared<ThreadException>(e, "std::exception caught"));
  }

  try
  {
    mc::MCParameters::instance()->rem(::pthread_self());
  }
  catch (const std::exception&)  // NOSONAR - Want to catch all exceptions here
  {
    ;  // DO NOTHING
  }

  return nullptr;
}

void Thread::setExternalCondition(Condition* c)
{
  _extCondition = c;
}

bool Thread::waitExitOrTimeout(unsigned long msecs) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  if (_threadId == ::pthread_self())
  {
    // Avoid entenring waitExitOrTimeout inside the thread itself because will not ever work
    // if I am inside the run cannot wait the exit from the run
    // call the StopController timeoutOrCancel instead
    static bool emitWarning = true;
    if (emitWarning)
    {
      // Emit the warning once per process... usually this function is called in a loop
      emitWarning = false;
      ACS_LOG_WARNING("Be careful: do not call waitExitOrTimeout from inside the run of a thread. will never exit!!");
    }
    return StopController::timeoutOrCancel(msecs);
  }

  bool retval = true;

  // Avoid screaming at each call in case of system issues
  static bool alreadywarned = false;

  if (!running())
  {
    // do not wait threads not running or never started
    retval = true;
  }
  else
  {
    // Try to wait it for msec milliseconds

    try
    {
      bool completed = true;
      {  // NOSONAR - Want a scope for lock variable
        // Release the lock just after the condition exits
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Acquiring the Lock [" << ::pthread_self() << "]");

        Lock lock(_condition);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Calling timed wait on condition [" << ::pthread_self() << "]");

        // This returns false in case of timeout
        completed = _condition.timedwait(msecs);

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exiting the condition, thread finished [" << ::pthread_self() << "]");
      }

      if (!completed)
      {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got timeout in timedwait after " << msecs << " milliseconds");
        retval = !running();
      }
    }
    catch (const std::exception& ex)  // NOSONAR - Want to catch all exceptions here
    {
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Got exception: " << ex.what());

      if (!alreadywarned)
      {
        alreadywarned = true;
        ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_WARNING("Unable to call timed wait on thread condition. Going with a simple delay without conditions");
      }

      nanosleep(msecs * 1000000);
      retval = !running();
    }
  }
  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to exit. Return value is " << std::boolalpha << retval);

  return retval;
}

void Thread::maskedSignals(sigset_t& mask) noexcept
{
  sigemptyset(&mask);
  sigaddset(&mask, SIGINT);
  sigaddset(&mask, SIGTERM);
}

_ACS_END_NAMESPACE
