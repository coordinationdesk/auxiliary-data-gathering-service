// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 5.2  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.6  2013/06/03 12:39:44  marpas
	coding best practices partially applied
	qa rules
	
	Revision 4.5  2013/05/14 18:09:25  marpas
	interface rationalization.
	support for PoatGRES/PostGIS
	
	Revision 4.4  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.3  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.2  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2012/02/13 17:37:17  marpas
	refactoring in progress
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 1.8  2009/03/02 14:03:29  marpas
	CODECHECK parsing, no regressions
	
	Revision 1.7  2008/07/10 13:21:42  lucio.pulvirenti
	BCD type added
	
	Revision 1.6  2008/05/14 15:04:17  crivig
	yearToFraction6 introduced
	
	Revision 1.5  2006/11/21 19:22:53  paoscu
	columnToConvDateTimeMode removed.
	
	Revision 1.4  2006/03/30 14:15:41  paoscu
	dbColumnTypeHelper::getTypeName fixed.
	
	Revision 1.3  2006/03/17 01:39:45  paoscu
	Definitions included in namespace to help doxygen.
	
	Revision 1.2  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 1.1  2006/02/28 19:05:34  paoscu
	dbColumnType imported into CVS.
	It contains two classes representing DB column types and replacing dbColumn::Type enum
	The two classes are  dbGenericColumnType (with the same values of dbColumn::Type )
	and dbSpecificColumnType in which numerical types are expanded.
	

*/

#include <dbColumnType.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

dbSpecificColumnType::dbSpecificColumnType() {}
dbSpecificColumnType::~dbSpecificColumnType() {}
dbSpecificColumnType::dbSpecificColumnType(const dbSpecificColumnType & ) {}
dbSpecificColumnType& dbSpecificColumnType::operator= (const dbSpecificColumnType & ) { return *this ;} // PRQA S 4072

dbGenericColumnType::dbGenericColumnType() {}
dbGenericColumnType::~dbGenericColumnType() {}
dbGenericColumnType::dbGenericColumnType(const dbGenericColumnType & ) {}
dbGenericColumnType& dbGenericColumnType::operator= (const dbGenericColumnType & ) { return *this ;} // PRQA S 4072


dbGenericColumnType::Type dbColumnTypeHelper::convertTypeToGeneric(dbSpecificColumnType::Type const & colType) // PRQA S 4020
{
// W8111_OFF: complex ... 
// W8115_OFF: multiple returns
// convenience method to translate (multiplex)
// a dbSpecificColumnType 
// into a generic one

	switch (colType)
	{
	//case dbSpecificColumnType::CHAR: 

	// Integer signed types
	case dbSpecificColumnType::SIGNED_CHAR: 
	case dbSpecificColumnType::SIGNED_SHORT_INT: 
	case dbSpecificColumnType::SIGNED_INT: 
	case dbSpecificColumnType::SIGNED_LONG_INT:
	case dbSpecificColumnType::SIGNED_LONG_LONG_INT:

	// Integer unsigned types
	case dbSpecificColumnType::UNSIGNED_CHAR: 
	case dbSpecificColumnType::UNSIGNED_SHORT_INT: 
	case dbSpecificColumnType::UNSIGNED_INT: 
	case dbSpecificColumnType::UNSIGNED_LONG_INT:
	case dbSpecificColumnType::UNSIGNED_LONG_LONG_INT:
		{ return dbGenericColumnType::INTEGER ; }

	//Floating point types		
	case dbSpecificColumnType::FLOAT:
	case dbSpecificColumnType::DOUBLE:
	case dbSpecificColumnType::LONG_DOUBLE:
		{ return dbGenericColumnType::DOUBLE ; }

	case dbSpecificColumnType::BCD:
		{ return dbGenericColumnType::BCD ; }

	case dbSpecificColumnType::BOOLEAN:
		{ return dbGenericColumnType::BOOLEAN ; }

	case dbSpecificColumnType::STRING:
		{ return dbGenericColumnType::STRING ; }

	case dbSpecificColumnType::DATETIME: 
		{ return dbGenericColumnType::DATETIME ; }

	case dbSpecificColumnType::BLOB: 
   		{ return dbGenericColumnType::BLOB ; }

	case dbSpecificColumnType::CLOB: 
		{ return dbGenericColumnType::CLOB ; }

	case dbSpecificColumnType::GEOOBJECT: 
		{ return dbGenericColumnType::GEOOBJECT ; }

	case dbSpecificColumnType::NOTSTORED:  
		{ return dbGenericColumnType::NOTSTORED ; }

	case dbSpecificColumnType::MULTISET:
		{ return dbGenericColumnType::MULTISET ; }

	case dbSpecificColumnType::UNDEFINED:
		{ return dbGenericColumnType::UNDEFINED  ; }

	default:
		{ return dbGenericColumnType::UNDEFINED ; }

	} /* switch */
}




// convenience method to translate 
// a dbSpecificColumnType
// into a string
std::string dbColumnTypeHelper::getTypeName (dbSpecificColumnType::Type const & colType)  // PRQA S 4020, 5500
{
// NB cyclomatic complexity is just slightly high, but is due to the switch control structure below
// any attemp to lower CC would increase complexity for maintenance
	switch (colType)
	{
	case dbSpecificColumnType::SIGNED_CHAR: { return "SIGNED_CHAR" ; }
	case dbSpecificColumnType::SIGNED_SHORT_INT: { return "SIGNED_SHORT_INT" ; }
	case dbSpecificColumnType::SIGNED_INT: { return "SIGNED_INT" ; }
	case dbSpecificColumnType::SIGNED_LONG_INT: { return "SIGNED_LONG_INT" ; }
	case dbSpecificColumnType::SIGNED_LONG_LONG_INT: { return "SIGNED_LONG_LONG_INT" ; }

	// unsigned integer types
	case dbSpecificColumnType::UNSIGNED_CHAR: { return "UNSIGNED_CHAR" ; }
	case dbSpecificColumnType::UNSIGNED_SHORT_INT: { return "UNSIGNED_SHORT_INT" ; }
	case dbSpecificColumnType::UNSIGNED_INT: { return "UNSIGNED_INT" ; }
	case dbSpecificColumnType::UNSIGNED_LONG_INT: { return "UNSIGNED_LONG_INT" ; }
	case dbSpecificColumnType::UNSIGNED_LONG_LONG_INT: { return "UNSIGNED_LONG_LONG_INT" ; }

	// floating point types		
	case dbSpecificColumnType::FLOAT: { return "FLOAT" ; }
	case dbSpecificColumnType::DOUBLE: { return "DOUBLE" ; }
	case dbSpecificColumnType::LONG_DOUBLE: { return "LONG_DOUBLE" ; }
	case dbSpecificColumnType::BCD: { return "BCD" ; }

	// others	
	case dbSpecificColumnType::BOOLEAN: { return "BOOLEAN" ; }
	case dbSpecificColumnType::STRING: { return "STRING" ; }

	case dbSpecificColumnType::DATETIME: { return "DATETIME" ; }
	case dbSpecificColumnType::BLOB: { return "BLOB" ; }
	case dbSpecificColumnType::CLOB: { return "CLOB" ; }
	case dbSpecificColumnType::GEOOBJECT: { return "GEOOBJECT" ; }

	case dbSpecificColumnType::NOTSTORED: { return "NOTSTORED" ; }
	case dbSpecificColumnType::MULTISET: { return "MULTISET" ; }
	case dbSpecificColumnType::UNDEFINED: { return "UNDEFINED" ; }

// unknown: no need to throw exceptions
	default:  { return "UNKNOWN" ; }
	
	} /* switch */
} 




// convenience method to translate 
// a dbGenericColumnType
// into a string
std::string dbColumnTypeHelper::getTypeName (dbGenericColumnType::Type const & colType) // PRQA S 4020 
{
	switch (colType)
	{
	case dbGenericColumnType::INTEGER: { return "INTEGER" ; }
	case dbGenericColumnType::BOOLEAN: { return "BOOLEAN" ; }
	case dbGenericColumnType::DOUBLE: { return "DOUBLE" ; }
	case dbGenericColumnType::BCD: { return "BCD" ; }
	case dbGenericColumnType::STRING: { return "STRING" ; }
	case dbGenericColumnType::DATETIME: { return "DATETIME" ; }
	case dbGenericColumnType::BLOB: { return "BLOB" ; }
	case dbGenericColumnType::CLOB: { return "CLOB" ; }
	case dbGenericColumnType::GEOOBJECT: { return "GEOOBJECT" ; }
	case dbGenericColumnType::NOTSTORED: { return "NOTSTORED" ; }
	case dbGenericColumnType::MULTISET: { return "MULTISET" ; }
	case dbGenericColumnType::UNDEFINED: { return "UNDEFINED" ; }
    // unknown ... no need to throw exceptions
	default: { return "UNKNOWN" ; }
	
	} /* switch */
} 


/**
 * Depreacated method
 * takes a string that is the Informix type
 * 
 */
dbGenericColumnType::Type dbColumnTypeHelper::getColumnType(std::string const & typeName) // PRQA S 4020
{
// multiple returns
// convenience method to translate ... 
	if ( typeName.find("char") != string::npos ) {
        return dbGenericColumnType::STRING ;
    }
    if ( (typeName=="smallint") || (typeName=="serial") || (typeName=="integer") || (typeName=="serial8") ) {
        return dbGenericColumnType::INTEGER ; // group integral types
    }

    if ( typeName=="datetime" )  {
        return dbGenericColumnType::DATETIME ;
    }

	if ( typeName=="boolean" )  {
        return dbGenericColumnType::BOOLEAN ;
    }

	if ( (typeName.find("float") != string::npos) || (typeName=="decimal") ) {
        return dbGenericColumnType::DOUBLE ; // group floating types
    }

	if ( (typeName=="geoobject") || (typeName=="geometadata") ) {
        return dbGenericColumnType::GEOOBJECT ;
    }

	if ( typeName=="blob" ) {
        return dbGenericColumnType::BLOB ;
    }

	if ( typeName=="clob" ) {
        return dbGenericColumnType::CLOB ;
    }

	if ( typeName.find("multiset") != string::npos ) {
        return dbGenericColumnType::MULTISET ;
    }

	ACS_THROW(InvalidType("getColumnType: Unsupported Type: \"" + typeName +"\".")) ; // PRQA S 3081
}


// Deprecated
dbConv::DateTimeMode dbColumnTypeHelper::columnToConvDateTimeMode(dbDateTimeMode::Type mode) // PRQA S 4020
{
// W8115_OFF: multiple returns
// convenience method to translate ... 
	switch ( mode )
	{
	case dbDateTimeMode::Year:
		{ return  dbConv::Year ; }

	case dbDateTimeMode::YearToMonth:
		{ return dbConv::YearToMonth  ; }


	case dbDateTimeMode::YearToDay:
		{ return dbConv::YearToDay ; }


	case dbDateTimeMode::YearToSecond:
		{ return dbConv::YearToSecond; }


	case dbDateTimeMode::YearToFraction3:
		{ return dbConv::YearToFraction3 ; }


	case dbDateTimeMode::YearToFraction5:
		{ return dbConv::YearToFraction5 ; }

	case dbDateTimeMode::YearToFraction6:
		{ return dbConv::YearToFraction6 ; }

	case dbDateTimeMode::UNKNOWN:
	default :
		{
			// error: can't translate
			ostringstream errorMessage;
			errorMessage << "Unknow datetime mode " << int(mode) ; //  PRQA S 3081 2
			ACS_THROW(InvalidType(errorMessage.str())) ;
		}
	}

}

dbDateTimeMode::dbDateTimeMode() {}
dbDateTimeMode::~dbDateTimeMode() {}
dbDateTimeMode::dbDateTimeMode(const dbDateTimeMode & ) {}
dbDateTimeMode& dbDateTimeMode::operator= (const dbDateTimeMode & ) { return *this ;} // PRQA S 4072

std::string dbDateTimeMode::toString(dbDateTimeMode::Type m)  // PRQA S 4020
{
	switch(m) {
    case UNKNOWN:
        { return "UNKNOWN" ; }
	case Year:
        { return "Year" ; }
	case YearToMonth:
        { return "YearToMonth" ; }
	case YearToDay:
        { return "YearToDay" ; }
	case YearToSecond:
        { return "YearToSecond" ; }
	case YearToFraction3:
        { return "YearToFraction3" ; }
	case YearToFraction5:
        { return "YearToFraction5" ; }
	case YearToFraction6:
        { return "YearToFraction6" ; }
    default:
        { 
            ostringstream os ;
            os << "<unknown dbDateTimeMode: " << int(m) << ">" ; // PRQA S 3081
            return os.str() ; 
        }
	} 
}

dbDateTimeMode::Type dbDateTimeMode::FractionDecimalDigits(unsigned short dgts) // PRQA S 4020
{
    switch(dgts) { // PRQA S 3000
    case 0:
    case 1:
    case 2: // PRQA S 4402 L1
        { return YearToSecond ; }
    case 3:
    case 4:
        { return YearToFraction3 ; }

    case 5:
        { return YearToFraction5 ; }
    
    case 6:
    case 7:
    case 8:
    case 9: // PRQA L:L1
    default:
        { return YearToFraction6 ; }
    }
}

std::ostream & operator<< (std::ostream &os, dbDateTimeMode::Type m) 
{
    os << dbDateTimeMode::toString(m) ;
    return os ; // PRQA S 4028
}

acs::exostream & operator<< (acs::exostream &os, dbDateTimeMode::Type m) // PRQA S 4222
{
    os << dbDateTimeMode::toString(m) ;
    return os ; // PRQA S 4028
}

_ACS_END_NAMESPACE
