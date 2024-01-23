// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
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
	Revision 5.19  2016/09/20 16:12:11  enrcar
	CSGACQ-96: EC:: dialect-specific method getTableNameForEmptyFROM added in order to handle SELECT without FROM
	
	Revision 5.18  2016/07/29 13:31:22  enrcar
	CSGACQ-87  EC:: translateSQLCast I/F slightly modified
	
	Revision 5.17  2016/07/26 15:09:33  enrcar
	CSGACQ-87 EC:: Added support and test for CASE WHEN ELSE and TIME INTERVAL
	
	Revision 5.16  2016/07/21 14:25:47  enrcar
	CSGACQ-82  EC:: intervalToSeconds interface added
	
	Revision 5.15  2016/07/14 09:48:48  marfav
	Adding table alias support also in join clauses
	
	Revision 5.14  2016/06/21 08:34:39  damdec
	clearOrderBy method added.
	
	Revision 5.13  2016/04/29 12:16:19  marpas
	fixed addFunctionParameter and addProcedureParameter bug when used with a string (or const char *)
	
	Revision 5.12  2016/03/15 09:21:06  marpas
	qa warnings fixed and minor changes
	
	Revision 5.11  2016/03/15 09:11:47  marpas
	moving into dialect (getLimitedQueryWrapped) the responsibility to provide its own wrapped sql string
	
	Revision 5.10  2016/03/14 15:39:14  marpas
	introducing offset in queries
	
	Revision 5.9  2015/09/30 09:09:19  enrcar
	EC:: New type of query: UNION handled
	
	Revision 5.8  2015/08/28 13:16:39  marfav
	Added support for update using a nested SQLString as value
	
	Revision 5.7  2015/05/08 09:53:02  davide.tiriticco
	typo fixed
	
	Revision 5.6  2015/03/19 14:36:55  marpas
	qa warnings removed
	
	Revision 5.5  2015/02/17 14:16:24  enrcar
	EC:: Added support (and public method setValuesList) to handle the Token NESTED_VALUES_LIST, that is a nested SELECT to provide the columns value of an INSERT
	
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.2  2013/06/13 18:10:54  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:56  marpas
	adopting libException 5.x standards
	
	Revision 4.18  2013/04/11 17:01:40  marpas
	too many ellipses: one pair removed
	
	Revision 4.17  2013/04/11 09:54:36  marpas
	addValuesList accept an SQLString as value. In this case encloses it in ellipsis.
	
	Revision 4.16  2013/02/27 09:59:15  marpas
	ql string limit vaues are now size_t
	
	Revision 4.15  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.14  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.13  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.12  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.4  2012/02/22 10:35:08  marpas
	refactoring
	
	Revision 4.3  2012/02/21 10:40:44  marpas
	refactoring
	
	Revision 4.2  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.10  2011/10/14 07:49:45  marpas
	FORREADONLY token added
	
	Revision 3.9  2011/03/31 15:50:32  lucio.pulvirenti
	EC:: Oracle: LIMIT now correctly handled if concurrent "ORDER BY" by using a subquery
	
	Revision 3.8  2010/09/27 13:04:55  lucio.pulvirenti
	EC:: optional alias on table list supported. Multiple JOINs supported.
	
	Revision 3.7  2010/09/11 18:11:31  marpas
	libException 3.3 interface adopted
	
	Revision 3.6  2010/05/24 12:49:19  lucio.pulvirenti
	EC: PL/SQL queries now handled
	
	Revision 3.5  2010/03/15 15:05:08  marpas
	transaction name can be passed trough beginTransaction method
	
	Revision 3.4  2010/02/26 16:08:29  crivig
	translateColumnName used in addSelectList
	
	Revision 3.3  2010/02/10 17:19:13  enrcar
	EC:: LIMIT inside the WHERE clause (e.g. Oracle dialect): now handled
	
	Revision 3.2  2010/02/02 18:26:12  enrcar
	EC:: getStatementTerminator method handled
	
	Revision 3.1  2009/10/29 13:11:33  enrcar
	EC:: addSelectList now handles the SQLDialect::SQLCast optional parameter
	
	Revision 3.0  2009/10/05 14:47:48  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.18  2009/06/16 13:09:21  marpas
	SQL join syntax implemented
	
	Revision 2.17  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.16  2008/11/10 16:21:59  marpas
	dbCursor implemented
	
	Revision 2.15  2008/11/04 15:52:36  marpas
	bitwise operators finalized
	
	Revision 2.14  2008/09/02 14:33:09  enrcar
	EC:: AddSelectList. Optional parameter "label" handled
	
	Revision 2.13  2008/05/19 14:27:44  enrcar
	EC:: Support for custom procedures added
	
	Revision 2.12  2008/05/08 16:36:20  enrcar
	EC:: ColumnTraslationMap method handled
	
	Revision 2.11  2008/04/16 13:54:48  enrcar
	EC:: Support for Custom-Functions added. Selects accept a SQLString as column name
	
	Revision 2.10  2008/04/09 13:19:55  enrcar
	EC:: SQLSelectList, SQLOrdGrpByList delimiters handled
	
	Revision 2.9  2007/12/17 10:44:32  paoscu
	Added support to retrieve information on dependant (child) tables.
	
	Revision 2.8  2007/12/11 13:10:04  enrcar
	EC:: getTypeAsString method added
	
	Revision 2.7  2007/10/16 13:10:24  enrcar
	EC:: Added temporary patch to handle MYSQL-modified schema. FIXME.
	
	Revision 2.6  2006/12/13 18:13:10  paoscu
	Added methods to understand if a LO field is NULL.
	
	Revision 2.5  2006/04/05 14:03:39  enrcar
	operator << added
	
	Revision 2.4  2006/04/04 08:47:01  enrcar
	Nested functions handled into dialect
	
	Revision 2.3  2006/04/03 16:36:19  enrcar
	FUNCTION added into QueryType
	addFunctionParameter, addFunctionParameterString, setFunctionName: methods added
	
	Revision 2.1  2006/03/10 17:31:58  marpas
	bug on query parameters in case of empty params fixed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.27  2006/01/04 18:08:58  enrcar
	empty commands handled
	
	Revision 1.26  2005/12/22 16:32:44  enrcar
	FIRST token updated to handle C-Style format descriptors
	
	Revision 1.25  2005/12/19 09:36:11  enrcar
	buildSQLQuery handles nested queries
	
	Revision 1.24  2005/11/30 19:50:00  enrcar
	LOBJs handling: interface changed to avoid memory loss
	
	Revision 1.23  2005/11/30 15:40:59  enrcar
	getCommandAndFileName(), setCommandAndFileName()
	Methods added. Allow upload/download LOBJs using an explicit SQLString
	
	Revision 1.22  2005/11/24 09:47:00  enrcar
	SQLString::QueryType structure: Large object upload/download operations renamed as 
	LOBJ_UPLOAD, LOBJ_DOWNLOAD to make code more readable (was: BLOB_UPLOAD, BLOB_DOWNLOAD)
	
	Revision 1.21  2005/10/25 07:54:02  marpas
	temporary added ctor for const char *
	
	Revision 1.20  2005/10/21 09:08:57  enrcar
	modified for informix
	
	Revision 1.19  2005/10/10 12:47:39  enrcar
	temporary patch added
	
	Revision 1.18  2005/09/29 16:36:04  enrcar
	get methods added for blob handling
	
	
	Revision 1.15  2005/09/08 12:05:24  paoscu
	New methods:addANDWhereParameter(string const &), addORWhereParameter(string const &) and metadataQuery
	
	Revision 1.14  2005/07/22 11:57:19  enrcar
	getOrderBy, getWhereParameter methods added
	
	Revision 1.13  2005/07/21 09:38:51  enrcar
	addWhereParameter method removed
	addORWhereParameter and addANDWhereParameter methods added
	
	Revision 1.12  2005/07/20 15:24:02  enrcar
	DELETE command handled
	
	Revision 1.11  2005/07/20 13:24:54  enrcar
	OrderBy interface changed. Now handles ASC/DESC or DEFAULT for every sort-item
	
	Revision 1.10  2005/07/19 13:16:23  enrcar
	INSERT (and relative tokens) now handled
	
	Revision 1.9  2005/07/19 12:07:23  enrcar
	DROP Method handled
	
	Revision 1.8  2005/07/19 07:55:23  enrcar
	Added debug level
	Added usedtoken vector
	
	Revision 1.7  2005/07/18 08:21:51  enrcar
	UPDATE method handled
	
	Revision 1.6  2005/07/14 13:14:03  enrcar
	Added OrderBy ASC/DESC string.
	Added SQLString with (explicit) string ctor
	
	Revision 1.5  2005/07/11 13:31:31  enrcar
	reset,setType  methods added
	
	Revision 1.4  2005/07/11 13:10:15  enrcar
	bug fixed
	
	Revision 1.3  2005/07/08 16:37:52  enrcar
	*** empty log message ***
	
	Revision 1.2  2005/07/07 14:27:35  enrcar
	GroupBy, OrderBy, Having now handled
	
	Revision 1.1  2005/07/06 14:51:18  enrcar
	SQLString added
*/


#include <RegExp.h>
#include <SQLString.h>
#include <SQLDialectFactory.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SQLString);


string SQLString::SQLCase::toString(const std::string& key) const 
{ 
	return SQLDialectFactory::sqlDialect(key).caseWhenElse(_paramsV, _else) ; 
}

SQLString::SelectListContainer::SelectListContainer() :
	_sqlCast(),
	_item(),
	_lbl() 
{ /* Empty c'tor */ }

SQLString::SelectListContainer::SelectListContainer(
        const string& item, 
        SQLDialect::SQLCast const &sqlCast, 
        const string& lbl) :
	_sqlCast(sqlCast),
	_item(item),
	_lbl(lbl)
{ /* Empty c'tor */ }


SQLString::SelectListContainer::SelectListContainer(const SelectListContainer &r) :
	_sqlCast(r._sqlCast),
	_item(r._item),
	_lbl(r._lbl)
{}

SQLString::SelectListContainer& SQLString::SelectListContainer::operator= (const SelectListContainer &r) 
{
    if (this != &r) {
	    _sqlCast = r._sqlCast ;
	    _item = r._item ;
	    _lbl = r._lbl ;
    }
    return *this ;
}

SQLString::SelectListContainer::~SelectListContainer() throw() {}



SQLString::SQLString(const string &key, const char *s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key(key)
{	
}


SQLString::SQLString(const string &key, string const & s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key(key)
{	
}

SQLString::SQLString(const string &key, string & s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key(key)
{	
}


SQLString::SQLString(const string &key, SQLString::QueryType t, bool readOnly) :
    ThreadSafe(),
	_queryType(t),
	_forcedString(""),
	_sqlLimit(0),
    _sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(readOnly),
	_key(key)
{	
}
	

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
SQLString::SQLString(const char *s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key("")
{	
}



SQLString::SQLString(string const & s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key("")
{	
}



SQLString::SQLString(string & s) :
    ThreadSafe(),
	_queryType(COMMAND),
	_forcedString(s),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(false),
	_key("")
{	
}



SQLString::SQLString(SQLString::QueryType t, bool readOnly) :
    ThreadSafe(),
	_queryType(t),
	_forcedString(""),
	_sqlLimit(0),
	_sqlOffset(0),
	_sqlDistinct(false),
    _sqlFunctionName(),
    _sqlProcedureName(),
    _sqlSelectList(),
    _sqlUnionList(),
    _sqlTableList(),
    _sqlFunctionParamsList(),
    _sqlProcedureParamsList(),
    _sqlJoinParameters(),
    _sqlWhereParameters(),
    _sqlGroupByList(),
    _sqlHavingParameters(),
    _sqlOrderByList(),
    _sqlSetList(),
    _sqlValuesList(),
    _sqlNestedValuesList(),
    _sqlColumnAndFileName(),
	_sqlCommandAndFileName(0),
    _usedTokens(),
	_isReadOnlySelect(readOnly),
	_key("")
{	
}	
#endif	
	

SQLString::~SQLString() throw() 
{
	delete _sqlCommandAndFileName ; // PRQA S 4631
}



//------------------------------------------------------------------------
//                                ::Assignment
//------------------------------------------------------------------------
SQLString::SQLString(const SQLString &e) :
    ThreadSafe(),
    _queryType(e._queryType ),
    _forcedString(e._forcedString ),
    _sqlLimit(e._sqlLimit ),
    _sqlOffset(e._sqlOffset ),
    _sqlDistinct(e._sqlDistinct ),
    _sqlFunctionName(e._sqlFunctionName ),
    _sqlProcedureName(e._sqlProcedureName ),
    _sqlSelectList(e._sqlSelectList ),
    _sqlUnionList(e._sqlUnionList ),
    _sqlTableList(e._sqlTableList ),
    _sqlFunctionParamsList(e._sqlFunctionParamsList),
    _sqlProcedureParamsList(e._sqlProcedureParamsList),
    _sqlJoinParameters(e._sqlJoinParameters ),
    _sqlWhereParameters(e._sqlWhereParameters ),
    _sqlGroupByList(e._sqlGroupByList ),
    _sqlHavingParameters(e._sqlHavingParameters ),
    _sqlOrderByList(e._sqlOrderByList ), 
    _sqlSetList(e._sqlSetList ),
    _sqlValuesList(e._sqlValuesList ),
    _sqlNestedValuesList(e._sqlNestedValuesList ),
    _sqlColumnAndFileName(e._sqlColumnAndFileName ),
    _sqlCommandAndFileName(e._sqlCommandAndFileName ? new pair <SQLString, string>(*e._sqlCommandAndFileName) : 0), // PRQA S 3380, 3385
    _usedTokens(e._usedTokens ),
	_isReadOnlySelect( e._isReadOnlySelect ),
	_key( e._key )
 
{
    // empty
}


//------------------------------------------------------------------------
//                                 ::Copy
//------------------------------------------------------------------------
SQLString& SQLString::operator=(const SQLString &e)
{
    if (this != &e) // avoid auto assignation
    {
		// lock both for safety 
		ThreadSafe::Lock lock(*this) ;
		ThreadSafe::Lock lockRhs(e) ;
		// deep copy of all parameters
        _queryType = e._queryType ;
        _forcedString = e._forcedString ;
        _sqlLimit = e._sqlLimit ;
        _sqlOffset = e._sqlOffset ;
        _sqlDistinct = e._sqlDistinct ;
        _sqlSelectList = e._sqlSelectList ;
        _sqlUnionList = e._sqlUnionList ;
        _sqlTableList = e._sqlTableList ;
        _sqlWhereParameters = e._sqlWhereParameters ;
        _sqlJoinParameters = e._sqlJoinParameters ;
        _sqlGroupByList = e._sqlGroupByList ;
        _sqlHavingParameters = e._sqlHavingParameters ;
        _sqlOrderByList = e._sqlOrderByList ; 
        _sqlSetList = e._sqlSetList ;
        _sqlValuesList = e._sqlValuesList ;
        _sqlNestedValuesList = e._sqlNestedValuesList ;
        _sqlColumnAndFileName = e._sqlColumnAndFileName ;
        _sqlFunctionName = e._sqlFunctionName ;
        _sqlFunctionParamsList = e._sqlFunctionParamsList;
        _sqlProcedureName = e._sqlProcedureName ;
        _sqlProcedureParamsList = e._sqlProcedureParamsList;
		// deep copy 
    	delete _sqlCommandAndFileName ;
        _sqlCommandAndFileName = e._sqlCommandAndFileName ? new pair <SQLString, string>(*e._sqlCommandAndFileName) : 0 ; // PRQA S 3380, 3385
        _usedTokens = e._usedTokens ;
		_isReadOnlySelect= e._isReadOnlySelect ;
		_key= e._key ;

    }
	// return the object
    return *this;
}


/*********************************************************************************/
/*  BEGIN  SET (RESET) METHODS:                                                  */
/*********************************************************************************/

void SQLString::reset()
{
	ThreadSafe::Lock lock(*this) ;
    /*
        All private variables are set to default status
        except _queryType (no default status).
        Can be set using setType method
    */

    _forcedString = "" ;
    _sqlLimit = 0 ;
    _sqlOffset = 0 ;
    _sqlDistinct = false ;
    _sqlSelectList.clear();
    _sqlUnionList.clear();
    _sqlTableList.clear() ;
    _sqlWhereParameters = "" ;
    _sqlJoinParameters.clear() ;
    _sqlGroupByList.clear() ;
    _sqlHavingParameters = ""  ;
    _sqlOrderByList.clear() ;
    _sqlSetList.clear() ;
    _sqlValuesList.clear() ;
    _sqlNestedValuesList.first.clear(); _sqlNestedValuesList.second = "" ; 
    _sqlFunctionName = "" ;
    _sqlFunctionParamsList.clear() ;
    _sqlProcedureName = "" ;
    _sqlProcedureParamsList.clear() ;
    _sqlColumnAndFileName = pair <string, string> ("", "") ;
	_isReadOnlySelect= false ;
	
	// reset pointer
	delete _sqlCommandAndFileName ;
    _sqlCommandAndFileName = 0 ;
    _usedTokens.clear() ;
}


void SQLString::forceSQLString( string const & s) 
{
	ThreadSafe::Lock lock(*this) ;

    if ( _queryType != SQLString::COMMAND )
    {
        ostringstream errorMessage;
        errorMessage << "SQLString::forceSQLString. Cannot force SQL Command if class not proper configured: " << _queryType ;
        ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
    }
    
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << s << "\"") ;

    _queryType = COMMAND ; // FIXME TDB. Are we sure ?!
    _forcedString = s ;

    return ;
}


void SQLString::setOrderByString(const string& s) // PRQA S 4020
{
    if (s.empty()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called with empty string") ;
        return ;
    }
	ThreadSafe::Lock lock(*this) ;
    /**
        Must be used to manually set a OrderBy string.
        String should be obtained using getOrderByString method.
          
        THIS METHOD MUST NOT BE USED WITH STANDARD addOrderByList INTERFACE !!
    */
    
    // To keep code simpler,
    // _sqlOrderByList map is used to store the whole string.
    
    // Map is cleared 
    _sqlOrderByList.clear() ;
    
    // Pair is created as SORT_DEFAULT, since ASC/DESC must be inserted into string
    pair <string, OrderBy> tmp(s, SQLString::SORT_DEFAULT) ;
    
    // A single row is inserted into OrderBy Map
	_sqlOrderByList.push_back(tmp) ;
}

void SQLString::copyJoinParameters(SQLString& sql) {
	// Lock the resources and copy join parameters
	bool toSet = true;
	while (toSet) {
		ThreadSafe::Lock lock(*this) ;
		ThreadSafe::TryLock lock2(sql);
		if (lock2.locked()) {
			toSet = false;
			sql._sqlJoinParameters.clear();
			sql._sqlJoinParameters = _sqlJoinParameters;
			sql.addToUsedTokenList(SQLDialect::JOIN);
		}
		else {
			usleep(10);
		}
	}
}

void SQLString::setReadOnlySelect(bool v) // PRQA S 4121
{
	_isReadOnlySelect = v ;
}

void SQLString::setType(SQLString::QueryType t, bool readOnly)
{
	ThreadSafe::Lock lock(*this) ;
	
	_queryType= t ;
	_isReadOnlySelect = readOnly ;
}


void SQLString::setLimit(size_t l)
{
	ThreadSafe::Lock lock(*this) ;
	
    addToUsedTokenList(SQLDialect::LIMIT) ;
	_sqlLimit=l ;		
}

void SQLString::setOffset(size_t o)
{
	ThreadSafe::Lock lock(*this) ;
	
    addToUsedTokenList(SQLDialect::OFFSET) ;
	_sqlOffset=o ;		
}


void SQLString::setDistinct(bool d)
{
	ThreadSafe::Lock lock(*this) ;
	addToUsedTokenList(SQLDialect::DISTINCT) ;
	_sqlDistinct= d ;
}


// Overloading c'tor
void SQLString::addSelectList(string const &s, string const &lbl)
{
    SQLDialect::SQLCast void_sqlCast ;	/* Create a void/UNDEFINED SQLCast */

    addSelectList(s, void_sqlCast, lbl) ;
	return ;
}

// Overloading c'tor
void SQLString::addSelectList(SQLString const &sqls, string const &lbl)
{
    SQLDialect::SQLCast void_sqlCast ;	/* Create a void/UNDEFINED SQLCast */
	
    addSelectList(sqls, void_sqlCast, lbl) ;
	return ;
}


void SQLString::addSelectList(string const &s, SQLDialect::SQLCast const &sqlCast, string const &lbl)
{
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::SELECT_LIST) ; 
	  
	SelectListContainer tmp( SQLDialectFactory::sqlDialect(_key).translateColumnName(s), sqlCast, lbl) ;
	_sqlSelectList.push_back(tmp) ;
}

void SQLString::addSelectList(SQLString const &sqls, SQLDialect::SQLCast const &sqlCast, string const &lbl)
{

	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::SELECT_LIST) ; 

    string s = sqls.getSQL(true) ;

	SelectListContainer tmp(s, sqlCast, lbl) ;
	_sqlSelectList.push_back(tmp) ;
}

void SQLString::addUnionList(SQLString const &sqls, bool union_all)
{
	if ( (sqls.getType() != SELECT) && (sqls.getType() != COMMAND) && (sqls.getType() != UNION) )
	ACS_THROW( Exception( "SQLString::addUnionList:  INVALID Query type" )) ; // PRQA S 3081

	string s = sqls.getSQL(true) ;

	addUnionList(s, union_all) ; 
}


void SQLString::addUnionList(const string& sqls, bool union_all)
{
	ThreadSafe::Lock lock(*this) ;
	addToUsedTokenList(SQLDialect::UNION_LIST) ; 

	_sqlUnionList.push_back( pair<string, bool>(sqls, union_all) ) ;
}


void SQLString::addSetList(string const &k, SQLString const &s)
{
    // Used for UPDATE (SQL Standard)...
	ThreadSafe::Lock lock(*this) ;
	const bool nested = true;

	addSetList (k, s.getSQL(nested) );
}

void SQLString::addSetList(string const &k, string const &v)
{
    // Used for UPDATE (SQL Standard)...
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::SET_LIST) ;
    pair <string, string> tmp( SQLDialectFactory::sqlDialect(_key).translateColumnName(k), v );
    
	_sqlSetList.push_back(tmp) ;
}


void SQLString::addFunctionParameterString(const string &v) 
{
	ThreadSafe::Lock lock(*this) ;
    
    if (getType() == FUNCTION) {
    	addToUsedTokenList(SQLDialect::FUNCTIONARG) ; 
	}
    else {
	    addToUsedTokenList(SQLDialect::CUSTFUNCTIONARG) ; 
    }
    // add a new parameter
    _sqlFunctionParamsList.push_back(v);
}


void SQLString::addProcedureParameterString(const string &v) 
{
	ThreadSafe::Lock lock(*this) ;
    
    addToUsedTokenList(SQLDialect::CUSTPROCEDUREARG) ; 
    
    _sqlProcedureParamsList.push_back(v);
}


void SQLString::setColumnName(string const &k)
{
    // Used for setting the column name to check if blob/clob is Null. JUST A CONTAINER (not SQL)
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::COLUMNNAME) ;
    pair <string, string> tmp(k, "") ;
    _sqlColumnAndFileName = tmp ;
}


void SQLString::setColumnAndFileName(string const &k, string const &v)
{
    // Used for setting the filename and the column to be stored/retrived. JUST A CONTAINER (not SQL)
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::FILENAME) ;
    pair <string, string> tmp(k,v) ;
    _sqlColumnAndFileName = tmp ;
}

void SQLString::setCommandAndFileName(SQLString const &k, string const &v_optional)
{
    // Since fileName can be unknown, it is optional 
    
    // Used for setting the filename and the column to be stored/retrived. JUST A CONTAINER (not SQL)
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::FILENAME) ; // TDB FIXME
   	delete _sqlCommandAndFileName ; _sqlCommandAndFileName = 0 ;
    _sqlCommandAndFileName =  new pair <SQLString, string>(k,v_optional) ;
}

void SQLString::setFunctionName(const string& k)
{
    // Used for setting the filename and the column to be stored/retrived. JUST A CONTAINER (not SQL)
	ThreadSafe::Lock lock(*this) ;
    if (getType() == FUNCTION) {
	    addToUsedTokenList(SQLDialect::FUNCTIONNAME) ; 
    }
    else {
    	addToUsedTokenList(SQLDialect::CUSTFUNCTIONNAME) ; 
    }
    _sqlFunctionName = k ;
}

void SQLString::setProcedureName(const string& k)
{
    // Used for setting the filename and the column to be stored/retrived. JUST A CONTAINER (not SQL)
	ThreadSafe::Lock lock(*this) ;
 
    addToUsedTokenList(SQLDialect::CUSTPROCEDURENAME) ; 

    _sqlProcedureName = k ;
}

void SQLString::addValuesList(string const &k, string const &v)
{
    // Used for INSERT (SQL Standard)...
	ThreadSafe::Lock lock(*this) ;
    ACS_COND_THROW(!_sqlNestedValuesList.first.empty(), Exception( "SQLString::addValuesList:  CONFLICT WITH setValuesList" )) ; // PRQA S 3081
    addToUsedTokenList(SQLDialect::VALUES_LIST) ;
    pair <string, string> tmp( SQLDialectFactory::sqlDialect(_key).translateColumnName(k), v );
	_sqlValuesList.push_back(tmp) ;
}


void SQLString::addValuesList(string const &k, SQLString const &v)
{
    // Used for INSERT (SQL Standard)...
	const bool nested = true ; 
	ThreadSafe::Lock lock(*this) ;
    ACS_COND_THROW(!_sqlNestedValuesList.first.empty(), Exception( "SQLString::addValuesList:  CONFLICT WITH setValuesList" )) ; // PRQA S 3081
    addToUsedTokenList(SQLDialect::VALUES_LIST) ;
    pair <string, string> tmp( SQLDialectFactory::sqlDialect(_key).translateColumnName(k),v.getSQL(nested));
	_sqlValuesList.push_back(tmp) ;
}


void SQLString::setValuesList(vector<string> const &colv_in, SQLString const &v)
{
	// Used for INSERT (SQL Standard) with a NESTED QUERY (SELECT) TO PROVIDE THE VALUES OF THE COLUMNS ...
	const bool nested = true ; 
	ThreadSafe::Lock lock(*this) ;
	ACS_COND_THROW(!_sqlValuesList.empty(), Exception( "SQLString::setValuesList:  CONFLICT WITH addValuesList" )) ; // PRQA S 3081
	addToUsedTokenList(SQLDialect::NESTED_VALUES_LIST) ; 
	vector <string> colv ; 
	for (size_t i=0; i<colv_in.size(); i++)
	{
		colv.push_back ( SQLDialectFactory::sqlDialect(_key).translateColumnName( colv_in.at(i) ) ) ; 
	}
	pair <vector<string>, string> tmp( colv, v.getSQL(nested));
	_sqlNestedValuesList = pair <vector<string>, string>( colv, v.getSQL(nested) );
}


void SQLString::addTableList(string const &s, string const &lbl)
{
	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::TABLE_LIST) ;
	
	pair <string, string> tmp(s, lbl) ;
	
	_sqlTableList.push_back( tmp ) ;
}


void SQLString::addGroupByList(string const &s)
{
 	ThreadSafe::Lock lock(*this) ;
	addToUsedTokenList(SQLDialect::GROUPBY) ;
    _sqlGroupByList.push_back( SQLDialectFactory::sqlDialect(_key).translateColumnName(s) ) ;
}


void SQLString::addOrderByList(string const &s, OrderBy const& o)
{
  	ThreadSafe::Lock lock(*this) ;
    addToUsedTokenList(SQLDialect::ORDERBY) ;
    pair <string, OrderBy> tmp( SQLDialectFactory::sqlDialect(_key).translateColumnName(s), o) ;
	_sqlOrderByList.push_back(tmp) ;
}

void SQLString::clearOrderBy()
{
  	ThreadSafe::Lock lock(*this) ;
	_sqlOrderByList.clear();

}
void SQLString::addANDWhereParameter(dbQueryParameters const &p)
{
	ThreadSafe::Lock lock(*this) ;
		
	if (!_sqlWhereParameters.empty() && ( p.parameterCount()!= 0))
	{
		_sqlWhereParameters += " AND " ; // AND with previous if any
	}
	
    addToUsedTokenList(SQLDialect::WHERE) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "params: \"" << p.getSqlString() << "\"") ;

	// new parameter
	_sqlWhereParameters += p.getSqlString() ; 

}


void SQLString::addORWhereParameter(dbQueryParameters const &p)
{
	ThreadSafe::Lock lock(*this) ;
		
	if (!_sqlWhereParameters.empty() && ( p.parameterCount() != 0 ) )
	{
		_sqlWhereParameters += " OR " ; // OR with previous if any
	}
	
    addToUsedTokenList(SQLDialect::WHERE) ;
	// new parameter
	_sqlWhereParameters += p.getSqlString() ; 

}


void SQLString::addANDWhereParameter(string const &p) // PRQA S 4020
{
    if (p.empty()) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called with empty string") ;
        return ;
    }
	ThreadSafe::Lock lock(*this) ;
		
	if (!_sqlWhereParameters.empty() && !p.empty())
	{
		_sqlWhereParameters += " AND " ; // AND with previous if any
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "params: \"" << p << "\"") ;

    addToUsedTokenList(SQLDialect::WHERE) ;
	// new parameter
	_sqlWhereParameters += p ; 

}


void SQLString::addANDWhereParameter(string const &p, SQLDialect::SQLCast const &sqlCast) 
{
	addANDWhereParameter( SQLDialectFactory::sqlDialect(_key).translateSQLCast(p, sqlCast) ) ;
}	


void SQLString::addORWhereParameter(string const &p)
{
	ThreadSafe::Lock lock(*this) ;
		
	if (!_sqlWhereParameters.empty() && !p.empty())
	{
		_sqlWhereParameters += " OR " ; // OR with previous if any
	}
	
    addToUsedTokenList(SQLDialect::WHERE) ;
	// new parameter
	_sqlWhereParameters += p ; 

}

void SQLString::addORWhereParameter(string const &p, SQLDialect::SQLCast const &sqlCast) 
{
	addORWhereParameter( SQLDialectFactory::sqlDialect(_key).translateSQLCast(p, sqlCast) ) ;
}	
	
void SQLString::addJoinList(SQLDialect::SQLJoinToken jt, string const &table, dbQueryParameters const &p) 
{
	ThreadSafe::Lock lock(*this) ;
	string tmp = SQLDialectFactory::sqlDialect(_key).translateJoinToken(jt) + " " + table + " ON " + p.getSqlString() ; 
	_sqlJoinParameters.push_back(tmp) ;
	addToUsedTokenList(SQLDialect::JOIN) ;
}

void SQLString::addJoinList(SQLDialect::SQLJoinToken jt, string const &table, string const& alias, dbQueryParameters const &p) 
{
	ThreadSafe::Lock lock(*this) ;
	string tmp = SQLDialectFactory::sqlDialect(_key).translateJoinToken(jt) + " " + table + " " + alias + " ON " + p.getSqlString() ; 
	_sqlJoinParameters.push_back(tmp) ;
	addToUsedTokenList(SQLDialect::JOIN) ;
}

void SQLString::addHavingParameter(dbQueryParameters const &p)
{
	ThreadSafe::Lock lock(*this) ;
		
	if (!_sqlHavingParameters.empty()) // having is always in and
	{
		_sqlHavingParameters += " AND " ; 
	}
	
    addToUsedTokenList(SQLDialect::HAVING) ; // token used
	_sqlHavingParameters += p.getSqlString() ; 

}


void SQLString::addToUsedTokenList(SQLDialect::SQLToken const& t)
{
    ThreadSafe::Lock lock(*this) ;
    
    _usedTokens.push_back( t ); 
}


/*********************************************************************************/
/*  END OF SET (RESET) METHODS.                                                  */
/*********************************************************************************/


/*********************************************************************************/
/*  BEGIN GET METHODS:                                                           */
/*********************************************************************************/


SQLString::QueryType SQLString::getType() const // PRQA S 4120
{
    return _queryType ;
}


string SQLString::getTypeAsString() const
{
	string queryType ;
    
	// return the query type as string ...
    switch (_queryType)
    {
        case QUERY_UNDEFINED :
        { queryType = "QUERY_UNDEFINED" ; break ; } // this is for undefined

        case COMMAND :       
        { queryType = "COMMAND" ; break ; }

        case DELETE :       
        { queryType = "DELETE" ; break ; }

        case DROP :       
        { queryType = "DROP" ; break ; }

        case INSERT :       
        { queryType = "INSERT" ; break ; }

        case SELECT :       
        { queryType = "SELECT" ; break ; }
        
        case UNION :       
        { queryType = "UNION" ; break ; }

        case UPDATE :       
        { queryType = "UPDATE" ; break ; }

        case LOBJ_UPLOAD :     // large object (blob/clob) upload
        { queryType = "LOBJ_UPLOAD" ; break ; }

        case LOBJ_DOWNLOAD :   // large object (blob/clob) download    
        { queryType = "LOBJ_DOWNLOAD" ; break ; }

        case LOBJ_ISNULL :     // large object is null  
        { queryType = "LOBJ_ISNULL" ; break ; }

        case FUNCTION :       
        { queryType = "FUNCTION" ; break ; }
   
        case CUSTFUNCTION :       
        { queryType = "CUSTFUNCTION" ; break ; }

        case CUSTPROCEDURE :       
        { queryType = "CUSTPROCEDURE" ; break ; }

  		default: // unrecognized or unsupported query type
        { queryType = "SQLString::getTypeAsString() INTERNAL ERROR. CALL SUPPORT." ; }

    }	/* switch */	
    
    
    return queryType ;
}


bool SQLString::sqlForcedStringEmpty() const
{
    if ( _queryType != SQLString::COMMAND ) // check 
    {
        ostringstream errorMessage;
        errorMessage << "SQLString::sqlForcedStringEmpty. Invalid query type: " << _queryType << " This method can be used only if type is COMMAND." ;
        ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
    }
	return _forcedString.empty() ; // return if empty
}



string SQLString::intervalToSeconds(const string& key, const string &intv_in) 
{
	const string intv (	SQLDialectFactory::sqlDialect(key).translateColumnName(intv_in) ) ;

	return ( SQLDialectFactory::sqlDialect(key).intervalToSeconds(intv) ) ;
}


string SQLString::intervalToSeconds(const string& key, const string &beg_intv_in, const string &end_intv_in) 
{
	const string beg_intv (	SQLDialectFactory::sqlDialect(key).translateColumnName(beg_intv_in) ) ;
	const string end_intv (	SQLDialectFactory::sqlDialect(key).translateColumnName(end_intv_in) ) ;

	return ( SQLDialectFactory::sqlDialect(key).intervalToSeconds(beg_intv, end_intv) ) ;
}



SQLString  const SQLString::getIsolationCommittedRead(const string& key) 
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).getIsolationCommittedRead() );

    return tmp ;
}


#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
SQLString const SQLString::getLockModeWait(int waitTimeSec) 
{
	const string def_key = "" ; 
	return getLockModeWait(def_key, waitTimeSec) ;
}

#endif
SQLString const SQLString::getLockModeWait(const string& key, int waitTimeSec) 
{    
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).getLockModeWait(waitTimeSec) );

    return tmp ;
}


SQLString const SQLString::beginTransaction(const string& key, const string& txname) 
{    
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).beginTransaction(txname) );

    return tmp ;
}


SQLString const SQLString::commitTransaction(const string& key) 
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).commitTransaction() );

    return tmp ;
}


string const SQLString::getSeparator(const string& key)
{
    return SQLDialectFactory::sqlDialect(key).getSeparator() ;
} 


SQLString const SQLString::bitAnd(const string& key, const string &l, const string &r) 
{
	SQLString s(key, SQLString::COMMAND ) ;
	s.forceSQLString( SQLDialectFactory::sqlDialect(key).bitAnd(l,r) ) ;
	return s ;
}

SQLString const SQLString::bitOr(const string& key, const string &l, const string &r) 
{
	SQLString s(key, SQLString::COMMAND ) ;
	s.forceSQLString( SQLDialectFactory::sqlDialect(key).bitOr(l,r) ) ;
	return s ;
}

SQLString const SQLString::bitXor(const string& key, const string &l, const string &r) 
{
	SQLString s(key, SQLString::COMMAND ) ;
	s.forceSQLString( SQLDialectFactory::sqlDialect(key).bitXor(l,r) );
	return s ;
}

SQLString const SQLString::bitNot(const string& key, const string &l) 
{
	SQLString s(key, SQLString::COMMAND ) ;
	s.forceSQLString( SQLDialectFactory::sqlDialect(key).bitNot(l) ) ;
	return s ;
}


#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
string const SQLString::getOrderByString(const string& s, const SQLString::OrderBy& o) 
{
	const string def_key = "" ; 
    return getOrderByString(def_key, s, o) ;
}
#endif

string const SQLString::getOrderByString(const string& key, const string& s, const SQLString::OrderBy& o) 
{
    /**
        Can be used to return a OrderBy string: ASC and DESC are got by the dialect.
        The string can be placed using setOrderByString(string) method   
    
        THIS METHOD MUST NOT BE USED WITH STANDARD addOrderByList INTERFACE !!
    */
    string tmpSort ;
    
    if (o == SQLString::ASC) {
        tmpSort = SQLDialectFactory::sqlDialect(key).orderByASC() + " " ;
    }
    else if (o == SQLString::DESC) {
        tmpSort = SQLDialectFactory::sqlDialect(key).orderByDESC() + " " ;
    }
    else {
        ; // nothing 
    }
    return s +  " " + tmpSort ;
}


SQLString const SQLString::rollbackTransaction(string const &key) 
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).rollbackTransaction() );

    return tmp ;
}

SQLString const SQLString::tableIndexesQuery (const string& key, const string& tableName)
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).getTableIndexesQuery(tableName) );

    return tmp ;
}

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
SQLString const SQLString::metadataQuery(string const & tableName)
{
	const string def_key = "" ; 
    return metadataQuery(def_key, tableName) ;
}
#endif

SQLString const SQLString::metadataQuery(string const & key, string const & tableName)
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).metadataQuery(tableName) );

    return tmp ;
}

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
SQLString const SQLString::dependantTablesQuery(string const & tableName)
{
	const string def_key = "" ; 
    return dependantTablesQuery(def_key, tableName) ;
}
#endif

SQLString const SQLString::dependantTablesQuery(string const & key, string const & tableName)
{
    SQLString tmp( key, SQLString::COMMAND ) ;
    tmp.forceSQLString( SQLDialectFactory::sqlDialect(key).childTablesQuery(tableName) );

    return tmp ;
}


string SQLString::getSQL(bool nestedStatement) const // PRQA S 4020
{
// W8115_OFF: multiple returns
    if ( _queryType == SQLString::COMMAND)
    {
        ACS_COND_THROW(_forcedString.empty(), // PRQA S 3081
			Exception( "SQLString::getSQL. _queryType is COMMAND but _forcedString is empty." )) ;

        if(_forcedString == "NULL_COMMAND_IGNORE" ) {
            return "" ; /* Useful for unsupported commands... */
        }
        
        string tmp = _forcedString ;
        if (nestedStatement)
        {
            tmp = " (" + _forcedString + ") " ; 

        } else {   /* Not nested */

            string oracle_pattern = "^[ ]*BEGIN.*END.*$" ;
            RegExp oracle_re(oracle_pattern) ;
            const string &dialect = SQLDialectFactory::dialectName(_key) ;
            if ( dialect != "Oracle11" || !oracle_re.match(_forcedString) )
            {
                if ( _forcedString.rfind(';') != string::npos ) {
            	    tmp = _forcedString.substr(0, _forcedString.rfind(';')) + SQLDialectFactory::sqlDialect(_key).getStatementTerminator() ;
                }
                else {
                    tmp = _forcedString + SQLDialectFactory::sqlDialect(_key).getStatementTerminator() ;
                }
            } else {
                   
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found a ORACLE PL/SQL String.") ;

                // Found a PL/SQL Expression. Terminator won't be parsed.
                tmp = _forcedString ;

            } // if ( SQLDialectFactory::dialectName() == "Oracle11" )

		}   // if (nestedStatement)
        
        
		return tmp; 

    }
    else {
        return buildSQLQuery(nestedStatement) ;
    }
}


const string &SQLString::getWhereParameters() const // PRQA S 4120
{
    return _sqlWhereParameters ;
}



const vector< pair<string, string> > &SQLString::getTableList() const // PRQA S 4120
{
    return _sqlTableList ;
}


const vector<std::string>& SQLString::getJoinParameters() const {
	return _sqlJoinParameters;
}


const pair <string, string> &SQLString::getColumnAndFileName() const  // PRQA S 4120
{
    return _sqlColumnAndFileName ;
}


pair <SQLString, string> SQLString::getCommandAndFileName() const 
{
    return 
		_sqlCommandAndFileName ? // PRQA S 3382, 3385
			*_sqlCommandAndFileName : 
			pair <SQLString, string> (SQLString(_key, QUERY_UNDEFINED),"" ) ;
}


string SQLString::getOrderBy() const 
{
    string sqlString ;

 	for (size_t k=0 ; k< _sqlOrderByList.size() ; k++) // foreach order by element
	{
		sqlString+= ( (k > 0) ? ", " : " ") ; // PRQA S 3380
		sqlString+= _sqlOrderByList[k].first ;

        if ( _sqlOrderByList[k].second == SQLString::ASC ) { // ascending ? 
            sqlString+= " " + SQLDialectFactory::sqlDialect(_key).orderByASC() ; // compose
        }
        else if ( _sqlOrderByList[k].second == SQLString::DESC ) { // descending 
            sqlString+= " " + SQLDialectFactory::sqlDialect(_key).orderByDESC() ; // compose
        }
        else { /* nothing */ }
	}
    
    return sqlString ;
}

/*********************************************************************************/
/*  END OF GET METHODS                                                           */
/*********************************************************************************/


//
//  checkSQLTokenMap
//
void SQLString::checkSQLTokenMap(vector<SQLDialect::SQLToken> const& t) const
{
    for (unsigned int i=0; i<_usedTokens.size(); i++)
    {
        bool found = false ;
        for (unsigned int j=0; j<t.size(); j++) {
            if ( _usedTokens[i] == t[j] ) { found = true ; }
        }
            
        if (!found)
        {
            ostringstream errorMessage;
            errorMessage << "SQLString::checkSQLTokenMap(). Token " 
						 << SQLDialectFactory::sqlDialect(_key).translateToken( _usedTokens[i], false ) 
						 << " not allowed by Template for query type: " << _queryType ;
            ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
        }
    } // END for _usedTokens.size() 
}


//
// buildSQLQuery()
//
string SQLString::buildSQLQuery(bool nestedStatement) const
{
// complex method

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Method started") ;


    vector<SQLDialect::SQLToken> SQLTemplate ;
    string sqlString = "" ;
    
    SQLDialect &theDialect = SQLDialectFactory::sqlDialect(_key) ;
	
	// get the right statement template ... 
	switch (_queryType)
    {
    case SQLString::DELETE :
        { SQLTemplate = theDialect.getDeleteTemplate() ; break ; }
    case SQLString::DROP :
        { SQLTemplate = theDialect.getDropTemplate() ; break ; }
    case SQLString::INSERT :
        { SQLTemplate = theDialect.getInsertTemplate() ; break ; }
    case SQLString::SELECT :
        { SQLTemplate = theDialect.getSelectTemplate() ; break ; }
    case SQLString::UNION :
        { SQLTemplate = theDialect.getUnionTemplate() ; break ; }
    case SQLString::UPDATE :
        { SQLTemplate = theDialect.getUpdateTemplate() ; break ; }
    case SQLString::LOBJ_UPLOAD :
        { SQLTemplate = theDialect.getBlobUploadTemplate() ; break ; }
    case SQLString::LOBJ_DOWNLOAD :
        { SQLTemplate = theDialect.getBlobDownloadTemplate() ; break ; }
    case SQLString::LOBJ_ISNULL :
        { SQLTemplate = theDialect.getBlobIsNullTemplate() ; break ; }
    case SQLString::FUNCTION :
        { SQLTemplate = theDialect.getFunctionTemplate() ; break ; }
    case SQLString::CUSTFUNCTION :
        { SQLTemplate = theDialect.getCustFunctionTemplate() ; break ; }
    case SQLString::CUSTPROCEDURE :
        {   SQLTemplate = theDialect.getCustProcedureTemplate() ; break ;   }
    default:
        {
            ostringstream errorMessage;
            errorMessage << "SQLString::buildSQLQuery(). Cannot process SQL Query of unknown type: " << int(_queryType) << ""; // PRQA S 3081 2
            ACS_THROW(Exception(errorMessage.str() )) ;
        }
    }

    checkSQLTokenMap(SQLTemplate) ; /* Exception will be raised if token not supported */

    size_t templsize = SQLTemplate.size() ;
	for(size_t templ=0; templ<templsize ; ++templ) // each template element, in the right order
	{ 
    
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing Token: " << SQLTemplate[templ]) ;

    	switch(SQLTemplate[templ]) // which template element type is it ?
		{	
            
        // SQLString used only as container, so cannot be used to get a SQL command. For example: upload/download operation built into dialect, etc.
        case SQLDialect::TEMPLATE_INVALID :
            {
            	ostringstream errorMessage;
				errorMessage << "SQLString::buildSQLQuery(). TEMPLATE_INVALID :  This template CANNOT BE USED to get a valid SQL Sentence. " ;
				ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
                break ;
            }

        /* Simple commands: are just translated */
        case SQLDialect::DELETE:    // fallthrough
        case SQLDialect::DROP:      // fallthrough
        case SQLDialect::FROM:      // fallthrough
        case SQLDialect::INSERT:    // fallthrough
        case SQLDialect::SELECT:    // fallthrough
        case SQLDialect::SET:       // fallthrough
        case SQLDialect::UPDATE:    // fallthrough
            { sqlString+=theDialect.translateToken(SQLTemplate[templ]) + " " ; break; }

		case SQLDialect::VALUES:   
			{ 
				if ( not _sqlValuesList.empty() ) 
				{ sqlString+=theDialect.translateToken(SQLTemplate[templ]) + " " ; }
				break; 
			}
        
		
		case SQLDialect::UNION_LIST: // List of SELECT queries to build a UNION
			{
				if (_sqlUnionList.empty())
				{
					ostringstream errorMessage;
					errorMessage << "SQLString::buildSQLQuery(). Union List is empty: No SELECT in UNION";
					ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
				}

				sqlString+= " ( " + (_sqlUnionList.at(0).first) + " )" ; 

                size_t ul_size = _sqlUnionList.size() ;
				for (size_t ul=1; ul<ul_size; ul++)
				{
					const bool isUnionAll (_sqlUnionList.at(ul).second) ;
					sqlString+= " " + ( isUnionAll? theDialect.translateToken(SQLDialect::UNION_ALL): theDialect.translateToken(SQLDialect::UNION) ) ; // PRQA S 3380
					sqlString+= " ( " + _sqlUnionList.at(ul).first + " )" ; 
				}
                
				sqlString+= " " ;
                break ;
			}


        case SQLDialect::COLUMN_LIST: // list of columns (a,b..)
            {
				// Note: either _sqlValuesList or _sqlNestedValuesList can be filled
				// (enforcement is made during the set methods)
			    for (size_t k=0 ; k<_sqlValuesList.size() ; ++k)
			    {
                    pair <string, string> tmp ;
                    tmp = _sqlValuesList[k] ;
				    sqlString+= ( (k > 0) ? ", ": "(" ) ; // PRQA S 3380
				    sqlString+= tmp.first ;
			    }
			    
				const vector <string>& sqlNestedColumns ( _sqlNestedValuesList.first ) ;
				for (size_t k=0 ; k<sqlNestedColumns.size() ; ++k)
			    {
				    sqlString+= ( (k > 0) ? ", ": "(" ) ; // PRQA S 3380
				    sqlString+= sqlNestedColumns[k] ;
			    }

                if ( !_sqlValuesList.empty() || !sqlNestedColumns.empty() ) { sqlString+=")" ; }
			    sqlString+=" " ;
			    break;
		    }

        case SQLDialect::DISTINCT: // the DISTINCT
            {
			    if(_sqlDistinct>0) {
				    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) +" " ;
                }
			    break;
            }

        case SQLDialect::GROUPBY: // GROUP BY a,b ...
            {
                if (! _sqlGroupByList.empty()) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }
                for (size_t k=0 ; k< _sqlGroupByList.size() ; k++)
			    {
				    sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
				    sqlString+= _sqlGroupByList[k];
			    }
			    sqlString+=" " ;
                break;
            }


        case SQLDialect::HAVING: // HAVING (in group by)
            {
			    if (not _sqlHavingParameters.empty()) {
				    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " + _sqlHavingParameters ;
                }
			    break;
            }

				
		case SQLDialect::LIMIT: // LIMIT #records
            {
			    if( not theDialect.limitWrapsQuery() && (_sqlLimit > 0) )
			    {
                    ostringstream limit ;
                    limit << theDialect.translateToken(SQLTemplate[templ]) << _sqlLimit ;
 				    sqlString+= " " + limit.str() + " ";	
			    }
			    break;
		    }

		case SQLDialect::OFFSET: // OFFSET #records
            {
			    if( not theDialect.limitWrapsQuery() && (_sqlOffset > 0) )
			    {
                    ostringstream offset ;
                    offset << theDialect.translateToken(SQLTemplate[templ]) << _sqlOffset ;
 				    sqlString+= " " + offset.str() + " ";	
			    }
			    break;
		    }

        case SQLDialect::ORDERBY: // the order by list
            {
                if (not _sqlOrderByList.empty()) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }
			    sqlString+=getOrderBy() + " " ;

    		    break;
		    }

        case SQLDialect::FORREADONLY: // the order by list
            {
                if (_isReadOnlySelect) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }

			    break;
		    }

			
        case SQLDialect::SELECT_LIST: // SELECT (not given) A, B, C
            {
                size_t slistsize = _sqlSelectList.size() ;
			    for (size_t k=0 ; k< slistsize ; ++k)
			    {
				    sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 

				    SelectListContainer tmp = _sqlSelectList[k]	 ;

                    string item = tmp.getItem() ; 
                    string lbl = tmp.getAlias() ;
                    SQLDialect::SQLCast sqlCast = tmp.getSQLCast() ;

                    sqlString += theDialect.translateSQLCast(item, sqlCast) ;

                    if (lbl != "") { sqlString+= " AS " + lbl ; }

			    }
			    sqlString+=" " ;
			    break;
		    }

		case SQLDialect::SET_LIST: // col1 = val1, col2 = val2 ... 
            {
			    for (size_t k=0 ; k< _sqlSetList.size() ; k++)
			    {
                    pair <string, string> tmp ;
                    tmp = _sqlSetList[k] ;
				    sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
				    sqlString+= tmp.first + " = " + tmp.second;
			    }
			    sqlString+=" " ;
			    break;
		    }


        case SQLDialect::TABLE_LIST: // list of tables (comma separated)
            {
				if (_sqlTableList.empty()) { sqlString += theDialect.getTableNameForEmptyFROM() ; } 
				else {
					for (size_t k=0 ; k< _sqlTableList.size() ; k++)
			    	{
				    	sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
				    	pair <string, string> tmp = _sqlTableList[k] ;
				    	sqlString+= tmp.first + " " + tmp.second ;
			    	}
				}
			    sqlString+=" " ;
			    break;
		    }

            
        case SQLDialect::FUNCTIONARG:// function arguments
            {
                sqlString+= " ( " ; /* If needed, will be moved into dialect */
			    for (size_t k=0 ; k< _sqlFunctionParamsList.size() ; k++)
			    {
				    sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
				    sqlString+= _sqlFunctionParamsList[k];
			    }
			    sqlString+=" ) " ; /* If needed, will be moved into dialect */
			    break;            
		    }


        case SQLDialect::CUSTFUNCTIONARG: // custom function arguments
            {
                sqlString+= " ( " ; /* If needed, will be moved into dialect */
			    for (size_t k=0 ; k< _sqlFunctionParamsList.size() ; k++)
			    {
				    sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
				    sqlString+= _sqlFunctionParamsList[k];
			    }
			    sqlString+=" ) " ; /* If needed, will be moved into dialect */
			    break;
		    }


        case SQLDialect::CUSTPROCEDUREARG: // custom procedure arguments
            {
                sqlString+= " ( " ; /* If needed, will be moved into dialect */
		        for (size_t k=0 ; k< _sqlProcedureParamsList.size() ; k++)
		        {
			        sqlString+= ( (k > 0) ? ", ": " ") ;  // PRQA S 3380 
			        sqlString+= _sqlProcedureParamsList[k];
		        }
		        sqlString+=" ) " ; /* If needed, will be moved into dialect */
		        break;
	        }
      
      
        case SQLDialect::FUNCTIONNAME: // function name 
            {
                if (! nestedStatement ) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }
                else {
                    sqlString+= " " + theDialect.translateToken(SQLDialect::NESTEDFUNCTIONNAME) + " " ;
                }
				sqlString+= " " + _sqlFunctionName + " " ;
                break;
			}


        case SQLDialect::CUSTFUNCTIONNAME: // custom function name 
            {
                if (! nestedStatement ) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }
                else {
                    sqlString+= " " + theDialect.translateToken(SQLDialect::CUSTNESTEDFUNCTIONNAME) + " " ;
                }
				sqlString+= " " + _sqlFunctionName + " " ;
                break;
			}


        case SQLDialect::CUSTPROCEDURENAME: // custom procedure name 
            {
                if (! nestedStatement ) {
                    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " ;
                }
                else {
                    sqlString+= " " + theDialect.translateToken(SQLDialect::CUSTNESTEDPROCEDURENAME) + " " ;
                }
				sqlString+= " " + _sqlProcedureName + " " ;
                break;
			}


        case SQLDialect::VALUES_LIST: // list of values
            {
                size_t vlistsize = _sqlValuesList.size() ;
				for (size_t k=0 ; k< vlistsize ; ++k)
				{
                    pair <string, string> tmp ;
                    tmp = _sqlValuesList[k] ;
					sqlString+= ( (k > 0) ? ", ": "(" ) ;  // PRQA S 3380 
					sqlString+= tmp.second ;
				}
                if ( not _sqlValuesList.empty() ) { sqlString+=")" ; }
				sqlString+=" " ;
			    break;
			}

        case SQLDialect::NESTED_VALUES_LIST: // if contains a single query
            {
				if (! _sqlNestedValuesList.first.empty())
				{ sqlString+= _sqlNestedValuesList.second ; }	// No parentheses are needed since the Query is already "nested"
				sqlString+=" " ;
			    break;
			}

        case SQLDialect::JOIN: // a JOIN
            {
				if (not _sqlJoinParameters.empty()) {
                    size_t jparsize = _sqlJoinParameters.size() ;
					for (size_t k = 0; k<jparsize ; ++k)
					{
						sqlString+= " " + _sqlJoinParameters[k] + " " ;
					}
				}
			    break;
			}


		case SQLDialect::WHERE: // the WHERE
		    {
			    if (not _sqlWhereParameters.empty()) {      
				    sqlString+= " " + theDialect.translateToken(SQLTemplate[templ]) + " " + _sqlWhereParameters ;
			    }

    		    break;
		    }



		default: // token not recognized: error
		    {
			    ostringstream errorMessage;
			    errorMessage << "SQLString::buildSQLQuery(): Cannot convert token " << SQLTemplate[templ] << "";
			    ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
		    }


		}   /* END switch */
		
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sqlString (so far): !" << sqlString << "!") ;

	}   /* END for */
	

    if ( theDialect.limitWrapsQuery() && ((_sqlLimit > 0) || (_sqlOffset > 0)) )
    {
	    if ( _queryType != SQLString::SELECT)
	    {
	        ostringstream errorMessage;
	        errorMessage << "SQLString::buildSQLQuery(). Cannot limit/offset SQL Query of type: " << _queryType << " different from SELECT when limitWrapsQuery set";
	        ACS_THROW(Exception(errorMessage.str() )) ; // PRQA S 3081
	    }
        sqlString = theDialect.getLimitedQueryWrapped(sqlString, _sqlLimit, _sqlOffset) ;
//      ostringstream o ; 
//      if (_sqlOffset > 0) {
//          o << _sqlOffset << '+' ;
//      }
//      o << _sqlLimit ;
//      sqlString = "SELECT * FROM ( " + sqlString + " ) WHERE ( ROWNUM <= " + o.str() + " ) " ;
    }				
	
    if (nestedStatement) { // is a nested statement ?
        sqlString = " (" + sqlString + ") " ; 
    }
    else {
        sqlString = " " + sqlString + theDialect.getStatementTerminator() ;
    }

    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Done. returned sqlString !" << sqlString << "!") ;
	
    // return the string .. 
    return sqlString ;
}


//
// removePattern
//
string SQLString::replacePattern(string s, const string& p, const string& r) // PRQA S 2010
{
	/*
    	Replace a pattern into a string.
        Example: pattern = "," replace = " ": "A,B" -> "A B" 
    */

	size_t pos = 0  ;
	while ( (pos = s.find(p)) != string::npos ) // PRQA S 4244
	{
		s = s.replace(pos, p.size(), r) ;
	}

	return s ;
}


//
// splitPattern
//
vector <string> SQLString::splitPattern(string s, const string& p) // PRQA S 2010
{
    /* 
        Split a string into vector entries, looking for a pattern 
        Example: pattern = " ":  "A B" -> { "A", "B" } 
    */   
        
        
	vector <string> v ;
	
	while ( s.size() > 0 )
	{
		size_t pos = s.find(p) ;

		if (pos == string::npos)
		{
			v.push_back(s) ;
			break ;
		}
		else
		{
			string a = s.substr(0, pos) ; // PRQA S 3084 2
			s = s.substr(pos+1, string::npos) ;
			if (not a.empty()) { v.push_back(a) ; }
		}
		
	}

	return v ;
}


// << operator
ostream &operator<<(ostream &os, const SQLString &n) 	// output to stream
{
    os << n.getSQL(true) ;
	
	return os ; // PRQA S 4028
}




string toString(SQLString::QueryType t)  // PRQA S 4020
{
    switch(t) {
    case SQLString::QUERY_UNDEFINED: { return "NONQUERY_UNDEFINEDE" ; }  
    case SQLString::COMMAND: { return "COMMAND" ; }
    case SQLString::DELETE: { return "DELETE" ; }
    case SQLString::DROP: { return "DROP" ; }
    case SQLString::INSERT: { return "INSERT" ; }
    case SQLString::SELECT: { return "SELECT" ; }
    case SQLString::UPDATE: { return "UPDATE" ; }
    case SQLString::LOBJ_UPLOAD: { return "LOBJ_UPLOAD" ; }
    case SQLString::LOBJ_DOWNLOAD: { return "LOBJ_DOWNLOAD" ; }
    case SQLString::LOBJ_ISNULL: { return "LOBJ_ISNULL" ; }
    case SQLString::FUNCTION: { return "FUNCTION" ; }
    case SQLString::CUSTFUNCTION: { return "CUSTFUNCTION" ; }
    case SQLString::CUSTPROCEDURE: { return "CUSTPROCEDURE" ; }
    default:
        {
            ostringstream os ; 
            os << "unrecognized QueryType <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

ostream & operator << (ostream &os, SQLString::QueryType t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, SQLString::QueryType t) {// PRQA S 4222
    return os << toString(t) ;
}


_ACS_END_NAMESPACE
