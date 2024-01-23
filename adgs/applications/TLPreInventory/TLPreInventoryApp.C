/*
    Copyright 1995-2008, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libTLPreInventoryApp$

    $Id$

    $Author$

    $Log$
    Revision 2.1  2016/11/22 10:04:33  marfav
    Removed dependency from libSendMessage

    Revision 2.0  2016/11/21 16:18:18  marfav
    Major release

    Revision 1.13  2016/11/21 16:18:04  marfav
    S3MPM-847
    Adding --cap in the command line
    Using plugin capabilities dump from libPreInventory

    Revision 1.12  2016/07/08 14:31:25  marfav
    CSGACQ-84
    New project/application based log rotation management strategy enforced

    Revision 1.11  2016/05/11 14:50:44  danalt
    use of ACS_THROW and ACS_COND_THROW

    Revision 1.10  2016/02/18 16:37:08  nicvac
    Minor. Cast fixed.

    Revision 1.9  2015/07/08 09:25:58  matteo.airoldi
    Now the logfile is created in the WorkingDir

    Revision 1.8  2015/06/25 14:48:37  damdec
    Compilation warnings fixed.

    Revision 1.7  2013/03/14 14:58:51  marpas
    connection pool concept is left to class inheriting from Application

    Revision 1.6  2013/02/21 18:35:15  marpas
    dbConnectionPool used

    Revision 1.5  2012/02/14 15:18:48  marpas
    fixing dtor

    Revision 1.4  2009/04/10 15:48:11  nicvac
    Command line printing. Useful in log inspection.

    Revision 1.3  2009/01/28 17:08:05  nicvac
    Ignoring of extra Command line parameter.

    Revision 1.2  2008/11/27 16:56:44  nicvac
    Key parameters are not Mandatory.

    Revision 1.1.1.1  2008/10/17 14:21:05  nicvac
    Import TLPreInventory


*/

#include <TLPreInventoryApp.h>
#include "TLPreInventoryAppV.h"

#include <Filterables.h>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#ifndef TLPREINVENTORY_SUBSYSTEM
#define TLPREINVENTORY_SUBSYSTEM "IPF"
#endif

#ifndef TLPREINVENTORY_APPNAME
#define TLPREINVENTORY_APPNAME "TLPreInventory"
#endif

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)
ACS_CLASS_DEFINE_DEBUG_LEVEL(TLPreInventoryApp)

static TLPreInventoryAppV version;

PROJECT_APP::GenericBuilder<TLPreInventoryApp> theApplicationBuilder(TLPREINVENTORY_APPNAME, TLPREINVENTORY_SUBSYSTEM);

TLPreInventoryApp::TLPreInventoryApp(const std::string& app, const std::string& subsys):
    PROJECT_APP(app, subsys),
    PreInventoryCommonApp( applicationName() )
{
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
}

TLPreInventoryApp::~TLPreInventoryApp() throw() {
// PRQA S 4621 L1
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
 // PRQA L:L1
}

string TLPreInventoryApp::getDefaultLogName () const
{
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
    return logFullPath;
}


int TLPreInventoryApp::main(int argc, char const * const * argv, char const * const * env) {

    //Command Line printing - Useful for log inspection
    {
        ostringstream msg; msg<<"TLPreInventoryApp::main: Executing: ";
        for ( int i=0; i<argc; i++  ) msg<<argv[i]<<" ";
        msg<<endl;
        ACS_LOG_INFO(msg.str());
    }

    //Command Line option definition
    //General Definition
    string opt;
    po::options_description optGeneralDef(string("\nUsage: ")+argv[0]+" <Joborder file>\nwith the following parameter");
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

    //po::store(po::command_line_parser(argc, (char**)argv).options(optAllDef).positional(p).run(), argvParameters);
    boost::program_options::command_line_parser parser(argc, const_cast<char**>(argv));
    parser.options(optAllDef).positional(p).allow_unregistered();
    boost::program_options::parsed_options parsed = parser.run();

    //@@@
    //Note! allow_unregistered() function instruct parset to ignore unknown options and don't let
    // 'store' function throwing 'unknown_option' exception.
    //But because a bug in boost 1.33.1, store throw anyway the the "unknown_option" exception.
    // When boost 1.33.1 will be obsolete in our environment,
    // remove the try - catch (boost::program_options::unknown_option& ex) section.
    //@@@
    try {
        po::store(parsed, argvParameters);
    } catch (boost::program_options::unknown_option& ex) {
            ostringstream msg; msg<<"Some command line parameters ignored by PreInventory."<<endl; ACS_LOG_INFO(msg.str()) ;
    }

    po::notify(argvParameters);

    //Print Named Unrecognized options
    vector<string> unrecognizedOptions =
        boost::program_options::collect_unrecognized(parsed.options, boost::program_options::exclude_positional);
    for ( vector<string>::iterator it = unrecognizedOptions.begin(); it != unrecognizedOptions.end(); it++ ) {
        ostringstream msg; msg<<"Ignored Option: "<<*it<<endl; ACS_LOG_INFO(msg.str()) ;
    }
    //Print Positionals Unrecognized options
    if ( argvParameters.count( PreInventoryCommonApp::c.IgnoredPositionals ) ) {
        ostringstream msg; msg<<"Some command line positional parameters ignored by PreInventory."<<endl; ACS_LOG_INFO(msg.str());
        vector<string> ignoredParams = argvParameters[PreInventoryCommonApp::c.IgnoredPositionals].as< vector<string> >();
        for (vector<string>::iterator it = ignoredParams.begin(); it != ignoredParams.end(); it++) {
            ostringstream msg; msg<<"Ignored Positional Option: "<<*it<<endl; ACS_LOG_INFO(msg.str()) ;
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
        if ( !argvParameters.count("jobOrder") ) {
            ostringstream msg; msg<<optGeneralDef<<endl; ACS_LOG_INFO(msg.str());
            return 1;
        }

        {
            ostringstream msg; msg<<"TLPreInventoryApp::main : jobOrder:"<<argvParameters["jobOrder"].as<string>()<<" satellite:"<<argvParameters["satellite"].as<string>()<<" mission:"<<argvParameters["mission"].as<string>();
            ACS_LOG_INFO( msg.str() );
        }


        ostringstream pid;
        pid << getpid();
    }

    try {

        this->getConfiguration();
    rotateLog() ; // open the first log
        AppPluginLoaderNotifier pln(this) ;
        int status = this->execute(argvParameters, env, *getDefaultConnectionPool(), pln);

        ACS_COND_THROW( (status!=0) && (!doShowCapabilities()), exTLPreInventoryAppCriticalException("Exiting in error"));

        return status;
    } catch (exException &e) {
        string msg(applicationName()+" : Exiting in ERROR...");
        ACS_LOG_INFO(msg) ;
        ACS_LOG_NOTIFY_EX(e);
        try {
            ACS_LOG_EVENT("STATUS",EXFE_ERROR);
            ACS_LOG_ERROR(applicationName()+" : Exiting in ERROR");
        } catch (exException& e) {
            ACS_LOG_NOTIFY_EX(e);
        }
        return(0xff);
    }
}
_ACS_END_NAMESPACE


