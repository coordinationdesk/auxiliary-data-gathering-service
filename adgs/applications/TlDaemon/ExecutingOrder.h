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

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.4  2017/11/06 17:15:39  nicvac
	ECICMF-94: resource computation. Reservation before precondition check
	
	Revision 3.3  2017/11/02 18:25:07  nicvac
	ECICMF-94: Resource Manager parameters.
	
	Revision 3.2  2017/10/24 15:37:44  marfav
	ECICMF-94 Preintegrazione servizi - Caso SLOT based coperto
	
	Revision 3.1  2017/03/21 16:08:05  marfav
	APF-411
	Added support for scheduler default exit code interpretation (posix / Gen-Proc-ICD)
	According to what stated in the WSConfig XML file
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.18  2016/10/11 07:57:03  marfav
	S3PMP-842
	CacheManager is now a pointer
	
	Revision 2.17  2016/03/07 11:39:01  nicvac
	ALSAT1B-72: Record Input of Order
	
	Revision 2.16  2015/12/01 17:44:45  marpas
	coding best practice application in progress
	
	Revision 2.15  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.14  2015/11/19 17:23:03  marpas
	coding best practices application in progress
	
	Revision 2.13  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.12  2013/03/12 17:57:34  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.11  2011/03/14 09:56:00  marpas
	aligned to libThread Rev 2.14
	
	Revision 2.10  2009/06/26 10:31:54  marpas
	no message from some class dtors
	
	Revision 2.9  2009/03/19 11:29:11  marfav
	Retrieving regular expression from the database for mandatory output
	and passing it to the scheduler using the ScWorkOrder
	
	Revision 2.8  2008/10/14 16:00:01  marfav
	Orders attachment support added
	
	Revision 2.7  2008/08/01 15:38:28  marfav
	PreConditionChecker plugins framework added
	
	Revision 2.6  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved
	
	Revision 2.5  2006/11/10 14:20:59  marfav
	refreshClob ifdeffed
	
	Revision 2.4  2006/10/24 15:58:14  marfav
	Addedd support for passing stream-encapsulated parameters file to special queries
	
	Revision 2.3  2006/06/12 11:05:01  marfav
	Added support for StdErr and StdOut Log Levels
	Code for NO_SLOTS support removed
	
	Revision 2.2  2006/04/07 09:09:04  marfav
	Added database support for Regexp inputs and outputs
	
	Revision 2.1  2006/03/06 15:59:19  marfav
	Using Task Tables having AtExit sequence support
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.27  2005/03/18 11:22:03  marfav
	Calval environment merged to standard one
	
	Revision 1.26  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	
	Revision 1.25  2004/12/03 17:24:47  marfav
	Header Fixed
	
	Revision 1.24  2004/11/19 14:57:45  fracar
	fixed problems with disk space check and open pipes dangling
	
	Revision 1.23  2004/11/15 16:52:50  marfav
	Using ProcessorTracker instead of CryosatProcessorTracker
	
	Revision 1.22  2004/11/08 14:20:50  marfav
	WD downloaded configuration space names have version instead of orderid
	
	Revision 1.21  2004/08/25 13:48:11  marfav
	Added support for orders parameters file
	
	Revision 1.20  2004/08/24 14:44:29  marfav
	Added support for satellite and mission parameters while creating the JO
	
	Revision 1.19  2004/05/25 16:49:34  marfav
	Using new interface of CryosatProcessorTracker
	
	Revision 1.18  2004/05/24 14:37:05  marfav
	Prepared for passing the log name to the scheduler via workorder
	
	Revision 1.17  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement
	
	Revision 1.16  2004/05/05 14:22:05  marfav
	Patch for SUSPENDED order state management added (CalVal environement only)
	
	Revision 1.15  2004/05/04 16:20:05  marfav
	Prepared for order slot number management
	
	Revision 1.14  2004/04/07 13:19:56  marfav
	Added support for IPC from ThinLayer Clients and to running Schedulers
	Does not use ACE anymore
	Using exStream.h
	
	Revision 1.13  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.12  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations
	
	Revision 1.11  2003/07/10 14:18:32  marfav
	clone method added to exceptions
	
	Revision 1.10  2003/06/06 10:31:04  marfav
	Dead code removed
	
	Revision 1.8  2003/04/30 16:20:16  marfav
	Align to ICD v2.0 and some logging improvement
	
	Revision 1.7  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h
	
	Revision 1.6  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support
	
	Revision 1.5  2003/03/11 18:25:50  marfav
	Patrol message handling added
	
	Revision 1.4  2003/02/18 09:09:22  fracar
	improved logging
	
	Revision 1.3  2003/02/04 14:08:13  marfav
	Using ConfigurationSingleton instead of parent config
	
	Revision 1.2  2003/02/04 09:58:34  marfav
	Added LOG field to headers
	


*/


#ifndef _EXECUTING_ORDER_H
#define _EXECUTING_ORDER_H

#include <InputResourceMgr.h>

#include <Thread.h>
#include <exException.h>

#include <SpawnedScheduler.h>
#include <SpawnProc.h>
#include <JobOrder.h>
#include <TaskTable.h>
#include <tlOrder.h>
#include <scOrder.h>
#include <TlMessage.h>
#include <CacheManager.h>

#include <OrderReservedResources.h>

#include <ProcessorTracker.h>

#include <PCC_Base.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class OrderExecutionMgr;

class ExecutingOrder : public Thread // PRQA S 2109
{
public:
	/*! class ExecutingOrderException declaration */
	exDECLARE_EXCEPTION(ExecutingOrderException, exException) ; // PRQA S 2131, 2502

    enum stopType { NO_STOP, CLEAN_STOP, IMMEDIATE_STOP };

    ExecutingOrder(unsigned int ID, OrderExecutionMgr& aManager, const tlOrder& anOrder, const std::string& aTaskTable, const std::string& BaseDir, 
                   const std::string& aLogLevel, const std::string& aErrLogLevel, const std::string& aOutLogLevel, CacheManager* cm, bool posix);
    virtual ~ExecutingOrder() throw() ;

    static StringKey getTaskTableKeyFromOrder( const tlOrder& anOrder );

    bool check(bool verboseFlag=false);
    bool download();
    void exec();
    void haltOrder(stopType stopMode=IMMEDIATE_STOP);
    void stopAndJoin();
    void cleanup();

    virtual void run(); // inherited from Task

    bool isValid() const;
    void isValid(bool b);

    operator tlOrder() const; // PRQA S 2181
    void manageCommand (TlMessage&);
    unsigned int getAssignedSlot() const;

    //Resource Manager
	void setResManager(const OrderReservedResourcesSptr& orderReservedResourcesSptr);

  private: // methods

    void createWorkingDir();
    scOrder buildWorkOrder();
    void buildJobOrder();

    bool checkDiskSpace(size_t requested, size_t reserved, const std::string& work_dir) const;

    void downloadConfigurationSpaces();
    void downloadParametersFile();

    std::string genLogFileName() const;
    std::string genJobOrderFileName() const;
    std::string genWorkOrderFileName() const;
    std::string genParametersFileName() const;

    std::string genConfigurationFileName(const std::string& aNameSpace, const std::string& aVersion ) const;

    std::string genInterimFullFileName(const std::string& file_name) const;
    SpawnedScheduler *getScheduler();
    ProcessorTracker *getTracker();
    double getDeltaT() const;
    void stopDownload();
    std::string genPrivateConfFileName() const;
    std::string genSchedulerLogName() const;
    std::string genTrackerFileName() const;

    std::string getFileTypeRegExp (const std::string&);

    void enterSuspendedStatus();
    void exitSuspendedStatus();
    
    void downloadAttachments();

    void recordOrderInput() const;

  private: 
    ExecutingOrder& operator = (const ExecutingOrder&); // not implemented
    ExecutingOrder (const ExecutingOrder&); // not implemented
    ExecutingOrder (); // not implemented


  private: // attributes
    unsigned int _mySlot;
    OrderExecutionMgr& _parent;
    tlOrder _order;
    int _maxTime;
    std::string _productionDir;
    std::string _baseDir;
    std::string _taskTableName;
    std::string _satelliteName;
    std::string _missionNumber;
    std::string _parametersFileName;
    TaskTable * _taskTable;
    SpawnedScheduler *_scheduler;
    joLogLevel _logLevel;
    joLogLevel _errLogLevel;
    joLogLevel _outLogLevel;
    bool _valid;
    bool _copyPConf;
    InputResourceMgr *_downloader;
    ProcessorTracker *_tracker;

    std::vector <ConfigSpaceDescriptor> _configSpaces;
    std::map <std::string, std::string> _fileTypesRegExpMap;
	std::map <std::string, std::string> _mandatoryOutputRegExpMap;
    CacheManager* _cacheManager;

    PCC_Base::SymTable _theSymTable;
    TaskTable _visitedTaskTable;
    
    std::vector <JobOrderAttachmentDescriptor> _orderAttachments;

	// APF-411
	bool _posixExitCode;

	// ECICMF-94: Resource Manager
	OrderReservedResourcesSptr _orderReservedResourcesSptr;

    ACS_CLASS_DECLARE_DEBUG_LEVEL (ExecutingOrder)
};

_ACS_END_NAMESPACE

#endif // _EXECUTING_ORDER_H
