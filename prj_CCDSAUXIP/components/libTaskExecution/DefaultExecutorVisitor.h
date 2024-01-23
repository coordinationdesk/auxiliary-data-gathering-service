/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
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
	Revision 1.6  2017/10/06 16:03:11  marfav
	ECICMF-76
	Added support for enable/disable TKR messages
	
	Revision 1.5  2017/10/02 15:28:12  marfav
	ECICMF-76
	Added function to bypass mandatory output checks
	
	Revision 1.4  2017/09/22 15:10:37  marfav
	Some default moved to base class
	
	Revision 1.3  2017/09/20 13:17:16  davide.tiriticco
	Renamed guard macros in headers
	
	Revision 1.2  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 1.1  2017/09/19 09:51:05  davide.tiriticco
	First add
	
*/

#ifndef _DefaultExecutorVisitor_h_
#define _DefaultExecutorVisitor_h_

#include <TaskTableVisitor.h>
#include <ExecutorBase.h>
#include <scOrder.h>
#include <tlTaskLauncher.h>
#include <TaskTableElementExecutor.h>
#include <ExecutorVisitor.h>

_ACS_BEGIN_NAMESPACE(acs)


class DefaultExecutorVisitorFactory;

/**
 *\brief A Class for defining the interface of the element executor visitors
 *       The Element Visitors shall be able to deal with every TaskTableElement
 *       One method is defined for each existent element class
 *       The customization is the action implemented in every method
 */
class DefaultExecutorVisitor : public ExecutorVisitor // PRQA S 2109, 2153
{
	
public:

using TaskTableVisitor::enterVisit;
using TaskTableVisitor::exitVisit;
using TaskTableVisitor::visit;

	/**
	 * Ctor and Dtor 
	 */ 
	explicit DefaultExecutorVisitor(const ExecutorVisitor::BuildParameters& params);
	virtual ~DefaultExecutorVisitor() throw() ;


	virtual bool enterVisit (TaskTableVisitable&);
	virtual bool exitVisit (TaskTableVisitable&);

	virtual bool enterVisit (Pool&);
	virtual bool exitVisit (Pool&);

	virtual bool exitVisit (TaskTable&);
	
	/**
	 *\brief Task interface definition
	 *       Every Task that needs to be executed will call this interface
	 *       passing itself as argument
	 */
	virtual bool visit (Task&);

	virtual void waitDetachedTasks();

	// ECICMF-76
	// Give the possibility to bypass output mandatory check
	// To re-implement it after the task execution
	void enableTaskMandatoryOutputsCheck (bool);
	bool isEnabledTaskMandatoryOutputsCheck () const;
	
protected:
	// Impementation of the base interface 
	// Used to manage events in the event queue
	virtual void manageEvents();

protected:

	/**
	 *\brief Extracts detached flag, stop signal and quit signal from an element
	 *       If element has some elements not set the inherited one will be used
	 *       Detached flag is forced for all the descending node of a detached one
	 **/ 
	void extractFlags (TaskTableElement&, Flags&);
	
	void propagateEventToExecutors (const std::pair < ExecutorEventType, int> &theEvent);
	void propagateEventToDetached (const std::pair < ExecutorEventType, int> &theEvent);
	void loadConfiguration();
	bool someoneRunning (std::map<ExecutorBase*, bool>&, ExecutorBase**) const;
	bool checkMandatoryOutputs (const Task&) const;

private:
	DefaultExecutorVisitor (); // not implemented 
	DefaultExecutorVisitor (const DefaultExecutorVisitor&); // not implemented 
	DefaultExecutorVisitor& operator= (const DefaultExecutorVisitor&); // not implemented 

	void applyArmageddonToCurrentTask (int signal);
	void applyArmageddonToDetachedTask (tlTaskLauncher* theLauncher, int signal);
	
protected:
	
	std::vector<ExecutorBase*> _elementExecutors;
	tlTaskLauncher* _theTaskLauncher;
	std::vector<tlTaskLauncher*> _detachedLaunchers;
	
	bool _enteredPool;
	
	ThreadSafe _taskLauncherMutex;

	// Message strings
	std::string _taskStartMessage;
	std::string _taskCompleteMessage;
	std::string _taskFailureMessage;
	std::string _poolStartMessage;
	std::string _poolCompleteMessage;
	std::string _sequenceStartMessage;
	std::string _sequenceCompleteMessage;

	// ECICMF-76 attributes
	bool _outputCheckEnabled;



	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultExecutorVisitor) ;


protected:
	// Pipes management section for capturing standard out and standard err
	typedef std::pair<std::pair<int,int>, std::pair<int,int> > PipeDesc;
	
	class PipeGuard // PRQA S 2109
	{
	public:
		explicit PipeGuard(PipeDesc& pipes) : _p(pipes) {}
		~PipeGuard() throw() 
        {
            ::close(_p.first.first); // PRQA S 4631 4
            ::close(_p.first.second);
            ::close(_p.second.first);
            ::close(_p.second.second);
        }
protected:
        PipeGuard() ; // not implemented
        PipeGuard(const PipeGuard &) ; // not implemented
        PipeGuard & operator= (const PipeGuard &) ; // not implemented
	private:
		PipeDesc _p;
	};


public:
	// Generate Timestamps for trk messages
	class TrkMessageUtils
	{
	public:
		static std::string generateTimeStamp();
	};

/**
protected:
	// Manages start/stop pool message issuing for trk purposes
	class PoolTrkMessageGuard // PRQA S 2109
	{
	public:
		PoolTrkMessageGuard (const std::string& startMsg, const std::string& completeMsg);
		~PoolTrkMessageGuard () throw() ;
	private:
        PoolTrkMessageGuard() ; // not implemented
        PoolTrkMessageGuard(const PoolTrkMessageGuard &) ; // not implemented
        PoolTrkMessageGuard& operator =(const PoolTrkMessageGuard &) ; // not implemented
	private:
		unsigned long _myPoolNumber;
		std::string _startMsg;
		std::string _completeMsg;

		static unsigned long _globalPoolNumber;
		static ThreadSafe _gloabPoolNumberGuard;
			
	};

	// Manages start/stop sequence message issuing for trk purposes
	class SequenceTrkMessageGuard // PRQA S 2109
	{
	public:
		SequenceTrkMessageGuard (const std::string& startMsg, const std::string& completeMsg);
		~SequenceTrkMessageGuard () throw() ;
	private:
        SequenceTrkMessageGuard() ; // not implemented
        SequenceTrkMessageGuard(const SequenceTrkMessageGuard &) ; // not implemented
        SequenceTrkMessageGuard& operator =(const SequenceTrkMessageGuard &) ; // not implemented
	private:
		unsigned long _mySequenceNumber;
		std::string _startMsg;
		std::string _completeMsg;

		static unsigned long _globalSequenceNumber;
		static ThreadSafe _gloabSequenceNumberGuard;

	};
**/

};

_ACS_END_NAMESPACE


#endif // _DefaultExecutorVisitor_h_

