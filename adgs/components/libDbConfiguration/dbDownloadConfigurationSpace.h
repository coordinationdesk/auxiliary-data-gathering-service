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

	$Prod: A.C.S. db Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2013/04/03 13:43:48  marpas
	run does not longer gets a void*
	
	Revision 2.6  2013/04/03 12:43:30  marpas
	Thread::run gets a void*
	
	Revision 2.5  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.4  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.3  2012/12/11 17:30:29  marpas
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


#ifndef _dbDownloadConfigurationSpace_H_
#define _dbDownloadConfigurationSpace_H_ 

#include <acs_c++config.hpp>
#include <dbDownloadFile.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbConfigurationSpace ;

//
//
//
// class dbDownloadConfigurationSpace
//
//
//
class dbDownloadConfigurationSpace : public dbDownloadFile // PRQA S 2109
{
public:
	
	// this is the factory that will create this
	friend class dbConfigurationFileActionFactory ; // PRQA S 2107
	
	// virtual dtor
	virtual ~dbDownloadConfigurationSpace() throw() ;

	// inherited from dbDownloadFile
	virtual void start() ;
	virtual bool downloading() ;
	
private:
	// default ctor - not implemented
	dbDownloadConfigurationSpace() ;
	explicit dbDownloadConfigurationSpace(dbConnectionPool &, dbConfigurationSpace &, const std::string &path = "");
	
	// declared but not implemented to prevent their use
	dbDownloadConfigurationSpace(const dbDownloadConfigurationSpace &) ;
	dbDownloadConfigurationSpace &operator=(const dbDownloadConfigurationSpace &) ;
	
	virtual void run() ;		
private:
	dbConfigurationSpace &_space ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbDownloadConfigurationSpace) ;
} ; 

_ACS_END_NAMESPACE


#endif // _dbDownloadConfigurationSpace_H_
