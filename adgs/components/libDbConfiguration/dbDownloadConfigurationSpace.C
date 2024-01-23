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

	$Prod: A.C.S. dbConfiguration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/04/11 13:27:13  marpas
	adopting new db interfaces
	
	Revision 5.1  2016/03/26 21:42:59  marpas
	using new db I/F
	
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.6  2013/04/03 13:43:48  marpas
	run does not longer gets a void*
	
	Revision 2.5  2013/04/03 12:43:30  marpas
	Thread::run gets a void*
	
	Revision 2.4  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.3  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.2  2009/06/26 10:39:49  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.0  2006/02/28 09:09:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/21 12:09:01  marpas
	new class from libCryosatFileAction
	
	Revision 1.1.1.1  2005/06/15 07:49:35  integrator
	Imported
	
		

*/


#include <dbDownloadConfigurationSpace.h>
#include <dbConnectionPool.h>
#include <dbConfigurationSpace.h>
#include <File.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDownloadConfigurationSpace)

// constructor
dbDownloadConfigurationSpace::dbDownloadConfigurationSpace(dbConnectionPool &pool, dbConfigurationSpace &s, const string &path) :
	dbDownloadFile(pool, s,path),
	_space(s)   // PRQA S 2528
{
}



dbDownloadConfigurationSpace::~dbDownloadConfigurationSpace() throw()
{
}

void dbDownloadConfigurationSpace::start() 
{
	// check if exists
	string tmpname ;
	if (File::exists(_path)) {
		// rename it to .old and store it in tmpname
		tmpname = _path + ".old" ;
		File::rename(_path, tmpname);
	}
	
	try {
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
		// write the file 
		_space.saveSpaceFile(_path, "", theConnection) ; 
		setCompleted(true) ;
		// remove the old (if any)
		if (not tmpname.empty()) {
			File::unlink(tmpname) ;
        }
	}
	catch(...) {
		// error ... un-rename it 
		if (tmpname != "") {
			File::rename(tmpname, _path);
		}
		throw ;
	}
}


bool dbDownloadConfigurationSpace::downloading() 
{
	dbPersistent::InvalidUse ex("invalid call") ;
	ACS_THROW (ex) ;
}

void dbDownloadConfigurationSpace::run()
{
}


_ACS_END_NAMESPACE
