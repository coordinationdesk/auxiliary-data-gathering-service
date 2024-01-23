// PRQA S 1050 EOF
/*

	Copyright 1995-2010, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/05/30 14:08:17  marpas
	getLanguageTypeName template method fixed
	
	Revision 4.4  2013/05/14 18:09:25  marpas
	interface rationalization.
	support for PoatGRES/PostGIS
	
	Revision 4.3  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.2  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.1  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.3  2011/03/02 13:34:25  marpas
	serial8 support added
	
	Revision 3.2  2010/09/11 18:08:57  marpas
	variable initialization
	
	Revision 3.1  2010/03/17 14:05:20  enrcar
	EC:: class dbSpecificColumnType. enum expressed as constant integer values to improve debugs.
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 1.8  2009/03/02 14:03:30  marpas
	CODECHECK parsing, no regressions
	
	Revision 1.7  2008/07/10 13:21:42  lucio.pulvirenti
	BCD type added
	
	Revision 1.6  2008/05/14 15:04:17  crivig
	yearToFraction6 introduced
	
	Revision 1.5  2006/11/21 19:22:52  paoscu
	columnToConvDateTimeMode removed.
	
	Revision 1.4  2006/11/07 16:10:22  paoscu
	Documentation improved.
	
	Revision 1.3  2006/11/02 18:44:19  paoscu
	dbDateTimeMode added.
	
	Revision 1.2  2006/03/30 14:16:56  paoscu
	Some template methods added.
	
	Revision 1.1  2006/02/28 19:05:34  paoscu
	dbColumnType imported into CVS.
	It contains two classes representing DB column types and replacing dbColumn::Type enum
	The two classes are  dbGenericColumnType (with the same values of dbColumn::Type )
	and dbSpecificColumnType in which numerical types are expanded.
	

*/


#ifndef _dbColumnType_H_
#define _dbColumnType_H_

#include <dbException.h>
#include <exStream.h>

// This is need for metod columnToConvDateTimeMode.
// The whole business is not elegant at all and only one enum shall be used
#include <dbConv.h>

#include <string>

#include <cxxabi.h> //For c++ type names demangling


namespace acs {

class DateTime;

//! The class dbSpecificColumnType can lists all the supported type in db columns.
/**
 * \note char types are ambiguous so unspecified chars are not allowed.
 */
class dbSpecificColumnType // PRQA S 2109
{
	public:
		enum Type {

			// char types are ambiguous so for now on 
			// unspecified chars are not allowed.
			// CHAR 


			// Integer signed types
			SIGNED_CHAR				=0x00, 
			SIGNED_SHORT_INT		=0x01, 
			SIGNED_INT				=0x02, 
			SIGNED_LONG_INT			=0x03,
			SIGNED_LONG_LONG_INT	=0x04,

			// Integer unsigned types
			UNSIGNED_CHAR			=0x05, 
			UNSIGNED_SHORT_INT		=0x06, 
			UNSIGNED_INT			=0x07, 
			UNSIGNED_LONG_INT		=0x08,
			UNSIGNED_LONG_LONG_INT	=0x09,

			// Floating point types		
			FLOAT					=0x0a, // 10
			DOUBLE					=0x0b,
			LONG_DOUBLE				=0x0c,

			//BCD		
			BCD						=0x0d,

			// Other types
			BOOLEAN					=0x10,
			STRING					=0x11,
			DATETIME				=0x12, 
    		BLOB					=0x13, 
			CLOB					=0x14, 
			GEOOBJECT				=0x15, 
			NOTSTORED				=0x16,  /* For Not-storable types */ 
			MULTISET				=0x17,
			UNDEFINED				=0xff	 
		}  ;
		
		template<dbSpecificColumnType::Type>
		struct LanguageTypeTraits ;
		
		template<typename T>
		struct RespectiveEnumTraits ;
	public:
		dbSpecificColumnType() ;
		~dbSpecificColumnType() ;
		dbSpecificColumnType(const dbSpecificColumnType & ) ;
		dbSpecificColumnType& operator= (const dbSpecificColumnType & ) ;

};


//! The class dbGenericColumnType can lists genric DB types.
/**
 * The difference between dbSpecificColumnType and dbGenericColumnType is that latter compresses
 * all numerical types into INTEGER and DOUBLE types.
 */
class dbGenericColumnType // PRQA S 2109
{
	public:
		enum Type {
                INTEGER		=0x00,
                DOUBLE		=0x01,
                BCD			=0x02,
                BOOLEAN		=0x03,
                STRING		=0x04,
                DATETIME	=0x05, 
                BLOB		=0x06, 
                CLOB		=0x07, 
                GEOOBJECT	=0x08, 
                NOTSTORED	=0x09,  /* For Not-storable types */ 
                MULTISET	=0x0a,
                UNDEFINED	=0xff 
		}  ;

	public:
		dbGenericColumnType() ;
		~dbGenericColumnType() ;
		dbGenericColumnType(const dbGenericColumnType & ) ;
		dbGenericColumnType& operator= (const dbGenericColumnType & ) ;
		

};


class dbDateTimeMode // PRQA S 2109
{
public:
	enum Type{
		UNKNOWN,
		Year,
		YearToMonth,
		YearToDay,
		YearToSecond,
		YearToFraction3,
		YearToFraction5,
		YearToFraction6
	} ;
    
    static std::string toString(Type) ; // PRQA S 2502
    static Type FractionDecimalDigits(unsigned short) ;
public:
	dbDateTimeMode() ;
	~dbDateTimeMode() ;
	dbDateTimeMode(const dbDateTimeMode & ) ;
	dbDateTimeMode& operator= (const dbDateTimeMode & ) ;
};

std::ostream & operator<< (std::ostream &, dbDateTimeMode::Type) ;
acs::exostream & operator<< (acs::exostream &, dbDateTimeMode::Type) ; // PRQA S 2072

//! This class is a repository of static methods.
class dbColumnTypeHelper
{
	public:
		/*! class InvalidType declaration */
		exDECLARE_EXCEPTION(InvalidType, dbException) ; // PRQA S 2131, 2153, 2502


		/**
		 * Convertion facility method 
		 */
		static dbGenericColumnType::Type convertTypeToGeneric(dbSpecificColumnType::Type const & colType) ;

		//static std::string getTypeName (dbColumn::SpecificColumnType const & colType) ;

		static std::string getTypeName (dbSpecificColumnType::Type const & colType) ;

		static std::string getTypeName (dbGenericColumnType::Type const & colType) ;


		template<typename T> static std::string getLanguageTypeName(const T &var) ;



		template<typename T> static std::string getLanguageTypeName() ;
		
		/**
		 * Depreacated method
		 * takes a string that is the Informix type
		 * 
		 */
		static dbGenericColumnType::Type getColumnType(std::string const & typeName) ;
		
		
		
		/**
		 * \deprecated
		 * convert betwee the two different enum types
		 */
		static  dbConv::DateTimeMode columnToConvDateTimeMode(dbDateTimeMode::Type) ;
	private:
		// declared but not implemented
		dbColumnTypeHelper() ;
		~dbColumnTypeHelper() ;
		dbColumnTypeHelper(const dbColumnTypeHelper & ) ;
		dbColumnTypeHelper& operator= (const dbColumnTypeHelper & ) ;

};



inline
std::ostream &operator<<(std::ostream &os, const dbGenericColumnType::Type &gctt) // PRQA S 2134
{ os << dbColumnTypeHelper::getTypeName(gctt) ; return os ; } // PRQA S 4028

inline
acs::exostream &operator<<(acs::exostream &os, const dbGenericColumnType::Type &gctt)  // PRQA S 2072, 2134, 4222
{ os << dbColumnTypeHelper::getTypeName(gctt) ; return os ; } // PRQA S 4028


inline
std::ostream &operator<<(std::ostream &os, const dbSpecificColumnType::Type &sctt)  // PRQA S 2134
{ os << dbColumnTypeHelper::getTypeName(sctt) ; return os ; } // PRQA S 4028

inline
acs::exostream &operator<<(acs::exostream &os, const dbSpecificColumnType::Type &sctt)  // PRQA S 2072, 2134, 4222
{ os << dbColumnTypeHelper::getTypeName(sctt) ; return os ; } // PRQA S 4028



template<typename T> 
inline std::string dbColumnTypeHelper::getLanguageTypeName(const T &var) // PRQA S 2010
{
	int status = 0 ;

	const std::type_info  &ti = typeid(var);

	char *realname = abi::__cxa_demangle(ti.name(), 0, 0, &status);
	std::string res; 
	if(status==0)
	{
		res=std::string(realname) ; // PRQA S 3081
		free(realname);
	}
	else {
		res=std::string("Not demangled:") + ti.name() ; // PRQA S 3081
    }
	return res ;

}

template<typename T>
inline std::string dbColumnTypeHelper::getLanguageTypeName()
{
	//There should be a way to obtain a type info without allocating a variable
	//ie without using RTTI
	return getLanguageTypeName(T());
}

		


//************************************ dbSpecificColumnType::LanguageTypeTraits  ************************************ 
// integer signed types
template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::SIGNED_CHAR> 
{typedef signed char Type ; } ; // PRQA S 2502 L1

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::SIGNED_SHORT_INT> 
{typedef signed short int Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::SIGNED_INT> 
{typedef signed int Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::SIGNED_LONG_INT> 
{typedef signed long int Type ; } ; ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::SIGNED_LONG_LONG_INT> 
{typedef signed long long int Type ; } ;


template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::UNSIGNED_CHAR> 
{typedef unsigned char Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::UNSIGNED_SHORT_INT>
{typedef unsigned short int Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::UNSIGNED_INT> 
{typedef unsigned int Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::UNSIGNED_LONG_INT> 
{typedef unsigned long int Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::UNSIGNED_LONG_LONG_INT> 
{typedef unsigned long long int Type ; } ;

//floating point types		

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::FLOAT > 
{typedef float Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::DOUBLE > 
{typedef double Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::LONG_DOUBLE > 
{typedef long double Type ; } ;


//Others
template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::BOOLEAN > 
{typedef bool Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::STRING	> 
{typedef std::string Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::DATETIME> 
{typedef DateTime Type ; } ;

template<> struct dbSpecificColumnType::LanguageTypeTraits<dbSpecificColumnType::GEOOBJECT> 
{typedef dbGeoObject Type ; } ;
// PRQA L:L1


//************************************ dbSpecificColumnType::RespectiveEnumTraits  ************************************ 
// integer signed types
template<> struct dbSpecificColumnType::RespectiveEnumTraits<signed char> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::SIGNED_CHAR ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<signed short int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::SIGNED_SHORT_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<signed int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::SIGNED_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<signed long int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::SIGNED_LONG_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<signed long long int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::SIGNED_LONG_LONG_INT ; } ;

// integer unsigned types
template<> struct dbSpecificColumnType::RespectiveEnumTraits<unsigned char> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::UNSIGNED_CHAR ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<unsigned short int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::UNSIGNED_SHORT_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<unsigned int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::UNSIGNED_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<unsigned long int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::UNSIGNED_LONG_INT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<unsigned long long int> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::UNSIGNED_LONG_LONG_INT	 ; } ;

//floating point types		
template<> struct dbSpecificColumnType::RespectiveEnumTraits<float> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::FLOAT ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<double> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::DOUBLE ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<long double> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::LONG_DOUBLE ; } ;


//Others
template<> struct dbSpecificColumnType::RespectiveEnumTraits<bool> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::BOOLEAN ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<std::string> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::STRING ; } ;

template<> struct dbSpecificColumnType::RespectiveEnumTraits<DateTime> 
{ static dbSpecificColumnType::Type const value = dbSpecificColumnType::DATETIME ; } ;


} //Close namespace

#endif // _dbColumnType_H_
