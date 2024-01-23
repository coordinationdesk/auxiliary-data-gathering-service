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

	$Prod: A.C.S. Signal Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/17 11:32:01  marpas
	adopting libException 5.x standards
	
	Revision 2.4  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.3  2012/11/06 17:44:33  marpas
	Quality improved, coverage test done
	
	Revision 2.2  2012/02/09 14:30:28  marpas
	refactoring
	
	Revision 2.1  2011/01/24 15:31:19  marpas
	Mutex.h does not longer exixts
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/14 12:13:38  marfav
	Added SigMask classes to library
	

*/


#ifndef _SigMaskManagerSingleton_h_
#define _SigMaskManagerSingleton_h_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <exException.h>

#include <map>

#include <csignal>
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class SigMaskManagerSingleton : public pattern::Singleton<SigMaskManagerSingleton> // PRQA S 2109, 2153
{
    friend SigMaskManagerSingleton* Singleton<SigMaskManagerSingleton>::instance(); // PRQA S 2107

public:
	/*! class ThreadException declaration */
	exDECLARE_EXCEPTION(SigMaskManagerException,exException) ;  // PRQA S 2131, 2502, 2634
	virtual ~SigMaskManagerSingleton() ACS_NOEXCEPT ;


public:
	// AddGuard and removeGuard are used to Mask and unMask a signal
	// Each process_id (each thread) has its own mask for the signals
	// Different pids are managed using different guard counters
	// When the first guard is issued, the signal handler is set to SIG_IGN and the original one is saved
	// When the last guard has been removed the original signal handler is restored
	void addGuard(int signum);
	void removeGuard(int signum) ;

protected:
	SigMaskManagerSingleton();
private:
	SigMaskManagerSingleton(const SigMaskManagerSingleton &) = delete ;
	SigMaskManagerSingleton & operator= (const SigMaskManagerSingleton &) = delete ;
	SigMaskManagerSingleton(SigMaskManagerSingleton &&) = delete ; // PRQA S 2038 2
	SigMaskManagerSingleton & operator= (SigMaskManagerSingleton &&) = delete ;
private:
	// Guard counter
	// divided by signal number and pid
	std::map<int, std::map<pid_t, int> > _guardCount;

	// Original handler repository
	// divided by signal number and pid
	std::map<int, std::map<pid_t, struct sigaction> > _oldHandlers;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SigMaskManagerSingleton) ;
};

_ACS_END_NAMESPACE


#endif // _SigMaskManagerSingleton_h_


