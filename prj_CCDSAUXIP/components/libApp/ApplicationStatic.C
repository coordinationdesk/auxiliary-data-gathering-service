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

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2015/10/22 16:11:59  marpas
	use of macro ACS_XXX enforced
	
	Revision 5.2  2015/06/12 08:22:00  tergem
	APF-267 Separate Log implementation for filterable events:
	_mclogRotationPolicy LogRotationPolicy defined and instantiated in the setLRPolicy function
	appendToLast set and get method defined in LogRotationPolicy class
	defineMCLogName virtual protected method added to allow its reimplementation
	forceRotation method added to implement the rotation on the mc log every time the rotation is Needed
	defaultMCLogPath defined
	
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.9  2013/04/17 20:38:54  marpas
	instance() method has now explicit exception listed
	
	Revision 4.8  2013/03/14 15:25:14  marpas
	seDefaultConnectionPool and getDefaultConnectionPool method substitutes setConnectionPool and getConnectionPool
	interface rationalization
	
	Revision 4.7  2013/03/12 17:43:33  marpas
	registerWaitingCP method and related ones implemented to manage connection pool.
	When the connection pool is set, any registered will be called with a reference of the connection pool
	
	Revision 4.6  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.5  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.4  2012/03/13 16:32:20  marpas
	help fixed
	
	Revision 4.3  2012/03/01 15:48:09  marpas
	using acs::base for default satic paths
	
	Revision 4.2  2012/02/14 14:37:10  marpas
	refactoring
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:59  marpas
	new statistics adopted
	
	Revision 3.4  2011/09/26 11:19:16  marpas
	SSF development in progress
	
	Revision 3.3  2011/09/22 18:26:56  marpas
	status is inherited from last status when restarting
	
	Revision 3.2  2011/09/20 17:34:52  marpas
	Software Status Notification (S1PDGS-1902) in progress
	
	Revision 3.1  2011/09/12 17:30:26  marpas
	setStatus development in progress (i/f creation)
	default for stack trace notification changed
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.5  2009/04/03 15:39:25  marpas
	some rules fixed
	
	Revision 2.4  2008/11/13 12:39:38  marpas
	new ConfigurationSingleton approach for configuration. Convenience functions added.
	
	Revision 2.3  2006/12/21 18:15:03  serfol
	Compiling bux fixed.
	
	Revision 2.2  2006/12/19 19:48:17  paoscu
	executableName modified to use /proc filesystem on Linux to get the real path
	
	Revision 2.1  2006/03/16 18:05:24  paoscu
	Added methods getDistributionVersion and getDistributionRelease to obtain
	the version and release under wich the application has been distributed.
	Current implementation performs the task
	calling an external application called ACS_Wh3r3RUFr0m
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/11/04 17:13:24  enrcar
	hostname uses HWScan homologous method
	
	Revision 1.6  2005/10/06 19:41:32  marpas
	ApplicationDebug concept introduced, installs a SIGUSR2 handler in order to change debug values at runtime
	
	Revision 1.5  2004/09/28 11:57:47  paoscu
	decoupling between static Applications member and instance ones
	
	Revision 1.4  2004/04/14 11:12:29  paoscu
	Log rotation policy uses filters now and preserve them
	Thread safety implemented with ThreadSafe
	
	Revision 1.3  2003/10/01 16:25:51  marpas
	Application::isRestarted method added to know if the application was automatically restarted
	
	Revision 1.2  2003/07/22 20:57:05  paoscu
	Application::appName added
	Application::instance() moved here.
	
	Revision 1.1  2003/07/14 19:22:23  paoscu
	static members for Application class separate in a different compilation
	unit in order to avoid useless dependencies
	

*/

#include <Application.h>

#include <HWScan.h>
#include <Filterables.h>
#include <exStackTrace.h>
#include <File.h>

#include <unistd.h>
#include <fstream>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Application) 
Application* Application::_instance=0; 
char ** Application::argv_=0;
const string Application::autoRestart("-autoRestart");
const string Application::defaultLogPath(base::defaultLogPath());
const string Application::defaultMCLogPath(base::defaultMCLogPath());
ThreadSafe  Application::_hNameMutex ;
std::string Application::_exeName;
const string Application::_distributionVersionApplication("ACS_Wh3r3RUFr0m");
string Application::_distributionVersion ;
string Application::_releaseVersion ;

Application::Status Application::restartStatus_ = Application::Initializing ;

Application::Builder* Application::Builder::_instance=0; 

string Application::hostname() 
{
	return HWScan::hostname() ;
}


void Application::getDistributionVersionAndRelease(std::string const &applicationExecutable)
{
	if(_distributionVersion.empty() || _releaseVersion.empty() )
	{
		// execute ACS_Wh3r3RUFr0m script on the givenexecutable
		string command = _distributionVersionApplication +  " " + applicationExecutable ;
		FILE * _popenp = popen(command.c_str(),"r") ;       // PRQA S 4412
		if (!_popenp) {
			ACS_THROW(Exception("Cannot run command \"" + command +"\"",errno)) ; // PRQA S 3081
		}
		// read the output
		File::ifdstream streamReader(_popenp) ;
		streamReader >> _distributionVersion ; // version
		if(streamReader.fail() )
		{
			_distributionVersion.clear() ;
			_releaseVersion.clear() ;
			ACS_THROW(Exception("Cannot obtain application version and release using command \"" + command +"\"",errno)) ; // PRQA S 3081
		}	
		streamReader >> _releaseVersion ; // release
		if(streamReader.fail() )
		{
			_distributionVersion.clear() ;
			_releaseVersion.clear() ;
			ACS_THROW(Exception("Cannot obtain application release using command \"" + command +"\"",errno)) ; // PRQA S 3081
		}	
	}
}


std::string const & Application::getDistributionVersion(std::string const &applicationExecutable) 
{
	ThreadSafe::Lock lock(_hNameMutex) ;
	getDistributionVersionAndRelease(applicationExecutable) ;
	return _distributionVersion ;
}

std::string const & Application::getDistributionRelease(std::string const &applicationExecutable) 
{
	ThreadSafe::Lock lock(_hNameMutex) ;
	getDistributionVersionAndRelease(applicationExecutable) ;
	return _releaseVersion ;
}

std::string const & Application::getDistributionVersion() 
{
	return getDistributionVersion(executableName()) ;
}

std::string const & Application::getDistributionRelease() 
{
	return getDistributionRelease(executableName()) ;
}



Application * Application::instance() //  throw(Application::Exception)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	// no istance still created --> exception
	ACS_COND_THROW(_instance==0,Exception("No Application has been built.")); // PRQA S 3081

	return _instance; 
}


string Application::parseRevision(const string &revision) // PRQA S 4020
{
	if (revision.empty()) {
		return "" ;
	}
    	
	istringstream is (revision) ;
	char revis[1024] ;                                  // PRQA S 4403
	char colon ;                                        // PRQA S 4101
	is.get(revis, sizeof(revis), ':') ; // revision is after the colon - so get it
	is >> colon ; // extract it
	string v ;
	is >> v ; // extract the revision
	// strip the final '$' if any
	return (v[v.length()-1] == '$') ? string(v,0,v.length()-1) : v; // PRQA S 3084, 3382
}

string Application::parseDate(const string &revision) // PRQA S 4020
{
	if (revision.empty()) {
		return "" ;
	}
    	
	istringstream is (revision) ;
	char revis[1024] ;  // PRQA S 4403
	char colon ;                                        // PRQA S 4101
	is.get(revis, sizeof(revis), ':') ; // date is after the colon - so get it
	is >> colon ; // extract it
	string v ;
	is >> v ; // extract the date
	// strip the final '$' if any
	return (v[v.length()-1] == '$') ? string(v,0,v.length()-1) : v; // PRQA S 3084, 3382
}


Application::Status Application::setStatus(Status s, const string &reason) 
{
    if(not _instance) // no app till now ?
	{
        try {
		    ACS_THROW(Exception("No app till now")) ;// become SEP (Someone Else Problem) // PRQA S 3081
        }
        catch(exception &x) {
		    ACS_LOG_NOTIFY_EX(x);
            throw ; // rethrow it 
        }
	}
	Lock l(*_instance) ;
    
    return _instance->setCurrentStatus(s, reason) ;
}



bool Application::exists()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "instance " << _instance) ;
	// the application exists if there is a not null instance
	return (_instance != 0); 
}



const string Application::executableName()      // PRQA S 4020
{
	ThreadSafe::Lock lock(_hNameMutex) ;

#ifdef __linux__
	if(!_exeName.empty())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_exeName already set to  \"" << _exeName << "\"") ;
		return _exeName ;
	}
	
		
	try{
		pid_t pid = getpid() ; // which is the current process id
		ostringstream appLinkInProc ;
		// create the path for the current process in /proc
		appLinkInProc << "/proc/" << pid <<"/exe";
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "link in proc is  \"" << appLinkInProc.str() << "\"") ;
		// the link points to the executable
		_exeName = File::readlink(appLinkInProc.str()) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_exeName is  \"" << _exeName << "\"") ;
		// return it (note also store it for further references as above in the function)
		return _exeName ;
	}
	catch(std::exception &e)
	{
		ACS_THROW( Application::Exception(e , "Error trying to obtain real executable path.") ) ; 
	}

#else
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "not a linux system. Return _argv[0] : \"" << _argv[0]) ;
	return _argv[0] ;
#endif
}



_ACS_END_NAMESPACE

