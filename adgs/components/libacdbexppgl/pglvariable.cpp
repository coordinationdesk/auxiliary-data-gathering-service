// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for Postgres $

	$Id$

	$Author$
	
	

*/


#include "pglvariable.hpp"

#include "pglerror.hpp"
#include "pglconversion.hpp"

#include <cmath>
#include <ctime>
#include <iostream>
#include <sstream>

using namespace std ;

/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractStringValue

CPGLAbstractStringValue::CPGLAbstractStringValue (INT32 length) : 
    _length(length),
    _value(0)
{
//	_length = length;
	_value = new char [length];
}

CPGLAbstractStringValue::CPGLAbstractStringValue (INT32 length, char *value) :
    _length(length),
    _value(0)
{
//	_length = length;
	_value = new char [length];
	memcpy(_value, value, length);
}

CPGLAbstractStringValue::~CPGLAbstractStringValue ()
{
	delete [] _value;
}

INT32 CPGLAbstractStringValue::getLength ()
{
	return _length;
}

char * CPGLAbstractStringValue::getData ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractStringValue::getData ptr: %p\n", _value );
#endif

	return _value;
}


void CPGLAbstractStringValue::setData(const char *s) 
{
	delete[] _value ;	
	_value = 0 ;
	_length = 0 ;
    
	if (s) {
		_length = strlen(s) ;
		_value = new char [_length+1];
		strncpy( _value, s, (_length+1) ) ;
	}
    
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractStringValue::setData %s\n",_value ? _value : "NULL PTR") ;
#endif
}

void CPGLAbstractStringValue::asString (char *value)
{
	if (value) 
    { 
        *value = 0 ;
        if (_length>=0) memset(value, 0, _length+1);	/* To be sure that resulting string will be a C-standard zero-terminated string */
        if (_value && (_length>0)) memcpy(value, _value, _length);
	}
}

CPGLAbstractStringValue::operator char * ()
{
	return _value;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractJsonValue

CPGLAbstractJsonValue::CPGLAbstractJsonValue (INT32 length, const bool is_binary) : 
    _length(length),
    _value(0),
	_is_binary(is_binary)
{
//	_length = length;
	_value = new char [length];
}

CPGLAbstractJsonValue::CPGLAbstractJsonValue (INT32 length, char *value, const bool is_binary) :
    _length(length),
    _value(0),
	_is_binary(is_binary)
{
//	_length = length;
	_value = new char [length];
	memcpy(_value, value, length);
}

CPGLAbstractJsonValue::~CPGLAbstractJsonValue ()
{
	delete [] _value;
}

INT32 CPGLAbstractJsonValue::getLength ()
{
	return _length;
}

char * CPGLAbstractJsonValue::getData ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractJsonValue::getData ptr: %p\n", _value );
#endif

	return _value;
}


void CPGLAbstractJsonValue::setData(const char *s) 
{
	delete[] _value ;	
	_value = 0 ;
	_length = 0 ;
	bool keep_first_byte(true) ; 
    
	if (s) 
	{
		_length = strlen(s) ;
		
		/*
			EC:: Reverse engineered: 
			The JSOBN buffer starts with byte 0x01 (^A, "soh: START OF HEADING"). It might be a marker between JSON and JSONB, for example.
			(Standard JSON buffers _do not_ seem affected by this issue).
			
			So, the shortest JSONB should be: 01 7B 7D (<soh>{}) which should be converted by this call to: 7B 7D 00 (NULL-terminated "{}").
			In order to prevent the stripping of the first byte if IT IS NOT a <soh>, a check is performed.
			
			The first byte SHOULD BE KEPT if:
			- Buffer is not binary (JSON and not JSONB)
			- or Buffer is empty
			- or First byte is _NOT_ <soh>
		*/
		
		keep_first_byte = ( (false == _is_binary) || (_length < 1) || (s[0] != 0x01/*<soh>*/)) ;
				
		if ( true == keep_first_byte )
		{
			// Standard JSON :
			_value = new char [_length+1];			// To take into account the NULL-termination
			strncpy( _value, s, (_length+1) ) ;		// Taking into account that s is NULL-terminated, too.
		}
		else
		{
			// JSONB (Binary JSON) :
			if (_length >= 1) { _length -- ; }		// The string is shorter since the first byte won't be copied.
					
			_value = new char [_length+1];			// To take into account the NULL-termination
			strncpy( _value, (s+1), (_length+1) ) ;	// Taking into account that s is NULL-terminated, too.
		}
	}	// END: if (s) 
    
#ifdef ACDBEXPPGL_DEBUG
	ostringstream o ; 
	o << "CPGLAbstractJsonValue::setData  is_binary? " << (_is_binary?"T":"F") << "  keep_first_byte? " << (keep_first_byte? "T":"F") \
														<< "  |" << ((NULL != _value) ? string(_value) : "NULL PTR") << "|  length: " << _length ;
	printf("%s\n", o.str().c_str()) ;
#endif
}

void CPGLAbstractJsonValue::asString (char *value)
{
	if (value) 
    { 
        *value = 0 ;
        if (_length>=0) memset(value, 0, _length+1);	/* To be sure that resulting string will be a C-standard zero-terminated string */
        if (_value && (_length>0)) memcpy(value, _value, _length);
	}
}

CPGLAbstractJsonValue::operator char * ()
{
	return _value;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractDecimalValue

CPGLAbstractDecimalValue::CPGLAbstractDecimalValue (INT32 precision, INT32 scale, INT32 len, IFetchSubject *fetchSubject) : 
	_precision(precision),
    _scale(scale),
    _length(len),
    _value(0),
 	_hasDouble(false),
	_double(),
	_hasBcd(false),
    _bcd()
{
//	_length = len;
//	_precision = precision;
//	_scale = scale;
//	_hasDouble = false;
//	_hasBcd = false;
	_value = new char[_length+1] ; 
	memset(_value,0,_length+1) ;
	fetchSubject->attach(this);
}

CPGLAbstractDecimalValue::CPGLAbstractDecimalValue (pFMTBcd value) :
	_precision(),
	_scale(),
	_length(),
	_value(),
	_hasDouble(false),
	_double(),
	_hasBcd(false),
	_bcd()
{
// EC:: Untested. 
	ACSNOTIMPLEMENTED("CPGLAbstractDecimalValue::CPGLAbstractDecimalValue: not implemented") ;
#ifdef MARCO

	_precision = value->iPrecision;
	_scale = value->iSignSpecialPlaces & 0x3f;
    
    // Convert FMTBcd to a temporary decimal using driver built-in function
    decimal dtmp ; bcd_to_decimal(value, &dtmp);
  
    _value = new char[128] ;
    // Convert temporary decimal into char* and store into _value
    decimaltopchar(&dtmp, _value, sizeof(value)) ;
    _length = strlen(_value);
#endif
}

CPGLAbstractDecimalValue::~CPGLAbstractDecimalValue () throw() 
{
	delete [] _value ;
}

INT32 CPGLAbstractDecimalValue::getLength ()
{
	return _length ; // sign and decimal dot
}

char * CPGLAbstractDecimalValue::getData ()
{
	return reinterpret_cast<char *>(_value);
}

double CPGLAbstractDecimalValue::asDouble ()
{
	if (!_hasDouble) {
   	_double = strtod(_value,NULL) ;
		_hasDouble = true;
	}
	return _double;
}

FMTBcd& CPGLAbstractDecimalValue::asFMTBcd ()
{
	ACSNOTIMPLEMENTED("CPGLAbstractDecimalValue::asFMTBcd: not implemented") ;
#ifdef MARCO
	if (!_hasBcd) {
        decimal dtmp ;
        pchartodecimal(_value, &dtmp);  /* convert a char* into decimal */
        decimal_to_bcd(&dtmp, &_bcd, _precision, _scale); /* standard driver-call */
        _hasBcd = true;

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractDecimalValue::asFMTBcd\n") ;
	printf("input number (as string): %s\n", _value) ;
    double d = decimal_to_double(&dtmp) ;
	printf("output number (as double): %f\n", d) ;
#endif

    }
	return _bcd;
#endif
}

void CPGLAbstractDecimalValue::asString (char *value)
{
	memcpy(value, _value,_length+1); // sign and decimal dot + string terminator
}

void CPGLAbstractDecimalValue::update ()
{
	_hasDouble = false;
	_hasBcd = false;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLDateTimeValue

CPGLDateTimeValue::CPGLDateTimeValue () :  _value(), _scale (-1)
{
	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLDateTimeValue::CPGLDateTimeValue. Method called\n") ;
	#endif
}

CPGLDateTimeValue::CPGLDateTimeValue (INT32 scale) : _value(), _scale (scale)
{
	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLDateTimeValue::CPGLDateTimeValue. Method called. Scale: %d\n", scale) ;
	#endif
}


CPGLDateTimeValue::CPGLDateTimeValue (pCTIMESTAMP value) :
	_value(),
    _scale()
{
	ACSNOTIMPLEMENTED("CPGLDateTimeValue::CPGLDateTimeValue: not implemented") ;
#ifdef MARCO
	_value = *reinterpret_cast<SQL_TIMESTAMP_STRUCT*>(value) ;
#endif
}

INT32 CPGLDateTimeValue::getType ()
{
	return fldDATETIME ;
}

INT32 CPGLDateTimeValue::getLength ()
{
	return sizeof(_value) ;
}

char * CPGLDateTimeValue::getData ()
{
	return reinterpret_cast<char *>(&_value);
}

void CPGLDateTimeValue::setData (const char *d)
{

//	double dbl ;
	signed long long int intDate, fp ;

    memset(&_value, 0, sizeof(_value) );
    
    if (!d) 
    {
        #ifdef ACDBEXPPGL_DEBUG
        	printf("CPGLDateTimeValue::setData: NULL PTR\n") ;
        #endif

        return ;
    }
   
   
   	if (_scale <= 0)
	{
		// Postgres server stores DateTime as a DOUBLE PRECISION DIGIT (default behaviour for server version <= 8)

		/* EC::
    		REVERSE-ENGINEREED.
    		Binary format for "timestamp(6) without time zone" is: 
    		JD2000 floating point, represented in DOUBLE-64 (IEEE-754, STANDARD) BIGENDIAN.
    		INTEGER PART IS: NUMBER OF SECONDS 
    		(So, two consecutive days differ from: 86400.0)

    		Example: 5.5 seconds after 01-01-2000: [5.50] 0x 40 16 00 00 00 00 00 00

    		Example: 1 day after 01-01-2000: [86400.0] 0x 40 F5 18 00 00 00 00 00
		*/


		/* Step 1: 
        	From BE FP to LE FP: */
		double dbl ;
		unsigned char* p = reinterpret_cast<unsigned char*>( &dbl ) ;
		for (int i=0; i<8; i++)
		{
#if __BYTE_ORDER == __BIG_ENDIAN
			p[i] = d[i] ;
#else
			p[i] = d[7-i] ;
#endif
    	}
	
		/* Step 1.1:
    		Get a signed long long integer to avoid precision-loss BEFORE applying offset */
    	intDate =  static_cast<signed long long int>( dbl ) ;


  		/* Step 1.2: 
  			Store fractional part */
        // const double usec = (1e6) ;
        const double nsec = (1e9) ;
		fp = static_cast<signed long long int>( round( ( dbl - static_cast<double> (intDate) ) * nsec )) ;    // Unit is nsec (10e-9 [s]) 

	}
	else
	{
		// Postgres server stores DateTime as a INT8 (default behaviour for server version >= 9)
	
		/* EC::
    		REVERSE-ENGINEREED.
    		Binary format for "timestamp(6) without time zone" is: 
    		JD2000 floating point number, multiplied by 10^scale and made integer, represented as SIGNED INT8 BIGENDIAN. So far, scale is always "6" [usec]
    		(So, two consecutive days differ from: int(86400.0*1e6) = 864e8 )

    		Example: 5.5 seconds after 01-01-2000: int( (0 + 5.5/86400.0) * 1.0e6 ) = int(63.65) = [ 63 ]  0x 00 00 .. 3F

    		Example: 1 day after 01-01-2000: int ( (1.0*86400.0) * 1.0e6 ) = int(864.0e8) ) [864E8]  0x 00 00 00 14 1D D7 60 00
		*/

		/* Step 1: 
        	From BE INT8 to LE INT8: */
		signed long long int i8 ;
		unsigned char* p = reinterpret_cast<unsigned char*>( &i8 ) ;
		for (int i=0; i<8; i++)
		{
#if __BYTE_ORDER == __BIG_ENDIAN
			p[i] = d[i] ;
#else
			p[i] = d[7-i] ;
#endif
    	}

		const signed long long int p10 = static_cast<signed long long int>(exp10(_scale)) ;

		/* Step 1.1:
    		Get a signed long long integer to avoid precision-loss BEFORE applying offset */
        intDate = i8 / p10 ; 
		
 		/* Step 1.2: 
  			Store fractional part */
		fp = i8 - (intDate * p10) ; 
        
  		/* Step 1.3: 
  			Rescale fractional part */
		const signed long long int p_rescale = (_scale <= 9)? (static_cast<signed long long int>(exp10(9 - _scale))) : 1 ;
        fp *= p_rescale ;   // Rescaled. Now Unit is always nsec (10e-9 [s]) 

	}	/* if (!_storedAsInteger) */
    	
		
    /*
        WHEN HERE:
          intDate is the (signed integ.) number of seconds, as JD2000_seconds
          fp is the (pos. integ.) number of nsec, representing the time of the day 
    */
        
	/* Step 2.1:
	     Check if the fractional part is negative
	*/	
	if (fp<0)
	{
		/*
			(fp<0) means that the fraction part is negative.
			E.G.
			12345.-22222
			Where 12345 might be + or -, irrelevant.
			
			Of course a negative fractional part is a nonsense !  It actually means:
			-1 (NEGATIVE integer part)
			+0.77778 (POSITIVE fractional part)
			This is true, since ( -1+0.77778 ) = -0.22222 .
			So .-22222 becames (-1 + (+1.0-0.22222)) = -1 + 0.77778
			   .-55555 becames (-1 + (+1.0-0.55555)) = -1 + 0.44445
			And so on.
			
			To summarise, a negative fp is splitted as: integer_part -= 1 . Fractional part += (1.0-abs(fp)) 
		*/
	
	
		intDate-- ; 		// DECREASE INTEGER PART BY 1
		fp += 1000000000 ;	// INCREASE NEG.FRACTIONAL PART BY 1.0
	}	
       
	    
	/* Step 2.2 :
    	Fill _value (a CTIMESTAMP structure) */    
	jd2000secs_to_YMDhms(intDate, _value.year, _value.month, _value.day, _value.hour, _value.minute, _value.second) ;

	/* Step 2.3 :
    	Fill the fractional part (of one second) in CTIMESTAMP structure _value  */  
	 _value.fraction = fp ;                      /* Unit is nsec: 10e-9 [s] */


#ifdef ACDBEXPPGL_DEBUG
	{
    	char tmp[1024];
 	    strcpy(tmp, "") ;
 	    for (int i=0; i<8; i++)
        {
    	    char f[16];
    	    sprintf(f, " %02x", int(reinterpret_cast<const unsigned char*>(d)[i]) );
    	    strcat(tmp, f);
        }

	    printf("CPGLDateTimeValue::setData  ptr %p - 0x[%s]  intDate: %lld  fp: %lld\n", &_value, tmp, intDate, fp) ;
    	printf("\t%04hu-%02hu-%02hu %02hu:%02hu:%02hu.%d\n", 
            _value.year, _value.month, _value.day, _value.hour, _value.minute, _value.second, _value.fraction ) ;

    }

#endif
    
}



CPGLDateTimeValue::operator CTIMESTAMP& ()
{

	ACSNOTIMPLEMENTED("CPGLDateTimeValue::operator CTIMESTAMP&: not implemented") ;
#ifdef MARCO
	return *reinterpret_cast<CTIMESTAMP*>(&_value) ;
#endif
}


void CPGLDateTimeValue::update ()
{
}


/////////////////////////////////////////////////////////////////////////////
// CPGLIntervalValue

CPGLIntervalValue::CPGLIntervalValue (INT16 qualifier, IFetchSubject *fetchSubject) :
    _isConverted(false)
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//	_value.in_qual = qualifier;
//	_isConverted = false;
	fetchSubject->attach(this);
}

INT32 CPGLIntervalValue::getType ()
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::getType body disabled
#endif
//	return SQLINTERVAL;
	ACSNOTIMPLEMENTED("CPGLIntervalValue::getType TBI") ;
}

INT32 CPGLIntervalValue::getLength ()
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//	return _value.in_qual;
	ACSNOTIMPLEMENTED("CPGLIntervalValue::getLength TBI") ;
}

char * CPGLIntervalValue::getData ()
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//	return reinterpret_cast<char *>(&_value);
	ACSNOTIMPLEMENTED("CPGLIntervalValue::getData TBI") ;
}

void CPGLIntervalValue::asString (char *value)
{
	if (!_isConverted) {
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//		interval_to_char(&_value, _buffer);
		_isConverted = true;
	}
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//	memcpy(value, _buffer, sizeof(_buffer));
}

void CPGLIntervalValue::update ()
{
	_isConverted = false;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLDateValue

CPGLDateValue::CPGLDateValue () :_value()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::CPGLDateValue\n") ;
#endif
}

CPGLDateValue::CPGLDateValue (INT32 value) :
	_value(value - DATE_DELTA)

{
//	_value = value - DATE_DELTA;
}

INT32 CPGLDateValue::getType ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::getType\n") ;
#endif
#ifdef PGL_SHOW_WARNING
#warning MP CPGLDateValue::getType body disabled
#endif
//	return fldDATE;
	ACSNOTIMPLEMENTED("CPGLDateValue::getType TBI") ;
}

INT32 CPGLDateValue::getLength ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::getLength\n") ;
#endif
	return sizeof(_value);
}

char * CPGLDateValue::getData ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::getData\n") ;
#endif
	return reinterpret_cast<char *>(&_value);
}

CPGLDateValue::operator INT32 ()
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::operator INT32\n") ;
#endif
	return _value + DATE_DELTA;
}

void CPGLDateValue::setData (const char *d)
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLDateValue::operator INT32\n") ;
#endif
	ACSNOTIMPLEMENTED("CPGLDateValue::getType TBI") ;
}
/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractInt8Value




CPGLAbstractInt8Value::CPGLAbstractInt8Value () : _value(), _bcd() {}
CPGLAbstractInt8Value::CPGLAbstractInt8Value (unsigned long long int value): _value(value), _bcd() {}
INT32 CPGLAbstractInt8Value::getLength () { return sizeof(_value); }
char * CPGLAbstractInt8Value::getData () { return reinterpret_cast<char *>(&_value); }
double CPGLAbstractInt8Value::asDouble () { return _value; }
CPGLAbstractInt8Value::operator signed long long int& () { return _value; }


void CPGLAbstractInt8Value::setData(const char *c)
{ 

    if (!c) 
    {
        #ifdef ACDBEXPPGL_DEBUG
        	printf("CPGLAbstractInt8Value::setData: NULL PTR\n") ;
        #endif

        return ;
    }
   

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractInt8Value::setData sizeof(_value): %d\n",int(sizeof(_value)) ) ;
	printf("CPGLAbstractInt8Value::setData getLength(): %d\n",getLength()) ;
#endif
	size_t s = sizeof(_value) ;	/* Size of Actual C type */
    _value = 0 ;	/* Reset value */

    if ( size_t(getLength()) != s )
		throw CInvalidCastError("CPGLAbstractInt8Value called with invalid type",__FILE__,__LINE__) ;
            
    /* Following value-assignment is endianness-independent: */
    for (size_t i=0; i<sizeof(_value); i++) 
    	_value |= static_cast<unsigned long long>(reinterpret_cast<const unsigned char*>(c)[i]) << (8*(s-i-1)) ;

				
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLAbstractInt8Value::setData %lld\n",_value) ;
#endif
} 


void CPGLAbstractInt8Value::asString (char *value) 
{
	char str[32] ;
    unsigned long long int val = static_cast<unsigned long long int>( _value );
    unsigned long long int ten = 10 ; 
    int sgn = (_value < 0)? -1: +1 ;
    int pos = sizeof(str) - 1 ;
     
    str[pos--] = 0 ;	/* NULL TERMINATED */

    do
    {
    	str[pos--] = '0' + (val % ten) ;
    	val /= ten ;
    }
	while (val) ;

	if (sgn<0) str[pos--] = '-' ;

	strcpy(value, str+pos+1) ;
    
	return ;
}


FMTBcd& CPGLAbstractInt8Value::asFMTBcd ()
{
	int8_to_bcd(&_value, &_bcd);
    
	return _bcd;
}


void CPGLAbstractInt8Value::update ()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractSimpleLOValue
CPGLAbstractSimpleLOValue::CPGLAbstractSimpleLOValue (INT32 size) :
    _value()
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSimpleLOValue::CPGLAbstractSimpleLOValue body disabled 
#endif
/*
	_value.loc_loctype = LOCMEMORY;
	if ((_value.loc_bufsize = size) < 0) {
		_value.loc_buffer = 0;
		_value.loc_mflags = LOC_ALLOC;
	} else {
		_value.loc_buffer = new char [size];
		_value.loc_mflags = 0;
	}
	_value.loc_oflags = 0;
*/
}

CPGLAbstractSimpleLOValue::CPGLAbstractSimpleLOValue (INT32 size, void *value) :
    _value()
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSimpleLOValue::CPGLAbstractSimpleLOValue body disabled 
#endif
/*
	_value.loc_loctype = LOCMEMORY;
	_value.loc_bufsize = size;
	_value.loc_size = size;
	_value.loc_buffer = new char [size];
	_value.loc_mflags = 0;
	_value.loc_oflags = 0;
	memcpy(_value.loc_buffer, value, size);
*/
}

CPGLAbstractSimpleLOValue::~CPGLAbstractSimpleLOValue () 
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSimpleLOValue::CPGLAbstractSimpleLOValue body disabled 
#endif
/*
	if (_value.loc_mflags == LOC_ALLOC)
		freeSimpleLOBuffer(_value.loc_buffer);
	else
		delete [] _value.loc_buffer;
*/
}

void CPGLAbstractSimpleLOValue::setData(const char * s)
{
	/* Input is a buffer with a BE-OID. Example: [00][00][12][34] for OID: 0x1234 */
	
    unsigned int oid ;
	oid ^= oid ;	/* reset value to zero */
    
	
    /* Convert the input buffer as a endianness-indipendent value. So oid is the number 0x1234 */
	if (s)
    {
        for (int i=0; i<4; i++) oid |= ( static_cast<const unsigned char>(s[i])  << 8*(3-i) ) ;
	}
    
	_value = oid ;	/* store internal value */

#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLAbstractSimpleLOValue::setData OID:  %d  [0x%04x]\n", _value, _value) ;
#endif

	return ;
}


INT32 CPGLAbstractSimpleLOValue::getLength ()
{
	return sizeof(_value);
}

char * CPGLAbstractSimpleLOValue::getData ()
{
	return reinterpret_cast<char *>(&_value);
}

INT32 CPGLAbstractSimpleLOValue::getBlobSize ()
{
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLAbstractSimpleLOValue::getBlobSize() Method called\n"); ;
#endif


#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSimpleLOValue::getBlobSize body disabled to be implemented
#endif
	ACSNOTIMPLEMENTED("CPGLAbstractSimpleLOValue::getBlobSize TBI") ;
//	return _value.loc_size;
}

void CPGLAbstractSimpleLOValue::getBlobData (INT32 length, void *value)
{
/*
#warning MP CPGLAbstractSimpleLOValue::getBlobData body disabled to be implemented
	ACSNOTIMPLEMENTED("CPGLAbstractSimpleLOValue::getBlobData TBI") ;
*/
#ifdef ACDBEXPPGL_DEBUG
    printf("CPGLAbstractSimpleLOValue::getBlobData() Method called\n"); ;
#endif


	ssize_t actSize ;	/* Actual size */
    actSize = (size_t(length)<=sizeof(_value)) ? size_t(length) : sizeof(_value) ;
    
    memcpy(value, &_value, actSize) ;	/* Only the OID is copied */
}


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractOpaqueValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSimpleLOValue class not declared
#endif
/*
CPGLAbstractOpaqueValue::CPGLAbstractOpaqueValue ()
{
	_value = 0;
	checkInformixResult(ifx_var_flag(&_value, 1));
	checkInformixResult(ifx_var_init(&_value));
}

CPGLAbstractOpaqueValue::CPGLAbstractOpaqueValue (INT32 size, void *value)
{
	_value = 0;
	checkInformixResult(ifx_var_flag(&_value, 0));
	checkInformixResult(ifx_var_alloc(&_value, size + 1));
	checkInformixResult(ifx_var_setlen(&_value, size));
	checkInformixResult(ifx_var_setdata(&_value, reinterpret_cast<char *>(value), size));
	checkInformixResult(ifx_var_init(&_value));
}

#ifdef WIN32
extern "C" int __cdecl ifx_var_freevar (void **variable);
#endif

CPGLAbstractOpaqueValue::~CPGLAbstractOpaqueValue ()
{
	ifx_var_freevar(&_value);
}

INT32 CPGLAbstractOpaqueValue::getLength ()
{
	return 0;
}

char * CPGLAbstractOpaqueValue::getData ()
{
	return reinterpret_cast<char *>(_value);
}

void CPGLAbstractOpaqueValue::asString (char *value)
{
	getBlobData(getBlobSize(), value);
}

INT32 CPGLAbstractOpaqueValue::getBlobSize ()
{
	return ifx_var_getlen(&_value);
}

void CPGLAbstractOpaqueValue::getBlobData (INT32 length, void *value)
{
	if (length > 1)
		memcpy(value, ifx_var_getdata(&_value), length);
	else
		*reinterpret_cast<char *>(value) = 0;
}
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractSmartLOValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSmartLOValue class not declared
#endif
/*
CPGLAbstractSmartLOValue::CPGLAbstractSmartLOValue (IFetchSubject *fetchSubject)
{
	_isOpened = false;
	_isMeasured = false;
	fetchSubject->attach(this);
}

CPGLAbstractSmartLOValue::CPGLAbstractSmartLOValue (INT32 size, void *value)
{
	_isOpened = false;

	mint error;
	ifx_int8 estbytes;
	ifx_lo_create_spec_t *create_spec = 0;
	mint byteswrote;

	checkInformixResult(ifx_lo_def_create_spec(&create_spec));

	ifx_lo_specset_flags(create_spec, LO_LOG);
	ifx_int8cvint(size, &estbytes);
	ifx_lo_specset_estbytes(create_spec, &estbytes);
	if ((_descriptor = ifx_lo_create(create_spec, LO_RDWR, &_value, &error)) == -1)
		checkInformixResult(error);
	_isOpened = true;

	byteswrote = ifx_lo_write(_descriptor, reinterpret_cast<char *>(value), size, &error);
	if (byteswrote < size)
		checkInformixResult(error);
}

CPGLAbstractSmartLOValue::~CPGLAbstractSmartLOValue ()
{
	if (_isOpened)
		ifx_lo_close(_descriptor);
}

INT32 CPGLAbstractSmartLOValue::getType ()
{
	return SQLUDTFIXED;
}

INT32 CPGLAbstractSmartLOValue::getLength ()
{
	return sizeof(_value);
}

char * CPGLAbstractSmartLOValue::getData ()
{
	return reinterpret_cast<char *>(&_value);
}

INT32 CPGLAbstractSmartLOValue::getBlobSize ()
{
	if (!_isMeasured) {
		ifx_lo_stat_t *lo_stat;
		checkInformixResult(ifx_lo_stat(getDescriptor(), &lo_stat));
		ifx_int8_t size;
		ifx_lo_stat_size(lo_stat, &size);
		ifx_int8toint(&size, reinterpret_cast<mint *>(&_size));
		_isMeasured = true;
		ifx_lo_stat_free(lo_stat);
	}
	return _size;
}

void CPGLAbstractSmartLOValue::getBlobData (INT32 length, void *value)
{
	mint error;
	mint bytesread;
	bytesread = ifx_lo_read(getDescriptor(), static_cast<char *>(value), length, &error);
	if (bytesread < length)
		checkInformixResult(error);
}

mint CPGLAbstractSmartLOValue::getDescriptor ()
{
	if (!_isOpened) {
		mint error;
		if ((_descriptor = ifx_lo_open(&_value, LO_RDONLY, &error)) == -1)
			checkInformixResult(error);
		_isOpened = true;
	}
	return _descriptor;
}

void CPGLAbstractSmartLOValue::update ()
{
	if (_isOpened) {
		checkInformixResult(ifx_lo_close(_descriptor));
		_isOpened = false;
		_isMeasured = false;
	}
}

*/


/////////////////////////////////////////////////////////////////////////////
// CPGLColDesc

CPGLColDesc::CPGLColDesc() : 
    colNameLen(),
    colType(),
    colSize(),
    colScale(),
    colNullable(),
    colLength(),
    colPrecision(),
    colAutoIncrement(false),
    colSearchable(false),
    colReadOnly(false),
    colExactBlobSize(false),
    customBool(false),
    geoOid(0),
    typeMod(0)
{
	memset(colName,0,sizeof(colName)) ;
}


void CPGLColDesc::initialize(const PGresult * res, INT32 rowno, INT32 colno)  
{
	strcpy(colName ,PQfname(res,colno)) ;
	colType = PQftype(res,colno) ;
	colSize = PQfsize(res,colno) ;
    typeMod = PQfmod(res,colno) ;
    if ((strcasecmp("ST_AsText", colName) == 0) && (geoOid > 0)) {
        colType = geoOid ;
        typeMod = 1 ;
    }

	// colScale
	// colNullable 
	if (rowno < 0) 
    {
        /* Processing a FAKE row: */
        colLength = (colSize >=0) ? colSize: 0 ;	/* to avoid a length <0, even in fake row */
	}
    else 
    {
		/* Processing an ACTUAL row: [index is 0-rel.] */
		colLength = PQgetlength(res, rowno, colno) ;	/* Defined only if row>=0, i.e. an ACTUAL ROW */
    }
        
        
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColDesc::initialize geoOid: " << geoOid << "\n" ;
	cout << "CPGLColDesc::initialize PQfname: " << colName << "\n" ;
	cout << "CPGLColDesc::initialize PQfmod: " << typeMod << "\n" ;
	cout << "CPGLColDesc::initialize PQftype: " <<  colType << "\n" ;
	cout << "CPGLColDesc::initialize PQfsize: " << colSize << "\n" ;
	cout << "CPGLColDesc::initialize PQfformat: " << PQfformat(res,colno) << "\n" ;
	cout << "CPGLColDesc::initialize PQgetlength: " << colLength << "\n" ;
#endif

}

/////////////////////////////////////////////////////////////////////////////
// CPGLVariable

CPGLVariable::CPGLVariable () :
    _bytesNum(0),
    _value(0),
    _isNull(false),
    _colDesc(),
    _GeoOid(0)
{
//	_value = 0;
//	_bytesNum = 0 ;
//	_isNull = false ;
#ifdef PGL_SHOW_WARNING
#warning MP CPGLVariable::_sqlvar member not declared
#endif
//	_sqlvar = 0;
}

CPGLVariable::~CPGLVariable ()
{
	delete _value;
}


void CPGLVariable::initialize (const PGresult *res, INT32 rowno, INT32 colno)
{
	_colDesc.geoOid = _GeoOid ;
	_colDesc.initialize(res,rowno,colno) ;

#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLVariable::initialize ----------\n" ;
	cout << "CPGLVariable::initialize    rowno " << rowno << "\n" ;
	cout << "CPGLVariable::initialize      col " << colno << "\n" ;
	cout << "CPGLVariable::initialize     name " << _colDesc.colName << "\n" ;
	cout << "CPGLVariable::initialize     type " << _colDesc.colType << "\n" ;
	cout << "CPGLVariable::initialize     size " << _colDesc.colSize << "\n" ;
	cout << "CPGLVariable::initialize   length " << _colDesc.colLength << "\n" ;
	cout << "CPGLVariable::initialize nullable " << boolalpha << _colDesc.colNullable << "\n" ;
	cout << "CPGLVariable::initialize  typeMod " << _colDesc.typeMod << "\n" ;
	cout << "CPGLVariable::initialize ----------\n" ;
#endif
}

void CPGLVariable::setGeographyOid(int v) 
{
    _GeoOid = v ;
}

INT32 CPGLVariable::getLength () const
{
#ifdef PGL_SHOW_WARNING
#warning MP CPGLVariable::_sqlvar member not declared
#endif
//	return _sqlvar->sqllen;
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLVariable::getLength: length: %d\n",int(_colDesc.colLength)) ;
#endif
	return _colDesc.colLength ;
}

CPGLAbstractValue * CPGLVariable::getValue () const
{
	return _value;
}

void CPGLVariable::setValue (CPGLAbstractValue *value, const PGresult *res, INT32 rowno, INT32 colno)
{
#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLVariable::setValue\n") ;
#endif
	delete _value;
	_value = value;
	_bytesNum = 0;

#ifdef ACDBEXPPGL_DEBUG
	printf("CPGLVariable::setValue [%p] type: %d len: %d, buffer %p - pgsize: %d (col: %d)\n", _value, int(value->getPglType()),int(value->getLength()), value->getData(), PQfsize(res, colno), colno) ;
#endif
    
	_isNull = true ;
	if (rowno>=0) {
		_isNull = PQgetisnull(res, rowno, colno) ;	// both zero relative
    	if (!_isNull) 
		{
            char * pval = PQgetvalue(res,rowno,colno) ; // both zero relative
		 
#ifdef ACDBEXPPGL_DEBUG
	        int i, l=value->getLength();
	        printf("CPGLVariable::setValue : 0x");
	        for (i=0; i<l; i++)
	        {
		        printf(" %02x", static_cast<unsigned int>(reinterpret_cast<unsigned char*>(pval)[i])) ;
	        }
	        printf("\n");
#endif
		   
		    value->setData(pval) ; 
            _bytesNum = int(value->getLength()) ;	
    	}
    }
    if (_isNull) value->setData(0) ; 
}

bool CPGLVariable::isNull () const
{
	return _isNull ;
}

void CPGLVariable::setNull ()
{
	delete _value;
	_value = 0;
	_bytesNum = 0 ;
	_isNull = true ;
}

const char * CPGLVariable::getName () const
{
	return _colDesc.colName ;
}

INT32 CPGLVariable::getXID () const
{
	return _value ? _value->getXID() : _colDesc.typeMod ;
}

bool CPGLVariable::getNullable () const
{
	return _colDesc.colNullable ;
}
