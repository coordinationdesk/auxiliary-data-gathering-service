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

	$Prod: A.C.S. ThinLayer Scheduler Library $

	$Id$

	$Author$

	$Log$
	Revision 2.10  2017/12/12 09:02:30  davide.tiriticco
	Added DefaultTaskTableFactory and DefaultExecutorVisitorFactory registration
	
	Revision 2.9  2015/06/17 13:28:05  marpas
	qa warning fixed
	
	Revision 2.8  2014/07/16 09:15:10  nicvac
	Protecting LOG and TRK streams during the creation to avoid pure virtual operation calls
	
	Revision 2.7  2013/12/16 19:31:41  marpas
	coding best practices applied
	
	Revision 2.6  2013/10/18 12:08:59  marpas
	streams have name
	
	Revision 2.5  2013/07/07 18:33:02  marpas
	application of coding best practices in progress
	some qa warnings and compilation ones removed
	adoption of libException 5.x standards in progress
	
	Revision 2.4  2013/02/26 09:00:50  marfav
	Compiler warnings removed
	
	Revision 2.3  2011/01/24 15:58:29  marpas
	Mutex.h does not longer exixts
	
	Revision 2.2  2008/04/22 11:02:05  marfav
	Aligned to new visitable task table elements
	
	Revision 2.1  2006/03/06 11:38:32  marfav
	Alignment to TaskTable having atexit structures
	
	Revision 2.0  2006/02/28 10:17:14  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/12/03 17:58:52  marfav
	Header fixed
	
	Revision 1.7  2004/11/19 14:58:21  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.6  2004/10/13 14:04:58  marfav
	Fixed a log plugins linking issue
	
	Revision 1.5  2004/09/14 16:51:50  marfav
	Using TaskPoolLauncher
	
	Revision 1.4  2004/08/24 14:54:08  marfav
	Added satellite and mission support in the WorkOrder
	Scheduler will use TaskTable settings if WorkOrder parameters are not set
	
	Revision 1.3  2004/04/09 10:11:03  marfav
	Disk quota reservation support added
	
	Revision 1.2  2004/04/07 13:11:31  marfav
	Added support for IPC
	Does not use ACE anymore
	using exStream.h
	
	Revision 1.1  2004/03/11 18:45:44  marfav
	Importing Scheduler Engine in Library
	

*/


#ifndef _SCHEDULER_ENGINE_H_
#define _SCHEDULER_ENGINE_H_


#include <acs_c++config.hpp>
#include <exStream.h>
#include <scOrder.h>
#include <SchedulerExitStatus.h>

#include <ScLogHandlerBase.h>
#include <exFilterable.h>

#include <ThreadSafe.h>

#include <string>


class tlLogFile;

_ACS_BEGIN_NAMESPACE(acs)

class TaskTableElementExecutor;
class TaskTable;
class ScLogHandlerGroup;

// This is a dummy handler class needed in order to correctly link Log Plugins 	
class SchedulerDummyLogHandler : public ScLogHandlerBase
{

public:
	SchedulerDummyLogHandler() : ScLogHandlerBase("SchedulerDummyLogHandler") {}
    virtual ~SchedulerDummyLogHandler() throw() { // PRQA S 2131
        try {
            Lock lock (globalMutex()); // PRQA S 4631 6
            destroying(); 
        }
        catch(std::exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
    }

	virtual void userFilterableNotification (exFilterable&) {} // PRQA S 2131
	virtual bool computeTaskCompletionPercentage (exFilterable&, float&) { return false; } // PRQA S 2131
private:
	SchedulerDummyLogHandler(const SchedulerDummyLogHandler &) ; // not implemented
	SchedulerDummyLogHandler& operator= (const SchedulerDummyLogHandler &) ; // not implemented

} ;

class SchedulerEngine // PRQA S 2109
{
  public:
    explicit SchedulerEngine(const scOrder& aWorkOrder);
    explicit SchedulerEngine(const std::string& aPath);
    virtual ~SchedulerEngine() throw() ;

    SchedulerExitStatus Activate();

    void suspend();
    void resume();
    void stop();
    void abort(); // PRQA S 2502
    void abortBySigTerm();
    void kill();
    void sendCmd (const std::string &, const std::string &);

    SchedulerStatus getStatus();
    bool setStatus (SchedulerStatus);
    bool isSuspended();
    bool isExiting();

#ifdef SCHEDULER_CHECKS_DISK_SPACE
    size_t getReservedDiskSpaceQuota() const;
    void setReservedDiskSpaceQuota(size_t MB);
    static const size_t NoReservedQuota = 0;
#endif // SCHEDULER_CHECKS_DISK_SPACE

  private:
	// Copy disabled
	SchedulerEngine ();
	SchedulerEngine (const SchedulerEngine&);
	SchedulerEngine& operator= (const SchedulerEngine&);


  private:
    void init();
    bool checkWorkDir(const std::string& aPath) const;
#ifdef SCHEDULER_CHECKS_DISK_SPACE
    bool checkDiskSpace(size_t requested, const std::string& work_dir) const;
#endif // SCHEDULER_CHECKS_DISK_SPACE
    void Armageddon();
    void cleanupWorkDir(const std::string&) const;
    void updateStatus (SchedulerStatus);
    void manageBackgroundTasks();

  private: // attributes
#ifdef SCHEDULER_CHECKS_DISK_SPACE
    size_t _reservedDiskSpaceQuota; // in MB; 0 == use all available
#endif // SCHEDULER_CHECKS_DISK_SPACE
    scOrder _workOrder;
    TaskTable *_taskTable;
    tlLogFile *_log;
    SchedulerStatus _internalStatus;
    ScLogHandlerGroup* _logHandler;
    TaskTableElementExecutor* _theExecutor;

    std::string _satellite;
    std::string _mission;
    std::string _procName;
    std::string _procVersion;

    ThreadSafe _internalStatusMutex;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(SchedulerEngine) ; 
};

_ACS_END_NAMESPACE


#endif // _SCHEDULER_ENGINE_H_

