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
	Revision 5.13  2017/11/24 15:43:22  marpas
	IN and NOT_IN now accept db::toSQL and/or SQLString
	
	Revision 5.12  2017/11/23 13:28:28  marpas
	default ctor must be inner to the compiler condition to prevent default db key
	
	Revision 5.11  2016/04/11 13:16:18  marpas
	useless methods removed
	
	Revision 5.10  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.9  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.8  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.7  2013/11/07 13:19:13  lucio.pulvirenti
	Check on NOT_IN added to CTORs
	
	Revision 5.6  2013/11/05 16:43:24  enrcar
	EC:: Operators NOT_IN, NOT_IN_TABLE handled
	
	Revision 5.5  2013/07/18 11:30:18  marpas
	fixed missing IN ctors with vector of signed and unsigned long long and the explicit dbkey
	
	Revision 5.4  2013/07/15 16:19:13  marpas
	deprecating methods having default db key string
	
	Revision 5.3  2013/06/13 18:10:54  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.2  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.15  2013/06/03 12:35:53  marpas
	long long supported other than unsigned long long
	
	Revision 4.14  2013/05/29 10:52:40  marpas
	IN operator accepts alsoan SQLString (useful to do DB independent subqueries)
	
	Revision 4.13  2013/05/27 10:24:28  marpas
	unsigned long long vector for IN paramaters
	
	Revision 4.12  2013/05/18 07:06:50  marpas
	removing old commented code and related warnings
	
	Revision 4.11  2013/05/14 18:13:59  marpas
	dbQuery.h
	
	Revision 4.10  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.9  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.8  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.7  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.5  2011/02/07 09:48:38  lucio.pulvirenti
	EC: dbParam::IN_TABLE syntax error fixed
	
	Revision 3.4  2011/01/26 15:11:55  lucio.pulvirenti
	EC+LP::  Parameter IN_TABLE handled
	
	Revision 3.3  2010/04/28 15:50:04  enrcar
	EC:: Added to Oracle DB the support for geoobjects
	
	Revision 3.2  2010/04/26 10:27:58  marpas
	Preparation to ORACLE integration (needs libDbGeo ORACLE aware)
	
	Revision 3.1  2010/04/07 16:08:38  lucio.pulvirenti
	LP: translateColumnName method of dialect applied to column names
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.15  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.14  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.13  2009/02/17 10:41:25  marpas
	deprecation in progress
	
	Revision 2.12  2009/01/26 16:53:21  marpas
	parameters for IN where clause fixed (if strings)
	test updated
	
	Revision 2.11  2008/12/15 15:27:44  enrcar
	EC:: Misleading ctor removed
	
	Revision 2.10  2008/11/04 13:41:37  marpas
	make up and remotion of useless stuff
	
	Revision 2.9  2008/07/28 08:56:18  manuel.maccaroni
	new library OriginatorName to query tOriginators table
	
	Revision 2.8  2006/07/14 12:24:34  paoscu
	getValue and operator== fixed
	
	Revision 2.7  2006/06/26 12:07:55  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.6  2006/06/20 16:35:12  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.5  2006/03/30 14:22:35  paoscu
	Method  "bool hasValue(int mask)" and similar ones added.
	Method  "operationsInMask" added.
	Method  "getType(dbGenericColumnType::Type &type) const" fixed
	
	Revision 2.4  2006/03/17 01:44:45  paoscu
	Added methods:
	static std::string paramIdentifier(const std::string &paramName , const std::string &tbl, const std::string &col)
	static dbGenericColumnType::Type getType(const std::string & paramName, const std::string &table, const std::string &column)
	
	Method obtainTableandField moved to public.
	
	Revision 2.3  2006/03/10 17:32:34  marpas
	paramIdentifier static method added
	
	Revision 2.2  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.1  2006/03/08 18:42:25  marpas
	mask operation bug fixed
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2006/02/21 20:22:34  marpas
	header fixed
	
	Revision 1.7  2006/02/17 18:01:48  marpas
	OperationMask enum and related functions added
	
	Revision 1.6  2006/01/13 18:05:35  paoscu
	Query parameters implementation completely changed.
	
	Revision 1.5  2005/07/19 07:56:17  enrcar
	*** empty log message ***
	
	Revision 1.4  2005/07/13 09:27:31  enrcar
	updated to dbClient version 1.15
	
	Revision 1.15  2005/07/12 16:16:11  marpas
	introducing dbParam::ISNOTNULL
	dbParam::verifyDbValues flag to avoid checking into dbvalues when composing complex where clauses
	
	Revision 1.14  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.13  2004/07/22 13:25:15  marpas
	dbParam::dbStringParamItem::getValue now returns string surronded by single quotes
	
	Revision 1.12  2004/07/20 10:15:11  marpas
	IN added to dbParam
	some minor changes and fixes
	
	Revision 1.11  2004/01/08 17:10:59  paoscu
	XMLOstream::Tag bool constructor used.
	
	Revision 1.10  2003/09/03 13:55:59  paoscu
	Export methods are now const.
	
	Revision 1.9  2003/07/11 13:08:03  marpas
	exException::clone implemented
	
	Revision 1.8  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.7  2003/04/15 20:21:37  marpas
	GCC3.2
	
	Revision 1.6  2003/02/03 18:55:28  marpas
	Exception hierarchy tree enhanced to hilight db exceptions
	
	Revision 1.5  2003/01/31 15:05:23  paoscu
	IS_NULL operator added
	
	Revision 1.4  2002/12/16 11:29:39  paoscu
	Export and import methods modified.
	
	Revision 1.3  2002/12/04 16:09:20  paoscu
	Changed importObj and exportObj methods.
	
	Revision 1.2  2002/11/29 17:53:38  danalt
	added exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/

#include <dbParam.h>
#include <SQLDialectFactory.h>
#include <SQLDialect.h>
#include <dbSchemaMetaDataProvider.h>
#include <SQLString.h>
#include <dbConv.h>
#include <Filterables.h>

#include <dbGeoObject.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <StringUtils.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using std::vector ;
using std::pair ;
using std::string ;

class dbSqlStringParamItem : public dbParam::dbParamItem { // PRQA S 2109
public:
    dbSqlStringParamItem() = delete ; // not implemented
	dbSqlStringParamItem(const string &k, const SQLString &s) : dbParamItem(k), _s(s) {}
    dbSqlStringParamItem(const dbSqlStringParamItem &) = default ;
    dbSqlStringParamItem & operator=(const dbSqlStringParamItem &) = default ;
	virtual ~dbSqlStringParamItem() = default ;

	virtual dbParamItem *clone() const { return new dbSqlStringParamItem(*this) ; } // PRQA S 2131, 2502
	virtual string getValue() const ;

private:
	SQLString _s ;
} ;



string dbSqlStringParamItem::getValue() const 
{
	return _s.getSQL(true) ;
}


//*********************************** Auxilary functions ***************************************


struct lowercase {
	char operator()(char c) // PRQA S 4211
	{
		return char(::tolower(int(c))) ; // PRQA S 3081
	}
} ;


bool dbParam::isNumeric(const string &s,bool floating) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "string is \""<<s<<"\" ( length = "<< s.length() <<").") ; 
	
	if (s.empty()) {
        return false ;
    }
	size_t len = s.length() ;
	const char *startConv = s.c_str() ;
	const char *endConv = s.c_str()+len ; // PRQA S 3084
	if ((*startConv == '\'') || (*startConv == '\"')) { // can start with 
		if (((startConv[len-1] == '\'') || (startConv[len-1] == '\"')) && (*startConv == startConv[len-1])) { // PRQA S 3084
			startConv++ ;
			endConv-- ;
		}
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "wrong char \""<<s[0]<<"\" at pos " << 0 ) ;
			return false ;
		}
	}
	char *realEnd = 0 ;
	
	if (floating) { // to be treated as floating
		strtod(startConv,&realEnd) ;
    }
	else { // else is an integral
		strtol(startConv,&realEnd,10) ;	// PRQA S 4400
	}
    
	if (realEnd != endConv) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "wrong char \""<<*realEnd<<"\" at pos "<<realEnd-s.c_str()) ; 
		return false ;// wrog conversion .. still some char unconverted
	}
	return true ; // good
}

pair<string, string> dbParam::obtainTableAndField(const string &key, const string &paramName) // PRQA S 4020
{
#warning parameter name to table and field conversion can fail under certain conditions
	// Table name must begin with T_ or C_ or V_
	
	if(paramName.length() < 5) { // PRQA S 4400
		return pair<string,string>("","") ;
    }
	string paramNameConverted =  SQLDialect::convertCase(key, paramName)  ;

		
	// check first two lowercase 
	string firstTwoChars=string(paramNameConverted, 0 , 2) ;
	transform(firstTwoChars.begin(), firstTwoChars.end(), firstTwoChars.begin(), lowercase() ) ;
	if( !( firstTwoChars == "t_" ||  firstTwoChars == "c_" || firstTwoChars == "v_")) {
		return pair<string,string>("","") ; // cant perform conversion
    }
	
	// check the 2nd _
	size_t fieldEnd=paramNameConverted.find("_" , 2) ; // PRQA S 4412
	if((fieldEnd==paramNameConverted.npos) || (fieldEnd == ( paramNameConverted.length() - 1 )) ) { // PRQA S 3084
		return pair<string,string>("","") ; // not found
    }

	// split
	string tableName= string(paramNameConverted,0,fieldEnd) ;
	string fieldName= string(paramNameConverted,fieldEnd+1) ; // PRQA S 3084

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "paramName is \"" << paramName 
            << "\"  fieldName \"" << fieldName 
            << "\" tableName is \"" << tableName 
            << "\"." ) ;
	return pair<string,string>(tableName,fieldName) ;
}

string dbParam::dbStringParamItem::getValue() const 
{
	return dbConv(_key, _s) ; // PRQA S 3050
}

string dbParam::dbIntParamItem::getValue() const 
{
	return dbConv(_key, _v) ; // PRQA S 3050
}

string dbParam::dbULLParamItem::getValue() const 
{
	return dbConv(_key, _v) ; // PRQA S 3050
}

string dbParam::dbLLParamItem::getValue() const 
{
	return dbConv(_key, _v) ; // PRQA S 3050
}



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbParam) 

// copy constructor
dbParam::dbParam(const dbParam &p) :
	_sqlString(p._sqlString),
    _leftConditionPart(p._leftConditionPart),
    _tableName(p._tableName),
    _fieldName(p._fieldName),
    _operation(p._operation),
	_value(p._value), 
	_value2(p._value2),
    _inValues(),
	_geoObject(p._geoObject ? p._geoObject->clone() : 0), // PRQA S 3380
	_key(p._key) 
{
	value(p._inValues) ; // copy the inValues
}




//**********************************  table & field constructors   ****************************************


dbParam::dbParam(const string &key) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(),
	_fieldName(),
	_operation(dbParam::NONE) , 	
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)		
{
}

//**********************************  table & field constructors   ****************************************

dbParam::dbParam(const string &key, const string &tableName, const string &fieldName, Operation operation, const string &value, const string &value2) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation), 
	_value(value), 
	_value2(value2),
    _inValues(),
	_geoObject(0),
	_key(key)
{
}

dbParam::dbParam(const string &key, 
                 const string &tableName, 
                 const string &fieldName, 
                 Operation operation) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation), 
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((operation != ISNULL) && (operation != ISNOTNULL), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
}

dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 const dbGeoObject &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation == IN) || (_operation == NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}

dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 const vector<string> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 const vector<int> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}

dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 const vector<unsigned long long> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect("").translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key) 
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 const vector<long long> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect("").translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key) 
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


//**********************************  pair constructors   ****************************************

dbParam::dbParam(const string &key, const pair<string, string > &tblAndCol, 
				 Operation operation, 
				 const string &value, 
				 const string &value2) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation), 
	_value(value), 
	_value2(value2),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation == IN) || (_operation == NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
}

dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation), 
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((operation != ISNULL) && (operation != ISNOTNULL), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
}

dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation, 
				 const dbGeoObject &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation == IN) || (_operation == NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}

dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation, 
				 const vector<string> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation, 
				 const vector<int> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation, 
				 const vector<unsigned long long> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect("").translateColumnName(tblAndCol.second)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key) 
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}

dbParam::dbParam(const string &key, const pair<string, string >&tblAndCol, 
				 Operation operation, 
				 const vector<long long> &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect("").translateColumnName(tblAndCol.second)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key) 
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


//********************************** SQLString constructors   ****************************************

dbParam::dbParam(const string &key, const string &tableName, 
				 const string &fieldName, 
				 Operation operation, 
				 SQLString const &v) :
	_sqlString(),
    _leftConditionPart(),
	_tableName(tableName),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(fieldName)),
	_operation(operation),
	_value(), 
	_value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
	value(v) ;
}


dbParam::dbParam(const string &key, Operation operation, 
				 SQLString const &leftConditionPart, 
				 const string &value1, 
				 const string &value2 ) :  
	_sqlString(),
    _leftConditionPart(leftConditionPart.getSQL(true)),
	_tableName(),
	_fieldName(),
	_operation(operation), 
	_value(value1), 
	_value2(value2),
    _inValues(),
	_geoObject(0),
	_key(key)
{

	// precondition check on arguments coherency
	ACS_COND_THROW((_operation == IN) || (_operation == NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
	ACS_COND_THROW(((_operation == BETWEEN) || (operation == NOT_BETWEEN) ) && (value1.empty() || value2.empty()), // PRQA S 3081
							  InvalidOperation("BETWEEN/NOT_BETWEEN operation requires two values !") ) ;
	ACS_COND_THROW(((operation != ISNULL) && (operation != ISNOTNULL) ) && (value1.empty() ), // PRQA S 3081
							  InvalidOperation("The operation require a value") ) ;
}


dbParam::dbParam(const string &key, Operation operation, 
				 SQLString const &leftConditionPart, 
				 const dbGeoObject & v, 
				 const pair<string, string >&tblAndCol) :
	_sqlString(),
    _leftConditionPart(leftConditionPart.getSQL(true)),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation),
    _value(),
    _value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{

	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}

  
dbParam::dbParam(const string &key, Operation operation, 
				 SQLString const &leftConditionPart, 
				 const vector<string> & v, 
				 const pair<string, string >&tblAndCol) :  
	_sqlString(),
    _leftConditionPart(leftConditionPart.getSQL(true)),
	_tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(key).translateColumnName(tblAndCol.second)),
	_operation(operation),
    _value(),
    _value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{


	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;
	value(v) ;
}

dbParam::dbParam(const string &key, Operation operation, 
				 SQLString const &leftConditionPart, 
				 const vector<int> & v, 
				 const pair<string, string >&tblAndCol) :
	_sqlString(),
    _leftConditionPart(leftConditionPart.getSQL(true)),
    _tableName(tblAndCol.first),
	_fieldName(SQLDialectFactory::sqlDialect(_key).translateColumnName(tblAndCol.second)),
	_operation(operation),
    _value(),
    _value2(),
    _inValues(),
	_geoObject(0),
	_key(key)
{
	ACS_COND_THROW((_operation != IN) && (_operation != NOT_IN), // PRQA S 3081
							  InvalidOperation("Invalid operation with this ctor") ) ;

	value(v) ;
}


dbParam::~dbParam() throw ()
{
// PRQA S 4631 L1
	try { // no exception out of dtor
		if (_geoObject) {
			delete _geoObject ;
        }
		clearInValues() ;
	}
	catch(std::exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
// PRQA L:L1
}


dbParam &dbParam::operator=(const dbParam &par)
{
	if(this != &par) { // avoid auto-assignation
		// deep copy
		_sqlString = par._sqlString ;
		_leftConditionPart = par._leftConditionPart ;
		_tableName = par._tableName ;
		_fieldName = par._fieldName ;
		_operation = par._operation ;
		_value = par._value ;
		_value2 = par._value2 ;
		_key = par._key ;
		// geoobject need to be recreated
		delete _geoObject ;
		_geoObject = par._geoObject ? par._geoObject->clone() : 0 ; // PRQA S 3380
		value(par._inValues) ; // copy _inValues
	}
	return *this; 
}



void dbParam::value(const dbGeoObject &s) 
{ 
	std::ostringstream os ;
	s.getSqlString(os,SQLDialectFactory::dialectName(_key)) ;
	_value = os.str() ;
	delete _geoObject ;
	_geoObject = s.clone() ;
}


void dbParam::value(const vector<string> &inv) 
{ 
	clearInValues() ;
    size_t isize = inv.size() ;
	for (size_t i=0; i < isize; ++i) {
		_inValues.push_back(new dbStringParamItem(_key, inv[i])) ;
    }
}

void dbParam::value(const SQLString &inv) 
{ 
	clearInValues() ;
	_inValues.push_back(new dbSqlStringParamItem(_key, inv)) ;
}


void dbParam::value(const vector<int> &inv) 
{ 
	clearInValues() ;
    size_t isize = inv.size() ;
	for (size_t i=0; i < isize; ++i) {
		_inValues.push_back(new dbIntParamItem(_key, inv[i])) ;
    }
}

void dbParam::value(const vector<unsigned long long> &inv) 
{ 
	clearInValues() ;
    size_t isize = inv.size() ;
	for (size_t i=0; i < isize; ++i) {
		_inValues.push_back(new dbULLParamItem(_key, inv[i])) ;
    }
}


void dbParam::value(const vector<long long> &inv) 
{ 
	clearInValues() ;
    size_t isize = inv.size() ;
	for (size_t i=0; i < isize; ++i) {
		_inValues.push_back(new dbLLParamItem(_key, inv[i])) ;
    }
}

void dbParam::value(const vector<dbParamItem *> &inv) 
{ 
	clearInValues() ;
    size_t isize = inv.size() ;
	for (size_t i=0; i < isize; ++i) {
		_inValues.push_back(inv[i]->clone()) ;
    }
}


const dbGeoObject &dbParam::geoObject() const 
{
	if (_geoObject) {
		return *_geoObject ; 
	}
    ACS_THROW(NoGeoObject("No dbGeoObject set")) ; // PRQA S 3081
}


string dbParam::getOperatorName(const string& key, const dbParam::Operation oper) 
{
	return SQLDialectFactory::sqlDialect(key).translateOperator( SQLDialect::SQLOperator(oper) ); // PRQA S 3081
} 


bool dbParam::getType(dbGenericColumnType::Type &type, dbConnection &conn) const // PRQA S 4020, 4214
{
// W8115_OFF: multiple returns 	
	type = getType(tableName(), fieldName(), conn ) ;
	if(type == dbGenericColumnType::UNDEFINED ) {
		return false ;
    }
	else {
		return true ;	
    }
}



dbGenericColumnType::Type dbParam::getType(const string &table, const string &column, dbConnection &conn) // PRQA S 4020
{
// W8115_OFF: multiple returns
	// first try Table and field.
	string tbl(table) ;
	string col(column) ;
	if( tbl.empty() || col.empty() ) {
		return dbGenericColumnType::UNDEFINED ;
    }

	return dbColumnTypeHelper::convertTypeToGeneric ( dbSchemaMetaDataProvider::instance(conn.getKey()) -> getFieldType(tbl , col, conn )) ;
}


// stream output
std::ostream &operator<<(std::ostream &str, const dbParam &par)
{
	// read input
	str << par._tableName << "\n" 
		<< par._fieldName << "\n" 
		<< par._operation << "\n"
		<< par._value << "\n"
		<< par._value2 << "\n" ;
		
	if (0 != str.rdstate()) 
	{
		ACS_THROW(exIOException("Write error",errno)) ; // PRQA S 3081
	}
	return str ; // PRQA S 4028
}

// stream output
exostream &operator<<(exostream &str, const dbParam &par) // PRQA S 4222
{
	try {
		// read input
		str << par._tableName << "\n" 
			<< par._fieldName << "\n" 
			<< par._operation << "\n"
			<< par._value << "\n"
			<< par._value2 << "\n" ;
	}
	catch(std::exception &x) {
		ACS_THROW(exIOException(x,"Parameter write error",errno)) ; // PRQA S 3081
	}
	return str ; // PRQA S 4028
}

void dbParam::ReadSingleParameter(std::istream &str, string &res)
{
	std::stringbuf line ;
	str.get(line) ;
		
#ifdef __linux__

	// porca miseria ...
	// istream::get() dichiara di aver letto gcount() caratteri, 
	// ma non mette il terminatore a strstreambuf
	#if (__GNUC__ < 3)
	   if (str.gcount() > 0)
	#endif
		line.str()[str.gcount()] = 0 ;
#endif

#ifdef _AIX
	// porca miseria ...
	// istream::get() dichiara di aver letto gcount() caratteri, 
	// ma non mette il terminatore a strstreambuf
		if (str.gcount() > 0)
			line.str()[str.gcount()] = 0 ;
#endif

#ifdef __linux__
	#if (__GNUC__ < 3)
		res = str.gcount() ? line.str() : "" ;
	#else
		res = line.str();
	#endif
#else
		res = line.str();
#endif

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ReadSingleParameter read: " << res) ; 
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "file status is: " << int(str.rdstate())) ; // PRQA S 3081
	
	if (0 != str.rdstate()) 
	{
		if (str.eof() || str.fail()) { // negligible state 
			res = "" ;
        }
		else { // error state
			ACS_THROW(exIOException("Read error",errno)) ; // PRQA S 3081
		}
	}
}


void dbParam::clearInValues() 
{
    size_t invsize = _inValues.size() ;
	for (size_t i=0; i < invsize; ++i) {
		delete _inValues[i] ;
    }
	_inValues.clear() ;
}

// stream input
std::istream &operator>>(std::istream &str, dbParam &par)
{
	// read from a stream
	par.ReadSingleParameter(str, par._sqlString)  ; 
	par.ReadSingleParameter(str, par._leftConditionPart)  ; 
	par.ReadSingleParameter(str, par._tableName)  ; 
	
	{
		string tmp;
		par.ReadSingleParameter(str, tmp)  ; 
		par._fieldName = SQLDialectFactory::sqlDialect(par._key).translateColumnName(tmp);
	}
	
	
	string tmpStr ; 
	par.ReadSingleParameter(str, tmpStr)  ; 
	par._operation = dbParam::Operation(atoi(tmpStr.c_str())) ; // PRQA S 3013, 3081
	
	if (dbParam::hasValue(par._operation)) { // has a value ?
		par.ReadSingleParameter(str, par._value)  ; 
    }
	if (dbParam::hasValue2(par._operation)) { // has a 2nd value ?
		par.ReadSingleParameter(str, par._value2)  ; 
    }
	
	return str ; // PRQA S 4028
}


// export the class to a resourceSet
void dbParam::exportObj(rsResourceSet &s) const
{
	// store all needed values
	s.setValue("_sqlString",_sqlString) ;
	s.setValue("_leftConditionPart",_leftConditionPart) ;
	s.setValue("_tableName",_tableName) ;
	s.setValue("_fieldName",_fieldName) ;
	s.setValue("_operation",int(_operation)) ; // PRQA S 3081
	s.setValue("_value",_value) ;
	s.setValue("_value2",_value2) ;
	// and the geo object ... 
	if (_geoObject)
	{
		s.setValue("_existGeoObject",true) ;
		_geoObject->exportObj(s) ;				
	} else {
		// if any ...
		s.setValue("_existGeoObject",false) ;
	}
}



// import the class from a resourceSet
void dbParam::importObj(rsResourceSet &s)
{
	// get the string and other needed params
	s.getValue("_sqlString",_sqlString) ;
	s.getValue("_leftConditionPart",_leftConditionPart) ;
	s.getValue("_tableName",_tableName) ;
	
	{
		string tmpString;
		s.getValue("_fieldName",tmpString) ;
		_fieldName = SQLDialectFactory::sqlDialect(_key).translateColumnName(tmpString);
	}
	
	// the operation
	int tmpOperation = 0 ; 
	s.getValue("_operation", tmpOperation) ;
	_operation = dbParam::Operation(tmpOperation) ; // PRQA S 3013, 3081
	
	// the values (1 and 2 if any)
	s.getValue("_value",_value) ;
	s.getValue("_value2",_value2) ;
	
	// the geo object if any
	bool existGeoObject = false ; 
	s.getValue("_existGeoObject",existGeoObject) ;
	_geoObject = existGeoObject ? dbGeoObject::importObject(s) : 0 ; // PRQA S 3380, 3385
}



// export the class to a XML stream
void dbParam::exportXML(XMLOstream &xst) const
{
	// <_sqlString>...</_sqlString>
	// <_leftConditionPart>...</_leftConditionPart>
	// <_tableName>...</_tableName>
	// <_fieldName>...</_fieldName>
	// <_operation>...</_operation>
	// <_value>...</_value>
	// <_value2>...</_value2>
	// <_existGeoObject>...</_existGeoObject>
	// <.. the geo some way>
	{ XMLOstream::Tag txml(xst,"_sqlString", _sqlString) ; }
	{ XMLOstream::Tag txml(xst,"_leftConditionPart", _leftConditionPart) ; }
	{ XMLOstream::Tag txml(xst,"_tableName", _tableName) ; }
	{ XMLOstream::Tag txml(xst,"_fieldName", _fieldName) ; }
	{
		std::ostringstream os;
		os << int(_operation); // PRQA S 3081
		XMLOstream::Tag txml(xst,"_operation", os.str()) ;
	}
	{ XMLOstream::Tag txml(xst,"_value", _value) ; }
	{ XMLOstream::Tag txml(xst,"_value2", _value2) ; }
	if (_geoObject)
	{
		XMLOstream::Tag txml(xst,"_existGeoObject", true) ;
		_geoObject->exportXML(xst) ;				
	} else {
		XMLOstream::Tag txml(xst,"_existGeoObject", false) ;
	}
}

// PRQA S 4120 EOF
const string &dbParam::tableName() const { return _tableName; }
// PRQA S 4121 EOF
void dbParam::tableName(const string &s) { _tableName = s ; }

const string &dbParam::fieldName() const { return _fieldName; }
void dbParam::fieldName(const string &s) { _fieldName = SQLDialectFactory::sqlDialect(_key).translateColumnName(s) ; }
	
string dbParam::composeTableAndColumn() const 
{
	return _tableName + "." + _fieldName ;
}

string dbParam::paramIdentifier(const string &tbl, const string &col)
{
	return tbl + "_" + col ;
}

string dbParam::paramIdentifier() const
{
	return paramIdentifier(_tableName ,_fieldName) ;
}


dbParam::Operation dbParam::operation() const { return _operation ; }
void dbParam::operation(const dbParam::Operation s) { _operation = s ; }

const string & dbParam::value() const { return _value ; }
void dbParam::value(const string &s) { _value = s ; }

const string & dbParam::value2() const { return _value2 ; }
void dbParam::value2(const string &s) { _value2 = s ; }

string dbParam::getSqlString() const // PRQA S 4020, 4214, 5500, 5510
{
// NB cyclomatic complexity is just slightly high, but is due to the switch control structure below
// any attemp to lower CC would increase complexity for maintenance
	if(!_sqlString.empty() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_sqlString is not empty: \""<< _sqlString <<"\".") ;
		return " ( " + _sqlString + " ) " ;
	}
	
	string sqlString(" (") ; // open the expression
	
	// Now I have to find out the left part of the condition
	string sqlParameterName(_leftConditionPart) ;
	
	if (sqlParameterName.empty() ) {
		sqlParameterName = composeTableAndColumn() ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Left part of the statement  is \""<< sqlParameterName <<"\".") ;


	switch( _operation ) // the operation and its value 
	{					 // (syntax depend on the operator)
	case dbParam::ISNULL:
	case dbParam::ISNOTNULL: // no arg operator
        {
		    sqlString += sqlParameterName ; 
		    sqlString += getOperatorName( _key, _operation ) ; 
		    break ;
        } 				
	case dbParam::NONE:     // single arg operator as in LIKE 'AAA' or > 3
	case dbParam::EQUAL_TO:
	case dbParam::LIKE:
	case dbParam::MINOR_OF:
	case dbParam::MAJOR_OF:
	case dbParam::MAJOR_OR_EQUAL_OF:
	case dbParam::MINOR_OR_EQUAL_OF:
	case dbParam::DIFFERENT_FROM:
        {
		    sqlString += sqlParameterName ; 
		    sqlString += getOperatorName(_key, _operation) ; 
		    sqlString += value() ; 
		    break ; 				
        }
	case dbParam::BETWEEN: // double args operator as in BETWEEN (A AND B )
	case dbParam::NOT_BETWEEN:
        {
		    sqlString += sqlParameterName ; 
		    sqlString += dbParam::getOperatorName(_key, _operation) ; 
		    sqlString += value() ; 
		    sqlString += " AND " ; 
		    sqlString += value2() ;
		    sqlString += " " ; 
		    break ;
        }
	case dbParam::INTERSECT:
	case dbParam::OUTSIDE:
        {
            const string &dname = SQLDialectFactory::dialectName(_key) ;
            if ( dname == "Informix9")
            {
			    sqlString += getOperatorName(_key, _operation) ; 
			    sqlString += sqlParameterName ; 
			    sqlString += ", '" ; 
			    sqlString += value() ; 
			    sqlString += "')" ; 
		    }
            else if (dname == "Postgres8")
            {
			    sqlString += getOperatorName(_key, _operation) ; 
			    sqlString += sqlParameterName ; 
			    sqlString += ", '" ; 
			    sqlString += value() ; 
			    sqlString += "')" ; 
            }
            else if ( dname == "Oracle11" )
            {
                sqlString += "SDO_RELATE(" ;
                sqlString += sqlParameterName ;          
                sqlString += ", " ; 
                sqlString += value() ; 
                sqlString += ", " ; 
                sqlString += getOperatorName(_key, _operation) ; 
                sqlString += ") = 'TRUE'" ; 
		    }
  		    else {
			    ACS_THROW( InvalidOperation( "Invalid/Unsupported dialect: " + dname) ) ; // PRQA S 3081
            }
            break ;
        }
	case dbParam::INSIDE:	// geo (functional) arg operator as in INSIDE(....)	
        {
            const string &dname = SQLDialectFactory::dialectName(_key) ;
            if ( dname == "Postgres8")
            {
			    sqlString += getOperatorName(_key, _operation) ; 
			    sqlString += "'" + value() ; 
			    sqlString += "' ," ; 
                if (dbSchemaMetaDataProvider::instance(_key)->getPostGISMajor() == 3) {
			        sqlString += sqlParameterName;
                }
                else {
                    // MP: extremely inefficient: geography converted into geometry -- will use ST_Contains
			        sqlString += "st_geomfromtext(st_astext(" + sqlParameterName+ "))" ; 
                }
			    sqlString += ")" ; 
		    }
            else if (dname == "Informix9")
            {
			    sqlString += getOperatorName(_key, _operation) ; 
			    sqlString += sqlParameterName ; 
			    sqlString += ", '" ; 
			    sqlString += value() ; 
			    sqlString += "')" ; 
            }
            else if ( dname == "Oracle11" )
            {
                sqlString += "SDO_RELATE(" ;
                sqlString += sqlParameterName ;          
                sqlString += ", " ; 
                sqlString += value() ; 
                sqlString += ", " ; 
                sqlString += getOperatorName(_key, _operation) ; 
                sqlString += ") = 'TRUE'" ; 
		    }
  		    else {
			    ACS_THROW( InvalidOperation( "Invalid/Unsupported dialect: " + dname) ) ; // PRQA S 3081
            }
            break ;
        }
	case dbParam::CONTAINS:	// geo (functional) arg operator as in INSIDE(....)	
        {
            const string &dname = SQLDialectFactory::dialectName(_key) ;
            if ( dname == "Postgres8")
            {
			    sqlString += getOperatorName(_key, dbParam::INSIDE) ; // use the same INSIDE but swap arguments order
                if (dbSchemaMetaDataProvider::instance(_key)->getPostGISMajor() == 3) {
			        sqlString += sqlParameterName;
                }
                else {
                    // MP: extremely inefficient: geography converted into geometry -- will use ST_Contains
			        sqlString += "st_geomfromtext(st_astext(" + sqlParameterName+ "))" ;
                } 
			    sqlString += ", '" ; 
			    sqlString += value() ; 
			    sqlString += "' )" ; 
		    }
            else if (dname == "Informix9")
            {
			    sqlString += getOperatorName(_key, dbParam::INSIDE) ; 
			    sqlString += "'" ; 
			    sqlString += value() ; 
			    sqlString += "' ," ; 
			    sqlString += sqlParameterName ; 
			    sqlString += ")" ; 
            }
            else if ( dname == "Oracle11" )
            {
                sqlString += "SDO_RELATE(" ;
                sqlString += value() ; 
                sqlString += ", " ; 
                sqlString += sqlParameterName ;          
                sqlString += ", " ; 
                sqlString += getOperatorName(_key, dbParam::INSIDE) ; 
                sqlString += ") = 'TRUE'" ; 
		    }
  		    else {
			    ACS_THROW( InvalidOperation( "Invalid/Unsupported dialect: " + dname) ) ; // PRQA S 3081
            }
            break ;
        }
	case dbParam::IN:  // multiple args operator as in IN (A, B, ...)
	case dbParam::NOT_IN:  // multiple args operator as in NOT IN (A, B, ...)
        {
		    sqlString += sqlParameterName ; 
		    sqlString +=  getOperatorName(_key, _operation) ;
		    if (value().empty()) {
                size_t vinsize = valueIn().size() ;  
			    for (size_t j=0; j <vinsize; ++j) {
				    if (j > 0) { sqlString += ", " ; }
				    sqlString += valueIn()[j]->getValue() ;
			    }
		    }
            else { // maybe an InSet
                sqlString += value() ;
            }
		    sqlString += ") " ; 
		    break ; 
        }	
// introducing the InSet it is possible to pass a string also to IN and NOT_IN, so the operation below can be 
// removed and substituted either by an InSet or a SQLString with no semicolon terminator.        
	case dbParam::IN_TABLE:
        {
		    sqlString += sqlParameterName ; 
		    sqlString += getOperatorName(_key, dbParam::IN) ; 
		    sqlString += " ( " ; 
		    sqlString += SQLDialectFactory::sqlDialect(_key).translateToken( SQLDialect::SELECT );
		    sqlString += " * " ;
		    sqlString += SQLDialectFactory::sqlDialect(_key).translateToken( SQLDialect::FROM );
		    sqlString += " " + value() ; 
		    sqlString += " ) ) " ; 
		    break ; 				
        }
	
	case dbParam::NOT_IN_TABLE:
        {
		    sqlString += sqlParameterName ; 
		    sqlString += getOperatorName(_key, dbParam::NOT_IN) ; 
		    sqlString += " ( " ; 
		    sqlString += SQLDialectFactory::sqlDialect(_key).translateToken( SQLDialect::SELECT );
		    sqlString += " * " ;
		    sqlString += SQLDialectFactory::sqlDialect(_key).translateToken( SQLDialect::FROM );
		    sqlString += " " + value() ; 
		    sqlString += " ) ) " ; 
		    break ; 				
        }	default: // not recognized
        {
		    ACS_THROW(InvalidParameter("Invalid Operation for Parameter: "  +  composeTableAndColumn() )) ;  // PRQA S 3081
        }
	}

	sqlString += ")"; // close the expression

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SQL string is \""<< sqlString <<"\".") ;
	
	return sqlString ;
}
		



const vector<dbParam::dbParamItem *> & dbParam::valueIn() const 
{ 
	return _inValues ; 
} 

void dbParam::has(Operation o, bool *value, bool * value2, bool *inValues) 
{
	switch(o) {
	case NONE: 
	case ISNULL:						//  -   IS NULL
	case ISNOTNULL:						//  -   IS NOT NULL
        {
			value ? *value = false : false ; // PRQA S 3380,3385 3
			value2 ? *value2 = false : false ;
			inValues ? *inValues = false : false ;
			break ;
        }
	case EQUAL_TO: 				// ==
	case LIKE:					// LIKE (only for characters)
	case MINOR_OF: 				// <
	case MAJOR_OF: 				// >
	case MAJOR_OR_EQUAL_OF:     // >=
	case MINOR_OR_EQUAL_OF:     // <=
	case DIFFERENT_FROM: 		// !=
	case IN_TABLE: 				// !=
	case NOT_IN_TABLE: 				// !=
        {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = false : false ;
		    break ;
        }
	case BETWEEN:				// BETWEEN
	case NOT_BETWEEN:			// !BETWEEN
        {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = true : true ;
		    inValues ? *inValues = false : false ;
		    break ;
        }
			// Geodetic data blade module functions
	case CONTAINS: 				// The CONTAINS functions compare two spatio-temporal objects
							    //	-	All point, line segments, and the interior area
							    //		(if any) of the second object are spatially
							    //		etirely inside the boundary and the interior area of 
							    //		the first object 
                                // INSIDE (a,b) implies CONTAINS(b,a)
       {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = false : false ;
		    break ;
							    // and return true if all of the following are true:
        }
	case INSIDE: 				// The INSIDE functions compare two spatio-temporal objects
							    //	-	All point, line segments, and the interior area
							    //		(if any) of the first object are spatially
							    //		etirely inside the boundary and the interior area of 
							    //		the second object
       {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = false : false ;
		    break ;
							    // and return true if all of the following are true:
        }
	case INTERSECT:             // 	The INTERSECT function returns TRUE if both of the following
							    //	are true:
							    //	-	Any point or partial line segment of the object in the
							    //		first argument lies partially inside of the object in
							    //		the second argument
							    //	-	The time ranges for the object intersect
        {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = false : false ;
		    break ;
        }
	case OUTSIDE:           // The OUTSIDE functions takes two spatio-temporal objects
							// and return true if both of the following are true:
							//	-	All points and line segment and the entire interior
							//		are (if any) of the first object lie spatially outside
							//		the second object
							//	-	The time ranges of the two objects intersect
        {
		    value ? *value = true : true ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = false : false ;
		    break ;
        }
	case IN:				// The IN function is to produce IN (,,,) where clause
	case NOT_IN:				// The IN function is to produce IN (,,,) where clause
        {
		    value ? *value = false : false ; // PRQA S 3380,3385 3
		    value2 ? *value2 = false : false ;
		    inValues ? *inValues = true : true ;
		    break ;
        }
	default: // not recognized
        {
		    ACS_THROW(InvalidParameter("Invalid Operation")) ;  // PRQA S 3081
        }
	}

}

bool dbParam::hasValue(Operation o) 
{
	bool v = false ;
	has(o,&v) ;
	return v ;
}

bool dbParam::hasValue2(Operation o) 
{
	bool v = false ;
	has(o,0,&v) ;
	return v ;
}

bool dbParam::hasInValues(Operation o) 
{
	bool v = false ;
	has(o,0,0,&v) ;
	return v ;
}
	

bool dbParam::opInMask(int mask, dbParam::Operation op) 
{
	return op2Mask(op) & mask; // PRQA S 3003, 3054
}

dbParam::OperationMask dbParam::op2Mask(dbParam::Operation o) // PRQA S 4020
{
	switch(o) {
	case EQUAL_TO:          { return EQMask ; } // the mask corresponding to EQUAL
	case LIKE:              { return LKMask ; } // the mask corresponding to LIKE
	case MINOR_OF:          { return LTMask ; } // the mask corresponding to MINOR_OF
	case MAJOR_OF:          { return GTMask ; } // the mask corresponding to MAJOR_OF
	case MAJOR_OR_EQUAL_OF: { return GEMask ; } // etc ... etc.. .
	case MINOR_OR_EQUAL_OF: { return LEMask ; }
	case DIFFERENT_FROM:    { return NEMask ; }
	case BETWEEN:           { return BTMask ; }
	case NOT_BETWEEN:       { return NBMask ; }
	case ISNULL:            { return NULMask ; }
	case IN:                { return INMask ; }
	case ISNOTNULL:         { return NONUMask ; }

	case OUTSIDE: // fallthrough
	case INTERSECT: // fallthrough
	case INSIDE: // fallthrough
	case CONTAINS: // fallthrough
	case NONE: // fallthrough
	default:                { return NoneMask ; }
	}
}


#warning PAOLO dbParam::type2Mask takes a dbGenericColumnType::Type
int dbParam::type2Mask(dbGenericColumnType::Type t)  // PRQA S 4020
{
	switch(t) {
	case dbGenericColumnType::INTEGER: // what can be do an integer ?
		{ return EQMask | LTMask | GTMask | GEMask | LEMask | NEMask | BTMask | NBMask | NULMask | INMask | NONUMask ; }
	case dbGenericColumnType::BOOLEAN: // .. a boolean ?
		{ return EQMask | NEMask | NULMask | NONUMask ; }
	case dbGenericColumnType::DOUBLE: // .. a double ?
		{ return EQMask | LTMask | GTMask | GEMask | LEMask | NEMask | BTMask | NBMask | NULMask | NONUMask ; }
	case dbGenericColumnType::STRING: // ... etc ... etc ...
		{ return EQMask | LKMask | LTMask | GTMask | GEMask | LEMask | NEMask | BTMask | NBMask | NULMask | INMask | NONUMask ; }
	case dbGenericColumnType::DATETIME:
		{ return EQMask | LTMask | GTMask | GEMask | LEMask | NEMask | BTMask | NBMask | NULMask | NONUMask ; }
	case dbGenericColumnType::BLOB:         // fallthrough
	case dbGenericColumnType::CLOB:
		{ return NULMask | NONUMask ; }
	case dbGenericColumnType::GEOOBJECT:
		{ return EQMask | NEMask | NULMask | NONUMask ; }
	case dbGenericColumnType::NOTSTORED:    // fallthrough
	case dbGenericColumnType::MULTISET:     // fallthrough
	case dbGenericColumnType::UNDEFINED:    // fallthrough
	default:
		{ return NoneMask ; }
	}	
}

int dbParam::defaultOpMask(dbConnection &conn) const  // PRQA S 4020, 4214
{
	dbGenericColumnType::Type coltype ; // PRQA S 4101
	if (getType(coltype, conn)) {
		return type2Mask(coltype) ;
    }

	return NoneMask ;
}


vector<dbParam::Operation> dbParam::operationsInMask(int mask)
{
	vector<dbParam::Operation> res ;
	
	if (opInMask(mask, EQUAL_TO)) { // can do EQUAL TO ..
		res.push_back(EQUAL_TO);
    }
	
	if (opInMask(mask, LIKE)) { // can do LIKE ..
		res.push_back(LIKE);
    }
	
	if (opInMask(mask, MINOR_OF)) {// can do MINOR_OF ..
		res.push_back(MINOR_OF);
    }
	
	if (opInMask(mask, MAJOR_OF)) {// can do MAJOR_OF ..
		res.push_back(MAJOR_OF);
    }
	
	if (opInMask(mask, MAJOR_OR_EQUAL_OF)) { // can do MAJOR_OR_EQUAL_OF ..
		res.push_back(MAJOR_OR_EQUAL_OF);
    }
	
	if (opInMask(mask, MINOR_OR_EQUAL_OF)) { // can do MINOR_OR_EQUAL_OF ..
		res.push_back(MINOR_OR_EQUAL_OF);
    }
	
	if (opInMask(mask, DIFFERENT_FROM)) { // can do DIFFERENT_FROM ..
		res.push_back(DIFFERENT_FROM);
    }
	
	if (opInMask(mask, BETWEEN)) { // can do BETWEEN
		res.push_back(BETWEEN);
    }
										// etc ... etc
	if (opInMask(mask, NOT_BETWEEN)) {
		res.push_back(NOT_BETWEEN);
    }
	
	if (opInMask(mask, INSIDE)) {
		res.push_back(INSIDE);
    }

	if (opInMask(mask, CONTAINS)) {
		res.push_back(CONTAINS);
    }
	
	if (opInMask(mask, INTERSECT)) {
		res.push_back(INTERSECT);
    }
	
	if (opInMask(mask, OUTSIDE)) {
		res.push_back(OUTSIDE);
    }
	
	if (opInMask(mask, ISNULL)) {
		res.push_back(ISNULL);
    }
	
	if (opInMask(mask, IN)) {
		res.push_back(IN);
    }
	
	if (opInMask(mask, ISNOTNULL)) {
		res.push_back(ISNOTNULL);
    }

	// return a vector filled with all allowed logical operators
	return res;
}



bool dbParam::hasValue(int mask) // PRQA S 4020
{
	vector<dbParam::Operation> operations = operationsInMask(mask) ;
    size_t opsize = operations.size() ;
	for( size_t i =0 ; i< opsize ; ++i)
	{
		if (hasValue(operations[i])) {
			return true ;
        }
	}
	return false ;
}


bool dbParam::hasValue2(int mask) // PRQA S 4020
{
	vector<dbParam::Operation> operations = operationsInMask(mask) ;
    size_t opsize = operations.size() ;
	for( size_t i =0 ; i< opsize ; ++i)
	{
		if (hasValue2(operations[i])) {
			return true ;
        }
	}
	return false ;
}


bool dbParam::hasInValues(int mask) // PRQA S 4020
{
	vector<dbParam::Operation> operations = operationsInMask(mask) ;
    size_t opsize = operations.size() ;
	for( size_t i =0 ; i< opsize ; ++i)
	{
		if(hasInValues(operations[i])) {
			return true ;
        }
	}
	return false ;
}

string toString(dbParam::Operation t)  // PRQA S 4020
{

    switch(t) {
    case dbParam::NONE: { return "NONE" ; }  
    case dbParam::EQUAL_TO: { return "EQUAL_TO" ; }
    case dbParam::LIKE: { return "LIKE" ; }
    case dbParam::MINOR_OF: { return "MINOR_OF" ; }
    case dbParam::MAJOR_OF: { return "MAJOR_OF" ; }
    case dbParam::MAJOR_OR_EQUAL_OF: { return "MAJOR_OR_EQUAL_OF" ; }
    case dbParam::MINOR_OR_EQUAL_OF: { return "MINOR_OR_EQUAL_OF" ; }
    case dbParam::DIFFERENT_FROM: { return "DIFFERENT_FROM" ; }
    case dbParam::BETWEEN: { return "BETWEEN" ; }
    case dbParam::NOT_BETWEEN: { return "NOT_BETWEEN" ; }
    case dbParam::INSIDE: { return "INSIDE" ; }
    case dbParam::CONTAINS: { return "CONTAINS" ; }
    case dbParam::INTERSECT: { return "INTERSECT" ; }
    case dbParam::OUTSIDE: { return "OUTSIDE" ; }
    case dbParam::ISNULL: { return "SEISNULLT_LIST" ; }
    case dbParam::ISNOTNULL: { return "ISNOTNULL" ; }
    case dbParam::IN: { return "IN" ; }
    default:
        {
            std::ostringstream os ; 
            os << "unrecognized Operation <" << int(t) << ">" ;// PRQA S 3081
            return os.str() ;
        }
    }
}

std::ostream & operator << (std::ostream &os, dbParam::Operation t) 
{
    return os << toString(t) ;
}
exostream & operator << (exostream &os, dbParam::Operation t) {// PRQA S 4222
    return os << toString(t) ;
}


_ACS_END_NAMESPACE
