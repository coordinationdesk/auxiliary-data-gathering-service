// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Standalone Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2016/03/09 10:12:48  marpas
	coding best practices application
	
	Revision 5.0  2013/06/20 08:41:56  marpas
	adopting libException 5.x standards
	coding best practices applied
	compilation warnings
	
	Revision 1.9  2012/03/08 09:09:19  marpas
	useless statements removed
	
	Revision 1.8  2012/02/14 14:46:52  marpas
	refactoring
	
	Revision 1.7  2009/11/05 08:55:38  giucas
	Removed include of dbStat.h
	
	Revision 1.6  2009/02/17 16:29:27  clanas
	adapted to the new interface
	
	Revision 1.5  2008/06/04 15:50:30  enrcar
	EC:: fileOperation01Factory intantiated
	
	Revision 1.4  2006/09/28 08:53:37  clanas
	useless factories removed
	
	Revision 1.3  2006/04/21 13:40:01  clanas
	removed Mcf handler
	
	Revision 1.2  2006/04/20 14:49:05  clanas
	removed if def of Patrol and SMPluginLoader
	
	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
	imported source code
	
	
*/
	

#include <StandaloneApp.h>
#include <SystemModeFactory.h>
#include <lsaV.h>
#include <exAdaptiveFilter.h>
#include <StandaloneResourceSpaceFactory.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace { // unnamed
    lsaV Standalone;
} 

ACS_CLASS_DEFINE_DEBUG_LEVEL(StandaloneApp) 


StandaloneApp::StandaloneApp(string const & name, string const &subSystem, bool nomcf):
 Application(name, subSystem), 
 _adaptiveFilter(0),
 _allowMcf(!nomcf)
{
} 

StandaloneApp::~StandaloneApp() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;// PRQA S 4631
}



void StandaloneApp::postDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}



void StandaloneApp::preDaemonize() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}


bool StandaloneApp::allowMcf() const // PRQA S 4120
{
	return _allowMcf ;
}



int StandaloneApp::start(int argc, char const * const * argv, char const * const * env) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START...") ;
	
	// factories and handler
	StandaloneResourceSpaceFactory resourcesFactory;
	SystemModeFactory	systemModeFactory ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to call Application::start method.") ;
	try
	{	int res = -1 ;
		initPlugins(argc, argv) ;
		res=Application::start(argc, argv, env);
		justBeforeStartEnd() ;
		exostream::getDefaultOutLogger().flush() ; // to all the chain
		return res;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e) ;
		exostream::getDefaultOutLogger().flush() ;
		return 255; // PRQA S 4400
	}
	catch(...)
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException("Unknown exception caught")) ;  // PRQA S 3081
		exostream::getDefaultOutLogger().flush() ;
		return 255;  // PRQA S 4400
	}
}



void StandaloneApp::initPlugins(int argc, char const * const * argv) 
{
}

void StandaloneApp::justBeforeStartEnd() 
{
}

exAdaptiveFilter *StandaloneApp::getAdaptiveFilter() // PRQA S 4120, 4211
{
	return _adaptiveFilter ;
}

	
void StandaloneApp::beforeRestart()
{
	/* kill Patrol */
	preDaemonize();
}

_ACS_END_NAMESPACE
