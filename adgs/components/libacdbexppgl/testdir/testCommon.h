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
	Revision 1.2  2012/02/06 10:03:03  marpas
	removing compiler warning
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	Revision 1.4  2005/12/20 21:43:05  marpas
	command and cursors improved and tested
	
	Revision 1.3  2005/12/19 22:55:26  marpas
	transaction implementation and support almost finished
	
	Revision 1.2  2005/12/16 17:35:10  marpas
	tests improved
	
	Revision 1.1  2005/12/15 20:25:31  marpas
	
	ptions on driver/connection still under test
	

*/

#include <dbxpress.hpp>
#include <string>
#include <cstring>

#define TESTTRACE (excerr << "TEST: " << funcName << ":" << __LINE__ << " ")
#define TESTTRCSQLRES(a) (excerr << "TEST: " << funcName << ":" << __LINE__ << (a == SQL_SUCCESS ? " OK(" : " ERR(") << a <<") ") 

#define TRCERROR(c,v)	if (v != SQL_SUCCESS) { \
 							UINT16 elen = 0 ;\
							c->getErrorMessageLen(&elen) ;\
							char es[elen+1] ; memset(es,0,elen+1) ;\
                			c->getErrorMessage(reinterpret_cast<unsigned char *>(es)) ;\
	    					excerr << "TEST: " << funcName << ":" << __LINE__ << " ERRLEN:" << elen << endl ;\
	    					excerr << "TEST: " << funcName << ":" << __LINE__ << " ERRMSG:" << es << endl ;\
						}

#define TRCERRORFUN(c,v,f)	if (v != SQL_SUCCESS) { \
 								UINT16 elen = 0 ;\
								c->getErrorMessageLen(&elen) ;\
								char es[elen+1] ; memset(es,0,elen+1) ;\
                				c->getErrorMessage((unsigned char *)es) ;\
	    						excerr << "TEST: " << funcName << ":" << __LINE__ << f << " ERRLEN:" << elen << endl ;\
	    						excerr << "TEST: " << funcName << ":" << __LINE__ << f << " ERRMSG:" << es << endl ;\
							}


extern SQLResult __stdcall (*getSQLDriver)(char * VendorLib, char * SResourceFile, ppSQLDriver);
typedef SQLResult __stdcall (*getSQLDriverProt)(char * VendorLib, char * SResourceFile, ppSQLDriver);



SQLDriver *testGetDriver(const std::string &dbx_so_path, const std::string &vendorLib, const std::string &function) ;

SQLConnection *testGetConnection(SQLDriver *sqlDriver) ;
SQLCommand *testGetCommand(SQLConnection *, 
						   const std::string &serverName = std::string("CRYOSAT"), 
						   const std::string &userName = std::string("db2inst2"), 
						   const std::string &password = std::string("rubrato")) ;

CBRType generalCallback(CBType ecbType, pVOID pCbInfo)  ;
