/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
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
	Revision 5.2  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.9  2013/05/14 18:02:29  marpas
	now uses Singleton pattern
	PostGRES/PostGIS support implemented
	
	Revision 4.8  2013/02/26 17:12:25  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.7  2013/01/22 17:54:37  marpas
	qa warnings
	
	Revision 4.6  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/12/11 17:30:12  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.1  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.1  2010/04/29 10:44:31  marpas
	deal with special columntype (proprietary type e.g. Postgres TEXT)
	some specific type can be treated as blob/clob
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.13  2009/06/19 17:52:52  marpas
	dbCOnnection defaulted to null
	
	Revision 2.12  2009/06/19 17:47:58  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.11  2008/10/17 17:04:09  marpas
	convenience methods added
	
	Revision 2.10  2008/10/17 16:28:53  marpas
	exceptions hierarchy from dbSchemaMetadataProvider rearranged
	
	Revision 2.9  2007/12/17 10:44:32  paoscu
	Added support to retrieve information on dependant (child) tables.
	
	Revision 2.8  2007/09/14 14:16:35  lucio.pulvirenti
	Added methods to find out if fields have default values.
	
	Revision 2.7  2006/11/02 18:45:18  paoscu
	Added support for dbDateTimeMode. Requires a change in db dialect libs.
	
	Revision 2.6  2006/10/26 17:49:23  paoscu
	Methods hasForeignKeys and getForeignKeys added.
	
	Revision 2.5  2006/10/13 13:33:04  paoscu
	Method getFieldNames added.
	
	Revision 2.4  2006/08/23 10:15:23  paoscu
	Work in progress on labels
	
	Revision 2.3  2006/05/12 17:38:57  paoscu
	getGenericFieldType added.
	
	Revision 2.2  2006/03/17 01:47:45  paoscu
	Case convertion properly performed.
	Added private methods getTable and getField.
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.13  2005/11/18 19:10:23  paoscu
	New Methods added to handle serial fields, primary keys and foreign keys.
	Added a method to provide the list of "updatable" fields (all but pk and clob/blobs )
	Multithread support enhanced.
	More debug printouts added.
	A global try/catch section has been placed in the metadata retrieving block
	to provide a better error message.
	
	Revision 1.12  2005/11/04 19:42:54  paoscu
	Methods isBlob and isClob added.
	
	Revision 1.11  2005/10/27 21:12:34  paoscu
	Method getFieldFromName added.
	New kind of access methods (based on field name) added.
	New check added during metadata extraction. If metadata query returns zero row an exception is raised.
	
	Revision 1.10  2005/10/21 15:47:26  paoscu
	Added method getFieldSize.
	
	Revision 1.9  2005/09/12 16:33:31  paoscu
	Work in progress
	
	Revision 1.8  2005/08/12 14:30:57  paoscu
	field fieldSize added to FieldMetaData
	
	Revision 1.7  2005/08/11 16:51:16  paoscu
	isClobOrBlob method added.
	
	Revision 1.6  2005/08/10 16:48:48  paoscu
	Work in progress
	
	Revision 1.5  2005/08/08 16:57:23  paoscu
	getPrimaryKeys method added.
	
	Revision 1.4  2005/08/08 15:25:08  paoscu
	getIdFieldName and getFields methods added.
	
	Revision 1.3  2005/08/05 16:51:27  paoscu
	Work in progress
	
	Revision 1.2  2005/08/04 15:49:52  paoscu
	Work in Progress.....
	
	Revision 1.1  2005/08/01 18:13:12  paoscu
	dbSchemaMetaDataProvider introduced instead of dbFieldDescription.
	
	
*/


#ifndef _dbSchemaMetaDataProvider_H_
#define _dbSchemaMetaDataProvider_H_

#include <acs_c++config.hpp>
#include <dbColumnType.h>
#include <exMacros.h>
#include <dbException.h>
#include <ThreadSafe.h>
#include <Singleton.hpp>


#include <map>
#include <set>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::ParamConstructibleSingletonMap ;

/**
 * Class to provide informations about database metadata
 * By definition a table has an Id field if it has one (and only one) primary key of numeric type
 * 
 */


//PAOLO ToDo
// Do something to manage labels
// A good idea is to have a translation table with fields
// field table label
// table can be NULL
// A unique contraints exist on the cople field,table
// If an entry does not exist the field name is used

// Thre should be a method to check if a table exist in the database
// This can be useful to obtain more infos in case of troubles
// it can be also usefull for label management: the 'labels' table  could be optional
// If it is not present a warning (and only one) is emitted 

class SQLDialect ;
class dbConnection ;

class dbSchemaMetaDataProvider : // PRQA S 2109, 2153
    public ParamConstructibleSingletonMap<dbSchemaMetaDataProvider, std::string> 
{
    friend dbSchemaMetaDataProvider* ParamConstructibleSingletonMap<dbSchemaMetaDataProvider, std::string>::instance(const std::string &); // PRQA S 2107


public:
	/*! class MetadataError declaration */
	exDECLARE_EXCEPTION(MetadataError,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class TableNotPresent declaration */
	exDECLARE_EXCEPTION(TableNotPresent,MetadataError) ; // PRQA S 2131, 2153, 2502

	/*! class FieldNotPresent declaration */
	exDECLARE_EXCEPTION(FieldNotPresent,MetadataError) ; // PRQA S 2131, 2153, 2502


public: // NOSONAR - for clarity 
	~dbSchemaMetaDataProvider() override = default ;

	dbSchemaMetaDataProvider() = delete ;	
	dbSchemaMetaDataProvider(const dbSchemaMetaDataProvider &) = delete ;		
	dbSchemaMetaDataProvider &operator=(const dbSchemaMetaDataProvider &) = delete ;	
	
	size_t fieldsNumber (std::string const & tableName, dbConnection &) ;
	
	//* returns a string in the form "table.field1, tabel.field2, table.field3, .....  "
	std::string getFields (std::string const & tableName, dbConnection &) ;

	//* returns a string in the form "table.field1, tabel.field2, table.field3, .....  "
    //* manage geobject case
	std::string getSelectFields (std::string const & tableName, dbConnection &) ;

	//* get the fields of the record (enclosed in <t> fieldn <\t> or <l> fieldn <\l>)
	bool hasPrimaryKeys (std::string const & tableName, dbConnection &) ;

	bool hasIdField (std::string const & tableName, dbConnection &) ;

	bool hasForeignKeys (std::string const & tableName, dbConnection &) ;

	size_t getIdField (std::string const & tableName, dbConnection &) ;

	bool hasSerialField (std::string const & tableName, dbConnection &) ;

	bool hasDefaultField (std::string const & tableName, dbConnection &) ;

	size_t getSerialField (std::string const & tableName, dbConnection &) ;

	std::string getIdFieldName (std::string const & tableName, dbConnection &) ;

	std::string getSerialFieldName (std::string const & tableName, dbConnection &) ;

	std::vector<size_t>	getPrimaryKeys(std::string const & tableName, dbConnection &) ;

	std::vector<size_t>	getForeignKeys (std::string const & tableName, dbConnection &) ;

	std::vector<size_t>	getUpdatableFields(std::string const & tableName, dbConnection &) ;

	std::vector<std::string> getFieldNames (std::string const & tableName, dbConnection &) ;

	std::vector<size_t>	getDefaultFields (std::string const & tableName, dbConnection &) ;
	
	std::string getFieldName (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	std::string getFieldProprietaryType	(std::string const & tableName, size_t fieldPos, dbConnection &) ;

	dbSpecificColumnType::Type getFieldType (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	dbGenericColumnType::Type getGenericFieldType (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	dbDateTimeMode::Type getDateTimeMode (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	unsigned long getFieldSize (std::string const & tableName, size_t fieldPos, dbConnection &) ;

    std::string getFieldLabel (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isPrimaryKey (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isUnique (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool notNull(std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isSerial (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool hasDefault (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isClob (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isBlob (std::string const & tableName, size_t fieldPos, dbConnection &) ;
	
	bool isJsonb (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isClobOrBlob (std::string const & tableName, size_t fieldPos, dbConnection &) ;
	
	bool isUploadableObject (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	bool isForeignKey (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	std::string getForeignTable (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	unsigned long getForeignField (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	std::string getForeignFieldName (std::string const & tableName, size_t fieldPos, dbConnection &) ;

	std::string getFieldProprietaryType	(std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	dbSpecificColumnType::Type getFieldType (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	dbGenericColumnType::Type getGenericFieldType (std::string const & tableName, std::string const & fieldName, dbConnection &) ;
	dbDateTimeMode::Type getDateTimeMode (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	unsigned long getFieldSize (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	std::string getFieldLabel (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool isPrimaryKey (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool isUnique (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool notNull (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool isSerial (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool hasDefault (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool isClobOrBlob (std::string const & tableName, std::string const & fieldName, dbConnection &) ;
	
	bool isUploadableObject (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool isForeignKey (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	std::string getForeignTable (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	unsigned long getForeignField (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	std::string getForeignFieldName (std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	/**
	 * Returns the position of the field with the given name.
	 * Note that case convertion is performed on both field and table names
	 * according to SQL dialect.
	 */
	size_t getFieldFromName(std::string const & tableName, std::string const & fieldName, dbConnection &) ;


	/**
	 * Returns the position of the field with the given label.
	 * Note that case convertion is performed on both field and table names
	 * according to SQL dialect.
	 */
	size_t getFieldFromLabel(std::string const & tableName, std::string const & fieldLabel, dbConnection &) ;
	
	
	bool hasFieldWithName(std::string const & tableName, std::string const & fieldName, dbConnection &) ;

	bool hasFieldWithLabel(std::string const & tableName, std::string const & fieldLabel, dbConnection &) ;

	/**
	 * Returns true if the table has an index composed by the given list of fields
	 */
	bool existIndex (const std::string &tableName, const std::vector<std::string> &fieldList, dbConnection &) ;

	/**
	 * Returns the PostGIS major version number -1 meand no PostGIS installed 
	 */
	int getPostGISMajor() ;
    void assumeNoPostGIS() noexcept ;

	/**
	 * Returns information on dependant tables, i.e. tables with a foreign key pointing to a given table
	 * The information are collected into an STL structure composed like this:
	 * The high level object is an std::map: for every dependant table there is an entry
	 * where the key is the name of the dependant table.
	 * Every entry is an std::vector of std::pair : The first element of the pair is the fieldName
	 * in the dependant table (FK) while the second one is the correspondant field in the given table (PK)
	 *
	 * Example of usage:
	 * \code
	 * map<string , vector< pair < string , string> > > childs;
	 * typedef map<string , vector< pair < string , string> > >::iterator MyIterType ;
	 * childs = dbSchemaMetaDataProvider::instance() -> getDependantTables(tableName)	;
	 * for ( MyIterType it=childs.begin() ; it !=  childs.end() ; it++ )
	 * {
	 *     excerr << it->first << " : " << "\n";
	 *     for( size_t col = 0 ; col < it->second.size() ; col ++)
	 *  	   excerr << "   " << it->first << "." << it->second[col].first << " --> "
	 *  			  << tableName << "." << it->second[col].second << "\n";
	 *     excerr <<"\n" ;
	 * }
	 *
	 * \endcode
	 */
	std::map<std::string , std::vector< std::pair<std::string , std::string > > > getDependantTables(std::string const & tableName, dbConnection &)  ; // NOSONAR 


	static bool existTable( const std::string &, dbConnection &) ;
	static bool hasField(const std::string &tableName, const std::string &fieldName, dbConnection &) ;
	
	static bool hasIndex (const std::string &tableName, const std::string &fieldName, dbConnection &) ;
	static bool hasIndex (const std::string &tableName, const std::vector<std::string> &fieldList, dbConnection &) ;	

private:

	using IndexSet = std::map < std::set < std::string > , std::string, std::less<> > ; // NOSONAR 

	struct FieldMetaData // PRQA S 2173, 2175
	{
		//members
		std::string 				fieldName = "" ;
		std::string 				fieldLabel = "" ;
		dbSpecificColumnType::Type 	type = {} ;
		std::string 				proprietaryType = {} ;
		unsigned long				fieldSize = 0 ;
		dbDateTimeMode::Type 		dateTimeMode = {} ;	
		bool 						primaryKey = false ;
		bool						notNull = false ;
		bool						isSerial = false ;
		bool						hasDefault = false ;
		std::string 				foreignTable = "" ;
		unsigned long 				foreignField = 0 ;
		std::string 				foreignFieldName = "" ;
        bool                        isUnique = false  ;
	};
	
	struct TableMetaData // PRQA S 2173, 2175
	{
		//members
		std::string 					tableName = "" ;
		std::string 					tableLabel = "" ;
		std::string						fieldsString = "" ;
		std::string						selectFieldsString = "" ;
		std::vector<FieldMetaData> 		fields = {} ;
		bool							hasIdField = false ;
		bool							hasSerialField = false ;
		bool							hasDefaultField = false ;
		size_t                          serialField = 0 ;
		std::vector<size_t>             primaryKeys = {} ;
		std::vector<size_t>             foreignKeys = {} ;
		std::vector<size_t>             updatableKeys = {} ;
		std::vector<size_t>             defaultFields = {} ;
	};
	
	explicit dbSchemaMetaDataProvider(const std::string& key) ;

	//* Check if the table has already been loaded and eventually do that.
	//* \throws TableNotPresent if table is not present in DB.
	void checkTable(std::string const & tableName, dbConnection &) ;
	
	// Retrieves the information about table indexes 
	// if not already retrieved
	void retrieveTableIndexes (std::string const & tableName, dbConnection &) ;

	//* Returns a reference to the given table
	//* Performs a case convertion on tableName using the SQL dialect
	//* Uses checkTable
	TableMetaData const & getTable(std::string const & tableName, dbConnection &) ;

	//* Returns a reference to the given field
	//* Uses getTable
	//* Throws FieldNotPresent if fieldPos exceeds fields number.
	FieldMetaData const & getField(std::string const & tableName, size_t fieldPos, dbConnection &) ;
		
	
	static dbDateTimeMode::Type evaluateDateTimeMode(std::string const & dateteTimeRangeFrom, std::string const & dateteTimeRangeTo ) ;
	
	
private: // NOSONAR - for clarity
	std::unordered_map<std::string ,  TableMetaData> _tables = {} ;
	std::string _key ; 
	// Map containing the table name (normalized according to the dialec)
	// and a map of set of field names versus indexes name
	std::unordered_map<std::string ,  IndexSet> _indexes = {}  ;
	int _postGisVersion = 0 ;
    Mutex _mutex = {} ; 
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSchemaMetaDataProvider) ;



} ;

_ACS_END_NAMESPACE

#endif // _dbSchemaMetaDataProvider_H_
