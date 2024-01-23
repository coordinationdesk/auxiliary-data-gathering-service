// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
	Revision 2.32  2018/03/07 12:11:04  nicvac
	QA check
	
	Revision 2.31  2018/03/07 10:41:37  nicvac
	Spawn Proc with logs. Executor of pool of Spawn Proc.
	
	Revision 2.30  2018/01/09 16:53:33  marfav
	ECICMF-194
	Added support for env variable setting and unsetting
	between fork and exec (in the child process)
	
	Revision 2.29  2016/04/12 12:00:07  marfav
	S2PDGS-1459 SpawnProc DTOR deletes first the procWrapper and then the procGuard
	
	Revision 2.28  2016/03/29 12:38:49  marfav
	S1PDGS-31708 reinit the mutex in the child after the fork
	
	Revision 2.27  2015/12/22 12:46:48  marpas
	qa warning fixed
	
	Revision 2.26  2015/12/22 09:11:08  marfav
	Fixed exitCause implementation (again)
	
	Revision 2.25  2015/12/17 14:50:33  marfav
	S2PDGS-1394 fixing exitCause return value handling
	
	Revision 2.24  2015/01/27 16:42:15  marfav
	Releasing the input descriptor before waiting the process to exit
	
	Revision 2.23  2015/01/20 14:18:27  marfav
	checking exit code of close syscall
	
	Revision 2.22  2014/06/26 16:24:28  marpas
	exitCause method added
	
	Revision 2.21  2014/04/02 11:32:24  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.20  2013/12/09 11:27:00  marpas
	splitting spawnProc method in two: all code for the child process is now separate for easiest maintenance
	
	Revision 2.19  2013/12/09 11:16:30  marpas
	symboolic links are now resolved
	
	Revision 2.18  2013/11/28 18:42:16  marpas
	doc improved & minor changes
	
	Revision 2.17  2013/11/28 15:40:05  marpas
	CmdIORedir is able to redirect simultaneously input and output on files
	When just one of the two streams is redirected, the class act as the other end of a pipe,
	either input or output according to which stream is redirected.
	If both ends are redirected to file system, the class cannnot act as pipe end
	
	Revision 2.16  2013/11/25 13:17:37  marfav
	WaitProcCompletion implemented by waitPid blocking call (restored)
	_procGuard pointer is now used in thread safe way
	
	Revision 2.15  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.14  2013/11/22 14:42:19  marfav
	Waiting the wrapper thread to exit before destroying it
	
	Revision 2.13  2013/11/12 16:45:37  marfav
	waitProcCompletion waitPid blocking call discontinued and implemented by condition
	
	Revision 2.12  2013/07/25 15:24:59  marfav
	ThreadedProcWrapper embedded in SpawnProc to allow waiting timed conditions
	
	Revision 2.11  2013/07/25 14:07:17  marfav
	Delay removed
	
	Revision 2.10  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.9  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.8  2012/12/01 20:06:29  marpas
	qa rules
	
	Revision 2.7  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.6  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.5  2011/03/01 11:52:55  marpas
	GCC 4.4.x support
	
	Revision 2.4  2009/12/22 17:36:06  marpas
	CmdIORedir class added
	
	Revision 2.3  2009/12/22 16:01:04  marpas
	work in progress
	
	Revision 2.2  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.1  2006/03/03 09:57:23  marpas
	exception catching improved
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.22  2005/05/20 15:18:31  fracar
	fixed problem with standard descriptors when in detached mode
	
	Revision 1.21  2004/07/15 14:29:27  envisat
	arguments erasure policy changed.
	
	Revision 1.20  2004/06/17 10:51:21  marpas
	*** empty log message ***
	
	Revision 1.19  2004/05/13 12:28:42  marpas
	spawn method returns always a SpawnException
	
	Revision 1.18  2004/04/28 12:12:36  marpas
	sleep changed into Timer::delay
	
	Revision 1.17  2004/04/19 15:36:47  marpas
	spawn exception rethrown
	
	Revision 1.16  2004/04/14 17:29:33  marpas
	ThreadSafe used
	
	Revision 1.15  2004/04/13 13:24:41  marpas
	signal.h was missing
	
	Revision 1.14  2004/04/02 12:53:21  marpas
	Dtor call sometimes crashed
	
	Revision 1.13  2004/03/31 16:15:17  marpas
	session concept implemented
	SpawnProc::setPGroupLeader method can be called BEFORE the spawn to create
	                           a new process group
	SpawnProc::killGroup method will propagate a signal to the whole group, only if
	                     the SpawnProc instance was spawned after setting the groupLeader flag.
	SpawnProc::killGroup and SpawnProc::kill methods now will set the errno even if they do not
	                                         really attempt to send the signal.
	
	Revision 1.12  2003/11/10 16:33:45  paoscu
	Extra debug print.
	
	Revision 1.11  2003/10/10 16:18:36  marpas
	Because threads but the main one could have blocked some signals (see Thread::maskedSignals
	static method), processes spawned from those threads would unblock those signals.
	
	Revision 1.10  2003/07/11 12:57:34  marpas
	exException::clone implemented
	
	Revision 1.9  2003/07/01 08:04:48  marpas
	Thread safety improved
	
	Revision 1.8  2003/04/30 16:30:06  marpas
	using namespace std no longer into acs includes
	
	Revision 1.7  2003/04/17 14:41:54  marpas
	Check of the existence of executable enforced before tryoing to execvp-ing it
	
	Revision 1.6  2003/04/15 17:54:51  marpas
	GCC3.2
	
	Revision 1.5  2003/01/23 16:23:10  marpas
	DEBUG conditions file closure
	
	Revision 1.4  2002/11/20 10:43:23  marpas
	Versioning support added
	
	Revision 1.3  2002/11/20 10:38:00  marpas
	Improved tests.
	SpawnProc destructor will not kill the process if it is not running any longer.
	
	Revision 1.2  2002/10/11 09:27:35  marpas
	compilation warning removed
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
			

*/

#include "SpawnProc.h"
#include <WaitPidGuard.h>
#include <ThreadedProcWrapper.h>
#include <Timer.h>
#include <File.h>
#include <Thread.h>
#include <Filterables.h>
#include <prV.h>

#include <sys/time.h>   // PRQA S 1013 4
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sstream>
#include <unistd.h>
#include <csignal>
#include <cstring>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

// versioning support 
namespace {
    prV version ;
}


//////////////////////////////
//							//
// classe SpawnProc			//
//							//
//////////////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(SpawnProc)
ACS_CLASS_DEFINE_DEBUG_LEVEL(CmdIORedir)

ThreadSafe  SpawnProc::_procMutex ;

pthread_t SpawnProc::_mainThread = pthread_self() ;

SpawnProc::SpawnProc(const string &pr, bool resolvSymLink) :
	_autoKill(true),
	_detached(false),
	_procGuard(0),
	_spawned(false),
	_process(pr),
    _args(),
	_inDescriptor(-1),
	_outDescriptor(-1),
	_errDescriptor(-1),
	_pGroupLeader(false),
	_spawning(false),
	_procWrapper(0),
    resolvSymLink_(resolvSymLink),
	envSet_(),
	envClr_(),
	_tagName()
{

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "DEBUG values:\n" 
            << "any values prints debugging info.\n" 
            << "if mod 10 is different from zero doesn't close open descriptors in children\n" 
            << "if > 49 sleeps 5 secs just at the begin of the fork\n" 
            << "if > 99 enables libc tracing" );
}

SpawnProc::~SpawnProc() throw()
{
// PRQA S 4631 L1
    try {

		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "spawned: " << boolalpha << spawned()
			    << " detached: " << boolalpha << _detached
			    << " autoKill: " << boolalpha << _autoKill );


	    int ret = 0 ;
	    // the process has been spawned and finished or never spawned
	    if ((spawned() && exitCode(ret)) || !spawned()) {

			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "removing arguments if needed");

		    // check args behaviour
		    _args.removeIfNeeded() ;
	    }

	    if (!_detached) { // it didn't was detached
		    if (_autoKill) { // autokill given ?

				ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "killing " << pid());

			    // kill if it is still running
			    if (isRunning()) {
				    kill() ;
				    _args.removeIfNeeded() ;
			    }
		    }
		    else {
			    // just register the args - they will be processed 
			    // at procGuard destruction
				ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "no auto kill " << pid());

			    _procGuard->registerArgs(_args) ;
		    }
	    }
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

	try
	{
		//S2PDGS-1459 procWrapper is actually using the procGuard
		//must stop and delete it before deleting the guard
		if (0 != _procWrapper)
		{
			// Stop and join the thread. 
			// Never delete the threads without stopping them!
			_procWrapper->stop(true);
			delete _procWrapper;
		}
		// now it is safe to delete the guard
		delete _procGuard ;
	}
	catch(exception &x) 
	{
		ACS_LOG_NOTIFY_EX(x) ;
	}

// PRQA L:L1
}

bool SpawnProc::spawned() const
{
	return _spawned || _spawning ;
}

bool SpawnProc::isRunning() const // PRQA S 4214
{
	// if there is a _procGuard ... then look at it
	// else for us is not running
	
	bool ret = getProcGuard() ? getProcGuard()->isRunning() : false ; // PRQA S 3384

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "returning " << boolalpha << ret);

	return ret ;
}

bool SpawnProc::waitProcCompletion(bool emitExceptionIfNotSpawn) // PRQA S 4211 
{
	bool ret = true;

	if (emitExceptionIfNotSpawn && (0 == getProcGuard()))
	{
		// Never started and exception requested in this case
		ACS_THROW(SpawnException("Unable to wait for completion a never started process")) ; // PRQA S 3081
	} 

	if (0 != getProcGuard())
	{
        ret = getProcGuard()->waitPid(true);
	}

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "returning " << boolalpha << ret);

	return ret; 
}

bool SpawnProc::waitExitOrTimeout (unsigned long msecs)
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "entering");
	bool ret = true;

	if (isRunning())
	{
		{
			// entering the critial zone
			ThreadSafe::Lock lock(_procMutex) ;
			if (0 == _procWrapper)
			{
				ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Creating a new ThreadedProcWrapper for monitoring");
				_procWrapper = new ThreadedProcWrapper (this);
				_procWrapper->start();
			}
		}

		// here the lock is released
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Invoking the condition on the wrapper");
		ret = _procWrapper->waitExitOrTimeout(msecs);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "The process is not running");
	}

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "returning " << boolalpha << ret);

	return ret;
}

int SpawnProc::kill(int signal) const // PRQA S 4214
{	
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "signal is " << signal);

	return getProcGuard() ? getProcGuard()->kill(signal) : 0 ; // PRQA S 3382
}

int SpawnProc::killGroup(int signal) const // PRQA S 4214
{	
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "signal is " << signal);

	return getProcGuard() ? getProcGuard()->killGroup(signal) : 0 ; // PRQA S 3382
}

const std::string& SpawnProc::getTagName() const // PRQA S 2133, 4120, 4121
{
	return _tagName;
}

void SpawnProc::setTagName(const std::string& tagName) // PRQA S 2133, 4120, 4121
{
	_tagName = tagName;
}


pid_t SpawnProc::pid() const
{
	return getProcGuard() ? getProcGuard()->pid() : 0 ; // PRQA S 3382
}

bool SpawnProc::exitCode(int &r) const
{	
	return getProcGuard() ? getProcGuard()->exitCode(r) : false ; // PRQA S 3382
}


bool SpawnProc::exitSignal(int &s) const 
{
	return getProcGuard() ? getProcGuard()->exitSignal(s) : false ; // PRQA S 3382
}

bool SpawnProc::exitCause(int &xit, int &sig) const 
{
    //S2PDGS-1394 fixing return value in case of signaled process
    //it was TRUE instead of FALSE
    ACS_COND_THROW(not getProcGuard(), SpawnException("cannot check the process, was it spawned ?")) ;  // PRQA S 3081
    bool ret = false ;

	// exitSignal returns true if process has a valid exit code (i.e. completed)
	// even if it has not been actually signalled
	bool exited = getProcGuard()->exitCode(xit);
	getProcGuard()->exitSignal(sig);

	// Handle the codes only if the process completed its execution
	// otherwise just return false

	if (exited)
	{
		if (0 != sig) 
		{
			ACS_CLASS_WRITE_DEBUG_TH (ACS_VLO_VERB, "process got a signal: signal was " << sig);
		    ret = false ;
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG_TH (ACS_VLO_VERB, "process exited: exit code was " << xit);
		    ret = true ;
		}
	}
	ACS_CLASS_WRITE_DEBUG_TH (ACS_VLO_VERB, "returning " << boolalpha << ret);
    return ret ;
}
	
void SpawnProc::setInDescriptor(int d) // PRQA S 4121
{
	_inDescriptor = d ;
}


void SpawnProc::setOutDescriptor(int d)  // PRQA S 4121
{
	_outDescriptor = d ;
}

void SpawnProc::setErrDescriptor(int d)  // PRQA S 4121
{
	_errDescriptor = d ;
}

void SpawnProc::spawnProcChild(bool detached) 
{
	if (_pGroupLeader) { // is a group leader ?
		setsid() ;
    }

	ACS_CLASS_BGN_DEBUG(50) // debug level is 50 or greater: simulate a delay in starting
		Timer::delay(5000) ; // PRQA S 4400
	ACS_CLASS_END_DEBUG

	// now change given standard descriptors (if applies)
	//
	// NOTE:
	// if we run in detached mode, then must redirect stdin, stderr and stdout
	// descriptors to /dev/null in order to avoid strange effects if the calling
	// process does an exit, in which case these descriptors would be no longer valid for
	// spawned processes
	// if no detached, than parent's descriptors must hold, and if they become invalid
	// we must assume that a programming error (unwanted exit) and/or crash happened
	if (detached)
	{
		 int fd = open("/dev/null", O_RDWR); // PRQA S 4412
		 if (fd == -1)
		 {
			 ACS_THROW(SpawnException("Cannot open /dev/null", errno));
		 }
		 else
		 {
			 _inDescriptor = _outDescriptor = _errDescriptor = fd;
		 }
	}


	// now we can finally proceed to descriptors redirection (dup)
	if (_inDescriptor >=0) { dup2(_inDescriptor,0) ; }
	if (_outDescriptor >=0) { dup2(_outDescriptor,1) ; }
	if (_errDescriptor >=0) { dup2(_errDescriptor,2) ; } // PRQA S 4400

	// close all open files
	struct rlimit rlim ; // PRQA S 4102
	getrlimit (RLIMIT_NOFILE, &rlim);

	rlim_t maxfd = std::max(rlim.rlim_cur, rlim.rlim_max) ;


	if (((ACS_CLASS_GET_DEBUG()%10)==0 ) || (ACS_CLASS_GET_DEBUG() <= 0) ) { // PRQA S 4400

		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "closing " << maxfd << " descriptors");

		// close all opened file descriptors but the standard three (in, out, err) 
		for (rlim_t i=4; i < maxfd; i++) {     //  PRQA S 3081 4
			if ((rlim_t(_inDescriptor) != i ) && 
				(rlim_t(_outDescriptor) != i ) && 
				(rlim_t(_errDescriptor) != i )
			   ) {
				::close(i) ;  // PRQA S 3000, 3010
            }
		}
	}

	// now, if we were running detached, before exec we must close the file
	// descriptor that was opened on /dev/null before dup2
	// fd was lost, actually, but its value is stored in
	// _inDescriptor = _outDescriptor = _errDescriptor = fd;
	if (detached) {
		if (0 != close(_inDescriptor)) { ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("::close syscall failed", errno)); }
    }

	// exec the process
	// LOAD INTO MEMORY
	char **argv = new  char *[_args.size()+2] ;     // PRQA S 3084, 4403
	argv[_args.size()+1] = 0 ;                      // PRQA S 3084
	argv[0] = strdup(_process.c_str()) ;

	// This was debug level 2 - Replace ACS_VLO_VERB with 2 if needed
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "process: \"" << argv[0] << "\""); 

	for (size_t i=1; i < _args.size()+1; ++i) { // PRQA S 3084, 4238, 4244
		argv[i] = strdup(_args[i-1].arg().c_str()) ;    // PRQA S 3084

		// This was debug level 2 - Replace ACS_VLO_VERB with 2 if needed
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "argv[" << i << "]: \"" << argv[i] << "\""); 
	}

	// This was debug level 2 - Replace ACS_VLO_VERB with 2 if needed
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "_mainThread: " << _mainThread); 

	if (pthread_self() != _mainThread) {
		// ask to the Thread library which signal would have been masked out 
		sigset_t masked ;   // PRQA S 4102
		Thread::maskedSignals(masked) ;
		pthread_sigmask(SIG_UNBLOCK, &masked, 0) ;
	}

	// Apply the ENV settings now, just before the exec
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Applying env settings"); 
	map<string, string>::iterator setit = envSet_.begin();
	while ( setit != envSet_.end() )
	{
		string varname = setit->first;
		string varvalue = setit->second;
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Setting env variable [" << varname << "] to value [" << varvalue << "]"); 

		// In case setenv fails, go ahead and leave a track in the log
		if ( setenv (varname.c_str(), varvalue.c_str(), 1) != 0 )
		{
			ACS_LOG_WARNING ("Error got while setting env variable " << varname);
		}

		// Loop on the variable map
		++setit;
	}

	// After setting the env variables go for the request UNSET
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Applying env settings - UNSET"); 
	set<string>::iterator unsetit = envClr_.begin();
	while ( unsetit != envClr_.end() )
	{

		string varname = *unsetit;
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Un-setting env variable [" << varname << "]"); 

		// In case unsetenv fails, go ahead and leave a track in the log
		if ( unsetenv (varname.c_str()) != 0 )
		{
			ACS_LOG_WARNING ("Error got while un-setting env variable " << varname);
		}

		// Loop on the variable map
		++unsetit;
	}



	// HERE IS THE exec ... 
	execvp(_process.c_str(),argv) ;

	ACS_LOG_TRYCATCHNOTIFY_EX(exIOException ("Spawn failed (execvp failure) on " + _process + ". If it is a script please check the requested interpreter because errno set to 2 can be misleading", errno));

	// committing suicide with signal 9... RIP
	::pthread_kill ( ::pthread_self(), 9 );
}

void SpawnProc::spawnProc(unsigned int msecDelay, bool detached)
{
	try {
		// we are spawning the process
		_spawning = true ;
		
		// check if a double spawn has been called
		if (isRunning()) {
			ostringstream os ;
			os << "Process \"" << _process << "\" was already spawned."
			   << " pid is: " << pid() ;
			ACS_THROW(SpawnException(os.str())) ;   // PRQA S 3081
		}

		// check if the process is executable
		string exe = File::which(_process) ;
        if (resolvSymLink_ && File::entry_is_symlink(exe)) {
            _process = File::readlink(exe, true) ;
            ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "exe (after link resolution) " << _process);
        }

		if (exe.empty() || !File::isExecutable(exe)) {
			ostringstream os ;
			os << "Process \"" << _process << "\" was not found or is not executable." ;
			ACS_THROW(SpawnException(os.str())) ;   // PRQA S 3081
		}
        
		// lock the spawn		
		ThreadSafe::Lock lock(_procMutex) ;
		
		// HERE IS THE FORK
		pid_t pid = fork() ;
		if (pid < 0) { // error - cannot spawn 
			ostringstream os ;
			os << "Cannot spawn \"" << _process << "\" Error is:" << pid ;
			ACS_THROW(SpawnException(os.str())) ;   // PRQA S 3081
		}
		if (pid == 0) { // CHILD PROCESS
			// reset the _procMutex in the child
			// it should not be needed but in case of failures can avoid deadlocks
			// it will not throw in case of failures
			_procMutex.reInit(false);
			// reset also the streams global mutex to avoid deadlocks in case
			// any debug message is issued between fork and exec
			exostream::globalMutex().reInit(false);
			// time to spawn the new executable
			spawnProcChild(detached) ;
		}
		else { // parent
			_spawned = true ;
			_spawning = false ;
			
			// prepare the guard
			_procGuard = new WaitPidGuard(pid,_pGroupLeader) ;

			if (msecDelay > 0 ) { // have we to wait a bit ?
				Timer::delay(msecDelay) ;
			}

			// This was debug level 2 - Replace ACS_VLO_VERB with 2 if needed
			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "process " << _process << " pid is: " << pid); 

			ACS_CLASS_BGN_DEBUG(100) 
				ostringstream c ;
				c << "strace -p" << pid ;
				system(c.str().c_str()) ;
			ACS_CLASS_END_DEBUG
		}
	}
	catch(SpawnException &) {
		_spawning = false ;
		throw ;
	}
	catch(exception &e) {
		SpawnException x(e,"Spawning " + _process) ;
		_spawning = false ;
		ACS_THROW(x) ;
	}
	catch(...) { // this should never happen ... unless we are in a thread an it was killed 
		_spawning = false ;
		throw ;
	}
}


void SpawnProc::spawn(bool detached, unsigned int delay, bool autoKill)
{

	// This was debug level 2 - Replace ACS_VLO_VERB with 2 if needed
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, 
			   "spawning: (detached : " << boolalpha << detached 
		    << ") delay " << delay << " [ms]"); 

	if (!spawned()) {
		// just spawn 
		spawnProc(delay, detached) ;
		// write down the values
		_detached = detached ;
		_autoKill = autoKill ;
	}
}

bool SpawnProc::isPGroupLeader() const  // PRQA S 4120
{
	return _pGroupLeader ;
}


// warning can be called only if not yet spawned
void SpawnProc::setPGroupLeader(bool set) 
{
	if (spawned()) { // already spawned ? 
	 
	 	// you cannot change the group leader status
		ostringstream os ;
		os << "Process \"" << _process << "\" was already spawned."
		   << " pid is: " << pid() ;
		ACS_THROW(SpawnException(os.str())) ; // PRQA S 3081
	}
	_pGroupLeader = set ;
}

WaitPidGuard* SpawnProc::getProcGuard () const
{
	// avoid the pointer change while reading it
	Lock lock (_procMutex);
	return _procGuard; // PRQA S 4628
}

WaitPid::ProcArgs &SpawnProc::args()  // PRQA S 4120
{
	return _args ;                    // PRQA S 4024  
}

void SpawnProc::args(const WaitPid::ProcArgs &a) 
{
	if (spawned()) { // already spawned ? 
	
		// you cannod add arguments
		ostringstream os ;
		os << "Process \"" << _process << "\" was already spawned."
		   << " pid is: " << pid() ;
		ACS_THROW(SpawnException(os.str())) ; // PRQA S 3081
	}
	
	_args =a ;
}



void SpawnProc::cleanEnvSettings()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	envSet_.clear();
	envClr_.clear();

}
void SpawnProc::addEnvSetting ( const string& variable, const string& value)
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Adding env variable " << variable << " set to " << value);
	if ( variable.find('=') != string::npos )
	{
		// '=' is not allowed in the variable name
		ACS_THROW ( exIllegalValueException ( "The variable name \"" + variable + "\" is not allowed " ) );
	}
	envSet_[variable] = value;
}

void SpawnProc::addEnvUnset   ( const string& variable)
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Adding env variable " << variable << " to be UNSET");
	if ( variable.find('=') != string::npos )
	{
		// '=' is not allowed in the variable name
		ACS_THROW ( exIllegalValueException ( "The variable name \"" + variable + "\" is not allowed " ) );
	}
	envClr_.insert (variable);
}



// ************************************************************* //
// ************************************************************* //

CmdIORedir::CmdIORedir(const string &command, 
                       const string &inpath, 
                       const string &outpath, 
                       const vector<string> &arguments) :
	SpawnProc (command, false),
    fd_()
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Method called");
    init(command, inpath, outpath, arguments) ;
}

CmdIORedir::CmdIORedir(const string &command, 
                       bool resolvSymLink,
                       const string &inpath, 
                       const string &outpath, 
                       const vector<string> &arguments) :
	SpawnProc (command, resolvSymLink),
    fd_()
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Method called");
    init(command, inpath, outpath, arguments) ;
}


CmdIORedir::~CmdIORedir() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Method called. Releasing file descriptor");

    // First of all release the fd_ (is an input) to avoid hangs on piped commands
    fd_.release();

    // PRQA S 4631 L2
    try {
		waitProcCompletion();
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    // PRQA L:L2
}


CmdIORedir::operator int() // PRQA S 4211
{
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "called");

	ACS_COND_THROW(fd_ == 0, exIOException("CmdIORedir: not open")) ; // PRQA S 3050, 3081
	return fd_ ; // PRQA S 3050
}


bool																		/* FALSE: stuck running. TRUE: Success. exit_code/signal are reliable. */ 
		CmdIORedir::hangup(int& exit_code, int& exit_signal, const size_t& timeout_s) const
{
/**
	Used to send a EOF by closing the descriptor.
	This method should be done if the process keeps looping "until EOF" or "until valid data found" (and the data does not arrive or is corrupted).
	- If the process is already stopped, it just return the exit code/exit signal.
	- If the process is running, the descriptor is closed and a check is performed (up to a timeout) to extract exit code / signal.
	- If eventually the process is still running (stuck?), TRUE is returned and exit code/signal are not valid.
*/ 
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Method called, timeout [s]: " << timeout_s);
	
	exit_code = exit_signal = 0 ; 
	
	// If the process is still running, send a EOF to the stream -- this should terminate the execution.
	if (true == isRunning())
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Still running, descriptor is closed [EOF will be sent].");
		
		fd_.release() ; // Close (and send a EOF) to the open descriptor
	}

	// Wait for AT MAXIMUM timeout_s [second] (step is 0.5s) for the execution to terminate.
	for (size_t i=0; ((true == isRunning()) && (i<(2*timeout_s))); i++)
	{
		usleep(500*1000) ; // 0.5s
	}
	
	// WHEN HERE, THE PROCESS SHOULD NOT BE RUNNING ANYMORE (unless it's, probably, stuck).
	
	// Compute, once, the return value
	const bool success = (not isRunning()) ;

	if (true == success)
	{
		// If _NOT_ running, the exit code or signal are captured and returned.
		int tmp ; 
		if ( (tmp = 0) || (true == exitCode(tmp)) )			{ exit_code = tmp ; }
		else if ( (tmp = 0) || (true == exitSignal(tmp)) )	{ exit_signal = tmp ; }
		
		// Note to code: "(tmp = 0) ||" is simply used to reset tmp to 0 before passing it to the exit-check function
	} 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ostringstream o ;
		o << "Returning: " << (success?"T":"F") ;
		if (false == success)	{ o << "  (maybe process is stuck...?)" ; }
		else					{ o << "  exit code: " << exit_code << "  exit signal: " << exit_signal ; }
		excerr << o.str() ; 
	ACS_CLASS_END_DEBUG
	
	return success ; 
}


void CmdIORedir::init(const string &command, 
                       const string &inpath, 
                       const string &outpath, 
                       const vector<string> &arguments) 
{
    using pattern::FDGuard ;
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "cmd:" << command
		    << " inpath: \"" << inpath << "\""
		    << " outpath: \"" << outpath<< "\"" 
		    << " #argumens: " << arguments.size() ); 

    FDGuard close_after_spawn ;
    FDGuard inFd ;
	if (not outpath.empty()) { // write on outpath 
		int fd = open(outpath.c_str(), O_WRONLY| O_CREAT, 0666) ;
		if (fd < 0 ) {
			ACS_THROW(exIOException("Cannot open \"" + outpath + "\"", errno)) ; // PRQA S 3081
		}
        inFd = FDGuard(fd) ; // PRQA S 3081
		setOutDescriptor(fd) ; // write on out descriptor
        if (inpath.empty()) { // not requested to read from file 
		    int outfiledes[2] ; // 1 to write       // PRQA S 4403
		    pipe(outfiledes) ;
		    setInDescriptor(outfiledes[0]) ; // should read from someone using this class as output
		    fd_ = FDGuard(outfiledes[1]) ; // prepare output // PRQA S 3081 2
            close_after_spawn = FDGuard(outfiledes[0]) ; // not longer needed after the fork
        }
    }
    
    FDGuard outFd ;
    if (not inpath.empty()) {
		ACS_COND_THROW(!File::exists(inpath) || !File::isReadable(inpath),       // PRQA S 3081
					exIOException("File: " + inpath + " does not exists or unreadable")) ;
		int fd = open(inpath.c_str(), O_RDONLY) ;
		if (fd < 0 ) {
			ACS_THROW(exIOException("Cannot open for input\"" + inpath + "\"", errno)) ; // PRQA S 3081
		}
        outFd = FDGuard(fd) ; // PRQA S 3081
		setInDescriptor(fd) ; // read from file
        if (outpath.empty()) { // not requested to write 
		    int outfiledes[2] ; // 1 to write      // PRQA S 4403
		    pipe(outfiledes) ;
		    setOutDescriptor(outfiledes[1]) ; // should write to this class to be used as out
		    fd_ = FDGuard(outfiledes[0]) ; // prepare input // PRQA S 3081 2
            close_after_spawn = FDGuard(outfiledes[1]) ; // not longer needed after the fork
        }
    }

    // add args ;
    size_t args_size = arguments.size() ;
    for(size_t i=0; i < args_size; ++i) {
        args()[i] = arguments[i] ;
    }

	try {

		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "spawning");

		spawn() ;
        close_after_spawn.release() ;
        inFd.release() ;
        outFd.release() ;
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "spawn succeded");
	}
	catch(exception &x) {
		ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "spawn failed: " << x);
        
		fd_.release() ;
        throw ;
	}
}





_ACS_END_NAMESPACE

