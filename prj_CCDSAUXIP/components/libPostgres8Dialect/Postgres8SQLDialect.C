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

	$Prod: A.C.S. libPostgres8Dialect $

	$Id$

	$Author$

	$Log$
	Revision 5.20  2016/10/07 09:39:04  marpas
	connect timeout now is honored - no longer default
	
	Revision 5.19  2016/09/20 16:12:11  enrcar
	CSGACQ-96: EC:: dialect-specific method getTableNameForEmptyFROM added in order to handle SELECT without FROM
	
	Revision 5.18  2016/09/01 14:24:07  marfav
	currantDateTime now returns a datetime without time zone
	
	Revision 5.17  2016/07/29 13:01:36  enrcar
	EC:: typo fixed
	
	Revision 5.16  2016/07/28 15:54:16  enrcar
	Method translateSQLCast made virtual (dialect-specific)
	
	Revision 5.15  2016/07/28 09:07:02  marpas
	adding casts to varchar because Texts are now mapped to HMemo and cannot be read as string any longer
	
	Revision 5.14  2016/07/26 15:20:03  enrcar
	CSGACQ-87  EC:: Added support for SQL CASE WHEN/ELSE
	
	Revision 5.13  2016/07/21 14:30:20  enrcar
	CSGACQ-82 EC:: Interface intervalToSeconds added
	
	Revision 5.12  2016/03/26 21:46:51  marpas
	enforcing use of connection reference getting last serial
	
	Revision 5.11  2016/03/14 14:56:03  marpas
	OFFSET support added
	
	Revision 5.10  2015/09/30 09:19:35  enrcar
	EC:: UNION query handled
	
	Revision 5.9  2015/03/19 14:43:48  marpas
	Robustness improved and useless conf now signaled
	
	Revision 5.8  2015/02/17 14:00:45  enrcar
	EC:: Added support for SQLDialect::NESTED_VALUES_LIST (used to INSERT the results of a nested query)
	
	Revision 5.7  2014/12/02 12:01:25  lucio.pulvirenti
	S2PDGS-908: getStringDelimiter() method removed: delimit() method implemented instead to delimit strings properly and escape delimiter inside strings.
	
	Revision 5.6  2014/04/08 16:38:48  marpas
	rounding mode useless with new DateTime 5.13
	
	Revision 5.5  2013/11/05 16:40:18  enrcar
	EC::: operator NOT_IN handled
	
	Revision 5.4  2013/10/03 18:06:15  marpas
	zero length download is in debug now
	
	Revision 5.3  2013/10/03 08:51:54  chicas
	Removed warning for empty clob
	
	Revision 5.2  2013/08/29 13:55:06  marfav
	Stored procedure invocation improved
	
	Revision 5.1  2013/08/28 14:30:32  marfav
	Custom function syntax fixed
	
	Revision 5.0  2013/06/18 09:49:23  marpas
	adoption of libException 5.x standards
	
	Revision 1.60  2013/05/30 11:58:10  marpas
	dbDriver is a singleton now
	
	Revision 1.59  2013/05/15 11:51:04  marpas
	propTypeClobFilter and propTypeBlobFilter spcific methods added
	
	Revision 1.58  2013/05/14 18:42:53  marpas
	PostGRES/PostGIS support
	
	Revision 1.57  2013/03/11 12:48:38  marpas
	FileGuard is now a template specialization
	
	Revision 1.56  2013/02/27 12:38:47  marpas
	coding best practices applied
	qa rules
	interface rationalization
	
	Revision 1.55  2013/02/18 13:19:43  chicas
	 0-length clobs were not handled; fixed;
	
	Revision 1.54  2012/11/21 10:10:22  marpas
	HEAD realignement
	
	Revision 1.46  2012/02/17 14:09:56  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 1.45  2012/02/13 15:27:48  marpas
	refactoring
	
	Revision 1.44  2011/12/23 11:49:59  lucio.pulvirenti
	getIsGoodDummyQuery now SELECT 1
	
	Revision 1.43  2011/10/20 13:45:52  lucio.pulvirenti
	Query labels now used
	
	Revision 1.42  2011/10/14 07:54:44  marpas
	FORREADONLY token implemented
	
	Revision 1.41  2011/09/12 12:37:10  marpas
	using filterable in nominal messages
	
	Revision 1.40  2011/07/07 09:49:16  micmaz
	REOPENED - issue S1PDGS-1600: CirculationAgent error message
	http://jira.acsys.it/browse/S1PDGS-1600
	
	Revision 1.39  2011/05/25 08:36:50  lucio.pulvirenti
	getIsGoodDummyQuery() method added
	
	Revision 1.38  2010/11/02 13:25:43  enrcar
	EC:: timeStampResolution taken from driver (if present)
	
	Revision 1.37  2010/10/26 08:41:21  enrcar
	EC:: always a 6-digits time fraction is returned (libacdbexppgl was modified accordingly)
	
	Revision 1.36  2010/10/12 14:02:35  marpas
	
	postgres includes are under libpq subdirectory
	
	Revision 1.35  2010/10/12 13:13:53  clanas
	modified to use pgsql public interfaces
	
	Revision 1.34  2010/09/11 18:16:22  marpas
	libDbClient 3.15 interface (dependent on libDbExpress 3.2)
	
	Revision 1.33  2010/08/19 09:45:40  marpas
	new method hasTimedWaitOnLock. To be used from dbQuery to lock the main methods
	
	Revision 1.32  2010/04/29 10:45:15  marpas
	some proprietary types can be treated as blob/clob - new method added
	
	Revision 1.31  2010/04/19 10:36:51  marpas
	executeSpecificCommandVirtual implemented
	
	Revision 1.30  2010/03/15 13:56:33  marpas
	beginTransaction method has transaction name as defaulted parameter
	
	Revision 1.29  2010/02/02 17:57:40  enrcar
	EC:: Added method: getStatementTerminator() { return ";" }
	
	Revision 1.28  2009/10/29 13:22:41  enrcar
	EC:: _sqlCastTypeMap added to dialect
	
	Revision 1.27  2009/09/29 15:11:30  marpas
	missing include added
	
	Revision 1.26  2009/07/07 16:52:36  marpas
	debug improved
	
	Revision 1.25  2009/06/22 12:43:49  marpas
	SQLDialect interface improved
	
	Revision 1.24  2009/06/19 18:00:02  marpas
	getLasSerial method slightly changed
	
	Revision 1.23  2009/06/16 13:17:11  marpas
	join syntax implemented
	
	Revision 1.22  2009/02/12 14:26:44  enrcar
	EC:: bug fixed. The whole 5-MB buffer was uploaded as CLOB instead of the actual filelength
	
	Revision 1.21  2009/02/03 17:50:17  enrcar
	EC:: Missing quotation marks. Fixed.
	CVS
	
	Revision 1.20  2009/02/03 17:13:14  ivafam
	Bug Fixed
	
	Revision 1.19  2008/11/20 10:46:44  marpas
	getAllowConcurrency added
	
	Revision 1.18  2008/11/19 12:17:44  marpas
	NOW changed into NOW()
	
	Revision 1.17  2008/11/17 15:27:02  marpas
	currentDateTime added
	
	Revision 1.16  2008/11/17 11:06:35  ivafam
	PostGres keyword added
	
	Revision 1.15  2008/11/04 12:03:23  marpas
	bitwise operation introduced
	
	Revision 1.14  2008/07/02 08:59:00  enrcar
	EC:: option eConnCustomInfo removed since redundant. eConnNativeHandle should be used instead.
	
	Revision 1.13  2008/06/09 20:14:33  paoscu
	Fixed dependencies identification and dropped column management in metadata query
	
	Revision 1.12  2008/06/09 09:51:56  crivig
	bug on metadataquery fixed
	
	Revision 1.11  2008/05/27 13:37:50  lucio.pulvirenti
	added Translation Map element for <offset> reserved keyword col name
	
	Revision 1.10  2008/05/22 16:29:49  lucio.pulvirenti
	EC:: CustomProcedure Handled. CV:: MetadataQuery fixed.  PS:: Syntax error in getLastSerial fixed
	
	Revision 1.9  2008/05/14 15:07:40  crivig
	childtablesquery introduced; bugs on metadataquery solved
	
	Revision 1.8  2008/05/09 09:28:01  enrcar
	EC:: ColumnTranslationMap filled. getLastSerial method fixed.
	
	Revision 1.7  2008/05/07 17:06:33  enrcar
	EC:: Clobs uploaded as escape sequences
	
	Revision 1.6  2008/05/07 16:05:15  enrcar
	EC:: PGresultGuard added
	
	Revision 1.5  2008/05/07 14:19:40  enrcar
	EC:: Blob & Clob handling: work in progress
	
	Revision 1.4  2008/04/14 12:28:14  enrcar
	EC:: work in progress
	
	Revision 1.2  2007/12/11 13:48:17  enrcar
	EC:: Methods returnExecForSet, returnExecForStatus implemented
	
	Revision 1.1.1.2  2007/12/04 10:44:23  crivig
	Imported source
	
*/


#include <Postgres8SQLDialectFactory.h>
#include <Postgres8SQLDialect.h>
#include <rsResourceSet.h>
#include <dbSchemaMetaDataProvider.h>

#include <StringUtils.h>
#include <SQLString.h>
#include <Plugin.h>
#include <dbConnection.h>
#include <dbQuery.h>
#include <dbConv.h>
#include <dbSet.h>
#include <dbDriver.h>
#include <Filterables.h>
#include <dlfcn.h>
#include <dbxcommon.hpp>    
#include <libpq/libpq-fs.h> // PRQA S 1013

/* unix open */
#include <sys/types.h> // PRQA S 1013 2
#include <sys/stat.h>
#include <fcntl.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
using acs::pattern::FDGuard ;


const char *Postgres8SQLDialect::_minimumConTime_="PGCONNECT_TIMEOUT=2" ;
char Postgres8SQLDialect::_dateStyle_[64]="" ; // PRQA S 4403

char Postgres8SQLDialect::_conTimeout_[32] ; // PRQA S 4403

uint64_t Postgres8SQLDialect::_notify_counter=0 ; 

ACS_CLASS_DEFINE_DEBUG_LEVEL(Postgres8SQLDialect)



Postgres8SQLDialect::TransactionHandler::TransactionHandler(PGconn* conn) :
		_conn(conn)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PGconn: 0x" << hex << conn) ;

    ACS_COND_THROW( (0 ==  _conn ), InvalidOperation ("Null pointer") ); // PRQA S 3081
  
    PGresult *res = PQexec(_conn, "begin"); // PRQA S 4412  
    PGresultGuard resultGuard(res) ;
    ExecStatusType status = PQresultStatus (res) ;
    if ( status != PGRES_COMMAND_OK) 
    {
        ostringstream s;
        s << "Cannot start transaction, status: " << PQresStatus(status) ; // PRQA S 3081 2
		ACS_THROW( InvalidOperation(s.str()));
	}

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Finished, TRANSACTION STARTED.") ;
}


Postgres8SQLDialect::TransactionHandler::~TransactionHandler() throw() // PRQA S 4020
{
    // PRQA S 4631 L1
    if ( 0 != _conn)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PGconn: 0x" << hex << _conn) ;
	}
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "nothing to do: return") ;
        
        return ; /* DO NOTHING */
    }

    try {

	    rollback() ;
	}
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Finished.")
    // PRQA L:L1
}


void Postgres8SQLDialect::TransactionHandler::commit()
{
   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PGconn: 0x" << hex << _conn) ;

 	
    ACS_COND_THROW ( 0== _conn, InvalidOperation("Commit out of sequence: no conn") ) ;  // PRQA S 3081

    PGresult *res = PQexec(_conn, "end"); // PRQA S 4412
    PGresultGuard resultGuard(res) ;
    ExecStatusType status = PQresultStatus (res) ;
    if ( status != PGRES_COMMAND_OK) 
    {
		ACS_THROW (InvalidOperation("Cannot commit transaction, status: "+ string(PQresStatus(status)))) ; // PRQA S 3081
	}

   	_conn = 0 ;
    

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Finished, TRANSACTION CLOSED.") ;
}



void Postgres8SQLDialect::TransactionHandler::rollback()
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PGconn: 0x" << hex << _conn) ;

 	
    ACS_COND_THROW (0 ==  _conn, InvalidOperation("Commit out of sequence: no conn") ) ; // PRQA S 3081
 

    PGresult *res = PQexec(_conn, "rollback"); // PRQA S 4412
    PGresultGuard resultGuard(res) ;
    ExecStatusType status = PQresultStatus (res) ;


    if ( status != PGRES_COMMAND_OK) 
    {
		ACS_THROW(InvalidOperation("Cannot rollback transaction, status: " + string(PQresStatus(status)))) ; // PRQA S 3081
	}

   	_conn = 0 ;
    

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method Finished, TRANSACTION ABORTED.") ;
}


Postgres8SQLDialect::Postgres8SQLDialect(const string &key) : 
    SQLDialect(Postgres8SQLDialectFactory::statDialectName), 
    _timeStampResolution(), 
    _gisOperatorAdjusted(),
    _key(key)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
	memset(_conTimeout_,0,sizeof(_conTimeout_)) ;
    
    //Fill Select template
    _selectTemplate.push_back(SELECT);
    _selectTemplate.push_back(DISTINCT);
    _selectTemplate.push_back(SELECT_LIST);
    _selectTemplate.push_back(FROM);
    _selectTemplate.push_back(TABLE_LIST);
    _selectTemplate.push_back(JOIN);
    _selectTemplate.push_back(WHERE);
    _selectTemplate.push_back(GROUPBY);
    _selectTemplate.push_back(HAVING);
    _selectTemplate.push_back(ORDERBY);
    _selectTemplate.push_back(LIMIT);
    _selectTemplate.push_back(OFFSET);
#warning FORREADONLY position could be wrong - still unsupported
    _selectTemplate.push_back(FORREADONLY);


    //Fill Union template
    _unionTemplate.push_back(UNION_LIST);
    _unionTemplate.push_back(ORDERBY);
    

    //Fill Update template
    _updateTemplate.push_back(UPDATE);
    _updateTemplate.push_back(TABLE_LIST);
    _updateTemplate.push_back(SET);
    _updateTemplate.push_back(SET_LIST);
    _updateTemplate.push_back(WHERE);
    _updateTemplate.push_back(ORDERBY);


    //Fill Drop template
    _dropTemplate.push_back(DROP);
    _dropTemplate.push_back(TABLE_LIST);
  
  
    //Fill Insert template
    _insertTemplate.push_back(INSERT);
    _insertTemplate.push_back(TABLE_LIST);
    _insertTemplate.push_back(COLUMN_LIST);
    _insertTemplate.push_back(VALUES);	// Used only if VALUES_LIST is present
    _insertTemplate.push_back(VALUES_LIST);
    _insertTemplate.push_back(NESTED_VALUES_LIST);
    _insertTemplate.push_back(ON_CONFLICT_DO);
    
    //Fill Delete template
    _deleteTemplate.push_back(DELETE);
    _deleteTemplate.push_back(TABLE_LIST);
    _deleteTemplate.push_back(WHERE);


    //Fill Function template
    _functionTemplate.push_back(FUNCTIONNAME);
    _functionTemplate.push_back(FUNCTIONARG);


    //Fill Custom Function template
    _custFunctionTemplate.push_back(CUSTFUNCTIONNAME);
    _custFunctionTemplate.push_back(CUSTFUNCTIONARG);

    //Fill Custom Procedure template
    _custProcedureTemplate.push_back(CUSTPROCEDURENAME);
    _custProcedureTemplate.push_back(CUSTPROCEDUREARG);


    //Fill UploadBlob template
    _uploadBlobTemplate.push_back(TEMPLATE_INVALID);

  
    //Fill DownloadBlob template
    _downloadBlobTemplate.push_back(TEMPLATE_INVALID);
 
 
   //Fill BlobIsNull template
    _blobIsNullTemplate.push_back(TEMPLATE_INVALID);
  

    //Fill SQl Tokens map	
    _sqlTokenMap.insert(pair<SQLToken, string const>(DELETE,	"DELETE FROM"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(DISTINCT,	"DISTINCT"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(DROP,	    "DROP TABLE"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(FROM, 		"FROM"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(GROUPBY, 	"GROUP BY"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(HAVING, 	"HAVING"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(LIMIT, 	"LIMIT "));
    _sqlTokenMap.insert(pair<SQLToken, string const>(OFFSET, 	"OFFSET "));
    _sqlTokenMap.insert(pair<SQLToken, string const>(ORDERBY, 	"ORDER BY"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(SELECT, 	"SELECT"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(SET, 	    "SET"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(WHERE, 	"WHERE"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(UPDATE,	"UPDATE"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(UNION,		"UNION"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(UNION_ALL,	"UNION ALL"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(INSERT,	"INSERT INTO"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(VALUES,	"VALUES"));
    _sqlTokenMap.insert(pair<SQLToken, string const>(ON_CONFLICT_DO, "ON CONFLICT")); 
    _sqlTokenMap.insert(pair<SQLToken, string const>(ON_CONFLICT_CNSR_DO, "ON CONFLICT ON CONSTRAINT")); 
    _sqlTokenMap.insert(pair<SQLToken, string const>(FUNCTIONNAME,	""));   /* Not nested. INTENTIONALLY BLANK */
    _sqlTokenMap.insert(pair<SQLToken, string const>(NESTEDFUNCTIONNAME,	""));       /* Nested. INTENTIONALLY BLANK */
    _sqlTokenMap.insert(pair<SQLToken, string const>(CUSTFUNCTIONNAME,	"EXECUTE"));   
    _sqlTokenMap.insert(pair<SQLToken, string const>(CUSTNESTEDFUNCTIONNAME,	""));       /* Nested. INTENTIONALLY BLANK */
    _sqlTokenMap.insert(pair<SQLToken, string const>(CUSTPROCEDURENAME,	"SELECT * FROM"));   /* Not nested */
    _sqlTokenMap.insert(pair<SQLToken, string const>(CUSTNESTEDPROCEDURENAME,	""));       /* Nested. INTENTIONALLY BLANK */
#warning FORREADONLY select actually unsupported
    _sqlTokenMap.insert(pair<SQLToken, string const>(FORREADONLY,"")); 

    //Fill SQl Join Tokens map	
    _sqlJoinTokenMap.insert(pair<SQLJoinToken, string const>(CROSS,	"CROSS JOIN"));
    _sqlJoinTokenMap.insert(pair<SQLJoinToken, string const>(INNER,	"INNER JOIN"));
    _sqlJoinTokenMap.insert(pair<SQLJoinToken, string const>(LEFT,  "LEFT OUTER JOIN"));
    _sqlJoinTokenMap.insert(pair<SQLJoinToken, string const>(RIGHT, "RIGHT OUTER JOIN"));
    _sqlJoinTokenMap.insert(pair<SQLJoinToken, string const>(FULL, 	"FULL OUTER JOIN"));

	//Fill SQl Operations map
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(EQUAL_TO, 			" = "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(LIKE, 				" LIKE "		));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(MINOR_OF, 			" < "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(MAJOR_OF, 			" > "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(MAJOR_OR_EQUAL_OF, 	" >= "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(MINOR_OR_EQUAL_OF, 	" <= "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(DIFFERENT_FROM, 		" <> "			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(BETWEEN, 			" BETWEEN "		));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(NOT_BETWEEN, 		" NOT BETWEEN "	));
    // MP: the adjustGisOperatorMap method will take care of it 
    _sqlOperatorMap.insert(pair<SQLOperator, string const>(INSIDE, 			    " ST_Contains("	));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(INTERSECT, 			" ST_Intersects(" ));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(OUTSIDE, 			" NOT ST_Intersects(" ));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(ISNULL, 				" IS NULL "		));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(ISNOTNULL, 			" IS NOT NULL "	));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(IN, 					" IN ("			));
	_sqlOperatorMap.insert(pair<SQLOperator, string const>(NOT_IN, 					" NOT IN ("			));


	//Fill SQl Cast Types map
    /*
    	Synopsis:
        
        To translate each SQLCastType entry, use the following statement:
        
        _sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    		(ENUMTYPE, make_pair("STRING", "ARGS")) );

    	Where: ENUMTYPE belongs to enum SQLCastType (e.g. SQL_FLOAT)
        "STRING" is the DB-dependent translation for the ENUMTYPE (e.g. "float", "real", "shortdouble" ...)
        "ARGS" is a empty or 3-chars description of the argument format expected by that type.
    	- If no argument is required (e.g. SQL_FLOAT), ARGS will be an EMPTY-STRING: ""
        - If one or two argument are required (e.g. SQL_VARCHAR) "ARGS" will be a 3-chars string:
        	"LEFT_BRACKET, RIGHT_BRAKET, NUMBER OF ARGUMENTS". E.G. :
            "()1" -> one argument, round brackets: (arg1)
            "[]2" -> two arguments, square brackets: [arg1, arg2]
            "<>1" -> one argument, angular brackets: <arg1>
            ...
            
    	Examples:
    	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    		(SQL_DOUBLE, make_pair("long_float_type", "")) ) ;
		_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
            (SQL_CHAR, make_pair("carattere", "[]1")) ) ;
    	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    		(SQL_DECIMAL, make_pair("Numerique", "()2")) ) ;
		...  ...
    */
    _sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_BOOLEAN,	make_pair("bool", "")) );
    _sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_SMALLINT,	make_pair("int2", "")) );
    _sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_INTEGER,	make_pair("int4", "")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_BIGINT,	make_pair("int8", "")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_SERIAL,	make_pair("integer", "")) );// NOTE: SERIAL exists but it's not a type: is a convenience for a INTEGER AUTO_INCREMENT 
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_BIGSERIAL,	make_pair("bigint", "")) );	// NOTE: BIGSERIAL exists but it's not a type: is a convenience for a BIGINT AUTO_INCREMENT 
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_FLOAT,		make_pair("float4", "")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_DOUBLE,	make_pair("float8", "")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_DECIMAL,	make_pair("decimal", "()2")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_CHAR,		make_pair("char", "()1")) );
	_sqlCastTypeMap.insert( pair<SQLCastType, pair<string const, string const> >
    	(SQL_VARCHAR,	make_pair("varchar", "()1")) );

	
	// Fill Translation Map for Reserved-keyword columns
	_columnTranslationMap["group"] = "\"group\"" ;
	_columnTranslationMap["user"] = "\"user\"" ;
	_columnTranslationMap["offset"] = "\"offset\"" ;
	_columnTranslationMap["name"] = "\"name\"" ;
	_columnTranslationMap["version"] = "\"version\"" ;
	_columnTranslationMap["schema"] = "\"schema\"" ;
	_columnTranslationMap["package"] = "\"package\"" ;
	_columnTranslationMap["storage"] = "\"storage\"" ;
	_columnTranslationMap["begin"] = "\"begin\"" ;
	_columnTranslationMap["end"] = "\"end\"" ;
	_columnTranslationMap["action"] = "\"action\"" ;
	_columnTranslationMap["password"] = "\"password\"" ;
	_columnTranslationMap["type"] = "\"type\"" ;
    
}

namespace {
    Mutex gisOpMapMutex ; // NB this is used only the very first time for each Dialect class.
                          // in case of multiple dialects, two of them could possibly sinchronize
                          // on the same mutex even being independent - negligible delay
} 

void Postgres8SQLDialect::adjustGisOperatorMap() const
{
    if (_gisOperatorAdjusted) { return ; }
    Mutex::Lock l(gisOpMapMutex) ;
    if (_gisOperatorAdjusted) { return ; } // check for concurrency
    
    if ( dbSchemaMetaDataProvider::instance(_key)->getPostGISMajor() == 3) {
	    _sqlOperatorMap.erase(INSIDE);
	    _sqlOperatorMap.insert(pair<SQLOperator, string const>(INSIDE, 	        " ST_Covers("	));
    }
    _gisOperatorAdjusted = true ;
}

Postgres8SQLDialect::~Postgres8SQLDialect() throw() {}

void Postgres8SQLDialect::setFastConnect() const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "putenv("<< _minimumConTime_ << ")") ;
	putenv(strdup(const_cast<char *>(_minimumConTime_))) ; // PRQA S 3081
}


bool Postgres8SQLDialect::getAllowConcurrency() const 
{
	return true ;
}

void Postgres8SQLDialect::setEnv(rsResourceSet &rset) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	if (rset.hasKey("DateTimeRoundingMode")) {
        ACS_LOG_WARNING("DateTimeRoundingMode useless in db conf. Please remove it. ") ;
	}

    // MP concurrency: is useless
	unsigned short fracdgts = DateTime::getDefaultFractionDigits() ;
	try {
		rset.getValue("DateTimeFractionDigits", fracdgts) ;
	}
	catch (rsResourceSet::NotFoundException &) {
        ACS_LOG_WARNING("DateTimeFractionDigits missing in db conf. using default: " << fracdgts) ;
	}
	
	unsigned short dbfracdgts = fracdgts ;
	try {
		rset.getValue("DateTimeDbFractionDigits", dbfracdgts) ;
	}
	catch (rsResourceSet::NotFoundException &) {
		ACS_LOG_WARNING("DateTimeDbFractionDigits missing in db conf. using default: " << dbfracdgts) ;
	}

	DateTime::setDefaultFractionDigits(fracdgts) ;
	DateTime::setDefaultDbFractionDigits(dbfracdgts) ;

	// if the timeout and retries are not defined, set them to default values.
	int timeout= 2 ; // secs reccomended by documentation
	try {
		rset.getValue("connectionTimeout", timeout) ;
	}
	catch (rsResourceSet::NotFoundException &) {
        ACS_LOG_WARNING("db.conf missing parameter - setting connectionTimeout to " << timeout) ;
	}
	if ((timeout > 0) && (timeout < 2)) { // PRQA S 4400
		ACS_LOG_WARNING("db.conf connectionTimeout is less than 2 sec") ;
    }
	if (strcmp(_conTimeout_,"") == 0) {
		ostringstream tmp ;
		tmp << "PGCONNECT_TIMEOUT=" << timeout ;
		strncpy(_conTimeout_,tmp.str().c_str(),sizeof(_conTimeout_)-1) ; // PRQA S 3084 2
		_conTimeout_[sizeof(_conTimeout_)-1] = 0 ;
		putenv(strdup(_conTimeout_)) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "putenv(" << _conTimeout_ << ")") ;
	}

	if (rset.hasKey("DefaultSingleStepSecondsToWait")) {
        ACS_LOG_WARNING("DefaultSingleStepSecondsToWait useless in db conf. Please remove it. ") ;
	}

	if (rset.hasKey("DefaultSecondsToWait")) {
        ACS_LOG_WARNING("DefaultSecondsToWait useless in db conf. Please remove it. ") ;
	}
}


string Postgres8SQLDialect::getLockModeWait(int) const 
{
    return "" ; // no lock mode on pg	
}


string Postgres8SQLDialect::childTablesQuery(std::string const & tableName) const
{

	string res(
" SELECT "
" CAST(cl2.relname as varchar) 		"
" ,CAST(cl.relname as varchar)          "
" FROM                  "
"  pg_constraint con    "
" , pg_class cl			"
" , pg_class cl2		"
" WHERE					"
"con.confrelid = cl.oid "
"  AND cl.relname = '__TaBleNaMe__' "
"  AND con.conrelid = cl2.oid "
	) ; //string res
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes generic childTablesQuery query (before tablename substitution) \""
            << res << "\"") ;


	string toBeReplaced("__TaBleNaMe__") ;
	res.replace( res.find(toBeReplaced) , toBeReplaced.size() , tableName ) ;


	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes specific childTablesQuery query \"" << res << "\"") ;


	return res ;
}

// Shall return a set of two columns: indexName, columnName 
// Describing all the indexes of a given table
string Postgres8SQLDialect::getTableIndexesQuery (string const & tableName ) const
{
	ostringstream queryoss;
	queryoss <<
		"select " <<
		"i.relname::VARCHAR as index_name, " <<
		"a.attname::VARCHAR as column_name " <<
		"from " <<
		"pg_class t, " <<
		"pg_class i, " <<
		"pg_index ix, " <<
		"pg_attribute a " <<
		"where " <<
		"t.oid = ix.indrelid " <<
		"and i.oid = ix.indexrelid " <<
		"and a.attrelid = t.oid " <<
		"and a.attnum = ANY(ix.indkey) " <<
		"and t.relkind = 'r' " <<
		"and t.relname  = '" << tableName << "' " <<
		"order by " <<
		"t.relname, " <<
		"i.relname; ";

	string res = queryoss.str();
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes table index query \"" << res << "\"") ;

	return res ;
}

string Postgres8SQLDialect::metadataQuery(std::string const & tableName) const
{
	// 2020-03-05, ENRCAR::
	//
	// About:
	//        WHEN att.atttypid = 3802 THEN 16384  ...
	//
	// The "case" is used to set the fieldSize ;
	// Type 3802 is JSONB.
	// (So it reads: -> size of JSONB is up to 16KB). 
	// Being a binary type, with variable size that remains unspecified, has both att.attlen=-1  and  att.atttypmod=-1, like usual BLOB/CLOB.
	// (So, BLOB, CLOB and JSONB are alike).
	// For BLOB/CLOB, the condition above triggers a fieldSize of ZERO bytes:
	// preventing any direct INSERT sql-query to succeed -- the specific API to push a file is always required.
	// Nevertheless, for JSONB was decided to also allow a direct INSERT sql-query up to 16384 bytes, without using the API.
	// This should be done for JSONB of known sizes, e.g. a MD5 (32 chars) or a filename (4096 chars).
	// For larger JSONB, or JSONB of unknown/run-time size (e.g. a configuration file, which might have very very long comments), the API should be used instead.
	//
	
	
	
	string res(
	" select DISTINCT att.attnum - 1 - \
	  ( SELECT COUNT(*) FROM pg_attribute att2 \
  		WHERE att.attrelid = att2 .attrelid AND att2.attnum < att.attnum \
		AND   att2.attnum >0 AND att2.attisdropped ) as attnum , \
	CAST(att.attname as varchar) as fieldName, \
	CAST(typ.typname as varchar) as propietaryType, \
	(CASE \
		WHEN att.atttypid = 3802 THEN 16384 \
		WHEN att.attlen = -1 AND  att.atttypmod	!= -1 THEN att.atttypmod - 4 \
		WHEN att.attlen = -1 AND  att.atttypmod	= -1 THEN 0 \
		ELSE att.attlen \
	END) as fieldSize, \
	CAST ((CASE \
		WHEN typ.typname = 'time' THEN 'HOUR' \
		WHEN typ.typname = 'date' THEN 'YEAR' \
		ELSE 'YEAR'  		\
	END) as varchar) as dateTimeRangeFrom, \
	CAST((CASE \
		WHEN typ.typname = 'date' THEN 'DAY' \
		ELSE \
		(CASE \
			WHEN att.atttypmod =0 THEN 'SECONDS' \
			WHEN att.atttypmod =1 THEN 'FRACTION(1)' \
			WHEN att.atttypmod =2 THEN 'FRACTION(2)' \
			WHEN att.atttypmod =3 THEN 'FRACTION(3)' \
			WHEN att.atttypmod =4 THEN 'FRACTION(4)' \
			WHEN att.atttypmod =5 THEN 'FRACTION(5)' 		\
			WHEN att.atttypmod =6 THEN 'FRACTION(6)' \
			ELSE 'SECONDS' \
		END) \
	END) as varchar) as dateTimeRangeTo, \
	CAST((CASE \
		WHEN conp.contype = 'p' \
		THEN 'T' \
		ELSE 'F' \
	END) as varchar) as isPk, \
	CAST((CASE \
		WHEN att.attnotnull = 't' \
		THEN 'T' \
		ELSE 'F' \
	END) as varchar) as isNotNull, \
	CAST((CASE \
		WHEN dep.classid IS NULL \
		THEN 'F' \
		ELSE 'T' \
	END) as varchar) as isSerialString, \
	CAST((CASE \
		WHEN (dep.classid IS NOT NULL OR att.atthasdef = 'f' ) \
		THEN 'F' \
		ELSE 'T' \
	END) as varchar) as hasDefaultString, \
	CAST (cl.relname as varchar) as foreignTable , \
	attfk.attnum -1 as foreignField, \
	CAST (attfk.attname  as varchar) as foreignFieldName, \
	0 as unused, \
    CAST((CASE \
		WHEN conp.contype = 'u' OR  conp.contype = 'p' \
		THEN 'T' \
		ELSE 'F' \
	END) as varchar) as isUnique \
	from pg_attribute att 		\
	INNER JOIN pg_type typ ON ( att.atttypid = typ.oid ) \
	INNER JOIN pg_class cl2 ON \
	( \
		cl2.relname = '__TaBleNaMe__' \
		AND att.attrelid = cl2.oid \
		AND att.attnum >= 1 \
		AND att.attisdropped = 'f' \
	) 		\
	LEFT JOIN \
	( \
		pg_constraint conf \
		LEFT JOIN pg_class cl ON ( conf.confrelid = cl.oid ) \
		LEFT JOIN pg_attribute attfk ON ( attfk.attrelid = cl.oid ) \
	) ON ( \
		att.attrelid = conf.conrelid \
		AND (conf.contype = 'f' ) \
		AND ((attfk.attnum = conf.confkey[1] AND att.attnum = conf.conkey[1] ) OR \
		(attfk.attnum = conf.confkey[2] AND att.attnum = conf.conkey[2])) \
	) 		\
	LEFT JOIN pg_constraint conp ON	\
	( \
		att.attrelid = conp.conrelid AND ( \
           ( conp.contype = 'p' AND att.attnum = ANY (conp.conkey) ) \
           OR \
           ( conp.contype = 'u' AND att.attnum = conp.conkey[1] AND array_length(conp.conkey, 1) = 1 ) \
        ) \
	) \
	LEFT JOIN \
	( \
		pg_depend dep \
		INNER JOIN pg_class cl3 ON \
		( \
		dep.objid = cl3.oid \
		AND cl3.relkind = 'S' \
		) \
	) ON ( \
		dep.refobjid = cl2.oid \
		AND dep.refobjsubid = att.attnum \
	) \
	order by attnum") ; //string res

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes generic metadata query (before tablename substitution) \""
            << res << "\"") ;

	string toBeReplaced("__TaBleNaMe__") ;
	res.replace( res.find(toBeReplaced) , toBeReplaced.size() , tableName ) ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Now comes specific metadata query \"" << res << "\"") ;

	return res ;
}


string Postgres8SQLDialect::getIsolationCommittedRead() const 
{  
    return "" ;	
#if(0)
    return "SET ISOLATION TO COMMITTED READ" ;  
#endif
}

string Postgres8SQLDialect::currentDateTime() const 
{   return "NOW()::timestamp" ;   }


string Postgres8SQLDialect::getIsGoodDummyQuery() const 
{   return "SELECT 1" ;   }


string Postgres8SQLDialect::caseWhenElse(const vector< pair<string, string> > &paramsV, const string &param_else) const 
{
	ostringstream o ; 
	o << "( CASE" ;
	for (size_t i=0; i<paramsV.size(); i++) { o << " WHEN ( " << paramsV.at(i).first << " ) THEN ( " << paramsV.at(i).second << " ) " ; }
	if (param_else.empty() == false) { o << " ELSE ( " << param_else << " ) " ; }
	o << " END )" ;
	return o.str() ; 
}


string Postgres8SQLDialect::beginTransaction(const std::string& txname) const 
{   return "BEGIN WORK" ;   }


string Postgres8SQLDialect::commitTransaction() const 
{   return "COMMIT WORK" ;  }


string Postgres8SQLDialect::rollbackTransaction() const 
{   return "ROLLBACK WORK" ;    }


string Postgres8SQLDialect::getSeparator() const
{   return ", " ;   }


string Postgres8SQLDialect::getStatementTerminator() const
{   return ";" ;	}


string Postgres8SQLDialect::orderByASC() const
{   return "ASC" ;  }


string Postgres8SQLDialect::orderByDESC() const 
{   return "DESC" ; }


string Postgres8SQLDialect::boolTrue() const
{   return "'t'" ;  }


string Postgres8SQLDialect::boolFalse() const 
{   return "'f'" ;  }


bool Postgres8SQLDialect::checkBooleanValue(const std::string &b) const 
{
	return (b == "'t'" || b == "'T'" || b == "'f'" || b == "'F'" ) ;
}	


string Postgres8SQLDialect::translateSQLCast(const string& item, const SQLDialect::SQLCast &s, bool raiseException) const  // PRQA S 4020
{
// W8115_OFF: multiple returns
    /**
        Setting raiseException to FALSE no exception will be returned if SQLCastType not found in map
        Useful for dumping values, also if no translation is available.
    */
	
    if (s.getSQLCastType() == SQL_TYPEUNDEF)
    {
	    return item ;	/* No type actually provided */
    }


    map<SQLCastType, pair<string const, string const> > ::const_iterator it=_sqlCastTypeMap.find( s.getSQLCastType() );
    if(it==_sqlCastTypeMap.end())
    {
        ostringstream errorMessage;
        errorMessage << "Cannot convert SQL type " << s.getSQLCastType() ; 
        if (raiseException)
        {
		    ACS_THROW(CastTypeNotFoundException(errorMessage.str() )) ; // PRQA S 3081
        }
        else
        {
            return errorMessage.str() ;
        }
    }
	
	// SQLType was found.
	
    string t = (it->second).first ;
    string a = (it->second).second ;
    string p = "" ;


    switch( a.length() )
    {
    case 0 :
	{
            /* Do nothing */
	    break ;
	}   
    case 3 : // PRQA S 4402
	{
            char lb	= a[0] ;
            char rb	= a[1] ;
            int n	= (a[2]-'0') ;	/* Convert a single char to a number. E.G. '2' -> 2 */
            switch (n)
	    {
            case 0:
			    /* Do nothing */
            case 1:
                {   
                    if (s.getArg1() != "") { p = lb + s.getArg1() + rb ; }
		    break ;
	        }
            case 2: // PRQA S 4402
                {
	            string sep = ( (s.getArg1() != "") && (s.getArg2() != "") ) ? ", " : "" ; // PRQA S 3384
                    if  ( (s.getArg1() != "") || (s.getArg2() != "") ) 
                    {
                	p = lb + s.getArg1() + sep + s.getArg2() + rb ;
                    }
                    break ;
		}
            default: 
                {       
                    ostringstream errorMessage;
                    errorMessage << "translateSQLCast:: Internal error parsing argument descriptor for type: " << s.getSQLCastType() ;
                    ACS_THROW(InternalErrorException(errorMessage.str() )) ; // PRQA S 3081
                }
	    } ;	/* switch (n) */
          
            break ;
	} /* case (a.size() == 3) */
	default:  
	{                  
            ostringstream errorMessage;
            errorMessage << "Internal error checking argument descriptor for type: " << s.getSQLCastType() ;
            ACS_THROW(InternalErrorException(errorMessage.str() )) ; // PRQA S 3081
	}
    } ; /* switch( a.size() ) */

    t = ( "(" + item + ")::" + t + p) ;

    return t ;
}


string Postgres8SQLDialect::bitAnd(const std::string &l, const std::string &r)
{ return string ("(") + l + "&" + r + ")" ; } // PRQA S 3081

string Postgres8SQLDialect::bitOr(const std::string &l, const std::string &r)
{ return string ("(") + l + "|" + r + ")" ; } // PRQA S 3081

string Postgres8SQLDialect::bitXor(const std::string &l, const std::string &r)
{ return string ("(") + l + "#" + r + ")" ; } // PRQA S 3081

string Postgres8SQLDialect::bitNot(const std::string &l)
{ return string ("(~") + l + ")" ; }          // PRQA S 3081
   
    
bool Postgres8SQLDialect::hasTimedWaitOnLock() const 
{
	return false ;
}

namespace {
    char delim = '\'' ; // PRQA S 4401

    struct escaper { // PRQA S 2109, 2171, 2173, 2175
        explicit escaper(std::string &out) : out_(out), escaped_(false) {} // PRQA S 2528 2
        escaper(const escaper &r) : out_(r.out_), escaped_(r.escaped_) {}
        ~escaper() {}
        void operator() (const char c) 
        { 
            if (c == delim) {
                out_ += "\\\'" ;
                escaped_ = true ;
            }
            else {
                out_ += c ;
            }
        }
        bool escaped() const throw() { return escaped_ ; }
        
    private:
        escaper() ;
        escaper & operator=(const escaper &) ;
    private:
        std::string &out_ ;
        bool escaped_ ; 
    } ;
}

string Postgres8SQLDialect::delimit(const string &s) const // PRQA S 4020
{
    if (s.empty()) {
        return "''" ; // see delim in the unnamed namespace above
    }
    
    string delimited ; 
    string final ;
    string::const_iterator start = s.begin() ;
    string::const_iterator last = s.end() ;
    if (s[0] == delim) { // delim at the beginning -> check if it is at the end 
        size_t len = s.length() ;
        if ((len > 1) && (s[len-1] == delim)) { // PRQA S 3084
            start++ ;
            last = s.begin() + len-1 ; // PRQA S 3000
        }
    }
    delimited += delim ;
    final += delim ;
    escaper e(delimited) ;
    escaper ef = for_each(start, last, e) ; 
    delimited += final ;
    if (ef.escaped()) {
        return 'E' + delimited ;
    }
    else {
        return delimited ;
    }
}

string Postgres8SQLDialect::getSQLSelectListDelimiter_L() const 
{   return "" ;  }


string Postgres8SQLDialect::getSQLSelectListDelimiter_R() const 
{   return "" ;  }


string Postgres8SQLDialect::getSQLOrdGrpByListDelimiter_L() const 
{   return "" ;  }


string Postgres8SQLDialect::getSQLOrdGrpByListDelimiter_R() const 
{   return "" ;  }

string Postgres8SQLDialect::getTableNameForEmptyFROM() const 
{   return "now()" ;  }

long Postgres8SQLDialect::SQLFilterReturn(long v) const 
{   return  v ;     }


string Postgres8SQLDialect::convertCaseFilter(string const &s) const
{
	return StringUtils::lowercase(s) ;
}


string Postgres8SQLDialect::function_ISNULL(string const &s) const
{
	string res ;
 	ACS_COND_THROW(s.empty(), InvalidOperation("Empty input string." )) ; // PRQA S 3081
 
    res = " " + s + " IS NULL " ;

	return res ;
}


string Postgres8SQLDialect::function_ISNOTNULL(string const &s) const
{
	string res ;
  	ACS_COND_THROW(s.empty(), InvalidOperation("Empty input string." )) ;// PRQA S 3081

    res = " " + s + " IS NOT NULL " ;
    
	return res ;
}


string Postgres8SQLDialect::getLastSerial(string const &table, dbConnection &conn) const
{
	string res ;
 
 	ACS_COND_THROW(table.empty(), InvalidOperation("Empty table name." )) ;// PRQA S 3081 
 
 	string s = "'" + table + "_" + dbSchemaMetaDataProvider::instance(_key)->getSerialFieldName(table, conn)+  "_seq" + "'" ; 
    res = "SELECT currval(" + s + ")" ;
    
	return res ;
}


PGconn* Postgres8SQLDialect::getConnectionPointer(dbConnection * conn) const
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

#ifndef __x86_64__   
	int ptr = 0 ;
#else
	long int ptr = 0;
#endif
	short int ptrLen = 0 ;


    (conn->getConnection())->getOption(eConnNativeHandle, &ptr, sizeof(ptr), &ptrLen);


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. 0x" << hex << ptr) ;

    return reinterpret_cast<PGconn*>( ptr ); // PRQA S 3081
}


void Postgres8SQLDialect::downloadClob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, const string& completePath ) 
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    string queryString ;
    queryString += "SELECT CAST (" + tableName + "." + columnName + " AS VARCHAR) " ; 
    queryString += " FROM " +  tableName ;
    if (!whereCondition.empty()) { queryString += " WHERE " + whereCondition ; }
    SQLString sqlString(conn->getKey(), SQLString::COMMAND) ;
    sqlString.forceSQLString(queryString);
    
    return downloadClob(conn, sqlString, completePath) ; 
}


void Postgres8SQLDialect::downloadBlob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, const string& completePath ) 
{ 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called [5]\n" 
		    << "\tTableName: !" << tableName << "!\n" 
		    << "\tColumnName: !" << columnName << "!\n" 
		    << "\tWhereCondition: !" << whereCondition << "!\n" 
		    << "\tCompletePath: !" << completePath << "!") ;

    const signed int lo_export_success = 1 ;
    PGconn* connectionPointer = getConnectionPointer(conn) ;
    Oid oid = getOid(conn, tableName, columnName, whereCondition ) ; 

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "oid is: " << oid) ;


	/* Begin transaction */
	TransactionHandler transactionHandler (connectionPointer) ;


    /* Executing the export command */
    int status = lo_export(connectionPointer, oid, completePath.c_str()) ;
    if ( status != lo_export_success)
    {
        ostringstream s;
        s << "Error running lo_export [" << status << "]" ;
		ACS_THROW(InvalidOperation(s.str())) ; // PRQA S 3081
	}

	transactionHandler.commit() ;	/* commit transaction*/

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Operation successfully. File: " << completePath << " status: " << status) ;
}


unsigned int Postgres8SQLDialect::getOid(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition)
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	PGconn* connectionPointer = getConnectionPointer(conn) ;
	
    string queryString ;
    
    queryString += "SELECT " + tableName + "." + columnName + " " ; 
    queryString += " FROM " + tableName + " WHERE " + whereCondition + " LIMIT 1" ; 


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << queryString) ;

	
    PGresult *res = PQprepare(	connectionPointer, 
                                "",  // default - will be replaced by next prepare // PRQA S 4412
                                queryString.c_str(), 0, 0) ;
 
    ExecStatusType status = PQresultStatus (res) ;
	if ( status != PGRES_COMMAND_OK) 
    {
		ACS_THROW(InvalidOperation("Cannot run PQprepare, status: " + string(PQresStatus(status)))); // PRQA S 3081
	}


    res = PQexecPrepared( connectionPointer, "", 0, 0, 0, 0, 1) ; //  bynary results
	PGresultGuard resultGuard(res) ;


	if ( (status = PQresultStatus (res)) != PGRES_TUPLES_OK) 
    {
		ACS_THROW(InvalidOperation("Cannot run PQexecPrepared, status: " + string(PQresStatus(status)))); // PRQA S 3081 2
	}


	/* IF HERE -- QUERY WAS SUCCEFULLY */

    int isNull = PQgetisnull (res, 0, 0) ;

	ACS_COND_THROW(0 != isNull, InvalidOperation("NULL OID")); // PRQA S 3081
    
    /* IF HERE -- NOT-NULL OID */

	unsigned char* oidbuff = reinterpret_cast<unsigned char*>(PQgetvalue (res, 0, 0)); // PRQA S 3030, 3081

	ACS_COND_THROW( 0 == oidbuff, InvalidOperation("Empty OID")) ; // PRQA S 3081
   
    /* IF HERE -- NOT-EMPTY OID */

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Freeing the result") ;
    
  
	/* Get OID with correct endianness */
	unsigned int oid = 0 ;
    for (size_t i=0; i<sizeof(oid); i++) // PRQA S 4231, 4235
    {
    	oid |= ( oidbuff[i] << (8*(sizeof(oid)-i-1)) ) ; // PRQA S 3000, 3007, 3084, 4400
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning: " << oid) ;

    return oid ;
}


int Postgres8SQLDialect::setOid(dbConnection* conn, Oid oid, const string &tableName, const string& columnName, const string& whereCondition)
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ostringstream ostroid ;
    
    ostroid << oid ;
	
    return setOidInternal(conn, ostroid.str(), tableName, columnName, whereCondition) ;
}


int Postgres8SQLDialect::setNullOid(dbConnection* conn, const string &tableName, const string& columnName, const string& whereCondition)
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    return setOidInternal(conn, "NULL", tableName, columnName, whereCondition) ;
}


int Postgres8SQLDialect::setOidInternal(dbConnection* conn, const string &oid, const string &tableName, const string& columnName, const string& whereCondition)
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	PGconn* connectionPointer = getConnectionPointer(conn) ;
	 
    string queryString = "UPDATE " + tableName + " SET \"" + columnName + "\" = " + oid + " WHERE " + whereCondition ; 

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Running query: " << queryString) ;

    PGresult *res = PQprepare(	connectionPointer, 
                                "",  // default - will be replaced by next prepare // PRQA S 4412
                                queryString.c_str(), 0, 0) ;
 
    ExecStatusType status = PQresultStatus (res) ;
	
	if ( status != PGRES_COMMAND_OK) 
    {
		ACS_THROW(InvalidOperation("Cannot run PQprepare, status: " + string(PQresStatus(status)) + " (" + queryString + ")")) ;  // PRQA S 3081
	}


    res = PQexecPrepared( connectionPointer, "", 0, 0, 0, 0, 1) ; //  bynary results

	PGresultGuard resultGuard(res) ;

	if ( (status = PQresultStatus (res)) != PGRES_COMMAND_OK) 
    {
		ACS_THROW(InvalidOperation("Cannot run PQexecPrepared, status: " + string(PQresStatus(status)))) ;  // PRQA S 3081 
	}

	return atoi(PQcmdTuples(res)) ;
}


void Postgres8SQLDialect::downloadBlob(dbConnection * conn, const SQLString & sqlString, const string& IGNORED_completePath ) 
{ 
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    return ;
}


void Postgres8SQLDialect::downloadClob(dbConnection * conn, const SQLString & sqlString, const string& completePath ) 
{ 
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUERY: " << sqlString.getSQL()) ;

    dbQuery q(*conn, sqlString, "lob dwnl") ;

    dbSet set;
    q.execForSet(set);
    ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081

	dbRow myRow(conn->getKey()) ; 
    
    set.getNextRow(myRow) ; // first and only 
  
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "execForSet successfully") ;

    string clObject ;
	try {
        myRow.column(0, clObject);
	
#if(0)	
        ACS_COND_THROW( clObject.empty(),InvalidOperation(" Couldn't download file" ) ) ; // PRQA S 3081
#else
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Downloaded Clob with length zero. Query is \"" << sqlString.getSQL() << "\"") ;
#endif
	}
	catch(exception &x) {
        ACS_THROW(
            InvalidOperation(x, "Error in column extraction. Query was: \"" + sqlString.getSQL() + "\"" )
        )
	}
    
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "buffer size is:" << clObject.length()) ;


    
    const char* buffer = clObject.c_str() ;
    const ssize_t chunk_size = 1024 ;
    
  	int fd = open(completePath.c_str(), O_RDWR | O_TRUNC | O_CREAT, 0777) ;
    ACS_COND_THROW(fd < 0, InvalidOperation ("Cannot create " + completePath, errno)) ; // PRQA S 3081
    {
        FDGuard fdguard(fd) ;
        ssize_t rem = clObject.length() ; // PRQA S 3000

        while ( rem > 0 )
        {
            ssize_t tw = (rem >= chunk_size)? chunk_size: rem ; // PRQA S 3384
		    ssize_t tot = clObject.length() - rem ;	/* amount written */ // PRQA S 3000, 3084
            ssize_t w = write(fd, buffer+tot, tw) ; // PRQA S 3000
            ACS_COND_THROW( w != tw, InvalidOperation("No room to save file " + completePath, errno)) ; // PRQA S 3081
            rem -= w ;
        }
        // FDGuard out of scope
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File: " << completePath << " wrote successfully [ size: " << clObject.length() << "]") ;

    return ;
}


size_t Postgres8SQLDialect::uploadBlob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, const string& completePath ) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called [5]\n"
		    << "\tTableName: !" << tableName << "!\n"
		    << "\tColumnName: !" << columnName << "!\n"
		    << "\tWhereCondition: !" << whereCondition << "!\n"
		    << "\tCompletePath: !" << completePath << "!") ;

    PGconn* connectionPointer = getConnectionPointer(conn) ;

   
	/* Begin transaction */
	TransactionHandler transactionHandler (connectionPointer) ;

    /* Executing the lo_import command */
    Oid lobjOid = lo_import(connectionPointer, completePath.c_str()) ;
    if ( lobjOid == InvalidOid ) // PRQA S 3081
    {
        ostringstream s;
        s << "Error running lo_import [" << lobjOid << "]" ;
		ACS_THROW(InvalidOperation (s.str())); // PRQA S 3081
	}


    unsigned long int oid = lobjOid ;
  
  
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File " << completePath << " imported with oid: " << oid ) ;


    size_t ret = setOid(conn, lobjOid, tableName, columnName, whereCondition) ;


	transactionHandler.commit() ;	/* commit transaction*/

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Operation successfully. File: " << completePath << " - affected: " << ret) ;
    
    return ret ;
}


size_t Postgres8SQLDialect::uploadClob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, const string& completePath ) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
    int fd = open(completePath.c_str(), O_RDONLY) ;
    ACS_COND_THROW(fd < 0, InvalidOperation ("Cannot open " + completePath, errno)) ; // PRQA S 3081
    FDGuard fdguard(fd) ;

    ssize_t tot = 0 ;
	char buffer[5*1024*1024] ; // PRQA S 4403
    memset (buffer, 0, sizeof(buffer)) ;
	ssize_t r = 0 ;
    char chunkb[1024] ; // PRQA S 4403

    while ( (r = read(fd, chunkb, sizeof(chunkb))) > 0)
    {
		if  ( size_t( tot + r ) > sizeof(buffer) ) // PRQA S 3081
		{
            ostringstream m ;
            m << "No room to save file " << completePath << " [ >= " << tot+r << "]" ;
            ACS_THROW (InvalidOperation ( m.str() )) ; // PRQA S 3081
        }

    	memcpy(buffer + tot, chunkb, r) ; // PRQA S 3000

        tot += r ;
    } 
    ACS_COND_THROW(fd < 0, InvalidOperation ("Error reading file: " + completePath, errno)) ; // PRQA S 3081
    fdguard.release() ; // close now ;

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File: " << completePath << " read successfully [ size: " << tot << "]") ;

    string queryString = "UPDATE " + tableName + " SET " + columnName + "=  E'" +  createEscapeSequence(buffer, tot) + "' " ; // PRQA S 3000
    if ( ! whereCondition.empty() ) { queryString += " WHERE " + whereCondition ; }
    SQLString sqlString(conn->getKey(), SQLString::COMMAND) ;
    sqlString.forceSQLString(queryString);

    return uploadBlobClob(conn, sqlString);
}


size_t Postgres8SQLDialect::uploadBlob(dbConnection * conn, const SQLString & sqlString, const string& IGNORED_completePath ) 
{
    // 1) completePath is required by interface, but is not required on informix
    // 2) The command must contain if is a blob or a clob, so no futher parameter is required.    
    return uploadBlobClob(conn, sqlString);
}


size_t Postgres8SQLDialect::uploadClob(dbConnection * conn, const SQLString & sqlString, const string& IGNORED_completePath ) 
{
    // 1) completePath is required by interface, but is not required on informix
    // 2) The command must contain if is a blob or a clob, so no futher parameter is required.    
    return uploadBlobClob(conn, sqlString);
}


size_t Postgres8SQLDialect::uploadBlobClob(dbConnection * conn, const SQLString & sqlString)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "QUERY: " << sqlString.getSQL()) ;

    dbQuery q(*conn, sqlString, "lob upld") ;

	bool status=false ;
    
    try
    {
        status = q.execForStatus() ;

        ACS_COND_THROW( (false==status), InvalidOperation( "Invalid status after execForStatus: " + sqlString.getSQL() )) ; // PRQA S 3081
    }
    catch (exception& e)
    {
        ACS_THROW ( InvalidOperation(e, "Cannot upload file") ) ;
    }
    
  
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "execForStatus successfully") ;

    return q.getAffectedRows() ;
}


size_t Postgres8SQLDialect::zeroBlob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition ) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method called\n"
            << "\tTableName: !" << tableName << "!\n"
            << "\tColumnName: !" << columnName << "!\n"
            << "\tWhereCondition: !" << whereCondition << "!") ;

   
    const signed int lo_unlink_success = 1 ;
    PGconn* connectionPointer = getConnectionPointer(conn) ;
    Oid oid = getOid(conn, tableName, columnName, whereCondition ) ; 

    
	/* Begin transaction */
	TransactionHandler transactionHandler (connectionPointer) ;

    /* Executing the lo_unlink command */
    int status  = lo_unlink(connectionPointer, oid) ;
    if ( status != lo_unlink_success )
    {
        ostringstream s;
        s << "Error running lo_unlink on oid: " << oid << " [" << status << "]" ;
		ACS_THROW (InvalidOperation (s.str())); // PRQA S 3081
	}
  
  
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Oid: " << oid << " removed succesfully, status: " << status) ;


    size_t ret = setNullOid(conn, tableName, columnName, whereCondition) ;


	transactionHandler.commit() ;	/* commit transaction*/


	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end method") ;

    return ret ;
}


size_t Postgres8SQLDialect::zeroClob(dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition ) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
    return 0 ;  // Since doesn't required specific operation. Standard function will be used.
}


bool Postgres8SQLDialect::blobIsNull( dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, bool& ) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    return 0 ;  // Since doesn't required specific operation. Standard function will be used.
}


bool Postgres8SQLDialect::clobIsNull( dbConnection * conn, const string &tableName, const string& columnName, const string& whereCondition, bool& ) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
    return false ;  // Since doesn't required specific operation. Standard function will be used.
}


void Postgres8SQLDialect::installConnectionCallback(dbConnection *) 
{
}

void Postgres8SQLDialect::uninstallConnectionCallback() 
{
}

unsigned short Postgres8SQLDialect::getTimeStampResolution() const // PRQA S 4020
{
	if ( 0 == _timeStampResolution) { // PRQA S 3000
		const Plugin *dbx = dbDriver::instance(_key)->getPlugin() ;
		if (!dbx) {
            ACS_LOG_WARNING("cannot find dbExpress shared obj handle from dbDriverSingleton") ;
			return _timeStampResolution = 6  ; // usec -- THIS IS THE LEGACY BEHAVIOUR // PRQA S 4400
		}
		unsigned short *drv_fraction_digits = reinterpret_cast<unsigned short *>(dlsym(dbx->handle(),"ctstmp_fraction_digits")) ; // PRQA S 3081, 4412

		if(drv_fraction_digits) { // found fraction digits in driver
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "*drv_fraction_digits: " << * drv_fraction_digits) ;
			
			return _timeStampResolution=* drv_fraction_digits ;
		}
	}

	return _timeStampResolution ;
}


dbSpecificColumnType::Type Postgres8SQLDialect::guessTypeFilter (dbSpecificColumnType::Type const & type, string const & dbType) const
{
   return type ;
}

std::string Postgres8SQLDialect::convertColumnToStringFilter(std::string const &c, dbSpecificColumnType::Type const &dbType) const // PRQA S 4020
{
    switch(dbType) {
    case dbSpecificColumnType::GEOOBJECT: 
        { return "ST_AsText(" + c + ")" ; }
    default:
        { return c ; }
    }
}

bool Postgres8SQLDialect::propTypeBlobClobFilter (std::string const & proprietarytype) const  // PRQA S 4020
{
	if (proprietarytype == "text") {
		return true ;
    }
	if (proprietarytype == "oid") {
		return true ;
    }
	return false ;
}

bool Postgres8SQLDialect::propTypeClobFilter (std::string const & proprietarytype) const  // PRQA S 4020
{
	return proprietarytype == "text" ;
}
bool Postgres8SQLDialect::propTypeJsonbFilter (std::string const & proprietarytype) const  // PRQA S 4020
{
	return proprietarytype == "jsonb" ;
}
bool Postgres8SQLDialect::propTypeBlobFilter (std::string const & proprietarytype) const  // PRQA S 4020
{
	return proprietarytype == "oid" ;
}

string Postgres8SQLDialect::intervalToSeconds(const std::string &intv) const
{
	return intervalToSeconds("INTERVAL \'0\'", intv) ;
}

string Postgres8SQLDialect::intervalToSeconds(const std::string &intv_beg, const std::string &intv_end) const
{
	ostringstream o ; 
	o << "(EXTRACT (EPOCH FROM ( " << intv_end << " - " << intv_beg << " )))" ;

	return o.str();
}

SQLDialect::ptrExecSQL Postgres8SQLDialect::returnExecForSet(SQLString const& sqlString, ptrExecSQL pExecForSet, ptrExecSQL) const
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning Default PTR (pExecForSet)") ;

#if(!0)
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UNIMPLEMENTED. Returning Default") ;
#endif

	return pExecForSet ;
} 


SQLDialect::ptrExecSQL Postgres8SQLDialect::returnExecForStatus(SQLString const& sqlString, ptrExecSQL pExecForStatus, ptrExecSQL) const
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

#if(!0)
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "UNIMPLEMENTED. Returning Default") ;
#endif

	return pExecForStatus ;
} 

int Postgres8SQLDialect::executeSpecificCommandVirtual(	void** ,		/* void*[], list of I/O parameters */ 
														unsigned char npar,	/* Number of parameters */
														signed short int cmd) const /* Command */
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "cmd: " << hex << cmd << dec << " npar: " << int(npar)) ; // PRQA S 3081
 
	return 0 ;
}


/*---------------------------------------------------------------------------------------------------------
			startListener
---------------------------------------------------------------------------------------------------------*/
void Postgres8SQLDialect::startListener (dbConnection *conn, const string& channel) const 
{
	/**
		Start a listener to a specific channel
	*/

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting listener for channel: \"" << channel << "\"") ;

#if(0)
	// THIS BLOCK MITIGATES CVE-2018-1058 
	// /see https://wiki.postgresql.org/wiki/A_Guide_to_CVE-2018-1058:_Protect_Your_Search_Path
	// I assume we don't care about it. Anyway, the query should be executed with "execForStatus" and not with libpq raw code.
	 
	/* Set always-secure search path, so malicious users can't take control. */    
	{
		PGconn* connectionPointer = getConnectionPointer(conn) ;
		ACS_COND_THROW( (NULL == connectionPointer), InvalidOperation("NULL Connection Pointer") ) ; 
		PGresult* res = NULL ;
		res = PQexec(connectionPointer, "SELECT pg_catalog.set_config('search_path', '', false)");
		PGresultGuard res_guard(res) ;	// Guard to deallocate PGresult. DO NOT explicitly call PQclear !
		if (PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			ACS_THROW(InvalidOperation("Unable to set search_path: " + PQerrorMessage(connectionPointer)) ; 
		}
	}
#endif


	/*
	 * Issue LISTEN command to enable notifications from the rule's NOTIFY.
	 */
	{
		const string cmd ( string("LISTEN") + " " + channel) ; 
	    SQLString sqlString(conn->getKey(), SQLString::COMMAND) ; sqlString.forceSQLString(cmd) ; 

	    dbQuery q(*conn) ; q.sqlString(sqlString, "listen notifications") ;
		bool status = q.execForStatus() ;
		
		ACS_COND_THROW( (false==status), InvalidOperation("Unable to execute command: \"" + cmd + "\"")) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method finished. Now listening to channel: \"" + channel + "\"") ;
}


/*---------------------------------------------------------------------------------------------------------
			listenToMessages
---------------------------------------------------------------------------------------------------------*/
#pragma GCC diagnostic ignored "-Wold-style-cast" /* EC:: LOCALLY DISABLING THE WARNING: -Wold-style-cast */
bool Postgres8SQLDialect::listenToMessages (dbConnection *conn, vector<string>& messages_V, const uint64_t& timeout_usec) const 
{
	/**
		Fill a vector, returned by the listener
		Note that: 
		- The vector will be cleaned at the beginning of the call
		- This methods use a "select", so it sleeps until success, error or timeout-elapsed.
		- Only in case of success it returns true.
		- The caller can perform a simple while-loop on listenToMessages with timeout e.g. 1 sec: the method will immediately return in case of success, 
		  or anyway (unsuccess) within 1 sec.
	*/

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB) ;

	messages_V.clear() ; 

	PGconn* connectionPointer = getConnectionPointer(conn) ;
	ACS_COND_THROW( (NULL == connectionPointer), dbConnection::ConnectionException("NULL Connection Pointer") ) ; 
 
	const int sock ( PQsocket(connectionPointer) ) ; // Obtains the file descriptor number of the connection socket to the server. 
	ACS_COND_THROW( (sock < 0), dbConnection::ConnectionException("Unable to obtain the connection socket") ) ;

	fd_set input_mask;
	FD_ZERO(&input_mask); 		// Raising compile-time warning ...
	FD_SET(sock, &input_mask);

 	struct timeval timeout ;
	timeout.tv_sec =	(timeout_usec / 1000000LL) ;	// sec part   (e.g. 2'500'000 -> 2 sec)
	timeout.tv_usec =	(timeout_usec % 1000000LL) ;	// usec part  (e.g. 2'500'000 -> 500'000 usec)   
  
  	/* Sleep until either: a message has been found, an error occurs or timeout is reached */
  	const int sel ( select(sock + 1, &input_mask, NULL, NULL, &timeout) ) ;
	if (sel < 0)
	{
		// Select failure: it should never happen ... ...
		ACS_LOG_WARNING( exDebugSignatureThread << "Illegal value returned by select") ;
		return false ;	
	}
	else if ( FD_ISSET(sock, &input_mask) )
	{
		// Select successfull, extract the payload
         PQconsumeInput(connectionPointer);	// Frees the select (move the list of notifications to a buffer). PQnotifies will read the buffer.
	  
#if(!0)
{
// FIXME TBD. SEE "Client 4 An Interactive Query Processor" of book: PostgreSQL: The comprehensive guide to building, programming, and administering PostgreSQL databases, Second Edition
const int s = PQisBusy( connectionPointer ) ; 
// if s != 0, the method should return FALSE and try again -- it means that the select only transferred a part of the total block.
if (s != 0) { excerr << "FIXME MESSAGE -- BUSY RETURNED TRUE -- NEXT CALL WILL HAVE TO WAIT" << endl ; /* return false ; */  }
}
#endif	  

	     PGnotify* notify = NULL ;	// Pointer -- REMEMBER TO ALWAYS EXPLICITLY CALL PQfreemem TO FREE THE MEMORY ALLOCATED BY PQnotifies
         while ((notify = PQnotifies(connectionPointer)) != NULL)
         {
		 	/* Extract all the notifications transferred to the buffer from PQconsumeInput */
// fprintf(stderr, "FIXME MESSAGE FROM Postgres8SQLDialect   CHANNEL '%s' message received from backend PID %d  \"%s\"\n", notify->relname, notify->be_pid, notify->extra);
					 
			ostringstream json_msg ; 
			DateTime time_now ; DateTime::MJD2000 time_now_mjd2000(time_now) ; 
			const uint64_t time_now_mjd2000_usec ( time_now_mjd2000.jd() * 86400.0 * 1000000.0 ) ; /* Number of usec from 01-01-2000. 
																		E.g. 3456000000001 = 345600 s + 1 usec = 4 days + 1 usec = Jan 05 2000, 00:00:00.000001 */
			json_msg 
						<< "{ " \
						<< "\"driver_name\": \"" << dialectName() << "\", " \
						<< "\"channel\": \"" << string(notify->relname) << "\", " \
						<< "\"counter\": " << _notify_counter++ << ", " \
						<< "\"timestamp\": " << time_now_mjd2000_usec << ", " \
						<< "\"payload\": " << string(notify->extra) \
						<< " }" ;
					 
			messages_V.push_back( json_msg.str() ) ; 

			PQfreemem(notify);	// Deallocate the notify

			// PREPARE NEXT LOOP -- DO NOT MOVE FROM HERE !
			PQconsumeInput(connectionPointer);	// Frees the select (move the list of notifications to a buffer). PQnotifies will read the buffer.

		}	// END: while
		
		// WHEN HERE: notify is NULL, so no PQfreemem is needed (the PQfreemem has been already called in the loop above)
     }
	else
	{
		// Select timeout
		return false ; 
	}

	return (messages_V.size() > 0) ; 
}
#pragma GCC diagnostic warning "-Wold-style-cast" /* EC:: RE-ENABLING THE WARNING: -Wold-style-cast */


string Postgres8SQLDialect::gisVersionQuery() const { return "SELECT PostGIS_Version()::VARCHAR ;" ; }



string Postgres8SQLDialect::dump(const vector<string> &v) 
{
	ostringstream o ; 
	for (size_t i=0; i<v.size(); i++)
	{
		o << v.at(i) << endl ;	
	}

	return o.str() ; 
} 



_ACS_END_NAMESPACE
