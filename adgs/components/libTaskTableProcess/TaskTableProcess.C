// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: libTaskTableProcess $

	$Id$

	$Author $

	$Log$
	Revision 5.42  2017/09/20 16:22:18  chicas
	message modified
	
	Revision 5.41  2016/04/27 14:35:55  marpas
	the string to build working dir is obtained with a datetime, but ':', '-' chars are converted into '_'
	robustness improved - speed and compactness also
	
	Revision 5.40  2016/03/26 21:28:06  marpas
	using new db I/F
	
	Revision 5.39  2016/03/23 15:02:56  marfav
	S3PDGS-2809
	added the possibility to generate the log file in the default log repository
	
	Revision 5.38  2016/03/14 14:35:07  nicvac
	OPMAN-163
	
	Revision 5.37  2016/03/14 14:22:11  nicvac
	OPMAN-163: Task table refresh at start.
	
	Revision 5.36  2016/03/14 10:52:21  marpas
	log improved to check addresses with blanks
	
	Revision 5.35  2016/03/14 10:50:38  marpas
	mail notification log improved in case of failure
	
	Revision 5.34  2016/03/10 14:13:27  nicvac
	OPMAN-158: satallite, mission, sensor on import rules.
	
	Revision 5.33  2015/12/16 11:43:01  damdec
	APF-272: code aligned to new specification (ICD v1.1.0 draft)
	
	Revision 5.32  2015/11/26 15:06:40  marfav
	mc_params are now lowercase
	
	Revision 5.31  2015/08/20 13:52:41  marfav
	Added jobid to the running MC message
	
	Revision 5.30  2015/08/20 10:25:35  marfav
	Fixed jobId computation
	
	Revision 5.29  2015/08/19 09:32:29  marfav
	APF-272 messages added
	
	Revision 5.28  2015/06/17 13:36:01  marpas
	fixing potential raise (unprotected variable) condition
	coding best practice applied
	
	Revision 5.27  2015/05/25 15:46:02  lucio.pulvirenti
	S3PDGS-2382" read only product flag and method added to IF. If flag set, product permission set as read-only after moving to production dir.
	
	Revision 5.26  2015/04/30 13:05:19  marfav
	Aligned to libScheduler 2.30 signatures
	
	Revision 5.25  2015/03/18 08:46:46  lucio.pulvirenti
	S2PDGS-1129: _inputFilefullpath attribute added to interface along with the proper get method.
	
	Revision 5.24  2015/03/03 14:22:32  lucio.pulvirenti
	S2PDGS-1115: mutex to read configuration introduced. Use of string variables improved in createWorkingDir method.
	
	Revision 5.23  2014/12/09 14:36:54  marpas
	removing rs nspaces
	
	Revision 5.22  2014/05/28 17:50:14  marpas
	minor changes
	
	Revision 5.21  2014/03/21 14:45:19  chicas
	Messages improved
	
	Revision 5.20  2014/01/24 11:53:36  lucio.pulvirenti
	S2PDGS-289: Method to set production dir added: if production dir not set, built as before. File guard list removed since no more useful.
	
	Revision 5.19  2014/01/17 14:06:00  lucio.pulvirenti
	Hard link temporary substituted with rename (as before) waiting to get configuration parameter for link/rename in next version.
	
	Revision 5.18  2014/01/17 12:43:48  lucio.pulvirenti
	If lock exception caught and tasktable exist use the old one.
	Statistics added.
	
	Revision 5.17  2014/01/13 17:15:37  lucio.pulvirenti
	S2PDGS-289: renaming substituted with hard link for files from working dir to production dir and a file guard on source path is set. It allows to detach file guard if an error occurs
	so that circulation destination path is not removed and circulation must not be repeated. If all OK, file guard automatically remove working dir path in DTOR.
	
	Revision 5.16  2013/12/17 15:42:14  marpas
	no mutex when downloading - speed ups them
	downloading a too old tt is done on temporary file and then moved into the final destination.
	
	Revision 5.15  2013/12/17 12:08:55  marpas
	qa warnings
	
	Revision 5.14  2013/12/16 11:49:33  marpas
	throw exception fixed
	
	Revision 5.13  2013/12/16 11:47:34  marpas
	exception throw fixed
	
	Revision 5.12  2013/11/12 17:38:42  marfav
	Using unique names for working directories
	Scheduler log placed in the working directory instead of default log repository
	
	Revision 5.11  2013/11/06 18:20:11  marpas
	qa warnings
	
	Revision 5.10  2013/11/06 11:56:42  marpas
	identifier is now a dbPersistent::IdType
	
	Revision 5.9  2013/09/19 09:56:31  chicas
	Removed exception notify inside prepareOrder (managed outside)
	
	Revision 5.8  2013/07/30 17:09:25  chicas
	scheduler waiting loop relaxed to 2000 msecs
	
	Revision 5.7  2013/07/26 14:57:14  marfav
	Using synchronous call to monitor scheduler execution
	
	Revision 5.6  2013/07/10 07:56:26  chicas
	modified max age of task table file from 600 to 1200
	
	Revision 5.5  2013/07/08 14:33:10  chicas
	Bug fixed: using File::lock when downloading/reading TaskTable
	
	Revision 5.4  2013/07/07 19:16:10  marpas
	k table can be downloaded in a temporary filename
	adoption of libException 5.x standards
	qa and compilation warnings removed
	
	Revision 5.3  2013/06/20 09:14:56  chicas
	Messages improved
	
	Revision 5.2  2013/06/20 08:17:47  chicas
	minor changes in log
	
	Revision 5.1  2013/06/19 13:00:08  chicas
	bug fixed adding _ttMutex
	
	Revision 5.0  2013/06/19 08:41:07  chicas
	*** empty log message ***
	
	Revision 1.22  2013/06/19 08:11:14  chicas
	replaced cp and mv system call with File::copy and File::rename
	
	Revision 1.21  2013/06/14 13:38:25  chicas
	added NAMESPACE macro
	
	Revision 1.20  2013/06/14 13:20:22  chicas
	new debugging and logging macros adopted
	
	Revision 1.19  2013/06/14 11:18:03  chicas
	added try-catch for mail notification
	
	Revision 1.18  2013/06/12 10:35:33  nicvac
	S2PDGS-298: Notifications on success.
	
	Revision 1.17  2013/04/29 08:54:54  chicas
	 useless include rmoeved; setExt added
	
	Revision 1.16  2013/04/03 13:44:23  marfav
	Rolling back to previous thread::run()
	
	Revision 1.15  2013/04/03 12:44:10  marfav
	thread::run takes a void * as argument
	
	Revision 1.14  2013/03/08 14:11:22  chicas
	Messages improved
	
	Revision 1.13  2013/03/07 08:46:09  chicas
	setMainConfSpace instead of setApplicationName
	
	Revision 1.12  2013/02/28 14:09:35  chicas
	Added identifier and downloadTaskTable static functions
	
	Revision 1.11  2013/02/25 13:32:46  chicas
	Modified createWorkingDir: searching and creating a not already existing dir
	Note that basedir passed to constructor now is common production dir
	
	Revision 1.10  2013/02/13 15:47:31  chicas
	setProcName setProcVersion removed (always taken form tasktable); input file with empty filetype moved in working dir but NOT added to JobOrder
	
	Revision 1.9  2013/01/10 15:59:48  chicas
	simplified buildJobOrder : list all passed inputs , no read task table ; WARNING buildJobOrder interface changed
	
	Revision 1.8  2013/01/09 13:05:33  chicas
	Minor changes
	
	Revision 1.7  2013/01/09 13:01:57  chicas
	Move files in backupDir in case of failure
	
	Revision 1.6  2013/01/07 10:27:21  chicas
	setApplicationName instead of setRootKey
	
	Revision 1.5  2012/12/11 15:05:18  chicas
	Configurable Root Namespace; added sendMail in case of failure; message improved
	
	Revision 1.4  2012/11/30 12:42:03  chicas
	work in progress
	
	Revision 1.3  2012/11/29 17:15:46  chicas
	work in progress
	
	Revision 1.2  2012/11/27 17:24:47  chicas
	working for static buildJobOrder
	
	Revision 1.1.1.1  2012/11/26 14:45:46  chicas
	Imported sources
	
	
	
*/
#include <TaskTableProcess.h>

#include <TTProcessEmailNotification.h>
#include <UserNotifier.h>
#include <SpawnedScheduler.h>
#include <TaskTableFactoryChain.h>
#include <JobOrderFactoryChain.h>
#include <StringKey.h>
#include <TaskTable.h>
#include <scOrder.h>
#include <MCFilterables.h>
#include <Timer.h>
#include <dbConnection.h>
#include <dbSmartPersistent.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <DefaultJobOrder.h>
#include <Statistics.h>
#include <lTTProcV.h>
#include <FileGuard.h>


#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
using namespace acs::mc;

using acs::math::StatNode ;

namespace {
    lTTProcV version;

    math::StatNode &statN() {
        return (*acs::math::Statistics::instance())()["TTProcess"];
    }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableProcess)

// Helper class
// Will guard the value of "_valid" flag during the run of the thread

class IsValidGuard // PRQA S 2109
{
	public:
		explicit IsValidGuard (bool& flag) : _flag(flag) { _flag = true; } // PRQA S 2528
		~IsValidGuard () throw() { _flag = false; }
    private:
        IsValidGuard() ; // not implemented
        IsValidGuard(const IsValidGuard &) ; // not implemented
        IsValidGuard& operator= (const IsValidGuard &) ; // not implemented
	private:
		bool& _flag;
};


namespace {
    string _procNameToken = "${PROCNAME}";
    string _procVersionToken = "${PROCVERSION}";
    string _satNameToken = "${SATNAME}";
    string _missionToken = "${MISSION}";
    string _taskNameToken = "${TASKNAME}";
} 

class ParamsAccumulator // PRQA S 2109
{
public:
    ParamsAccumulator (vector<string>& v, const Task& t , const string& pn, const string& pv, const string& sat, const string& mis) :
        _v(v), // PRQA S 2528 2
        _task(t),
        _pName(pn),
        _pVersion(pv),
        _satName(sat),
        _missNumber(mis)
    {
    }
    ParamsAccumulator(const ParamsAccumulator &r) :
        _v(r._v), // PRQA S 2528 2
        _task(r._task),
        _pName(r._pName),
        _pVersion(r._pVersion),
        _satName(r._satName),
        _missNumber(r._missNumber)
    {
    }
    
    ~ParamsAccumulator() throw() {  }

    bool replaceToken (string& value, const string& token, const string& newval, bool& invalidReplace, ostringstream& os)
    {
        bool retval = false;

        if (value.find (token) != string::npos) 
        {
            retval = true;
            if (newval != "") {
                  boost::replace_first(value, token, newval); 
            }
            else
            {
                invalidReplace = true;
                if (os.tellp() == ostringstream::pos_type(0) ) { // PRQA S 3081
                       os << "Unable to replace te following tokens: " << token; 
                }
                else {
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
        bool isNameSet = false ;
        string name = d.getName (&isNameSet);
        string value = d.getValue();
        ostringstream message;
        bool replaceFailed = false;

        if (replaceToken (value, _procNameToken, _pName, replaceFailed, message))
        { // Just for code readability // ProcessorName Replaced 
        }
        else if (replaceToken (value, _procVersionToken, _pVersion, replaceFailed, message))
        { // Just for code readability // ProcessorVersion Replaced
        }
        else if (replaceToken (value, _satNameToken, _satName, replaceFailed, message))
        { // Just for code readability // SatelliteName Replaced }
        }
        else if (replaceToken (value, _missionToken, _missNumber, replaceFailed, message))
        { // Just for code readability // MissionNumber Replaced }
        }
        else if (replaceToken (value, _taskNameToken, _task.getName(), replaceFailed, message))
        { // Just for code readability // TaskName Replaced
        }
        else { /* nothing */ }

        if (replaceFailed) {
            ACS_COND_THROW (d.isMandatory(), TaskTableProcess::TaskTableProcessException (message.str())); // PRQA S 3081
        }
        else
        {
            if (isNameSet) {
                _v.push_back(name);
            }
            _v.push_back(value);
        }
    }
private:
    ParamsAccumulator() ; // not implemented
    ParamsAccumulator& operator = (const ParamsAccumulator &) ; // not implemented
private:
    vector<string>& _v;
    const Task& _task;
    string _pName;
    string _pVersion;
    string _satName;
    string _missNumber;
};

class ParametersTaskParser // PRQA S 2109
{
public:
    ParametersTaskParser (scOrder& wo, const string& pname, const string& pversion, const string& sat, const string& miss) :
        _wo(wo), // PRQA S 2528
        _pName(pname),
        _pVersion(pversion),
        _satName(sat),
        _missNumber(miss)
    {
    }
    ParametersTaskParser(const ParametersTaskParser &r) :
       _wo(r._wo), // PRQA S 2528
        _pName(r._pName),
        _pVersion(r._pVersion),
        _satName(r._satName),
        _missNumber(r._missNumber)
    {}
    
    ~ParametersTaskParser() throw() {  }

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
    ParametersTaskParser() ; // not implemented
    ParametersTaskParser &operator= (const ParametersTaskParser &) ; // not implemented
   
private:
    scOrder& _wo;
    string _pName;
    string _pVersion;
    string _satName;
    string _missNumber;
};

// STL HELPER CLASSES - END

ThreadSafe TaskTableProcess::_wdirMutex;

TaskTableProcess::TaskTableProcess( const TaskTableProcessConfig::Sptr& configSptr,
	                                const string& fullpathfile,
                                    const string& filetype,
                                    const string& aTaskTable,
                                    const string& workingDir ):
    Thread(),
    _configSptr( configSptr ),
    _maxTime(0),
    _baseDir(workingDir),             // common production dir 
    _taskTable(0),
    _scheduler(0),
    _valid(true),
    _extension("TP"),
    _parametersFileName(""),
    _filetypeVsInputFile(),
    _inputFile(),
    _fileType(filetype),
    _thrashDir(),
    _joParams(),
    _orderType(),
	_successAddress(),
    _failureAddress(),
    _schedulerExitStatus(ssNOTSTARTED)  ,
    //_processorName(), _processorVersion(),
    _satelliteName(), _missionNumber(), //optionally set
    _satelliteId(), _sensorId(), //optionally set
    _identifier(0),//optionally set
    _setExt(false),
	_productionDir(),
	_inputFilefullpath(),
	_readOnlyProduct(false),
	_taskLogInDefaultLogDir(false),
	_wdsInErrorBasket(),
	_basketMutex(),
	_wdMovedInErrorBasket(false)
{
	_inputFilefullpath = fullpathfile; // if the caller is a thread can be never modified
	_inputFile = File::getFileName(_inputFilefullpath);
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Processing " << _inputFilefullpath);

    if (! File::exists(_baseDir) ) {
        ACS_THROW(TaskTableProcessException("Passed production dir " + _baseDir+ " not found. Create it.")); // PRQA S 3081
    }

    _filetypeVsInputFile.insert ( make_pair(_fileType, _inputFilefullpath) );
  
    _taskTable = new TaskTable(); // TaskTableFactoryChain::instance()->newObject( key ); // this can throw!
	string ttPath = aTaskTable; // if the caller is a thread can be never modified
    _taskTable->readFromFile(ttPath);
    /*/
    _taskTable = new TaskTable(aTaskTable); // this can throw!
    */
    _maxTime   = _taskTable->getMaxAllottedTime();

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "production dir  " << _baseDir  << " max time " << _maxTime);

}

TaskTableProcess::~TaskTableProcess() throw() 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Destructor called .... "); // PRQA S 4631

    if (_taskTable) { delete _taskTable; }
    if (_scheduler) { delete _scheduler; } 
	
}
  
void TaskTableProcess::addInputFile(const string &filename, const string &filetype)
{
    _filetypeVsInputFile.insert(make_pair(filetype, filename) );
}


bool TaskTableProcess::isValid() const throw() // PRQA S 4120
{
  return _valid;
}

SpawnedScheduler * TaskTableProcess::getScheduler()
{

  if (!_scheduler)
  {
    const string& schedulerPath = _configSptr->getSchedulerPath();

    ACS_COND_THROW( schedulerPath.empty(), TaskTableProcessException("Configuration not set for Scheduler Path") );

    _scheduler = new SpawnedScheduler (schedulerPath, genWorkOrderFileName());
  }

  return _scheduler;
}


void
TaskTableProcess::prepareOrder()
{
    try
    {
	//Check if stopped or aborted
	//if( requestedCancel() ) return ; 

		//HaltLevel haltLevel = this->getHaltLevel();

		//if( haltLevel == StopController::LAZY) {
        //createWorkingDir
        createWorkingDir();
        
        //NEXT..
        //downloadConfigurationSpaces in work dir; //  from TT
        //move ConfigFile in work dir (if any)  -> from TT 

        //create a parameter file if needed
        downloadParametersFile();
       
        // copy ALL input files in the working directory 
        multimap<string,string>::iterator it; 
        for ( it = _filetypeVsInputFile.begin(); it !=  _filetypeVsInputFile.end(); it++)
        { 
            string inputpathfile = (*it).second;

            if (File::exists(inputpathfile) )
            {
                string inputfile= File::getFileName(inputpathfile);
				string destinationPath = genInterimFullFileName(_baseDir, inputfile);
                if ( _configSptr->isCopyInputData() )  //debug purpose
                {    File::copy(inputpathfile, destinationPath); }
                else
                { 
					File::rename(inputpathfile, destinationPath) ;
					ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "\"" << inputpathfile << "\" file renamed as \"" << destinationPath << '\"');
				}
				
				if( _readOnlyProduct ) {
					mode_t mode = File::getmod(destinationPath);
					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB,'\"' << destinationPath << "\" original permissions are " << oct << mode);
					if( (mode & 0222) != 0 ) { // product is not read-only // PRQA S 4400 2
						mode &= 0777555;
						try {
							File::chmod(destinationPath, mode);
							ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB,'\"' << inputfile << "\" file in \"" << _baseDir << "\" path set as read-only: " << oct << mode );
						}
						catch(exception &e)
						{
							ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, e.what());
							ACS_LOG_WARNING("[" <<  threadId() <<  "]: cannot set \"" << inputfile << "\" file in \"" << _baseDir << "\" path permissions to read-only (" <<  oct << mode << ')' );
						}
					}
				}
				
            }
            else
            {	
                // error
                ACS_LOG_ERROR( "[" << threadId() << "]:: input file " << inputpathfile<< " does not exist!");
                ACS_THROW (TaskTableProcessException("Cannot copy " + inputpathfile+ " in " +_baseDir, errno));
            }
        }
    }
	catch (const exception& toCatch)
	{
        _valid = false; //this thread can be purged..
		// we are here inside the thread
		// notify the exception to be sure it shows up at the proper time
		// then rethrow it to force exit to the parent process  

	    //ACS_LOG_NOTIFY_EX(toCatch);

        //move file[s] in a backup dir  
        if ( !_thrashDir.empty() )     
        {
            multimap<string,string>::iterator it; 
            for ( it = _filetypeVsInputFile.begin(); it != _filetypeVsInputFile.end(); it++)
            { 
                string inputpathfile = (*it).second;
                try 
                {
                    if (File::exists(inputpathfile) )
                    {
                        string inputfile= File::getFileName(inputpathfile);
                        
                        ACS_LOG_INFO("[" << threadId()<<  "]: cleaning dir moving " << inputpathfile << " to " << _thrashDir);

                        File::unlink(_thrashDir + "/" + inputfile);
						
						File::rename(inputpathfile, _thrashDir + "/" + inputfile);	
						
                    }
                }
                catch (exception & e)
                {
                    ACS_LOG_WARNING("[" <<  threadId() <<  "]: Failed  removing  "<< inputpathfile << " from working dir..  "  );
                    ACS_LOG_NOTIFY_EX(e);
                }
            }
        }

        ////
        throw;
	}

}

const int polling_freq=1000; // 1 second in millisec

void
TaskTableProcess::run()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File " << _inputFile  << ": processing started ; threadId is " << threadId() );
	
	acs::math::StatNode& SN = statN()[File::getFileName(_taskTable->getPath())] ;
    SN["shlock"] += 1;
    Timer tl ;
    tl.start() ;

    //a shared lock over tasktable file
    File::Lock lock(_taskTable->getPath()) ;
    File::Lock::tryLock(lock,10,1000,true) ; // will try 10 times with 1 second timeout, then File::Lock::IOException // PRQA S 4400 
    SN["shlock acq [ms]"] += double(tl.elapsed()) ; // PRQA S 3081
    Timer tld ;
    tld.start() ;

	// Identify and use the working directory as the jobId
	// Removing any slash from the baseDir tail
	string jobId("N/A");

	try {
    	try
    	{

        	IsValidGuard theGuard (_valid);

        	prepareOrder();

			// Now the _baseDir has been updated to include the filetype and a timestamp
			jobId = File::getFileName ( StringUtils::pathNormalize(_baseDir) );

#ifndef LTA // removed not to send huge amount of messages in the framework of LTA project
			
			//APF-272
			//Emit the message for the new file under management
			ACS_LOG_MC_INFO( 
			    mc_param("jobid", jobId) << 
			    mc_param("filename", File::getFileName (_inputFile) ) << 
			    mc_metric (Metric_RUNNING) ) ;
#endif

        	// job order 
        	//call static instance..
        	buildJobOrder( 
                	_filetypeVsInputFile,
                	_baseDir, 
                	genJobOrderFileName(false), //no full path
                	_parametersFileName,
                	_orderType,
                	_satelliteName,
                	_missionNumber,
                	_satelliteId,
                	_sensorId);

        	//work order
        	scOrder theWorkOrder = buildWorkOrder();

        	try
        	{
            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << threadId() << "]: spawning scheduler " << genWorkOrderFileName() );

            	getScheduler()->execute(theWorkOrder);

            	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << threadId() << "]: scheduler pid=" << _scheduler->getSchedulerPid() );
        	}
        	catch(SpawnedScheduler::SpawnedSchedulerException &)
        	{
            	ACS_LOG_ERROR( "[" << threadId() << "]: Cannot spawn scheduler(" << genWorkOrderFileName() << ")");

            	throw;
        	}
        	// Have Just spawned a new file to be processed 

        	// create timer
        	Timer tmax;
        	if (0 != _maxTime) {
        	  tmax.start();
        	}

	    	bool stopped = false ;

			string exitStatus;

        	// periodically poll for process completion or timer expiration
        	for (;;)
        	{
            	// perform a check every 100 msecs and exit immediately if scheduler exits
            	getScheduler()->timedWaitExit (2000); // PRQA S 4400

            	// check if timeout expired
            	if ( (0 != _maxTime) && !stopped )
            	{
                	tmax.stop(); // makes the timer read current time (otherwise elapsed won't work)
                	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "[" << threadId() << "]: total expired time " << tmax.elapsed() << "[ ms]"); 


                	if (tmax.elapsed() >= Timer::Delay(_maxTime*1000)) // PRQA S 3081, 4400
                	{

                    	ACS_LOG_INFO( "[" << threadId() << "]: timeout timer expired - killing the process");

                    	getScheduler()->sendKillSignal();
                    	stopped = true;		

                    	// give the process time to exit
                    	// Timer::delay(1000); // 1 second // PRQA S 4400
                	}
            	} //max time


            	// check if process completed and how
            	if (!getScheduler()->isSchedulerRunning())
            	{
                	//suspended ??

                	SchedulerExitStatus ec; // PRQA S 4101
                	if (getScheduler()->getExitStatus(ec))
                	{
                    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Process terminated with exit code " << int(ec)); // PRQA S 3081
                    	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, notifyExitStatus(ec));

                    	_schedulerExitStatus = ec;
                	} 
                	else 
                	{	
                    	ACS_LOG_ERROR("[" << threadId() << "]:  Process terminated but no valid exit code is available"); 
                    	 //if (_parent)
                    	//    _parent->notifySchedulerCompletion(*this, SchedulerExitStatus(ssTASK_CRASHED) );

                    	 _schedulerExitStatus = SchedulerExitStatus(ssTASK_CRASHED); // PRQA S 3081
                	}

                	exitStatus = schedulerExitStatus2String(_schedulerExitStatus);
                	// quit the loop and exit
                	break;

            	}// no running

        	}//loop   

        	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, " Terminated.  Exit status = " << exitStatus);

			if ( _schedulerExitStatus != ssOK )
			{
			// APF-272 
			// Notify failure of the import chain
				ACS_LOG_MC_INFO (
					mc_param("jobid", jobId) <<	mc_param("filename", File::getFileName(_inputFile)) << 
					mc_param("message", "Import processing chain failed") << 
					mc_metric (Metric_FAIL)
				);
				
				moveWdOnError();
				
			}

            string address ;
        	try {

            	TTProcessEmailNotificationSptr notification( new TTProcessEmailNotification );
            	if ( (_schedulerExitStatus != ssOK) && (!_failureAddress.empty())) {
                    address = _failureAddress ;
                	notification->setInfo(_inputFile, _failureAddress, false).setDescription("Importer failure for "+_inputFile);
                	UserNotifier::instance()->sendAsynch( notification ); //Return immediately - asynch call

            	} else if ( (_schedulerExitStatus == ssOK) && (!_successAddress.empty()) ) {

                    address = _successAddress ;
                	notification->setInfo(_inputFile, _successAddress, true).setDescription("Importer success for "+_inputFile);
                	UserNotifier::instance()->sendAsynch( notification ); //Return immediately - asynch call
            	}
            	else { /* nothing */ }
        	}
        	catch (const exception& e)
        	{
            	ACS_LOG_WARNING("[" <<  threadId() <<  "] Mail notification failed to address: \"" << address << "\" " << e.what());
        	}


    	} catch (const exception& toCatch) {

        	ACS_LOG_ERROR("[" <<  threadId() <<  "] Terminated for exception  ");
        	ACS_LOG_NOTIFY_EX(toCatch);
			
            string msg = string("Execution terminated by a framework exception: ") + toCatch.what(); // PRQA S 3081
			// APF-272 
			// Notify failure for exception
			ACS_LOG_MC_INFO (
				mc_param("jobid", jobId) <<	mc_param("filename", File::getFileName(_inputFile)) << 
				mc_param("message", msg) << 
				mc_metric (Metric_FAIL)
			);
			
			moveWdOnError();

        	// Make notification of the thread setup failure
        	_schedulerExitStatus = ssTASK_FAILED;

    	   try {
            	if ( !_failureAddress.empty()) 
            	{
                	TTProcessEmailNotificationSptr notification( new TTProcessEmailNotification );
                	notification->setInfo(_inputFile, _failureAddress, false).setDescription("Importer failure for "+_inputFile);

                	UserNotifier::instance()->sendAsynch( notification ); //Return immediately - asynch call
            	}
        	}
        	catch (const exception& e)
        	{
            	ACS_LOG_WARNING( "[" <<  threadId() <<  "]: Mail notification failed");
        	}

        	throw;  
    	}
	}
    catch(exception &) {
        SN["shlock dur [ms]"] += double(tld.elapsed()) ; // PRQA S 3081
        throw ; 
    }
    SN["shlock dur [ms]"] += double(tld.elapsed()) ; // PRQA S 3081

}


void TaskTableProcess::createWorkingDir()
{
	{
    	Lock guard(_wdirMutex); 

		map<char,char> tr ;
		tr[':'] = '_' ;
		tr['-'] = '_' ;
		string timeSignature = StringUtils::transform(DateTime().asISO8601(6), tr) ;
    	string wdbase ;
		if( _productionDir.empty() ) {
    		wdbase = _baseDir + "/" + _fileType + "_" + timeSignature;
		}
		else {
    		wdbase = _baseDir + "/" + _productionDir;
		}

    	size_t n = 1;
    	ostringstream wdir ;
		wdir << wdbase << "_" << n;
		string workingdir = wdir.str();

    	while ( File::exists(workingdir) )
    	{   
        	ostringstream wdir1;
        	n++;
        	wdir1<< wdbase << "_" << n;
        	workingdir = wdir1.str();
    	}          

    	//update baseDir value
    	_baseDir = workingdir;

    	if (!_setExt) 
    	{
        	ostringstream ext;
        	ext << _fileType << "_" << timeSignature << "_"  << n;
        	_extension = ext.str(); 
    	}
    	if (mkdir(_baseDir.c_str(), 0775) != 0) // PRQA S 4400
    	{
        	ACS_THROW(TaskTableProcessException("Cannot create working dir " + _baseDir, errno));
    	}

    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[" << threadId()<<  "]:  Work dir is " << _baseDir);

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Copy input data: " << _configSptr->isCopyInputData() );

	}

}



scOrder TaskTableProcess::buildWorkOrder() // PRQA S 4211
{
	scOrder wo;

	// useDefaultLogPath for the log ?
	wo.setLogFileName(genLogFileName());

	wo.setSchedulerLogName (_baseDir + "/SchedulerLogFile.log");
	wo.setProcName(_taskTable->getProcName());
	wo.setProcVersion(_taskTable->getProcVersion());
	wo.setTaskTable(_taskTable->getPath());
	wo.setJobOrder(genJobOrderFileName());
	wo.setBaseDir(_baseDir);
	wo.doICreateDir(false);
	wo.doIKillBackgroundTasks(true);
    if (! _satelliteName.empty() ) {
	    wo.setSatellite (_satelliteName);
    }
    if (! _missionNumber.empty() ) {
        wo.setMission (_missionNumber);
    }

	wo.setTrackerFileName (genTrackerFileName());

	// let the scheduler cleanup the directory only for non-troubleshooting orders
	// (note that if order execution fails, the sheduler won't cleanup anyhow)
	wo.doICleanupOnExit( _configSptr->isCleanWorkingDir() );

	// scan the task table searching for task's command line parameters
	vector<Task> theTaskList;

	// cleanup the vector and do recursive search
	_taskTable->getTaskList (theTaskList, true, true);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "found " << theTaskList.size() << " tasks in the Task Table");

	// Accumulate the parameters in the workorder
	for_each (theTaskList.begin(), theTaskList.end(), 
			  ParametersTaskParser(wo, _taskTable->getProcName(), _taskTable->getProcVersion(), _satelliteName, _missionNumber));

	// Finally set the mandatory output regexp association map
	//wo.setRegExpAssociationMap (_mandatoryOutputRegExpMap);

    return wo;
}


void TaskTableProcess::downloadTaskTablesOfType( const string& ttType, const string & ttdir, dbConnection & conn ) {


	const string& dbKey = conn.getKey();
	try {

        SQLString sqlString(dbKey, SQLString::SELECT ) ;
    	sqlString.addTableList("t_tasktables");
    	sqlString.addSelectList("id_tasktable");

		// Add Join
		{
			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "t_operationtypes","id_operationtype" , dbParam::EQUAL_TO , "t_tasktables.type_id" )) ;
			sqlString.addJoinList(SQLDialect::INNER,"t_operationtypes",pp) ;
		}
		{
			dbQueryParameters qp(dbKey) ;
			qp.addParameter(dbParam(dbKey, "t_operationtypes", "type",  dbParam::EQUAL_TO, db::toSQL(dbKey, ttType))); // PRQA S 3050, 3081

			sqlString.addANDWhereParameter(qp);
		}

        dbQuery query(conn, sqlString, "TaskTableIdSearch");
        dbSet ttSet;
        query.execForSet( ttSet );

        if ( ttSet.empty() ) {
        	ACS_LOG_WARNING( "Cannot found in DB any Task Table with type ["+ttType+"]. Task tables not refreshed. Using the cached ones in ["+ttdir+"]" );
        }

        vector< dbPersistent::IdType > ttIds;
        dbRow row(dbKey) ;
        while ( ttSet.getNextRow( row ) ) {

        	dbPersistent::IdType ttId = 0;
        	row.column(0, ttId);

        	ttIds.push_back( ttId );
        }

        //Download task tables
        for ( vector< dbPersistent::IdType >::iterator it = ttIds.begin(); it!=ttIds.end(); ++it ) { // PRQA S 4238
        	TaskTableProcess::downloadTaskTable( *it, ttdir, conn, true);
        }

    } catch (exception & ee) {
        ACS_LOG_ERROR( "Error getting Task Tables of type ["+ttType+"]. " );

        ACS_LOG_NOTIFY_EX(ee);
        throw ;
    }

}

string  TaskTableProcess::downloadTaskTable(const string & ttname, const string & ttdir, dbConnection & conn)
{
    string ttfullpath; 
    
    int ttId = 0; 
    const string & dbKey = conn.getKey() ;
    try {
        SQLString sqlString(dbKey, SQLString::SELECT ) ;
    	sqlString.addTableList("t_tasktables");
    	sqlString.addSelectList("id_tasktable"); 
        {
		    dbQueryParameters qp(dbKey) ;
		    qp.addParameter(dbParam(dbKey, "t_tasktables", "name",  dbParam::EQUAL_TO, db::toSQL(dbKey, ttname))); // PRQA S 3050, 3081
            
            sqlString.addANDWhereParameter(qp);
        }
        
        dbQuery query(conn, sqlString, "TaskTableIdSearch");
        dbRow row(dbKey) ;

		// here the query could raise an exception if no data found
        dbSet set ;
		query.execForSet(set) ;
        ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
        ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
        set.getNextRow(row) ; // first and only 
        row.column(0, ttId) ;
    }
    catch (exception & ee) 
    {
        ACS_LOG_ERROR( "Task Table NOT FOUND for name " + ttname );
        
        ACS_LOG_NOTIFY_EX(ee);
        throw ;
    }

    ttfullpath = downloadTaskTable(ttId,ttdir,conn); // PRQA S 3000

    return ttfullpath;

}

string TaskTableProcess::downloadTaskTable(dbPersistent::IdType id, const string & ttdir, dbConnection & conn, bool forceDownload)
{
    //_tasktableDir
    ostringstream ttfilename;
    ttfilename<< "TaskTable_" << id;
    //search inside tasktable dir otherwise download
    string ttfullpath = ttdir+ "/" + ttfilename.str(); 
    acs::math::StatNode& SN = statN()[ttfilename.str()] ;
    SN += 1 ;

    try {

        if ( !File::exists(ttfullpath) || File::isOlder(ttfullpath, 1200) || forceDownload ) // PRQA S 4400
        { 
            Timer tl ;
            tl.start() ;
            //exclusive lock
            File::Lock lock(ttfullpath) ;
		    File::Lock::tryLock(lock) ; // will try 10 times with 1 second timeout, then File::Lock::IOException
			
			tl.stop() ;
            SN["exlock acq [ms]"] += double(tl.elapsed()) ; // PRQA S 3081
            Timer tld ;
            tld.start() ;

            //recheck after lock
            if ( !File::exists(ttfullpath) || File::isOlder(ttfullpath, 1200) || forceDownload ) // PRQA S 4400
            {
                //download tt
                dbSmartPersistent ttItem("t_tasktables" , conn);
                ttItem.setId(id); // PRQA S 3000
                ttItem.load(conn);
                
                if (File::exists(ttfullpath)) { // already present 
                    string tmptt = File::tempname(ttfullpath, "tt") ; // PRQA S 3050
                    ttItem.downloadFile("tasktable", tmptt, conn );
                    File::rename(tmptt, ttfullpath) ;
                }
                else { 
                    ttItem.downloadFile("tasktable", ttfullpath, conn );
                }
       
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TaskTable  " <<   ttfullpath << " downloaded" ); 
            }  
            else
            {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TaskTable  " <<   ttfullpath << "  already found" ); 
            }
			SN["exlock dur [ms]"] += double(tld.elapsed()) ; // PRQA S 3081
        }
        else
        {
            SN["found"] += 1 ; 
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "TaskTable  " <<   ttfullpath << "  already found" ); 
        }
    }
    catch (File::Lock::LockException const& ee) 
    {
        if ( !File::exists(ttfullpath)) { 
            // just to avoid being in the very short interval another process is creating, moving it
            Timer::delay(10) ; // PRQA S 4400
            if ( !File::exists(ttfullpath)) { // still missing
                ACS_LOG_ERROR("Task Table Download failed for id number " << id );
                SN["not found"] += 1 ;
                throw ;
            }
        }
        SN["old ret"] += 1 ;
        ACS_LOG_PRIVINFO("Proceeding with already recently downloaded task table: " <<  ttfullpath << " for id number " << id);
    }
    catch (exception const& ee) 
    {
        ACS_LOG_ERROR("Task Table Download failed for id number " << id );
        SN["failed"] += 1 ;
        throw ;
    }

    return ttfullpath;
}


void TaskTableProcess::buildJobOrder( multimap<string,string> const & filetypeVsInputFile,
                                      const string& productiondir, 
                                      const string& jobordername,
                                      const string& parametersFileName,
                                      const string& orderType,
                                      const string& satelliteName,
                                      const string& missionNumber,
                                      const string& satelliteId,
                                      const string& sensorId)
{
	try
	{
        ACS_COND_THROW (filetypeVsInputFile.empty(), TaskTableProcessException ("Cannot build job order with empty input file map." ) ); // PRQA S 3081


        DefaultJobOrder jo;

        jo.setProcName        ("GLOBAL_PROC");
		jo.brkEnabled         (false );
		jo.isTroubleshooting  (false); 
        if (! parametersFileName.empty() ) {
            jo.setParametersFileName (parametersFileName);
        }
		jo.setWorkingDir    (productiondir);
	    if ( ! orderType.empty() ) {
            jo.setOrderType(orderType); 
        }
        if (! satelliteName.empty() ) {
    	    jo.setSatellite (satelliteName);
        }
        if (! missionNumber.empty() ) {
            jo.setMission (missionNumber);
        }
        if ( ! satelliteId.empty() ) {
        	jo.setSatelliteId( satelliteId );
        }
        if ( ! sensorId.empty() ) {
        	jo.setSensorId( sensorId );
        }

        ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)

        
//------------------------ Task I/O ----------------------------------
        {
			Task t;

			// 1 - copy general info
			t.setName("INPUT_TASK");//ptask->getName());
            // - copy input file inside input descriptor
      	    {
                //note: it 's better to have one alternative for each input descritpor 
                //to make coincide the number of input tag with the declare done
                // (see write function  of defaultjoborder)
                //
                /* create one input descriptor for each filetype (with its list of files)*/
                multimap<string,string>::const_iterator it;
                for(  it = filetypeVsInputFile.begin() ; it != filetypeVsInputFile.end();
                      it = filetypeVsInputFile.upper_bound(it->first) )
                {
                    string filetype = it->first; 

                    //empty filetype should no be inserted inside job order (for instance  DBL files..)
                    if ( filetype.empty() ) { continue;  }

                    InputDescriptor newInput;
                    InputAlternative altern;
                    //necessary for preinventory plugin
                    altern.setFileType( filetype); 
                    altern.setFileNameType( FileNameTypePHYSICAL); 

                    //get all files of same filetype
                    multimap<string,string>::const_iterator itft;
                    pair < multimap<string,string>::const_iterator, multimap<string,string>::const_iterator> ret;

                    ret = filetypeVsInputFile.equal_range(filetype); 
                    for (itft = ret.first; itft != ret.second; ++itft)
                    {
                        string inputpathfile=  (*itft).second;
                        string inputfile= File::getFileName(inputpathfile);
                        altern.addPathName(genInterimFullFileName(productiondir, inputfile ));

                        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding input file " << inputfile); 
                    }
                    newInput.push_back(altern);

                    t.addInput(newInput);

                } //new input descriptor 
            }       

            jo.push_back(t);
        }
//------------------------------------------------------------------

		// As last step of the JobOrder configuration set the task table
		// This will start the parameter extraction iside the joborder plugin
		// and for this reason must be performed as the final step

		ofstream o(genInterimFullFileName( productiondir,jobordername).c_str() );//genJobOrderFileName(productiondir, extension).c_str());
        ACS_COND_THROW (o.fail(), TaskTableProcessException ("Cannot create "+ genInterimFullFileName( productiondir,jobordername) + "file" ) ); // PRQA S 3081
	
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "writing " << genInterimFullFileName( productiondir,jobordername).c_str() );
		o << jo ;

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

void TaskTableProcess::buildJobOrder( const string& filename,
                                      const string& filetype, 
                                      const string& productiondir, 
                                      const string& jobordername,
                                      const string& parametersFileName,
                                      const string& orderType,
                                      const string& satelliteName,
                                      const string& missionNumber,
                                      const string& satelliteId,
                                      const string& sensorId)
{
    multimap<std::string, std::string> filetypeVsInputFile;
    filetypeVsInputFile.insert(make_pair(filetype, filename) );
    
    buildJobOrder(  filetypeVsInputFile,
                    productiondir,
                    jobordername,
                    parametersFileName,
                    orderType,
                    satelliteName,
                    missionNumber,
                    satelliteId,
                    sensorId);

}


void TaskTableProcess::downloadParametersFile ()
{

    _parametersFileName = "";


    if (! _joParams.empty() )
    {   
        _parametersFileName = genParametersFileName();
        ofstream o(_parametersFileName.c_str());
        o << _joParams;
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _parametersFileName << " created");
    }
    else 
    {
        // Just debug messages here
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no params set for parameters file");
    }
}

string
TaskTableProcess::genParametersFileName() const
{
	// The parameters file is a clob in the database
	// there is no need to assume a format or filetype so no extensions is added
    return genInterimFullFileName (_baseDir, "DSOrderParametersFile") + "." + _extension ;
}

string TaskTableProcess::notifyExitStatus(const SchedulerExitStatus& ec) const // PRQA S 4020
{
    switch(ec)
    {
    case ssKILLED:
        {
            return "File " + _inputFile + " processing failure: process was killed";
        }
    case ssOK:
        {
            return "File " + _inputFile + " process was successful";
        }
    case ssINTERNAL_ERR:
    case ssNO_ORDER:
        {
            return "File " + _inputFile + " processing failure: scheduler internal error";
        }
    case ssNO_SPACE:
    case ssNO_WDIR:
        {
            return "File " + _inputFile + " processing failure: not enough space on disk to run the order";
        }
    case ssTASK_FAILED:
    case ssTASK_CRASHED:
    case ssABORTED:
    case ssSTOPPED:
        {
            return "File " + _inputFile + " processing failure: one of the processor's task failed";
        }
    default:
        {
            ostringstream msg ;
            msg << "File " << _inputFile<< " processing failure: unknown exit code " << int(ec) << " returned from the scheduler"; // PRQA S 3081
            return msg.str();
        }
    }
}

string
TaskTableProcess::genJobOrderFileName(bool fullpath) const // PRQA S 4020
{
    string jos = "JobOrder." + _extension + ".xml";
    
    if (fullpath)
    {
        return genInterimFullFileName(_baseDir, jos);
    }
    else {
        return jos;
    }
}

void TaskTableProcess::taskLogInDefaultLogDir (bool b) throw() // PRQA S 4121
{
	_taskLogInDefaultLogDir = b;
}

bool TaskTableProcess::taskLogInDefaultLogDir () const throw() // PRQA S 4120
{
	return _taskLogInDefaultLogDir;
}

string
TaskTableProcess::genLogFileName() const // PRQA S 4020
{
    string tmp = "ImporterChain." + _extension + ".log";
    return genInterimFullFileName( ( taskLogInDefaultLogDir () ? Application::defaultLogPath :_baseDir) , tmp);  // PRQA S 3383
}

string
TaskTableProcess::genTrackerFileName() const
{
    return genInterimFullFileName (_baseDir,"Trkfile") + "." + _extension + ".trk";
}


string
TaskTableProcess::genWorkOrderFileName() const
{
    return genInterimFullFileName(_baseDir,"WorkOrder") + "." + _extension + ".xml";
}

string
TaskTableProcess::genInterimFullFileName(const string& workdir, const string& file_name) 
{
  // this routine assumes to have as input a file name according to the conventions
  // of the specific mission job order, and generates the full pathname to insert
  // in the joborder
  return workdir +  "/" + file_name; 
}

//**************************************************************************************


void TaskTableProcess::halt_()
{
    //  IMMEDIATE_STOP
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;


    // It is not an issue because sendKillSignal has no effect if isSchedulerRunning is false
    // even if a spawn is in progress
	if (getScheduler()->isSchedulerRunning())
	{
	  // this means that the thread is running and needs to be killed .... (SIGTERM)
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "process # " << threadId() << " running"); 
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "killing (sending SIGTERM) process  -  #" << threadId()); 

	  getScheduler()->sendKillSignal(SIGTERM);  // Kill the child proc
	}
	else  // if (getScheduler()->spawned())
	{
	  // this means that the order is not in running state... forcing it to ILLEGAL
	  // because no one can start it during an IMMEDIATE_STOP

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "thread ID # " << threadId()  << " not running "); 

	  _valid=false;
	} // if (getScheduler()->spawned())
}
void TaskTableProcess::stop(bool joinAlso)
{
	halt_();
	Thread::stop(joinAlso);
}

void TaskTableProcess::setBasketForWdsInError( const std::string & s) 
{
	Lock guard(_basketMutex);
	_wdsInErrorBasket = s;
}

string TaskTableProcess::getBasketForWdsInError() const
{
	Lock guard(_basketMutex);
	return _wdsInErrorBasket ;
}

void TaskTableProcess::moveWdOnError() ACS_NOEXCEPT 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	string backupBasket ;
	try {
		backupBasket = getBasketForWdsInError();
		if( not backupBasket.empty() ) {
			ACS_COND_THROW( not File::exists(_baseDir), TaskTableProcessException('\"'+_baseDir+"\" working dir does not occur on disk!") );
			ACS_COND_THROW( not File::exists(backupBasket), TaskTableProcessException('\"'+backupBasket+"\" basket for working dir in ERROR does not occur on disk!") );
			string newPath = StringUtils::pathJoin(backupBasket, File::getFileName(_baseDir) );
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Going to rename \"" << _baseDir << "\" wdir as \"" << newPath << '\"');
			File::rename(_baseDir, newPath) ;
			_wdMovedInErrorBasket = true;
			ACS_LOG_WARNING( "[" <<  threadId() <<  "]: \"" << _baseDir << "\" processing ERROR: working directory moved into \"" << backupBasket << "\" basket");
		}
		else { // just for debug
			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Backup basket directory not valorized: no action to be performed on \"" << _baseDir << "\" wdir");
		}
	}
	catch( exception &e )
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR( "[" <<  threadId() <<  "]: Cannot move \"" << _baseDir << "\" working directory into \"" << backupBasket << "\" basket (processing ERROR occurred)");
	}

}

string TaskTableProcess::getLogDirInError() const 
{
	string logDir = workDir();
	if( _wdMovedInErrorBasket ) {
		logDir = StringUtils::pathJoin(getBasketForWdsInError(), File::getFileName(_baseDir) );
	}
	return logDir;

}
_ACS_END_NAMESPACE

