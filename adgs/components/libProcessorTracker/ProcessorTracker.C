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

	$Prod: A.C.S. Processor Tracker Library $

	$Id$

	$Author$

	$Log$
	Revision 2.9  2014/06/05 17:53:01  marpas
	coding best practices applied
	compilation and qa warnings fixed
	
	Revision 2.8  2014/04/14 14:36:06  marpas
	adopting DateTime 5.13 interface
	qa and g++ warnings fixed
	tests are wrong ...
	
	Revision 2.7  2013/04/09 08:02:43  marpas
	deprecated macro removed
	
	Revision 2.6  2010/08/03 14:32:58  nicvac
	Task Table plugins management
	
	Revision 2.5  2009/06/26 10:47:27  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.4  2008/04/22 10:57:38  marfav
	Aligned to new visitable task table elements
	
	Revision 2.3  2006/03/27 18:11:48  fracar
	fixed tracking problem when executable file does not exist for a task (no start message was generated)
	
	Revision 2.2  2006/03/09 14:31:37  giucas
	do not catch (...) anymore
	do not use references to object returned by tasktable (tasks or pools)
	
	Revision 2.1  2006/03/06 11:33:22  marfav
	Alignment to TaskTable having atexit structures
	
	Revision 2.0  2006/02/28 10:18:10  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.15  2005/05/13 10:07:26  marfav
	Thread safe lock objects management fixed
	
	Revision 1.14  2004/12/03 17:50:50  marfav
	Header fixed
	
	Revision 1.13  2004/11/15 16:50:50  marfav
	CryosatProcessorTracker included in ProcessorTracker
	CryosatProcessorTracker prepared for deletion
	
	Revision 1.12  2004/09/29 13:14:58  marfav
	Bug fixed on time estimation when processing percentage is 100
	
	Revision 1.11  2004/09/29 12:48:31  marfav
	Added support for processing completion time estimation
	
	Revision 1.10  2004/09/28 16:33:30  marfav
	Addedd support for task percentage
	
	Revision 1.9  2004/09/20 15:38:22  marfav
	Added getTaskDescriptors method
	
	Revision 1.8  2004/09/01 09:17:56  marfav
	Added support for exit code of tracked processes
	
	Revision 1.7  2004/05/28 15:08:18  marfav
	Support for processing start time added
	
	Revision 1.6  2004/05/26 09:33:58  marfav
	Now the tracker thread waits for not existing trk files until stop requests
	
	Revision 1.5  2004/05/25 16:26:05  marfav
	Added a ctor
	now ProcessorTracker can track a scheduler log
	using orderId and processingRootPath
	
	Revision 1.4  2004/05/11 09:40:43  marfav
	Bug in LineSplitter fixed
	
	Revision 1.2  2004/04/26 10:33:46  marfav
	Using Timer::delay instead of nanosleep
	
	Revision 1.1.1.1  2004/04/15 12:19:57  marfav
	Import libProcessorTracker
	
	

*/


#include <ProcessorTracker.h>

#include <TaskTable.h>
#include <StringUtils.h>
#include <TaskTableFactoryChain.h>
#include <exException.h>
#include <exStream.h>
#include <Timer.h>
#include <scOrder.h>
#include <Application.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <RegExp.h>

#include <ctime>
#include <sys/types.h> // PRQA S 1013 2 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ptrkV.h>


_ACS_BEGIN_NAMESPACE(acs)
	
using namespace std;


namespace {
 // Resources keys
 const string _taskStartMessageKey =     "TlScheduler.Messages.TaskStarted";
 const string _taskCompleteMessageKey =  "TlScheduler.Messages.TaskCompleted";
 const string _taskFailureMessageKey =   "TlScheduler.Messages.TaskFailed";
 const string _startTimeMessageKey =     "TlScheduler.Messages.StartProcessingTime";
 const string _taskPercentageMessageKey ="TlScheduler.Messages.TaskPercentage";

 // Default Values
 const string _taskStartDefaultMessage =     "starting execution of task #";
 const string _taskCompleteDefaultMessage =  "completed execution of task #"; // completed execution of task #<TASKNAME>[<EXITCODE>]
 const string _taskFailureDefaultMessage =   "execution failed on task #"; // execution failed on task #<TASKNAME>[<EXITCODE>]
 const string _startTimeDefaultMessage =     "processing start time #";
 const string _taskPercentageDefaultMessage ="updating percentage of task #"; // updating percentage of task #<TASKNAME>[<pct>]

 ptrkV version;

}


ACS_CLASS_DEFINE_DEBUG_LEVEL(ProcessorTracker);


// Helper class that manages blocks of text
// returning a vector of lines having '\n' as delimiter

class LineSplitter // PRQA S 2109
{

public:
    LineSplitter() : _lines(), _buffer() {};
    ~LineSplitter() throw() {};

	void addBlock(const string&  newBlock)
	{
		_buffer.append (newBlock);
		size_t firstIndex = 0 ;
        size_t lastIndex = 0 ;
		while ((lastIndex = _buffer.find('\n',firstIndex)) != string::npos  )
		{
			string temp = _buffer.substr (firstIndex, lastIndex-firstIndex); // PRQA S 3084
			// do not push back empty messages
			if (temp.size() > 0) { _lines.push_back (temp);  }
            firstIndex = lastIndex+1; // PRQA S 3084
		}

		// check if there is a remainder and store it for using in the next block
		if (firstIndex != _buffer.size())
		{
			_buffer = _buffer.substr (firstIndex, _buffer.size() - firstIndex); // PRQA S 3084
		}
		else
		{
			_buffer = "";
		}
	}

	void getLines (vector<string>& result)
	{
		result = _lines;
		_lines.clear();
	}
private:
    LineSplitter(const LineSplitter &) ; // not implemented
    LineSplitter& operator= (const LineSplitter &) ; // not implemented
private:
	vector<string> _lines;
	string _buffer;
};

// *********************
// CTOR
// *********************

namespace {
    inline string th_name(int oid) { ostringstream os ; os << "Traker_" << oid; return os.str() ; } // PRQA S 2134
}

ProcessorTracker::ProcessorTracker(int orderId, const string &rootPath) :
    Thread(th_name(orderId)), 
	_taskTable (),
	_logFile (""),
	_lineSplitter (new LineSplitter()),
	_fd(-1),
	_ProcessorDescrMap(),
	_processingStartTime ("01-JAN-1970_00:00:00.000"),
	_lastComputedEstimationTime ("01-JAN-1970_00:00:00.000"),
	_expectedStopTime ("01-JAN-1970_00:00:00.000"),
	_estimationPercentage(),
	_estimatedOnePercentTime(),	
	_progressChanged(),
	_currentCompletionPercentage(),
	_taskStartingMessage(),
	_taskCompletionMessage(),
	_taskFailureMessage(),
	_startTimeMessage(),
    _taskPercentageMessage(),
	_taskStartingRegExp(),
	_taskCompletionRegExp(),
	_taskFailureRegExp(),
	_startTimeRegExp(),
    _taskPercentageRegExp()


{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Start") ;

	scOrder wo ( ProcessorTracker::computeWorkOrderName (orderId, rootPath) );
	_logFile = wo.getTrackerFileName();
	if (_logFile == "")
	{
		_logFile = wo.getSchedulerLogName();
	}
	if (_logFile == "")
	{
		_logFile = ProcessorTracker::computeSchedulerLogName(orderId);
	}
	readTaskTable (wo);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Executed") ;

}

ProcessorTracker::ProcessorTracker (const scOrder& wo) :
    Thread("Tracker_" + wo.getBaseDir()), 
	_taskTable (),
	_logFile (""),
	_lineSplitter (new LineSplitter()),
	_fd(-1),
	_ProcessorDescrMap(),
	_processingStartTime ("01-JAN-1970_00:00:00.000"),
	_lastComputedEstimationTime ("01-JAN-1970_00:00:00.000"),
	_expectedStopTime ("01-JAN-1970_00:00:00.000"),
	_estimationPercentage(),
	_estimatedOnePercentTime(),
	_progressChanged(false),
	_currentCompletionPercentage(0),
	_taskStartingMessage(),
	_taskCompletionMessage(),
	_taskFailureMessage(),
	_startTimeMessage(),
	_taskPercentageMessage(),
	_taskStartingRegExp(),
	_taskCompletionRegExp(),
	_taskFailureRegExp(),
	_startTimeRegExp(),
	_taskPercentageRegExp()
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Start") ;

	_logFile = wo.getTrackerFileName();
	if (_logFile == "")
	{
		_logFile = wo.getSchedulerLogName();
	}

	readTaskTable (wo);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Executed") ;
}


ProcessorTracker::ProcessorTracker() :
    Thread("Tracker_<def_ctor>"), 
	_taskTable (),
	_logFile (""),
	_lineSplitter (new LineSplitter()),
	_fd(-1),
	_ProcessorDescrMap(),
	_processingStartTime ("01-JAN-1970_00:00:00.000"),
	_lastComputedEstimationTime ("01-JAN-1970_00:00:00.000"),
	_expectedStopTime ("01-JAN-1970_00:00:00.000"),
	_estimationPercentage(),
	_estimatedOnePercentTime(),
	_progressChanged(),
	_currentCompletionPercentage(),
	_taskStartingMessage(),
	_taskCompletionMessage(),
	_taskFailureMessage(),
	_startTimeMessage(),
    _taskPercentageMessage(),
	_taskStartingRegExp(),
	_taskCompletionRegExp(),
	_taskFailureRegExp(),
	_startTimeRegExp(),
    _taskPercentageRegExp()

{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Start") ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CTOR - Executed") ;

}


// *********************
// DTOR
// *********************
ProcessorTracker::~ProcessorTracker () throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DTOR - Start") ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stopping thread if running") ;

	try {
		ProcessorTracker::stop();
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DTOR - Executed") ;
    // PRQA L:L1
}

// *********************
// readTaskTable
// *********************
void
ProcessorTracker::readTaskTable (const scOrder& wo)
{
	Lock lock(mutex()) ;
	
	// if the creation of the TaskTable fails
	// it will throw an exception and the ctor will fail

	//Build key and get task table
	//*/
	StringKey key = StringKeyUtils::makeKey(  wo.getSatellite(), wo.getMission(), wo.getProcName(), wo.getProcVersion() );
	ACS_SMARTPTR<TaskTable> theTaskTable( TaskTableFactoryChain::instance()->newObject( key ) );
	theTaskTable->readFromFile( wo.getTaskTable() );

	// have a valid task table pointer,
	// take ownership
	_taskTable.reset(theTaskTable.release()) ;

}

// *************************
// Estimation time algorithm
// *************************

double 
ProcessorTracker::getTimeLeftEstimation (int& flags)
{
	Lock lock(mutex()) ;
	long double secs = 0;
	
	if (getTotalPercentComplete() >= 100) // PRQA S 4400
	{
		// There is nothing to compute, just return !
		secs = 0;
		flags = 0;
	}
	else
	{
		doTimeComputation();
		
		if (_lastComputedEstimationTime == DateTime ("01-JAN-1970_00:00:00.000")) // PRQA S 3081
		{
			// Never computed the time... set the right flag and return
			flags = 1;
		}
		else
		{
			flags = 0;
			DateTime now;
			long double deltaTime = _expectedStopTime.jd50() - now.jd50() ;
			secs = deltaTime * (60 *60 * 24 ); // fraction of seconds // PRQA S 4400
			if (secs <= 0) 
			{
				// Bad estimation time !!!
				// Return one second and compute the estimation again
				secs = 1;
				timeComputation();
			}
		}
	}
	return double(secs) ; // PRQA S 3081
}

void 
ProcessorTracker::doTimeComputation()
{
	Lock lock(mutex()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "begin.") ;

	if (_processingStartTime != DateTime ("01-JAN-1970_00:00:00.000")) // PRQA S 3081
	{
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing start time set") ;
		
		// The processing has started !!
		if (_estimationPercentage != getTotalPercentComplete() ) 
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing percentage has changed") ;
			
			// The processing percentage has changed
			timeComputation();			
		}
		else
		{
			// The processing percentage is unchanged
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing percentage has NOT changed") ;
			
			long double elapsedTime = DateTime().jd50() - _lastComputedEstimationTime.jd50();
			if (( _estimatedOnePercentTime > 0) && (elapsedTime > _estimatedOnePercentTime)) 
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "one percent delay treshold reached") ;
				
				// Force the time computation now!
				timeComputation();
			}
		}
	}
	else
	{
		// The processing has not been started
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing start time NOT set") ;
	}
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end.") ;

}


void
ProcessorTracker::timeComputation()
{
	Lock lock(mutex()) ;
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "begin.") ;

	
	DateTime now;
	float pct = getTotalPercentComplete();
	long double elapsedTime = now.jd50() - _processingStartTime.jd50();
	long double estimatedTime = elapsedTime / pct * 100;
	_expectedStopTime = DateTime ( DateTime::MJD(estimatedTime + DateTime::MJD(_processingStartTime)) ); // PRQA S 3050, 3081
	_lastComputedEstimationTime = now;
	_estimationPercentage = getTotalPercentComplete();
	_estimatedOnePercentTime = double(elapsedTime / _estimationPercentage) ; // PRQA S 3081
	
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end.") ;
}


// *********************
// processorStarted
// *********************
bool 
ProcessorTracker::processorStarted() const 
{
	Lock lock(mutex()) ;
	return !_ProcessorDescrMap.empty();
}


// *********************
// getTaskDescriptors
// *********************
//
void 
ProcessorTracker::getTaskDescriptors (vector<TaskList>& theList) const
{
	Lock lock(mutex()) ;
	TaskList myList;
	getTaskDescriptors(myList);
	theList.push_back(myList);
}

namespace {
    // STL HELPER CLASS
    class TaskDescrPusher // PRQA S 2109
    {
    public:
	    explicit TaskDescrPusher (ProcessorTracker::TaskList& list) : _list(list) {} // PRQA S 2528 2
	    TaskDescrPusher (const TaskDescrPusher& r) : _list(r._list) {} 
        ~TaskDescrPusher() throw() {}

 
	    void operator() (const pair <string, ProcessorTracker::TaskDescr>& p)
	    {
		    _list.push_back(p.second);
	    }
    private:
        TaskDescrPusher() ; // not implemented
	    TaskDescrPusher& operator= (const TaskDescrPusher& r) ; // not implemented
    private:
	    ProcessorTracker::TaskList& _list;
    };
}
void 
ProcessorTracker::getTaskDescriptors (TaskList& theList) const
{
	Lock lock(mutex()) ;
	for_each (_ProcessorDescrMap.begin(), _ProcessorDescrMap.end(), TaskDescrPusher (theList)); // PRQA S 3081
}


// *********************
// getProcessingStartTime
// *********************

bool
ProcessorTracker::getProcessingStartTime(DateTime& date) const // PRQA S 4020
{
	Lock lock(mutex()) ;
	if (_processingStartTime == DateTime ("01-JAN-1970_00:00:00.000")) // PRQA S 3081
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning false") ;

		return false;
	}
	else
	{
		date = _processingStartTime;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning true. Time [" << date.asISO8601(6) << "]") ;

		return true;
	}
}



// **************************
// getTotalPercentComplete
// **************************
float
ProcessorTracker::getTotalPercentComplete() // const
{
    Lock lock(mutex()) ;

    if (_progressChanged) 
    {
	    updateCompletionPercentage();
	    _progressChanged = false;
    }

    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning value " << _currentCompletionPercentage) ;
    
    
    return _currentCompletionPercentage;
}


// **************************
// updateCompletionPercentage
// **************************
void 
ProcessorTracker::updateCompletionPercentage()
{
    Lock lock(mutex()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "updating requested") ;

    float pct = 0;	    
    for (map<string, TaskDescr>::const_iterator it = _ProcessorDescrMap.begin(); // PRQA S 4238
	 it != _ProcessorDescrMap.end();
	 ++it) 
    {
	    ACS_CLASS_BGN_DEBUG(10)
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ProcessorTracker::updateCompletionPercentage() : task [" << it->second.name << "]") ;
	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "- completed percentage : " << it->second.pctComplete) ;
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "- weight               : " << it->second.taskWeight) ;
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "- overall weight       : " << it->second.taskWeight * it->second.pctComplete / 100) ;
	    ACS_CLASS_END_DEBUG
	    
	    // add the contribution of each task
	    pct += it->second.pctComplete * it->second.taskWeight / 100.0; // PRQA S 3012, 4400
    }
    _currentCompletionPercentage = pct;
    
}



// **************************
// _init
// **************************
void
ProcessorTracker::_init()
{

	Lock lock(mutex()) ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "starting initialization... ") ;

	rsResourceSet& rs = ConfigurationSingleton::instance() -> get();

	// Loading task starting message settings
	if (rs.hasKey (_taskStartMessageKey))
	{
		rs.getValue (_taskStartMessageKey, _taskStartingMessage);
	}
	else
	{
		_taskStartingMessage = _taskStartDefaultMessage;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default task starting message [" << _taskStartDefaultMessage << "]") ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task starting message set to [" << _taskStartingMessage << "]") ;


	// Loading task completion message settings
	if (rs.hasKey (_taskCompleteMessageKey))
	{
		rs.getValue (_taskCompleteMessageKey, _taskCompletionMessage);
	}
	else
	{
		_taskCompletionMessage = _taskCompleteDefaultMessage;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default task completion message [" << _taskCompleteDefaultMessage << "]" ) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task completion message set to [" << _taskCompletionMessage << "]") ;


	// Loading task failure message settings
	if (rs.hasKey (_taskFailureMessageKey))
	{
		rs.getValue (_taskFailureMessageKey, _taskFailureMessage);
	}
	else
	{
		_taskFailureMessage = _taskFailureDefaultMessage;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default task failure message [" << _taskFailureDefaultMessage << "]") ;
	}


	// Loading start time message settings
	if (rs.hasKey (_startTimeMessageKey))
	{
		rs.getValue (_startTimeMessageKey, _startTimeMessage);
	}
	else
	{
		_startTimeMessage = _startTimeDefaultMessage;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default start time message [" << _startTimeDefaultMessage << "]") ;
	}

	// Loading task percentage message settings
	if (rs.hasKey (_taskPercentageMessageKey))
	{
		rs.getValue (_taskPercentageMessageKey, _taskPercentageMessage);
	}
	else
	{
		_taskPercentageMessage = _taskPercentageDefaultMessage;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "using default task percentage message [" << _taskPercentageDefaultMessage << "]") ;
	}


	// Regular expression generation
	ostringstream expr;
	expr << ".*" << _taskStartingMessage;
	_taskStartingRegExp = expr.str();
	
    expr.str("");
	expr << ".*" << _taskCompletionMessage << ".*\\[\\-?[0-9]+\\]$";
	_taskCompletionRegExp = expr.str();
	
    expr.str("");
	expr << ".*" << _taskFailureMessage << ".*\\[\\-?[0-9]+\\]$";
	_taskFailureRegExp = expr.str();
	
    expr.str("");
	expr << ".*" << _startTimeMessage;
	_startTimeRegExp = expr.str();
	
    expr.str("");
	expr << ".*" << _taskPercentageMessage << ".*\\[\\-?[0-9]+\\]$";
	_taskPercentageRegExp = expr.str();


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "regular expression generated:") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TaskStarting    = \"" << _taskStartingRegExp << "\"") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TaskCompletion  = \"" << _taskCompletionRegExp << "\"") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TaskFailure     = \"" << _taskFailureRegExp << "\"") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StartTime       = \"" << _startTimeRegExp << "\"") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TaskPercentage  = \"" << _taskPercentageRegExp << "\"") ;
	ACS_CLASS_END_DEBUG

				
	// do specific derived classes initialization
	init();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executed.") ;

}



// **************************
// init
// **************************

void
ProcessorTracker::init ()
{
}




// **************************
// run
// **************************
void
ProcessorTracker::run()
{

	ACS_COND_THROW (0 == _taskTable.get(), ProcessorTrackerException ("Task Table not set")); // PRQA S 3081

	// initialize configuration parameters
	_init();

	struct stat oldStat ; // PRQA S 4102 2
    struct stat newStat ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "starting thread... ") ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "opening file " << _logFile) ;


	while ((_fd<0) && !requestedCancel() )
	{
		_fd = open (_logFile.c_str(), O_RDONLY);
		if (_fd < 0)
		{
			// ostringstream os;
			// os << "Unable to open logfile [" << _logFile << "]";
			// ACS_THROW (ProcessorTrackerException (os.str(), errno));
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "unable to open file - sleeping one second") ;
			Timer::delay (1000); // 1000 msec // PRQA S 4400
		}
	}

	if (!requestedCancel())
	{

		if (fstat (_fd, &newStat) < 0)
		{
			ACS_THROW (ProcessorTrackerException ("Unable to stat logfile [" + _logFile + "]", errno));
		}

		oldStat = newStat;
		bool haveToFollow = false ;

		while (!requestedCancel())
		{
			followLogFile();
  	
			haveToFollow = false;
			while (!haveToFollow && !requestedCancel())
			{

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "going to sleep") ;

				Timer::delay (100); // sleep 100 msecs // PRQA S 4400
  	
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking file stat") ;

				if (fstat (_fd, &newStat) < 0)
				{
					ACS_THROW (ProcessorTrackerException ("Unable to stat logfile [" + _logFile + "]", errno));
				}
				if ( (newStat.st_mtime != oldStat.st_mtime) || (newStat.st_size  != oldStat.st_size ) )
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file was changed") ;
					// The log file was modified
					oldStat = newStat;
					haveToFollow = true;
				}
			}
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "closing file " << _logFile) ;

	if (_fd >= 0) {
		close (_fd);
    }

	// reset _fd
	_fd = -1;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting thread.") ;

}


// **************************
// followLogFile
// **************************

#define MAX_MSG_LEN 4096

void
ProcessorTracker::followLogFile() // PRQA S 4020
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "starting ... ") ;
	// no opened file ... not necessary an error
	if (_fd == -1) {
		return;
    }

	char buf[MAX_MSG_LEN+1]; // PRQA S 1021
	ssize_t count = 0 ;
	string string_buf;

	// read until end of file and store in a string
	while ( (count = read(_fd, buf, sizeof(buf)-1)) != 0) // PRQA S 3084, 4236
	{
		if (count == -1)
		{
			ACS_THROW (ProcessorTrackerException ("Unable to read from logfile [" + _logFile + "]", errno));
		}
		replace(buf, &buf[count], '\r', '\n');
		buf[count] = '\0';
		string_buf.append (buf);
	}

	// if nothing read -> do nothing !
	if (string_buf.empty()) {
		return;
    }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding block [" << string_buf << "]") ;

	_lineSplitter->addBlock(string_buf);
	vector<string> lines;
	_lineSplitter->getLines(lines);
	for (vector<string>::iterator it=lines.begin(); it!=lines.end(); ++it) // PRQA S 4238
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "decoding line [" << *it << "]") ;

		decodeLogLine (*it);
	}

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executed.") ;


}





// **************************
// taskStarted
// **************************
void
ProcessorTracker::taskStarted(const string &task_name, const string &task_version, bool background) // creates new TaskList entry // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start ....") ;
	Lock lock(mutex()) ;
	_progressChanged = true;


	// Create a new TaskDescr structure and store task information
	TaskDescr theDescr;
	theDescr.name = task_name;
	theDescr.version = task_version;
	theDescr.pctComplete = 0;
	theDescr.taskWeight = 100; // PRQA S 4400
	theDescr.background = background;
	theDescr.completionStatus = RUNNING;
	theDescr.validExitCode = false;
	theDescr.exitCode = 0;

	try
	{
		//Task t = (*_taskTable).TaskTableElement::operator[] (task_name);
		Task t = (*_taskTable)[task_name];
		theDescr.taskWeight = t.getAssignedPercentage();

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Task retrieved ....") ;

	}
	catch (exception& e)
	{
		ACS_LOG_TRYCATCHNOTIFY_EX( ProcessorTrackerException (e, "Unable to register start for task [" + task_name +", "+ task_version + "]. Task does not exists in task table")); // PRQA S 3081
		return;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inserting TaskDescr ") ;

	if (_ProcessorDescrMap.find(task_name) != _ProcessorDescrMap.end()) 
	{
		// DUPLICATED TASK !!
		// This should never happen
		// Complain about that and go on
 	    ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException ("Task [" + task_name + "] start has been already detected. Percentage computation accuracy affected.")); // PRQA S 3081
	}

	_ProcessorDescrMap[task_name] = theDescr;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executed.") ;
}


// **************************
// taskStopped
// **************************

void
ProcessorTracker::taskStopped(const string &task_name, Status completionStatus, int exitCode) // updates TaskList
{
	Lock lock(mutex()) ;
	_progressChanged = true;
	
	// first retrieve the task from current TaskList

	map<string, TaskDescr>::iterator it = _ProcessorDescrMap.find(task_name);
	
	if (it != _ProcessorDescrMap.end())
	{
		// 100% complete !!
		it->second.pctComplete = 100.0; // PRQA S 3012, 4400
		it->second.completionStatus = completionStatus;
		if (exitCode != -1)
		{
			it->second.exitCode = exitCode;
			it->second.validExitCode = true;
		}
		else
		{
			it->second.exitCode = 0;
			it->second.validExitCode = false;
		}
	}
	else
	{
 	    ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException ("Unable to register stop for task [" + task_name + "]. Task descriptor not found")); // PRQA S 3081
	}

}


// **************************
// taskProgress
// **************************
void
ProcessorTracker::taskProgress(const string &task_name, float pctg) // updates TaskList
{
    Lock lock(mutex()) ;
    _progressChanged = true;
    map<string, TaskDescr>::iterator it = _ProcessorDescrMap.find(task_name);
    if (it != _ProcessorDescrMap.end())
    {
	// 100% complete !!
	it->second.pctComplete = pctg;
    }
    else
    {
 	    ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException ("Unable to update progress for task [" + task_name + "]. Task descriptor not found")); // PRQA S 3081
    }   
}


// **************************
// updateStartTime
// **************************
void
ProcessorTracker::updateStartTime (const string& utcs_time) // updates processing start time attribute
{

	Lock lock(mutex()) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "updating start time to [" << utcs_time << "]") ;


	try
	{
		_processingStartTime = DateTime(utcs_time); // PRQA S 3081
	}
	catch (exception& ex)
	{
		ACS_LOG_ERROR("ProcessorTracker::updateStartTime : ERROR setting processingStartTime [" << utcs_time << "]") ;
		ACS_LOG_NOTIFY_EX (ex);
		_processingStartTime = DateTime("01-JAN-1970_00:00:00.000");// PRQA S 3081
	}
}



// **************************
// decodeLogLine
// **************************

void
ProcessorTracker::decodeLogLine (const string &line)
{
	Lock lock(mutex()) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "current line is [" << line << "]") ;

	size_t pos = 0 ;

	RegExp expression;

	// Processing start time
	if ((pos = line.find(_startTimeMessage)) != string::npos)
	{
		string remainder = line.substr (pos + _startTimeMessage.size()); // PRQA S 3084
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing start time detected [" << remainder << "]") ;
		updateStartTime (remainder);
	}

	// Task START
	if ((pos = line.find(_taskStartingMessage)) != string::npos)
	{
		string taskName = line.substr (pos + _taskStartingMessage.size()); // PRQA S 3084
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task starting detected [" << taskName << "]" ) ;
		try
		{
			//Task task = (*_taskTable).TaskTableElement::operator[] (taskName);
			Task task = (*_taskTable)[taskName];
			string taskVersion = task.getVersion();
			bool detached = task.isDetached();
			taskStarted (taskName, taskVersion, detached);
		}
		catch (exception& ex)
		{
			ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException (ex, "Unable to retrieve informations about task [" + taskName + "] in the task table")); // PRQA S 3081
		}
	}

	// Task PERCENTAGE
	expression.setReg (_taskPercentageRegExp);
	if (expression.match (line))
	{
		pos = line.find(_taskPercentageMessage);
		size_t posOpenBracket = line.rfind ("["); // PRQA S 4412 2
		size_t posCloseBracket = line.rfind ("]");
		size_t posTasknameStart = pos + _taskPercentageMessage.size(); // PRQA S 3084 3
		string taskName = line.substr (posTasknameStart, posOpenBracket - posTasknameStart);
		string percentage = line.substr (posOpenBracket + 1, posCloseBracket - posOpenBracket - 1);
		istringstream istr;
		istr.str(percentage);
		int pct = 0 ;
		istr >> pct;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task percentage detected [" << taskName << "] with percentage [" << pct << "]");
	
		try
		{
            taskProgress(taskName, pct); // PRQA S 3011
		}
		catch (exception& ex)
		{
			ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException (ex, "Unable to retrieve informations about task [" + taskName + "] in the task table")); // PRQA S 3081
		}
	}

    
	// Task COMPLETED
	expression.setReg (_taskCompletionRegExp);
	if (expression.match (line))
	{
		pos = line.find(_taskCompletionMessage);
		size_t posOpenBracket = line.rfind ("["); // PRQA S 4412 2
		size_t posCloseBracket = line.rfind ("]");
		size_t posTasknameStart = pos + _taskCompletionMessage.size(); // PRQA S 3084 3 
		string taskName = line.substr (posTasknameStart, posOpenBracket - posTasknameStart);
		string exitCodeStr = line.substr (posOpenBracket + 1, posCloseBracket - posOpenBracket - 1);
		istringstream istr;
		istr.str(exitCodeStr);
		int exitCode = 0;
		istr >> exitCode;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task stopping detected [" << taskName << "] with exit code [" << exitCode << "]") ;
		try
		{
            taskStopped (taskName, SUCCESS, exitCode);
		}
		catch (exception& ex)
		{
			ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException (ex, "Unable to retrieve informations about task [" + taskName + "] in the task table")); // PRQA S 3081
		}
	}

	// Task FAILURE
	expression.setReg (_taskFailureRegExp);
	if (expression.match (line))
	{
		pos = line.find(_taskFailureMessage);
		size_t posOpenBracket = line.rfind ("["); // PRQA S 4412 2 
		size_t posCloseBracket = line.rfind ("]");
		size_t posTasknameStart = pos + _taskFailureMessage.size(); // PRQA S 3084 3 
		string taskName = line.substr (posTasknameStart, posOpenBracket - posTasknameStart);
		string exitCodeStr = line.substr (posOpenBracket + 1, posCloseBracket - posOpenBracket - 1);
		istringstream istr;
		istr.str(exitCodeStr);
		int exitCode = 0;
		istr >> exitCode;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "task failure detected [" << taskName << "] with exit code [" << exitCode << "]") ;
		try
		{
            taskStopped (taskName, ERROR, exitCode);
		}
		catch (exception& ex)
		{
			ACS_LOG_TRYCATCHNOTIFY_EX (ProcessorTrackerException (ex, "Unable to retrieve informations about task [" + taskName + "] in the task table")); // PRQA S 3081
		}
	}
}


// **************************
// computeWorkOrderName
// **************************

string
ProcessorTracker::computeWorkOrderName (int orderId, const string &rootPath)
{
	ostringstream os;
	os << rootPath << "/" << orderId << "/" << "WorkOrder." << orderId << ".xml";

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "WorkOrder file name is [" << os.str() << "]") ;

	return os.str();
}

// **************************
// computeSchedulerLogName
// **************************

string
ProcessorTracker::computeSchedulerLogName (int orderId)
{
	ostringstream os;
	os << Application::defaultLogPath << "/" << "Scheduler_WorkOrder." << orderId << ".xml.log";

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Scheduler Log file name is [" << os.str() << "]") ;
	return os.str();
}



_ACS_END_NAMESPACE


