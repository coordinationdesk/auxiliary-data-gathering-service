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
	Revision 2.6  2014/04/14 14:36:06  marpas
	adopting DateTime 5.13 interface
	qa and g++ warnings fixed
	tests are wrong ...
	
	Revision 2.5  2011/01/24 15:54:09  marpas
	Mutex.h does not longer exixts
	
	Revision 2.4  2010/08/03 14:32:58  nicvac
	Task Table plugins management
	
	Revision 2.3  2009/06/26 10:47:27  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.2  2008/04/22 10:57:38  marfav
	Aligned to new visitable task table elements
	
	Revision 2.1  2006/03/06 11:33:22  marfav
	Alignment to TaskTable having atexit structures
	
	Revision 2.0  2006/02/28 10:18:10  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/12/03 17:50:50  marfav
	Header fixed
	
	Revision 1.6  2004/11/15 16:50:50  marfav
	CryosatProcessorTracker included in ProcessorTracker
	CryosatProcessorTracker prepared for deletion
	
	Revision 1.5  2004/09/29 12:48:31  marfav
	Added support for processing completion time estimation
	
	Revision 1.4  2004/09/01 09:17:56  marfav
	Added support for exit code of tracked processes
	
	Revision 1.3  2004/05/28 15:08:18  marfav
	Support for processing start time added
	
	Revision 1.2  2004/05/25 16:26:05  marfav
	Added a ctor
	now CryosatProcessorTracker can track a scheduler log
	using orderId and processingRootPath
	
	Revision 1.1.1.1  2004/04/15 12:19:57  marfav
	Import libProcessorTracker
	
	

*/


#ifndef _ProcessorTracker_H
#define _ProcessorTracker_H

#include <scOrder.h>

#include <exException.h>
#include <Thread.h>
#include <DateTime.h>

#include <vector>
#include <string>



_ACS_BEGIN_NAMESPACE(acs)

class TaskTable;

class LineSplitter;

class ProcessorTracker : public Thread // PRQA S 2109
{

  public:
	/*! class ProcessorTrackerException declaration */
	exDECLARE_EXCEPTION(ProcessorTrackerException,exException) ; // PRQA S 2131, 2502

    enum Status
    {
      RUNNING,
      SUCCESS,
      ERROR
    };

    struct TaskDescr // PRQA S 2173, 2175
    {
      TaskDescr() : 
        name(), 
        version(), 
        pctComplete(), 
        taskWeight(), 
        background(), 
        completionStatus(), 
        validExitCode(), 
        exitCode() 
      {}
      TaskDescr(const TaskDescr &r) : 
        name(r.name), 
        version(r.version), 
        pctComplete(r.pctComplete), 
        taskWeight(r.taskWeight), 
        background(r.background), 
        completionStatus(r.completionStatus), 
        validExitCode(r.validExitCode), 
        exitCode(r.exitCode) 
      {}
      TaskDescr& operator=(const TaskDescr &r) 
      {
        if (&r != this) {
          name = r.name ;
          version = r.version ;
          pctComplete = r.pctComplete ;
          taskWeight = r.taskWeight ;
          background = r.background ;
          completionStatus = r.completionStatus ;
          validExitCode = r.validExitCode ;
          exitCode = r.exitCode ;
        }
        return *this ;
      }
      
      ~TaskDescr() throw() {}
      
      std::string name; // PRQA S 2100 8
      std::string version;
      float pctComplete; // 0..100
      float taskWeight; // 0..100
      bool background;
      Status completionStatus;
      bool validExitCode;
      int exitCode;
    };

    typedef std::vector<TaskDescr> TaskList;


  public:
    // The CTOR instantiates a new TaskTable object...
    // it can throw exceptions !!
    // Use try-catch when instantiating new ProcessorTracker object

	explicit ProcessorTracker(const scOrder& wo);
    ProcessorTracker(int orderId, const std::string &rootPath);

    virtual ~ProcessorTracker() throw() ;

    bool processorStarted() const ; // false until a task start has been detected
    float getTotalPercentComplete(); // const;
    bool getProcessingStartTime(DateTime &) const;
    void getTaskDescriptors (TaskList &) const;
    void getTaskDescriptors (std::vector<TaskList> &) const;
    
    // Returns the time left estimation in seconds
    // flags are set using a bitmask
    // 0 -> real computation
    double getTimeLeftEstimation (int& flags);

  protected: //methods
    virtual void run(); // inherited from Thread
    virtual void decodeLogLine(const std::string &);
    virtual void init();
    virtual std::string computeWorkOrderName (int orderId, const std::string &rootPath);
    virtual std::string computeSchedulerLogName (int orderId);

#ifdef ACS_PROCESSORTRACKER_TEST
  public:
#else
  protected:
#endif

    // protected CTOR
    ProcessorTracker();

    void readTaskTable (const scOrder& wo);
    void taskStarted(const std::string &task_name, const std::string &task_version, bool background); // creates new TaskList entry
    void taskStopped(const std::string &task_name, Status completionStatus, int exitCode); // updates TaskList
    void taskProgress(const std::string &task_name, float pctg); // updates TaskList
    void updateStartTime (const std::string& utcs_time); // updates processing start time attribute
    void doTimeComputation();
    void timeComputation();
    void updateCompletionPercentage();

  private: // copy disabled (it is a thread!)
           // methods declared but not implemented
    ProcessorTracker (const ProcessorTracker&);
    ProcessorTracker& operator= (const ProcessorTracker&);

  private: //methods

    void followLogFile();
    void _init();


  private:
    ACS_SMARTPTR<TaskTable> _taskTable;
    std::string _logFile;
    ACS_SMARTPTR<LineSplitter> _lineSplitter;
    int _fd; // file descriptor
    std::map <std::string, TaskDescr> _ProcessorDescrMap;
    DateTime _processingStartTime;
    DateTime _lastComputedEstimationTime;
    DateTime _expectedStopTime;
    float _estimationPercentage;
    double _estimatedOnePercentTime;
    bool _progressChanged;
    float _currentCompletionPercentage;
	
  private:
    // Log messages
    std::string _taskStartingMessage;
    std::string _taskCompletionMessage;
    std::string _taskFailureMessage;
    std::string _startTimeMessage;
    std::string _taskPercentageMessage;
	
    // Decoding regular expressions
    std::string _taskStartingRegExp;
    std::string _taskCompletionRegExp;
    std::string _taskFailureRegExp;
    std::string _startTimeRegExp;
    std::string _taskPercentageRegExp;
	

  private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(ProcessorTracker)
};



_ACS_END_NAMESPACE

#endif // _ProcessorTracker_H
