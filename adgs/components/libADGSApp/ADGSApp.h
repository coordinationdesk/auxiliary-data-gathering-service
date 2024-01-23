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


#ifndef _ADGSApp_H_
#define _ADGSApp_H_

#include <acs_c++config.hpp>
#include <Application.h>
#include <ADGSLogFmtFactory.h>
#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class exLogStream ;
class exostream ;
class exAdaptiveFilter ;

class ADGSApp : public Application // PRQA S 2109
{
public:
	ADGSApp(std::string const & name, std::string const &subSystem);
	virtual ~ADGSApp() throw();

protected:

	/**
	 * inherited from Application
	 */	
	virtual void postDaemonize();
	virtual void preDaemonize();

	virtual int start(int argc, char const * const * argv, char const * const * env);
	virtual void initPlugins() ;
	virtual void justBeforeStartEnd() ;
	exAdaptiveFilter *getAdaptiveFilter() ;

	virtual void dbConfArg(const std::string &) ;
    virtual int internalusage(const std::string &exename) const ;
private:
	// reimplemented from Application
	virtual void beforeRestart();

	// declared but not implemented to prevent their use
	ADGSApp();
	ADGSApp(const ADGSApp & );
	ADGSApp &operator=(const ADGSApp & );

	// Syslog handler management
	void createSyslogHandler() ;
	void destroySyslogHandler() throw() ;

private:
	exAdaptiveFilter *_adaptiveFilter ;

	std::string _dbConfArg ;
	ADGSLogFmtFactory _theLogFormatterFactory ;

	exSyslog * _syslog ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ADGSApp) ;
};

_ACS_END_NAMESPACE

#endif //_ADGSApp_H_

