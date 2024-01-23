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

#ifndef Importer_H_
#define Importer_H_

#include <exException.h>
#include <DiskEventNotifier.h>
#include <ProjectAppIncludes>
#include <Thread.h>

#ifdef HAS_APPNAME_DEFINITIONS
    #include <AppnameDefinitions>
#endif

#include <set>
#include <utility>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class TaskTableProcess;
class DiskEventNotifier;
class Importer;

// Implements a queue with efficient find
class SmartQueue
{
public:
    SmartQueue(const std::string&, size_t, size_t);
    virtual ~SmartQueue() throw();

    // operations
    void enqueue (const std::string&);

    // Dequeue extracts from the queue but not from the SET
    // the complete cleanup is performed by removeElement
    std::string dequeue ();

    bool hasElement (const std::string&) const;
    void removeElement (const std::string&);

    size_t size() const;
    bool empty() const;

protected:
SmartQueue ( const SmartQueue&);
SmartQueue& operator= (const SmartQueue&);

private:
    std::string _queueName;
    size_t _sizeWarning;
    size_t _dropLimit;
    bool _sizeWarningEmitted;
    bool _dropWarningEmitted;
    std::set<std::string> _theSet;
    std::deque<std::string> _theQueue;
    Mutex _mutex;
};

class DirectoryInspector ;

class Importer: // PRQA S 2109, 2153
     public PROJECT_APP
{
public:
    exDECLARE_EXCEPTION(ImporterException,exException) ; // PRQA S 2131, 2502

    Importer(std::string const& appName, std::string const& subSystem);
    virtual ~Importer() throw() ;
    virtual int main(int argc, char const * const * argv, char const * const * env);


    // Helper function accessed by DirectoryInspector
    void addToObservationMap (const std::string&, time_t);
    void addToInternalQueue  (const std::string&);

protected:
    /** Derived classes must declare proper configuration spaces to load.
     * mainNamespace: Namespace to access configuration keys (i.e. the root key to access configuration keys)
     * confSpaceList: list of configuration space to load */
    virtual void getConfigurationSpaceInfo( std::string & mainNamespace, std::vector<std::string> & confSpaceList ) = 0;

private:
    Importer();
    Importer(const Importer&);
    Importer& operator=(const Importer&);

    // returns true if any new work has been taken onboard
    bool doWork() ;

    void mainLoop() ;

    virtual void customSigTermHandler() ;
    virtual void customSigIntHandler() ;

    void manageStop();

    void checkDataAvailability(std::set <std::string> &, size_t, bool& ); //fill only
    void checkProcessThreads();

    void cleanExit();
    void stopImmediate();
    void getRecRulesFromDb( std::vector<std::string> &  );

    void loadConf() ;
    void restartConfiguration(std::unique_ptr<DiskEventNotifier>&, DirectoryInspector&, bool first_time= false);
    std::string getWorkingDirName(const std::string & filetype) const;

    void promoteObservedFiles();

    void populateInventoryAnomaly (const std::string& pathname) const;

    /**
     * in: fullpathfile: full pathname of the file triggering the import processor
     *     isPaired:     the full path file is paired with another file (like hdr+dbl scenario)
     * out: additionalFiles: additional files complementing fullpathfile (if configured)
     *      for example: fullpathfile = /import/aux_file.h5
     *                   configuration = mtd, mandatory; txt, optional
     *                   addidionalFiles = /import/aux_file.h5.mtd,
     *                                     /import/aux_file.h5.txt (if existing)
     *                   in case of hdr+dbl, the additionalFile is aux_file.mtd
     *  */
    bool collectAdditionalFiles( const std::string & fullpathfile, bool isPaired,
            std::vector<std::string> & additionalFiles, std::vector<std::string> & additionalFilesError ) ACS_NOEXCEPT ;

    // returns true if the path has an additional extension
    bool hasAdditionaExtension(std::string const &) const ;
private:


    struct RecRulesInfo // PRQA S 2173, 2175
    {
        RecRulesInfo():
            ttId(),
            ttParam(),
            procFailureAddress(),
            successNotificationAddresses(),
            satelliteId(),
            mission(),
            sensorId(),
            generateHeader(false)
        {}

        ~RecRulesInfo() throw() {}

        RecRulesInfo(const RecRulesInfo &r) :
            ttId(r.ttId),
            ttParam(r.ttParam),
            procFailureAddress(r.procFailureAddress),
            successNotificationAddresses(r.successNotificationAddresses),
            satelliteId(r.satelliteId),
            mission(r.mission),
            sensorId(r.sensorId),
            generateHeader(r.generateHeader)
        {}
        RecRulesInfo& operator= (const RecRulesInfo &r) {
            if (this != &r) {
                ttId = r.ttId ;
                ttParam = r.ttParam ;
                procFailureAddress = r.procFailureAddress ;
                successNotificationAddresses = r.successNotificationAddresses;
                satelliteId = r.satelliteId,
                mission = r.mission,
                sensorId = r.sensorId,
                generateHeader = r.generateHeader ;
            }
            return *this ;
        }

        // PRQA S 2100 8
        size_t ttId;
        std::string ttParam;
        std::string procFailureAddress;
        std::string successNotificationAddresses;
        std::string satelliteId;
        std::string mission;
        std::string sensorId;
        bool generateHeader;
    };

    struct inDataStruct // PRQA S 2173, 2175
    {
        inDataStruct() :
            fullpathname(),
            coupledPath(),
            filetype(),
            localpath(),
            procFailureAddress(),
            successNotificationAddresses(),
            ttParam(),
            satelliteId(),
            mission(),
            sensorId(),
            ttId(),
            valid(true)
        {}

        ~inDataStruct() throw() {}
        inDataStruct(const inDataStruct &r) :
            fullpathname(r.fullpathname),
            coupledPath(r.coupledPath),
            filetype(r.filetype),
            localpath(r.localpath),
            procFailureAddress(r.procFailureAddress),
            successNotificationAddresses(r.successNotificationAddresses),
            ttParam(r.ttParam),
            satelliteId(r.satelliteId),
            mission(r.mission),
            sensorId(r.sensorId),
            ttId(r.ttId),
            valid(r.valid)
        {}
        inDataStruct& operator= (const inDataStruct &r)
        {
            if (this != &r) {
                fullpathname = r.fullpathname ;
                coupledPath = r.coupledPath ;
                filetype = r.filetype ;
                localpath = r.localpath ;
                procFailureAddress = r.procFailureAddress ;
                successNotificationAddresses = r.successNotificationAddresses;
                satelliteId = r.satelliteId,
                mission = r.mission,
                sensorId = r.sensorId,
                ttParam = r.ttParam ;
                ttId = r.ttId ;
                valid = r.valid ;
            }
            return *this ;
        }

        bool isValid() const {return valid;}

        bool sameName(std::string val) const { return fullpathname == val;} // STL use passed by copy // PRQA S 2010

        // PRQA S 2100 L1
        std::string fullpathname; // full path
        std::string coupledPath; // full path of coupled file (if HDR/DBL)
        std::string filetype;
        std::string localpath;
        std::string procFailureAddress;
        std::string successNotificationAddresses;
        std::string ttParam;
        std::string satelliteId;
        std::string mission;
        std::string sensorId;
        int ttId;
        bool valid;
        // PRQA L:L1
    };

    typedef std::pair < std::string, std::string > Pair_Ftype_LocalPath ;

    inDataStruct file2DataStruct( std::string fullpath) const;
    std::pair < std::string, std::string > file2Key(const std::string & fullpath) const;


// Check if reference to reference is allowed by the compiler
#if __GNUC__ > 4 || \
              (__GNUC__ == 4 && (__GNUC_MINOR__ > 4 || \
                                 (__GNUC_MINOR__ == 4 && \
                                  __GNUC_PATCHLEVEL__ > 3)))
    static bool compareName( std::pair< inDataStruct, time_t> elem , const std::string& val) { return (elem.first).sameName(val) ;} // PRQA S 2010
    static bool IsAWaitedFile(std::pair< Pair_Ftype_LocalPath, RecRulesInfo>  elem , const std::pair < std::string, std::string >& val );
#else
#warning Reference to reference not allowed by the compiler using copy of parameters
    static bool compareName( std::pair< inDataStruct, time_t> elem , const std::string val) { return (elem.first).sameName(val) ;} // PRQA S 2010
    static bool IsAWaitedFile(std::pair< Pair_Ftype_LocalPath, RecRulesInfo>  elem , const std::pair < std::string, std::string > val );
#endif

  public:

    struct AdditionalExt {
        AdditionalExt() : extension(), mandatory() {}
        AdditionalExt(const AdditionalExt &) = default ;
        std::string extension ;
        bool mandatory ;
    } ;


  private:
    DiskEventNotifier::EventElementsList _eventsList;

    std::vector<TaskTableProcess *> _processThreads;


    //filetype - localpath -> tasktable path
    std::map< Pair_Ftype_LocalPath , RecRulesInfo>    _mapReceptionRules;
    std::map< std::string , std::vector<std::string> > _mapLocalPathToFTypes ;
    std::map< std::string, uint32_t> _mapLocalPathToScanTime;


    SmartQueue _waitingFiles; ////potential second files of an HDR/DBL couple
    std::list< std::pair< inDataStruct, time_t> > _toProcessDataFiles;


    static const int polling_freq=3000; // milliseconds

    static const unsigned int _defaultRestartTime;
    unsigned int _restartTime;

    bool _logInDefaultDir;

    static std::string _backupDirDefault;
    std::string _backupDir;
    static std::string _processDirDefault;
    std::string _processDir;
    static std::string _tasktableDirDefault;
    std::string _tasktableDir;

    static bool        _confCleanWorkingDirDefault;
    bool               _confCleanWorkingDir;
    static bool        _confCopyInputDataDefault;
    bool               _confCopyInputData;
    static std::string _confSchedulerPathDefault;
    std::string        _confSchedulerPath;

    static const unsigned int _maxThreadsDefault = 5;
    unsigned int _maxThreads;

    std::vector<Importer::AdditionalExt> _additionalExtension ;

    bool _populateInvAnomalies;

    std::vector<std::string> _currentWatchedPaths;

    ThreadSafe _internalQueueMutex;
    SmartQueue _internalQueue;
    std::map<std::string, time_t> _observationMap;

    Condition _condition;

    std::set<std::string> _processingFiles;

    bool _newEvents;

    // Keep info of evaluated files
    std::map <std::string, inDataStruct> _dataStructCache;

    // multiplication factor for restart times defaluted to 0
    uint16_t _pathScanPollingMultiplier;
    // minimum time for a complete rescan, defaulted to the restart time * 2
    // (avoid rescan just in the middle of a restart)
    uint32_t _minimumPathScanPeriod;

    std::string _clusterName;

    std::string _basketForWdsInError;

    static const unsigned int max_stored_files_threshold_= 60;
    static const unsigned int max_stored_files_lowlimit_ = 50;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(Importer) ;
};

_ACS_END_NAMESPACE

#endif /* Importer_H_*/
