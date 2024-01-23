// PRQA S 1050 EOF
// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Exprivia SPA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: SignalDispatcher Management Library $

*/

#include <SignalDispatcher.h>
#include <exStackTrace.h>
#include <Filterables.h>
#include <SignalV.h>

#include <algorithm>
#include <sys/resource.h>   // PRQA S 1013
#include <sys/types.h>      // PRQA S 1013 
#include <sys/stat.h>       // PRQA S 1013 

#include <unistd.h>
#include <acs_c++config.hpp>
#include <Thread.h>


_ACS_BEGIN_NAMESPACE(acs)

using std::string ;
using std::ostringstream ;
using std::map ;
using std::vector ;
using std::exception ;
using std::boolalpha ;
using std::hex ;

namespace { // unnamed
    SignalV Version;
} ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(SignalDispatcher)

//
//
//	Singleton Implementation
//
//


string SignalDispatcher::_corePath = "/tmp"; // PRQA S 4636 # in case string ctor throws we are happy to terminate

pthread_t SignalDispatcher::_stackPrintingThread = 0;
ostringstream SignalDispatcher::_stackStream; // PRQA S 4634 #  in case string ctor throws we are happy to terminate
map<pthread_t, string> SignalDispatcher::_liveThreadsMap;

SignalDispatcher::SignalDispatcher():
pattern::Singleton<SignalDispatcher> (),
	_new_action(),
	_old_action(),
	_sigTermCounter(0),
	_sigIntCounter(0),
	_sigPhoenixCounter(0),
	_sigUsr1Counter(0),
	_mutex(),
	_emitCoreFile(true)
{

    // initialize debug level
	installDefaultHandlers() ;

	installBadSignalHandlers(true) ;

    // try to set the core path
    setCorePath (acs::base::defaultCorePath()); // PRQA S 2839 # it is guaranteed well formed 
}

SignalDispatcher::~SignalDispatcher() ACS_NOEXCEPT {}

// PRQA S 1040 EOF
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
#pragma GCC diagnostic ignored "-Wold-style-cast"

void SignalDispatcher::installHandler(int signum, Handler handler) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "signum: 0x" << hex << signum << " handler: 0x" << handler) ;
    /* Set up the structure to specify the new action. */
    _new_action.sa_handler = handler; // PRQA S 0228
    sigemptyset (&_new_action.sa_mask);
    _new_action.sa_flags = 0;


    sigaction (signum, 0, &_old_action);

    if (_old_action.sa_handler != SIG_IGN)  // PRQA S 3039, 3048, 3080, 3081
    {
        if( sigaction (signum, &_new_action, 0) != 0)
        {
            ostringstream msg ;
			msg << " Failed to set handler for SignalDispatcher" << signum ;
            ACS_THROW(exIllegalValueException(msg.str(),errno)); //PRQA S 3224
        }
    }
}
// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"


void SignalDispatcher::installDefaultHandlers() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Installing default handlers for all signals") ;
	try {
			installHandler(SIGTERM, catchSigTerm) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGTERM handler installed") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
			installHandler(SIGINT, catchSigInt) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGINT handler installed") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
			installHandler(SIGUSR1, catchSigUsr1) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGUSR1 handler installed") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
			installHandler(SIG_PHOENIX, catchSigPhoenix) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIG_PHOENIX handler installed") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
			installHandler(SIG_CALLSTACK, catchPrintStackSignal) ; // PRQA S 1021
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIG_CALLSTACK handler installed") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}
    
	try {
			installHandler(SIGPIPE, SIG_IGN) ; // PRQA S 1021
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGPIPE handler installed (will be ignored)") ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}
}

void SignalDispatcher::installBadSignalHandlers(bool install) // PRQA S 6001 # Just a try-catch chain
{
	try {
		installHandler(SIGHUP, install ? catchBadSignal : 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGHUP handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
		installHandler(SIGILL, install ? catchBadSignal : 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGILL handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
		installHandler(SIGABRT, install ? catchBadSignal : 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGABRT handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
		installHandler(SIGFPE, install ? catchBadSignal: 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGFPE handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
		installHandler(SIGSEGV, install ? catchBadSignal: 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGSEGV handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}

	try {
		installHandler(SIGBUS, install ? catchBadSignal: 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGBUS handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}
	
	try {
		installHandler(SIGSYS, install ? catchBadSignal: 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGSYS handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}
	
	try {
		installHandler(SIGPWR, install ? catchBadSignal: 0) ; // PRQA S 3380
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SIGPWR handler installed: " << boolalpha << install) ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x)
	}
}

// SIGTERM SignalDispatcher handler
void SignalDispatcher::catchSigPhoenix(int s)
{
	++(SignalDispatcher::instance()->_sigPhoenixCounter);
}


// SIGTERM SignalDispatcher handler
void SignalDispatcher::catchSigTerm(int s)
{
	++(SignalDispatcher::instance()->_sigTermCounter);
}

// SIGINT SignalDispatcher handler
void SignalDispatcher::catchSigInt(int s)
{
	++(SignalDispatcher::instance()->_sigIntCounter);
}

// SIGUSR1 SignalDispatcher handler
void SignalDispatcher::catchSigUsr1(int s)
{
	++(SignalDispatcher::instance()->_sigUsr1Counter);
}

// Set the 
void SignalDispatcher::setCorePath (const string& path) ACS_NOEXCEPT
{
    bool canWrite = false;

	struct stat s; // PRQA S 4102
	if( stat(path.c_str() , &s) == 0 )	// stat file in order to obtain st_mode flag
	{
        if (S_ISDIR(s.st_mode)) // PRQA S 3600 # it is an octal
        {
            // ok it is a directory.. can I write into it?
            canWrite = ( access(path.c_str(),W_OK) == 0 );
        }
	}

    
    if (canWrite)
    {
        // OK Can use it to write the core files
        _corePath = path;
    }
    else
    {
        ACS_LOG_WARNING("Unable to write core files in " << path << " - Core files will be placed in /tmp") ;
        _corePath = "/tmp";
    }
	
}

// SIG_CALLSTACK SignalDispatcher handler
void SignalDispatcher::catchPrintStackSignal(int)
{
	pthread_t myself = ::pthread_self();
	if (_stackPrintingThread != myself)
	{
		_stackStream << "Stack trace for thread " << myself << " - \"" << _liveThreadsMap[myself] << "\"\n";
		_stackStream << exStackTrace::getStack(true) << "\n" ;
		if ( _stackPrintingThread != 0 )
		{
			// someone asked me to print my stack
			// and was not myself
			// send back a SignalDispatcher to wake it up
			::pthread_kill(_stackPrintingThread, SIG_CALLSTACK) ; // PRQA S 1021
		}
	}
}


// SIGINT SignalDispatcher handler
void SignalDispatcher::catchBadSignal(int s) // PRQA S 6002 # Splitting will result in worse maintainability
{

	_liveThreadsMap = exStackTrace::getLiveThreadIdSet();
	chdir(_corePath.c_str()) ; // change directory to /tmp
	
	// set no limit to the core file
	struct rlimit lim ; // PRQA S 4102
	getrlimit(RLIMIT_CORE, &lim) ;
	if (SignalDispatcher::instance()->coreEmissionEnabled())
	{
		lim.rlim_cur = RLIM_INFINITY ;  // PRQA S 2900, 0476
	}
	else
	{
		// No core file emission
		lim.rlim_cur = 0 ;  
	}
	setrlimit(RLIMIT_CORE, &lim) ; // PRQA S 2976 # it has been initialized by getrlimit

	// install null handlers to bad SignalDispatcher
	SignalDispatcher::instance()->installBadSignalHandlers(false) ;

	time_t now_t = time(0) ;

	_stackStream << ctime(&now_t) << " PID: " << getpid() << " SignalDispatcher " << s << " THREAD: " << pthread_self() << "\n";

	if (lim.rlim_cur != 0)
	{
		_stackStream << " core in: " << _corePath << "/core." << getpid();
	}
	
	_stackStream << " - stack follow\n";

	// First of all print my stack
	catchPrintStackSignal(s);

	// now start managing the others
	_stackPrintingThread = ::pthread_self();

	map <pthread_t, string>::iterator it = _liveThreadsMap.begin();

	// SignalDispatcher mask for the sisgsuspend system call
	sigset_t mask; // PRQA S 4102
	sigfillset(&mask);
	sigdelset(&mask, SIG_CALLSTACK); // PRQA S 1021

	// SignalDispatcher mask for the sigmask system call
	sigset_t blockmask; // PRQA S 4102
	sigemptyset(&blockmask);
	sigaddset(&blockmask, SIG_CALLSTACK); // PRQA S 1021

	// SignalDispatcher mask for restoring the original mask after the sigsuspend
	sigset_t oldmask; // PRQA S 4102

	while (it != _liveThreadsMap.end() )
	{
		// raise the SignalDispatcher on the thread and wait for the return SignalDispatcher
		// that will be issued at the end of the generation of the thread's stack
		// in the common _stackStream stream
		if (it->first != _stackPrintingThread)
		{
			// queue any SIG_CALLSTACK delivered to this thread
			// this will allow sigsuspend to catch any SignalDispatcher delivered in the meantime
			int sigmaskresult = pthread_sigmask(SIG_BLOCK, &blockmask, &oldmask);

			// Do not suspend execution if no SignalDispatcher has been sent and no mask was set
			if ((::pthread_kill (it->first, SIG_CALLSTACK) == 0) && (sigmaskresult == 0) ) // PRQA S 1021
			{
				// wait for SIG_CALLSTACK on this thread
				sigsuspend(&mask);
			}
			// Restore the SignalDispatcher mask
			pthread_sigmask(SIG_BLOCK, &oldmask, 0);
		}
		++it;
	}

	// Print all the stacks in one single message to avoid message interleaving from the running threads
	ACS_LOG_ERROR(_stackStream.str()) ;
	exostream::getDefaultOutLogger().flush() ;


	switch(s) { // PRQA S 4018, 4019 # i prefer the switch for readability
	case SIGHUP:
	case SIGPWR:
	case SIGABRT:   // PRQA S 4066
        {
#ifdef TEST_COVERAGE
            exit(s) ;
#else
		    abort() ;
#endif
        }
    // followthrough
	default:        // PRQA S 4068
        {
#ifdef TEST_COVERAGE
            exit(s) ;
#else
		    kill(getpid(),s) ;
#endif
            break ;
        }
	}
}

int 
SignalDispatcher::getSigTerm() const    // PRQA S 4120
{
    return _sigTermCounter;
}

int 
SignalDispatcher::getSigInt() const       // PRQA S 4120
{
    return _sigIntCounter;
}

int 
SignalDispatcher::getSigPhoenix() const       // PRQA S 4120
{
    return _sigPhoenixCounter;
}

int 
SignalDispatcher::getSigUsr1() const       // PRQA S 4120
{
    return _sigUsr1Counter;
}

void SignalDispatcher::enableCoreEmission () ACS_NOEXCEPT
{
	_emitCoreFile = true;
}

void SignalDispatcher::disableCoreEmission () ACS_NOEXCEPT
{
	_emitCoreFile = false;
}

bool SignalDispatcher::coreEmissionEnabled() const ACS_NOEXCEPT
{
	return _emitCoreFile;
}


_ACS_END_NAMESPACE

