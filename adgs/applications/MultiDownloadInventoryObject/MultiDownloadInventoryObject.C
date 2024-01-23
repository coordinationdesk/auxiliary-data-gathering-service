// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. MultiDownloadInventoryObject $

	$Id$

	$Author$

	$Log$
	Revision 1.20  2016/07/08 12:43:13  marfav
	Application definition management fixed
	
	Revision 1.19  2016/04/11 13:50:55  marpas
	adopting new db interfaces
	
	Revision 1.18  2016/03/29 17:11:47  marpas
	adopting new db I/F
	
	Revision 1.17  2014/01/17 10:29:39  marfav
	S1PDGS-31392 managing the extensions and the split policies using libFile services
	
	Revision 1.16  2013/09/25 16:59:29  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 1.15  2013/05/12 14:53:03  marpas
	using db connection key
	
	Revision 1.14  2013/04/09 09:03:04  marpas
	new Statistics
	
	Revision 1.13  2013/04/03 12:30:15  enrcar
	EC:: typo fixed
	
	Revision 1.12  2013/03/26 10:41:07  enrcar
	EC:: Interface modified
	
	
	
*/


#include <MultiDownloadInventoryObject.h>
#include <File.h>
#include <StringUtils.h>
#include <dbDownloadInventoryFile.h>
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
#include <dbFileType.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>

#include <mdiV.h>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef MULTIDOWNLOADINVENTORYOBJECT_SUBSYSTEM
#define MULTIDOWNLOADINVENTORYOBJECT_SUBSYSTEM "SDF"
#endif

#ifndef MULTIDOWNLOADINVENTORYOBJECT_APPNAME
#define MULTIDOWNLOADINVENTORYOBJECT_APPNAME "MultiDownloadInventoryObject"
#endif 

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MultiDownloadInventoryObject) 

namespace { // unnamed
    mdiV version;

    Application::GenericBuilder<MultiDownloadInventoryObject> theApplicationBuilder(MULTIDOWNLOADINVENTORYOBJECT_APPNAME, MULTIDOWNLOADINVENTORYOBJECT_SUBSYSTEM) ;
}

/***************************************************************************************
**
                                MultiDownloadInventoryObject 
**
***************************************************************************************/
MultiDownloadInventoryObject::MultiDownloadInventoryObject(const string &appName, const string &subSystem) :
	PROJECT_APP(appName,subSystem) ,
	_verbose(false), 
	_paramsMap(), 
	_usedParams(), 
	_def_ext_hdr("HDR"), 
	_def_ext_dbl("DBL"), 
	_def_ext_unsplit("")
{
}


/***************************************************************************************
**
                                ~MultiDownloadInventoryObject 
**
***************************************************************************************/
MultiDownloadInventoryObject::~MultiDownloadInventoryObject() throw() 
{
}


/***************************************************************************************
**
                                     usage 
**
***************************************************************************************/
int MultiDownloadInventoryObject::usage(const string& exeName) const
{

	excerr << "\n\t**************************************************************************************************" << endl ;
	excerr << "\t**                                     TEMPORARY  NOTICE                                        **" << endl ;
	excerr << "\t**************************************************************************************************" << endl ;
	excerr << "\t**                                                                                              **" << endl ;     
	excerr << "\t**    THE INTERFACE HAS SLIGHTLY CHANGED                                                        **" << endl ;
	excerr << "\t**    REFER TO THE usage (BELOW)                                                                **" << endl ;
	excerr << "\t**    FOR SUPPORT OR IN CASE OF ISSUES PLEASE REPORT TO ACS                                     **" << endl ;
	excerr << "\t**                                                                                              **" << endl ;
	excerr << "\t**************************************************************************************************\n\n" << endl ;

	excerr << "Usage: " << applicationName() << " [ std-opts ] [ specific-opts ]  arguments\n" ;
	excerr << "Specific options (all the options can be COMBINED. Logic operator AND will be applied):\n" ; 
	excerr << "\t--filetype\t[FileType1]  [FileType2]...\t: List of filetypes to be included"<< endl ;
	excerr << "\t--nofiletype\t[FileType1]  [FileType2]...\t: List of filetypes to be excluded"<< endl ;
	excerr << "\t--source\t[Source1]    [Source2]...\t: List of sources to be included"<< endl ;
	excerr << "\t--nosource\t[Source1]    [Source2]...\t: List of sources to be excluded"<< endl ;
	excerr << "\t--filename\t[FileName1]  [FileName2]...\t: List of filenames to be included"<< endl ;
	excerr << "\t--nofilename\t[FileName1]  [FileName2]...\t: List of filenames to be excluded"<< endl ;
	excerr << "\t--date\t\t[<lowest_date>]  <highest_date>\t\t: Range for both start/stop validity in UTC format: e.g.  19991231T000000  19991231T235959" << endl ;
	excerr << "\t--where\t\t<SQL where condition>" << endl ;
	excerr << "\t--path\t\t<destination path>" << endl ;
	excerr << "\t--ext_hdr\t<extension for hdr files (default: \"" << _def_ext_hdr << "\")" << endl ;
	excerr << "\t--ext_dbl\t<extension for dbl files (default: \"" << _def_ext_dbl << "\")" << endl ;
	excerr << "\t--ext_unsplit\t<extension for unsplit files (default: \"" << _def_ext_unsplit << "\")" << endl ;
	excerr << "\t--all\t\tdownload all stored files" << endl ;
	excerr << "\t--enablestat\tenable statistics" << endl ;
	excerr << "\t--verbose\tbe verbose" << endl ;
	excerr << "\t--demo\t\tdemo mode (no file will be downloaded)" << endl ;
	excerr << "\t--clientname\t<client name> (download operation recorded in DB)" << endl ;
	return 1 ;
}


/***************************************************************************************
**
                                      dumpParameters
**
***************************************************************************************/
void MultiDownloadInventoryObject::dumpParameters() 
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
			
			case EXT_DBL :
			{
				exFMessage msg ; msg << "Option:  --path\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}
			
			case EXT_HDR :
			{
				exFMessage msg ; msg << "Option:  --path\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}
			
			case EXT_UNSPLIT :
			{
				exFMessage msg ; msg << "Option:  --path\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}
			
			case DESTPATH :
			{
				exFMessage msg ; msg << "Option:  --path\n" ; excerr << msg ; 
				print_vector = false ;
				break ; 
			}

			case CLIENTNAME :
			{
				exFMessage msg ; msg << "Option:  --clientname\n" ; excerr << msg ;
				print_vector = false ;
				break ;
			}


			default:
			{
				print_vector = false ;
				break ; 
			}

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
int MultiDownloadInventoryObject::doWork() // PRQA S 4020
{    
	bool demo = false ; 
	bool found = false ; 
	string ext_hdr = _def_ext_hdr, ext_dbl = _def_ext_dbl, ext_unsplit = _def_ext_unsplit ;
	string download_path = "." ; 
	string clientName = "";

	dbConnectionWrapper pw(*getDefaultConnectionPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	dbInventoryObjectQuery * theQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey));
	dbGenQueryScopeGuard theQuesryScopeGuard(theQuery) ;


	for (size_t i=0; i<_usedParams.size(); i++)
	{
	
		switch (_usedParams[i])
		{
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
				dbQueryParameters parameters(dbKey ) ; 
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
				dbQueryParameters parameters(dbKey ) ; 
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
				dbQueryParameters parameters(dbKey ) ; 
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
				dbQueryParameters parameters(dbKey ) ; 
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
				dbQueryParameters parameters(dbKey ) ; 
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
                dbQueryParameters  parameters2(dbKey) ; 
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
			
			case EXT_DBL :
			{
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				if ((v.size() >= 1) && !v[0].empty()) ext_dbl = v[0] ;
			
				break ;  
			}
			
			case EXT_HDR :
			{
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				if ((v.size() >= 1) && !v[0].empty()) ext_hdr = v[0] ;
				
				break ;  
			}
			
			case EXT_UNSPLIT :
			{
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				if ((v.size() >= 1) && !v[0].empty()) ext_unsplit = v[0] ;

				break ;  
			}
		
			case DESTPATH:
			{
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				if ((v.size() >= 1) && !v[0].empty()) {
					download_path = v[0] ;
			
					if ( !(File::isDir(download_path) && File::isExecutable(download_path)) )
					{
						ACS_THROW(exIOException("Invalid destination path: \"" + download_path + "\"")) ;
					}
				}
				
				break ;  
			}

			case CLIENTNAME:
			{
				const vector <string>& v = _paramsMap[ _usedParams[i] ] ;
				if ((v.size() >= 1) && !v[0].empty()) {
					clientName = v[0] ;
				}

				break ;
			}


			
			case ALL:
			{
				excerr << "\n\t**************************************************************************************************" << endl ;
				excerr << "\t**                                        WARNING                                               **" << endl ;
				excerr << "\t**************************************************************************************************" << endl ;
				excerr << "\t**                                                                                              **" << endl ;     
				excerr << "\t**    ALL THE DATA IN T_INVENTORY WILL BE DOWNLOADED.                                           **" << endl ;
				excerr << "\t**    THE OPERATION WILL START IN 30 SECONDS                                                    **" << endl ;
				excerr << "\t**    PRESS ^C TO INTERRUPT ...                                                                 **" << endl ;
				excerr << "\t**                                                                                              **" << endl ;
				excerr << "\t**************************************************************************************************\n\n" << endl ;

				sleep (30) ; 

				found = true ; break ;  
			}
			
			case ALL_NOW:
			{
				// EC:: Undocumented feature
				
				found = true ; break ;  
			}
					
			case DEMO:
			{
				demo = true ; 			
				break ;  
			}

			default:
			{
				ACS_THROW( exIllegalValueException("***  INTERNAL ERROR:  CASE NOT HANDLED. PLS REPORT TO ACS  ***") ) ; 
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
	msg << theQuery->size() << "  entries " << (demo? "would" : "will" ) << "  be downloaded\n" ;
	excerr << msg ;
	
	if (demo)
	{
		for (size_t j=0; j < theQuery->size(); j++) 
		{
			exFMessage msg ; msg << "Not downloading: \"" << (*theQuery)[j]->getFileName() << "\"  version: \"" << (*theQuery)[j]->getFileVersion() << "\"\n" ;
			excerr << msg ;
		}
	}
	else
	{

		for (size_t j=0; j < theQuery->size(); j++) 
		{
			const string completePath = StringUtils::pathJoin(download_path, (*theQuery)[j]->getFileName()) ;
			
			excerr << "\n*********************  DOWNLOADING FILE " << (j+1) << "/" << theQuery->size() << "  " << (*theQuery)[j]->getFileName() << " " << (*theQuery)[j]->getFileVersion() << endl ;

			try {

				/* Query on t_filetype to fill filetype parameters above */
				string fileType = (*theQuery)[j]->getFileType();
				dbFileType *ft= dynamic_cast<dbFileType*>(dbPersistentFactoryChain::instance()->newObject("dbFileType", dbKey));
				dbPersistentScopeGuard ftGuard(ft) ;
				ft->reset();
				ft->setFileType(fileType);
				if(!ft->load(theConnection))
				{
					// unable to load filetypes
					ostringstream msg ;
					msg << "MultiDownloadInventoryObject : " << __LINE__ << " Unable to load filetypes [" << fileType << "]" ;
					exIOException e(msg.str());
					ACS_THROW(e) ;
				}

				dbDownloadFile* downloader = dynamic_cast<dbDownloadFile*> (dbFileActionFactoryChain::instance()->newDownloadAction(db::ConnPools::instance()->getPool(""), *(*theQuery)[j], completePath, clientName));
				dbFileActionScopeGuard guard(downloader);
				downloader->start();
				for (size_t i=0; downloader->downloading(); i++ )
				{
					if (!(i%10))
						excout << "Still downloading " + completePath << endl;
					Timer::delay(100) ;
				}

				string headerFile, dblFile ;
				File::splitMatchExt(completePath, headerFile, dblFile, (*theQuery)[j]->getHeaderSize(), (*theQuery)[j]->getDataSize(), ft->getSngExt(), ft->getHdrExt(), ft->getDblExt(), ft->getMngSngExt(), true, true ) ;

				if ((headerFile != completePath) && (dblFile != completePath))
				{
					File::unlink(completePath);
				}


			} 
			catch (exception& e)
			{
				exFError msg ; msg << "Line: " << __LINE__ << "  ***  EXCEPTION RECEIVED downloading file " << (j+1) << "/" << theQuery->size() << "  --  File skipped  ***\n" << e ;
				excerr << msg ;
			}
		}	//END: for

	}

	return 0 ;
} 


/***************************************************************************************
**
                                          main
**
***************************************************************************************/
int MultiDownloadInventoryObject::main(int, char const * const *, char const * const *) // PRQA S 5520
{
	int status = 0;

	try
	{	
    	math::Statistics::instance()->enableStatistics(false) ;

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
				else if ("--path" == parsedArgs[i]) {
					curparam = DESTPATH ; _usedParams.push_back(curparam) ;
				}
				else if ("--clientname" == parsedArgs[i]) {
					curparam = CLIENTNAME ; _usedParams.push_back(curparam) ;
				}
				else if ("--ext_hdr" == parsedArgs[i]) {
					curparam = EXT_HDR ; _usedParams.push_back(curparam) ;
				}
				else if ("--ext_dbl" == parsedArgs[i]) {
					curparam = EXT_DBL ; _usedParams.push_back(curparam) ;
				}
				else if ("--ext_unsplit" == parsedArgs[i]) {
					curparam = EXT_UNSPLIT ; _usedParams.push_back(curparam) ;
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
			
			try {
				this->loadConfiguration() ;

			} catch(exException &e) {
				ACS_LOG_NOTIFY_EX(e);
				ACS_LOG_WARNING("Problem occurred loading configuration. Trying simple download without configuration settings.");
			}

			status = doWork() ;
			
		}	// END: if (status == 0)

	}
	catch(exception &e) {

		exFError msg ; msg << "Line: " << __LINE__ << "  *** FATAL  --  EXCEPTION RECEIVED ***\n" << e ;
		excerr << msg ;

		status = 1 ;
	}
	
	exFMessage msg ; msg << "\nExecution finished with " << (status? "invalid ":"") << "status code: " << status << "\n" ;
	excerr << msg ; 
	
	return status ; 
}


_ACS_END_NAMESPACE
