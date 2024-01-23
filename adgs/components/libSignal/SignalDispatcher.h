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

#ifndef _Signal_H_
#define _Signal_H_


#include <acs_c++config.hpp>

#include <exException.h>
#include <exRTDebuggable.h>

#include <unistd.h>
#include <csignal>

#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

#define SIG_CALLSTACK (SIGRTMIN+5) // PRQA S 1026 
#define SIG_PHOENIX (SIGRTMIN+7) // +6 is for dbClient To be moved here

using pattern::Singleton ;

/** 
 *\brief The SignalDispatcher class manages SIGTERM and SIGINT. 
 *
 * The SignalDispatcher class is responsible to catch the asynchronous SignalDispatcher SIGTERM,SIGINT and SIGUSR1.
 * When a program makes an instance of this class the signals are masked to all threads of the program and a counter
 * of each occurence is available everywhere using a singleton pattern
 */


class SignalDispatcher : public pattern::Singleton<SignalDispatcher> // PRQA S 2109, 2153
{
	friend SignalDispatcher* Singleton<SignalDispatcher>::instance(); // PRQA S 2107

public:
	/*
	 * \brief This class provide a default factory which build the SignalDispatcher class. 
	 *
	 * A simple user on SignalDispatcher library
	 * needs to create an instance of this class in the main  
	 */

	virtual ~SignalDispatcher() ACS_NOEXCEPT ;
	int getSigTerm() const ;
	int getSigInt() const ;
	int getSigPhoenix() const;
	int getSigUsr1() const;

	static void catchSigPhoenix(int); // PHOENIX management simulates a signal

	static void setCorePath (const std::string&) ACS_NOEXCEPT;
	void enableCoreEmission () ACS_NOEXCEPT;
	void disableCoreEmission () ACS_NOEXCEPT;
	bool coreEmissionEnabled() const ACS_NOEXCEPT;


protected:
	explicit SignalDispatcher();

private:
	typedef void(*Handler)(int) ;
	void installBadSignalHandlers(bool install) ;	
	void installHandler(int,Handler) ;
	void installDefaultHandlers() ;
	static void catchSigTerm(int) ; // SIGTERM handler
	static void catchSigInt(int);  	// SIGINT handler
	static void catchBadSignal(int);  // SIGABRT handler
	static void catchSigUsr1(int);  // SIGUSR1 handler
	static void catchPrintStackSignal(int); // CALLSTACK_SIG handler (defined in acs_c++config.hpp)
	SignalDispatcher(const SignalDispatcher&) = delete ;
	SignalDispatcher& operator=(const SignalDispatcher&) = delete ;
	SignalDispatcher(SignalDispatcher&&) = delete ; // PRQA S 2038 2 
	SignalDispatcher& operator=(SignalDispatcher&&) = delete ;


	struct sigaction _new_action ;
	struct sigaction _old_action ;
	int _sigTermCounter;
	int _sigIntCounter;
	int _sigPhoenixCounter;
	int _sigUsr1Counter;

	static std::string _corePath;
	ThreadSafe _mutex;

	static pthread_t _stackPrintingThread;
	static std::ostringstream _stackStream;
	static std::map<pthread_t, std::string> _liveThreadsMap;
	bool _emitCoreFile;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(SignalDispatcher) ;
};


_ACS_END_NAMESPACE


#endif //_Signal_H_
