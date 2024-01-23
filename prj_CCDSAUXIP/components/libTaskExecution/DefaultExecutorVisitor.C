// PRQA S 1050 EOF
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
	Revision 1.7  2017/10/30 14:43:58  marfav
	Destruction robustness improved
	
	Revision 1.6  2017/10/06 16:03:11  marfav
	ECICMF-76
	Added support for enable/disable TKR messages
	
	Revision 1.5  2017/10/03 15:28:24  marfav
	Adding dbKey to BuildParameters
	
	Revision 1.4  2017/10/02 15:28:12  marfav
	ECICMF-76
	Added function to bypass mandatory output checks
	
	Revision 1.3  2017/09/22 15:10:37  marfav
	Some default moved to base class
	
	Revision 1.2  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 1.1  2017/09/19 09:51:05  davide.tiriticco
	First add
	
*/


#include <DefaultExecutorVisitor.h>
#include <TaskTableElementExecutor.h>
#include <TaskTableExecutorFactoryChain.h>
#include <Timer.h>

#include <TaskDataTypes.h>
#include <Task.h>
#include <TaskTable.h>
#include <Pool.h>
#include <Sequence.h>

#include <StringUtils.h>
#include <ScErrFilterable.h>
#include <ScOutFilterable.h>
#include <ScLogManager.h>
#include <TrkFilterable.h>
#include <MCFilterables.h>

#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>

#include <scOrder.h>

#include <Functional.h>

#include <Dir.h>
#include <File.h>
#include <RegExp.h>

#include <algorithm>
#include <numeric>
#include <memory>
#include <string>

#if !(__GNUC__ > 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 3))
#define GCC_LESS_THAN_4_3_0
#endif


_ACS_BEGIN_NAMESPACE(acs)


using namespace std;
using namespace acs::mc;

namespace {

    // Resources keys
    const char* _taskStartMessageKey =        "TlScheduler.Messages.TaskStarted";
    const char* _taskCompleteMessageKey =     "TlScheduler.Messages.TaskCompleted";
    const char* _taskFailureMessageKey =      "TlScheduler.Messages.TaskFailed";
    const char* _poolStartMessageKey =        "TlScheduler.Messages.PoolStarted";
    const char* _poolCompleteMessageKey =     "TlScheduler.Messages.PoolCompleted";
    const char* _sequenceStartMessageKey =    "TlScheduler.Messages.SequenceStarted";
    const char* _sequenceCompleteMessageKey = "TlScheduler.Messages.SequenceCompleted";

    // Default Values
    const char* _taskStartDefaultMessage =        "starting execution of task #";
    const char* _taskCompleteDefaultMessage =     "completed execution of task #";
    const char* _taskFailureDefaultMessage =      "execution failed on task #";
    const char* _poolStartDefaultMessage =        "starting execution of pool #";
    const char* _poolCompleteDefaultMessage =     "completed execution of pool #";
    const char* _sequenceStartDefaultMessage =    "starting execution of sequence #";
    const char* _sequenceCompleteDefaultMessage = "completed execution of sequence #";

}



ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultExecutorVisitor)

DefaultExecutorVisitor::DefaultExecutorVisitor(const ExecutorVisitor::BuildParameters& params) :
    	ExecutorVisitor(params),
	_elementExecutors(0),
	_theTaskLauncher(0),
	_detachedLaunchers(),
	_enteredPool(false),
	_taskLauncherMutex(),
	_taskStartMessage(),
	_taskCompleteMessage(),
	_taskFailureMessage(),
	_poolStartMessage(),
	_poolCompleteMessage(),
	_sequenceStartMessage(),
	_sequenceCompleteMessage(),
	_outputCheckEnabled(true)
{
	// Retrieve the trk messages configuration settings 
	loadConfiguration();		
		
}


DefaultExecutorVisitor::~DefaultExecutorVisitor() throw()
{
	ThreadSafe::Lock lock (*this);
    // PRQA S 4631 L1
	for (vector<ExecutorBase*>::iterator it=_elementExecutors.begin(); it != _elementExecutors.end(); ++it) // PRQA S 4238 
	{
		// stop and delete all the stored elements
		delete *it;
	}
	_elementExecutors.clear();

	for (vector<tlTaskLauncher*>::iterator it = _detachedLaunchers.begin(); it != _detachedLaunchers.end(); ++it)  // PRQA S 4238
	{
		delete *it;
	}
	_detachedLaunchers.clear();

	delete _theTaskLauncher;
	_theTaskLauncher=0;
	
	// vector DTOR will not delete the pointed objects
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DESTROYNG DEFAULT EXECUTOR VISITOR") ;
    // PRQA L:L1
}


void DefaultExecutorVisitor::enableTaskMandatoryOutputsCheck (bool b)
{
	ThreadSafe::Lock lock (*this);
	_outputCheckEnabled = b;
}

bool DefaultExecutorVisitor::isEnabledTaskMandatoryOutputsCheck () const
{
	ThreadSafe::Lock lock (*this);
	return _outputCheckEnabled;
}

void
DefaultExecutorVisitor::loadConfiguration()
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "loading configuration settings") ;
			
	rsResourceSet& rs = ConfigurationSingleton::instance() -> get();

	// Task Start
	if (rs.hasKey (_taskStartMessageKey))
	{
		rs.getValue (_taskStartMessageKey, _taskStartMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get taskStart Message. Using default.") ;
		_taskStartMessage = _taskStartDefaultMessage;
	}

	// Task Complete
	if (rs.hasKey (_taskCompleteMessageKey))
	{
		rs.getValue (_taskCompleteMessageKey, _taskCompleteMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get taskComplete Message. Using default." ) ;
		_taskCompleteMessage = _taskCompleteDefaultMessage;
	}

	// Task Failure
	if (rs.hasKey (_taskFailureMessageKey))
	{
		rs.getValue (_taskFailureMessageKey, _taskFailureMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get taskFailure Message. Using default.") ;
		_taskFailureMessage = _taskFailureDefaultMessage;
	}

	
	// Pool Start
	if (rs.hasKey (_poolStartMessageKey))
	{
		rs.getValue (_poolStartMessageKey, _poolStartMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get poolStart Message. Using default.") ;
		_poolStartMessage = _poolStartDefaultMessage;
	}

	// Pool Complete
	if (rs.hasKey (_poolCompleteMessageKey))
	{
		rs.getValue (_poolCompleteMessageKey, _poolCompleteMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get poolComplete Message. Using default.") ;
		_poolCompleteMessage = _poolCompleteDefaultMessage;
	}

	// Sequence Start
	if (rs.hasKey (_sequenceStartMessageKey))
	{
		rs.getValue (_sequenceStartMessageKey, _sequenceStartMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get sequenceStart Message. Using default.") ;
		_sequenceStartMessage = _sequenceStartDefaultMessage;
	}

	// Sequence Complete
	if (rs.hasKey (_sequenceCompleteMessageKey))
	{
		rs.getValue (_sequenceCompleteMessageKey, _sequenceCompleteMessage);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unable to get sequenceComplete Message. Using default.") ;
		_sequenceCompleteMessage = _sequenceCompleteDefaultMessage;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Task Start message is          [" << _taskStartMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Task Complete message is       [" << _taskCompleteMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Task Failure message is        [" << _taskFailureMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Pool Start message is          [" << _poolStartMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Pool Complete Start message is [" << _poolCompleteMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Sequence Start message is      [" << _sequenceStartMessage << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Sequence Complete message is   [" << _sequenceCompleteMessage << "]") ;
}




void 
DefaultExecutorVisitor::extractFlags (TaskTableElement& e, Flags& flags) // PRQA S 4211
{
	// Retrieve Detached, KillSignal and StopSignal

	if ( e.isSetDetached() )
	{
		flags.detached = e.isDetached();
	}
	else
	{
		flags.detached = getDetached();
	}

	// Attention !!
	// When a node is marked as detached.. all the descending nodes SHALL be detached
	// Detached flag will be forced if necessary without terminating the execution
	if (getDetached() && !flags.detached) 
	{
		ACS_LOG_INFO("Not detached sequence having a detached parent found!! Detached flag will be forced!") ;
		flags.detached = true;
	}

	if ( e.isSetKillSignal() ) 
	{
		flags.killSignal = e.getKillSignal();
	}
	else
	{
		flags.killSignal = getDefKillSignal();
	}

	if ( e.isSetQuitSignal() ) 
	{
		flags.quitSignal = e.getQuitSignal();
	}
	else
	{
		flags.quitSignal = getDefQuitSignal();
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "extracted parameters") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "KillSignal = " << flags.killSignal) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QuitSignal = " << flags.quitSignal) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Detached   = " << boolalpha << flags.detached) ;

}


// -------------------------------------------------------------
// STL Helper Class
// Finds the AtExitSequence having a given exit status condition
// -------------------------------------------------------------
class EqualExitStatus // PRQA S 2109
{
public:
	explicit EqualExitStatus (ExitStatus e) : _theExitStatus(e) {}

	bool operator() (const AtExitSequence& s) const
	{
		return (s.getExitStatus() == _theExitStatus);
	}
private:
    EqualExitStatus() ; // not implemented
private:
	ExitStatus _theExitStatus;
};
// -------------------------------------------------------------



bool DefaultExecutorVisitor::enterVisit (TaskTableVisitable& v) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "(" << typeid(v).name() << ")") ;

	if ( !dynamic_cast<TaskTable*>(&v) && !dynamic_cast<Pool*>(&v) && !dynamic_cast<Sequence*>(&v) && !dynamic_cast<Task*>(&v)) // PRQA S 3081
	{
		// The only visitable elements are:
		// - TaskTable
		// - Sequence
		// - Pool
		// - Task
	
		return false; // Do not visit me and neither my children
	}

	if (_enteredPool) 
	{
		// Check if an exit request has been issued
		if (getStatus() != exeRUN)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "stop requested... setting exit code to -1.") ;
			setExitCode (-1);
		}
		else
		{
			// We are running in a pool... launch a parallel thread
			// and block the main visit of this node
	
			Flags theFlags = topFlags();

			// A new function here, converting the common part of 
			// ExecutorVisitor::BuildParameters in a TaskTableElementExecutor::BuildParameters
			// allowing the automatic propagation of the extra parameters 
			// would be perfect. Do it at the next added extra parameter

			TaskTableElementExecutor::BuildParameters params(&static_cast<TaskTableElement&>(v),
							_params._order,
							_params._key,
							theFlags.detached, 
							theFlags.killSignal, 
							theFlags.quitSignal, 
							this);

			// propagate the extra parameters 
			params._dbKey = _params._dbKey;
			params._emitTrk = _params._emitTrk;
													
			TaskTableElementExecutor* theNewExecutor = TaskTableExecutorFactoryChain::instance()->newObject(_params._key, params);
	
			// Store all the executors 
			_elementExecutors.push_back (theNewExecutor);
			
			theNewExecutor->startThread();
		}
		
		return false; // Do not visit me and neither my children
	}
	else
	{
		// Extract the flags from the visitable or inherit them from the parent	
		Flags f;
		extractFlags (static_cast<TaskTableElement&>(v), f);  // PRQA S 3081
		pushFlags(f);

		if (getStatus() != exeRUN)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "stop requested... setting exit code to -1.") ;

			// A stop has been required
			// do not visit the node
			setExitCode (-1);
			return false;
		}

		return true;
	}
}

bool 
DefaultExecutorVisitor::exitVisit (TaskTableVisitable& v)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "(" << typeid(v).name() << ")" ) ;

	if ( dynamic_cast<TaskTable*>(&v) ||   // PRQA S 3081 4
         dynamic_cast<Pool*>(&v) || 
         dynamic_cast<Sequence*>(&v) || 
         dynamic_cast<Task*>(&v))
	{
		if (!_enteredPool) 
		{
			popFlags();
		}
	}

	// Il the exit code is different from zero return false
	// Here we have the sequential exit code collected during the whole visit

	bool retCode = true;

	if (getExitCode() != 0) 
	{
		retCode = false;; // Visit failed!! STOP IT!
	}
	else
	{
		retCode = TaskTableVisitor::exitVisit (v);
	}

	return retCode;
}



bool 
DefaultExecutorVisitor::enterVisit (Pool& p)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	bool result = enterVisit (static_cast<TaskTableVisitable&> (p) );  // PRQA S 3081

	_enteredPool = true;
	return result;
}


bool
DefaultExecutorVisitor::someoneRunning (map<ExecutorBase*, bool>& theMap, ExecutorBase** ptr) const
{
	bool retVal = false;

	for (map<ExecutorBase*, bool>::iterator it = theMap.begin(); it != theMap.end(); ++it) // PRQA S 4238
	{
		if (!theMap[it->first]) 
		{
 			bool running = (it->first)->isRunning();
 			theMap[it->first] = !running;
 			if (running)
 			{
 				retVal=true;
 				*ptr = it->first;
 			}
		}
	}

    return retVal;
}

bool 
DefaultExecutorVisitor::exitVisit (Pool& p)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	// Collect all the running children and emit the proper boolean exit code
	// Note that once entered in a pool all the other executions run in other executors than this
	// it is safe to reset the flag as soon as the first exit pool

	// collect the TaskTableElementExecution exit status...	
	// 
	
	int theExitCode = getExitCode();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "the exit code is [" << theExitCode << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executor size is [" << _elementExecutors.size() <<  // PRQA S 3380
		          (_elementExecutors.empty() ? "] EMPTY" : "] NOT EMPTY")) ;

	// Wait for childs if and only if the entervisit returned true
	if (p.getEnterVisitResult())
	{
		map<ExecutorBase*, bool> completedElement;
	
		for (vector<ExecutorBase*>::iterator it = _elementExecutors.begin(); // PRQA S 4238
			it != _elementExecutors.end(); 
			++it) 
		{
	
			completedElement [*it] = false;
		}
	
		ExecutorBase* runningExecutor = 0 ;
		while (someoneRunning(completedElement, &runningExecutor)) 
		{
			TaskTableElementExecutor* ptr (dynamic_cast<TaskTableElementExecutor*> (runningExecutor));   // PRQA S 3081
			if (!ptr)
			{
				// It is not a TaskTableElementExecutor. Use the old plain delay
				ACS_LOG_WARNING("Unable to use synchronous wake up while waiting running tasks. using delay") ;
				Timer::delay(42); // PRQA S 4400
			}
			else
			{
				ptr->waitExitOrTimeoutThread(100); // PRQA S 4400
			}
			manageEvents();
		}
	
		int exitCode = 0;
        bool gotNegativeCodes = false;
		
		for (vector<ExecutorBase*>::iterator it = _elementExecutors.begin();  // PRQA S 4238
			it != _elementExecutors.end(); 
			++it) 
		{
			int uc = ((*it)->getExitCode());
            if (uc < 0)
            {
                // Got a negative value. Final value will be -1 (internal failures)
                gotNegativeCodes = true;
            }
			// find out the worst exit code
			exitCode = max (exitCode, uc );
			// get also the worst exit status (the setStatus map will filter the transitions)
			this->setStatus ((*it)->getStatus());
		}
		theExitCode = (gotNegativeCodes ? -1 : exitCode);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "the FINAL exit code is [" << theExitCode << "]") ;
	{
		// Critical Section
		// Set the exit code here 
		Lock lock (*this);
		setExitCode(theExitCode);
		setHasExitCode(true);
	}
	
	bool exitVisitRetval = exitVisit (static_cast<TaskTableVisitable&> (p) );  // PRQA S 3081
	
	_enteredPool = false;
	popFlags();

	if (theExitCode != 0)
	{
		// There were errors during the execution
		exitVisitRetval = false; 
	}	

	return exitVisitRetval;
}


bool DefaultExecutorVisitor::exitVisit (TaskTable& tt)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	// Verify the current exit code and select (if exists) the atExitSequence
	
	int currentExitCode = getExitCode();
	int newExitCode = currentExitCode;
	ExitStatus exitStatusToSearch = ExitStatusOK;

	if (currentExitCode == 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Searching for \"OK\" AT EXIT") ;
		exitStatusToSearch = ExitStatusOK;
	}
	else if (getStatus() == exeFAILED)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Searching for \"ERROR\" AT EXIT") ;
		exitStatusToSearch = ExitStatusERROR;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Searching for \"ABORT\" AT EXIT") ;
		exitStatusToSearch = ExitStatusABORT;
	}

	vector<AtExitSequence> v = tt.getAtExitSequences();
	vector<AtExitSequence>::iterator it = find_if (v.begin(), v.end(), EqualExitStatus(exitStatusToSearch)); // PRQA S 3081

	if (it != v.end())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AtExitSequence found... running it") ;

		Flags theFlags = topFlags();
		extractFlags (static_cast<TaskTableElement&>(*it), theFlags); // PRQA S 3081


		TaskTableElementExecutor::BuildParameters params(&static_cast<TaskTableElement&>(*it),
										_params._order,
										_params._key,
										theFlags.detached, 
										theFlags.killSignal, 
										theFlags.quitSignal, 
										this);

		// propagate the dbKey
		params._dbKey = _params._dbKey;
		params._emitTrk = _params._emitTrk;
																						
		TaskTableElementExecutor* theNewExecutor = TaskTableExecutorFactoryChain::instance()->newObject(_params._key, params);

		// Store the new executor in order to let him receive eventual events
		// The standard atExitSequences are not interruptable
		_elementExecutors.push_back (theNewExecutor);
		
		theNewExecutor->startThread();
		while (theNewExecutor->isRunning())
		{
			theNewExecutor->waitExitOrTimeoutThread(100); // PRQA S 4400
			//Timer::delay(100);
			manageEvents();
		}

		if (it->getOverrideStatus())
		{
			// Override the exit status with the new computed one
			newExitCode = theNewExecutor->getExitCode();

			// Critical Section
			// Apply the new computed exit code
			// Task failure will be reported if and only if the task is critical
			Lock lock (*this);
			setExitCode(newExitCode);
			setHasExitCode(true);
		}

	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No AtExitSequence found!") ;
	}

	// Use the standard exit code / status computation pattern
	return exitVisit ( static_cast<TaskTableVisitable&>(tt) );  // PRQA S 3081
}


// PRQA S 1040 L2
// pragma added to avoid old_style_cast waring caused by FD_ISSET
#pragma GCC diagnostic ignored "-Wold-style-cast"

/*
 * Task interface definition. Every Task that needs to be executed will call this interface
 * passing itself as argument.
 */
bool DefaultExecutorVisitor::visit (Task& t) // PRQA S 4020
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << t.getName() << ", " << t.getPath() << "] starting") ;

	// APF-271
	MCScopedParams scpr;
	string jobid = File::getFileName (( File::getDirPath ( File::getAbsolutePath (_params._order.getJobOrder()), true ) ));
	scpr << mc_param ("jobid", jobid);

	if (getStatus() != exeRUN) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "aborting execution start. Status is not exeRUN!") ;

		{
			// Critical section
			// set the exit code here
			Lock lock (*this);
			setExitCode(-1);
			setHasExitCode(true);
		}

		return false;	
	}
		

	// Filter Trk in case are not enabled
	if (_params._emitTrk)
	{
		// Issue the start message
		TrkFilterable msg;

		msg << "DefaultExecutorVisitor::VisitTask [" << DefaultExecutorVisitor::TrkMessageUtils::generateTimeStamp() << "]: " << 
			_taskStartMessage << t.getName() <<'\n' ;
		sclog << msg;		
	}
	else
	{
		ACS_LOG_INFO ("Task [" << t.getName() << "] started") ;

	}

	// Initialize all the pipe descriptors to -1
	
	PipeDesc thePipeDesc = make_pair(make_pair(-1,-1), make_pair(-1,-1));
	
	if (!getDetached()) 
	{
		int outfiledes[2]; // PRQA S 4403 2
		int errfiledes[2];
		
		if (pipe(outfiledes) == 0)
		{
			thePipeDesc.first.first   = outfiledes[0];
			thePipeDesc.first.second  = outfiledes[1];
		}
		else
		{
			// Unable to create the pipe
			// Emit a warning and go on without capturing standard out from the task
			ACS_LOG_WARNING("Unable to create pipes for stdout") ;
		}
			
		if (pipe(errfiledes) == 0)
		{
			thePipeDesc.second.first  = errfiledes[0];
			thePipeDesc.second.second = errfiledes[1];
		}
		else
		{
			// Unable to create the pipe
			// Emit a warning and go on without capturing standard error from the task
			ACS_LOG_WARNING("Unable to create pipes for stderr") ;
		}
	}

	PipeGuard thePipeGuard (thePipeDesc);

	{
		// Creation and deletion of _theTaskLauncher shall be under lock!
		ThreadSafe::Lock lock (_taskLauncherMutex);
		if (_theTaskLauncher) 
		{
			// This should never happen... 
			// try to recover the situation..
			delete _theTaskLauncher;
			_theTaskLauncher = 0;
		}

		_theTaskLauncher = new tlTaskLauncher (t.getPath(), t.getCriticalityLevel(), _params._order, t.getName(), getDefQuitSignal(), getDefKillSignal(), thePipeDesc.first.second, thePipeDesc.second.second, getDetached() );
	}

    const int MAX_MSG_LEN = 4096 ;

	if (_theTaskLauncher->run()) 
	{

		// In case of detached tasks just don't loop and go on
		bool haveToLoop = !getDetached();
		
		// stdout and stderr data buffers
		string outBuffer ;
        string errBuffer;


		// Do event management 
		// This call will apply to the Detached tasks too
		// Maybe someone has called armageddon just before spawning detached tasks...
		manageEvents();

		// This flag is used to allow the visitor to collect
		// the last messages issued on the stdout/stderr pipes
		// before exiting
		bool gotDataFromStreams = true;

		while (haveToLoop) 
		{


			// Collect all the data from stdout and stderr 
			// executing an extra loop even if the task has completed
			// and continuing the select of the file descriptors until
			// a timeout is generated -> empty descriptors
			haveToLoop = _theTaskLauncher->isRunning() || gotDataFromStreams;

			// Prepare to exit at the next loop if no data is received and the task completed its execution
			gotDataFromStreams = false;

			// Collect stdout and stderr and redirect it into sclog using appropriate filterable messages

			// Watch the two pipe ends to see when they have input
			int max_fd = 0;
			fd_set rfds; // PRQA S 4102
			FD_ZERO(&rfds); // PRQA S 4092, 4101, 4107

			FD_SET(thePipeDesc.first.first, &rfds); // PRQA S 3003, 3080, 3081 2
			FD_SET(thePipeDesc.second.first, &rfds);

			max_fd = max(thePipeDesc.first.first, thePipeDesc.second.first);

			struct timeval tv; // PRQA S 4102
			/* Wait timeout inside the select. */
			tv.tv_sec = 0;
			// Since the select is blocking
			// in case the task is not running anymore do a minimal wait on the descriptors
			// just to allow pipes to be completely flushed
			
			if (_theTaskLauncher->isRunning())
			{
				tv.tv_usec = 100000; // 1/10th of second // PRQA S 4400
			}
			else
			{
				tv.tv_usec = 1; // 1 microsecond
			}

			int retval = select(max_fd+1, &rfds, 0, 0, &tv);
			// Don't rely on the value of tv now! it must be reset before next call 

			switch (retval)
			{
			case -1: // ERROR
                {
					if (errno == EINTR) //signal received: do nothing for the moment, but in future...
					{
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "select interrupted by a signal") ;

						// This is the only recoverable error
						// give it another try to read descriptors
						gotDataFromStreams = true;
					}
					
					ACS_LOG_ERROR("select returned -1; errno=" << errno << " (" << strerror(errno) << ")")  ;
				    break;
                }

			case 0: // timeout
                {
				    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "select timeout") ;

				    break;
                }
					
            default: // data available
                {
					// Data is ready
					// Force another reading loop if the task is not running anymore
					gotDataFromStreams = true;

					char buf[MAX_MSG_LEN+1];
					
					// first check for messages on stderr...
					if ( FD_ISSET(thePipeDesc.second.first, &rfds) )
					{
						ssize_t count = read(thePipeDesc.second.first, buf, MAX_MSG_LEN);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read " << count << " chars on err descriptor "
								   << thePipeDesc.second.first) ;

						if (count == -1)
						{
							ACS_LOG_ERROR("-1 returned while reading errfiledes; errno=" << errno) ;
						}
						else if (count == 0)
						{
							ACS_LOG_ERROR("0 returned while reading errfiledes") ;
						}
						else
						{
							buf[count] = '\0';
							
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read string " << buf) ;

							string currentRead(buf);

							// Get rid of extra \r (windows like output streams)
							for (size_t offs = 0 ; // PRQA S 4238, 4244
							     (offs = currentRead.find ('\r')) != string::npos ;
							    )
							{
								currentRead.erase (offs, 1);
							}
							
							errBuffer.append(currentRead);

							ScErrFilterable msg;
							// Set the originator taskname using the i index
							msg.setOriginatorTaskName (t.getName());
							// Set order signature
							msg.setOrderSignature (_params._order.getLogFileName());
							
							// Split the message in single lines using '\n' as delimiter
							vector<string> messages;
							errBuffer = StringUtils::split (errBuffer, messages);
							size_t m_size = messages.size(); 
							for (size_t msgIdx=0; msgIdx < m_size; ++msgIdx)
							{
								msg << messages[msgIdx];
								sclog << msg;
							}
						}
					}
					
					// then check for messages on stdout...
					if ( FD_ISSET(thePipeDesc.first.first, &rfds) )
					{
						ssize_t count = read(thePipeDesc.first.first, buf, MAX_MSG_LEN);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read " << count << " chars on out descriptor "
								   << thePipeDesc.first.first) ;

						if (count == -1)
						{
							ACS_LOG_ERROR("-1 returned while reading outfiledes; errno=" << errno) ;
						}
						else if (count == 0)
						{
							ACS_LOG_ERROR("0 returned while reading outfiledes") ;
						}
						else
						{
							buf[count] = '\0';
							
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "read string " << buf) ;

							string currentRead(buf);
                            							
							// Get rid of extra \r (windows like output streams)
							for ( size_t offs = 0; // PRQA S 4238, 4244
							      (offs = currentRead.find ('\r')) != string::npos ; 
							    )
							{
								currentRead.erase (offs, 1);
							}

							outBuffer.append(currentRead);

							ScOutFilterable msg;
							// Set the originator taskname using the i index
							msg.setOriginatorTaskName (t.getName());
							msg.setOrderSignature (_params._order.getLogFileName());
							
							// Split the message in single lines using '\n' as delimiter
							vector<string> messages;
							outBuffer = StringUtils::split (outBuffer, messages);
							
                            size_t m_size = messages.size();
							for (size_t msgIdx=0; msgIdx < m_size; ++msgIdx)
							{
								msg << messages[msgIdx];
								sclog << msg;
							}
						}
					}

				    break;
                }
			}

			if (!haveToLoop) 
			{
				// This is the last chance for processing outputs
				// Force standard out and standard err buffers to flush

				errBuffer.append("\n");
				outBuffer.append("\n");

				ScErrFilterable errmsg;
				// Set the originator taskname using the i index
				errmsg.setOriginatorTaskName (t.getName());
				errmsg.setOrderSignature (_params._order.getLogFileName());


				// Split the message in single lines using '\n' as delimiter
				vector<string> messages;
				errBuffer = StringUtils::split (errBuffer, messages);

				size_t m_size = messages.size();
				for (size_t msgIdx=0; msgIdx < m_size; ++msgIdx)
				{
					errmsg << messages[msgIdx];
					sclog << errmsg;
				}

				ScOutFilterable outmsg;
				// Set the originator taskname using the i index
				outmsg.setOriginatorTaskName (t.getName());
				outmsg.setOrderSignature (_params._order.getLogFileName());


				// Split the message in single lines using '\n' as delimiter
				messages.clear();
				outBuffer = StringUtils::split (outBuffer, messages);

				m_size = messages.size();
				for (size_t msgIdx=0; msgIdx < m_size; ++msgIdx)
				{
					outmsg << messages[msgIdx];
					sclog << outmsg;
				}
			}
			
			_theTaskLauncher->waitExitOrTimeout (10); // PRQA S 4400
			manageEvents();
		}
		// Notify the number of skipped messages (until now)
		size_t skippedMsg = sclog.getSyslogSkippedMessageCount(t.getName());
		if (skippedMsg > 0)
		{
			ostringstream msg;
			msg << "Message dropped from task [" << t.getName() << "] is " << skippedMsg << ". Limit per task is " << sclog.getSyslogTaskMessageLimit() << " messages.";
			ACS_LOG_MC_INFO ( mc_param("message", msg.str() ) << mc_metric (Metric_LOG) );
		}

	}
	else
	{
		// DO NOT PANIC
		// Unable to spawn... collect exit code as usual and mark the failure !!
	}


	// Task has completed its run... now collect exit code and prepare to report to parents !!

	int exitCode = 0 ;
	int exitSignal = 0;

	if (getDetached()) 
	{
		// Detached tasks have exit code 0 by definition
		exitCode = 0;
	}
	else if (!_theTaskLauncher->exitCode(exitCode))
	{
		// The task has not an exit code... Mark it as a failure and report -1
		exitCode = -1;
	}
	else if (exitCode == -1)
	{
		// The task has an exit code but it may have been hit by a signal
		_theTaskLauncher->exitSignal(exitSignal);
		
	}
	else if (exitCode == 0 && isEnabledTaskMandatoryOutputsCheck() ) 
	{
		// This is not a detached task and the task execution was OK
		// Complete the execution checks with the mandatory outputs verification steps
		exitCode = (checkMandatoryOutputs (t) ? 0 : -1); // PRQA S 3380
	}
    else {
        // nothing 
    }


	// APF-411
	// Override the exit code if not posix and in the range 1-127 (warning exit codes)
	bool posixExitCode = ( t.isSetPosixExitCode() ? t.isPosixExitCode() : _params._order.getPosixExitCode() ); // get default exit code mode

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task completed [" << t.getName() << "]") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current exit code is [" << exitCode << "] and critical flag is set to " << boolalpha << t.isCritical()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current exit signal is [" << exitSignal << "]" );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Detected exit code is [" << exitCode << "]") ; // PRQA S 3380
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exit code interpretetation is [" << (posixExitCode ? "POSIX" : "GEN-PROC ICD") << "]") ; // PRQA S 3380

	// Finally override exit code with zero if the task is not critical
	// From this point the exit code will be always set to zero for non critical tasks
	exitCode = t.isCritical() ? exitCode : 0 ; // PRQA S 3380

	// In case of not posix exit code management
	// Reset to 0 any warning exit code after logging it as an informational event
	if (!posixExitCode && (exitCode > 0) && (exitCode < 128))
	{
		ACS_LOG_INFO ("Task exit code for task [" << t.getName() << "] is a Warning [" << exitCode << "]");
		exitCode = 0;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Final exit code will be [" << (t.isCritical() ? exitCode : 0) << "]") ; // PRQA S 3380
	
	// Manage parent error notifications
	if (exitCode != 0) 
	{
		// An error occurred... and the task is critical
		// If the error has been caused by a signal do not override the status
		// Set the status to exeFAILED 
		// this will not override EXIT or ABORT status
		setStatus (exeFAILED);
		
		if (_params._theParent && (flagsStackSize() == 2)) // PRQA S 4400
		{
			// Notify the parent with the failure only if the tasks are the at the first level of execution
			// New visitor will be made only when entering a pool composing element
			// The stack size equal to 2 implies a Pool made of Tasks
			_params._theParent->notifyCriticalTaskFailure (t.getCriticalityLevel());
		}

		// Task error message
		// TRK only of not disabled
		if (_params._emitTrk)
		{
			TrkFilterable msg;
			msg.setOriginatorTaskName (t.getName());
			msg.setOrderSignature (_params._order.getLogFileName());

			// This is the tracking filterable message
			msg << "DefaultExecutorVisitor::VisitTask [" << DefaultExecutorVisitor::TrkMessageUtils::generateTimeStamp() << "]: " << 
				_taskFailureMessage << t.getName() << "[" << exitCode << "]\n" ;
			sclog << msg;
		}
		// APF-371
		// Issue a specific ERROR message to log the reasons of the failure
		// In case the task has been killed by a signal notify also the signal number in the log
		ostringstream os;
		os << "The execution of the task named " << t.getName() << " failed with exit code " << exitCode;
		if (0 != exitSignal)
		{
			os << " because it has been killed by signal " << exitSignal;
		}

		// Notify it on the log as an error message
		ACS_LOG_ERROR (os.str());		

	    // APF-271
		// Issue it in the MC log stream
        ACS_LOG_MC_INFO ( mc_param("message", os.str() ) << mc_metric (Metric_FAIL) );
		
	}
	else
	{
		if (_params._emitTrk)
		{

			// Task completion message
			TrkFilterable msg;
			msg.setOriginatorTaskName (t.getName());
			msg.setOrderSignature (_params._order.getLogFileName());

			msg << "DefaultExecutorVisitor::VisitTask [" << DefaultExecutorVisitor::TrkMessageUtils::generateTimeStamp() << "]: " << 
				_taskCompleteMessage << t.getName() << "[" << exitCode << "]\n" ;
			sclog << msg;
		}
		else
		{
			ACS_LOG_INFO ("Task [" << t.getName() << "] completed with code [" << exitCode << "]") ;
		}
	}

	{
		// Critical Section
		// Set the exit code here 
		// Task failure will be reported if and only if the task is critical
		Lock lock (*this);
		setExitCode(exitCode);
		setHasExitCode(true);
	}

	{
		ThreadSafe::Lock lock (_taskLauncherMutex);
		// Manage _theTaskLauncher object and the detached object list
		if (getDetached())
		{
			_detachedLaunchers.push_back (_theTaskLauncher);
		}
		else
		{
			delete _theTaskLauncher;
		}
		_theTaskLauncher = 0;
	}

	if (exitCode != 0) {
		return false;
    }
	//else if (getStatus() != exeRUN)
	//	return false; // a stop request has been received
	else {
		return true;
    }
	
}

// reactivate -Wold-style-cast
#pragma GCC diagnostic warning "-Wold-style-cast"
// PRQA L:L2


namespace {
    class MandatoryOutputChecker 
    {
    public:
	    MandatoryOutputChecker (const Dir& wd, const scOrder& theScOrder, bool critical) :
	      _workDir(wd), // PRQA S 2528 2
	      _scOrder (theScOrder),
	      _critical (critical)
	    {}
        MandatoryOutputChecker(const MandatoryOutputChecker &r) :
	      _workDir(r._workDir), // PRQA S 2528 2
	      _scOrder (r._scOrder),
	      _critical (r._critical)
        {}
	    virtual ~MandatoryOutputChecker() throw() {} // PRQA S 2131
#ifdef GCC_LESS_THAN_4_3_0
	    static bool isOutputMandatory (OutputDescriptor d)
#else
	    static bool isOutputMandatory (const OutputDescriptor &d)
#endif
	    {
		    return d.isMandatory();
	    }

	    bool operator() (bool _result, const OutputDescriptor& od) const // PRQA S 4020
	    {
		    // if a previous file check has failed return failure immediately
		    if (!_result)
		    {
			    ACS_LOG_INFO("Skipping check of mandatory output " << od.getFileType() << "." <<
				    "A prevoius check failed!") ;
			    return _result;
		    }

		    // The work order will contain the regular expression to be used
		    // in order to check the presence of the mandatory outputs
		    // In case the regexp has not been included in the scOrder XML file
		    // use a default regexp based on output filetype

		    RegExp theRegExp;
		    if (_scOrder.hasRegExpAssociation (od.getFileType()))
		    {
			    // Set the regexp according to the content of the work order
			    theRegExp.setReg (_scOrder.getRegExpAssociation(od.getFileType()));
		    }
		    else
		    {
			    // No regexp in the work order
			    // Use the FileType based search

			    ostringstream os;
			    os << ".*" << od.getFileType() << ".*";
			    theRegExp.setReg (os.str());

			    ACS_LOG_WARNING("Warning: FileType " << od.getFileType() << " has no regexp association in the WorkOrder! Using [" << os.str() << "]") ;

		    }

		    // search the filetype inside directory entires matching the regular expression
		    for (size_t i=0; i<_workDir.size(); ++i)
		    {
			    if ( theRegExp.match(_workDir[i]) )
			    {
				    return _result; // _result && true
			    }
		    }

		    // Issue a WARNING or an ERROR message according to the criticality of the task
		    if (_critical) 
		    {
                ACS_LOG_ERROR("cannot find mandatory output " <<
		          od.getFileType() <<
		          " in working directory [" <<
		          _workDir.path() <<
		          "] after execution. RegExp is [" << theRegExp.getReg() << "]") ;
		    }
		    else
		    {
                ACS_LOG_WARNING("cannot find mandatory output " <<
		          od.getFileType() <<
		          " in working directory [" <<
		          _workDir.path() <<
		          "] after execution. RegExp is [" << theRegExp.getReg() << "]") ;
		    }

		    // if we didn't find a file and the task is critical the whole check will return false
		    return !_critical;
	    }
    private:
        MandatoryOutputChecker() ; // not implemented
        MandatoryOutputChecker& operator= (const MandatoryOutputChecker &) ; // not implemented
    private:
	    const Dir& _workDir;
	    const scOrder& _scOrder;
	    bool _critical;

    };
} // unnamed namespace

bool DefaultExecutorVisitor::checkMandatoryOutputs (const Task& t) const // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called [" << t.getName() << "]") ;

	ACS_LOG_PRIVINFO("Checking mandatory outputs for task [" << t.getName() << "]")  ;

	// check the files if the 	

	vector<OutputDescriptor> outputFiles;

	t.getOutputList (outputFiles);  // returns all the output descriptors of this task

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task [" << t.getName() << "] has " << outputFiles.size() << " outputs") ;

	outputFiles.erase(remove_if(outputFiles.begin(),
					  outputFiles.end(),
					  not1(ptr_fun (MandatoryOutputChecker::isOutputMandatory)) ),
					  outputFiles.end());  // removes all not mandatory file descriptors

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ask [" << t.getName() << "] has " << outputFiles.size() << " mandatory outputs") ;

	// The output files shall be placed in the current working dir
	Dir workDir(".");

	// return the result of check
	// if not all mandatory output files exist
	// this method will return FALSE

	bool check = accumulate(outputFiles.begin(), outputFiles.end(), true, MandatoryOutputChecker(workDir, _params._order, t.isCritical()));
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The check for mandatory output was " << (check? "" : " NOT") <<" OK") ; // PRQA S 3380

	return check;
}


// wait for the completion of the currently running detached tasks
void DefaultExecutorVisitor::waitDetachedTasks ()
{
	if (!_elementExecutors.empty()) 
	{
		// Wait all the executors in a loop
		for (vector<ExecutorBase*>::iterator it = _elementExecutors.begin(); it != _elementExecutors.end(); ++it) // PRQA S 4238
		{
			(*it)->waitDetachedTasks();
		}
	}
	else if (!_detachedLaunchers.empty()) 
	{
		
		bool haveToLoop = true;
		tlTaskLauncher* lastRunningLauncher = 0;

		while (haveToLoop) 
		{
			manageEvents();
			haveToLoop = false;

			for (vector<tlTaskLauncher*>::iterator it = _detachedLaunchers.begin(); it != _detachedLaunchers.end(); ++it)  // PRQA S 4238
			{
				haveToLoop = haveToLoop || (*it)->isRunning();
				if ((*it)->isRunning())
				{
					lastRunningLauncher = *it;
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "the detached task [" << (*it)->Name() << 
							" - " << (*it)->pid() << "] is still running... waiting") ;
				}
			}
			if (haveToLoop)
			{
				// this will wake up the controller in the case a running launcher ends its execution
				lastRunningLauncher->waitExitOrTimeout (100); // PRQA S 4400
			}
		}
	}
	else
	{
		// No elements to wait for...
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no tasks to wait !!")  ;
	}
}


void DefaultExecutorVisitor::manageEvents () // PRQA S 4020
{
	// Manages one element from the queue in every call
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	pair < ExecutorEventType, int> theEvent;

	ThreadSafe::Lock lock (_taskLauncherMutex);

	// If no objetcs available to apply the events
	// Just return and wait for better times!

	if (_elementExecutors.empty() && _detachedLaunchers.empty() && !_theTaskLauncher)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no objects to apply the events!!") ;
		return;
	}
	
	while (getEventFromQueue(theEvent)) 
	{
	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "managing event (" 
			<< eventTypeToString (theEvent.first) << ", " << theEvent.second << ")") ;


		bool managed = false;
		if (_elementExecutors.size() > 0) 
		{
			managed = true;
			propagateEventToExecutors (theEvent);
		}
		if (_detachedLaunchers.size() > 0) 
		{
			managed = true;
			propagateEventToDetached (theEvent);
		}
		if (_theTaskLauncher)
		{			
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "applying the event to the current running task") ;

			managed = true;

			// This visitor is executing a task
			// Executing the event on the current task
			switch (theEvent.first) 
			{
			case eeAbort:
                {
				    _theTaskLauncher->kill(getDefKillSignal());
				    break;
                }
			case eeAbortLevel:
                {
				    _theTaskLauncher->killLevel(theEvent.second, getDefKillSignal());
				    break;
                }
			case eeQuit:
                {
				    _theTaskLauncher->kill(getDefQuitSignal());
				    break;
                }
			case eeKill:
                {
				    _theTaskLauncher->kill(theEvent.second);
				    break;
                }
			case eeKillGroup:
                {
				    _theTaskLauncher->killGroup(theEvent.second);
				    break;
                }
			case eeArmageddon:
                {
				    applyArmageddonToCurrentTask (theEvent.second);
				    break;
                }
			default:
                {
				    break;
                }
			}

		}
		if (!managed)
		{
			// School case... 
			// Someone has removed all the objects while running the event management procedure 
			// exit now and avoid the messages to be lost!!
			insertEventInQueue (theEvent);
		}
	}
}

void DefaultExecutorVisitor::applyArmageddonToCurrentTask (int signal) // PRQA S 4211
{
	if (0 != signal) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using user signal [" << signal << "]") ;
	    _theTaskLauncher->killGroup(signal);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default kill signal [" << getDefKillSignal() << "]") ;
	    _theTaskLauncher->killGroup(getDefKillSignal());
	}
}

void DefaultExecutorVisitor::applyArmageddonToDetachedTask (tlTaskLauncher* theLauncher, int signal)
{
	if (0 != signal) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using user signal [" << signal << "]") ;
	    theLauncher->killGroup(signal);
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default kill signal [" << theLauncher->getKillSignal() << "]") ;
	    theLauncher->killGroup(theLauncher->getKillSignal());
	}
}

void DefaultExecutorVisitor::propagateEventToDetached(const pair < ExecutorEventType, int> &theEvent)
{
	for (vector<tlTaskLauncher*>::iterator it = _detachedLaunchers.begin(); it != _detachedLaunchers.end(); ++it) // PRQA S 4238
	{
		switch (theEvent.first) 
		{
		case eeArmageddon:
            {
			    applyArmageddonToDetachedTask (*it, theEvent.second);
			    break;
            }
		default:
            {
			    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "discarding event [" << eventTypeToString (theEvent.first) << "] for detached task") ;

			    break;
            }
		}
	}
}

void DefaultExecutorVisitor::propagateEventToExecutors(const pair < ExecutorEventType, int> &theEvent)
{
	for (vector<ExecutorBase*>::iterator it = _elementExecutors.begin(); it != _elementExecutors.end(); ++it) // PRQA S 4238
	{
		switch (theEvent.first) 
		{
		case eeAbort:
            {
			    (*it)->abort();
			    break;
            }
		case eeAbortLevel:
            {
			    (*it)->abortLevel(theEvent.second);
			    break;
            }
		case eeQuit:
            {
			    (*it)->quit();
			    break;
            }
		case eeKill:
            {
			    (*it)->signal(theEvent.second);
			    break;
            }
		case eeKillGroup:
            {
			    (*it)->signalGroup(theEvent.second);
			    break;
            }
		case eeArmageddon:
            {
			    (*it)->armageddon(theEvent.second);
			    break;
            }
		default:
            {
			    // ISSUE A WARNING MESSAGE??
			    break;
            }
		}
	}
}

// Trk message helper classes
//


string DefaultExecutorVisitor::TrkMessageUtils::generateTimeStamp()
{
	DateTime::UTCD utcd = DateTime() ; // PRQA S 3050
	ostringstream os;
    // PRQA S 4400 8
	os << setfill('0') <<
	setw(4) << utcd.year    << " " << 
	setw(2) << utcd.month   << " " <<
	setw(2) << utcd.day     << " " <<
	setw(2) << utcd.hour    << " " <<
	setw(2) << utcd.min     << " " <<
	setw(2) << utcd.sec;

	return os.str();
}


_ACS_END_NAMESPACE
