// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.9  2017/11/21 18:45:06  marpas
	deprecated method removed in favour of execForSet
	minor changes and makeup
	debug improved
	
	Revision 5.8  2016/04/11 13:17:24  marpas
	using db key got from conn
	
	Revision 5.7  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.6  2016/03/04 16:14:37  lucio.pulvirenti
	CSGACQ-42: dialect spceific translateColumnName method used before convertCase.
	
	Revision 5.5  2015/03/19 14:32:41  marpas
	evaluateDateTimeMode takes into account the DateTime::getDefaultDbFractionDigits. So the format is the minimum between the actual table's column limit and the value set into the DateTime class.
	This because code using dbConv/toSQL and dbSmartPersistent MUST behave in the same way.
	
	Revision 5.4  2014/02/07 18:36:53  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/19 12:24:35  marpas
	Transaction class added
	
	Revision 5.2  2013/07/09 08:24:15  marpas
	compilation warning removed
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.16  2013/06/03 12:37:00  marpas
	ng rid of deprecated dbConnectionMgr use
	
	Revision 4.15  2013/05/30 17:12:18  marpas
	using connection pools in case of null connection instead of deprecated dbConnectionMgr
	
	Revision 4.14  2013/05/16 16:22:09  marpas
	CLOB is considered updatable field
	
	Revision 4.13  2013/05/15 11:50:23  marpas
	isClob and isBlob now use the specific SQLDialect filter functions and not longer the generale Blob/Clob one.
	
	Revision 4.12  2013/05/14 18:02:29  marpas
	now uses Singleton pattern
	PostGRES/PostGIS support implemented
	
	Revision 4.11  2013/03/27 18:15:00  marpas
	statistical query names changed
	
	Revision 4.10  2013/03/26 00:58:19  marpas
	dbRow columns are accessed throu a uint16_t
	other interfaces modified accordingly
	
	Revision 4.9  2013/02/26 17:12:25  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.8  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.7  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.6  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/10/18 17:23:46  marpas
	statistics improved
	
	Revision 4.0  2011/10/18 12:31:15  marpas
	new statistics adopted
	
	Revision 3.4  2010/09/08 17:47:26  marpas
	debug improved
	
	Revision 3.3  2010/04/29 10:44:31  marpas
	deal with special columntype (proprietary type e.g. Postgres TEXT)
	some specific type can be treated as blob/clob
	
	Revision 3.2  2010/04/09 10:26:02  crivig
	bug fixed
	
	Revision 3.1  2010/03/17 14:13:00  enrcar
	Debug improved
	
	Revision 3.0  2009/10/05 14:47:53  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.17  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.16  2009/06/19 17:47:58  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.15  2009/01/27 13:16:19  marpas
	db statistics improved, some other minor changes with no regression
	
	Revision 2.14  2008/10/17 17:04:09  marpas
	convenience methods added
	
	Revision 2.13  2008/10/17 16:28:53  marpas
	exceptions hierarchy from dbSchemaMetadataProvider rearranged
	
	Revision 2.12  2008/05/14 15:05:57  crivig
	fake query modified to handle strange fieldname in tables
	
	Revision 2.11  2007/12/17 10:44:32  paoscu
	Added support to retrieve information on dependant (child) tables.
	
	Revision 2.10  2007/12/04 10:17:11  crivig
	ENRCAR:: suggestedType handled
	
	Revision 2.9  2007/12/03 13:23:10  crivig
	modified in order to handle short int as bool for MySQL
	
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
	
	Revision 1.9  2005/10/10 15:01:03  paoscu
	Is nullable parameter is obtained from dedicated query instead of SQLCursor.
	
	Revision 1.8  2005/09/12 16:33:31  paoscu
	Work in progress
	
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

#include <dbSchemaMetaDataProvider.h>

#include <SQLString.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbConnectionPool.h>
#include <StringUtils.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)

using std::string ;
using std::set ;
using std::map ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSchemaMetaDataProvider);


dbSchemaMetaDataProvider::~dbSchemaMetaDataProvider() throw()
{}

int dbSchemaMetaDataProvider::getPostGISMajor() 
{
    if (0 == _postGisVersions) { // not yet got
        dbConnectionWrapper cw(db::ConnPools::instance()->getPool(_key)) ;

        dbConnection &conn = cw ;
		SQLString vqs(_key, SQLString::COMMAND) ;
        vqs.forceSQLString(SQLDialectFactory::sqlDialect(_key).gisVersionQuery());
        try {
		    dbQuery vqsQuery(conn, vqs, "Get PostGIS Version" ) ;
            dbSet set ;
	        vqsQuery.execForSet(set) ;
		    dbRow r(_key) ;
            set.getNextRow(r) ; // first and only 

            string version ;
            r.column(0, version);
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GIS version: \"" << version << "\"") ;
            // expected "2.0 USE_GEOS=1 USE_PROJ=1 USE_STATS=1"
            _postGisVersions = atoi(version.c_str()) ;
        }
        catch(std::exception &x) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, x) ;
 
            _postGisVersions = -1 ;
        }
    }
    return _postGisVersions ; 
}

size_t dbSchemaMetaDataProvider::fieldsNumber(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).fields.size() ; // PRQA S 3010
}



std::string dbSchemaMetaDataProvider::getFields(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).fieldsString ;
}

std::string dbSchemaMetaDataProvider::getSelectFields(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).selectFieldsString ;
}


bool dbSchemaMetaDataProvider::hasPrimaryKeys (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return 	( not getTable(tableName, conn).primaryKeys.empty() ) ;
}


bool dbSchemaMetaDataProvider::hasForeignKeys (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return 	( not getTable(tableName, conn).foreignKeys.empty() ) ;
}



bool dbSchemaMetaDataProvider::hasIdField (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).hasIdField ;
}



size_t dbSchemaMetaDataProvider::getIdField(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ; // lock 
	ACS_COND_THROW( ! hasIdField(tableName, conn), // PRQA S 3081
							  FieldNotPresent("Table \"" + tableName + "\" has not an Id field.")) ; ;
	return getTable(tableName, conn).primaryKeys[0] ;
}


bool dbSchemaMetaDataProvider::hasSerialField (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).hasSerialField ;
}

bool dbSchemaMetaDataProvider::hasDefaultField (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).hasDefaultField ;
}


size_t dbSchemaMetaDataProvider::getSerialField(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	ACS_COND_THROW( ! hasSerialField(tableName, conn),// PRQA S 3081
							   FieldNotPresent("Table \"" + tableName + "\" has not a serial field.")) ;
	return getTable(tableName, conn).serialField ;
}


std::string dbSchemaMetaDataProvider::getIdFieldName (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).fields[getIdField(tableName, conn)].fieldName ;
}


std::string dbSchemaMetaDataProvider::getSerialFieldName (std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).fields[getSerialField(tableName, conn)].fieldName ;
}



std::vector<size_t> dbSchemaMetaDataProvider::getPrimaryKeys(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	ACS_COND_THROW( ! hasPrimaryKeys(tableName, conn),// PRQA S 3081
							  FieldNotPresent("Table \"" + tableName + "\" has not primary keys.")) ;
	return getTable(tableName, conn).primaryKeys ;
}



std::vector<size_t> dbSchemaMetaDataProvider::getForeignKeys(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).foreignKeys ;
}



std::vector<size_t> dbSchemaMetaDataProvider::getUpdatableFields(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).updatableKeys ;
}


std::vector<std::string> dbSchemaMetaDataProvider::getFieldNames(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	std::vector<std::string> res ;
	std::vector<FieldMetaData> const& fields( getTable(tableName, conn).fields ) ;
	for ( size_t i=0 ; i<fields.size() ; i++) { // each field
		res.push_back ( fields[i].fieldName ) ; // push its name
	}
    
	return res ;
}

std::vector<size_t>	dbSchemaMetaDataProvider::getDefaultFields(std::string const & tableName, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getTable(tableName, conn).defaultFields ;
}



std::string dbSchemaMetaDataProvider::getFieldName(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).fieldName ;
}

std::string dbSchemaMetaDataProvider::getFieldProprietaryType(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tableName:" << tableName  << " fieldPos:" << fieldPos
            << " proprietaryType: " << getField( tableName ,  fieldPos, conn).proprietaryType) ;
	return getField( tableName ,  fieldPos, conn).proprietaryType ;
}




dbSpecificColumnType::Type dbSchemaMetaDataProvider::getFieldType(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tableName:" << tableName << " fieldPos:" << fieldPos
            << " type: " << getField( tableName ,  fieldPos, conn).type) ;
	return getField( tableName ,  fieldPos, conn).type ;
}


dbGenericColumnType::Type dbSchemaMetaDataProvider::getGenericFieldType(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	return dbColumnTypeHelper::convertTypeToGeneric( getFieldType(tableName , fieldPos, conn) ) ;
}




dbDateTimeMode::Type dbSchemaMetaDataProvider::getDateTimeMode (std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).dateTimeMode ;
}


unsigned long dbSchemaMetaDataProvider::getFieldSize(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).fieldSize ;
}



std::string dbSchemaMetaDataProvider::getFieldLabel(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).fieldLabel ;
}



bool dbSchemaMetaDataProvider::isPrimaryKey	(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).primaryKey ;
}

bool dbSchemaMetaDataProvider::isUnique	(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).isUnique ;
}



bool dbSchemaMetaDataProvider::notNull	(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).notNull ;
}


bool dbSchemaMetaDataProvider::isSerial	(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).isSerial ;
}


bool dbSchemaMetaDataProvider::hasDefault (std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return getField( tableName ,  fieldPos, conn).hasDefault ;
}

bool dbSchemaMetaDataProvider::isClob(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	return 
        (getFieldType(tableName , fieldPos, conn) == dbSpecificColumnType::CLOB ) || // PRQA S 3230
        ( SQLDialect::propTypeClob(conn.getKey(), getField( tableName ,  fieldPos, conn).proprietaryType)) ;
}

bool dbSchemaMetaDataProvider::isBlob(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	return 
        ( getFieldType(tableName , fieldPos, conn) == dbSpecificColumnType::BLOB ) || // PRQA S 3230
        ( SQLDialect::propTypeBlob(conn.getKey(), getField( tableName ,  fieldPos, conn).proprietaryType)) ;
}

bool dbSchemaMetaDataProvider::isJsonb (std::string const & tableName, size_t fieldPos, dbConnection &conn) {

	return (SQLDialect::propTypeJsonb(conn.getKey(), getField( tableName ,  fieldPos, conn).proprietaryType)) ;
}


bool dbSchemaMetaDataProvider::isClobOrBlob(std::string const & tableName, size_t fieldPos, dbConnection &conn) // PRQA S 4020
{
	dbSpecificColumnType::Type t = getFieldType(tableName , fieldPos, conn) ;
	switch(t) // wich type ?
	{
	    case dbSpecificColumnType::BLOB:
	    case dbSpecificColumnType::CLOB:
        {
		    return true ; // correct one: is a clob or blob   
	    }
        default:
        {
		    return SQLDialect::propTypeBlobClob(conn.getKey(), getField( tableName ,  fieldPos, conn).proprietaryType);
        }
	}
}

bool dbSchemaMetaDataProvider::isUploadableObject(std::string const & tableName, size_t fieldPos, dbConnection &conn) // PRQA S 4020
{
	// Return true if it is JSONB
	if ( true == isJsonb(tableName , fieldPos, conn) ) { return true ; }

	// Return true if it is CLOB or BLOB
	if (true == isClobOrBlob(tableName, fieldPos, conn) ) { return true ; }
	
	return false ; 
}


bool dbSchemaMetaDataProvider::isForeignKey(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	return ( not getField( tableName ,  fieldPos, conn).foreignTable.empty() );
}



std::string dbSchemaMetaDataProvider::getForeignTable(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	if( ! isForeignKey ( tableName, fieldPos, conn )) // not a foreign key
	{
		std::ostringstream errorMessage ;
		errorMessage << "Table \""<< tableName << "\" has not a foreign key at position " << fieldPos ;
		ACS_THROW(MetadataError(errorMessage.str())) ;// PRQA S 3081
	}
	return getField( tableName ,  fieldPos, conn).foreignTable ;
}



unsigned long dbSchemaMetaDataProvider::getForeignField(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	if( ! isForeignKey ( tableName, fieldPos, conn )) // not a foreign key
	{
		std::ostringstream errorMessage ;
		errorMessage << "Table \""<< tableName << "\" has not a foreign key at position " << fieldPos ;
		ACS_THROW(MetadataError(errorMessage.str())) ;// PRQA S 3081
	}

	return getField( tableName ,  fieldPos, conn).foreignField ;
}


dbSpecificColumnType::Type dbSchemaMetaDataProvider::getFieldType (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getFieldType (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

std::string dbSchemaMetaDataProvider::getFieldProprietaryType (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getFieldProprietaryType (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

dbGenericColumnType::Type dbSchemaMetaDataProvider::getGenericFieldType (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getGenericFieldType (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

dbDateTimeMode::Type dbSchemaMetaDataProvider::getDateTimeMode (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getDateTimeMode (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

unsigned long dbSchemaMetaDataProvider::getFieldSize (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getFieldSize (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

std::string dbSchemaMetaDataProvider::getFieldLabel (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getFieldLabel (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isPrimaryKey (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isPrimaryKey (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isUnique (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isUnique (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::notNull (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return notNull (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isSerial (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isSerial (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::hasDefault (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return hasDefault( tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isClobOrBlob (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isClobOrBlob (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isUploadableObject (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isUploadableObject (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

bool dbSchemaMetaDataProvider::isForeignKey (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return isForeignKey (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

std::string dbSchemaMetaDataProvider::getForeignTable (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getForeignTable (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}

unsigned long dbSchemaMetaDataProvider::getForeignField (std::string const & tableName, std::string const & fieldName, dbConnection &conn) 
{
	return getForeignField (tableName , getFieldFromName(tableName , fieldName, conn), conn ) ;
}


size_t dbSchemaMetaDataProvider::getFieldFromName(std::string const & tableName, std::string const & fieldName, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "tableName:" << tableName << " fieldName:" << fieldName) ;
	
	string convertedFieldName = SQLDialectFactory::sqlDialect(conn.getKey()).translateColumnName(fieldName);
	convertedFieldName = SQLDialect::convertCase(conn.getKey(), convertedFieldName) ;

	ThreadSafe::Lock lock(_mutex) ;
	TableMetaData const & table( getTable(tableName, conn) ) ;
	
	
	
	for(size_t i=0 ; i<table.fields.size() ; i++ )
	{
		if(convertedFieldName == table.fields[i].fieldName) { // found ?
			return i ; // return its pos
        }
	}

	ACS_THROW(FieldNotPresent("Cannot find field position for name \"" + // PRQA S 3081
								   convertedFieldName   + 
								   "\" in table \"" + 
								   tableName + 
								   "\"" )
				  ) ;
}



size_t dbSchemaMetaDataProvider::getFieldFromLabel(std::string const & tableName, std::string const & fieldLabel, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	TableMetaData const & table( getTable(tableName, conn) ) ;
	for(size_t i=0 ; i< table.fields.size() ; i++ )
	{
		if(fieldLabel == table.fields[i].fieldLabel) { // found ?
			return i ; // return its pos
        }
	}

	ACS_THROW(FieldNotPresent("Cannot find field position for label \"" + // PRQA S 3081
								   fieldLabel  + 
								   "\" in table \"" + 
								   tableName + 
								   "\"" )
				  ) ;
}



bool dbSchemaMetaDataProvider::hasFieldWithName	(std::string const & tableName, std::string const & fieldName, dbConnection &conn) // PRQA S 4020
{
	string convertedFieldName = SQLDialectFactory::sqlDialect(conn.getKey()).translateColumnName(fieldName);
	convertedFieldName = SQLDialect::convertCase(conn.getKey(), convertedFieldName) ;

	ThreadSafe::Lock lock(_mutex) ;
	TableMetaData const & table( getTable(tableName, conn) ) ;
	for(size_t i=0 ; i<table.fields.size() ; i++ )
	{
		if(convertedFieldName == table.fields[i].fieldName) { // found ?
			return true ; // ok has the field
        }
	}

	return false ; // no field with that name
}


bool dbSchemaMetaDataProvider::hasFieldWithLabel(std::string const & tableName, std::string const & fieldLabel, dbConnection &conn) // PRQA S 4020
{
	ThreadSafe::Lock lock(_mutex) ;
	TableMetaData const & table( getTable(tableName, conn) ) ;
	for(size_t i=0 ; i< table.fields.size() ; i++ )
	{
		if(fieldLabel == table.fields[i].fieldLabel) { // found ?
			return true ; // ok has the field
        }
	}

	return false ; // no field with that label
}

using std::map ; 
using std::vector ;
using std::pair ; 

map<string , vector< pair < string , string> > > dbSchemaMetaDataProvider::getDependantTables(std::string const & tableName, dbConnection &conn) 
{
	// discover dependant tables
	map<string , vector< pair < string , string> > > res ;
	try
	{
#if __cplusplus > 199711L
        std::unique_ptr<dbConnectionWrapper> cwguard(nullptr) ; // PRQA S 3081
#else
        std::auto_ptr<dbConnectionWrapper> cwguard(0) ; // PRQA S 3081
#endif
 		// the query class
		dbQuery tableMetadataQuery ( conn, SQLString::dependantTablesQuery(conn.getKey(), tableName), "MPSEL DEP" + tableName ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes specific dependant tables query: \"" 
				<< (SQLString::dependantTablesQuery(conn.getKey(), tableName)).getSQL() << "\"") ;

		dbSet tableMetadataSet ;
		
		// execute
		tableMetadataQuery.execForSet(tableMetadataSet) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" has " << tableMetadataSet.size() << " fields.") ;

		dbRow row( conn.getKey() ) ;
		while( tableMetadataSet.getNextRow(row))
		{
			string childTable ;
			row.column(0, childTable) ; // a child table
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" is parent of table \"" << childTable << "\".") ;
			
			vector<pair<string , string > > assoc ;
			vector<size_t> childFields = getForeignKeys(childTable, conn) ;
            size_t cfsize = childFields.size() ;
			for(size_t cc = 0 ; cc < cfsize ; ++cc) // fill the association
			{
				if (StringUtils::equalsNoCase( getForeignTable(childTable , childFields[cc], conn ) , tableName) )
				{
					unsigned long parentField =	getForeignField	(childTable , childFields[cc], conn ) ;

					assoc.push_back ( pair<string, string>(
					                        getFieldName(childTable , childFields[cc], conn), 
											getFieldName(tableName , parentField, conn) 
									) ) ;
				}
			}
			
			// and put them in the result
			res[childTable] = assoc ;
		}		

	}
	catch(std::exception &e)
	{
		MetadataError ee(e, "Error while attempting to get dependant tables for table \""+tableName+"\".") ;
		ACS_THROW(ee) ;
	}

	// finally return the result
	return res ;
}

bool dbSchemaMetaDataProvider::existIndex (const string &tableName, const vector<string> &fieldList, dbConnection & conn)
{
	string internalTableName = SQLDialect::convertCase(conn.getKey(), tableName) ;
	set<string> indexSet;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to check Index for table \"" << internalTableName << "\""); 

	// Create a set of the given fields
	for (size_t idx=0; idx<fieldList.size(); ++idx)
	{
		string internalFieldName = SQLDialect::convertCase(conn.getKey(), fieldList.at(idx) ) ;
		internalFieldName= SQLDialectFactory::sqlDialect(conn.getKey()).translateColumnName(internalFieldName) ;
		indexSet.insert (internalFieldName);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding the column \"" << internalFieldName << "\" to the required index set"); 
	}

	ThreadSafe::Lock lock(_mutex) ;
	// Load indexes if needed
	retrieveTableIndexes (internalTableName, conn);
	
	// Check if the index exists or not
	bool retval = ( _indexes[internalTableName].find(indexSet) != _indexes[internalTableName].end() );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The requestd index " << (retval?"exists" : "does not exist")); 
	return retval;
}

bool dbSchemaMetaDataProvider::hasIndex (const string &tableName, const string &fieldName, dbConnection & conn) 
{
	// Use the vector implementation
	vector<string> v;
	v.push_back (fieldName);
	return hasIndex (tableName, v, conn);
}

bool dbSchemaMetaDataProvider::hasIndex (const string &tableName, const vector<string> &fieldList, dbConnection & conn) 
{
	// Use the internal operation 
	return dbSchemaMetaDataProvider::instance(conn.getKey())->existIndex(tableName, fieldList, conn) ;
}

bool dbSchemaMetaDataProvider::existTable(const string &tableName, dbConnection &conn) // PRQA S 4020
{
	try {
		dbSchemaMetaDataProvider::instance(conn.getKey())->fieldsNumber(tableName, conn) ;
		return true ; // table exists	
	}
	catch(TableNotPresent &)
	{
		return false ; // doesn't exixts
	}
}


bool dbSchemaMetaDataProvider::hasField(const string &tableName, const string &fieldName, dbConnection &conn) // PRQA S 4020
{
	try {
		dbSchemaMetaDataProvider::instance(conn.getKey())->getFieldType(tableName, fieldName, conn) ;
		return true ; // field is present	
	}
	catch(FieldNotPresent &)
	{
		return false ; // not present
	}
}
	

dbSchemaMetaDataProvider::dbSchemaMetaDataProvider(const string& key) :
	ParamConstructibleSingletonMap<dbSchemaMetaDataProvider, std::string>(),
	_tables(),
	_key(key),
	_indexes(),
    _postGisVersions(),
    _mutex()
{
}

void dbSchemaMetaDataProvider::retrieveTableIndexes (string const & tableName, dbConnection & conn) // PRQA S 4020
{
	// Note assume case convertion already performed.
	ThreadSafe::Lock lock(_mutex) ;
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	// table found (already checked) ... return
	if( _indexes.find(tableName) != _indexes.end() ) {
		return ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Indexes fot table \""<<tableName<<"\" must be retrieved"
            << " connection is: " << &conn) ;
	
    const string &dbKey = conn.getKey() ;
	try
	{
		//Perform here a first query on system tables
		// Next call will ask the database dialect to generate the poper SQL query
		dbQuery theQuery (conn, SQLString::tableIndexesQuery(dbKey, tableName), "MPSEL IDX " + tableName ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes specific index query: \"" 
				<< (SQLString::metadataQuery(dbKey, tableName)).getSQL() << "\"") ;

		dbSet resultSet ;
		theQuery.execForSet(resultSet) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" has " << resultSet.size() << " indexes.") ;

		// No result does not mean the table does not exists
		// It is possible to create a table without primary keys and indexes

		// Prepare an empty set
		dbSchemaMetaDataProvider::IndexSet theIndexSet;

		// Need to prepare a reverse map to store the index data
		map< string, set<string> > reverseIndexMap;

		// Start looping on the result
		dbRow theRow(dbKey);
		while ( resultSet.getNextRow (theRow) )
		{
			// Each row contains the index name and the related column
			string idxname, idxcol;
			theRow.column (0, idxname);
			theRow.column (1, idxcol);
			
			// Convert in the database dependent case
			idxname = SQLDialect::convertCase(conn.getKey(), idxname ) ;
			idxcol  = SQLDialect::convertCase(conn.getKey(), idxcol ) ;
			// Manage special column names
			idxcol = SQLDialectFactory::sqlDialect(conn.getKey()).translateColumnName(idxcol) ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding index (" << idxname << ", " << idxcol << ") to table \""<<tableName<<"\"") ;
			reverseIndexMap[idxname].insert(idxcol);
		}

		// If the reverse map is not empty populate the index set structure
		for (map< string, set<string> >::const_iterator it = reverseIndexMap.begin(); it != reverseIndexMap.end(); ++it)
		{
			theIndexSet [ it->second ] = it->first;
		}

		// Store the result in the persistent in-memory data structure
		_indexes[tableName] = theIndexSet;
	} //try
	catch(std::exception &e)
	{
		ACS_THROW(MetadataError(e, "Error getting indexes for table \""+tableName+"\".")) ;
	}
	
}

void dbSchemaMetaDataProvider::checkTable(std::string const & tableName, dbConnection &conn) // PRQA S 4020
{
	// Note assume case convertion already performed.
	ThreadSafe::Lock lock(_mutex) ;
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	// table found (already checked) ... return
	if( _tables.find(tableName) != _tables.end() ) {
		return ;	
    }
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" must be loaded"
            << " connection is: " << &conn) ;
	
    const string &dbKey = conn.getKey() ;
	try
	{
		TableMetaData newTable ;
		newTable.tableName=tableName ;
		newTable.hasIdField=false ; //this will be checked later
		
#warning Tables Label still to be implemented.
		newTable.tableLabel=tableName ;

		//**** Here the table metada are loaded from the db *******

		//Perform here a first query on system tables
		dbQuery tableMetadataQuery (conn, SQLString::metadataQuery(dbKey, tableName), "MPSEL MTD " + tableName ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes specific metadata query: \"" 
				<< (SQLString::metadataQuery(dbKey, tableName)).getSQL() << "\"") ;


		dbSet tableMetadataSet ;
		tableMetadataQuery.execForSet(tableMetadataSet) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" has " << tableMetadataSet.size() << " fields.") ;

		

		if(tableMetadataSet.empty()) // error: no result
		{
			std::ostringstream errorMessage;
			errorMessage << "Table \""<< tableName << "\" is not present in db or it seems to have 0 fields." ;
			TableNotPresent e(errorMessage.str() );
			ACS_THROW(e) ;
		}

		std::vector <std::string> dbType;
        size_t rcount = tableMetadataSet.size() ; 
		for(size_t i=0 ; i<rcount ; ++i) // each record
		{
		
			FieldMetaData newField ;
			dbRow fieldMetadataRow( dbKey ) ;
			if( ! tableMetadataSet.getRow( (i+1) , fieldMetadataRow) ) // error, no record but should be // PRQA S 3000, 3084
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", error in obtaining row n " << i ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}

			
			if( fieldMetadataRow.columns() < 14 ) // expected row size is too short // PRQA S 3000, 4400
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", TableMetadataQuery result has not enough columns: " << fieldMetadataRow.columns() ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}

			//column 0 should be the position let's check it....
			size_t position = -1 ; // PRQA S 2890
			fieldMetadataRow.column(0 , position ) ;
			if(position != i) // check the position // PRQA S 3000
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", Record n " << i <<" seems to correspond to field in position "<< position ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}

			bool nullValue = false ;
			string fieldNameWithOriginalCase ;
			fieldMetadataRow.column(1 , fieldNameWithOriginalCase ) ;
			newField.fieldName = SQLDialectFactory::sqlDialect(dbKey).translateColumnName(fieldNameWithOriginalCase) ;
			newField.fieldName= SQLDialect::convertCase(dbKey, newField.fieldName) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " name is " << newField.fieldName) ;
 
			//NOTE: column 2 is the type, but in proprietary format
			std::string dbTypeStr;
			fieldMetadataRow.column(2 ,  dbTypeStr); // PRQA S 4400
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " proprietaryType is " << dbTypeStr) ;
			newField.proprietaryType = dbTypeStr ;
			dbType.push_back(dbTypeStr);
			
			
			fieldMetadataRow.column(3  , newField.fieldSize ) ;  //This can also be obtained from driver // PRQA S 4400
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " fieldSize is " << newField.fieldSize) ;
			
			{
				string dateTimeRangeFrom;
                string dateTimeRangeTo ;
				fieldMetadataRow.column(4  , dateTimeRangeFrom, &nullValue) ; // PRQA S 4400
				if(!nullValue) {
					 fieldMetadataRow.column(5  , dateTimeRangeTo,   &nullValue ) ;   // PRQA S 4400
                }
				if(!nullValue) {
					newField.dateTimeMode = evaluateDateTimeMode (dateTimeRangeFrom , dateTimeRangeTo) ;
                }
				else {
					newField.dateTimeMode = dbDateTimeMode::UNKNOWN ;
                }
			}
			
			std::string isPk;
			fieldMetadataRow.column(6  , isPk ) ;   // PRQA S 4400
			if(isPk == "T") { // primary key ?
				newField.primaryKey = true ;
            }
			else if (isPk == "F") {
				newField.primaryKey = false ;
            }
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", is PK field is \"" << isPk << "\"." ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " IS PK is " << isPk) ;

			std::string isNotNull;
			fieldMetadataRow.column(7  , isNotNull ) ; // PRQA S 4400
			if(isNotNull == "T") { // allow nulls ?
				newField.notNull = true ;
            }
			else if (isNotNull == "F") {
				newField.notNull = false ;
            }
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", is Not Null field is \"" << isNotNull << "\"." ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " IS NOT NULL is " << isNotNull) ;


			std::string isSerialString;
			fieldMetadataRow.column(8  , isSerialString , &nullValue) ; // PRQA S 4400
			if(isSerialString == "T") { // serial ? 
				newField.isSerial = true ;
            }
			else if (isSerialString == "F") {
				newField.isSerial = false ;
            }
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", serial field is \"" << isSerialString << "\"." ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " IS Serial is " << isSerialString) ;

			std::string hasDefaultString;
			fieldMetadataRow.column(9  , hasDefaultString , &nullValue) ; // PRQA S 4400
			if(hasDefaultString == "T") {// default ?
				newField.hasDefault = true ;
            }
			else if (hasDefaultString == "F") {
				newField.hasDefault = false ;
            }
			else
			{
				std::ostringstream errorMessage;
				errorMessage << "Table \""<< tableName << "\", default field is \"" << hasDefaultString << "\"." ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " HAS DEFAULT IS " << hasDefaultString) ;

			fieldMetadataRow.column(10   , newField.foreignTable ,  &nullValue) ; // PRQA S 4400 2
			fieldMetadataRow.column(11  , newField.foreignField , &nullValue) ; 


			if( fieldMetadataRow.columns() < 15 ) // it does not contain uniqueness
			{
                newField.isUnique = false ;
			}
            else {
			    std::string isUnique;
			    fieldMetadataRow.column(14  , isUnique ) ;   // PRQA S 4400
			    if(isUnique == "T") { // primary key ?
				    newField.isUnique = true ;
                 }
			     else if (isPk == "F") {
				     newField.isUnique = false ;
                 }
			     else
			     {
				     std::ostringstream errorMessage;
				     errorMessage << "Table \""<< tableName << "\", isUnique field is \"" << isUnique << "\"." ;
				     ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			     }
            }

			newTable.fields.push_back(newField) ;
		} //for(long i=0 ; i<tableMetadataSet.size() ; i++)

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" tableMetadataQuery completed") ;

		// NOW get some info from dbexpress Cursor class using a query on the table.
		// Now this is used  to get fields types 
		SQLString fakeTableQueryString(dbKey, SQLString::SELECT) ;
        size_t ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i) {
			string fakeSelect ;
			
			fakeSelect = tableName + "." + newTable.fields[i].fieldName ;
			fakeTableQueryString.addSelectList(fakeSelect ) ;
		}
		fakeTableQueryString.addTableList(tableName) ;
		fakeTableQueryString.addANDWhereParameter("1=-1") ; //fake condition to have an empty result set

		dbQuery fakeTableQuery(conn, fakeTableQueryString, "MPSEL TYP " + tableName ) ;

        dbSet set ;
	    fakeTableQuery.execForSet(set, vector<dbSpecificColumnType::Type>()) ;
		dbRow returnedFields(dbKey) ;
        set.getEmptyRow(returnedFields) ; // types only 

		uint16_t rfcols = returnedFields.columns() ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "rfcols: "<<rfcols) ;
        for(uint16_t i=0 ; i< rfcols ; ++i) // discover fields type
		{
			newTable.fields[i].type = SQLDialect::guessType(dbKey, returnedFields.getColumnType(i), dbType[i]);
 		}

		//Check if DateTime formats are OK
		rfcols = returnedFields.columns() ;
        for(size_t i=0 ; i< rfcols ; ++i)
		{
			if((newTable.fields[i].type == dbSpecificColumnType::DATETIME) && 
               (newTable.fields[i].dateTimeMode == dbDateTimeMode::UNKNOWN) )
			{
				// wrong date format
				std::ostringstream errorMessage;
				errorMessage << "Unresolved datetime mode in table \"" <<tableName<< "\" field " << i ;
				ACS_THROW(MetadataError(errorMessage.str() )) ; // PRQA S 3081
			}
		}		
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "table \""<<tableName<<"\" FakeQuery completed") ;



#warning Field Labels still to be implemented.
        ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i) {
			newTable.fields[i].fieldLabel =  newTable.fields[i].fieldName ;
		}


		//******** Now some support structures are filled *********			
		//Load Primary keys vector
        ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			if( newTable.fields[i].primaryKey)
			{
				newTable.primaryKeys.push_back(i)	 ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " is PK") ;
			}
		}


		//Load Foreign keys vector
		ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			if( not newTable.fields[i].foreignTable.empty())
			{
				newTable.foreignKeys.push_back(i)	 ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " is FK") ;
			}
		}


		ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			// an updatable field must not be a primarykey neither a clob/blob
			// for the latter 2 there are specific method to update them
			if( !( newTable.fields[i].primaryKey)) {
// MP: clob/blobs can be set to null so they are technically "updatable"            
//			       || (newTable.fields[i].type == dbSpecificColumnType::BLOB) )) {
				newTable.updatableKeys.push_back(i)	 ;
            }
		}

		//Search for Id
		if(newTable.primaryKeys.size() == 1 )
		{
			dbGenericColumnType::Type t = dbColumnTypeHelper::convertTypeToGeneric ( newTable.fields[ newTable.primaryKeys[0] ].type );
			switch(t)
			{
			case dbGenericColumnType::INTEGER:
                {
					newTable.hasIdField=true ;
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" has an ID at position  " << newTable.primaryKeys[0]) ; 
				    break ;
                }

			default: { break ; }
			}
		} 

		//Search for serial 
		newTable.hasSerialField=false ;
		ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			if(newTable.fields[i].isSerial)
			{
				// Check if another serial field has already been found
				// if so throw a nice exception
				if(newTable.hasSerialField) 
				{
					std::ostringstream  errorMessage;
					errorMessage << "Table has two fields of type serial: field n " <<  newTable.serialField << " and field n. " << i ;
					MetadataError e( errorMessage.str() ) ;
					ACS_THROW(e) ;
			    }
				newTable.serialField = i ;
				newTable.hasSerialField=true ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" has a Serial at position  " << i)  ;
			}
		} 

		//Search for default fields 
		newTable.hasDefaultField=false ;
		ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
        {
			if(newTable.fields[i].hasDefault)
			{
				newTable.defaultFields.push_back(i)	 ;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" field " << i << " has default value") ;
			}
		}
		if( ! newTable.defaultFields.empty() ) {
			newTable.hasDefaultField=true ;
        }

		//Prepare fieldsString	
		ntfsize = newTable.fields.size() ;
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			newTable.fieldsString += tableName + "." + newTable.fields[i].fieldName ;
			if(i != (newTable.fields.size() -1) ) { // PRQA S 3084
				newTable.fieldsString += ", ";
            }
		}

		//Prepare selectFieldsString	
		for(size_t i=0 ; i< ntfsize ; ++i )
		{
			newTable.selectFieldsString += SQLDialect::convertColumnToString(dbKey,tableName + "." + newTable.fields[i].fieldName, newTable.fields[i].type) ;
			if(i != (newTable.fields.size() -1) ) { // PRQA S 3084
				newTable.selectFieldsString += ", ";
            }
		}

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "table \""<<tableName<<"\" About to load new table into map ... " 
			    << " connection was: " << &conn) ;


		//Finally Set the table into the map
		_tables[tableName]=newTable ;
	} //try
	catch(TableNotPresent &) {
		throw ;
	}
	catch(std::exception &e)
	{
		ACS_THROW(MetadataError(e, "Error getting metadata for table \""+tableName+"\".")) ;
	}
	
}

dbSchemaMetaDataProvider::TableMetaData const & dbSchemaMetaDataProvider::getTable(std::string const & tableName, dbConnection &conn)
{
	string convertedTableName = SQLDialect::convertCase(conn.getKey(), tableName) ;
	ThreadSafe::Lock lock(_mutex) ;
	checkTable(convertedTableName, conn) ;
	return _tables[convertedTableName];
}

dbSchemaMetaDataProvider::FieldMetaData const & dbSchemaMetaDataProvider::getField(std::string const & tableName, size_t fieldPos, dbConnection &conn)
{
	ThreadSafe::Lock lock(_mutex) ;
	dbSchemaMetaDataProvider::TableMetaData const & table( getTable(tableName, conn) ) ;
	if( fieldPos >=table.fields.size() ) // check boundaries
	{
		std::ostringstream os ;
		os<<"Field n " << fieldPos << " excess array dimension in table \"" << tableName << "\"" ;
		ACS_THROW(FieldNotPresent(os.str() )) ; // PRQA S 3081
	}
	return table.fields[fieldPos] ;
}



dbDateTimeMode::Type dbSchemaMetaDataProvider::evaluateDateTimeMode(std::string const & dateTimeRangeFrom, std::string const & dateTimeRangeTo ) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "dateTimeRangeFrom \""<<dateTimeRangeFrom
			    << "\"; dateTimeRangeTo \""<<dateTimeRangeTo << "\".") ;

	// which date time mode (from, to ) ?
	// string comparison chain unavoidable
	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="YEAR" ) {
		return dbDateTimeMode::Year;
    }
		
	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="MONTH" ) {
		return dbDateTimeMode::YearToMonth;
    }

	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="DAY" ) {
		return dbDateTimeMode::YearToDay;
    }

	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="SECONDS" ) {
		return dbDateTimeMode::YearToSecond;
    }

	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="FRACTION(3)" ) {
        unsigned short dgts = DateTime::getDefaultDbFractionDigits() ;
        if (dgts > 3 ) { // PRQA S 3000, 4400
		    return dbDateTimeMode::YearToFraction3;
        }
        return dbDateTimeMode::FractionDecimalDigits(dgts) ;
    }

	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="FRACTION(5)" ) {
        unsigned short dgts = DateTime::getDefaultDbFractionDigits() ;
        if (dgts > 5 ) { // PRQA S 3000, 4400
		    return dbDateTimeMode::YearToFraction5 ;
        }
        return dbDateTimeMode::FractionDecimalDigits(dgts) ;
    }
	
	if( dateTimeRangeFrom=="YEAR" && dateTimeRangeTo=="FRACTION(6)" ) {
        unsigned short dgts = DateTime::getDefaultDbFractionDigits() ;
        if (dgts > 5 ) { // PRQA S 3000, 4400
		    return dbDateTimeMode::YearToFraction6 ;
        }
        return dbDateTimeMode::FractionDecimalDigits(dgts) ;
    }

	return dbDateTimeMode::UNKNOWN ;
}


// FieldMetaData
dbSchemaMetaDataProvider::FieldMetaData::FieldMetaData() :
	fieldName(),
	fieldLabel(),
	type(),
	proprietaryType(),
	fieldSize(),
	dateTimeMode(),
	primaryKey(),
	notNull(),
	isSerial(),
	hasDefault(),
	foreignTable(),
	foreignField(),
    isUnique()


{}

dbSchemaMetaDataProvider::FieldMetaData::~FieldMetaData() throw() {}
dbSchemaMetaDataProvider::FieldMetaData::FieldMetaData(dbSchemaMetaDataProvider::FieldMetaData const &rhs) :
	fieldName(rhs.fieldName),
	fieldLabel(rhs.fieldLabel),
	type(rhs.type),
	proprietaryType(rhs.proprietaryType),
	fieldSize(rhs.fieldSize),
	dateTimeMode(rhs.dateTimeMode),
	primaryKey(rhs.primaryKey),
	notNull(rhs.notNull),
	isSerial(rhs.isSerial),
	hasDefault(rhs.hasDefault),
	foreignTable(rhs.foreignTable),
	foreignField(rhs.foreignField),
    isUnique(rhs.isUnique)

{
	// empty
}


dbSchemaMetaDataProvider::FieldMetaData const & dbSchemaMetaDataProvider::FieldMetaData::operator=(FieldMetaData const & rhs )
{
	if(this != &rhs) { // avoid auto assignation

		// deep copy
		fieldName 		= rhs.fieldName ;
		fieldLabel  	= rhs.fieldLabel ;
		type			= rhs.type ;
		proprietaryType	= rhs.proprietaryType ;
		fieldSize 		= rhs.fieldSize ;
		dateTimeMode	= rhs.dateTimeMode ;
		primaryKey  	= rhs.primaryKey ;
		notNull 		= rhs.notNull ;
		isSerial 		= rhs.isSerial ;
		hasDefault 		= rhs.hasDefault;
		foreignTable	= rhs.foreignTable ;
		foreignField	= rhs.foreignField ;
		isUnique	    = rhs.isUnique ;
	}
	return *this ;
}


// TableMetaData
dbSchemaMetaDataProvider::TableMetaData::TableMetaData() :
	tableName(),
	tableLabel(),
	fieldsString(),
	selectFieldsString(),
	fields(),
	hasIdField(),
	hasSerialField(),
	hasDefaultField(),
	serialField(),
	primaryKeys(),
	foreignKeys(),
	updatableKeys(),
	defaultFields()
{}

dbSchemaMetaDataProvider::TableMetaData::~TableMetaData() throw ()
{}

dbSchemaMetaDataProvider::TableMetaData::TableMetaData(dbSchemaMetaDataProvider::TableMetaData const &rhs) :
	tableName(rhs.tableName),
	tableLabel(rhs.tableLabel),
	fieldsString(rhs.fieldsString),
	selectFieldsString(rhs.selectFieldsString),
	fields(rhs.fields),
	hasIdField(rhs.hasIdField),
	hasSerialField(rhs.hasSerialField),
	hasDefaultField(rhs.hasDefaultField),
	serialField(rhs.serialField),
	primaryKeys(rhs.primaryKeys),
	foreignKeys(rhs.foreignKeys),
	updatableKeys(rhs.updatableKeys),
	defaultFields(rhs.defaultFields)
{
	// empty
}


dbSchemaMetaDataProvider::TableMetaData const & dbSchemaMetaDataProvider::TableMetaData::operator=(dbSchemaMetaDataProvider::TableMetaData const & rhs )
{
	if(this != &rhs) { // avoid auto assignation

		// deep copy
		tableName      = rhs.tableName ;
		tableLabel     = rhs.tableLabel ;
		fieldsString   = rhs.fieldsString ;
		selectFieldsString   = rhs.selectFieldsString ;
		fields 	       = rhs.fields ;
		hasIdField     = rhs.hasIdField;
		hasSerialField = rhs.hasSerialField ;
		hasDefaultField= rhs.hasDefaultField ;
		serialField    = rhs.serialField ;
		primaryKeys    = rhs.primaryKeys ;
		foreignKeys    = rhs.foreignKeys ;
		updatableKeys  = rhs.updatableKeys ;
		defaultFields  = rhs.defaultFields ;
	}
	return *this ;
}


_ACS_END_NAMESPACE

