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
	Revision 2.9  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.8  2013/11/12 16:45:37  marfav
	waitProcCompletion waitPid blocking call discontinued and implemented by condition
	
	Revision 2.7  2013/11/07 16:32:18  marfav
	Thread safe implementation to avoid problems in waitpid syscall
	
	Revision 2.6  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.5  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.4  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.3  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.2  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.1  2007/04/19 12:52:30  marpas
	no exception during atExit and garbage collection
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/07/25 15:42:35  ivafam
	Header format fixed
	
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

#ifndef _WaitPid_H_
#define _WaitPid_H_

#include <exException.h>
#include <vector>



_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////////
//							//
// classe WaitPid			//
//							//
//////////////////////////////


class WaitPid // PRQA S 2108, 2109, 2195
{
private:

	class GarbageCollAtExit {
	public:
		GarbageCollAtExit() ;
		~GarbageCollAtExit() throw() ;
	private:
		GarbageCollAtExit(const GarbageCollAtExit &) ;
		GarbageCollAtExit &operator=(const GarbageCollAtExit &) ;

	} ;

public:

	class ProcArg // PRQA S 2109
	{
	public:
		explicit ProcArg(const std::string &, bool removeAtExit = false) ;
		~ProcArg() throw() ;
		
		ProcArg &operator=(const std::string &) ;

		ProcArg(const ProcArg &) ;
		ProcArg &operator=(const ProcArg &) ;
		
		void removeAtExit(bool) throw() ;
		bool removeAtExit() const throw() ;
		
		void removeIfNeeded() const ;
		
		const std::string &arg() const throw() ; 
	private:
        ProcArg() ; // not implemented
	private:
		bool _removeAtExit ;
		std::string _arg ; 
		
	} ;
	

	class ProcArgs {
	public:
		ProcArgs() throw() ;
		~ProcArgs() throw() ;
		
		ProcArgs(const ProcArgs &) ;
		ProcArgs &operator=(const ProcArgs &) ;


		const ProcArg &operator[](size_t) const ;
		ProcArg &operator[](size_t) ;
		
		size_t size() const throw() ;
				
		void removeIfNeeded() const ;

	private:
		std::vector<ProcArg> _args ;
	} ;

public:
	WaitPid(pid_t, bool gLeader) ;
	~WaitPid() throw() ;
	
	bool exitCode(int &) const ;
	bool exitSignal(int &) const ;

	bool isRunning() const ;
	int kill(int signal = 9) const ; // PRQA S 4400
	// only if groupleader ctor called with gLeader true
	int killGroup(int signal = 9) const ;
	pid_t pid() const ;	
	void registerArgs(const ProcArgs &) ;
	
	static void killall() ;

	void regUser() ;
	void unregUser() ;

	bool waitPid(bool block=false) const ;

private:
	// declared but not implemented to prevent their use
	WaitPid() ;
	WaitPid(const WaitPid &) ;
	WaitPid &operator=(const WaitPid &) ;
	
	static void append(WaitPid *) ;
	static void remove(WaitPid *) ; // PRQA S 2502
	static void check() ;
	static void garbageCollection(bool evenIfRunning = false) ;
	static void unsafeGarbageCollection(bool evenIfRunning = false) ;

	bool isUsed() const ;
    bool internalWaitPid(bool block) const;


private:

	static ThreadSafe  _procMutex ;

	static std::vector<WaitPid*> _procs ;
	static GarbageCollAtExit _garbageCollAtExit;

	pid_t _pid ;
	mutable int _statptr ;
	mutable bool _exitCodeValid ;
	bool _used ;
	bool _gLeader ;
	ProcArgs _args ;
    ThreadSafe _waitPidMutex;
    ThreadSafe _exitCodeValidMutex;

	ACS_CLASS_DECLARE_DEBUG_LEVEL (WaitPid);
} ;


_ACS_END_NAMESPACE

#endif	// _WaitPid_H_
