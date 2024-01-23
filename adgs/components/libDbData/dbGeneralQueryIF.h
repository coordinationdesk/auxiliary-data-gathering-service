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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.9  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.8  2013/02/25 16:50:17  marpas
	interface minor changes
	
	Revision 2.7  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.6  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.4  2009/06/16 13:09:58  marpas
	join implemented on queries
	
	Revision 2.3  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2006/09/25 17:01:36  danalt
	virtual dtor fixed
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/08/08 11:49:06  enrcar
	DBEXPRESS handling added
	
	Revision 1.5  2005/07/21 13:02:05  marpas
	addParameter I/F changed, now accepts the AND/OR operator
	
	Revision 1.4  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.3  2004/08/30 17:47:45  marpas
	dbGeneralQueryIF::doQuery method has a new parameter to enable/disable the query.
	The default behaviour should be:
	1) if the incremental is not set, the query should clear the previous result if any
	2) if the enable is false, the query should not be performed and the size of the result should be returned
	
	Because many of the class inheriting this interface do not implement the incremental feature, for the moment a clear of the result and returning 0 is then accepted
	
	Revision 1.2  2004/04/28 13:09:22  paoscu
	Incremental query support.
	
	Revision 1.1  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
		

*/


#ifndef _dbGeneralQueryIF_H_
#define _dbGeneralQueryIF_H_ 

#include <acs_c++config.hpp>

#include <SQLDialect.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ; 
class dbPersistent ;
class dbQueryParameters ;

/** A pure abstract class to define the GeneralQuery Interface
 */
struct dbGeneralQueryIF // PRQA S 2171, 2173, 2175
{
    enum ParamOperator {
        AND,
        OR
    } ;

	virtual void addJoin(SQLDialect::SQLJoinToken, const std::string &table, const dbQueryParameters &) = 0 ; 

	virtual void addParameter(const dbQueryParameters &, ParamOperator = AND) = 0 ; 
	virtual void addParameter(const std::string &, ParamOperator = AND) = 0 ; 

	virtual void resetParameters()  = 0 ; 
	
	virtual dbGeneralQueryIF* clone() const = 0 ; // PRQA S 2502
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled=true) = 0;
	 
	
	//* remove the object from the collection
	virtual void erase(unsigned long pos) = 0 ; 		
	
	//* remove the object from the collection
	virtual void clear()  = 0 ; 							
	
	virtual dbPersistent *item(unsigned long)  = 0 ;
	virtual const dbPersistent *item(unsigned long) const  = 0 ;

	virtual size_t size() const = 0 ; 
    virtual bool empty() const = 0 ;
	
	//* the max number of items returned by the query
	virtual ssize_t maxItems() const = 0 ; 

	//* set the max number of items returned by the query
	virtual void maxItems(ssize_t) = 0 ; 

	virtual void setOrderBy(const std::string &) = 0 ;	

	virtual const std::string &getOrderBySqlClause() const = 0 ;
	virtual const std::string &getOrderBy() const = 0 ;

	virtual const std::string &getParameters() const = 0 ;

    virtual ~dbGeneralQueryIF() throw() {} // PRQA S 2131
    
protected:
    dbGeneralQueryIF() throw() {} 
    dbGeneralQueryIF(const dbGeneralQueryIF &) throw() {} 
    dbGeneralQueryIF & operator= (const dbGeneralQueryIF &) throw() { return * this ; } // PRQA S 4072
} ; 

_ACS_END_NAMESPACE

#endif
