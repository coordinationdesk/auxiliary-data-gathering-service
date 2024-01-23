// PRQA S 1050 EOF
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
	Revision 5.8  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.7  2016/03/15 09:21:06  marpas
	qa warnings fixed and minor changes
	
	Revision 5.6  2016/03/15 09:11:47  marpas
	moving into dialect (getLimitedQueryWrapped) the responsibility to provide its own wrapped sql string
	
	Revision 5.5  2015/09/30 09:00:03  enrcar
	EC:: extended to handle UNION queries
	
	Revision 5.4  2015/02/17 14:16:24  enrcar
	EC:: Added support (and public method setValuesList) to handle the Token NESTED_VALUES_LIST, that is a nested SELECT to provide the columns value of an INSERT
	
	Revision 5.3  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:55  marpas
	adopting libException 5.x standards
	
	Revision 4.14  2013/06/03 12:39:44  marpas
	coding best practices partially applied
	qa rules
	
	Revision 4.13  2013/05/15 11:18:26  marpas
	propTypeBlob and propTypeClob methods added along their virtual counterparts
	
	Revision 4.12  2013/05/14 18:05:30  marpas
	implementing PostGRES/PostGIS support: in case of PostGIS geography type, the column is translated into ST_AsText(column) in order to get properly parsed by dbCrow/dbColumn classes
	
	Revision 4.11  2013/02/27 09:59:15  marpas
	ql string limit vaues are now size_t
	
	Revision 4.10  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.9  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.8  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.7  2012/11/19 13:30:19  marpas
	HEAD REALIGNEMENT
	
	Revision 4.3  2012/04/02 11:12:26  marpas
	attempt to repeat queries - execForStatus - if some conditions are met into the db
	
	Revision 4.2  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.11  2011/03/31 15:47:43  lucio.pulvirenti
	method rowLimitInsideWhere turned into limitWrapsQuery
	
	Revision 3.10  2011/03/02 13:34:25  marpas
	serial8 support added
	
	Revision 3.9  2010/09/08 17:50:10  marpas
	debug improved, other general code improvements
	
	Revision 3.8  2010/04/29 10:44:31  marpas
	deal with special columntype (proprietary type e.g. Postgres TEXT)
	some specific type can be treated as blob/clob
	
	Revision 3.7  2010/03/31 16:38:32  lucio.pulvirenti
	EC:: method setColumnNameTraslationMap added
	
	Revision 3.6  2010/03/19 11:27:35  marpas
	class SQLDialectAppSpecific added to manage specific dialect issues for applications
	
	Revision 3.5  2010/03/17 14:02:35  enrcar
	ENRCAR:: executeSpecificCommand / executeSpecificCommandVirtual methods added
	MARPAS:: Finalize methods added
	
	Revision 3.4  2010/03/04 17:20:58  enrcar
	EC:: translateColumnName made recursive
	
	Revision 3.3  2010/02/26 16:07:26  crivig
	translateColumnName modified in order to handle tablename.columnName
	
	Revision 3.2  2010/02/10 17:19:13  enrcar
	EC:: LIMIT inside the WHERE clause (e.g. Oracle dialect): now handled
	
	Revision 3.1  2009/10/29 13:07:50  enrcar
	EC:: enum SQLCastType, class SQLCast added. Method translateSQLCast() added.
	
	Revision 3.0  2009/10/05 14:47:47  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.12  2009/06/16 13:09:21  marpas
	SQL join syntax implemented
	
	Revision 2.11  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.10  2008/12/09 16:11:23  marpas
	dbConv::Null implemented to allow setting Null values using SQLString
	
	Revision 2.9  2008/05/19 14:27:44  enrcar
	EC:: Support for custom procedures added
	
	Revision 2.8  2008/05/08 16:36:20  enrcar
	EC:: ColumnTraslationMap method handled
	
	Revision 2.7  2008/05/07 17:07:21  enrcar
	EC:: createEscapeSequence method added
	
	Revision 2.6  2008/04/16 13:51:18  enrcar
	EC:: template for custom functions added
	
	Revision 2.5  2007/12/11 13:12:51  enrcar
	EC:: returnExecForSet, returExecForStatus methods made pure-virtual since must be implemented into specific dialect
	
	Revision 2.4  2007/12/07 18:40:56  enrcar
	ENRCAR: Methods returnExecForSet, returnExecForStatus added
	
	Revision 2.3  2007/12/03 13:23:10  crivig
	modified in order to handle short int as bool for MySQL
	
	Revision 2.2  2006/12/13 18:13:10  paoscu
	Added methods to understand if a LO field is NULL.
	
	Revision 2.1  2006/04/03 16:37:06  enrcar
	Function template handled
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.19  2006/02/27 11:22:12  giucas
	setEnv & setFastConnect are now abstract const
	
	Revision 1.18  2005/12/22 18:47:00  marpas
	added FileDescriptorGuard class
	new concept about delimiters for strings implemented
	
	Revision 1.17  2005/11/30 15:43:04  enrcar
	interface fixed
	
	Revision 1.16  2005/11/25 16:27:53  paoscu
	Methods:
		static std::string convertCase(std::string const &)
		virtual std::string convertCaseFilter(std::string const &) const
	added.
	
	Revision 1.15  2005/10/10 12:44:45  enrcar
	InternalError exception added
	
	Revision 1.14  2005/09/28 09:18:12  enrcar
	Dialect IF changed to handle blobs
	
	Revision 1.13  2005/09/27 14:53:40  enrcar
	Exception added
	IF changed to handle blobs (temporary - still under dvl)
	
	Revision 1.12  2005/08/10 07:58:43  enrcar
	Interface changed: SQLFilterReturn() made pure-virtual.
	MUST BE IMPLEMENTED into specific-db dialect
	
	Revision 1.11  2005/07/20 15:24:31  enrcar
	DELETE template added
	
	Revision 1.10  2005/07/19 13:15:22  enrcar
	INSERT added
	Boolean operators handled
	
	Revision 1.9  2005/07/19 12:07:23  enrcar
	DROP Method handled
	
	Revision 1.8  2005/07/19 07:47:14  enrcar
	Added debug level
	Method translate renamed
	
	Revision 1.7  2005/07/18 08:20:53  enrcar
	UPDATE template added
	
	Revision 1.6  2005/07/06 14:47:30  enrcar
	Added token & command handling
	
	Revision 1.5  2005/06/09 08:38:13  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/05/05 14:59:09  enrcar
	Added filter for SQL driver returned values
	
	Revision 1.3  2005/04/19 08:16:10  marpas
	work in progress
	
	Revision 1.2  2005/04/18 19:42:27  marpas
	work in progress
	
	Revision 1.1  2005/04/18 16:37:33  marpas
	just added
	
	

*/


#include <SQLDialect.h>
#include <SQLDialectFactory.h>
#include <StringUtils.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(SQLDialect);

using std::string ; 

SQLDialect::SQLCast::SQLCast(SQLDialect::SQLCastType castType, const string &arg1, const string &arg2) :
	_castType(castType),
	_arg1(arg1),
	_arg2(arg2)
{ /* Empty c'tor */ }


SQLDialect::SQLDialect(const string &name) : 
    _sqlTokenMap(),
    _sqlJoinTokenMap(),
    _sqlOperatorMap(),
    _sqlCastTypeMap(),
    _columnTranslationMap(),
    _blobIsNullTemplate(),
    _deleteTemplate(),
    _downloadBlobTemplate(),
    _dropTemplate(),
    _functionTemplate(),
    _custFunctionTemplate(),
    _custProcedureTemplate(),
    _insertTemplate(),
    _selectTemplate(),
    _unionTemplate(),
    _updateTemplate(),
    _uploadBlobTemplate(),
    _dialectName(name)
{
}

SQLDialect::~SQLDialect() noexcept 
{
// PRQA S 4631 5
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

const string &SQLDialect::dialectName() const noexcept // PRQA S 4120
{
	return _dialectName ;
}

void SQLDialect::beginFinalize(dbSQLConnection &) const {}

void SQLDialect::commitFinalize(dbSQLConnection &) const {}

void SQLDialect::rollbackFinalize(dbSQLConnection &) const {}

std::vector<SQLDialect::SQLToken> const & SQLDialect::getBlobIsNullTemplate() const
{
	return _blobIsNullTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getDeleteTemplate() const
{
	return _deleteTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getDropTemplate() const
{
	return _dropTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getInsertTemplate() const
{
	return _insertTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getSelectTemplate() const
{
	return _selectTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getUnionTemplate() const
{
	return _unionTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getUpdateTemplate() const
{
	return _updateTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getBlobDownloadTemplate() const
{
	return _downloadBlobTemplate ;
}


std::vector<SQLDialect::SQLToken> const & SQLDialect::getBlobUploadTemplate() const
{
	return _uploadBlobTemplate ;
}

std::vector<SQLDialect::SQLToken> const & SQLDialect::getFunctionTemplate() const
{
	return _functionTemplate ;
}

std::vector<SQLDialect::SQLToken> const & SQLDialect::getCustFunctionTemplate() const
{
	return _custFunctionTemplate ;
}

std::vector<SQLDialect::SQLToken> const & SQLDialect::getCustProcedureTemplate() const
{
	return _custProcedureTemplate ;
}


std::map<std::string, std::string> const & SQLDialect::getColumnNameTraslationMap() const 
{
	return _columnTranslationMap ;
}
	
std::map<std::string, std::string> & SQLDialect::getColumnNameTraslationMap()  
{	
	return _columnTranslationMap ; // PRQA S 4024
}

void SQLDialect::setColumnNameTraslationMap( std::map<std::string, std::string>& o )  
{	
	_columnTranslationMap = o ;
}
	

int SQLDialect::executeSpecificCommand(const string &key, void** v, unsigned char n, signed short int cmd) 
{
    return SQLDialectFactory::sqlDialect(key).executeSpecificCommandVirtual(v, n, cmd) ;
}


int SQLDialect::executeSpecificCommandVirtual(void** v, unsigned char n, signed short int cmd) const
{
    ACS_THROW(InternalErrorException("INTERNAL ERROR -- METHOD NOT DEFINED")) ; // PRQA S 3081
}


std::string SQLDialect::translateToken(SQLToken s, bool raiseException) const // PRQA S 4020
{
// W8115_OFF: multiple returns
    /**
        Setting raiseException to FALSE no exception will be returned if Token not found in map
        Useful for dumping values, also if no translation is available.
    */
	std::map<SQLToken, string const>::const_iterator it=_sqlTokenMap.find(s);
	if(it==_sqlTokenMap.end())
	{
		std::ostringstream errorMessage;
		errorMessage << "Cannot convert token " << s ;
        if (raiseException)
        {
		    ACS_THROW(TokenNotFoundException(errorMessage.str() )) ; // PRQA S 3081
        }
        else
        {
            return errorMessage.str() ;
        }
	}

	return it->second;

}


std::string SQLDialect::translateJoinToken(SQLJoinToken s, bool raiseException) const // PRQA S 4020
{
// W8115_OFF: multiple returns
    /**
        Setting raiseException to FALSE no exception will be returned if Token not found in map
        Useful for dumping values, also if no translation is available.
    */
	std::map<SQLJoinToken, string const>::const_iterator it=_sqlJoinTokenMap.find(s);
	if(it==_sqlJoinTokenMap.end())
	{
		std::ostringstream errorMessage;
		errorMessage << "Cannot convert join token " << s ;
        if (raiseException)
        {
		    ACS_THROW(TokenNotFoundException(errorMessage.str() )) ; // PRQA S 3081
        }
        else
        {
            return errorMessage.str() ;
        }
	}

	return it->second;

}

void SQLDialect::adjustGisOperatorMap() const {}

std::string SQLDialect::translateOperator(SQLOperator s, bool raiseException) const // PRQA S 4020
{
// W8115_OFF: multiple returns
   /**
        Setting raiseException to FALSE no exception will be returned if Operator not found in map
        Useful for dumping values, also if no translation is available.
    */
    adjustGisOperatorMap() ; // do not remove 
	std::map<SQLOperator, string const>::const_iterator it=_sqlOperatorMap.find(s);
	if(it==_sqlOperatorMap.end())
	{
	    std::ostringstream errorMessage;
	    errorMessage << "Cannot convert operator " << s ;
        if (raiseException)
        {
			ACS_THROW(OperatorNotFoundException(errorMessage.str() )) ; // PRQA S 3081
        }
        else
        {  
        	return errorMessage.str() ;
        }
	}

	return it->second;

}


string SQLDialect::translateColumnName(const string &s_in) const // PRQA S 4020, 4214
{
	
    // W8115_OFF: multiple returns
    /**
            Translate a ColumnName -- useful for handling reserved keyword.
            Example of translationMap entries:
            IN: resw -> OUT: "resw" 
     		IN: resw -> OUT: noresw (requires a different schema)
     */

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called: !" << s_in << "!") ;

    string s1 ;
    string s2 ;
    size_t pos = string::npos;
    
	/* Check if input is in the form "expr1, expr2 [...]" */
    if ( (pos = s_in.find(",")) != string::npos)
    {
    	s1=s_in.substr(0, pos) ;
        s2=s_in.substr(pos+1, s_in.length()) ; // PRQA S 3084
           
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Spliting input.. : !" << s1 << "!" << s2 << "!") ;

        return ( translateColumnName(s1) + ", " + translateColumnName(s2) ) ;	/* (!) RECURSIVE (!) */
    }
    
    /* IF HERE: A single expression is going to be processed */
    
    const string s = StringUtils::trim(s_in) ;	/* */
    string tabname ;
    string colname ;
 
    /* Check if input is in the form "table.column" */
    if ( (pos = s_in.find(".")) != string::npos)
    {
        tabname=s_in.substr(0, pos+1) ;	/* Including "." */ // PRQA S 3084 2
        colname=s_in.substr(pos+1, s_in.length()) ;
    }
    else
    {
    	tabname="" ;
    	colname=s ;
    }
    
	
    std::map <string, string>::const_iterator it = _columnTranslationMap.find( StringUtils::lowercase(colname) );

    if( it != _columnTranslationMap.end() ) 
    {
    	// Translation found !
    	string t = tabname + (it->second) ;
          
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returning: !" << t << "!") ;

        return t ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "No translation done: !" << s << "!") ;

	return s ;

}


bool SQLDialect::limitWrapsQuery() const
{ 
	return false ;	// Specific dialects might set to TRUE if limit should be processed in the where clause (e.g. ORACLE ...)
} 

string SQLDialect::getLimitedQueryWrapped(const std::string &, size_t  limit, size_t offset) const
{
    ACS_THROW(InternalErrorException("INTERNAL ERROR -- METHOD NOT DEFINED")) ; // PRQA S 3081
}


long SQLDialect::SQLReturn(const string& key, long v)  
{ return SQLDialectFactory::sqlDialect(key).SQLFilterReturn(v) ;  }

bool SQLDialect::shouldRepeatCommand(const std::string &key, const dbSQLConnection &conn, const dbSQLCommand &comm) 
{
    return SQLDialectFactory::sqlDialect(key).shouldRepeatCommand(conn, comm) ;
}


bool SQLDialect::shouldRepeatCommand(const dbSQLConnection &, const dbSQLCommand &) const
{
    return false ;
}


std::string SQLDialect::convertCase(const string& key, std::string const & s) {
	return SQLDialectFactory::sqlDialect(key).convertCaseFilter(s) ;
}

std::string SQLDialect::convertColumnToString(const string& key, const std::string &c, dbSpecificColumnType::Type const &dbType) 
{
	return SQLDialectFactory::sqlDialect(key).convertColumnToStringFilter(c, dbType) ;
}

dbSpecificColumnType::Type SQLDialect::guessType (const string& key, dbSpecificColumnType::Type const & type, std::string const & dbType) {
	return SQLDialectFactory::sqlDialect(key).guessTypeFilter(type, dbType) ;
}



bool SQLDialect::propTypeBlobClobFilter (std::string const & proprietarytype) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "defaulted to false") ;
	
	return false ;
	
}
bool SQLDialect::propTypeBlobClob (const string& key, std::string const & proprietarytype) 
{
	return SQLDialectFactory::sqlDialect(key).propTypeBlobClobFilter(proprietarytype) ;

}


bool SQLDialect::propTypeBlobFilter (std::string const & proprietarytype) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "defaulted to false") ;
	
	return false ;
	
}
bool SQLDialect::propTypeBlob (const string& key, std::string const & proprietarytype) 
{
	return SQLDialectFactory::sqlDialect(key).propTypeBlobFilter(proprietarytype) ;

}



bool SQLDialect::propTypeClobFilter (std::string const & proprietarytype) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "defaulted to false") ;
	
	return false ;
	
}
bool SQLDialect::propTypeClob (const string& key, std::string const & proprietarytype) 
{
	return SQLDialectFactory::sqlDialect(key).propTypeClobFilter(proprietarytype) ;

}


bool SQLDialect::propTypeJsonbFilter (std::string const & proprietarytype) const 
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "defaulted to false") ;
	
	return false ;
	
}
bool SQLDialect::propTypeJsonb (const string& key, std::string const & proprietarytype) 
{
	return SQLDialectFactory::sqlDialect(key).propTypeJsonbFilter(proprietarytype) ;

}


void SQLDialect::startListener (dbConnection *conn, const string& channel) const  
{
	/* This method should be re-implemented. The base class is not able to handle this. */
	ACS_THROW(exIllegalValueException("Not implemented: startListener")) ; 
}

bool SQLDialect::listenToMessages (dbConnection *conn, std::vector<std::string>& messages_V, const uint64_t& timeout_usec) const 
{
	/* This method should be re-implemented. The base class is not able to handle this. */
	ACS_THROW(exIllegalValueException("Not implemented: listenToMessages")) ; 
	return false ; 
}

string SQLDialect::gisVersionQuery() const { return "Gis Not Implemented" ; }


string SQLDialect::getLastSerial4(string const & t, dbConnection &conn) const 
{
	return getLastSerial(t, conn) ;
}

string SQLDialect::getLastSerial8(string const & t, dbConnection &conn) const 
{
	return getLastSerial(t, conn) ;
}





/* Convert a string (usually, a clob) in an escape-sequence */
string SQLDialect::createEscapeSequence(const string& s)
{
	return createEscapeSequence(s.c_str(), s.size()); 
}


/* Convert a string (usually, a clob) in an escape-sequence */
string SQLDialect::createEscapeSequence(const char* buffer, size_t l)
{
	std::ostringstream os ;
    os << std::oct ;

	for (size_t i=0; i<l; i++) // each char 
	{
        os << '\\' << std::setw(3) << std::setfill('0') << std::right << int(buffer[i]) ;
	}
	
	return os.str() ;
}

std::string SQLDialect::nullValue() const 
{
	return "NULL" ;
}

string toString(SQLDialect::SQLCastType t) // PRQA S 4020
{
    switch(t) {
    case SQLDialect::SQL_TYPEUNDEF: { return "SQL_TYPEUNDEF" ; }
    case SQLDialect::SQL_BOOLEAN: { return "SQL_BOOLEAN" ; }
    case SQLDialect::SQL_SMALLINT: { return "SQL_SMALLINT" ; }
    case SQLDialect::SQL_INTEGER: { return "SQL_INTEGER" ; }
    case SQLDialect::SQL_BIGINT: { return "SQL_BIGINT" ; }
    case SQLDialect::SQL_SERIAL: { return "SQL_SERIAL" ; }
    case SQLDialect::SQL_BIGSERIAL: { return "SQL_BIGSERIAL" ; }
    case SQLDialect::SQL_FLOAT: { return "SQL_FLOAT" ; }
    case SQLDialect::SQL_DOUBLE: { return "SQL_DOUBLE" ; }
    case SQLDialect::SQL_DECIMAL: { return "SQL_DECIMAL" ; }
    case SQLDialect::SQL_CHAR: { return "SQL_CHAR" ; }
    case SQLDialect::SQL_VARCHAR: { return "SQL_VARCHAR" ; }
    default:
        {
            std::ostringstream os ; 
            os << "unrecognized SQLCastType <" << int(t) << ">" ; // PRQA S 3081
            return os.str() ;
        }
    }
}


std::ostream & operator << (std::ostream &os, SQLDialect::SQLCastType t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, SQLDialect::SQLCastType t) {// PRQA S 4222
    return os << toString(t) ;
}



string toString(SQLDialect::SQLToken t)  // PRQA S 4020, 5500
{
// NB cyclomatic complexity is just slightly high, but is due to the switch control structure below
// any attemp to lower CC would increase complexity for maintenance

    switch(t) {
    case SQLDialect::TEMPLATE_INVALID: { return "TEMPLATE_INVALID" ; }  
    case SQLDialect::SELECT: { return "SELECT" ; }
    case SQLDialect::UPDATE: { return "UPDATE" ; }
    case SQLDialect::SET: { return "SET" ; }
    case SQLDialect::DROP: { return "DROP" ; }
    case SQLDialect::INSERT: { return "INSERT" ; }
    case SQLDialect::DELETE: { return "DELETE" ; }
    case SQLDialect::VALUES: { return "VALUES" ; }
    case SQLDialect::VALUES_LIST: { return "VALUES_LIST" ; }
    case SQLDialect::NESTED_VALUES_LIST: { return "NESTED_VALUES_LIST" ; }
    case SQLDialect::COLUMN_LIST: { return "COLUMN_LIST" ; }
    case SQLDialect::LIMIT: { return "LIMIT" ; }
    case SQLDialect::DISTINCT: { return "DISTINCT" ; }
    case SQLDialect::SELECT_LIST: { return "SELECT_LIST" ; }
    case SQLDialect::SET_LIST: { return "SET_LIST" ; }
    case SQLDialect::FROM: { return "FROM" ; }
    case SQLDialect::TABLE_LIST: { return "TABLE_LIST" ; }
    case SQLDialect::JOIN: { return "JOIN" ; }
    case SQLDialect::WHERE: { return "WHERE" ; }
    case SQLDialect::GROUPBY: { return "GROUPBY" ; }
    case SQLDialect::HAVING: { return "HAVING" ; }
    case SQLDialect::ORDERBY: { return "ORDERBY" ; }
    case SQLDialect::FILENAME: { return "FILENAME" ; }
    case SQLDialect::FUNCTIONARG: { return "FUNCTIONARG" ; }
    case SQLDialect::CUSTFUNCTIONARG: { return "CUSTFUNCTIONARG" ; }
    case SQLDialect::CUSTPROCEDUREARG: { return "CUSTPROCEDUREARG" ; }
    case SQLDialect::FUNCTIONNAME: { return "FUNCTIONNAME" ; }
    case SQLDialect::CUSTFUNCTIONNAME: { return "CUSTFUNCTIONNAME" ; }
    case SQLDialect::NESTEDFUNCTIONNAME: { return "NESTEDFUNCTIONNAME" ; }
    case SQLDialect::CUSTNESTEDFUNCTIONNAME: { return "CUSTNESTEDFUNCTIONNAME" ; }
    case SQLDialect::CUSTPROCEDURENAME: { return "CUSTPROCEDURENAME" ; }
    case SQLDialect::CUSTNESTEDPROCEDURENAME: { return "CUSTNESTEDPROCEDURENAME" ; }
    case SQLDialect::COLUMNNAME: { return "COLUMNNAME" ; }
    case SQLDialect::FORREADONLY: { return "FORREADONLY" ; }
    case SQLDialect::UNION: { return "UNION" ; }
    case SQLDialect::UNION_ALL: { return "UNION_ALL" ; }
    case SQLDialect::UNION_LIST: { return "UNION_LIST" ; }
    case SQLDialect::ON_CONFLICT_DO: { return "ON_CONFLICT_DO" ; }
    case SQLDialect::ON_CONFLICT_CNSR_DO: { return "ON_CONFLICT_CNSR_DO" ; }
    default:
        {
            std::ostringstream os ; 
            os << "unrecognized SQLToken <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, SQLDialect::SQLToken t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, SQLDialect::SQLToken t) {// PRQA S 4222
    return os << toString(t) ;
}


string toString(SQLDialect::SQLJoinToken t)  // PRQA S 4020
{
    switch(t) {
    case SQLDialect::CROSS: { return "CROSS" ; }  
    case SQLDialect::INNER: { return "INNER" ; }
    case SQLDialect::LEFT: { return "LEFT" ; }
    case SQLDialect::RIGHT: { return "RIGHT" ; }
    case SQLDialect::FULL: { return "FULL" ; }
    default:
        {
            std::ostringstream os ; 
            os << "unrecognized SQLJoinToken <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, SQLDialect::SQLJoinToken t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, SQLDialect::SQLJoinToken t) { // PRQA S 4222
    return os << toString(t) ;
}




string toString(SQLDialect::SQLOperator t)  // PRQA S 4020
{

    switch(t) {
    case SQLDialect::NONE: { return "NONE" ; }  
    case SQLDialect::EQUAL_TO: { return "EQUAL_TO" ; }
    case SQLDialect::LIKE: { return "LIKE" ; }
    case SQLDialect::MINOR_OF: { return "MINOR_OF" ; }
    case SQLDialect::MAJOR_OF: { return "MAJOR_OF" ; }
    case SQLDialect::MAJOR_OR_EQUAL_OF: { return "MAJOR_OR_EQUAL_OF" ; }
    case SQLDialect::MINOR_OR_EQUAL_OF: { return "MINOR_OR_EQUAL_OF" ; }
    case SQLDialect::DIFFERENT_FROM: { return "DIFFERENT_FROM" ; }
    case SQLDialect::BETWEEN: { return "BETWEEN" ; }
    case SQLDialect::NOT_BETWEEN: { return "NOT_BETWEEN" ; }
    case SQLDialect::INSIDE: { return "INSIDE" ; }
    case SQLDialect::INTERSECT: { return "INTERSECT" ; }
    case SQLDialect::OUTSIDE: { return "OUTSIDE" ; }
    case SQLDialect::ISNULL: { return "SEISNULLT_LIST" ; }
    case SQLDialect::ISNOTNULL: { return "ISNOTNULL" ; }
    case SQLDialect::IN: { return "IN" ; }
    default:
        {
            std::ostringstream os ; 
            os << "unrecognized SQLOperator <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, SQLDialect::SQLOperator t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, SQLDialect::SQLOperator t) {// PRQA S 4222
    return os << toString(t) ;
}


_ACS_END_NAMESPACE
