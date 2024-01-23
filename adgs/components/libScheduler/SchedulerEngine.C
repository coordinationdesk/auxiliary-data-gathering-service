// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Scheduler Library $

	$Id$

	$Author$

	$Log$
	Revision 2.28  2017/12/14 11:29:07  marfav
	ECICMF-75
	S2PDGS-1900
	Removed the executor and task table default factory, relying on default builders
	
	Revision 2.27  2017/12/12 09:02:30  davide.tiriticco
	Added DefaultTaskTableFactory and DefaultExecutorVisitorFactory registration
	
	Revision 2.26  2017/09/20 13:18:13  davide.tiriticco
	Aligned to libTaskExecution update
	
	Revision 2.25  2017/09/19 10:04:02  davide.tiriticco
	Aligned to libTaskExecution update
	
	Revision 2.24  2015/11/27 15:13:06  marfav
	Checking configurable limits for syslog notification on per task base
	
	Revision 2.23  2015/08/28 10:13:06  matteo.airoldi
	Processor messages notification on Syslog can be disabled by configuration
	
	Revision 2.22  2015/07/17 14:13:52  marfav
	Reverting changes of APF-285
	
	Revision 2.20  2015/05/25 15:37:13  lucio.pulvirenti
	S3PDGS-2382: cleanUpWorkDir: File::unlink method called with change permission flag on to remove products even if read-only.
	
	Revision 2.19  2015/04/30 13:04:21  marfav
	Deriving privately from Thread and Proc
	Alignment to libTaskExecution 2.30 signatures
	Operation clashing with base class signatures renamed
	
	Revision 2.18  2014/07/16 09:15:10  nicvac
	Protecting LOG and TRK streams during the creation to avoid pure virtual operation calls
	
	Revision 2.17  2014/01/31 15:37:09  marfav
	S2PDGS-603 Processor messages can be filtered by configuration
	
	Revision 2.16  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.15  2013/11/19 12:46:55  marpas
	useless comments removed
	
	Revision 2.14  2013/10/18 12:27:55  marpas
	temptative version
	
	Revision 2.13  2013/10/18 11:33:29  marpas
	coding best practices applying
	
	Revision 2.12  2013/09/27 14:15:23  marpas
	streams have name
	
	Revision 2.11  2013/07/26 14:04:00  marfav
	waitExitOrTimeout operation exposed to the users
	
	Revision 2.10  2013/07/25 16:20:20  marfav
	Using synchronouws condition instead of timer delay
	
	Revision 2.9  2013/07/07 18:33:02  marpas
	application of coding best practices in progress
	some qa warnings and compilation ones removed
	adoption of libException 5.x standards in progress
	
	Revision 2.8  2013/02/26 09:00:50  marfav
	Compiler warnings removed
	
	Revision 2.7  2010/08/03 14:34:28  nicvac
	Task Table plugins management
	
	Revision 2.6  2008/04/22 11:02:05  marfav
	Aligned to new visitable task table elements
	
	Revision 2.5  2006/05/16 13:07:36  fracar
	improved comments
	
	Revision 2.4  2006/04/21 09:33:08  marfav
	RegExp check for mandatory outputs added
	
	Revision 2.3  2006/04/07 11:05:36  fracar
	logging task/pool start/stop times
	
	Revision 2.2  2006/04/07 10:24:02  marfav
	Preparing regexp output management
	
	Revision 2.1  2006/03/06 11:38:32  marfav
	Alignment to TaskTable having atexit structures
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.15  2005/07/26 13:26:55  fracar
	modified to support libScOrder rel 1.6
	
	Revision 1.14  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.13  2004/11/19 14:58:21  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.12  2004/10/13 14:04:58  marfav
	Fixed a log plugins linking issue
	
	Revision 1.11  2004/09/23 15:21:32  marfav
	TrkFilter rewritten as TrkHandler
	
	Revision 1.10  2004/09/15 09:20:22  marfav
	Versioning fixed
	
	Revision 1.9  2004/09/14 16:51:50  marfav
	Using TaskPoolLauncher
	
	Revision 1.8  2004/08/24 14:54:08  marfav
	Added satellite and mission support in the WorkOrder
	Scheduler will use TaskTable settings if WorkOrder parameters are not set
	
	Revision 1.7  2004/05/28 15:07:53  marfav
	Support for start processing time added in trk file
	
	Revision 1.6  2004/05/25 16:21:41  marfav
	Tracker file name added to scOrder structure
	
	Revision 1.5  2004/04/28 08:02:37  marfav
	Using Timer::delay instead of sleep or nanosleep
	Inserted a sleep period between two contiguous signals
	
	Revision 1.4  2004/04/15 12:28:59  marfav
	Added messages for ProcessorTracker support
	
	Revision 1.3  2004/04/09 10:11:03  marfav
	Disk quota reservation support added
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/


#include <SchedulerEngine.h>

#include <TaskTableElementExecutor.h>
#include <TaskTable.h>
#include <tlLogFile.h>
#include <ScLogHandlerGroup.h>

#include <TaskTableFactoryChain.h>
#include <TaskTableExecutorFactoryChain.h>

#include <Pool.h>
#include <ScAbstractCommand.h>
#include <ScCommandFactoryChain.h>
#include <TrkFilterable.h>
#include <TrkHandler.h>
#include <ScLogManager.h>
#include <ScLogHandlerFactoryChain.h>

#include <exStream.h>
#include <Filterables.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <Dir.h>
#include <File.h>
#include <Timer.h>
#include <DateTime.h>
#include <RegExp.h>

#include <algorithm>
#include <numeric>
#include <memory>
#include <string>



#ifdef SCHEDULER_CHECKS_DISK_SPACE

#include <sys/stat.h> // PRQA S 1013 3 
#include <sys/types.h>
#include <sys/vfs.h>

#endif // SCHEDULER_CHECKS_DISK_SPACE




#include <unistd.h>
#include <cstdlib>
#include <scV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    // Resources keys
    const string _startTimeMessageKey =		"TlScheduler.Messages.StartProcessingTime";

    // Default Values
    const string _startTimeDefaultMessage =		"processing start time #";

	const string _notifyOnSyslogConfKey = "Scheduler.NotifyOnSyslog";
	const string _taskSyslogMsgLimitKey       = "Scheduler.TaskSyslogMessageLimit";

    scV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SchedulerEngine)


// Ctor using a WorkOrder&
SchedulerEngine::SchedulerEngine (const scOrder& aWorkOrder) :  
#ifdef SCHEDULER_CHECKS_DISK_SPACE
     _reservedDiskSpaceQuota(NoReservedQuota),
#endif // SCHEDULER_CHECKS_DISK_SPACE
     _workOrder (aWorkOrder),
	 _taskTable (0),
     _log(0),
	 _internalStatus (),
     _logHandler(0),
     _theExecutor(0),
	 _satellite(),
	 _mission(),
	 _procName(),
	 _procVersion(),
	 _internalStatusMutex()
    
{
  init();
}

// Ctor reading WorkOrder from a file
SchedulerEngine::SchedulerEngine(const string& aPath)
  :  
#ifdef SCHEDULER_CHECKS_DISK_SPACE
  _reservedDiskSpaceQuota(NoReservedQuota),
#endif // SCHEDULER_CHECKS_DISK_SPACE
     _workOrder (aPath),
	 _taskTable (0),
     _log(0),
	 _internalStatus (),
     _logHandler(0),
     _theExecutor(0),
	 _satellite(),
	 _mission(),
	 _procName(),
	 _procVersion(),
	 _internalStatusMutex()
{
  init();
}


SchedulerEngine::~SchedulerEngine() throw() 
{
  if (_taskTable) { delete _taskTable; }
  if (_log) { delete _log; }
  if (_logHandler) { delete _logHandler; }
  if (_theExecutor) { delete _theExecutor; }
}


// ABORT
void
SchedulerEngine::abortBySigTerm()
{
	excout << "Scheduler: SIGTERM received: stopping all processes\n" ;
	Armageddon();
	setStatus (schedulerABORTING);
}

void
SchedulerEngine::abort()
{
	if (setStatus (schedulerABORTING))
	{
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,			// Mission
			 _procName,			// Processor Name
			 _procVersion,		// Processor Version
			 "ABORT");								// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
	}
}

void
SchedulerEngine::stop()
{
	if (setStatus (schedulerSTOPPING))
	{
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,			// Mission
			 _procName,			// Processor Name
			 _procVersion,		// Processor Version
			 "STOP");								// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
	}
}

void
SchedulerEngine::suspend()
{
	if (setStatus (schedulerSUSPENDED))
	{
		// Retrieve command
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,			// Mission
			 _procName,			// Processor Name
			 _procVersion,		// Processor Version
			 "SUSPEND");							// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
	}
}

void
SchedulerEngine::resume()
{
	if (setStatus (schedulerRUNNING))
	{
		// Retrieve command
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,			// Mission
			 _procName,			// Processor Name
			 _procVersion,		// Processor Version
			 "RESUME");								// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
	}
}

void
SchedulerEngine::kill()
{
	if (setStatus (schedulerABORTING))
	{
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,			// Mission
			 _procName,			// Processor Name
			 _procVersion,		// Processor Version
			 "KILL");								// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
	}
}

void SchedulerEngine::sendCmd (const string& cmdValue, const string& paramValue) // PRQA S 4211
{
		StringKey key = StringKeyUtils::makeKey
			(_satellite,		// Satellite
			 _mission,		// Mission
			 _procName,		// Processor Name
			 _procVersion,		// Processor Version
			 "USER");		// Command
		ACS_SMARTPTR <ScAbstractCommand> theCmd(ScCommandFactoryChain::instance() -> newObject (key));
		theCmd->value (cmdValue);
		theCmd->param (paramValue);
		// execut command 
		if(_theExecutor) {
			theCmd->execute(*_theExecutor);
        }
}




void
SchedulerEngine::init()
{

  //Build key and get task table
  //*/
  StringKey keyTT = StringKeyUtils::makeKey( _workOrder.getSatellite(), _workOrder.getMission(), _workOrder.getProcName(), _workOrder.getProcVersion() );

  // use ACS_SMARTPTR to protect against exceptions
  ACS_SMARTPTR<TaskTable> pTT( TaskTableFactoryChain::instance()->newObject( keyTT ) ); 
  pTT->readFromFile(_workOrder.getTaskTable());
  
  
  //auto_ptr<TaskTableElementExecutor> pTTEE(new TaskTableElementExecutor( *pTT, _workOrder));
  TaskTableElementExecutor::BuildParameters params( pTT.get(), _workOrder, keyTT);
  ACS_SMARTPTR<TaskTableElementExecutor> pTTEE(TaskTableExecutorFactoryChain::instance()->newObject(keyTT, params));

  // if we reach this point, we no longer need the ACS_SMARTPTR: constructors were successful
  _taskTable = pTT.release();
  _theExecutor = pTTEE.release();

  _internalStatus = schedulerIDLE;

  // Use TaskTable values if WorkOrder does not provide Satellite or Mission values
  if ( (_satellite = _workOrder.getSatellite() ) == "")
  {
     _satellite = _taskTable->getSatellite();
  }

  if ( (_mission = _workOrder.getMission() ) == "")
  {
     _mission = _taskTable->getMission();
  }

  _procName = _taskTable->getProcName();
  _procVersion = _taskTable->getProcVersion();

  // Configure the syslog output here according to the configuration spaces
  bool b (true);
  rsResourceSet& rs = ConfigurationSingleton::instance() -> get();
  // this does not throw and the default value is true
  rs.getValue (_notifyOnSyslogConfKey, b, false);
  sclog.notifyOnSyslog (b);

  // Set the message limit in the syslog for each runned task
  size_t msgLimit (100);
  rs.getValue (_taskSyslogMsgLimitKey, msgLimit, false);
  sclog.setSyslogTaskMessageLimit (msgLimit);

  // Try instantiating log handler plugin
  StringKey key = StringKeyUtils::makeKey (_satellite, _mission, _procName, _procVersion, "");
  try
  {
      // get the global stream mutex to avoid notifications during the ctor
      ThreadSafe::Lock lock (exostream::globalMutex());
      _logHandler = ScLogHandlerFactoryChain::instance()->newObject(key, _workOrder);
   }

  catch (const exException& toCatch)
  {
      // not a problem. All the log will be re-directed to Application logfile
      ACS_LOG_WARNING("Scheduler::init : WARNING unable to find a log handler. Key is " << key) ;
  }
}

SchedulerStatus SchedulerEngine::getStatus() // PRQA S 4211
{
	Lock guard(_internalStatusMutex);
	return _internalStatus;
}

void
SchedulerEngine::updateStatus (SchedulerStatus newStatus)
{
	excout << "Scheduler: updating internal status to " << schedulerStatus2String (newStatus) << '\n' ;
	Lock guard(_internalStatusMutex);
	_internalStatus = newStatus;
}


bool
SchedulerEngine::setStatus (SchedulerStatus newStatus)
{
	bool retvalue = false;
	if (newStatus == getStatus()) 
	{
		// Allow loops on steady state
		retvalue = true;
	}
	else
	{
		switch (getStatus())
		{
		case schedulerIDLE:
            {
			    if (newStatus == schedulerRUNNING) {
				    retvalue = true;
                }
		        break;
            }
		case schedulerRUNNING:
            {
			    if (	(newStatus == schedulerSUSPENDED) ||
				    (newStatus == schedulerSTOPPING) ||
				    (newStatus == schedulerABORTING) ||
				    (newStatus == schedulerCOMPLETED) ) {
				    retvalue = true;
                }
		        break;
            }
		case schedulerSUSPENDED:
            {
			    // due to updating status logic
			    // methods implementing changing status commands
			    // must update status befor starting  their execution

			    if (newStatus == schedulerRUNNING) {
				    retvalue = true; // resuming
                }
			    else if (	(newStatus == schedulerSTOPPING) ||
						    (newStatus == schedulerABORTING) ||
						    (newStatus == schedulerCOMPLETED) )
			    {
				    resume();
				    Timer::delay (250); // give signals time to propagate // PRQA S 4400
				    retvalue = true;
			    }
                else {
                    // nothing 
                }
		        break;
            }
		case schedulerSTOPPING:
            {
			    if ((newStatus == schedulerABORTING) ||
				    (newStatus == schedulerCOMPLETED) ) {
				    retvalue = true;
                }
		        break;
            }
		case schedulerABORTING:
            {
			    if ( newStatus == schedulerCOMPLETED ) {
				    retvalue = true;
                }
		        break;
            }
		case schedulerCOMPLETED:
            {
			    retvalue = false; // not useful but meaningful ;)
		        break;
            }
		default: 
            {
			    retvalue = false;
            }
		}
		if (retvalue)
		{
			updateStatus (newStatus);
		}
	}
	return retvalue;
}

bool
SchedulerEngine::isSuspended()
{
	return (getStatus() == schedulerSUSPENDED);
}

bool
SchedulerEngine::isExiting()
{
	return ((getStatus() == schedulerSTOPPING) || (getStatus() == schedulerABORTING));
}


void
SchedulerEngine::manageBackgroundTasks()
{
    if (_workOrder.doIKillBackgroundTasks())
    {
      excout << "Scheduler: Pools execution loop completed. Killing background tasks (if any)\n" ;
      Armageddon();
    }
    else
    {
      excout << "Scheduler: Pools execution loop completed. Waiting for background tasks (if any) to terminate\n";

      // gotta wait for those dummy background tasks to finish
      _theExecutor->waitDetachedTasks();
    }
}



SchedulerExitStatus SchedulerEngine::Activate() // PRQA S 4020
{

  // Report the processor logs on the application log only if requested 
  sclog.notifyStdStreams (_workOrder.getLogProcMessages());

  // Enable the TrkFilterable handler if a TrackerFile is requested
  // and the file is writable
  ACS_SMARTPTR<TrkHandler> theTrkHandler;

  string trackerIpcFileName = _workOrder.getTrackerFileName();
  if (trackerIpcFileName != "")
  {
    try
    {
       // Lock the global mutex to avoid notifications during the CTOR
       ThreadSafe::Lock lock (exostream::globalMutex());
       ACS_SMARTPTR<TrkHandler> tempHandler (new TrkHandler("tmphnd", trackerIpcFileName, &sclog) );
       theTrkHandler.reset (tempHandler.release());
    }
    // notify the failure and then continue without any tracker file (the messages will be put in the logfile)
    catch (exception& ex)
    {
      ACS_LOG_ERROR("SchedulerEngine: ERROR cannot open Tracker Communication File [" << trackerIpcFileName << "]") ;
      ACS_LOG_NOTIFY_EX (ex);
    }
  }

  // watch out! next two checks cannot be done in the reverse order,
  // since the working directory could not exist and be created by
  // the checkWorkDir call
  if ( !checkWorkDir(_workOrder.getBaseDir()) )
  {
    // messages have been already issued
    ACS_LOG_ERROR("Scheduler: exiting return code NO_WDIR (" << int(ssNO_WDIR) << ")"); // PRQA S 3081
    return ssNO_WDIR;
  }

#ifdef SCHEDULER_CHECKS_DISK_SPACE
  if ( !checkDiskSpace(_taskTable->getMinDiskSpace(), _workOrder.getBaseDir()) ) // PRQA S 3000
  {
    ACS_LOG_ERROR("Scheduler: ERROR: not enough disk space on " << _workOrder.getBaseDir()) ;
    ACS_LOG_ERROR("Scheduler: exiting return code NO_SPACE (" << int(ssNO_SPACE) << ")") ; // PRQA S 3081
    return ssNO_SPACE;
  }
#endif // SCHEDULER_CHECKS_DISK_SPACE

  // position in the working dir
  if (chdir(_workOrder.getBaseDir().c_str()) != 0)
  {
    ACS_LOG_ERROR("Scheduler: ERROR: cannot chdir to " << _workOrder.getBaseDir() <<
	      "[errno=" << errno << "]") ;
    ACS_LOG_ERROR("Scheduler: exiting return code NO_WDIR (" << int(ssNO_WDIR) << ")") ; // PRQA S 3081

    return ssNO_WDIR;
  }

  // Log messages are used as interface to ProcessorTracker thread
  rsResourceSet& rs = ConfigurationSingleton::instance() -> get();

  string startTimeMessage;
  if (rs.hasKey (_startTimeMessageKey))
  {
    rs.getValue (_startTimeMessageKey, startTimeMessage);
  }
  else
  {
    ACS_LOG_WARNING("Scheduler: WARNING: cannot find the resource [" << _startTimeMessageKey << "]." <<
              "Using default value [" << _startTimeDefaultMessage << "]") ;
    startTimeMessage = _startTimeDefaultMessage;
  }


  SchedulerStatus loopExitStatus = schedulerRUNNING;
  updateStatus (schedulerRUNNING);

  // give the start processing time to the tracker
  {
    TrkFilterable msg;
    DateTime currentTime;
    string tmp;
    msg << startTimeMessage << currentTime.utcs(tmp) << '\n' ;
    sclog << msg;
  }

  _theExecutor->startThread();
  while ( _theExecutor->isRunning())
  {
	  // something to do during this loop ??
	  _theExecutor->waitExitOrTimeoutThread(100); // PRQA S 4400
  }

  // If the executor has a valid exit code and the exit code is zero
  // means that all the tasks have been executed 
  
  int executorExitCode = _theExecutor->getExitCode ();
  bool exitCodeValid = _theExecutor->hasExitCode ();
  
  // all tasks have been run ?
  bool workCompleted = ((executorExitCode == 0) && (exitCodeValid));
  loopExitStatus = getStatus();

  // if exit occurred before work completion
  // the scheduler was stopped or aborted so cannot return ssOK
  // but in this way I kill the background tasks even if I had to wait them

  SchedulerExitStatus theReturnValue = ssOK;

  if (workCompleted)
  {
       manageBackgroundTasks();
       updateStatus (schedulerCOMPLETED);
       ACS_LOG_INFO("Scheduler: exiting return code OK (" << int(ssOK) << ")") ; // PRQA S 3081

       // return ssOK;
       theReturnValue = ssOK;
  } // workCompleted

  // some error or incomplete run
  else if (loopExitStatus == schedulerABORTING)
  {
      ACS_LOG_INFO("Scheduler: Aborting in progress. Killing background tasks (if any)") ;
      Armageddon();
      updateStatus (schedulerCOMPLETED);
      ACS_LOG_INFO("Scheduler: exiting return code ABORTED (" << int(ssABORTED) << ")")  ; // PRQA S 3081
      theReturnValue = ssABORTED;
  }
  else if (loopExitStatus == schedulerSTOPPING)
  {
      ACS_LOG_INFO("Scheduler: Stopping in progress. Managing background tasks") ;
      manageBackgroundTasks();
      updateStatus (schedulerCOMPLETED);
      ACS_LOG_INFO("Scheduler: exiting return code STOPPED (" << int(ssSTOPPED) << ")") ; // PRQA S 3081
      theReturnValue = ssSTOPPED;
  }
  else
  {
      ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Scheduler pool execution error: task failure; killing remaining tasks/pools") ;

      Armageddon();
      updateStatus (schedulerCOMPLETED);
      ACS_LOG_INFO("Scheduler: exiting return code TASK_FAILED (" << int(ssTASK_FAILED) << ")")  ; // PRQA S 3081
      theReturnValue = ssTASK_FAILED;
  }

    // WorkingDir will be cleaned up if
    // 1) doICleanupOnExit is requested
    //    AND
    // 2) the processing is completed OR the cleanup is applicable also in case of errors
    if ( _workOrder.doICleanupOnExit() && (workCompleted || _workOrder.applyCleanupPolicyOnError()) )
    {
        ACS_LOG_PRIVINFO("Scheduler: cleaning up working dir " << _workOrder.getBaseDir()) ;
        cleanupWorkDir(_workOrder.getBaseDir());
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "no working dir cleanup requested in the work order") ;
    }

  return theReturnValue;
}


#ifdef SCHEDULER_CHECKS_DISK_SPACE
size_t
SchedulerEngine::getReservedDiskSpaceQuota() const
{
    Lock guard(_internalStatusMutex);
    return _reservedDiskSpaceQuota;
}


void
SchedulerEngine::setReservedDiskSpaceQuota(size_t MB)
{
    Lock guard(_internalStatusMutex);
    _reservedDiskSpaceQuota = MB;
}



bool
SchedulerEngine::checkDiskSpace(size_t requested, const string& work_dir) const // PRQA S 4020
{
    struct statfs buf; // PRQA S 4102

    if (statfs(work_dir.c_str(), &buf) == -1) // error
    {
	    ACS_LOG_ERROR("Scheduler: ERROR: cannot statfs working dir " << work_dir <<
		  " [errno=" << errno << "]") ;
	    return false;
    }

    Lock guard(_internalStatusMutex);
    
    const int BYTES_PER_MEGA = 1048576 ;

    size_t totalDiskSpace = buf.f_bavail * buf.f_bsize / BYTES_PER_MEGA; // PRQA S 3000, 3084
    size_t reserved = getReservedDiskSpaceQuota();

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, buf.f_bavail <<" free blocks * " << buf.f_bsize << " bytes per block = "
				  << totalDiskSpace << " Mb");
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "requested free space is " << requested << " Mb") ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "reserved quota is " << reserved << " Mb") ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "free space needed is " << requested + reserved << " Mb") ; // PRQA S 3084 2
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "the check is " << ((totalDiskSpace >= (requested+reserved))?"":"NOT ") << "successful") ; // PRQA S 3380

    return (totalDiskSpace >= (requested + reserved)); // PRQA S 3084
}
#endif // SCHEDULER_CHECKS_DISK_SPACE

bool
SchedulerEngine::checkWorkDir(const string& aPath) const // PRQA S 4020
{
  if (File::exists(aPath)) {
    return true;
  }
  else {
    if (!_workOrder.doICreateDir())
    {
      ACS_LOG_ERROR("Scheduler: ERROR: working dir " << aPath << "does not exist") ;
      return false;
    }
    else
    {
      if (mkdir(aPath.c_str(), 0775) == 0) // PRQA S 4400
      {
	return true;
      }
      else
      {
	    ACS_LOG_ERROR("Scheduler: ERROR: cannot create working dir " << aPath <<
	          "[errno=" << errno << "]");

	    return false;
      }
    }
  }
  return true; // never reach this point: just to make that dummy compiler stop screaming
}

void
SchedulerEngine::cleanupWorkDir(const string& aPath) const
{
  ACS_LOG_PRIVINFO("Scheduler: cleaning up working dir " << aPath) ;

  bool changePermissions = true;
  ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"executing call to File::unlink method with flag changePerm = " << boolalpha << changePermissions)  ;

  File::unlink(aPath, changePermissions);
}



void
SchedulerEngine::Armageddon() // PRQA S 4211
{
    _theExecutor->armageddon();
}


_ACS_END_NAMESPACE

