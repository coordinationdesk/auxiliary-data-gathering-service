// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 7.18  2017/11/16 15:43:56  lucio.pulvirenti
	S3EVOL-78: t_storagepriority upload_predicate column added to query if existing. If storecost >= 0, upload_predicate is got and the value
	passed to a factory to get an object to decide (pass method) if the given inventory obj can be uploaded on the current SM.
	
	Revision 7.17  2017/11/14 16:35:01  lucio.pulvirenti
	S3EVOL-78: work in progress.
	
	Revision 7.16  2017/10/31 11:33:39  lucio.pulvirenti
	S2PDGS-1876: error message in upload fixed.
	
	Revision 7.15  2016/12/07 10:00:00  marfav
	Releasing dbConnection ASAP to limit multiple connection usage
	
	Revision 7.14  2016/12/06 16:11:57  lucio.pulvirenti
	S3MPM-843: dbInventoryObjectCacheManager object used.
	
	Revision 7.13  2016/07/15 13:39:02  marfav
	CSGACQ-86 Using only SQLString statements
	
	Revision 7.12  2016/05/12 10:18:36  lucio.pulvirenti
	PDSEV-34: if _deleteUploadedFilepathOnSuccess flag of base class is set, path of uploaded file is removed.
	
	Revision 7.11  2016/04/29 16:03:17  marpas
	ids are of dbPersistent::IdType type - to be overall consistent
	coding best practices applied
	
	Revision 7.10  2016/04/11 13:29:59  marpas
	adopting new db interfaces
	
	Revision 7.9  2016/03/26 21:42:14  marpas
	using new db I/F
	
	Revision 7.8  2015/11/26 14:59:41  marfav
	mc_params are now lowercase
	
	Revision 7.7  2015/11/05 11:33:27  damdec
	Check on the existance of t_sm_sync_missingactions on Db introduced. Useless catch(...) branch removed.
	
	Revision 7.6  2015/11/03 15:28:32  damdec
	Bug fixing: t_sm_sync_missingactions updated only when at least one upload has been successful. MinimumAsyncUploadCost handling removed.
	
	Revision 7.5  2015/10/28 13:37:43  damdec
	PDSEV-16: blob handling removed.
	
	Revision 7.4  2015/10/27 15:55:24  damdec
	PDSEV-16: blob handling removed.
	
	Revision 7.3  2015/10/26 13:57:43  damdec
	Handling of t_sm_sync_missingactions introduced.
	
	Revision 7.2  2015/09/30 13:47:19  damdec
	APF-315: In case of uploading failure on a synchronous storage manager, a record is added in the t_sm_missing.
	
	Revision 7.1  2015/08/17 07:59:01  marfav
	APF-270 work in progress
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.7  2015/02/04 14:56:05  marfav
	S2PDGS-1072 adding support to options noactions and restoreonsync
	
	Revision 6.6  2015/02/02 16:08:06  marfav
	S2PDGS-1072 upload without triggers implemented
	
	Revision 6.5  2015/01/30 17:22:19  marfav
	S2PDGS-976 managing restore on sync storage managers
	
	Revision 6.4  2014/06/06 10:13:38  marfav
	Do not check filesize if the blobl is not needed in t_inventory
	
	Revision 6.3  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.2  2013/10/24 13:19:58  marpas
	narrowing delay
	
	Revision 6.1  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.1  2013/09/19 15:39:56  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.35  2013/04/12 12:16:30  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 1.34  2013/04/03 13:47:32  marpas
	run does not longer gets a void*
	
	Revision 1.33  2013/04/03 12:49:28  marpas
	Thread::run gets a void*
	
	Revision 1.32  2013/03/29 13:53:03  marpas
	fix a regression introduced with the new File::tempname interface
	
	Revision 1.31  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.30  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.29  2013/03/11 12:51:26  marpas
	FileGuard is a template specialization now
	deprecated functions substituted
	
	Revision 1.28  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.27  2012/12/14 14:11:16  marfav
	Passing the connection to the schema provider
	
	Revision 1.26  2012/12/12 16:37:54  marfav
	Added support to AllowDefaultCost on t_storagemanagers (S1PDGS-30147)
	
	Revision 1.25  2012/11/28 13:16:50  marpas
	logical op fixed
	
	Revision 1.24  2012/11/20 09:04:04  marpas
	define PDS_OLD_SCHEMA to use old schema features
	
	Revision 1.23  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.22  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.21  2011/04/19 16:19:55  marpas
	in case of failed storage manager keys insert / update tries to erase the file from the storage manager itself
	
	Revision 1.20  2011/04/05 17:01:47  marpas
	tree management in progress
	
	Revision 1.19  2011/04/05 16:52:35  marpas
	Tree management work in progress
	
	Revision 1.18  2010/09/20 10:04:57  marpas
	connections are not passed to upload methods of storage managers
	
	Revision 1.17  2010/08/11 09:03:14  marpas
	bug resolution reverted to 1.14 because connection management is in charge of
	dialect
	
	Revision 1.16  2010/08/11 08:42:12  marpas
	bug fixed and debug improved
	
	Revision 1.15  2010/08/11 08:39:00  marpas
	debug improved
	
	Revision 1.14  2010/08/11 08:35:39  marpas
	debug still improved
	
	Revision 1.13  2010/08/11 08:31:58  marpas
	debug improved
	
	Revision 1.12  2010/08/10 09:37:45  marpas
	Oracle11 support completed
	
	Revision 1.11  2010/04/20 13:48:55  marpas
	potential bug fixed (deferencing null pointer)
	
	Revision 1.10  2010/04/20 11:07:55  marpas
	leak and deadlock fixed
	
	Revision 1.9  2010/04/19 16:55:48  marpas
	memory leak fixed
	
	Revision 1.8  2010/03/08 14:53:33  enrcar
	EC:: Oracle DB handled
	
	Revision 1.7  2009/06/26 10:43:07  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.6  2009/02/02 16:04:26  ivafam
	Bug fixed in query
	
	Revision 1.5  2009/01/29 11:01:40  marpas
	debug improved
	
	Revision 1.4  2009/01/20 18:15:13  ivafam
	Regression fixed
	
	Revision 1.3  2008/12/03 17:19:16  ivafam
	Work in progress
	
	Revision 1.2  2008/11/11 14:58:20  ivafam
	Work in progess
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
*/

#include <dbUploadInventoryFile.h>
#include <UploadPredicateFactory.h>
#include <UploadPredicate.h>
#include <GFASMTmp.h>
#include <dbFileType.h>

#include <dbPersistentFactoryChain.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <dbConnectionPool.h>
#include <dbConv.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbInventoryObject.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <SMInterfaceScopeGuard.h>
#include <MCFilterables.h>
#include <dbInventoryObjectOperatorIF.h>
#include <dbInventoryObjectCacheManager.h>
#include <SMPackingStrategy.h>
#include <SMPackingStrategyFactoryChain.h>
#include <SMPackingStrategyFactory.h>
#include <SMPackingStrategyScopeGuard.h>
#include <dbSMactions.h>
#include <dbPersistentScopeGuard.h>
#include <dbPersistentFactoryChain.h>
#include <XMLIstream.h>
#include <unistd.h>
#include <algorithm>




_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbUploadInventoryFile)
bool dbUploadInventoryFile::_debugMessageEmitted = false ;

// constructor
dbUploadInventoryFile::dbUploadInventoryFile(dbConnectionPool &pool, dbInventoryObject &s, const string &path) :
	dbUploadFile(pool, s,path) , //PRQA S 4052
	_inventory(s) , // PRQA S 2528
	_fileType(0) ,
	 _preconditionPassed(false) , 
	 _externalRepositoryPassed(false) , 
	 _savePassed(false) ,
     _tmpfile()
{
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
		debugMessage() ;
	ACS_CLASS_END_DEBUG

    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	_fileType=	 dynamic_cast<dbFileType*>(dbPersistentFactoryChain::instance()->newObject("dbFileType", pool.getKey())) ; // PRQA S 3081
	ACS_COND_THROW(_fileType == 0, exIllegalValueException("Error uploading file \"" + s.getFileName() + "\" dynamic cast failed. Expecting a dbFileType or derived class pointer")) ; // PRQA S 3081

}


dbUploadInventoryFile::~dbUploadInventoryFile() ACS_NOEXCEPT
{
    // PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	delete _fileType;
	
    size_t tsize = _tmpfile.size() ;
	for(size_t i=0;i<tsize;++i) {
		delete _tmpfile[i] ; // FileGuard dtor gets rid of guarded file
    }
    // PRQA L:L1

}


void dbUploadInventoryFile::start() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
    size_t tsize = _tmpfile.size() ;
	for(size_t i=0;i<tsize;++i) {
		delete _tmpfile[i] ; // FileGuard dtor gets rid of guarded file
    }

	_tmpfile.clear() ;

	ACS_COND_THROW(isComplete(),dbPersistent::InvalidUse("File has been already uploaded.")) ; // PRQA S 3081


	ACS_COND_THROW(running(), dbPersistent::InvalidUse("Thread is already running.")) ; // PRQA S 3081

	Thread::start();	
}


bool dbUploadInventoryFile::uploading()  // PRQA S 4020
{
	if( running() )
	{
	    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
		return true;
	}
	
	exception *e_i = 0 ;
	runException(e_i) ;
    ACS_SMARTPTR<exception> e(e_i) ; 
	if(e.get())
	{
		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
						 mc_param("message", string("There was an error during upload ") + e->what() ) <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("ARC") << MCFunction ("Archive")) ;

		ACS_COND_THROW(!_preconditionPassed, PreconditionException(*e,"Error in uploading thread")) ; // PRQA S 3081

		ACS_COND_THROW(!_externalRepositoryPassed, UploadException(*e,"Error in uploading thread")) ; // PRQA S 3081

		ACS_COND_THROW(!_savePassed, SaveException(*e,"Error in uploading thread")) ; // PRQA S 3081

        ACS_THROW(dbFileAction::Exception(*e,"Unexpected exception")) ; // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "done.") ;
	
	return false;
}




void dbUploadInventoryFile::run() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	//If re-inventory and storage == "BOTH" file is downloaded from ARF and
	//must be downloaded at the end.
	string filetype =_inventory.getFileType();
	
	
	//check Storage Value
	bool fileTypeSet = false;
	string fileType= _inventory.getFileType(&fileTypeSet);
	ACS_COND_THROW(!fileTypeSet,PreconditionException("Error uploading file \"" + _inventory.getFileName() + "\" File Type is not set.")) ; // PRQA S 3081

	_fileType->setFileType(fileType);


	// Needed in all the operation
	bool hasMissingActionsInSchema (false);
	bool ftloaded (false);
	bool fromDB (false);

	// Reducing the scope of the connection
    {
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
        ftloaded = _fileType->load(theConnection) ;
	    hasMissingActionsInSchema = dbSchemaMetaDataProvider::existTable("t_sm_sync_missingactions", theConnection);
	    ACS_COND_THROW( not ftloaded, PreconditionException("Error uploading file \"" + _inventory.getFileName() + "\" Unknown File Type :\""+filetype+"\".")); // PRQA S 3081

	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "precondition passed") ;

	    _preconditionPassed = true;

	    fromDB = _inventory.isFromDatabase();

	    if(!fromDB)
	    {

		    try
		    {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "saving inventory into DB") ;
			    _inventory.setValidityFlag(false);
	  		    _inventory.save(theConnection);
                if (this->noActions())
                {

                    string dbKey = theConnection.getKey();
                    SQLString theString(dbKey, SQLString::DELETE, "CLN t_pendinginventory");
                    theString.addTableList ("t_pendinginventory");
                    {
                        dbQueryParameters pp(dbKey) ;
                        pp.addParameter(dbParam(dbKey, "t_pendinginventory","filename" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getFileName() ))) ;
                        theString.addANDWhereParameter(pp) ;
                    }    
                    {
                        dbQueryParameters pp(dbKey) ;
                        pp.addParameter(dbParam(dbKey, "t_pendinginventory","fileversion" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getFileVersion() ))) ;
                        theString.addANDWhereParameter(pp) ;
                    }    
                    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Executing query [" << theString.getSQL() << "]");
                    dbQuery query( theConnection, theString, "CLN t_pendinginventory" );

                    try
                    { 
                        if (!query.execForStatus())
                        {
                            ACS_LOG_WARNING ("Unable to remove the pending inventory - command " << theString.getSQL() << " - error " << query.errorString());    
                        }
                    }
                    catch (exception& ex)
                    {
                        ACS_LOG_WARNING ("Unable to remove the pending inventory - command " << theString.getSQL());
                        ACS_LOG_NOTIFY_EX (ex);
                    }

                }

		    }
		    catch(exception &ex)
		    {
			    ACS_THROW(SaveException(ex, "Error while save metadata for file \"" + _inventory.getFileName() + "\".")); // PRQA S 3081
		    }
	    }

	} // dbconnection scope ends here 


	// ----------------------------------------------------------------- //
	// ----------------------------------------------------------------- //
	// HERE THE METADATA HAVE BEEN SAVED WITH THE VAL FLAG SET TO FALSE	 //
	// ----------------------------------------------------------------- //
	// ----------------------------------------------------------------- //
	
	vector<GFASMTmp> SMListToUpload ; 

	// save _path because could be damaged ;
	string uploadPath = getPath() ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "uploadPath: \"" << uploadPath << "\"") ;
	dbSet set;

	// Reducing the scope of the connection
    {
    dbConnectionWrapper pw(getPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050


	try {
        
		string sql3 ;
        
        // be sure that the t_storagemanager table has the field allowdefaultcost
        bool hasADCfield = dbSchemaMetaDataProvider::hasField ("t_storagemanager", "allowdefaultcost", theConnection); // PRQA S 4412
		

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AllowDefaultCost field is " << (hasADCfield ? "" : "NOT ") << "available.") ; // PRQA S 3380
		
		// be sure that the t_storagepriority table has the field smSelectionAlgo
        bool hasUploadPredicate = dbSchemaMetaDataProvider::hasField ("t_storagepriority", "upload_predicate", theConnection); // PRQA S 4412
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "upload_predicate field is " << (hasUploadPredicate ? "" : "NOT ") << "available.") ; // PRQA S 3380

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
	   	sqlString.addSelectList( "(CASE WHEN p.parameters is null THEN 1 ELSE 0 END)", "strategy" );	
	   	sqlString.addSelectList( subSql, "policycount" );	// the select count subquery
		if (hasADCfield)
		{            
		   	sqlString.addSelectList( "m.allowdefaultcost", "adc" );	
		}

		if( hasUploadPredicate ) {
		   	sqlString.addSelectList( "p.upload_predicate");	
		}
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
		ACS_COND_THROW(set.empty(), UploadException("No StorageManager configured to upload " + _inventory.getFileName())); // PRQA S 3081

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Number of storage manager configured for defined filetype (" << set.size() << ")") ;
		
		size_t srsize = set.size() ;
        for(size_t i=0; i<srsize; ++i)
		{
			dbRow row(dbKey) ;
			set.getNextRow(row);
			
            int storeid = 0 ;
            int storecost = 0 ;
            int packingstrategy = 0;
			string type;
			bool syncupload= false;
            bool adc = true; 
            unsigned int policycounter = 0;
            bool isNull = false ;
			string uploadPredicate;

			row.column(0,storeid);
			row.column(1,type);
			row.column(2,storecost, &isNull); // PRQA S 4400 7
			row.column(3,syncupload);
			row.column(4,packingstrategy);
            row.column(5,policycounter);
            if (hasADCfield)
            {
                row.column(6,adc);
            }
            
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
				else if( storecost >= 0 ) {
					
					bool pNull = false ;
            		if (hasUploadPredicate)
            		{
                		row.column(7, uploadPredicate, &pNull); // PRQA S 4400
            		}
					
				}
                else { /* nothing: storecost is negative */ 
                }
            }

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SMId " << storeid //PRQA S 3380, 3385
				    << " SM type " << type
				    << " Storage upload cost " << storecost << (isNull ? " (was NULL)" : "") // PRQA S 3380
				    << " sync upload " << boolalpha << syncupload
                    << " SM rows in t_storagepriority " << policycounter
                    << " SM allow default cost " << boolalpha << adc << (hasADCfield ? " (from DB)" : " (not on DB)") // PRQA S 3380
					<< (hasUploadPredicate ? " Upload predicate \""+uploadPredicate+'\"' : "") );
			
			
			if(storecost>=0)
			{
				
				bool doUpload = true;
				if( not uploadPredicate.empty() ) {
					try {
						UploadPredicate* predicate = UploadPredicateFactory::instance()->get(uploadPredicate);
						ACS_COND_THROW( not predicate, exCriticalException("Cannot create UploadPredicate object of type \""+ uploadPredicate +'\"')) ; // PRQA S 3081
						UploadPredicateScopeGuard guard(predicate);
						if (not predicate->pass(storeid, _inventory, theConnection) ) {
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _inventory.getFileName() << "\" file will NOT uploaded on SM with id=" << storeid << " according to \"" << uploadPredicate << "\" predicate");
    						doUpload = false;
						}
					}
					catch( exception &e)
					{
						ACS_LOG_NOTIFY_EX(e);
						ACS_LOG_ERROR("Skipping uploading of \"" << _inventory.getFileName() << "\" file (version \"" << _inventory.getFileVersion() << "\") on SM with id " << storeid << ". Trying with next one, if any");
						doUpload = false;
					}
				}
				
				if( doUpload ) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage Id " << storeid << " added to list for upload");
					GFASMTmp up(storeid, type, 0) ;
					up.hasParameters = (packingstrategy != 0);
					up.cost = storecost ;
					up.synchronous = syncupload;
					SMListToUpload.push_back(up);
				}
				else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Do NOT upload on storage Id " << storeid);
				}
			
			}

		}  
	}
	catch (...) 
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
	vector<pair <SMInterface*, bool> > tocheck;
	size_t smltusize = SMListToUpload.size() ;
	for (size_t i=0 ; (i < smltusize); ++i) { // PRQA S 4231, 4235
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage Id " << SMListToUpload[i].id
			    << " Storage upload cost " << SMListToUpload[i].cost
			    << " Storage upload synchronous  " << boolalpha << SMListToUpload[i].synchronous) ;
		
		if(SMListToUpload[i].synchronous) {
			putFile(uploadPath,SMListToUpload[i].id,SMListToUpload[i].type,SMListToUpload[i].hasParameters,tocheck) ;
        }
		yield() ;
		
	}
	
	yield() ;	
	// the validity flag is _externalRepositoryPassed in order to save it if at least one SM has the file

	bool updateInventory = storeOnDatabase(false, _externalRepositoryPassed, uploadPath) ;

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
	bool uploaded = false;
    // wait 5 ms before recycle
	for(allChecked = 0; allChecked < tocheck.size(); nanosleep(5000000), yield()) { // PRQA S 4230, 4235, 4236, 4238, 4244, 4400
		bool atLeastOneUploadComplete = false ;   ////// controllare ....
		allChecked=0 ;
        size_t tcsize = tocheck.size() ;
		for (size_t i=0; i< tcsize; yield(), i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "inner allChecked: " << allChecked
                    << " tocheck size: " << tocheck.size()) ;
			try {
				if (!tocheck[i].second) {
					if (tocheck[i].first && !tocheck[i].first->uploading()) {
						tocheck[i].second = true ; // finished 
						if (tocheck[i].first->isUploadSynchronous()) {
							// was syncro, so now the inventory can be downloaded hence 
							// the val flag must be true
							validityFlag = true;
						}
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
						uploaded = true;
					}
				}
				else {
					allChecked++ ;
                }
			}
			catch (exception &e) {
				ACS_LOG_NOTIFY_EX(e);
				ACS_LOG_WARNING("Exception got while uploading filename: \"" << _inventory.getFileName() << "\", fileversion: \"" << _inventory.getFileVersion() << "\" SmId: " << tocheck[i].first->SMId() << " trying with next (if any).") ;
				tocheck[i].second = true ; // do not check at this item
				//SMListToUpload[i].synchronous=false;
				
				syncMissingActions.push_back(make_pair<dbPersistent::IdType, dbPersistent::IdType> (_inventory.getId(), tocheck[i].first->SMId()));

				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
								 mc_param("equipmentid", tocheck[i].first->SMId()) <<
								 mc_param("message", string("Exception got from storage manager ") + e.what()) <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				
			}
		}
		
		if (atLeastOneUploadComplete) {
			updateInventory = storeOnDatabase(false, validityFlag, uploadPath) ;   ////// controllare ....
        }
		yield() ;	
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "outer verification loop: allChecked=" << allChecked) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end of verification loop: allChecked=" << allChecked) ;

    // if t_sm_sync_missingactions is in db and at least one file has been uploaded...
	if (hasMissingActionsInSchema && uploaded)
	{
		dbConnectionWrapper pw(getPool()) ;
	    dbConnection &theConnection = pw ; // PRQA S 3050
        size_t sMA_size = syncMissingActions.size() ;
		for (size_t s = 0; s < sMA_size; ++s)
		{
			try
			{
				// add a record on t_sm_sync_missingactions to trace the uploading failure
				dbSmartPersistent therecord ("t_sm_sync_missingactions", theConnection);

				// Set the record on the database
				therecord.set("inv_id",  syncMissingActions[s].first, theConnection);
				therecord.set("sm_id", syncMissingActions[s].second, theConnection );
				string operation("UPLOAD");
				therecord.set("operation", operation, theConnection);
				therecord.save(theConnection);
				ACS_LOG_INFO("Added item inv_id #" << syncMissingActions[s].first << " and sm_id # " << syncMissingActions[s].second << " on t_sm_sync_missingactions." );

			}
			catch (exception &e) 
			{
				ACS_LOG_WARNING("Unable to insert the record in t_sm_sync_missingactions for inv_id # "<< syncMissingActions[s].first << " and sm_id # " << syncMissingActions[s].second);
			}
		}
	}
		
	
	if (!_externalRepositoryPassed) { 
		dbConnectionWrapper pw(getPool()) ;
    	dbConnection &theConnection = pw ; // PRQA S 3050
		removeInventoryForced(fromDB, theConnection) ;
		ACS_THROW(UploadException("Unable to upload on every Storage Manager. See previous log.")) ; // PRQA S 3081
	}

	// Store in bidirectional cache if configured
	try {
		dbInventoryObjectCacheManager cacheManager;
		dbPersistent * recordPtr = &_inventory ;
		bool cacheManagerConfigured = cacheManager.checkIsConfigured(*recordPtr);
		if( cacheManagerConfigured ) {
			if( cacheManager.uploadFile(uploadPath) ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "filename \"" <<  _inventory.getFileName() << "\" fileversion \"" <<  _inventory.getFileVersion() << "\" (source path \"" <<  uploadPath << "\") successfully uploaded on cache layer." );
				ACS_LOG_PRIVINFO("Filename \"" <<  _inventory.getFileName() << "\" fileversion \"" <<  _inventory.getFileVersion() << "\" succesfully uploaded on \"" << cacheManager.getCachePath() << "\" cache");
			}
			else {
				ACS_LOG_WARNING("Cannot upload filename \"" <<  _inventory.getFileName() << "\" fileversion \"" <<  _inventory.getFileVersion() << "\" on \"" << cacheManager.getCachePath() << "\" cache");
			}
		}
		else { // just for debug
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Bidirectional cache layer not configured.");
		}
	}
	catch( exception &e )
	{
		ACS_LOG_WARNING("WARNING exception got: '" << e.what() << "' while uploading on cache layer.") ;
		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
						 mc_param("message", string("Exception got from cache layer manager ") + e.what()) <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("ARC") << MCFunction ("Archive")) ;
	}

			
	
	
	bool deleteFilePath = false; // just for warning message
	try {
		if( deleteUploadedFilepathOnSuccess() ) {
			deleteFilePath = true;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << uploadPath << "\" must be deleted after upload");
			File::remove(uploadPath);
		}
	}
	catch( exception &ex )
	{
		ostringstream exMsg;
		exMsg << "The following error occur while uploading inventory with filename=\"" << _inventory.getFileName() << "\" and version=" << _inventory.getFileVersion()
				<< " (id=" <<  _inventory.getId() << "): ";
		if(deleteFilePath) { 
			exMsg << "unable to remove \"" << uploadPath << "\" after successful upload, please check why and REMOVE IMMEDIATELY";
		}
		else {
			exMsg << "error from deleteUploadedFilepathOnSuccess() check configuration of the caller and in case REMOVE PATH IMMEDIATELY";
		}
		ACS_LOG_TRYCATCHNOTIFY_EX(exCriticalException( ex, exMsg.str() ));
	}

			
	if (!updateInventory) {
		ACS_LOG_WARNING("WARNING: last attempt to update inventory.") ;
		updateInventory = storeOnDatabase(true, validityFlag, uploadPath) ; // uploadPath not really used in this method
	}

    if (!this->restoreOnSync())
    {
        size_t smlsize = SMListToUpload.size() ;
	    for (size_t i=0 ; i < smlsize ; i++) {

			dbConnectionWrapper pw(getPool()) ;
			dbConnection &theConnection = pw ; // PRQA S 3050
	
		    if(!SMListToUpload[i].synchronous)
		    {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage Id " << SMListToUpload[i].id
					    << " Storage upload cost " << SMListToUpload[i].cost
					    << " Storage upload synchronous  " << boolalpha << SMListToUpload[i].synchronous);
		
			    dbSMactions* uploadaction = dynamic_cast<dbSMactions*>(dbPersistentFactoryChain::instance()->newObject(dbSMactions::staticClassName(), theConnection.getKey())) ; // PRQA S 3081
			    dbPersistentScopeGuard scopeG(uploadaction) ;
			    ACS_COND_THROW(!uploadaction,exCriticalException("Cannot create objects of type dbSMactions")) ; // PRQA S 3081
			
			    uploadaction->setSMId(SMListToUpload[i].id, theConnection);
			    uploadaction->setFileName(_inventory.getFileName(), theConnection);
			    uploadaction->setFileVersion(_inventory.getFileVersion(), theConnection);
			    uploadaction->setOperation("UPLOAD", theConnection);
                uploadaction->setInventoryId (_inventory.getId(), theConnection);
                uploadaction->setUniqueId (_inventory.getUniqueId(), theConnection);

			
			    try
			    {
				    uploadaction->save(theConnection);
			    }
			    catch (exception& ex)
			    {
				    ACS_LOG_WARNING ("Unable to insert the upload action for inventory id [" << _inventory.getId() << "] on the SM Id [" << SMListToUpload[i].id << "]");
				    ACS_LOG_NOTIFY_EX (ex);
			    }
		
		    }

        }
    }
	

	setCompleted(true) ;

    // the upload is completed here can issue the message for MC
	// APF-270
	ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
					 mc_param("filesize", _inventory.getDataSize() + _inventory.getHeaderSize() ) << mc_metric(Metric_IN) << 
					 MCCode ("ARC") << MCFunction ("Archive")) ;
	


}

string dbUploadInventoryFile::getFile(int storageManId,const string& smtype,int id,bool defaultpacking)
{
	return "" ;
}


void dbUploadInventoryFile::putFile(const string &path, dbPersistent::IdType storageManId,const string& smtype,bool defaultpacking,vector<pair<SMInterface*,bool> >& tocheck) throw ()
{
	try {
		string newpath = path ;	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "packing path: " << newpath) ;
		pack(storageManId ,defaultpacking,newpath);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "packed path: " << newpath) ;
		if (newpath != path) { // remove newpath if it is a NEW path
			_tmpfile.push_back(new FileGuard(newpath));
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "asking for a new SMInterface(" << "," << smtype << "," << storageManId << ", ...)") ; 
		SMInterface* storageup = SMInterfaceFactoryMethod::getInterface(smtype, storageManId,&_inventory, getPool()); // PRQA S 3000
		if(!storageup)
		{
			ostringstream os ;
			os << "No StorageManager defined for type " << smtype << " Smid " << storageManId ;
			ACS_THROW(UploadException(os.str())); // PRQA S 3081
		}

        storageup->setClientName(getClientName()) ; // set the client name for the current action
		if (ACS_CLASS_GET_DEBUG() == simulateNoCommunicationWithSM) { 
			ACS_THROW(UploadException("Storage Manager comunication failure (simulated)")) ; // PRQA S 3081
		}
		
		try {
			dbInventoryObjectOperatorIF oper;
			storageup->upload(newpath,oper);
		}
		catch(exception &) {
			delete storageup ;
			throw ;
		}
		tocheck.push_back(pair<SMInterface*,bool>(storageup,false));
	}
	catch (exception &e) {
		
	#ifdef COSMO
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	#endif
		    ACS_LOG_WARNING("WARNING dbUploadInventoryFile::putFile exception got: <<<" << e.what() << ">>> while uploading on storage type "<< smtype << " id "<<storageManId) ;
	#ifdef COSMO
		ACS_CLASS_END_DEBUG
	#endif
	}
}

void dbUploadInventoryFile::pack(dbPersistent::IdType storageManId , bool defaultpacking, string& path)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: \"" << path << "\"") ;

	ACS_COND_THROW(path.empty(),exIllegalValueException("dbUploadInventoryFile::pack: null string as path")) ; // PRQA S 3081

	string strategyconf(File::tempname("/tmp/storage", "rs")) ;	// PRQA S 3050 
    // do not merge the above line and the one below !!!
    FileGuard strategyconf_guard(strategyconf) ;

	string packingstrategy;
    const string &dbKey = getPool().getKey() ;
    SQLString querySMString(dbKey, SQLString::LOBJ_DOWNLOAD ) ;
    querySMString.setColumnAndFileName( "Parameters", strategyconf ); // PRQA S 3050

	if(defaultpacking)
	{

        string table = "T_Storagemanager" ;
        querySMString.addTableList( table ) ;
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}
	else
	{
        string table = "T_Storagepriority" ;
        querySMString.addTableList( table ) ;
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "FileType" , dbParam::EQUAL_TO, db::toSQL(dbKey, _inventory.getFileType())) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}


	// Release the dbConnection ASAP
    {
		dbConnectionWrapper pw(getPool()) ;
		dbConnection &theConnection = pw ; // PRQA S 3050

		dbQuery smQuery(theConnection, querySMString, "FAUPL parameters dwnl") ;	

		smQuery.downloadClob();
    }

    // "check if correctly" downloaded not required.

	// change permissions allowing other users to override this file later.
	File::chmod(strategyconf,0664, File::AddPerm) ; // PRQA S 3050, 4400

	rsResourceSet rset;
	XMLIstream ifile( strategyconf); // PRQA S 3050
	ifile >> rset ;

	rset.getValue("SMConfiguration.PackingStrategy.name",packingstrategy) ;

	SMPackingStrategy* strategy = SMPackingStrategyFactoryChain::instance()->getStrategy(packingstrategy, _inventory, getPool());
	SMPackingStrategyScopeGuard guard(strategy) ;
	ACS_COND_THROW(!strategy,SMPackingStrategyFactory::NotFoundException("No PackingStrategy got with name \"" + packingstrategy + "\" for dbInventory" )) ; // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "packingstrategy: \"" << packingstrategy << "\"") ;
	
	string newpath;
	strategy->pack(path, ifile , newpath);
	path = newpath;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "after packing path is: \"" << path << "\"") ;
}

void dbUploadInventoryFile::unpack(dbPersistent::IdType storageManId , bool defaultpacking, string& newpath)
{
	string strategyconf(File::tempname("/tmp/storage", "rs")) ;	// PRQA S 3050
    // do not merge the above line and the one below !!!
    FileGuard strategyconf_guard(strategyconf) ;
    
    const string &dbKey = getPool().getKey() ;
	string packingstrategy;
    SQLString querySMString(dbKey, SQLString::LOBJ_DOWNLOAD ) ;
    querySMString.setColumnAndFileName( "Parameters", strategyconf ); // PRQA S 3050
	if(defaultpacking)
	{

        string table = "T_Storagemanager" ;
        querySMString.addTableList( table ) ;
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}
	else
	{
        string table = "T_Storagepriority" ;
        querySMString.addTableList( table ) ;
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "FileType" , dbParam::EQUAL_TO, db::toSQL(dbKey, _inventory.getFileType())) ) ; // PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}

	// Release the connection ASAP
	{
		dbConnectionWrapper pw(getPool()) ;
		dbConnection &theConnection = pw ; // PRQA S 3050
		dbQuery smQuery(theConnection, querySMString, "FAUPL parameters dwnl") ;	


		smQuery.downloadClob();
	}

    // "check if correctly" downloaded not required.


	// change permissions allowing other users to override this file later.
	File::chmod(strategyconf,0664, File::AddPerm) ; // PRQA S 3050, 4400

	rsResourceSet rset;
	XMLIstream ifile(strategyconf); // PRQA S 3050
	ifile >> rset ;

	rset.getValue("SMConfiguration.PackingStrategy.name",packingstrategy);

	SMPackingStrategy* strategy = SMPackingStrategyFactoryChain::instance()->getStrategy(packingstrategy, _inventory, getPool());
	SMPackingStrategyScopeGuard guard(strategy) ;
	ACS_COND_THROW(!strategy,SMPackingStrategyFactory::NotFoundException("No PackingStrategy got with name " + packingstrategy + " for dbInventory" )) ; // PRQA S 3081
	
	strategy->unpack(newpath, ifile , newpath);

	return ;
}


void dbUploadInventoryFile::removeInventoryForced(bool wasFromDb, dbConnection &theConnection) // PRQA S 4020
{
	if (wasFromDb) {
		return ;
    }
    
	try
	{
		_inventory.remove(theConnection);
	}
	catch(exception &ex)
	{
		ACS_LOG_WARNING("WARNING: forced removal of inventory metadata was not possible. The exception got is: " << ex.what() << "\n" 
			<< "The t_inventory will contain incongruence for the file: " << _inventory.getFileName() << " correct it as soon as possible.") ;
	}
}

bool dbUploadInventoryFile::storeOnDatabase(bool emitException, bool valflag, const string &path) // PRQA S 4020
{
	try {
 	    _inventory.setValidityFlag(valflag || _inventory.getValidityFlag() ); // keep it at true 
            
            
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to update inventory record.") ;

		if (ACS_CLASS_GET_DEBUG() == simulateDatabaseUpdateFailure) {
			ACS_THROW(UploadException("Database update failure (simulated)")) ; // PRQA S 3081
		}
		
		
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050
		_inventory.update(theConnection);

		_savePassed=true;
		return true ;
	}
	catch(exception &ex)
	{
		ACS_LOG_ERROR("dbUploadInventoryFile::storeOnDatabase error got updating inventory: " << _inventory.getFileName() << " Exception was:" << ex.what()) ;
		if (emitException) { throw ; }
		return false ;
	}
}

void dbUploadInventoryFile::debugMessage()
{
	if (!_debugMessageEmitted) {
		_debugMessageEmitted = true ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbUploadInventoryFile_DEBUG=" << simulateFirstDbCommunicationFailure << " to simulate fisrt DB communication failure ") ; 
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbUploadInventoryFile_DEBUG=" << simulateNoCommunicationWithSM << " to simulate SM communication failure ") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbUploadInventoryFile_DEBUG=" << simulateDatabaseUpdateFailure << " to simulate DB update failure ") ;
	}
}



_ACS_END_NAMESPACE
