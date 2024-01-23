/*
	Copyright 2021-2022, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.
	
        
*/
  
#include <LTAInventoryObject.h>
#include <LTAInventoryObjectOperatorIF.h>
#include <dbFileActionFactoryChain.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <dbSmartQuery.h>

#include <dbConnectionPool.h>
#include <dbAppResSet.h>
#include <File.h>
#include <exException.h>

#include <boost/format.hpp>

using namespace std;
using namespace acs;
using namespace acs::db;


void testNotPassed()
{
	excerr << "-----------------------------------------------" << endl ;
	excerr << "Some tests were *** NOT *** passed." << endl ;
	excerr << "Please check and correct/report" << endl;
	excerr << "-----------------------------------------------" << endl ;
}


void testPassed()
{
	excerr << "-----------------------------------------------" << endl ;
	excerr << "Congratulations (to you/or the library author)." << endl ;
	excerr << "All tests are PASSED." << endl ;
	excerr << "-----------------------------------------------" << endl ;
}

dbPersistent::IdType getInvSmTableId(const string & invSmTable, const dbPersistent::IdType invId, dbConnection &conn)
{
	const string &dbKey = conn.getKey() ;
	dbSmartQuery sq(invSmTable, dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, invSmTable, "inv_id", dbParam::EQUAL_TO, db::toSQL(dbKey, invId)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	if( sq.size() == 0 ) {
		ostringstream exMsg ;
		exMsg << "No " << invSmTable << " record occurs with id=" << invId ;
		ACS_THROW( exIllegalValueException(exMsg.str())); // PRQA S 3081
		
	}

	dbPersistent::IdType smInvId = sq.at(0) ->getId();
	excerr << invSmTable << " id=" << smInvId << '\n';
	return smInvId ;
}

int test_upload(int argc, char **argv)
{
	if (argc < 4 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filePath> <storage manager id> <filetypeId> [ -dbconf <confFile> ]\n";
		return -1 ;
	}
		
	string invfilepath(argv[1]) ;
	excerr << "Input filepath to upload \"" << invfilepath << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	dbPersistent::IdType filetypeId = 0;
	{
		istringstream iss ( string(argv[3]), istringstream::in );
		iss >> filetypeId;
	}
	excerr << "filetype id " << filetypeId << '\n';
	
	string dbConfArg = "-dbconf" ;
	string dbConf ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
		}

	}
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

			
	const string invfilename = File::getFileName(invfilepath);	
	
	db::LTAInventoryObject obj( conn );
	obj.setName(invfilename, conn);
	obj.setContentLength(File::size(invfilepath), conn);
	obj.setFileTypeId(filetypeId, conn);
	obj.setValidityFlag(false, conn);
	obj.setContentType("test_contenttype", conn);
	obj.setChecksum("[{\"Value\": \"8294AF9622AA1B7EF0DAC7AA532E00CE\", \"Algorithm\": \"MD5\"}]", conn);
	// set validity start and stop, otherwise an error is issued.
	obj.setValidityStart(DateTime("2020-01-01_09:00:00"), conn);
	obj.setValidityStop(DateTime("2020-01-01_10:00:00"), conn);
	
	obj.save( conn );
	
	SMInterface* storageup = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storageup)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}

	try {
		LTAInventoryObjectOperatorIF oper;
		storageup->upload(invfilepath,oper);
		while(storageup->uploading() )
		{
			excerr << "Still uploading \"" << invfilepath << '\"';
			sleep(2) ;
		}
		excerr << "Upload completed\n";
	}
	catch(exception &) {
		throw ;
	}
	


	return 0;
}


int test_download(int argc, char **argv)
{
	if (argc < 4 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> <downloadPath> [ -dbconf <confFile>] [-c <client name>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename to upload \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string downloadPath(argv[3]) ;
	excerr << "Download path \"" << downloadPath << "\"\n";
	
	string dbConfArg = "-dbconf" ;
	string dbConf ;
	string clientName ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
		}
		else if(string("-c") == argv[i] && i < argc-1) {
			clientName = argv[++i] ; 
		}
	}
	excerr << "Client name \"" << clientName << "\"\n";
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	if( not clientName.empty() ) {
		storage->setClientName(clientName) ; 
	}
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->download(smInvId, downloadPath, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the download of file with of invsm record with id #" << smInvId << " from ISM on \"" << downloadPath << "\"\n";
		throw;
	}


	return 0;
}

int test_erase(int argc, char **argv)
{
	if (argc < 3 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> [ -dbconf <confFile>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename to upload \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
			break;
		}
	}
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->erase(smInvId, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the erase of file with of invsm record with id #" << smInvId << '\n';
		throw;
	}


	return 0;
}


int test_giveup(int argc, char **argv)
{
	if (argc < 3 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> [ -dbconf <confFile>] [-c <client name>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename to upload \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	string clientName ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
		}
		else if(string("-c") == argv[i] && i < argc-1) {
			clientName = argv[++i] ; 
		}
	}
	excerr << "Client name \"" << clientName << "\"\n";
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	if( not clientName.empty() ) {
		storage->setClientName(clientName) ; 
	}
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->giveup(smInvId, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the giveup of file with of invsm record with id #" << smInvId << '\n';
		throw;
	}


	return 0;
}

int test_isOnCache(int argc, char **argv)
{
	if (argc < 3 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> [ -dbconf <confFile>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename to check if on cache \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
			break;
		}
	}
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		dbFileAction::CacheStatus cacheStatus = storage->isOnCache(smInvId, oper) ;
		excerr << "On cache status of file \"" << invfilename << "\" is: " << cacheStatus << '\n';
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the giveup of file with of invsm record with id #" << smInvId << '\n';
		throw;
	}


	return 0;
}


int test_putOnCache(int argc, char **argv)
{
	if (argc < 3 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> [ -dbconf <confFile>] [-c <client name>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename to put \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	string clientName ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
		}
		else if(string("-c") == argv[i] && i < argc-1) {
			clientName = argv[++i] ; 
		}
	}
	excerr << "Client name \"" << clientName << "\"\n";
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	if( not clientName.empty() ) {
		storage->setClientName(clientName) ; 
	}
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		dbFileAction::CacheStatus cacheStatus = storage->putOnCache(smInvId, oper) ;
		excerr << "Put on cache status of file \"" << invfilename << "\" is: " << cacheStatus << '\n';
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the giveup of file with of invsm record with id #" << smInvId << '\n';
		throw;
	}


	return 0;
}

int test_releaseCache(int argc, char **argv)
{
	if (argc < 3 ){
		excerr << "Usage: " <<  argv[0]
			   << " <filename> <storage manager id> [ -dbconf <confFile>]\n";
		return -1 ;
	}
		
	string invfilename(argv[1]) ;
	excerr << "Input filename release from cache \"" << invfilename << "\"\n";
	
	dbPersistent::IdType storageManId = 0;
	{
		istringstream iss ( string(argv[2]), istringstream::in );
		iss >> storageManId;
	}
	excerr << "storage manager id " << storageManId << '\n';

	string dbConfArg = "-dbconf" ;
	string dbConf ;
	for (int i=0; i < argc; i++) { 
		if (dbConfArg == argv[i] && i < argc-1) {
			dbConf = argv[++i] ; 
			break;
		}
	}
	
	dbAppResSetFactory::instance(dbConf) ;
	
	dbConnectionPool &pool = ConnPools::instance()->getPool("") ;
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;
	
	const string &dbKey = conn.getKey() ;
	
	dbSmartQuery sq("t_srv_inventory", dbKey) ;
	{
        dbQueryParameters qp( dbKey);
		qp.addParameter( dbParam(dbKey, "t_srv_inventory", "name", dbParam::EQUAL_TO, db::toSQL(dbKey, invfilename)) ) ;
		sq.addParameter(qp) ;
	}
	sq.doQuery(conn) ;
	ACS_COND_THROW( sq.size() == 0, exIllegalValueException("No t_srv_inventory record occurs with name=\""+invfilename+'\"')); // PRQA S 3081

	dbPersistent::IdType invId = sq.at(0) ->getId();
	db::LTAInventoryObject obj( conn );
	obj.load(conn, invId) ;
	
	dbPersistent::IdType smInvId = getInvSmTableId(dbFileActionFactoryChain::instance()->getInvSmTableName(obj), invId, conn) ;

	SMInterface* storage = SMInterfaceFactoryMethod::getInterface("ACSISM", storageManId, &obj, pool); // PRQA S 3000
	if(!storage)
	{
		ostringstream os ;
		os << "No StorageManager defined for type ACSISM and Smid " << storageManId ;
		ACS_THROW(exIllegalValueException(os.str())); // PRQA S 3081
	}
	
	try
	{
		LTAInventoryObjectOperatorIF oper;
		storage->releaseCache(smInvId, oper) ;
	}
	catch (exception& ex)
	{
		ACS_LOG_NOTIFY_EX(ex);
		excerr << "An error occured during the releaseCache of file with of invsm record with id #" << smInvId << '\n';
		throw;
	}


	return 0;
}


int main(int argc, char **argv)
{

	excerr.notifyDetailFromFilterable(true) ;
	exStackTrace::allow(true) ;

	exLogStream log("log", StringUtils::pathJoin(".", File::addExt( argv[0], "log") ) );
	log.allowStackTrace(true);
	math::Statistics::instance()->enableStatistics(true) ;
	int retCode = 0;
	try
	{

		if (argc < 2 ){
			excerr << "Usage: " <<  argv[0]
				   << " <filePath> [Test specific parameters] [ -dbconf <confFile> ]\n";
			return -1 ;
		}

    	int MenuNumber = -1;
		int res = 0;
    	while (MenuNumber != 0 ) {

        	excerr <<"      ********************** MENU	***************************\n";
        	excerr <<"	0 - Exit\n";
        	excerr <<"	1 - Test Upload\n";
        	excerr <<"	2 - Test Download\n";
        	excerr <<"	3 - Test Erase\n";
        	excerr <<"	4 - Test Giveup\n";
        	excerr <<"	5 - Test isOnCache\n";
        	excerr <<"	6 - Test putOnCache\n";
        	excerr <<"	7 - Test releaseCache\n";
       		excerr <<"  Insert Number ->  ";
    		fflush(stdin);
    		scanf("%d", &MenuNumber );
        	switch(MenuNumber) 
			{
               	case 0:
                    excerr <<" Bye Bye ...\n" ;
                    break ;
                case 1:
    				res = test_upload(argc, argv);
				break;
                case 2:
    				res = test_download(argc, argv);
				break;
                case 3:
    				res = test_erase(argc, argv);
				break;
                case 4:
    				res = test_giveup(argc, argv);
				break;
                case 5:
    				res = test_isOnCache(argc, argv);
				break;
                case 6:
    				res = test_putOnCache(argc, argv);
				break;
                case 7:
    				res = test_releaseCache(argc, argv);
				break;
             	default:
                    excerr <<"Invalid choice: Bye Bye ...\n" ;
                    break ;
        	}

			if( !res ) {
				testPassed();
			}
			else {
				excerr << "Returned code " << res << '\n';
				testNotPassed(); 
			}

		}

    }         
	catch(exception &e)
	{
		excerr << "*********** exception caught****************\n"; 
		excerr.notify(e) ;
		excerr << "**********************************************\n";
		testNotPassed();
		retCode = -1; 
	}
	catch(...)
	{
		excerr << "*********** Unknown exception caught ***********\n"; 
		testNotPassed(); 
		retCode = -1; 
	}
	excerr << "**********************************************\n";
	string dbStatistics ;
	math::Statistics::getStatistics(dbStatistics) ;
	excerr <<dbStatistics << '\n';  

	return retCode ;

}
