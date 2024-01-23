// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA common functions test $


*/
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestFailure.h>
#include <cppunit/tools/XmlElement.h>
#include <cppunit/XmlOutputterHook.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>

#include <RemoteSite.h>
#include <StringUtils.h>
#include <LTACommon.h>
#include <exStackTrace.h>
#include <exMacros.h>
//#include <boost/date_time/posix_time/posix_time.hpp> // PRQA S 1013 2

using namespace std;
using namespace acs;
using namespace curl;
//using namespace boost::posix_time;

class testLTACommon: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE ( testLTACommon);
    CPPUNIT_TEST ( test_uuidV1_bckt);
    CPPUNIT_TEST ( test_compose_remote_site);
    CPPUNIT_TEST ( test_convertOrderStatus);
	CPPUNIT_TEST_SUITE_END ();


public:

    testLTACommon() {
    }

    void setUp();
    void tearDown();

    void test_uuidV1_bckt();
    void test_compose_remote_site();
    void test_convertOrderStatus();
};

CPPUNIT_TEST_SUITE_REGISTRATION ( testLTACommon);

void testLTACommon::setUp() {
}

void testLTACommon::tearDown() {
}

const char *uuids[] = {
"b69403e2-cf05-11e9-bb65-2a2ae2dbcce4",
"b694090a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6940a72-cf05-11e9-bb65-2a2ae2dbcce4",
"b6940ba8-cf05-11e9-bb65-2a2ae2dbcce4",
"b6940ce8-cf05-11e9-bb65-2a2ae2dbcce4",
"b694160c-cf05-11e9-bb65-2a2ae2dbcce4",
"b6941a4e-cf05-11e9-bb65-2a2ae2dbcce4",
"b6941bac-cf05-11e9-bb65-2a2ae2dbcce4",
"b6941cd8-cf05-11e9-bb65-2a2ae2dbcce4",
"b6941e0e-cf05-11e9-bb65-2a2ae2dbcce4",
"b6941f3a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942066-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942250-cf05-11e9-bb65-2a2ae2dbcce4",
"b69426a6-cf05-11e9-bb65-2a2ae2dbcce4",
"b69427f0-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942926-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942a52-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942b7e-cf05-11e9-bb65-2a2ae2dbcce4",
"b6942ca0-cf05-11e9-bb65-2a2ae2dbcce4",
"b69430b0-cf05-11e9-bb65-2a2ae2dbcce4",
"b694320e-cf05-11e9-bb65-2a2ae2dbcce4",
"b694334e-cf05-11e9-bb65-2a2ae2dbcce4",
"b694354c-cf05-11e9-bb65-2a2ae2dbcce4",
"b69436aa-cf05-11e9-bb65-2a2ae2dbcce4",
"b69437d6-cf05-11e9-bb65-2a2ae2dbcce4",
"b6943b46-cf05-11e9-bb65-2a2ae2dbcce4",
"b6943c90-cf05-11e9-bb65-2a2ae2dbcce4",
"b6943dc6-cf05-11e9-bb65-2a2ae2dbcce4",
"b6943f92-cf05-11e9-bb65-2a2ae2dbcce4",
"b69440f0-cf05-11e9-bb65-2a2ae2dbcce4",
"b694423a-cf05-11e9-bb65-2a2ae2dbcce4",
"b694456e-cf05-11e9-bb65-2a2ae2dbcce4",
"b69446ae-cf05-11e9-bb65-2a2ae2dbcce4",
"b69447da-cf05-11e9-bb65-2a2ae2dbcce4",
"b6944906-cf05-11e9-bb65-2a2ae2dbcce4",
"b6944a32-cf05-11e9-bb65-2a2ae2dbcce4",
"b6944df2-cf05-11e9-bb65-2a2ae2dbcce4",
"b694502c-cf05-11e9-bb65-2a2ae2dbcce4",
"b69451bc-cf05-11e9-bb65-2a2ae2dbcce4",
"b69452f2-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945428-cf05-11e9-bb65-2a2ae2dbcce4",
"b694554a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945676-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945b30-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945d06-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945e46-cf05-11e9-bb65-2a2ae2dbcce4",
"b6945f7c-cf05-11e9-bb65-2a2ae2dbcce4",
"b69460a8-cf05-11e9-bb65-2a2ae2dbcce4",
"b69462a6-cf05-11e9-bb65-2a2ae2dbcce4",
"b69463f0-cf05-11e9-bb65-2a2ae2dbcce4",
"b6946760-cf05-11e9-bb65-2a2ae2dbcce4",
"b69468a0-cf05-11e9-bb65-2a2ae2dbcce4",
"b69469cc-cf05-11e9-bb65-2a2ae2dbcce4",
"b6946af8-cf05-11e9-bb65-2a2ae2dbcce4",
"b6946c2e-cf05-11e9-bb65-2a2ae2dbcce4",
"b6946d5a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6946fbc-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947138-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947278-cf05-11e9-bb65-2a2ae2dbcce4",
"b69473a4-cf05-11e9-bb65-2a2ae2dbcce4",
"b69474c6-cf05-11e9-bb65-2a2ae2dbcce4",
"b69475f2-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947714-cf05-11e9-bb65-2a2ae2dbcce4",
"b694793a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947a7a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947bc4-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947cfa-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947e30-cf05-11e9-bb65-2a2ae2dbcce4",
"b6947f5c-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948196-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948376-cf05-11e9-bb65-2a2ae2dbcce4",
"b69484d4-cf05-11e9-bb65-2a2ae2dbcce4",
"b694860a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948740-cf05-11e9-bb65-2a2ae2dbcce4",
"b69488ee-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948c40-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948dda-cf05-11e9-bb65-2a2ae2dbcce4",
"b6948f56-cf05-11e9-bb65-2a2ae2dbcce4",
"b694908c-cf05-11e9-bb65-2a2ae2dbcce4",
"b69491b8-cf05-11e9-bb65-2a2ae2dbcce4",
"b69492f8-cf05-11e9-bb65-2a2ae2dbcce4",
"b6949424-cf05-11e9-bb65-2a2ae2dbcce4",
"b6949780-cf05-11e9-bb65-2a2ae2dbcce4",
"b69498d4-cf05-11e9-bb65-2a2ae2dbcce4",
"b6949a0a-cf05-11e9-bb65-2a2ae2dbcce4",
"b6949b36-cf05-11e9-bb65-2a2ae2dbcce4",
"b694a0c2-cf05-11e9-bb65-2a2ae2dbcce4",
"b694a248-cf05-11e9-bb65-2a2ae2dbcce4",
"b694a630-cf05-11e9-bb65-2a2ae2dbcce4",
"b694a77a-cf05-11e9-bb65-2a2ae2dbcce4",
"b694a8a6-cf05-11e9-bb65-2a2ae2dbcce4",
"b694aa5e-cf05-11e9-bb65-2a2ae2dbcce4",
"b694aba8-cf05-11e9-bb65-2a2ae2dbcce4",
"b694acd4-cf05-11e9-bb65-2a2ae2dbcce4",
"b694ae00-cf05-11e9-bb65-2a2ae2dbcce4",
"b694b1ca-cf05-11e9-bb65-2a2ae2dbcce4",
"b694b30a-cf05-11e9-bb65-2a2ae2dbcce4",
"b694b436-cf05-11e9-bb65-2a2ae2dbcce4",
"b694b5a8-cf05-11e9-bb65-2a2ae2dbcce4",
"b694b6de-cf05-11e9-bb65-2a2ae2dbcce4" } ;

void testLTACommon::test_compose_remote_site() {
	using acs::lta::compose_remote_site ;
	using acs::lta::uuidV1_bckt ;
	
	cout << endl << "*** TEST compose_remote_site ***" << endl << endl;
	
	try {
		std::string bucket = uuidV1_bckt("b69451bc-cf05-11e9-bb65-2a2ae2dbcce4");
		RemoteSite site = compose_remote_site("ftp://user:passw0rd@thehostname:21/this////is/the///unnormalized/path", bucket);
		CPPUNIT_ASSERT(site.getPath() == StringUtils::pathJoin("/this/is/the/unnormalized/path/", bucket));
	}
	catch(exception &e) {
		CPPUNIT_ASSERT(false);
	}
}

void testLTACommon::test_uuidV1_bckt() {
    using acs::lta::uuidV1_bckt ;
    
    cout << endl << "*** TEST uuidV1_bckt ***" << endl << endl;
    try {
        string res = uuidV1_bckt("b694b6de-cf05-11e9-bb65-2a2ae2dbcce4",0) ; 
        CPPUNIT_ASSERT("Null quote" == string("is not valid"));
    }
    catch(exception &e) {
        CPPUNIT_ASSERT(dynamic_cast<exIllegalValueException *>(&e) != nullptr);
    }
    
    try {
        string res = uuidV1_bckt("invalid") ; 
        CPPUNIT_ASSERT("invalid" == string("is not valid"));
    }
    catch(exception &e) {
        CPPUNIT_ASSERT(dynamic_cast<exIllegalValueException *>(&e) != nullptr);
    }
    
    for (size_t i=0; i < sizeof(uuids) / sizeof(uuids[0]); ++i) {
        cout << uuids[i] << "   " << uuidV1_bckt( uuids[i]) << "\n" ;
    }
    
/*
    cout << endl << "*** TEST TRIM ***" << endl << endl;
    string s1 = "  100";
    string res = StringUtils::trim(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "100");
    res = StringUtils::trimBegin(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "100");

    s1 = "  1 2 3 4   ";
    res = StringUtils::trim(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "1 2 3 4");

    s1 = "aaaa  ";
    res = StringUtils::trim(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "aaaa");
    res = StringUtils::trimEnd(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "aaaa");

    s1 = "       ";
    res = StringUtils::trim(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "");
    res = StringUtils::trimBegin(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "");
    res = StringUtils::trimEnd(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "");

    s1 = "aaa aaaa";
    res = StringUtils::trim(s1);
    cout << "From \"" << s1 << "\" to \"" << res << "\"" << endl;
    CPPUNIT_ASSERT(res == "aaa aaaa");
    
    {
        string tbt = " \tthe string \t " ;
        string res ;
        res = StringUtils::trimBegin(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimBegin(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "the string \t ");
        res = StringUtils::trimEnd(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimEnd(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == " \tthe string");
        res = StringUtils::trim(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trim(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "the string");
    }
    
    {
        string tbt = "" ;
        string res ;
        res = StringUtils::trimBegin(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimBegin(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trimEnd(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimEnd(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trim(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trim(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
    }
    {
        string tbt = "the string" ;
        string res ;
        res = StringUtils::trimBegin(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimBegin(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "the string");
        res = StringUtils::trimEnd(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimEnd(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "the string");
        res = StringUtils::trim(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trim(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "the string");
    }
    
    {
        string tbt ;
        string res ;
        res = StringUtils::trimBegin(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimBegin(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trimEnd(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trimEnd(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trim(tbt, StringUtils::is_not_space) ;
        cout << "StringUtils::trim(\"" << tbt << "\", StringUtils::is_not_space): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
    }
    
    {
        string tbt ;
        string res ;
        res = StringUtils::trimBegin(tbt, '/') ;
        cout << "StringUtils::trimBegin(\"" << tbt << "\", '/'): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trimEnd(tbt, '/') ;
        cout << "StringUtils::trimEnd(\"" << tbt << "\", '/'): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
        res = StringUtils::trim(tbt, '/') ;
        cout << "StringUtils::trim(\"" << tbt << "\", '/'): \"" << res << "\"\n" ;
        CPPUNIT_ASSERT(res == "");
    }
*/    
}

void testLTACommon::test_convertOrderStatus() {
    
    cout << endl << "*** TEST convertOrderStatus ***" << endl << endl;

    dbConnectionPool& pool = db::ConnPools::instance()->getPool("");
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
    const std::string& dbKey = conn.getKey();
    using namespace lta;

    LTAStatus *statuses = LTAStatus::instance(dbKey);
	
	//CPPUNIT_ASSERT(false);
    dbPersistent::IdType id = convertOrderStatus(conn, ORDERSTATUS_QUEUED);
    CPPUNIT_ASSERT(id == statuses->id("SUBMITTED"));
    CPPUNIT_ASSERT(ORDERSTATUS_QUEUED == convertOrderStatus(conn, id));
    
    id = convertOrderStatus(conn, ORDERSTATUS_INPROGRESS);
    CPPUNIT_ASSERT(id == statuses->id("RUNNING"));
    CPPUNIT_ASSERT(ORDERSTATUS_INPROGRESS == convertOrderStatus(conn, id));

    id = convertOrderStatus(conn, ORDERSTATUS_COMPLETED);
    CPPUNIT_ASSERT(id == statuses->id("COMPLETED"));
    CPPUNIT_ASSERT(ORDERSTATUS_COMPLETED == convertOrderStatus(conn, id));

    id = convertOrderStatus(conn, ORDERSTATUS_FAILED);
    CPPUNIT_ASSERT(id == statuses->id("ERROR"));
    CPPUNIT_ASSERT(ORDERSTATUS_FAILED == convertOrderStatus(conn, id));

    id = convertOrderStatus(conn, ORDERSTATUS_CANCELLED);
    CPPUNIT_ASSERT(id == statuses->id("CANCELLED"));
    CPPUNIT_ASSERT(ORDERSTATUS_CANCELLED == convertOrderStatus(conn, id));
    
    CPPUNIT_ASSERT(0LL == convertOrderStatus(conn, ORDERSTATUS_INVALIDSTATUS));
    CPPUNIT_ASSERT(ORDERSTATUS_INVALIDSTATUS == convertOrderStatus(conn, 0LL));
}

int main(int argc, char* argv[]) 
{
   CppUnit::TextUi::TestRunner runner;
   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
   runner.addTest( registry.makeTest() );
   runner.setOutputter (new CppUnit::TextOutputter (&runner.result(), std::cerr));
   bool success = runner.run();
   ofstream xmlFileOut(string(argv[0]) + "_Results.xml");
   CppUnit::XmlOutputter xmlOut(&runner.result(), xmlFileOut);
   xmlOut.write(); 
   return success ? 0 : 1;
}
