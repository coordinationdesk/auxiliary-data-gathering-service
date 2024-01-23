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

	$Prod: A.C.S. DBXpress for Postgres $

	$Id$

	$Author$
	
	$Log$
	Revision 1.7  2013/05/07 10:43:52  marpas
	mplementation of geography type, lookup function rationalization
	
	Revision 1.6  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.5  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.4  2010/10/22 15:56:46  enrcar
	EC:: Added support for Postgres Server ver >= 9.0 (DateTime internally stored as integer)
	


*/

#pragma once

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif

#include "pglvariable.hpp"
#include "pglcommand.hpp"
#include <map>


/////////////////////////////////////////////////////////////////////////////
// CColumnDescription

class CColumnDescription
{

public:
    CColumnDescription() : 	
        _name(0),
	    _dataType(),
	    _length(),
	    _precision(),
	    _scale(),
	    _isNullable(false),
	    _isAutoIncrement(false),
	    _isReadOnly(false),
	    _isSearchable(false),
	    _isBlobSizeExact(false)
    {}

    
	void describe ( const char* name, 
					DataType dataType, 
					UINT32 length,
					INT16 precision, 
					INT16 scale, 
					BOOL isNullable, 
					BOOL isAutoIncrement, 
					BOOL isReadOnly, 
					BOOL isSearchable, 
					BOOL isBlobSizeExact ) ;

	UINT16 getNameLength () const { return strlen(_name); }
	const char * getName () const { return _name; }
	const DataType& getDataType () const { return _dataType; }
	UINT32 getLength () const { return _length; }
	void setLength (UINT32 value) { _length = value; }
	INT16 getPrecision () const { return _precision; }
	INT16 getScale () const { return _scale; }
	BOOL isNullable () const { return _isNullable; }
	BOOL isAutoIncrement () const { return _isAutoIncrement; }
	BOOL isReadOnly () const { return _isReadOnly; }
	BOOL isSearchable () const { return _isSearchable; }
	BOOL isBlobSizeExact () const { return _isBlobSizeExact; }
private:
    CColumnDescription(const CColumnDescription &) ;
    CColumnDescription & operator= (const CColumnDescription &) ;
private:
	const char * _name;
	DataType _dataType;
	UINT32 _length;
	INT16 _precision;
	INT16 _scale;
	BOOL _isNullable;
	BOOL _isAutoIncrement;
	BOOL _isReadOnly;
	BOOL _isSearchable;
	BOOL _isBlobSizeExact;
};



/////////////////////////////////////////////////////////////////////////////
// CPGLColumnFactory

class CPGLColumnFactory
{
public:

	CPGLColumnFactory() ;
		
public:
	struct CreateValueArgs
	{
		INT32 length;
		INT32 precision;
		INT32 scale;
		INT32 blobSize;
		IFetchSubject *fetchSubject;
	};

	typedef CPGLAbstractValue * (*CreateValueFunc) (const CreateValueArgs& args);

	class DescribeArgs
	{
	public:
		CPGLTypesMapping *mapping;
		INT32 type;
		INT32 length;
		const char *name;
		INT16 scale;
		INT32 xid;
		INT32 precision;
		bool isNullable ;
		bool autoIncrement ;
		bool isReadOnly ;
		bool isSearchable ;
		bool isBlobSizeExact ;
		CColumnDescription *description;
		CPGLAbstractValue * createValue (const CreateValueArgs& args) const;
	private:
		CreateValueFunc createValueFunc;
		friend class CPGLColumnFactory;
	};

	static void describe (DescribeArgs& args);
    static void setGeographicOID(int) ;
	typedef void (*DescribeFunc) (DescribeArgs& args);

private:
	void initPGLLookup() ;

private:
	static void describeUnknown (DescribeArgs& args);
	static CPGLAbstractValue * createUnknownValue (const CreateValueArgs& args);

	static void describeChar (DescribeArgs& args);
	static CPGLAbstractValue * createCharValue (const CreateValueArgs& args);

	static void describeSmallInt (DescribeArgs& args);
	static CPGLAbstractValue * createSmallIntValue (const CreateValueArgs& args);

	static void describeInteger (DescribeArgs& args);
	static CPGLAbstractValue * createIntegerValue (const CreateValueArgs& args);

	static void describeFloat (DescribeArgs& args);
	static CPGLAbstractValue * createFloatValue (const CreateValueArgs& args);

	static void describeSmallFloat (DescribeArgs& args);
	static CPGLAbstractValue * createSmallFloatValue (const CreateValueArgs& args);

	static void describeDecimal (DescribeArgs& args);
	static CPGLAbstractValue * createDecimalValue (const CreateValueArgs& args);

	static void describeSerial (DescribeArgs& args);
	static CPGLAbstractValue * createSerialValue (const CreateValueArgs& args);

	static void describeDate (DescribeArgs& args);
	static CPGLAbstractValue * createDateValue (const CreateValueArgs& args);

	static void describeMoney (DescribeArgs& args);
	static CPGLAbstractValue * createMoneyValue (const CreateValueArgs& args);

	static void describeDateTime (DescribeArgs& args);
	static CPGLAbstractValue * createDateTimeValue (const CreateValueArgs& args);

	static void describeBytes (DescribeArgs& args);
	static CPGLAbstractValue * createBytesValue (const CreateValueArgs& args);

	static void describeText (DescribeArgs& args);
	static CPGLAbstractValue * createTextValue (const CreateValueArgs& args);

	static void describeVarChar (DescribeArgs& args);
	static CPGLAbstractValue * createVarCharValue (const CreateValueArgs& args);

	static void describeJson (DescribeArgs& args);
	static CPGLAbstractValue * createJsonValue (const CreateValueArgs& args);

	static void describeJsonB (DescribeArgs& args);
	static CPGLAbstractValue * createJsonBValue (const CreateValueArgs& args);

	static void describeInterval (DescribeArgs& args);
	static CPGLAbstractValue * createIntervalValue (const CreateValueArgs& args);

	static void describeInt8 (DescribeArgs& args);
	static CPGLAbstractValue * createInt8Value (const CreateValueArgs& args);

	static void describeSerial8 (DescribeArgs& args);
	static CPGLAbstractValue * createSerial8Value (const CreateValueArgs& args);

	static void describeVarBinary (DescribeArgs& args);
	static CPGLAbstractValue * createVarBinaryValue (const CreateValueArgs& args);

	static void describeUDTFixed (DescribeArgs& args);
	static CPGLAbstractValue * createUDTValue (const CreateValueArgs& args);
	static CPGLAbstractValue * createBlobValue (const CreateValueArgs& args);
	static CPGLAbstractValue * createClobValue (const CreateValueArgs& args);

	static void describeLVarChar (DescribeArgs& args);
	static void describeLVarCharAsString (DescribeArgs& args);
	static void describeLVarCharAsBlob (DescribeArgs& args);
	static CPGLAbstractValue * createLVarCharValue (const CreateValueArgs& args);

	static void describeBool (DescribeArgs& args);
	static CPGLAbstractValue * createBoolValue (const CreateValueArgs& args);

	static std::map<int, DescribeFunc> DescribeFuncTable ;
	static CPGLColumnFactory _theColumnFactory ;
};


/////////////////////////////////////////////////////////////////////////////
// CPGLCursor - SQLCursor implementation for Postgres

class CPGLCursor : public CErrorHandlingImpl< CUnknownImpl<SQLCursor> >, private IFetchSubject
{
public:
	CPGLCursor (PGresult *res, CPGLCommand *);
	virtual ~CPGLCursor () throw() ;

	CPGLConnection * getConnection ();
	INT16 getColumnCount ();
	CPGLVariable& getColumn (INT16 number);
	CColumnDescription& getDescription (INT16 number);
	void setErrorMessage (const char* errorMessage);

// SQLCursor
	SQL_METHOD setOption (eSQLCursorOption eCurOption, INT32 lValue);
	SQL_METHOD getOption (eSQLCursorOption eCurOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength);
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLCursorOption eCurOption, INT64 lValue);
	SQL_METHOD getOption (eSQLCursorOption eCurOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength);
#endif
	SQL_METHOD getColumnCount (pUINT16 puColumns);
	SQL_METHOD getColumnNameLength (UINT16 uColumnNumber, pUINT16 puLen);
	SQL_METHOD getColumnName (UINT16 uColumnNumber, pCHAR pColumnName);
	SQL_METHOD getColumnType (UINT16 uColumnNumber, pUINT16 puLogType, pUINT16 puSubType);
	SQL_METHOD getColumnLength (UINT16 uColumnNumber, pUINT32 pulLength);
	SQL_METHOD getColumnPrecision (UINT16 uColumnNumber, pINT16 piPrecision);
	SQL_METHOD getColumnScale (UINT16 uColumnNumber, pINT16 piScale);
	SQL_METHOD isNullable (UINT16 uColumnNumber, pBOOL pbNullable);
	SQL_METHOD isAutoIncrement (UINT16 uColumnNumber, pBOOL pbAutoIncrement);
	SQL_METHOD isReadOnly (UINT16 uColumnNumber, pBOOL pbReadOnly);
	SQL_METHOD isSearchable (UINT16 uColumnNumber, pBOOL pbSearchable);
	SQL_METHOD isBlobSizeExact (UINT16 uColumnNumber, pBOOL pbBlobExactSize);
	SQL_METHOD next ();
	SQL_METHOD getString (UINT16 uColumnNumber, pCHAR pData, pBOOL pbIsNull);
	SQL_METHOD getShort (UINT16 uColumnNumber, pINT16 pData, pBOOL pbIsNull);
	SQL_METHOD getLong (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull);
	SQL_METHOD getDouble (UINT16 uColumnNumber, pDFLOAT pData, pBOOL pbIsNull);
	SQL_METHOD getBcd (UINT16 uColumnNumber, pFMTBcd pData, pBOOL pbIsNull);
	SQL_METHOD getTimeStamp (UINT16 uColumnNumber, pCTIMESTAMP pData, pBOOL pbIsNull);
	SQL_METHOD getTime (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull);
	SQL_METHOD getDate (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull);
	SQL_METHOD getBytes (UINT16 uColumnNumber, pBYTE pData, pBOOL pbIsNull);
	SQL_METHOD getBlobSize (UINT16 uColumnNumber, pUINT32 pulLength, pBOOL pbIsNull);
	SQL_METHOD getBlob (UINT16 uColumnNumber, pVOID pData, pBOOL pbIsNull, UINT32 ulLength);

private:

	virtual void attach (Observer observer);
	virtual void notify ();
    CPGLCursor(const CPGLCursor &) ;
    CPGLCursor & operator= (const CPGLCursor &) ;
    
private:

	int _currentRow ;

	PGresult *_res;
	CPGLCommand *_command;
	bool _opened;
	CPGLVariable *_variables;
	CColumnDescription *_descriptions;
	INT16 _variablesNumber ;
	INT32 _serverVersion ;
	int _observerCount;
	Observer *_observers;
	CPGLColumnFactory::DescribeArgs _describeArgs;
};

class CPGLTrimingCursor : public CPGLCursor
{
public:
	CPGLTrimingCursor (PGresult *res, CPGLCommand *c) : CPGLCursor(res, c) {}
	SQL_METHOD getString (UINT16 uColumnNumber, pCHAR pData, pBOOL pbIsNull);
};

