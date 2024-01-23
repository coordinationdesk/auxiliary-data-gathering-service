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
#include <AipParameters.h>
#include <exStackTrace.h>
#include <exMacros.h>
#include <boost/date_time/posix_time/posix_time.hpp> // PRQA S 1013 2

using namespace std;
using namespace acs;
using namespace curl;
using namespace boost::posix_time;
using namespace lta;

class testLTACommon: public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE ( testLTACommon);
	CPPUNIT_TEST (testInitialization);
    CPPUNIT_TEST (testWellFormedParameters);
	CPPUNIT_TEST (testMissingOptionalParameters);
	CPPUNIT_TEST (testMissingMandatoryEndpoint);
	CPPUNIT_TEST (testMissingMandatoryKeyAK);
	CPPUNIT_TEST (testMissingMandatoryKeySK);
	CPPUNIT_TEST (testMissingMandatoryProvider);
	CPPUNIT_TEST (testMissingMandatoryRemoteBasePath);
	CPPUNIT_TEST_SUITE_END ();


public:

    testLTACommon() {
    }

    void setUp();
    void tearDown();

	void testInitialization();
    void testWellFormedParameters();
	void testMissingOptionalParameters();
	void testMissingMandatoryEndpoint();
	void testMissingMandatoryKeyAK();
	void testMissingMandatoryKeySK();
	void testMissingMandatoryProvider();
	void testMissingMandatoryRemoteBasePath();
};

CPPUNIT_TEST_SUITE_REGISTRATION ( testLTACommon);

void testLTACommon::setUp() {
}

void testLTACommon::tearDown() {
}


void testLTACommon::testInitialization() {
	AipParameters params("endpoint", "keyak", "keysk", "provider", "basepath");
	bool set = false;
	CPPUNIT_ASSERT(params.getAwsS3Endpoint() == "endpoint");
	CPPUNIT_ASSERT(params.getAwsS3KeyAK() == "keyak");
	CPPUNIT_ASSERT(params.getAwsS3KeySK() == "keysk");
	CPPUNIT_ASSERT(params.getAwsS3Provider() == "provider");
	CPPUNIT_ASSERT(params.getAwsS3RemoteBasePath() == "basepath");
	params.getDirectDlAip(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3Region(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3UploadPoolSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3UploadBufferSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3DownloadPoolSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3DownloadBufferSize(set) ; CPPUNIT_ASSERT(!set); set = false;

	params.setDirectDlAip(true);
	params.setAwsS3Region("region");
	params.setAwsS3UploadPoolSize(5);
	params.setAwsS3UploadBufferSize(1024000);
	params.setAwsS3DownloadPoolSize(3);
	params.setAwsS3DownloadBufferSize(512000);
	CPPUNIT_ASSERT((params.getDirectDlAip(set) == true) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3Region(set) == "region") && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3UploadPoolSize(set) == 5) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3UploadBufferSize(set) == 1024000) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3DownloadPoolSize(set) == 3) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3DownloadBufferSize(set) == 512000) && set); set = false;
	cout << "[SUCCESS] Test initialization\n";
}

void testLTACommon::testWellFormedParameters() {
	// Test well formed json
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_provider"] = "provider";
	js["aws_s3_region"] = "region";
	js["aws_s3_remotebasepath"] = "basepath";
	js["aws_s3_ul_pool_size"] = 5;
	js["aws_s3_ul_buffer_size"] = 1024000;
	js["aws_s3_dl_pool_size"] = 3;
	js["aws_s3_dl_buffer_size"] = 512000;
	jsRoot["direct_dl_aip"] = true;
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	AipParameters params(jsStr);

	bool set = false;
	CPPUNIT_ASSERT(params.getAwsS3Endpoint() == "endpoint");
	CPPUNIT_ASSERT(params.getAwsS3KeyAK() == "keyak");
	CPPUNIT_ASSERT(params.getAwsS3KeySK() == "keysk");
	CPPUNIT_ASSERT(params.getAwsS3Provider() == "provider");
	CPPUNIT_ASSERT(params.getAwsS3RemoteBasePath() == "basepath");
	CPPUNIT_ASSERT((params.getAwsS3Region(set) == "region") && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3UploadPoolSize(set) == 5) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3UploadBufferSize(set) == 1024000) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3DownloadPoolSize(set) == 3) && set); set = false;
	CPPUNIT_ASSERT((params.getAwsS3DownloadBufferSize(set) == 512000) && set); set = false;
	CPPUNIT_ASSERT((params.getDirectDlAip(set) == true) && set); set = false;
	cout << "[SUCCESS] Test well-formed json parameters\n";
}

void testLTACommon::testMissingOptionalParameters() {
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_provider"] = "provider";
	js["aws_s3_remotebasepath"] = "basepath";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	AipParameters params(jsStr);

	bool set = false;
	CPPUNIT_ASSERT(params.getAwsS3Endpoint() == "endpoint");
	CPPUNIT_ASSERT(params.getAwsS3KeyAK() == "keyak");
	CPPUNIT_ASSERT(params.getAwsS3KeySK() == "keysk");
	CPPUNIT_ASSERT(params.getAwsS3Provider() == "provider");
	CPPUNIT_ASSERT(params.getAwsS3RemoteBasePath() == "basepath");
	params.getAwsS3Region(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3UploadPoolSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3UploadBufferSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3DownloadPoolSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	params.getAwsS3DownloadBufferSize(set) ; CPPUNIT_ASSERT(!set); set = false;
	cout << "[SUCCESS] Test missing optional parameters\n";
}

void testLTACommon::testMissingMandatoryEndpoint() {
	// Endpoint missing
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_provider"] = "provider";
	js["aws_s3_remotebasepath"] = "basepath";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	CPPUNIT_ASSERT_THROW(AipParameters params(jsStr), AipParameters::AipParametersMissingKeyException);

	cout << "[SUCCESS] Test missing mandatory endpoint parameters\n";
}

void testLTACommon::testMissingMandatoryKeyAK() {
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_provider"] = "provider";
	js["aws_s3_remotebasepath"] = "basepath";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	CPPUNIT_ASSERT_THROW(AipParameters params(jsStr), AipParameters::AipParametersMissingKeyException);

	cout << "[SUCCESS] Test missing mandatory keyak parameters\n";
}

void testLTACommon::testMissingMandatoryKeySK() {
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_provider"] = "provider";
	js["aws_s3_remotebasepath"] = "basepath";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	CPPUNIT_ASSERT_THROW(AipParameters params(jsStr), AipParameters::AipParametersMissingKeyException);
	cout << "[SUCCESS] Test missing mandatory keysk parameters\n";
}

void testLTACommon::testMissingMandatoryProvider() {
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_remotebasepath"] = "basepath";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	CPPUNIT_ASSERT_THROW(AipParameters params(jsStr), AipParameters::AipParametersMissingKeyException);
	cout << "[SUCCESS] Test missing mandatory provider parameters\n";
}

void testLTACommon::testMissingMandatoryRemoteBasePath() {
	nlohmann::json js;
	nlohmann::json jsRoot;
	js["aws_s3_endpoint"] = "endpoint";
	js["aws_s3_key_ak"] = "keyak";
	js["aws_s3_key_sk"] = "keysk";
	js["aws_s3_provider"] = "provider";
	jsRoot["aws_s3_parameters"] = js;
	std::string jsStr = jsRoot.dump();

	CPPUNIT_ASSERT_THROW(AipParameters params(jsStr), AipParameters::AipParametersMissingKeyException);
	cout << "[SUCCESS] Test missing mandatory remotebasepath parameters\n";
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

