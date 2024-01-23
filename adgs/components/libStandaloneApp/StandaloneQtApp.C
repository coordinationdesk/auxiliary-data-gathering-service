// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Standalone Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/03/10 14:21:46  nicvac
	Qt part: Porting to head.
	
	Revision 5.0  2013/06/20 08:41:56  marpas
	adopting libException 5.x standards
	coding best practices applied
	compilation warnings
	
	Revision 1.5  2012/04/11 16:11:34  marpas
	dtor fixed
	
	Revision 1.4  2012/02/14 14:46:52  marpas
	refactoring
	
	Revision 1.3  2007/10/15 09:04:47  marpas
	 no exception shou rise from a dtor
	
	Revision 1.2  2007/07/16 09:43:20  marpas
	improved exception handling
	
	Revision 1.1.1.1  2006/04/20 12:15:46  clanas
	imported source code
	
	
	

*/

#include <StandaloneQtApp.h>
#include <ApplicationQApp.h>
#include <ExceptionPanel.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(StandaloneQtApp) 


StandaloneQtApp::StandaloneQtApp(string const & name, string const & subSystem, bool nopatrol)
: StandaloneApp(name,subSystem, nopatrol) , _qtApp(0), _exceptionPanel(0) 
{
} 

StandaloneQtApp::~StandaloneQtApp() throw() 
{
	delete _exceptionPanel ; // PRQA S 4631
}


QApplication * StandaloneQtApp::qtApp()
{
	if(_qtApp==0)
	{
		ACS_THROW(Exception("_qtApp is not created")) ; // PRQA S 3081
	}
	return _qtApp;
}


int StandaloneQtApp::startMainLoop()
{
    int res=0;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting main loop") ;


    for(bool testExit = true; testExit ;)
	{
		try
		{
            res= _qtApp->exec();
			testExit = false ; 
		}
		catch (exception &e)
		{
			ACS_LOG_NOTIFY_EX(e) ;
        }
		catch(...) 
		{
			try {
 				ACS_THROW(Application::Exception("Unknown exception caught")) ;
			}
			catch(exception &x) {
				ACS_LOG_NOTIFY_EX(x) ;
			}
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting with value: " << res) ;
	return res;

}



int StandaloneQtApp::start(int argc, char const * const * argv, char const * const * env)
{
	preQtApp() ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "START...") ;

	_qtApp=new ApplicationQApp(*this, argc, const_cast<char **>(argv) );
	QString appName(applicationName()) ;
	_exceptionPanel = new ExceptionPanel( appName + " Exception" ) ;

	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling StandaloneApp::start method.") ;
	int res=0;
	res=StandaloneApp::start(argc, argv, env);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "exiting with value: " << res) ;

	return res;
}

void StandaloneQtApp::justBeforeStartEnd() 
{
	try {
		delete _exceptionPanel ;
		_exceptionPanel = 0 ;
	}
	catch (exception &) {
		// do nothing - no exception out from a dtor !
		_exceptionPanel = 0 ;
	}
}


void StandaloneQtApp::preQtApp() 
{
}

ApplicationDebug::Mode StandaloneQtApp::debugMode(int argc, char const * const * argv, char const * const * env, std::string &debugPath) const 
{
	return ApplicationDebug::OnRequest ;	
}



_ACS_END_NAMESPACE
