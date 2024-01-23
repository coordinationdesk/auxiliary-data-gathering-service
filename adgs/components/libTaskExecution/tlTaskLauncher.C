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
	Revision 2.8  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.7  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.6  2013/07/25 16:18:28  marfav
	Using synchronous waitin loop monitoring udergoing processes
	
	Revision 2.5  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.4  2012/02/13 17:06:34  marpas
	removing compiler warnings
	
	Revision 2.3  2008/04/22 11:13:22  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/


#include <tlTaskLauncher.h>
#include <TrkFilterable.h>
#include <ScLogManager.h>
	
#include <ConfigurationSingleton.h>
#include <DateTime.h>
#include <exStream.h>
#include <rsResourceSet.h>

#include <unistd.h> 

namespace 
{
	// Resources keys
	const char* _taskNameEnvKey = "TlScheduler.EnvTaskNameVariable";
}

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(tlTaskLauncher)

tlTaskLauncher::tlTaskLauncher(const string& aTask,
			       int aCriticalityLevel,
			       const scOrder& aWorkOrder,
			       const string& aName,
			       int quitSignal,
			       int killSignal,
			       int outd, int errd,
			       bool isDetached) :
    _task(aTask), 
    _criticalityLevel (aCriticalityLevel), 
    _order(aWorkOrder), 
    _name(aName), 
    _proc(0), 
    _outd(outd), 
    _errd(errd), 
    _isDetached(isDetached),
    _taskStartingMessage (),
    _quitSignal (quitSignal),
    _killSignal (killSignal)

{
}

tlTaskLauncher::~tlTaskLauncher() throw() 
{
    delete _proc;
}


bool tlTaskLauncher::isFailed() const // PRQA S 4020
{
// Here it is assumed that if a task never started it
// cannot result as a failed one
  if (!isStarted() || isRunning()) {
    return false;
  }

  int ec = 0 ;
  if (_proc->exitCode(ec)) {
    return ec != tsOK;
  }

  return false;
}

bool tlTaskLauncher::run() // PRQA S 4020
{
  if (!_proc)
  {
    _proc = new SpawnProc(_task);

    size_t argc = 0;


    //
    // first, set the jobOrder (if available)
    //

    if (_order.getJobOrder() != "") {
      _proc->args()[argc++] = _order.getJobOrder();
    }


    //
    // next, set the common cmd line parameters (if any)
    //

    {
	    vector<string> v;
	    _order.getJobOrderParams(v);

        size_t v_size = v.size() ;
	    for (size_t i=0; i<v_size; ++i) {
	        _proc->args()[argc++] = v[i];
        }
    }


    // 
    // next, set the task specific cmd line parameters (if any)
    //

    {
	    vector<string> v;
	    _order.getCmdLineParams(Name(), v);

        size_t v_size = v.size() ;
	    for (size_t i=0; i<v_size; ++i) {
	        _proc->args()[argc++] = v[i];
        }
    }

	// ECICMF-194
	// Set the processing environment variable
	_proc->addEnvSetting ( getEnvForTaskName(), Name());


    // Each task is started as a group leader
    // This is needed in order to send one shot signals (i.e. SIGSTOP, SIGCONT)
    // to it, its children, the children of its children... (to the process group)
    _proc->setPGroupLeader(true);

    // capture stdout and stderr if necessary
    if (_outd != -1) {
      _proc->setOutDescriptor(_outd) ;
    }
    if (_errd != -1) {
      _proc->setErrDescriptor(_errd) ;
    }
  }

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Task [" << Name() << "(" << _proc->pid() << "," << _outd << "," << _errd
		   << ")]: spawning process [" << _task << " " << _order.getJobOrder() << "]") ;


  if (!_proc->spawned())
  {
    try
    {
      _proc->spawn(_isDetached);

	  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Task [" << Name() << "(" << _proc->pid() << "," << _outd << "," << _errd
			       << ")]: spawning process [" << _task << " " << _order.getJobOrder() << "]") ;
    }
    catch(SpawnProc::SpawnException &ex)
    {
      ACS_LOG_ERROR("Task [" << Name() << "]: cannot spawn process [" << _task << " " << _order.getJobOrder() << "]") ;
      ACS_LOG_TRYCATCHNOTIFY_EX(TaskLauncherException(ex,"Unable to spawn process") );
      return false;
    }
    catch(exception& e)
    {
      ACS_LOG_ERROR("Task [" << Name() << "]: exception caught while spawning process [" << _task << " " << _order.getJobOrder() << "]") ;
	  ACS_LOG_NOTIFY_EX(e);
      return false;
    }

  }

  // some log...
  ACS_LOG_INFO("Task [" << Name() <<"] executing process [" << _task << " " << _order.getJobOrder() <<  "] - PID = " << _proc->pid()) ;

  return true;
}

pid_t tlTaskLauncher::pid() const // PRQA S 4020
{
	if (_proc) {
		return _proc->pid();
    }
	else { 
        return 0;
    }
}

void tlTaskLauncher::killLevel (int level, int sig) const
{

	if (level < _criticalityLevel) 
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<"), " << _criticalityLevel << " ] - Requested level [" << level << "] too low. Skipping ") ;
	}
	else
	{
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<"), " << _criticalityLevel << " ] - Requested level [" << level << "] matches. Trying to kill") ;
			
		if (!_proc )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not spawned; ignoring  signal [" << sig << "]") ;
		}
		else if (isRunning())
		{
			ACS_LOG_INFO("Task [ " << Name() <<" (" << pid() <<") ]: sending signal [" << sig <<"] to process.") ;
			_proc->kill(sig);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not running; ignoring signal [" << sig << "]") ;
		}
	}

}

void
tlTaskLauncher::kill (int sig) const
{
  if (!_proc )
  {
	 ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not spawned; ignoring  signal [" << sig << "]") ;
  }
  else if (isRunning())
  {
    ACS_LOG_INFO("Task [ " << Name() <<" (" << pid() <<") ]: sending signal [" << sig <<"] to process.") ;
    _proc->kill(sig);
  }
  else
  {
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not running; ignoring signal [" << sig << "]") ;
  }
}


void
tlTaskLauncher::killGroup (int sig) const
{
  if (!_proc )
  {
     ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not spawned; ignoring  signal [" << sig << "]") ;
  }
  else if (isRunning())
  {
    ACS_LOG_INFO("Task [ " << Name() <<" (" << pid() <<") ]: sending signal [" << sig <<"] to process.") ;
    _proc->killGroup(sig);
  }
  else
  {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Task [ " << Name() <<" (" << pid() <<") ] not running; ignoring signal [" << sig << "]") ;
  }
}


bool tlTaskLauncher::waitExitOrTimeout (unsigned long msecs) const // PRQA S 4214
{
  return _proc->waitExitOrTimeout (msecs);
}

string tlTaskLauncher::getEnvForTaskName()
{
	string envName = "TL_TASKNAME";
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	// Not mandatory
	rs.getValue (_taskNameEnvKey, envName, false);
	return envName;
}

_ACS_END_NAMESPACE
