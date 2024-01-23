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
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/19 12:24:34  marpas
	Transaction class added
	
	Revision 5.2  2013/06/19 18:12:07  marpas
	removing compilation warning over deprecated include files
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:57  marpas
	adopting libException 5.x standards
	
	Revision 4.11  2013/05/14 18:08:28  marpas
	interface rationalization
	improved dbGeoObject coherency support
	implementing PostGRES/PostGIS support
	
	Revision 4.10  2013/03/29 19:47:21  marpas
	getting rid of Html related functions and libraries
	
	Revision 4.9  2013/03/26 14:17:39  marpas
	regression fixed
	debug improved
	
	Revision 4.8  2013/03/26 00:58:19  marpas
	dbRow columns are accessed throu a uint16_t
	other interfaces modified accordingly
	
	Revision 4.7  2013/01/24 19:27:57  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.6  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.5  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:12  marpas
	new statistics adopted
	
	Revision 3.1  2010/09/11 18:09:40  marpas
	libDbExpress 3.2 interface adopted
	
	Revision 3.0  2009/10/05 14:47:49  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.14  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.13  2008/07/10 13:20:25  lucio.pulvirenti
	BCDValue added
	
	Revision 2.12  2007/12/03 14:13:15  crivig
	modified in order to handle null BLOB/CLOB in function isNull() of dbColumn
	
	Revision 2.11  2007/01/03 16:06:20  ivafam
	CannotCheckNull exception added.
	
	Revision 2.10  2006/12/15 10:50:21  paoscu
	dbColumn::GeoValue::getSqlString() fixed.
	
	Revision 2.9  2006/12/15 10:33:03  paoscu
	IsNull and operator== fixed for geo objects
	
	Revision 2.8  2006/12/13 18:12:35  paoscu
	isNull() throws exception for non storable types.
	write methods return always the type name for not storable types (even if NULL)
	
	Revision 2.7  2006/12/07 15:22:48  paoscu
	getSql fixed for dbGeoObject
	
	Revision 2.6  2006/10/11 10:58:36  paoscu
	Method getGenericColumnType added.
	
	Revision 2.5  2006/09/28 17:23:11  serfol
	bug fixed in the column dump
	
	Revision 2.4  2006/07/14 12:19:25  paoscu
	Constructors and load funtions are now template funtions
	Set methods for dbGeoObject added
	Several other implementation details changed
	
	Revision 2.2  2006/03/17 01:37:18  paoscu
	Definitions included in namespace to help doxygen.
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.24  2005/12/19 09:41:38  enrcar
	debug improved
	
	Revision 1.23  2005/12/12 18:27:00  paoscu
	Added a warning for missing method "std::string& dbColumn::ValueInst<bool>::getValue"
	
	Revision 1.22  2005/11/17 18:54:20  paoscu
	Printout methods modified: if the type is a not printable one and the column is NULL
	the output string is the default for NULL column instead of the type name.
	
	Revision 1.21  2005/11/04 19:27:51  paoscu
	It is not anymore possible to init() with type=GEOOBJECT and isNull=false
	reset() method now maintains type information.
	
	Revision 1.20  2005/10/26 13:14:48  paoscu
	Every type of column can be converted into string.
	
	Revision 1.19  2005/10/25 15:39:55  paoscu
	Geo objects can now be stored as strings.
	operator== implemented for  geo objects (using sql strings comparison)
	
	Revision 1.18  2005/10/24 16:23:52  marpas
	GEOOBJECT value to/from string supported
	
	Revision 1.17  2005/10/21 17:03:47  paoscu
	Geo objects and Clob / Blob support added.
	
	Revision 1.16  2005/10/18 18:53:41  paoscu
	Stream insertion operators added.
	load methods fixed.
	
	Revision 1.15  2005/10/17 17:26:42  paoscu
	Excepion ErrorInNumericConversion name fixed
	New constructor that get a ColumnType
	Init and reset methods added.
	Template access method enhanced (traits used).
	
	Revision 1.14  2005/10/13 18:35:58  paoscu
	Work in progress on general access methods.
	
	Revision 1.13  2005/10/10 17:13:12  paoscu
	Work in progress.
	
	Revision 1.12  2005/10/06 21:17:46  paoscu
	Work in progress on template engine.
	
	Revision 1.11  2005/10/06 17:09:43  paoscu
	Work in progress for a new version.
	
	Revision 1.1  2005/09/13 18:03:48  paoscu
	Temporarly added to CVS.
	

	

*/


#include <dbColumn.h>

#include <DateTime.h>

#include <Filterables.h>
#include <sstream>

using namespace std ;




//////////////////////
//              	//
//  class dbColumn	//
//              	//
//////////////////////

namespace acs {

	std::string const dbColumn::_nullString("<null>") ;

	std::string const &dbColumn::nullString(){
		return _nullString ;
	}





//****************************************  dbColumn ************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbColumn)

//default constructor


#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY

dbColumn::dbColumn(dbSpecificColumnType::Type t, bool isNull) :
    _type(t) ,
	_key("") ,
	_value(0)
{

	init(t,isNull) ;	
}

/*
dbColumn::dbColumn(const dbGeoObject& value) :
    _type(),
	_key("") ,
	_value(0)
{
	load(value) ;

}
*/

dbColumn::dbColumn(void* value, bool isNull) :
	_type(dbSpecificColumnType::NOTSTORED),
	_key("") ,
	_value(0)
{
    //       value is ignored : NULL pointer...
}

#endif


dbColumn::dbColumn(const std::string &key) :
    _type(dbSpecificColumnType::UNDEFINED) ,
	_key(key),
	_value(0)
{
}


dbColumn::dbColumn(const std::string &key, dbSpecificColumnType::Type t, bool isNull) :
    _type(t) ,
    _key(key) ,
	_value(0)
{
	init(t,isNull) ;	
}

/*
dbColumn::dbColumn(const string &key, const dbGeoObject& value,  bool isNull) :
    _type() ,
	_key(key),
	_value(0)
{
	load(value, isNull) ;

}
*/

dbColumn::dbColumn(const std::string &key, void* value, bool isNull) :
	_type(dbSpecificColumnType::NOTSTORED),
	_key(key),
	_value(0)
{
    //       value is ignored : NULL pointer...
}

dbColumn::~dbColumn() throw() 
{
	reset() ;
}


void dbColumn::init(dbSpecificColumnType::Type t, bool isNull)
{
	reset() ;
	_type = t ; // assign type
	_value = 0 ; // null
	if(not isNull) { // not null
		switch (_type) 
		{
			case dbSpecificColumnType::BLOB:
			case dbSpecificColumnType::CLOB:
			case dbSpecificColumnType::NOTSTORED:
			case dbSpecificColumnType::MULTISET:
			case dbSpecificColumnType::UNDEFINED:
            {
				_value=new DummyValue(_key) ;
				break ;
            }
			case dbSpecificColumnType::GEOOBJECT:
            {
				_value=new GeoValue(_key) ;
				break ;
            }
			default:
            {
				ACS_THROW(InvalidType(string("It is not possible to init a not null column if type is " ) + dbColumnTypeHelper::getTypeName(_type)  )) ; // PRQA S 3081
				break ;
            }
		}	
    }
}


void dbColumn::reset() throw() 
{
	try { // no exception out of this method
		delete _value ;
		_value=0;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
}



dbColumn::dbColumn(const dbColumn &r) :
    _type(r._type),
    _key(r._key),
    _value(r._value ? r._value -> clone() : 0) // PRQA S 3380
{

    // empty 
}


dbColumn& dbColumn::operator=(const dbColumn &r)
{
    if (this != &r)
    {
        _type = r._type ;
        _key = r._key ;
 		delete _value ;
        _value = r._value ? r._value -> clone() : 0 ; // PRQA S 3380
    }

    return *this;
}



//************************   Load methods  ************************
/*
void dbColumn::load(const dbGeoObject & value, bool isNull)
{
	reset() ;
	_type = dbSpecificColumnType::GEOOBJECT ;
	if (value) {
		_value = new GeoValue(_key, value) ; 
    }
}
*/

//************************   dbGeoObject methods  ************************



dbGeoObject* dbColumn::column(dbGeoObject* &v) // PRQA S 2017, 4211
{
    if ( (_type != dbSpecificColumnType::STRING) && 
         (_type != dbSpecificColumnType::GEOOBJECT )) // wrong type ?
    {
		ostringstream os ; 
		os << "dbColumn::column : Attempt to get a geo object value but type is " << dbColumnTypeHelper::getTypeName(_type) ;
		ACS_THROW(InvalidType(os.str())) ; // PRQA S 3081
	}
    
    ACS_COND_THROW( 0 == _value,NullColumnException("dbColumn::column : Null value") )  ; // PRQA S 3081

    if ( _type  == dbSpecificColumnType::GEOOBJECT ) { // db contain geoobjects ? (informix with  geo-datablade)
		_value -> getValue(v) ;
    }
    if ( _type  == dbSpecificColumnType::STRING ) // db contains strings 
    {
		std::string tmp ;
		_value -> getValue(tmp) ;
		v= dbGeoObject::newDbGeo(tmp) ;
	}
	
	return v ;
}


/*
void dbColumn::setColumn(const dbGeoObject & value )
{
    if (  _type  != dbSpecificColumnType::GEOOBJECT )
    {
		ostringstream os ; 
		os << "dbColumn::setColumn : Attempt to set a geo object value but type is " << dbColumnTypeHelper::getTypeName(_type) ;
		InvalidType e(os.str()) ;
		ACS_THROW(e) ;
	}

	reset() ;
	_type = dbSpecificColumnType::GEOOBJECT ;
	_value = new GeoValue(_key, value) ; //Set to 0 value

}
*/



//************************   other methods  ************************



bool dbColumn::isNull() const 
{
 	switch (_type ) // according to types
	{
	case dbSpecificColumnType::NOTSTORED:
	case dbSpecificColumnType::UNDEFINED:
        {
		    ACS_THROW(CannotCheckNull(string("Cannot call isNull method for type ") +  dbColumnTypeHelper::getTypeName(_type ) )); // PRQA S 3081
		    break ;
        }

	default :
        {
  		    return (_value == 0) ;
        }
	}
}


void dbColumn::getColumnType (dbSpecificColumnType::Type& colType) const
{
    colType = _type ;

    return ;
}



dbSpecificColumnType::Type dbColumn::getColumnType () const // PRQA S 4120
{
    return _type ;
}



dbGenericColumnType::Type  dbColumn::getGenericColumnType () const
{
	return dbColumnTypeHelper::convertTypeToGeneric(_type) ;
}


bool dbColumn::operator==(const dbColumn & other) const // PRQA S 4020
{
// W8115_OFF: multiple returns
	if(_type != other._type) { // types are different
		return false ;
    }

 	switch (_type )
	{
	// assume equality for some types (cannot compare content)
	case dbSpecificColumnType::BLOB:
	case dbSpecificColumnType::CLOB:
	case dbSpecificColumnType::NOTSTORED:
	case dbSpecificColumnType::UNDEFINED:
        {
			return true ;
        }
	default:
        {
		    break ;
        }
	}

	if(isNull()) // this one is null
	{
		return other.isNull() ; // return if the other one is null too
	}
	else // not null
	{
		return other.isNull() ? false : *_value ==  *(other._value) ; // compare // PRQA S 3382
	}
}


bool dbColumn::operator!=(const dbColumn & other) const
{
	return !(*this == other ) ;
}



std::string dbColumn::getSqlString() const // PRQA S 4020
{
// W8115_OFF: multiple returns
    if ( !_value )
    {
		return " NULL " ;
	}
    
    return _value->getSqlString() ;
}



void dbColumn::dumpColumn() const // PRQA S 4214
{
	if(!_value) {
    	ACS_LOG_DEBUG("Type is: "<< dbColumnTypeHelper::getTypeName (_type) << " column is NULL") ;
    }
	else
	{
        ostringstream os ;
        _value->insertIntoStream(os) ;
    	ACS_LOG_DEBUG("Type is: "<< dbColumnTypeHelper::getTypeName (_type) << " value is " << os.str()) ;
	}  
}


std::ostream &operator<<(std::ostream &str, const dbColumn &col)
{
	switch (col.getColumnType() )
	{
	    case dbSpecificColumnType::BLOB:
	    case dbSpecificColumnType::CLOB:
	    case dbSpecificColumnType::NOTSTORED:
	    case dbSpecificColumnType::UNDEFINED:
        {
		    // write just the type
		    str<< dbColumnTypeHelper::getTypeName(col.getColumnType() ) ;
		    break ;
        }
	    case dbSpecificColumnType::GEOOBJECT:
        {
            if(col.isNull()) {
	   		    str << dbColumn::nullString() ;  // null 
            }
		    else {
			    col._value->insertIntoStream(str) ; // the value
            }
            break ;
        }
	    default :
        {
		    if(col.isNull()) {
	   		    str << dbColumn::nullString() ;  // null 
            }
		    else {
			    col._value->insertIntoStream(str) ; // the value
            }
	    }
    }


	return str ;  // PRQA S 4028
}


acs::exostream &operator<<(acs::exostream &str, const dbColumn &col) // PRQA S 4222
{
	switch (col.getColumnType() )
	{
	    case dbSpecificColumnType::BLOB:
	    case dbSpecificColumnType::CLOB:
	    case dbSpecificColumnType::NOTSTORED:
	    case dbSpecificColumnType::UNDEFINED:
        {
		    // write just the type
		    str<< dbColumnTypeHelper::getTypeName(col.getColumnType() ) ;
		    break ;
        }
	    case dbSpecificColumnType::GEOOBJECT:
        {
            if(col.isNull()) {
	   		    str << dbColumn::nullString() ;  // null 
            }
		    else {
			    col._value->insertIntoStream(str) ; // the value
            }
            break ;
        }
	    default :
        {
		    if(col.isNull()) {
	   		    str << dbColumn::nullString() ;  // null 
            }
            else {
			    col._value->insertIntoStream(str) ; // the value
            }
        }
	}


	return str ;  // PRQA S 4028
}


void dbColumn::checkNull() const
{
	if(!_value){
		NullColumnException e("Empty Column.") ;
		ACS_THROW(e) ;
	}
}


dbColumn::Value * dbColumn::createValue(const std::string &key, dbSpecificColumnType::Type type) // PRQA S 5500
{
// NB cyclomatic complexity is just slightly high, but is due to the switch control structure below
// any attemp to lower CC would increase complexity for maintenance
	dbColumn::Value * res(0) ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB,"key: \"" << key << "\" type: " << dbColumnTypeHelper::getTypeName(type)) ;
	//WARNING !!!! This switch must always cover all the possible cases!!!!
	switch (type)
	{
	case dbSpecificColumnType::BLOB:
	case dbSpecificColumnType::CLOB:
	case dbSpecificColumnType::NOTSTORED:
	case dbSpecificColumnType::MULTISET:
	case dbSpecificColumnType::UNDEFINED:
        {
		    res = new DummyValue(key) ; // dummy value

		    break ;
        }
	// creates right value with type
	case dbSpecificColumnType::SIGNED_CHAR:				{ res = new ValueInst<signed char>(key, 0) ; break ; }
	case dbSpecificColumnType::SIGNED_SHORT_INT:		{ res = new ValueInst<signed short int>(key, 0) ; break ; } 
	case dbSpecificColumnType::SIGNED_INT:				{ res = new ValueInst<signed int>(key, 0) ; break ; }
	case dbSpecificColumnType::SIGNED_LONG_INT:			{ res = new ValueInst<signed long int>(key, 0) ; break ; }
	case dbSpecificColumnType::SIGNED_LONG_LONG_INT:	{ res = new ValueInst<signed long long int>(key, 0) ; break ; }

	// Integer unsigned types
	case dbSpecificColumnType::UNSIGNED_CHAR:			{ res = new ValueInst<unsigned char>(key, 0) ; break ; }
	case dbSpecificColumnType::UNSIGNED_SHORT_INT:		{ res = new ValueInst<unsigned short int>(key, 0) ; break ; }
	case dbSpecificColumnType::UNSIGNED_INT:			{ res = new ValueInst<unsigned int>(key, 0) ; break ; }
	case dbSpecificColumnType::UNSIGNED_LONG_INT:		{ res = new ValueInst<unsigned long int>(key, 0) ; break ; }
	case dbSpecificColumnType::UNSIGNED_LONG_LONG_INT:	{ res = new ValueInst<unsigned long long int>(key, 0) ; break ; }

	//Floating point types		
	case dbSpecificColumnType::FLOAT:					{ res = new ValueInst<float>(key, 0) ; break ; }
	case dbSpecificColumnType::DOUBLE:					{ res = new ValueInst<double>(key, 0) ; break ; }
	case dbSpecificColumnType::LONG_DOUBLE:				{ res = new ValueInst<long double>(key, 0) ; break ; }

	//BCDValue
	case dbSpecificColumnType::BCD:					    { res = new BCDValue(key, "") ; break ; }


	//Others
	case dbSpecificColumnType::BOOLEAN:					{ res = new ValueInst<bool>(key, false) ; break ; }
	case dbSpecificColumnType::STRING:					{ res = new ValueInst<std::string>(key, "") ; break ; }
	case dbSpecificColumnType::DATETIME:				{ res = new ValueInst<DateTime>(key, DateTime()) ; break ; }
	case dbSpecificColumnType::GEOOBJECT:				{ res = new GeoValue(key) ; break ; }

	//Default is just for security, but it should never been actually used!!!
	default:
        {
            ostringstream os ;
            os << "Unknown dbSpecificColumnType: <" << int(type) << ">" ; // PRQA S 3081 2 
		    ACS_THROW(InvalidType(os.str())) ; 
        }
	}
	return res ;
}       


//****************************************  dbColumn::Value ************************************************************

// PAOLO exception could include ouput type name
// integer signed types
signed char& dbColumn::Value::getValue(signed char& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

signed short int& dbColumn::Value::getValue(signed short int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

signed int& dbColumn::Value::getValue(signed int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

signed long int& dbColumn::Value::getValue(signed long int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

signed long long int& dbColumn::Value::getValue(signed long long int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081


// integer unsigned types
unsigned char& dbColumn::Value::getValue(unsigned char& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081
 
unsigned short int& dbColumn::Value::getValue(unsigned short int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081

unsigned int& dbColumn::Value::getValue(unsigned int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081

unsigned long int& dbColumn::Value::getValue(unsigned long int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

unsigned long long int& dbColumn::Value::getValue(unsigned long long int& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081


//floating point types		
float& dbColumn::Value::getValue(float& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081

double& dbColumn::Value::getValue(double& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 

long double& dbColumn::Value::getValue(long double& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081 



bool& dbColumn::Value::getValue(bool& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081

DateTime& dbColumn::Value::getValue(DateTime& v ) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081

dbGeoObject* dbColumn::Value::getValue(dbGeoObject* &v) const 
{ ACS_THROW(InvalidType("Impossible to get this type")) ;} // PRQA S 3081
 



// integer signed types
void dbColumn::Value::setValue(signed char const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(signed short int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(signed int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(signed long int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(signed long long int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}


// integer unsigned types
void dbColumn::Value::setValue(unsigned char const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(unsigned short int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(unsigned int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(unsigned long int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(unsigned long long int const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

//floating point types		
void dbColumn::Value::setValue(float const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(double const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(long double const & v )
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}


void dbColumn::Value::setValue(std::string const & v)
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(bool const & v)
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(DateTime const & v)
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}

void dbColumn::Value::setValue(dbGeoObject const & v)
{ InvalidType e("Impossible to set this type") ; ACS_THROW(e) ;}







//****************************************  dbColumn::GeoValue ************************************************************



//************************   dbColumn::BCDValue  ************************

dbColumn::BCDValue::BCDValue(std::string const &key, std::string const &s) : Value(key), _value(s) {} 

dbColumn::BCDValue::~BCDValue() throw() {}


dbColumn::Value *dbColumn::BCDValue::clone() const
{
	return new dbColumn::BCDValue(*this) ;
}

			
dbColumn::BCDValue::BCDValue(const BCDValue &r) :
	Value(r),
    _value(r._value) 
{
	// empty
}

dbColumn::BCDValue &dbColumn::BCDValue::operator=(const BCDValue &other)
{
	if ( this != &other ) {
		Value::operator= (other) ; 
		_value = other._value ;
	}
	return *this ;
}

bool dbColumn::BCDValue::operator==(Value const &other) const
{
	return getSqlString() == other.getSqlString() ;
}

bool dbColumn::BCDValue::operator!=(Value const &other) const
{
	return !(*this == other) ;
}


string dbColumn::BCDValue::getSqlString() const
{
	return _value ;
}



exostream & dbColumn::BCDValue::insertIntoStream(acs::exostream &os) const
{
	os <<_value ;
	return os ; // PRQA S 4028
}

ostream &   dbColumn::BCDValue::insertIntoStream(std::ostream &os) const
{
	os<<_value ;
	return os; // PRQA S 4028
}



			
string& dbColumn::BCDValue::getValue(std::string& v) const
{
	v=_value ;
	return v ; // PRQA S 4028
}


void checkNumeric(const string &s) 
{
	
	size_t len = s.length() ;
	ACS_COND_THROW( (len == 0), // empty string, // PRQA S 3081
							   dbColumn::ErrorInNumericConversion( "Empty string" ) ) ;
		
	const char *startConv = s.c_str() ;
	const char *endConv = s.c_str()+len ; // PRQA S 3084
	if ((*startConv == '\'') || (*startConv == '\"')) { // can start with double or single quotes (and end with the same char
		if (((startConv[len-1] == '\'') || (startConv[len-1] == '\"')) && (*startConv == startConv[len-1])) { // PRQA S 3084
			startConv++ ;
			endConv-- ;
		}
		else {
			ostringstream errorMessage;
			errorMessage << "isNumeric : wrong char \""<<s[0]<<"\" in pos 0"  ; 
			ACS_THROW( dbColumn::ErrorInNumericConversion( errorMessage.str() ) ) ; // PRQA S 3081
		}
	}
	char *realEnd = 0 ;
	
	strtod(startConv,&realEnd) ; // convert to double 
	
	if (realEnd != endConv) { // check if consumed all the available chars
		ostringstream errorMessage;
		errorMessage << "isNumeric : wrong char \""<<*realEnd<<"\" at pos "<<realEnd-s.c_str() ; 
		ACS_THROW( dbColumn::ErrorInNumericConversion( errorMessage.str() ) ) ; // PRQA S 3081
	}
}





void dbColumn::BCDValue::setValue(std::string const & v)
{
	checkNumeric(v) ;
	_value = v ;
}


// integer signed types
// PRQA S 4028 L1
signed char& 			dbColumn::BCDValue::getValue(signed char& 			v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }

signed short int& 		dbColumn::BCDValue::getValue(signed short int& 		v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
signed int& 			dbColumn::BCDValue::getValue(signed int&  			v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
signed long int&		dbColumn::BCDValue::getValue(signed long int& 		v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
signed long long int&	dbColumn::BCDValue::getValue(signed long long int& 	v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }

// integer unsigned types
unsigned char& 			dbColumn::BCDValue::getValue(unsigned char& 		    v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
unsigned short int&		dbColumn::BCDValue::getValue(unsigned short int& 	v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
unsigned int& 			dbColumn::BCDValue::getValue(unsigned int& 			v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
unsigned long int&  	dbColumn::BCDValue::getValue(unsigned long int& 	    v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
unsigned long long int& dbColumn::BCDValue::getValue(unsigned long long int& v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }

// floating point types		
float& 					dbColumn::BCDValue::getValue(float& 				    v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
double&					dbColumn::BCDValue::getValue(double&				    v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
long double&			dbColumn::BCDValue::getValue(long double&			v ) const { istringstream is(_value.c_str()) ; is >> v; return v ; }
// PRQA L:L1

} //namespace
