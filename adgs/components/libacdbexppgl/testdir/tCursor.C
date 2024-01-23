/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DBXpress for DB2 test $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2009/01/19 12:57:31  marpas
	CPGLCommand::getRowsAffected method fixed and tested
	
	Revision 1.2  2008/07/07 14:30:01  enrcar
	EC:: 64bit compatibility improved
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	Revision 1.2  2005/12/21 17:39:14  marpas
	cursor improved and tested again
	
	Revision 1.1  2005/12/20 21:43:05  marpas
	command and cursors improved and tested
	
	Revision 1.1  2005/12/19 22:55:26  marpas
	transaction implementation and support almost finished
	
	

*/

#include <testCommon.h>
#include <exStream.h>

using namespace std ;
using namespace acs ;


ostream & operator << (ostream &os, const CTIMESTAMP &t)
{
	stringstream out ;
	out << setw(4) << t.year << '-'
	   << setw(2) << setfill('0') << t.month << '-'
	   << setw(2) << setfill('0') << t.day << ' '
	   << setw(2) << setfill('0') << t.hour << ':'
	   << setw(2) << setfill('0') << t.minute << ':'
	   << setw(2) << setfill('0') << t.second << '.'
	   << setw(3) << setfill('0') << (t.fraction) << setfill(' ') ;
	os << out.str() ;
	return os ;
}
	

exostream & operator << (exostream &os, const CTIMESTAMP &t)
{
	stringstream out ;
	out << setw(4) << t.year << '-'
	   << setw(2) << setfill('0') << t.month << '-'
	   << setw(2) << setfill('0') << t.day << ' '
	   << setw(2) << setfill('0') << t.hour << ':'
	   << setw(2) << setfill('0') << t.minute << ':'
	   << setw(2) << setfill('0') << t.second << '.'
	   << setw(3) << setfill('0') << (t.fraction) << setfill(' ') ;
	os << out.str() ;
	return os ;
}

exostream & operator << (exostream &os, const FMTBcd &t)
{
	stringstream out ;
	out << " operator << FMTBcd: not implemented " ;
	os << out.str() ;
	return os ;
}

ostream & operator << (ostream &os, const FMTBcd &t)
{
	stringstream out ;
	out << " operator << FMTBcd: not implemented " ;
	os << out.str() ;
	return os ;
}

void showCursorResult(SQLCursor *cursor)
{
	static const string funcName = "showCursorDescription" ;
	SQLResult retvalue ;
	if (!cursor) {
		excerr << "NO CURSOR RETURNED" << endl ;
		return ;	
	}
	UINT16 colcount = 0 ;
	retvalue = cursor->getColumnCount(&colcount) ;
	TESTTRCSQLRES(retvalue) << "SQLCursor::getColumnCount() " << colcount << endl ;
	TRCERROR(cursor,retvalue) ;

	for (int record=1; record < 100; record++) {
		SQLResult nextres = cursor->next();
		if (nextres == DBXERR_EOF) 
			break ;
		TESTTRCSQLRES(nextres) << "SQLCursor::next() " << endl ;
		TRCERROR(cursor,nextres) ;

		short colw = 5 ;
		short typw = 10 ;
		short valw = 64 ;
		ostringstream res;
		res <<  "RECORD # " << record << " ---------------------------------------------------------------------------" << endl ;
		res << setw(colw) << "col#" 
			<< " " << setw(typw) << "type"
			<< " " << setw(valw) << "value"
		    << endl ;

		for (UINT16 i=1; i < colcount+1; i++) {
			UINT16 columnType, columnSubType ;

			retvalue = cursor->getColumnType (i, &columnType, &columnSubType);
			TESTTRCSQLRES(retvalue) << "SQLCursor::getColumnType() " << endl ;
			TRCERROR(cursor,retvalue) ;

			switch(columnType) {
			case fldDATETIME:
				{
				 	CTIMESTAMP timestamp ;
					BOOL isNull ;
					retvalue = cursor->getTimeStamp (i, &timestamp, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getTimeStamp() " << endl ;
					TRCERROR(cursor,retvalue) ;
				
					res << setw(colw) << i 
						<< " " << setw(typw) << "CTIMESTAMP" ;
					if (!isNull)
						res << " " << setw(valw) << timestamp ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;
				    break ;
				 }

			case fldBLOB:
				 {
				 	UINT32 blobSize ;
					BOOL isNull ;
					retvalue = cursor->getBlobSize (i, &blobSize, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getBlobSize() " << endl ;
					TRCERROR(cursor,retvalue) ;
					res << setw(colw) << i 
						<< " " << setw(typw) << "BLOB size" ;
					if (!isNull)
						res << " " << setw(valw) << blobSize ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

				    break ;
				 }
			case fldFLOAT:
				 {
				 	DFLOAT data ;
					BOOL isNull ;
					retvalue = cursor->getDouble (i, &data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getBlobSize() " << endl ;
					TRCERROR(cursor,retvalue) ;
					res << setw(colw) << i 
						<< " " << setw(typw) << "DFLOAT" ;
					if (!isNull)
						res << " " << setw(valw) << setprecision(20) << data ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

				    break ;
				 }
			case fldINT32:
				 {
				 	INT32 data ;
					BOOL isNull ;
					retvalue = cursor->getLong (i, &data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getLong() " << endl ;
					TRCERROR(cursor,retvalue) ;
					res << setw(colw) << i 
						<< " " << setw(typw) << "INT32" ;
					if (!isNull)
						res << " " << setw(valw) << data ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

				    break ;
				 }
			case fldINT16:
				 {
				 	INT16 data ;
					BOOL isNull ;
					retvalue = cursor->getShort (i, &data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getShort() " << endl ;
					TRCERROR(cursor,retvalue) ;
					res << setw(colw) << i 
						<< " " << setw(typw) << "INT16" ;
					if (!isNull)
						res << " " << setw(valw) << data ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;
				    break ;
				 }
			case fldBOOL:
				 {
				 	INT16 data ;
					BOOL isNull ;
					retvalue = cursor->getShort (i, &data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getShort() " << endl ;
					TRCERROR(cursor,retvalue) ;
					res << setw(colw) << i 
						<< " " << setw(typw) << "BOOL" ;
					if (!isNull)
						res << " " << setw(valw) << (data ? 't' : 'f') ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;
				    break ;
				 }
			case fldBCD:
				 {
				 	DFLOAT data ;
					BOOL isNull ;
					FMTBcd bcdData ;
				 	char sdata[1024] ;
					retvalue = cursor->getDouble (i, &data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getDouble() " << endl ;
					TRCERROR(cursor,retvalue) ;

					res << setw(colw) << i 
						<< " " << setw(typw) << "DOUBLE" ;
					if (!isNull)
						res << " " << setw(valw) << data ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

					retvalue = cursor->getString (i, sdata, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getString() " << endl ;
					TRCERROR(cursor,retvalue) ;

					res << setw(colw) << i 
						<< " " << setw(typw) << "CHAR" ;
					if (!isNull)
						res << " " << setw(valw) <<  string("'")+sdata+string("'") ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;


					retvalue = cursor->getBcd (i, &bcdData,&isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getBcd() " << endl ;
					TRCERROR(cursor,retvalue) ;

					res << setw(colw) << i 
						<< " " << setw(typw) << "BCD" ;
					if (!isNull)
						res << " " << setw(valw) << bcdData ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

				    break ;
				 }
			case fldZSTRING:
				 {
				 	char data[1024] ;
					BOOL isNull ;
					retvalue = cursor->getString (i, data, &isNull);
					TESTTRCSQLRES(retvalue) << "SQLCursor::getString() " << endl ;
					TRCERROR(cursor,retvalue) ;

					res << setw(colw) << i 
						<< " " << setw(typw) << "CHAR" ;
					if (!isNull)
						res << " " << setw(valw) << string("'")+data+string("'") ;
					else 
						res << " " << setw(valw) << "<nil>" ;
					res	<< endl ;

				    break ;
				 }
			case fldADT:
				 {
                    switch(columnSubType) {
                    case fldstFMTMEMO:
                        {
				 	        char data[1024] ;
					        BOOL isNull ;
					        retvalue = cursor->getString (i, data, &isNull);
					        TESTTRCSQLRES(retvalue) << "SQLCursor::getString() " << endl ;
					        TRCERROR(cursor,retvalue) ;

					        res << setw(colw) << i 
						        << " " << setw(typw) << "ADT" ;
					        if (!isNull)
						        res << " " << setw(valw) << string("'")+data+string("'") ;
					        else 
						        res << " " << setw(valw) << "<nil>" ;
					        res	<< endl ;
                            break ;
                        }
                    case fldstBINARY:
                       {
				 	        UINT32 blobSize ;
					        BOOL isNull ;
					        retvalue = cursor->getBlobSize (i, &blobSize, &isNull);
					        TESTTRCSQLRES(retvalue) << "SQLCursor::getBlobSize() " << endl ;
					        TRCERROR(cursor,retvalue) ;
					        res << setw(colw) << i 
						        << " " << setw(typw) << "BLOB size" ;
					        if (!isNull)
						        res << " " << setw(valw) << blobSize ;
					        else 
						        res << " " << setw(valw) << "<nil>" ;
					        res	<< endl ;
                            break ;
                        }
			        default:
                        {
				            res << setw(colw) << i 
					            << " " << setw(typw) << columnType 
					            << " " << setw(typw) << " sub " 
                                << columnSubType << " NOT YET IMPLEMENTED \n" ;
			            
                            break ;
                        }
                    }
 
				    break ;
				 }
			default:
                {
				    res << setw(colw) << i 
					    << " " << setw(typw) << columnType 
					    << " " << setw(typw) << " sub " 
                        << columnSubType << " NOT YET IMPLEMENTED " ;
                    break ;
                }
			}
		}
		
		excerr << res.str() ;
		if (nextres !=SQL_SUCCESS)
			break ;
	}
}



void showCursorDescription(SQLCursor *cursor)
{
	static const string funcName = "showCursorDescription" ;
	SQLResult retvalue ;
	if (!cursor) {
		excerr << "NO CURSOR RETURNED" << endl ;
		return ;	
	}

	UINT16 colcount = 0 ;
	retvalue = cursor->getColumnCount(&colcount) ;
	TESTTRCSQLRES(retvalue) << "SQLCursor::getColumnCount() " << colcount << endl ;
	TRCERROR(cursor,retvalue) ;

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
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnNameLength()") ;

		char columnName[1024] ;
		retvalue = cursor->getColumnName (i, columnName);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnName()") ;

		UINT16 columnType, columnSubType ;

		retvalue = cursor->getColumnType (i, &columnType, &columnSubType);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnName()") ;

		UINT32 columnLength ;
		retvalue = cursor->getColumnLength (i, &columnLength);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		INT16 columnPrecision ;
		retvalue = cursor->getColumnPrecision (i, &columnPrecision);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		INT16 columnScale ;
		retvalue = cursor->getColumnScale (i, &columnScale);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		BOOL columnNullable ;
		retvalue = cursor->isNullable (i, &columnNullable);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		BOOL columnAutoIncrement ;
		retvalue = cursor->isAutoIncrement (i, &columnAutoIncrement);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		BOOL columnReadOnly ;
		retvalue = cursor->isReadOnly (i, &columnReadOnly);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		BOOL columnSearchable ;
		retvalue = cursor->isSearchable (i, &columnSearchable);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

		BOOL columnBlobExactSize ;
		retvalue = cursor->isBlobSizeExact (i, &columnBlobExactSize);
		TRCERRORFUN(cursor,retvalue,"SQLCursor::getColumnLength()") ;

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

}


int testPrepareCursor(int argc, char **argv) 
{
	static const string funcName = "testPrepareCursor" ;
	SQLResult retvalue ;
	
	SQLDriver *driver = testGetDriver("../libacdbexppgl.so","","getSQLDriverPOSTGRESQL") ;


	retvalue = driver->setOption (eDrvCallBack, (ACS_PTRASINT_T)generalCallback);
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBack)" << endl ;

	retvalue = driver->setOption (eDrvCallBackInfo, (ACS_PTRASINT_T)"callback set by testCursor");
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBackInfo)" << endl ;

	SQLConnection *conn = testGetConnection(driver) ;
	if (conn) {
		SQLCommand *comm = testGetCommand(conn) ; // use defaults

		if (comm) {
			
			string SQLString ;
			for (int i=0; i < argc; i++) {
				SQLString += argv[i] ;
				SQLString += " " ;
				
			}
			
			excerr << "--------------------------------------------" << endl ;
			excerr << SQLString << endl ;
			excerr << "--------------------------------------------" << endl ;
		

			retvalue = comm->prepare ((char *)SQLString.c_str(), 0) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::prepare(\"" <<SQLString << "\",0)" << endl ;
			TRCERROR(comm,retvalue) ;

			SQLCursor *cursor = 0 ;
			
			retvalue = comm->execute (&cursor) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::execute()" << endl ;
			TRCERROR(comm,retvalue) ;

			INT32 nrows ; 
			retvalue = comm->getRowsAffected(&nrows) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getRowsAffected() " << nrows << endl ;
			TRCERROR(comm,retvalue) ;
	
			showCursorDescription(cursor) ;
			showCursorResult(cursor) ;

			if (cursor) {

				retvalue = cursor->Release() ;
				TESTTRCSQLRES(retvalue) << "SQLCursor::Release" << endl ;
				TRCERROR(cursor,retvalue) ;
			}


			retvalue = comm->Release() ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::Release" << endl ;
			TRCERROR(comm,retvalue) ;

		}

		retvalue = conn->disconnect() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->Release() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::Release" << endl ;
		TRCERROR(conn,retvalue) ;
	}

	retvalue = driver->Release() ;
					
	TESTTRCSQLRES(retvalue) << "SQLDriver::Release" << endl ;
	TRCERROR(conn,retvalue) ;
			  	
	return 0 ;
}

int testImmediateCursor(int argc, char **argv)
{
	static const string funcName = "testImmediateCursor" ;
	SQLResult retvalue ;
	
	SQLDriver *driver = testGetDriver("../libacdbexppgl.so","","getSQLDriverPOSTGRESQL") ;


	retvalue = driver->setOption (eDrvCallBack, (ACS_PTRASINT_T)generalCallback);
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBack)" << endl ;

	retvalue = driver->setOption (eDrvCallBackInfo, (ACS_PTRASINT_T)"callback set by testCursor");
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBackInfo)" << endl ;

	SQLConnection *conn = testGetConnection(driver) ;
	if (conn) {
		SQLCommand *comm = testGetCommand(conn) ; // use defaults

		if (comm) {
			
			string SQLString ;
			for (int i=0; i < argc; i++) {
				SQLString += argv[i] ;
				SQLString += " " ;
				
			}
			
			excerr << "--------------------------------------------" << endl ;
			excerr << SQLString << endl ;
			excerr << "--------------------------------------------" << endl ;
		

			SQLCursor *cursor = 0 ;
			
			retvalue = comm->executeImmediate((char *)SQLString.c_str(), &cursor) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::executeImmediate(\"" <<SQLString << "\",0)" << endl ;
			TRCERROR(comm,retvalue) ;

			INT32 nrows ; 
			retvalue = comm->getRowsAffected(&nrows) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getRowsAffected() " << nrows << endl ;
			TRCERROR(comm,retvalue) ;
	

			showCursorDescription(cursor) ;
			showCursorResult(cursor) ;

			if (cursor) {
				retvalue = cursor->Release() ;
				TESTTRCSQLRES(retvalue) << "SQLCursor::Release" << endl ;
				TRCERROR(cursor,retvalue) ;
			}


			retvalue = comm->Release() ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::Release" << endl ;
			TRCERROR(comm,retvalue) ;

		}

		retvalue = conn->disconnect() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->Release() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::Release" << endl ;
		TRCERROR(conn,retvalue) ;
	}

	retvalue = driver->Release() ;
					
	TESTTRCSQLRES(retvalue) << "SQLDriver::Release" << endl ;
	TRCERROR(conn,retvalue) ;
			  	
	return 0 ;
}


int testCursor(int argc, char **argv)
{
	if (argc > 0) {
		if (string(argv[0]) == "-p")
			return testPrepareCursor(argc-1,&argv[1]) ;
		if (string(argv[0]) == "-i") 
			return testImmediateCursor(argc-1,&argv[1]) ;
	}
	excerr << "Use: " << argv[-1] << ": [-p|-i] < SQL statement > " << endl << endl ;
	return 1 ;
}

int main(int argc, char **argv)
{
	try {
		return testCursor(argc-1, &argv[1]) ;
	}
	catch(exException &x) {
		x.notify() ;
		return 1 ;
	}
	catch(exception &x) {
		excerr << x.what() << endl ; ;
		return 1 ;
	}
	catch(...) {
		excerr << "Unknown exception" << endl ;
		return 1 ;
	}
}
