// PRQA S 1050 EOF
/*

	Copyright 2018-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 1.5  2018/08/01 10:29:01  lucio.pulvirenti
	PDSEV-47: Singleton removed.
	
	Revision 1.4  2018/07/23 10:08:42  lucio.pulvirenti
	PDSEV-47: refresh public method added to IF. setInternalResource private method added. _loaded and _pkColumn private attribute added.
	Both getValue and setValue methods throw exception if not _loaded.
	
	Revision 1.3  2018/07/20 16:40:15  lucio.pulvirenti
	PDSEV-47: rsTableSingleton added. Classes tested.
	
	Revision 1.2  2018/07/20 12:22:53  lucio.pulvirenti
	PDSEV-47: work in progress.
	
	Revision 1.1  2018/07/19 15:42:53  lucio.pulvirenti
	PDSEV-47: work in progress.
	
	
	
*/

#ifndef _rsTable_H_
#define _rsTable_H_

#include <dbQuery.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <rsResourceSet.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * This Class manages a database table the records of which correspond to elements of a ResourceSet object with the key on one column and the value on another one. The table may have in principle
 * columns other than key and value, in particular it may have a primary key column. If occurring, it is used to query a bunch of records at a time needed to manage very large amounts of data.
 **/
class rsTable // PRQA S 2109
{
public:
	/** exRsTableException Exceptions */
	exDECLARE_EXCEPTION(exRsTableException, exException) ; // Base exRsTableException Exception. // PRQA S 2131, 2502

	/**
	* \brief ctor
	* \param dbkey The database key
	*/
	rsTable(const std::string &dbkey) ;

	rsTable() = delete ;
	rsTable(const rsTable &) = delete ;
	rsTable &operator=(const rsTable &) = delete ;
	rsTable( rsTable &&) = delete ;
	rsTable &operator=( rsTable &&) = delete ;

	/**
	* \brief ctor. When object is built, the table records have been loaded so that rsResourceSet private attribute is filled
	* \param dbkey The database key
	* \param tbl The table name
	* \param keyCol The name of column with ResourceSet key
	* \param valCol The name of column with ResourceSet value
	*/
	rsTable(const std::string &dbkey, const std::string &tbl, const std::string &keyCol = "key", const std::string &valCol = "value" );     					
	/**
	 * \brief dtor
	 */
	virtual ~rsTable() noexcept ;
	
	/**
	* \brief Query all the table records and fill rsResourceSet private attribute if called the first time, otherwise call refresh method. 
	* Exception thrown if invoked more than once on a different table or with different keyCol or with different valCol
	* \param tbl The table name
	* \param keyCol The name of column with ResourceSet key
	* \param valCol The name of column with ResourceSet value
	*/
	void load(const std::string &tbl, const std::string &keyCol = "key", const std::string &valCol = "value" );  
	/**
	* \brief Load the input ResourceSet on table by inserting a bunch of resources at a time. The original table content is removed. Exception thrown if invoked with no loaded table 
	* \param ResourceSet to save on db
	* \param step The number of input resources to be inserted at one time.
	*/
	void save( const rsResourceSet &, size_t step = 100 );	
	
	/**
	* \brief It allows casting of rsTable object to rsResourceSet by returning the rsResourceSet private attribute. It allows e.g. using code such as rsPushNSpace ns(t,"field3"). Exception thrown if invoked with no loaded table
	*/
	operator rsResourceSet & ();

	
	/**
	* \brief Return a resource value given its key by accessing to  rsResourceSet private attribute. Exception thrown if invoked with no loaded table
	* \param ResourceSet key
	* \param ResourceSet value.
	*/
	template <typename T> void getValue( const std::string &, T & ) const;						
	/**
	* \brief Load on table record a resource element: if a record with that key already exists, simply update with the new value, else update a new record. rsResourceSet private attribute is updated accordingly. Exception thrown if invoked with no loaded table
	* \param key
	* \param value.
	*/
	template <typename T> void setValue( const std::string &, T & ) ;						
	/**
	* \brief Add the input ResourceSet to table records. For every element, setValue method is invoked. Exception thrown if invoked with no loaded table
	* \param ResourceSet to add.
	*/
	void add( const rsResourceSet &);	
	/**
	* \brief Refresh rsResourceSet private attribute by downloading table records. Exception thrown if invoked with no loaded table
	*/
	void refresh();  


private:
	void load();
	void setInternalResource(dbConnection &);
	template <typename T> void setValue( const std::string &, T &, dbConnection & ) ;						
private:
	std::string _dbKey ; 
	std::string _tableName ; 
	std::string _keyColumn ; 
	std::string _valueColumn ; 
	rsResourceSet _rs;
	bool _loaded;
	std::string _pkColumn;
	static Mutex  _mutex ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(rsTable) ;
} ;


// 
// template<typename T>
// void rsTable::getValue(const std::string &res, T &v) const 
// {
// 	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
// 	Lock lock(_mutex) ;
// 	static_cast<rsResourceSet*>(this)->getValue(res, v);
// } 
// 
template<typename T>
void rsTable::getValue(const std::string &res, T &v) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock lock(_mutex) ;
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );
	_rs.getValue(res, v);
} 

template<typename T>
void rsTable::setValue(const std::string &res, T &v)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock lock(_mutex) ;
	
	ACS_COND_THROW( not _loaded, exRsTableException("No table loaded!") );
	setValue(res, v, dbConnectionWrapper(db::ConnPools::instance()->getPool(_dbKey)));

}


template<typename T>
void rsTable::setValue(const std::string &res, T &v, dbConnection &conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	db::Transaction transaction(conn ) ;
	transaction.begin_work("Set " + _tableName + " value") ;

	SQLString sqlString(_dbKey, "" ) ;
    sqlString.addTableList(_tableName);
	if( _rs.hasKey(res) ) {
        sqlString.setType(SQLString::UPDATE ) ;
        sqlString.addSetList(_valueColumn, db::toSQL(_dbKey, v) ) ;
		{
			dbQueryParameters qp(_dbKey);
			qp.addParameter( dbParam(_dbKey, _tableName, _keyColumn, dbParam::EQUAL_TO, db::toSQL(_dbKey, res)) ) ;
			sqlString.addANDWhereParameter(qp);
		}
	}
	else {
        sqlString.setType(SQLString::INSERT ) ;
        sqlString.addValuesList(_keyColumn, db::toSQL(_dbKey, res) ) ;
        sqlString.addValuesList(_valueColumn, db::toSQL(_dbKey, v) ) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following command: " << sqlString.getSQL() );

	dbQuery query(conn, sqlString, "Set " + _tableName + " value") ;
    if (not query.execForStatus())  {
        ACS_THROW(  dbQuery::QueryExecuteException("Query Execution error; Command : " + sqlString.getSQL() + " Error : " + query.errorString())) ;
    }

	_rs.setValue(res, v);
	transaction.commit();

} 
 

_ACS_END_NAMESPACE

#endif /* _rsTable_H_ */
