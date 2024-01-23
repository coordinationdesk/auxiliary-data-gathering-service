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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.7  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.6  2013/01/23 17:23:10  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.5  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.4  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.3  2009/08/03 16:13:18  marpas
	speeding up system mode retrieval (less connections)
	
	Revision 2.2  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.1  2008/12/16 14:51:37  marpas
	dbSystemMode implemented
	
	
*/


#ifndef _dbSystemMode_H_
#define _dbSystemMode_H_ 

#include <acs_c++config.hpp>
#include <SystemMode.h>
#include <exException.h>
#include <cstring>
#include <memory.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class dbConnection ;
class dbConnectionPool ;

//
//
//
// class SystemMode
//
//
//
class dbSystemMode : public SystemMode // PRQA S 2109, 2153
{
public:

	explicit dbSystemMode(dbConnectionPool &cp, const std::string &app, const std::string &sub = "");
	dbSystemMode(const dbSystemMode &) ;
	dbSystemMode &operator=(const dbSystemMode &) ;
	virtual ~dbSystemMode() throw() ;

	virtual std::string load() ;
private:
	// deafult ctor - not implemented
	dbSystemMode() ;
	std::string dbSelectMode(const std::string &app, const std::string &sub) ;
private:
    dbConnectionPool &connectionPool_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSystemMode) ;
} ; 

_ACS_END_NAMESPACE



#endif // _SystemMode_H_
