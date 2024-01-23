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
	Revision 5.6  2015/03/19 14:33:22  marpas
	wrong condition to discover compiler version fixed
	qa warning fixed
	
	Revision 5.5  2014/05/21 08:20:26  marfav
	Added check for compiler version less than 4.3.0 (reference to reference issue)
	
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/07/19 12:42:27  marpas
	PermissiveDbColumnConversions allows to set a long long into a int
	
	Revision 5.2  2013/06/19 18:12:08  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.1  2013/06/13 18:10:54  marpas
	libException 5.x standards adoption finished
	interface rationalization
	removing useless code
	coding best practices added
	some performances improvements.
	test fixed
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.10  2013/05/24 11:09:53  marpas
	qa rules
	
	Revision 4.9  2013/05/14 18:08:28  marpas
	interface rationalization
	improved dbGeoObject coherency support
	implementing PostGRES/PostGIS support
	
	Revision 4.8  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.7  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.1  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:13  marpas
	new statistics adopted
	
	Revision 3.1  2011/03/01 15:47:03  marpas
	GCC 4.4.x support
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.16  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.15  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.14  2009/03/02 14:03:29  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.13  2008/07/10 13:20:54  lucio.pulvirenti
	BCDValue added
	
	Revision 2.12  2008/04/07 17:30:03  ivafam
	stilll patching setValue allowing in Permissive mode int into short
	
	Revision 2.11  2008/04/07 17:14:02  ivafam
	permissive mode temporary extended to allow unsigned into signed ... to be confirmed
	
	Revision 2.10  2007/11/30 17:35:09  paoscu
	A bool dbColumto can now be set from a short int.
	
	Revision 2.9  2007/11/28 18:35:12  paoscu
	Now possible to obtain a bool from a short int column.
	
	Revision 2.8  2007/01/03 16:06:20  ivafam
	CannotCheckNull exception added.
	
	Revision 2.7  2006/11/07 16:10:22  paoscu
	Documentation improved.
	
	Revision 2.6  2006/10/11 10:58:36  paoscu
	Method getGenericColumnType added.
	
	Revision 2.5  2006/09/14 10:13:56  marfav
	Fixed to compile with gcc 3.2.2
	
	Revision 2.4  2006/07/14 12:19:25  paoscu
	Constructors and load funtions are now template funtions
	Set methods for dbGeoObject added
	Several other implementation details changed
	
	Revision 2.3  2006/03/17 19:20:08  paoscu
	Better doxygen documentation.
	
	Revision 2.2  2006/03/17 01:35:41  paoscu
	Doxygen documentation improved.
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.26  2005/12/19 09:41:27  enrcar
	getTypeName template method added
	
	Revision 1.25  2005/11/21 15:04:49  clanas
	wrapper included from dbRow.h, but no longer from dbColumn.h
	
	Revision 1.24  2005/11/18 18:57:14  paoscu
	Template methods implementation moved outside class declaration
	Template constructor and template load method added.
	
	Revision 1.23  2005/11/07 14:40:40  paoscu
	All casts from DateTime to string are now performed using DateTime::getSqlString
	instead of DateTime::sortableDateTimeString
	
	Revision 1.22  2005/10/26 13:14:48  paoscu
	Every type of column can be converted into string.
	
	Revision 1.21  2005/10/25 15:39:55  paoscu
	Geo objects can now be stored as strings.
	operator== implemented for  geo objects (using sql strings comparison)
	
	Revision 1.20  2005/10/24 16:23:52  marpas
	GEOOBJECT value to/from string supported
	
	Revision 1.19  2005/10/21 17:03:47  paoscu
	Geo objects and Clob / Blob support added.
	
	Revision 1.18  2005/10/19 13:16:09  paoscu
	Conversion trait for type 'char' added.
	
	Revision 1.17  2005/10/18 18:53:41  paoscu
	Stream insertion operators added.
	load methods fixed.
	
	Revision 1.16  2005/10/17 17:26:42  paoscu
	Excepion ErrorInNumericConversion name fixed
	New constructor that get a ColumnType
	Init and reset methods added.
	Template access method enhanced (traits used).
	
	Revision 1.15  2005/10/13 18:35:58  paoscu
	Work in progress on general access methods.
	
	Revision 1.14  2005/10/10 17:13:12  paoscu
	Work in progress.
	
	Revision 1.13  2005/10/06 21:17:46  paoscu
	Work in progress on template engine.
	
	Revision 1.12  2005/10/06 17:09:43  paoscu
	Work in progress for a new version.
	
	Revision 1.1  2005/09/13 18:03:48  paoscu
	Temporarly added to CVS.
	

	

*/

#ifndef _dbColumn_H_
#define _dbColumn_H_

#include <acs_c++config.hpp>
#include <dbConv.h>
#include <dbException.h>
#include <SQLDialectFactory.h>
#include <SQLDialect.h>
#include <dbColumnType.h>

#include <dbGeoObject.h>
#include <DateTime.h>
#include <StringUtils.h>
#include <exStream.h>
#include <Filterables.h>

#include <boost/numeric/conversion/converter.hpp>   // PRQA S 1013
#include <boost/numeric/conversion/converter_policies.hpp>  // PRQA S 1013

#include <limits>
#include <string>
#include <functional>

#include <iomanip>

#define PermissiveDbColumnConversions

#if (__GNUC__ < 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ < 3))
#define GCC_LESS_THAN_4_3_0
#endif

_ACS_BEGIN_NAMESPACE(acs)


//! The class dbColumn can store different kind (types) of values.
/**
 * - It is mainly intended for database I/O operations.
 * - It is basically a variant class, that is it can inernally
 * 	 store values of several different types.
 * - Every dbColumn has its own type from class dbSpecificColumnType
 * - Every attempt to build a dbColumn from an unsupported type will result in the compile error: 
 * -\code incomplete type `acs::dbSpecificColumnType::RespectiveEnumTraits<T>' \endcode
 * - The value of the column can be specified or not set, in this case the dbColumn object
 * - represent a NULL column.
 * - For dbSpecificColumnType::CLOB , dbSpecificColumnType::BLOB and dbSpecificColumnType::MULTISET 
 *   it is not possible to insert a value inside a dbColumn. It is still possible to store the type 
 *   and whether the column is NULL. \n
 * - Column value and type can be set together with one of the constructors or using the correspondent load methods.
 *   Note that load methods update not only value, but column type as well.
 * - Another way to set the value is using the template method void setColumn(inputType const & v ).
 * - Values can be otained using template methods ResultType & column(ResultType &v ) const \n
 *
 * \par Numerical types consistency
 * dbColumn can be compiled using two different approaches to handle numerical conversion.
 * First one is the \b permissive \b mode. It permits to set and retrieves column value 
 * passing to template methods  column(ResultType &v ) const  and setColumn(inputType const & v )
 * variables of types different from stored ones.
 * For example it is possible to set a short int passing a long long int variable and so on. 
 * Overflow condition are automatically checked and exception raised.
 * \code
 * signed long long int slli, slli2 ;
 * signed short int usi ;
 * 
 * slli = 12 ;
 * dbColumn col(slli, false) ;
 * excerr << "Value (inside a signed long long int) :" << col.column(slli2) << "\n" ; // OK (No conversion needed)
 * excerr << "Value (unsigned short int) :" << col.column(usi) << "\n" ; // OK (Convertion possible)
 * //now try to set it 
 * usi = 100 ;
 * col.setColumn(sli) ; // OK (Convertion possible)
 * // Now load the column with a really large number
 * signed long long int tooLargeInteVal = (signed long long int)boost::numeric::bounds<signed short int>::highest() + 1000 ;
 * col.load( tooLargeInteVal , false) ;
 * excerr<<col.column(usi) << "\n" ; // Overflow !! Exception is raised.
 *
 * \endcode
 * The other mode is the \b strict \b mode which throws exception whenever a dangerous numerical conversion
 * is performed.
 * \code
 * signed long int sli2 , sli = 12 ;
 * signed short int usi ;
 * signed long long int slli ;
 * dbColumn colSli(sli, false) ;
 * excerr << "Value (inside a signed long  int) : " << colSli.column(sli2) << "\n" ; // OK (No conversion)
 * excerr<<colSli.column(slli) << "\n" ; // OK (Safe conversion)
 * excerr<<colSli.column(usi) << "\n" ; // Exception: signed short int range is smaller than signed long int
 *
 * \endcode
 *
 */
class dbColumn // PRQA S 2108, 2109
{
public:	

    // PRQA S 2107 3
	friend std::ostream &operator<<(std::ostream &, const dbColumn &) ;
	friend acs::exostream &operator<<(acs::exostream &, const dbColumn &) ; // PRQA S 2072
	
	
	/**
	 * This method returns the default string to be shown to users to 
	 * indicate that a given column contains a NULL value
	 * \note In current implementation the value of this string is defined in dbColumn.C but it could be retrieven somewhere
	 *       else even at run time.
	 */
	static std::string const & nullString() ; //"<null>" The value can be stored somewhere else 



	//****************************************  Exceptions ************************************************************
	//@{ \name Exceptions
	/*! class ColumnException declaration */
	exDECLARE_EXCEPTION(ColumnException,dbException) ;  // PRQA S 2131, 2153, 2502
	
	/*! class NullColumnException declaration */
	exDECLARE_EXCEPTION(NullColumnException,dbException) ;  // PRQA S 2131, 2153, 2502

	/*! class InvalidType declaration */
	exDECLARE_EXCEPTION(InvalidType,dbException) ;  // PRQA S 2131, 2153, 2502

	/*! class CannotCheckNull declaration */
	exDECLARE_EXCEPTION(CannotCheckNull,dbException) ;  // PRQA S 2131, 2153, 2502

	/*! class ErrorInNumericConversion declaration */
	exDECLARE_EXCEPTION(ErrorInNumericConversion,dbException) ;  // PRQA S 2131, 2153, 2502
	//@}


//****************************************  Allocation/init *******************************************************
public:	
	/**
	 * Init an empty column. Type is dbSpecificColumnType::UNDEFINED
	 */
	explicit dbColumn(const std::string& key) ;

	/**
	 * Init an empty column with a give type.
	 * \param isNull Column value is NULL
	 * \param t the specific column type to be constructed
	 * It is possible to init NOT NULL columns only if type is
	 * a not storable one: dbSpecificColumnType::CLOB , dbSpecificColumnType::BLOB and dbSpecificColumnType::MULTISET
	 * \throw InvalidType An attempted to build a NOT NULL column with storable type has been made.
	*/
	explicit dbColumn(const std::string& key, dbSpecificColumnType::Type t, bool isNull=true) ;
	
	/**
	 * This template can be used for all the numerical types
	 * string, DateTime and bool
	 */
	template<typename T>
	dbColumn(const std::string &key, T const & value,  bool isNull) ;
	
	/// The type is set to dbSpecificColumnType::NOTSTORED
	dbColumn(const std::string &key, void* value, bool isNull);

	dbColumn(const dbColumn &) ; 			
	dbColumn &operator=(const dbColumn &) ; 
    // do not declare move ctor and move assigment, leave them implicit

	virtual	~dbColumn() noexcept ;					

	/**
	 * Sets the type and clear content. 
	 */
	void init(dbSpecificColumnType::Type, bool isNull=true) ;


	/**
	 * Mantains the type and clear content. 
	 */
	void reset() noexcept ;

    const std::string &getKey() const noexcept { return _key ; }


//**************************  Access Methods ****************************************
    
	
	/**
	 * \return the stored value.
	 */
	template<typename ResultType>
	ResultType & column(ResultType &v ) const ;
	
	/**
	 * Set the value. Column type must match inputType.
	 */
	template<typename inputType>
	void setColumn(inputType const & v ) ;
	
	/**
	 * \return A new copy of the stored geo object
	 *
	 * \note the pointer is NOT previously deleted.
	 * If the value contained inside the column is not a geo object an exeption is raised.
	 */
	dbGeoObject* column (dbGeoObject * & value);


//***************************************  Load methods *********************************************
	//@{ \name load methods

	/**
	 * This template can be used for all the numerical types
	 * string, DateTime and bool
	 */
	template<typename T>
	void load(T const & value, bool isNull);

	//@}
	


//***************************************  Various methods *********************************************
	bool isNull() const ;    
    void getColumnType (dbSpecificColumnType::Type& colType) const ;
    dbSpecificColumnType::Type getColumnType () const ;
	dbGenericColumnType::Type  getGenericColumnType () const ;

	bool operator==(const dbColumn &) const ; // PRQA S 2070 2
	bool operator!=(const dbColumn &) const ;

	//* Retuns the value in a string suitable for SQl. It should uses dbConv to achieve this goal
	std::string getSqlString() const ;
	
	/// \deprecated This method should be used only for debug.
	void dumpColumn () const ;


private:
    // PRQA S 2101 3
	dbSpecificColumnType::Type _type ;
	std::string _key ;
    
protected:
    template <typename type>
    static std::string getNameFromType(type const & val) ;

	
//********************************************  Private classes *******************************************
private:
	template <typename ExternalType>
	class ConversionTraits;


	//****************** Class Value ******************
	class Value {
		public:
			explicit Value(const std::string &key) : _key(key) {}
			virtual ~Value() noexcept {} // PRQA S 2131

			virtual Value *clone() const =0;    // PRQA S 2502

			virtual bool operator==(Value const &) const =0 ;   // PRQA S 2070 2
			virtual bool operator!=(Value const &) const =0 ;

			virtual std::string getSqlString() const =0 ;
			virtual acs::exostream & insertIntoStream(acs::exostream &) const =0 ;
			virtual std::ostream &   insertIntoStream(std::ostream &) const =0 ;
       		
			//*************** getValue ***************
			
			/**
			 * This should aways try to convert the value into a string
			 */
			virtual std::string& getValue(std::string& v) const = 0  ;
        	
			// Following base methods always throw exceptions
			
			// integer signed types
			virtual signed char& 			getValue(signed char& 			v ) const ;
			virtual signed short int& 		getValue(signed short int& 		v ) const ;
			virtual signed int& 			getValue(signed int&  			v ) const ;
			virtual signed long int&		getValue(signed long int& 		v ) const ;
			virtual signed long long int&	getValue(signed long long int& 	v ) const ;

			// integer unsigned types
			virtual unsigned char& 			 getValue(unsigned char& 		v ) const ;
			virtual unsigned short int&		 getValue(unsigned short int& 	v ) const ;
			virtual unsigned int& 			 getValue(unsigned int& 			v ) const ;
			virtual unsigned long int&  	 getValue(unsigned long int& 	v ) const ;
			virtual unsigned long long int&  getValue(unsigned long long int&v ) const ;

			// floating point types		
			virtual float& 					getValue(float& 				v ) const ;
			virtual double&					getValue(double&				v ) const ;
			virtual long double&			getValue(long double&			v ) const ;

			// others
        	virtual bool&                   getValue(bool&                  v ) const ;
        	virtual DateTime&               getValue(DateTime&              v ) const  ;
			virtual dbGeoObject*            getValue(dbGeoObject * &        v ) const  ; 


			//*************** setValue ***************
			
			// To help external sources to set values from string
			// It can be usefull to have smart methods to convert strings into 
			// the correct types
			virtual void setValue(std::string const & v)  = 0; 
			
			
			// integer signed types
			virtual void setValue(signed char const & v ) ;
			virtual void setValue(signed short int const & v ) ;
			virtual void setValue(signed int const & v ) ;
			virtual void setValue(signed long int const & v ) ;
			virtual void setValue(signed long long int const & v ) ;

			// integer unsigned types
			virtual void setValue(unsigned char const & v ) ;
			virtual void setValue(unsigned short int const & v ) ;
			virtual void setValue(unsigned int const & v ) ;
			virtual void setValue(unsigned long int const & v ) ;
			virtual void setValue(unsigned long long int const & v ) ;

			//floating point types		
			virtual void setValue(float const & v ) ;
			virtual void setValue(double const & v ) ;
			virtual void setValue(long double const & v ) ;


        	virtual void setValue(bool const & v) ;
        	virtual void setValue(DateTime const & v)  ;
        	virtual void setValue(dbGeoObject const & v)  ;
			const std::string &getKey() const noexcept { return _key ; }
		protected:
			Value(const Value &r) : _key(r._key) {}
			Value &operator=(const Value &v) { if (this != &v) { _key = v._key ; } return *this ; } 

        private:
            Value() ; // not implemented
			std::string _key ; // PRQA S 2101
	};


	//****************** Class DummyValue ******************
	class DummyValue : public Value{
	public:
		explicit DummyValue(const std::string &key) : Value(key) {}
		// PRQA S 2131 EOF
        virtual ~DummyValue() noexcept {} 
		virtual Value *clone() const { return new DummyValue(*this) ; } // PRQA S 2502

		DummyValue(const DummyValue &r) : Value(r) {}
		DummyValue &operator=(const DummyValue &r) {if (this != &r) { Value::operator=(r) ; } return *this ; } 

		virtual bool operator==(Value const &r) const // PRQA S 2070
            { return dynamic_cast<DummyValue const *>(&r) != 0 ;} // PRQA S 3081

		virtual bool operator!=(Value const &r) const // PRQA S 2070
            { return !operator==(r) ; }


		//this throw exception
		virtual std::string getSqlString() const  
            { ACS_THROW(InvalidType("It is not possible to have sql string from a dummy type.")) ; }  // PRQA S 3081

		virtual acs::exostream & insertIntoStream(acs::exostream &os) const
            { os << "Dummy Value" ; return os ; } // PRQA S 4028

		virtual std::ostream &   insertIntoStream(std::ostream &os) const  
            { os << "Dummy Value" ; return os ; } // PRQA S 4028

		virtual signed char &          getValue(signed char & 		   v ) const { return Value::getValue(v) ; }
		virtual signed short int &     getValue(signed short int &     v ) const { return Value::getValue(v) ; }
		virtual signed int &           getValue(signed int &           v ) const { return Value::getValue(v) ; }
		virtual signed long int &      getValue(signed long int &      v ) const { return Value::getValue(v) ; }
		virtual signed long long int & getValue(signed long long int & v ) const { return Value::getValue(v) ; }

		virtual unsigned char &         getValue(unsigned char& 		v ) const { return Value::getValue(v) ; }
		virtual unsigned short int &    getValue(unsigned short int& 	v ) const { return Value::getValue(v) ; }
		virtual unsigned int & 			getValue(unsigned int&         v ) const { return Value::getValue(v) ; }
		virtual unsigned long int &     getValue(unsigned long int& 	v ) const { return Value::getValue(v) ; }
		virtual unsigned long long int& getValue(unsigned long long int&v ) const { return Value::getValue(v) ; }

		virtual float &       getValue(float &       v ) const { return Value::getValue(v) ; }
		virtual double &      getValue(double &      v ) const { return Value::getValue(v) ; }
		virtual long double & getValue(long double & v ) const { return Value::getValue(v) ; }

        virtual bool&        getValue(bool &          v ) const { return Value::getValue(v) ; }
        virtual DateTime&    getValue(DateTime &      v ) const { return Value::getValue(v) ; }
		virtual dbGeoObject* getValue(dbGeoObject * & v ) const { return Value::getValue(v) ; }

		virtual std::string& getValue(std::string& v) const 
            { v = "Dummy Value" ; return v ; } // PRQA S 4028

		virtual void setValue(std::string const & v) 
            { ACS_THROW(InvalidType("It is not possible to set a string into a dummy type.")) ; } // PRQA S 3081

		virtual void setValue(signed char const & v )            { Value::setValue(v) ; }
		virtual void setValue(signed short int const & v )       { Value::setValue(v) ; }
		virtual void setValue(signed int const & v )             { Value::setValue(v) ; }
		virtual void setValue(signed long int const & v )        { Value::setValue(v) ; }
		virtual void setValue(signed long long int const & v )   { Value::setValue(v) ; } 

		virtual void setValue(unsigned char const & v )          { Value::setValue(v) ; }
		virtual void setValue(unsigned short int const & v )     { Value::setValue(v) ; }
		virtual void setValue(unsigned int const & v )           { Value::setValue(v) ; }
		virtual void setValue(unsigned long int const & v )      { Value::setValue(v) ; }
		virtual void setValue(unsigned long long int const & v ) { Value::setValue(v) ; }

        virtual void setValue(float const &       v ) { Value::setValue(v) ; }
        virtual void setValue(double const &      v ) { Value::setValue(v) ; }
        virtual void setValue(long double const & v ) { Value::setValue(v) ; }

        virtual void setValue(bool const &     v) { Value::setValue(v) ; }
        virtual void setValue(DateTime const & v) { Value::setValue(v) ; }
        virtual void setValue(dbGeoObject const & v) { Value::setValue(v) ; }
    private:
        DummyValue() ; // not implemented
               
	};


	//****************** Class GeoValue ******************
	class GeoValue : public Value{ // PRQA S 2109
	public:
		explicit GeoValue(const std::string &key) : Value(key), _value(0) {}
        
		GeoValue(const std::string &key, const dbGeoObject &p) : 
            Value(key), _value(p.clone()) {} // PRQA S 3380
            
		virtual ~GeoValue() noexcept { delete _value ; }
		virtual Value *clone() const { return new GeoValue(*this) ; } // PRQA S 2502

		GeoValue(const GeoValue &r) :  Value(r), _value(r._value ? r._value -> clone() : 0 ) {}  // PRQA S 3380
        
		GeoValue &operator=(const GeoValue &r ) 
        {
	        if(this!=&r) {
		        delete _value ; _value= 0 ;
		        Value::operator=(r) ;
                _value = r._value ? r._value -> clone() : 0 ; // PRQA S 3380
	        }
	        return *this ;
        }


		/**
		 * Compares sql strings
		 */
		virtual bool operator==(Value const &r) const   // PRQA S 2070
        {
	        GeoValue const *rc=dynamic_cast<GeoValue const *>(&r) ;  // PRQA S 3081
	
        	//In theory this should not happen because dbColum must 
            // check the type before calling this.
	        ACS_COND_THROW( !rc , // PRQA S 3081
                InvalidType("Error in dynamic cast! 'other' is not a dbColumn::GeoValue" )
                ) ;

        	return this->getSqlString() == rc->getSqlString() ;
        }

		virtual bool operator!=(Value const &r) const { return !operator == (r) ; }  // PRQA S 2070

		virtual std::string getSqlString() const // PRQA S 4020
        {
            if (_value) { return dbConv(getKey(), *_value) ; } // PRQA S 3050
            else { return "<unknown>" ; }
        }


		/**
		 * retuns a new copy of the stored geo object
		 * the pointer is NOT previously deleted.
		 */

		virtual acs::exostream & insertIntoStream(acs::exostream &os) const { os << getSqlString() ; return os ; } // PRQA S 4028

		virtual std::ostream & insertIntoStream(std::ostream &os) const { os << getSqlString() ; return os ; }// PRQA S 4028
        
		//Call getSqlString
		virtual std::string& getValue(std::string& v) const { v=getSqlString() ; return v ; } // PRQA S 4028


		virtual signed char &          getValue(signed char & 		   v ) const { return Value::getValue(v) ; }
		virtual signed short int &     getValue(signed short int &     v ) const { return Value::getValue(v) ; }
		virtual signed int &           getValue(signed int &           v ) const { return Value::getValue(v) ; }
		virtual signed long int &      getValue(signed long int &      v ) const { return Value::getValue(v) ; }
		virtual signed long long int & getValue(signed long long int & v ) const { return Value::getValue(v) ; }

		virtual unsigned char &         getValue(unsigned char& 		v ) const { return Value::getValue(v) ; }
		virtual unsigned short int &    getValue(unsigned short int& 	v ) const { return Value::getValue(v) ; }
		virtual unsigned int & 			getValue(unsigned int&         v ) const { return Value::getValue(v) ; }
		virtual unsigned long int &     getValue(unsigned long int& 	v ) const { return Value::getValue(v) ; }
		virtual unsigned long long int& getValue(unsigned long long int&v ) const { return Value::getValue(v) ; }
        
		virtual float &       getValue(float &       v ) const { return Value::getValue(v) ; }
		virtual double &      getValue(double &      v ) const { return Value::getValue(v) ; }
		virtual long double & getValue(long double & v ) const { return Value::getValue(v) ; }

        virtual bool&        getValue(bool &          v ) const { return Value::getValue(v) ; }
        virtual DateTime&    getValue(DateTime &      v ) const { return Value::getValue(v) ; }
		virtual dbGeoObject* getValue(dbGeoObject* &v) const { v = _value->clone() ; return v ; }

		virtual void setValue(std::string const & v) { 
            delete _value ; _value=dbGeoObject::newDbGeo(v) ; 
        }
        
		virtual void setValue(dbGeoObject const & v) { 
            delete _value ; _value=v.clone() ; 
        }

		virtual void setValue(signed char const & v )            { Value::setValue(v) ; }
		virtual void setValue(signed short int const & v )       { Value::setValue(v) ; }
		virtual void setValue(signed int const & v )             { Value::setValue(v) ; }
		virtual void setValue(signed long int const & v )        { Value::setValue(v) ; }
		virtual void setValue(signed long long int const & v )   { Value::setValue(v) ; } 

		virtual void setValue(unsigned char const & v )          { Value::setValue(v) ; }
		virtual void setValue(unsigned short int const & v )     { Value::setValue(v) ; }
		virtual void setValue(unsigned int const & v )           { Value::setValue(v) ; }
		virtual void setValue(unsigned long int const & v )      { Value::setValue(v) ; }
		virtual void setValue(unsigned long long int const & v ) { Value::setValue(v) ; }

        virtual void setValue(float const & v )                 { Value::setValue(v) ; }
        virtual void setValue(double const & v )                { Value::setValue(v) ; }
        virtual void setValue(long double const & v )           { Value::setValue(v) ; }

        virtual void setValue(bool const & v)                   { Value::setValue(v) ; }
        virtual void setValue(DateTime const & v)               { Value::setValue(v) ; }
    private:
        GeoValue() ; // not implemented
	private:
		dbGeoObject * _value ;

	};




	//****************** Class BCDValue ******************
	class BCDValue : public Value { // PRQA S 2109
	public:
		BCDValue(const std::string &key, std::string const &) ;
		virtual ~BCDValue() noexcept ;
		virtual Value *clone() const ;  // PRQA S 2502

		BCDValue(const BCDValue &) ;
		BCDValue &operator=(const BCDValue &) ;

		virtual bool operator==(Value const &) const  ; // PRQA S 2070 2
		virtual bool operator!=(Value const &) const  ;

		virtual std::string getSqlString() const  ;

		virtual acs::exostream & insertIntoStream(acs::exostream &) const  ;
		virtual std::ostream &   insertIntoStream(std::ostream &) const  ;

		//*************** getValue ***************
		virtual std::string& getValue(std::string& v) const   ;

		// integer signed types
		virtual signed char& 			getValue(signed char& 			v ) const ;
		virtual signed short int& 		getValue(signed short int& 		v ) const ;
		virtual signed int& 			getValue(signed int&  			v ) const ;
		virtual signed long int&		getValue(signed long int& 		v ) const ;
		virtual signed long long int&	getValue(signed long long int& 	v ) const ;

		// integer unsigned types
		virtual unsigned char& 			getValue(unsigned char& 		v ) const ;
		virtual unsigned short int&		getValue(unsigned short int& 	v ) const ;
		virtual unsigned int& 			getValue(unsigned int& 			v ) const ;
		virtual unsigned long int&  	getValue(unsigned long int& 	v ) const ;
		virtual unsigned long long int& getValue(unsigned long long int&v ) const ;

		// floating point types		
		virtual float& 					getValue(float& 				v ) const ;
		virtual double&					getValue(double&				v ) const ;
		virtual long double&			getValue(long double&			v ) const ;

        virtual bool&        getValue(bool &          v ) const { return Value::getValue(v) ; }
        virtual DateTime&    getValue(DateTime &      v ) const { return Value::getValue(v) ; }
		virtual dbGeoObject* getValue(dbGeoObject * & v ) const { return Value::getValue(v) ; }
		//*************** setValue ***************

		virtual void setValue(std::string const & v) ; 

		virtual void setValue(signed char const & v )            { Value::setValue(v) ; }
		virtual void setValue(signed short int const & v )       { Value::setValue(v) ; }
		virtual void setValue(signed int const & v )             { Value::setValue(v) ; }
		virtual void setValue(signed long int const & v )        { Value::setValue(v) ; }
		virtual void setValue(signed long long int const & v )   { Value::setValue(v) ; } 

		virtual void setValue(unsigned char const & v )          { Value::setValue(v) ; }
		virtual void setValue(unsigned short int const & v )     { Value::setValue(v) ; }
		virtual void setValue(unsigned int const & v )           { Value::setValue(v) ; }
		virtual void setValue(unsigned long int const & v )      { Value::setValue(v) ; }
		virtual void setValue(unsigned long long int const & v ) { Value::setValue(v) ; }

        virtual void setValue(float const & v )                 { Value::setValue(v) ; }
        virtual void setValue(double const & v )                { Value::setValue(v) ; }
        virtual void setValue(long double const & v )           { Value::setValue(v) ; }

        virtual void setValue(bool const & v)                   { Value::setValue(v) ; }
        virtual void setValue(DateTime const & v)               { Value::setValue(v) ; }
        virtual void setValue(dbGeoObject const & v)               { Value::setValue(v) ; }
    private:
        BCDValue() ; // not implemented 
	private:
		std::string _value ;

	};


	//****************** Template Class  *********************
	
	
	/**
	 * This class has been introduced to manage basic nueric types types + bool + string + DateTime
	 */
	template<typename T>
	class ValueInst : public Value // PRQA S 2109
	{
    public:
/* Test for GCC >= 4.3.0 */
#ifdef GCC_LESS_THAN_4_3_0
		ValueInst(const std::string &key, T value) ;
#else
		ValueInst(const std::string &key, T const & value) ;
#endif

		ValueInst(ValueInst<T> const & other) : Value(other), _value(other._value) {} ;

		ValueInst<T> & operator=(ValueInst<T> const & other) ;
		virtual ~ValueInst() noexcept ;
		virtual Value *clone() const;  // PRQA S 2502

		virtual bool operator==(Value const &) const  ;  // PRQA S 2070 2
		virtual bool operator!=(Value const &) const  ;
		virtual std::string getSqlString() const  ;
		virtual acs::exostream & insertIntoStream(acs::exostream &) const ;
		virtual std::ostream &   insertIntoStream(std::ostream &) const ;

		// *******************  Get methods  ******************* 

		// getValue(std::string& v)   must be always specialized
		virtual std::string& getValue(std::string& v) const  ;

 		// Safe conversions are directly implemented with specialized template methods
		// Others are handled by convertToDifferentType method

		// integer signed types
		virtual signed char& getValue(signed char& v ) const ;
		virtual signed short int& getValue(signed short int& v ) const ;
		virtual signed int& getValue(signed int& v ) const ;
		virtual signed long int& getValue(signed long int& v ) const ;
		virtual signed long long int& getValue(signed long long int& v ) const ;

		// integer unsigned types
		virtual unsigned char& getValue(unsigned char& v ) const ;
		virtual unsigned short int& getValue(unsigned short int& v ) const ;
		virtual unsigned int& getValue(unsigned int& v ) const ;
		virtual unsigned long int& getValue(unsigned long int& v ) const ;
		virtual unsigned long long int& getValue(unsigned long long int&v ) const ;

		//floating point types		
		virtual float& getValue(float& v ) const ;
		virtual double& getValue(double& v ) const ;
		virtual long double& getValue(long double& v ) const ;

        virtual bool& getValue(bool& v ) const ;
        virtual DateTime& getValue(DateTime& v ) const ;
		virtual dbGeoObject* getValue(dbGeoObject* & v ) const ;



		// *******************  Set methods  ******************* 

		// setValue(std::string const & v)   must be always specialized
		virtual void setValue(std::string const & v) ;

		// integer signed types
		virtual void setValue(signed char const & v ) ;
		virtual void setValue(signed short int const & v ) ;
		virtual void setValue(signed int const & v ) ;
		virtual void setValue(signed long int const & v ) ;
		virtual void setValue(signed long long int const & v ) ;

		// integer unsigned types
		virtual void setValue(unsigned char const & v ) ;
		virtual void setValue(unsigned short int const & v ) ;
		virtual void setValue(unsigned int const & v ) ;
		virtual void setValue(unsigned long int const & v ) ;
		virtual void setValue(unsigned long long int const & v ) ;

		//floating point types		
		virtual void setValue(float const & v ) ;
		virtual void setValue(double const & v ) ;
		virtual void setValue(long double const & v ) ;


        virtual void setValue(bool const & v) ;
        virtual void setValue(DateTime const & v) ;
		virtual void setValue(const dbGeoObject & v) ;



#ifdef PermissiveDbColumnConversions 
 	private:
  		struct overflowHandler
		{
			void operator() ( boost::numeric::range_check_result r );
		} ;

		template<typename RequestedType>
			RequestedType	convertToDifferentType(RequestedType&) const ;
#endif
    private:
        ValueInst() ; // not implemented
	private:
		T _value ;
	};



//****************************************  Private methods *********************************
	/**
	 * Just throws exception if _value is null
	 */
	void checkNull() const ;


	template<typename ResultType>
	void getIncompatibilityException(ResultType &v ) const ;

	template<typename externalType>
	void setIncompatibilityException(InvalidType &x, externalType &v ) const ;

	
	static Value * createValue(const std::string &key, dbSpecificColumnType::Type ) ;
	
//****************************************  Private members *********************************
private:
	dbColumn::Value *_value ;
	static std::string const _nullString ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbColumn)

} ;  //dbColumn


//**********************************************************************************************************************
//***************************************** END OF dbColumn CLASS DECLARATION ******************************************
//**********************************************************************************************************************


/**
 * This method calls  insertIntoStream(std::ostream &)
 */
std::ostream &operator<<(std::ostream &, const dbColumn &) ;


/**
 * This method calls  insertIntoStream(acs::exostream &)
 */
acs::exostream &operator<<(acs::exostream &, const dbColumn &) ;

template<typename T> 
inline
dbColumn::dbColumn(const std::string &key, T const & value,  bool isNull) :
    _type(),
	_key(key), 
	_value(0)
{
	load(value , isNull) ;
}	


template<typename T> 
inline
void dbColumn::load(T const & value, bool isNull)
{
	dbColumn::reset() ;
	_type = dbSpecificColumnType::RespectiveEnumTraits<T>::value ;
	if (!isNull)
		_value = new ValueInst<T>(_key, value) ; 
}	


// //**********************************  Template access methods definitions ****************************************

template<typename ResultType> 
inline
ResultType & dbColumn::column(ResultType &v ) const
{
	checkNull() ;
	try{
		v= _value->getValue(v) ;
	}
	catch(InvalidType &e) {
		getIncompatibilityException(v) ;
	}
	return v ; // PRQA S 4028
}
	



template<typename inputType> 
inline
void dbColumn::setColumn(inputType const & v )
{
	//If the column is NULL I must now init it with the correct type
	bool toDelete=false ; //If setValue fails _value maybe is to be deleted
	ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "this: " << this 
            << " key: \"" << _key << "\" _type: " 
            << dbColumnTypeHelper::getTypeName(_type) << " _value: " << _value) ;
	if(!_value)
	{
		toDelete = true ;
		_value = createValue(_key, _type) ;
		//Now its value is undefined but it will be set hereafter
	}
	try{
		_value->setValue(v) ;
	}
	catch(InvalidType &e){
		if(toDelete){
			delete _value ;
			_value=0 ;
		}
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, e) ;
		setIncompatibilityException(e, v) ;
	}
}



template<typename ResultType> 
inline
void dbColumn::getIncompatibilityException(ResultType &v ) const
{
	ACS_THROW( // PRQA S 3081
        InvalidType("Impossible to get a " + getNameFromType(v) + " from a field of type " + dbColumnTypeHelper::getTypeName(_type))
    ) ;
}

template<typename externalType> 
inline
void dbColumn::setIncompatibilityException(InvalidType &x, externalType &v ) const
{
	ACS_THROW( // PRQA S 3081
        InvalidType(x, 
                    "Impossible to set a " + getNameFromType(v) + " into a field of type " + dbColumnTypeHelper::getTypeName(_type))) ;
}


template <typename T> 
inline
std::string dbColumn::getNameFromType(T const & val)
{
	return dbColumnTypeHelper::getLanguageTypeName(val) ;
}

#ifdef PermissiveDbColumnConversions 
// Safe conversions are directly implemented with specialized template methods
// Others are handled by convertToDifferentType method

#else	
// Default getValue methods call the correspective in Value class
// thus generating an exception
// Possible conversion must be implemented as specialized methods

// PRQA S 2133, 2134 L1
// integer signed types
template<> 
inline signed char& dbColumn::getValue(signed char& v ) const 
{return Value::getValue(v) ; }

template<> 
inline signed short int& dbColumn::getValue(signed short int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline signed int& dbColumn::getValue(signed int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline signed long int& dbColumn::getValue(signed long int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline signed long long int& dbColumn::getValue(signed long long int& v ) const 
{return Value::getValue(v) ; }

// integer unsigned types
template<> 
inline unsigned char& dbColumn::getValue(unsigned char& v ) const 
{return Value::getValue(v) ; }

template<> 
inline unsigned short int& dbColumn::getValue(unsigned short int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline unsigned int& dbColumn::getValue(unsigned int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline unsigned long int& dbColumn::getValue(unsigned long int& v ) const 
{return Value::getValue(v) ; }

template<> 
inline unsigned long long int& dbColumn::getValue(unsigned long long int &v ) const 
{return Value::getValue(v) ; }

//floating point types		
template<> 
inline float& dbColumn::getValue(float &v ) const 
{return Value::getValue(v) ; }

template<> 
inline double& dbColumn::getValue(double & v ) const 
{return Value::getValue(v) ; }

template<> 
inline long double& dbColumn::getValue(long double & v ) const 
{return Value::getValue(v) ; }
// PRQA L:L1
#endif



// //******************************************  getNameFromType specialization *****************************************
// // PAOLO doxygen complain a lot with these methods.
// 
// /**
//  * For default type just print UNKNOWN
//  */
// template <typename type> inline
// std::string dbColumn::getNameFromType(type const & val)
// {
// 	return "UNKNOWN" ;
// }
// 
// // integer signed types
// template <> inline std::string dbColumn::getNameFromType<signed char>			( signed char 				const & val) { return "signed char" ; }
// template <> inline std::string dbColumn::getNameFromType<signed short int>		( signed short int 			const & val) { return "signed short int" ; }
// template <> inline std::string dbColumn::getNameFromType<signed int>			( signed int 				const & val) { return "signed int" ; }
// template <> inline std::string dbColumn::getNameFromType<signed long int>		( signed long int 			const & val) { return "signed long int" ; }
// template <> inline std::string dbColumn::getNameFromType<signed long long int>	( signed long long int 		const & val) { return "signed long long int" ; }
// 
// // integer unsigned types
// template <> inline std::string dbColumn::getNameFromType<unsigned char>			( unsigned char 			const & val) { return "unsigned char" ; }
// template <> inline std::string dbColumn::getNameFromType<unsigned short int>	( unsigned short int		const & val) { return "unsigned short int" ; }
// template <> inline std::string dbColumn::getNameFromType<unsigned int>			( unsigned int				const & val) { return "unsigned int" ; }
// template <> inline std::string dbColumn::getNameFromType<unsigned long int>		( unsigned long int			const & val) { return "unsigned long int" ; }
// template <> inline std::string dbColumn::getNameFromType<unsigned long long int>( unsigned long long int 	const & val) { return "unsigned long long int" ; }
// 
// //floating point types		
// template <> inline std::string dbColumn::getNameFromType<float>					( float 					const & val) { return "float" ; }
// template <> inline std::string dbColumn::getNameFromType<double>				( double 					const & val) { return "double" ; }
// template <> inline std::string dbColumn::getNameFromType<long double>			( long double 				const & val) { return "long double" ; }
// 
// //Others
// template <> inline std::string dbColumn::getNameFromType<std::string>			( std::string 				const & val) { return "std::string" ; }
// template <> inline std::string dbColumn::getNameFromType<bool>					( bool 						const & val) { return "bool" ; }
// template <> inline std::string dbColumn::getNameFromType<DateTime>				( DateTime 					const & val) { return "DateTime" ; }
// template <> inline std::string dbColumn::getNameFromType<dbGeoObject*>			( dbGeoObject* 				const & val) { return "dbGeoObject* " ; }
// 

//******************************************  ColumnTypeToLanguageTypeConverter specialization *****************************************
/*
template <>
class dbColumn::ColumnTypeToLanguageTypeConverter<dbSpecificColumnType::SIGNED_CHAR>{
	public:
		typedef signed char storedType ;
} ;
*/

//***********************************  Template Class ValueInst defnitions ***************************

template<typename T> 

#ifdef GCC_LESS_THAN_4_3_0
inline dbColumn::ValueInst<T>::ValueInst(const std::string &key, T value): Value(key), _value(value) { } // PRQA S 2010, 2133, 2134
#else
inline dbColumn::ValueInst<T>::ValueInst(const std::string &key, T const & value): Value(key), _value(value) { } // PRQA S 2133, 2134
#endif


template<typename T> 
inline dbColumn::ValueInst<T>::~ValueInst() noexcept {}


template<typename T> 
inline dbColumn::ValueInst<T> & dbColumn::ValueInst<T>::operator=(dbColumn::ValueInst<T> const & other)
{
	if(this != &other) {
		Value::operator= (other) ;
		_value = other._value ;
	}
  
	return *this;
}



template<typename T> 
inline dbColumn::Value *dbColumn::ValueInst<T>::clone() const
{ return new ValueInst<T>(*this) ; }


template<typename T> 
inline bool dbColumn::ValueInst<T>::operator==(Value const & other) const
{
// W8115_OFF: multiple returns
	dbColumn::ValueInst<T> const *otherCasted = dynamic_cast<ValueInst<T> const *>(&other) ;  // PRQA S 3081
	
	//In theory this should not happen because dbColum must check the type before calling this method
	if(!otherCasted) {
		return false;
    }
	else {
		return _value== otherCasted->_value ;
    }
}

template<typename T> 
inline bool& dbColumn::ValueInst<T>::getValue(bool& v ) const 
{return Value::getValue(v) ; }

template<typename T> 
inline DateTime& dbColumn::ValueInst<T>::getValue(DateTime& v ) const 
{return Value::getValue(v) ; }

template<typename T> 
inline dbGeoObject* dbColumn::ValueInst<T>::getValue(dbGeoObject* & v ) const 
{return Value::getValue(v) ; } 


template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(signed char const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(signed short int const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(signed int const & v )
{ return Value::setValue(v) ; }
template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(signed long int const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(signed long long int const & v )
{ return Value::setValue(v) ; }

// integer unsigned types
template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(unsigned char const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(unsigned short int const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(unsigned int const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(unsigned long int const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(unsigned long long int const & v )
{ return Value::setValue(v) ; }

//floating point types		
template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(float const & v )
{ return Value::setValue(v) ; }


template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(double const & v )
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(long double const & v )
{ return Value::setValue(v) ; }


template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(bool const & v)
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(DateTime const & v)
{ return Value::setValue(v) ; }

template<typename T> 
inline void dbColumn::ValueInst<T>::setValue(const dbGeoObject & v)
{ return Value::setValue(v) ; } 





template<typename T> 
inline bool dbColumn::ValueInst<T>::operator!=(Value const & other) const
{ return !(*this == other) ; }


template<typename T> 
inline std::string dbColumn::ValueInst<T>::getSqlString() const
{ return dbConv(getKey(), _value) ; }


// PRQA S 2133, 2134, 3050 EOF
//NOTE dbConv() has a different form for DateTime
template<> 
inline std::string dbColumn::ValueInst<DateTime>::getSqlString() const
{
	//warning DateTime sql string is always YearToFraction
	//Maybe an enhanced method can be used to obtain from the schema the better representation
	return dbConv(getKey(), _value, dbConv::YearToFraction) ;   // PRQA S 3050
}


//Stream insertion methods.

template<typename T> 
inline acs::exostream & dbColumn::ValueInst<T>::insertIntoStream(acs::exostream & s) const
{ s << _value ;	return s ; }

// PRQA S 3380, 4028 EOF
// PRQA S 2133, 2134 L2

template<> 
inline acs::exostream & dbColumn::ValueInst<bool>::insertIntoStream(acs::exostream & s) const
{
	//PAOLO In theory these strings could be configurable
	s << std::boolalpha << _value << std::noboolalpha ;
	return s ;
}

template<> 
inline acs::exostream & dbColumn::ValueInst<acs::DateTime>::insertIntoStream(acs::exostream & s) const
{
	std::ostringstream os;
	_value.getSqlString(os) ;
	return s << os.str();
}
// PRQA L:L2

template<typename T> 
inline std::ostream & dbColumn::ValueInst<T>::insertIntoStream(std::ostream & s) const
{ s << _value ;	return s ; }

// PRQA S 2133, 2134 L3
template<> 
inline std::ostream & dbColumn::ValueInst<bool>::insertIntoStream(std::ostream & s) const
{ s << std::boolalpha << _value << std::noboolalpha ; return s ; }

template<> 
inline std::ostream & dbColumn::ValueInst<acs::DateTime>::insertIntoStream(std::ostream & s) const
{ _value.getSqlString(s) ; return s ; }





//*********************************************************************************************************
//*********************************************  Get methods  *********************************************
//*********************************************************************************************************



//************************* Methods to return a bool ***********************
template<> 
inline bool& dbColumn::ValueInst<bool>::getValue(bool& v) const
{ v=_value ; return v ; }

// This is required because MySQL uses short int as boolean type
template<> 
inline bool& dbColumn::ValueInst<short int>::getValue(bool& v) const
{ v=(_value != 0) ; return v ; }

// PRQA L:L3

//************************* Methods to return a string ***********************
// Default one implies the  the use of a ostringstream
template<typename T> 
inline std::string& dbColumn::ValueInst<T>::getValue(std::string& v) const
{ std::ostringstream os; os << _value ;	v = os.str() ; return v ; }

// PRQA S 2133, 2134 L4

template<> 
inline std::string& dbColumn::ValueInst<std::string>::getValue(std::string& v) const
{ v=_value ; return v ; }
	
	

template<> 
inline std::string& dbColumn::ValueInst<DateTime>::getValue(std::string& v) const
{ std::ostringstream os; _value.getSqlString(os) ; v = os.str() ; return v ; }

template<> 
inline std::string& dbColumn::ValueInst<bool>::getValue(std::string& v) const
{
	if(_value) {
		v = SQLDialectFactory::sqlDialect(getKey()).boolTrue() ;
    }
	else {
		v = SQLDialectFactory::sqlDialect(getKey()).boolFalse() ;
    }
	return v ; 		
}


//************************* Methods to return a DateTime ***********************
template<> 
inline DateTime& dbColumn::ValueInst<DateTime>::getValue(DateTime& v) const
{ return ( v=_value ); }


//PAOLO it could be possible to configure (at DB level) 
//which kind of JD to use to convert floating point values
//to DateTime
template<> 
inline DateTime& dbColumn::ValueInst<float>::getValue(DateTime& v) const
{ return v=DateTime::JD50(_value) ; } // PRQA S 3081 EOF

template<> 
inline DateTime& dbColumn::ValueInst<double>::getValue(DateTime& v) const
{ return v=DateTime::JD50(_value) ; }


template<> 
inline DateTime& dbColumn::ValueInst<long double>::getValue(DateTime& v) const
{ return v=DateTime::JD50(_value) ; }



// PRQA L:L4




//**************************************** GENERIC IMPLEMENTATION TO BE DEPRECATED !!!********************************************************
#ifdef PermissiveDbColumnConversions 
template<typename T> 
inline void dbColumn::ValueInst<T>::overflowHandler::operator() ( boost::numeric::range_check_result r )
{
	ACS_COND_THROW( r == boost::numeric::cNegOverflow ||
							    r == boost::numeric::cPosOverflow,
								ErrorInNumericConversion("Overflow error.")
							  ) ;
}

#ifndef CODECHECK
template<typename storedType>
template<typename RequestedType> 
inline
RequestedType dbColumn::ValueInst<storedType>::convertToDifferentType(RequestedType& v) const
{
	typedef boost::numeric::converter<    RequestedType,
				                          storedType,
										  boost::numeric::conversion_traits<RequestedType , storedType> ,
										  overflowHandler
									 > storedType2RequestedType ;
	try{
		v=storedType2RequestedType::convert( _value );
	}
	catch(std::exception &x)
	{
		std::ostringstream errorMessage;
		errorMessage << "Overflow error: Cannot convert " << _value << "  into the requested type: " << getNameFromType(RequestedType())  ;
		ACS_THROW(ErrorInNumericConversion(x,errorMessage.str())) ;  // PRQA S 3081
	}

	return v ;
}
#endif


template<>
template<typename RequestedType> 
inline
RequestedType dbColumn::ValueInst<DateTime>::convertToDifferentType(RequestedType&) const
{
	ACS_THROW(InvalidType("Overflow error: Cannot convert a Datetime into the requested type: " + getNameFromType(RequestedType()))) ; // PRQA S 3081
	//RequestedType res ;
	//return res ;
}

template<>
template<typename RequestedType> 
inline
RequestedType dbColumn::ValueInst<std::string>::convertToDifferentType(RequestedType&) const
{
	ACS_THROW(InvalidType("Overflow error: Cannot convert a string into the requested type: " + getNameFromType(RequestedType()))) ; // PRQA S 3081
	//RequestedType res ;
	//return res ;
}

template<>
template<typename RequestedType> 
inline
RequestedType dbColumn::ValueInst<dbGeoObject&>::convertToDifferentType(RequestedType&) const
{
	ACS_THROW(InvalidType("Overflow error: Cannot convert a dbGeoObject into the requested type: " + getNameFromType(RequestedType()))) ; // PRQA S 3081
}

template<>
template<typename RequestedType> 
inline
RequestedType dbColumn::ValueInst<bool>::convertToDifferentType(RequestedType&) const
{
	ACS_THROW(InvalidType("Overflow error: Cannot convert a bool into the requested type: " + getNameFromType(RequestedType()))) ; // PRQA S 3081
}





template<typename T> 
inline signed char& dbColumn::ValueInst<T>::getValue(signed char & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline signed short int& dbColumn::ValueInst<T>::getValue(signed short int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline signed int& dbColumn::ValueInst<T>::getValue(signed int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline signed long int& dbColumn::ValueInst<T>::getValue(signed long int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline signed long long int& dbColumn::ValueInst<T>::getValue(signed long long int & v)	const 
{ convertToDifferentType(v) ; return v ; }


template<typename T> 
inline unsigned char& dbColumn::ValueInst<T>::getValue(unsigned char & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline unsigned short int& dbColumn::ValueInst<T>::getValue(unsigned short int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline unsigned int& dbColumn::ValueInst<T>::getValue(unsigned int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline unsigned long int& dbColumn::ValueInst<T>::getValue(unsigned long int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline unsigned long long int&	dbColumn::ValueInst<T>::getValue(unsigned long long int & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline float& dbColumn::ValueInst<T>::getValue(float & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline double& dbColumn::ValueInst<T>::getValue(double & v) const 
{ convertToDifferentType(v) ; return v ; }

template<typename T> 
inline long double& dbColumn::ValueInst<T>::getValue(long double & v) const 
{ convertToDifferentType(v) ; return v ; }

#endif // PermissiveDbColumnConversions




//*********************************************** Numerical types  ****************************************
// NOTE: all the following numerical conversions assume long int == int


//**************************************** char ********************************************************
// template<> inline signed char& 			dbColumn::ValueInst<char>::getValue(signed char & v)		 const { v=_value ; return v ; }
// template<> inline signed short int& 	dbColumn::ValueInst<char>::getValue(signed short int & v)	 const { v=_value ; return v ; }
// template<> inline signed int& 			dbColumn::ValueInst<char>::getValue(signed int & v) 		 const { v=_value ; return v ; }
// template<> inline signed long int& 		dbColumn::ValueInst<char>::getValue(signed long int & v)	 const { v=_value ; return v ; }
// template<> inline signed long long int& dbColumn::ValueInst<char>::getValue(signed long long int & v)const { v=_value ; return v ; }

//**************************************** signed char ********************************************************
template<> 
inline signed char& dbColumn::ValueInst<signed char>::getValue(signed char & v) const 
{ v=_value ; return v ; }

template<> 
inline signed short int& dbColumn::ValueInst<signed char>::getValue(signed short int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed int& dbColumn::ValueInst<signed char>::getValue(signed int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long int& dbColumn::ValueInst<signed char>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<signed char>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }

//**************************************** signed short int ********************************************************
template<> 
inline signed short int& dbColumn::ValueInst<signed short int>::getValue(signed short int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed int& dbColumn::ValueInst<signed short int>::getValue(signed int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long int& dbColumn::ValueInst<signed short int>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<signed short int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }


//**************************************** signed int ********************************************************
template<> 
inline signed int& dbColumn::ValueInst<signed int>::getValue(signed int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long int& dbColumn::ValueInst<signed int>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<signed int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }

//**************************************** signed long int ********************************************************

template<> 
inline signed int& dbColumn::ValueInst<signed long int>::getValue(signed int & v) const 
{ v=_value ; return v ; } // PRQA S 3010

template<> 
inline signed long int& dbColumn::ValueInst<signed long int>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<signed long int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }

//**************************************** signed long  long int ********************************************************
template<> 
inline signed long long int& dbColumn::ValueInst<signed long long int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }


//**************************************** unsigned char ********************************************************
template<> 
inline signed short int& dbColumn::ValueInst<unsigned char>::getValue(signed short int & v) const 
{ v=_value ; return v ; } // PRQA S 3000

template<> 
inline signed int& dbColumn::ValueInst<unsigned char>::getValue(signed int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long int& dbColumn::ValueInst<unsigned char>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<unsigned char>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }


template<> 
inline unsigned char& dbColumn::ValueInst<unsigned char>::getValue(unsigned char & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned short int& dbColumn::ValueInst<unsigned char>::getValue(unsigned short int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned int& dbColumn::ValueInst<unsigned char>::getValue(unsigned int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long int& dbColumn::ValueInst<unsigned char>::getValue(unsigned long int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long long int& dbColumn::ValueInst<unsigned char>::getValue(unsigned long long int & v) const 
{ v=_value ; return v ; }



//**************************************** unsigned short int ********************************************************
template<> 
inline signed int& dbColumn::ValueInst<unsigned short int>::getValue(signed int & v) const
{ v=_value ; return v ; } // PRQA S 3000

template<> 
inline signed long int& dbColumn::ValueInst<unsigned short int>::getValue(signed long int & v) const 
{ v=_value ; return v ; }

template<> 
inline signed long long int& dbColumn::ValueInst<unsigned short int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }


template<> 
inline unsigned short int& dbColumn::ValueInst<unsigned short int>::getValue(unsigned short int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned int& dbColumn::ValueInst<unsigned short int>::getValue(unsigned int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long int& dbColumn::ValueInst<unsigned short int>::getValue(unsigned long int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long long int& dbColumn::ValueInst<unsigned short int>::getValue(unsigned long long int & v) const 
{ v=_value ; return v ; }


//**************************************** unsigned int ********************************************************
template<> 
inline signed long long int& dbColumn::ValueInst<unsigned int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; } // PRQA S 3000



template<> 
inline unsigned int& dbColumn::ValueInst<unsigned int>::getValue(unsigned int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long int& dbColumn::ValueInst<unsigned int>::getValue(unsigned long int & v) const 
{ v=_value ; return v ; }

template<> 
inline unsigned long long int& dbColumn::ValueInst<unsigned int>::getValue(unsigned long long int & v) const 
{ v=_value ; return v ; }

//**************************************** unsigned long int ********************************************************
template<> 
inline signed long long int& dbColumn::ValueInst<unsigned long int>::getValue(signed long long int & v) const 
{ v=_value ; return v ; }  // PRQA S 3000


template<> 
inline unsigned int& dbColumn::ValueInst<unsigned long int>::getValue(unsigned int & v) const 
{ v=_value ; return v ; } // PRQA S 3010

template<> 
inline unsigned long int& dbColumn::ValueInst<unsigned long int>::getValue(unsigned long int & v) const 
{ v=_value ; return v ; }

template<>
inline unsigned long long int& dbColumn::ValueInst<unsigned long int>::getValue(unsigned long long int & v)	const 
{ v=_value ; return v ; }



//**************************************** unsigned long long int ********************************************************
template<> 
inline unsigned long long int& dbColumn::ValueInst<unsigned long long int>::getValue(unsigned long long int & v) const 
{ v=_value ; return v ; }


//**************************************** float ********************************************************
template<> 
inline float& dbColumn::ValueInst<float>::getValue(float & v) const 
{ v=_value ; return v ; }

template<> 
inline double& dbColumn::ValueInst<float>::getValue(double & v) const 
{ v=_value ; return v ; }

template<> 
inline long double& dbColumn::ValueInst<float>::getValue(long double & v) const 
{ v=_value ; return v ; }


//**************************************** double ********************************************************
template<> 
inline double& dbColumn::ValueInst<double>::getValue(double & v) const 
{ v=_value ; return v ; }

template<> 
inline long double& dbColumn::ValueInst<double>::getValue(long double & v) const 
{ v=_value ; return v ; }

//**************************************** long double ********************************************************
template<> 
inline long double& dbColumn::ValueInst<long double>::getValue(long double & v) const { v=_value ; return v ; }




//*********************************************************************************************************
//*********************************************  Set methods  *********************************************
//*********************************************************************************************************


template<> 
inline void dbColumn::ValueInst<bool>::setValue(bool const & v)
{ _value = v ; }

// This is required because MySQL uses short int as boolean type
template<> 
inline void dbColumn::ValueInst<bool>::setValue(short int const & v)
{ _value = v ; } // PRQA S 3054


template<> 
inline void dbColumn::ValueInst<DateTime>::setValue(DateTime const & v)
{ _value = v ; }


//*************************************  conversion  from string *********************************************
template<> 
inline void dbColumn::ValueInst<std::string>::setValue(std::string const & v)
{ _value = v ; }

template<> 
inline void dbColumn::ValueInst<std::string>::setValue(dbGeoObject const & v)
{ _value = v.getSqlString(SQLDialectFactory::dialectName(getKey())) ; }

template<> 
inline void dbColumn::ValueInst<bool>::setValue(std::string const & v)
{
	if(StringUtils::equalsNoCase( v , SQLDialectFactory::sqlDialect(getKey()).boolTrue() ) ) {
		_value = true ;
    }
	else if ( StringUtils::equalsNoCase( v , SQLDialectFactory::sqlDialect(getKey()).boolFalse() ) ) {
		_value = false ;
    }
	else {
		ACS_THROW( exIllegalValueException("Cannot convert to boolean string \"" + v + "\"") );  // PRQA S 3081		
    }
}


template<> 
inline  void dbColumn::ValueInst<DateTime>::setValue(std::string const & v)
{
	//DateTime tries several formats
	_value = v ;
}

//This generic implementation should handle all numerical types
template <typename T> 
inline
void dbColumn::ValueInst<T>::setValue(std::string const & v)
{
	T tmp ;
	std::istringstream is(v) ;
	is >> tmp ;
	setValue(tmp) ;
}


//*********************************************** Numerical types  ****************************************
// NOTE: all the following numerical conversions assume long int == int

//**************************************** signed char ********************************************************
template<> 
inline  void dbColumn::ValueInst<signed char>::setValue(signed char const & v) 
{ _value = v ; }

//**************************************** signed short int ********************************************************
template<> 
inline  void dbColumn::ValueInst<signed short int>::setValue(signed char const & v)
{ _value = v ; }

template<> 
inline  void dbColumn::ValueInst<signed short int>::setValue(signed short int const & v)
{ _value = v ; }

template<> 
inline  void dbColumn::ValueInst<signed short int>::setValue(unsigned char const & v)
{ _value = v ; }  // PRQA S 3000


//**************************************** signed int ********************************************************
template<> 
inline  void dbColumn::ValueInst<signed int>::setValue(signed char const & v)
{ _value = v ; }

template<> 
inline  void dbColumn::ValueInst<signed int>::setValue(signed short int  const & v)
{ _value = v ; }

template<> 
inline  void dbColumn::ValueInst<signed int>::setValue(signed int  const & v)
{ _value = v ; }

template<> 
inline  void dbColumn::ValueInst<signed int>::setValue(signed long int  const & v)
{ _value = v ; } // PRQA S 3010


 // PRQA S 3000 EOF
template<> inline  void dbColumn::ValueInst<signed int>::setValue(unsigned char  const & v)             { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed int>::setValue(unsigned short int  const & v)        { _value = v ; }


//**************************************** signed long int ********************************************************
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(signed char const & v)           { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(signed short int  const & v)     { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(signed int  const & v)           { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(signed long int  const & v)      { _value = v ; }


template<> inline  void dbColumn::ValueInst<signed long int>::setValue(unsigned char  const & v)        { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(unsigned short int  const & v)   { _value = v ; }

#ifdef PermissiveDbColumnConversions 

// PRQA S 3010 EOF
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(unsigned long int  const & v)    { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed int>::setValue(unsigned long int  const & v)         { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long int>::setValue(unsigned int  const & v)         { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed int>::setValue(unsigned int  const & v)              { _value = v ; }

template<> inline  void dbColumn::ValueInst<signed short int>::setValue(signed long int   const & v)    { _value = v ; }
template<> inline  void dbColumn::ValueInst<signed int>::setValue(signed long long int   const & v)    { _value = v ; }

template<> inline  void dbColumn::ValueInst<signed short int>::setValue(signed int   const & v)         { _value = v ; }


#endif

//**************************************** signed long long int ********************************************************
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(signed char const & v)					{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(signed short int  const & v)			{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(signed int  const & v)					{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(signed long int  const & v)				{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(signed long long int  const & v)		{ _value = v ; }


template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(unsigned char  const & v)				{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(unsigned short int  const & v)			{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(unsigned int  const & v)				{ _value = v ; }
template<> inline  void dbColumn::ValueInst<signed long long int>::setValue(unsigned long int  const & v)			{ _value = v ; }



//**************************************** unsigned long long int ********************************************************
template<> inline  void dbColumn::ValueInst<unsigned long long int>::setValue(unsigned char  const & v)			  	{ _value = v ; }
template<> inline  void dbColumn::ValueInst<unsigned long long int>::setValue(unsigned short int  const & v)		{ _value = v ; }
template<> inline  void dbColumn::ValueInst<unsigned long long int>::setValue(unsigned int  const & v) 				{ _value = v ; }
template<> inline  void dbColumn::ValueInst<unsigned long long int>::setValue(unsigned long int  const & v)		  	{ _value = v ; }
template<> inline  void dbColumn::ValueInst<unsigned long long int>::setValue(unsigned long long int  const & v)	{ _value = v ; }


//**************************************** float ********************************************************
template<> inline void dbColumn::ValueInst<float>::setValue(float const & v) 										{ _value = v ; }


//**************************************** double ********************************************************
template<> inline void dbColumn::ValueInst<double>::setValue(float const & v) 										{ _value = v ; }
template<> inline void dbColumn::ValueInst<double>::setValue(double const & v) 										{ _value = v ; }

//**************************************** long double ********************************************************
template<> inline void dbColumn::ValueInst<long double>::setValue(float const & v) 									{ _value = v ; }
template<> inline void dbColumn::ValueInst<long double>::setValue(double const & v) 								{ _value = v ; }
template<> inline void dbColumn::ValueInst<long double>::setValue(long double const & v) 							{ _value = v ; }


_ACS_END_NAMESPACE

#endif // _dbColumn_H_
