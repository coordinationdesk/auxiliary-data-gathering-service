// PRQA S 1050 EOF
/*

	Copyright 2019-2029, Exprivia , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, Inc.

	$Prod: Exprivia FLEX L1BPP $

	$Id$

	$Author$

*/


#include <osfiIPFStandaloneApp.h>

using namespace std;


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(osfiIPFStandaloneApp) 


osfiIPFStandaloneApp::osfiIPFStandaloneApp(string const & name, string const &subSystem, bool nomcf):
 StandaloneApp (name, subSystem), // PRQA S 4052
 _theLogFmtFactory(),
 _theLogStream(),
 _thelogFilter(&_theLogStream),
 _defaultOutLogger(0)
{
	attachLogHandler() ;
} 

osfiIPFStandaloneApp::~osfiIPFStandaloneApp() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
    try {
        detachLogHandler() ;  // PRQA S 4631
    }
    catch(exception &x) {
        ; // do nothing 
    }
}


int osfiIPFStandaloneApp::start(int argc, char const * const * argv, char const * const * env)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	return StandaloneApp::start (argc, argv, env);
}

void osfiIPFStandaloneApp::attachLogHandler() 
{
    _defaultOutLogger = exostream::setDefaultOutLogger (&_theLogStream);
    
}

void osfiIPFStandaloneApp::detachLogHandler() // PRQA S 4211
{
    exostream::setDefaultOutLogger (_defaultOutLogger);
}

void osfiIPFStandaloneApp::setLogProcName(const std::string& s) throw() 
{
    _theLogFmtFactory.setProcName (s);
}

void osfiIPFStandaloneApp::setLogProcVersion(const std::string& s) throw() 
{
    _theLogFmtFactory.setProcVersion (s);
}

void osfiIPFStandaloneApp::setLogTaskName(const std::string& s) throw() 
{
    _theLogFmtFactory.setTaskName (s);
}

void osfiIPFStandaloneApp::setLogEndLine(const char& endLine) throw()
{
	_theLogFmtFactory.setEndLine( endLine );
}


void osfiIPFStandaloneApp::setLogLevel(const IPFLogConfiguration::LogLevel& level) throw()
{
    IPFLogConfiguration::instance()->setLogLevel (level);
}

void osfiIPFStandaloneApp::setEmitTime(bool b) throw() 
{
    _theLogFmtFactory.emitTime(b);
}

void osfiIPFStandaloneApp::setEmitPid(bool b) throw() 
{
    _theLogFmtFactory.emitPid(b);
}

_ACS_END_NAMESPACE
