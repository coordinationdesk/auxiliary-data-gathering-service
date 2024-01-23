/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DownloadInventoryFile $

	$Id$

	$Author$

	$Log$
	Revision 2.18  2017/03/20 10:05:56  clanas
	removed useless extern declaration with proper include
	
	Revision 2.17  2016/07/08 09:34:14  marfav
	Fixing project checks for CSG
	
	Revision 2.16  2016/07/08 08:43:32  marfav
	CSGACQ-84
	New project/application based log rotation management strategy enforced
	
	Revision 2.15  2016/05/11 14:46:26  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.14  2016/04/11 13:45:17  marpas
	adopting new db interfaces
	
	Revision 2.13  2013/03/14 15:17:18  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.12  2013/03/12 17:58:07  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.11  2012/02/14 15:17:53  marpas
	fixing dtor
	
	Revision 2.10  2011/09/22 12:18:03  marpas
	debug message forgot
	
	Revision 2.9  2011/09/22 12:05:40  marpas
	args management follows the new standard, logname naming changed
	
	Revision 2.8  2011/09/22 10:21:42  marpas
	usage method rewritten for standard
	
	Revision 2.7  2010/06/15 09:44:13  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.6  2008/12/05 13:09:42  marpas
	loadConfiguration added ... because it is an application
	
	Revision 2.5  2008/11/26 18:19:34  ivafam
	Management of DownloadAsynchException added
	
	Revision 2.4  2008/11/26 11:23:43  danalt
	removed MergeConfiguration header
	
	Revision 2.3  2007/04/20 10:54:05  ivafam
	Patch to remove libProc dependence of libGfeSatStore
	
	Revision 2.2  2007/03/23 17:34:09  ivafam
	This version includes -V optional parameter to retrieves from SDF the file with version.
	This version is not fully tested
	
	Revision 2.1  2006/10/23 10:08:41  marpas
	variables and/or methods related to file size, header size and blob sized now take into account 64 bits
	
	Revision 2.0  2006/02/28 10:38:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2006/01/20 15:57:09  giucas
	Fixed exception description
	
	Revision 1.9  2006/01/12 18:48:57  aleric
	Log info added
	
	Revision 1.8  2005/11/17 13:27:59  serper
	Usage description changed
	
	Revision 1.7  2005/11/15 15:01:44  serper
	After CMF update
	
	Revision 1.6  2005/07/18 10:26:15  ivafam
	Now splid HDR-DBL
	
	Revision 1.5  2005/07/15 14:34:50  ivafam
	Work in progress
	
	Revision 1.4  2005/07/15 14:21:05  ivafam
	Modified for multi storage manager management
	
	Revision 1.3  2005/06/20 12:13:15  marpas
	compilation fixed
	
	Revision 1.2  2005/05/31 15:35:16  marpas
	file action interface changed
	
	Revision 1.1.1.1  2005/02/23 11:33:52  ivafam
	Import DownloadInventoryFile 
	

	
*/


#include <dbDownloadInventoryFile.h>
#include <dbGeneralQuery.h>
#include <dbGenQueryScopeGuard.h>
#include <dbCryosatFactory.h>
#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <File.h>
#include <dbQueryFactoryChain.h>
#include <dbQueryParameters.h>
#include <dbConv.h>
#include <dbResourceSpaceFactory.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <SMPluginLoader.h>
#include <unistd.h>
#include <DownloadFileClass.h>
#include <DownloadInventoryFile.h>
#include <SpawnProc.h>
#include <Filterables.h>
#include <getopt.h>
#include <dfV.h>

using namespace acs ;
using namespace std;

//ACS_CLASS_DEFINE_DEBUG_LEVEL(DownloadInventoryFile) 

static dfV version;

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef DOWNLOADINVENTORYFILE_SUBSYSTEM
#define DOWNLOADINVENTORYFILE_SUBSYSTEM "SDF"
#endif

#ifndef DOWNLOADINVENTORYFILE_APPNAME
#define DOWNLOADINVENTORYFILE_APPNAME "DownloadInventoryFile"
#endif 

Application::GenericBuilder<DownloadInventoryFile> theApplicationBuilder(DOWNLOADINVENTORYFILE_APPNAME, DOWNLOADINVENTORYFILE_SUBSYSTEM) ;


// Constructor
DownloadInventoryFile::DownloadInventoryFile(const string &appName, const string &subSystem) :
	PROJECT_APP(appName,subSystem) ,  vargs_()
{
}

// Destructor
DownloadInventoryFile::~DownloadInventoryFile() throw()
{
    for_each(vargs_.begin(), vargs_.end(), free) ;
}


static char * allocChar(const string &s)
{
    return strdup(s.c_str()) ;
}
    

int DownloadInventoryFile::usage(const string &exeName) const
{
	excerr << "\n" ;
	excerr << "usage: " << File::getFileName(exeName) << "  ( -f <filename> [-V <Version>] | -l <ASCII File> ) -d <target directory> [-L <log name>]\n";
	excerr << "Parameters:\n" ;
	excerr << "\t-f <filename>   : <filename> is the name of Inventory Object to retrieve from\n" ;
	excerr << "\t                  I/F archive.\n";
	excerr << "\t-V <Version>    : <Version> is the file version. This option has to be used with the -f option" ;
	excerr << "\t-l <ASCII File> : <ASCII File> is the name of input file containing a list\n" ;
	excerr << "\t                  of Inventory Object(s) to retrieve from I/F archive.\n" ;
	excerr << "\t-d <Target Dir> : <Target Dir> is the name of target directory where\n";
	excerr << "\t                  the retrieved Inventory Object(s) will be stored.\n" ;
	excerr << "\t-L Log name     : This parameter is the name of log file. If this parameter is\n" ;
    excerr << "\t                  not set, a default naming is applied\n" ;
	excerr << "\t-h|-?           : This help.\n" ;
	excerr << "Returns   :\n" ;
	excerr << "\t (0)   zero upon successful\n" ;
	excerr << "\t (> 0) greater than zero if fails\n" ;
	excerr << "\n" ;
	return 0 ;
}

string DownloadInventoryFile::getDefaultLogName () const
{
	DateTime currTime ;
	string str ;
	ostringstream oo;
	oo << "LOG_SDF_" << applicationName() << "_" ; 
	currTime.asISO8601(oo, 3) << ".log" ;
	string logFullPath = defaultLogPath + "/" + oo.str().c_str() ; // create the log path name
	return logFullPath;
}

int DownloadInventoryFile::main(int argc, char const * const * argv, char const * const * env)
{
	int status = 0 ;

    const vector<string> &parsedArgs = getParsedArgs() ;
    vargs_.push_back(strdup(appName().c_str())) ; // first is app name
    back_insert_iterator< vector<char *> > insert_it_a (vargs_) ;  
    transform(parsedArgs.begin(), parsedArgs.end(), insert_it_a , allocChar) ;
    vargs_.push_back(0) ; // last is a null ptr

	try
	{

    	char options[100];
    	strcpy(options,"?hf:l:d:L:V:") ;

    	int fFlag = 0 ;
    	int lFlag = 0 ;
    	int LFlag = 0 ;
    	int dFlag = 0 ;
		int VFlag = 0 ;

		string names ;
		string nameLog ;
		string targetDir ;
		string version;

  		int optIndex = getopt(vargs_.size()-1, &vargs_[0], options) ;
    	while (optIndex != EOF)
    	{
        	switch (optIndex)
        	{
        	case 'f':
				fFlag++ ;
				names = optarg ;
				if ( names[0] == '-' )
				{
					excerr << "\nOption -f requires an argument\n" ;
					internalusage(appName()) ;
					return 0 ;
				}
            	break ;

        	case 'V':
				VFlag++ ;
				version = optarg ;
				if ( version[0] == '-' )
				{
					excerr << "\nOption -V requires an argument\n" ;
					internalusage(appName()) ;
					return 1 ;
				}
            	break ;

			case 'l':
				lFlag++ ;
				names = optarg ;
				if ( names[0] == '-' )
				{
					excerr << "\nOption -l requires an argument\n" ;
					internalusage(appName()) ;
					return 1 ;
				}
				break ;
			case 'L':
				LFlag++ ;
				nameLog = optarg ;
				if ( nameLog[0] == '-' )
				{
					excerr << "\nOption -L requires an argument\n" ;
					internalusage(appName()) ;
					return 1 ;
				}
				break ;
			case 'd':
				dFlag++ ;
				targetDir = optarg ;
				if ( targetDir[0] == '-' )
				{
					excerr << "\nOption -d requires an argument\n" ;
					internalusage(appName()) ;
					return 1 ;
				}
				break ;
        	case 'h':
        	case '?':
        	default:
                 return internalusage(appName()) ;
        	}

  		    optIndex = getopt(vargs_.size()-1, &vargs_[0], options) ;
		}

		if ((!fFlag && !lFlag) || (fFlag && lFlag))
		{
    			excerr << "\n-f flag or -l flags is mandatory\n" ;
    			internalusage(appName()) ;
    			return 1 ;
		}

		if ((VFlag) && (!fFlag))
		{
    			excerr << "\nThe usage -V option has to used only with -f option\n" ;
    			internalusage(appName()) ;
    			return 1 ;
		}
		
		if (!dFlag)
		{
    			excerr << "\n-d flag is mandatory\n" ;
    			internalusage(appName()) ;
    			return 1 ;
		}

		string logdir ;
		string logname ;
		string logFullPath ;

/**
		if (!LFlag)
		{
            DateTime currTime ;
            string str ;
            ostringstream oo;
            oo << "LOG_SDF_" << applicationName() << "_" ; 
            currTime.asISO8601(oo, 3) << ".log" ;
            logFullPath = defaultLogPath + "/" + oo.str().c_str() ; // create the log path name
		}
		else
**/
		if (LFlag)
		{
			// Must override the log name
      		logname = File::getFileName(nameLog) ;
        	logdir = File::getDirPath(nameLog) ;
			if (logdir == "") logdir = "./" ;
			logFullPath = logdir + "/" + logname ;

			// Disable automatic log management
			setRotateLogParametersFromConf (false);

			// Instruct the LRP to override the logfilename
			LogRotationPolicy& lrp = getLRPolicy();
			lrp.forceLogName (logFullPath);
		}

		// In case of CSG application add the thread ID to the log name
#ifdef CSG
		putThreadInLogName(true);
#endif
//		rotateLog() ; // open the first log

		ACS_LOG_INFO ("Current log file is " << getLRPolicy().getCurrentLogName() );

		try
		{
			loadConfiguration() ;
		}
		catch(exException &e)
		{
			//exit with error.
			exCriticalException e2(e,"ERROR on dowloading");
			ACS_THROW(e2) ;
		}

		rotateLog() ; // open the first log
		
		{
			exFMessage msg ; 
			msg << "Starting the DownloadInventoryFile\n" ;
			excerr << msg ; 
		}
		DownloadFileClass dl ;
		
		if (fFlag)
		{
			exFMessage msg ; 
			msg << "Inventory Object to retrieve is : <" << names << ">\n" ;
			excerr << msg ; 
			
			if (VFlag) {		
				exFMessage msg ; 
				msg << "Inventory Object Version to retrieve is : <" << version << ">\n" ;
				excerr << msg ; 
			}
		}
		else {
			exFMessage msg ; 
			msg << "The ASCII file containing the list of Inventory Object(s) to retrieve is : <" << names << ">\n" ;
			excerr << msg ; 
		}	
			
		{	
			exFMessage msg ; 
			msg << "The target Directory is : <" << targetDir << ">\n" ;
			excerr << msg ; 
		}
		
		// Set the corresponding value of input arguments
		if (fFlag) // if single file
		{
			if (VFlag)
				dl.addFilename(names, version) ;
			else
				dl.addFilename(names) ;
		}
		else
			dl.readInFile(names) ;
		
		dl.setTargetDir(targetDir) ;
		

		// Download files
		dl.download(*getDefaultConnectionPool()) ;
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
