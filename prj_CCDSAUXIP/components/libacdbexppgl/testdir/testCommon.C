/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 1.6  2013/05/14 18:01:28  marpas
	implementing PostGRES/PostGIS support
	
	Revision 1.5  2012/02/06 10:03:03  marpas
	removing compiler warning
	
	Revision 1.4  2010/11/02 13:33:40  enrcar
	Test improved
	
	Revision 1.3  2009/01/19 12:57:31  marpas
	CPGLCommand::getRowsAffected method fixed and tested
	
	Revision 1.2  2008/07/07 14:22:36  enrcar
	EC:: 64bit compatibility improved
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	Revision 1.5  2005/12/22 18:48:24  marpas
	COSMO is the default database
	
	Revision 1.4  2005/12/20 21:43:05  marpas
	command and cursors improved and tested
	
	Revision 1.3  2005/12/19 22:55:26  marpas
	transaction implementation and support almost finished
	
	Revision 1.2  2005/12/16 17:35:10  marpas
	tests improved
	
	Revision 1.1  2005/12/15 20:25:31  marpas
	
	ptions on driver/connection still under test
	

*/


#include <Plugin.h>
#include <Filterables.h>
#include <testCommon.h>
#include <iomanip>

using namespace acs ;
using namespace std ;


unsigned short testGetCTIMESTAMPFraction(void *so_handle) 
{
	unsigned short *drv_fraction_digits = (unsigned short *)dlsym(so_handle,"ctstmp_fraction_digits") ;
	
	if (drv_fraction_digits)
		return *drv_fraction_digits ;
	return -1 ;
}


SQLDriver *testGetDriver(const string &dbx_so_path, const string &vendorLib, const string &function)
{
	static const string funcName = "testDriver" ;

	Plugin *_so_loader = 0 ;
	try {
		_so_loader=new Plugin(dbx_so_path,false,"",RTLD_NOW|RTLD_GLOBAL) ;

		TESTTRACE << "loading so: " << dbx_so_path << '\n' ;

		void *SQLDllHandle = _so_loader->open() ;

		TESTTRACE << "locating symbol: " << function 
			   << " open error (if any): \"" << _so_loader->openError() << "\""
			   << '\n' ;

		void *GetDriver = dlsym(SQLDllHandle,function.c_str()) ;


		if (!GetDriver) {
			ostringstream os ;
			os << "Unable to find symbol \"" << function << "\"" ;
			exIOException e(os.str(),errno) ; 
			throwException(e) ;
		}

		TESTTRACE << "symbol found : " << GetDriver 
			   << '\n' ;

		getSQLDriverProt _getDriver = (getSQLDriverProt)GetDriver ;

		TESTTRACE << "calling : _getDriver with vendorlib:" << vendorLib
			   << '\n' ;

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
		TESTTRACE << "_getDriver returned: " << _sqlDriver 
			   << '\n' ;
		unsigned short fractionctms = testGetCTIMESTAMPFraction(SQLDllHandle) ;
		TESTTRACE << "checking CTIMESTAMP fraction digits: " <<  fractionctms 
			      << " (" << (fractionctms == 65535 ? "INVALID": "OK")  << ")"
				  << '\n' ;


		return _sqlDriver ;
	}
	catch(...) {
	
		delete _so_loader ;
		throw ;
	}
	
}

SQLConnection *testGetConnection(SQLDriver *driver)
{
	static const string funcName = "testGetConnection" ;

	SQLResult retvalue ;

	retvalue = driver->setOption (eDrvCallBack, reinterpret_cast<ACS_PTRASINT_T>(generalCallback));
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBack)" << '\n' ;

	retvalue = driver->setOption (eDrvCallBackInfo, reinterpret_cast<ACS_PTRASINT_T>("callback set by testGetConnection"));
	TESTTRCSQLRES(retvalue) << "SQLDriver::setOption(eDrvCallBackInfo)" << '\n' ;
	SQLConnection *conn = 0 ;

	retvalue = driver->getSQLConnection(&conn) ;
	TESTTRCSQLRES(retvalue) << "getSQLConnection: " << conn << '\n' ;


	return conn ;
	
}


SQLCommand *testGetCommand(SQLConnection *conn, 
						   const std::string &serverName, 
						   const std::string &userName, 
						   const std::string &password)
{
	static const string funcName = "testGetCommand" ;
	
	INT32 timeout = 1 ;
	SQLResult retvalue = conn->setOption (eConnTimeOut, timeout);
	TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnTimeOut, " << timeout << ")" << '\n' ;
	TRCERROR(conn,retvalue) ;

	retvalue = conn->setOption (eConnCallBack, reinterpret_cast<ACS_PTRASINT_T>(generalCallback));
	TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnCallBack)" << '\n' ;
	TRCERROR(conn,retvalue) ;

	retvalue = conn->setOption (eConnCallBackInfo, reinterpret_cast<ACS_PTRASINT_T>("callback set by testGetCommand"));
	TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnCallBackInfo)" << '\n' ;
	TRCERROR(conn,retvalue) ;

	const char* hostName = "sentinel-sl6" ;
	retvalue = conn->setOption(eConnHostName,reinterpret_cast<ACS_PTRASINT_T>(hostName)) ;
	TESTTRCSQLRES(retvalue) << "SQLConnection::setOption(eConnHostName," << hostName << " \"sentinel-sl6\")" << '\n' ;
	TRCERROR(conn,retvalue) ;

	retvalue = conn->connect(const_cast<char *>("prova_postgis2"), const_cast<char *>("pdsuser"), const_cast<char *>("12qwas")) ;
//	TESTTRCSQLRES(retvalue) << "SQLConnection::connect(\"CRYOSAT\", \"db2inst2\", \"rubrato\")" << '\n' ;
	TRCERROR(conn,retvalue) ;

	SQLCommand *comm =0 ;
	retvalue = conn->getSQLCommand(&comm) ;
	TESTTRCSQLRES(retvalue) << "SQLConnection::getSQLCommand()" << '\n' ;
	TRCERROR(conn,retvalue) ;
	
	return comm ;
}
						   


CBRType generalCallback(CBType ecbType, pVOID pCbInfo) 
{
	static const string funcName = "callback" ;
	TESTTRACE << "----------------------------------" << '\n' ;
	TESTTRACE << "ecbType: " << ecbType << " pCbInfo " << pCbInfo << '\n' ;
	
	TRACEDesc *desc = reinterpret_cast<TRACEDesc *> (pCbInfo) ;
	if (desc) {
		TESTTRACE << "Msg: " << desc->szTrace << '\n' ;
		TESTTRACE << "Cat: " << desc->eTraceCat << '\n' ;
		TESTTRACE << "Inf: " << desc->iCBInfo << " (" << (char *)desc->iCBInfo << ")" << '\n' ;
	}
	TESTTRACE << "----------------------------------" << '\n' ;
	
	return cbrABORT ;
}

