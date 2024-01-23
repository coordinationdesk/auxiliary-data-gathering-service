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
	Revision 2.16  2017/10/30 14:43:58  marfav
	Destruction robustness improved
	
	Revision 2.15  2017/10/06 16:03:11  marfav
	ECICMF-76
	Added support for enable/disable TKR messages
	
	Revision 2.14  2017/10/03 15:28:24  marfav
	Adding dbKey to BuildParameters
	
	Revision 2.13  2017/09/26 15:16:25  marfav
	Added support for an external condition
	
	Revision 2.12  2017/09/22 15:10:37  marfav
	Some default moved to base class
	
	Revision 2.11  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 2.10  2017/09/19 09:57:45  davide.tiriticco
	Introduced factory class. Constructor has a private scope now.
	
	Revision 2.9  2017/03/17 16:00:47  marfav
	Posix Exit Code compliancy flag embedded in ScOrder file
	
	Revision 2.8  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.7  2015/04/30 13:01:39  marfav
	Clashing operations with base classes renamed
	
	Revision 2.6  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.5  2013/07/07 18:29:17  marpas
	applying coding best practices in progress
	some qa and compilation warnings removed
	
	Revision 2.4  2013/04/03 13:47:03  marfav
	Rolling back to previous thread::run()
	
	Revision 2.3  2013/04/03 12:46:29  marfav
	thread::run takes a void * as argument
	
	Revision 2.2  2009/06/26 10:50:24  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2008/04/22 11:13:22  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/

#ifndef _TaskTableElementExecutor_h_
#define _TaskTableElementExecutor_h_


#include <acs_c++config.hpp>
#include <Thread.h>
#include <ExecutorBase.h>
#include <scOrder.h>
#include <StringKey.h>

_ACS_BEGIN_NAMESPACE(acs)

class ExecutorVisitor;

class TaskTableElementExecutor : private Thread, public ExecutorBase // PRQA S 2109
{

public:
	
	/**
	 * \brief This class wraps the input parameters that must be passed to TaskTableElementExecutor constructors.
	 */
	class BuildParameters {	// PRQA S 2109

	public:
		/**
		 * \brief Constructor
		 */
		BuildParameters(TaskTableElement* elem, 
				const scOrder& order,
				const StringKey& key,
				bool detached = false, 
				int kills = 15, 
				int quits = 2, 
				ExecutorVisitor* theParent = 0);

		/**
		 * \brief Destructor
		 */			
		virtual ~BuildParameters() throw();
		
		/**
		 * \brief Copy constructor
		 */			
		BuildParameters(const BuildParameters&);

		/**
		 * \brief Assignment operator overload
		 */			
		BuildParameters& operator=(const BuildParameters&);
		
	public:
		TaskTableElement* _elem;
		scOrder _order;
		StringKey _key;
		bool _detached;
		int _kills;
		int _quits;
		ExecutorVisitor* _theParent;
		std::string _dbKey;	
		bool _emitTrk;
		
	private:
		/**
		 * \brief Default constructor
		 */
		BuildParameters(); // Defined but not implemented		
	};
	
public:

	TaskTableElementExecutor (const BuildParameters&);
	virtual ~TaskTableElementExecutor () throw() ; 

	virtual void run();
	virtual bool isRunning() const;

	virtual void abort();     // PRQA S 2502 
	virtual void abortLevel(int theLevel); 
	virtual void quit();
	virtual void signal (int signal); 
	virtual void signalGroup (int signal);

	// Breaking the thread shell
	void startThread () { Thread::start(); }
	bool waitExitOrTimeoutThread (unsigned long msecs) { return Thread::waitExitOrTimeout (msecs); }
    void setThreadExtCondition (Condition* c) { Thread::setExternalCondition (c); }

	void notifyCriticalTaskFailure (int criticalityLevel);
	
	// wait for the completion of the currently running detached tasks
	virtual void waitDetachedTasks ();

	// send a signal using normal event queue and asynchronous calls
	virtual void armageddon(int signal=0);

protected:

                               	
	virtual void manageEvents();	

	// It is better to mask quit signal before inserting the object in the event messaging queues
	// once masked it is better to not unmask it until execution has completed
	void maskQuitSignal ();
	void unmaskQuitSignal ();
	bool quitSignalMasked() const;

private:
	// Declared but not implemented to prevent their use
	TaskTableElementExecutor ();
	TaskTableElementExecutor (const TaskTableElementExecutor&);
	TaskTableElementExecutor& operator= (const TaskTableElementExecutor&);
	

private:
	BuildParameters _params;
	ExecutorVisitor* _theVisitor;

	// Message mask flags
	bool _maskQuitSignal;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableElementExecutor) ;
};


_ACS_END_NAMESPACE


#endif //_TaskTableElementExecutor_h_


