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
	Revision 2.18  2017/10/30 14:43:58  marfav
	Destruction robustness improved
	
	Revision 2.17  2017/10/27 11:03:41  marfav
	Robustness improved managing ExecutorVisitor ctor and visit exceptions
	
	Revision 2.16  2017/10/06 16:03:11  marfav
	ECICMF-76
	Added support for enable/disable TKR messages
	
	Revision 2.15  2017/10/03 15:28:24  marfav
	Adding dbKey to BuildParameters
	
	Revision 2.14  2017/09/27 15:36:39  marfav
	ECICMF-76 debug for REST parameters added
	
	Revision 2.13  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 2.12  2017/09/19 09:57:45  davide.tiriticco
	Introduced factory class. Constructor has a private scope now.
	
	Revision 2.11  2017/03/17 16:00:47  marfav
	Posix Exit Code compliancy flag embedded in ScOrder file
	
	Revision 2.10  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.9  2015/04/30 13:01:39  marfav
	Clashing operations with base classes renamed
	
	Revision 2.8  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.7  2013/11/07 16:42:13  marfav
	Setting the exit code before declaring that exit code exists
	
	Revision 2.6  2013/07/07 18:29:17  marpas
	applying coding best practices in progress
	some qa and compilation warnings removed
	
	Revision 2.5  2013/04/03 13:47:03  marfav
	Rolling back to previous thread::run()
	
	Revision 2.4  2013/04/03 12:46:29  marfav
	thread::run takes a void * as argument
	
	Revision 2.3  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.2  2009/06/26 10:50:24  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2008/04/22 11:13:21  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/

#include <TaskTableElementExecutor.h>
#include <TaskTableElement.h>

#include <ExecutorVisitor.h>
#include <ExecutorVisitorFactoryChain.h>

#include <DateTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableElementExecutor)

/********************************************************************
 * 		BUILD PARAMETERS IMPLEMENTATION
 ********************************************************************/

/*
 * Constructor
 */
TaskTableElementExecutor::BuildParameters::BuildParameters(TaskTableElement* elem,
				const scOrder& order,
				const StringKey& key,
				bool detached, 
				int kills, 
				int quits, 
				ExecutorVisitor* theParent) :
	_elem(0),	// PRQA S 2528 2
	_order(order),
	_key(key),
	_detached(detached),
	_kills(kills),
	_quits(quits),
	_theParent(theParent),
	_dbKey(""),
	_emitTrk(true)
{
	ACS_COND_THROW(elem == 0, exIllegalValueException("TaskTableElement pointer cannot be null."));
	_elem = elem;
}

/**
 * Destructor
 */			
TaskTableElementExecutor::BuildParameters::~BuildParameters() throw() 
{ 
}


TaskTableElementExecutor::BuildParameters::BuildParameters(const BuildParameters& param) : 
	_elem(param._elem),
	_order(param._order),
	_key(param._key),
	_detached(param._detached),
	_kills(param._kills),
	_quits(param._quits),
	_theParent(param._theParent),
	_dbKey(param._dbKey),
	_emitTrk(param._emitTrk)
{ }


TaskTableElementExecutor::BuildParameters& TaskTableElementExecutor::BuildParameters::operator=(const BuildParameters& param) {
	if (this != &param) {
		_elem = param._elem;
		_order = param._order;
		_key = param._key;
		_detached = param._detached;
		_kills = param._kills;
		_quits = param._quits;
		_theParent = param._theParent;
		_dbKey = param._dbKey;
		_emitTrk = param._emitTrk;
	}
	
	return *this;
}

/********************************************************************
 * 		TASK TABLE ELEMENT EXECUTOR IMPLEMENTATION
 ********************************************************************/

TaskTableElementExecutor::TaskTableElementExecutor(const BuildParameters& params) :
	Thread(),
	ExecutorBase(),
	_params(params),
	_theVisitor (0),
	_maskQuitSignal(false)
{

	ExecutorBase::setDetached (params._detached);
	ExecutorBase::setDefKillSignal (params._kills);
	ExecutorBase::setDefQuitSignal (params._quits);
	ExecutorBase::setDefaultPosixExitCode (params._order.getPosixExitCode());

	if (!_params._elem->isInterruptable()) 
	{
		maskQuitSignal();
	}
	
		
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KillSignal        = " << ExecutorBase::getDefKillSignal()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QuitSignal        = " << ExecutorBase::getDefQuitSignal() ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Detached          = " << boolalpha << params._detached ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Interruptable     = " << boolalpha << !quitSignalMasked() ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Posix exit code   = " << boolalpha << params._order.getPosixExitCode() ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "REST URL          = " << params._order.getResManagerUrl() ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "REST Client ID    = " << params._order.getResManagerClientId() ) ;

}

TaskTableElementExecutor::~TaskTableElementExecutor () throw()
{
	// This is a running thread, managing other tasks
	// Before destroying it shut everything down right now
	if (isRunning())
	{
		// First abort and then fire a sigkill
		this->abort();
		this->signal(9);
		
		// Give it one more second to die and the give up
		this->waitExitOrTimeoutThread (1000);
	}
	delete _theVisitor;
	_theVisitor=0;
}



void
TaskTableElementExecutor::waitDetachedTasks ()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;
	
	if (_theVisitor) 
	{
		_theVisitor->waitDetachedTasks();
	}
	else
	{
		// No visitor has been created yet..
		// This is an early call... ignore it
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no visitor to wait for") ;
		
	}
}

void
TaskTableElementExecutor::run ()
{
	// In this thread just let the visitor visit the node and wait for its completion
	DateTime theTime;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "#### Thread execution started ####" << theTime.getSqlString()) ;

	// A new function here, converting the common part of 
	// ExecutorVisitor::BuildParameters in a TaskTableElementExecutor::BuildParameters
	// allowing the automatic propagation of the extra parameters 
	// would be perfect. Do it at the next added extra parameter	
	ExecutorVisitor::BuildParameters params(_params._order, _params._key, getDetached(), getDefKillSignal(), getDefQuitSignal(), this);
	// Propagate the extra parameters
	params._dbKey = _params._dbKey;
	params._emitTrk = _params._emitTrk;

	try
	{
		_theVisitor = ExecutorVisitorFactoryChain::instance()->newObject(params._key, params);

		// Fill the Visitor's events queue and then start the execution
		manageEvents();
		_params._elem->acceptVisitor(*_theVisitor);

		// Just after the execution of the Visitor. 
		// Get the exit code and let the parent's know
		{
			// This is a critical section...
			// The flags about the exit status shall be set togheter to avoid thread interferences
			// Forward the status to the caller
			Lock theLock (*this); 
			this->setStatus (_theVisitor->getStatus());
			this->setExitCode( _theVisitor->getExitCode() );
			this->setHasExitCode(true);
		}
	}
	catch (exception& ex)
	{
		// Oops an exception has been caught.
		// Avoid uncontrolled exit. It is definitely an internal error.
		ACS_LOG_ERROR ("The task execution reported an internal error (see exception below)");
		ACS_LOG_NOTIFY_EX (ex);
		Lock theLock (*this); 
		this->setExitCode( -1 );
		this->setHasExitCode(true);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "#### Thread execution ended ####" <<  theTime.getSqlString() << " exit code is [" << this->getExitCode() << "]") ;
}

void 
TaskTableElementExecutor::notifyCriticalTaskFailure (int criticalityLevel) // PRQA S 4211
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called - criticality level is [" << criticalityLevel << "]") ;

	// Notify the parent queuing an abortLevel event
	if (_params._theParent) 
	{
		_params._theParent->abortLevel (criticalityLevel);	
	}
}

bool
TaskTableElementExecutor::isRunning () const
{
	return Thread::running();
}


void 
TaskTableElementExecutor::abort()
{
	// Insert an abort event in the queue
	setStatus (exeABORT);
	insertEventInQueue (make_pair (eeAbort, 0));

	// And try to execute it immediately
	manageEvents();
}
	
void 
TaskTableElementExecutor::abortLevel(int theLevel)
{
	// Insert an abortLevel event in the queue
	setStatus (exeFAILED);
	insertEventInQueue (make_pair (eeAbortLevel, theLevel));

	// And try to execute it immediately
	manageEvents();
}

void 
TaskTableElementExecutor::quit()
{

	if (!quitSignalMasked()) 
	{
		// Insert a quit event in the queue
		setStatus (exeEXIT);
		insertEventInQueue (make_pair (eeQuit, 0));

		// And try to execute it immediately
		manageEvents();
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "IGNORING QUIT COMMAND - QUIT is masked!") ;
	}
}

void 
TaskTableElementExecutor::signal(int signal)
{
	// Insert a kill event in the queue
	insertEventInQueue (make_pair (eeKill, signal));

	// And try to execute it immediately
	manageEvents();
}

void 
TaskTableElementExecutor::signalGroup (int signal)
{
	// Insert a killGroup event in the queue
	insertEventInQueue (make_pair (eeKillGroup, signal));

	// And try to execute it immediately
	manageEvents();
}

void 
TaskTableElementExecutor::armageddon (int signal)
{
	// Insert a killGroup event in the queue
	setStatus (exeABORT);
	insertEventInQueue (make_pair (eeArmageddon, signal));

	// And try to execute it immediately
	manageEvents();
}

void 
TaskTableElementExecutor::maskQuitSignal ()
{
	// - Acquire the lock
	// - reset the status if found in exeEXIT
	// - remove from the queue any eeQuit event
	Lock lock(*this);
	
	_maskQuitSignal = true;
	
	if (getStatus() == exeEXIT) 
	{
		resetStatus();
	}
}

void 
TaskTableElementExecutor::unmaskQuitSignal ()
{
	Lock lock(*this);
	_maskQuitSignal = false;
}


bool 
TaskTableElementExecutor::quitSignalMasked() const
{
	Lock lock(*this);
	return _maskQuitSignal;
}



void TaskTableElementExecutor::manageEvents() // PRQA S 4020
{

	// Manages one element from the queue in every call
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "called") ;

	if (!_theVisitor) 
	{
		// The visitor is not yet created.
		// Wait for its creation
		// manageEvents will be invoked just after its creation
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "the visitor is not yet created.. returning") ;
		return;
	}

	pair < ExecutorEventType, int> theEvent;

	while (getEventFromQueue(theEvent)) 
	{
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "managing event (" 
				   <<  eventTypeToString (theEvent.first)<< ", " << theEvent.second << ")") ;


		switch (theEvent.first) 
		{
		case eeAbort:
            {
			    _theVisitor->abort();
			    break;
            }
		case eeAbortLevel:
            {
			    _theVisitor->abortLevel(theEvent.second);
			    break;
            }
		case eeQuit:
            {
			    _theVisitor->quit();
			    break;
            }
		case eeKill:
            {
			    _theVisitor->signal(theEvent.second);
			    break;
            }
		case eeKillGroup:
            {
			    _theVisitor->signalGroup(theEvent.second);
			    break;
            }
		case eeArmageddon:
            {
			    _theVisitor->armageddon(theEvent.second);
			    break;
            }
		default:
            {
			    break;
            }
		}
	}
}


_ACS_END_NAMESPACE

