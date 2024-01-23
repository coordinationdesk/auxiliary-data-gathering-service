// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2017/10/06 11:09:42  marpas
	fixing qa warnings
	
	Revision 5.4  2016/04/29 15:59:41  marpas
	getting rid of deprecated methods
	
	Revision 5.3  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.2  2016/03/26 21:45:49  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.1  2013/07/22 17:46:09  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.6  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.5  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.1  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/08/30 17:47:45  marpas
	dbGeneralQueryIF::doQuery method has a new parameter to enable/disable the query.
	The default behaviour should be:
	1) if the incremental is not set, the query should clear the previous result if any
	2) if the enable is false, the query should not be performed and the size of the result should be returned
	
	Because many of the class inheriting this interface do not implement the incremental feature, for the moment a clear of the result and returning 0 is then accepted
	
	Revision 1.7  2004/04/28 13:09:22  paoscu
	Incremental query support.
	
	Revision 1.6  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.5  2004/02/04 10:11:27  paoscu
	dbPersistent interface chanched.
		

*/

// This file is automatically generated by dbGenerator tool

#ifndef _dbApplicationStatusQuery_H_
#define _dbApplicationStatusQuery_H_

#include <acs_c++config.hpp>
#include <dbGeneralQuery.h>
#include <dbDataFactory.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbApplicationStatus ;

//
//
//
// class dbApplicationStatusQuery
//
//
//
class dbApplicationStatusQuery : // PRQA S 2109
	public dbGeneralQuery
{
public:
	friend dbGeneralQuery *dbDataFactory::buildDbApplicationStatusQuery(const std::string &) ; // PRQA S 2107

	static  std::string staticClassName() ;
	virtual ~dbApplicationStatusQuery() throw() ;

	virtual dbGeneralQueryIF* clone() const ; // PRQA S 2502

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual dbApplicationStatus *operator[](unsigned long i) ; 
	virtual const dbApplicationStatus *operator[](unsigned long i) const ; 

protected:
	explicit dbApplicationStatusQuery(const std::string &dbKey);

private:
	dbApplicationStatusQuery() ; // not implemented
	dbApplicationStatusQuery(const dbApplicationStatusQuery &) ;
	dbApplicationStatusQuery &operator=(const dbApplicationStatusQuery &) ;  // operator =
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbApplicationStatusQuery) ;
} ;


_ACS_END_NAMESPACE

#endif // _dbApplicationStatusQuery_H_
