// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Main for every Applications $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/09/12 13:16:26  marpas
	debug messages improved
	
	Revision 5.2  2014/05/27 13:55:54  marfav
	Added dump of last managed exception on a file in /tmp
	
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.3  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.2  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.1  2012/02/14 14:37:10  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:59  marpas
	new statistics adopted
	
	Revision 3.2  2011/03/01 12:01:23  marpas
	GCC 4.4.x support
	
	Revision 3.1  2010/11/24 20:04:37  marpas
	debug added
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.3  2009/04/03 15:39:25  marpas
	some rules fixed
	
	Revision 2.2  2007/07/16 09:31:44  marpas
	exception handling improved
	
	Revision 2.1  2006/05/17 10:37:14  giucas
	delete Application::instance() moved out of try/catch
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2004/12/14 15:10:49  paoscu
	headers fixed
	
	Revision 1.6  2004/03/29 17:27:30  paoscu
	putenv LANG=C added as first statement of the application
	
	Revision 1.5  2003/07/11 12:58:26  paoscu
	restart implementation policy
	
	Revision 1.4  2003/06/09 17:06:36  paoscu
	exception handling with message in main
	
	Revision 1.3  2003/04/30 16:10:45  paoscu
	using namespace std no longer into acs include
	
	Revision 1.2  2003/04/09 13:04:20  paoscu
	Application::Builder and Application::GenericBuilder classes added.
	
	Revision 1.1.1.1  2002/11/27 10:23:36  paoscu
	Import libApp
	
	

*/

#include "Application.h"
#include <Filterables.h>
#include <unistd.h>

namespace {
    class Main {
    public:
	    ACS_CLASS_DECLARE_DEBUG_LEVEL(Main) ;
    } ;


    ACS_CLASS_DEFINE_DEBUG_LEVEL(Main) ;

}

extern "C" {

using namespace acs;
using namespace std;

int main(int argc, char const * const * argv, char const * const * env)
{	
	ACS_FUNC_ANNOUNCE_DEBUG_TH(Main,ACS_VLO_VERB) ;
	
	// in order to avoid LANG settings to corrupt formatted I/O
	char lang_[] ="LANG=C" ;
	putenv(lang_) ;
	int res=1;
	bool deleteApplication=false;
	bool mustRestart=false;
	try
	{
		ACS_FUNC_WRITE_DEBUG_TH(Main,ACS_VLO_VERB, "checking application existence") ;
		if(!Application::exists()) // check if the Application class has been created
		{
			ACS_FUNC_WRITE_DEBUG_TH(Main,ACS_VLO_VERB, "building application using Application::Builder") ;
			Application::Builder::instance()->build(); // create it 
			deleteApplication=true; // memento: remove the application when needed
			
		}
		ACS_FUNC_WRITE_DEBUG_TH(Main,ACS_VLO_VERB, "run application") ;
		res=Application::run(argc, argv, env) ; // run the application
		mustRestart=Application::instance()->mustRestart(); // check if the app should restart
	}
	catch(exception &e)
	{
		ostringstream fname;
		fname << "/tmp/" << Application::instance()->appName() << "_" << getpid() << "_LastException.log"; 
		exLogStream tempLog ("lastScream", fname.str(), &acs::exostream::getDefaultOutLogger());
		ACS_LOG_NOTIFY_EX(e) ;
		ACS_LOG_ERROR("The last exception was handled in main.") ;
		res=1;
	}

	catch(...)
	{
		ostringstream fname;
		fname << "/tmp/" << Application::instance()->appName() << "_" << getpid() << "_LastException.log"; 
		exLogStream tempLog ("lastScream", fname.str(), &acs::exostream::getDefaultOutLogger());
		ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException("Unknown exception caught")) ; // PRQA S 3081
		ACS_LOG_ERROR("The last exception was handled in main.") ;
		res=1; 
	}
		
    ACS_FUNC_WRITE_DEBUG_TH(Main,ACS_VLO_VERB, "mustRestart is " << boolalpha << mustRestart) ;
    ACS_FUNC_WRITE_DEBUG_TH(Main,ACS_VLO_VERB, "deleteApplication is " << boolalpha << deleteApplication) ;
	
	if(deleteApplication) { // has to be deleted ?
		delete Application::instance();
    }
		
	if(mustRestart) { // has to be restarted (phoenix)
		ACS_LOG_INFO ("Restarting the application now...");
		Application::restart();	// would not return 
    }
	
	return res;
}

}//extern "C"
