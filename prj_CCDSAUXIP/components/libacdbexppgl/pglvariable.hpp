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

#ifndef __pglvariable_hpp__
#define __pglvariable_hpp__

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "pglerror.hpp"


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractValue

class CPGLAbstractValue
{
public:
    CPGLAbstractValue() throw() {}
	virtual ~CPGLAbstractValue () {} // PRQA S 2131

	virtual INT32 getType () = 0;
	virtual INT32 getLength () = 0;
	virtual char * getData () = 0;
	virtual void setData(const char *) { throw CInvalidCastError() ; }    // PRQA S 2131, 3508
	virtual INT16 getXID () { return 0; }                                // PRQA S 2131
//	virtual char * getTypeName () { return 0; }
//	virtual INT16 getTypeNameLength () { return 0; }

	virtual void asString (char *value) { throw CInvalidCastError(); }  // PRQA S 2131, 3508
	virtual double asDouble () { throw CInvalidCastError(); }   // PRQA S 2131, 3508
	virtual FMTBcd& asFMTBcd () { throw CInvalidCastError(); }  // PRQA S 2131, 3508
	virtual INT32 getPglType () { return getType() ; }          // PRQA S 2131
protected:
    CPGLAbstractValue(const CPGLAbstractValue &) throw() {}
    CPGLAbstractValue& operator= (const CPGLAbstractValue &) throw() { return *this ; } // PRQA S 4072
};


/////////////////////////////////////////////////////////////////////////////
// Fetch observing classes

struct IFetchObserver // PRQA S 2171, 2173, 2175
{
    IFetchObserver() throw() {}
	virtual ~IFetchObserver () throw() {} // PRQA S 2131
	virtual void update () = 0;
protected:
    IFetchObserver(const IFetchObserver &) throw() {}
    IFetchObserver& operator= (const IFetchObserver &) throw() {return *this ; } // PRQA S 4072
};

struct IFetchSubject   // PRQA S 2171, 2173, 2175
{
    IFetchSubject() throw() {}
	virtual ~IFetchSubject () {} // PRQA S 2131
	typedef IFetchObserver * Observer;
	virtual void attach (Observer observer) = 0;
	virtual void notify () = 0;
protected:
    IFetchSubject(const IFetchSubject &) throw() {}
    IFetchSubject& operator= (const IFetchSubject &) throw() {return *this ; } // PRQA S 4072
};


/////////////////////////////////////////////////////////////////////////////
// CPGLSimpleValue

template <typename ValueType>
class CPGLAbstractSimpleValue : public CPGLAbstractValue
{
public:
	CPGLAbstractSimpleValue () : CPGLAbstractValue(), _value() {}
	explicit CPGLAbstractSimpleValue (ValueType value): CPGLAbstractValue(), _value(value) {}
	virtual ~CPGLAbstractSimpleValue () throw() {} // PRQA S 2131

	virtual INT32 getLength () { return sizeof(_value); }
	virtual char * getData () { return reinterpret_cast<char *>(&_value); }
	virtual double asDouble () { return _value; }
	operator ValueType& () { return _value; }


	virtual void setData(const char *c) 
	{ 
    	if (!c)
        {
            #ifdef ACDBEXPPGL_DEBUG
	            printf("CPGLAbstractSimpleValue::setData: NULL PTR\n") ;
            #endif
            
            return ;
        }
    
    
		ssize_t s = sizeof(_value) ;	/* Size of Actual C type */
    	_value ^= _value ;	/* Reset value */

    	if ( getLength() != s )
			throw CInvalidCastError("CPGLAbstractSimpleValue called with invalid type",__FILE__,__LINE__) ;

	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLAbstractSimpleValue::setData") ;
		for (size_t i=0; i < sizeof(_value); i++)
			printf(" 0x%02x", reinterpret_cast<const unsigned char*>(c)[i]) ;
		printf("\n") ;
	#endif
    	/* Following value-assignment is endianness-independent: */
    	for (size_t i=0; i<sizeof(_value); i++) 
    		_value |=  (reinterpret_cast<const unsigned char*>(c)[i] << 8*(s-i-1) ) ;

	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLAbstractSimpleValue::setData %d\n",int(_value)) ;
	#endif

	} 


private:
	ValueType _value;
};


template <typename ValueType>
class CPGLAbstractFloatValue : public CPGLAbstractValue
{
public:
	CPGLAbstractFloatValue () :_value() {}
	CPGLAbstractFloatValue (ValueType value): _value(value) {}
	virtual INT32 getLength () { return sizeof(_value); }
	virtual char * getData () { return reinterpret_cast<char *>(&_value); }
	virtual double asDouble () { return _value; }
	operator ValueType& () { return _value; }


	virtual void setData(const char *c) 
	{ 
    	if (!c)
        {
            #ifdef ACDBEXPPGL_DEBUG
	            printf("CPGLAbstractFloatValue::setData: NULL PTR\n") ;
            #endif
            
            return ;
        }
    
		ssize_t s = sizeof(_value) ;	/* Size of Actual C type */
    	_value = 0 ;	/* Reset value */

    	if ( getLength() != s )
			throw CInvalidCastError("CPGLAbstractFloatValue called with invalid type",__FILE__,__LINE__) ;

		char tmp[sizeof (ValueType)]  ;
    	/* Following value-assignment is endianness-independent: */
    	for (size_t i=0; i<sizeof(_value); i++) 
    		tmp[i] = c[sizeof (ValueType)-i-1] ;
		
        ValueType *vt = reinterpret_cast<ValueType *>(tmp) ;
		_value = *vt ;
	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLAbstractFloatValue::setData %10.20lf\n",double(_value)) ;
	#endif

	} 

private:
	ValueType _value;
};




template <typename ValueType>
class CPGLAbstractBoolValue : public CPGLAbstractValue
{
public:
	CPGLAbstractBoolValue () {}
	CPGLAbstractBoolValue (ValueType value): _value(value) {}
	virtual INT32 getLength () { return sizeof(_value); }
	virtual char * getData () { return reinterpret_cast<char *>(&_value); }
	virtual double asDouble () { return _value; }
	operator ValueType& () { return _value; }


	virtual void setData(const char *c) 
	{ 
        if (!c)
        {
            #ifdef ACDBEXPPGL_DEBUG
	            printf("CPGLAbstractBoolValue::setData: NULL PTR\n") ;
            #endif
            
            return ;
        }
   
    
		ssize_t s = sizeof(_value) ;	/* Size of Actual C type */
    	_value ^= _value ;	/* Reset value */

    	if ( getLength() != s )
			throw CInvalidCastError("CPGLAbstractSimpleValue called with invalid type",__FILE__,__LINE__) ;

	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLAbstractBoolValue::setValue") ;
		for (size_t i=0; i < sizeof(_value); i++)
			printf(" 0x%02x", c[i]) ;
		printf("\n") ;
	#endif
    	_value = (* reinterpret_cast<const ValueType *>(c) & 0x0001) ? 1 : 0 ;

	#ifdef ACDBEXPPGL_DEBUG
		printf("CPGLAbstractBoolValue::setData %d\n",int(_value)) ;
	#endif

	} 


private:
	ValueType _value;
};



template <typename ValueType, INT32 PGLType>
class CPGLSimpleValue : public CPGLAbstractSimpleValue<ValueType>
{
public:
	CPGLSimpleValue () {}
	CPGLSimpleValue (ValueType value): CPGLAbstractSimpleValue<ValueType>(value) {}

    virtual INT32 getType () { return PGLType; }
};

template <typename ValueType, INT32 PGLType>
class CPGLFloatValue : public CPGLAbstractFloatValue<ValueType>
{
public:
	CPGLFloatValue () {}
	CPGLFloatValue (ValueType value): CPGLAbstractFloatValue<ValueType>(value) {}

    virtual INT32 getType () { return PGLType; }
};

template <typename ValueType, INT32 PGLType>
class CPGLBoolValue : public CPGLAbstractBoolValue<ValueType>
{
public:
	CPGLBoolValue () {}
	CPGLBoolValue (ValueType value): CPGLAbstractBoolValue<ValueType>(value) {}

    virtual INT32 getType () { return PGLType; }
};


/////////////////////////////////////////////////////////////////////////////
// CPGLStringValue

class CPGLAbstractStringValue : public CPGLAbstractValue
{
public:
	CPGLAbstractStringValue (INT32 length);
	CPGLAbstractStringValue (INT32 length, char *value);
	virtual ~CPGLAbstractStringValue ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void asString (char *value);
	virtual void setData(const char *) ; 

	operator char * ();
private:
    CPGLAbstractStringValue(const CPGLAbstractStringValue &) ;
    CPGLAbstractStringValue & operator= (const CPGLAbstractStringValue &) ;
private:
	INT32 _length;
	char *_value;
};

template <INT32 PGLType>
class CPGLStringValue : public CPGLAbstractStringValue
{
public:
	CPGLStringValue (INT32 length): CPGLAbstractStringValue(length) {}
	CPGLStringValue (INT32 length, char *value): CPGLAbstractStringValue(length, value) {}
	virtual INT32 getType () { return PGLType; }
    virtual INT16 getXID() { return PGLType == fldADT  ? 1 : 0 ; }
};


/////////////////////////////////////////////////////////////////////////////
// CPGLJsonValue / CPGLJsonBValue

class CPGLAbstractJsonValue : public CPGLAbstractValue
{
public:
	CPGLAbstractJsonValue (INT32 length, const bool is_binary);
	CPGLAbstractJsonValue (INT32 length, char *value, const bool is_binary);
	virtual ~CPGLAbstractJsonValue ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void asString (char *value);
	virtual void setData(const char *) ; 
	virtual INT32 getType () { return fldZSTRING; }

	operator char * ();
private:
    CPGLAbstractJsonValue(const CPGLAbstractJsonValue &) ;
    CPGLAbstractJsonValue & operator= (const CPGLAbstractJsonValue &) ;
private:
	INT32 _length;
	char *_value;
	bool _is_binary ; // TRUE for JSONB, FALSE for JSON
};

class CPGLJsonValue : public CPGLAbstractJsonValue
{
public:
	CPGLJsonValue (INT32 length): CPGLAbstractJsonValue(length, false) {}
	CPGLJsonValue (INT32 length, char *value): CPGLAbstractJsonValue(length, value, false) {}
};

class CPGLJsonBValue : public CPGLAbstractJsonValue
{
public:
	CPGLJsonBValue (INT32 length): CPGLAbstractJsonValue(length, true) {}
	CPGLJsonBValue (INT32 length, char *value): CPGLAbstractJsonValue(length, value, true) {}
};


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractDecimalValue

class CPGLAbstractDecimalValue : public CPGLAbstractValue, private IFetchObserver
{
public:
	CPGLAbstractDecimalValue (INT32 precision, INT32 scale, INT32 length, IFetchSubject *fetchSubject);
	CPGLAbstractDecimalValue (pFMTBcd value);
	virtual ~CPGLAbstractDecimalValue () throw() ;
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual double asDouble ();
	virtual void asString (char *value) ;
	virtual FMTBcd& asFMTBcd();
private:
    CPGLAbstractDecimalValue(const CPGLAbstractDecimalValue & ) ;
    CPGLAbstractDecimalValue & operator= (const CPGLAbstractDecimalValue & ) ;
	virtual void update ();
private:
	INT32 _precision;
	INT32 _scale;
	INT32 _length;
	char *_value ;
	bool _hasDouble;
	double _double;
	bool _hasBcd;
	FMTBcd _bcd;
};

template <INT32 PGLType>
class CPGLDecimalValue : public CPGLAbstractDecimalValue
{
public:
	CPGLDecimalValue (INT32 precision, INT32 scale, INT32 len, IFetchSubject *fetchSubject): CPGLAbstractDecimalValue(precision, scale, len, fetchSubject) {}
	CPGLDecimalValue (pFMTBcd value): CPGLAbstractDecimalValue(value) {}
	virtual INT32 getType () { return PGLType; }
};


/////////////////////////////////////////////////////////////////////////////
// CPGLDateTimeValue
// MARCO
class CPGLDateTimeValue : public CPGLAbstractValue
{
public:
	CPGLDateTimeValue ();
	CPGLDateTimeValue (INT32 value);
	CPGLDateTimeValue (pCTIMESTAMP value);
	virtual INT32 getType ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void setData (const char *);
	operator CTIMESTAMP& ();
private:
	virtual void update ();
	CTIMESTAMP _value ;
	static const signed long long int epoch_jd2000_offset = 946684800 ;	/* From EPOCH to JD2000 (No. of days) * 1000 in [sec]*/
	INT32 _scale ;
};

/////////////////////////////////////////////////////////////////////////////
// CPGLIntervalValue

class CPGLIntervalValue : public CPGLAbstractValue, private IFetchObserver
{
public:
	CPGLIntervalValue (INT16 qualifier, IFetchSubject *fetchSubject);
	virtual ~CPGLIntervalValue () throw() {}
	virtual INT32 getType ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void asString (char *value);
private:
	virtual void update ();
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::intrvl _value member not declared
#endif
//	intrvl _value;
	bool _isConverted;
#ifdef PGL_SHOW_WARNING
#warning MP CPGLIntervalValue::char _buffer[INTERVAL_AS_VARCHAR_LENGTH + 1] member not declared
#endif
//	char _buffer[INTERVAL_AS_VARCHAR_LENGTH + 1];
};


/////////////////////////////////////////////////////////////////////////////
// CPGLDateValue

class CPGLDateValue : public CPGLAbstractValue
{
public:
	CPGLDateValue ();
	CPGLDateValue (INT32 value);
	virtual INT32 getType ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void setData (const char *);
	operator INT32 ();
private:
	INT32 _value;
};


/////////////////////////////////////////////////////////////////////////////
// CPGLInt8Value

class CPGLAbstractInt8Value : public CPGLAbstractValue, private IFetchObserver
{
public:
	CPGLAbstractInt8Value () ;
	CPGLAbstractInt8Value (unsigned long long int value) ; 
	virtual ~CPGLAbstractInt8Value () throw() {}
	virtual INT32 getLength () ;
	virtual char * getData () ;
	virtual double asDouble () ;
	operator signed long long int& () ;
	virtual void asString (char *value) ; 
	virtual FMTBcd& asFMTBcd ();



	/* ENRCAR ADDED THIS FIXME (NOTE: WORKS) */	
	virtual void setData(const char *c) ;

private:
	virtual void update ();
	long long int _value;
	FMTBcd _bcd;
};


template <INT32 PGLType>
class CPGLInt8Value : public CPGLAbstractInt8Value
{

public:
	CPGLInt8Value () {} 
	CPGLInt8Value (unsigned long long int value): CPGLAbstractInt8Value(value) {}
//	virtual INT32 getType ();
//	virtual INT32 getLength ();
//	virtual char * getData ();
//	operator INT32 ();
private:
//	unsigned long long int _value;


public:

    virtual INT32 getType () { return PGLType; }

};


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractLOValue

class CPGLAbstractLOValue : public CPGLAbstractValue
{
public:
	virtual INT32 getBlobSize () = 0;
	virtual void getBlobData (INT32 length, void *value) = 0;
};


/////////////////////////////////////////////////////////////////////////////
// CPGLSimpleLOValue

class CPGLAbstractSimpleLOValue : public CPGLAbstractLOValue
{
public:
	CPGLAbstractSimpleLOValue (INT32 size);
	CPGLAbstractSimpleLOValue (INT32 size, void *value);
	virtual ~CPGLAbstractSimpleLOValue ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual INT32 getBlobSize ();
	virtual void getBlobData (INT32 length, void *value);
    virtual void setData(const char*);
    
private:
	UINT32 _value ;	/* Container for OID */
};


template <INT32 PGLType>
class CPGLSimpleLOValue : public CPGLAbstractSimpleLOValue
{
public:
	CPGLSimpleLOValue (INT32 size): CPGLAbstractSimpleLOValue(size) {}
	CPGLSimpleLOValue (INT32 size, void *value): CPGLAbstractSimpleLOValue(size, value) {}
	virtual INT32 getType () { return PGLType; }
};


/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractOpaqueValue
/*
class CPGLAbstractOpaqueValue : public CPGLAbstractLOValue
{
public:
	CPGLAbstractOpaqueValue ();
	CPGLAbstractOpaqueValue (INT32 size, void *value);
	virtual ~CPGLAbstractOpaqueValue ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual void asString (char *value);
	virtual INT32 getBlobSize ();
	virtual void getBlobData (INT32 length, void *value);
private:
	void *_value;
};
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLLVarCharValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLLVarCharValue class not declared
#endif
/*
class CPGLLVarCharValue : public CPGLAbstractOpaqueValue
{
public:
	CPGLLVarCharValue () {}
	CPGLLVarCharValue (INT32 size, void *value): CPGLAbstractOpaqueValue(size, value) {}
	virtual INT32 getType () { return CLVCHARPTRTYPE; }
	virtual INT16 getXID () { return XID_LVARCHAR; }
//	virtual char * getTypeName () { return "lvarchar"; }
//	virtual INT16 getTypeNameLength () { return sizeof("lvarchar") - 1; }
};
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLAbstractSmartLOValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLAbstractSmartLOValue class not declared
#endif
/*
class CPGLAbstractSmartLOValue : public CPGLAbstractLOValue, private IFetchObserver
{
public:
	CPGLAbstractSmartLOValue (IFetchSubject *fetchSubject);
	CPGLAbstractSmartLOValue (INT32 size, void *value);
	virtual ~CPGLAbstractSmartLOValue ();
	virtual INT32 getType ();
	virtual INT32 getLength ();
	virtual char * getData ();
	virtual INT32 getBlobSize ();
	virtual void getBlobData (INT32 length, void *value);
private:
	mint getDescriptor ();
	virtual void update ();
	ifx_lo_t _value;
	bool _isOpened;
	bool _isMeasured;
	INT32 _size;
	mint _descriptor;
};
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLCLOBValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLCLOBValue class not declared
#endif
/*
class CPGLCLOBValue : public CPGLAbstractSmartLOValue
{
public:
	CPGLCLOBValue (IFetchSubject *fetchSubject): CPGLAbstractSmartLOValue (fetchSubject) {}
	CPGLCLOBValue (INT32 size, void *value): CPGLAbstractSmartLOValue (size, value) {}
	virtual INT16 getXID () { return XID_CLOB; }
//	virtual char * getTypeName () { return "clob"; }
//	virtual INT16 getTypeNameLength () { return sizeof("clob") - 1; }
};
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLBLOBValue

#ifdef PGL_SHOW_WARNING
#warning MP CPGLBLOBValue class not declared
#endif
/*
class CPGLBLOBValue : public CPGLAbstractSmartLOValue
{
public:
	CPGLBLOBValue (IFetchSubject *fetchSubject): CPGLAbstractSmartLOValue (fetchSubject) {}
	CPGLBLOBValue (INT32 size, void *value): CPGLAbstractSmartLOValue (size, value) {}
	virtual INT16 getXID () { return XID_BLOB; }
//	virtual char * getTypeName () { return "blob"; }
//	virtual INT16 getTypeNameLength () { return sizeof("blob") - 1; }
};
*/

/////////////////////////////////////////////////////////////////////////////
// CPGLBooleanValue

class CPGLBooleanValue : public CPGLBoolValue<INT16, fldBOOL >
{
public:
	CPGLBooleanValue (): CPGLBoolValue<INT16, fldBOOL>(0) {}
	CPGLBooleanValue (INT16 value): CPGLBoolValue<INT16, fldBOOL>(value ? 1 : 0) {}
// MARCO
//	virtual INT16 getDb2Type () { return SQL_C_CHAR ; }
//	virtual INT16 getXID () { return XID_BOOLEAN; }
//	virtual char * getTypeName () { return "boolean"; }
//	virtual INT16 getTypeNameLength () { return sizeof("boolean") - 1; }
};




#define MAX_COLNAME_LENGTH 128

/////////////////////////////////////////////////////////////////////////////
// CPGLColDesc
struct CPGLColDesc
{
	CPGLColDesc() ;
	void initialize(const PGresult *, INT32 rowno, INT32 colno)  ;
	char    colName[MAX_COLNAME_LENGTH] ;
	INT16   colNameLen;
	INT32   colType;
	UINT32  colSize;
  	INT32   colScale;
	INT32   colNullable ;
	INT32   colLength ;
	INT32   colPrecision ;
	BOOL    colAutoIncrement ;
	BOOL    colSearchable ;
	BOOL    colReadOnly ;
	BOOL    colExactBlobSize ;
	bool    customBool ;
    int     geoOid ;
    INT32   typeMod ;
private:
	// not implemented ;
	CPGLColDesc(const CPGLColDesc&) ;
	CPGLColDesc operator=(const CPGLColDesc&) ;

} ;

/////////////////////////////////////////////////////////////////////////////
// CPGLVariable

class CPGLVariable
{
public:
	CPGLVariable ();
	~CPGLVariable ();
	void initialize(const PGresult *,INT32, INT32) ;
	INT32 getType () const;
	INT32 getLength () const;
	CPGLAbstractValue * getValue () const;
    void setGeographyOid(int) ;
	void setValue (CPGLAbstractValue *value, const PGresult * hstmt, INT32 rowno, INT32 colno);
	bool isNull () const;
	void setNull ();
	const char * getName () const;
	INT32 getXID () const;
	bool getNullable () const;
	INT16 getScale () const;
	bool getAutoIncrement() const ;
	INT32 getBytes() const ;
	bool getReadOnly() const ;
	bool getSearchable() const ;
	bool getBlobSizeExact() const ;
	INT32 getPrecision() const ;
private:
    CPGLVariable(const CPGLVariable &) ;
    CPGLVariable & operator= (const CPGLVariable &) ;
private:
    INT32 _bytesNum;
    CPGLAbstractValue *_value;
	bool _isNull ;
	CPGLColDesc _colDesc ;
    int _GeoOid ;
};

#warning CUSTOM_SQL_BOOL defined to 500 to support BOOL custom values
#define CUSTOM_SQL_BOOL 500

inline INT32 CPGLVariable::getType () const
{
//	return _sqlvar->sqltype;
	if (_colDesc.customBool)
		return CUSTOM_SQL_BOOL ;
	return _colDesc.colType ;
}

inline INT16 CPGLVariable::getScale () const
{
	return _colDesc.colScale ;
}

inline bool CPGLVariable::getAutoIncrement () const
{
	return _colDesc.colAutoIncrement ;
}

inline INT32 CPGLVariable::getBytes () const
{
//	return _sqlvar->sqltype;
	return _bytesNum ;
}

inline bool CPGLVariable::getReadOnly () const
{
	return _colDesc.colReadOnly ;
}

inline bool CPGLVariable::getSearchable () const
{
	return _colDesc.colSearchable ;
}

inline bool CPGLVariable::getBlobSizeExact () const
{
	return _colDesc.colExactBlobSize ;
}

inline INT32 CPGLVariable::getPrecision () const
{
	return _colDesc.colPrecision ;
}

#endif /* __pglvariable_hpp__ */
