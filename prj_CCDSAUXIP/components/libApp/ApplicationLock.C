// PRQA S 1050 EOF
/*

	Copyright 1995-2019, Advanced Computer Systems , Inc.
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
	Revision 5.4  2017/09/12 13:15:16  marpas
	added actualInstance method to return the actual instance number that provided a succesfull lock.
	
	Revision 5.3  2016/11/14 09:41:36  marfav
	Using default pid permissions settings
	Depends from libException Rev_5_88
	
	Revision 5.2  2015/10/22 16:11:59  marpas
	use of macro ACS_XXX enforced
	
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.4  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.3  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.2  2012/02/14 14:37:10  marpas
	refactoring
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:58  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.1  2009/04/03 15:39:24  marpas
	some rules fixed
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2004/04/06 10:21:21  paoscu
	exStream.h included
	
	Revision 1.5  2003/09/15 18:33:47  paoscu
	Prevent to remove pid file if lock fails.
	
	Revision 1.4  2003/09/12 13:57:36  paoscu
	More debug.
	
	Revision 1.3  2003/07/22 20:58:39  paoscu
	Enhanced management of locks.
	
	Revision 1.2  2003/07/11 12:58:26  paoscu
	restart implementation policy
	
	Revision 1.1  2003/06/26 10:13:31  paoscu
	lock implemented
	
	
*/

#include <ApplicationLock.h>
#include <exStream.h>
#include <Filterables.h>

#include <sstream>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ApplicationLock)

//
// load the configuration file
//

ApplicationLock::ApplicationLock(unsigned short maxInstances, const string& lockName) ACS_NOEXCEPT :
	_lockName(File::getFileName(lockName)),
    _actualLockName(),
    actualInstance_(),
	_maxInstances(maxInstances),
	_lock("") 
{
	//In this case it is safe to catch anything because
	//ApplicationLock is allocated from main thread
	try {
		Application *theApp = Application::instance() ;
		if (theApp) { theApp->setLock(this) ; }
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
	catch(...) {
		;
	}
}


ApplicationLock::~ApplicationLock() ACS_NOEXCEPT
{
// PRQA S 4631 L1
	//In this case it is safe to catch anything because
	//ApplicationLock is allocated from main thread
	try {
		Application *theApp = Application::instance() ;
		if (theApp && (theApp->getLock() == this) ) {
			theApp->setLock(0) ;
		}
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
	
	if (!_actualLockName.empty()) {
		try {
			File::unlink(_actualLockName) ;
		}
		catch(exception &x) {
	        ACS_LOG_NOTIFY_EX(x) ;
        }
	}
// PRQA L:L1
}

const string &ApplicationLock::lock(bool exception) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	ACS_COND_THROW(_maxInstances < 1,exIllegalValueException("ApplicationLock cannot have an instance number less than 1")) ;    // PRQA S 3000, 3081
	
	ACS_COND_THROW(_lockName.empty(),exIllegalValueException("ApplicationLock cannot have an empty string as file")) ;           // PRQA S 3081
	
	// check if all the allowed instances are running
	for (actualInstance_=0; actualInstance_ < _maxInstances; actualInstance_++) {
		// each istance has a lock file named /tmp/<lockName>#Pid
		ostringstream os ;
		os << acs::base::defaultLockPath() << "/" << _lockName ;
		os << actualInstance_ << "Pid" ;
		_actualLockName = os.str() ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying to lock the file " << _actualLockName) ;
		_lock.setLock(_actualLockName) ;
		// try to lock the i-th instance
		if (_lock.lock(50)) { // 50 [msec] to lock the file else next       // PRQA S 4400
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "the lock was succesful") ;
			// if here the file has been locked, means that no process own a 
			// lock on it
			writePid() ; // write actual pid into the file
			try {
				File::chmod(_actualLockName,base::defaultPidPermissions() ) ;                        // PRQA S 4400
			} 
			catch (std::exception &e) {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "exception caught chmod...ding: " << e) ;
 			}
			return _actualLockName ; // return it 
			// NOTE that _lock still mantain the lock - will be discarded at its dtor
		}
	}
	// if here - no lock was possible on every allowed instance 
	if (exception) { // an exception should be raised ? (as argument)
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			string command="/sbin/pidof " + Application::instance()->executableName(); // PRQA S 3081
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Lock failed. I'm going to execute command: "<<command ) ;
			system(command.c_str() );
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "command \""<<command<<"\" executed") ;
		ACS_CLASS_END_DEBUG
		_actualLockName = "" ;
		// throw the exception
		ACS_THROW(File::Lock::LockException("Too many instances for " + _lockName)) ; // PRQA S 3081
	}
	_actualLockName = "" ;
	return _actualLockName ;
}

void ApplicationLock::writePid()    // PRQA S 4211
{
	ofstream pid(_actualLockName.c_str()) ;
	// Pid word removed from PID file (it was "Pid: xxxxx")
	pid << getpid() << "\n" ;
}

void ApplicationLock::unlock()
{
	_lock.unlock() ;
	_actualLockName="" ;
}

_ACS_END_NAMESPACE

