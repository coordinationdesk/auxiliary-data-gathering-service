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

	$Prod: A.C.S. TaskTableExecution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.9  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.8  2015/04/30 13:01:39  marfav
	Clashing operations with base classes renamed
	
	Revision 2.7  2013/12/16 19:32:26  marpas
	qa warning
	
	Revision 2.6  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.5  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.4  2013/07/07 18:29:17  marpas
	applying coding best practices in progress
	some qa and compilation warnings removed
	
	Revision 2.3  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.2  2012/02/20 15:09:57  marpas
	refactoring
	
	Revision 2.1  2008/04/22 11:13:21  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/

#ifndef _ExecutorBase_H_
#define _ExecutorBase_H_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <exException.h>

#include <queue>
#include <stack>

_ACS_BEGIN_NAMESPACE(acs)


/**
 *\brief A Class defining the interface of the element executors
 */

class TaskTableElement;

	
class ExecutorBase : public virtual ThreadSafe // PRQA S 2109, 2153
{

public:
	enum ExecutorStatus { exeRUN, exeEXIT, exeABORT, exeFAILED };

public:

	/**
	 * This exception is the base exception for Executors
	 */ 
	exDECLARE_EXCEPTION(ExecutorBaseException, exException) ; // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(FlagsStackException, ExecutorBaseException) ; // PRQA S 2153
	

	/**
	 * Ctor and Dtor are empty. 
	 */ 
	ExecutorBase(); 
	virtual ~ExecutorBase() throw() ;
protected:
	ExecutorBase (const ExecutorBase&);
	ExecutorBase& operator= (const ExecutorBase&);
public:
	virtual int getExitCode() const;

	virtual bool isRunning() const;

	virtual bool hasExitCode() const;

	virtual bool getDetached() const;
	virtual void setDetached(bool);

	virtual int getDefKillSignal() const;
	virtual void setDefKillSignal(int);
	
	virtual int getDefQuitSignal() const;
	virtual void setDefQuitSignal(int);

	virtual ExecutorStatus getStatus() const;

	// abort using the kill signal
	virtual void abort(); // PRQA S 2502
	
	// abort using the kill signal tasks having criticality level less than the requested one
	virtual void abortLevel(int theLevel); 

	// stop using the quit signal
	virtual void quit();

	// send a signal to processes
	virtual void signal(int signal); // PRQA S 2502

	// send a signal to the whole process group
	virtual void signalGroup (int signal);

	// send a signal using normal event queue and asynchronous calls
	virtual void armageddon(int signal=0);
	
	virtual void waitDetachedTasks() = 0;


protected:
	virtual void setIsRunning(bool);
	virtual void setHasExitCode(bool);
	virtual void setExitCode (int);
	virtual bool setStatus (ExecutorStatus);
	virtual void resetStatus();

	/**
	 *  Manage the default posix exid code management
	 **/
	virtual void setDefaultPosixExitCode (bool p);
	virtual bool isDefaultPosixExitCode () const;

protected:
	// Events management logic shall be implemented by any subclass
	virtual void manageEvents() = 0;

	enum ExecutorEventType 
	{ 
		eeAbort,
		eeAbortLevel,
		eeQuit,
		eeKill,
		eeKillGroup,
		eeArmageddon
	};

	static std::string eventTypeToString (ExecutorEventType);
	
	void insertEventInQueue (const std::pair < ExecutorEventType, int> &theEvent);
	bool haveEventsInQueue () const ;
	bool getEventFromQueue (std::pair < ExecutorEventType, int>& theEvent);

	// Flags stack management
	struct Flags // PRQA S 2173, 2175
	{
		bool detached; // PRQA S 2100 3
		int killSignal;
		int quitSignal;

		Flags (bool d, int k, int q) :
			detached (d),
			killSignal (k),
			quitSignal (q)
		{
		}

		Flags () :
			detached (false),
			killSignal (9), // PRQA S 4400
			quitSignal (0)
		{
		}
	};

	void pushFlags (const Flags &);
	Flags topFlags () const;
	void popFlags ();
	size_t flagsStackSize() const;

private:
	int _exitCode;
	bool _isRunning;
	bool _hasExitCode;

	// APF-411 Posix exit code flag 
	bool _defaultPosixExitCode;
	
	std::stack<Flags> _flagsStack;
	ExecutorStatus _internalStatus; // starts assuming exeRUN value
	std::queue < std::pair < ExecutorEventType, int> > _eventQueue;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ExecutorBase) ;
};

_ACS_END_NAMESPACE

#endif //_ExecutorBase_H_

