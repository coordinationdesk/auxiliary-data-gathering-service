// PRQA S 1050 EOF
/*

    Copyright 2012-2021, Exprivia SpA.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA, Inc.

*/

#include <Importer.h>
#include <ImporterVersion.h>
#include <dbConnectionPool.h>
#include <ApplicationLock.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SQLString.h>
#include <UserNotifier.h>
#ifdef USE_LIBPDSDB
  // from libPdsDb
  #include <FileTypeRecognition.h>
#else // from libDbCryosat
  #include <dbFileTypeRecognition.h>
#endif
#include <TaskTableProcess.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbSmartPersistent.h>

#include <rsPushNSpace.h>
#include <Dir.h>
#include <File.h>

#include <MCFilterables.h>

#include <AnomalyTypes.h>

#include <ImporterTableName.h>


_ACS_BEGIN_NAMESPACE(acs)

using std::string ;
using std::exception ;
using std::vector ;
using std::map ;
using std::pair ;
using std::ostringstream ;
using std::unique_ptr ;

using acs::mc::MCParameters ;


#ifdef USE_LIBPDSDB
   typedef acs::db::FileTypeRecognition FTRecognition ;
#else
   typedef acs::dbFileTypeRecognition FTRecognition ;
#endif    //split

string Importer::_backupDirDefault = "/raid0/inventory/backup";
string Importer::_processDirDefault = "/raid0/productions";
const unsigned int Importer::_maxThreadsDefault ;
string Importer::_tasktableDirDefault = string(base::defaultConfPath()) + "/tasktables" ; // PRQA S 3081

bool   Importer::_confCleanWorkingDirDefault = true;
bool   Importer::_confCopyInputDataDefault   = false;
string Importer::_confSchedulerPathDefault   = string(base::defaultBinPath()) + "/Scheduler";


namespace { // unnamed
    // version management
    ImporterVersion version ;
}



class DirectoryInspector : public Thread
{
public:
    DirectoryInspector (Importer*);
    virtual ~DirectoryInspector() throw();

    // Add a directory in the job queue
    void addPathToJobQueue( const std::string&, const uint32_t rescanPeriod);

    static time_t getCurrentMonotonicTime();

    // Configure rescan parameters
    void setScanPollingMultiplier (uint16_t multiplier);
    void setMinimumScanPeriod (uint32_t minPeriod);

protected:
    // Inherithed from Thread
    virtual void run();

    // Declared but not implemented
    DirectoryInspector (const DirectoryInspector&);
    DirectoryInspector& operator= (const DirectoryInspector&);

private:
    // Business logic
    void addAllFiles(const std::string & path);
    void updateJobsQueue();

    // Force restan time when enabling auto-rescan
    void forceRescanTime();

    // Attributes
    std::map<std::string, uint32_t> _rescanPeriods;
    std::map<std::string, uint32_t> _jobsMap;
    std::deque<std::string> _jobQueue;

    // multiplication factor for restart times defaluted to 0
    uint16_t _pathScanPollingMultiplier;
    // minimum time for a complete rescan, defaulted to the restart time * 2
    // (avoid rescan just in the middle of a restart)
    uint32_t _minimumPathScanPeriod;

    Importer* _importer;
    Condition _condition;


    ACS_CLASS_DECLARE_DEBUG_LEVEL(DirectoryInspector) ;
};


ACS_CLASS_DEFINE_DEBUG_LEVEL(Importer)


const unsigned int Importer::_defaultRestartTime = 86400; //One day in seconds


/*
 * Class constructor
 */
Importer::Importer(const string &appName, const string &subSystem):
    PROJECT_APP (appName, subSystem),
    _eventsList(),
    _processThreads(),
    _mapReceptionRules(),
    _mapLocalPathToFTypes(),
    _mapLocalPathToScanTime(),
    _waitingFiles("Unknown Files", 10000, 500000),
    _toProcessDataFiles(),
    _restartTime (_defaultRestartTime),
    _logInDefaultDir(false),
    _backupDir(),
    _processDir(),
    _tasktableDir(),
    _confCleanWorkingDir(),
    _confCopyInputData(),
    _confSchedulerPath(),
    _maxThreads(),
    _additionalExtension(),
    _populateInvAnomalies(false),
    _currentWatchedPaths(),
    _internalQueueMutex(),
    _internalQueue("Processing", 0, 500000),
    _observationMap(),
    _condition(),
    _processingFiles(),
    _newEvents(false),
    _dataStructCache(),
    _pathScanPollingMultiplier(0),
    _minimumPathScanPeriod(2 * _restartTime),
    _clusterName(),
    _basketForWdsInError()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << version.name()) ;
}

Importer::~Importer() throw () {
    //empty.
}

void Importer::customSigTermHandler()
{
    ACS_LOG_INFO("SigTerm caught: Stop scheduled") ;

    // Exit from wait condition in case it is sleeping
    _condition.broadcast();

}

void Importer::customSigIntHandler()
{
    ACS_LOG_INFO("SigInt caught: Stop scheduled") ;

    // Exit from wait condition in case it is sleeping
    _condition.broadcast();

}

struct fillFileFromEvent {
    string operator() (const DiskEventNotifier::EventElement& ev) // PRQA S 4211
    {
       return ev.src_absolute_path;
    }
};



pair < string, string > Importer::file2Key(const string & fullpath) const // PRQA S 4020
{
    dbConnectionWrapper cw(*getDefaultConnectionPool()) ;
    dbConnection &conn = cw ; // PRQA S 3050
    string localpath = File::getDirPath(fullpath);
    map< string, vector<string> >::const_iterator it = _mapLocalPathToFTypes.find(localpath) ;

    if (it == _mapLocalPathToFTypes.end()) {
        ACS_LOG_WARNING( "File " << fullpath << ": no localpath configured. ") ;
        return std::make_pair(string(), string()) ;
    }

    const vector<string> &ftypes = it->second ;
    string fname = File::getFileName(fullpath);

    //split
    FTRecognition ftr;
    size_t ft_size = ftypes.size() ;
    for (size_t i=0; i < ft_size; ++i) {
        string ftype = ftypes[i] ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking vs ftype: " << ftype) ;
        string r_ftype = ftr.fnameBelong2RFtype(ftype, fname,conn) ;
        if (not r_ftype.empty()) {
            return std::make_pair(r_ftype, localpath) ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File " << fullpath << ": unable to associate to any FileType of active rules.") ;
    return std::make_pair(string(), string()) ;
}


#if __GNUC__ > 4 || \
              (__GNUC__ == 4 && (__GNUC_MINOR__ > 4 || \
                                 (__GNUC_MINOR__ == 4 && \
                                  __GNUC_PATCHLEVEL__ > 3)))
bool Importer::IsAWaitedFile( std::pair< Pair_Ftype_LocalPath, RecRulesInfo>  elem , const pair<string,string>& val) // PRQA S 2010, 4020
#else
bool Importer::IsAWaitedFile( std::pair< Pair_Ftype_LocalPath, RecRulesInfo>  elem , const pair<string,string> val) // PRQA S 2010, 4020
#endif
{
    Pair_Ftype_LocalPath fp = elem.first;

    string ftype = fp.first;
    string localpath = fp.second;

    if (val.first != localpath) {
        return false;
    }

    string fn  = val.second;

    FTRecognition rec;
    return rec.fnameBelong2Ftype( ftype, fn, dbConnectionWrapper(*(Application::instance()->getDefaultConnectionPool() ) )); // PRQA S 3050, 3081

}

Importer::inDataStruct Importer::file2DataStruct( string fullpath) const// PRQA S 2010, 4020
{
    inDataStruct inData;
    //filename
    inData.fullpathname = fullpath;

    //filetype
    Pair_Ftype_LocalPath fp = file2Key(fullpath);
    inData.filetype = fp.first;   //real file type
    inData.localpath = fp.second; //empty if filetype not found

    map< Pair_Ftype_LocalPath , RecRulesInfo>::const_iterator it;
    //try first the fast search..
    it = _mapReceptionRules.find(fp);
    //if fails try also against virtual file type
    if (it == _mapReceptionRules.end() && !inData.filetype.empty() )
    {
        string filename = File::getFileName(fullpath) ;
        pair < string, string > myInput(inData.localpath,filename);

        it = find_if(_mapReceptionRules.begin(),_mapReceptionRules.end(),bind2nd(ptr_fun(Importer::IsAWaitedFile),myInput));
    }

    if (it == _mapReceptionRules.end() )
    {
        //file without reception rules or no configured filetype.
        inData.valid = false;
        return inData;
    }

    inData.procFailureAddress           = (*it).second.procFailureAddress;
    inData.successNotificationAddresses = (*it).second.successNotificationAddresses;
    inData.ttParam                      = (*it).second.ttParam; //clob parameters
    inData.ttId                         = (*it).second.ttId; //task table // PRQA S 3000, 3010
    inData.satelliteId					= (*it).second.satelliteId;
    inData.mission						= (*it).second.mission;
    inData.sensorId						= (*it).second.sensorId;


    //check and fill second filename if needed
    bool generate_header = (*it).second.generateHeader;

    if (!generate_header)
    {
        FTRecognition::ExtensionInfo extInfo;
        if( !FTRecognition::checkFNameExtension(inData.filetype, fullpath, extInfo, dbConnectionWrapper(*getDefaultConnectionPool())) ) // PRQA S 3050, 3081
        {
            ACS_LOG_WARNING("Extension of file \"" << fullpath<< "\" does not match the one configured on db for file type \"" << inData.filetype << "\"") ;
            inData.valid = false; //anyway add it to waiting list
        }
        else if (extInfo.expectedExtNr == 2 ) // PRQA S 4400
        {
            if ( extInfo.extId == FTRecognition::HDR ) {
                inData.coupledPath = File::addExt( File::removeExt(fullpath), extInfo.otherExt ) ;
            }
            else { //DBL data ;  wait for HDR..
                inData.valid = false;
            }

        }
        else {
            // nothing
        }
    }

    return inData;
}

int Importer::main(int argc, char const * const * argv, char const * const * env)	// PRQA S 4020
{
    // only one Importer application for host
#if defined (LTA)
    ApplicationLock appLock(6) ;
#else
    ApplicationLock appLock(1) ;
#endif
    appLock.lock(); //exception is raised if lock fails

    // APF-272
    MCParameters::instance()->setDefaultFunction ("Data Import");
    MCParameters::instance()->setDefaultCode ("IMP");

    if (&exostream::getDefaultOutLogger() != &excmcf) {
        exostream::getDefaultOutLogger().appendChild(&excmcf) ;
    }

//    Application::rotateLog() ; // open the first log

    daemonize();

    //Load Configuration
    loadConf();

    try {
        Application::setStatus (Application::Ok);

        mainLoop() ; // loop and do nominal work

        ACS_LOG_INFO("Main loop finished regularly") ;
    }
    catch(exception &e)
    {
        ACS_LOG_ERROR("Exiting due to following exception: " << e) ;

           Application::setStatus (Application::Critical, "Error due to exception");

        cleanExit();
        return 1 ;
    }

    ACS_LOG_INFO("Starting exiting sequence...") ;
    Application::setStatus (Application::Exiting);

    manageStop();

    return 0;
}

void Importer::manageStop()
{
    if ( hasStopModeChanged() ) {
        const Application::StopMode stopMode = getCurrentStopMode() ;
        if( (CleanStop == stopMode ) || (PhoenixRestart == stopMode ) ) {

            ACS_LOG_INFO("Waiting for running threads...") ;

            cleanExit(); //STOP -> stop gracefully, finish running requests threads..

        }
        else if ( ImmediateStop == stopMode ) {

            ACS_LOG_INFO("Importer:  Stop running threads...") ;

            stopImmediate(); //ABORT to all threads of all request
        }
    }
}

void Importer::mainLoop()
{
    wantToRestart(_restartTime); //restart every day

    ACS_LOG_PRIVINFO("Restarting every " << _restartTime << " [s]") ;

    DiskEventNotifier * notifier = 0;

    unique_ptr<DiskEventNotifier>  notifierGuard(notifier);

    // Create the directory inspector, configure it and start the thread
    DirectoryInspector theInspector (this);
    theInspector.setScanPollingMultiplier (_pathScanPollingMultiplier);
    theInspector.setMinimumScanPeriod (_minimumPathScanPeriod);
    theInspector.start();

// the inotify and directory inspector threads are configured inside this call
    restartConfiguration (notifierGuard, theInspector, true);

    //Download all the Import Task Tables
    TaskTableProcess::downloadTaskTablesOfType("IMPORT", _tasktableDir, dbConnectionWrapper(*getDefaultConnectionPool()) ); // PRQA S 3050, 3081

    //************     LOOP ****************************
    //
    Application::Status current_status = Application::Ok ;
    while ( NoStop == getCurrentStopMode() )
    {
        try {

            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Rotating log if size > " << getLRPolicy().size() << " or older than " << getLRPolicy().aging() << " [s]") ;
            // rotate log if needed
            Application::rotateLog() ;

            if ( NoStop == getCurrentStopMode() )
            {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Managing status - current status: " << current_status) ;
                // here to manage the transition from critical to ok status
                string status_description ; // empty
                if (current_status != Application::Ok) {
                    status_description = "Resuming from previous OK status" ;
                    current_status = Application::Ok ;
                }
                Application::setStatus (Application::Ok, status_description);
                // end of status management section

                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Calling do work") ;
                bool somethingStarted = doWork();

                if (!somethingStarted)
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No workers started. Waiting for condition or timeout " << polling_freq << " ms" ) ;

                    // nothing started.. wait for new events
                    {
                        ThreadSafe::Lock lock (_condition);
                        if ( _condition.getEventCounter() == 0 )
                        {
                            _condition.timedwait (polling_freq); // PRQA S 4400
                        }
                        // The condition is still locked ... must consume the events before entering another loop
                        _condition.resetEventCounter();
                    }
                }
                else
                {
                        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "New workers started. Make another doWork loop without any delay.") ;
                }

                 ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Calling checkProcessThreads") ;
                checkProcessThreads( );
            }

        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
            // keep the application running as long as possible.
            current_status = Application::Critical ;
               Application::setStatus (Application::Critical, "Error due to exception, see log");
        }


    } //loop

    //Restart (phoenix)
    if ( PhoenixRestart == getCurrentStopMode() ) {
        if (current_status != Application::Ok) {
            ACS_LOG_WARNING("Time to restart, due the actual Application Status there could be problem.") ;
        }
        else {
            ACS_LOG_PRIVINFO( "Time to restart");
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Final size of data structure cache is " << _dataStructCache.size()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "End of loop") ;

}

bool Importer::collectAdditionalFiles( const string & fullpathfile, bool isPaired,
        vector<string> & additionalFiles, vector<string> & additionalFilesError ) ACS_NOEXCEPT {

    ACS_CLASS_ANNOUNCE_DEBUG( ACS_LOW_VERB );

    bool isOk(true);
    try {

        if (not _additionalExtension.empty()) {
            size_t extTot = _additionalExtension.size() ;
            for ( size_t i(0); i<extTot; ++i ) {

                string fullpathAdditionalFile = ( isPaired ) ?
                        File::fileWExt(fullpathfile, _additionalExtension[i].extension, true) :
                        File::addExt  (fullpathfile, _additionalExtension[i].extension  )     ;

                if ( File::exists( fullpathAdditionalFile ) ) {
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found Additional file ["<<fullpathAdditionalFile<<"]") ;

                    additionalFiles.push_back(fullpathAdditionalFile);
                } else {
                    if ( _additionalExtension[i].mandatory ) {
                        ACS_LOG_WARNING("Not Found Mandatory Additional file ["<<fullpathAdditionalFile<<"]") ;

                        additionalFilesError.push_back( fullpathAdditionalFile );
                    }
                }
            }
        }

    } catch( exception& ex ) {
        ACS_LOG_NOTIFY_EX( ex );
        isOk = false;
    }

    return isOk && additionalFilesError.empty();

}


bool  Importer::doWork() // PRQA S 4020
{

    bool newWorkersStarted(false);

    //if maximum threads number reached do nothing
    size_t availableThreadsNum = _maxThreads - _processThreads.size() ; // PRQA S 3000, 3084

    //search new input files. and new events
    // sync logic is needed inside checkDataAvailability to prevend trigger lost
    bool newEvents = false;
    using std::set ;
    set <string> inputFileNames;
    checkDataAvailability(inputFileNames, availableThreadsNum, newEvents); //fill only to max availability
    size_t numfiles = inputFileNames.size();

    if ( numfiles != 0 || newEvents == true ) //something new has found!
    {

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, " Found " << numfiles << " new files in input baskets. ") ;

        // all the files in the internal queue are associated with actual rules
        // and have been already checked as valid
        // size_t foundDVsize = inputFileNames.size() ;
        for (set<string>::iterator it=inputFileNames.begin(); it != inputFileNames.end(); ++it)
        {
            if ( NoStop != getCurrentStopMode() ) {
                return newWorkersStarted;
            }

            if ( _dataStructCache.find(*it) == _dataStructCache.end() )
            {
                // OOPS.. the file DataStruct has been lost!
                // Report the issue and go ahead
                ACS_LOG_ERROR ("The metadata information of the file \"" << *it << "\" have been lost. It will be left on disk and re-evaluated if touched or at next application restart.")
            }
            else
            {
                // Found it
                inDataStruct currentData = _dataStructCache[ *it ];
                string fullpathname = currentData.fullpathname;
                _toProcessDataFiles.remove_if(bind2nd(ptr_fun(Importer::compareName ),fullpathname  ) );
                _toProcessDataFiles.push_back( std::make_pair(currentData, time(0)));
                // Cleanup the dataStructure cache area
                _dataStructCache.erase(*it);
            }
        }

        //start a new process for each found file
        //--------------------------------------
        std::list< pair< inDataStruct, time_t> >::iterator iter;
        iter = _toProcessDataFiles.begin();
        while ( (iter != _toProcessDataFiles.end()) && ( NoStop == getCurrentStopMode() ) && ( _maxThreads > _processThreads.size() ) )
        {
            //input file
            string fullpathfile = (*iter).first.fullpathname;

            //related info
            string filetype =  (*iter).first.filetype;
            size_t ttId =  (*iter).first.ttId; // PRQA S 3000
            string ttparam =  (*iter).first.ttParam;
            string procFailureAddress = (*iter).first.procFailureAddress;
            string successNotificationAddresses = (*iter).first.successNotificationAddresses;
            string coupledPath = (*iter).first.coupledPath;
            string satelliteId = (*iter).first.satelliteId;
            string mission = (*iter).first.mission;
            string sensorId = (*iter).first.sensorId;

            bool doubleFile = (! coupledPath.empty());

            if  (  doubleFile  && !_waitingFiles.hasElement(coupledPath) )
            {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Missing " + coupledPath + ". Adding file to waiting list...");

                //cannot proceed ; still missing the relative DBL file
                ++iter;
            }
            else
            {

                ACS_LOG_INFO("File \"" << File::getFileName(fullpathfile) << "\"" << (coupledPath.empty() ? "" : (" + \"" + File::getFileName(coupledPath) + "\""))<< ". Filetype " << filetype  << " - Local path " + (*iter).first.localpath);

                vector<string> additionalFiles;
                vector<string> additionalFilesError;
                try
                {
                    // First of all remove the coupled file from the waiting list
                    // To ensure its remotion even in case of processing or setup failures
                    if (doubleFile)
                    {
                        _waitingFiles.removeElement (coupledPath);
                    }

                    // Collect additional files, if configured
                    if ( not this->collectAdditionalFiles( fullpathfile, doubleFile, additionalFiles, additionalFilesError ) ) {
                        ostringstream msg;
                        msg << "Error retrieving additional files. Expected files not found: "<<additionalFilesError.size();
                        ACS_THROW(ImporterException(msg.str()));
                    }

                    //string tasktablePath = getTaskTable(ttId); //search in default path or reload from db
                    string tasktablePath = TaskTableProcess::downloadTaskTable(ttId,_tasktableDir, dbConnectionWrapper(*getDefaultConnectionPool())); // PRQA S 3000, 3050, 3081

                    TaskTableProcessConfig::Sptr configTTP(
                            std::make_shared<TaskTableProcessConfig>(_confSchedulerPath, _confCopyInputData, _confCleanWorkingDir) );

                    TaskTableProcess *ttp = new TaskTableProcess(configTTP,
                                                                 fullpathfile,
                                                                 filetype ,
                                                                 tasktablePath,
                                                                 _processDir );
                    // Add the coupled input
                    if (doubleFile)
                    {
                        ttp->addInputFile( coupledPath );
                    }

                    // Add the additional files
                    for ( const auto & additionalFile : additionalFiles ) {
                        ttp->addInputFile( additionalFile );
                    }

                    ttp->setOrderType("IMP"); //Import
                    ttp->setParameters(ttparam);
                    ttp->setMailNotificationAddresses(successNotificationAddresses, procFailureAddress);
                    ttp->setBackupDir(_backupDir);

                    ttp->setSatelliteId( satelliteId );
                    ttp->setMission( mission );
                    ttp->setSensorId( sensorId );

                    // set the Importer condition pointer
                    ttp->setExternalCondition (&_condition);

                    // set if the log has to be placed in the working dir or in the default log path
                    ttp->taskLogInDefaultLogDir (_logInDefaultDir);

                    ttp->setBasketForWdsInError(_basketForWdsInError);

                    ttp->start();
                    //push_back
                    _processThreads.push_back(ttp);
                    newWorkersStarted=true;

                    _processingFiles.insert (fullpathfile);
                    ACS_LOG_INFO("New processing thread started on \"" << fullpathfile << "\" file.") ;


                }
                catch (exception const& ee)
                {
                    ACS_LOG_ERROR("Received exception for " << fullpathfile  << " " << ee) ;

                    ostringstream msg;
                    msg << "Importer got an internal error during the processing setup: " << ee.what();
                    // APF-272
                    ACS_LOG_MC_INFO (
                        acs::mc::mc_param("filename", File::getFileName(fullpathfile)) <<
                        acs::mc::mc_param("message", msg.str()) <<
                        acs::mc::mc_metric (acs::mc::Metric_FAIL)
                    );

                    //move file[s] in a backup dir
                    if ( !_backupDir.empty() )
                    {
                        try
                        {
                            if (File::exists(fullpathfile) )
                            {
                                string inputfile= File::getFileName(fullpathfile);
                                ACS_LOG_INFO("Cleaning dir moving " << fullpathfile << "  to " << _backupDir) ;

                                File::unlink(_backupDir+ "/" + inputfile);
                                File::rename(fullpathfile, _backupDir+ "/" + inputfile);
                            }
                            if (doubleFile && File::exists (coupledPath) )
                            {
                                string inputfile= File::getFileName(coupledPath);
                                ACS_LOG_INFO("Cleaning dir moving " << coupledPath << "  to " << _backupDir) ;

                                File::unlink(_backupDir+ "/" + inputfile);
                                File::rename(coupledPath, _backupDir+ "/" + inputfile);
                            }
                            for ( const auto & additionalFile : additionalFiles ) {
                                if ( File::exists( additionalFile ) ) {
                                    string inputfile= File::getFileName(additionalFile);
                                    ACS_LOG_INFO("Cleaning dir moving " << additionalFile << " to " << _backupDir) ;

                                    File::unlink(_backupDir+ "/" + inputfile);
                                    File::rename(additionalFile, _backupDir+ "/" + inputfile);
                                }
                            }
                        }
                        catch (exception & e)
                        {
                            ACS_LOG_WARNING("Error removing  "+ fullpathfile+ " from working dir..  " << e) ;
                        }
                    }
                    //
                }

                // ok ...processed
                iter = _toProcessDataFiles.erase(iter);

            }//else [ process now or wait ]
        } //while  [ next file ]

        //Debug

        ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)

            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Unrecognized files count: " << _waitingFiles.size()) ;

            std::list< pair< inDataStruct, time_t> >::iterator iter_tpdf;
            iter_tpdf = _toProcessDataFiles.begin();

            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Waiting primary files: ") ;
            while ( iter_tpdf != _toProcessDataFiles.end() )
            {
               ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,(*iter_tpdf).first.fullpathname << " "  << (*iter_tpdf).second) ;
                ++iter_tpdf;
            }
        ACS_CLASS_END_DEBUG

    } //numfile

    return newWorkersStarted;
}



class PathNotExists // PRQA S 2109
{
public:
    PathNotExists(vector<string>&cur, vector<string>&newp) : _cur(cur), _new(newp) {} // PRQA S 2528 2
    PathNotExists(const PathNotExists &r)  : _cur(r._cur), _new(r._new) {}
    ~PathNotExists() throw() {}

    void operator() (const string& s)
    {
        if (find(_cur.begin(), _cur.end(), s) == _cur.end())
        {
            _new.push_back(s);
        }
    }

private:
    PathNotExists() ; // not implemented
    PathNotExists& operator= (const PathNotExists &) ; // not implemented

private:
    vector<string>& _cur;
    vector<string>& _new;
};

void Importer::restartConfiguration( // PRQA S 4020
    unique_ptr<DiskEventNotifier> & notifierG,
    DirectoryInspector& theInspector,
    bool first_time)
{

    //fill DiskEventNotifier pathsToWatch (task tables are managed directly during the process)
    //wait all running process terminate...

    ACS_LOG_PRIVINFO((first_time? "Load DB table first time " : "DB table reload  ")); // PRQA S 3380
    // get filetype and local path of active and right hostname reception rules
    vector<string> pathsToWatch;
    getRecRulesFromDb(pathsToWatch); //fill _mapReceptionRules


    if (ACS_CLASS_GET_DEBUG() == 13579) // PRQA S 4400
    {
        pathsToWatch.clear();
        pathsToWatch.push_back("/tmp/so_notifier/tmp");
    }

    // Detect the new added paths to watch
    // in order to perform a directory scan before starting the new notifier
    vector<string> newPaths;
    PathNotExists pathChecker(_currentWatchedPaths, newPaths);
    for_each (pathsToWatch.begin(), pathsToWatch.end(), pathChecker);

    if ( newPaths.size() > 0 )
    {
        ACS_LOG_PRIVINFO ("Adding " << newPaths.size() << " paths to the notify listener");
    }

    if ( pathsToWatch.empty() )
    {
        ACS_LOG_WARNING("No configured path to watch.") ;
        if ( NoStop != getCurrentStopMode() ) {
            return ;
        }
    }
    else if ( _currentWatchedPaths != pathsToWatch )
    {
        ACS_LOG_INFO((first_time? "Starting disk notifier for paths:" : "Restart disk notifier for paths:")) ; // PRQA S 3380

        size_t pwsize = pathsToWatch.size() ;
        for (size_t i=0; i < pwsize; ++i) {
            ACS_LOG_INFO('[' << i << "] - " << pathsToWatch[i]) ;
        }
    }
    else { /* nothing */ }

    // update the current watched paths
    _currentWatchedPaths=pathsToWatch;

    if (first_time) {
        // Configure the events list object
        _eventsList.clear();
        _eventsList.setNotificationCondition (&_condition);
    }

    os::Notify::EventsMask mask = static_cast<os::Notify::EventsMask> (os::Notify::NOTIFY_CLOSE_WRITE  | os::Notify::NOTIFY_MOVED_TO | os::Notify::NOTIFY_CREATE ) ; // PRQA S 3013, 3081
    DiskEventNotifier * notifier = new DiskEventNotifier(pathsToWatch, false, mask, _eventsList);

    //delete  DiskEventNotifier and allocate new
    notifierG.reset(notifier);
    notifier->start();

    // Add paths to the inspector job list
    for (size_t i = 0; i < _currentWatchedPaths.size(); ++i )
    {
        // Can add here the paths and the scan period in seconds
        // the inspector will decide if rescan and when
        // If any configuration reload will be implemented, remember to create a brand new inspector somewhere
        theInspector.addPathToJobQueue (_currentWatchedPaths[i], _mapLocalPathToScanTime[_currentWatchedPaths[i]]);
    }

 }


void Importer::checkProcessThreads()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"checking " <<   _processThreads.size() <<  " threads") ;
    size_t ptsize = _processThreads.size() ;
    for ( size_t counter = 0; counter < ptsize; ++counter )
    {
        if ( _processThreads[counter] && _processThreads[counter]->running() ) {
            continue;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"found thread in pos " << counter << " not running!") ;

        {
            string filenamefullpath =  _processThreads[counter]->mainInputFilefullpath();
            Lock lock (_internalQueueMutex);
            if ( _processingFiles.erase (filenamefullpath) == 0 )
            {
                // not expected!
                ACS_LOG_WARNING('\"' << filenamefullpath << "\" file just processed does not belong to list of files being processed!")
            }
            // Complete the cleanup of the queue, freeing the set
            _internalQueue.removeElement(filenamefullpath);
        }

        string filename =  _processThreads[counter]->mainInputFile();

   // check if thread exit in error
        std::exception* e = 0;
        _processThreads[counter]->runException ( e );
        if ( e != 0 ) {

         //   failedRequests.push_back ( dsIdSerial);

            ACS_LOG_TRYCATCHNOTIFY_EX (
                ImporterException( *e,
                                        "Importer::checkProcessThreads Received Exception during processing   "
                                        + filename ));
            delete e;

            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"Stopping the processing thread...") ;

            _processThreads[counter]->stop();
            while ( _processThreads[counter]->running() ) {
                Timer::delay ( 10 ); // PRQA S 4400
            }

            ACS_LOG_ERROR("Thread for request " << filename << " terminated with exception; ") ;

            delete _processThreads[counter];
            _processThreads[counter] = 0;
            continue;
        } //exc

        SchedulerExitStatus status =  _processThreads[counter]->exitStatus();

        ostringstream msgs;
        msgs << "Thread for request " << filename << " terminated with status ; " <<  schedulerExitStatus2String(status);
        if ( status != ssOK) {
            ACS_LOG_ERROR( msgs.str() );
             ACS_LOG_ERROR( "Inspect log file inside directory : " <<  _processThreads[counter]->getLogDirInError() );

        }
        else {
            ACS_LOG_INFO( msgs.str() );
        }


        delete _processThreads[counter];
        _processThreads[counter] = 0;

        continue; // useless, just for clarity

    }   // next thread

    // remove from _processThreads all the elements set to NULL
    void *nullpointer = 0 ;
    _processThreads.erase( remove(_processThreads.begin(),_processThreads.end(), nullpointer ) ,_processThreads.end());
    //
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"_processThreads.size() = " << _processThreads.size()) ;

}


// *** STL Helper
class InternalQueueMerger // PRQA S 2109
{
public:
    explicit InternalQueueMerger (Importer* i) : _importer(i) {} // PRQA S 2528 2
    InternalQueueMerger(const InternalQueueMerger &r) : _importer(r._importer) {}
    ~InternalQueueMerger() throw() {}

    // Fill it with EventElement
    void operator() (const DiskEventNotifier::EventElement& e)
    {
        _importer->addToInternalQueue(e.src_absolute_path);
    }

    // Fill it with absolute paths
    void operator() (const string& path)
    {
        _importer->addToInternalQueue(path);
    }

private:
    InternalQueueMerger() ; // not implemented
    InternalQueueMerger& operator= (const InternalQueueMerger &) ; // not implemented
private:
    Importer* _importer;
};

void Importer::checkDataAvailability(std::set <string> &inFilesString, size_t maxnum, bool& newEvents) //fill only // PRQA S 4020
{
    // Get all the data from the inotify, but lock the internal structure before doing this
    Lock lock (_internalQueueMutex);

    inFilesString.clear();

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "(Before promotion) - Observation list size is: " << _observationMap.size() << " - Current internal queue size is now: " << _internalQueue.size() );

    // Promote the observed files if needed
    promoteObservedFiles();

    size_t elNr = _eventsList.getSize();

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"found  " << elNr << "  new events in the inotify queue");


    if (elNr > 0)
    {
        // Merge the new files in the internal queue discarding duplicates
        std::list<DiskEventNotifier::EventElement> wl;
        _eventsList.getData(wl);

        for_each (wl.begin(), wl.end(), InternalQueueMerger(this)); // PRQA S 3081
    }

    // Save if new events occurred and then reset the flags
    // Note we have the mutex locked right now
    newEvents = _newEvents;

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "(Before extraction) - Current internal queue size is now: " << _internalQueue.size() );


    // The list has been merged, extract maxnum elements
    for (size_t idx=0; not _internalQueue.empty() && (idx<maxnum); ++idx) // PRQA S 4238, 4244
    {
        // dequeue will also remove elements from the queue

        string pathname = _internalQueue.dequeue();

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding the following path to the current batch: " << pathname);
        // The file insertied in inFilesString has been extracted from the internalqueue
        // so a valid entry in the inDataStruct cache will be found
        // Cache will be cleaned up when consuming inFilesString entries
        // in the doWork function
        inFilesString.insert (pathname);
    }

    // Now new events can be triggered
    _newEvents = false;

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "(After extraction) - Current internal queue size is now: " << _internalQueue.size() );

}

void Importer::cleanExit()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_processThreads.size(): " << _processThreads.size()) ;

    while (_processThreads.size() > 0 )
    {
        Timer::delay ( 100 ); // fast check // PRQA S 4400

        checkProcessThreads(); //check only already processing threads..
    }

    UserNotifier::instance()->closeAndFlushAndJoin(false);
}

void Importer::stopImmediate()
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"_processThreads.size(): " << _processThreads.size()) ;

 //stop to each  Request Mgr
 //
    for_each(_processThreads.begin(), _processThreads.end(), std::mem_fun(&TaskTableProcess::halt_));
    //for ( size_t i = 0; i < _processThreads.size(); ++i )
      //  _processThreads[i]->stopImmediate();  //set internal _stop...

    UserNotifier::instance()->closeAndFlushAndJoin(true);

    cleanExit();

}

namespace {
void getTTParameters(dbSmartPersistent &sp, std::ostream &theStream, dbConnection& conn)
{
    string tempFileName = File::tempname("/tmp/dbRecRuleParamFileTemp"); // PRQA S 3050
    // The temporary file will be deleted by the guard when exiting this method
    FileGuard theGuard (tempFileName);

    sp.downloadFile("tt_parameters",tempFileName,conn) ;

    // Now the parameter file is downlaoded in tmp repository
    // transfer it into the stream

    std::ifstream theFile (tempFileName.c_str());

    // Assuming here that the parameter file is not an HUGE file!!
    size_t theFileSize = File::size(tempFileName); // PRQA S 3000

    char buffer[theFileSize];
    theFile.read(buffer, theFileSize); // PRQA S 3000 2
    theStream.write(buffer, theFileSize);

    theFile.close();

    // work complete !!
}
}

void Importer::getRecRulesFromDb( vector<string> & localpaths )
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    localpaths.clear();

   /*/ SELECT   T_ReceptionRules.id_receptionrule,
  *             T_ReceptionRules.reception_tt_id,
                T_ReceptionRules.localpath,
                T_ReceptionRules.procfailureaddress,
                T_ReceptionRules.successnotificationaddress,
                t_filetypes.filetype,
                T_ReceptionRules.generateheader
                FROM  T_ReceptionRules
                INNER JOIN t_filetypes
                    ON  ( ( (t_filetypes.id_filetype = T_ReceptionRules.filetype_id) ) )
                INNER JOIN t_users
                    ON  ( ( (T_ReceptionRules.User_Id = t_users.id_user)  )
                            AND (  (T_ReceptionRules.UserMode_id = t_users.mode) ) )
                WHERE  ( ( (T_ReceptionRules.hostname = 'sentinel-sl6')  )
                            OR
                            ((T_ReceptionRules.hostname IS NULL ) ) )
                       AND
                       ( ( (T_ReceptionRules.active = 't') ) )  */

    const string t_receptionrules = ImporterTableName::instance()->get( ImporterTableName::t_receptionrules );

    _mapReceptionRules.clear();
    _mapLocalPathToFTypes.clear();
    _mapLocalPathToScanTime.clear();

    try
    {
        dbConnectionWrapper pw(*getDefaultConnectionPool());
        dbConnection &conn = pw ; // PRQA S 3050
        const string &dbKey = conn.getKey() ;

        bool hasField_ReceptionRulesSatelliteId =
                dbSchemaMetaDataProvider::hasField(t_receptionrules, "satellite_id", conn); // PRQA S 4412

        bool hasField_ReceptionRulesUserId =
                dbSchemaMetaDataProvider::hasField(t_receptionrules, "user_id", conn); // PRQA S 4412

        SQLString sqlString(dbKey, SQLString::SELECT ) ;
        sqlString.addSelectList(t_receptionrules + ".id_receptionrule");
        sqlString.addSelectList(t_receptionrules + ".reception_tt_id");
        sqlString.addSelectList(t_receptionrules + ".localpath");
        sqlString.addSelectList(t_receptionrules + ".procfailureaddress");
        sqlString.addSelectList(t_receptionrules + ".successnotificationaddress");
        sqlString.addSelectList("t_filetypes.filetype");
        sqlString.addSelectList(t_receptionrules + ".generateheader");
        sqlString.addSelectList(t_receptionrules + ".pollingperiod_sec");
        if ( hasField_ReceptionRulesSatelliteId ) {
            sqlString.addSelectList(t_receptionrules + ".satellite_id");
            sqlString.addSelectList(t_receptionrules + ".mission");
            sqlString.addSelectList(t_receptionrules + ".sensor_id");
        }
        sqlString.addTableList(t_receptionrules);

         // Add Join
        {
            dbQueryParameters pp(dbKey) ;
            pp.addParameter(dbParam(dbKey, "t_filetypes","id_filetype" , dbParam::EQUAL_TO , t_receptionrules + ".filetype_id" )) ;
            sqlString.addJoinList(SQLDialect::INNER,"t_filetypes",pp) ;
        }
        if (hasField_ReceptionRulesUserId) {
            dbQueryParameters pp(dbKey);
            pp.addParameter(dbParam(dbKey, t_receptionrules, "User_Id"    , dbParam::EQUAL_TO, "t_users.id_user"));
            pp.addParameter(dbParam(dbKey, t_receptionrules, "UserMode_id", dbParam::EQUAL_TO,
                    SQLDialectFactory::dialectName(dbKey) == "Oracle11" ? "t_users.\"mode\"" : "t_users.mode")); // PRQA S 3380

            sqlString.addJoinList(SQLDialect::INNER,"t_users",pp) ;
        }

        // Build Where clause
        {
          dbQueryParameters pp(dbKey);
          pp.addParameter (dbParam(dbKey, t_receptionrules,"hostname", dbParam::EQUAL_TO, db::toSQL(dbKey, _clusterName))); // PRQA S 3050, 3081
          pp.addParameter (dbParam(dbKey, t_receptionrules,"hostname", dbParam::ISNULL));
          pp.setParamOperator (dbQueryParameters::OR);
          sqlString.addANDWhereParameter(pp) ;
        }
        {
          dbQueryParameters pp(dbKey);
          pp.addParameter (dbParam(dbKey,t_receptionrules,"active", dbParam::EQUAL_TO, db::toSQL(dbKey, true))); // PRQA S 3050, 3081
          sqlString.addANDWhereParameter(pp) ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"Executing query " << sqlString.getSQL()) ;

        dbQuery queryFt(conn, sqlString, "ReceptionRulesGetInfo") ;
        dbSet ftSelSet ;
        queryFt.execForSet(ftSelSet) ;

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"loaded  " << ftSelSet.size()  <<  " active reception rules for my host") ;

        size_t ftSelSize = ftSelSet.size() ;
        for (size_t i = 0 ; i < ftSelSize; ++i)
        {
            RecRulesInfo rule;

            dbRow row(dbKey) ;
            ftSelSet.getNextRow(row);
            //bool genheader;

            dbPersistent::IdType r_id = 0 ;
            uint16_t j = 0 ;
            row.column(j++,r_id);

            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Reception rules #" <<  r_id ) ;

            //task table id
            row.column(j++,rule.ttId) ;
            //input local path
            string localpath;
            row.column(j++,localpath) ; // PRQA S 4400
            //mail address for warning
            bool procFailureAddressNull = false ;
            row.column(j++,rule.procFailureAddress, &procFailureAddressNull) ; // PRQA S 4400
            //mail address for notification
            bool successNotificationAddressesNull = false ;
            row.column(j++,rule.successNotificationAddresses, &successNotificationAddressesNull) ;
            //filetype
            string filetype;
            row.column(j++,filetype) ; // PRQA S 4400
            //generate header
            row.column(j++,rule.generateHeader) ; // PRQA S 4400
            // Polling period in seconds
            uint32_t pollingPeriod;
            row.column(j++, pollingPeriod) ; // PRQA S 4400

            //task table clob parameters
            if ( hasField_ReceptionRulesSatelliteId ) {
                bool isNull(false);
                dbPersistent::IdType value(0);

                row.column( j++, value, &isNull );
                { ostringstream v; v<<value; if (!isNull) { rule.satelliteId = v.str(); } }

                row.column( j++, value, &isNull );
                { ostringstream v; v<<value; if (!isNull) { rule.mission = v.str(); } }

                row.column( j++, value, &isNull );
                { ostringstream v; v<<value; if (!isNull) { rule.sensorId = v.str(); } }
            }

            dbSmartPersistent rr(t_receptionrules, conn);

            rr.setId(r_id); // PRQA S 3000
            rr.load(conn);
            if (rr.isSet("tt_parameters") ) //not null
            {
                ostringstream os;
                getTTParameters(rr, os, conn);
                rule.ttParam = os.str();

                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Clob dump is \n" << "\"" << os.str() << "\"") ;
            }

            string localpath_norm = StringUtils::pathNormalize(localpath);
            Pair_Ftype_LocalPath mkey(filetype,localpath_norm);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"getRecRulesFromDb: Adding ( Localpath: " << localpath_norm<<
                  " - Filetype: " << filetype <<  ")") ;
            //check
            if ( File::exists(localpath_norm) ) {

                _mapReceptionRules.insert(std::make_pair(mkey,rule));
                _mapLocalPathToFTypes[localpath_norm].push_back(filetype) ;
                localpaths.push_back(localpath_norm);
                // Keep only the fastest scan period for each local path
                if ( _mapLocalPathToScanTime.find(localpath_norm) == _mapLocalPathToScanTime.end() )
                {
                    _mapLocalPathToScanTime[localpath_norm] = pollingPeriod;
                }
                else
                {
                    // Keep the minimum value in the scan map
                    _mapLocalPathToScanTime[localpath_norm] = std::min (pollingPeriod, _mapLocalPathToScanTime[localpath_norm]);
                }
            }
            else
            {
                ACS_LOG_WARNING( "Local path " << localpath_norm << " not exists! Skipping reception rule." );
            }

        }

        sort(localpaths.begin(), localpaths.end());
        vector<string >::iterator it;
        it = unique (localpaths.begin(), localpaths.end());
        localpaths.resize( it - localpaths.begin() );  // PRQA S 3000

    } catch (exception &e)
    {
        ACS_THROW(ImporterException(e, "Problem while loading DB values for reception rules"));
    }

}

namespace {

    template <typename T>
    void loadConfigurationParam(rsResourceSet &set, std::string const & tag, T & value, T const & , std::string const & = "" );


// TEMPLATE IMPLEMENTATIONS
template <typename T>
void loadConfigurationParam(rsResourceSet &set, std::string const & tag, T & value,  T const & defvalue , const std::string & unit)
{
    if (set.hasKey(tag)) {
        set.getValue(tag, value);
        ACS_LOG_PRIVINFO(tag << " set to: \""  << value << "\" " << unit) ;
    } else {
        ACS_LOG_PRIVINFO(tag << " DEFAULTED to: \"" <<  defvalue << "\" " << unit ) ;
        value = defvalue;
    }
}

// Partial specialization for boolean parameters
template <>
void loadConfigurationParam(rsResourceSet &set, std::string const & tag, bool & value,  bool const & defvalue , const std::string & unit)
{
    if (set.hasKey(tag)) {
        set.getValue(tag, value);
        ACS_LOG_PRIVINFO(tag << " set to: \"" << value << "\"") ;
    } else {
        ACS_LOG_PRIVINFO(tag << " DEFAULTED to: \"" << std::boolalpha << defvalue << "\" " << unit ) ;
        value = defvalue;
    }
}

}

void Importer::loadConf() // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    if ( NoStop != getCurrentStopMode() ) { return ; }

    string mainNamespace;
    vector<string> confSpaceList;
    this->getConfigurationSpaceInfo(mainNamespace, confSpaceList);

    for ( const auto & confSpace : confSpaceList ) {
        addSpace(ConfigurationSingleton::ConfSpace( confSpace ), ConfigurationSingleton::NoLoad) ;
    }

      loadConfiguration(false); //notifyReplacementException

    rotateLog();

    rsResourceSet &resultSet = ConfigurationSingleton::instance()->get() ;

    rsPushNSpace ImporterNS(resultSet, mainNamespace);

    loadConfigurationParam(resultSet, "BackupDir",   _backupDir, _backupDirDefault);
    loadConfigurationParam(resultSet, "ProcessDir",  _processDir, _processDirDefault); //base dir for production
    loadConfigurationParam(resultSet, "TaskTableDir",  _tasktableDir, _tasktableDirDefault); //base dir for production
    loadConfigurationParam(resultSet, "MaxThreads", _maxThreads, _maxThreadsDefault, "");
    loadConfigurationParam(resultSet, "PopulateInvAnomalies", _populateInvAnomalies, false);
    loadConfigurationParam(resultSet, "RestartTime", _restartTime, _defaultRestartTime, " seconds");
    loadConfigurationParam(resultSet, "LogInDefaultDir", _logInDefaultDir, false);

    // Rescan configuration
    loadConfigurationParam(resultSet, "PathScanPollingMultiplier", _pathScanPollingMultiplier, uint16_t(0), " times");
    loadConfigurationParam(resultSet, "MinimumScanPeriod", _minimumPathScanPeriod, 2 * _restartTime, " seconds");

    // basket for working dir in error
    loadConfigurationParam(resultSet, "WdsInErrorBasket", _basketForWdsInError, string());

    loadConfigurationParam(resultSet, "Clean_Working_Dir", _confCleanWorkingDir, _confCleanWorkingDirDefault);
    loadConfigurationParam(resultSet, "Copy_Input_Data"  , _confCopyInputData  , _confCopyInputDataDefault  );
    loadConfigurationParam(resultSet, "Scheduler_Path"   , _confSchedulerPath  , _confSchedulerPathDefault  );

    {
        std::vector<Importer::AdditionalExt> lAddExt ;

        ostringstream keyBase("AdditionalExtensions.extension");

        if ( resultSet.hasKey( keyBase.str() ) ) {

            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Additional extensions found in the configuration.") ;

            const size_t extTot = resultSet.getArraySize( keyBase.str() );
            for ( size_t i(0); i<extTot; ++i ) {
                ostringstream keyValue;
                ostringstream keyMandatory;
                keyValue     << keyBase .str() <<"["<< i <<"]";
                keyMandatory << keyValue.str() <<".mandatory.value";

                AdditionalExt ext ;
                resultSet.getValue( keyValue    .str(), ext.extension, true );
                resultSet.getValue( keyMandatory.str(), ext.mandatory, true );
                // add a '.' in front of the extension if there is not
                if (not ext.extension.empty()) {
                    if (ext.extension[0] != '.') {
                        ext.extension = "." + ext.extension ;
                    }

                    lAddExt.push_back(ext) ;
                }
            }
        }
        _additionalExtension.swap(lAddExt) ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"AdditionalExtensions found: " << _additionalExtension.size()) ;
    }

    loadConfigurationParam(resultSet, "ClusterName", _clusterName, Application::hostname());

    //check
    if (!File::exists(_backupDir) ) {
        ACS_LOG_WARNING("BackUp dir " + _backupDir + " does not exist; no backup input data if some error") ;

    }
    if (!File::exists(_processDir) ) {
        ACS_LOG_ERROR("Process dir " + _processDir+ " does not exist; create it.!") ;
    }
    if (!File::exists(_tasktableDir) ) {
        ACS_LOG_ERROR("Task table dir " + _tasktableDir+ " does not exist; create it!") ;
    }
    if( ( not _basketForWdsInError.empty() ) && ( not File::exists(_basketForWdsInError) ) ) {
        ACS_LOG_WARNING("Basket for working dirs in ERROR \"" + _basketForWdsInError+ "\" does not exist; please create it!") ;
    }

}

class ObserverPromoter // PRQA S 2109
{
public:
    ObserverPromoter (vector<string>& p, vector<string>&d) : _promote(p), _delete(d), _now(time(0)) {} // PRQA S 2528 2
    ObserverPromoter(const ObserverPromoter &r) : _promote(r._promote), _delete(r._delete), _now(r._now) {}
    ~ObserverPromoter() throw() {}

    void operator() (const pair <const string, time_t> & element)
    {
        // consider only elements with last modification time more than 2 seconds ago
        // others are simply skipped and managed later on
        if ((_now - element.second) > 2) // PRQA S 4400
        {
            // Ok, this file needs to be considered
            try
            {
                time_t curmtime = File::getModificationTime (element.first);
                // Two cases here.. same mtime -> it is time to promote it
                //                  differemt mtime -> it is open for writing and inotify will manage it upon closure
                if (curmtime == element.second)
                {
                    _promote.push_back (element.first);
                    _delete.push_back (element.first);
                }
                else
                {
                    _delete.push_back (element.first);
                }
            }
            catch (exception& ex)
            {
                // not able to compute modification time. Check if the file has been deleted and remove it from the list
                // otherwise check it again later
                if (!File::exists (element.first))
                {
                    _delete.push_back(element.first);
                }
            }
        }
    }
private:
    ObserverPromoter() ; // not implemented
    ObserverPromoter& operator= (const ObserverPromoter &) ; // not implemented
private:
    vector<string>& _promote;
    vector<string>& _delete;
    time_t _now;

};


class ObserverRemover // PRQA S 2109
{
public:
    explicit ObserverRemover (map<string, time_t>& m) : _map(m) {} // PRQA S 2528 2
    ObserverRemover(const ObserverRemover &r) : _map(r._map) {} // PRQA S 2528 2
    ~ObserverRemover() throw() {}

    void operator() (const string& s)
    {
        _map.erase (s);
    }
private:
    ObserverRemover() ; // not implemented
    ObserverRemover& operator= (const ObserverRemover &) ; // not implemented
private:
    map<string, time_t>& _map;
};

void Importer::promoteObservedFiles ()
{
    // Lock the internal structures to avoid interferences
    Lock lock (_internalQueueMutex);

    // Do nothing if the observation map is empty
    if (!_observationMap.empty())
    {
        vector<string> toPromote;
        vector<string> toDelete;
        for_each (_observationMap.begin(), _observationMap.end(), ObserverPromoter (toPromote, toDelete) );

        // got list to promote... promote it
        for_each (toPromote.begin(), toPromote.end(), InternalQueueMerger(this)); // PRQA S 3081

        // and also list to remove
        for_each (toDelete.begin(), toDelete.end(), ObserverRemover (_observationMap)); // PRQA S 3081

    }
}

// ************************************************************

void Importer::populateInventoryAnomaly (const string& pathname) const
{
    try
    {
        // Get a connection here
        dbConnectionWrapper pw(*getDefaultConnectionPool());
        dbConnection &conn = pw ; // PRQA S 3050

        // Populate anomaly table
        dbSmartPersistent anomaly("t_inventoryanomalies", conn);
        anomaly.set( "filename", File::getFileName (pathname), conn );
        anomaly.set( "fileversion", string("N/A"), conn ); // PRQA S 3081
        ostringstream message;
        message << "File " << pathname << " does not have any configured import rule";
        anomaly.set( "message", message.str(), conn );
        anomaly.set( "anomaly_type_id", AnomalyTypes::instance(conn.getKey())->id("FAULT"), conn );
        anomaly.save(conn);
    }
    catch (exception& ex)
    {
        ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_WARNING ("Unable to insert the inventory anomaly in the database for file " << pathname);
    }

}

void Importer::addToObservationMap (const string& path, time_t mtime)
{
    ThreadSafe::Lock lock (_internalQueueMutex);
    _observationMap[path] = mtime;
}

bool Importer::hasAdditionaExtension (string const &path) const
{
    for (auto && e: _additionalExtension) {
        if (StringUtils::endsWith(path, e.extension)) { // ends with the correct extension, but ...
            // avoid /.extension case
            if (not StringUtils::endsWith(path, "/"+e.extension)) {
                ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Skipping " << path << " because it has " << e.extension << " extension");
                return true ;
            }
        }
    }

    return false ;
}

void Importer::addToInternalQueue (const string& path)
{
    ThreadSafe::Lock lock (_internalQueueMutex);
    ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Entering add to queue. Current size: " << _internalQueue.size() << " in queue - " << _waitingFiles.size() << " - in waiting");
    // Skip items that will belong to additionaExtensions - they would picked up when the "root" file
    // will be discovered
    if (hasAdditionaExtension(path)) {
        ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Skipping a file because it has an add. ext. " << path);
        return ;
    }

    // Skip items that are currently under processing
    if ( _processingFiles.find (path) != _processingFiles.end() )
    {
        ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Skipping a file because it was in processing. " << path);
        return;
    }

    // Do not try to add to the queue files that have already been removed
    if (!File::exists (path) )
    {
        //You can be here for example if:
        // 1. The file has been manually removed just after the inode event
        // 2. Race condition:
        //    - The feeder generated inode event (MOVE_TO)
        //    - The processing starts and ends
        //    - The feeder generated another inode event on the same file (CLOSE_WRITE)
        // If this warning is logged (debug should be enabled), check if the feeder can be modified to generate only
        //  a registered inode event for each file. For example this happended on ImporterPHP when it:
        //  creates file F in path P, moves F to Inbox (MOVE_TO), closes F (CLOSE_WRITE). Fixed to do close in P, before the move.
        ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Queue discarded a file because do not exists anymore: " << path);
        return;
    }

    // Keeps the dataStruct information of the current file
    inDataStruct currentFileDataStruct;

    try
    {
        // This performs a database access to extract the filetype
        // associations on the given file
        currentFileDataStruct = file2DataStruct(path);
    }
    catch (exception& x)
    {
        // This is a function exit point
        // It was not possible to access the database information and the file cannot be evaluated
        // Notify the problem on the log and go ahead with the next one if any
        ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "file2DataStruct falied due to an exception:" << x);
        ACS_LOG_ERROR ("A database failure occurred. The file \"" << path << "\" has been left on disk. It will be re-evaluated if touched or at next application restart.");
        // Exit from the call with no effect
        ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Exiting add to queue. Current size: " << _internalQueue.size() << " in queue - " << _waitingFiles.size() << " - in waiting");
        return;
    }

    // remember a new event happened here
    // and we have the mutex right now
    _newEvents = true;
    _condition.broadcast();

    // This is inefficient and can be further optimized
    // DataStruct can be saved somewhere and accessed later
    // but only for valid products
    if ( currentFileDataStruct.isValid() == false)
    {
        _waitingFiles.enqueue (path);
        // Create inventory anomaly if needed
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File " << path << " with no configured rules; added to waiting list.") ;

        // Here the anomaly related to the file having no active rules is inserted in the database
        if (_populateInvAnomalies)
        {
            populateInventoryAnomaly (path);
        }
    }
    else
    {
        // It can be a second scan and the rules might be different
        // avoid getting files in both queue and waiting list
        _waitingFiles.removeElement (path);
        _internalQueue.enqueue (path);
        // Keep the inDataStruct safe in the dedicated cache
        // It will be cleaned when consuming from the queue
        _dataStructCache[path]=currentFileDataStruct;
    }

    ACS_CLASS_WRITE_DEBUG ( ACS_HIG_VERB, "Exiting add to queue. Current size: " << _internalQueue.size() << " in queue - " << _waitingFiles.size() << " - in waiting");
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(DirectoryInspector)


DirectoryInspector::DirectoryInspector(Importer* i) :
    Thread ("DirectoryInspector"),
    _rescanPeriods(),
    _jobsMap(),
    _jobQueue (),
    _pathScanPollingMultiplier(0),
    _minimumPathScanPeriod(0),
    _importer (i),
    _condition()
{
    ACS_CLASS_ANNOUNCE_DEBUG ( ACS_VLO_VERB );
}

DirectoryInspector::~DirectoryInspector() throw()
{
    ACS_CLASS_ANNOUNCE_DEBUG ( ACS_VLO_VERB );
    // Request cancel but do not join
    Thread::lazyStop (false);
    // Interrupt any active wait on the condition
    _condition.broadcast();
    // Exit and join now
    Thread::lazyStop(true);
}

// Configure rescan parameters
void DirectoryInspector::setScanPollingMultiplier (uint16_t multiplier)
{
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Setting scan multiplier to " << multiplier );
    // In case the mode has been modified update it
    if (_pathScanPollingMultiplier != multiplier)
    {
        ThreadSafe::Lock lock (_condition);
        _pathScanPollingMultiplier = multiplier;
        // Force also the rescan times to reflect the current settings
        forceRescanTime();
    }
}

void DirectoryInspector::setMinimumScanPeriod (uint32_t minPeriod)
{
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Setting minimum scanning period to " << minPeriod );
    // In case the mode has been modified update it
    if (_minimumPathScanPeriod != minPeriod)
    {
        ThreadSafe::Lock lock (_condition);
        _minimumPathScanPeriod = minPeriod;
        // Force also the rescan times to reflect the current settings
        forceRescanTime();
    }
}

void DirectoryInspector::forceRescanTime ()
{
    // First of all lock all the protected members
    ThreadSafe::Lock lock (_condition);

    // Then force an update of all the rescan times
    time_t currentTime = getCurrentMonotonicTime();

    // Loop on the jobs map
    for (auto it = _jobsMap.begin(); it != _jobsMap.end(); ++it)
    {
        // 0 means no rescans, otherwise schedule a new scan right now
        it->second = ( _pathScanPollingMultiplier == 0 ) ? 0 : ( currentTime + std::max (_minimumPathScanPeriod, _rescanPeriods[it->first] * _pathScanPollingMultiplier) );
    }
}

time_t DirectoryInspector::getCurrentMonotonicTime()
{
    struct timespec t ;

    if( (clock_gettime(CLOCK_MONOTONIC, &t) != 0) )
    {
        // should never occur.. return zero and warn
        ACS_LOG_WARNING ("Unable to get system time");
        return 0;
    }
    return t.tv_sec;
}

void DirectoryInspector::addPathToJobQueue ( const std::string& path, const uint32_t rescanPeriod)
{
    ACS_CLASS_ANNOUNCE_DEBUG ( ACS_VLO_VERB );

    if ( "" == path)
    {
        // empty path is not allowed
        ACS_LOG_WARNING ( "Asked to inspect a null path. Discarding it. Please check configuration" );
        return;
    }

    ThreadSafe::Lock lock (_condition);
    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "The following path will be evaluated " << path );

    // get the current time from system
    time_t currentTime = getCurrentMonotonicTime();

    // Verify if the path has already been submitted
    if (_jobsMap.find (path) == _jobsMap.cend())
    {
        ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "It is the first time that the path has been added [" << path << "] at time " << currentTime <<
                                " - rescan every " << rescanPeriod << " seconds");

        _jobsMap[path] = currentTime;
        _rescanPeriods[path] = rescanPeriod;

        // It is the first time this path has been added, the first scan is forced
        // But only if not currently scheduled
        if ( find (_jobQueue.begin(), _jobQueue.end(), path) == _jobQueue.end() )
        {
            ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Adding the path to the scan queue " << path );
            _jobQueue.push_back (path);
        }
    }
    else
    {
        // Must update the rescan period
        if (_rescanPeriods[path] != rescanPeriod)
        {
            ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Adjusting the rescan period for [" << path << "] to " << rescanPeriod << " seconds");
            _rescanPeriods[path] = rescanPeriod;
        }
    }

    // Do broadcast to unlock the running thread
    // Do not add the same path multiple times
    _condition.broadcast();
}

void DirectoryInspector::updateJobsQueue()
{
    ThreadSafe::Lock lock (_condition);
    // Get current time
    time_t currentTime = getCurrentMonotonicTime();

    for (auto it = _jobsMap.cbegin(); it != _jobsMap.cend(); ++it)
    {
        // Add every expired item, if not already under handling
        // and only if a rescan has been enabled
        if ( (it->second <= currentTime) && (it->second != 0) )
        {
            // But only if not currently scheduled
            if ( find (_jobQueue.begin(), _jobQueue.end(), it->first) == _jobQueue.end() )
            {
                ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Adding the path to the scan queue " << it->first );
                _jobQueue.push_back (it->first);
            }
        }
    }
}

void DirectoryInspector::run()
{
    while ( ! requestedCancel() )
    {
        {
            // Critical section
            ThreadSafe::Lock lock (_condition);
            while ( _jobQueue.empty() && ! requestedCancel() )
            {
                _condition.timedwait (10); // ATTENZIONE abilitare a 1000
                // Populate the jobsqueue if a timeout expires
                updateJobsQueue();
            }
        }

        // Lock has been released here.. check if needs to work
        if ( ! _jobQueue.empty() )
        {
                string curDir = _jobQueue.front();
                addAllFiles (curDir);
                // Manage protected structures
                {
                    ThreadSafe::Lock lock (_condition);
                    if (_pathScanPollingMultiplier == 0 )
                    {
                        // No rescan
                        _jobsMap[curDir] = 0;
                    }
                    else
                    {
                        // compute new rescan schedule, if applicable
                        _jobsMap[curDir] = getCurrentMonotonicTime() + std::max (_minimumPathScanPeriod, _rescanPeriods[curDir] * _pathScanPollingMultiplier );
                    }
                    ACS_CLASS_WRITE_DEBUG ( ACS_LOW_VERB, "Next rescan for " << curDir << " set at " << _jobsMap[curDir] );

                }
                // And cleanup the queue removing the front job
                _jobQueue.pop_front();
        }
    }
}



void DirectoryInspector::addAllFiles(const string & path)
{
    try {

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting scan of the path " << path );

        Dir  theDir(path);
        time_t now = time(0);

        bool emitInfo = ( theDir.size() > 0 );

        if (emitInfo)
        {
            ACS_LOG_PRIVINFO ("Starting backlog scan on path " << path << " having " << theDir.size() << " items");
        }

        for (size_t i=0; i < theDir.size() && !requestedCancel() ; ++i)
        {
            string filepath = path + "/" + theDir[i] ;

            ACS_CLASS_WRITE_DEBUG (ACS_HIG_VERB,"Managing file: " << filepath) ;

            if ( (i+1) % 2500 == 0 )
            {
                // Issue a log message every 1000 files read
                ACS_LOG_INFO ("Progress on backlog scan on path " << path << " - processed " << (i+1) << " ot of " << theDir.size() << " items");

            }

            try
            {
                // Split the stable (older than 5 seconds) and the potentially open files
                // and avoid managing files that do not exist anymore
                if (File::exists (filepath))
                {
                    time_t mtime = File::getModificationTime (filepath);
                    if ( (now-mtime) > 5 ) // PRQA S 4400
                    {
                        // more than 5 seconds
                        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"File: " << filepath << " is stable (older than 5 seconds) and will be added to the queue") ;
                        _importer->addToInternalQueue (filepath);
                    }
                    else
                    {
                        // otherwise needs to be placed under observation
                        // keep the modification time in a safe place
                        ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"File: " << filepath << " has been placed under observation. Mtime is: " << mtime) ;
                        _importer->addToObservationMap (filepath, mtime);
                    }
                }
            }
            catch (exception const& ee)
            {
                ACS_LOG_ERROR("Cannot manage file " << filepath << " - "  <<  ee) ;
            }
        }

        if (emitInfo)
        {
            ACS_LOG_PRIVINFO ("Completed backlog scan of path " << path );
        }

    }
    catch (exception const& e)
    {
        ACS_LOG_ERROR("Unable to scan path " << path << " due to - "  <<  e) ;

    }

}

SmartQueue::SmartQueue(const string& name, size_t sizeWarning, size_t dropLimit):
    _queueName (name),
    _sizeWarning (sizeWarning),
    _dropLimit (dropLimit),
    _sizeWarningEmitted (sizeWarning == 0),
    _dropWarningEmitted (dropLimit == 0),
    _theSet(),
    _theQueue(),
    _mutex()
{}

SmartQueue::~SmartQueue() throw()
{}

    // operations
void SmartQueue::enqueue (const string& e)
{
    Mutex::Lock lock (_mutex);
    if ( !hasElement (e) )
    {
        _theQueue.push_back (e);
        _theSet.insert (e);

        if ( !_sizeWarningEmitted && _theSet.size() > _sizeWarning)
        {
            _sizeWarningEmitted = true;
            ACS_LOG_WARNING ("The " << _queueName << " queue has more than " << _sizeWarning << " elements. Please check flows or reception rules.");
        }

        if ( _theSet.size() > _dropLimit && _dropLimit > 0)
        {
            dequeue();
            if ( !_dropWarningEmitted )
            {
                _dropWarningEmitted = true;
                ACS_LOG_ERROR ("The " << _queueName << " queue has more than " << _dropLimit << " elements and will drop elements. Some files will be ignored.");
            }
        }

    }

}

string SmartQueue::dequeue ()
{
    Mutex::Lock lock (_mutex);
    string retval;
    if ( !empty() )
    {
        retval = _theQueue.front();
        _theQueue.pop_front();
        // Commented to manage race conditions in between dequeue and processing
        // the dequeue MUST be completed with a removeElement
        // _theSet.erase (retval);
    }
    return retval;
}

bool SmartQueue::hasElement (const string& e) const
{
    Mutex::Lock lock (_mutex);
    bool retval = ( _theSet.find(e) != _theSet.end() );
    return retval;
}

void SmartQueue::removeElement (const string& e)
{
    Mutex::Lock lock (_mutex);
    if ( hasElement (e) )
    {
        _theSet.erase (e);
        std::deque<std::string>::iterator it = find (_theQueue.begin(), _theQueue.end(), e);
        if (it != _theQueue.end() )
        {
            _theQueue.erase ( find (_theQueue.begin(), _theQueue.end(), e) );
        }
    }
}

size_t SmartQueue::size() const
{
    Mutex::Lock lock (_mutex);
    return _theQueue.size();
}

bool SmartQueue::empty() const
{
    Mutex::Lock lock (_mutex);
    return _theQueue.empty();
}


// ************************************************************


_ACS_END_NAMESPACE




