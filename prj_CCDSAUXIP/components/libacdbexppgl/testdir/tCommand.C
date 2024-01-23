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
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	Revision 1.2  2005/12/20 21:43:05  marpas
	command and cursors improved and tested
	
	Revision 1.1  2005/12/19 22:55:26  marpas
	transaction implementation and support almost finished
	
	

*/

#include <testCommon.h>
#include <exStream.h>

using namespace std ;
using namespace acs ;



int testCommand() 
{
	static const string funcName = "testCommand" ;
	SQLResult retvalue ;
	
	SQLDriver *driver = testGetDriver("../../lib/shared/libacdbexpdb2.so","","getDB2Driver") ;


	retvalue = driver->setOption (eDrvCallBack, (ACS_PTRASINT_T)generalCallback);
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBack)" << endl ;

	retvalue = driver->setOption (eDrvCallBackInfo, (ACS_PTRASINT_T)"callback set by testCommand");
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBackInfo)" << endl ;

	SQLConnection *conn = testGetConnection(driver) ;
	if (conn) {
		SQLCommand *comm = testGetCommand(conn) ; // use defaults

		if (comm) {

			char ocValue[1024] ;
			strcpy(ocValue,"") ;
			INT32 oValue ;
			INT16 oLen ;
			retvalue = comm->getOption (eCommRowsetSize, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommRowsetSize): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommBlobSize, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommBlobSize): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommBlockRead, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommBlockRead): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommParamCount, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommParamCount): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommNativeHandle, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommNativeHandle): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			strcpy(ocValue,"") ;
			retvalue = comm->getOption (eCommCursorName, (pINT32)ocValue, sizeof(ocValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommCursorName): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommStoredProc, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommStoredProc): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommSQLDialect, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommSQLDialect): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->getOption (eCommTransactionID, &oValue, sizeof(oValue), &oLen) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::getOption(eCommTransactionID): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
			TRCERROR(comm,retvalue) ;


			string SQLString = "SELECT IDENTITY_VAL_LOCAL() FROM t_ordersqueue FETCH FIRST ROW ONLY ;" ;
			retvalue = comm->prepare ((char *)SQLString.c_str(), 0) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::prepare(\"" <<SQLString << "\",0)" << endl ;
			TRCERROR(comm,retvalue) ;

			retvalue = comm->Release() ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::Release" << endl ;
			TRCERROR(comm,retvalue) ;

		/*	
			// Delphi 7
			eCommPackageName      = 10,
			eCommTrimChar         = 11,
			eCommQualifiedName    = 12,
			eCommCatalogName      = 13,
			eCommSchemaName       = 14,
			eCommObjectName       = 15,
			eCommQuotedObjectName = 16
		*/


		}


		comm = testGetCommand(conn) ; // use defaults

		if (comm) {
			SQLCursor *cursor = 0 ;
			
			string SQLString = "SELECT IDENTITY_VAL_LOCAL() FROM t_ordersqueue FETCH FIRST ROW ONLY ;" ;
			retvalue = comm->executeImmediate ((char *)SQLString.c_str(), &cursor) ;
			TESTTRCSQLRES(retvalue) << "SQLCommand::executeImmediate(\"" <<SQLString << "\",cursor)" << endl ;
			TRCERROR(comm,retvalue) ;
			
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


int main(int argc, char **argv)
{
	try {
		return testCommand() ;
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
