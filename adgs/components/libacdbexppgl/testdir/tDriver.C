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
	Revision 1.3  2008/11/20 11:01:23  marpas
	depends on libpq
	
	Revision 1.2  2008/07/07 14:30:01  enrcar
	EC:: 64bit compatibility improved
	
	Revision 1.1.1.1  2007/12/14 18:42:20  marpas
	Import Postgres dbExpress driver
	
	Revision 1.3  2005/12/19 22:55:26  marpas
	transaction implementation and support almost finished
	
	Revision 1.2  2005/12/16 17:35:10  marpas
	tests improved
	
	Revision 1.1  2005/12/15 20:25:31  marpas
	
	ptions on driver/connection still under test
	

*/

#include <testCommon.h>
#include <exStream.h>

using namespace std ;
using namespace acs ;
	

int testDriver() 
{
	static const string funcName = "testDriver" ;
	
	SQLDriver *_sqlDriver = testGetDriver("../../../shared/libacdbexppgl.so","","getSQLDriverPOSTGRESQL") ;
	SQLResult retvalue  ;
	
	retvalue = _sqlDriver->setOption (eDrvCallBack, (ACS_PTRASINT_T)generalCallback);
	TESTTRCSQLRES(retvalue) << "setOption(eDrvCallBack)" << endl ;

	retvalue = _sqlDriver->setOption (eDrvCallBackInfo, 5678);
	TESTTRCSQLRES(retvalue) << "setOption(eDrvCallBack)" << endl ;

	ACS_PTRASINT_T oValuePTR ;
	INT32 oValue ;
	INT16 oLen ;
	retvalue = _sqlDriver->getOption (eDrvBlobSize, &oValue, sizeof(oValue), &oLen) ;
	TESTTRCSQLRES(retvalue) << "SQLDriver::getOption(eDrvBlobSize): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;

	retvalue = _sqlDriver->getOption (eDrvCallBack, &oValuePTR, sizeof(oValuePTR), &oLen) ;
	TESTTRCSQLRES(retvalue) << "SQLDriver::getOption(eDrvCallBack): " << oValuePTR << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;

	retvalue = _sqlDriver->getOption (eDrvCallBackInfo, &oValuePTR, sizeof(oValuePTR), &oLen) ;
	TESTTRCSQLRES(retvalue) << "SQLDriver::getOption(eDrvCallBackInfo): " << oValuePTR << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;

	retvalue = _sqlDriver->getOption (eDrvRestrict, &oValue, sizeof(oValue), &oLen) ;
	TESTTRCSQLRES(retvalue) << "SQLDriver::getOption(eDrvRestrict): " << oValue << " bytelen: " << oLen << (!oLen ? " UNSUPPORTED":"") << endl ;

	retvalue = _sqlDriver->Release() ;
	TESTTRCSQLRES(retvalue) << "SQLDriver::Release" << endl ;
			  	
	return 0 ;
}


int main(int argc, char **argv)
{
	try {
		return testDriver() ;
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
