// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Scheduler $

	$Id$

	$Author$

	$Log$
	Revision 1.66  2017/12/12 09:04:03  davide.tiriticco
	Moved DefaultTaskTableExecutorFactory and DefaultExecutorVisitorFactory registration in libScheduler
	
	Revision 1.65  2017/09/19 10:05:36  davide.tiriticco
	Added declarations of factory classes
	
	Revision 1.64  2017/06/06 08:49:40  lucio.pulvirenti
	DFEP-1014: OPEN_LOG_IN_APPEND compilation flag turned into SCHEDULER_LOG_IN_APPEND.
	
	Revision 1.63  2017/06/05 13:21:36  lucio.pulvirenti
	DFEP-1014: if OPEN_LOG_IN_APPEND compilation flag is set, application log (exLogStream) opened in append mode.
	
	Revision 1.62  2017/05/04 16:35:35  lucio.pulvirenti
	S2PDGS-1761: exceptions from both loadScConfiguration and loadPlugins methods managed in main: ssINTERNAL_ERR code returned.
	
	Revision 1.61  2016/11/10 17:16:43  marpas
	OPMAN-219 porting with 3 attempts
	coding best practice applied
	
	Revision 1.60  2016/07/07 13:31:58  marfav
	Using appname external definitions
	
	Revision 1.59  2015/08/21 09:35:05  marfav
	MC warning removed
	
	Revision 1.58  2015/08/13 15:26:39  marfav
	Adding default fuction for MC messages
	
	Revision 1.57  2015/08/12 15:59:16  marfav
	APF-271 work in progress
	
	Revision 1.56  2015/07/17 14:18:35  marfav
	Reverting changes made in APF-285
	
	Revision 1.54  2015/04/30 14:07:05  marfav
	Compilator warning removed
	Aligned to libScheduler 2.30 interfaces
	
	Revision 1.53  2014/03/13 11:25:37  nicvac
	S2PDGS-737: disabled log rotation from configuration.
	
	Revision 1.52  2013/09/27 15:32:34  marpas
	streams have name
	
	Revision 1.51  2013/07/26 14:22:45  marfav
	Scheduler completion wait uses now synchronous call
	
	Revision 1.50  2013/07/19 07:34:20  chicas
	streams management fixed
	
	Revision 1.49  2012/05/08 11:30:33  luipir
	addedd managing of scOrder without positional constraint
	
	Revision 1.48  2012/05/08 10:40:51  luipir
	modified limit to input parameters number. now generate erroronly in case no parameters found
	This modification assume that optional parametrs are in the tail of the command after scOrder
	
	Revision 1.47  2012/02/09 18:19:48  marpas
	refactoring in progress
	
	Revision 1.46  2011/06/06 10:01:02  chicas
	added HAS_MISSION_INCLUDES for defining BASE_APP
	
	Revision 1.45  2011/04/04 16:55:59  giucas
	Added BASE_APP for Dfep project
	
	Revision 1.44  2010/08/03 14:38:52  nicvac
	Task Table plugins management
	
	Revision 1.43  2010/06/15 09:22:18  marpas
	EMSA support added
	
	Revision 1.42  2008/11/25 10:17:33  danalt
	changed loadConfiguration name method
	
	Revision 1.41  2008/11/14 14:10:18  marfav
	ConfigurablePluginLoader interface changed
	
	Revision 1.40  2008/11/13 12:56:25  marpas
	new ConfigurationSingleton approach
	
	Revision 1.39  2008/04/22 13:25:18  marfav
	exception management improved
	
	Revision 1.38  2008/04/10 15:58:26  integrator
	Added ifdef for SRV project
	
	Revision 1.37  2007/12/17 14:20:04  serfol
	adding the Harm project
			                                      
	Revision 1.36  2007/04/19 08:16:00  giucas
	Updated for CSLibya project
	
	Revision 1.35  2006/04/21 13:38:45  clanas
	implemented Standalone application model
	
	Revision 1.34  2005/06/28 08:48:44  marfav
	Added OrderTypeManager configurations
	
	Revision 1.33  2005/05/20 08:02:53  marfav
	Plugin regular expression fixed
	
	Revision 1.32  2005/05/19 16:26:36  marfav
	Old ifdef removed
	Added a check on the IPC listener thread
	
	Revision 1.31  2005/05/17 09:26:47  marfav
	Code indentation modified
	
	Revision 1.30  2005/04/04 15:18:06  ivafam
	Commented "add Patrol"
	
	Revision 1.29  2004/12/03 17:26:01  marfav
	Header Fixed
	
	Revision 1.28  2004/11/23 15:49:36  marfav
	Check disk free quota ifdeffed
	
	Revision 1.27  2004/10/13 13:44:36  marfav
	Patrol configuration space loading ifdeffed
	
	Revision 1.26  2004/09/14 16:26:18  marfav
	Plugin loader messaging improved
	
	Revision 1.25  2004/08/30 13:26:57  marfav
	Added support for Cosmo environment
	
	Revision 1.24  2004/08/25 15:50:18  marfav
	ConfigurablePlugin support added
	Plugin must be placed in /usr/acs/plugins/SchedulerPlugins
	
	Revision 1.23  2004/05/27 09:46:06  marfav
	Scheduler log rename on exit ifdeffed
	
	Revision 1.22  2004/05/24 14:24:32  marfav
	Added support for log pathname in workorder
	
	Revision 1.21  2004/05/17 08:53:30  integrator
	AIM - Removed unuseful CAL_VAL_APP
	
	Revision 1.20  2004/05/17 08:21:52  marfav
	TlSimulator environement fixed
	
	Revision 1.19  2004/04/22 16:02:38  integrator
	CalVal log path location changed to wd
	
	Revision 1.18  2004/04/22 07:09:53  integrator
	CalVal Environement support added
	
	Revision 1.17  2004/04/09 10:12:26  marfav
	Disk quota reservation support added
	
	Revision 1.16  2004/04/08 09:16:21  marfav
	Plugin loader error handling improved
	
	Revision 1.15  2004/04/07 13:26:10  marfav
	Added support for IPC and commands factory
	Using exStream.h
	Does not use ACE anymore
	
	Revision 1.14  2004/03/15 15:37:24  marfav
	Fixed Envisat / TlSimulator environements
	
	Revision 1.13  2004/03/11 19:13:14  marfav
	Align to new version of Scheduler Engine (libScheduler)
	
	Revision 1.12  2003/09/05 15:49:24  marfav
	Added support for ENVISAT environment
	
	Revision 1.11  2003/07/15 14:30:21  marfav
	Align to new version of PatrolHandler and CryosatApp
	
	Revision 1.10  2003/06/06 09:18:36  marfav
	Using the same application for Scheduler and TlSimulator
	
	Revision 1.9  2003/06/05 14:21:38  marfav
	Align to new Main class of CryosatApp
	
	Revision 1.8  2003/04/30 16:17:08  marfav
	Align to ICD v2.0
	
	Revision 1.7  2003/03/05 10:39:14  fracar
	handling scheduler termination on request
	
	Revision 1.6  2003/02/18 09:10:31  fracar
	improved logging
	
	Revision 1.5  2003/02/04 16:33:53  fracar
	fixed problem in logfile naming
	
	Revision 1.4  2003/02/04 15:03:51  fracar
	improved log messaging
	
	Revision 1.3  2003/02/03 16:49:52  marfav
	*** empty log message ***
	
	Revision 1.1.1.1  2003/02/03 15:56:21  marfav
	Import Scheduler
	
	
	


*/

#include <acs_c++config.hpp>
#if defined (HAS_MISSION_INCLUDES)
#define BASE_APP PROJECT_APP
#elif EMSA
#define BASE_APP EmsaApp
#elif TL_SIMULATOR
#define BASE_APP Application
#elif HARM
#define BASE_APP HarmApp
#elif STANDALONE
#define BASE_APP StandaloneApp
#elif ENVISAT
#define BASE_APP CryosatApp
#elif COSMO
#define BASE_APP CosmoApp
#elif CAL_VAL_APP
#define BASE_APP CvfSchApp
#elif SRV_APP
#define BASE_APP SrvSchApp
#elif Dfep
#define BASE_APP StandaloneApp
#else // CRYOSAT
#define BASE_APP CryosatApp
#endif 


#ifndef TL_SIMULATOR
#include <tlsV.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <ConfigurablePluginLoader.h>
#include <DirPluginSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#endif //TL_SIMULATOR

#include <TaskTable.h>
#include <TaskTableFactoryChain.h>
#include <ThreadedScheduler.h>
#include <SchedulerExitStatus.h>
#include <ScIpcServer.h>
#include <TlMessage.h>
#include <ScAbstractCommand.h>
#include <dbConnectionPool.h>
#include <exStream.h>

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#elif TL_SIMULATOR
#include <Application.h>
#elif HARM
#include <HarmApp.h>
#elif STANDALONE
#include <StandaloneApp.h>
#elif ENVISAT
#include <CryosatApp.h>
#elif COSMO
#include <CosmoApp.h>
#elif CAL_VAL_APP
#include <CvfSchApp.h>
#elif SRV_APP
#include <SrvSchApp.h>
#else // CRYOSAT
#include <CryosatApp.h>
#endif 

#include <MCFilterables.h>
#include <JobOrder.h>

#include <iostream>
#include <sstream>

#include <libgen.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

// PRQA S 1020 AVOID_MACROS

#ifndef SCHEDULER_SUBSYSTEM
#define SCHEDULER_SUBSYSTEM "IPF"
#endif

#ifndef SCHEDULER_APPNAME
#define SCHEDULER_APPNAME "Scheduler"
#endif 

// PRQA L:AVOID_MACROS

#ifndef TL_SIMULATOR
namespace {
    tlsV version;
}
#endif

namespace {

const char *_scPluginsPathKey = "Scheduler.Scheduler_Plugins_Path";
const char *_scListOfPluginsPathKey = "Scheduler.Scheduler_List_of_Plugins_Path.Scheduler_Plugins_Path";
const char *_pluginRegExp = "\\.so$";
}

class tlSchedulerApp : public BASE_APP // PRQA S 2109, 2153
{
public:
	tlSchedulerApp(const string &appName, const string &subSystem) : 
		BASE_APP (appName, subSystem),
		_myConfiguration(),_pluginLoader (),
		_ipcServer(), _haveToManageCommands(true),
		_scOrderFileName("")
	{
	}
	virtual ~tlSchedulerApp() throw() {} // PRQA S 2131

	virtual int main(int argc, char const * const * argv, char const * const * env);
	SchedulerExitStatus execute (int argc, char const * const * argv, char const * const * env);

#ifndef TL_SIMULATOR
    void loadScConfiguration();
    void loadPlugins();

private:
       rsResourceSet _myConfiguration;
       ConfigurablePluginLoader _pluginLoader;
#endif  // TL_SIMULATOR

private:
    tlSchedulerApp() ; // not implemented
    tlSchedulerApp(const tlSchedulerApp &) ; // not implemented
    tlSchedulerApp& operator= (const tlSchedulerApp &) ; // not implemented
    void manageSignals(ThreadedScheduler&);
    void manageCommands (ThreadedScheduler&);

private:
    ScIpcServer _ipcServer;
    bool _haveToManageCommands;
    string _scOrderFileName;
    ACS_CLASS_DECLARE_DEBUG_LEVEL (tlSchedulerApp)
};

ACS_CLASS_DEFINE_DEBUG_LEVEL (tlSchedulerApp)

namespace {
    // Needed by plugins due to linkage issue
	ScAbstractCommand __sac;

    Application::GenericBuilder<tlSchedulerApp> theApplicationBuilder(SCHEDULER_APPNAME, SCHEDULER_SUBSYSTEM) ; // PRQA S 1021
}
#ifndef TL_SIMULATOR

namespace {
    const char* namespaces[] =
    {
      "Scheduler",
      "OrderTypeManager"
    };
}


void
tlSchedulerApp::loadScConfiguration()
{
  //first: load general configuration spaces

  for (size_t i=0; i<sizeof(namespaces)/sizeof(namespaces[0]); i++) // PRQA S 3084
  {
    excout << "Scheduler: requesting configuration space " << namespaces[i] << "\n";
    addSpace(ConfigurationSingleton::ConfSpace(namespaces[i])); // PRQA S 3081
  }
  loadConfiguration(false) ;
}

void tlSchedulerApp::loadPlugins()
{
	excout << "Scheduler: loading plugins\n";
	// don't exit on plugin loading errors...
	// leave a message in the log and go ahead
	try
	{
		rsResourceSet& rs ( ConfigurationSingleton::instance() -> get() );
		vector<string> pluginsPaths;
		{
			//Just one dir where all plugins are located. Means each plugins is independent by each others - (retro-compatible)
			if ( rs.hasKey(_scPluginsPathKey) ) 
			{
				string pluginsPath("");
				rs.getValue (_scPluginsPathKey, pluginsPath);
				pluginsPaths.push_back(pluginsPath);
			}
			//More than one dir where plugins are located. Means plugins in dir P2 depends by plugins in dir P1. P1 must be loaded before P2.
			else
			{
				rs.getArray( _scListOfPluginsPathKey, pluginsPaths );
			}
			
		}

		DirPluginSet myPluginSet (pluginsPaths, _pluginRegExp);
		myPluginSet.load (_pluginLoader);
		if (!_pluginLoader.load())
		{
			ACS_LOG_WARNING("Plugin loader failed. Some plugins have not been installed") ;
			const Plugin& failed = _pluginLoader.failed(true);
			ACS_LOG_ERROR(failed.openError()) ;
		}
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING("exception caught while loading plugins") ;
		ACS_LOG_NOTIFY_EX (ex);
	}
}
#endif // TL_SIMULATOR

void
tlSchedulerApp::manageSignals (ThreadedScheduler& theScheduler)
{
	// SigInt is IGNORED due to legacy issue
	if (hasStopModeChanged() && ( Application::ImmediateStop == getCurrentStopMode() ) )
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Aborting the Scheduler");
		try
		{
			theScheduler.abortBySigTerm();
		}
		catch (ThreadedScheduler::ThreadedSchedulerException& ex)
		{            
			// The scheduler is not running... not a problem, the application will exit soon
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Exception caught: " << ex.what() );
		}
		catch (exception &x)
		{
			// an unexpected exception. Log it but continue the run.
			ACS_LOG_ERROR("unexpected exception caught while aborting the scheduler: " << x.what()) ;
		}
	}
}

void
tlSchedulerApp::manageCommands (ThreadedScheduler& theScheduler) // PRQA S 4020
{
    if (!_haveToManageCommands)
    {
       // The IPC thread is not running... exit now and avoid issuing any kind of warning
       // The warning message has to be emitted once, the first time the thread is found not running

       // Exiting...
       ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Commands management disabled!");
       return;
    }

    if (!_ipcServer.isListening())
    {
        // No IPC Server running.
	    // The stdin stream was closed. There is no way to receive command
	    // Emit a warning message and don't try to manage commands anymore

	    _haveToManageCommands = false;
	    ACS_LOG_WARNING("commands communication channel disabled. Continuing without managing stdin commands.") ;
	    return;
    }
    
    // manage one command per cycle
    TlMessage theMessage;
    try
    {
	if (_ipcServer.popMessage(theMessage))
	{
	    excout << "tlSchedulerApp: processing command [" << theMessage.cmdType() << ", " << theMessage.cmdValue() << ", " << theMessage.paramValue() << "]\n";
	    if (theMessage.cmdType() == TlMessage::suspendType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Suspending");
    		theScheduler.suspend();
	    }
	    else if (theMessage.cmdType() == TlMessage::resumeType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Resuming");
    		theScheduler.resume();
	    }
	    else if (theMessage.cmdType() == TlMessage::stopType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Stopping");
    		theScheduler.stop();
	    }
	    else if (theMessage.cmdType() == TlMessage::abortType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Aborting");
    		theScheduler.abort();
	    }
	    else if (theMessage.cmdType() == TlMessage::killType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Killing");
    		theScheduler.doKill();
	    }
	    else if (theMessage.cmdType() == TlMessage::userType)
	    {
            ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Sending user command [" << theMessage.cmdValue() << ", " << theMessage.paramValue() << "]");
    		theScheduler.sendCmd (theMessage.cmdValue(), theMessage.paramValue());
	    }
	    else
	    {
		    // Unknown message type... just drop the message and notify the event
		    ACS_LOG_WARNING("unknown command type received from stdin [" << theMessage.cmdType() << "]");
	    }
	}
    } // try
    catch (exception& ex)
    {
	    // cannot process the command. Drop the message and notify the event
	    ACS_LOG_WARNING("exception caught while processing command [" << theMessage.cmdType() << "]") ;
	    ACS_LOG_NOTIFY_EX(ex);
    }

}



int tlSchedulerApp::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
	// optional parameters [-nomcf] [-dbconf] are managed my BASEAPP = CryosatApp
    if (argc == 1)
    {
      excerr << "Scheduler: missing argument. Usage:\n\t" << argv[0] << " filename [-nomcf] [-dbconf]\n";
      return ssNO_ORDER;
    }

	// APF-271
	mc::MCParameters::instance()->setDefaultFunction ("Processing Chain");
	mc::MCParameters::instance()->setDefaultCode ("DPR");
    
    dbConnectionPool *pool = getDefaultConnectionPool() ;
 	if (0 != pool) {
        pool->setMaxConnectionAttempts(3) ; // PRQA S 4400
    }
    else {
        ACS_LOG_WARNING("No default connection pool to set max connection attempts") ;
    }
    
    // initialise the log file
    string wo;
    if (argc > 1)
    {
        // look for workorder filename
        // it should be the first positional parameter jumping all "-parname" before
        int scorderIndex = 0;
        for (scorderIndex=1; scorderIndex<argc; scorderIndex++) { // PRQA S 4230
            if (strncmp(argv[scorderIndex], "-", 1) == 0) {
                continue;
            }
            else {
                break;
            }
        }
        if (scorderIndex == argc) {
            excerr << "Scheduler: missing filename. Usage:\n\t" << argv[0] << " filename [-nomcf] [-dbconf]\n";
            return ssNO_ORDER;
        }

        string pathname = argv[scorderIndex] ;
        wo = File::getFileName(pathname);
        _scOrderFileName = pathname;
    }
    else {
        wo = "no_work_order";
    }


    SchedulerExitStatus exitCode = ssUNDEFINED ;

#ifdef TL_SIMULATOR

    // NO LOG FILE  and NO LOAD OF THE CONFIGURATION SPACES

#else  // TL_SIMULATOR

  ostringstream logName;
  {
    logName << defaultLogPath << "/Scheduler_" << wo << ".log";

    // try to retrieve scheduler log file name from the scheduler work order
    // if the workorder cannot be load the message will be put on the default log

    try
    {
        scOrder o(_scOrderFileName);
        if (o.getSchedulerLogName() != "") {
            logName.str(o.getSchedulerLogName());
        }
    }
    catch (exception &)
    {
      // do nothing... the failure will be notified in the next try-catch block
    }

    vector<exostream *> streams ;
    streams.push_back(&exostream::getDefaultOutLogger()) ;
    streams.push_back(&excout) ;

	bool append = false;
#ifdef SCHEDULER_LOG_IN_APPEND
#warning "SCHEDULER_LOG_IN_APPEND defined"
	append = true;
#endif
    exLogStream log("log", logName.str(), streams, append);
    exostream::getDefaultOutLogger().allowStackTrace(true);
    excout.allowStackTrace(true);

    Application::instance()->setRotateLogParametersFromConf(false);

	try {
    	loadScConfiguration();
    	loadPlugins();
    }
    catch (exception &e)
    {
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Scheduler: exiting with code ssINTERNAL_ERR (" << int(ssINTERNAL_ERR) << ")") ; // PRQA S 3081
		exitCode = ssINTERNAL_ERR;
    }

#endif // TL_SIMULATOR
	if( exitCode == ssUNDEFINED ) {
    	exitCode = execute (argc, argv, env);
	}

#ifndef TL_SIMULATOR

  } // destroy the exLogStream object and close the logfile

#ifdef RENAME_LOG_ON_EXIT
  // rename the log file according to exitCode value
  ostringstream newLogName;
  newLogName << logName.str();
  if (exitCode == ssOK)
  {
     newLogName << ".COMPLETED";
  }
  else
  {
     newLogName << ".ERROR";
  }
  try
  {
     File::rename (logName.str(), newLogName.str());
  }
  catch (exException& ex)
  {
     // This message will not be logged
     excerr << "Scheduler: unable to rename the logFile\n";
     excerr.notify (ex);
  }
  catch (...)
  {
     // This message will not be logged
     excerr << "Scheduler: unknown exception while renaming [" << logName.str() << "] in [" << newLogName.str() << "]\n";
  }
#endif //RENAME_LOG_ON_EXIT

#endif //TL_SIMULATOR
    // no log here... nominal exit messages have been already logged
    return exitCode;
}





SchedulerExitStatus
tlSchedulerApp::execute(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
    // load work order
    //
    // make here a pre-load of xml files, so we are sure the tlScheduler constructor
    // doesn't fail, and we can exit the right error code
    try
	{
		scOrder o(_scOrderFileName);
		
		excout << "Scheduler: work order " << _scOrderFileName << " successfully loaded.\n";
		
		// now load task table
		try
		{

			//Build key and get task table
			//*/
			StringKey key = StringKeyUtils::makeKey( o.getSatellite(), o.getMission(), o.getProcName(), o.getProcVersion() );
			ACS_SMARTPTR<TaskTable> tt( TaskTableFactoryChain::instance()->newObject( key ) );
			tt->readFromFile(o.getTaskTable());
			/*/
			TaskTable tt(o.getTaskTable());
			//*/

			
			excout << "Scheduler: TaskTable " << o.getTaskTable() << " successfully loaded.\n";
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				  excout << "Other parameters:\n";
				  excout << "\tLogfile Name: "		<< o.getLogFileName() << "\n";
				  excout << "\tWorking Dir: "		<< o.getBaseDir() << "\n";
				  excout << "\tCreate Working Dir: " << boolalpha << o.doICreateDir() << noboolalpha << "\n\n";
			ACS_CLASS_END_DEBUG
		}
		catch (exception& toCatch)
		{
			ACS_LOG_ERROR("ERROR while reading TaskTable " << o.getTaskTable()) ;
			ACS_LOG_NOTIFY_EX(toCatch);
			ACS_LOG_ERROR("Scheduler: exiting with code ssINTERNAL_ERR (" << int(ssINTERNAL_ERR) << ")") ; // PRQA S 3081
			return ssINTERNAL_ERR;
		}
	}
	catch (exception& toCatch)
	{
		ACS_LOG_ERROR("ERROR: while reading work order " << _scOrderFileName) ;
		ACS_LOG_NOTIFY_EX(toCatch);
		ACS_LOG_ERROR("Scheduler: exiting with code ssNO_ORDER (" << int(ssNO_ORDER) << ")") ; // PRQA S 3081
		return ssNO_ORDER;
	}


    // let's go

    //tlScheduler theSched(_scOrderFileName);
    SchedulerExitStatus exitCode = ssUNDEFINED;
    ThreadedScheduler theSched;

    // Main loop starts here
    try
    {
	    theSched.execute(scOrder(_scOrderFileName)); // PRQA S 3081
	    _ipcServer.startListening();

	    while (!theSched.getExitStatus(exitCode))
	    {
		    manageSignals(theSched);
		    manageCommands(theSched);
		    theSched.timedWaitExit (100); // 100 msecs delay if running // PRQA S 4400
	    }
    }
    catch(exception &e)
    {
      ACS_LOG_NOTIFY_EX(e) ;
      ACS_LOG_ERROR( "Scheduler: exiting with code ssINTERNAL_ERR (" << int(ssINTERNAL_ERR) << ")") ; // PRQA S 3081
      return ssINTERNAL_ERR;
    }

	 _ipcServer.stopListening();

    // no log here... nominal exit messages have been logged inside the main loop
    return exitCode;
}


_ACS_END_NAMESPACE
