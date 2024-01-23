/*
 *
 *  Copyright 2019, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  libCirculationSoap $
 *
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactory.h>
#include <cppunit/ui/text/TestRunner.h>

#include <ConsumableFtpCurl.h>

class TestConsumableFtpCurl : public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE ( TestConsumableFtpCurl );
	CPPUNIT_TEST(testVerifyAndRemoveDirectory);
	CPPUNIT_TEST_SUITE_END();
	
	void testVerifyAndRemoveDirectory();
	void testVerifyAndRemoveWithCheckPreCommandDisabled();
};

void TestConsumableFtpCurl::testVerifyAndRemoveDirectory() {
	StopController sc;
	ConsumableFtpCurl cfc(sc);
	
	RemoteSite site;
}

int main(int argc, char ** argv )  {
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);
	runner.setOutputter(new CppUnit::TextOutputter(&runner.result(), std::cerr));
	exLogStream log("log", File::getFileName(argv[0])+string(".log")) ;

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	for (int i=0; i < argc; i++) {
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; // note advance i
		}
	}

	dbAppResSetFactory::instance(dbConf) ;

	try {
		ACS_LOG_INFO("running test") ;
		bool success = runner.run();
		ACS_LOG_INFO("Going to exit") ;
		return success ? 0 : 1;
	}
	catch(std::exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
		return 1 ;
	}
}
