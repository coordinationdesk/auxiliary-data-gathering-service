/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. RemoveInventoryObject $

	$Id$

	$Author$

	$Log$
	Revision 1.20  2016/03/29 17:14:09  marpas
	adopting new db I/F
	
	Revision 1.19  2015/07/08 12:59:00  damdec
	Log macros used to fix compilation warnings.
	
	Revision 1.18  2013/04/09 09:04:31  marpas
	new Statistics
	
	Revision 1.17  2013/03/14 16:00:06  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 1.16  2013/03/12 17:59:19  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 1.15  2013/02/21 09:29:08  enrcar
	EC:: options separator is now --, like most of unix commands
	
	Revision 1.14  2013/02/21 08:46:30  enrcar
	EC:: QAreports improved
	
	Revision 1.13  2013/02/21 08:23:38  enrcar
	EC:: exFMessage used for filterable / info messages
	
	Revision 1.12  2013/02/20 10:55:37  enrcar
	EC:: Application refactoring
	
	Revision 1.11  2012/02/20 14:08:27  marpas
	minor changes
	
	Revision 1.10  2011/10/26 09:53:50  masdal
	changed forceStatistics with enableStatics
	
	Revision 1.9  2011/09/22 13:32:47  marpas
	S1PDGS-1904 closure
	
	Revision 1.8  2009/09/28 16:57:53  marpas
	dbParam interface obsolecence
	
	Revision 1.7  2009/02/19 12:36:03  marpas
	eless libraries removed
	
	Revision 1.6  2008/11/25 17:28:16  danalt
	removed MergeCOnfiguration header
	
	Revision 1.5  2005/07/15 14:29:33  ivafam
	Manage multi storage manager
	
	Revision 1.4  2005/06/22 10:28:45  enrcar
	using namespace std added
	
	Revision 1.3  2005/06/15 13:35:26  integrator
	Added libStorageManager
	
	Revision 1.2  2005/05/31 15:39:06  marpas
	file action interface changed
	
	Revision 1.1.1.1  2005/02/23 12:45:49  ivafam
	Import RemoveInventoryObject 
	
	
*/


#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
	#include <StationApp.h>					
#elif COSMO
	#include <CosmoApp.h>					
#elif CAL_VAL_APP
	#include <CalValApp.h>					
#else
    #include <CryosatApp.h>
#endif

#include <RemoveInventoryObject.h>
#include <File.h>
#include <dbInventoryObjectQuery.h>
#include <dbConnectionPool.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <Statistics.h>
#include <Filterables.h>

#include <riV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RemoveInventoryObject)

namespace { // unnamed
    riV version;

    Application::GenericBuilder<RemoveInventoryObject> theApplicationBuilder("RemoveInventoryObject", "SDF") ;
}

/***************************************************************************************
**
                                RemoveInventoryObject 
**
***************************************************************************************/
RemoveInventoryObject::RemoveInventoryObject(const string &appName, const string &subSystem) :
#if defined (HAS_MISSION_INCLUDES)
	PROJECT_APP(appName,subSystem) 
#elif GENERIC_STATION_APP
	StationApp(appName,subSystem)
#elif COSMO
	CosmoApp(appName,subSystem) 
#elif CAL_VAL_APP
	CalValApp(appName,subSystem)  
#else
	CryosatApp(appName,subSystem) 
#endif
	, 
	_removeExt_firstdot(false), 
	_removeExt_lastdot(false)
{
}


/***************************************************************************************
**
                                ~RemoveInventoryObject 
**
***************************************************************************************/
RemoveInventoryObject::~RemoveInventoryObject() throw() 
{
}


/***************************************************************************************
**
                                     usage 
**
***************************************************************************************/
int RemoveInventoryObject::usage(const string& exeName) const
{
	excerr << "Usage: " << applicationName() << " [ std-opts ] [ specific-opts ]  Inventory1  [Inventory2] ...\n" ;
	excerr << "Specific options:\n" ; 
	excerr << "\t--remove_ext : removes filename extension, starting from the last \"dot\"\n" ;
	excerr << "\t--remove_first_ext : removes filename extension, starting from the first \"dot\"\n" ;
	excerr << "\t--enablestat : default stats are disabled, this option allow them\n" ;
	excerr << "\t--no_remove_ext : KEEPING THE EXTENSIONS IS THE DEFAULT BEHAVIOUR; this option is provided for legacy compatibility\n" ;
	return 1 ;
}


/***************************************************************************************
**
                                      doWork
**
***************************************************************************************/
int RemoveInventoryObject::doWork(const vector<string>& fileNames) // PRQA S 4020
{    
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
 	for (size_t i=0; i < fileNames.size(); i++) {
		dbInventoryObjectQuery * theQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey));

		dbGenQueryScopeGuard theQueryScopeGuard(theQuery) ;
		dbQueryParameters parameters(dbKey) ; 
		
		string FileName ;
		
		if (_removeExt_lastdot) {
		
			FileName = File::getFileName(File::removeExt(fileNames[i])) ;
		
		} else if (_removeExt_firstdot) {
		
			FileName = File::getFileName(File::removeFExt(fileNames[i])) ;
		
		}
		else {
		
			FileName = fileNames[i];
		
		}
		
		{	
			exFMessage msg ; msg << "Querying for \"" << FileName << "\"" << "\n" ;
			excerr << msg ;
		}
		
		parameters.addParameter(dbParam(dbKey, "t_inventory","filename", dbParam::EQUAL_TO, db::toSQL(dbKey,FileName))) ;
		theQuery->addParameter(parameters) ;
		theQuery->doQuery(theConnection) ;
		
		{	
			ACS_LOG_INFO(theQuery->size() << " entries found." << "\n" );
		}
				
		try {
   			
			for (size_t j=0; j < theQuery->size(); j++) {
                
				dbFileAction* eraser = dbFileActionFactoryChain::instance()->newEraseAction(*getDefaultConnectionPool(), *(*theQuery)[j]);
				dbFileActionScopeGuard guard(eraser);
				eraser->start();

				{
					ACS_LOG_INFO((*theQuery)[j]->getFileName() << " " << (*theQuery)[j]->getFileVersion() << "\n" );
				}

				(*theQuery)[j]->remove(theConnection) ;
				{
					ACS_LOG_INFO("*** REMOVED ***" << "\n" );
				}
			}
		}
		catch(exception &e) {

			ACS_LOG_ERROR( "*** FATAL  --  EXCEPTION RECEIVED ***\n" << e );
			
			return 1 ;
		}
		
	}

	return 0 ;
} 


/***************************************************************************************
**
                                          main
**
***************************************************************************************/
int RemoveInventoryObject::main(int, char const * const *, char const * const *) // PRQA S 5520
{
    math::Statistics::instance()->enableStatistics(false) ;
	_removeExt_firstdot = false ;
	_removeExt_lastdot = false ;
  
	const vector<string> &parsedArgs = getParsedArgs();
	vector<string> fileNames ;

	if (parsedArgs.size() > 0)
	{

		for (size_t i=0; i < parsedArgs.size(); i++) {
			if ("--remove_first_ext" == parsedArgs[i]) {
				_removeExt_firstdot = true; _removeExt_lastdot = false ; 
			}
			else if ("--remove_ext" == parsedArgs[i]) { 
				_removeExt_firstdot = false; _removeExt_lastdot = true ; 
		    }
			else if (("--no_remove_ext" == parsedArgs[i])||("-no_remove_ext" == parsedArgs[i])) {
				_removeExt_firstdot = _removeExt_lastdot = false ; 
		    }
            else if(parsedArgs[i] == "--enablestat") {
				math::Statistics::instance()->enableStatistics(true) ;
            }
            else fileNames.push_back(parsedArgs[i]) ;
	    }

    }	// END if (parsedArgs.size() > 0)

	
	int status = 1;
	
    if ( fileNames.size() == 0 ) {
        internalusage(appName()) ;	/* No filename provided */
        status = 1 ;
    }
	else
	{
		status = doWork(fileNames) ;
	}	
	
	{
		ACS_LOG_INFO("\nExecution finished with " << (status? "invalid ":"") << "status code: " << status << "\n") ;
	}
	
	return status ; 
}


_ACS_END_NAMESPACE
