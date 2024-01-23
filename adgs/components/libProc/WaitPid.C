// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.14  2014/04/02 11:32:25  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.13  2013/11/25 13:16:15  marfav
	blocking waitpid call restored
	non blocking calls will now pass through without waiting blocking ones
	
	Revision 2.12  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.11  2013/11/12 16:45:37  marfav
	waitProcCompletion waitPid blocking call discontinued and implemented by condition
	
	Revision 2.10  2013/11/07 16:32:18  marfav
	Thread safe implementation to avoid problems in waitpid syscall
	
	Revision 2.9  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.8  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.7  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.6  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.5  2011/03/01 11:52:55  marpas
	GCC 4.4.x support
	
	Revision 2.4  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.3  2007/07/19 13:14:47  marfav
	waitPid modified in order to manage requests coming from other threads than the parent
	
	Revision 2.2  2007/06/26 15:36:18  giucas
	exceptions caught in dtors
	
	Revision 2.1  2007/04/19 12:52:30  marpas
	no exception during atExit and garbage collection
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/07/25 15:42:34  ivafam
	Header format fixed
	
	Revision 1.8  2004/09/17 10:35:28  marpas
	potential regression in exitCode/exitSignal fixed
	
	Revision 1.7  2004/09/16 13:26:07  marpas
	bug fixed for isRunning ... first call can sometimes give erroneous positive result, killing a zombie process which has not yet notified its parent SIGCHLD
	
	Revision 1.6  2004/07/15 14:29:27  envisat
	arguments erasure policy changed.
	
	Revision 1.5  2004/04/14 17:29:33  marpas
	ThreadSafe used
	
	Revision 1.4  2004/03/31 16:15:17  marpas
	session concept implemented
	SpawnProc::setPGroupLeader method can be called BEFORE the spawn to create
	                           a new process group
	SpawnProc::killGroup method will propagate a signal to the whole group, only if
	                     the SpawnProc instance was spawned after setting the groupLeader flag.
	SpawnProc::killGroup and SpawnProc::kill methods now will set the errno even if they do not
	                                         really attempt to send the signal.
	
	Revision 1.3  2003/06/13 15:44:33  marpas
	global lock in _proc vector
	
	Revision 1.2  2003/04/30 10:05:42  marpas
	using namespace std was removed from includes
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#include <WaitPid.h>
#include <Filterables.h>
#include <csignal>
#include <unistd.h>

#if defined __linux__ 
  #if (__GNUC__ < 3) || (__GNUC__ >= 4)
#include <algorithm>
  #endif
#endif

#include <sys/wait.h>   // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ThreadSafe WaitPid::_procMutex ;

WaitPid::GarbageCollAtExit::GarbageCollAtExit() 
{
}

WaitPid::GarbageCollAtExit::~GarbageCollAtExit()  throw() 
{
// PRQA S 4631 L1
	try {
		unsafeGarbageCollection(true) ;
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e);
	}
// PRQA L:L1
}



//////////////////////////////////
//								//
// classe WaitPid::ProcArg	//
//								//
//////////////////////////////////


WaitPid::ProcArg::ProcArg(const string &f, bool removeAtExit) :
	_removeAtExit(removeAtExit),
	_arg(f)
{
}

WaitPid::ProcArg::~ProcArg() throw() {}

WaitPid::ProcArg &WaitPid::ProcArg::operator=(const string &f) 
{
	_arg = f ;
	return *this ;
}

WaitPid::ProcArg::ProcArg(const ProcArg &pa) : 
	_removeAtExit(pa._removeAtExit),
	_arg(pa._arg)

{
}

WaitPid::ProcArg &WaitPid::ProcArg::operator=(const ProcArg &pa) 
{
	if (this != & pa) {
		_arg = pa._arg ; 
		_removeAtExit = pa._removeAtExit ;
	}
	
	return *this ;
}

void WaitPid::ProcArg::removeIfNeeded() const 
{
	if (_removeAtExit) { unlink(_arg.c_str()) ; }
}

void WaitPid::ProcArg::removeAtExit(bool r) throw() // PRQA S 4121
{ 
	_removeAtExit = r ; 
}

bool WaitPid::ProcArg::removeAtExit() const throw() // PRQA S 4120
{
	return _removeAtExit ; 
}

const string &WaitPid::ProcArg::arg() const throw()  // PRQA S 4120
{
	return _arg ; 
}

//////////////////////////////////
//								//
// classe WaitPid::ProcArgs		//
//								//
//////////////////////////////////


WaitPid::ProcArgs::ProcArgs() throw() :
    _args()
{
}

WaitPid::ProcArgs::~ProcArgs() throw() { }

WaitPid::ProcArgs::ProcArgs(const ProcArgs &args) :
    _args(args._args)
{
}

WaitPid::ProcArgs &WaitPid::ProcArgs::operator=(const ProcArgs &args)
{
	if (this != &args) {
		_args = args._args ; 
    }
	return *this ;
}

const WaitPid::ProcArg &WaitPid::ProcArgs::operator[](size_t i) const 
{
	if (i >= size()) {
		ostringstream os ;
		os << i << " greater than " << size()-1 ; // PRQA S 3084
		
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	return _args[i] ;
}


WaitPid::ProcArg &WaitPid::ProcArgs::operator[](size_t idx) 
{
	if (idx >= size()) {
		for (size_t i = size() ; i < idx+1; i++) {  // PRQA S 3084
			_args.push_back(ProcArg(string(""))) ;  // PRQA S 3081
		}
    }
	return _args[idx] ;
}


void WaitPid::ProcArgs::removeIfNeeded() const 
{
	for (size_t i=0; i < _args.size(); i++) {
		_args[i].removeIfNeeded() ;
	}
}

size_t WaitPid::ProcArgs::size() const throw() 
{ 
	return _args.size() ; 
}

vector<WaitPid*> WaitPid::_procs ;
WaitPid::GarbageCollAtExit WaitPid::_garbageCollAtExit ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(WaitPid);

WaitPid::WaitPid(pid_t pid, bool gLeader) :
	_pid(pid), 
    _statptr(),
	_exitCodeValid(false),
	_used(false),
	_gLeader(gLeader),
    _args(),
    _waitPidMutex(),
    _exitCodeValidMutex()

{
	append(this) ;
}

WaitPid::~WaitPid() throw()
{
    // PRQA S 4631 L2
	try {
		_args.removeIfNeeded() ;
		remove(this) ;
	}
	catch(exception &e) {
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, e) ;
	}
    // PRQA L:L2
}

// PRQA S 1040, 3080, 3081 L3
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"

bool WaitPid::exitCode(int &ret) const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_exitCodeValid: " << _exitCodeValid) ;
	bool ok = _exitCodeValid ;
	if (ok) { // the process exited
		ret =  WIFEXITED(_statptr) ? WEXITSTATUS(_statptr) : -1 ;       // PRQA S 3003, 3380
    }
	check() ;
	if (!ok) { // was not ok before
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_exitCodeValid: " << _exitCodeValid) ;
		if (_exitCodeValid) { // the process exited
			ret =  WIFEXITED(_statptr) ? WEXITSTATUS(_statptr) : -1 ;  // PRQA S 3003, 3380
        }
	}
	return _exitCodeValid ;
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"



// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"

bool WaitPid::exitSignal(int &sig) const // PRQA S 4214
{
	bool ok = _exitCodeValid ;
    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_exitCodeValid: " << _exitCodeValid) ;
	if (ok) { // the process exited
		sig = WIFSIGNALED(_statptr) ? WTERMSIG(_statptr) : 0 ;          // PRQA S 3003, 3380
    }
	check() ;
	if (!ok) { // was not ok before
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_exitCodeValid: " << _exitCodeValid) ;
		if (_exitCodeValid) { // the process exited
			sig = WIFSIGNALED(_statptr) ? WTERMSIG(_statptr) : 0 ;      // PRQA S 3003, 3380
        }
	}
	return _exitCodeValid ;
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"

bool WaitPid::waitPid(bool block) const
{
    bool result (false);
    if (block)
    {
        // This is a blocking call.. be aware of that
        result = internalWaitPid (true);
    }
    else
    {
        try
        {
            // Pre allocate the waitPidMutex to avoid deadlocks
            TryLock tryLock (_waitPidMutex, 0, true);
            result = internalWaitPid (false);
        }
        catch (ThreadSafe::Busy&)
        {
            // someone else is waiting for the pid, rely on its result or a previous one
            Lock lock (_exitCodeValidMutex);
            result = _exitCodeValid;
        }
    }
    return result;
}


// PRQA L:L3

bool WaitPid::internalWaitPid(bool block) const // PRQA S 4020, 4214
{
    // Thread-Safe implementation to avoid multiple concurrent calls to waitpid syscall
    Lock lock (_waitPidMutex);

	if (!_exitCodeValid) {
		// returns the pid if the process exited
		int options (0);
        if (!block)
        {
            options = WNOHANG;
        }
		pid_t p = waitpid(pid(), &_statptr, options) ;
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "waitpid(" << pid() << ",...) returned: " << p ) ;
		if (p == -1)
		{
			if (EINTR == errno)
			{
				// In case the call has been interrupted by EINTR
				// there is the need to signal it to the external world
				// simulate that the process is still running
				// and let the caller make another run
				return false;
			}

			// We are not the parent of the task... 
			// try to discover if it is still running in another way
			if (kill (0) == 0)
			{
                ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "kill(0) still running") ;
				// The process is still running
				return false;
			}
			else
			{
                ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "kill(0) no longer running") ;
				// The process is not running 
				// Do not care about the exit status.. set to zero
				_exitCodeValid = true;
				_statptr = 0;
				return true;
			}
		}
		if (p == pid()) {
            ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "no longer running") ;
            Lock exitCodeLock (_exitCodeValidMutex);
			_exitCodeValid = true ;
			return true ;
		}
		return false ;
	}
	return true ;
}

bool WaitPid::isRunning() const // PRQA S 4214
{
	bool ret = not waitPid() ;
	check() ;
	return ret ;
}

int WaitPid::kill(int signal) const // PRQA S 4020, 4214
{
	if (_pid > 0) {

		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "sending " << signal << " to pid: " << _pid);

		// send the signal ... 
		return ::kill(_pid,signal) ;
	}
	// .. or set the errno if the pid is invalid
	errno = ESRCH ;	
	
	// and return the error 
	return -1 ;
}

int WaitPid::killGroup(int signal) const // PRQA S 4020, 4214
{
	if ((_pid > 0) && _gLeader) { // is a group leader ?
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "sending " << signal << " to pid: " << _pid);

		// send the signal ( as negative )
		return ::kill(-_pid,signal) ; // note negative pid
	}
	
	// ... or set errno if invalid pid or not groupleader
	errno = ESRCH ;
	// and return the error
	return -1 ;
}

pid_t WaitPid::pid() const // PRQA S 4120
{
	return _pid ;
}	

void WaitPid::append(WaitPid *p) 
{
	ThreadSafe::Lock lock(_procMutex) ;
	// append the pointer to the list
	_procs.push_back(p) ;

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "objects #: " << _procs.size());

}

void WaitPid::remove(WaitPid *p) // PRQA S 4020
{
	if (!p) {
		return ;
    }
	ThreadSafe::Lock lock(_procMutex) ;
	// look for the pointer
	vector<WaitPid *>::iterator ii = find(_procs.begin(), _procs.end(), p) ;
	if (ii != _procs.end()) { // erase it if is in the list
		_procs.erase(ii) ;
    }

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "objects #: " << _procs.size());
}

void WaitPid::check() 
{
	ThreadSafe::Lock lock(_procMutex) ;
	
	unsigned long exited = 0 ;
	// chech if some process has exited
	for (unsigned long i=0; i < _procs.size(); i++) { // PRQA S 4238, 4244
		exited += _procs[i]->waitPid() ? 1 : 0 ; // PRQA S 3000, 3084, 3380
    }
	
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "checked #: " << _procs.size() << " exited # " << exited );

	// at least one ... so proceed with the cleanup
	if (0 != exited) {
		garbageCollection() ;
    }
}

void WaitPid::garbageCollection(bool evenIfRunning) 
{
	try {
		// thread safety
		ThreadSafe::Lock lock(_procMutex) ;
		// call the unsafe method 
		unsafeGarbageCollection(evenIfRunning) ;
	}
	catch(exception &e) {
		ACS_LOG_NOTIFY_EX(e) ;
	}	
}


void WaitPid::unsafeGarbageCollection(bool evenIfRunning) 
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "procs.size(): " << _procs.size());

	for (long i=_procs.size()-1; i >= 0; i--) { // PRQA S 3000, 3084
		WaitPid *p = _procs[i] ; // PRQA S 3000

		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "pid: " << p->_pid << " isUsed: " << boolalpha << p->isUsed() );
		// clean if 
		// 1. isUsed and 
		//   1a. is done
		//   1b. even if it is running (see arguments)
		if (!p->isUsed() && (p->waitPid() || evenIfRunning)) { // PRQA S 3230

			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "erasing pid: " << p->_pid );

			// remove from list
			_procs.erase(find(_procs.begin(), _procs.end(), p)) ;
			delete p ;
		}
	}
}


void WaitPid::killall() 
{
	ThreadSafe::Lock lock(_procMutex) ;
	for (unsigned long i=0; i < _procs.size(); i++) { // PRQA S 4238, 4244
		// kill all running processes
		if (_procs[i]->isRunning()) {
			_procs[i]->kill() ; // ;-)
        }
	}
}

bool WaitPid::isUsed() const // PRQA S 4120
{ 
	return _used ; 
}

void WaitPid::regUser() 
{ 
	_used = true ; 
	check() ;
}

void WaitPid::unregUser() 
{ 
	_used = false ; 
	check() ;
}

void WaitPid::registerArgs(const ProcArgs &a) 
{
	ThreadSafe::Lock lock(_procMutex) ;

	_args = a ;
}




_ACS_END_NAMESPACE
