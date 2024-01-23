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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.4  2013/11/12 16:45:37  marfav
	waitProcCompletion waitPid blocking call discontinued and implemented by condition
	
	Revision 2.3  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.2  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.1  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.4  2004/07/15 14:29:27  envisat
	arguments erasure policy changed.
	
	Revision 1.3  2004/03/31 16:16:05  marpas
	small bug fixed
	
	Revision 1.2  2004/03/31 16:15:17  marpas
	session concept implemented
	SpawnProc::setPGroupLeader method can be called BEFORE the spawn to create
	                           a new process group
	SpawnProc::killGroup method will propagate a signal to the whole group, only if
	                     the SpawnProc instance was spawned after setting the groupLeader flag.
	SpawnProc::killGroup and SpawnProc::kill methods now will set the errno even if they do not
	                                         really attempt to send the signal.
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#include <WaitPidGuard.h>
#include <WaitPid.h>


_ACS_BEGIN_NAMESPACE(acs)

WaitPidGuard::WaitPidGuard(pid_t p, bool gLeader) : 
	_wp(0)
{
	_wp = new WaitPid(p, gLeader) ;
	_wp->regUser() ;
	
}

WaitPidGuard::~WaitPidGuard() throw() 
{
// PRQA S 4631 L1
    try {
	    _wp->unregUser() ;
// do not delete _wp, it is removed by the garbageCollection() ;
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}

bool WaitPidGuard::exitCode(int &r) const 
{
	return _wp->exitCode(r) ;
}

bool WaitPidGuard::exitSignal(int &r) const 
{
	return _wp->exitSignal(r) ;
}


bool WaitPidGuard::isRunning() const 
{
	return _wp->isRunning() ;
}

int WaitPidGuard::kill(int signal) const 
{
	return _wp->kill(signal) ;
}

int WaitPidGuard::killGroup(int signal) const 
{
	return _wp->killGroup(signal) ;
}

pid_t WaitPidGuard::pid() const 
{
	return _wp->pid() ;
}
	
void WaitPidGuard::registerArgs(const WaitPid::ProcArgs &a) // PRQA S 4211
{
	_wp->registerArgs(a) ;
}

bool WaitPidGuard::waitPid (bool block) const
{
    return _wp->waitPid (block);
}
	

_ACS_END_NAMESPACE
