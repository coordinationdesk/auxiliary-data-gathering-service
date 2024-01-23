/*

	Copyright 1995-2019, Exprivia SPA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. MultiRemoveInventoryObject $

	$Id$

	$Author$

	$Log$
	Revision 1.22  2016/03/29 17:11:59  marpas
	adopting new db I/F
	
	Revision 1.21  2015/11/03 14:50:30  damdec
	Configuration loaded.
	
	Revision 1.20  2013/05/12 14:53:49  marpas
	using db connection key
	
	Revision 1.19  2013/04/09 09:03:18  marpas
	new Statistics
	
	Revision 1.18  2013/03/15 13:11:54  chicas
	Using getDefaultConnectionPool
	
	Revision 1.17  2013/03/12 17:59:40  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 1.16  2013/02/28 16:53:36  enrcar
	EC:: versioning renamed
	
	Revision 1.15  2013/02/26 15:12:25  enrcar
	EC:: Application updated
	

	
	
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

#include <MultiRemoveInventoryObject.h>
#include <File.h>
#include <dbInventoryObjectQuery.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbInventoryObject.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbSchemaMetaDataProvider.h>
#include <dbConnectionPool.h>
#include <Statistics.h>
#include <Filterables.h>

#include <miV.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MultiRemoveInventoryObject) 

namespace { // unnamed
    miV version;

    Application::GenericBuilder<MultiRemoveInventoryObject> theApplicationBuilder("MultiRemoveInventoryObject", "SDF") ;
    static const string REMOVE_OPERATION = "MultiRemoveAction";
    static const string THRESHOLD_PARAM = "MultiRemoveInventoryObject.QueryThreshold"; 
}

/***************************************************************************************
**
                                MultiRemoveInventoryObject 
**
***************************************************************************************/
MultiRemoveInventoryObject::MultiRemoveInventoryObject(const string &appName, const string &subSystem) :
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
	, \
	_verbose(false),	\
	_paramsMap(),	\
	_usedParams()	\
{
}


/***************************************************************************************
**
                                ~MultiRemoveInventoryObject 
**
***************************************************************************************/
MultiRemoveInventoryObject::~MultiRemoveInventoryObject() throw() 
{
}


/***************************************************************************************
**
                                     usage 
**
***************************************************************************************/
int MultiRemoveInventoryObject::usage(const string& exeName) const
{

	excerr << "Usage: " << applicationName() << " [ std-opts ] [ specific-opts ]  arguments\n" ;
	excerr << "Specific options (all the options can be COMBINED. Logic operator AND will be applied):\n" ; 
	excerr << "\t--filetype\t[FileType1]  [FileType2]...\t: List of filetypes to be included\n" ;
	excerr << "\t--nofiletype\t[FileType1]  [FileType2]...\t: List of filetypes to be excluded\n" ;
	excerr << "\t--source\t[Source1]    [Source2]...\t: List of sources to be included\n" ;
	excerr << "\t--nosource\t[Source1]    [Source2]...\t: List of sources to be excluded\n" ;
	excerr << "\t--filename\t[FileName1]  [FileName2]...\t: List of filenames to be included\n" ;
	excerr << "\t--nofilename\t[FileName1]  [FileName2]...\t: List of filenames to be excluded\n" ;
	excerr << "\t--date\t\t[<lowest_date>]  <highest_date>\t\t: Range for both start/stop validity in UTC format: e.g.  19991231T000000  19991231T235959\n" ;
	excerr << "\t--where\t\t<SQL where condition>\n" ;
	excerr << "\t--all\t\t\t\t\t\t: remove all stored files\n" ;
	excerr << "\t--enablestat\t\t\t\t\t\t: enable statistics\n" ;
	excerr << "\t--verbose\t\t\t\t\t: be verbose\n" ;
	excerr << "\t--demo\t\t\t\t\t\t: demo mode (no file will be removed)\n" ;
	return 1 ;
}


/***************************************************************************************
**
                                      dumpParameters
**
***************************************************************************************/
void MultiRemoveInventoryObject::dumpParameters() 
{

	for (size_t i=0; i<_usedParams.size(); i++)
	{
		bool print_vector = false ; 
		switch (_usedParams[i])
		{
			case FTYPE :
			{
				exFMessage msg ; msg << "Option:  --filetype\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case XFTYPE :
			{
				exFMessage msg ; msg << "Option:  --nofiletype\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case SOURCE :
			{
				exFMessage msg ; msg << "Option:  --source\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case XSOURCE :
			{
				exFMessage msg ; msg << "Option:  --nosource\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case FNAME :
			{
				exFMessage msg ; msg << "Option:  --filename\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case XFNAME :
			{
				exFMessage msg ; msg << "Option:  --nofilename\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case DATE :
			{
				exFMessage msg ; msg << "Option:  --date\n" ; excerr << msg ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					DateTime d( "UTC="+v[j] ) ; 
					exFMessage msg ; msg << "\t\tUTC=" << d.YYYYMMDDThhmmss() << "  (" << d.getSqlString() << ")\n" ; excerr << msg ; 
				}

				print_vector = false ;
				break ; 
			}

			case WHERE :
			{
				exFMessage msg ; msg << "Option:  --where\n" ; excerr << msg ; 
				print_vector = true ;
				break ; 
			}

			case ALL :
			case ALL_NOW :
			{
				exFMessage msg ; msg << "Option:  --all\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}

			case DEMO :
			{
				exFMessage msg ; msg << "Option:  --demo\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}
			default:
			{
				print_vector = false ;
			}
			break ; 

		}	// END: switch

		if (print_vector)
		{
			exFMessage msg ; msg << "Arguments :\n" ; excerr << msg ; 
			const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
			for (size_t j=0; j<v.size(); j++)
			{
				exFMessage msg ; msg << "\t\t\"" << v[j] << "\"\n" ; excerr << msg ; 
			}
		}
	}	// END: for

}


/***************************************************************************************
**
                                      doWork
**
***************************************************************************************/
int MultiRemoveInventoryObject::doWork() // PRQA S 4020
{    
	bool demo = false ; 
	bool found = false ; 
	bool removeall = false;
	
	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	dbInventoryObjectQuery * theQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey));
	dbGenQueryScopeGuard theQuesryScopeGuard(theQuery) ;


	for (size_t i=0; i<_usedParams.size(); i++)
	{
	
		switch (_usedParams[i])
		{
//FIXME			enum Cmdparam { NOARGS=0x00, FTYPE, XFTYPE, SOURCE, XSOURCE, FNAME, XFNAME, DATE, WHERE, ALL, ENSTATS, DEMO } ; 
			case FTYPE:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::OR ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "filetype", dbParam::LIKE, db::toSQL(dbKey, v[j] )) ) ;
				}
				
				theQuery->addParameter(parameters) ;
			
				found = true ; break ;  
			}
			
			case XFTYPE:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::AND ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "filetype", dbParam::DIFFERENT_FROM, db::toSQL(dbKey, v[j] ))) ;
				}
			
				theQuery->addParameter(parameters) ;
				found = true ; break ;  
			}

			case FNAME:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::OR ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "filename", dbParam::LIKE, db::toSQL(dbKey, v[j] ))) ;
				}

				theQuery->addParameter(parameters) ;
				found = true ; break ;  
			}
			
			case XFNAME:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::AND ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "filename", dbParam::DIFFERENT_FROM, db::toSQL(dbKey, v[j] ))) ;
				}
			
				theQuery->addParameter(parameters) ;
				found = true ; break ;  
			}
	
			case SOURCE:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::OR ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "source", dbParam::LIKE, db::toSQL(dbKey, v[j] ))) ;
				}
			
				theQuery->addParameter(parameters) ;
				found = true ; break ;  
			}
			
			case XSOURCE:
			{
				dbQueryParameters parameters(dbKey) ;
                parameters.setParamOperator( dbQueryParameters::AND ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					parameters.addParameter(dbParam(dbKey, "t_inventory", "source", dbParam::DIFFERENT_FROM, db::toSQL(dbKey, v[j] ))) ;
				}
			
				theQuery->addParameter(parameters) ;
				found = true ; break ;  
			}
			
			case DATE:
			{
				dbQueryParameters parameters1(dbKey) ;
                dbQueryParameters parameters2(dbKey) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				
				if (v.size() == 1)
				{
					DateTime high( "UTC=" + v[0] ) ;
					parameters1.addParameter(dbParam(dbKey, "t_inventory","validitystart", dbParam::MINOR_OR_EQUAL_OF, 
						db::toSQL(dbKey, high , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystart", theConnection) ) ) ) ) ;
					parameters2.addParameter(dbParam(dbKey, "t_inventory","validitystop", dbParam::MINOR_OR_EQUAL_OF, 
						db::toSQL(dbKey, high , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystop", theConnection) ) ) ) ) ;
				}
				else if (v.size() >= 2)
				{
					DateTime low( "UTC=" + v[0] ), high( "UTC=" + v[1] ) ;
					parameters1.addParameter(dbParam(dbKey, "t_inventory","validitystart", dbParam::BETWEEN, 
						db::toSQL(dbKey, low , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystart", theConnection) ) ),
						db::toSQL(dbKey, high , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystart", theConnection) ) ) 
						) ) ;
					
					parameters2.addParameter(dbParam(dbKey, "t_inventory","validitystop", dbParam::BETWEEN, 
						db::toSQL(dbKey, low , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystop", theConnection) ) ),
						db::toSQL(dbKey, high , dbColumnTypeHelper::columnToConvDateTimeMode ( 
								dbSchemaMetaDataProvider::instance(theConnection.getKey())->getDateTimeMode("t_inventory" ,"validitystop", theConnection) ) ) 
						) ) ;

				}
				
				theQuery->addParameter(parameters1) ;
				theQuery->addParameter(parameters2) ;
				found = true ; break ;  
			}

			case WHERE:
			{
				dbQueryParameters parameters(dbKey) ; //parameters.setParamOperator( dbQueryParameters::AND ) ; 
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				for (size_t j=0; j<v.size(); j++)
				{
					theQuery->addParameter( v[j] ) ;
				}
			
				found = true ; break ;  
			}
			
			case ALL:
			{
				excerr << "\n\t**************************************************************************************************\n" ;
				excerr << "\t**                                        WARNING                                               **\n" ;
				excerr << "\t**************************************************************************************************\n" ;
				excerr << "\t**                                                                                              **\n" ;     
				excerr << "\t**    ALL THE DATA IN T_INVENTORY WILL BE REMOVED.                                              **\n" ;
				excerr << "\t**    THE OPERATION WILL START IN 30 SECONDS                                                    **\n" ;
				excerr << "\t**    PRESS ^C TO INTERRUPT ...                                                                 **\n" ;
				excerr << "\t**                                                                                              **\n" ;
				excerr << "\t**************************************************************************************************\n\n\n" ;

				sleep (30) ; 
				removeall = true;
				found = true ; break ;  
			}
			
			case ALL_NOW:
			{
				removeall = true;
				found = true ; break ;  
			}
					
			case DEMO:
			{
				demo = true ; 			
				break ;  
			}

			default:
			{
				ACS_THROW( exIllegalValueException("***  INTERNAL ERROR:  CASE NOT HANDLED. PLS REPORT TO ACS/EXPRIVIA  ***") ) ; 
			}
		
		}	// END: switch
	
	}	// END: for
	
	
	/*
		if (found = false)
		means that no option was provided.
		Since at least ONE option must be provided (the option --all can be used to confirm that the operation must be carried on)
		an error is returned
	*/
	
	ACS_COND_THROW( !found, exIllegalValueException("***  FATAL  ***  No parameter provided") ) ; 

	theQuery->doQuery( theConnection ) ;

	exFMessage msg ; 
	msg << theQuery->size() << "  entries " << (demo? "would" : "will" ) << "  be removed\n" ;
	excerr << msg ;
	
	if (removeall || theQuery->size() > queryThreshold() )
	{
		string authMessage;
		cout << "Checking authorization for deletion...\n";
		bool granted=authOperation(REMOVE_OPERATION, authMessage);
		if (!granted)
		{
			cerr << "Authorization Failed!\n";
			ACS_LOG_ERROR ("Authorization failed. Reported error is: " << authMessage);
			// This is a function exit point. The operation has not been performed due to lack of authorization 
			return 2;
		}
		cout << "Authorization granted.\n";
	}

	if (demo)
	{
		for (size_t j=0; j < theQuery->size(); j++) 
		{
			exFMessage msg ; msg << "Not removing: \"" << (*theQuery)[j]->getFileName() << "\"  version: \"" << (*theQuery)[j]->getFileVersion() << "\"\n" ;
			excerr << msg ;
		}
	}
	else
	{
		for (size_t j=0; j < theQuery->size(); j++) {

			dbFileAction* eraser = dbFileActionFactoryChain::instance()->newEraseAction(*getDefaultConnectionPool(), *(*theQuery)[j]);
			dbFileActionScopeGuard guard(eraser);
			eraser->start();

			{
				exFMessage msg ; msg << "Filename: \"" << (*theQuery)[j]->getFileName() << "\"  version: \"" << (*theQuery)[j]->getFileVersion() << "\"\n" ;
				excerr << msg ; 
			}

			(*theQuery)[j]->remove(theConnection) ;
			{
				exFMessage msg ; msg << "*** REMOVED ***" << "\n" ;
				excerr << msg ; 
			}
		}
	}

	return 0 ;
} 

size_t MultiRemoveInventoryObject::queryThreshold () const
{
	size_t theThreshold = 10;
	
	// Threshold will be limited to 100 items to avoid unintendend 
	// large amount of data deletion
	const size_t maxThreshold = 100;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	// Not mandatory
	rs.getValue (THRESHOLD_PARAM, theThreshold, false);
	
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Threshold set is " << theThreshold << ". Limited to " << maxThreshold << ".");
	return min(maxThreshold, theThreshold);
}

/***************************************************************************************
**
                                          main
**
* 
* Exit codes:
* 1- general failure
* 2- wrong authentication (if needed)
***************************************************************************************/
int MultiRemoveInventoryObject::main(int, char const * const *, char const * const *) // PRQA S 5520
{
	int status = 0;

	try
	{	
    	math::Statistics::instance()->enableStatistics(false) ;

		Application::loadConfiguration();
        
		const vector<string> &parsedArgs = getParsedArgs();
		vector<string> fileNames ;


		if (parsedArgs.size() > 0)
		{

			Cmdparam curparam = NOARGS ;
			for (size_t i=0; i < parsedArgs.size(); i++) 
			{
				/* Options followed by one or more arguments: */
				if ("--filetype" == parsedArgs[i]) {
					curparam = FTYPE ; _usedParams.push_back(curparam) ;
				}
				else if ("--nofiletype" == parsedArgs[i]) {
					curparam = XFTYPE ; _usedParams.push_back(curparam) ;
				}
				else if ("--source" == parsedArgs[i]) {
					curparam = SOURCE ; _usedParams.push_back(curparam) ;
				}
				else if ("--nosource" == parsedArgs[i]) {
					curparam = XSOURCE ; _usedParams.push_back(curparam) ;
				}
				else if ("--filename" == parsedArgs[i]) {
					curparam = FNAME ; _usedParams.push_back(curparam) ;
				}
				else if ("--nofilename" == parsedArgs[i]) {
					curparam = XFNAME ; _usedParams.push_back(curparam) ;
				}
				else if ("--date" == parsedArgs[i]) {
					curparam = DATE ; _usedParams.push_back(curparam) ;
				}
				else if ("--where" == parsedArgs[i]) {
					curparam = WHERE ; _usedParams.push_back(curparam) ;
				}

				/* Options don't followed by arguments: */
				else if ("--all" == parsedArgs[i]) {
					curparam = NOARGS ; _usedParams.push_back(ALL) ;
				}
				else if ("--allnow" == parsedArgs[i]) {
					curparam = NOARGS ; _usedParams.push_back(ALL_NOW) ;
				}
       			else if ("--demo" == parsedArgs[i]) {
					curparam = NOARGS ; _usedParams.push_back(DEMO) ;
				}

				/* Options not stored in _usedParams map : */
				else if ("--enablestats" == parsedArgs[i]) {
					curparam = NOARGS ;
					math::Statistics::instance()->enableStatistics(true) ;
				}
      			else if ("--verbose" == parsedArgs[i]) {
					curparam = NOARGS ; 
					_verbose = true ;
				}

				else {
					// Not found in the list of options: IT MUST BE AN ARGUMENT 

					if (curparam == NOARGS)
					{
						// The last option doesn't allow arguments: ERROR !
						exFError msg ; 
						msg << "Invalid commandline: No valid parameter provided" ; excerr << msg ; 
						internalusage(appName()) ;	
						status = 1 ;
						break ; 
					}

					// The last option allows arguments: added to list 
					if (_paramsMap.find(curparam) == _paramsMap.end())
					{
						vector <string> v ;
						_paramsMap[curparam] = v ;	/* An empty vector is added */
					}

					(_paramsMap.find(curparam)->second) .push_back(parsedArgs[i]) ;

				}
	    	}	// END for

    	}	// END if (parsedArgs.size() > 0)
		else
		{
			// No parameters provided!
			exFError msg ; 
			msg << "Invalid commandline: No parameters provided" ; excerr << msg ; 
			internalusage(appName()) ;	
			status = 1 ;
		}


		if (status == 0)
		{
			if (_verbose) dumpParameters() ;
			
			status = doWork() ;
			
		}	// END: if (status == 0)

	}
	catch(exception &e) {

		exFError msg ; msg << "Line: " << __LINE__ << "  *** FATAL  --  EXCEPTION RECEIVED ***\n" << e ;
		excerr << msg ;

		status = 1 ;
	}
	
	ACS_LOG_INFO ("Execution finished with " << (status? "invalid ":"") << "status code: " << status);
	
	return status ; 
}


_ACS_END_NAMESPACE
