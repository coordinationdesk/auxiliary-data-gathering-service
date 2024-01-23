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

#ifndef _ADGSStandaloneApp_H_
#define _ADGSStandaloneApp_H_

#include <acs_c++config.hpp>
#include <StandaloneApp.h>
#include <ADGSLogFmtFactory.h>
#include <exAdaptiveFilter.h>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

// This class does not override the StandaloneApp behavior
// it just instantiates the Sentinel-1 specifc log formatter

class ADGSStandaloneApp : public StandaloneApp // PRQA S 2109
{

public:
	ADGSStandaloneApp(std::string const & name, std::string const &subSystem, bool nomcf=true);
	virtual ~ADGSStandaloneApp() throw() ;

protected:

	/**
	 * inherited from Application
	 */	
	virtual int start(int argc, char const * const * argv, char const * const * env);

protected:

	/**
	 * inherited from Application
	 */	
	virtual void postDaemonize();
	virtual void preDaemonize();

private:

	// declared but not implemented to prevent their use
	ADGSStandaloneApp();
	ADGSStandaloneApp(const ADGSStandaloneApp & );
	ADGSStandaloneApp &operator=(const ADGSStandaloneApp & );

    void createSyslogHandler() ;

private:
    ADGSLogFmtFactory _adgsLogFormatter;
    ACS_SMARTPTR<exSyslog> _syslog ;
    ACS_SMARTPTR<exAdaptiveFilter> _logFilter ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ADGSStandaloneApp) ;
};

_ACS_END_NAMESPACE

#endif //_ADGSStandaloneApp_H_

