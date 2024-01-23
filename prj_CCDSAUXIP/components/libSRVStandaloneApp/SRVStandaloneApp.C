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

	$Prod: SRV Standalone App $


*/


#include <SRVStandaloneApp.h>
#include <Filterables.h>
#include <SRVStandaloneAppV.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


namespace { // unnamed 
    SRVStandaloneAppV Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVStandaloneApp) 


SRVStandaloneApp::SRVStandaloneApp(string const & name, string const &subSystem, bool nomcf):
 StandaloneApp (name, subSystem),
 _srvLogFormatter(),
 _syslog(),
 _logFilter()
 
{
	exMsgFmtFactory::instance()->appName(name) ;
    createSyslogHandler() ;
} 

SRVStandaloneApp::~SRVStandaloneApp() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
}


int SRVStandaloneApp::start(int argc, char const * const * argv, char const * const * env)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	return StandaloneApp::start (argc, argv, env);
}

void SRVStandaloneApp::preDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


void SRVStandaloneApp::postDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	createSyslogHandler() ;
}

void SRVStandaloneApp::createSyslogHandler() 
{
    _syslog.reset(new exSyslog("syslog")) ;
    _logFilter.reset(new exAdaptiveFilter(_syslog.get())) ;
    _logFilter->allowPrivateInfo(false) ;
	_logFilter->allowDebug(false) ;
	_logFilter->allowError(false) ;
	_logFilter->allowEvent(false) ;
	_logFilter->allowMessage(false) ;
	_logFilter->allowWarning(false) ;
	_logFilter->allowOpAlert(false) ;
	_logFilter->allowMCFilterable(true);
	_logFilter->allowAnyFilterable(false);
}


_ACS_END_NAMESPACE

