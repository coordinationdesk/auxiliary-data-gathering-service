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
	Revision 2.19  2018/01/09 16:53:33  marfav
	ECICMF-194
	Added support for env variable setting and unsetting
	between fork and exec (in the child process)
	
	Revision 2.18  2015/11/27 15:44:48  marpas
	qa warning fixed
	
	Revision 2.17  2014/06/26 16:24:28  marpas
	exitCause method added
	
	Revision 2.16  2014/04/02 11:32:25  marpas
	new throwing macros adopted
	if ifucstream (uncompress) does not find the executable, will try with gunzip
	[io]fucstream classes refactorized to not deal directly with pointers (auto_ptr used)
	SpawnProc ctor has a new flag defaulted to true to resolve symbolic link on commands.
	CmdIORedir class uses the above flag to false (because compress and uncompress commands are the same executable, but their behaviour depends on the argv[0] i.e. the summoned name)
	
	Revision 2.15  2013/12/09 11:27:00  marpas
	splitting spawnProc method in two: all code for the child process is now separate for easiest maintenance
	
	Revision 2.14  2013/11/29 14:35:28  marpas
	doc fixed & test improved
	
	Revision 2.13  2013/11/28 18:42:16  marpas
	doc improved & minor changes
	
	Revision 2.12  2013/11/28 15:40:05  marpas
	CmdIORedir is able to redirect simultaneously input and output on files
	When just one of the two streams is redirected, the class act as the other end of a pipe,
	either input or output according to which stream is redirected.
	If both ends are redirected to file system, the class cannnot act as pipe end
	
	Revision 2.11  2013/11/25 13:17:37  marfav
	WaitProcCompletion implemented by waitPid blocking call (restored)
	_procGuard pointer is now used in thread safe way
	
	Revision 2.10  2013/11/22 20:18:31  marpas
	coding best practices applied
	
	Revision 2.9  2013/11/12 16:45:37  marfav
	waitProcCompletion waitPid blocking call discontinued and implemented by condition
	
	Revision 2.8  2013/07/25 15:24:59  marfav
	ThreadedProcWrapper embedded in SpawnProc to allow waiting timed conditions
	
	Revision 2.7  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.6  2012/12/02 13:43:07  marpas
	qa rules
	
	Revision 2.5  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.4  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.3  2009/12/22 17:36:06  marpas
	CmdIORedir class added
	
	Revision 2.2  2009/12/22 16:01:04  marpas
	work in progress
	
	Revision 2.1  2009/04/02 13:10:49  marpas
	exMailStream class introduced
	some quality issues fixed
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/07/25 15:42:34  ivafam
	Header format fixed
	
	Revision 1.10  2005/05/20 15:18:31  fracar
	fixed problem with standard descriptors when in detached mode
	
	Revision 1.9  2004/07/15 14:29:27  envisat
	arguments erasure policy changed.
	
	Revision 1.8  2004/04/14 17:29:33  marpas
	ThreadSafe used
	
	Revision 1.7  2004/03/31 16:15:17  marpas
	session concept implemented
	SpawnProc::setPGroupLeader method can be called BEFORE the spawn to create
	                           a new process group
	SpawnProc::killGroup method will propagate a signal to the whole group, only if
	                     the SpawnProc instance was spawned after setting the groupLeader flag.
	SpawnProc::killGroup and SpawnProc::kill methods now will set the errno even if they do not
	                                         really attempt to send the signal.
	
	Revision 1.6  2003/10/10 16:18:36  marpas
	Because threads but the main one could have blocked some signals (see Thread::maskedSignals
	static method), processes spawned from those threads would unblock those signals.
	
	Revision 1.5  2003/07/11 12:57:34  marpas
	exException::clone implemented
	
	Revision 1.4  2003/07/01 08:04:48  marpas
	Thread safety improved
	
	Revision 1.3  2003/04/30 10:05:42  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2002/11/20 10:38:00  marpas
	Improved tests.
	SpawnProc destructor will not kill the process if it is not running any longer.
	
	Revision 1.1.1.1  2002/10/08 13:59:08  marpas
	libProc import
	

*/

#ifndef _SpawnProc_H_
#define _SpawnProc_H_

#include <WaitPid.h>
#include <exException.h>
#include <exMacros.h>
#include <Guard.hpp>
#include <sys/resource.h> // PRQA S 1013

#include <string>
#include <map>
#include <set>


_ACS_BEGIN_NAMESPACE(acs)

class WaitPidGuard ;
class ThreadedProcWrapper ;

//////////////////////////////
//							//
// classe SpawnProc			//
//							//
//////////////////////////////

class SpawnProc // PRQA S 2109
{
public:

	/*! class SpawnException declaration */
	exDECLARE_EXCEPTION(SpawnException,exException) ; // PRQA S 2131, 2502


public:
	explicit SpawnProc(const std::string &, bool resolvSymLink = true) ;
	virtual ~SpawnProc() throw() ;
	
	bool isRunning() const ;
	int kill(int signal = 9) const ; // PRQA S 2502, 4400
	int killGroup(int signal = 9) const ; // PRQA S 4400
	pid_t pid() const ;
	bool exitCode(int &) const ;
	bool exitSignal(int &) const ;

    /*- throws exception if the process has never started or is detached.
      returns true if the process exited and set in xit the exit code
      returns false if the process was signaled and set in sig the signal it received
     */
    bool exitCause(int &xit, int &sig) const ;
	
	void setInDescriptor(int) ;
	void setOutDescriptor(int) ;
	void setErrDescriptor(int) ;
	
	
	virtual void spawn(bool detached = false, unsigned int delay = 0, bool autoKill = true) ;
	bool spawned() const ;
	
	WaitPid::ProcArgs &args() ;
	void args(const WaitPid::ProcArgs &) ;
	
	bool isPGroupLeader() const ;
	// warning can be called only if not yet spawned
	void setPGroupLeader(bool set=true) ;

	// return if the process is running or not
	// true means not running
	// false means still running
	bool waitProcCompletion(bool emitExceptionIfNotSpawn = true); 

	//* Uses a thread to make a non blocking call to waitProcCompletion
    //* \returns false if not running
	bool waitExitOrTimeout (unsigned long msecs);

	/**
	   Cleans the attributes related to the env variables to set before the proc spawn
	**/
	void cleanEnvSettings();
	/**
	   Adds a new env variable to be set before starting the spawned proc
	   variable names are not allowed to contain the character '='
	   an exception will be thrown in case of not allowed variable names
	**/
	void addEnvSetting ( const std::string& variable, const std::string& value);

	/**
	   Adds a new env variable to be un-set before starting the spawned proc
	   unset are performed after the set, so they win in case of conflicting settings
	**/
	void addEnvUnset   ( const std::string& variable);

	/** Tag name getter */
	const std::string& getTagName() const;
	/** Tag bane setter */
	void setTagName(const std::string& tagName);

private:
	void spawnProc(unsigned int msecDelay, bool detached) ;
	void spawnProcChild(bool detached) ;

	// return a thread safe copy of the pointer
	WaitPidGuard* getProcGuard() const;

	// declared but not defined
	SpawnProc() ;
	SpawnProc(const SpawnProc &) ;
	SpawnProc &operator=(const SpawnProc &) ;

private:
	static ThreadSafe  _procMutex ;
	static pthread_t _mainThread ;

	bool _autoKill ;
	bool _detached ;
	WaitPidGuard *_procGuard ;
	bool _spawned ;
	std::string _process ;
	WaitPid::ProcArgs _args ;
	int _inDescriptor ;
	int _outDescriptor ;
	int _errDescriptor ;
	bool _pGroupLeader ;
	volatile bool _spawning ;
	ThreadedProcWrapper* _procWrapper;
    bool resolvSymLink_ ;

	std::map < std::string, std::string > envSet_;
	std::set < std::string > envClr_;

	/** Symbolic name */
	std::string _tagName;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpawnProc);
} ;

class CmdIORedir : public SpawnProc { // PRQA S 2109, 2153
public:
	CmdIORedir(std::string const &command, 
               std::string const &inpath, 
               std::string const &outpath, 
               const std::vector<std::string> &arguments=std::vector<std::string>() ) ;
	CmdIORedir(std::string const &command, bool resolvSymLink, 
               std::string const &inpath, 
               std::string const &outpath, 
               const std::vector<std::string> &arguments=std::vector<std::string>() ) ;
	virtual ~CmdIORedir() throw() ;
	
	operator int() ; // PRQA S 2181

	/**
		If the process is still running, and EOF is send through the descriptor.
		(EOF is not sent if the process is not running anymore)
		If, before maximum timeout_s [s] it has terminated, TRUE is returned and exit_code/exit_signal are filled.
		If it's stuck, FALSE is returned and exit_code and exit_signal are unfilled (zero).
	*/
	bool hangup(int& exit_code, int& exit_signal, const size_t& timeout_s=30) const ;

private:

    void init(std::string const &command, 
               std::string const &inpath, 
               std::string const &outpath, 
               const std::vector<std::string> &arguments=std::vector<std::string>() ) ;

	// declared but not implemented
	CmdIORedir() ;
	CmdIORedir(const CmdIORedir &) ;
	CmdIORedir &operator=(const CmdIORedir &) ;
private:
    pattern::FDGuard fd_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CmdIORedir);
} ;

_ACS_END_NAMESPACE

#endif	// _SpawnProc_H_
