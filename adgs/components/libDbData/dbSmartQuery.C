// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 5.7  2017/10/16 13:16:28  marpas
	SmartQuery template added
	
	Revision 5.6  2017/02/23 11:08:03  lucio.pulvirenti
	Offset now considered.
	
	Revision 5.5  2016/07/29 13:48:16  marfav
	Fixed CLOB/BLOB management when loading smart persistent and related objects
	
	Revision 5.4  2016/04/11 13:23:03  marpas
	doQuery accept a connection reference
	default ctor has been removed
	
	Revision 5.3  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.2  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.20  2013/06/04 09:31:23  marpas
	getting rid of deprecated methods to get new connections, using dbConnectionPool instead
	
	Revision 2.19  2013/05/14 19:03:31  marpas
	minor changes to release old deprecated libMessages support
	dbSchemaMetadataProvider interface now requires db connection key
	
	Revision 2.18  2013/03/28 13:49:15  marpas
	coding best practices improved
	statistics improved to allow integrators to check wueries efficiency
	Connection and ConnectionPool usage enforced
	
	Revision 2.17  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.16  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.15  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.14  2012/11/19 22:49:56  marpas
	dbSmartPersistent operator at introduced and operator[] removed in order to allow dbXXXQuery to reimplement it.
	
	Revision 2.13  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.10  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.9  2011/10/20 13:51:10  lucio.pulvirenti
	Statistics improved
	
	Revision 2.8  2011/10/14 07:53:09  marpas
	smart persistent and related classes load queries with read only
	refresh interface now has a connection pointer defaulted to null
	
	Revision 2.7  2009/06/19 18:36:29  marpas
	still working on dbConnection speedup
	
	Revision 2.6  2009/06/19 18:11:35  marpas
	dbConnection used to speed up work and to avoid issuing multiple connections and queries when it is useless
	
	Revision 2.5  2008/09/23 15:45:48  crivig
	porting to postgres8
	
	Revision 2.4  2007/11/29 11:36:58  marpas
	tablename string improved for queries statistics
	
	Revision 2.3  2007/11/29 11:35:11  marpas
	if query stat label is "tablename" the stat label will be the table name
	
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

#include <dbSmartQuery.h>

#include <dbSmartPersistent.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <Filterables.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSmartQuery)

string dbSmartQuery::staticClassName()
{
	return "dbSmartQuery" ;
}

// constructor
dbSmartQuery::dbSmartQuery(std::string const & tableName, const string &dbKey) : 
    dbGeneralQuery(dbKey),
	_tableName(tableName)
{
}


dbSmartQuery::dbSmartQuery(dbSmartQuery const &r) :
    dbGeneralQuery(r),
    _tableName(r._tableName) 
{
	
}

dbSmartQuery &dbSmartQuery::operator=(dbSmartQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
		_tableName = other._tableName ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbSmartQuery::clone() const
{
	return new dbSmartQuery(*this) ;
}


dbSmartQuery::~dbSmartQuery() throw() {}


dbSmartPersistent *dbSmartQuery::newRecord(dbConnection &conn) 
{
    return new dbSmartPersistent(_tableName, conn) ;
}


unsigned long dbSmartQuery::doQuery(dbConnection &conn, bool incremental, bool enabled) // PRQA S 4020
{

	if(incremental)
	{
		ACS_THROW(InvalidUse("Incremental query not allowed.")) ; // PRQA S 3081
	}
	
 	clear() ; // erase previous result set
	if(!enabled) { 	return 0; }
    const string &dbKey = conn.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT, true) ;
	queryString.addTableList( _tableName ) ;

	if (maxItems() > 0) {
		queryString.setLimit( maxItems() ) ; // PRQA S 3000
    }

	if (offset() > 0) {
		queryString.setOffset( offset() ) ; // PRQA S 3000
    }


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        size_t mfsize = dbSchemaMetaDataProvider::instance(dbKey) -> fieldsNumber(_tableName, conn) ;
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Fields:") ;
        for (size_t i=0; i < mfsize; ++i) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*\tname: " << dbSchemaMetaDataProvider::instance(dbKey) -> getFieldName(_tableName, i, conn)) ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\tproprietaryType: " << dbSchemaMetaDataProvider::instance(dbKey) -> getFieldProprietaryType(_tableName, i, conn)) ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\ttype: " << dbColumnTypeHelper::getTypeName(dbSchemaMetaDataProvider::instance(dbKey) -> getFieldType(_tableName, i, conn) ));
        }
	ACS_CLASS_END_DEBUG

	// returns also blobs and clobs as SIGNED INTs
	// Those fields will not be really loaded, but it will be possible to see if is NULL or not. 
	dbSmartPersistent::setLOBFreeSelectList (_tableName, queryString, conn);

	queryString.addANDWhereParameter( getParameters() ) ;
    queryString.setOrderByString( getOrderBySqlClause() ) ;
    copyJoinParameters(queryString);
    
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(conn, queryString, "SQSEL " + _tableName) ;

	dbSet set ;
	query.execForSet(set) ;

	dbRow row(dbKey) ;
	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		ACS_SMARTPTR<dbSmartPersistent> record(newRecord(conn)) ;
		record->loadRow(row,0) ;
		_objects.push_back(record.release()) ; // release ownership
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "new record at position #" << i) ;
	}

	return size() ;
}

dbSmartPersistent *dbSmartQuery::at(unsigned long i)
{
	return dynamic_cast<dbSmartPersistent *>(item(i)) ; // PRQA S 3081
}

const dbSmartPersistent *dbSmartQuery::at(unsigned long i) const
{
	return dynamic_cast<const dbSmartPersistent *>(item(i)) ; // PRQA S 3081
}



_ACS_END_NAMESPACE
