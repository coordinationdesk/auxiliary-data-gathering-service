// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 5.6  2016/04/11 13:16:18  marpas
	useless methods removed
	
	Revision 5.5  2016/03/26 21:48:28  marpas
	removing dbConnectionMgr adn dbConnectionGuard classes
	good (connected) connections can now only be obtained from a dbConnectionPool
	passing of connection reference is enforced (preferred to the obsolete method to pass a default nullable connection pointer)
	
	Revision 5.4  2013/11/05 16:43:24  enrcar
	EC:: Operators NOT_IN, NOT_IN_TABLE handled
	
	Revision 5.3  2013/07/15 16:19:13  marpas
	deprecating methods having default db key string
	
	Revision 5.2  2013/06/13 18:10:54  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.1  2013/06/11 17:03:00  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.15  2013/06/03 12:35:53  marpas
	long long supported other than unsigned long long
	
	Revision 4.14  2013/05/29 10:52:40  marpas
	IN operator accepts alsoan SQLString (useful to do DB independent subqueries)
	
	Revision 4.13  2013/05/27 10:24:29  marpas
	unsigned long long vector for IN paramaters
	
	Revision 4.12  2013/05/18 07:06:50  marpas
	removing old commented code and related warnings
	
	Revision 4.11  2013/05/14 18:14:00  marpas
	dbQuery.h
	
	Revision 4.10  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.9  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.8  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.7  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.2  2011/02/02 16:39:40  enrcar
	EC:: dbParam counter added in enum
	
	Revision 3.1  2011/01/26 15:11:55  lucio.pulvirenti
	EC+LP::  Parameter IN_TABLE handled
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.11  2009/06/19 17:47:57  marpas
	dbConnection used to speed up work and to avoid multiple connection when collecting data
	
	Revision 2.10  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.9  2009/02/17 10:41:25  marpas
	deprecation in progress
	
	Revision 2.8  2008/12/15 15:27:44  enrcar
	EC:: Misleading ctor removed
	
	Revision 2.7  2008/11/04 13:41:37  marpas
	make up and remotion of useless stuff
	
	Revision 2.6  2008/07/28 08:56:18  manuel.maccaroni
	new library OriginatorName to query tOriginators table
	
	Revision 2.5  2006/09/21 18:02:20  danalt
	Fix for GCC 4.1.0
	
	Revision 2.4  2006/03/30 14:22:35  paoscu
	Method  "bool hasValue(int mask)" and similar ones added.
	Method  "operationsInMask" added.
	Method  "getType(dbGenericColumnType::Type &type) const" fixed
	
	Revision 2.3  2006/03/17 01:44:45  paoscu
	Added methods:
	static std::string paramIdentifier(const std::string &paramName , const std::string &tbl, const std::string &col)
	static dbGenericColumnType::Type getType(const std::string & paramName, const std::string &table, const std::string &column)
	
	Method obtainTableandField moved to public.
	
	Revision 2.2  2006/03/10 17:32:34  marpas
	paramIdentifier static method added
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2006/02/17 18:01:48  marpas
	OperationMask enum and related functions added
	
	Revision 1.6  2006/01/13 18:05:35  paoscu
	Query parameters implementation completely changed.
	
	Revision 1.5  2005/10/24 16:22:45  marpas
	minor change
	
	Revision 1.4  2005/10/18 10:41:15  marpas
	headers fixed
	
	Revision 1.3  2005/07/13 09:27:42  enrcar
	updated to dbClient version 1.14
	
	Revision 1.14  2005/07/12 16:16:11  marpas
	introducing dbParam::ISNOTNULL
	dbParam::verifyDbValues flag to avoid checking into dbvalues when composing complex where clauses
	
	Revision 1.13  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.12  2004/07/20 10:15:11  marpas
	IN added to dbParam
	some minor changes and fixes
	
	Revision 1.11  2004/07/19 15:33:35  ivafam
	Param::IN added
	
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

#ifndef _dbParam_H_
#define _dbParam_H_ 

#include <acs_c++config.hpp>
#include <dbColumnType.h>
#include <dbException.h>
#include <exStream.h>
#include <iostream>
#include <string>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ; 
class rsResourceSet ;
class XMLOstream ;
class dbGeoObject ;
class SQLString ;

class dbParam  // PRQA S 2109
{

public:

	//***************************************    Operations *********************************************
	enum Operation {
		NONE = 0, 
		EQUAL_TO = 1, 			// ==
		LIKE = 2,				// LIKE (only for characters)
		MINOR_OF = 3, 			// <
		MAJOR_OF = 4, 			// >
		MAJOR_OR_EQUAL_OF = 5, 	// >=
		MINOR_OR_EQUAL_OF = 6, 	// <=
		DIFFERENT_FROM = 7, 	// !=
		BETWEEN = 8,			// BETWEEN
		NOT_BETWEEN = 9,		// !BETWEEN
								// Geodetic data blade module functions
		INSIDE = 10, 			// The INSIDE functions compare two spatial objects
								// and return true if all of the following are true:
								//	-	All point, line segments, and the interior area
								//		(if any) of the first object are spatially
								//		etirely inside the boundary and the interior area of 
								//		the second object
		INTERSECT = 11, 		// 	The INTERSECT function returns TRUE if both of the following
								//	are true:
								//	-	Any point or partial line segment of the object in the
								//		first argument lies partially inside of the object in
								//		the second argument
		OUTSIDE = 12,			// The OUTSIDE functions takes two spatial objects
								// and return true if both of the following are true:
								//	-	All points and line segments and the entire interior
								//		are (if any) of the first object lie spatially outside
								//		the second object
		ISNULL = 13,			//  -   IS NULL
		IN = 14,				// The IN function is to produce IN (,,,) where clause
		NOT_IN = 15,				// The NOT_IN function is to produce NOT IN (,,,) where clause
		IN_TABLE = 16,			// The IN_TABLE function is to produce IN ( select * from TABLE ) where clause
		NOT_IN_TABLE = 17,		// The NOT_IN_TABLE function is to produce NOT IN ( select * from TABLE ) where clause
		ISNOTNULL = 18,			//  -   IS NOT NULL
		CONTAINS = 19			//  The CONTAINS function compare two spatial objects
								//  and returns true if all of the following are true:
								//	-	All point, line segments, and the interior area
								//		(if any) of the second object are spatially
								//		etirely inside the boundary and the interior area of 
								//		the first object
                                // so if INSIDE(a,b) is true, implies CONTAINS(b,a) 
	} ; 

	enum OperationMask {
		NoneMask =			  0,	// No mask applicable
		EQMask =		1 <<  0,	// Equal
		LKMask =		1 <<  1,	// Like
		LTMask =		1 <<  2,	// Less Than
		GTMask =  		1 <<  3,	// Greather Than
		GEMask =  		1 <<  4,	// Greater Equal Than
		LEMask =  		1 <<  5,	// Less Equal Than
		NEMask =  		1 <<  6, 	// Not Equal
		BTMask =  		1 <<  7, 	// Between
		NBMask =  		1 <<  8,	// Not Between
		NULMask = 		1 <<  9,	// Null
		INMask = 		1 << 10,	// IN
		NONUMask =		1 << 11		// Not Null
	} ;


	
	//***************************************    Exceptions *********************************************
	/*! class NoGeoObject declaration */
	exDECLARE_EXCEPTION(NoGeoObject,exException) ; // PRQA S 2131, 2502

	/*! class InvalidParameter declaration */
	exDECLARE_EXCEPTION(InvalidParameter,exException) ; // PRQA S 2131, 2502

	/*! class InvalidOperation declaration */
	exDECLARE_EXCEPTION(InvalidOperation,exException) ; // PRQA S 2131, 2502


	//***************************************    dbParamItem *********************************************
	class dbParamItem { // PRQA S 2109
		
	public:
		virtual ~dbParamItem() throw() {} // PRQA S 2131
		
		virtual dbParamItem *clone() const = 0 ; // PRQA S 2502
		virtual std::string getValue() const = 0 ;

	protected:
		explicit dbParamItem(const std::string &key) : _key(key) { }
		dbParamItem(const dbParamItem &r) : _key(r._key) { }
		dbParamItem &operator=(const dbParamItem &r) { if (this != &r) { _key=r._key; } return *this ; }
    private:
        dbParamItem() ; // not implemented
	protected:
		std::string _key ;  // PRQA S 2101
				
	} ;
	
	class dbStringParamItem : public dbParamItem { // PRQA S 2109
	public:
		dbStringParamItem(const std::string &k, const std::string &s) : dbParamItem(k), _s(s) {}
		virtual ~dbStringParamItem() throw() {} // PRQA S 2131
		
		virtual dbParamItem *clone() const { return new dbStringParamItem(*this) ; } // PRQA S 2131, 2502
		virtual std::string getValue() const ;

        dbStringParamItem(const dbStringParamItem &r) : dbParamItem(r), _s(r._s) {}
        dbStringParamItem & operator=(const dbStringParamItem &r) 
        { if (this != &r) { dbParamItem::operator=(r) ;  _s = r._s; } return *this ; }
    private:
        dbStringParamItem() ; // not implemented
	private:
		std::string _s ;
	} ;
	
	class dbIntParamItem : public dbParamItem { // PRQA S 2109
	public:
		dbIntParamItem(const std::string &k, int v) : dbParamItem(k), _v(v) {}
		virtual ~dbIntParamItem() throw() {} // PRQA S 2131
		
		virtual dbParamItem *clone() const  { return new dbIntParamItem(*this) ; } // PRQA S 2131, 2502
		virtual std::string getValue() const ;

        dbIntParamItem(const dbIntParamItem &r) : dbParamItem(r), _v(r._v) {}
        dbIntParamItem & operator=(const dbIntParamItem &r) 
        { if (this != &r) { dbParamItem::operator=(r) ;  _v = r._v; } return *this ; }
    private:
        dbIntParamItem() ; // not implemented
	private:
		int _v ;
	} ;

	class dbULLParamItem : public dbParamItem { // PRQA S 2109
	public:
		dbULLParamItem(const std::string &k, unsigned long long v) : dbParamItem(k), _v(v) {}
		virtual ~dbULLParamItem() throw() {} // PRQA S 2131
		
		virtual dbParamItem *clone() const  { return new dbULLParamItem(*this) ; } // PRQA S 2131, 2502
		virtual std::string getValue() const ;

        dbULLParamItem(const dbULLParamItem &r) : dbParamItem(r), _v(r._v) {}
        dbULLParamItem & operator=(const dbULLParamItem &r) 
        { if (this != &r) { dbParamItem::operator=(r) ;  _v = r._v; } return *this ; }
    private:
        dbULLParamItem() ; // not implemented
	private:
		unsigned long long  _v ;
	} ;

	class dbLLParamItem : public dbParamItem { // PRQA S 2109
	public:
		dbLLParamItem(const std::string &k, long long v) : dbParamItem(k), _v(v) {}
		virtual ~dbLLParamItem() throw() {} // PRQA S 2131
		
		virtual dbParamItem *clone() const  { return new dbLLParamItem(*this) ; } // PRQA S 2131, 2502
		virtual std::string getValue() const ;

        dbLLParamItem(const dbLLParamItem &r) : dbParamItem(r), _v(r._v) {}
        dbLLParamItem & operator=(const dbLLParamItem &r) 
        { if (this != &r) { dbParamItem::operator=(r) ;  _v = r._v; } return *this ; }
    private:
        dbLLParamItem() ; // not implemented
	private:
		long long  _v ;
	} ;

	
	//***************************************    Methods *********************************************

#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	explicit __attribute__((deprecated)) dbParam() ; 

	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation) ; 
	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation, const std::string &, const std::string &value2="") ; 
	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation, const std::vector<std::string> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation, const std::vector<int> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation, const std::vector<unsigned long long> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::string & tableName, const std::string & fieldName, Operation, const std::vector<long long> &) ; // to be used with the IN function 

	__attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation) ; 
	__attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation, const std::string &, const std::string &value2="") ; 
    __attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation, const std::vector<std::string> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation, const std::vector<int> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation, const std::vector<unsigned long long> &) ; // to be used with the IN function 
	__attribute__((deprecated)) dbParam(const std::pair<std::string, std::string> &tblAndCol, Operation, const std::vector<long long> &) ; // to be used with the IN function 


	__attribute__((deprecated)) dbParam(const std::string & tableName, 
			                            const std::string & fieldName, 
			                            Operation, 
			                            SQLString const &v) ; // to be used with the IN function 
			
	__attribute__((deprecated)) dbParam(Operation, 
		                                SQLString const &leftConditionPart, 
			                            const std::string &value1="", 
			                            const std::string &value2="") ; 
			
	__attribute__((deprecated)) dbParam(Operation, 
			                            SQLString const &leftConditionPart, 
			                            const dbGeoObject &,
			                            const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;  // PRQA S 3081

	
	__attribute__((deprecated)) dbParam(Operation, 
			                            SQLString const &leftConditionPart, 
			                            const std::vector<std::string> &, 
			                            const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;   // PRQA S 3081
			
	__attribute__((deprecated)) dbParam(Operation, 
			                            SQLString const &leftConditionPart, 
			                            const std::vector<int> &, 
			                            const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;   // PRQA S 3081

#endif

	explicit dbParam(const std::string &key) ; 
    
	dbParam(const std::string &key, 
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation) ; 
            
	dbParam(const std::string &key, 
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, const std::string &, 
            const std::string &value2="") ; 
	
    dbParam(const std::string &key, 
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, 
            const dbGeoObject &) ; 
            
	dbParam(const std::string &key, // to be used with the IN function 
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, 
            const std::vector<std::string> &) ; 
            
	dbParam(const std::string &key,  // to be used with the IN function 
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, 
            const std::vector<int> &) ;
	
    dbParam(const std::string &key,  // to be used with the IN function
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, const std::vector<unsigned long long> &) ;
	
    dbParam(const std::string &key,  // to be used with the IN function
            const std::string & tableName, 
            const std::string & fieldName, 
            Operation, 
            const std::vector<long long> &) ; // to be used with the IN function 


	dbParam(const std::string &key, 
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation) ; 
            
	dbParam(const std::string &key, 
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const std::string &, 
            const std::string &value2="") ; 
            
	dbParam(const std::string &key, 
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const dbGeoObject &) ; 
            
	dbParam(const std::string &key, 
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const std::vector<std::string> &) ; // to be used with the IN function 
            
	dbParam(const std::string &key,  // to be used with the IN function
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const std::vector<int> &) ; 
	
    dbParam(const std::string &key,  // to be used with the IN function
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const std::vector<unsigned long long> &) ; 
            
	dbParam(const std::string &key,  // to be used with the IN function
            const std::pair<std::string, std::string> &tblAndCol, 
            Operation, 
            const std::vector<long long> &) ;


	dbParam(const std::string &key, 
            const std::string & tableName, 
			const std::string & fieldName, 
			Operation, 
			SQLString const &v) ; // to be used with the IN function 
			
	dbParam(const std::string &key, 
            Operation, 
		    SQLString const &leftConditionPart, 
			const std::string &value1="", 
			const std::string &value2="") ; 
			
	dbParam(const std::string &key, 
            Operation, 
			SQLString const &leftConditionPart, 
			const dbGeoObject &,
			const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;   // PRQA S 3081

	
	dbParam(const std::string &key, 
            Operation, 
			SQLString const &leftConditionPart, 
			const std::vector<std::string> &, 
			const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;   // PRQA S 3081
			
	dbParam(const std::string &key, 
            Operation, 
			SQLString const &leftConditionPart, 
			const std::vector<int> &, 
			const std::pair<std::string, std::string> &tblAndCol = make_pair(std::string(""), std::string(""))) ;   // PRQA S 3081


	dbParam(const dbParam &) ; 
	
	virtual ~dbParam() throw() ; 
	dbParam &operator=(const dbParam &) ; 	

	const std::string &tableName() const ;
	void tableName(const std::string &s) ;

	const std::string &fieldName() const ;
	void fieldName(const std::string &s) ;
	
	std::string composeTableAndColumn() const ;
	// Build a conventional name:
	// <tablename>_<fieldname>
	static std::string paramIdentifier(const std::string &tbl, const std::string &col) ;
	std::string paramIdentifier() const ;
	void operation(const Operation) ; 
	void value(const std::string &) ; 
	void value(const dbGeoObject &) ; 
	void value2(const std::string &) ;
	void value(const std::vector<std::string> &) ;
	void value(const std::vector<int> &) ;
	void value(const std::vector<unsigned long long> &) ;
	void value(const std::vector<long long> &) ;
	void value(const std::vector<dbParamItem *> &) ;
	void value(const SQLString &) ;

	std::string getSqlString() const ;
	std::string getKey() const { return _key ; }

	Operation operation() const ; 
	const std::string &value() const; 
	const std::string &value2() const; 
	const std::vector<dbParamItem *> &valueIn() const; 
	const dbGeoObject &geoObject() const ; 
	
	static std::string getOperatorName(const std::string &key, const Operation oper) ; 
	
	
	bool getType(dbGenericColumnType::Type &type, dbConnection &) const ;
	
	/**
	 * This method tries to get the type given the table and the column
	 * The type is obtained according to this procedure:
	 * The type is directly obtained from schema metadata
	 * The type is obtained from schema metadata
	 * If the type cannot be obtained it returns dbGenericColumnType::UNDEFINED
	 */
	static dbGenericColumnType::Type getType(const std::string &table, const std::string &column, dbConnection &) ;


	/**
	 * Obtains table and field of the old single string parameter.
	 * Tries to extract them directly from the param name in the form
	 * T_<tablename>_<fieldname>
	 */
	static std::pair<std::string, std::string> obtainTableAndField(const std::string &key, const std::string &) ;
	
    // PRQA S 2107 4 
	friend std::istream &operator>>(std::istream &, dbParam &) ;			// read from a stream
	friend std::ostream &operator<<(std::ostream &, const dbParam &) ;	// write to a stream 
	friend acs::exostream &operator<<(acs::exostream &, const dbParam &) ;	// write to a stream  // PRQA S 2072
	
	// inherited from dbExportable
	void exportObj(rsResourceSet &s) const ;        // export the class to resourceSet
	void importObj(rsResourceSet &s)  ;             // import the class from a resourceSet
	void exportXML(XMLOstream &xst) const ;         // export the class to XML stream
	

	static bool hasValue(Operation) ;
	static bool hasValue2(Operation) ;
	static bool hasInValues(Operation) ;
	
	static void has(Operation, bool *value=0, bool *value2=0, bool *inValues=0) ;
	
	static bool opInMask(int mask, Operation) ;
	static OperationMask op2Mask(Operation) ;
	static int type2Mask(dbGenericColumnType::Type) ;
	int defaultOpMask(dbConnection &) const ;
	static std::vector<Operation> operationsInMask(int mask);
	static bool hasValue(int mask) ;
	static bool hasValue2(int mask) ;
	static bool hasInValues(int mask) ;

private:
	
	void ReadSingleParameter(std::istream &, std::string &) ;
	void clearInValues() ;
	static bool isNumeric(const std::string &s,bool floating) ;
	
	
	
private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbParam) ;
		
	std::string	_sqlString ;
	std::string _leftConditionPart ; 
	std::string	_tableName ; 
	std::string	_fieldName ; 
	Operation	_operation ; 
	std::string	_value ; 
	std::string	_value2 ; 
	std::vector<dbParamItem *> _inValues ;
	dbGeoObject	*_geoObject ;
	std::string _key ; 

} ;


std::istream &operator>>(std::istream &, dbParam &) ;		 // read from a stream
std::ostream &operator<<(std::ostream &, const dbParam &) ;   // write to a stream 
exostream &operator<<(exostream &, const dbParam &) ;  // write to a stream 


std::string toString(dbParam::Operation) ;

std::ostream & operator << (std::ostream &, dbParam::Operation) ;
exostream & operator << (exostream &, dbParam::Operation) ; // PRQA S 2072

_ACS_END_NAMESPACE


#endif // _dbQueryParameters_H_
