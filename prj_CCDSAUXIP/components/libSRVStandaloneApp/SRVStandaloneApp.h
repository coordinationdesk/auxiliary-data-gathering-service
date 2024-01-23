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



#ifndef _SRVStandaloneApp_H_
#define _SRVStandaloneApp_H_

#include <acs_c++config.hpp>
#include <StandaloneApp.h>
#include <SRVLogFmtFactory.h>
#include <exAdaptiveFilter.h>
#include <memory>


_ACS_BEGIN_NAMESPACE(acs)

// This class does not override the StandaloneApp behavior
// it just instantiates the SRV specifc log formatter

class SRVStandaloneApp : public StandaloneApp // PRQA S 2109
{

public:
	SRVStandaloneApp(std::string const & name, std::string const &subSystem, bool nomcf=true);
	virtual ~SRVStandaloneApp() throw() ;

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
	SRVStandaloneApp();
	SRVStandaloneApp(const SRVStandaloneApp & );
	SRVStandaloneApp &operator=(const SRVStandaloneApp & );

    void createSyslogHandler() ;

private:
    SRVLogFmtFactory _srvLogFormatter;
    std::unique_ptr<exSyslog> _syslog ;
    std::unique_ptr<exAdaptiveFilter> _logFilter ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVStandaloneApp) ;
};


_ACS_END_NAMESPACE


#endif //_SRVStandaloneApp_H_

