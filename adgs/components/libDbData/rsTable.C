// PRQA S 1050 EOF
/*

	Copyright 2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2018/07/23 10:16:41  lucio.pulvirenti
	PDSEV-47: load method cannot be invoked more than once. _pkColumn attribute set in load method. refresh public method added. setInternalResource private method
	developed to be called by both load and refresh public methods.
	
	Revision 1.3  2018/07/20 16:41:20  lucio.pulvirenti
	PDSEV-47: class tested.
	
	Revision 1.2  2018/07/20 12:22:53  lucio.pulvirenti
	PDSEV-47: work in progress.
	
	Revision 1.1  2018/07/19 15:42:53  lucio.pulvirenti
	PDSEV-47: work in progress.
	

*/

#include <rsTable.h>

#include <dbSmartQuery.h>
#include <dbSchemaMetaDataProvider.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
	const size_t recordsNumberToLoad = 5000;
};

ACS_CLASS_DEFINE_DEBUG_LEVEL(rsTable)
		
Mutex rsTable::_mutex;


rsTable::rsTable(const string &dbKey) :
	_dbKey(dbKey),
	_tableName(), 
	_keyColumn(), 
	_valueColumn(), 
	_rs(),
	_loaded(false),
	_pkColumn()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
}

rsTable::rsTable(const string &dbKey, const string &tbl, const string &keyCol, const string &valCol) :
	_dbKey(dbKey),
	_tableName(tbl), 
	_keyColumn(keyCol), 
	_valueColumn(valCol), 
	_rs(),
	_loaded(false),
	_pkColumn()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	load();
}


rsTable::~rsTable() throw() 
{}



void rsTable::load(const string &tbl, const string &keyCol, const string &valCol )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock lock(_mutex); 
	if( _loaded ) {
		if( tbl != _tableName || keyCol != _keyColumn || valCol != _valueColumn ) {
			ostringstream exMsg;
			exMsg << "Load method already invoked on \"" << _tableName << "\" with key column=\"" << _keyColumn << "\", val column=\"" << _valueColumn
					<< "\" now invoked on table=\"" << tbl << "\", key column=\"" << keyCol << "\", val column=\"" << valCol << '\"';
			ACS_THROW( exRsTableException(exMsg.str()) );
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Table " << _tableName << " already loaded, going to refresh" );
		refresh() ;
	}
	else {
	
		_tableName = tbl;
		_keyColumn = keyCol;
		_valueColumn = valCol;

		load();
	}
}

void rsTable::load() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

// some checks	
	ACS_COND_THROW( _tableName.empty(), exRsTableException("Table name not valorized!") );
	ACS_COND_THROW( ( _keyColumn.empty() || _valueColumn.empty() ), exRsTableException("Either Key or Value column not valorized!") );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Table=" << _tableName << "; key column=" << _keyColumn << ", value column=" << _valueColumn );
	
	dbConnectionWrapper dbcw(db::ConnPools::instance()->getPool(_dbKey)) ;
    dbConnection &conn = dbcw ; // PRQA S 3050 

// primary column needs for loading table records bunches if plenty of records occur
	if( dbSchemaMetaDataProvider::instance(_dbKey)->hasPrimaryKeys( _tableName, conn) ) {
		size_t numOfColumns = dbSchemaMetaDataProvider::instance(_dbKey)->fieldsNumber( _tableName, conn) ;
		for( size_t i = 0; i < numOfColumns; ++i ) {
			if( dbSchemaMetaDataProvider::instance(_dbKey)->isPrimaryKey ( _tableName, i, conn) ) {
				_pkColumn = dbSchemaMetaDataProvider::instance(_dbKey)->getFieldName( _tableName, i, conn) ;
				break;
			}
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _tableName << " has primary key column: " << _pkColumn) ;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _tableName << " has no primary key column") ;
	}

	Lock lock(_mutex); 
	setInternalResource(conn);
	_loaded = true;

}

void rsTable::save( const rsResourceSet & rs, size_t step)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock lock(_mutex); 
	
	// some checks	
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );

	string *keys = 0;
	string *values = 0;
	
	try {
	
		size_t el = rs.keys() ; 
		keys = new string[el] ;
		values = new string[el] ;
		rs.keys(keys, values) ;

		dbConnectionWrapper dbcw(db::ConnPools::instance()->getPool(_dbKey)) ;
    	dbConnection &conn = dbcw ; // PRQA S 3050 
		db::Transaction transaction(conn ) ;
		transaction.begin_work("Insert into " + _tableName ) ;

		{
			SQLString sqlString(_dbKey, SQLString::DELETE ) ;
    		sqlString.addTableList(_tableName);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following command: " << sqlString.getSQL() );
			dbQuery query(conn, sqlString, "DELETE" + _tableName) ;
        	if (not query.execForStatus())  {
            	ACS_THROW(  dbQuery::QueryExecuteException("Query Execution error; Command : " + sqlString.getSQL() + " Error : " + query.errorString())) ;
        	}
		}

		for (size_t i=0; i<el ; i+=step) { // PRQA S 3084

	    	string queryString = "INSERT INTO " + _tableName +  " (" + _keyColumn +','+ _valueColumn + ") VALUES " ;
        	for( size_t s =0; (s < step) && ( (i+s) < el ); s++)  { // PRQA S 3084
				ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,"\t#" << (i+s) << ' ' << keys[i+s] << ": " << values[i+s]) ;
            	if (s > 0) { 
					queryString += " , "; 
				}
            	queryString += "( ";
            	queryString += db::toSQL(_dbKey,keys[i+s]); // PRQA S 3081, 3050 4
            	queryString += ", " + db::toSQL(_dbKey,values[i+s]) ; // PRQA S 3084
            	queryString += ") ";
        	}

        	SQLString sqlString(_dbKey, SQLString::COMMAND) ;
        	sqlString.forceSQLString(queryString) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following command\n" << sqlString.getSQL() );

        	dbQuery query(conn, sqlString, "INS " + _tableName + " multiple") ;
        	if (not query.execForStatus())  {
            	ACS_THROW(  dbQuery::QueryExecuteException("Query Execution error; Command : " +  queryString + "\n Error : " + query.errorString())) ;
        	}

    	} //next insert 
			
		delete [] keys ;
		delete [] values ;

		_rs = rs;
		transaction.commit();
	}
	catch( exception & )
	{
		if( keys != 0 ) {
			delete [] keys ;
		}
		if( values != 0 ) {
			delete [] values ;
		}

		throw;
	}
}

void  rsTable::add( const rsResourceSet &rs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock lock(_mutex); 
	
	// some checks	
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );

	string *keys = 0;
	string *values = 0;
	try {
	
		size_t el = rs.keys() ; 
		keys = new string[el] ;
		values = new string[el] ;
		rs.keys(keys, values) ;
		
		dbConnectionWrapper dbcw(db::ConnPools::instance()->getPool(_dbKey)) ;
    	dbConnection &conn = dbcw ; // PRQA S 3050 
		db::Transaction transaction(conn ) ;
		transaction.begin_work("Adding to " + _tableName ) ;

		for (size_t i=0; i<el ; ++i) { // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB,"\t#" << i << ' ' << keys[i] << ": " << values[i]) ;
			setValue(keys[i], values[i], conn);		
    	}
			
		delete [] keys ;
		delete [] values ;
		
		transaction.commit();
	}
	catch( exception & )
	{
		if( keys != 0 ) {
			delete [] keys ;
		}
		if( values != 0 ) {
			delete [] values ;
		}

		throw;
	}
	
}



void rsTable::refresh() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	// some checks	
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );
	
	dbConnectionWrapper dbcw(db::ConnPools::instance()->getPool(_dbKey)) ;
    dbConnection &conn = dbcw ; // PRQA S 3050 

	Lock lock(_mutex); 
	rsResourceSet oldRs = _rs;
	try {
		setInternalResource(conn);
	}
	catch( exception & )
	{
		_rs = oldRs ;
		throw;
	}


}

void rsTable::setInternalResource(dbConnection & conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	_rs.clear() ;
	dbPersistent::IdType tableId = 0;
	size_t recSize = 0;
	do {
		dbSmartQuery query(_tableName, _dbKey) ;
		if( not _pkColumn.empty() ) {

	        dbQueryParameters qp( _dbKey);
			qp.addParameter( dbParam(_dbKey, _tableName, _pkColumn, dbParam::MAJOR_OF, db::toSQL(_dbKey, tableId)) ) ;
			query.addParameter(qp) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Query going to be executed on " << _tableName  << " table where "  << qp.getSqlString());
			query.maxItems(recordsNumberToLoad) ;
			query.addOrderBy(_pkColumn, SQLString::ASC) ;

		}
		query.doQuery(conn) ;
		recSize = query.size();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found " << recSize << " " << _tableName << " records.") ;

 		for(size_t i=0 ; i< recSize ; i++ ) {

			bool isNull;
			string key;
			query.at(i)->get(_keyColumn, key, isNull, conn) ;
			string val;
			query.at(i)->get(_valueColumn, val, isNull, conn) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, key << ": " << val) ;
			_rs.setValue(key, val) ;

			if( not _pkColumn.empty() ) {
				tableId = query.at(i)->getId() ;
			}

		}

	} while( ( not _pkColumn.empty() ) && ( recSize >= recordsNumberToLoad ) );
	
}

rsTable::operator rsResourceSet & ()
{
	Lock lock(_mutex); 
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );
	return _rs;
}	
_ACS_END_NAMESPACE
