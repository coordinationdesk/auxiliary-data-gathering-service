/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV App $


*/


#ifndef _SRVApp_h_
#define _SRVApp_h_

#include <Application.h>
#include <SRVLogFmtFactory.h>
#include <exAdaptiveFilter.h>


_ACS_BEGIN_NAMESPACE(acs)

class SRVApp : public Application {
public:

	SRVApp(const std::string& name, const std::string& subSystem);
	virtual ~SRVApp() ACS_NOEXCEPT;

protected:

	/*
	 * inherited from Application
	 */
	virtual void postDaemonize();
	virtual void preDaemonize();
	virtual void initPlugins() ;
	virtual int start(int argc, char const * const * argv, char const * const * env);
	virtual void justBeforeStartEnd();

	// returns the default log size when rotation occur
	virtual std::streampos getDefaultRotationLogSize() const ;

	// returns the default log age when rotation occur
	virtual size_t getDefaultRotationLogAgeSec() const ;

	// returns the default log num to save
	virtual unsigned int getDefaultRotationLogNumToSave() const ;

	// returns the default log num to save
	virtual bool getDefaultRotationLogCompressOld() const ;

	// returns if default is to append or not
	virtual bool getDefaultRotationAppend() const;

	// returns the default log mode
	virtual Application::LogRotationPolicy::Mode getDefaultRotationMode() const;

    virtual int internalusage(const std::string &exename) const ;

private:
	SRVApp();	// Declared but not implemented
	SRVApp(const SRVApp& ); // Declared but not implemented
	SRVApp &operator=(const SRVApp& ); // Declared but not implemented

    void createSyslogHandler() ;

private:
	SRVLogFmtFactory _logFormatter;
    std::unique_ptr<exSyslog> _syslog ;
    std::unique_ptr<exAdaptiveFilter> _logFilter ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVApp);
};

_ACS_END_NAMESPACE

#endif /* _SRVApp_h_ */
