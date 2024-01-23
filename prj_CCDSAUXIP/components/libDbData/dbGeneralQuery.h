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
	Revision 5.4  2016/03/26 21:45:49  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.3  2016/03/14 16:56:37  marpas
	implementing getKey()
	
	Revision 5.2  2016/03/14 13:16:39  marpas
	introducing offset support in queries
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.15  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.14  2013/02/25 16:50:16  marpas
	interface minor changes
	
	Revision 2.13  2013/02/20 17:08:27  marpas
	qa warning
	
	Revision 2.12  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.11  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.10  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.6  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.5  2009/06/16 13:09:57  marpas
	join implemented on queries
	
	Revision 2.4  2008/12/15 16:45:12  marpas
	comments added
	
	Revision 2.3  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.2  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.1  2007/11/29 11:30:22  marpas
	statistical improvements on dbQuery follow-on
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.16  2006/01/11 13:53:28  aleric
	addOrderBy interface added
	
	Revision 1.15  2005/10/11 07:56:13  marpas
	getParameters I/F also for non-DBEXPRESS
	
	Revision 1.14  2005/08/08 11:44:59  enrcar
	Added DBEXPRESS handling
	
	Revision 1.11  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.10  2004/08/30 17:47:45  marpas
	dbGeneralQueryIF::doQuery method has a new parameter to enable/disable the query.
	The default behaviour should be:
	1) if the incremental is not set, the query should clear the previous result if any
	2) if the enable is false, the query should not be performed and the size of the result should be returned
	
	Because many of the class inheriting this interface do not implement the incremental feature, for the moment a clear of the result and returning 0 is then accepted
	
	Revision 1.9  2004/04/28 13:09:22  paoscu
	Incremental query support.
	
	Revision 1.8  2004/04/13 16:37:25  marpas
	copy ctor and assignement operator fixed
	
	Revision 1.7  2004/04/13 14:17:33  marpas
	ThreadSafe inherited virtually
	
	Revision 1.6  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.5  2004/02/16 13:37:50  paoscu
	Comments in Doxygen style.
	
	Revision 1.4  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.2  2003/02/13 13:53:55  marpas
	*** empty log message ***
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/


#ifndef _dbGeneralQuery_H_
#define _dbGeneralQuery_H_ 

//
//
//
// class dbGeneralQuery
//		
//
//

#include <dbGeneralQueryIF.h>

#include <dbException.h>

#include <vector>
#include <string>

#include <SQLString.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbQueryParameters ; 
class dbConnection ; 
class dbPersistent ;

class dbGeneralQuery : public dbGeneralQueryIF // PRQA S 1703, 2109, 2153
{
public:

	/*! class InvalidUse declaration */
	exDECLARE_EXCEPTION(InvalidUse,dbException) ; // PRQA S 2131, 2153, 2502
	
	dbGeneralQuery() = delete ;  
	explicit dbGeneralQuery(const std::string &key);
	virtual ~dbGeneralQuery() noexcept ;

	/*! cast operator to Mutex. It allows to lock a resource set from external.
	*/
    operator Mutex &() const noexcept { return _mutex ; } // PRQA S 2181

	virtual void addJoin(SQLDialect::SQLJoinToken, const std::string &table, const dbQueryParameters &) ; 

	virtual void addParameter(const dbQueryParameters &,ParamOperator = AND) ; 
	virtual void addParameter(const std::string &,ParamOperator = AND) ; 
	virtual void resetParameters() ; 
	
	virtual dbGeneralQueryIF* clone() const = 0 ; // PRQA S 2502
	
	/*! returns the number of rows retrieved, that in incremental mode do not necessarily is the same of size() */
	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled=true) = 0;
	 
	
	//* remove the object from the collection
	virtual void erase(unsigned long pos) ; 		
	
	//* remove the object from the collection
	virtual void clear() ; 							
	
	virtual dbPersistent *item(unsigned long) ;
	virtual const dbPersistent *item(unsigned long) const ;

	virtual size_t size() const ; 
    virtual bool empty() const ;
    	
	//* the max number of items returned by the query
	virtual ssize_t maxItems() const ; 


	//* set the max number of items returned by the query
	virtual void maxItems(ssize_t) ; 


	ssize_t offset() const noexcept { return _offset ; } 
	void offset(ssize_t o) noexcept { _offset = o ; } 

	virtual void setOrderBy(const std::string &) ;
	virtual const std::string &getOrderBySqlClause() const ;
	virtual const std::string &getOrderBy() const ;
	virtual const std::string &getParameters() const ;
	virtual const std::vector<std::string>& getJoinParameters() const;
	virtual void copyJoinParameters(SQLString&);
	virtual void addOrderBy(const std::string &, SQLString::OrderBy const & o = SQLString::SORT_DEFAULT) ;

	const std::string &getKey() const noexcept { return _key ; }

protected:

	dbGeneralQuery(const dbGeneralQuery &) ; 
	dbGeneralQuery &operator=(const dbGeneralQuery &) ; 
    // do not declare any move ctor or assigment op
protected:		
		
	// PRQA S 2101 4
	std::vector<dbPersistent*> _objects ; 
    SQLString          _sqlString ;
	ssize_t	           _maxItems ;
    mutable std::string _orderBy ;

private:
    mutable Mutex   _mutex ;
	ssize_t _offset ;
    std::string _key ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGeneralQuery) ;

} ; 

_ACS_END_NAMESPACE

#endif
