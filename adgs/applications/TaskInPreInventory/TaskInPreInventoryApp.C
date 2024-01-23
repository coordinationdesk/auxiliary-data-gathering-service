// PRQA S 1050 EOF
/*
    Copyright 1995-2014, Advanced Computer Systems , Inc.
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
    Revision 2.4  2017/11/15 12:38:36  chicas
    some message to PRIVINFO

    Revision 2.3  2017/05/18 12:57:41  tergem
    Log specific definition restored

    Revision 2.2  2017/05/04 09:01:44  tergem
    S2PDGS-1641: call to LogRotationPolicy suppressed

    Revision 2.1  2017/05/02 10:50:44  tergem
    S2PDGS-1641 log harmonization

    Revision 2.0  2016/11/22 09:58:18  marfav
    Major release

    Revision 1.9  2016/11/22 09:57:58  marfav
    S3MPM-847
    Adding functions to dump plugin capabilities
    Removed dependency from libSendMessage

    Revision 1.8  2015/07/07 08:47:01  matteo.airoldi
    Logname unique

    Revision 1.7  2014/02/17 13:48:01  marpas
    coding best practices applied

    Revision 1.6  2013/07/19 07:38:41  chicas
    streams management fixed

    Revision 1.5  2013/05/29 16:03:52  marpas
    useless statements removed

    Revision 1.4  2013/03/14 13:56:31  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.3  2013/03/05 09:42:27  chicas
    Modified log path in local dir

    Revision 1.2  2013/02/21 18:44:44  marpas
    dbConnectionPool used

    Revision 1.1.1.1  2012/11/30 12:44:54  chicas
    Imported sources




*/

#include <TaskInPreInventoryApp.h>
#include "TaskInPreInventoryAppV.h"

#include <Filterables.h>

#include <boost/program_options.hpp> // PRQA S 1013
namespace po = boost::program_options;


using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskInPreInventoryApp)

namespace {
    TaskInPreInventoryAppV version;
    PROJECT_APP::GenericBuilder<TaskInPreInventoryApp> theApplicationBuilder("TaskInPreInventory","PF");
}


TaskInPreInventoryApp::TaskInPreInventoryApp(const std::string& subsys, const std::string& app):
    PROJECT_APP("TaskInPreInventory","PF"),
    TaskInPreInventory(),
    PreInventoryCommonApp( applicationName() )
{
   static char Id[] = "@(#) ACS: $Id$";
   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id) ;
}

TaskInPreInventoryApp::~TaskInPreInventoryApp() throw() {
   ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}

int TaskInPreInventoryApp::main(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{

    //Command Line printing - Useful for log inspection
    {
        ostringstream msg; msg<<"TaskInPreInventoryApp::main: Executing: ";
        for ( int i=0; i<argc; i++  ) { msg<<argv[i]<<" "; }
        ACS_LOG_PRIVINFO(msg.str());
    }

    //Command Line option definition
    //General Definition
    string opt;
    po::options_description optGeneralDef(string("\nUsage: ")+argv[0]+" <Joborder file>\nwith the following parameter"); // PRQA S 3081
    optGeneralDef.add_options()
        ("cap",         "Show capabilities and exit.")
        ("satellite",   po::value< string >(&opt)->default_value(""), "arg: <Satellite Name>. Not Mandatory")
        ("mission",     po::value< string >(&opt)->default_value(""), "arg: <Mission>. Not Mandatory")
        ("procname",    po::value< string >(&opt)->default_value(""), "arg: <Processor Name>. Not Mandatory")
        ("procversion", po::value< string >(&opt)->default_value(""), "arg: <Processor Version>. Not Mandatory");

    //JobOrder is positional
    po::options_description optPositDef("Positional parameters");
    optPositDef.add_options()
        ("jobOrder", po::value< string >(), "arg: <Joborder file>");

    //Add special Positional parameter in tail. Useful to ignore other parameter passed in command line
    optPositDef.add_options()( PreInventoryCommonApp::c.IgnoredPositionals.c_str(), boost::program_options::value< vector<string> >() );


    //Merge all the option definitions
    po::options_description optAllDef("All options");
    optAllDef.add(optGeneralDef).add(optPositDef);

    //JobOrder is positional
    po::positional_options_description p;
    p.add("jobOrder", 1);

    //Add The special Ignoring Positional parameter in tail. -1 means one or more positional. After -1 cannot call other posPars.add func.
    p.add( PreInventoryCommonApp::c.IgnoredPositionals.c_str(), -1 );


    po::variables_map argvParameters;

    boost::program_options::command_line_parser parser(argc, const_cast<char**>(argv)); // PRQA S 3081
    parser.options(optAllDef).positional(p).allow_unregistered();
    boost::program_options::parsed_options parsed = parser.run();

    //@@@
    //Note! allow_unregistered() function instruct parser to ignore unknown options and don't let
    // 'store' function throwing 'unknown_option' exception.
    //But because a bug in boost 1.33.1, store throw anyway the the "unknown_option" exception.
    // When boost 1.33.1 will be obsolete in our environment,
    // remove the try - catch (boost::program_options::unknown_option& ex) section.
    //@@@
    try {
        po::store(parsed, argvParameters);
    } catch (boost::program_options::unknown_option& ex) {
        ACS_LOG_INFO("Some command line parameters ignored by PreInventory: " << ex) ;
    }

    po::notify(argvParameters);

    //Print Named Unrecognized options
    vector<string> unrecognizedOptions =
        boost::program_options::collect_unrecognized(parsed.options, boost::program_options::exclude_positional);
    for ( vector<string>::iterator it = unrecognizedOptions.begin(); it != unrecognizedOptions.end(); ++it ) { // PRQA S 4238
        ACS_LOG_WARNING("Ignored Option: "<<*it) ;
    }
    //Print Positionals Unrecognized options
    if ( 0 != argvParameters.count( PreInventoryCommonApp::c.IgnoredPositionals ) ) {
        ACS_LOG_WARNING("Some command line positional parameters ignored by PreInventory.") ;
        vector<string> ignoredParams = argvParameters[PreInventoryCommonApp::c.IgnoredPositionals].as< vector<string> >();
        for (vector<string>::iterator it1 = ignoredParams.begin(); it1 != ignoredParams.end(); ++it1) { // PRQA S 4238
            ACS_LOG_WARNING("Ignored Positional Option: "<<*it1) ;
        }
    }


    // In case have to show capabilities there is no need to check the command line anymore
    if ( argvParameters.count("cap"))
    {
        doShowCapabilities (true);
    }
    else
    {

        //MANDATORY PARAMETERS CHECK
        if ( 0 == argvParameters.count("jobOrder") ) {
            ostringstream os ;
            os << optGeneralDef ;
            ACS_LOG_ERROR(os.str());
            return 1;
        }

        ACS_LOG_PRIVINFO("jobOrder:"<<argvParameters["jobOrder"].as<string>()
                         <<" satellite:"<<argvParameters["satellite"].as<string>()
                         <<" mission:"<<argvParameters["mission"].as<string>());

        ostringstream pid;
        pid << getpid();

        if (&exostream::getDefaultOutLogger() != &excmcf) {
            exostream::getDefaultOutLogger().appendChild(&excmcf) ;
        }

        // Merge all streams into the log
        vector<exostream *> streams ;
        streams.push_back(&exostream::getDefaultOutLogger()) ;
        streams.push_back(&excout) ;

        //create the log files the first time
        string logName ;
        {
            DateTime now ;
            ostringstream os ;
            now.asISO8601(os,6) ; // PRQA S 4400
            logName = os.str() ;
            // remove ':'
            string::iterator new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),':')) ;
            logName.erase(new_end, logName.end());

            // remove '-'
            new_end = remove_if(logName.begin(), logName.end(), bind2nd(equal_to<char>(),'-')) ;
            logName.erase(new_end, logName.end());
        }

        string logFullPath =  "./" + applicationName() + "_" + logName + ".log" ; // create the log path name
        // Rotate keeping 20 logs, compressed.
        // The size has no effect as I don't call the rotate during the execution
        LogRotationPolicy lrp(logFullPath, streams,LogRotationPolicy::FirstApplying, defaultLogAge, (1024*1024*1024), 20, true) ;
        setLRPolicy(lrp) ;
        //rotateLog() ; // open the first log
    }
    try {

        this->getConfiguration();
        rotateLog() ; // open the first log
        AppPluginLoaderNotifier pln(this) ;

        int status = this->execute(argvParameters, env, *getDefaultConnectionPool(), pln);

        ACS_COND_THROW( (status!=0) && (!doShowCapabilities()), exTaskInPreInventoryAppCriticalException("Exiting in error")); // PRQA S 3081

        return status;
    } catch (exception &e) {

        ACS_LOG_NOTIFY_EX(e) ;
        ACS_LOG_EVENT("STATUS",EXFE_ERROR);
        ACS_LOG_ERROR("Exiting in ERROR : " << e.what() ) ;
        return(0xff); // PRQA S 4400
    }
}


