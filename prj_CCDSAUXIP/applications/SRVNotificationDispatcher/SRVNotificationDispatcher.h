// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Notification Dispatcher $


*/
#ifndef _SRVNotificationDispatcher_H_
#define _SRVNotificationDispatcher_H_ 

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

class SRVNotificationDispatcher : // PRQA S 2109, 2153
	public PROJECT_APP 
{
public:
	exDECLARE_EXCEPTION(InternalServiceException, exException); // PRQA S 2131, 2502
	
public:
	SRVNotificationDispatcher( const std::string&, const std::string& );
	virtual ~SRVNotificationDispatcher() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

	// inherited from Application
// 	virtual void customSigIntHandler(); 
// 	virtual void customSigTermHandler(); 
	virtual int usage(const std::string &) const ;

private:
	// Deleted constructors
	SRVNotificationDispatcher(const SRVNotificationDispatcher&) = delete;
	SRVNotificationDispatcher & operator=(const SRVNotificationDispatcher&) = delete;

	bool loadConf();
	void mainLoop();

private:
	unsigned int _restartTime;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVNotificationDispatcher) ;

private:
	static const unsigned int _defaultRestartTime=3600*24 ; // [sec] (one day)
};

_ACS_END_NAMESPACE

#endif /* _SRVNotificationDispatcher_H_ */
