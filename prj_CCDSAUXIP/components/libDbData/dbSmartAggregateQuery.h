// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.2  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.10  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.9  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.8  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.7  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.4  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.3  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2008/04/28 21:49:26  paoscu
	Added dbSmartAggregateQuery
	
	
	
*/



#ifndef _dbSmartAggregateQuery_H_
#define _dbSmartAggregateQuery_H_

#include <acs_c++config.hpp>
#include <dbGeneralQuery.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbSmartAggregatePersistent ;

//
//
//
// class dbSmartAggregateQuery
//
//
//
class dbSmartAggregateQuery : // PRQA S 2109
	public dbGeneralQuery
{
public:	
	dbSmartAggregateQuery() = delete ; 
	dbSmartAggregateQuery(std::string const & parentTable , std::vector<std::string> const & childTables, const std::string &dbKey);
	dbSmartAggregateQuery(dbSmartAggregateQuery const &) ;
	dbSmartAggregateQuery &operator=(dbSmartAggregateQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ; // PRQA S 2502
	
	static  std::string staticClassName() ;
	virtual ~dbSmartAggregateQuery() noexcept ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

	virtual dbSmartAggregatePersistent *operator[](unsigned long i) ; 
	virtual const dbSmartAggregatePersistent *operator[](unsigned long i) const ; 

private:


private:
	std::string _parentTable ;
	std::vector<std::string> _childTables ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSmartAggregateQuery) ;
} ;

_ACS_END_NAMESPACE

#endif // _dbSmartAggregateQuery_H_

