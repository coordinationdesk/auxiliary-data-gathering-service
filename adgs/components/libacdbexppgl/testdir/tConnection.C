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
	Revision 1.7  2013/05/14 18:01:28  marpas
	implementing PostGRES/PostGIS support
	
	Revision 1.6  2012/02/06 10:03:03  marpas
	removing compiler warning
	
	Revision 1.5  2008/07/07 14:30:01  enrcar
	EC:: 64bit compatibility improved
	
	Revision 1.4  2008/07/02 08:56:25  enrcar
	EC:: option eConnCustomInfo removed since redundant. eConnNativeHandle should be used instead.
	
	Revision 1.3  2008/05/16 10:03:48  enrcar
	minor changes
	
	Revision 1.2  2008/02/18 15:07:01  marpas
	work in progress
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	
	

*/


#include <testCommon.h>
#include <exStream.h>

class NativeConnection : public SQLConnection
{
public:
	virtual void getLastInsertedIdentity (long long &value) = 0 ;
} ; 


using namespace std ;
using namespace acs ;



int testConnection() 
{
	static const string funcName = "testConnection" ;
	SQLResult retvalue ;
	
	SQLDriver *driver = testGetDriver("../../../shared/libacdbexppgl.so","","getSQLDriverPOSTGRESQL") ;


	SQLConnection *conn = testGetConnection(driver) ;
		
	if (conn) {
		retvalue = conn->disconnect() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect" << endl ;
		TRCERROR(conn,retvalue) ;

		INT32 timeout = 1 ;
		retvalue = conn->setOption (eConnTimeOut, timeout);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTimeOut, " << timeout << ")" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->setOption (eConnCallBack, (ACS_PTRASINT_T)generalCallback);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnCallBack)" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->setOption (eConnCallBackInfo, (ACS_PTRASINT_T)"callback set by testConnection");
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnCallBackInfo)" << endl ;
		TRCERROR(conn,retvalue) ;

		INT32 autoCommit = 1 ;
		retvalue = conn->setOption (eConnAutoCommit, autoCommit);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnAutoCommit," << autoCommit << " 1)" << endl ;
		TRCERROR(conn,retvalue) ;

		const char* hostName = "sentinel-sl6" ;
		retvalue = conn->setOption(eConnHostName, (ACS_PTRASINT_T)hostName) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnHostName," << hostName << " \"sentinel-sl6\")" << endl ;
		TRCERROR(conn,retvalue) ;

	    retvalue = conn->connect(const_cast<char *>("prova_postgis2"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::connect(\"prova_postgis2\", \"pdsuser\", \"12qwas\")" << endl ;
		TRCERROR(conn,retvalue) ;

		char ocValue[1024] ;
		strcpy(ocValue,"") ;
        ACS_PTRASINT_T oValuePTR ;
		INT32 oValue ;
		INT16 oLen ;
		retvalue = conn->getOption (eConnAutoCommit, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnAutoCommit): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnBlockingMode, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnBlockingMode): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnBlobSize, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnBlobSize): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnRoleName, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnRoleName): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnWaitOnLocks, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnWaitOnLocks): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnCommitRetain, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnCommitRetain): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnTxnIsoLevel, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnTxnIsoLevel): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

        retvalue = conn->getOption (eConnNativeHandle, &oValuePTR, sizeof(oValuePTR), &oLen) ;
        TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnNativeHandle): " << oValuePTR << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
        TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnServerVersion, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnServerVersion): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnCallBack, &oValuePTR, sizeof(oValuePTR), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnCallBack): " << oValuePTR << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnHostName, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnHostName): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnDatabaseName, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnDatabaseName): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;


		retvalue = conn->getOption (eConnServerPort, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnServerPort): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;


		retvalue = conn->getOption (eConnCallBackInfo, &oValuePTR, sizeof(oValuePTR), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnCallBackInfo): " << oValuePTR << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnObjectMode, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnObjectMode): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnMaxActiveComm, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnMaxActiveComm): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnServerCharSet, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnServerCharSet): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnSqlDialect, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnSqlDialect): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnRollbackRetain, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnRollbackRetain): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnObjectQuoteChar, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnObjectQuoteChar): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		strcpy(ocValue,"") ;
		retvalue = conn->getOption (eConnConnectionName, (ACS_PTRASINT_T*)ocValue, sizeof(ocValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnConnectionName): " << ocValue << " bytelen: " << oLen << (!oLen ? " null string or UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnOSAuthentication, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnOSAuthentication): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnSupportsTransaction, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnSupportsTransaction): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnMultipleTransaction, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnMultipleTransaction): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnServerPort, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnServerPort): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnOnLine, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnOnLine): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnTrimChar, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnTrimChar): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->getOption (eConnTimeOut, &oValue, sizeof(oValue), &oLen) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::getOption(eConnTimeOut): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;
		TRCERROR(conn,retvalue) ;



	/*
		eConnQualifiedName       = 26,
		eConnCatalogName         = 27,
		eConnSchemaName          = 28,
		eConnObjectName          = 29,
		eConnQuotedObjectName    = 30,
	*/



		eXILType xil = xilCUSTOM ;
		retvalue = conn->setOption (eConnTxnIsoLevel, xil);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTxnIsoLevel, " << xil << ")" << endl ;
		TRCERROR(conn,retvalue) ;

		xil = xilREPEATABLEREAD ;
		retvalue = conn->setOption (eConnTxnIsoLevel, xil);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTxnIsoLevel, " << xil << ")" << endl ;
		TRCERROR(conn,retvalue) ;
		xil = xilDIRTYREAD ;
		retvalue = conn->setOption (eConnTxnIsoLevel, xil);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTxnIsoLevel, " << xil << ")" << endl ;
		TRCERROR(conn,retvalue) ;
		xil = xilREADCOMMITTED ;
		retvalue = conn->setOption (eConnTxnIsoLevel, xil);
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTxnIsoLevel, " << xil << ")" << endl ;
		TRCERROR(conn,retvalue) ;


		retvalue = conn->beginTransaction(123) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::beginTransaction(123)" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->rollback(123) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::rollback(123)" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->commit(123) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::commit(123)" << endl ;
		TRCERROR(conn,retvalue) ;

		NativeConnection *nConn = (NativeConnection *)conn ;
		long long id ;
		nConn->getLastInsertedIdentity(id) ;
		excerr << endl << "Last: " << id << endl << endl ;

		retvalue = conn->disconnect() ;

		TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect" << endl ;
		TRCERROR(conn,retvalue) ;

		retvalue = conn->Release() ;

		TESTTRCSQLRES(retvalue) << "SQLConnection::Release" << endl ;
		TRCERROR(conn,retvalue) ;
	}
	

	SQLConnection *conn1 = testGetConnection(driver) ;
	
	if (conn1) {
		
		const char* hostName = "sentinel-sl6" ;
		retvalue = conn1->setOption(eConnHostName,(ACS_PTRASINT_T)hostName) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnHostName," << hostName << " \"sentinel-sl6\")" << endl ;
		TRCERROR(conn,retvalue) ;
		
	    retvalue = conn1->connect(const_cast<char *>("prova_postgis2"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::connect(\"prova_postgis2\", \"pdsuser\", \"12qwas\") (1)" << endl ;
		TRCERROR(conn1,retvalue) ;

		SQLConnection *conn2 = testGetConnection(driver) ;

		if (conn2) {

			const char* hostName = "sentinel-sl6" ;
			retvalue = conn2->setOption(eConnHostName,(ACS_PTRASINT_T)hostName) ;
			TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnHostName," << hostName << " \"sentinel-sl6\")" << endl ;
			TRCERROR(conn,retvalue) ;
		
			retvalue = conn2->connect(const_cast<char *>("prova_postgis2"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
			TESTTRCSQLRES(retvalue) << "SQLConnection::connect(\"prova_postgis2\", \"pdsuser\", \"12qwas\") (2)" << endl ;
			TRCERROR(conn2,retvalue) ;

			retvalue = conn2->disconnect() ;
			TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect (2)" << endl ;
			TRCERROR(conn2,retvalue) ;

			retvalue = conn2->Release() ;
			TESTTRCSQLRES(retvalue) << "SQLConnection::Release (2)" << endl ;
			TRCERROR(conn2,retvalue) ;
		}
		
		retvalue = conn1->disconnect() ;
		TESTTRCSQLRES(retvalue) << "SQLConnection::disconnect (1)" << endl ;

		retvalue = conn1->Release() ;

		TESTTRCSQLRES(retvalue) << "SQLConnection::Release (1)" << endl ;
		TRCERROR(conn1,retvalue) ;
	}
					
			  	
	retvalue = driver->Release() ;
					
	TESTTRCSQLRES(retvalue) << "SQLDriver::Release" << endl ;
			  	
	return 0 ;
}


int main(int argc, char **argv)
{
	try {
		return testConnection() ;
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
