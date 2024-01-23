// PRQA S 1050 EOF
/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2014/06/23 10:54:48  lucio.pulvirenti
	Test is over. Ready to satifsfy ADC-265.
	
	Revision 1.1.1.1  2014/06/20 15:22:31  lucio.pulvirenti
	Import FileVersioningTask
	
        
*/ 

#include <FileVersioningTask.h>
#include <FileVersioningTaskV.h>

#include <dbQueryFactoryChain.h>
#include <dbConnectionPool.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>

#include <Metadata.h>

#include <rsPushNSpace.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(FileVersioningTask)

static FileVersioningTaskV version;

const string FileVersioningTask::_confSpaceName = "TaskInPreInventory";

namespace {
	 
	bool nameMatches (const Metadata::NameValue &n)
	{
		bool matches = false;
  		if( n.name() == MetadataNameValues::getVersioning() ) {
			rsResourceSet::rsValue rsV(n.name());
			rsV.set(n.value());
			rsV.get(matches);
		}
		return matches;
	}

}

PROJECT_APP::GenericBuilder<FileVersioningTask> theApplicationBuilder("FileVersioningTask","DSD");

FileVersioningTask::FileVersioningTask(const std::string& subsys, const std::string& app):
    PROJECT_APP("FileVersioningTask","DSD"),
	_confSetw(0),
	_confSetFill('0')
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
}

FileVersioningTask::~FileVersioningTask() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


int FileVersioningTask::usage(const std::string &exename) const 
{    
    
	excerr << "Use: \n" ;

    excerr <<  exename << " <Metadata file>\n";

    return 0 ;
}

void FileVersioningTask::readConfiguration()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	addSpace(ConfigurationSingleton::ConfSpace(_confSpaceName),ConfigurationSingleton::NoLoad);
	loadConfiguration(false);  
	
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    rsPushNSpace confNs(conf, _confSpaceName ); 
	
    string confSetFillStr;
    conf.getValue("Versioning.Format.setw.value",    _confSetw);
    conf.getValue("Versioning.Format.setfill.value", confSetFillStr);
    ACS_COND_THROW( confSetFillStr.empty(), exFileVersioningTaskException("Versioning.Format.setfill is empty in \"" + _confSpaceName + "\" Configuration Space") ); // PRQA S 3081
    _confSetFill = confSetFillStr.at(0);
	
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "The following parameters got from configuration") ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_confSetw=" << _confSetw ) ;	 
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_confSetFill=" << _confSetFill ) ;	 
}


int FileVersioningTask::main(int argc, char const * const * argv, char const * const * env)
{

    const vector<string> &parsedArgs = getParsedArgs();
    ostringstream msg; msg<<"FileVersioningTask::main: Executing: ";
	if (parsedArgs.size() != 1) {
		internalusage(argv[0]) ;
		return 1 ;
    }

	string inputFile = parsedArgs[0];
	msg<< inputFile;

    ACS_LOG_INFO(msg.str());


    ostringstream pid;
    pid << getpid();    

    //create the log files the first time
    string logFullPath = applicationName() + ".log" ;
    //string logFullPath = defaultLogPath + "/" + applicationName() + ".log" ; // create the log path name
    // Rotate keeping 20 logs, compressed. 
    // The size has no effect as I don't call the rotate during the execution
    LogRotationPolicy lrp(logFullPath, LogRotationPolicy::FirstApplying, defaultLogAge, (1024*1024*1024), 20, true) ;
    setLRPolicy(lrp) ;
    rotateLog() ; // open the first log

    try {
        
        readConfiguration();
        execute(inputFile);

        return 0;                               
    } catch (exception &e) {
        ACS_LOG_ERROR(applicationName() << " : Exiting in ERROR...");
        ACS_LOG_NOTIFY_EX(e);
        try {
            exFEvent mcfAdvise("STATUS",EXFE_ERROR);
            excerr << mcfAdvise;
            excmcf << mcfAdvise;
            exFError msgErr;
            msgErr << applicationName()+" : Exiting in ERROR";
            excmcf << msgErr;
        } catch (exception& ee) {
            ACS_LOG_NOTIFY_EX(ee);
        }
        return(0xff);
    }
}

void FileVersioningTask::execute(const string & metadataFilename) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	Metadata metadata;
	try
	{
		metadata.readMetadataFile(metadataFilename);
		ACS_LOG_INFO("\"" << metadataFilename << "\" file successfully read");
	}
	catch(exception & e)
	{
		ACS_THROW(exFileVersioningTaskException(e, "Error reading Metadata file \"" + metadataFilename + '\"') ) ;
	}
	
	const vector<Metadata::NameValue> &nv = metadata.getNameValues() ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
    	ACS_LOG_DEBUG(SimpleDebugSignature << "Optional name values present: " << boolalpha << (not nv.empty()) ) ;
    	for (size_t i=0; i < nv.size() ; ++i) {
        	ACS_LOG_DEBUG(SimpleDebugSignature << '[' << i << "]: |" << nv[i].name() << "| |" << nv[i].value() << '|') ;
    	}
	ACS_CLASS_END_DEBUG
			
	if( find_if(nv.begin(), nv.end(), nameMatches) != nv.end() ) {
		ACS_LOG_INFO("Going to perform versioning");
		dbConnectionWrapper connWrp ( *(Application::instance()->getDefaultConnectionPool()));
		dbConnection& conn (connWrp);
		doVersioning( metadata, metadataFilename, conn);
	}
	else {
		ACS_LOG_INFO("Versioning not to be performed: EXITING");
	}
	
	
	ACS_LOG_INFO("FileVersioningTask: Files processing is over.") ;

}


void FileVersioningTask::doVersioning(Metadata& metadata, const string & mtdFileName, dbConnection &conn) const
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    //Execute Query
    dbInventoryObjectQuery* dbInvObjectQuery =dynamic_cast<dbInventoryObjectQuery*>( dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", conn.getKey()) ); // PRQA S 3081, 4412
	ACS_COND_THROW( !dbInvObjectQuery, exFileVersioningTaskException("Error in executing dbInventoryObjectQuery") );	 // PRQA S 3081
	dbGenQueryScopeGuard queryGuard(dbInvObjectQuery);
	
	string filename = metadata.getGlobalFileName();
	ACS_COND_THROW( filename.empty(), exFileVersioningTaskException("Filename field from metadata is empty") );	 // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File name from metadta: \"" << filename << '\"' ); 
	
	const string &dbKey = conn.getKey() ;
    dbQueryParameters parameters(dbKey) ; 
    parameters.addParameter(dbParam(dbKey, "t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey, filename))) ; // PRQA S 3050, 3081
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to query t_inventory record with the following condition: " << parameters.getSqlString());

    dbInvObjectQuery->addParameter( parameters );

    unsigned long int countRow = dbInvObjectQuery->doQuery(conn);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "num = dbInventoryObjectQuery->doQuery() = " << countRow ); 
	    
	string newVersion;
    if( countRow > 0) {

        int maxVers = std::numeric_limits<int>::min();
		
		string currentVersion = metadata.getMetadataValue( "File_Version");
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Current file version from metadata: \"" << currentVersion << '\"' ); 
		
		bool found = false;
        for(unsigned long int i=0; i<countRow; i++) {

            string version = ((*dbInvObjectQuery)[i])->getFileVersion();
            ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "dbInvObjectQuery->getFileVersion-> version = " << version ); 
			if( version == currentVersion ) {
				found = true;
            	ACS_LOG_INFO("t_inventory entry with filename \"" << filename << "\" and version \"" << currentVersion << "\" already occurring in db") ; 
			}
			
			{
				int vers;
				istringstream iss(version);
				iss >> vers;
            	maxVers = max(vers, maxVers) ; 
			}

        }	
		
		if( found ) {
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "maxVers = " << maxVers ); 
        	int newVers = maxVers+1;
			
			ostringstream os; 
    		os << setw(_confSetw) << setfill(_confSetFill) << newVers;
    		newVersion = os.str();

    		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New Formatted version \"" << newVersion << "\"");

		}
		else {
            ACS_LOG_INFO("No t_inventory entry with filename \"" << filename << "\" has version \"" << currentVersion << "\": no new Version required." ); 
		}

    } 
	else {
       ACS_LOG_INFO("No t_inventory entry with filename \"" << filename << "\" occurs in db: no new Version required." ); 
    }

    if( not newVersion.empty() ) {
		
		metadata.setMetadataValue( "File_Version", newVersion);
		// QUALITY_INFO
		string qualityInfo = "DUP";
		metadata.setMetadataValue( "Quality_Info", qualityInfo);
		
		metadata.writeMetadataFile(mtdFileName);
	
    	ACS_LOG_INFO("Metadata File_Version field updated with the value \"" << newVersion << "\" and quality_Info field set to \"" << qualityInfo << '\"');

	}
}

_ACS_END_NAMESPACE
