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

	$Prod: A.C.S. ThinLayer TaskTable Execution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2016/04/20 07:46:08  marfav
	APF-371
	Added management and notification of the signals killing the tasks
	
	Revision 2.4  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.3  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.2  2013/07/25 16:18:28  marfav
	Using synchronous waitin loop monitoring udergoing processes
	
	Revision 2.1  2008/04/22 11:13:22  marfav
	Executors implemented using the conditioned visitor patter
	
	Revision 2.0  2006/02/28 10:09:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/07/26 13:28:35  fracar
	modified to support libScOrder rel 1.6
	
	Revision 1.2  2004/12/03 18:02:10  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 17:02:38  marfav
	Import libTaskExecution
	
	

*/


#ifndef _tlTaskLauncher_H_
#define _tlTaskLauncher_H_

#include <exException.h>

#include <tlLogFile.h>
#include <scOrder.h>
#include <SpawnProc.h>
#include <ThreadedProcWrapper.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class tlTaskLauncher // PRQA S 2109
{
  public:


	/*! class TaskPoolNotDetachedException declaration */
	exDECLARE_EXCEPTION(TaskLauncherException,acs::exException) ; // PRQA S 2131, 2502

    tlTaskLauncher(const std::string& aTask, 
                   int aCriticalityLevel, 
                   const acs::scOrder& aWorkOrder, 
                   const std::string& aName, 
                   int quitSignal, 
                   int killSignal, 
                   int outd=-1, 
                   int errd=-1, 
                   bool isDetached=false);

    virtual ~tlTaskLauncher() throw() ;

    virtual bool run();
    bool isStarted() const throw() { return _proc != 0; }
    bool isRunning() const { return isStarted() && _proc->isRunning(); }
    bool isFailed() const;
    bool exitCode (int& code) const { return isStarted() && _proc->exitCode(code); }

	// APF-371 adding exit signal management
	bool exitSignal (int& signal) const { return isStarted() && _proc->exitSignal(signal); }

    void kill (int sig = 9) const;
    void killGroup (int sig = 9) const;
    void killLevel (int level, int sig=9) const;
    const std::string &Name() const throw() { return _name; }
    pid_t pid() const;

    int getKillSignal() const throw() {	return _killSignal; }
    int getQuitSignal() const throw() { return _quitSignal; }



	// uses the spawnproc threaded wrapper to synch with the process completion
	bool waitExitOrTimeout (unsigned long msecs) const;


private:
	// Shall not be used
    tlTaskLauncher();
    tlTaskLauncher(const tlTaskLauncher&);
    tlTaskLauncher& operator = (const tlTaskLauncher&);

	// Internal stuff
	// Generate the name of the ENV variable used to dispatch the task name to the spawned task
	std::string getEnvForTaskName();


private: // attributes
    enum TaskExitStatus { tsOK=0, tsINCOMPLETE=1, tsFAILURE=2 }; // map exit codes according to GEN_PROC icd

    std::string _task;
    int _criticalityLevel;
    acs::scOrder _order;
    std::string _name;
    acs::SpawnProc *_proc;
    int _outd ;
    int _errd ;
    bool _isDetached;
    std::string _taskStartingMessage;
    int _quitSignal ;
    int _killSignal ;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(tlTaskLauncher)
};



_ACS_END_NAMESPACE


#endif // _tlTaskLauncher_H_

