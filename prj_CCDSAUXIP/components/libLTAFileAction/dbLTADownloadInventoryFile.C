// PRQA S 1050 EOF
/*

	Copyright 2020-2022, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
*/


#include <dbLTADownloadInventoryFile.h>
#include <LTAInventoryObjectOperatorIF.h>
#include <dbFileActionFactoryChain.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConnectionPool.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <MCFilterables.h>
#include <LTAStat.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace acs::mc ;
 

using acs::math::StatNode ;

namespace {
    StatNode &statN() { return lta::stat()["Download"] ; }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLTADownloadInventoryFile)

// constructor
dbLTADownloadInventoryFile::dbLTADownloadInventoryFile(dbConnectionPool &pool, db::LTAInventoryObject &s, const string &path, bool downloadInvalidFile) :
	dbDownloadFile(pool, s,path) ,  
	_inventory(s) , // PRQA S 2528 
	_path(path),
	_preconditionPassed(false) ,
	_downloadInvalidFile(downloadInvalidFile),
	_inventoryName( s.getName( dbConnectionWrapper(pool) ) ),
	_simulateUnknownFileException(false),
	_simulateNearlineException(false),
	_simulateOfflineException(false)
{
	statN()+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileName: " << _inventoryName 
		    << " Path: " << _path ) ;
}

void dbLTADownloadInventoryFile::start() 
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	ACS_COND_THROW(isComplete(), dbPersistent::InvalidUse("File has been already downloaded.")) ; // PRQA S 3081 2
    ACS_COND_THROW(running(), dbPersistent::InvalidUse("Thread is already running.")) ;

    Thread::start();
}


bool dbLTADownloadInventoryFile::downloading()  // PRQA S 4020 
{
	if( running() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Still downloading.") ;
		
		return true;
	}
	
	std::shared_ptr<const std::exception> e = runException() ;
	if(e.get())
	{
		if (!_preconditionPassed)
		{
			// APF-270
			ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
							 mc_param("message", "Some precondition failed") <<
							 mc_metric(Metric_FAIL) << 
							 MCCode ("ARC") << MCFunction ("Archive")) ;
			ACS_THROW(PreconditionException (*e,"Error in downloading thread: some precondition failed")) ; //  PRQA S 3081
		}

		if( not isComplete() ) 
		{
			
			if ( dynamic_cast<const FileNotDownloadableException*>   (e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								 mc_param("message", "Definitive error in downloading thread: file not downloadable") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				
				ACS_THROW(FileNotDownloadableException(*e,"Definitive error in downloading thread")) ; // PRQA S 3081
			}
			
			if ( dynamic_cast<const SMUnknownFileException*>   (e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								 mc_param("message", "Definitive error in downloading thread: file unknown") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(SMUnknownFileException(*e,"Definitive error in downloading thread")) ; // PRQA S 3081
			}
			
			if ( dynamic_cast<const FileToDownloadIsOfflineExc*>   (e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								 mc_param("message", "Definitive error in downloading thread: file is offline") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(FileToDownloadIsOfflineExc(*e,"Definitive error in downloading thread")) ; // PRQA S 3081
			}

			if ( dynamic_cast<const FileToDownloadIsNearlineExc*>   (e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								 mc_param("message", "Definitive error in downloading thread: file is near-online") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(FileToDownloadIsNearlineExc(*e,"Definitive error in downloading thread")) ; // PRQA S 3081
			}

			// APF-270
			ACS_LOG_MC_INFO( mc_param("filename", _inventoryName)  << // PRQA S 3081
							 mc_param("message", "Error in downloading thread") <<
							 mc_metric(Metric_FAIL) << 
							 MCCode ("ARC") << MCFunction ("Archive")) ;
			ACS_THROW(DownloadException(*e,"Error in downloading thread")) ; // PRQA S 3081

		}

		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) <<  // PRQA S 3081
						 mc_param("message", "Unexpected exception got during download") <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("ARC") << MCFunction ("Archive")) ;
        ACS_THROW(dbFileAction::Exception(*e,"Unexpected exception")) ; // PRQA S 3081		
	}

	
	if(isComplete())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "done") ;
	}
	return false;
}

void dbLTADownloadInventoryFile::run()
{
    try {
        statN()["started"] += 1 ;
        Timer timer ;
        timer.start() ;

	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	    ACS_LOG_PRIVINFO("Start downloading file \"" <<_inventoryName << "\" in path \"" << _path << '\"') ;

        const string &dbKey = getPool().getKey() ;

        // Every connection is gathered from the pool and released ASAP

        { // Connectionwrapper scope
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050

	        ACS_COND_THROW( // PRQA S 3081
                ( (not _inventory.isFromDatabase()) and ( not _inventory.load(theConnection) ) ),
                PreconditionException("Error downloading file: inventory \"" + _inventoryName + "\" can't be found in database.")
            ) ;
        } // Connectionwrapper scope

	    ACS_COND_THROW(  // PRQA S 3081
            ( ( not _downloadInvalidFile ) && ( not _inventory.getValidityFlag( dbConnectionWrapper(getPool()) ) ) ),
            PreconditionException("Cannot download inventory: validity flag is false.") 
        ) ;

	    ACS_COND_THROW(  // PRQA S 3081
            File::exists(_path), 
	        PreconditionException("Error downloading a file to \"" +  File::getDirPath(_path) + "/" +_inventoryName + "\" because it exists.")
        ) ;


	    _preconditionPassed=true;

	    bool canGetFromSynchSM = false;
	    {
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050
		    dbPersistent * recordPtr = &_inventory ;
		    LTAInventoryObjectOperatorIF oper;
		    canGetFromSynchSM = oper.canGetFromSynchSM(*recordPtr, theConnection);
	    }
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file " << _inventoryName << " can" << (canGetFromSynchSM ? " " : " NOT " ) << "be downloaded from synchronous SM") ; // PRQA S 3380
	    if( not canGetFromSynchSM ) {
		    invalidateInventory();
		    ACS_THROW( FileNotDownloadableException( '\"'+_inventoryName+"\" file not downloadable from synchronous SM(s)") );
	    }


        // Container for the exception that has to be notified and thrown
        // if the download is not successful
        const string dexMessage = "Error downloading file \"" + _inventoryName + '\"' ;
        ACS_SMARTPTR<DownloadException> dex(new DownloadException(dexMessage)); 


	    // It returns synchronous SMs where file is, ordered by retrieve cost ASC
	    const string invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory);
	    SQLString sqlString = getSyncSMsForDownloadQuery(invSmTableName, dbKey);
	    dbSet smset;
	    {
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050

		    dbQuery query(theConnection,sqlString, "FADWNSEL prior, cost") ;
		    query.execForSet(smset) ;
        }
	    const size_t smsetsize = smset.size() ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file \"" << _inventoryName << "\" can be downloaded from " << smsetsize << " synchronous SMs.") ;

	    string abspath = File::getAbsolutePath(_path);
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_path: \"" << _path << "\"" << " abspath: " << abspath << '\"') ;

	    // Exception counters to detect UnknownFileException
        // and generic download exception thrown by the SMs
        int genericExceptionCounter (0);
	    int nearLineExcCounter(0);
	    int offlineExcCounter(0);
		int unknownInventorysmCounter (0); // just for debug
	
	    bool downloaded=false ;
	    for (size_t ri=0; (ri < smsetsize) && (not downloaded); ++ri) {

		    dbPersistent::IdType storageManId = 0 ;
			bool removeUnknownLinks = true ;
		    try {
			    dbRow row(getPool().getKey()) ;
			    smset.getNextRow(row);

 			    unsigned short ci = 0;
			    row.column(ci++,storageManId);
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying smid: " << storageManId) ;

			    dbPersistent::IdType id = 0 ;
			    row.column(ci++,id);

			    string smtype;
			    row.column(ci++,smtype);

			    SMInterface* storage= SMInterfaceFactoryMethod::getInterface(smtype, storageManId, &_inventory, getPool()); // PRQA S 3000
			    SMInterfaceScopeGuard guard(storage);
			    if(!storage)
			    {	
				    ostringstream os ;
				    os << "No storage got for smid: " << storageManId << " type : " << smtype ;
				    ACS_THROW( DownloadException (os.str()) );
			    }
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got SMInterface for smid: " << storage->SMId()
					    << " type: " << storage->SMType()) ;

                storage->setClientName(getClientName()) ; // set the client name for the current action

				removeUnknownLinks = storage->shallRemoveUnknownLinks() ;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "removeUnknownLinks: " << boolalpha << removeUnknownLinks ) ;
				
			    bool isSet;
			    off_t fileSize = _inventory.getContentLength( dbConnectionWrapper(getPool()), &isSet );
			    if( not isSet ) {
				    fileSize = 0;
			    }

			    if( isSimulated() ) {
				    simulateDownload(_inventory, abspath, fileSize);
			    }
			    else {
				    LTAInventoryObjectOperatorIF oper;
				    storage->download(id,abspath,oper);

				    // APF-270
				    ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								     mc_param("filesize", fileSize ) << mc_metric(Metric_OUT) << 
								     MCCode ("ARC") << MCFunction ("Archive")) ;

			    }

			    downloaded=true ; // signal loop exit because downloaded

		    }
   		    catch(dbDownloadFile::SMUnknownFileException &ex)
		    {

				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
							     mc_param("equipmentid", storageManId) <<
							     mc_param("message", "Storage Manager reports the file as Unknown") <<
							     mc_metric(Metric_FAIL) << 
							     MCCode ("ARC") << MCFunction ("Archive")) ;
				
				ACS_LOG_WARNING("Product " + _inventoryName + " UNKNOWN to StorageManagerId: " + std::to_string(storageManId)+". Trying with next SM, if any");
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ACS_LOG_NOTIFY_EX( ex);
				ACS_CLASS_END_DEBUG
				
				unknownInventorysmCounter++ ;
				if( removeUnknownLinks ) {
					try {// remove t_inventorysm rows
        				dbConnectionWrapper pw(getPool()) ;
        				dbConnection &theConnection = pw ; // PRQA S 3050
						deleteUnknownInventorysm(storageManId, dbKey, invSmTableName, theConnection);
					}
					catch(exception &ee)
					{
						ACS_LOG_NOTIFY_EX(ee) ;
						ACS_LOG_ERROR( "Error while trying to remove UNKNOWN " << invSmTableName << " row for SM id=" << storageManId << " and inventory id=" << _inventory.getId() ) ;
					}
				}
		    }
   		    catch(dbDownloadFile::FileToDownloadIsNearlineExc &ex)
		    {
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
					mc_param("equipmentid", storageManId) <<
					mc_param("message", "Storage Manager reports the file as near-online") <<
					mc_metric(Metric_FAIL) << MCCode ("ARC") << MCFunction ("Archive")) ;

				ACS_LOG_WARNING("Product " + _inventoryName + " is NEARLINE on StorageManagerId: " + std::to_string(storageManId)+". Trying with next SM, if any");
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
				nearLineExcCounter++;
		    }
		    catch(dbDownloadFile::FileToDownloadIsOfflineExc &ex)
		    {
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
					mc_param("equipmentid", storageManId) <<
					mc_param("message", "Storage Manager reports the file as offline") <<
					mc_metric(Metric_FAIL) << MCCode ("ARC") << MCFunction ("Archive")) ;

				ACS_LOG_WARNING("Product " + _inventoryName + " is OFFLINE on StorageManagerId: " + std::to_string(storageManId)+". Trying with next SM, if any");
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex);
				offlineExcCounter++;

		    }
		    catch (exception &ex)
		    {
				ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
							     mc_param("equipmentid", storageManId) <<
							     mc_param("message", "Storage Manager reports a generic error during the download") <<
							     mc_metric(Metric_FAIL) << 
							     MCCode ("ARC") << MCFunction ("Archive")) ;

				ACS_LOG_WARNING("Generic exception caught from SM id " << storageManId) ;

				dex = ACS_SMARTPTR<DownloadException>(new DownloadException (ex, dexMessage)) ; // PRQA S 3050, 3081
				genericExceptionCounter++;

				ACS_LOG_NOTIFY_EX(ex) ;
				ACS_LOG_WARNING("Trying with next SM, if any") ;
		    }

	    } // for (size_t i=0; i < smsetsize; ++i)

   	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exception counters: SMUnknownFileException [" << unknownInventorysmCounter << "], FileToDownloadIsOfflineExc ["<< offlineExcCounter << "], FileToDownloadIsNearlineExc ["
	    << nearLineExcCounter <<  "], std::exception [" << genericExceptionCounter << ']') ;

	    if( not downloaded ) {

		    if( 0 == genericExceptionCounter &&  0 == nearLineExcCounter && 0 == offlineExcCounter) { // SMUnknownFileException > 0

			    try {
				    invalidateInventory();
			    }
			    catch(exception &e)
			    {
				    ACS_THROW( DownloadException(e, "Error while trying to invalidate \""+ _inventoryName+"\" inventory") );
			    }

			    ACS_THROW( SMUnknownFileException( '\"'+_inventoryName+"\" file reported as unknown from synchronous SM(s)") );
		    }
		    else { // genericExceptionCounter || nearLineExcCounter || offlineExcCounter > 0
		    	if (nearLineExcCounter != 0)
			{
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileToDownloadIsNearlineExc ");
			    ACS_THROW( FileToDownloadIsNearlineExc( '\"'+_inventoryName+"\" file reported as near-online from synchronous SM(s)") );
			}
			else if (offlineExcCounter != 0 )
			{
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileToDownloadIsOfflineExc ");
			    ACS_THROW( FileToDownloadIsOfflineExc( '\"'+_inventoryName+"\" file reported as offline from synchronous SM(s)") );
			}
			else
			{
			    if (dex.get()) { 
				    ACS_THROW(*dex); 
			    }
			    else {
			    // it should not occur!!!!
				    ACS_THROW( DownloadException("***UNEXPECTED*** Some generic exceptions caught. " + dexMessage) ); // PRQA S 3081
			    }
		    	}
		}
	    }  		

	    ACS_LOG_PRIVINFO("Finished to download file \"" <<_inventoryName << "\" in path " << _path) ;

	    setCompleted(true) ;

	    timer.stop() ;

        statN()["success"] += 1 ;
        (statN()["elapsed [ms]"])+=double(timer.elapsed()); // PRQA S 3081
    }
    catch (exception &) {
        statN()["error"] += 1 ;
        throw ;
    }
}


void dbLTADownloadInventoryFile::simulateDownload(db::LTAInventoryObject &obj, const string &path, const off_t & fileSize)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
		
// It replicates the behaviour of ISM Client lib
	ACS_COND_THROW( File::exists(path), DownloadException("\"" + path + "\" file already exists.") ) ; // PRQA S 3081 2

// simulated exception	
	ACS_COND_THROW( _simulateUnknownFileException, dbDownloadFile::SMUnknownFileException("SMUnknownFileException simulated for \"" + path + "\".") ) ; // PRQA S 3081 2
	ACS_COND_THROW( _simulateNearlineException, dbDownloadFile::FileToDownloadIsNearlineExc("FileToDownloadIsNearlineExc simulated for \"" + path + "\".") ) ; // PRQA S 3081 2
	ACS_COND_THROW( _simulateOfflineException, dbDownloadFile::FileToDownloadIsOfflineExc("FileToDownloadIsOfflineExc simulated for \"" + path + "\".") ) ; // PRQA S 3081 2
			 
	// get a random number between 0 and 1
    srand(time(0)) ; // random seed // PRQA S 3000, 3010
	bool randomBehaviour = (0 != rand()%2) ; // remainder: 0 or 1 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Random behaviour: " << boolalpha << randomBehaviour) ;
	if( randomBehaviour ) {
						
		double transferRate = 10485760.0 ;// 10 MB/s
		Timer::Delay msecsToWait = 1000 * (fileSize / transferRate) ; // PRQA S 3011, 3084
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to wait " << msecsToWait << " ms (file size " << fileSize/1048576.0 << " [MB]; transfer rate "  // PRQA S 3011, 3084
				<< transferRate/1048576.0 << " [MB/s]) to simulate download ") ;

		Timer timer ;
		timer.start();
		while( timer.stop(), timer.elapsed() < msecsToWait ) {
			Timer::delay(10) ; // delay of 10 ms // PRQA S 4400
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Simulated transfer is over: " << timer.elapsed() << " [ms] elapsed") ;
					
	}

	// create dummy file
	ofstream xmlSimulatedFile; 
	xmlSimulatedFile.open(path.c_str());
	ACS_COND_THROW(!xmlSimulatedFile, exFileOpenException("Unable to open \""+path + "\" file") );	 // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to write \"" << path << "\" simulated file") ;

 	XMLOstream xmlOstr(xmlSimulatedFile,"SimulatedFile");
	XMLOstream::Tag tag(xmlOstr, "Filepath", path,"",true);
	
}


SQLString dbLTADownloadInventoryFile::getSyncSMsForDownloadQuery(const string & invSmTableName, const string& dbKey) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

// SELECT StorageManId, id, type
// FROM (
// SELECT ism.StorageManId, ism.id, sm.type, p.retrievecost,  
// (select count (*) from t_storagepriority where t_storagepriority.id = ism.StorageManID ) as policycount , 
// sm.allowdefaultcost as adc  
// FROM t_inventorysm ism 
// INNER JOIN t_storagemanager sm on sm.id = ism.StorageManId AND sm.syncdownload = 't'
// LEFT OUTER JOIN t_storagepriority p on p.id=ism.StorageManId AND p.filetype = 'MSI_L1C_DS' 
// WHERE ism.inv_id = 1356 
// ) AS inner_table 
// WHERE inner_table.policycount = 0 
// OR (retrievecost IS NOT NULL AND retrievecost >= 0) 
// OR (retrievecost IS NULL AND adc = 't')
// order by CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END ASC

	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	sqlString.addSelectList("StorageManId");
	sqlString.addSelectList("id");
	sqlString.addSelectList("type");

	const string runTimeTable("inner_table");
	{
		SQLString nestedSelect( dbKey, SQLString::SELECT ) ;

		nestedSelect.addSelectList("ism.StorageManId");
		nestedSelect.addSelectList("ism.id");
		nestedSelect.addSelectList("sm.type");
		nestedSelect.addSelectList("p.retrievecost");

		SQLString innerSelect( dbKey, SQLString::SELECT ) ;
		SQLString tmp(  dbKey, SQLString::FUNCTION ) ;
        tmp.setFunctionName("COUNT"); tmp.addFunctionParameter("*");
		innerSelect.addSelectList(tmp);
		innerSelect.addTableList("t_storagepriority");
		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "t_storagepriority" ,"id" , dbParam::EQUAL_TO , "ism.StorageManID" )) ;
			innerSelect.addANDWhereParameter(pp) ;
		}

		nestedSelect.addSelectList(innerSelect, "policycount");
        nestedSelect.addSelectList("allowdefaultcost", "adc"); 

		nestedSelect.addTableList(invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "sm","syncdownload" , dbParam::EQUAL_TO, db::toSQL(dbKey, true) )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
		}
		{
			LTAInventoryObjectOperatorIF oper;
			string filetype = oper.getFileType(_inventory);
			ACS_COND_THROW( filetype.empty(), DownloadException( "Filetype not defined for t_srv_inventory with name \""+_inventoryName+'\"') );
			
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "p","id" , dbParam::EQUAL_TO , "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "p","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, filetype) )) ;
			nestedSelect.addJoinList(SQLDialect::LEFT,"t_storagepriority p",pp) ;
		}

		// Add Where
		{
    		dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, "ism" ,"inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId()) )) ;
			nestedSelect.addANDWhereParameter(pp) ;
		}

		sqlString.addTableList(nestedSelect.getSQL(true), runTimeTable );

 	}

	// Add Where
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"policycount" , dbParam::EQUAL_TO , db::toSQL(dbKey, 0) )) ;
		sqlString.addANDWhereParameter(pp) ;
	}

	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNOTNULL )) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, 0) )) ;
		sqlString.addORWhereParameter(pp) ;
	}

	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"retrievecost" , dbParam::ISNULL )) ;
		pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
		sqlString.addORWhereParameter(pp) ;
	}

	sqlString.addOrderByList("CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END", SQLString::ASC) ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlString.getSQL() );
	
	return sqlString;

}


void dbLTADownloadInventoryFile::deleteUnknownInventorysm( dbPersistent::IdType unknownInventorysmId, const string& dbKey, const string & table, dbConnection &theConnection) const // PRQA S 4020
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	SQLString sqlDelete(dbKey,  SQLString::DELETE) ;
	sqlDelete.addTableList(table) ;
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, table, "inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId())) ) ;
		pp.addParameter( dbParam(dbKey, table, "StorageManId", dbParam::EQUAL_TO, db::toSQL(dbKey, unknownInventorysmId) ) );
		sqlDelete.addANDWhereParameter(pp);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following command: " << sqlDelete.getSQL()) ;
	
	dbQuery query(theConnection,sqlDelete, "Delete unknown " + table ) ;
	
	if( not query.execForStatus() ) {
		string ErrorString ;
		ErrorString += "Query Execution error SQL command is \"" ;
		ErrorString += sqlDelete.getSQL() ;
		ErrorString += "\". Error is \"" + query.errorString()  + "\".";

		ACS_THROW (dbQuery::QueryExecuteException(ErrorString)) ; // PRQA S 3081
	}

}

void dbLTADownloadInventoryFile::invalidateInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	bool canInvalidate (true);
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    if ( conf.hasKey ("DownloadInventoryObject.CanInvalidateFile") ) {
        conf.getValue ("DownloadInventoryObject.CanInvalidateFile", canInvalidate);
    }
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inventory record will be " << ( canInvalidate ? "" : "NOT " ) << "invalidated");
	
	bool inventoryInvalidated = false ;
    if (canInvalidate)
    {           
        // This file cannot be downloaded anymore... it is time to invalidate it
        ACS_LOG_WARNING("File not available on any storage manager. Going to invalidate it [" << _inventoryName << ']') ;

        try
        {
            dbConnectionWrapper pw(getPool()) ;
            dbConnection &theConnection = pw ; // PRQA S 3050

            _inventory.setValidityFlag( false, theConnection );
            _inventory.update(theConnection);
			inventoryInvalidated = true ;
			ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) <<  // PRQA S 3081
				 			mc_param("message", "Inventory invalidated") << 
							mc_metric(Metric_WARN) << 
				 			MCCode ("ARC") << MCFunction ("Archive")) ;

        }
        catch (exception& exupdate)
        {
            // Notify the exception and then go on
			if( not inventoryInvalidated ) {
            	ACS_LOG_WARNING("Unable to invalidate the inventory item") ;
			}
            ACS_LOG_NOTIFY_EX(exupdate) ;
        }
    }
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "By configuration, inventory cannot be invalidated" );
	}
}	

_ACS_END_NAMESPACE
