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

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$
	
	$Log$
	Revision 5.6  2017/11/21 18:15:23  marpas
	diagnostic message fixed
	
	Revision 5.5  2017/11/20 13:59:27  marpas
	debug message would cause a crash - fixed
	
	Revision 5.4  2016/07/28 09:08:15  marpas
	added ADT support because was missing - problem rose when Text were no longer be readable as strings but GeoObjects are blobs
	
	Revision 5.3  2014/02/07 18:36:53  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.2  2013/12/19 12:24:35  marpas
	Transaction class added
	
	Revision 5.1  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.0  2013/06/06 18:52:59  marpas
	adopting libException 5.x standards
	
	Revision 4.13  2013/05/14 18:12:52  marpas
	support for PostGRES/PostGIS geography type added
	
	Revision 4.12  2013/03/26 14:17:40  marpas
	regression fixed
	debug improved
	
	Revision 4.11  2013/03/26 00:58:19  marpas
	dbRow columns are accessed throu a uint16_t
	other interfaces modified accordingly
	
	Revision 4.10  2013/02/26 17:12:24  marpas
	coding best practice appplied
	interface rationalization
	
	Revision 4.9  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.8  2012/12/17 18:41:18  marpas
	MemoryGuard changed with acs::pattern::CMemoryScopeGuard
	
	Revision 4.7  2012/12/14 18:38:54  marpas
	coding best practices
	qa rules
	
	Revision 4.6  2012/11/19 13:30:21  marpas
	HEAD REALIGNEMENT
	
	Revision 4.4  2012/03/01 15:23:01  marpas
	introducing new db.conf path policy
	
	Revision 4.3  2012/02/17 13:55:15  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.1  2011/12/05 13:24:00  enrcar
	EC:: Debug improved. Informix9: Type dbExpress/fldINT64 is now handled (since it is returned by the new dbExpress.Googlecode for INT8 and SERIAL8);
	the signed long long is obtained directly via getBytes (instead of generating a temporary BCD).
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.4  2011/01/20 10:36:35  marpas
	empty statement removed
	
	Revision 3.3  2010/09/11 17:46:56  marpas
	adopts libDbExpress 3.2 interface
	adopta libDbException 3.3 interface
	
	Revision 3.2  2010/03/18 15:17:49  enrcar
	EC:: bcd_to_string modified to improve BCD compatibility
	
	Revision 3.1  2010/03/17 14:12:03  enrcar
	EC:: ORA_FAST_BCD_HANDLING. Methods OCINumberToInt, OCINumberToText added
	
	Revision 3.0  2009/10/05 14:47:52  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.15  2009/07/14 13:02:06  enrcar
	EC:: c++rules
	
	Revision 2.14  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.13  2008/07/10 13:16:29  lucio.pulvirenti
	INT64 is trated as unsigned long long type and not as BCD
	
	Revision 2.12  2008/07/09 15:01:15  enrcar
	EC:: bcd_to_string Method added
	
	Revision 2.11  2008/05/08 16:38:58  enrcar
	EC:: null-terminated buffer is allocated
	
	Revision 2.10  2008/04/09 09:16:49  enrcar
	EC:: debug improved
	
	Revision 2.9  2008/03/27 15:03:26  enrcar
	EC:: Support for dbx-type fldINT64 added (untested)
	
	Revision 2.8  2007/12/04 10:11:39  crivig
	EC:: suggestedType handled
	
	Revision 2.7  2007/12/03 14:13:15  crivig
	modified in order to handle null BLOB/CLOB in function isNull() of dbColumn
	
	Revision 2.6  2007/06/25 15:31:36  marpas
	fixed again
	
	Revision 2.5  2007/06/25 15:13:16  marpas
	fixing potential datetime bug
	
	Revision 2.4  2007/01/30 15:57:11  paoscu
	Null value exception message enhanced.
	
	Revision 2.3  2006/06/28 16:57:25  marpas
	SQLDialect::getTimeStampResolution  method used to unmarshall fldDATETIME
	
	Revision 2.2  2006/06/26 13:04:01  marpas
	new DateTime / dbExpress interface changed
	
	Revision 2.1  2006/03/09 16:36:01  marpas
	new dbColumn implementation - column type moved into dedicated classes
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	

*/

#include <dbRow.h>
#include <DateTime.h>

#include <sstream>
#include <SQLDialectFactory.h>
#include <dbAppResSet.h>
#include <Guard.hpp>
#include <Filterables.h>
_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

template <class T> struct to_lower : 
	public unary_function<T, T>
{
	to_lower() {} ;
	T operator()(T x) { return ::tolower(x) ; }
} ;

//////////////////////
//              	//
// class dbRow		//
//              	//
//////////////////////

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbRow)


/**
  * dbRow:: WARNING on Column Indexes:
  * ====================================
  *
  * dbRow interface is 0-bases.
  * So columns are counted from 0 to (Num_col-1)
  *
  * Internal DbExpress interface is 1-based.
  * So, Internal methods of DbExpress are called passing (index+1).
  *
  * Check for example getAnyColumn():
  * Interface is 0-based, while DbAccesses method are called using 1-based indexes.
  *
  */
  

/*************************************************************************
dbRow::dbRow() 
*************************************************************************/

dbRow::dbRow(const string &key) :
    _cursor(),
    _dbColumnVector(),
    _storedRow(),
	_key(key)
{
}


/*************************************************************************
dbRow::~dbRow() 
*************************************************************************/
dbRow::~dbRow() throw() 
{
// PRQA S 4631 L1
	try { // don't allow any exception out of the method
	    reset() ;
	}
	catch(exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
	}
// PRQA L:L1
}


/*************************************************************************
dbRow::reset() 
*************************************************************************/
void dbRow::reset() 
{
	_cursor.reset() ;
   	_dbColumnVector.clear() ;
}


void dbRow::row(dbSQLCursor &itrow, 
				const std::vector<dbSpecificColumnType::Type>& suggestedTypes, 
                bool storedRow) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dbSQLCursor empty: " << boolalpha << (itrow.get() != 0) 
            << " storedRow: " << boolalpha << storedRow ) ;
                
	reset() ;
	ACS_COND_THROW(!itrow.get() && storedRow, // PRQA S 3081
			ExecuteException(string("dbRow: invalid input"))) ;
    _storedRow = storedRow ; 
	_cursor = itrow ;
    
    if ( _storedRow ) { 
        storeCurrentRow(suggestedTypes) ;  // Row is local, i.e. will be stored into _dbColumnVector
    }
}


void dbRow::row(const dbRow& row)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "localRow: " << boolalpha << _storedRow) ;
    
    (*this) = row ;
} 


/*************************************************************************
dbRow::storeCurrentRow() 
*************************************************************************/
void dbRow::storeCurrentRow(const vector<dbSpecificColumnType::Type>& suggestedTypes)
{
    /**
      *  Access current cursor and store row into _dbColumnVector
      */
    // Check row is null
	//exception on empty row
	ACS_COND_THROW(!_cursor.get(), ExecuteException("Invalid row state (null pointer)")) ; // PRQA S 3081
    

    UINT16 columnCount = 0 ;
    long returnedValue = SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
   
    // Get column range
	if ( returnedValue!=SQL_SUCCESS) 
    {
        ostringstream os ;
        os << "dbRow::storeCurrentRow invalid return from getColumnCount: " << returnedValue ;
        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
 	}

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "columnCount: " << columnCount << " _storedRow: " << boolalpha << _storedRow ) ;


	if ( ( suggestedTypes.size() > 0) && ( suggestedTypes.size() != columnCount) )
    {
        ostringstream os ;
        os << "dbRow::storeCurrentRow suggestedTypes size " << suggestedTypes.size() << " differs from ColumnCount: " << columnCount ;
        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }



// FIXME ENRICO ATTENZIONE COME LO CAPISCO SE UN CURSORE E' FUORI RANGE ???????
// E IN QT. CASO CHE ECCEZIONE RITORNO????? Direi un'eccezione apposita!

    for (uint16_t i=0; i<columnCount; i++)
    {
        // loop-index i is 0-relative
        
        UINT16 uLogType = 0 ;
        UINT16 uSubType = 0 ;
        uint16_t rel_col = i+1 ;// PRQA S 3000, 3010
        returnedValue=SQLDialect::SQLReturn(_key, _cursor->getColumnType (rel_col, &uLogType, &uSubType)) ;
        // Get column type & subtype as uLogType:uSubType
	    if ( returnedValue!=SQL_SUCCESS ) 
        {
            ostringstream os ;
            os << "dbRow::storeCurrentRow invalid return from getColumnType: " << returnedValue ;
			ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	    }
  
        // Access DB ang Get Column Type as dbSpecificColumnType::Type for _cursor
        dbSpecificColumnType::Type columnType = getColumnType(i);
  
       

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "column: " << i << " Name: " << columnName(i) << " type:subtype " 
                << uLogType << ":" << uSubType << " dbColumn::ColumnType (as int): " 
                << columnType) ;


        bool isNull_required = false ;  // required (will be ignored). MUST BE != from NULL POINTER
        dbColumn column(_key) ;
       
        dbSpecificColumnType::Type suggested = (suggestedTypes.size() != 0) ? suggestedTypes[i] : dbSpecificColumnType::UNDEFINED ; // PRQA S 3384
        
        getAnyColumn(i, column, &isNull_required, false, true, suggested) ;
        _dbColumnVector.push_back(column);

        
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Processed column (1-rel): " << rel_col << "/" << columnCount << " vector size: "  // PRQA S 3084
				<< _dbColumnVector.size()) ;
        
    }   /* for i */    
    
    
    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) 
        dumpStoredRow() ;
	ACS_CLASS_END_DEBUG
    
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Done. Columns: " << columnCount << " VectorSize: " << _dbColumnVector.size()) ;
}


void dbRow::dumpStoredRow()
{
    /*
        Useful for DEBUG Only
    */
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    size_t dbCVsize = _dbColumnVector.size() ;
    for (size_t i=0; i<dbCVsize; ++i)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Column " << i << "/" << _dbColumnVector.size()) ;
        (_dbColumnVector[i]).dumpColumn() ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Done.") ;
}


uint16_t dbRow::columns() const // PRQA S 4020, 4214
{
// This method has multiple returns in order to keep its complexity
// as low as possible

	if (_cursor.get() && (!_storedRow) )
        {
			/* Row not stored */
            UINT16 numColumns = 0 ;
            long commandStatus = SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&numColumns)) ;	/* Call the dbExpress driver getColumnCount method */
            if (commandStatus != SQL_SUCCESS )
            {
				/* Unable to obtain value by dbExpress */
                UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
	            os << "dbRow:columns() Cannot run getColumnCount(). Got status: " << commandStatus << " (" 
                   << es << ")" ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }         
            
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Access Database. Returned value: " << numColumns) ;
               
            return numColumns ;
        }  

    if ( _storedRow )
    {
		/* Row already loaded -- it will be returned the number of stored colums */
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Access Local Row. Returned value: " << _dbColumnVector.size()) ;
        
        return static_cast<uint16_t>(_dbColumnVector.size()) ; // PRQA S 3010, 3081
    }   

	return 0 ;
}


dbColumn const & dbRow::column(uint16_t idx) const
{
    ACS_COND_THROW(! _storedRow, // PRQA S 3081
    		ExecuteException("dbRow::column(long) Cannot return column since not stored." ));
    
    
    // Check column range
	if (idx >= _dbColumnVector.size() ) {
    	ostringstream os ;
        os << "dbRow::column(long) invalid index: " << idx << " shall be less than " << _dbColumnVector.size() ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    // IF HERE: column is stored && range is valid:

    return _dbColumnVector [idx] ;
}


dbSpecificColumnType::Type dbRow::getColumnType(uint16_t idx) const // PRQA S 4214
{
    // return column type as dbSpecificColumnType::Type
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    dbColumn nullColumn(_key) ;
    return getAnyColumn(idx, nullColumn, 0, true) ;
}


unsigned int  dbRow::columnLength(uint16_t idx) const
{
    // idx is 0-relative

    
	//exception on empty row
	ACS_COND_THROW(!_cursor.get(), ExecuteException("Invalid row state (null pointer)")) ; // PRQA S 3081


    UINT16 columnCount = 0 ;
    long queryStatus = SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
	if ( (queryStatus != SQL_SUCCESS) || (columnCount<=idx) ) {
    	// Check column indexes
		ostringstream os ;
        if (queryStatus != SQL_SUCCESS) {
            os << "dbRow::columnLength invalid return from getColumnCount: " << queryStatus ;
        }
        else {
            os << "dbRow::columnLength invalid index" << idx << " shall be less than " << columnCount ;
        }
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    UINT32 columnLength  = 0 ;
  	// Return the dbExpress getColumnLength method
    uint16_t rel_col = idx+1 ; // PRQA S 3000, 3010
    queryStatus=SQLDialect::SQLReturn(_key, _cursor->getColumnLength (rel_col, &columnLength)) ;
	if( queryStatus != SQL_SUCCESS )
    {
        ostringstream os ;
        os << "dbRow::columnLength invalid return from getColumnLength: " << queryStatus ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }
    
    return columnLength ;  
}


bool dbRow::getIfColumnIsNullable(uint16_t idx) const
{
    // idx is 0-relative
	//exception on empty row
	ACS_COND_THROW(!_cursor.get(), ExecuteException("Invalid row state (null pointer)")) ; // PRQA S 3081
    
    UINT16 columnCount = 0 ;
    long queryStatus =SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
    
	if ( (queryStatus != SQL_SUCCESS) || (columnCount<=idx) ) {
    	// Check column indexes
		ostringstream os ;
        if (queryStatus != SQL_SUCCESS) {
            os << "dbRow::getIfColumnIsNullable invalid return from getColumnCount: " << queryStatus ;
        }
        else {
            os << "dbRow::getIfColumnIsNullable invalid index" << idx << " shall be less than " << columnCount ;
		}
        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    BOOL isNullable = 0 ;
	// Return the dbExpress isNullable method
    uint16_t rel_col = idx+1 ;// PRQA S 3000, 3010
    queryStatus=SQLDialect::SQLReturn(_key, _cursor->isNullable (rel_col, &isNullable)) ;
	if( queryStatus != SQL_SUCCESS )
    {
        ostringstream os ;
        os << "dbRow::getIfColumnIsNullable invalid return from getColumnLength: " << queryStatus ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }
    
    return isNullable != 0 ;  
}


string dbRow::columnType(uint16_t idx) const // PRQA S 4214, 5500, 5510
{
// return column type as string, using INFORMIX syntax
// NB cyclomatic complexity is just slightly high, but is due to the switch control structure below
// any attemp to lower CC would increase complexity for maintenance

/*------------------------------------------
Method to be dismissed
WARNING: NOT-PORTABLE METHOD!!!
USAGE OF THIS METHOD IS DEPRECATED
-------------------------------------------*/

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method started for column: " << idx) ;
    
	//exception on empty row
	ACS_COND_THROW(!_cursor.get(), ExecuteException("Invalid row state (null pointer)")) ; // PRQA S 3081
	
    UINT16 columnCount = 0 ;
    long queryStatus =SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
    
	// Check column indexes
	if ( (queryStatus!=SQL_SUCCESS) || (columnCount<=idx) ) {
    	ostringstream os ;
        if (queryStatus != SQL_SUCCESS) {
            os << "dbRow: invalid return from getColumnCount: " << queryStatus ;
        }
        else  {
            os << "dbRow: invalid index" << idx << " shall be less than " << columnCount ;
        }
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}


    UINT16 logType = 0 ;
    UINT16 subType = 0 ;
    uint16_t rel_col = idx+1 ;// PRQA S 3000, 3010
    queryStatus=SQLDialect::SQLReturn(_key, _cursor->getColumnType (rel_col, &logType, &subType)) ;
	if( queryStatus != SQL_SUCCESS )
    {
        ostringstream os ;
        os << "dbRow: invalid return from getColumnType: " << queryStatus ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "processing type [" << logType << ":" << subType << "]") ;

    //
    // Following table obtained running test on Informix.
    // Several types can be still missing
    //

    string typeString ;
    switch(logType) // PRQA S 3000
    {
    case fldZSTRING:
        {
            switch (subType) // PRQA S 3000
            {
            case fldstFIXED: { typeString = "char" ; break ; }
            /* DBEXPRESS-INFORMIX Incompatibility: DBExpress doesn't know lvarchar, varchar. 
            Type char [1:0] is always returned! */ 
            // WARNING, could be also: lvarchar or varchar
            default: { typeString = "nchar" ; break ; }
            }   /* switch (subType) */
            break ;
        }
        
    case fldINT16: { typeString = "smallint" ; break ; }

    case fldINT32:
        {
            switch (subType) // PRQA S 3000
            {
            case fldstAUTOINC: { typeString = "serial" ;  break ; }

            default: { typeString = "integer" ; break ; }

            }   /* switch (subType) */
            break ;
        }

    case fldDATETIME: { typeString = "datetime" ;  break ; }

    case fldBOOL: { typeString = "boolean" ; break ; }
        
    // WARNING, could be also: "smallfloat"
    /* DBEXPRESS-INFORMIX Incompatibility: DBExpress doesn't know smallfloat. Type float [7:0] is always returned! */ 
    case fldFLOAT: { typeString = "float" ; break ; }
        
    case fldFMTBCD:
    case fldINT64: { typeString = "serial8" ; break ; }
    // For example: Informix, returned by count(*)
    case fldBCD: { typeString = "decimal" ; break ; }

    case fldADT:
    case fldBLOB:
        {
            switch (subType) // PRQA S 3000
            {
            // WARNING, could be also "sysbldsqltext", "geometadata"
            /* DBEXPRESS-INFORMIX Incompatibility: DBExpress doesn't know: sysbldsqltext, geometadata */
            case fldstFMTMEMO: { typeString = "geoobject" ; break ; }
            case fldstHBINARY: { typeString = "blob" ; break ; }
            case fldstHMEMO: { typeString = "clob" ; break ; }
            default: { typeString = "blob" ; break ; }
            }   /* switch subType */
            break ;
        }
        
    default:
        {
            ostringstream os ;
            os << "dbRow::columnType: INTERNAL ERROR Unsupported type: " << logType << ":" << subType;
			ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
        }
    }   /* switch logType */

    
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Column: " << idx << " type [" << logType << ":" << subType << "]: " << typeString) ;
    
    return typeString ;
}


string dbRow::columnName(uint16_t idx) const
{
    // idx is 0-relative
	//exception on empty row
	ACS_COND_THROW(!_cursor.get(), ExecuteException("Invalid row state (null pointer)")) ; // PRQA S 3081

    UINT16 columnCount = 0 ;
    long queryStatus =SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
    
	if ( (queryStatus !=SQL_SUCCESS ) || (columnCount<=idx) ) {
    	ostringstream os ;
        if (queryStatus != SQL_SUCCESS) {
            os << "dbRow: invalid return from getColumnCount: " << queryStatus ;
        }
        else {
            os << "dbRow: invalid index" << idx << " shall be less than " << columnCount ;
        }
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

  
    UINT16 columnNameLength = 0;	 
	// Call the dbExpress getColumnNameLength method
    uint16_t rel_col = idx+1 ; // PRQA S 3000, 3010
    queryStatus=SQLDialect::SQLReturn(_key, _cursor->getColumnNameLength (rel_col, &columnNameLength)) ;
	if( queryStatus != SQL_SUCCESS )
    {
        ostringstream os ;
        os << "dbRow: invalid return from getColumnNameLength: " << queryStatus ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }

    char columnName[columnNameLength+1] ; // PRQA S 3000
	// Return the dbExpress getColumnName method
    queryStatus=SQLDialect::SQLReturn(_key, _cursor->getColumnName (rel_col, columnName)) ;
	if( queryStatus != SQL_SUCCESS )
    {
        ostringstream os ;	/* Unable to execute method getColumnName */
        os << "dbRow: invalid return from getColumnName: " << queryStatus ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
    }
    
    return columnName ;  
}


dbSpecificColumnType::Type dbRow::getStoredColumn(uint16_t idx, dbColumn& column, bool *isNull) const// PRQA S 4214
{
    // idx is 0-based

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "idx: " << idx << " size: " << _dbColumnVector.size()) ;

    // Check column range
	if (idx >= _dbColumnVector.size() ) {
    	ostringstream os ;
        os << "dbRow::getStoredColumn invalid index: " << idx << " shall be less than " << _dbColumnVector.size() ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    // reference column
    column = _dbColumnVector[idx] ;


    // Get column Type
    dbSpecificColumnType::Type columnType = dbSpecificColumnType::UNDEFINED;
    column.getColumnType (columnType) ;
    // NOTE: Also works: dbSpecificColumnType::Type columnType = getColumnType(idx) ;

    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        column.dumpColumn() ;
    ACS_CLASS_END_DEBUG
    
    if (isNull)
    {
		(*isNull) = column.isNull() ;    // Return value for isNull assigned.
    }


    if ( (!isNull) && column.isNull() )
    {
        ostringstream os ;
        os << "dbRow::getStoredColumn():  got Null value for column n " <<idx ;
        ACS_THROW(NullValueException(os.str())) ; // PRQA S 3081
    }
    
    return columnType ;    
}               
               
               
     

// WAS dbColumn::ColumnType dbRow::getAnyColumn(long idx, void* colPointer, bool *isNull, bool checkType, bool forceReadDb_IN) const

dbSpecificColumnType::Type dbRow::getAnyColumn( // PRQA S 4020, 4214
    uint16_t idx, 
    dbColumn& column, 
    bool *isNull, 
    bool checkType, 
	bool forceReadDb_IN, 
    dbSpecificColumnType::Type suggestedType) const
{

// This method returns the value of a required column, given the index. Since it trivially uses a "switch" mechanism, is it quite long.


    // idx is 0-based

/*
    Supported dbColumn types:
    string :  For any char* ;
    signed long long int :  For any type of (un)signed integers and for booleans ;
    long double : For any type of (un)signed double or float ;
    DateTime: For TIMESTAMP types.
    
    
    Input Flags:
    if checkType = TRUE -> dbSpecificColumnType::Type is returned immediatly, BEFORE getting data from DB
    if forceReadDb is TRUE, Data are read from Db REGARDLESS the value of _storedRow 
    
    If pointer (isNull==0) Null Value CANNOT BE Accepted. If data is null, an exception is raised
    if pointer (isNull!=0), the value into the pointer will contain TRUE or FALSE. (i.e. (*isNull)=TRUE of FALSE) )
    
    
    FIXME: unsigned 64 bit types still UNSUPPORTED:
        TODO: unsigned long long int: For 64-bit unsigned integers
*/


    dbSpecificColumnType::Type columnType = dbSpecificColumnType::UNDEFINED ;
    
    bool forceReadDb = ( forceReadDb_IN || (!_storedRow) ) ;
    
    
   
     
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "idx: " << idx << " checkType: " << checkType << " localRow: " <<_storedRow << " forceReadDb: " << forceReadDb) ;
     
    // Check row is null
    if (!_cursor.get()) {
        ostringstream os ;
        os << "dbRow::getAnyColumn Line: " << __LINE__ << " Invalid row state (null pointer) for idx: " << idx ;
        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
  	}
    
    
    // Column is local ?
    if ( (!forceReadDb) && (!checkType)) { return getStoredColumn(idx, column, isNull); }
    
    
    // If Still here: column must be read from DB.
    
    
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        if (!checkType) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Column idx: " << idx << " will be taken from database.") ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Column Type for idx: " << idx << " will be taken from database.") ;
        }
    ACS_CLASS_END_DEBUG

    
    // Check column range
    UINT16 columnCount = 0 ;
    long returnedValue=SQLDialect::SQLReturn(_key, _cursor->getColumnCount(&columnCount)) ;
	if ( (returnedValue!=SQL_SUCCESS) || (columnCount<=idx) ) {
    	ostringstream os ;
        if (returnedValue != SQL_SUCCESS) {
            os << "dbRow::getAnyColumn invalid return from getColumnCount: " << returnedValue ;
        }
        else {
            os << "dbRow::getAnyColumn invalid index " << idx << " shall be less than " << columnCount ;
		}
        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    // Get column type & subtype
    uint16_t rel_col = idx+1 ; // PRQA S 3000, 3010
    UINT16 uLogType = 0 ;
    UINT16 uSubType = 0 ;
    returnedValue=SQLDialect::SQLReturn(_key, _cursor->getColumnType (rel_col, &uLogType, &uSubType)) ;
	if ( returnedValue!=SQL_SUCCESS ) 
    {
        ostringstream os ;
        os << "dbRow::getAnyColumn invalid return from getColumnType: " << returnedValue ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}

    // Get column length (not applicable for BLOBs...)
    UINT32 ulLength = 0 ;
    returnedValue=SQLDialect::SQLReturn(_key, _cursor->getColumnLength (rel_col, &ulLength)) ;
	if ( returnedValue!=SQL_SUCCESS )
    {
        ostringstream os ;
        os << "dbRow::getAnyColumn invalid return from getColumnLength: " << returnedValue ;
		ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
	}
   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "processing type [" << uLogType << ":" << uSubType << "]  ColumnLength: " << ulLength) ;

    BOOL returnedNull = 0 ;	
    switch(uLogType) // PRQA S 3000
    {
    case fldZSTRING :
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type STRING [" << uLogType << ":" << uSubType << "]") ;
            
            columnType = dbSpecificColumnType::STRING ; if (checkType) { return columnType ; }
    
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Running calloc of: " << ulLength+1 << " bytes") ; // PRQA S 3084

            //  SPECIFIC-DB STRING PATCHES
            // ==============================
            const string &dialect = dbAppResSetFactory::instance("",_key)->getSQLDialectName() ; // PRQA S 4412
            if (dialect == "Informix9")
            {
                if (0 == ulLength) { ulLength=16384 ; } // PRQA S 4400
				// cfr sysbldobjects: ci sono i "informix".lvarchar 
                // che si presentano con un ulLength==0.... IN REALTA' SONO DELLE STRINGHE !="" !!!!
                
            }

            pCHAR v = static_cast<pCHAR>(calloc(ulLength+1, 1));    /* (+1) added due to '\0' */ // PRQA S 3081, 3084
            if (!v)
            {
                ostringstream os ; 
                os << "dbRow::getAnyColumn - Cannot execute calloc of: " << ulLength+1 << " bytes" ; // PRQA S 3084
                ACS_THROW(ExecuteException( os.str() )); // PRQA S 3081
            }
            
            acs::pattern::CMemoryScopeGuard self_destroy(v); // free will be executed when scope is lost
            returnedValue=SQLDialect::SQLReturn(_key, _cursor->getString (rel_col, v, &returnedNull)) ;
            if( returnedValue != SQL_SUCCESS )
            {
                UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
                os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                   << es << ")" ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }
            
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                column.init (columnType, true) ;
            }
			else {
                column.init (suggestedType, true) ;
			}
            
            if (0 == returnedNull)
            {
                string tmp(v);  
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: !" << v << "!\"") ;
               
                tmp = StringUtils::trimEnd(tmp, StringUtils::is_not_space);  /* Informix Reverse-engineered */
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value after strip: !" << tmp << "!") ;
            
                column.setColumn(tmp) ;
            }
            break ;
        }
        
    case fldADT:
        {
 
           // For example: Postgres geography 
            //
            // WARNING:
            // Since DbExpressWrapper is not able to discern Formatted text and geography
            // (while Informix can), this object is ALWAYS returned as TEXT (ie STRING)
            // (and NOT as Geoobject)
            // Must be converted to Geobject (from string) by final user.
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type GEOOBJECT [" << uLogType << ":" << uSubType << "]") ;
            switch (uSubType) // PRQA S 3000
            {

            case fldstBINARY:
                {
                    columnType = dbSpecificColumnType::GEOOBJECT ;
					if (checkType) {
						return columnType ;
                    }
            		UINT32 blobLen=0 ;
                    returnedValue=SQLDialect::SQLReturn(_key,  _cursor->getBlobSize (rel_col, &blobLen, &returnedNull)) ;
  					if( returnedValue != SQL_SUCCESS )
					{
						UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
						_cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
						ostringstream os ; 
						os << "dbRow::getAnyColumn Cannot run getBlobSize(). Got status: " << returnedValue << " (" 
						<< es << ")" ;
						ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
					}
                   
                    if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                        column.init (columnType, returnedNull != 0 ) ;
                    }
			        else {
                        column.init (suggestedType, returnedNull != 0) ;
			        }

                    break ;
                }
            case fldstFMTMEMO:
                {
                    columnType = dbSpecificColumnType::GEOOBJECT ;
					if (checkType) {
						return columnType ;
                    }
                    pCHAR v = static_cast<pCHAR>(calloc(ulLength+1, 1));    /* (+1) added due to '\0' */ // PRQA S 3081, 3084
                    if (!v)
                    {
                        ostringstream os ; 
                        os << "dbRow::getAnyColumn - Cannot execute calloc of: " << ulLength+1 << " bytes" ; // PRQA S 3084
                        ACS_THROW(ExecuteException( os.str() )); // PRQA S 3081
                    }

                    acs::pattern::CMemoryScopeGuard self_destroy(v); // free will be executed when scope is lost
                    returnedValue=SQLDialect::SQLReturn(_key, _cursor->getString (rel_col, v, &returnedNull)) ;
                    if( returnedValue != SQL_SUCCESS )
                    {
                        UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                        _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                        ostringstream os ; 
                        os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                           << es << ")" ;
				        ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                    }
                    
                    if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                        column.init (columnType, returnedNull != 0 ) ;
                    }
			        else {
                        column.init (suggestedType, returnedNull != 0) ;
			        }

                    if (0 == returnedNull)
                    {
                        string tmp(v);  
                        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: !" << v << "!\"") ;
                        tmp = StringUtils::trimEnd(tmp, StringUtils::is_not_space);  /* Informix Reverse-engineered */
                        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value after strip: !" << tmp << "!") ;
#if __cplusplus > 199711L
                        unique_ptr<dbGeoObject> geo(dbGeoObject::newDbGeo(tmp)) ;
#else
                        auto_ptr<dbGeoObject> geo(dbGeoObject::newDbGeo(tmp)) ;
#endif
                       
                        column.setColumn(*geo) ;
                    }
                    break ;
                }
      
                
            default:
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "UNDEFINED (Unknown) Type [" << uLogType << ":" << uSubType << "]") ;
                    columnType = dbSpecificColumnType::UNDEFINED ;
					if (checkType) {
						return columnType ;
                    }

                    // IF HERE - Unknown value required
                    ostringstream os ;
                    os << "dbRow::getAnyColumn unsupported type: " << uLogType << " subType: " << uSubType ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                 }
            }   // switch uSubType
            break ;
        }
        
        
    case fldBLOB:
        {
 
            switch (uSubType) // PRQA S 3000
            {
            case fldstMEMO:     /* DB2 CLOB */
            case fldstHMEMO:    /* IFX CLOB */ /* TBV: MYSQL CLOB too ???? */
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type CLOB [" << uLogType << ":" << uSubType << "]") ;
                    columnType = dbSpecificColumnType::CLOB ;
					if (checkType) {
						return columnType ;
                    }
            		UINT32 blobLen=0 ;
                    returnedValue=SQLDialect::SQLReturn(_key,  _cursor->getBlobSize (rel_col, &blobLen, &returnedNull)) ;
  					if( returnedValue != SQL_SUCCESS )
					{
						UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
						_cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
						ostringstream os ; 
						os << "dbRow::getAnyColumn Cannot run getBlobSize(). Got status: " << returnedValue << " (" 
						   << es << ")" ;
						ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
					}
                    
                    if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                        column.init (columnType, returnedNull != 0) ;
                    }
			        else {
                        column.init (suggestedType, returnedNull != 0) ;
			        }

                    break ;
                }
                
            case fldstFMTMEMO:
                {                    
                    // For example: Informix geographiclocalization "informix".geoobject
                    //
                    // WARNING:
                    // Since DbExpressWrapper is not able to discern Formatted text and Geoobject
                    // (while Informix can), this object is ALWAYS returned as TEXT (ie STRING)
                    // (and NOT as Geoobject)
                    // Must be converted to Geobject (from string) by final user.
                    
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type STRING (FORMATTED TEXT) [" << uLogType << ":" << uSubType << "]") ;
                    columnType = dbSpecificColumnType::STRING ; if (checkType) { return columnType ; }

                    // WARNING: ulLength is not defined here, and CAN BE IGNORED.
                    // getBlobSize method MUST BE USED!
                    UINT32 blobLen=0 ;
                    returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBlobSize (rel_col, &blobLen, &returnedNull)) ;
                    if( returnedValue != SQL_SUCCESS )
                    {
                        UINT16 elen =0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                        _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                        ostringstream os ; 
                        os << "dbRow::getAnyColumn Cannot run getBlobSize(). Got status: " << returnedValue << " (" 
                           << es << ")" ;
 						ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                    }
                          
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "getBlobSize returned: " << blobLen) ;
                    
                    if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                        column.init (columnType, true) ;
                    }
			        else {
                        column.init (suggestedType, true) ;
			        }
                    
                    if (0 == returnedNull)
                    {
                        pVOID v = calloc(blobLen+1, 1);   // Memory is allocated and will be NULL-terminated // PRQA S 3084
                        if (!v)
                        {
                            ostringstream os ; 
                            os << "dbRow::getAnyColumn - Cannot execute calloc of: " << blobLen << " bytes" ;
							ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                        }
                        
                        memset(v, 0, (blobLen+1)) ;// PRQA S 3084
                        acs::pattern::CMemoryScopeGuard self_destroy(v);    // free will be executed when scope is lost
                         returnedValue=SQLDialect::SQLReturn(_key,  _cursor->getBlob (rel_col, v, &returnedNull, blobLen)) ;
                        if( returnedValue != SQL_SUCCESS )
                        {
                            UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                            _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                            ostringstream os ; 
                            os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                               << es << ")" ;
							ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                        }

						string tmp = static_cast<char*>(v) ; // PRQA S 3081
                        column.setColumn(tmp) ;
     
                        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: !" << tmp << "!") ;
                    } /* end_else */
                    break ;
                } /* fldstFMTMEMO */
                       
                          
            case fldstHBINARY:
            case fldstBINARY:
                {
                    // Reverse-engineered:
                    // fldstBINARY: MYSQL longblob
                    //
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type BLOB [" << uLogType << ":" << uSubType << "]") ;
                    columnType = dbSpecificColumnType::BLOB ;
					if (checkType) {
						return columnType ;
                    }
            		UINT32 blobLen=0 ;
                    returnedValue=SQLDialect::SQLReturn(_key,  _cursor->getBlobSize (rel_col, &blobLen, &returnedNull)) ;
  					if( returnedValue != SQL_SUCCESS )
					{
						UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
						_cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
						ostringstream os ; 
						os << "dbRow::getAnyColumn Cannot run getBlobSize(). Got status: " << returnedValue << " (" 
						<< es << ")" ;
						ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
					}
                   
                    if (suggestedType == dbSpecificColumnType::UNDEFINED) {
                        column.init (columnType, returnedNull != 0 ) ;
                    }
			        else {
                        column.init (suggestedType, returnedNull != 0) ;
			        }

                    break ;
                }
      
                
            default:
                {
                    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "UNDEFINED (Unknown) Type [" << uLogType << ":" << uSubType << "]") ;
                    columnType = dbSpecificColumnType::UNDEFINED ;
					if (checkType) {
						return columnType ;
                    }

                    // IF HERE - Unknown value required
                    ostringstream os ;
                    os << "dbRow::getAnyColumn unsupported type: " << uLogType << " subType: " << uSubType ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                 }
            }   // switch uSubType
            break ;
        }
        
    case fldBOOL : // falltrough // PRQA S 4011
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type fldBOOL [" << uLogType << ":" << uSubType << "]") ;
            columnType = dbSpecificColumnType::BOOLEAN ; if (checkType) { return columnType ; }
            // IMPLEMENTED AS INT16
        } // falltrough
    case fldINT16 :
        {
            if (uLogType == fldINT16)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type INT16 [" << uLogType << ":" << uSubType << "]") ;

                columnType = dbSpecificColumnType::SIGNED_SHORT_INT ; 
			            
            	if (checkType) { return columnType ; }
            }
        }
        {    
            // FOR BOTH BOOL AND INT16

            INT16 v = 0;
            if ( sizeof(v) < ulLength)
            {
                ostringstream os ; 
                os << "dbRow::getAnyColumn INT16 Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }
            returnedValue=SQLDialect::SQLReturn(_key, _cursor->getShort (rel_col, &v, &returnedNull)) ;
            if( returnedValue != SQL_SUCCESS )
            {
                UINT16 elen = 0; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
                os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                   << es << ")" ;
 				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }
            
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
	            column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }
            
   			if (0 == returnedNull)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: " << v) ;
                
                column.setColumn(v) ;
            }

            break ;
        }
     
     
    case fldDATETIME:
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type DATETIME [" << uLogType << ":" << uSubType << "]") ;
            columnType = dbSpecificColumnType::DATETIME ; if (checkType) { return columnType ; }
            CTIMESTAMP v ; // PRQA S 4102
            if ( sizeof(v) < ulLength)
            {
                /*---------------------------------------------------------------------------------
                    PLEASE NOTE: A size of 24 bytes is allocated into the colum (ulLength == 24)
                    But only 16 bytes are allocated in structure CTIMESTAMP (sizeof is 16).
                    So, this exception MUST BE IGNORED 
                ---------------------------------------------------------------------------------*/
                // ostringstream os ; 
                // os << "dbRow::getAnyColumn Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
                // ACS_THROW(ExecuteException( os.str() ));  // PRQA S 3081
            }
            returnedValue=SQLDialect::SQLReturn(_key, _cursor->getTimeStamp (rel_col, &v, &returnedNull)) ;

            if( returnedValue != SQL_SUCCESS )
            {
                UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
                os << "dbRow::getAnyColumn Cannot run getTime(). Got status: " << returnedValue << " (" 
                   << es << ")" ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }

			
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
    	        column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }


            if (0 == returnedNull)
            {
                ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
                {
                    char debug_string[1024]; // PRQA S 4403
                    sprintf(debug_string,"YYYY-MM-DD hh:mm:ss.(frac)= %04d-%02d-%02d %02d:%02d:%02d.(%d)", \
						v.year,v.month,v.day,v.hour,v.minute,v.second,v.fraction);
                    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Returned value: " << debug_string) ;
                }
                ACS_CLASS_END_DEBUG            
                
				
				// READ CAREFULLY !!!!!
                // Create UTCD Object - ATTENTION !!! now the new dbExpress uses fraction as microsec  !!!!
				// READ CAREFULLY !!!!!
                DateTime::UTCD tmp_utcd(v.year,v.month,v.day,v.hour,v.minute,v.second,v.fraction,SQLDialectFactory::sqlDialect(_key).getTimeStampResolution()) ; // PRQA S 3000
                // Create DateTime Object
                DateTime tmp_DateTime(tmp_utcd) ; 	  
                column.setColumn( tmp_DateTime ) ;
            }
            break ;
        }
     
     
    case fldINT32 :
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type INT32 [" << uLogType << ":" << uSubType << "]") ;
            
            columnType = dbSpecificColumnType::SIGNED_INT ; if (checkType) { return columnType ; }
            INT32 v = 0 ;
            if ( sizeof(v) < ulLength)
            {
                ostringstream os ; 
                os << "dbRow::getAnyColumn INT32 Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            
            }
            returnedValue=SQLDialect::SQLReturn(_key, _cursor->getLong (rel_col, &v, &returnedNull)) ;
            if( returnedValue != SQL_SUCCESS )
            {
                UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
                os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                   << es << ")" ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }
            
            
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
    	        column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }
    		
            if (0 == returnedNull)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: " << v) ;
                
                column.setColumn( v ) ;
            }
           

            break ;
        }
  
    case fldFLOAT :
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type FLOAT [" << uLogType << ":" << uSubType << "]") ;
            
            columnType = dbSpecificColumnType::DOUBLE ; if (checkType) { return columnType ; }
            DFLOAT v = 0 ;
            if ( sizeof(v) < ulLength)
            {
                ostringstream os ; 
                os << "dbRow::getAnyColumn DFLOAT Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            
            }
            returnedValue=SQLDialect::SQLReturn(_key, _cursor->getDouble (rel_col, &v, &returnedNull)) ;
            if( returnedValue != SQL_SUCCESS )
            {
                UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                ostringstream os ; 
                os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                   << es << ")" ;
				ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
            }

            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
    	        column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }
            
            if (0 == returnedNull)
            {
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: " << v) ;
                
                column.setColumn( v ) ; 

            }
            break ;
        }
  

    case fldINT64 :
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type BCD [" << uLogType << ":" << uSubType << "]") ;
            
			columnType = dbSpecificColumnType::SIGNED_LONG_LONG_INT ; if (checkType) { return columnType ; }

            FMTBcd v ; // PRQA S 4102
            unsigned char v_ora[128] ; // PRQA S 4403
			signed long long int v_ifx = 0 ;
            const string &dialect = SQLDialectFactory::dialectName(_key) ;
            if ( dialect == "Informix9" )
            {
                returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBytes (rel_col, reinterpret_cast<unsigned char *>(&v_ifx), &returnedNull)) ; // PRQA S 3030, 3081
                if( returnedValue != SQL_SUCCESS )
                {
                    UINT16 elen = 0; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                    _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn Cannot execute getBcd(). Got status: " << returnedValue << " (" 
                       << es << ")" ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                }
            }
            else if ( dialect == "Oracle11" )
            {
            	/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */
                if ( sizeof(v_ora) < ulLength)
                {
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn v_ora[128] Internal Error. BCD Allocated size: " << sizeof(v_ora) << " less than column size: " << ulLength ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081

                }
                returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBlob (rel_col, &v_ora, &returnedNull, ulLength)) ;
				if( returnedValue != SQL_SUCCESS )
                {
                    UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                    _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn  Handling type BCD Cannot execute getBlob(). Got status: " << returnedValue << " (" 
                       << es << ")" ;
					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                }
            
            } else { 
			
                /* This check is probably useless, since the ulLength returns the lenght of the DB specific-decimal type, 
                	and v is the generic dbExpress FMTBcd type */
                if ( sizeof(v) < ulLength)
                {
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn FMTBcd Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081

                }
                returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBcd (rel_col, &v, &returnedNull)) ;
                if( returnedValue != SQL_SUCCESS )
                {
                    UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ;  // PRQA S 3000
                    _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn Cannot execute getBcd(). Got status: " << returnedValue << " (" 
                       << es << ")" ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                }
            }
			
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
    	        column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }
            
            
            if (0 == returnedNull)
            {
                signed long long int tmp = 0 ;
                
				if ( dialect == "Informix9" ) {
					tmp = v_ifx ;
				} else if ( dialect == "Oracle11" ) {
					dbRow::OCINumberToInt(v_ora, tmp) ;	/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */
				} else {
					tmp = bcd_to_long(v) ;
				}

                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: " << tmp) ;
                
                column.setColumn( tmp ) ;
            }
            break ;
        }


    case fldFMTBCD :
    case fldBCD :
        {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got type BCD [" << uLogType << ":" << uSubType << "]") ;
            
			columnType = dbSpecificColumnType::BCD ; if (checkType) { return columnType ; }
            FMTBcd v ; // PRQA S 4102
			unsigned char v_ora[128] ; // PRQA S 4403
            const string &dialect = SQLDialectFactory::dialectName(_key) ;
            if ( dialect != "Oracle11" )
            { 
                /* This check is probably useless, since the ulLength returns the lenght of the DB specific-decimal type, 
                	and v is the generic dbExpress FMTBcd type */
                if ( sizeof(v) < ulLength)
                {
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn FMTBcd Internal Error. Allocated size: " << sizeof(v) << " less than column size: " << ulLength ;
					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081

                }
                returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBcd (rel_col, &v, &returnedNull)) ;
                if( returnedValue != SQL_SUCCESS )
                {
                    UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ; // PRQA S 3000
                    _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn Cannot run getString(). Got status: " << returnedValue << " (" 
                       << es << ")" ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
               }
            }
            else
            {
            	/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */
                if ( sizeof(v_ora) < ulLength)
                {
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn v_ora[128] Internal Error. BCD Allocated size: " << sizeof(v_ora) << " less than column size: " << ulLength ;
					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081

                }
                returnedValue=SQLDialect::SQLReturn(_key, _cursor->getBlob (rel_col, &v_ora, &returnedNull, ulLength)) ;
				if( returnedValue != SQL_SUCCESS )
                {
                    UINT16 elen = 0 ; _cursor->getErrorMessageLen(&elen) ; char es[elen+1] ;// PRQA S 3000
                    _cursor->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ; // PRQA S 3030, 3081
                    ostringstream os ; 
                    os << "dbRow::getAnyColumn  Handling type BCD Cannot execute getBlob(). Got status: " << returnedValue << " (" 
                       << es << ")" ;
 					ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
                }
            }            
            
            
            if (suggestedType == dbSpecificColumnType::UNDEFINED) {
    	        column.init (columnType, true) ;
            }
            else {
	            column.init (suggestedType, true) ;
            }
            
            
            if (0 == returnedNull)
            {
                string tmp ; 
                if ( dialect != "Oracle11" ) {
                	tmp = bcd_to_string(v) ;
                }
				else {
                    dbRow::OCINumberToText(v_ora, tmp) ;	/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */
                }
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: !" << tmp << "!") ;
                
                column.setColumn( tmp ) ;
            }
            break ;
        }
    
    
    default:    	
        {
            ostringstream os ;
            os << "dbRow::getAnyColumn unsupported type: " << uLogType << " subType: " << uSubType ;
			ACS_THROW(ExecuteException(os.str())) ; // PRQA S 3081
        }
    }

   
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Null value: " << ((returnedNull != 0) ? "yes" : "no")) ; // PRQA S 3380
    
    if (isNull) {
		(*isNull) = (returnedNull != 0) ;    // Return value for isNull assigned.
    }
    
    if ( (0 == isNull) && (returnedNull != 0) )
    {
        ostringstream os ;
        os << "dbRow::getAnyColumn:  got Null value" ;
        ACS_THROW(NullValueException(os.str())) ; // PRQA S 3081
    }
    
   
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        if (0 == isNull) {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"isNull: null pointer") ;
        }
        else {
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"isNull: " << boolalpha << *isNull) ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Returned type: " << columnType ) ; // PRQA S 3081
    ACS_CLASS_END_DEBUG
    

    return columnType ;
}   /* dbRow::getAnyColumn */


signed long long int dbRow::bcd_to_long(const FMTBcd& bcd)  // PRQA S 4020
{
// This method has multiple returns in order to keep its complexity
// as low as possible


    signed long long int value = 0 ;
    bool isNeg = false ;

    // WARNING: dbExpress conversion methods can be extracted from CVS:
    // dbRow.C (release 31) ; dbRow.h (release 17)

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        bcd_dump(bcd) ;
    ACS_CLASS_END_DEBUG


    if ( 0 == bcd.iPrecision ) { return value ; }
    // if ( bcd.iSignSpecialPlaces & 0x40 ) return value ; // Unimplemeted since never used

    isNeg = ( 0 != ( bcd.iSignSpecialPlaces & 0x80)) ?  true: false ; // PRQA S 3380, 4400
     

    for (int i=0; i<(bcd.iPrecision >> 1); i++)
    {
        // ONLY THE FIRST int(bcd.iPrecision/2) BYTES ARE EVALUATED.
        // NOTE: IF bcd.iPrecision IS ODD THE LAST 4 BITS ARE IGNORED (SEE BELOW) 
        unsigned int p =
            ((( static_cast<unsigned char>(bcd.iFraction[i]) & 0xf0)>>4)*10) +  // PRQA S 3000, 3007, 3081, 4400
			(static_cast<unsigned char>(bcd.iFraction[i]) & 0x0f) ; // PRQA S 3081, 4400

        value *= static_cast<signed long long int>(100) ; // PRQA S 3081, 4400
        value += static_cast<signed long long int>(p) ;  // PRQA S 3081
    }

    if (0 != (bcd.iPrecision & 0x01))
    {
        // THE LAST 4 bits (IF bcd.iPrecision IS ODD) ARE EVALUATED HERE
        int i = (bcd.iPrecision >> 1) ;

        unsigned int p = ((static_cast<unsigned char>(bcd.iFraction[i]) & 0xf0)>>4) ; // PRQA S 3000, 3007, 3081, 4400 

        value *= static_cast<signed long long int>(10); // PRQA S 3081, 4400 
        value += static_cast<signed long long int>(p);  // PRQA S 3081
    }


	if ((bcd.iSignSpecialPlaces & 0x3f)>0)	/* FP part is present */ // PRQA S 4400
    {
        signed long long int p = static_cast<signed long long int>(powl( 10.0,bcd.iSignSpecialPlaces & 0x3f )) ; // PRQA S 3011, 3081
        value /= p ;
    }


    value = ( isNeg? -value: value ) ; // PRQA S 3380

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returned value: " << value) ;

    return value ;
}


/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */        
void dbRow::OCINumberToInt(const void* oci_num, signed long long& res) const // PRQA S 4214
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    void* param_vec[3] ; // PRQA S 4403
    short int l = 0 ;    
    const signed short int cmd=0 ;

#ifdef __x86_64__
	int64_t p = 0 ;	/* To manage a 64bit ptr. */	
#else
	int32_t p = 0 ;	/* To manage a 32bit ptr. */	
#endif
    
    /* eCurParentFieldID:	This parameter has NO MEANING -- IT SHOULD BE: eCurGetErrorHandlerPtr */
    long result = _cursor->getOption (eCurParentFieldID, &p, sizeof(p), &l) ;
    ACS_COND_THROW( result != SQL_SUCCESS ,  // PRQA S 3081
        ExecuteException( "dbRow::getAnyColumn  Error running getOption for param. eCurParentFieldID" )
        ) ;

    param_vec[0] = reinterpret_cast<void*> ( p ) ; // PRQA S 3081 3
    param_vec[1] = const_cast<void*> (oci_num) ;
    param_vec[2] = static_cast<void*> (&res) ; // PRQA S 4400

    SQLDialect::executeSpecificCommand(_key, param_vec, sizeof(param_vec)/sizeof(param_vec[0]), cmd) ; // PRQA S 3084

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Returned value: " << res) ;
}


/* ORACLE DATABASE  --  ENRCAR:: ORA_FAST_BCD_HANDLING */        
void dbRow::OCINumberToText(const void* oci_num, string& res) const // PRQA S 4214
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;	

    void* param_vec[4] ; // PRQA S 4403 2
    char buffer[1024] ;
    short int l = 0 ;
    int buffer_len = sizeof(buffer);
    const signed short int cmd=1 ;
#ifdef __x86_64__
	int64_t p = 0 ;	/* To manage a 64bit ptr. */	
#else
	int32_t p = 0;	/* To manage a 32bit ptr. */	
#endif
    
    /* eCurParentFieldID:	This parameter has NO MEANING -- IT SHOULD BE: eCurGetErrorHandlerPtr */
    long result = _cursor->getOption (eCurParentFieldID, &p, sizeof(p), &l) ;
    ACS_COND_THROW( result != SQL_SUCCESS , // PRQA S 3081
        ExecuteException( "dbRow::getAnyColumn  Error running getOption for param. eCurParentFieldID" )
		);

	memset(buffer, 0, buffer_len) ; // PRQA S 3000
    param_vec[0] = reinterpret_cast<void*> (p) ; // PRQA S 3081 4
    param_vec[1] = const_cast<void*> (oci_num) ;
    param_vec[2] = static_cast<void*> (buffer) ; // PRQA S 4400 2
    param_vec[3] = static_cast<void*> (&buffer_len) ;

    SQLDialect::executeSpecificCommand(_key, param_vec, sizeof(param_vec)/sizeof(param_vec[0]), cmd) ; // PRQA S 3084

	res = buffer ;

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method finished. Returned value: !" << res << "!") ;
}

       
char* dbRow::bcd_to_string(	const FMTBcd& bcd,	/* I: BCD input */  // PRQA S 4020
								char buffer[64])	/* I/O: Preallocated buffer */ // PRQA S 4403
{
// This method has multiple returns in order to keep its complexity
// as low as possible

	unsigned char c[2] ;  // PRQA S 4403

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        bcd_dump(bcd) ;
    ACS_CLASS_END_DEBUG

    strcpy(buffer, "") ; // reset the output string

    if ( 0 == bcd.iPrecision ) 
    {
    	strcpy(buffer, "0") ;
    	return buffer ;
    }
    
    // if ( bcd.iSignSpecialPlaces & 0x40 ) return value ; // Unimplemented since never used

    if ( (bcd.iSignSpecialPlaces & 0x80) != 0) { // PRQA S 4400
        strcpy(buffer, "-") ;	/* Negative number */
    }
	else {
        strcpy(buffer, "") ;	/* Positive number */
	}
        

    signed int intPartDigits = bcd.iPrecision - (bcd.iSignSpecialPlaces & 0x3f);	/* 3F is the mask for getting 6 bits */
    c[1]=0; 
    unsigned int written = 0 ;


	if (intPartDigits<=0)
	{
    	/* Number is in the form 0.[0..0]xxx */
        strcat (buffer, "0.") ;	written ++ ;	
		for (ssize_t i=0; i<(-intPartDigits); i++)
        {
        	strcat (buffer, "0") ;		
            written ++ ;
        }    
    }

	for (ssize_t i=0; i<bcd.iPrecision; i++)
	{

		c[0] = ((i%2) != 0)? (bcd.iFraction[i/2] & 0x0f) : ((bcd.iFraction[i/2] & 0xf0) >> 4) ; // PRQA S 3000, 3007, 3010, 3380, 4400

		if ( (intPartDigits>0) && (i == intPartDigits) )  { strcat(buffer, ".") ; }

		c[0] += '0' ;	/* e.g. From 5 to '5' */
		strcat(buffer, reinterpret_cast<char*>(c)); // PRQA S 3030, 3081
		written ++ ;

	}

	if ( 0 == written) { strcpy (buffer, "0") ; }


    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returned value: !" << buffer << "!") ;

    return buffer ;
}
        
        
string dbRow::bcd_to_string(const FMTBcd& bcd) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    
	char s[64] ; // PRQA S 4403
    
	return string(bcd_to_string(bcd, s)) ; // PRQA S 3081
}


void dbRow::bcd_dump(const FMTBcd& bcd) 
{
	// DEBUG -- dump a BCD value
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "bcd.iPrecision: " << bcd.iPrecision) ; 

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "bcd.iSignSpecialPlaces: 0x" << hex << static_cast<unsigned int>(bcd.iSignSpecialPlaces)) ; // PRQA S 3081

    for (int i=0; i<32; i++) { // PRQA S 4400
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "bcd.iFraction [" << i << "]: 0x" << hex << static_cast<unsigned int>(bcd.iFraction [i])) ; // PRQA S 3081
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Done") ;
}




//------------------------------------------------------------------------
//                                ::Assignment
//------------------------------------------------------------------------
dbRow::dbRow(const dbRow &r) :
    _cursor(r._cursor),
    _dbColumnVector(r._dbColumnVector),
    _storedRow(r._storedRow),
    _key(r._key)

{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


//------------------------------------------------------------------------
//                                 ::Copy
//------------------------------------------------------------------------
dbRow& dbRow::operator=(const dbRow &e)
{
	// copy ctor
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

    if (this != &e)
    {
        _cursor = e._cursor ;
        _dbColumnVector = e._dbColumnVector ;
        _storedRow = e._storedRow ;
        _key = e._key ;
	}
	
    return *this;
}



_ACS_END_NAMESPACE
