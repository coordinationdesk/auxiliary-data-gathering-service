// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
	
*/

#include <dbLTAUploadInventoryFile.h>
#include <LTASMTmp.h>
#include <LTAInventoryObjectOperatorIF.h>

#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <MCFilterables.h>
#include <LTAStat.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace acs::mc ;

using acs::math::StatNode ;

namespace {
    StatNode &statN() { return lta::stat()["Upload"] ; }
}

namespace {
	
	const int simulateNoCommunicationWithSM = 2468 ;
	const int simulateDatabaseUpdateFailure = 3579 ;
	const string confRootTag = "LTAFileAction";

}

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbLTAUploadInventoryFile)
bool dbLTAUploadInventoryFile::_debugMessageEmitted = false ;
int dbLTAUploadInventoryFile::_notSimulatedDebugLevel = -1 ;
bool dbLTAUploadInventoryFile::_simulateUpdateValidityException = false;

// constructor
dbLTAUploadInventoryFile::dbLTAUploadInventoryFile(dbConnectionPool &pool, db::LTAInventoryObject &s, const string &path) :
	dbUploadFile(pool, s, path) , //PRQA S 4052
	_inventory(s) , // PRQA S 2528
	_fileType( dbConnectionWrapper(pool) ) ,
	_preconditionPassed(false) , 
	_externalRepositoryPassed(false) , 
	_savePassed(false) ,
	_doNotUpdateValidityFlag(false) ,
	_invokeSpOnInventorySuccessFlag(true),
	_deleteUploadedFilepathOnSuccess(false),
	_errorInUpdateValidityTransaction(false),
	_transactionFileDir("/tmp"),
	_inventoryName( s.getName( dbConnectionWrapper(pool) ) )  // PRQA S 2528
{
	statN()+=1 ;
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ThreadName = \"" << threadName() << "\"") ;
	
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		debugMessage() ;
	ACS_CLASS_END_DEBUG
		
	loadConf();	
		
}

void dbLTAUploadInventoryFile::loadConf()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace ("libLTAFileAction", "", false));
	
	rsResourceSet& rset = ConfigurationSingleton::instance()->get()	;
	ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, rset);

	string tag = confRootTag + ".DeleteUploadedFilepathOnSuccess";
	if (rset.hasKey(tag)) {
		rset.getValue(tag, _deleteUploadedFilepathOnSuccess);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << " set to " << boolalpha << _deleteUploadedFilepathOnSuccess ); 
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << " not found; set to default value: " << boolalpha << _deleteUploadedFilepathOnSuccess ); 
	}

	tag = confRootTag + ".TransactionFileDir";
	if (rset.hasKey(tag)) {
		rset.getValue(tag, _transactionFileDir);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << " set to \"" << _transactionFileDir << '\"' ); 
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, tag << " not found; set to default value: \"" << _transactionFileDir << '\"' );
	}
	
}

void dbLTAUploadInventoryFile::start() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	ACS_COND_THROW(isComplete(),dbPersistent::InvalidUse("File has been already uploaded.")) ; // PRQA S 3081

	ACS_COND_THROW(running(), dbPersistent::InvalidUse("Thread is already running.")) ; // PRQA S 3081

	Thread::start();	
}


bool dbLTAUploadInventoryFile::uploading()  // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	if( running() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Still uploading.") ;
		return true;
	}
	
	std::shared_ptr<const std::exception> e_i = runException() ;
	if(e_i.get())
	{
		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
						 mc_param("message", string("There was an error during upload ") + e_i->what() ) <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("ARC") << MCFunction ("Archive")) ;

		ACS_COND_THROW(!_preconditionPassed, PreconditionException(*e_i,"Error in uploading thread")) ; // PRQA S 3081

		ACS_COND_THROW(!_externalRepositoryPassed, UploadException(*e_i,"Error in uploading thread")) ; // PRQA S 3081
		
		ACS_COND_THROW( _errorInUpdateValidityTransaction, UpdateValidityException(*e_i, "Error in uploading thread" ) ) ;

		ACS_COND_THROW(!_savePassed, SaveException(*e_i,"Error in uploading thread")) ; // PRQA S 3081

        ACS_THROW(dbFileAction::Exception(*e_i,"Unexpected exception")) ; // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "done.") ;
	
	return false;
}


void dbLTAUploadInventoryFile::setDoNotUpdateValidityFlag(bool v) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock l(mutex());
	_doNotUpdateValidityFlag = v;
}

bool dbLTAUploadInventoryFile::getDoNotUpdateValidityFlag() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	Lock l(mutex());
	return _doNotUpdateValidityFlag;
}

void dbLTAUploadInventoryFile::setInvokeSpOnInventorySuccessFlag(bool v) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	Lock l(mutex());
	_invokeSpOnInventorySuccessFlag = v;
}

bool dbLTAUploadInventoryFile::getInvokeSpOnInventorySuccessFlag() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
	Lock l(mutex());
	return _invokeSpOnInventorySuccessFlag;
}

void dbLTAUploadInventoryFile::run() 
{

	bool hasMissingActionsInDbSchema = false;
	
    try {
        statN()["started"] += 1 ;
        Timer timer ;
        timer.start() ;
	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

	    // Needed in all the operation
	    bool fromDB (false);
	    string filetype ;
	    //check Storage Value
	    // Reducing the scope of the connection
        {
    	    dbConnectionWrapper pw(getPool()) ;
    	    dbConnection &theConnection = pw ; // PRQA S 3050

		    bool fileTypeSet = false;

		    hasMissingActionsInDbSchema = dbSchemaMetaDataProvider::existTable("t_srv_sm_sync_missingactions", theConnection);
		    dbPersistent::IdType fileTypeId = _inventory.getFileTypeId(theConnection, &fileTypeSet);
		    ACS_COND_THROW(!fileTypeSet,PreconditionException("Error uploading file \"" + _inventoryName + "\" File Type is not set.")) ; // PRQA S 3081

    	    bool ftloaded = _fileType.load(theConnection , fileTypeId) ;
		    if( not ftloaded ) {
			    ostringstream exMsg ;
			    exMsg << "Error uploading file \"" << _inventoryName << "\" Unknown File Type id: " << fileTypeId ;
			    ACS_THROW( PreconditionException( exMsg.str() ) ); // PRQA S 3081
		    }

		    filetype = _fileType.getFileType(theConnection ) ;
		    if( filetype.empty() ) {
			    ostringstream exMsg ;
			    exMsg << "Empty filetype for t_filetypes record with id=" << fileTypeId ;
			    ACS_THROW( PreconditionException( exMsg.str() ) ); // PRQA S 3081
		    }


		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "precondition passed") ;

		    _preconditionPassed = true;

		    fromDB = _inventory.isFromDatabase();

		    if(!fromDB)
		    {

			    try
			    {
				    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "saving inventory into DB") ;
				    _inventory.setValidityFlag( false, theConnection);
	  			    _inventory.save(theConnection);

			    }
			    catch(exception &ex)
			    {
				    ACS_THROW(SaveException(ex, "Error while saving metadata for file \"" + _inventoryName + "\".")); // PRQA S 3081
			    }
		    }

		    // load the inventory to have up-to date information
		    _inventory.load(theConnection);

	    } // dbconnection scope ends here 


	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //
	    // HERE THE METADATA HAVE BEEN SAVED WITH THE VAL FLAG SET TO FALSE	 //
	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //

	    vector<LTASMTmp> SMListToUpload ; 

	    // save _path because could be damaged ;
	    string uploadPath = getPath() ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "uploadPath: \"" << uploadPath << "\"") ;
	    dbSet set;

	    // Reducing the scope of the connection
        {
    	    dbConnectionWrapper pw(getPool()) ;
    	    dbConnection &theConnection = pw ; // PRQA S 3050


		    try {

    // Going to build the following query			
    // SELECT  m.id, m."type", p.storecost, m.syncupload,  
    // (
    // SELECT   (   COUNT  (  * ) )  
    // FROM  t_storagepriority   
    // WHERE  (( (t_storagepriority.id = m.id) ))  
    // )  AS policycount, 
    // m.allowdefaultcost AS adc 
    // FROM  t_storagemanager m 
    // LEFT OUTER JOIN  t_storagepriority p ON  (( (m.id = p.id) ) AND (  (p.filetype = 'fileType_ECS_17') ))   
    // ORDER BY  m.syncupload DESC, p.storecost DESC ;

        	    const string &dbKey = theConnection.getKey() ;

			    SQLString countStar(dbKey, SQLString::FUNCTION ) ;
			    countStar.setFunctionName("COUNT");
			    countStar.addFunctionParameter("*");

			    // subquery for the select count
			    SQLString subSql (dbKey, SQLString::SELECT);
			    subSql.addSelectList (countStar);
			    subSql.addTableList ("t_storagepriority");

			    {
				    dbQueryParameters pp(dbKey) ;
				    pp.addParameter(dbParam(dbKey, "t_storagepriority", "id", dbParam::EQUAL_TO , "m.id")) ;
				    subSql.addANDWhereParameter (pp);
			    }

			    SQLString sqlString (dbKey, SQLString::SELECT );
	   		    sqlString.addSelectList( "m.id" );	
	   		    sqlString.addSelectList( "m.type" );	
	   		    sqlString.addSelectList( "p.storecost" );	
	   		    sqlString.addSelectList( "m.syncupload" );	
	   		    sqlString.addSelectList( subSql, "policycount" );	// the select count subquery
			    sqlString.addSelectList( "m.allowdefaultcost", "adc" );

			    sqlString.addTableList( "t_storagemanager", "m" );


			    // Add Join on t_storagemanager
			    {
				    dbQueryParameters pp(dbKey) ;
				    pp.addParameter(dbParam(dbKey, "m", "id", dbParam::EQUAL_TO , "p.id")) ;
				    pp.addParameter(dbParam(dbKey, "p", "filetype", dbParam::EQUAL_TO , db::toSQL(dbKey, filetype))) ;
				    sqlString.addJoinList(SQLDialect::LEFT, " t_storagepriority", "p", pp) ;
			    }

			    sqlString.addOrderByList("m.syncupload", SQLString::DESC);
			    sqlString.addOrderByList("p.storecost", SQLString::DESC);
			    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following query: " << sqlString.getSQL());


			    dbQuery query3(theConnection,sqlString, "FAUPLSEL t_storagemanager vs prio cost" ) ;
			    query3.execForSet(set) ;

        	    // no SM configured for upload  ? 
			    ACS_COND_THROW(set.empty(), UploadException("No StorageManager configured to upload " + _inventoryName )); // PRQA S 3081

			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Number of storage manager configured for defined filetype (" << set.size() << ")") ;

			    const size_t srsize = set.size() ;
        	    for(size_t i=0; i<srsize; ++i)
			    {
				    dbRow row(dbKey) ;
				    set.getNextRow(row);

            	    int storeid = 0 ;
            	    int storecost = 0 ;
				    string type;
				    bool syncupload= false;
            	    bool adc = true; 
            	    unsigned int policycounter = 0;
            	    bool isNull = false ;

				    int j = 0;
				    row.column(j++,storeid);
				    row.column(j++,type);
				    row.column(j++,storecost, &isNull); // PRQA S 4400 7
				    row.column(j++,syncupload);
            	    row.column(j++,policycounter);
            	    row.column(j++,adc);

            	    // policycounter is the number of rows in t_storagepriority referring to a specific SM.
            	    // if no row is present this means that by default (as in the beginning of PDS) every filetype shall 
            	    // go in that SM.
            	    // Algo is:
            	    // 1 - if no policies for SM (policycounter = 0) allow the operation.
            	    // 2 - if some policies for the SM (policycounter >0 0) we need to check if the absence has a default (ADC: true or false) 
            	    //     2.1 - if cost is set use the cost and the upload_predicate (if any) when cost allows the upload
            	    //     2.2 - if cost is not set use +1 if ADC is true or -1 if ADC is false

            	    if (policycounter == 0)
            	    {
                	    storecost = 1;
            	    }
            	    else
            	    {
                	    if (isNull)
                	    {
                    	    storecost = ( adc ? 1 : -1 ); // PRQA S 3380
                	    }
                	    else { /* nothing: storecost is negative */ 
                	    }
            	    }

				    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SMId " << storeid //PRQA S 3380, 3385
				    	    << " SM type " << type
				    	    << " Storage upload cost " << storecost << (isNull ? " (was NULL)" : "") // PRQA S 3380
				    	    << " sync upload " << boolalpha << syncupload
                    	    << " SM rows in t_storagepriority " << policycounter
                    	    << " SM allow default cost " << adc  );


				    if( ( storecost>=0 ) && syncupload ) {
					    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage Id " << storeid << " added to list for upload");
					    LTASMTmp up(storeid, type, 0) ;
					    up.cost = storecost ;
					    SMListToUpload.push_back(up);
				    }

			    }  
		    }
		    catch (std::exception &) 
		    {
			    removeInventoryForced(fromDB, theConnection) ;
			    throw ;
		    }

	    } // dbconnection scope ends here 


	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //
	    // HERE THE HAVE THE LIST OF STORAGE MANAGERS WHERE THE FILE IS      //
	    // TO BE UPLOADED SORTED BY THEIR STORE COST                          //
	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Number of storage manager where the file has to be stored (" << SMListToUpload.size() << ")") ;
	    vector<pair <SMInterface*, bool> > tocheck; // SMInterface, upload finished
	    const size_t smltusize = SMListToUpload.size() ;
	    for (size_t i=0 ; (i < smltusize); ++i) { // PRQA S 4231, 4235

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage Id " << SMListToUpload[i].id
			        << " Storage upload cost " << SMListToUpload[i].cost ) ;

		    putFile(uploadPath, SMListToUpload[i].id, SMListToUpload[i].type, tocheck) ;
		    yield() ;

	    }

	    yield() ;	
	    // the validity flag is _externalRepositoryPassed in order to save it if at least one SM has the file

	    bool updateInventory = storeOnDatabase(false, _externalRepositoryPassed) ;

	    yield() ;
	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //
	    // CHECK RESULTS							                         //
	    // ----------------------------------------------------------------- //
	    // ----------------------------------------------------------------- //

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CHECK RESULT") ;
	    bool validityFlag = false ;
	    size_t allChecked = 0 ;
		vector <pair <dbPersistent::IdType, dbPersistent::IdType> > syncMissingActions; // collects the list of pairs <inventory ID, SM id> for which upload fails
	    const size_t tcsize = tocheck.size() ;
	    bool atLeastOneUploadComplete = false ; 
        // wait 5 ms before recycle
	    for(allChecked = 0; allChecked < tcsize; nanosleep(5000000), yield()) { // PRQA S 4230, 4235, 4236, 4238, 4244, 4400
		    allChecked=0 ;
		    for (size_t i=0; i< tcsize; yield(), i++) {

			    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "inner allChecked: " << allChecked << " tocheck size: " << tcsize) ;
			    try {
				    if (!tocheck[i].second) {
					    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "SM[" << i << "] still not finished") ;
					    if (tocheck[i].first && !tocheck[i].first->uploading()) {
						    tocheck[i].second = true ; // finished
						    ACS_COND_THROW( not tocheck[i].first->isUploadSynchronous(), UploadException( "Not synchronous SM") );
						    validityFlag = true;
						    // here delete the thread 
						    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "deleting uploader #: " << i) ;
						    yield() ;
						    delete tocheck[i].first ;
						    tocheck[i].first = 0 ;
						    // anyway at least one SM has the file, 
						    // in case the storage pol was both, try to save the blob
						    // the val flag could be left to false if no blob should be saved
						    _externalRepositoryPassed = true ;
						    atLeastOneUploadComplete = true ;
					    }
				    }
				    else {
					    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SM[" << i << "] finished") ;
					    allChecked++ ;
                    }
			    }
			    catch (exception &e) {
				    ACS_LOG_NOTIFY_EX(e);

				    tocheck[i].second = true ; // do not check at this item
				    dbPersistent::IdType SMId = tocheck[i].first->SMId();

					syncMissingActions.push_back(make_pair<dbPersistent::IdType, dbPersistent::IdType> (_inventory.getId(), tocheck[i].first->SMId()));
					delete tocheck[i].first ;
					 tocheck[i].first = 0 ;

				    ACS_LOG_WARNING("Exception got while uploading filename: \"" << _inventoryName << "\" SmId: " << SMId << " trying with next (if any).") ;

				    // APF-270
				    ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
								     mc_param("equipmentid", SMId) <<
								     mc_param("message", string("Exception got from storage manager ") + e.what()) <<
								     mc_metric(Metric_FAIL) << 
								     MCCode ("ARC") << MCFunction ("Archive")) ;

			    }
		    } // for (size_t i=0; i< tcsize; yield(), i++)

		    yield() ;	
		    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "outer verification loop: allChecked=" << allChecked) ;

	    } // for(allChecked = 0; allChecked < tocheck.size(); nanosleep(5000000), yield())
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end of verification loop: allChecked=" << allChecked) ;

	    if (atLeastOneUploadComplete) {
		    updateInventory = storeOnDatabase(false, validityFlag, true) ;   ////// controllare ....
			if (hasMissingActionsInDbSchema) {
				dbConnectionWrapper pw(getPool()) ;
				dbConnection &theConnection = pw ; // PRQA S 3050

				for (auto ma : syncMissingActions) {
					try {
						// add a record on t_sm_sync_missingactions to trace the uploading failure
						dbSmartPersistent therecord ("t_srv_sm_sync_missingactions", theConnection);

						// Set the record on the database
						therecord.set("inv_id", ma.first, theConnection);
						therecord.set("sm_id" ,ma.second, theConnection );
						string operation("UPLOAD");
						therecord.set("operation", operation, theConnection);
						therecord.save(theConnection);
						ACS_LOG_INFO("Added item inv_id #" << ma.first << " and sm_id # " << ma.second << " on t_srv_sm_sync_missingactions." );
					} catch (const std::exception& e) {
						ACS_LOG_WARNING("Unable to insert the record in t_srv_sm_sync_missingactions for inv_id # "<< ma.first << " and sm_id # " << ma.second);
					}
				}
			}
        }

	    if (!_externalRepositoryPassed) { 
		    removeInventoryForced(fromDB, dbConnectionWrapper(getPool())) ;
		    ACS_THROW(UploadException("Unable to upload on every Storage Manager. See previous log.")) ; // PRQA S 3081
	    }

	    if( _deleteUploadedFilepathOnSuccess ) {
		    try {
			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << uploadPath << "\" must be deleted after upload");
			    File::remove(uploadPath);
		    }
		    catch( exception &ex )
		    {
			    ostringstream exMsg;
			    exMsg << "The following error occur while uploading inventory with filename=\"" << _inventoryName << "\" (id=" <<  _inventory.getId()
					    << "): unable to remove \"" << uploadPath << "\" after successful upload, please check why and REMOVE IMMEDIATELY";
			    ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException( ex, exMsg.str() ));
		    }
	    }


	    if (!updateInventory) {
		    ACS_LOG_WARNING("WARNING: last attempt to update inventory.") ;
		    updateInventory = storeOnDatabase(true, validityFlag, true) ; // uploadPath not really used in this method
	    }

	    setCompleted(true) ;

#ifndef LTA // removed not to send huge amount of messages in the framework of LTA project
	    {
    	    // the upload is completed here can issue the message for MC
		    // APF-270
		    bool isSet;
		    off_t fileSize = _inventory.getContentLength( dbConnectionWrapper(getPool()), &isSet );
		    if( not isSet ) {
			    fileSize = 0;
		    }
		    ACS_LOG_MC_INFO( mc_param("filename", _inventoryName) << // PRQA S 3081
						     mc_param("filesize", fileSize) << mc_metric(Metric_IN) << 
						     MCCode ("ARC") << MCFunction ("Archive")) ;
	    }
#endif
		
	    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Upload is over");

	    timer.stop() ;

        statN()["success"] += 1 ;
        (statN()["elapsed [ms]"])+=double(timer.elapsed()); // PRQA S 3081
    }
    catch (exception &) {
        statN()["error"] += 1 ;
        throw ;
    }
    
}


void dbLTAUploadInventoryFile::putFile(const string &path, dbPersistent::IdType storageManId, const string& smtype,vector<pair<SMInterface*,bool> >& tocheck) throw ()
{
	try {
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "asking for a new SMInterface(" << "," << smtype << "," << storageManId << ", ...)") ; 
		SMInterface* storageup = SMInterfaceFactoryMethod::getInterface(smtype, storageManId, &_inventory, getPool()); // PRQA S 3000
		if(!storageup)
		{
			ostringstream os ;
			os << "No StorageManager defined for type " << smtype << " Smid " << storageManId ;
			ACS_THROW(UploadException(os.str())); // PRQA S 3081
		}

		if (ACS_CLASS_GET_DEBUG() == simulateNoCommunicationWithSM) { 
			ACS_THROW(UploadException("Storage Manager comunication failure (simulated)")) ; // PRQA S 3081
		}
		
        storageup->setClientName(getClientName()) ; // set the client name for the current action

		try {
			LTAInventoryObjectOperatorIF oper;
			storageup->upload(path,oper);
		}
		catch(exception &) {
			delete storageup ;
			throw ;
		}
		tocheck.push_back(pair<SMInterface*,bool>(storageup,false));
	}
	catch (exception &e) {
		ACS_LOG_WARNING("WARNING dbLTAUploadInventoryFile::putFile exception got: <<<" << e.what() << ">>> while uploading on storage type "<< smtype << " id "<<storageManId) ;
	}
}



void dbLTAUploadInventoryFile::removeInventoryForced(bool wasFromDb, dbConnection &theConnection) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	if( not wasFromDb ) {
		try
		{
			_inventory.remove(theConnection);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory record successfully removed");
		}
		catch(exception &ex)
		{
			ACS_LOG_WARNING("WARNING: forced removal of inventory metadata was not possible. The exception got is: " << ex.what() << "\n" 
				<< "The t_inventory will contain incongruence for the file: " << _inventoryName << " correct it as soon as possible.") ;
		}
	}
	else { // just for debug
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Inventory record will not be removed since occurring on db before uploading");
	}
}

bool dbLTAUploadInventoryFile::storeOnDatabase(bool emitException, bool valflag, bool callSp) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	bool updated = false;
	bool validityFlagToSet = false;
	try {
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
		
		bool callSpEffectively = false;
		if (!getDoNotUpdateValidityFlag()) {
			validityFlagToSet = valflag || _inventory.getValidityFlag(theConnection) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to set validity flag to " << boolalpha << validityFlagToSet );
			_inventory.setValidityFlag( validityFlagToSet, theConnection ); // keep it at true 
			if( callSp && _invokeSpOnInventorySuccessFlag && validityFlagToSet ) {
				callSpEffectively = true;
			}
		}
		else { // just for debug
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Do not update validity flag");
		}
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to call inventory record update; call sp: " << boolalpha << callSpEffectively ) ;

		if (ACS_CLASS_GET_DEBUG() == simulateDatabaseUpdateFailure) {
			ACS_THROW(UploadException("Database update failure (simulated)")) ; // PRQA S 3081
		}

		if( callSpEffectively ) {
			
			SQLString sqlStringSp = buildSpSuccessfulInventoryCmd(_inventory.getId(), theConnection) ;
			
			string transactionFilePath = writeTransactionFile (sqlStringSp, theConnection) ;
			FileGuard fg(transactionFilePath);
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				fg.detach();
				ACS_LOG_DEBUG(SimpleDebugSignature << '\"' << transactionFilePath << "\" file will not be deleted for debug purposes" );
			ACS_CLASS_END_DEBUG

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "About to perform transaction" );
			db::Transaction transaction(theConnection) ;	
			try {
				_errorInUpdateValidityTransaction = false;

				transaction.begin_work("Update validityFlag") ;

				_inventory.update(theConnection);
				
				ACS_COND_THROW( _simulateUpdateValidityException, UploadException("Simulate exception in update inventory validity flag transaction") );

				executeSQLCmd (sqlStringSp, "P_SRV_SuccessfulInventory execution", theConnection)  ;

				transaction.commit();
			}
			catch( exception & e) 
			{
				_errorInUpdateValidityTransaction = true;
				transaction.roll_back();
				fg.detach();
				throw;
			}
		}
		else {
			_inventory.update(theConnection);
		}
		
		updated = true;
		_savePassed=true;
		
	}
	catch(exception &ex)
	{
		ACS_LOG_ERROR("dbLTAUploadInventoryFile::storeOnDatabase error got updating inventory: " << _inventoryName << " Exception was:" << ex.what()) ;
		if (emitException) { 
			throw ; 
		}
	}
	
	return updated ;

}


SQLString dbLTAUploadInventoryFile::buildSpSuccessfulInventoryCmd(dbPersistent::IdType invid, dbConnection& conn) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	const std::string& dbKey = conn.getKey();
	
	const std::string& dialectName = SQLDialectFactory::dialectName(dbKey);
	ACS_COND_THROW ((dialectName != "Postgres8"), UploadException("Unsupported sql dialect " + dialectName));
	
	ostringstream ss;
	ss << "CALL p_srv_successful_inventory(" << invid << ")";
	
	SQLString sqlstring (dbKey, ss.str());

	return sqlstring;
}

void dbLTAUploadInventoryFile::executeSQLCmd (const SQLString& sqlstring, const string & label, dbConnection& conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
    
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following SQL command: \"" << sqlstring.getSQL() << '\"');

    dbQuery query( conn, sqlstring, label );
	if( not query.execForStatus() ) {
		string errorString;
		errorString += "Query Execution error SQL command is \"" ;
		errorString += sqlstring.getSQL() ;
		errorString += "\". Error is \"" + string(query.errorString())  + "\".";
		ACS_THROW ( dbQuery::QueryExecuteException (errorString) ) ;
	}

}

string dbLTAUploadInventoryFile::writeTransactionFile (const SQLString & sqlStringSp, dbConnection& conn) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
	if( not File::exists(_transactionFileDir) ) {
		ACS_LOG_INFO( '\"' << _transactionFileDir << "\" transaction file directory does not exist: going to create");
		File::mkdirtree(_transactionFileDir);
	}
	else if( not File::isDir(_transactionFileDir) ) {
		ACS_THROW( exIllegalValueException( '\"'+_transactionFileDir+"\" entry already exists but it is not a directory (it should be transaction file directory)!" ) );
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"'+_transactionFileDir+"\" transaction file directory already exists");
	}

	string transactionFilePath = StringUtils::pathJoin( _transactionFileDir, (_inventoryName +".sql") ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Transaction file path=\"" << transactionFilePath << '\"');
	ofstream out(transactionFilePath.c_str(), ios_base::trunc);
	ACS_COND_THROW( not out.is_open(), exIOException("Cannot open \""+transactionFilePath+"\" file", errno ) );

	vector<size_t> updatedFields;
	SQLString sqlStringUpdateInv = _inventory.updateQuery(updatedFields, conn) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Inventory update string: " << sqlStringUpdateInv.getSQL());
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sp execution string: " << sqlStringSp.getSQL());


	const std::string& dbKey = conn.getKey();

	out << SQLDialectFactory::sqlDialect(dbKey).beginTransaction() << ";\n";
	out << sqlStringUpdateInv.getSQL() << '\n';
	out << sqlStringSp.getSQL() << '\n';
	out << SQLDialectFactory::sqlDialect(dbKey).commitTransaction() << ';';

	out.close();
	
	return transactionFilePath;
	
}

void dbLTAUploadInventoryFile::debugMessage()
{
	if (!_debugMessageEmitted) {
		_debugMessageEmitted = true ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbLTAUploadInventoryFile_DEBUG=" << simulateNoCommunicationWithSM << " to simulate SM communication failure ") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbLTAUploadInventoryFile_DEBUG=" << simulateDatabaseUpdateFailure << " to simulate DB update failure ") ;
	}
}


void dbLTAUploadInventoryFile::setSimulateNoCommunicationWithSM()
{
	const int dbl = ACS_CLASS_GET_DEBUG();
	if( dbl != simulateDatabaseUpdateFailure && dbl != simulateNoCommunicationWithSM ) {
		_notSimulatedDebugLevel = dbl;
	}
	ACS_CLASS_SET_DEBUG(simulateNoCommunicationWithSM);
}

void dbLTAUploadInventoryFile::setSimulateDatabaseUpdateFailure() 
{
	const int dbl = ACS_CLASS_GET_DEBUG();
	if( dbl != simulateDatabaseUpdateFailure && dbl != simulateNoCommunicationWithSM ) {
		_notSimulatedDebugLevel = dbl;
	}
	ACS_CLASS_SET_DEBUG(simulateDatabaseUpdateFailure);
}

void dbLTAUploadInventoryFile::setSimulateUpdateValidityException(bool v) 
{
	_simulateUpdateValidityException = v;
}


void dbLTAUploadInventoryFile::unsetSimulationFlags()
{
	const int dbl = ACS_CLASS_GET_DEBUG();
	if( dbl == simulateDatabaseUpdateFailure || dbl == simulateNoCommunicationWithSM ) {
		ACS_CLASS_SET_DEBUG(_notSimulatedDebugLevel);
	}
}



_ACS_END_NAMESPACE
