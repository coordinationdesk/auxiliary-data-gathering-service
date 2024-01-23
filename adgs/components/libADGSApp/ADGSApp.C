// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: ADGS App $

*/


 

#include <ADGSApp.h>
#include <ladgsaV.h>
#include <StandaloneResourceSpaceFactory.h>
#include <dbCryosatFactory.h>
#include <dbSpaceFactory.h>
#include <dbDataFactory.h>
//#include <ADGSDBFactory.h>
#include <dbConnectionPool.h>
#include <exAdaptiveFilter.h>
#include <Filterables.h>
//#include <CryosatFixedHeaderFactory.h>
//#include <GoceFixedHeaderFactory.h>
//#include <AeolusFixedHeaderFactory.h>
//#include <EnvisatFixedHeaderFactory.h>
//#include <VenusFixedHeaderFactory.h>
//#include <OcadaFixedHeaderFactory.h>
#include <dbAppResSet.h>
#include <SMPluginLoader.h>
#include <DateTime.h>
#include <XMLOstream.h>
#include <S3PDGSFixedHeaderFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    ladgsaV Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ADGSApp) 

ADGSApp::ADGSApp(string const & name, string const &subSystem):
 Application(name, subSystem), // PRQA S 4052
 _adaptiveFilter(0),
 _dbConfArg(""),
 _theLogFormatterFactory(true, true, "", "", "|"),
 _syslog(0)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "name " << name << " " 
               << "subsystem " << subSystem ) ;
	exMsgFmtFactory::instance()->appName(name) ;
    createSyslogHandler() ;
} 

ADGSApp::~ADGSApp() throw()
{
// PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
// PRQA L:L1
	destroySyslogHandler() ;
}



void ADGSApp::postDaemonize() 
{
}

void ADGSApp::preDaemonize() 
{
}

void ADGSApp::createSyslogHandler() 
{
	destroySyslogHandler() ;
	_syslog = new exSyslog("syslog") ;
	_adaptiveFilter = new exAdaptiveFilter(_syslog) ;
	_adaptiveFilter->allowPrivateInfo(false) ;
}


void ADGSApp::destroySyslogHandler() throw() 
{
    delete _syslog ; _syslog = 0 ; 
}

int ADGSApp::start(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START.. ") ;
	
    const vector<string> &parsedArgs = getParsedArgs();

	string dbConf ;

	if (parsedArgs.size() > 0)
	{
        vector<string> newParsedArgs ;
		// check if -nomcf was given
	    const string dbConfArg  = "-dbconf" ;
	    const string dbConfArg2 = "--dbconf" ;

        size_t pargsize = parsedArgs.size() ;
		for (size_t i=0; i < pargsize; i++) { // PRQA S 4235, 4236
            if (( (dbConfArg == parsedArgs[i]) || (dbConfArg2 == parsedArgs[i]) ) && 
                (i < parsedArgs.size()-1)) { // PRQA S 3084
			    dbConf = parsedArgs[++i] ; // note advance i
		    }
            else { newParsedArgs.push_back(parsedArgs[i]) ; }
        }
        setParsedArgs(newParsedArgs) ;
	}

    dbAppResSet::setDefaultDbConfName(dbConf) ; // set a new default name

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbconf: \"" << dbConf << "\"") ;

	// factories and handler
	dbDataFactory dataFactory ;
	StandaloneResourceSpaceFactory resourcesFactory ;
	dbSpaceFactory spaceFactory ;
	dbCryosatFactory cryosatFactory ;
	//CryosatFixedHeaderFactory cryosatFixedHeaderFactory ;
	S3PDGSFixedHeaderFactory s3FixedHeaderFactory ; 
//	GoceFixedHeaderFactory goceFixedHeaderFactory ; 
//	AeolusFixedHeaderFactory aeolusFixedHeaderFactory ;
//	EnvisatFixedHeaderFactory envisatFixedHeaderFactory ;
//	VenusFixedHeaderFactory venusFixedHeaderFactory ;
//	OcadaFixedHeaderFactory ocadaFixedHeaderFactory ;
//	CDPDBFactory cdpDbFactory;
    setDefaultConnectionPool(&db::ConnPools::instance()->getPool("")) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to call Application::start method.") ;
	try
	{	int res = -1 ;
		initPlugins() ;
        try {
		    res=Application::start(argc, argv, env);
		    justBeforeStartEnd() ;
        }
        catch(exception &) {
            try {
	            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Releasing connection pool") ;
                db::ConnPools::instance()->release() ;
            }
            catch(exception &x) {
                ACS_LOG_NOTIFY_EX(x) ;
            }
            throw ;
        }
        
        try {
	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Releasing connection pool") ;
            db::ConnPools::instance()->release() ;
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
		exostream::getDefaultOutLogger().flush() ;
		excout.flush() ;
		return res;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
		excout << e ;
		exostream::getDefaultOutLogger().flush() ;
		excout.flush() ;
		return 255; // PRQA S 4400
	}
	catch(...)
	{
		try {
			throwException(exCriticalException("Unknown exception caught")) ; // PRQA S 3081
		}
		catch(exException &x) {
		    ACS_LOG_NOTIFY_EX(x) ;
		    excout << x ;
		}
		exostream::getDefaultOutLogger().flush() ;
		excout.flush() ;
		return 255;  // PRQA S 4400
	}
}




int ADGSApp::internalusage(const std::string &exename) const 
{
    int ret = Application::internalusage(exename) ;
	excerr << "\n\tS3PDGS options:\n" ;
	string dbConfArgS = (not _dbConfArg.empty()) ? _dbConfArg : "-dbconf" ; // PRQA S 3384, 3385
	excerr << "\t" << dbConfArgS << " [conf file]   : uses a specific dbconf file\n" ;
	excerr << "\n" ;
	return ret ;
}

void ADGSApp::initPlugins() 
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "path: " << dbAppResSetFactory::instance(dbAppResSet::getDefaultDbConfName(),"")->getSMPluginPath()) ;

	SMPluginLoader::init(dbAppResSetFactory::instance(dbAppResSet::getDefaultDbConfName(),"")->getSMPluginPath()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "done") ;
}

void ADGSApp::justBeforeStartEnd() 
{
}

exAdaptiveFilter *ADGSApp::getAdaptiveFilter() // PRQA S 4120, 4211
{
	return _adaptiveFilter ;
}

	
void ADGSApp::beforeRestart()
{
	/* kill Patrol */
	preDaemonize();
}

void ADGSApp::dbConfArg(const std::string &d) 
{
	_dbConfArg = d ;
}
_ACS_END_NAMESPACE
