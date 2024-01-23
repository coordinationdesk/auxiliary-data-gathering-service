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

	$Prod: ADGS Standalone App $

*/

#include <ADGSStandaloneApp.h>
#include <Filterables.h>
#include <ladgssaV.h>

using namespace std;

namespace {
    acs::ladgssaV Version;
}

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ADGSStandaloneApp) 


ADGSStandaloneApp::ADGSStandaloneApp(string const & name, string const &subSystem, bool nomcf):
 StandaloneApp (name, subSystem), // PRQA S 4052
 _adgsLogFormatter(),
 _syslog(),
 _logFilter()
 
{
    createSyslogHandler() ;
} 

ADGSStandaloneApp::~ADGSStandaloneApp() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}


int ADGSStandaloneApp::start(int argc, char const * const * argv, char const * const * env)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	return StandaloneApp::start (argc, argv, env);
}

void ADGSStandaloneApp::preDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


void ADGSStandaloneApp::postDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	createSyslogHandler() ;
}

void ADGSStandaloneApp::createSyslogHandler() 
{
    _syslog.reset(new exSyslog("syslog")) ;
    _logFilter.reset(new exAdaptiveFilter(_syslog.get())) ;
    _logFilter->allowPrivateInfo(false) ;
}

_ACS_END_NAMESPACE
