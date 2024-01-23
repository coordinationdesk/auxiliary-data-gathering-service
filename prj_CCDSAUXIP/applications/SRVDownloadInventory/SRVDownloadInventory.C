/*

	Copyright 2020, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. SRVDownloadInventory $
	

*/


#include <SRVDownloadInventory.h>
#include <dbConnectionPool.h>
#include <LTAInventoryObject.h>
#include <dbDownloadFile.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <Statistics.h>
#include <DateTime.h>
#include <File.h>

#include <sdiV.h>


_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVDownloadInventory)

namespace {
    static sdiV version;
}

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef SRVDownloadINVENTORY_SUBSYSTEM
#define SRVDownloadINVENTORY_SUBSYSTEM "SRV"
#endif

#ifndef SRVDownloadINVENTORY_APPNAME
#define SRVDownloadINVENTORY_APPNAME "SRVDownloadInventory"
#endif 

Application::GenericBuilder<SRVDownloadInventory> theApplicationBuilder(SRVDownloadINVENTORY_APPNAME, SRVDownloadINVENTORY_SUBSYSTEM) ;

using std::string ;
using std::ostringstream ;
using std::vector ;

namespace {
    double speed_Mb_sec(off_t size, double msec) 
    {
        return size/(msec*1024*1.024) ; // bytes /1024*1024   msec/ 1000  
    }

    class FilterMsgs : public exFilter 
    {
    public: 
	    FilterMsgs(exostream *s) : exFilter(s) {}
	    virtual ~FilterMsgs() = default ;

	    virtual bool filterInsertion(exFilterable &fo) 
        {
	        exFEvent *mfo = dynamic_cast<exFEvent *>(&fo) ;

	        return (0 == mfo) ;
        }
    } ;

}

// Constructor
SRVDownloadInventory::SRVDownloadInventory(const string &appName, const string &subSystem) :
	PROJECT_APP(appName,subSystem),
    _targetDir(),
    _totalSize(),
    _total_msec()
{
}

int SRVDownloadInventory::usage(const string &exeName) const
{

	excerr << "Usage: " << applicationName() << " [ std-opts ] [ specific-opts ] {--name|--uuid|--id} <prod1> ... \n" ;
	excerr << "\t--name : prod1 ... are product names\n" ;
	excerr << "\t--uuid : prod1 ... are uuids\n" ;
	excerr << "\t--id   : prod1 ... are product ids\n" ;
	excerr << "Specific options:\n" ; 
	excerr << "\t-d <directory> : give the path to store retrieved inventories\n";
	excerr << "\t-l <logfile>   : writes the lgo in specified file instead than default one\n" ;
	return 1 ;
}

string SRVDownloadInventory::getDefaultLogName () const
{
	DateTime currTime ;
	string str ;
	ostringstream oo;
	oo << this->defaultLogPath << "/LOG_SRV_" << applicationName() << "_" << currTime.asISO8601(3) << ".log" ;
	return oo.str() ;
}


namespace {             ;
    bool checkAndTransform(vector<string> const &prods, vector<dbPersistent::IdType> &ids) 
    {
        ids.clear() ;
        for (auto&& p : prods) {// access by f-d reference, the type of i is const int&
            char *end = 0 ;
            ids.push_back(strtoll(p.c_str(), &end, 10));
            if (*end != 0) {
                excerr << "\n\"" << p << "\" is not a valid id\n\n" ;
                return false ;
            }
        }
        return true ;
    }
}



int SRVDownloadInventory::main(int argc, char const * const * argv, char const * const * env)
{
    math::Statistics::instance()->enableStatistics(false) ;

	const vector<string> &parsedArgs = getParsedArgs();
	vector<string> prods ;

	int retcode = 0 ;

    string nameLog ;
    _targetDir = "." ;

    bool argsAreNames = false ;
    bool argsAreUUids = false ;
    bool argsAreIds = false ;
    short exclusiveOpts = 0 ;
    
	if (parsedArgs.size() > 0)
	{
        
		for (size_t i=0; i < parsedArgs.size(); i++) {
			if(parsedArgs[i] == "--enablestat") {
				math::Statistics::instance()->enableStatistics(true) ;
            }
            else if (parsedArgs[i] == "--name") {
                argsAreNames = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--uuid") {
                argsAreUUids = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "--id") {
                argsAreIds = true ;
                exclusiveOpts ++ ;
            }
            else if (parsedArgs[i] == "-l") {
                ++i ;
                bool valid = false;
                if (i < parsedArgs.size()) {
                    nameLog = parsedArgs[i] ;
                    if ( (not nameLog.empty()) && (nameLog[0] != '-') )
				    {
					    valid = true ;
				    }
                }
                
                if (not valid) {
                    excerr << "\ninvalid logfile\n" ;
                    internalusage(appName()) ;
                    return 1;                     
                }
            }
            else if (parsedArgs[i] == "-d") {
                ++i ;
                bool valid = false;
                if (i < parsedArgs.size()) {
                    _targetDir = parsedArgs[i] ;
                    if ( (not _targetDir.empty()) && (_targetDir[0] != '-') )
				    {
					    valid = true ;
				    }
                }
                
                if (not valid) {
                    excerr << "\ninvalid target directory\n" ;
                    internalusage(appName()) ;
                    return 1;                     
                }
            }
            else {
                prods.push_back(parsedArgs[i]) ;
	            ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Adding prod \"" << parsedArgs[i] << "\"");
            }
	    }


    }	// END if (parsedArgs.size() > 0)

    if (exclusiveOpts != 1) {
        excerr << "\none of (--name, --uuid, --id) must be provided \n\n" ;
        internalusage(appName()) ;
        return 1; 
    }
    
    if (prods.empty()) {
        excerr << "\nno products given \n\n" ;
        internalusage(appName()) ;
        return 1; 
    }
        
	if (not nameLog.empty()) // Must override the log name
	{
		// Disable automatic log management
		setRotateLogParametersFromConf (false);

		// Instruct the LRP to override the logfilename
		LogRotationPolicy& lrp = getLRPolicy();

      	string logname = File::getFileName(nameLog) ;
        string logdir = File::getDirPath(nameLog) ;
		if (logdir.empty()) { logdir = "." ; }

		lrp.forceLogName (logdir + "/" + logname);
	}

	ACS_LOG_INFO ("Current log file is " << getLRPolicy().getCurrentLogName() );

	try
	{
		loadConfiguration() ;
	}
	catch(std::exception &e)
	{
		ACS_THROW(exCriticalException(e,"Configuration load error")) ;
	}

	rotateLog() ; // open the first log
    
    if (argsAreNames) {
        retcode = downloadSet(queryNames(prods)) ;
    }
    else if (argsAreUUids) {
        retcode = downloadSet(queryUUIDs(prods)) ;
    }
    else if (argsAreIds) {
        vector<dbPersistent::IdType> ids ;
        if (!checkAndTransform(prods, ids)) {
            internalusage(appName()) ;
            return 1; 
        }
        retcode = downloadSet(ids) ;
    }	

    ACS_LOG_INFO("Speed: " << std::setw(6) << std::setprecision(1) << std::fixed <<  speed_Mb_sec(_totalSize, _total_msec) << " [MB/sec] total") 
	
	ACS_LOG_INFO("Execution finished with " << (retcode? "invalid ":"") << "status code: " << retcode) ;
	
	return retcode ; 
}



vector<db::LTAInventoryObject> SRVDownloadInventory::queryNames(vector<string> const &names) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

    vector<db::LTAInventoryObject> objects ;
    for (auto n: names) {
        if(NoStop != getCurrentStopMode()) {
            return vector<db::LTAInventoryObject>() ;  // empty set
        }
        
        db::LTAInventoryObject obj(theConnection);

	    if (obj.loadByUniqueKey(theConnection,"name",n)) {
            objects.push_back(obj) ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Name: " << n << " not found");
        }
    }

	return objects ;
}

vector<db::LTAInventoryObject> SRVDownloadInventory::queryUUIDs(vector<string> const &uuids) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

    vector<db::LTAInventoryObject> objects ;
    for (auto u: uuids) {
        if(NoStop != getCurrentStopMode()) {
            return vector<db::LTAInventoryObject>() ;  // empty set
        }
        db::LTAInventoryObject obj(theConnection);

	    if (obj.loadByUniqueKey(theConnection,"uuid",u)) {
            objects.push_back(obj) ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UUID: " << u << " not found");
        }
    }

	return objects ;
}

int SRVDownloadInventory::downloadSet(vector<dbPersistent::IdType>const & ids) // PRQA S 4020
{    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing " << ids.size() << " inventories");
    
 	for (auto id : ids) {
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing id: " << id);
  	    
        downloadInventory(id, theConnection) ;
	}

	return 0 ;
} 

int SRVDownloadInventory::downloadSet(vector<db::LTAInventoryObject>const & objects) // PRQA S 4020
{    
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing " << objects.size() << " inventories");
    
 	for (auto obj : objects) {
        if(NoStop != getCurrentStopMode()) {
            return 0; 
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing id: " << obj.getId());
  	    
        downloadInventory(obj, theConnection) ;
	}

	return 0 ;
} 


int SRVDownloadInventory::downloadInventory(db::LTAInventoryObject & obj, dbConnection &theConnection) // PRQA S 4020
{ 
    FilterMsgs myfilter1(&excerr) ;	
    Timer timer ; 
    timer.start() ;   
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    if(NoStop != getCurrentStopMode()) {
        return 0; 
    }
	
    if (not obj.getValidityFlag(theConnection)) {
        ACS_LOG_WARNING("Skipping " << obj.getName(theConnection) << " is not valid") ;
        return 1 ; 
    }

    string path = _targetDir + "/" + obj.getName(theConnection) ; 
    dbFileAction *fa = dbFileActionFactoryChain::instance()->newDownloadAction(*getDefaultConnectionPool(), obj, path, appName()) ;
    dbDownloadFile* LTADownloader = dynamic_cast<dbDownloadFile*>(fa) ; 

	ACS_COND_THROW(!LTADownloader, exIllegalValueException("no dbDownloadFile for LTAInventoryObject")) ;
					
    dbFileActionScopeGuard guard(LTADownloader);

    LTADownloader->start() ;
    ACS_LOG_INFO("Downloading " << obj.getName(theConnection)) ; 
    do {
		LTADownloader->waitExitOrTimeout(1000) ;
	} while(LTADownloader->downloading()) ; 
    
    timer.stop() ;
    Timer::Delay msec = timer.elapsed() ;
    off_t size = File::size(path) ;
    _totalSize += size ;
    _total_msec += msec ; 
    ACS_LOG_INFO("Speed: " << std::setw(6) << std::setprecision(1) << std::fixed <<  speed_Mb_sec(size, msec) << " [MB/sec]") 
	return 0 ;
} 

int SRVDownloadInventory::downloadInventory(dbPersistent::IdType id, dbConnection &theConnection)
{   
    db::LTAInventoryObject obj(theConnection);

	if (not obj.load(theConnection , id)) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << id << " not found");
        return 0 ;
    }

    return downloadInventory(obj, theConnection) ;
}


_ACS_END_NAMESPACE
