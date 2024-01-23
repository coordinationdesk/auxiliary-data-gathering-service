/*

    Copyright 1995-2021, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

	$Prod: A.C.S. ThinLayer Daemon $

	$Id$

	$Author$

	$Log$
	Revision 3.12  2017/11/06 17:15:39  nicvac
	ECICMF-94: resource computation. Reservation before precondition check
	
	Revision 3.11  2017/11/02 18:25:07  nicvac
	ECICMF-94: Resource Manager parameters.

	Revision 3.10  2017/10/24 15:37:44  marfav
	ECICMF-94 Preintegrazione servizi - Caso SLOT based coperto

	Revision 3.9  2017/10/11 16:13:37  marfav
	Added polymorphism support for input/output/breakpoint
	descriptors when creating the job order
	Task is still of base type

	Revision 3.8  2017/10/11 07:29:58  marfav
	make_pair avoided due to c++11 compiler issues

	Revision 3.7  2017/10/10 15:22:11  marfav
	Removed useless dependency from DefaultJobOrder

	Revision 3.6  2017/09/14 08:25:42  marfav
	Const interator returned by TaskTableSmartVector container

	Revision 3.5  2017/09/12 08:45:18  marfav
	RAII CTOR

	Revision 3.4  2017/09/12 08:38:37  marfav
	Unused variable removed

	Revision 3.3  2017/09/12 08:18:45  marfav
	Added support to polymorphic objects when dealing with input descriptors and alternatives

	Revision 3.2  2017/03/21 16:08:05  marfav
	APF-411
	Added support for scheduler default exit code interpretation (posix / Gen-Proc-ICD)
	According to what stated in the WSConfig XML file

	Revision 3.1  2016/11/04 15:23:17  marfav
	APF-271
	Adding slot id to splunk messages

	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications

	Revision 2.51  2016/10/11 07:57:03  marfav
	S3PMP-842
	CacheManager is now a pointer

	Revision 2.50  2016/10/07 13:20:25  marpas
	OrdersAttachmentType is a NameIdAssociator from libDbClient now

	Revision 2.49  2016/06/13 15:17:05  marpas
	log message could rise an exception in case of null sat id

	Revision 2.48  2016/05/11 11:53:57  danalt
	use of ACS_THROW and ACS_COND_THROW

	Revision 2.47  2016/04/18 15:25:00  marpas
	some NameIdAssociators adhere to the standards now - they need a db key

	Revision 2.46  2016/04/11 13:45:45  marpas
	adopting new db interfaces

	Revision 2.45  2016/03/29 17:16:35  marpas
	adopting new db I/F

	Revision 2.44  2016/03/08 10:41:49  nicvac
	ALSAT1B-72: Record Inputs for Order

	Revision 2.43  2016/03/07 11:39:01  nicvac
	ALSAT1B-72: Record Input of Order

	Revision 2.42  2016/01/27 14:33:13  marfav
	APF-271
	jobid is a string field and not a number

	Revision 2.41  2016/01/18 14:15:23  marfav
	ALSAT1B-62 adding the atexit sequence parameters in the sc work order

	Revision 2.40  2015/12/23 10:40:12  marfav
	Splunk messages updated

	Revision 2.39  2015/12/01 15:49:53  marfav
	Fixing metric types

	Revision 2.38  2015/11/26 15:14:25  marfav
	mc_params are now lowercase

	Revision 2.37  2015/11/19 17:23:03  marpas
	coding best practices application in progress

	Revision 2.36  2015/08/12 14:21:43  marfav
	APF-271 work in progress

	Revision 2.35  2015/07/17 14:25:28  marfav
	Reverting changes made in APF-285

	Revision 2.33  2015/05/14 08:20:49  damdec
	Compilation warning fixed.

	Revision 2.32  2015/04/30 15:49:17  marfav
	Compiler warning removed

	Revision 2.31  2013/04/29 16:32:44  marpas
	file type recognition class interface changed!

	Revision 2.30  2013/04/09 09:00:09  marpas
	NameIdAssociator refactoring

	Revision 2.29  2013/03/14 15:05:38  marpas
	connection pool concept is left to class inheriting from Application

	Revision 2.28  2013/03/12 17:57:34  marpas
	dbConnectionPool is mandatory in file actions

	Revision 2.27  2013/03/11 12:21:49  marfav
	Using ConnectionPool

	Revision 2.26  2013/03/11 12:09:03  marfav
	dbConnectionPool use enforced

	Revision 2.25  2013/02/26 16:36:23  marfav
	Some compiler warning removed

	Revision 2.24  2012/02/20 15:45:29  marpas
	removing deprecated macro

	Revision 2.23  2012/02/09 17:26:24  marpas
	refactoring in progress

	Revision 2.22  2011/05/10 12:35:24  marfav
	Moved setTaskTable to the last statement of the joborder configuration

	Revision 2.21  2011/03/14 09:56:00  marpas
	aligned to libThread Rev 2.14

	Revision 2.20  2010/08/03 14:39:34  nicvac
	Task Table plugins management

	Revision 2.19  2009/09/28 17:00:30  marpas
	dbParam interface obsolecence

	Revision 2.18  2009/09/10 14:44:16  marfav
	Using different entries in the SymTable for the file to download and
	the file to be used in the time intervals

	Revision 2.17  2009/06/26 10:31:53  marpas
	no message from some class dtors

	Revision 2.16  2009/06/25 08:56:45  marfav
	Removed default values for satellite

	Revision 2.15  2009/03/19 11:29:11  marfav
	Retrieving regular expression from the database for mandatory output
	and passing it to the scheduler using the ScWorkOrder

	Revision 2.14  2009/03/10 16:32:37  marpas
 *** empty log message ***

	Revision 2.13  2008/11/05 14:38:40  marfav
	Addedd support for ExternalOrderId

	Revision 2.12  2008/10/22 12:20:43  marfav
	Added task table 2 scheduler cmd line parameter routing logic
	with support for special parameters value token

	Revision 2.11  2008/10/14 16:00:01  marfav
	Orders attachment support added

	Revision 2.10  2008/08/01 15:38:28  marfav
	PreConditionChecker plugins framework added

	Revision 2.9  2008/04/24 15:37:33  marfav
	Addedd support for JobOrder StdOut and StdErr log levels

	Revision 2.8  2008/04/22 13:35:27  marfav
	Aligned to the task table conditioned visitor design pattern
	Exception management improved

	Revision 2.7  2006/11/10 13:48:27  fracar
	handling parameters file loading from the order table clob

	Revision 2.6  2006/10/24 15:58:14  marfav
	Addedd support for passing stream-encapsulated parameters file to special queries

	Revision 2.5  2006/07/07 10:44:37  marfav
	Fixed error in output regexp filename generation

	Revision 2.4  2006/06/12 11:05:01  marfav
	Added support for StdErr and StdOut Log Levels
	Code for NO_SLOTS support removed

	Revision 2.3  2006/04/07 09:09:04  marfav
	Added database support for Regexp inputs and outputs

	Revision 2.2  2006/03/27 18:06:57  fracar
	added management of REGEXP file name type (To Be Completed)

	Revision 2.1  2006/03/06 15:59:19  marfav
	Using Task Tables having AtExit sequence support

	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted

	Revision 1.68  2006/01/19 18:05:58  marfav
	Added support for mapping alternatives instead of filetypes when downloading inputs

	Revision 1.67  2005/09/01 13:14:02  marfav
	Emitting critical events in case of processing failures or disk full

	Revision 1.66  2005/07/19 15:20:50  fracar
	improved communication with MCF upon order processing failure

	Revision 1.65  2005/06/28 16:16:52  fracar
	fixed rounding problem with getTracker()->getTotalPercentComplete()

	Revision 1.64  2005/06/28 08:33:45  marfav
	Removed OrderType translation to enumerative types
	Only plain text strings are handled

	Revision 1.63  2005/05/31 16:21:26  marpas
	file action interface changed

	Revision 1.62  2005/05/17 09:26:16  marfav
	Code indentation modified

	Revision 1.61  2005/03/31 18:13:22  fracar
	messages to mcf improved

	Revision 1.60  2005/03/18 11:22:03  marfav
	Calval environment merged to standard one

	Revision 1.59  2005/03/18 10:22:16  marfav
	Naming convention for parameters file changed. No extensions.

	Revision 1.58  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files

	Revision 1.57  2005/02/11 18:02:15  marfav
	InputDownloader construction fixed. No pushed resources now

	Revision 1.56  2005/01/26 18:56:28  ivafam
	Control added

	Revision 1.55  2005/01/26 17:39:22  ivafam
	dbDownloadConfigurationSpace I/F changed

	Revision 1.54  2004/12/23 18:34:26  marfav
	Multislot Environement now active
	WSConfig dump changed using WSConfigMessage class

	Revision 1.53  2004/12/03 17:24:47  marfav
	Header Fixed

	Revision 1.52  2004/11/19 14:57:45  fracar
	fixed problems with disk space check and open pipes dangling

	Revision 1.51  2004/11/15 16:52:50  marfav
	Using ProcessorTracker instead of CryosatProcessorTracker

	Revision 1.50  2004/11/08 14:20:50  marfav
	WD downloaded configuration space names have version instead of orderid

	Revision 1.49  2004/10/22 19:57:05  fracar
	Added time interval extensions for Interim inputs

	Revision 1.48  2004/10/19 14:19:36  marfav
	Old code removed

	Revision 1.47  2004/10/19 08:17:07  marfav
	Support for orders satellite and mission fields added

	Revision 1.46  2004/09/06 09:22:32  danalt
	hasParameters

	Revision 1.45  2004/08/26 15:18:17  marfav
	Using dbOrder.haveParameterFile method

	Revision 1.44  2004/08/25 13:48:11  marfav
	Added support for orders parameters file

	Revision 1.43  2004/08/24 14:44:29  marfav
	Added support for satellite and mission parameters while creating the JO

	Revision 1.42  2004/05/26 09:32:54  marfav
	Debug messages improved

	Revision 1.41  2004/05/25 16:49:34  marfav
	Using new interface of CryosatProcessorTracker

	Revision 1.40  2004/05/24 14:37:05  marfav
	Prepared for passing the log name to the scheduler via workorder

	Revision 1.39  2004/05/17 08:19:55  marfav
	Order slots support completed

	Revision 1.38  2004/05/13 08:49:33  marfav
	Percentage messages decimation fixed

	Revision 1.37  2004/05/12 15:25:25  marfav
	Added multislot support for CalVal Patch

	Revision 1.36  2004/05/12 08:19:53  marfav
	Preparing for multislot processing status environement

	Revision 1.35  2004/05/05 14:22:05  marfav
	Patch for SUSPENDED order state management added (CalVal environement only)

	Revision 1.34  2004/05/04 16:20:05  marfav
	Prepared for order slot number management

	Revision 1.33  2004/04/26 12:24:42  integrator
	Scheduler exit codes updated

	Revision 1.32  2004/04/08 14:07:05  marfav
	Fixed unknown exit status handling on spawned scheduler

	Revision 1.31  2004/04/07 13:19:56  marfav
	Added support for IPC from ThinLayer Clients and to running Schedulers
	Does not use ACE anymore
	Using exStream.h

	Revision 1.30  2004/03/22 17:39:53  marfav
	Exception handled when trying to retrieve private config pathname from
	TaskTables

	Revision 1.29  2004/03/11 19:10:29  marfav
	Preparing to manage external commands
	Align to new version of Scheduler Engine (libScheduler)

	Revision 1.28  2003/10/03 11:36:33  marfav
	Processor name read from the TaskTable instead of the TlOrder

	Revision 1.27  2003/09/25 09:39:16  marfav
	Using references from ConfigurationSingleton

	Revision 1.26  2003/09/23 14:02:42  fracar
	handling verbose execution mode in support to Thin Layer MMI interactive operations

	Revision 1.25  2003/08/29 12:00:46  fracar
	fixed policy for cleaning up existing working directories
	reading file extensions from config file for downloaded inputs
	issuing a critical exception message to Patrol if cannot update order queue

	Revision 1.24  2003/08/08 10:49:37  marfav
	Added support for cleaning WorkingDir on errors

	Revision 1.23  2003/07/29 10:45:00  marfav
	Reload configuration support at run-time

	Revision 1.22  2003/07/18 14:50:45  marfav
	tlDaemon configuration space fixed

	Revision 1.21  2003/07/17 13:44:06  marfav
	Log messages using exFWarning and exFError

	Revision 1.20  2003/07/10 14:18:32  marfav
	clone method added to exceptions

	Revision 1.19  2003/06/06 10:31:04  marfav
	Dead code removed

	Revision 1.17  2003/05/30 14:44:48  marfav
	Added TLD_Clean_Working_Dir key in configuration space

	Revision 1.16  2003/05/13 08:45:19  marfav
	Fixed memory leak at cleanup

	Revision 1.15  2003/05/07 15:06:27  marfav
	Improved memory allocation management and exception handling

	Revision 1.14  2003/04/30 16:20:16  marfav
	Align to ICD v2.0 and some logging improvement

	Revision 1.13  2003/04/18 16:26:06  fracar
	align to new compiler version

	Revision 1.12  2003/04/18 12:59:33  marfav
	Using namespaces removed from .h

	Revision 1.11  2003/03/26 09:56:11  marfav
	Robustness improved

	Revision 1.10  2003/03/24 16:35:26  marfav
	Included JobOrderFactory support

	Revision 1.9  2003/03/11 18:25:50  marfav
	Patrol message handling added

	Revision 1.8  2003/02/18 09:09:22  fracar
	improved logging

	Revision 1.7  2003/02/10 18:50:19  fracar
	prepared for multi-interval management

	Revision 1.6  2003/02/10 09:17:48  fracar
	improved logging

	Revision 1.5  2003/02/07 08:30:48  fracar
	handling downloaded file split and improved messaging

	Revision 1.4  2003/02/04 14:08:13  marfav
	Using ConfigurationSingleton instead of parent config

	Revision 1.3  2003/02/04 09:58:34  marfav
	Added LOG field to headers



 */


#include <ExecutingOrder.h>

#include <TaskTableFactoryChain.h>

#include <JobOrderFactoryChain.h>
#include <OrderExecutionMgr.h>
#include <StringKey.h>
#include <TaskTable.h>
#include <TlStatusMgrSingleton.h>
#include <tlOrder.h>
#include <OrdersAttachmentType.h>
#include <PreconditionCheckerFactoryChain.h>

#include <Application.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <Timer.h>
#include <SatelliteName.h>
#include <dbOrdersInput.h>
#include <dbConnection.h>
#include <dbConfigurationSpace.h>
#include <dbConv.h>
#include <dbFileAction.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbFileTypeCheck.h>
#include <dbGenQueryScopeGuard.h>
#include <dbOrdersAttachment.h>
#include <dbOrdersAttachmentQuery.h>
#include <dbPersistentFactoryChain.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbRow.h>
#include <dbSatellite.h>
#include <dbSatelliteQuery.h>
#include <Filterables.h>
#include <MCFilterables.h>
#include <exStream.h>
#include <rsPushNSpace.h>

#include <Functional.h>
#include <Application.h>
#include <dbConnectionPool.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <string>

#include <math.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/vfs.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

using namespace acs::mc;


// Can define some variable to change default behaviour
// SET_SCHEDULER_LOG_NAME -> force the scheduler log name to the one computed by TLDaemon


ACS_CLASS_DEFINE_DEBUG_LEVEL(ExecutingOrder)

static const char *_tldRootKey = "TlDaemon";
static const char *_schedulerPathKey = "TLD_Scheduler_Path";
static const char *_cleanDirKey = "TLD_Clean_Working_Dir";
static const char *_wipeWDKey = "TLD_Wipe_Existing_WD";
static const char *_recordOrderInputKey = "TLD_Record_Order_Input";
static const char *_recordOrderInputIsCriticalKey = "TLD_Record_Order_Input_IsCritical";
static const char *_copyPConfKey = "TLD_Copy_PConf";

static const char *_procStationKey = "Station.CentreID";
static const char *_procStationNameKey = "Station.Name";

static const char *_schedRootKey = "Scheduler";
static const char *_reservedQuotaKey = "ReservedDiskSpaceQuota";


// constants used to create Keys or messages
static const char *_percentageBaseString = "PROCESSING_PERCENTAGE";
static const char *_errorEventRegister = "ProcessingError";
static const char *_diskFullEventRegister = "DiskFull";



// Helper class
// Will guard the value of "_valid" flag during the run of the thread

class IsValidGuard
{
public:
	IsValidGuard (bool& flag) : _flag(flag)
{
		_flag = true;
}

	~IsValidGuard ()
	{
		_flag = false;
	}

private:
	bool& _flag;
};


ExecutingOrder::ExecutingOrder(unsigned int ID, OrderExecutionMgr& aManager, const tlOrder& anOrder,
		const string& aTaskTable, const string& aDir, const string& aLogLevel,
		const string& aErrLogLevel, const string& aOutLogLevel, CacheManager* cm, bool posix)
: _mySlot(ID),
  _parent(aManager),
  _order(anOrder),
  _maxTime(0),
  _productionDir(aDir),
  _baseDir(),
  _taskTableName(aTaskTable),
  _satelliteName(""),
  _missionNumber(""),
  _parametersFileName(""),
  _taskTable(NULL),
  _scheduler(NULL),
  _logLevel(),
  _errLogLevel(),
  _outLogLevel(),
  _valid(true),
  _copyPConf(true),
  _downloader(NULL),
  _tracker(NULL),
  _configSpaces(),
  _fileTypesRegExpMap(),
  _mandatoryOutputRegExpMap(),
  _cacheManager (cm),
  _theSymTable(),
  _visitedTaskTable(),
  _orderAttachments(),
  _posixExitCode(posix),
  _orderReservedResourcesSptr()

{

	//Build key and get task table
	StringKey key = ExecutingOrder::getTaskTableKeyFromOrder( anOrder );

	// Safe the pointer in an auto-ptr until the end of the CTOR
	ACS_SMARTPTR<TaskTable> ttAutoPtr ( TaskTableFactoryChain::instance()->newObject( key ) );

	ttAutoPtr->readFromFile(aTaskTable);
	_maxTime = ttAutoPtr->getMaxAllottedTime();

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "CTOR called .... [" << _order.getOrderId() << "]");
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Order [" << anOrder.getOrderId() << "]: initialising max allotted time to " << _maxTime << " seconds");

	// derive the base dir
	ostringstream os;
	os << aDir << '/' << _order.getOrderId();
	_baseDir = os.str();

	// derive the log level
	_logLevel    = string2LogLevel(aLogLevel,    LogLevelINFO); // default to INFO if wrong string
	_errLogLevel = string2LogLevel(aErrLogLevel, LogLevelDEFAULT); // default to DEFAULT if wrong string
	_outLogLevel = string2LogLevel(aOutLogLevel, LogLevelDEFAULT); // default to DEFAULT if wrong string

	if (anOrder.getTroubleshooting())
	{
		_logLevel    = LogLevelDEBUG; // bypass settings for troubleshooting orders
		_errLogLevel = LogLevelDEBUG; // bypass settings for troubleshooting orders
		_outLogLevel = LogLevelDEBUG; // bypass settings for troubleshooting orders
	}

	// CTOR completed, can release the ACS_SMARTPTR in the class attribute (RAII)
	_taskTable = ttAutoPtr.release();
}


ExecutingOrder::~ExecutingOrder() throw() 
{
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "DTOR called .... [" << _order.getOrderId() << "]");

	//Note: Order resources will be automatically released at _orderReservedResourcesSptr destruction

	if (_taskTable) delete _taskTable;
	if (_scheduler) delete _scheduler;

	if (_downloader)
	{
		delete _downloader;
	}

	if (_tracker)
	{
		try {
			_tracker->stop();
		}
		catch(exception &x) {
			excerr.notify(x) ;
		}
		delete _tracker;
	}

}

StringKey ExecutingOrder::getTaskTableKeyFromOrder( const tlOrder& anOrder ) {

	//Satellite Name
	string satelliteName(""); bool isSatelliteSet(false);
	int satId = anOrder.getSatelliteId(&isSatelliteSet);
	try
	{
		if ( isSatelliteSet )
			satelliteName = SatelliteName::instance(Application::instance()->getDefaultConnectionPool()->getKey())->name( satId );
	}
	catch (exception& exc)
	{
		ACS_LOG_WARNING ("Unable to retrieve the satelliteName from the Id in the database. Defaulting to empty string. Got exception: " << exc.what());
	}

	//Mission Number
	ostringstream missionNumber(""); bool isMissionSet(false);
	int misNum = anOrder.getMission(&isMissionSet);
	if ( isMissionSet )
	{
		missionNumber<<misNum;
	}

	//Build Task Table Key
	StringKey key = StringKeyUtils::makeKey( satelliteName, missionNumber.str(), anOrder.getProcessorName(), anOrder.getProcessorVersion() );

	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Executing order [" << anOrder.getOrderId() << "]: task table KEY is " << key );

	return key;
}


void
ExecutingOrder::setResManager(const OrderReservedResourcesSptr& orderReservedResourcesSptr )
{
	_orderReservedResourcesSptr = orderReservedResourcesSptr;
}


unsigned int
ExecutingOrder::getAssignedSlot() const
{
	return _mySlot;
}

ExecutingOrder::operator tlOrder() const
		{
	return _order;
		}

bool
ExecutingOrder::isValid() const
{
	return _valid;
}

void
ExecutingOrder::isValid(bool b)
{
	_valid = b;
}

void
ExecutingOrder::createWorkingDir()
{
	// check if working dir already exists...
	if (File::exists(_baseDir))
	{
		// check if this is an error or not; default is error
		bool canWipe = false;

		// read the settings in the daemon configuration space
		rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
		rsPushNSpace nspace_tld (rs, _tldRootKey);

		if (rs.hasKey(_wipeWDKey) )
		{
			rs.getValue(_wipeWDKey, canWipe);

			ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Value of " << _wipeWDKey << " is " << boolalpha << canWipe);
		}

		if (canWipe)
		{
			string s("rm -rf ");
			s.append(_baseDir);

			ACS_LOG_INFO("Cleaning up working dir " << _baseDir);

			system(s.c_str());
		}
		else
		{
			// error!
			throw (ExecutingOrderException("Working dir " + _baseDir + "already exists: not allowed to overwrite it!", errno));
		}
	}

	if (mkdir(_baseDir.c_str(), 0775) != 0)
	{
		throw (ExecutingOrderException("Cannot create working dir " + _baseDir, errno));
	}
}



bool
ExecutingOrder::checkDiskSpace(size_t requested, size_t reserved, const string& work_dir) const
{
	struct statfs buf;

	if (statfs(work_dir.c_str(), &buf) == -1) // error
	{
		ACS_LOG_ERROR("Cannot statfs working dir " << work_dir << " [errno=" << errno << "]");
		return false;
	}

#define BYTES_PER_MEGA 1048576

	size_t totalDiskSpace = buf.f_bavail * buf.f_bsize / BYTES_PER_MEGA;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
	excerr << "ExecutingOrder::checkDiskSpace: " << buf.f_bavail <<" free blocks * " << buf.f_bsize << " bytes per block = "
	<< totalDiskSpace << " Mb\n";
	excerr << "ExecutingOrder::checkDiskSpace: requested free space is " << requested << " Mb\n";
	excerr << "ExecutingOrder::checkDiskSpace: reserved quota is " << reserved << " Mb\n";
	excerr << "ExecutingOrder::checkDiskSpace: free space needed is " << requested + reserved << " Mb\n";
	excerr << "ExecutingOrder::checkDiskSpace: the check is " << ((totalDiskSpace >= (requested+reserved))?"":"NOT ") << "successful\n";
	ACS_CLASS_END_DEBUG

	return (totalDiskSpace >= (requested + reserved));
}

bool
ExecutingOrder::check(bool verboseFlag)
{
	// first check if necessary disk space is available
	//
	size_t reserved = 0;

	{
		// read the settings for the reserved Disk Quota in the daemon configuration space
		rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
		rsPushNSpace nspace_tld (rs, _schedRootKey);

		if (rs.hasKey(_reservedQuotaKey) )
		{
			rs.getValue(_reservedQuotaKey, reserved);

			ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Value of " << _reservedQuotaKey << " is " << reserved);

		}


		if ( !checkDiskSpace(_taskTable->getMinDiskSpace(), reserved, _productionDir) )
		{
			ostringstream os;
			os << "Not enough disk space on [" << _productionDir <<
					"]. Cannot execute order " << ((*this).operator tlOrder()).getOrderSignature() << "\n";
			ACS_LOG_OPEVENTALERT(_diskFullEventRegister, os.str() );
			//excmcf << alert;
			return false;
		}
	}


	bool result = false;

	// In a nominal using way the exception will be caught by OrderExecutionMgr::checkPreconditions
	// and the order will not continue its processing cycle
	string satelliteName;

	// read the satellite and the mission from the TlOrder
	// The mission is defaulted to CRYOSAT
	{
		dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (connWrp);
		const string &dbKey = conn.getKey() ;
		dbSatelliteQuery * satQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", dbKey));
		if(!satQuery)
		{
			ExecutingOrderException e("Error in executing dbSatelliteQuery");
			ACS_THROW(e);
		}
		dbGenQueryScopeGuard satQueryGuard(satQuery);
		dbQueryParameters satParameters(dbKey) ;
		satParameters.addParameter(dbParam(dbKey, "t_satellites","satelliteid", dbParam::EQUAL_TO, db::toSQL(dbKey, _order.getSatelliteId()))) ;
		satQuery->addParameter(satParameters) ;
		int num = satQuery->doQuery(conn) ;
		if (num > 0)
		{
			satelliteName = ((*satQuery)[0])->getSatelliteName();
		}
		else
		{
			satelliteName = "CRYOSAT";
		}
	}

	// Verify if the mission number is set in the order
	// the case mission number not set is implied by a null string
	string missionNumber = "";
	bool missionNumberSet;
	int misNum = _order.getMission (&missionNumberSet);
	if (missionNumberSet)
	{
		ostringstream os ;
		os << misNum ;
		missionNumber = os.str() ;
	}


	// store the mission number and the satellite name
	// in order to use them later on
	_satelliteName = satelliteName;
	_missionNumber = missionNumber;

	StringKey theKey = StringKeyUtils::makeKey (_satelliteName, _missionNumber, _order.getProcessorName(), _order.getProcessorVersion());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::check the generated key is : " << theKey );

	// This can throw....
	_downloader = new InputResourceMgr(theKey, _order, *_taskTable, _cacheManager);
	result = _downloader->checkInputSet(verboseFlag);

	// If the check is successfuly executed
	// store internally the symbol table and the visited task table
	// in order to correctly handle the job order creation process
	if (result)
	{
		_downloader->getVisitedTaskTable(_visitedTaskTable);
		_downloader->getSymbolTable (_theSymTable);
	}


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::TL_PCC check was [" << (result?"OK":"NOT OK") << "]");

	return result;

}

bool
ExecutingOrder::download()
{
	exFMessage msg;

	// _downloader can not exist at all
	// we assume that will never use download if the creation of downloader fails

	if (!_downloader)
	{
		ACS_LOG_ERROR("ERROR - Cannot download without any valid downloader");

		// There was a call to download() without creating any valid downloader
		// we notify this event and return false to the caller
		// Housekeeping is made by the caller
		return false;
	}

	try
	{
		createWorkingDir();
	}
	catch(ExecutingOrderException& e)
	{
		ACS_LOG_NOTIFY_EX(e);
		return false;
	}
	catch (exception&)
	{
		// all other exceptions are caught by parent
		// only debug informations and then re-throw
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"download: unknown exception while creating working dir");
		throw;
	}


	// download input files
	ACS_LOG_INFO("Downloading input set for order " << _order.getOrderSignature());
	{
		// APF-271
		// This will force the jobid tag in the messages emitted during the download of the input set
		MCScopedParams mcProcId;

		// jobid must be a string
		ostringstream osjobid;
		osjobid << _order.getOrderId();

		mcProcId << mc_param("jobid", osjobid.str());

		if (!_downloader->downloadInputSet(_baseDir))
			return false;
	}

	// download configuration space files
	try
	{
		ACS_LOG_INFO("Downloading configuration space files for order " << _order.getOrderSignature());
		downloadConfigurationSpaces ();
	}
	catch(ExecutingOrderException& e)
	{
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_LOG_NOTIFY_EX(e);
		return false;
	}
	catch (exception&)
	{
		// all other exceptions are caught by parent
		// notify error status to MCF, give some debug informations and then re-throw
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"download: unknown exception while downloading the configuration files");
		throw;
	}

	// download the parameters file (if any)
	try
	{
		ACS_LOG_INFO("Downloading parameters file for order " << _order.getOrderSignature());
		downloadParametersFile ();
	}
	catch(ExecutingOrderException& e)
	{
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_LOG_NOTIFY_EX(e);
		return false;
	}
	catch (exception&)
	{
		// all other exceptions are caught by parent
		// notify error status to MCF, give some debug informations and then re-throw
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"download: unknown exception while downloading the parameter file");
		throw;
	}

	// Download all the order attachments
	// and prepare the attachment structure to be used in the job order
	try
	{
		downloadAttachments ();
	}
	catch(ExecutingOrderException& e)
	{
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_LOG_NOTIFY_EX(e);
		return false;
	}
	catch (exception&)
	{
		// all other exceptions are caught by parent
		// notify error status to MCF, give some debug informations and then re-throw
		(DownloadStatusMgrSingleton::instance()->getMgr())-> pulseStatus (DL_ERROR);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"download: unknown exception while downloading the order attachments");
		throw;
	}


	// if the private configuration file is not needed by the processor
	// the tasktable will not return a name... but this is not an error
	string _privateConfig;
	try
	{
		_privateConfig = _taskTable->getConfigFile (_order.getProcessorConfiguration());
	}
	catch (TaskTable::TaskTableNotFoundException& ex)
	{
		// the requested version of the configuration does not exist in the Task Table
		ostringstream os;
		if (_order.getProcessorConfiguration() == "")
		{
			os << "ERROR requesting default processor configuration file";
		}
		else
		{
			os << "ERROR requesting processor configuration version [" << _order.getProcessorConfiguration() <<
					"]";
		}
		ACS_LOG_ERROR (os.str());
		ACS_LOG_NOTIFY_EX (ex);
		// return false and set the order in ERROR
		return false;
	}
	if (_privateConfig != "")
	{
		rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		rsPushNSpace nspace_tld (rs, _tldRootKey);
		rs.getValue (_copyPConfKey, _copyPConf, false);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _copyPConfKey << " configured parameter set to: " << boolalpha << _copyPConf);
		if (_copyPConf)
		{
			// copy processor's private configuration file in the working directory
			if (File::exists(_taskTable->getConfigFile(_order.getProcessorConfiguration())))
			{
				ostringstream os;
				os << "cp " << _taskTable->getConfigFile(_order.getProcessorConfiguration()) << " " << genPrivateConfFileName();

				if (system(os.str().c_str()) != 0)
				{
					// copy failed
					ostringstream os;
					os << "ERROR: cannot copy private configuration file " <<
						_taskTable->getConfigFile(_order.getProcessorConfiguration()) <<
						" in the working directory for order " << _order.getOrderSignature();
					ACS_LOG_ERROR (os.str());
					return false;
				}
			}
			else
			{
				// error
				ostringstream os;
				os << "Private configuration file " <<
					_taskTable->getConfigFile(_order.getProcessorConfiguration()) <<
					" for order " << _order.getOrderSignature() << " does not exist!";
				ACS_LOG_ERROR (os.str());
				return false;
			}
		}
	}
	return true;
}


void
ExecutingOrder::exec()
{
	// start the thread
	start();
}


ProcessorTracker *
ExecutingOrder::getTracker()
{
	// The getTracker method
	// - creates the tracker (is it does not exist) generating the log filename to be followed
	// - starts it (re-starts if it is found not running)
	// - returnes the "live" instance of the tracker

	if (!_tracker)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::getTracker : Tracker does not exists ... creating ");

		try
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getTracker : TaskTableName [" << _taskTableName << "]");
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"getTracker : logPath [" << genSchedulerLogName() << "]");

			ACS_SMARTPTR<ProcessorTracker> theTracker(new ProcessorTracker( _order.getOrderId(), File::getDirPath(_baseDir) ));
			theTracker->start();
			_tracker = theTracker.release();
		}
		catch (exception&)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::getTracker : unable to create the tracker");
		}
	}
	else
	{
		if (!_tracker->running())
		{
			ACS_CLASS_BGN_DEBUG (ACS_LOW_VERB)

								ACS_CLASS_BGN_DEBUG (ACS_MID_VERB)
								std::exception* ex;
			if (_tracker->runException(ex))
			{
				ACS_LOG_INFO("Tracker thread stopped due to exception");
				ACS_LOG_NOTIFY_EX (*ex);
				delete ex;
			}
			ACS_CLASS_END_DEBUG

			ACS_LOG_INFO("ExecutingOrder::getTracker : Tracker is not running... starting it");
			ACS_CLASS_END_DEBUG

			_tracker->start();
		}
	}
	return _tracker;
}


SpawnedScheduler *
ExecutingOrder::getScheduler()
{

	if (!_scheduler)
	{
		string scheduler;

		// retrieving configuration
		rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
		rsPushNSpace nspace_tld (rs, _tldRootKey);
		if(!rs.hasKey(_schedulerPathKey) )    //no scheduler available
		{
			ExecutingOrderException e(string("Cannot find ") +
					_schedulerPathKey +
					string(" in the configuration file"));
			ACS_THROW(e) ;
		}
		else
		{
			rs.getValue(_schedulerPathKey, scheduler);
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"getScheduler[" << _order.getOrderId() << "]: scheduler path is " << scheduler);
		}

		_scheduler = new SpawnedScheduler (scheduler, genWorkOrderFileName());
		// _scheduler->setWorkOrder (genWorkOrderFileName());
	}

	return _scheduler;
}


static const int polling_freq=1000; // 1 second in millisec
void
ExecutingOrder::run()
try
{
	// WARNING!  WARNING!  WARNING!
	// this function heavily relies on having only 1 return at the end (throws are handled anyway)
	// DO NOT insert any return in the middle, or the meaning and consistency of the _valid flag would be worsely affected
	// WARNING!  WARNING!  WARNING!
	//

	// APF-271
	MCScopedParams mcSlotId;

	// slotid must be a string
	ostringstream osslotid;
	osslotid << this->getAssignedSlot();

	mcSlotId << mc_param("slotid", osslotid.str());


	IsValidGuard theGuard (_valid);

	ACS_LOG_INFO("Running order " << _order.getOrderSignature());


	// Note that the JobOrder must be created befor the workorder
	// some information as satellite name and mission number are computed
	// while generating the job order file
	buildJobOrder();

	scOrder theWorkOrder = buildWorkOrder();

	try
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"run[" << _order.getOrderId() << "]: spawning scheduler(" <<
				genWorkOrderFileName() << ")");

		getScheduler()->execute(theWorkOrder);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::run[" << _order.getOrderId() << "] pid=" << _scheduler->getSchedulerPid());
	}
	catch(SpawnedScheduler::SpawnedSchedulerException &)
	{
		ACS_LOG_ERROR("ERROR: ExecutingOrder: cannot spawn scheduler(" << genWorkOrderFileName() << ")");
		throw;
	}


	// Have Just spawned a new order for a processor - Update Patrol status
	// Using a status guard to update the PR_RUNNING status
	ProcessorStatusMgrSingleton::StatusGuard _g(*ProcessorStatusMgrSingleton::instance()->getMgr(), PR_RUNNING, _mySlot);

	{
		// sending informations about the current processor

		// Collecting processor information

		ostringstream os;
		os << "CURRENT_PROCESSOR";
		os << "#" << _mySlot;
		ACS_LOG_EVENT (os.str(), _order.getProcessorName());

		os.str("");
		os << "PROCESSOR_VERSION";
		os << "#" << _mySlot;
		ACS_LOG_EVENT(os.str(), _order.getProcessorVersion());

		string proConVer;
		string proConFile;
		if  ((proConVer=_order.getProcessorConfiguration())==string("")) proConVer=_taskTable->getDefaultConfigVers();
		try
		{
			proConFile=_taskTable->getConfigFile(proConVer);
		}
		catch (exception&)
		{
			proConFile=" Unable to retrieve processor configuration filename";
		}

		os.str("");
		os << "PROCESSOR_CONF_NAME";
		os << "#" << _mySlot;
		ACS_LOG_EVENT(os.str(), proConFile);

		os.str("");
		os << "PROCESSOR_CONF_VERSION";
		os << "#" << _mySlot;
		ACS_LOG_EVENT(os.str(), proConVer);

		// APF-271

		// jobid must be a string
		ostringstream osjobid;
		osjobid << _order.getOrderId();

		string satelliteName(""); bool isSatelliteSet(false);
		int satId = _order.getSatelliteId(&isSatelliteSet);
		try
		{
			if ( isSatelliteSet ) {
				satelliteName = SatelliteName::instance(Application::instance()->getDefaultConnectionPool()->getKey())->name( satId );
			}
		}
		catch (exception& exc)
		{
			ACS_LOG_WARNING ("ExecutingOrder CTOR Unable to retrieve the satelliteName from the Id in the database. Defaulting to empty string. Got exception: " << exc.what());
		}
		ACS_LOG_MC_INFO ( mc_param("jobid", osjobid.str()) <<
				mc_param("satellite", satelliteName) <<
				mc_param("missionnumber", _order.getMission()) <<
				mc_param("processorname", _order.getProcessorName()) << mc_param("processorversion", _order.getProcessorVersion()) <<
				mc_param ("sensingstarttime", _order.getStartTime().asISO8601(3)) <<
				mc_param ("sensingstoptime", _order.getStopTime().asISO8601(3)) <<
				mc_metric(Metric_RUNNING));
	}
	// create timer
	Timer tmax;
	if (_maxTime) {
		tmax.start();
	}


	// this will keep track of the processing percentage
	// in order to emit the events only when pct changes
	// It starts with "-1" to assure pct=0% message to patrol
	int pctComplete = -1;

	// periodically poll for process completion or timer expiration
	for (;;)
	{
		Timer tperiod;
		tperiod.delay(polling_freq);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "run[" << _order.getOrderId() << "]: periodic timer expired ");

		// execute processor tracker and send overall percentage message to Patrol
		if (getTracker())
		{
			int pct = lround(getTracker()->getTotalPercentComplete());

			if (pct != pctComplete)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ExecutingOrder::run : processing percentage detected " << pct);


				pctComplete = pct;
				ostringstream event_str;
				event_str << _percentageBaseString;
				event_str << "#" << _mySlot;

				// MCF needs an integer as percentage value
				ACS_LOG_EVENT(event_str.str(), pct);

				// APF-271
				// jobid must be a string
				ostringstream osjobid;
				osjobid << _order.getOrderId();
				ACS_LOG_MC_INFO ( mc_param("jobid", osjobid.str()) << mc_param("percentagedone", pct) <<
						mc_param ("status", "PROCESSING") << mc_metric(Metric_PROGRESS)) ;

			}
		}

		// check if timeout expired
		if (_maxTime)
		{
			tmax.stop(); // makes the timer read current time (otherwise elapsed won't work)

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::run[" << _order.getOrderId() << "]: total expired time (in millisec) " << tmax.elapsed());


			if (tmax.elapsed() >= unsigned(_maxTime*1000))
			{
				ACS_LOG_INFO("ExecutingOrder[" << _order.getOrderId() << "]: timeout timer expired - killing the process");

				getScheduler()->sendKillSignal();

				// give the process time to exit
				Timer t;
				t.delay(1000); // 1 second

				// now fall down to handle completion
			}
			else
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "run[" << _order.getOrderId() << "]: max timer did not expire ");
			}
		}

		// check if process completed and how
		if (!getScheduler()->isSchedulerRunning())
		{

			// exit the Suspended status is needed in order to avoid persistent SUSPENDED status
			// caused by a scheduler that exits the run ignoring a SUSPEND command
			exitSuspendedStatus();

			SchedulerExitStatus ec;
			if (getScheduler()->getExitStatus(ec))
			{
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "run[" << _order.getOrderId() << "]: process terminated with exit code " << int(ec));
				ostringstream msg;
				switch(ec)
				{
				case ssKILLED:
					msg << "Order #" << _order.getOrderId() << " processing failure: process was killed\n";
					ACS_LOG_INFO(msg.str());
					break;
				case ssOK:
					msg << "Order #" << _order.getOrderId() << ": processing process was successful\n";
					ACS_LOG_INFO(msg.str());
					break;
				case ssINTERNAL_ERR:
				case ssNO_ORDER:
					msg << "Order #" << _order.getOrderId() << " processing failure: scheduler internal error\n";
					ACS_LOG_INFO(msg.str());
					break;
				case ssNO_SPACE:
				case ssNO_WDIR:
					msg << "Order #" << _order.getOrderId() << " processing failure: not enough space on disk to run the order\n";
					ACS_LOG_INFO(msg.str());
					break;
				case ssTASK_FAILED:
				case ssTASK_CRASHED:
				case ssABORTED:
				case ssSTOPPED:
					msg << "Order #" << _order.getOrderId() << " processing failure: one of the processor's task failed\n";
					ACS_LOG_INFO(msg.str());
					break;
				default:
					msg << "Order #" << _order.getOrderId() << " processing failure: unknown exit code " <<
					int(ec) << " returned from the scheduler\n";
					ACS_LOG_INFO(msg.str());
					break;
				}

				//Actions to be performed after the Order's execution ends.

				//
				//In case of Success, and if requested by configuration, relate the used Inputs with this Order.
				//
				if ( ec == ssOK )
				{
					recordOrderInput();
				}

				//
				// in case of failure, send to MCF completion percentage
				// with negative sign, in order to allow the mimic to show
				// some visual clue of the failure, otherwise operator will not
				// be able to distinguish between a failed order and a running one
				//
				// then, send an alert to MCF
				//

				if (ec != ssOK)
				{
					pctComplete = -pctComplete;

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::run : sending negative processing percentage " <<
							pctComplete << " to MCF");


					ostringstream event_str;
					event_str << _percentageBaseString;
					event_str << "#" << _mySlot;

					ACS_LOG_EVENT(event_str.str(), pctComplete);
					//excmcf << event;

					ACS_LOG_OPALERT(msg.str());
					//		  excmcf << alert;
				}



				// make parent update order status
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "run[" << _order.getOrderId() << "]: notifying parent");
				_parent.notifySchedulerCompletion(*this, SchedulerExitStatus(ec));

			}
			else
			{
				ostringstream msg;
				msg << "Order #" << _order.getOrderId() << " ERROR: process terminated but no valid exit code is available\n";
				ACS_LOG_ERROR(msg.str());
				ACS_LOG_OPALERT(msg.str());
				//excmcf << alert;


				pctComplete = -pctComplete;

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::run : sending negative processing percentage " <<
						pctComplete << " to MCF");


				ostringstream event_str;
				event_str << _percentageBaseString;
				event_str << "#" << _mySlot;

				ACS_LOG_EVENT(event_str.str(), pctComplete);
				//excmcf << event;


				// force parent update order status to ERROR
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "run[" << _order.getOrderId() << "]: notifying parent");
				_parent.notifySchedulerCompletion(*this, SchedulerExitStatus(ssTASK_CRASHED));
			}

			// Pulsing the ERROR or WARNING (not used yet) status according to the scheduler exit status
			if (SchedulerExitStatus(ec) != ssOK)
			{
				(ProcessorStatusMgrSingleton::instance()->getMgr())-> pulseStatus (PR_ERROR, _mySlot);

				// emit a critical event to alert operator using MCF streams
				ostringstream os;
				os << "Failure processing orderId# " << _order.getOrderId();
				ACS_LOG_OPEVENTALERT(_errorEventRegister, os.str());
				//excmcf << alert;
			}


			// quit the loop and exit
			break;
		}
	}
}
catch (const exception& toCatch) 
{
	ACS_LOG_NOTIFY_EX(toCatch);

	// Make notification of the thread setup failure
	// parent will do the database update
	_parent.notifySetupFailure (*this);

	throw;
}

void
ExecutingOrder::stopDownload()
{
	if (_downloader)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "stopDownload [" << _order.getOrderId()<< "]: stopping downloaders ...  ");

		_downloader->abortDownload();
	}
}

void
ExecutingOrder::cleanup()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cleanup: going to wait for process completion - Order " <<
			_order.getOrderSignature());


	// wait for end of task; it has been killed previously by haltOrder() (==>IMMEDIATE_STOP)
	// or it has not been killed at all (==>CLEAN_STOP)
	while (_valid)
	{
		Timer::delay (1000); // 1 second
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ExecutingOrder::cleanup: finished waiting for process completion - Order " <<
			_order.getOrderSignature());

	// then the run() method will take care of housekeeping until the purge will be called
}
void
ExecutingOrder::stopAndJoin()
{
	stop(true) ;
}

void
ExecutingOrder::haltOrder(stopType stopMode)
{
	// check for IMMEDIATE_STOP
	if (stopMode==IMMEDIATE_STOP)
	{
		// stop current downloads (if any)

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "haltOrder: stopping active downloads");

		stopDownload();


		// sendKillSignal will not work if isSchedulerRunning is false even if spawn in progress
		if (getScheduler()->isSchedulerRunning())
		{
			// this means that the thread is running and needs to be killed .... (SIGTERM)

			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"cleanup: OrderID # " << _order.getOrderId() << " running ");
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"cleanup: killing (sending SIGTERM)" <<
					" process  - OrderID #" << _order.getOrderId());

			getScheduler()->sendKillSignal(SIGTERM);  // Kill the child proc
		}
		else  // if (getScheduler()->spawned())
		{
			// this means that the order is not in running state... forcing it to ILLEGAL
			// because no one can start it during an IMMEDIATE_STOP

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "cleanup: OrderID # " << _order.getOrderId() << " not running ");

			_valid=false;
		} // if (getScheduler()->spawned())
	}   // (stopMode==IMMEDIATE_STOP)
}


// STL HELPER CLASSES - BEGIN


// Search and replace token
// to be used while defining the parameters

static const char* _procNameToken = "${PROCNAME}";
static const char* _procVersionToken = "${PROCVERSION}";
static const char* _satNameToken = "${SATNAME}";
static const char* _missionToken = "${MISSION}";
static const char* _taskNameToken = "${TASKNAME}";


class ParamsAccumulator
{
public:
	ParamsAccumulator (vector<string>& v, const Task& t , const string& pn, const string& pv, const string& sat, const string& mis) :
		_v(v),
		_task(t),
		_pName(pn),
		_pVersion(pv),
		_satName(sat),
		_missNumber(mis)
{
}

	bool replaceToken (string& value, const string& token, const string& newval, bool& invalidReplace, ostringstream& os)
	{
		bool retval = false;

		if (value.find (token) != string::npos)
		{
			retval = true;
			if (newval != "")
			{
				boost::replace_first(value, token, newval);
			}
			else
			{
				invalidReplace = true;
				if (os.tellp() == ostringstream::pos_type(0))
				{
					os << "Unable to replace te following tokens: " << token;
				}
				else
				{
					os << ", " << token;
				}
			}
		}
		return retval;
	}

	// Extract the name and value from
	// the parameter descriptor
	// and store it in the vector<string>
	void operator() (const TaskParameterDescriptor& d)
	{
		bool isNameSet;
		string name = d.getName (&isNameSet);
		string value = d.getValue();
		ostringstream message;
		bool replaceFailed = false;

		if (replaceToken (value, _procNameToken, _pName, replaceFailed, message))
		{
			// Just for code readability
			// ProcessorName Replaced
		}
		else if (replaceToken (value, _procVersionToken, _pVersion, replaceFailed, message))
		{
			// Just for code readability
			// ProcessorVersion Replaced
		}
		else if (replaceToken (value, _satNameToken, _satName, replaceFailed, message))
		{
			// Just for code readability
			// SatelliteName Replaced
		}
		else if (replaceToken (value, _missionToken, _missNumber, replaceFailed, message))
		{
			// Just for code readability
			// MissionNumber Replaced
		}
		else if (replaceToken (value, _taskNameToken, _task.getName(), replaceFailed, message))
		{
			// Just for code readability
			// TaskName Replaced
		}

		if (replaceFailed)
		{
			ACS_COND_THROW (d.isMandatory(), ExecutingOrder::ExecutingOrderException (message.str()));
		}
		else
		{
			if (isNameSet)
			{
				_v.push_back(name);
			}
			_v.push_back(value);
		}
	}

private:
	vector<string>& _v;
	const Task& _task;
	string _pName;
	string _pVersion;
	string _satName;
	string _missNumber;
};

class ParametersTaskParser
{
public:
	ParametersTaskParser (scOrder& wo, const string& pname, const string& pversion, const string& sat, const string& miss) :
		_wo(wo),
		_pName(pname),
		_pVersion(pversion),
		_satName(sat),
		_missNumber(miss)
{
}

	// Call the extraction algo for each
	// task parameter descriptor
	void operator() (const Task& t)
	{
		vector<TaskParameterDescriptor> tparams;
		t.getList(tparams, always_true_pred(tparams), true, true);
		if (tparams.size() > 0)
		{
			vector<string> taskParameters;
			for_each (tparams.begin(), tparams.end(), ParamsAccumulator(taskParameters, t, _pName, _pVersion, _satName, _missNumber));
			_wo.setCmdLineParams (t.getName(), taskParameters);
		}
	}

private:
	scOrder& _wo;
	string _pName;
	string _pVersion;
	string _satName;
	string _missNumber;
};

// STL HELPER CLASSES - END

scOrder
ExecutingOrder::buildWorkOrder()
{
	scOrder wo;

	wo.setLogFileName(genLogFileName());
	wo.setProcName(_taskTable->getProcName());
	wo.setProcVersion(_taskTable->getProcVersion());
	wo.setTaskTable(_taskTable->getPath());
	wo.setJobOrder(genJobOrderFileName());
	wo.setBaseDir(_baseDir);
	wo.doICreateDir(false);
	wo.doIKillBackgroundTasks(true);
	wo.setSatellite (_satelliteName);
	wo.setMission (_missionNumber);

	// APF-411 - set default exit code interpretation
	wo.setPosixExitCode (_posixExitCode);

#ifdef SET_SCHEDULER_LOG_NAME
	wo.setSchedulerLogName (genSchedulerLogName());
#endif //SET_SCHEDULER_LOG_NAME

	wo.setTrackerFileName (genTrackerFileName());

    // let the scheduler cleanup the directory only for non-troubleshooting orders
    // (note that if order execution fails, the sheduler won't cleanup if not explicitely request using FORCE)
    bool haveToCleanup = !_order.getTroubleshooting();
    bool applyCleanupToFailures = false;

    {
        // read the settings in the daemon configuration space
        rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
        rsPushNSpace nspace_tld (rs, _tldRootKey);

        // Default implementation is TRUE, that means -> delete if not in trobuleshooting and do not force
        // In case of non-troubleshooting haveToCleanup is already set to true and so an update is only needed in case of FALSE or FORCED
        string cleanDir = "TRUE";
        // not mandatory.. defaulted to false
        // TLD_Clean_Working_Dir
        // FALSE -> never delete the working directories
        // TRUE -> delete the working directory if the processing is completed
        // FORCED -> delete the working directory in any case, even in case of failures
        rs.getValue (_cleanDirKey, cleanDir, false);
        ACS_LOG_INFO ("CLEANDIR VALUE SET TO " << cleanDir);
        if (StringUtils::compareNoCase ("FALSE", cleanDir) == 0)
        {
            // False... override it 
            haveToCleanup = false;
        }
        else if (StringUtils::compareNoCase ("FORCED", cleanDir) == 0)
        {
            // Apply cleanup policy also in case of error (will not delete troubleshooing orders too)
            applyCleanupToFailures = true;
        }
    }

    wo.doICleanupOnExit(haveToCleanup);
    wo.applyCleanupPolicyOnError(applyCleanupToFailures);
    ACS_LOG_INFO ("haveToCleanup = " << boolalpha << haveToCleanup << " - onFailures = " << applyCleanupToFailures);

	// scan the task table searching for task's command line parameters
	vector<Task> theTaskList;

	// cleanup the vector ad do recursive search
	_taskTable->getTaskList (theTaskList, true, true);

	// get the atexit tasks to manage their parameters too
	const vector<AtExitSequence>& atExSeqs=_taskTable->getAtExitSequences();

	for (size_t idx=0; idx<atExSeqs.size(); ++idx)
	{
		// do not cleanup but go inside nested elements
		atExSeqs[idx].getTaskList (theTaskList, false, true);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::buildWorkOrder() : found " << theTaskList.size() << " tasks in the Task Table");

	// Accumulate the parameters in the workorder
	for_each (theTaskList.begin(), theTaskList.end(), 
			ParametersTaskParser(wo, _order.getProcessorName(), _order.getProcessorVersion(), _satelliteName, _missionNumber));

	// Set the mandatory output regexp association map
	wo.setRegExpAssociationMap (_mandatoryOutputRegExpMap);

	// Set the resource manager section
	string resManagerKey("");
	string resManagerUrl("");
	string resManagerClientId("");
	if ( _orderReservedResourcesSptr && _orderReservedResourcesSptr->isService() ) {
		resManagerKey =      _orderReservedResourcesSptr->getServiceResManagerKey();
		resManagerUrl =      _orderReservedResourcesSptr->getResManagerUrl();
		resManagerClientId = _orderReservedResourcesSptr->getClientId();
	}
	wo.setResManagerKey( resManagerKey );
	wo.setResManagerUrl( resManagerUrl );
	wo.setResManagerClientId( resManagerClientId );

	return wo;
}



class Altern_finder : public acs::func::unary_function<InputDescriptor, bool>
{
	const InputAlternative& a;

public:
	explicit Altern_finder(const InputAlternative& aa) : a(aa) {}
	bool operator() (const InputDescriptor& d) const
	{
		// returns true if the Alternative is found into the descriptor
		//return (find(d.begin(), d.end(), a) != d.end());
		vector<InputAlternative> alternatives;
		d.getList(alternatives, always_true_pred(alternatives));
		return (find(alternatives.begin(), alternatives.end(), a) != alternatives.end());
	}
};

class FileType_finder : public acs::func::unary_function<InputDescriptor, bool>
{
	const string& s;

public:
	explicit FileType_finder(const string& ss) : s(ss) {}
	bool operator() (const InputDescriptor& d) const
	{
		try
		{
			// next statement throws if filetype s is not found in any of alternatives of d
			d.getAlternative(s);
			return true;
		}
		catch(exception&)
		{
			return false;
		}
	}
};


class configSpaceAdder
{
	const ACS_SMARTPTR<acs::JobOrder> & _jo;

public:
	configSpaceAdder (const ACS_SMARTPTR<acs::JobOrder>& jo): _jo(jo) {}

	void operator() (const ConfigSpaceDescriptor& aDescriptor)
	{
		_jo -> addConfigSpace( aDescriptor );
	}
};

class orderAttachmentAdder
{
	const ACS_SMARTPTR<acs::JobOrder> & _jo;

public:
	orderAttachmentAdder (const ACS_SMARTPTR<acs::JobOrder>& jo): _jo(jo) {}

	void operator() (const JobOrderAttachmentDescriptor& aDescriptor)
	{
		_jo -> addAttachment ( aDescriptor );
	}
};

class notUniqueAlternative
{
	InputAlternative _alternative;
	ACS_CLASS_DECLARE_DEBUG_LEVEL (notUniqueAlternative)

public:
	notUniqueAlternative (const InputAlternative& theAlternative): _alternative(theAlternative)
	{

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NotUniqueAlternative:: class ctor for comparing alternative filetype " << _alternative.getFileType());

	}

	bool operator() (const InputDescriptor& a) const
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NotUniqueAlternative:: Operator () called for an Input descriptor... computing for input alternative");

		// returns true if the target filetype is found inside an input alternative
		vector<InputAlternative> alternatives;
		a.getList(alternatives, always_true_pred(alternatives));
		bool result = ( find_if (alternatives.begin(), alternatives.end(), notUniqueAlternative(_alternative)) != alternatives.end() );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "NotUniqueAlternative:: (InputDescriptor) Returning " << (result? "TRUE" : "FALSE") << " for filetype " << _alternative.getFileType() );

		return result;
	}

	bool operator() (const InputAlternative& a) const
	{
		ACS_CLASS_BGN_DEBUG (ACS_LOW_VERB)
							excout << "NotUniqueAlternative:: Operator () called for an Input alternative ... \n";
		excout << "NotUniqueAlternative:: comparing input alternative [" << a.getFileType() << "] and target alternative [" <<
				_alternative.getFileType() << "] (result=" << ( a == _alternative ) << ")\n";
		a.dump();
		_alternative.dump();
		ACS_CLASS_END_DEBUG

		return ( a == _alternative );
	}
};


ACS_CLASS_DEFINE_DEBUG_LEVEL(notUniqueAlternative)

string
ExecutingOrder::getFileTypeRegExp (const std::string& theFileType)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getFileTypeRegExp called for filetype [" << theFileType << "]");

	// retrieve regular expression from DB... put it in the fileType versus RegExp map
	// in order to avoid re-access the DB later on if this filetype is used again
	if (_fileTypesRegExpMap.find (theFileType) == _fileTypesRegExpMap.end())
	{
		// it is the first time the filetype regexp is requested
		// db access is needed

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::getFileTypeRegExp accessing database...");
		try
		{
			dbFileTypeCheck regExpRetriever(theFileType);

			// Minimize the connections used
			dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));	
			dbConnection& conn (connWrp);
			// getRealRegexp returns the collection of all the regular
			// expression associated to a virtual filetype
			// if a virtual one has to be used as output in a processing
			_fileTypesRegExpMap[theFileType] = regExpRetriever.getRealRegExp(conn);
		}
		catch (exception& ex)
		{
			// this filetype is not known at database level
			// using the default form .*FILETYPE.*
			ostringstream os;
			os << ".*" << theFileType << ".*";
			_fileTypesRegExpMap[theFileType] = os.str();
		}
	}
	else
	{
		// the filetype regexp has been previously retrieved from the database
		// there is no need to access the database. Use the mapped value

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getFileTypeRegExp regexp already mapped");
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getFileTypeRegExp returning [" << _fileTypesRegExpMap[theFileType] <<
			"] for filetype [" << theFileType << "]");

	return _fileTypesRegExpMap[theFileType];
}


void
ExecutingOrder::buildJobOrder()
{
	try
	{
		TaskTable &tt = *_taskTable;

		string satelliteName = "";

		// read the satellite and the mission from the TlOrder

		bool satelliteIdSet;
		_order.getSatelliteId(&satelliteIdSet);

		if (satelliteIdSet)
		{
			dbSatelliteQuery * satQuery=dynamic_cast<dbSatelliteQuery*>(dbQueryFactoryChain::instance()->newObject("dbSatelliteQuery", Application::instance()->getDefaultConnectionPool()->getKey()));
			if (!satQuery)
			{
				ExecutingOrderException e("Error in executing dbSatelliteQuery");
				ACS_THROW(e);
			}
			dbGenQueryScopeGuard satQueryGuard(satQuery);
			try
			{
				dbConnectionWrapper connWrp (*(Application::instance()->getDefaultConnectionPool()));
				dbConnection& conn (connWrp);
				const string &dbKey = conn.getKey() ;
				dbQueryParameters satParameters(dbKey) ;
				satParameters.addParameter(dbParam(dbKey,"t_satellites","satelliteid", dbParam::EQUAL_TO, db::toSQL(dbKey,_order.getSatelliteId()))) ;
				satQuery->addParameter(satParameters) ;
				int num = satQuery->doQuery(conn) ;
				if (num > 0)
				{
					satelliteName = ((*satQuery)[0])->getSatelliteName();
				}
			}
			catch (exception&)
			{
				// Problem in the query execution
				ACS_LOG_WARNING("Unable to retrieve the satelliteName from the Id in the database. Defaulting to empty string.");

				// The satelliteName is set to "" by default
				satelliteName = "";
			}
		}

		// Verify if the mission number is set in the order
		// the case mission number not set is implied by a null string
		string missionNumber = "";
		bool missionNumberSet;
		int misNum = _order.getMission (&missionNumberSet);
		if (missionNumberSet)
		{
			ostringstream os ;
			os << misNum ;
			missionNumber = os.str() ;
		}

		string processingStationId;
		string processingStationName;

		// retrieve the proper JobOrder Type using the configuration singleton
		rsResourceSet &_conf = ConfigurationSingleton::instance() ->get();

		// this resource doesn't depend from the configuration space TlDaemon
		_conf.getValue (_procStationKey, processingStationId);
		_conf.getValue (_procStationNameKey, processingStationName);


		// store the mission number and the satellite name
		// in order to use them generating the work order file
		_satelliteName = satelliteName;
		_missionNumber = missionNumber;



		StringKey key = StringKeyUtils::makeKey (satelliteName, missionNumber, _order.getProcessorName(), _order.getProcessorVersion());

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "buildJobOrder : job order KEY is " << key);

		ACS_SMARTPTR<JobOrder> jo;
		try
		{
			jo.reset ((JobOrderFactoryChain::instance())->newObject(key));
		}
		catch (exception& ex)
		{
			ACS_LOG_WARNING("Unable to get a valid JobOrder from the factory. Skipping the order.");
			throw;
		}

		if (jo.get() == NULL)
		{
			// The factory returned a null pointer
			// Cannot go on with the processing
			ExecutingOrderException e(string("Unable to allocate a job order for the order (NULL pointer from the factory)"));
			ACS_THROW (e) ;
		}

		jo->setProcStation     (processingStationId);
		jo->setStationName     (processingStationName);
		jo->setProcName        (tt.getProcName());
		jo->setProcVersion     (_order.getProcessorVersion());
		jo->setLogLevel        (_logLevel);
		jo->setOutLogLevel     (_outLogLevel == LogLevelDEFAULT ? _logLevel : _outLogLevel );
		jo->setErrLogLevel     (_errLogLevel == LogLevelDEFAULT ? _logLevel : _errLogLevel );
		jo->setConfig          (genPrivateConfFileName());
		jo->setOrderType       (_order.getOrderType());
		jo->setExternalOrderId (_order.getExternalOrderId());
		jo->brkEnabled         (_order.getTroubleshooting());
		jo->isTroubleshooting  (_order.getTroubleshooting());
		jo->setOrderId         (_order.getOrderId());

		// Set the parameters file name
		// if no parameters file was uploaded in the database
		// the filename will be a null string

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "buildJobOrder[" << _order.getOrderId() <<
				"]: parameters file is \"" << _parametersFileName << "\"");

		jo->setParametersFileName (_parametersFileName);

		for_each (_configSpaces.begin(), _configSpaces.end(), configSpaceAdder(jo) );

		jo->setTimeInterval  (_order.getStartTime(), _order.getStopTime());
		jo->isTest           (tt.isTest());
		jo->setWorkingDir    (_baseDir);
		jo->setSatellite     (satelliteName);
		jo->setMission       (missionNumber);

		// set the attachment section
		for_each (_orderAttachments.begin(), _orderAttachments.end(), orderAttachmentAdder(jo) );

		// prepare to generate the input sections:
		// next list will contain the purged inputs after DB check and download
		vector<InputDescriptor> downloadedInputs;
		PCC_Base::SymTable downloadedSymTable;
		_downloader->getDownloadedSymbolTable(downloadedSymTable);

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "buildJobOrder[" << _order.getOrderId() <<
				"]: downloadedInputList size is " << downloadedInputs.size());

		ACS_CLASS_BGN_DEBUG (ACS_MID_VERB)
		excout << "ExecutingOrder::buildJobOrder dumping returned inputs\n";
		for (size_t i=0; i<downloadedInputs.size(); i++)
		{
			excout << "Input descriptor # " << i+1 << " has size " << downloadedInputs[i].size() << "\n";
			for (size_t j=0; j<downloadedInputs[i].size(); j++)
			{
				excout << "- input alternative # " << j+1 << "\n";
				//downloadedInputs[i][j].dump();
				(static_cast<InputAlternative*>(downloadedInputs[i][j]))->dump();
				vector<string> paths;
				//downloadedInputs[i][j].getPathNames(paths);
				(static_cast<InputAlternative*>(downloadedInputs[i][j]))->getPathNames(paths);
				excout << "  list of files size is " << paths.size() << "\n";

			}
			excout << "\n";
		}
		ACS_CLASS_END_DEBUG

		// collect all the tasks simulating a large single pool
		// Unfortunately the job order is a vector of tasks.. so the tasks cannot be polymorphic
		// will use polymorphic objects instead for input, output and breakpoints 
		vector<Task> ptaskpool;
		tt.getTaskList(ptaskpool);

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "buildJobOrder[" << _order.getOrderId() <<
				"]: Task Table's TASK count is [" << ptaskpool.size() << "]");

		// Cleanup the mandatory output regular expression map
		_mandatoryOutputRegExpMap.clear();


		{
			// ... for each task...
			for (vector<Task>::iterator ptask = ptaskpool.begin(); ptask != ptaskpool.end(); ++ptask)
			{
				// ... generate a mirror task (cleaned!) to be inserted in the job order:
				Task t;

				// 1 - copy general info
				t.setName(ptask->getName());
				t.setVersion(ptask->getVersion());
				t.setPath(ptask->getPath());
				t.isCritical(ptask->isCritical());

				// 2 - copy outputs
				{

					TaskTableElement::TaskTableSmartVector<OutputDescriptor> ptro;
					ptask->getPtrList (ptro, always_true_pred(vector<OutputDescriptor> ()) );

					for (vector<OutputDescriptor*>::const_iterator poutputIterator = ptro.container().begin();
							poutputIterator != ptro.container().end();
							++poutputIterator)
					{

						OutputDescriptor* poutput = *poutputIterator;

						// Mandatory output have to be checked by the scheduler
						if (poutput->isMandatory())
						{
							// Add the regular expression to the mandatory map
							_mandatoryOutputRegExpMap[poutput->getFileType()] = getFileTypeRegExp (poutput->getFileType());
						}

						if (poutput->getFileNameType() == FileNameTypeREGEXP)
						{
							// the outputs having REGEXP needs the definition of the pathname
							// according to the same convention of the input
							// the pathnames of the outputs are managed inside the joborders when the fileNameType is set to
							// - Logical
							// - Stem
							// - Physical (deprecated... use Directory instead!)
							// - Directory
							string theRegExp = getFileTypeRegExp (poutput->getFileType());
							poutput->setPathName( genInterimFullFileName(theRegExp) );
						}
						t.addOutput(*poutput);
					}
				}

				// 3 - copy breakpoints
				{

					TaskTableElement::TaskTableSmartVector<BreakpointDescriptor> l;
					ptask->getPtrList (l, always_true_pred(vector<BreakpointDescriptor> ()) );

					for (vector<BreakpointDescriptor*>::const_iterator brkIterator = l.container().begin();
							brkIterator != l.container().end();
							++brkIterator)
					{

						BreakpointDescriptor* pbreakpoint = *brkIterator;
						t.addBreakpoint(*pbreakpoint);
					}
				}

				// 4 - copy inputs
				{

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
							"]: copying inputs for task " << ptask->getName() );

					TaskTableElement::TaskTableSmartVector<InputDescriptor> ptrl;
					ptask->getExtInputPtrList (ptrl, _order.getOrderType());

					// Cycle on the ptrl container elements (pointers to InputDescriptors)
					for (vector<InputDescriptor*>::const_iterator pinputIterator = ptrl.container().begin();
							pinputIterator != ptrl.container().end();
							++pinputIterator)
					{

						// Conversion added to safe the existing code and avoid regressions
						InputDescriptor* pinput = *pinputIterator;

						// This needs the parameter CLOB to ensure sorting in exotic cases (like ECICMF)
						pinput->sortAlternatives(InputDescriptor::SortByPreferenceOrder, _order.getParametersCLOB() );

						// create the input descriptor to be passed to the job order
						// start from a copy of the one we have
						InputDescriptor newInput(*pinput);

						// remove existing alternatives
						newInput.clear();

						bool isDuplicatedInput = false;	// false --> this is not a duplicate of a previously loaded input

						// scan original alternatives to check which one to put in the newInput

						TaskTableElement::TaskTableSmartVector<InputAlternative> altv;
						pinput->getPtrList (altv, always_true_pred(vector<InputAlternative> ()));

						for (vector<InputAlternative*>::const_iterator palternIter = altv.container().begin();
								palternIter != altv.container().end();
								++palternIter)
						{


							// Conversion added to safe the existing code and avoid regressions
							InputAlternative* paltern = *palternIter;

							// Set AltActivationTime to 0 in order to compare the InputAlternative with the one contained in the symbolic Table:
							// at this step of the processing the input is active and the activation time can be reset in order to avoid alternative duplications 
							// (inputAlternative could differ just for the AlternativeActivationTime)
							paltern->setAltActivationTime(0);

							// get the input descriptor stored in the current task
							vector<InputDescriptor> newInDesc;
							t.getInputList ( newInDesc , true );

							// check if the alternative is already mapped in the current task
							if ( find_if (newInDesc.begin(), newInDesc.end(), notUniqueAlternative (*paltern)) != newInDesc.end() )
							{
								// this alternative was already pushed in the input set
								// stop scanning this descriptor, and signal that the whole input needs not to be duplicated in the Job Order

								isDuplicatedInput = true;
								break;
							}

							if (paltern->getOrigin() == InputOriginDB)
							{
								// check if this alternative is among the ones loaded
								//vector<InputDescriptor> downloadedInputs;

								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::buildJobOrder[" << _order.getOrderId() <<
										"]: searching DB alternative " << paltern->getFileType() << " among the downloaded ones");

								PCC_Base::SymTable::iterator dd = downloadedSymTable.find(*paltern);


								if (dd != downloadedSymTable.end())
								{
									// we've found it: we've got a downloaded file for this input
									//InputAlternative aa(*paltern);
									paltern->clear();

									// fill up the pathnames (all the query results)
									for (size_t idx=0; idx < dd->second.fileList.size(); ++idx)
									{
										paltern->addPathName (dd->second.fileList[idx].fileName);
									}

									// fill up the time slots with related file name pointers
									for (size_t idx=0; idx < dd->second.fileTimeline.size(); ++idx)
									{
										int timeIndex = dd->second.fileTimeline[idx].fileNameIdx;
										acs::TimeRange timeRange = dd->second.fileTimeline[idx].timeRange;
										pair<int, acs::TimeRange> myPair (timeIndex, timeRange);
										paltern->addInterval ( myPair );
									}


									// push the alternative in the newInput...
									newInput.push_back(*paltern);

									ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
											"]: DB alternative " << paltern->getFileType() << " found and inserted");

									// ... and quit the for loop
									break;
								}
								else
								{
									ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
											"]: alternative " << paltern->getFileType() << " NOT FOUND among the downloaded ones; searching next");

									// nothing... simply, we've not found this alternative among the downloaded ones
									// but there could be the following one...
								}
							}
							else if (paltern->getOrigin() == InputOriginPROC)
							{
								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
										"]: inserting PROC alternative " << paltern->getFileType());


								//
								// it's up to the job order to generate the filename for PROC inputs;
								// however, in case of regexp filenametype, it's up to the TlDaemon to retrieve from DB
								// the regular expression associated to the involved FileType
								//

								if (paltern->getFileNameType() == FileNameTypeREGEXP)
								{
									ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
											"]: loading from DB regular expression for alternative " <<
											paltern->getFileType());

									string theRegExp = getFileTypeRegExp (paltern->getFileType());

									// The filename will be composed by the absolute pathname of the working dir
									// followed by the regular expression retrieved from the database
									paltern->addPathName( genInterimFullFileName(theRegExp) );
								}

								{
									// generate for Interim files a single TimeInterval for the whole file
									// (for DB inputs the intervals are generated by the input downloader)
									// the JobOrder generator will be in charge to generate the actual file name and dump the extended interval
									//
									DateTime startTime = _order.getStartTime();
									DateTime stopTime  = _order.getStopTime();


									startTime -= DateTime::JD50( paltern->getT0() / 86400.0);
									stopTime  += DateTime::JD50( paltern->getT1() / 86400.0);

									TimeRange extendedTimeRange (startTime, stopTime);
									paltern->addInterval(make_pair(0, extendedTimeRange));
								}


								newInput.push_back(*paltern);

								// now continue with other alternatives, should be there some DB alternative
							}
							else if (paltern->getOrigin() == InputOriginLOG)
							{
								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
										"]: inserting LOG alternative " << paltern->getFileType());

								paltern->addPathName(genLogFileName());
								newInput.push_back(*paltern);

								// now continue with other alternatives, should be there some DB alternative
							}
							else
							{
								ExecutingOrderException e(string("Bad input origin: ") + InputOrigin2String(paltern->getOrigin()));
								ACS_THROW (e) ;
							}
						}


						// at this point we've finished scanning alternatives, and we've got the new input descriptor
						// if it is empty, then there's definitely an error, since we've passed the precondition check
						// and download steps!
						if (newInput.size())
						{
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buildJobOrder[" << _order.getOrderId() <<
									"]: adding new input to task " << t.getName() << "; altern num. is " << newInput.size());

							t.addInput(newInput);
						}
						else
						{
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ExecutingOrder::buildJobOrder[" << _order.getOrderId() <<
									"]: new input size is null for task " << t.getName() <<
									"; the input is " << (isDuplicatedInput?"":"not ") << "duplicated");

							if (!newInput.isMandatory() || isDuplicatedInput)
							{
								// do nothing: all under control
							}
							else
							{
								ExecutingOrderException e("empty alternative list for mandatory input");
								ACS_THROW (e) ;
							}
						}
					}
				}

				jo->push_back(t);
			}
		}

		// As last step of the JobOrder configuration set the task table
		// This will start the parameter extraction iside the joborder plugin
		// and for this reason must be performed as the final step
		jo->setTaskTable (_taskTable);

		ofstream o(genJobOrderFileName().c_str());
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing..");

		o << *jo ;
	}

	catch (const exception& toCatch)
	{
		// we are here inside the thread
		// notify the exception to be sure it shows up at the proper time
		// then rethrow it to force exit to the parent process
		ACS_LOG_NOTIFY_EX(toCatch);
		throw;
	}

}

void
ExecutingOrder::downloadParametersFile ()
{
	_parametersFileName = "";
	bool isClobSet;
	const string& prms = _order.getParametersCLOB(&isClobSet);

	if (isClobSet)
	{
		_parametersFileName = genParametersFileName();
		ofstream o(_parametersFileName.c_str());
		o << prms;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "downloadParametersFile [" << _order.getOrderId() <<
				"]: created file \"" << _parametersFileName << "\"");
	}
	else
	{
		// Just debug messages here
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "downloadParametersFile [" << _order.getOrderId() <<
				"]: no parameters file");
	}
}



void
ExecutingOrder::downloadConfigurationSpaces ()
{
	#ifdef Cryosat
		#ifndef ENABLE_TL_CONFSPACE_DOWNLOAD
			#error Please, define -DENABLE_TL_CONFSPACE_DOWNLOAD in project makefile
		#endif
	#endif
	#ifdef ENABLE_TL_CONFSPACE_DOWNLOAD 
	_taskTable -> getConfigSpaceList( _configSpaces);

	for ( vector<ConfigSpaceDescriptor>::iterator i=_configSpaces.begin();
			i != _configSpaces.end();
			i++ )
	{
		string currentSpaceName = i->getSpaceName();

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "downloadConfigurationSpaces [" << _order.getOrderId() <<
				"]: downloading configuration space " << currentSpaceName);


		dbConfigurationSpace *cfg = dynamic_cast<dbConfigurationSpace *> (dbPersistentFactoryChain::instance()->newObject("dbConfigurationSpace", Application::instance()->getDefaultConnectionPool()->getKey()));

		try
		{

			dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
			dbConnection& conn (connWrp) ;
			cfg->setNamespace(currentSpaceName);
			cfg->load(conn) ;

			string currentVersion = cfg->getVersion();
			string currentFileName = genConfigurationFileName (currentSpaceName, currentVersion);
			i -> setPath (currentFileName);

			dbFileAction* downloader = dbFileActionFactoryChain::instance()->newDownloadAction(*(Application::instance()->getDefaultConnectionPool()),*cfg,currentFileName);
			if(!downloader)
			{
				exIllegalValueException e("ExecutingOrder::downloadConfigurationSpaces : Dynamic_cast failure.");
				ACS_THROW(e);
			}
			dbFileActionScopeGuard guard(downloader);
			downloader->start();


		}
		catch(exception &ex)
		{
			// delete objects
			delete cfg;
			// wrap exception
			ExecutingOrderException e(ex,"Failure downloading " + currentSpaceName + " namespace from database");
			ACS_THROW (e) ;
		}

		// delete objects
		delete cfg;

	}
	#endif
}

void 
ExecutingOrder::downloadAttachments()
{
	// Start downloading the attachments (if any)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "downloadAttachments: entering");

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "downloadAttachments: creating the connection");

	// Cleaer the internal attachment list
	_orderAttachments.clear();

	// First of all create a db connection and 
	// use it for the whole check/download phase
	dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
	dbConnection& conn (connWrp);
	const string &dbKey = conn.getKey() ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "downloadAttachments: creating the dbOrdersAttachmentQuery");

	// Let's get a query
	// and discover how many attachments are attached to the order 
	dbOrdersAttachmentQuery* attQuery (dynamic_cast<dbOrdersAttachmentQuery*>(dbQueryFactoryChain::instance()->newObject("dbOrdersAttachmentQuery", dbKey)));
	if (!attQuery)
	{
		// The query has not been instantiated 
		// that's another problem
		ACS_THROW(ExecutingOrderException("Unable to create an Order Attachments query")); // PRQA S 3081
	}
	dbGenQueryScopeGuard attQueryGuard(attQuery);
	dbQueryParameters attParameters(dbKey) ;

	// The parameter of the query is the orderId
	attParameters.addParameter(dbParam(dbKey, "t_ordersattachments","orderid", dbParam::EQUAL_TO, db::toSQL(dbKey,_order.getOrderId()))) ;
	attQuery->addParameter(attParameters) ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "downloadAttachments: executing the query");

	// Execute the query and get the attachments
	int num;
	try
	{
		num = attQuery->doQuery(conn);
	}
	catch (exception& ex)
	{
		// Cannot execute the query...
		// wrap the exception and then throw
		ACS_THROW(ExecutingOrderException(ex, "Unable to execute the orders attachment query")); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "downloadAttachments: Found " << num << " attachments for order #" << _order.getOrderId());

	for (int idx=0; idx < num; ++idx)
	{
		// managing the attachment #idx

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "downloadAttachments: Managing attachment #" << idx << " out of #" << num );

		bool null ;
		string attType = OrdersAttachmentType::instance(conn.getKey())->name((*attQuery)[idx]->getAttachType(null, conn)) ;

		// Store the attachment on disk
		// The name of the attachment will be returned by the download call
		string attName = (*attQuery)[idx]->download(_baseDir, conn);
		string attPath = genInterimFullFileName (attName);

		// Create the attachment descriptor
		JobOrderAttachmentDescriptor attDescriptor;
		attDescriptor.setType (attType);
		attDescriptor.setPath (attPath);

		// Save the descriptor in an internal structure
		// the job order will use it later
		_orderAttachments.push_back (attDescriptor);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"downloadAttachments: exiting");

}


void
ExecutingOrder::recordOrderInput() const
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB);

	unsigned long orderId = _order.getOrderId(); //not throwing

	rsResourceSet& rs = ConfigurationSingleton::instance() ->get();
	rsPushNSpace nspace_tld (rs, _tldRootKey);

	bool recordOrderInput(false);
	bool recordOrderInputIsCritical(false);

	rs.getValue( _recordOrderInputKey, recordOrderInput, false );
	rs.getValue( _recordOrderInputIsCriticalKey, recordOrderInputIsCritical, false );

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Recording Inputs requested:"<<recordOrderInput<<"; Is critical: "<<recordOrderInputIsCritical<<";" );

	if ( recordOrderInput )	{
		try {

			PCC_Base::SymTable downloadedSymTable;
			_downloader->getDownloadedSymbolTable(downloadedSymTable);

			dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
			dbConnection& conn (connWrp);
			const string &dbKey = conn.getKey() ;
			//Collect unique Inputs
			std::set< PCC_Base::InputFileName > inputs;
			for ( PCC_Base::SymTable::const_iterator it=downloadedSymTable.begin(); it != downloadedSymTable.end(); ++it ) {

				PCC_Base::SymTableParams const& params = it->second;

				//If Item passed the precondition check and was downloaded
				for ( vector<PCC_Base::InputFileName>::const_iterator jt = params.fileList.begin(); jt!=params.fileList.end(); ++jt ) {

					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
								ACS_LOG_DEBUG( SimpleDebugSignature ) ;
					ACS_LOG_DEBUG( "params.checkPassed: "<<params.checkPassed ) ;
					ACS_LOG_DEBUG( "params.toBeDownloaded: "<<params.toBeDownloaded ) ;
					ACS_LOG_DEBUG( jt->fileName ) ;
					ACS_LOG_DEBUG( jt->fileVersion ) ;
					ACS_CLASS_END_DEBUG

					if ( params.checkPassed && params.toBeDownloaded ) {
						inputs.insert( *jt );
					}
				}
			}

			ACS_LOG_INFO( "Recording ["<<inputs.size()<<"] Inputs for Order ["<<orderId<<"]" );

			//Record Order's Inputs
			for ( std::set< PCC_Base::InputFileName >::const_iterator it = inputs.begin(); it!=inputs.end(); ++it ) {
				PCC_Base::InputFileName const& input = *it;

				ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Recording Inputs for Order ["<<orderId<<"]: Input Item: ["<<input.fileName<<", "<<input.fileVersion<<"]." );

				//Fill t_ordersinput
				ACS_SMARTPTR<dbOrdersInput> dbOInput ( dynamic_cast<dbOrdersInput*>(dbPersistentFactoryChain::instance()->newObject("dbOrdersInput", dbKey)));
				dbOInput->setFileName( File::getFileName( input.fileName ) ) ;
				dbOInput->setFileVersion( input.fileVersion ) ;
				dbOInput->setOrderId( orderId ) ;
				dbOInput->save(conn);
			}

		} catch( exception& ex ) {
			ostringstream msg; msg<<"Problem occurred while recording the Inputs for Order ["<<orderId<<"]";
			if ( recordOrderInputIsCritical ) {
				ACS_THROW( ExecutingOrderException(ex, msg.str() ));
			} else {
				ACS_LOG_WARNING(msg.str());
			}
		}
	}
}


void
ExecutingOrder::manageCommand (TlMessage& theMessage)
{
	// Exceptions are handled outside from there....
	if (!_scheduler)
	{
		ACS_THROW (ExecutingOrderException("Cannot execute commands. Scheduler not spawned yet."));
	}
	if (theMessage.cmdType() == TlMessage::stopType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"manageCommand : calling scheduler->stop for orderId #" << _order.getOrderId());

		exitSuspendedStatus();
		_scheduler->stop();
	}
	else if (theMessage.cmdType() == TlMessage::abortType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"manageCommand : calling scheduler->abort for orderId #" << _order.getOrderId());

		exitSuspendedStatus();
		_scheduler->abort();
	}
	else if (theMessage.cmdType() == TlMessage::suspendType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "manageCommand : calling scheduler->suspend for orderId #" << _order.getOrderId());

		enterSuspendedStatus();
		_scheduler->suspend();

	}
	else if (theMessage.cmdType() == TlMessage::resumeType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"manageCommand : calling scheduler->resume for orderId #" << _order.getOrderId());

		exitSuspendedStatus();
		_scheduler->resume();

	}
	else if (theMessage.cmdType() == TlMessage::killType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"manageCommand : calling scheduler->kill for orderId #" << _order.getOrderId());

		exitSuspendedStatus();
		_scheduler->doKill();
	}
	else if (theMessage.cmdType() == TlMessage::userType)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "manageCommand : calling scheduler->sendCmd for orderId #" << _order.getOrderId());

		exitSuspendedStatus();
		_scheduler->sendCmd( theMessage.cmdValue(), theMessage.paramValue() );
	}
	else
	{
		ostringstream os;
		os << "Unknown command type [" << theMessage.cmdType() << "]";
		ACS_THROW (ExecutingOrderException(os.str()));
	}
}



void
ExecutingOrder::enterSuspendedStatus()
{
	if ( ( (ProcessorStatusMgrSingleton::instance()->getMgr() ) -> getStatus(_mySlot) ) != PR_SUSPENDED )
	{
		( ProcessorStatusMgrSingleton::instance()->getMgr() ) -> enterStatus (PR_SUSPENDED, _mySlot);

		if (!_parent.updateOrderStatus (*this, OrderStatusSUSPENDED, false))
		{
			ACS_LOG_WARNING("Cannot set order status to " << OrderStatus2String(OrderStatusSUSPENDED)
					<< " for order " << ((*this).operator tlOrder()).getOrderSignature());
			//	      excmcf << msg;
		}

	}
}

void
ExecutingOrder::exitSuspendedStatus()
{
	if ( ( (ProcessorStatusMgrSingleton::instance()->getMgr() ) -> getStatus(_mySlot) ) == PR_SUSPENDED )
	{
		( ProcessorStatusMgrSingleton::instance()->getMgr() ) -> exitStatus (PR_SUSPENDED, _mySlot);
		if (!_parent.updateOrderStatus (*this, OrderStatusPROCESSING, false))
		{
			ACS_LOG_WARNING("Cannot set order status to " << OrderStatus2String(OrderStatusPROCESSING)
					<< " for order " << ((*this).operator tlOrder()).getOrderSignature());
			//		    excmcf << msg;
		}
	}
}



string
ExecutingOrder::genLogFileName() const
{
	ostringstream os;
	os << genInterimFullFileName("LOG") << "." << _order.getOrderId();

	return os.str();
}


string
ExecutingOrder::genJobOrderFileName() const
{
	ostringstream os;
	os << genInterimFullFileName("JobOrder") << "." << _order.getOrderId() << ".xml";

	return os.str();
}


string
ExecutingOrder::genWorkOrderFileName() const
{
	ostringstream os;
	os << genInterimFullFileName("WorkOrder") << "." << _order.getOrderId() << ".xml";

	return os.str();
}

string
ExecutingOrder::genConfigurationFileName(const string& aNameSpace, const string& aVersion ) const
{
	ostringstream os;
	if (aVersion != "")
	{
		os << genInterimFullFileName(aNameSpace) << "_" << aVersion << ".xml";
	}
	else
	{
		os << genInterimFullFileName(aNameSpace) << "_" << "NoVersion" << ".xml";
	}

	return os.str();
}

string
ExecutingOrder::genPrivateConfFileName() const
{
	string privateConfig =_taskTable->getConfigFile(_order.getProcessorConfiguration());
	ostringstream os;
	if (privateConfig != "")
	{
		if (_copyPConf)
		{
			os << genInterimFullFileName(File::getFileName(privateConfig));
		}
		else
		{
			os << privateConfig;
		}
	}

	return os.str();
}

string
ExecutingOrder::genTrackerFileName() const
{
	ostringstream os;
	os << genInterimFullFileName ("Trkfile") << "." << _order.getOrderId() << ".trk";
	return os.str();
}

string
ExecutingOrder::genParametersFileName() const
{
	ostringstream os;
	// The parameters file is a clob in the database
	// there is no need to assume a format or filetype so no extensions is added
	os << genInterimFullFileName ("OrderParametersFile") << "." << _order.getOrderId();
	return os.str();
}



string
ExecutingOrder::genSchedulerLogName() const
{
	ostringstream os;
	os << Application::defaultLogPath << "/Scheduler_" << File::getFileName(genWorkOrderFileName()) << ".log";
	return os.str();
}

string
ExecutingOrder::genInterimFullFileName(const string& file_name) const
{
	// this routine assumes to have as input a file name according to the conventions
	// of the specific mission job order, and generates the full pathname to insert
	// in the joborder
	ostringstream os;
	os << _baseDir << "/" << file_name;

	return os.str();
}

_ACS_END_NAMESPACE



