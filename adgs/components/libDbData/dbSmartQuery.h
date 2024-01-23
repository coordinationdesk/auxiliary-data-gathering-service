// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2017/10/17 10:40:38  marpas
	adding
	db::SmartQuery::fill(vector<P> &) const
	method
	
	Revision 5.5  2017/10/16 15:20:05  marpas
	SmartQuery template is in db namespace
	
	Revision 5.4  2017/10/16 13:16:28  marpas
	SmartQuery template added
	
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
	
	Revision 2.12  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.11  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.10  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.9  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.6  2012/02/21 16:46:51  marfav
	fixed dependency from dbSmartPersistent header file
	
	Revision 2.5  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.4  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.3  2008/09/23 15:45:49  crivig
	porting to postgres8
	
	Revision 2.2  2007/11/29 11:30:22  marpas
	statistical improvements on dbQuery follow-on
	
	Revision 2.1  2006/10/30 15:03:04  paoscu
	ifdef removed.
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/21 15:36:34  paoscu
	Class is compiled only if _NEW_dbParam_STYLE_ has been defined.
	
	Revision 1.1  2005/11/18 18:35:43  paoscu
	dbSmartQuery added.
	
	
	
*/

#ifndef _dbSmartQuery_H_
#define _dbSmartQuery_H_

#include <acs_c++config.hpp>
#include <dbGeneralQuery.h>
#include <dbSmartPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbSmartPersistent ;

//
//
//
// class dbSmartQuery
//
//
//
class dbSmartQuery :    // PRQA S 2109
	public dbGeneralQuery
{
public:	
	dbSmartQuery(std::string const & tableName, const std::string &dbKey);
	dbSmartQuery(dbSmartQuery const &) ;
	dbSmartQuery &operator=(dbSmartQuery const &) ;
	
	virtual dbGeneralQueryIF *clone() const ;   // PRQA S 2502
	
	static  std::string staticClassName() ;
	virtual ~dbSmartQuery() throw() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 
    virtual dbSmartPersistent *newRecord(dbConnection &) ; 
    
	virtual dbSmartPersistent *at(unsigned long i) ; 
	virtual const dbSmartPersistent *at(unsigned long i) const ; 

private:
	dbSmartQuery() ; // declared but not implemented

private:
	std::string _tableName ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSmartQuery) ;
} ;

_ACS_BEGIN_NAMESPACE(db)
template <typename P>
class SmartQuery : // PRQA S 2109, 2153
	public dbSmartQuery
{
public:	
	explicit SmartQuery(const std::string &dbkey) : dbSmartQuery(P::table(), dbkey) {}
	virtual ~SmartQuery() throw() {}
	SmartQuery(SmartQuery const &r) : dbSmartQuery(P::table(), r.getKey()) {}
	SmartQuery &operator=(SmartQuery const &r) 
    {
	    if(this != &r)
	    {
		    Lock lock1(*this) ; // PRQA S 3050 2
		    Lock lock2(r) ;

		    dbSmartQuery::operator=(r) ;
	    }

	    return *this ;
    }

    virtual dbSmartPersistent *newRecord(dbConnection &conn)  { return new P(conn) ; }
	virtual P *operator[](unsigned long i) { return dynamic_cast<P *>(at(i)) ; } // PRQA S 2131, 3081
	virtual const P *operator[](unsigned long i) const { return dynamic_cast<const P *>(at(i)) ; } // PRQA S 2131, 3081
    size_t fill(std::vector<P> &v) const { // PRQA S 2502
        for (size_t i=0; i < size();++i) {
            v.push_back(*(*this)[i]) ;
        }
        return size() ;
    }

	static std::string staticClassName() { return P::staticQueryClassName() ; }

private:
    SmartQuery() ; // not implemented
} ;
_ACS_END_NAMESPACE


_ACS_END_NAMESPACE

#endif // _dbSmartQuery_H_
