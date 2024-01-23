// PRQA S 1050 EOF
/*

	Copyright 2014-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2015/07/15 06:51:08  ricfer
	setEmitTime and setEmitPid added.
	
	Revision 5.2  2014/07/15 11:44:40  nicvac
	Endline char handling.
	
	Revision 5.1  2014/05/20 12:16:13  marfav
	Addedd support for IPF Standalone app
	

*/


#include <IPFStandaloneApp.h>

using namespace std;


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(IPFStandaloneApp) 


IPFStandaloneApp::IPFStandaloneApp(string const & name, string const &subSystem, bool nomcf):
 StandaloneApp (name, subSystem), // PRQA S 4052
 _theLogFmtFactory(),
 _theLogStream(),
 _thelogFilter(&_theLogStream),
 _defaultOutLogger(0)
{
	attachLogHandler() ;
} 

IPFStandaloneApp::~IPFStandaloneApp() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
    try {
        detachLogHandler() ;  // PRQA S 4631
    }
    catch(exception &x) {
        ; // do nothing 
    }
}


int IPFStandaloneApp::start(int argc, char const * const * argv, char const * const * env)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	return StandaloneApp::start (argc, argv, env);
}

void IPFStandaloneApp::attachLogHandler() 
{
    _defaultOutLogger = exostream::setDefaultOutLogger (&_theLogStream);
    
}

void IPFStandaloneApp::detachLogHandler() // PRQA S 4211
{
    exostream::setDefaultOutLogger (_defaultOutLogger);
}

void IPFStandaloneApp::setLogProcName(const std::string& s) throw() 
{
    _theLogFmtFactory.setProcName (s);
}

void IPFStandaloneApp::setLogProcVersion(const std::string& s) throw() 
{
    _theLogFmtFactory.setProcVersion (s);
}

void IPFStandaloneApp::setLogTaskName(const std::string& s) throw() 
{
    _theLogFmtFactory.setTaskName (s);
}

void IPFStandaloneApp::setLogEndLine(const char& endLine) throw()
{
	_theLogFmtFactory.setEndLine( endLine );
}


void IPFStandaloneApp::setLogLevel(const IPFLogConfiguration::LogLevel& level) throw()
{
    IPFLogConfiguration::instance()->setLogLevel (level);
}

void IPFStandaloneApp::setEmitTime(bool b) throw() 
{
    _theLogFmtFactory.emitTime(b);
}

void IPFStandaloneApp::setEmitPid(bool b) throw() 
{
    _theLogFmtFactory.emitPid(b);
}

_ACS_END_NAMESPACE
