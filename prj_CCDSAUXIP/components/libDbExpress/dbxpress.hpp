
/*
 *  dbxpress.hpp - dbExpress types, constants and interfaces
 *
 *    Copyright (c) 2001-2004, Luxena Software Company. All rights reserved.
 *
 *  Purpose:
 *
 */
#ifndef _dbxpress_hpp_
#define _dbxpress_hpp_ 


/* modified by MP on 11/08/2005 to avoid too many warnings */
/* #pragma once */

#ifndef __cplusplus
#error This header requires a C++ compiler ...
#endif


#include <sys/types.h>	



/////////////////////////////////////////////////////////////////////////////
// Types definitions


typedef void *pVOID;  // NOSONAR - old style 
typedef int BOOL, *pBOOL; // NOSONAR - old style
typedef char CHAR, *pCHAR; // NOSONAR - old style
typedef unsigned char BYTE, *pBYTE; // NOSONAR - old style
typedef __int16_t INT16, *pINT16; // NOSONAR - old style
typedef __uint16_t UINT16, *pUINT16; // NOSONAR - old style
//typedef int INT32, *pINT32;
//typedef unsigned int UINT32, *pUINT32;
typedef __int32_t INT32 ; // NOSONAR - old style
typedef __int32_t *pINT32; // NOSONAR - old style

typedef __uint32_t UINT32, *pUINT32; // NOSONAR - old style
typedef __int64_t INT64; // NOSONAR - old style
typedef __int64_t *pINT64; // NOSONAR - old style
typedef __uint64_t UINT64, *pUINT64; // NOSONAR - old style
typedef float FLOAT, *pFLOAT; // NOSONAR - old style
typedef double DFLOAT, *pDFLOAT; // NOSONAR - old style


#pragma pack(push, before_typedef, 1)   
typedef struct CTIMESTAMP  // NOSONAR - old style
{
	INT16 year;
	UINT16 month;
	UINT16 day;
	UINT16 hour;
	UINT16 minute;
	UINT16 second;
	UINT32 fraction;
} *pCTIMESTAMP;

typedef struct FMTBcd // NOSONAR - old style
{
  BYTE iPrecision;
  BYTE iSignSpecialPlaces;
  BYTE iFraction[32];       // NOSONAR - format is a POD struct packed
} *pFMTBcd;
#pragma pack(pop, before_typedef)




/////////////////////////////////////////////////////////////////////////////
// Macros definitions


#ifdef __x86_64__
        #define ACS_PTRASINT_T int64_t
        #define ACS_PTRASUINT_T uint64_t
#else
        #define ACS_PTRASINT_T int32_t
        #define ACS_PTRASUINT_T uint32_t
#endif

/////////////////////////////////////////////////////////////////////////////
// Result type and result constants

typedef INT32 SQLResult; // NOSONAR - old style

const SQLResult SQL_ERROR               = -1;
const SQLResult SQL_NULL_DATA           = 100;

const SQLResult SQL_SUCCESS             = 0;
const SQLResult DBXERR_NOMEMORY         = 1;
const SQLResult DBXERR_INVALIDFLDTYPE   = 2;
const SQLResult DBXERR_INVALIDHNDL      = 3;
const SQLResult DBXERR_INVALIDTIME      = 4;
const SQLResult DBXERR_NOTSUPPORTED     = 5;
const SQLResult DBXERR_INVALIDXLATION   = 6;
const SQLResult DBXERR_INVALIDPARAM     = 7;
const SQLResult DBXERR_OUTOFRANGE       = 8;
const SQLResult DBXERR_SQLPARAMNOTSET   = 9;
const SQLResult DBXERR_EOF              = 10;
const SQLResult DBXERR_INVALIDUSRPASS   = 11;
const SQLResult DBXERR_INVALIDPRECISION = 12;
const SQLResult DBXERR_INVALIDLEN       = 13;
const SQLResult DBXERR_INVALIDXISOLEVEL = 14;
const SQLResult DBXERR_INVALIDXTXNID    = 15;
const SQLResult DBXERR_DUPLICATETXNID   = 16;
const SQLResult DBXERR_DRIVERRESTRICTED = 17;
const SQLResult DBX_MAXSTATICERRORS     = 18;

const int MAX_RESERVED_STATIC_ERRORS    = 255;


/////////////////////////////////////////////////////////////////////////////
// Constants for date conversions

// Days between 1/1/0001 and 01/01/1900
const int DATE_DELTA = 693595;

// Milliseconds in day
const int DAY_MSECS = 24*60*60*1000;


/////////////////////////////////////////////////////////////////////////////
// Data Types (Logical)

struct DataType { 
	DataType () = default ;
	DataType (UINT16 ltype, UINT16 stype) : logType(ltype), subType(stype)  {}
	UINT16 logType = 0 ; 
	UINT16 subType = 0 ;
};



const UINT16 fldUNKNOWN         = 0;
const UINT16 fldZSTRING         = 1;               // Null terminated string
const UINT16 fldDATE            = 2;               // Date     (32 bit)
const UINT16 fldBLOB            = 3;               // Blob
const UINT16 fldBOOL            = 4;               // Boolean  (16 bit)
const UINT16 fldINT16           = 5;               // 16 bit signed number
const UINT16 fldINT32           = 6;               // 32 bit signed number
const UINT16 fldFLOAT           = 7;               // 64 bit floating point
const UINT16 fldBCD             = 8;               // BCD
const UINT16 fldBYTES           = 9;               // Fixed number of bytes
const UINT16 fldTIME            = 10;              // Time        (32 bit)
const UINT16 fldTIMESTAMP       = 11;              // Time-stamp  (64 bit)
const UINT16 fldUINT16          = 12;              // Unsigned 16 bit Integer
const UINT16 fldUINT32          = 13;              // Unsigned 32 bit Integer
const UINT16 fldFLOATIEEE       = 14;              // 80-bit IEEE float
const UINT16 fldVARBYTES        = 15;              // Length prefixed var bytes
const UINT16 fldLOCKINFO        = 16;              // Look for LOCKINFO typedef
const UINT16 fldCURSOR          = 17;              // For Oracle Cursor type
const UINT16 fldINT64           = 18;              // 64 bit signed number
const UINT16 fldUINT64          = 19;              // Unsigned 64 bit Integer
const UINT16 fldADT             = 20;              // Abstract datatype (structure)
const UINT16 fldARRAY           = 21;              // Array field type
const UINT16 fldREF             = 22;              // Reference to ADT
const UINT16 fldTABLE           = 23;              // Nested table (reference)
const UINT16 fldDATETIME        = 24;              // DateTime structure field
const UINT16 fldFMTBCD          = 25;              // BCD Variant type: required by Midas, same as BCD for DBExpres

const int MAX_LOG_FLD_TYPES     = 26;              // Number of logical field types

// Sub Types

// fldFLOAT subtype
const UINT16 fldstMONEY          = 21;              // Money
// fldBLOB subtypes
const UINT16 fldstMEMO           = 22;              // Text Memo
const UINT16 fldstBINARY         = 23;              // Binary data
const UINT16 fldstFMTMEMO        = 24;              // Formatted Text
const UINT16 fldstOLEOBJ         = 25;              // OLE object (Paradox)
const UINT16 fldstGRAPHIC        = 26;              // Graphics object
const UINT16 fldstDBSOLEOBJ      = 27;              // dBASE OLE object
const UINT16 fldstTYPEDBINARY    = 28;              // Typed Binary data
const UINT16 fldstACCOLEOBJ      = 30;              // Access OLE object
const UINT16 fldstHMEMO          = 33;              // CLOB
const UINT16 fldstHBINARY        = 34;              // BLOB
const UINT16 fldstBFILE          = 36;              // BFILE
// fldZSTRING subtype
const UINT16 fldstPASSWORD       = 1;               // Password
const UINT16 fldstFIXED          = 31;              // CHAR type
const UINT16 fldstUNICODE        = 32;              // Unicode
// fldINT32 subtype
const UINT16 fldstAUTOINC        = 29;
// fldADT subtype
const UINT16 fldstADTNestedTable = 35;             // ADT for nested table (has no name)
// fldDATE subtype
const UINT16 fldstADTDATE        = 37;              // DATE (OCIDate) with in an ADT


const int MAX_COLUMN_NAME_LENGTH = 64;



/////////////////////////////////////////////////////////////////////////////
// Forward declarations

typedef struct SQLDriver **ppSQLDriver; // NOSONAR - old style
typedef struct SQLConnection **ppSQLConnection; // NOSONAR - old style
typedef struct SQLCommand **ppSQLCommand; // NOSONAR - old style
typedef struct SQLCursor **ppSQLCursor; // NOSONAR - old style
typedef struct SQLMetaData **ppSQLMetaData; // NOSONAR - old style


/////////////////////////////////////////////////////////////////////////////
// Method header macroses (compiler dependent)

#ifdef _MSC_VER
 #define com_interface
#elif __GNUC__
 #ifndef __x86_64__
  #define __stdcall __attribute__((stdcall))
  #define com_interface __attribute__((com_interface))
 #else
  #define __stdcall
  #define com_interface __attribute__((com_interface))
 #endif
#endif

#define API_CALL_TYPE __stdcall
#define COM_METHOD_CALL_TYPE __stdcall

#define COM_METHOD(type) virtual type COM_METHOD_CALL_TYPE
#define COM_METHOD_IMP(type) type COM_METHOD_CALL_TYPE

#define SQL_METHOD COM_METHOD(SQLResult)
#define SQL_METHOD_IMP COM_METHOD_IMP(SQLResult)


/////////////////////////////////////////////////////////////////////////////
// SQLUnknown interface

struct SQLUnknown   
{
    SQLUnknown() = default ; 
	virtual ~SQLUnknown() = default ; 
    
#pragma pack(push, before_typedef, 1)
	struct GUID
	{
		UINT32 Data1;
		UINT16 Data2;
		UINT16 Data3;
		BYTE Data4[8];  // NOSONAR - packed struct 
	};
#pragma pack(pop, before_typedef)

	COM_METHOD(INT32) QueryInterface (const GUID& riid, void **ppvObject) = 0; // NOSONAR - any pointer
	COM_METHOD(UINT32) AddRef (void) = 0;
	COM_METHOD(UINT32) Release (void) = 0;
protected:
    SQLUnknown(const SQLUnknown &)  = default ; 
    SQLUnknown& operator= (const SQLUnknown &) = default ; 
} ;


/////////////////////////////////////////////////////////////////////////////
// Trace callback

enum CBType // NOSONAR - plain enum
{
	cbGENERAL,                          // General purpose
	cbRESERVED1,
	cbRESERVED2,
	cbINPUTREQ,                         // Input requested
	cbRESERVED4,
	cbRESERVED5,
	cbBATCHRESULT,                      // Batch processing results
	cbRESERVED7,
	cbRESTRUCTURE,                      // Restructure
	cbRESERVED9,
	cbRESERVED10,
	cbRESERVED11,
	cbRESERVED12,
	cbRESERVED13,
	cbRESERVED14,
	cbRESERVED15,
	cbRESERVED16,
	cbRESERVED17,
	cbTABLECHANGED,                     // Table changed notification
	cbRESERVED19,
	cbCANCELQRY,                        // Allow user to cancel Query
	cbSERVERCALL,                       // Server Call
	cbRESERVED22,
	cbGENPROGRESS,                      // Generic Progress report.
	cbDBASELOGIN,                       // dBASE Login
	cbDELAYEDUPD,                       // Delayed Updates
	cbFIELDRECALC,                      // Field(s) recalculation
	cbTRACE,                            // Trace
	cbDBLOGIN,                          // Database login
	cbDETACHNOTIFY,                     // DLL Detach Notification
	cbNBROFCBS                          // Number of cbs
};

enum CBRType // NOSONAR - plain enum
{
	cbrUSEDEF,
	cbrCONTINUE,
	cbrABORT,
	cbrCHKINPUT,
	cbrYES,
	cbrNO,
	cbrPARTIALASSIST,
	cbrSKIP,
	cbrRETRY,
	cbrNOHANDLER
};

enum TRACECat // NOSONAR - plain enum
{
	trUNKNOWN     = 0x0000,
	traceQPREPARE = 0x0001,
	traceQEXECUTE = 0x0002,
	traceERROR    = 0x0004,
	traceSTMT     = 0x0008,
	traceCONNECT  = 0x0010,
	traceTRANSACT = 0x0020,
	traceBLOB     = 0x0040,
	traceMISC     = 0x0080,
	traceVENDOR   = 0x0100,
	traceDATAIN   = 0x0200,
	traceDATAOUT  = 0x0400
};

#pragma pack(push, before_typedef, 1)
typedef struct TRACEDesc // NOSONAR - old style
{
	CHAR szTrace[1024]; // NOSONAR - packed struct
	TRACECat eTraceCat;
	INT32 iCBInfo;
	UINT16 uTotalMsgLen;
} *pTRACEDesc;
#pragma pack(pop, before_typedef)

typedef CBRType (API_CALL_TYPE * pfCallBack) (CBType ecbType, pVOID pCbInfo); // NOSONAR - old style


/////////////////////////////////////////////////////////////////////////////
// SQLDriver interface

enum eSQLDriverOption // NOSONAR - plain enum
{
	eDrvBlobSize     = 0,
	eDrvCallBack     = 1,
	eDrvCallBackInfo = 2,
	eDrvRestrict     = 3
};

struct SQLDriver : public SQLUnknown 
{
    SQLDriver() noexcept = default ;
	~SQLDriver() override = default ; 
    SQLDriver(const SQLDriver &r) = delete ; 
    SQLDriver& operator= (const SQLDriver &r) = delete ; 

	SQL_METHOD getSQLConnection (ppSQLConnection ppConn) = 0;
	SQL_METHOD setOption (eSQLDriverOption eDOption, INT32 PropValue) = 0;
	SQL_METHOD getOption (eSQLDriverOption eDOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLDriverOption eDOption, INT64 PropValue) = 0;
	SQL_METHOD getOption (eSQLDriverOption eDOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#endif
} ;


/////////////////////////////////////////////////////////////////////////////
// SQLConnection interface

enum eSQLConnectOption // NOSONAR - plain enum
{
	// Delphi 6
	eConnAutoCommit          = 0,
	eConnBlockingMode        = 1,
	eConnBlobSize            = 2,
	eConnRoleName            = 3,
	eConnWaitOnLocks         = 4,
	eConnCommitRetain        = 5,
	eConnTxnIsoLevel         = 6,
	eConnNativeHandle        = 7,
	eConnServerVersion       = 8,
	eConnCallBack            = 9,
	eConnHostName            = 10,
	eConnDatabaseName        = 11,
	eConnCallBackInfo        = 12,
	eConnObjectMode          = 13,
	eConnMaxActiveComm       = 14,
	eConnServerCharSet       = 15,
	eConnSqlDialect          = 16,
	// Delphi 7
	eConnRollbackRetain      = 17,
	eConnObjectQuoteChar     = 18,
	eConnConnectionName      = 19,
	eConnOSAuthentication    = 20,
	eConnSupportsTransaction = 21,
	eConnMultipleTransaction = 22,
	eConnServerPort          = 23,
	eConnOnLine              = 24,
	eConnTrimChar            = 25,
	eConnQualifiedName       = 26,
	eConnCatalogName         = 27,
	eConnSchemaName          = 28,
	eConnObjectName          = 29,
	eConnQuotedObjectName    = 30,
	eConnCustomInfo          = 31,	// returns the pointer to the PGconn (pgl connection)
	eConnTimeOut             = 32,
    eConnErrorSql            = 33, 
    eConnErrorInternal       = 34 
};

enum eXILType // NOSONAR - plain enum
{
	xilREADCOMMITTED,
	xilREPEATABLEREAD,
	xilDIRTYREAD,
	xilCUSTOM
};

typedef struct TransactionDesc // NOSONAR - old style
{
	UINT32 uTransID;
	UINT32 uGID;
	eXILType eTransIsoLevel;
	UINT32 uCustomIsolation;
} *pTransactionDesc;

struct SQLConnection : public SQLUnknown    
{
    SQLConnection() noexcept = default ;
	~SQLConnection() override = default ; 
    SQLConnection(const SQLConnection &r) = delete ; 
    SQLConnection& operator= (const SQLConnection &r) = delete ; 
    
	SQL_METHOD connect (pCHAR pszServerName, pCHAR pszUserName, pCHAR pszPassword) = 0; 
	SQL_METHOD disconnect () = 0;
	SQL_METHOD getSQLCommand (ppSQLCommand ppComm) = 0;
	SQL_METHOD getSQLMetaData (ppSQLMetaData ppMeta) = 0;
	SQL_METHOD setOption (eSQLConnectOption eCOption, INT32 lValue) = 0;
	SQL_METHOD getOption (eSQLConnectOption eCOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLConnectOption eCOption, INT64 lValue) = 0;
	SQL_METHOD getOption (eSQLConnectOption eCOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#endif
	SQL_METHOD beginTransaction (UINT32 ulTransDesc) = 0;
	SQL_METHOD commit (UINT32 ulTransDesc) = 0;
	SQL_METHOD rollback (UINT32 ulTransDesc) = 0;
	SQL_METHOD getErrorMessage (pBYTE pszError) = 0;
	SQL_METHOD getErrorMessageLen (pUINT16 puErrorLen) = 0;

} ;


/////////////////////////////////////////////////////////////////////////////
// SQLCommand interface

enum eSQLCommandOption // NOSONAR - plain enum
{
	// Delphi 6
	eCommRowsetSize       = 0,
	eCommBlobSize         = 1,
	eCommBlockRead        = 2,
	eCommBlockWrite       = 3,
	eCommParamCount       = 4,
	eCommNativeHandle     = 5,
	eCommCursorName       = 6,
	eCommStoredProc       = 7,
	eCommSQLDialect       = 8,
	eCommTransactionID    = 9,
	// Delphi 7
	eCommPackageName      = 10,
	eCommTrimChar         = 11,
	eCommQualifiedName    = 12,
	eCommCatalogName      = 13,
	eCommSchemaName       = 14,
	eCommObjectName       = 15,
	eCommQuotedObjectName = 16,
    eCommErrorSql         = 17,
    eCommErrorInternal    = 18
};

enum STMTParamType // NOSONAR - plain enum
{
	paramUNKNOWN = 0,
	paramIN      = 1,
	paramOUT     = 2,
	paramINOUT   = 3,
	paramRET     = 4
};

struct SQLCommand : public SQLUnknown   
{
    SQLCommand() noexcept = default ;
	~SQLCommand() override = default ; 
    SQLCommand(const SQLCommand &) = delete ; 
    SQLCommand& operator= (const SQLCommand &) = delete ; 

	SQL_METHOD setOption (eSQLCommandOption eSOption, INT32 lValue) = 0;
	SQL_METHOD getOption (eSQLCommandOption eSOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLCommandOption eSOption, INT64 lValue) = 0;
	SQL_METHOD getOption (eSQLCommandOption eSOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#endif
	SQL_METHOD setParameter (UINT16 uParameterNumber, UINT16 uChildPos, STMTParamType ePType, UINT16 uLogType, UINT16 uSubType, INT32 lMaxPrecision, INT32 lMaxScale, UINT32 ulLength, pVOID pBuffer, BOOL bIsNull) = 0; // NOSONAR 
	SQL_METHOD getParameter (UINT16 uParameterNumber, UINT16 uChildPos, pVOID pData, UINT32 ulLength, pINT32 plInd) = 0; // NOSONAR
	SQL_METHOD prepare (pCHAR pszSQL, UINT16 uParamCount) = 0;
	SQL_METHOD execute (ppSQLCursor ppCur) = 0;
	SQL_METHOD executeImmediate (pCHAR pszSQL, ppSQLCursor ppCur) = 0;
	SQL_METHOD getNextCursor (ppSQLCursor ppCur) = 0;
	SQL_METHOD getRowsAffected (pINT32 plRows) = 0;
	SQL_METHOD close () = 0;        
	SQL_METHOD getErrorMessage (pBYTE pszError) = 0;
	SQL_METHOD getErrorMessageLen (pUINT16 puErrorLen) = 0;
} ;


/////////////////////////////////////////////////////////////////////////////
// SQLCursor interface

enum eSQLCursorOption // NOSONAR - plain enum
{
	eCurObjectAttrName = 0,
	eCurObjectTypeName = 1,
	eCurParentFieldID  = 2
};

struct SQLCursor : public SQLUnknown   
{
    SQLCursor() noexcept = default ;
	~SQLCursor() override = default ;
    SQLCursor(const SQLCursor &) = delete ; 
    SQLCursor& operator= (const SQLCursor &) = delete ; 

	SQL_METHOD setOption (eSQLCursorOption eCurOption, INT32 lValue) = 0;
	SQL_METHOD getOption (eSQLCursorOption eCurOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLCursorOption eCurOption, INT64 lValue) = 0;
	SQL_METHOD getOption (eSQLCursorOption eCurOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#endif
	SQL_METHOD getErrorMessage (pBYTE pszError) = 0;
	SQL_METHOD getErrorMessageLen (pUINT16 puErrorLen) = 0;
	SQL_METHOD getColumnCount (pUINT16 puColumns) = 0;
	SQL_METHOD getColumnNameLength (UINT16 uColumnNumber, pUINT16 puLen) = 0;
	SQL_METHOD getColumnName (UINT16 uColumnNumber, pCHAR pColumnName) = 0;
	SQL_METHOD getColumnType (UINT16 uColumnNumber, pUINT16 puLogType, pUINT16 puSubType) = 0;
	SQL_METHOD getColumnLength (UINT16 uColumnNumber, pUINT32 pulLength) = 0;
	SQL_METHOD getColumnPrecision (UINT16 uColumnNumber, pINT16 piPrecision) = 0;
	SQL_METHOD getColumnScale (UINT16 uColumnNumber, pINT16 piScale) = 0;
	SQL_METHOD isNullable (UINT16 uColumnNumber, pBOOL pbNullable) = 0;
	SQL_METHOD isAutoIncrement (UINT16 uColumnNumber, pBOOL pbAutoIncrement) = 0;
	SQL_METHOD isReadOnly (UINT16 uColumnNumber, pBOOL pbReadOnly) = 0;
	SQL_METHOD isSearchable (UINT16 uColumnNumber, pBOOL pbSearchable) = 0;
	SQL_METHOD isBlobSizeExact (UINT16 uColumnNumber, pBOOL pbBlobExactSize) = 0;
	SQL_METHOD next () = 0;
	SQL_METHOD getString (UINT16 uColumnNumber, pCHAR pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getShort (UINT16 uColumnNumber, pINT16 pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getLong (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getDouble (UINT16 uColumnNumber, pDFLOAT pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getBcd (UINT16 uColumnNumber, pFMTBcd pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getTimeStamp (UINT16 uColumnNumber, pCTIMESTAMP pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getTime (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getDate (UINT16 uColumnNumber, pINT32 pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getBytes (UINT16 uColumnNumber, pBYTE pData, pBOOL pbIsNull) = 0;
	SQL_METHOD getBlobSize (UINT16 uColumnNumber, pUINT32 pulLength, pBOOL pbIsNull) = 0;
	SQL_METHOD getBlob (UINT16 uColumnNumber, pVOID pData, pBOOL pbIsNull, UINT32 ulLength) = 0; // NOSONAR
} ;


/////////////////////////////////////////////////////////////////////////////
// SQLMetaData interface

enum eSQLMetaDataOption // NOSONAR - plain enum
{
	// Delphi 6
	eMetaCatalogName          = 0,
	eMetaSchemaName           = 1,
	eMetaDatabaseName         = 2,
	eMetaDatabaseVersion      = 3,
	eMetaTransactionIsoLevel  = 4,
	eMetaSupportsTransaction  = 5,
	eMetaMaxObjectNameLength  = 6,
	eMetaMaxColumnsInTable    = 7,
	eMetaMaxColumnsInSelect   = 8,
	eMetaMaxRowSize           = 9,
	eMetaMaxSQLLength         = 10,
	eMetaObjectQuoteChar      = 11,
	eMetaSQLEscapeChar        = 12,
	eMetaProcSupportsCursor   = 13,
	eMetaProcSupportsCursors  = 14,
	eMetaSupportsTransactions = 15,
	// Delphi 7
	eMetaPackageName          = 16
};

enum eSQLObjectType // NOSONAR - plain enum
{
	// Delphi 6
	eObjTypeDatabase  = 0,
	eObjTypeDataType  = 1,
	eObjTypeTable     = 2,
	eObjTypeView      = 3,
	eObjTypeSynonym   = 4,
	eObjTypeProcedure = 5,
	eObjTypeUser      = 6,
	eObjTypeRole      = 7,
	eObjTypeUDT       = 8,
	// Delphi 7
	eObjTypePackage   = 9
};

enum eSQLTableType // NOSONAR - plain enum
{
	eSQLTable       = 0x0001,
	eSQLView        = 0x0002,
	eSQLSystemTable = 0x0004,
	eSQLSynonym     = 0x0008,
	eSQLTempTable   = 0x0010,
	eSQLLocal       = 0x0020
};

enum eSQLProcType // NOSONAR - plain enum
{
	eSQLProcedure    = 0x0001,
	eSQLFunction     = 0x0002,
	eSQLPackage      = 0x0004,
	eSQLSysProcedure = 0x0008
};

enum eSQLColType // NOSONAR - plain enum
{
	eSQLRowId      = 0x0001,
	eSQLRowVersion = 0x0002,
	eSQLAutoIncr   = 0x0004,
	eSQLDefault    = 0x0008
};

enum eSQLIndexType // NOSONAR - plain enum
{
	eSQLNonUnique  = 0x0001,
	eSQLUnique     = 0x0002,
	eSQLPrimaryKey = 0x0004
};

struct SQLMetaData : public SQLUnknown   
{
    SQLMetaData() noexcept = default ;
	~SQLMetaData() override = default ; 
    SQLMetaData(const SQLMetaData &) = delete ; 
    SQLMetaData& operator= (const SQLMetaData &) = delete ;

	SQL_METHOD setOption (eSQLMetaDataOption eDOption, INT32 lValue) = 0;
	SQL_METHOD getOption (eSQLMetaDataOption eDOption, pINT32 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#ifdef __x86_64__
	SQL_METHOD setOption (eSQLMetaDataOption eDOption, INT64 lValue) = 0;
	SQL_METHOD getOption (eSQLMetaDataOption eDOption, pINT64 plValue, INT16 iMaxLength, pINT16 piLength) = 0;
#endif
	SQL_METHOD getObjectList (eSQLObjectType eObjType, ppSQLCursor ppCur) = 0;
	SQL_METHOD getTables (pCHAR pszTableName, UINT32 uTableType, ppSQLCursor ppCur) = 0;
	SQL_METHOD getProcedures (pCHAR pszProcName, UINT32 uProcType, ppSQLCursor ppCur) = 0;
	SQL_METHOD getColumns (pCHAR pszTableName, pCHAR pszColumnName, UINT32 uColType, ppSQLCursor ppCur) = 0;
	SQL_METHOD getProcedureParams (pCHAR pszProcName, pCHAR pszParamName, ppSQLCursor ppCur) = 0;
	SQL_METHOD getIndices (pCHAR pszTableName, UINT32 uIndexType, ppSQLCursor ppCur) = 0;
	SQL_METHOD getErrorMessage (pBYTE pszError) = 0;
	SQL_METHOD getErrorMessageLen (pUINT16 puErrorLen) = 0;
} ;
#endif // _dbxpress_hpp_
