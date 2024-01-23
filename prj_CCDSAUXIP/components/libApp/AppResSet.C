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
	Revision 5.2  2015/10/22 16:09:50  marpas
	use of ACS_XXX macros encouraged
	
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.6  2013/02/22 12:51:25  marpas
	resource set interface changed
	
	Revision 4.5  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.4  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.3  2012/03/01 15:48:09  marpas
	using acs::base for default satic paths
	
	Revision 4.2  2012/02/14 14:37:10  marpas
	refactoring
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:58  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.3  2009/04/03 15:39:24  marpas
	some rules fixed
	
	Revision 2.2  2007/06/19 10:37:07  marpas
	trying to get dcw conf from user's home
	
	Revision 2.1  2006/06/01 13:18:57  marpas
	debug improved
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/06/08 14:37:19  marpas
	exceptions catching fixed
	
	Revision 1.8  2004/12/14 15:10:49  paoscu
	headers fixed
	
	Revision 1.7  2004/03/08 22:59:03  paoscu
	singleton relese at exit conditioned by the compilation flag RELEASE_SINGLETON_AT_EXIT
	
	Revision 1.6  2003/08/05 11:16:11  paoscu
	save method now takes a rsResourceSet as argument.
	
	Revision 1.5  2003/06/26 10:13:31  paoscu
	lock implemented
	
	Revision 1.4  2003/04/30 16:10:45  paoscu
	using namespace std no longer into acs include
	
	Revision 1.3  2003/02/18 18:41:11  marpas
	i/f changed
	
	Revision 1.2  2003/01/31 13:56:22  paoscu
	application name from Application class. No longer in AppResSet class.
	
	Revision 1.1.1.1  2002/11/27 10:23:36  paoscu
	Import libApp
	

*/

#include <AppResSet.h>
#include <Application.h>
#include <File.h>
#include <Filterables.h>
#include <acs_c++config.hpp>
#include <sys/types.h>  // PRQA S 1013
#include <sys/stat.h>   // PRQA S 1013
#include <unistd.h>

#include <laV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    laV version ;
}

string AppResSet::_homeDir ; 

ACS_CLASS_DEFINE_DEBUG_LEVEL(AppResSet)

AppResSet::AppResSet(bool loadConf, bool mergeDcw, bool userEnv )  :
    _rset(),
    _optSet(),
	_mergeDcw(mergeDcw),
	_loadConf(loadConf) 
{
	
	try {
		if (userEnv) { initUserEnv() ; }
	} catch (exFileOpenException &e) {
		ACS_LOG_NOTIFY_EX(e) ; 
	}
	
	loadOptSet() ;	
	revertToSaved() ;
}

void AppResSet::loadOptSet()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << optConfFileName()) ;
	ifstream file(optConfFileName().c_str()) ;
	if (file.rdstate()) {   // PRQA S 3054
		// ACS_THROW(exFileOpenException("Impossible to open the file named " + confFileName() ,errno)) ; // PRQA S 3081
	}
	else {
		file >> _optSet ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "done") ;
}


void AppResSet::revertToSaved()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << confFileName()) ;
	ifstream file(confFileName().c_str()) ;
	if (file.rdstate()) {   // PRQA S 3054
		// ACS_THROW(exFileOpenException("Impossible to open the file named " + confFileName() ,errno)) ; // PRQA S 3081
	}
	else {
		file >> _rset ;
	}

	if (_loadConf) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << defaultConfFileName()) ;
		// merge the default conf for the application
		ifstream defaultFile(defaultConfFileName().c_str()) ;
		if (defaultFile.bad()) {
			ACS_THROW(exFileOpenException("Impossible to open the file named " + defaultConfFileName() ,errno)) // PRQA S 3081
		}
		rsResourceSet defaultconf ;
		defaultFile >> defaultconf ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "merging conf: " << defaultConfFileName()) ;
		// merge them
		_rset.mergeMode(rsResourceSet::ModeNoChange) ;
		_rset.merge(defaultconf) ;
	}
	
	
	if (_mergeDcw) {
		// now load the dcw.conf file	
		rsResourceSet dcwconf ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "loading conf: " << dcwConfFileName()) ;
		ifstream dcwConfFile(dcwConfFileName().c_str()) ;

		if (dcwConfFile.bad()) {
			ACS_THROW(exFileOpenException("Impossible to open the file named " + dcwConfFileName() ,errno)) ;  // PRQA S 3081
		}
		dcwConfFile >> dcwconf ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "merging conf: " << dcwConfFileName()) ;
		// merge them
		_rset.mergeMode(rsResourceSet::ModeNoChange) ;
		_rset.merge(dcwconf) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "done") ;
}




AppResSet::~AppResSet() ACS_NOEXCEPT {}


//
// save the configuration file
//

void AppResSet::save(rsResourceSet *inrs)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "saving conf: " << confFileName()) ;
	// the internal or the given one
	rsResourceSet &rs = inrs ? *inrs : _rset ;      // PRQA S 3384
	// open the output file
	ofstream file(confFileName().c_str()) ;
	if (file.rdstate()) {                           // PRQA S 3054
		ACS_THROW(exFileOpenException("Cannot open the file named " + confFileName() ,errno)) ; // PRQA S 3081
	}
	
	// fill it
	file << rs ;
}


rsResourceSet &AppResSet::resourceSet() // PRQA S 4120
{
	return _rset ; // PRQA S 4024
}

rsResourceSet &AppResSet::optResourceSet() // PRQA S 4120
{
	return _optSet ; // PRQA S 4024 
}

//
// DESCRIPTION: return (eventually set) the _homeDir member
// 				
// Returns: The home directory
//
const string &AppResSet::homeDir()
{
	if (_homeDir.empty()) {
		_homeDir = getenv("HOME") ;
		_homeDir += "/." + Application::instance()->applicationName() ;
	}
	return _homeDir ;
}

//
// DESCRIPTION: return the confFileName
// 				
// Returns: The log file name
//
string AppResSet::confFileName()
{
	return homeDir() + "/conf/" + Application::instance()->applicationName() + ".conf" ;
}

string AppResSet::defaultConfFileName()
{
	return string(base::defaultConfPath()) + "/" + Application::instance()->applicationName() + ".conf" ; // PRQA S 3081
}

string AppResSet::optConfFileName()
{
	return string(base::defaultConfPath()) + "/"  + Application::instance()->applicationName() + "Opt.conf" ; // PRQA S 3081
}

string AppResSet::dcwConfFileName() // PRQA S 4020
{
	string conffile = string(base::defaultConfPath()) + "/HyBroDcw.conf" ; // PRQA S 3081
	try {
		_rset.getValue("dcwConfigurationFile",conffile) ;
	}
	catch (rsResourceSet::NotFoundException &) {
		; // nothing 
	}
	
	{
		string dcwconf = confFileName() ;
		if (File::exists(dcwconf) ) {
			return dcwconf ;
        }
	} 
	return conffile ;
}


//
// DESCRIPTION: this function checks and if needed creates the .application user space
// Returns: error code
//
void AppResSet::initUserEnv()
{

	// 1) check for the $HOME/.applicationName directory
	string base(homeDir()) ;
	createDir(base) ;

	// 2) check for the $HOME/.applicationName/log directory	
	string path(base) ; 
	path += "/log" ;
	createDir(path) ;
	
	// 3) check for the $HOME/.applicationName/work directory
    path = base + "/work" ;
	createDir(path) ;

    // 4) check for the $HOME/.applicationName/conf directory
    path = base + "/conf" ;
	createDir(path) ;
}



void AppResSet::createDir(const string &path)
{
 	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking:\"" << path << "\"") ;
	
	bool exists = false ;
	try { // check if the path exists and is a directory
		exists = File::isDir(path) ;
	}
	catch(exFileOpenException &) {
		exists = false ;
	}
	
	if ( !exists )
	{
		// do not exists ... create it
		ACS_LOG_INFO("Creating " << path << " directory") ; 
		try {
			File::mkdir(path) ; // here 
		}
		catch(exception &x) {
			ACS_THROW(exIOException(x,"Cannot create the directory " + path ,errno)) ; // PRQA S 3081
		}
	}	
}



//
//
//	Singleton Implementation
//
//

AppResSet* AppResSetSingleton::_instance = 0 ; 
bool AppResSetSingleton::_mergeDcw = false ;
bool AppResSetSingleton::_loadConf = false ;
bool AppResSetSingleton::_userEnv = true ;

AppResSet* AppResSetSingleton::instance()
{
	if(0 == _instance)
	{
		_instance = new AppResSet(_loadConf, _mergeDcw, _userEnv) ;
#ifdef RELEASE_SINGLETON_AT_EXIT
		atexit(release) ;
#endif
	}
	
	return _instance ; 
}

AppResSetSingleton::AppResSetSingleton() ACS_NOEXCEPT {}

AppResSetSingleton::~AppResSetSingleton() ACS_NOEXCEPT {}

void AppResSetSingleton::save(rsResourceSet *rs)
{
	if(_instance) {	_instance->save(rs) ; }
}

void AppResSetSingleton::mergeDcw(bool v) 
{
	_mergeDcw = v ;
}


void AppResSetSingleton::loadConf(bool v) 
{
	_loadConf = v ;
}

void AppResSetSingleton::userEnv(bool v) 
{
	_userEnv = v ;
}



void AppResSetSingleton::release() 
{
	delete _instance ;
	_instance = 0 ;
}

_ACS_END_NAMESPACE

