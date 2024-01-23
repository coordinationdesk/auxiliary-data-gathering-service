/*

	Copyright 2021-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.


 */
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/extensions/TestFactory.h>
#include <cppunit/ui/text/TestRunner.h>

#include <dbLTAUploadInventoryFile.h>
#include <LTAInventoryObject.h>
#include <LTAInventoryObjectOperatorIF.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>
#include <SMPluginLoader.h>

#include <dbConnectionPool.h>
#include <dbAppResSet.h>
#include <File.h>
#include <exException.h>

#include <boost/format.hpp>

using namespace acs;
using namespace std;
using namespace acs::db;


namespace {
	
	const string filetypeName = "fileType_LTA_395";
	const string group = "group2";	
	const string invfilenameRoot = "IsmSMClientTest";
	const string invRootPath = ".";
	const string SMType = "ACSISM";
	const string SMName = "ISM_LTA_395";
    const string dbKey = "" ;
	
	const string SMClob ( "<SMConfigurationRoot>\
        				 	<SMConfiguration>\
                				<!-- ISM Server URL -->\
                				<ServerUrl>localhost</ServerUrl>\
                				<!-- Server port -->\
                				<port>7627</port>\
                				<port2>7625</port2>\
                				<!-- User -->\
                				<user>ismmgr</user>\
                				<!-- password -->\
								<password>12qwas</password>\
                				<group>ismmgr</group>\
                				<permission>666</permission>\
                				<uploadModality>copy</uploadModality>\
                				<downloadModality>copy</downloadModality>\
                				<FtpUser>ismmgr</FtpUser>\
                				<FtpPassword>12qwas</FtpPassword>\
                				<TapeRetrievalTimeOut>905</TapeRetrievalTimeOut>\
        					</SMConfiguration>\
						</SMConfigurationRoot>" );
	
	const string clientReference = "clientReference_LTA_395";
  
}

class TestISMStorageManager: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE ( TestISMStorageManager );
 	CPPUNIT_TEST ( testDownload );
 	CPPUNIT_TEST ( testErase );
 	CPPUNIT_TEST ( testGiveup );
 	CPPUNIT_TEST ( testIsOnCache );
 	CPPUNIT_TEST ( testPutOnCache );
 	CPPUNIT_TEST ( testReleaseCache );
	CPPUNIT_TEST ( testShallRemoveUnknownLinksDefault );
	CPPUNIT_TEST ( testShallRemoveUnknownLinksFalse );
	CPPUNIT_TEST_SUITE_END ();
public:

    TestISMStorageManager() {}
    virtual ~TestISMStorageManager() {} 

    void setUp()    { } // prepare
    void tearDown() { } // cleanup

    // Helpers
    static void prepare();
    static void cleanup();
	
	void testDownload();
	void testErase();
	void testGiveup() ;
	void testIsOnCache() ;
	void testPutOnCache() ;
	void testReleaseCache() ;
	void testShallRemoveUnknownLinksDefault() ;
	void testShallRemoveUnknownLinksFalse() ;

		
private:
		
	static std::string createFile(const off_t & ) ;
	static dbPersistent::IdType uploadInventory(const std::string & invFilepath, dbPersistent::IdType filetypeId, db::LTAInventoryObject &) ;
	static dbPersistent::IdType saveFileType();
	static dbPersistent::IdType saveStorageManager();
	static void deleteFileType() ;
	static void deleteTestSm() ;
	static std::string getInvSmTable();
	static void executeSQLCmd (const std::string&);
	static string randomName();
	
	template <typename T>
	static T getItemFromSelect( const std::string &dbK, const std::string &, bool mustExist = true) ;

	static void executeSQLQuery (const std::string& , dbSet & ) ;
	

private:	
		
	static dbPersistent::IdType _filetypeId;
	static dbPersistent::IdType _smId;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TestISMStorageManager)


};

ACS_CLASS_DEFINE_DEBUG_LEVEL(TestISMStorageManager)

dbPersistent::IdType TestISMStorageManager::_filetypeId = 0;
dbPersistent::IdType TestISMStorageManager::_smId = 0;

template <typename T>
T TestISMStorageManager::getItemFromSelect( const std::string &dbK, const std::string & select, bool mustExist )
{
	dbSet s;
	executeSQLQuery( select, s);
	dbRow r(dbK);
	CPPUNIT_ASSERT ( s.getNextRow(r) == mustExist );

	T item = T();
	if( mustExist ) {
		r.column (0, item);
	}
	
	return item;
}

void TestISMStorageManager::deleteFileType()
{
    string delsql = "DELETE FROM t_filetypes where filetype = '"+ filetypeName +"';";
    executeSQLCmd (delsql); 
	
}

void TestISMStorageManager::deleteTestSm()
{
    string delsql = "DELETE FROM t_storagemanager where name = '"+ SMName +"';";
    executeSQLCmd (delsql); 
	
}


dbPersistent::IdType TestISMStorageManager::saveFileType()
{
	string inFTsql;
    inFTsql += "INSERT INTO t_filetypes (filetype, \"group\") ";
    inFTsql += "VALUES ('"+ filetypeName +"', '" + group +"');";
    executeSQLCmd (inFTsql); 
	
	const string queryFtSql = "SELECT id_filetype from t_filetypes where filetype = '" + filetypeName + "';";
	
	return getItemFromSelect <dbPersistent::IdType> (dbKey, queryFtSql ) ;

}
	
dbPersistent::IdType TestISMStorageManager::saveStorageManager()
{
	string inSMSql = "INSERT INTO t_storagemanager(type, name, parameters, description, invrelationtable) VALUES ('" + SMType+ "', '" + SMName + "', '" + SMClob + "', 'TEST ISM', 't_acsisminvrelation');";
	
    executeSQLCmd (inSMSql); 
	
	const string querySMSql = "SELECT id from t_storagemanager where name = '" + SMName + "';";
	
	return getItemFromSelect <dbPersistent::IdType> (dbKey, querySMSql ) ;
}

void TestISMStorageManager::executeSQLCmd (const string& sql)
{
    dbConnectionWrapper cw(ConnPools::instance()->getPool(dbKey));
    dbConnection& conn = cw;
    
    SQLString sqlstring (conn.getKey(), sql);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following SQL command: \"" << sqlstring.getSQL() << '\"');

    dbQuery query( conn, sqlstring, "executeSQLCmd" );
	if( not query.execForStatus() ) {
		string errorString;
		errorString += "Query Execution error SQL command is \"" ;
		errorString += sqlstring.getSQL() ;
		errorString += "\". Error is \"" + string(query.errorString())  + "\".";
		ACS_THROW ( dbQuery::QueryExecuteException (errorString) ) ;
	}

}

void TestISMStorageManager::executeSQLQuery (const string& sql, dbSet & s)
{
	dbConnectionWrapper cw(ConnPools::instance()->getPool(dbKey));
	dbConnection& conn = cw;

	SQLString sqlstring (conn.getKey(), sql);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following SQL query: \"" << sqlstring.getSQL());
	dbQuery query (conn, sqlstring, "executeSQLQuery");
	query.execForSet(s);
}


string TestISMStorageManager::createFile(const off_t & fileSize)
{
	string invfilepath = File::tempname(StringUtils::pathJoin(invRootPath, invfilenameRoot), "bin");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << invfilepath << "\" temporary file generated.") ;
	
	string ddCommandString;
	{
		boost::format ddCommand("dd if=/dev/urandom of=%s bs=1 count=%d 2> /dev/null");
		ddCommand % invfilepath;
		ddCommand % fileSize;
		ddCommandString = ddCommand.str();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following command: \"" << ddCommandString << '\"');
	int status = system(ddCommandString.c_str());
	if( status != 0 ) {
		ACS_LOG_ERROR( "system function returned exit code " << status << " when executing the command \"" << ddCommandString << '\"');
	}
	CPPUNIT_ASSERT (status == 0 );
	CPPUNIT_ASSERT ( fileSize == File::size(invfilepath) );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << invfilepath << "\" file is " << fileSize << " bytes long" );
	return invfilepath;
}	

string TestISMStorageManager::randomName()
{    
/*
	random pattern is: SM_aaaaaabbbbbcccccc
		SM: Storage Manager
		aaaaaa: seconds from epoch, last 24 bit (resolution: 2^24 seconds, i.e. 194 days)
		bbbbb:  usec 
		cccccc:  24 bit random number
		Total: 3+(6+5+6) = 20 bytes
*/

    struct timeval tv ; // PRQA S 4102
	CPPUNIT_ASSERT(  0 == gettimeofday(&tv, NULL) );
	
	unsigned int r[3] ; // PRQA S 4403 2
    // PRQA S 3000, 3003, 3010, 4400 4
	r[0] = tv.tv_sec & 0x00ffffff ;	// Second from Epoch -- lower 24 bit are used (resolution: 194 days)
	r[1] = tv.tv_usec & 0x000fffff  ;	// usec: range is 0-1e6 i.e. <= 20 bit
	r[2] = rand() & 0x00ffffff; // 24 bit random number

	string rName ;
	{
		boost::format fmt("SM_%06u%05u%06u");
		fmt % r[0] ;
		fmt % r[1] ;
		fmt % r[2] ;
		rName = fmt.str();
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning \"" << rName << '\"') ;

	return rName ;
}


dbPersistent::IdType TestISMStorageManager::uploadInventory(const string & invfilepath, dbPersistent::IdType filetypeId, db::LTAInventoryObject &obj)
{
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

			
	const string invfilename = File::getFileName(invfilepath);	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\"" << invfilename << "\" filename.") ;
	
	obj.setName(invfilename, conn);
	obj.setContentLength(File::size(invfilepath), conn);
	obj.setFileTypeId(filetypeId, conn);
	obj.setValidityFlag(false, conn);
	obj.setContentType("test_contenttype", conn);
	obj.setChecksum("[{\"Value\": \"8294AF9622AA1B7EF0DAC7AA532E00CE\", \"Algorithm\": \"MD5\"}]", conn);
	// set validity start and stop, otherwise an error is issued.
	obj.setValidityStart(DateTime("2020-01-01_09:00:00"), conn);
	obj.setValidityStop(DateTime("2020-01-01_10:00:00"), conn);
	
	dbPersistent::IdType invId = obj.save( conn );

	dbLTAUploadInventoryFile* uploader = dynamic_cast<dbLTAUploadInventoryFile*>(dbFileActionFactoryChain::instance()->newUploadAction(pool, obj, invfilepath)); // PRQA S 3081
	ACS_COND_THROW(!uploader, exIllegalValueException("Cannot obtain a dbUploadFile instance")) ; // PRQA S 3081
	dbFileActionScopeGuard guard(uploader);

	uploader->start() ;

	try { 
		while(uploader->uploading() )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,  "Still uploading \"" << invfilepath << '\"');
			sleep(1) ;
		}

		ACS_LOG_INFO("Upload completed");
	}
	catch( exception &e ) // not expected
	{
		ACS_LOG_NOTIFY_EX( e );
		CPPUNIT_ASSERT( false );
	}
	
	CPPUNIT_ASSERT ( uploader->isComplete() );
	
	string smTable = getInvSmTable(); // t_srv_inventorysm / t_inventorysm
	string sqlString;
	{
		boost::format querySqlFmt("SELECT storagemanid from %s where inv_id = %d ;");
		querySqlFmt % smTable;
		querySqlFmt % invId ;
		sqlString = querySqlFmt.str();
	}
	dbPersistent::IdType SMId = TestISMStorageManager::getItemFromSelect<dbPersistent::IdType>(dbKey, sqlString ) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory " << invId << " linked to Storage manager id=" << SMId);
	CPPUNIT_ASSERT( SMId == _smId );
	
	
	{
		boost::format querySqlFmt("SELECT id from %s where inv_id = %d ;");
		querySqlFmt % smTable;
		querySqlFmt % invId ;
		sqlString = querySqlFmt.str();
	}
	dbPersistent::IdType smInvId = TestISMStorageManager::getItemFromSelect<dbPersistent::IdType>(dbKey, sqlString ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, smTable << " id=" << smInvId) ;
	
	return smInvId ;
	
}


string TestISMStorageManager::getInvSmTable()
{	
	db::LTAInventoryObject obj(dbConnectionWrapper(ConnPools::instance()->getPool(dbKey)));
	return dbFileActionFactoryChain::instance()->getInvSmTableName(obj) ;
}

void TestISMStorageManager::cleanup()
{
	deleteFileType( );
	
	deleteTestSm();
	
}

void TestISMStorageManager::prepare()
{
    cleanup();
	
	const string querySMSql = "SELECT id from t_storagemanager where type = '" + SMType + "';";
	
	dbSet s;
	executeSQLQuery( querySMSql, s);
	dbRow r(dbKey);
	if( not s.getNextRow(r) ) {
		_smId = saveStorageManager();
	}
	else {
		r.column (0, _smId);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SM with type=\"" << SMType << "\" already exists with id = " << _smId) ;
	}
			
	_filetypeId = saveFileType();

}

void TestISMStorageManager::testDownload()
{
	ACS_LOG_INFO("Test of download");
	
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	storage->setClientName(clientReference) ; 

	string filepathToDownload = StringUtils::pathJoin(invRootPath, randomName());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filepathToDownload << "\" local path to download.") ;
	
	FileGuard fgDnl(filepathToDownload);
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->download(smInvId, filepathToDownload, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured during the download of file with of invsm record with id #" << smInvId << " from sm " << storage->SMId() << " on \"" << filepathToDownload << '\"');
		CPPUNIT_ASSERT( true );
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << filepathToDownload << "\" path successfully downloaded" ) ;
	
	
	CPPUNIT_ASSERT( File::computeMd5Sum(filepathToDownload) == File::computeMd5Sum(invfilepath) ) ;

}



void TestISMStorageManager::testErase()
{
	ACS_LOG_INFO("Test of erase");
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		storage->erase(smInvId, obj.getName(conn), "") ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured during the erase of " << getInvSmTable() << " record with id #" << smInvId << " on sm " << storage->SMId() );
		CPPUNIT_ASSERT( true );
	}
	
	dbSmartPersistent rec(getInvSmTable(), conn) ;
	CPPUNIT_ASSERT( not rec.load(conn, smInvId) ) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getInvSmTable() << " record with id=" << smInvId << " erased" ) ;
	
	CPPUNIT_ASSERT( obj.load(conn) ) ;

}

void TestISMStorageManager::testGiveup()
{
	ACS_LOG_INFO("Test of giveup");
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	storage->setClientName(clientReference) ; 

	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->giveup(smInvId, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured during the giveup of " << getInvSmTable() << " record with id #" << smInvId << " on sm " << storage->SMId() );
		CPPUNIT_ASSERT( true );
	}
	

}

void TestISMStorageManager::testIsOnCache()
{
	ACS_LOG_INFO("Test of isOnCache");
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		dbFileAction::CacheStatus cacheStatus = storage->isOnCache(smInvId, oper) ;
		CPPUNIT_ASSERT( dbFileAction::ON_CACHE == cacheStatus ) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when invoking isOnCache on " << getInvSmTable() << " record with id #" << smInvId << " on sm " << storage->SMId() );
		CPPUNIT_ASSERT( true );
	}

}

void TestISMStorageManager::testPutOnCache()
{
	ACS_LOG_INFO("Test of putOnCache");
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		dbFileAction::CacheStatus cacheStatus = storage->putOnCache(smInvId, oper) ;
		CPPUNIT_ASSERT( dbFileAction::ON_CACHE == cacheStatus ) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when invoking putOnCache on " << getInvSmTable() << " record with id #" << smInvId << " on sm " << storage->SMId() );
		CPPUNIT_ASSERT( true );
	}

}

void TestISMStorageManager::testReleaseCache()
{
	ACS_LOG_INFO("Test of releaseCache");
	
	const off_t fileSize = 1024;
	const string invfilepath = createFile(fileSize) ;
	FileGuard fgInv(invfilepath);
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	dbPersistent::IdType smInvId = uploadInventory(invfilepath, _filetypeId, obj);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->releaseCache(smInvId, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when invoking releaseCache on " << getInvSmTable() << " record with id #" << smInvId << " on sm " << storage->SMId() );
		CPPUNIT_ASSERT( true );
	}

}

void TestISMStorageManager::testShallRemoveUnknownLinksDefault()
{
	ACS_LOG_INFO("Test of shallRemoveUnknownLinks (default behaviour)");
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	db::LTAInventoryObject obj( conn );
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << _smId) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, _smId, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << _smId << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		bool shallRemoveUnknownLinks = storage->shallRemoveUnknownLinks() ;
		CPPUNIT_ASSERT(shallRemoveUnknownLinks) ; 
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when invoking shallRemoveUnknownLinks on sm " << storage->SMId() );
		CPPUNIT_ASSERT( false );
	}

}

void TestISMStorageManager::testShallRemoveUnknownLinksFalse()
{
	ACS_LOG_INFO("Test of shallRemoveUnknownLinks false");
	
	dbConnectionPool &pool = ConnPools::instance()->getPool(dbKey) ;
	dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

	const string SMName2 = "ISM_LTA_576";
	
	const string SMClob2 ( "<SMConfigurationRoot>\
        						<SMConfiguration>\
                					<!-- ISM Server URL -->\
                					<ServerUrl>localhost</ServerUrl>\
                					<!-- Server port -->\
                					<port>7627</port>\
                					<port2>7625</port2>\
                					<!-- User -->\
                					<user>ismmgr</user>\
                					<!-- password -->\
									<password>12qwas</password>\
                					<group>ismmgr</group>\
                					<permission>666</permission>\
                					<uploadModality>copy</uploadModality>\
                					<downloadModality>copy</downloadModality>\
                					<FtpUser>ismmgr</FtpUser>\
                					<FtpPassword>12qwas</FtpPassword>\
                					<TapeRetrievalTimeOut>905</TapeRetrievalTimeOut>\
                					<RemoveUnknownLinks>false</RemoveUnknownLinks>\
        						</SMConfiguration>\
							</SMConfigurationRoot>" );

	
	string inSMSql = "INSERT INTO t_storagemanager(type, name, parameters, description, invrelationtable) VALUES ('" + SMType+ "', '" + SMName2 + "', '" + SMClob2 + "', 'TEST ISM', 't_acsisminvrelation');";
	
    executeSQLCmd (inSMSql); 
	
	const string querySMSql = "SELECT id from t_storagemanager where name = '" + SMName2 + "';";
	
	dbPersistent::IdType smId2 =  getItemFromSelect <dbPersistent::IdType> (dbKey, querySMSql ) ;
	
	db::LTAInventoryObject obj( conn );
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMType << " Smid:" << smId2) ;
	SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMType, smId2, &obj, pool); // PRQA S 3000
	SMInterfaceScopeGuard guard(storage);
	if(0 == storage)
	{
		ostringstream os ;
		os << "No storage manager interface for SMid: " << smId2 << " type: " << SMType ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType()  << " Smid:" << storage->SMId()) ;
	
	try
	{
		bool shallRemoveUnknownLinks = storage->shallRemoveUnknownLinks() ;
		CPPUNIT_ASSERT(not shallRemoveUnknownLinks) ; 
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when invoking shallRemoveUnknownLinks on sm " << storage->SMId() );
		CPPUNIT_ASSERT( false );
	}
	
    string delsql = "DELETE FROM t_storagemanager where name = '"+ SMName2 +"';";
	
	try
	{
   		executeSQLCmd (delsql); 
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		ACS_LOG_INFO( "An error occured when deleting t_storagemanager record with name \"" << SMName2 << '\"');
		CPPUNIT_ASSERT( false );
	}
	
	
}



CPPUNIT_TEST_SUITE_REGISTRATION (TestISMStorageManager);

int main(int argc, char ** argv ) 
{
	CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(suite);
	runner.setOutputter(new CppUnit::TextOutputter(&runner.result(), std::cerr));
	exLogStream log("log", StringUtils::pathJoin(".", File::addExt( argv[0], "log") ) ) ;
	
	SMPluginLoader::init();
	std::string dbConfArg = "-dbconf";
	string statisticsArg = "-s" ;
	
	std::string dbConf;
	bool enableStat = false ;
	for (int i=0; i < argc; i++) {
		std::string key = argv[i];
		
		if (dbConfArg == key && i < argc-1) {
			dbConf = argv[++i]; // note advance i
		}
		else if( statisticsArg == key ) {
			enableStat = true ;
		} 
	}

	dbAppResSetFactory::instance(dbConf);
	excerr << "enable statistics = " << boolalpha << enableStat << '\n' ;
	
	TestISMStorageManager::prepare();

	try {
		ACS_LOG_INFO("running test") ;
		bool success = runner.run();

		TestISMStorageManager::cleanup();
		
		string stat ;
    	math::Statistics::instance()->enableStatistics(enableStat) ;
		math::Statistics::getStatistics(stat) ;
		ACS_LOG_INFO(stat) ;
		
		return success ? 0 : 1;
	}
	catch(std::exception &x) {
		ACS_LOG_NOTIFY_EX(x) ;
		TestISMStorageManager::cleanup();
		return 1 ;
	}
}

