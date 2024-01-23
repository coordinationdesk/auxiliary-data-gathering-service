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

	$Log$
	Revision 1.12  2017/11/21 16:11:54  marpas
	in case no variable has been allocated for an empty set, the cursor emit 'no data' error trying to read values
	
	Revision 1.11  2016/07/28 09:06:07  marpas
	mapping Text into (fldBLOB, fldstHMEMO) in order to support clobs
	
	Revision 1.10  2013/05/14 18:01:27  marpas
	implementing PostGRES/PostGIS support
	
	Revision 1.9  2013/05/07 10:43:52  marpas
	mplementation of geography type, lookup function rationalization
	
	Revision 1.8  2012/12/10 17:02:29  marpas
	qa rules
	
	Revision 1.7  2012/02/02 14:43:02  marpas
	removing compiler warnings and enforcing robustness
	
	Revision 1.6  2010/10/27 08:36:53  enrcar
	EC:: time resolution is always (since the variable is filled from DB) expressed in nsec. No further conversion is needed
	
	Revision 1.5  2010/10/22 15:56:46  enrcar
	EC:: Added support for Postgres Server ver >= 9.0 (DateTime internally stored as integer)
	
	
*/

#include "pglcursor.hpp"
#include "pglerror.hpp"
#include <iostream>
#include <iomanip>

using namespace std; 

/////////////////////////////////////////////////////////////////////////////
// CColumnDescription

void CColumnDescription::describe(const char * name, 
                                  DataType dataType,
                                  UINT32 length,
                                  INT16 precision, 
                                  INT16 scale, 
                                  BOOL isNullable, 
                                  BOOL isAutoIncrement,
                                  BOOL isReadOnly, 
					              BOOL isSearchable, 
                                  BOOL isBlobSizeExact)
{
	#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLCursor::CColumnDescription::describe\n" ;
		cout << "\tname: " << name << "\n" ;
		cout << "\tdataType: (" << dataType.logType << "," << dataType.subType << ")\n" ;
		cout << "\tlength: " << length << "\n" ;
		cout << "\tprecision: " << precision << "\n" ;
		cout << "\tscale: " << scale << "\n" ;
		cout << "\tisNullable: " << boolalpha << isNullable << "\n" ;
		cout << "\tisAutoIncrement: " << boolalpha << isAutoIncrement << "\n" ;
		cout << "\tisReadOnly: " << boolalpha << isReadOnly << "\n" ;
		cout << "\tisSearchable: " << boolalpha << isSearchable << "\n" ;
		cout << "\tisBlobSizeExact: " << boolalpha << isBlobSizeExact << "\n" ;
	#endif

	_name = name;
	_dataType = dataType;
	_length = length;
	_precision = precision;
	_scale = scale;
	_isNullable = isNullable;
	_isAutoIncrement = isAutoIncrement;
	_isReadOnly = isReadOnly;
	_isSearchable = isSearchable;
	_isBlobSizeExact = isBlobSizeExact;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLCursor - SQLCursor implementation for Postgres
CPGLCursor::CPGLCursor (PGresult *res, CPGLCommand *command) : 
	_currentRow(-1),
	_res(res),
    _command(0),
	_opened(false),
	_variables(0),
	_descriptions(0),
	_variablesNumber(0),
	_serverVersion(-1),
    _observerCount(0),
	_observers(0),
    _describeArgs()
{

#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::CPGLCursor: ctor method called. res: " << res <<" command: " << command << "\n" ;
#endif

	(_command = command)->AddRef();

#ifdef PGL_SHOW_WARNING
#warning MP CPGLCursor::CPGLCursor partial implementation
#endif

	_opened = true;

	(_variables = new CPGLVariable [_variablesNumber = getColumnCount()])--;
	(_descriptions = new CColumnDescription [getColumnCount()])--;

	_observers = new Observer [getColumnCount()];

	_describeArgs.mapping = getConnection()->getTypesMapping();

	_serverVersion = getConnection()->getServerVersionInt() ;	/* Store server version */

	CPGLColumnFactory::CreateValueArgs createValueArgs;


	if ((createValueArgs.blobSize = _command->getBlobSize()) > 0)
		createValueArgs.blobSize *= 1024;
	createValueArgs.fetchSubject = this;

	CPGLVariable *variable = _variables;
	CColumnDescription *description = _descriptions;
	#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLCursor::CPGLCursor: columns: " << getColumnCount() << "\n" ;
	#endif
	for (int i = 0; i < getColumnCount(); i++)
	{
        #ifdef ACDBEXPPGL_DEBUG
	        cout << "\nCPGLCursor::CPGLCursor: executing initialize method for column #" << i << "\n" ;
        #endif
        ++variable ;
        variable->setGeographyOid(getConnection()->getGeographyOid()) ;
		variable->initialize(_res, _currentRow, i); 

		_describeArgs.type = variable->getType();
		_describeArgs.length = createValueArgs.length = variable->getLength();
		_describeArgs.name = variable->getName();
		_describeArgs.scale = createValueArgs.scale = variable->getScale();
		_describeArgs.xid = variable->getXID();
		_describeArgs.autoIncrement = variable->getAutoIncrement();
		_describeArgs.description = ++description;
		_describeArgs.isNullable = variable->getNullable();
		_describeArgs.isReadOnly = variable->getReadOnly();
		_describeArgs.isSearchable = variable->getSearchable();
		_describeArgs.isBlobSizeExact = variable->getBlobSizeExact();
		_describeArgs.precision = createValueArgs.precision = variable->getPrecision();

// Perform server + type specific overrides:
		{
			const INT32 type_DATETIME = 1114 ;
			if ( (_serverVersion >= 90000) && (_describeArgs.type == type_DATETIME) )
			{
				/* Postgres version >= 9 stores DateTimes as integer, with fraction of 6 digits */
				_describeArgs.scale = createValueArgs.scale = 6 ;
			} else if ( (_serverVersion < 90000) && (_describeArgs.type == type_DATETIME) )
			{
				/* Postgres version < 9 store DateTimes as double */
				_describeArgs.scale = createValueArgs.scale = -1 ;
			}
		}
	

#ifdef ACDBEXPPGL_DEBUG
	    cout << "CPGLCursor::CPGLCursor: executing describe method\n" ;
#endif
		CPGLColumnFactory::describe(_describeArgs);
	
	}
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::CPGLCursor: end\n" ;
#endif
}

CPGLCursor::~CPGLCursor () throw()
{
	if (_variables) delete[] ( ++ _variables ) ;
	if (_descriptions) delete[] ( ++ _descriptions ) ;
	if (_observers) delete[] _observers ;
	
	if (_res) PQclear(_res) ;
	_command->Release();
}

CPGLConnection * CPGLCursor::getConnection ()
{
	return _command->getConnection();
}

INT16 CPGLCursor::getColumnCount()
{
	return static_cast<INT16>(PQnfields(_res));
}

CPGLVariable& CPGLCursor::getColumn (INT16 number) 
{
	if (number > _variablesNumber) {
		char buffer[1024];
		sprintf(buffer, "Column %d is not available", int(number));
		throw CPGLDriverError(buffer);
	}
	return _variables[number];
}

CColumnDescription& CPGLCursor::getDescription (INT16 number)	/* IN: 1-relative */ 
{
	if (number > _variablesNumber) {
		char buffer[1024];
		sprintf(buffer, "Column %d is not available", int(number));
		throw CPGLDriverError(buffer);
	}
	return _descriptions[number];
}

void CPGLCursor::setErrorMessage (const char* errorMessage)
{
	CErrorHandlingImpl< CUnknownImpl<SQLCursor> >::setErrorMessage(errorMessage);
	_command->setErrorMessage(errorMessage);
}


// SQLCursor interface implementation

SQL_METHOD_IMP CPGLCursor::setOption (eSQLCursorOption eCurOption, INT32 lValue)
{
	return DBXERR_NOTSUPPORTED;
}

SQL_METHOD_IMP CPGLCursor::getOption (eSQLCursorOption eCurOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength)
{
	return DBXERR_NOTSUPPORTED;
}
#ifdef __x86_64__

SQL_METHOD_IMP CPGLCursor::setOption (eSQLCursorOption eCurOption, INT64 lValue)
{
	return DBXERR_NOTSUPPORTED;
}

SQL_METHOD_IMP CPGLCursor::getOption (eSQLCursorOption eCurOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength)
{
	return DBXERR_NOTSUPPORTED;
}
#endif


SQL_METHOD_IMP CPGLCursor::getColumnCount (pUINT16 puColumns) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getColumnCount: about to call internal\n" ;
#endif
	*puColumns = getColumnCount();
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getColumnCount: internal getColumnCount returned: " << *puColumns << "\n" ;
#endif
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnNameLength (UINT16 uColumnNumber, pUINT16 puLen) SAFE_SQL_METHOD
{
	*puLen = getDescription(uColumnNumber).getNameLength();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnName (UINT16 uColumnNumber, pCHAR pColumnName) SAFE_SQL_METHOD
{
	strcpy(pColumnName, getDescription(uColumnNumber).getName());
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnType (UINT16 uColumnNumber, pUINT16 puLogType, pUINT16 puSubType) SAFE_SQL_METHOD
{
	CColumnDescription& description = getDescription(uColumnNumber);
	*puLogType = description.getDataType().logType;
	*puSubType = description.getDataType().subType;
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnLength (UINT16 uColumnNumber, pUINT32 pulLength) SAFE_SQL_METHOD
{

	*pulLength = getDescription(uColumnNumber).getLength();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnPrecision (UINT16 uColumnNumber, pINT16 piPrecision) SAFE_SQL_METHOD
{
	*piPrecision = getDescription(uColumnNumber).getPrecision();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getColumnScale (UINT16 uColumnNumber, pINT16 piScale) SAFE_SQL_METHOD
{
	*piScale = getDescription(uColumnNumber).getScale();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::isNullable (UINT16 uColumnNumber, pBOOL pbNullable) SAFE_SQL_METHOD
{
	*pbNullable = getDescription(uColumnNumber).isNullable();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::isAutoIncrement (UINT16 uColumnNumber, pBOOL pbAutoIncrement) SAFE_SQL_METHOD
{
	*pbAutoIncrement = getDescription(uColumnNumber).isAutoIncrement();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::isReadOnly (UINT16 uColumnNumber, pBOOL pbReadOnly) SAFE_SQL_METHOD
{
	*pbReadOnly = getDescription(uColumnNumber).isReadOnly();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::isSearchable (UINT16 uColumnNumber, pBOOL pbSearchable) SAFE_SQL_METHOD
{
	*pbSearchable = getDescription(uColumnNumber).isSearchable();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::isBlobSizeExact (UINT16 uColumnNumber, pBOOL pbBlobExactSize) SAFE_SQL_METHOD
{
	*pbBlobExactSize = getDescription(uColumnNumber).isBlobSizeExact();
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::next () SAFE_SQL_METHOD
{
	
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::next: " << getColumnCount() << " - current row *before increment* " << _currentRow << "\n" ;
#endif
    
	_currentRow++ ;
    
    if ( (_currentRow+1) > PQntuples(_res) ) 
    {	
        return DBXERR_EOF ;
	};


	CPGLColumnFactory::CreateValueArgs createValueArgs;

	CPGLVariable *variable = _variables;
    
    CColumnDescription *description = _descriptions;

	for (int i = 0; i < getColumnCount(); i++)
	{
#ifdef ACDBEXPPGL_DEBUG
	    cout << "CPGLCursor::next: executing initialize method\n" ;
#endif

   
		(++variable)->initialize(_res, _currentRow, i); /* REQ'D IN ORDER TO STORE THE ACTUAL colLength */

        _describeArgs.type = variable->getType();
        _describeArgs.length = createValueArgs.length = variable->getLength();	
		_describeArgs.name = variable->getName();
		_describeArgs.scale = createValueArgs.scale = variable->getScale();
		_describeArgs.xid = variable->getXID();
		_describeArgs.autoIncrement = variable->getAutoIncrement();
		_describeArgs.isNullable = variable->getNullable();
		_describeArgs.isReadOnly = variable->getReadOnly();
		_describeArgs.isSearchable = variable->getSearchable();
		_describeArgs.isBlobSizeExact = variable->getBlobSizeExact();
		_describeArgs.precision = createValueArgs.precision = variable->getPrecision();
		_describeArgs.description = ++description;	

// Perform server + type specific overrides:
		{
			const INT32 type_DATETIME = 1114 ;
			if ( (_serverVersion >= 90000) && (_describeArgs.type == type_DATETIME) )
			{
				/* Postgres version >= 9 stores DateTimes as integer, with fraction of 6 digits */
				_describeArgs.scale = createValueArgs.scale = 6 ;
			} else if ( (_serverVersion < 90000) && (_describeArgs.type == type_DATETIME) )
			{
				/* Postgres version < 9 store DateTimes as double */
				_describeArgs.scale = createValueArgs.scale = -1 ;
			}
		}


#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLCursor::next: executing describe method\n" ;
#endif
        CPGLColumnFactory::describe(_describeArgs);	
    
#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLCursor::next: executing createValue method\n" ;
#endif
        CPGLAbstractValue * tmp = _describeArgs.createValue(createValueArgs) ;	

#ifdef ACDBEXPPGL_DEBUG
		cout << "CPGLCursor::next: executing setValue method\n" ;
#endif
        variable->setValue(tmp, _res, _currentRow, i );

	}	/* for i */ 



	TRACE_MESSAGE(getConnection()->getCallback())(traceSTMT, "next");

	notify();

	
#ifdef ACDBEXPPGL_DEBUG
	    cout << "CPGLCursor::next method finished\n" ;
#endif

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getString (UINT16 uColumnNumber, pCHAR pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
/*
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getString column: " << uColumnNumber << "\n" ;
#endif
*/
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }

        variable.getValue()->asString(pData);
    }

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getShort (UINT16 uColumnNumber, pINT16 pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
/*
#ifdef ACDBEXPPGL_DEBUG
	count << "CPGLCursor::getShort column: " <<  uColumnNumber << "\n";
#endif
*/
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }
		if (variable.getType() == CUSTOM_SQL_BOOL) 
			*pData = (*variable.getValue()->getData() == 't') ;
		else
			*pData = *reinterpret_cast<CPGLAbstractSimpleValue<INT16> *>(variable.getValue());
	}
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getLong (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
/*
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getLong column: " << uColumnNumber << "\n" ;
#endif
*/
	CPGLVariable& variable = getColumn(uColumnNumber);

	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }

        *pData = *reinterpret_cast<CPGLAbstractSimpleValue<INT32> *>(variable.getValue());
    }

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getDouble (UINT16 uColumnNumber, pDFLOAT pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
/*
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getDouble column: " << uColumnNumber << "\n";
#endif*/
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }

        *pData = variable.getValue()->asDouble();
    }
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getBcd (UINT16 uColumnNumber, pFMTBcd pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getBcd column: " << uColumnNumber << "\n" ;
#endif
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }

		*pData = variable.getValue()->asFMTBcd();
    }
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getTimeStamp (UINT16 uColumnNumber, pCTIMESTAMP pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
	//ACSNOTIMPLEMENTED("CPGLCursor::getTimeStamp: not implemented") ;
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getTimeStamp column: " <<  uColumnNumber << "\n" ;
#endif

	CPGLVariable& variable = getColumn(uColumnNumber);
    
	if (!(*pbIsNull = variable.isNull()))
    {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }

        *pData = *reinterpret_cast<pCTIMESTAMP> ((variable.getValue())->getData());


#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getTimeStamp returned: " 
         << setw(4) << setfill('0') << pData->year << "-"
         << setw(2) << setfill('0') << pData->month << "-"
         << setw(2) << setfill('0') << pData->day << " "
         << setw(2) << setfill('0') << pData->hour << ":"
         << setw(2) << setfill('0') << pData->minute << ":"
         << setw(2) << setfill('0') << pData->second << "."
         << pData->fraction << "\n" ; 
#endif

    }

	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getTime (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getTime column: " << uColumnNumber << "\n" ;
#endif
	return DBXERR_NOTSUPPORTED;
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getDate (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getDate column: " << uColumnNumber << "\n" ;
#endif
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }
		*pData = *reinterpret_cast<CPGLDateValue *>(variable.getValue());
    }
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getBytes(UINT16 uColumnNumber, pBYTE pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getBytes column: " << uColumnNumber << "\n" ;
#endif
	return DBXERR_NOTSUPPORTED;
	RETURN_SQL_RESULT;
}

SQL_METHOD_IMP CPGLCursor::getBlobSize(UINT16 uColumnNumber, pUINT32 pulLength, pBOOL pbIsNull) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getBlobSize column: " << uColumnNumber << "\n" ;
#endif
//	try {

		CPGLVariable& variable = getColumn(uColumnNumber);

		if (!(*pbIsNull = variable.isNull()))
#ifdef PGL_SHOW_WARNING
#warning MP - it should be tested fully
#endif

        *pulLength = variable.getBytes() ;


//			*pulLength = reinterpret_cast<CPGLAbstractLOValue *>(variable.getValue())->getBlobSize();
		return SQL_SUCCESS;
/*
	} catch (std::bad_alloc&) {
		return DBXERR_NOMEMORY;
	} catch (std::exception& e) {
		CErrorHandlingImpl< CUnknownImpl<SQLCursor> >::setErrorMessage(e.what());
		return SQL_ERROR;
	} catch (SQLResult sqlResult) {
		return sqlResult;
	} catch (...) {
		CErrorHandlingImpl< CUnknownImpl<SQLCursor> >::setErrorMessage("Unknown exception caught. File "__FILE__".");\
		return SQL_ERROR;
	}
*/
	RETURN_SQL_RESULT;

}

SQL_METHOD_IMP CPGLCursor::getBlob (UINT16 uColumnNumber, pVOID pData, pBOOL pbIsNull, UINT32 ulLength) SAFE_SQL_METHOD
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getBlob column: " << uColumnNumber << " length " <<  ulLength << "\n" ;
#endif
	TRACE_MESSAGE(getConnection()->getCallback())(traceBLOB, "get blob data from column=%d", uColumnNumber);
	
    CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull()))
	{
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }
        memcpy(pData, (variable.getValue())->getData(), ulLength);
#if(0)
        reinterpret_cast<CPGLAbstractLOValue *>(variable.getValue())->getBlobData(ulLength, pData);
#endif
	}

	RETURN_SQL_RESULT;
}

// Private methods
#ifdef PGL_SHOW_WARNING
#warning MP CPGLCursor::getHandle() temporary disabled 
#endif
/*
ifx_cursor_t * CPGLCursor::getHandle ()
{
	return _command->getHandle()->_SQCcursptr;
}
*/
// IFetchSubject interface implementation

void CPGLCursor::attach (Observer observer)
{
	_observers[_observerCount++] = observer;
}

void CPGLCursor::notify ()
{
	if (_observerCount != 0) {
		Observer *observer = _observers;
		for (int i = 0; i < _observerCount; i++)
			(*observer++)->update();
	}
}

SQL_METHOD_IMP CPGLTrimingCursor::getString(UINT16 uColumnNumber, pCHAR pData, pBOOL pbIsNull) SAFE_SQL_METHOD
{
/*
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLCursor::getString column: " << uColumnNumber  << "\n" ;
#endif
*/
	CPGLVariable& variable = getColumn(uColumnNumber);
	if (!(*pbIsNull = variable.isNull())) {
        if (0 == variable.getValue()) {
            setErrorMessage("no data");
		    return SQL_ERROR;
        }
		variable.getValue()->asString(pData);
		_cstr_t::trim(pData);
	}
	RETURN_SQL_RESULT;
}


/////////////////////////////////////////////////////////////////////////////
// CPGLColumnFactory


map <int, CPGLColumnFactory::DescribeFunc> CPGLColumnFactory::DescribeFuncTable ;
CPGLColumnFactory CPGLColumnFactory::_theColumnFactory ;

CPGLColumnFactory::CPGLColumnFactory() 
{
	initPGLLookup() ;
}

void CPGLColumnFactory::setGeographicOID(int oid) 
{
    if (oid > 0) {
        DescribeFuncTable[oid] = CPGLColumnFactory::describeUDTFixed ;	// geography 
#ifdef ACDBEXPPGL_DEBUG
        cout << "DescribeFuncTable[" << oid << "]: " << reinterpret_cast<void *>(DescribeFuncTable[oid]) << "\n" ;
#endif
    }
}

void CPGLColumnFactory::initPGLLookup() 
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::initPGLLookup\n" ;
#endif
	DescribeFuncTable[16]   = CPGLColumnFactory::describeBool ;
	DescribeFuncTable[17]   = CPGLColumnFactory::describeBytes ;    /* BYTEA */
	DescribeFuncTable[20]   = CPGLColumnFactory::describeInt8 ;     /* BIGINT */
	DescribeFuncTable[21]   = CPGLColumnFactory::describeSmallInt ; /* smallint */
	DescribeFuncTable[23]   = CPGLColumnFactory::describeInteger ;
	DescribeFuncTable[25]   = CPGLColumnFactory::describeText ; /* text */
	DescribeFuncTable[26]   = CPGLColumnFactory::describeBytes ;
	DescribeFuncTable[114]  = CPGLColumnFactory::describeJson ;	/* JSON */
	DescribeFuncTable[700]  = CPGLColumnFactory::describeSmallFloat ;  /* 4-bytes FP */
	DescribeFuncTable[701]  = CPGLColumnFactory::describeFloat ;       /* 8-bytes FP */

	DescribeFuncTable[1042] = CPGLColumnFactory::describeVarChar ;
	DescribeFuncTable[1043] = CPGLColumnFactory::describeVarChar ;
	DescribeFuncTable[1114] = CPGLColumnFactory::describeDateTime ;

    DescribeFuncTable[1186] = CPGLColumnFactory::describeVarChar ; /* INTERVAL */

	DescribeFuncTable[1700] = CPGLColumnFactory::describeInt8 ;		/* BIGINT */

	DescribeFuncTable[3802] = CPGLColumnFactory::describeJsonB ;	/* JSONB (Binary Json) */

#ifdef ACDBEXPPGL_DEBUG
    for (std::map<int, DescribeFunc>::iterator i = DescribeFuncTable.begin(); 
         i != DescribeFuncTable.end(); 
         ++i) {
        cout << "DescribeFuncTable[" << i->first << "]: " << reinterpret_cast<void *>(i->second) << "\n" ;
    }
#endif
}
	
CPGLAbstractValue * CPGLColumnFactory::DescribeArgs::createValue (
	const CPGLColumnFactory::CreateValueArgs& args) const
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::DescribeArgs::createValue: args.length: " << args.length << "\n" ;
#endif	
	if (createValueFunc != 0) {
		return createValueFunc(args);
    }
	char buffer[1024];
	sprintf(buffer, "Column '%s' is of unsupported data type [%d]", name, type);
#ifdef ACDBEXPPGL_DEBUG
	cout << buffer << "\n" ;
#endif	
	throw CPGLDriverError(buffer);
}


void CPGLColumnFactory::describe(CPGLColumnFactory::DescribeArgs& args)
{

#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describe: args.type: " << args.type << "\n" ;
#endif

    if (DescribeFuncTable.find(args.type) != DescribeFuncTable.end()) {
		return DescribeFuncTable[args.type](args);
    }
	else {
		return describeUnknown(args);
    }
}

void CPGLColumnFactory::describeUnknown (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeUnknown\n" ;
#endif
	args.description->describe(args.name, DataType(fldUNKNOWN, 0), args.length, 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = 0;
}
	
void CPGLColumnFactory::describeChar (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeChar: args.length " << args.length << "\n" ;
#endif
	args.description->describe(args.name, DataType(fldZSTRING, fldstFIXED), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createCharValue;
}

CPGLAbstractValue * CPGLColumnFactory::createCharValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createCharValue: not implemented") ;
#ifdef MARCO
	return new CPGLStringValue<SQL_C_CHAR>(args.length + 1);
#endif
//	return new CPGLStringValue<SQLCHAR>(args.length + 1);
}

void CPGLColumnFactory::describeSmallInt (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeSmallInt\n" ;
#endif
	args.description->describe(args.name, DataType(fldINT16, 0), sizeof(INT16), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createSmallIntValue;
}

CPGLAbstractValue * CPGLColumnFactory::createSmallIntValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLSimpleValue<INT16, fldINT16>();
}

void CPGLColumnFactory::describeInteger (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeInteger\n" ;
#endif
	args.description->describe(args.name, DataType(fldINT32, 0), sizeof(INT32), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createIntegerValue;
}

CPGLAbstractValue * CPGLColumnFactory::createIntegerValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLSimpleValue<INT32, fldINT32>() ;
}

void CPGLColumnFactory::describeFloat (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeFloat\n" ;
#endif
	args.description->describe(args.name, DataType(fldFLOAT, 0), sizeof(double), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createFloatValue;
}

CPGLAbstractValue * CPGLColumnFactory::createFloatValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLFloatValue<double, fldFLOAT>();
}

void CPGLColumnFactory::describeSmallFloat (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeSmallFloat\n" ;
#endif
	args.description->describe(args.name, DataType(fldFLOAT, 0), sizeof(float), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createSmallFloatValue;
}

CPGLAbstractValue * CPGLColumnFactory::createSmallFloatValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLFloatValue<float, fldFLOAT>();
}

void CPGLColumnFactory::describeDecimal (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeDecimal\n" ;
#endif
/*
	INT16 precision = static_cast<INT16>(args.length >> 8);
	INT16 scale = static_cast<INT16>(args.length & 0xff);
	if (scale == 0xff) {
		precision = FLOATING_DECIMAL_PRECISION;
		scale = FLOATING_DECIMAL_SCALE;
	}
	args.description->describe(args.name, args.mapping->decimal, sizeof(decimal), precision, scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createDecimalValue;
*/
	args.description->describe(args.name, args.mapping->decimal, args.length, args.precision, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createDecimalValue;
}

CPGLAbstractValue * CPGLColumnFactory::createDecimalValue (const CPGLColumnFactory::CreateValueArgs& args)
{
/*
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createDecimalValue: precision: " << args.precision 
         <<" scale " << args.scale << " length " << args.length << "\n" ;
#endif
*/
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createDecimalValue: not implemented") ;
#ifdef MARCO
	return new CPGLDecimalValue<SQL_DECIMAL>(args.precision, args.scale, args.length, args.fetchSubject);
#endif
}

void CPGLColumnFactory::describeSerial (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeSerial\n"  ;
#endif
	args.description->describe(args.name, DataType(fldINT32, fldstAUTOINC), sizeof(INT32), 0, args.scale, args.isNullable, -1, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createSerialValue;
}

CPGLAbstractValue * CPGLColumnFactory::createSerialValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLSimpleValue<INT32, SQLSERIAL>();
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createSerialValue TBI") ;
}

void CPGLColumnFactory::describeDate (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeDate\n" ;
#endif
	args.description->describe(args.name, DataType(fldDATE, 0), sizeof(INT32), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createDateValue;
}

CPGLAbstractValue * CPGLColumnFactory::createDateValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createDateValue\n" ;
#endif
	return new CPGLDateValue();
}

void CPGLColumnFactory::describeMoney (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeMoney\n" ;
#endif
/*
	args.description->describe(args.name, args.mapping->money, sizeof(decimal), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createMoneyValue;
*/
	ACSNOTIMPLEMENTED("CPGLColumnFactory::describeMoney TBI") ;
}

CPGLAbstractValue * CPGLColumnFactory::createMoneyValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLDecimalValue<SQLMONEY>(args.length >> 8, args.length & 0xff, args.fetchSubject);
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createMoneyValue TBI") ;
}

void CPGLColumnFactory::describeDateTime (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeDateTime\n" ;
#endif
	args.description->describe(args.name, DataType(fldDATETIME, 0), sizeof(CTIMESTAMP), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createDateTimeValue;
}

CPGLAbstractValue * CPGLColumnFactory::createDateTimeValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLDateTimeValue(args.scale);
}

void CPGLColumnFactory::describeBytes (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeBytes\n" ;
#endif
    args.description->describe(args.name, DataType(fldBLOB, fldstBINARY), sizeof(INT32), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createBytesValue;
}

CPGLAbstractValue * CPGLColumnFactory::createBytesValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout <<"CPGLColumnFactory::createBytesValue " << args.length << "\n";
#endif
	return new CPGLSimpleLOValue<fldBLOB>(args.length) ;
}

void CPGLColumnFactory::describeText (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeText\n" ;
#endif
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeText: length: " << args.length << "\n" ;
#endif
	args.description->describe(args.name, DataType(fldBLOB, fldstHMEMO), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createTextValue;
}

CPGLAbstractValue * CPGLColumnFactory::createTextValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLSimpleLOValue<SQLTEXT>(args.blobSize);
//	ACSNOTIMPLEMENTED("CPGLColumnFactory::createTextValue: not implemented") ;

//	return new CPGLSimpleLOValue<fldBLOB>(args.blobSize) ;

#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createTextValue: length+1: " << args.length+1 << "\n" ;
#endif
	return new CPGLStringValue<fldBLOB>(args.length + 1) ;
}

void CPGLColumnFactory::describeVarChar (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeVarChar: length: " << args.length << "\n" ;
#endif
	args.description->describe(args.name, DataType(fldZSTRING, 0), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createVarCharValue;
}

CPGLAbstractValue * CPGLColumnFactory::createVarCharValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createVarCharValue: length: " << args.length << "\n" ;
#endif
	return new CPGLStringValue<fldZSTRING>(args.length + 1);
}

void CPGLColumnFactory::describeJson (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeJson: length: " << args.length << "\n" ;
#endif
	args.description->describe(args.name, DataType(fldZSTRING, 0), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createJsonValue;
}

CPGLAbstractValue * CPGLColumnFactory::createJsonValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createJsonValue: length: " << args.length << "\n" ;
#endif
	return new CPGLJsonValue(args.length + 1);
}

void CPGLColumnFactory::describeJsonB (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeJsonB: length: " << args.length << "\n" ;
#endif
	args.description->describe(args.name, DataType(fldZSTRING, 0), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createJsonBValue;
}

CPGLAbstractValue * CPGLColumnFactory::createJsonBValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createJsonBValue: length: " << args.length << "\n" ;
#endif
	return new CPGLJsonBValue(args.length + 1); 
}

void CPGLColumnFactory::describeInterval (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeInterval\n" ;
#endif
/*
	args.description->describe(args.name, DataType(fldZSTRING, 0), INTERVAL_AS_VARCHAR_LENGTH, INTERVAL_AS_VARCHAR_LENGTH, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createIntervalValue;
*/
	ACSNOTIMPLEMENTED("CPGLColumnFactory::describeInterval TBI") ;
}

CPGLAbstractValue * CPGLColumnFactory::createIntervalValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLIntervalValue(static_cast<INT16>(args.length), args.fetchSubject);
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createIntervalValue TBI") ;
}

void CPGLColumnFactory::describeInt8 (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeInt8\n" ;
#endif
	args.description->describe(args.name, DataType(fldINT64, 0), sizeof(long long int), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createInt8Value;
}

CPGLAbstractValue * CPGLColumnFactory::createInt8Value (const CPGLColumnFactory::CreateValueArgs& args)
{
    return new CPGLInt8Value<fldINT64>() ;
}

void CPGLColumnFactory::describeSerial8 (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeSerial8\n" ;
#endif
/*
EC:: UNCOMMENT AND TEST THIS:
	args.description->describe(args.name, DataType(fldINT64, 0), sizeof(long long int), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createInt8Value;
*/
	ACSNOTIMPLEMENTED("CPGLColumnFactory::describeSerial8 TBI") ;
}

CPGLAbstractValue * CPGLColumnFactory::createSerial8Value (const CPGLColumnFactory::CreateValueArgs& args)
{
/*
EC:: UNCOMMENT AND TEST THIS:
    return new CPGLInt8Value<fldINT64>() ;
*/
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createSerial8Value TBI") ;
}

void CPGLColumnFactory::describeVarBinary(CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeVarBinary\n" ;
#endif
	describeLVarCharAsBlob(args);
}

void CPGLColumnFactory::describeUDTFixed(CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeUDTFixed args.name: " << args.name 
         << " args.xid " << args.xid
         << "\n" ;
#endif

	switch (args.xid)
	{
	case 1: 
        {
            args.description->describe(args.name, DataType(fldADT, fldstFMTMEMO), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
            args.createValueFunc = createUDTValue;
            break; 
        };

	default:
        {
            args.description->describe(args.name, DataType(fldADT, fldstBINARY), args.length, static_cast<INT16>(args.length), args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
            args.createValueFunc = createBytesValue;
            break; 
        };
	}
}

CPGLAbstractValue * CPGLColumnFactory::createUDTValue (const CPGLColumnFactory::CreateValueArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::createTextValue: length+1: " << args.length+1 << "\n" ;
#endif
	return new CPGLStringValue<fldADT>(args.length + 1) ;
}


CPGLAbstractValue * CPGLColumnFactory::createBlobValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLBLOBValue(args.fetchSubject);
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createBlobValue TBI") ;
}

CPGLAbstractValue * CPGLColumnFactory::createClobValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLCLOBValue(args.fetchSubject);
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createClobValue TBI") ;
}

void CPGLColumnFactory::describeLVarChar (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeLVarChar\n" ;
#endif
	if (args.mapping->lvarchar.logType == fldBLOB && args.mapping->lvarchar.subType == fldstFMTMEMO)
		describeLVarCharAsBlob(args);
	else
		describeLVarCharAsString(args);
}

void CPGLColumnFactory::describeLVarCharAsString (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeLVarCharAsString\n" ;
#endif
	args.description->describe(args.name, DataType(fldZSTRING, 0), args.length, static_cast<INT16>(args.length), 0, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createLVarCharValue;
}

void CPGLColumnFactory::describeLVarCharAsBlob (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeLVarCharAsBlob\n" ;
#endif
	args.description->describe(args.name, DataType(fldBLOB, fldstFMTMEMO), sizeof(void *), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createLVarCharValue;
}

CPGLAbstractValue * CPGLColumnFactory::createLVarCharValue (const CPGLColumnFactory::CreateValueArgs& args)
{
//	return new CPGLLVarCharValue();
	ACSNOTIMPLEMENTED("CPGLColumnFactory::createLVarCharValue TBI") ;
}

void CPGLColumnFactory::describeBool (CPGLColumnFactory::DescribeArgs& args)
{
#ifdef ACDBEXPPGL_DEBUG
	cout << "CPGLColumnFactory::describeBool\n" ;
#endif
	args.description->describe(args.name, DataType(fldBOOL, 0), sizeof(char), 0, args.scale, args.isNullable, args.autoIncrement, args.isReadOnly, args.isSearchable, args.isBlobSizeExact);
	args.createValueFunc = createBoolValue;
}

CPGLAbstractValue * CPGLColumnFactory::createBoolValue (const CPGLColumnFactory::CreateValueArgs& args)
{
	return new CPGLBooleanValue();
}
