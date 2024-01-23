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
	Revision 1.12  2012/02/06 10:03:03  marpas
	removing compiler warning
	
	Revision 1.11  2009/01/19 12:57:31  marpas
	CPGLCommand::getRowsAffected method fixed and tested
	
	Revision 1.10  2008/11/20 14:18:42  marpas
	rewriting and fixing
	
	Revision 1.9  2008/07/07 14:31:15  enrcar
	EC:: bug fixed
	
	Revision 1.8  2008/07/07 14:30:01  enrcar
	EC:: 64bit compatibility improved
	
	Revision 1.7  2008/05/16 10:03:48  enrcar
	minor changes
	
	Revision 1.6  2008/03/27 14:16:37  enrcar
	EC:: Debug improved
	
	Revision 1.5  2008/03/20 10:00:17  enrcar
	EC:: debug improved
	
	Revision 1.4  2008/02/25 17:20:56  marpas
	work in progres
	
	Revision 1.3  2007/12/18 16:56:07  marpas
	work in progress
	
	Revision 1.2  2007/12/17 18:38:31  marpas
	work in progress
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/


#include <Plugin.h>
#include <exFWarning.h>
#include <File.h>
#include <dbxpress.hpp>
#include <iomanip>
#include <cstring>

extern SQLResult __stdcall (*getSQLDriver)(char * VendorLib, char * SResourceFile, ppSQLDriver);
typedef SQLResult __stdcall (*getSQLDriverProt)(char * VendorLib, char * SResourceFile, ppSQLDriver);


using namespace acs ;
using namespace std ;
	

ostream & operator << (ostream &os, const CTIMESTAMP &t)
{
	os << setw(4) << t.year << '-'
	   << setw(2) << setfill('0') << t.month << '-'
	   << setw(2) << setfill('0') << t.day << ' '
	   << setw(2) << setfill('0') << t.hour << ':'
	   << setw(2) << setfill('0') << t.minute << ':'
	   << setw(2) << setfill('0') << t.second << '.'
	   << setw(3) << setfill('0') << (t.fraction) << setfill(' ') ;
	return os ;
}
	

exostream & operator << (exostream &os, const CTIMESTAMP &t)
{
	os << setw(4) << t.year << '-'
	   << setw(2) << setfill('0') << t.month << '-'
	   << setw(2) << setfill('0') << t.day << ' '
	   << setw(2) << setfill('0') << t.hour << ':'
	   << setw(2) << setfill('0') << t.minute << ':'
	   << setw(2) << setfill('0') << t.second << '.'
	   << setw(3) << setfill('0') << (t.fraction) << setfill(' ')  ;
	return os ;
}

exostream & operator << (exostream &os, const FMTBcd &t)
{
	char outstring[4096] ;
    unsigned char* p = (unsigned char*)&t ;
    unsigned int i ;
    
    memset (outstring, 0, sizeof(outstring)) ;
    strcpy(outstring, "FMTBcd Binary dump: ");
    
    for (i=0; i<sizeof(FMTBcd); i++)
    {
        char t[16] ;
        sprintf(t, " %02x", (int)p[i]);
        strcat(outstring, t); 
    }
    
	os << outstring ;
	return os ;
}


ostream & operator << (ostream &os, const FMTBcd &t)
{
	char outstring[4096] ;
    unsigned char* p = (unsigned char*)&t ;
    unsigned int i ;
    
    memset (outstring, 0, sizeof(outstring)) ;
    strcpy(outstring, "FMTBcd Binary dump: ");

    for (i=0; i<sizeof(FMTBcd); i++)
    {
        char t[16] ;
        sprintf(t, " %02x", (int)p[i]);
        strcat(outstring, t); 
    }
    
	os << outstring ;
	return os ;
}


int init(const std::string &dbx_so_path, const std::string &vendorLib, const std::string &function, const string &table) 
{
	Plugin *_so_loader=new Plugin(dbx_so_path,false,"",RTLD_NOW|RTLD_GLOBAL) ;

	excerr << "init:" << __LINE__ 
		   << " loading so: " << dbx_so_path 
		   << endl ;

	void *SQLDllHandle = _so_loader->open() ;

	excerr << "init:" << __LINE__ 
		   << " locating symbol: " << function 
		   << " open error: \"" << _so_loader->openError() << "\""
		   << endl ;


	void *GetDriver = dlsym(SQLDllHandle,function.c_str()) ;


	if (!GetDriver) {
		ostringstream os ;
		os << "Unable to find symbol \"" << function << "\"" ;
		exIOException e(os.str(),errno) ; 
		throwException(e) ;
	}

	excerr << "init:" << __LINE__ 
		   << " symbol found : " << GetDriver 
		   << endl ;

	getSQLDriverProt _getDriver = (getSQLDriverProt)GetDriver ;

	excerr << "init:" << __LINE__ 
		   << " calling : _getDriver with vendorlib:" << vendorLib
		   << endl ;

	SQLDriver *_sqlDriver ;

	int result = _getDriver(const_cast<char *>(vendorLib.c_str()),const_cast<char *>(""),&_sqlDriver) ;
	try {
		if (result != SQL_SUCCESS) {
			ostringstream os ;
			os << "Error calling symbol " << function << "(" << vendorLib << ") [" << result << "]" ;
			exIOException e(os.str(),errno) ; 
			throwException(e) ;
		}
	}
	catch(exception &x) {
		if (!_sqlDriver)
			throw ;
		exFWarning war ;
		war << "WARNING: " << x.what() ;
		excerr << war ;
	}
	excerr << "init:" << __LINE__ 
		   << " _getDriver returned: " << _sqlDriver 
		   << endl ;

	unsigned short ret = _sqlDriver->setOption(eDrvRestrict,0) ;
	if (ret != SQL_SUCCESS) {
		ostringstream os ;
		os << "SetOption(eDrvRestrict,0) returned " << ret ;
		exIOException e(os.str()) ;
		throwException(e) ;
	}
	
	
	SQLConnection *conn = 0 ;
	
	int retvalue ;
	
	retvalue = _sqlDriver->getSQLConnection(&conn) ;
	
	excerr << "init:" << __LINE__ 
		   << " getSQLConnection returned: " << retvalue 
		   << " (" << conn <<  ")" 
		   << endl ;
	
	
	
	
	retvalue = conn->disconnect() ;
	excerr << "init:" << __LINE__ 
		   << " disconnect returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	

	ret = conn->setOption(eConnHostName,reinterpret_cast<ACS_PTRASINT_T>("localhost")) ;
	if (ret != SQL_SUCCESS) {
		ostringstream os ;
		os << "SetOption(eConnHostName,0) returned " << ret ;
		exIOException e(os.str()) ;
		throwException(e) ;
	}

	retvalue = conn->connect(const_cast<char *>("pdsdb"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
	excerr << "init:" << __LINE__ 
		   << " connect returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	char serverVersion[1024] ;
	INT16 serverVersionLen ;
	retvalue = conn->getOption (eConnServerVersion, reinterpret_cast<ACS_PTRASINT_T*>(serverVersion), sizeof(serverVersion), &serverVersionLen) ;
	excerr << "init:" << __LINE__ 
		   << " getOption(eConnServerVersion) returned: " << retvalue 
		   << " (\"" << serverVersion << "\")"
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	char dbName[1024] ;
	INT16 dbNameLen ;
	retvalue = conn->getOption (eConnDatabaseName, reinterpret_cast<ACS_PTRASINT_T*>(dbName), sizeof(dbName), &dbNameLen) ;
	excerr << "init:" << __LINE__ 
		   << " getOption(eConnDatabaseName) returned: " << retvalue 
		   << " (\"" << dbName << "\")"
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	INT32 blockingMode ;
	INT16 blockingModeLen ;
	retvalue = conn->getOption (eConnBlockingMode, &blockingMode, sizeof(blockingMode), &blockingModeLen) ;
	excerr << "init:" << __LINE__ 
		   << " getOption(eConnBlockingMode) returned: " << retvalue 
		   << " blockingMode: " << blockingMode << "(" << blockingModeLen << ")"
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	excerr << "init:" << __LINE__ 
		   << " about to call disconnect" 
		   << endl ;
	retvalue = conn->disconnect() ;
	excerr << "init:" << __LINE__ 
		   << " disconnect returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	
	excerr << "init:" << __LINE__ 
		   << " about to call connect" 
		   << endl ;
	retvalue = conn->connect(const_cast<char *>("pdsdb"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
	excerr << "init:" << __LINE__ 
		   << " connect returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	SQLCommand *command = 0 ;
	
	retvalue = conn->getSQLCommand(&command) ;
	excerr << "init:" << __LINE__ 
		   << " getSQLCommand returned: " << retvalue 
		   << " (" << command <<  ")" 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
		{
 		UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		command->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
		}
	}
	
//	string SQLString = "SELECT IDENTITY_VAL_LOCAL() FROM t_ordersqueue FETCH FIRST ROW ONLY ;" ;
	string SQLString = "select * from " + (table.length() ? table : string(" t_inventory"))  ;
// string SQLString = "select space from " + (table.length() ? table : string(" t_configurationspaces")) + " where namespace = 'test'" ;
//	string SQLString = "select soundex('STOCA'), soundex('STOKA'), CURRENT TIMESTAMP from " + (table.length() ? table : string(" t_inventory")) ;
//	string SQLString = "select CURRENT TIMESTAMP, orderid, CONCAT(processorname,processorversion) from " + (table.length() ? table : string(" t_inventory")) + " FOR UPDATE" ;
//	string SQLString = "select NULLIF(SOUNDEX('STOKA'),SOUNDEX('STOCA')) from " + (table.length() ? table : string(" t_inventory")) + " FOR UPDATE" ;
	excerr << "init:" << __LINE__ 
		   << " executing: " << SQLString 
		   << endl ;
	
	retvalue = command->prepare ((char *)SQLString.c_str(), 0) ;
	excerr << "init:" << __LINE__ 
		   << " prepare returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		command->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	SQLCursor *cursor = 0  ;
	retvalue = command->execute (&cursor) ;
	excerr << "init:" << __LINE__ 
		   << " execute returned: " << retvalue 
		   << " (" << cursor <<  ")" 
		   << endl ;
           
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		command->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	else {
		
        excerr << endl << "init: " << __LINE__ << " command->execute was SUCCESSFULLY." << endl << endl ;
		
        INT32 nrows ; 
		retvalue = command->getRowsAffected(&nrows) ;
		excerr << "init:" << __LINE__ 
			   << " getRowsAffected exited with status: " << retvalue 
			   << " (nrows: " << nrows <<  ")" 
			   << endl ;
		if (retvalue != SQL_SUCCESS) {
 			UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                			command->getErrorMessage((unsigned char *)es) ;
	    	excerr << "LEN:" << elen << endl ;
	    	excerr << "MSG:\n" << es << endl ;
		}
		
		UINT16 colcount = 0 ;
		retvalue = cursor->getColumnCount(&colcount) ;
		excerr << "init:" << __LINE__ 
			   << " getColumnCount exited with status: " << retvalue 
			   << " (ncol: " << colcount <<  ")" 
			   << endl ;
		if (retvalue != SQL_SUCCESS) {
 			UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                			command->getErrorMessage((unsigned char *)es) ;
	    	excerr << "LEN:" << elen << endl ;
	    	excerr << "MSG:\n" << es << endl ;
		}
		
		{ // column headers
			excerr << endl << endl 
				   << "          Legenda" << endl
				   << "  nlen - getColumnNameLength (UINT16) " << endl 
				   << "  name - getColumnName (CHAR)" << endl 
				   << "  type - getColumnType[1] - Type (UINT16)" << endl 
				   << "  subt - getColumnType[2] - SubType (UINT16)" << endl 
				   << "  clen - getColumnLength (UINT32)" << endl 
				   << "  prec - getColumnPrecision (INT16)" << endl 
				   << "  scal - getColumnScale (INT16)" << endl 
				   << "  null - isNullable (BOOL)" << endl 
				   << "  ainc - isAutoIncrement (BOOL)" << endl 
				   << "  rdon - isReadOnly (BOOL)" << endl 
				   << "  sear - isSearchable (BOOL)" << endl 
				   << "  bsiz - isBlobSizeExact (BOOL)" << endl 
				   << endl ;
			excerr << setw(4) << "id" 
				   << " " << setw(4) << "nlen"
				   << " " << setw(24) << "name"
				   << " " << setw(5) << "type"
				   << " " << setw(5) << "subt"
				   << " " << setw(10) << "clen"
				   << " " << setw(6) << "prec"
				   << " " << setw(6) << "scal"
				   << " " << setw(4) << "null"
				   << " " << setw(4) << "ainc"
				   << " " << setw(4) << "rdon"
				   << " " << setw(4) << "sear"
				   << " " << setw(4) << "bsiz"
				   << endl ;
		}					   
		for (UINT16 i=1; i < colcount+1; i++) {
			UINT16 columnNameLength ;
			retvalue = cursor->getColumnNameLength (i, &columnNameLength);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnNameLength returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			char columnName[1024] ;
			retvalue = cursor->getColumnName (i, columnName);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnName returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}
			
			UINT16 columnType, columnSubType ;
			
			retvalue = cursor->getColumnType (i, &columnType, &columnSubType);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnType returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			UINT32 columnLength ;
			retvalue = cursor->getColumnLength (i, &columnLength);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnLength returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			INT16 columnPrecision ;
			retvalue = cursor->getColumnPrecision (i, &columnPrecision);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnPrecision returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}
			
			INT16 columnScale ;
			retvalue = cursor->getColumnScale (i, &columnScale);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " getColumnScale returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}
			
			BOOL columnNullable ;
			retvalue = cursor->isNullable (i, &columnNullable);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " isNullable returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			BOOL columnAutoIncrement ;
			retvalue = cursor->isAutoIncrement (i, &columnAutoIncrement);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " isAutoIncrement returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			BOOL columnReadOnly ;
			retvalue = cursor->isReadOnly (i, &columnReadOnly);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " isReadOnly returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			BOOL columnSearchable ;
			retvalue = cursor->isSearchable (i, &columnSearchable);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " isSearchable returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}
			
			BOOL columnBlobExactSize ;
			retvalue = cursor->isBlobSizeExact (i, &columnBlobExactSize);
			if (retvalue != SQL_SUCCESS) {
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " isBlobSizeExact returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
			}

			{
				excerr << setw(4) << i 
					   << " " << setw(4) << columnNameLength
					   << " " << setw(24) << columnName
					   << " " << setw(5) << columnType
					   << " " << setw(5) << columnSubType
					   << " " << setw(10) << columnLength
					   << " " << setw(6) << columnPrecision
					   << " " << setw(6) << columnScale
					   << " " << setw(4) << (columnNullable ? 'y' : 'n')
					   << " " << setw(4) << (columnAutoIncrement ? 'y' : 'n')
					   << " " << setw(4) << (columnReadOnly ? 'y' : 'n')
					   << " " << setw(4) << (columnSearchable ? 'y' : 'n')
					   << " " << setw(4) << (columnBlobExactSize ? 'y' : 'n')
					   << endl ;
			}


		}

		excerr << endl << "END OF DATA SCAN (i.e. ROW #0)"<< endl << endl  ;

		int record = 1 ;
/*while (record ++)  */
        while (record ++ )
        {
			excerr << "Fetching record # " << record << endl ;
			retvalue = cursor->next ();

			excerr << endl << "cursor->next() method returned with status:" << retvalue << endl ;
            
			if (retvalue != SQL_SUCCESS) {
				if (retvalue == DBXERR_EOF)
					break ;
 				UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                				command->getErrorMessage((unsigned char *)es) ;
				excerr << "init:" << __LINE__ 
					   << " next returned: " << retvalue 
			   		   << " - MSG follow :\n" << es << endl ;
				break ;
			}

			for (UINT16 i=1; i < colcount+1; i++) {
				UINT16 columnType, columnSubType ;

				retvalue = cursor->getColumnType (i, &columnType, &columnSubType);
				if (retvalue != SQL_SUCCESS) {
 					UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                					command->getErrorMessage((unsigned char *)es) ;
					excerr << "init:" << __LINE__ 
						   << " getColumnType returned: " << retvalue 
			   			   << " - MSG follow :\n" << es << endl ;
				}

				switch(columnType) {
				case fldDATETIME:
					{
				 		CTIMESTAMP timestamp ;
						BOOL isNull ;
						retvalue = cursor->getTimeStamp (i, &timestamp, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getTimeStamp returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (CTIMESTAMP) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (CTIMESTAMP) size: "<< timestamp
								   << endl ;
						}
					}
					break ;
				
				case fldBLOB:
					 {
				 		UINT32 blobSize ;
						BOOL isNull ;
						retvalue = cursor->getBlobSize (i, &blobSize, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getBlobSize returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (BLOB) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (BLOB) size: "<< blobSize
								   << endl ;
						}

					 }
					 break ;
				case fldFLOAT:
					 {
				 		DFLOAT data ;
						BOOL isNull ;
						retvalue = cursor->getDouble (i, &data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getDouble returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (DFLOAT) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (DFLOAT) " << setprecision(20) << data
								   << endl ;
						}

					 }
					 break ;
				case fldINT32:
					 {
				 		INT32 data ;
						BOOL isNull ;
						retvalue = cursor->getLong (i, &data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getLong returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (INT32) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (INT32) " << data
								   << endl ;
						}

					 }
					 break ;
				case fldINT16:
					 {
				 		INT16 data ;
						BOOL isNull ;
						retvalue = cursor->getShort (i, &data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getShort returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (INT16) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (INT16) " << data
								   << endl ;
						}

					 }
					 break ;
				case fldBOOL:
					 {
				 		INT16 data ;
						BOOL isNull ;
						retvalue = cursor->getShort (i, &data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getShort returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (BOOL) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (BOOL) " << (data ? 't' : 'f')
								   << endl ;
						}

					 }
					 break ;
                     
                     
				case fldBCD:
				case fldINT64:
				case fldUINT64:
					 {
                     
				 		DFLOAT data ;
						BOOL isNull ;
						FMTBcd bcdData ;
				 		char sdata[1024] ;

						retvalue = cursor->getDouble (i, &data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getDouble returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						retvalue = cursor->getString (i, sdata, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getString returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}
						
						retvalue = cursor->getBcd (i, &bcdData,&isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getBcd returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (DOUBLE) NULL" 
								   << " (CHAR) NULL"
								   << " (BCD) NULL"
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (DOUBLE) " << data
								   << " (CHAR) \"" << sdata << "\"" 
								   << " (BCD) \"" << bcdData << "\"" 
								   << endl ;
						}

					 }
					 break ;
				case fldZSTRING:
					 {
				 		char data[1024] ;
						BOOL isNull ;
						retvalue = cursor->getString (i, data, &isNull);
						if (retvalue != SQL_SUCCESS) {
 							UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                							command->getErrorMessage((unsigned char *)es) ;
							excerr << "init:" << __LINE__ 
								   << " getString returned: " << retvalue 
			   					   << " - MSG follow :\n" << es << endl ;
						}

						if (isNull) {
							excerr << "COLUMN: " << i 
								   << " (CHAR) NULL" 
								   << endl ;
						}
						else {
							excerr << "COLUMN: " << i 
								   << " (CHAR) '" << data << "'"
								   << endl ;
						}

					 }
					 break ;
				default:
					excerr << "COLUMN: " << i 
						   << " type: " << columnType 
						   << " sub: " << columnSubType
						   << " NOT YET IMPLEMENTED "
						   << endl ;
				}
			}
		}
		
		retvalue = cursor->Release() ;
		excerr << "init:" << __LINE__ 
			   << " cursor release returned: " << retvalue 
			   << endl ;
		if (retvalue != SQL_SUCCESS) {
 			UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                			command->getErrorMessage((unsigned char *)es) ;
	    	excerr << "LEN:" << elen << endl ;
	    	excerr << "MSG:\n" << es << endl ;
		}
	
	}

	retvalue = command->Release() ;
	excerr << "init:" << __LINE__ 
		   << " command release returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; command->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		command->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}

	retvalue = conn->disconnect() ;
	excerr << "init:" << __LINE__ 
		   << " disconnect returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}
	
	retvalue = conn->Release() ;
	excerr << "init:" << __LINE__ 
		   << " connection release returned: " << retvalue 
		   << endl ;
	if (retvalue != SQL_SUCCESS) {
 		UINT16 elen = 0 ; conn->getErrorMessageLen(&elen) ; char es[elen+1] ; memset(es,0,elen+1) ;
                		conn->getErrorMessage((unsigned char *)es) ;
	    excerr << "LEN:" << elen << endl ;
	    excerr << "MSG:\n" << es << endl ;
	}

	retvalue = _sqlDriver->Release() ;
	excerr << "init:" << __LINE__ 
		   << " driver release returned: " << retvalue 
		   << endl ;
	
	unsigned long long ll = 0xfedcba9876543210LL;
	excerr << hex << ll << " " << (31 << 8) << endl ;
	long hl[2] ;
	memcpy(&hl, &ll, sizeof(hl)) ;
	excerr << hex << hl[0] << " " << hex << hl[1] << endl ;
	return 0 ;
}


int f(int argc, char **argv)
{
	return init("../libacdbexppgl.so","","getSQLDriverPOSTGRESQL", (argv[1] ? argv[1]: "")) ;
}


int main(int argc, char **argv)
{
	exLogStream log("log", File::getFileName(argv[0])+".log") ;
	try {
		return f(argc, argv) ;
	}
	catch(exException &x) {
		excerr << "MAIN:" << endl ;
		x.notify() ;
		return 1 ;
	}
	catch(exception &x) {
		excerr << "MAIN:" << endl ;
		excerr << x.what() << endl ; ;
		return 1 ;
	}
	catch(...) {
		excerr << "MAIN:" << endl ;
		excerr << "Unknown exception" << endl ;
		return 1 ;
	}
}
