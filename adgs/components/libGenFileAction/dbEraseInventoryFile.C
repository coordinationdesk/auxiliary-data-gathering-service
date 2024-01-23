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
	Revision 7.15  2017/11/30 16:05:05  marfav
	VEN-2260
	the procedure p_remove_obsolete_actions is performed
	only in the case the file is no more available for download in any
	synchronous storge manager
	
	Revision 7.14  2017/11/27 18:03:16  marpas
	deprecated function call removed
	
	Revision 7.13  2017/04/13 14:28:51  nicvac
	S2PDGS-1747: Syslog messages revised.
	
	Revision 7.12  2016/07/15 13:39:02  marfav
	CSGACQ-86 Using only SQLString statements
	
	Revision 7.11  2016/07/15 12:23:21  marfav
	Using slqstring for custom procedure call
	
	Revision 7.10  2016/04/29 16:03:17  marpas
	ids are of dbPersistent::IdType type - to be overall consistent
	coding best practices applied
	
	Revision 7.9  2016/04/11 13:29:59  marpas
	adopting new db interfaces
	
	Revision 7.8  2016/03/26 21:42:14  marpas
	using new db I/F
	
	Revision 7.7  2015/11/26 14:59:41  marfav
	mc_params are now lowercase
	
	Revision 7.6  2015/11/05 11:26:18  damdec
	Check on the existance of t_sm_sync_missingactions on Db introduced.
	
	Revision 7.5  2015/10/28 12:05:57  damdec
	PDSEV-16: blob handling removed.
	
	Revision 7.4  2015/10/26 13:57:43  damdec
	Handling of t_sm_sync_missingactions introduced.
	
	Revision 7.3  2015/10/20 13:44:31  damdec
	APF-335: handling of erase failure on synchronous SM introduced.
	
	Revision 7.2  2015/08/17 08:22:11  marfav
	Always reporting equipment id when delete is successful
	
	Revision 7.1  2015/08/17 07:59:01  marfav
	APF-270 work in progress
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.2  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.1  2013/09/27 18:23:07  nicvac
	Fixed log info.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.1  2013/09/19 15:39:56  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.25  2013/04/12 12:16:30  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 1.24  2013/03/29 13:53:03  marpas
	fix a regression introduced with the new File::tempname interface
	
	Revision 1.23  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.22  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.21  2013/03/11 13:25:19  marpas
	tempname interface changed
	
	Revision 1.20  2013/03/11 12:51:26  marpas
	FileGuard is a template specialization now
	deprecated functions substituted
	
	Revision 1.19  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.18  2012/12/14 14:11:16  marfav
	Passing the connection to the schema provider
	
	Revision 1.17  2012/12/12 16:37:53  marfav
	Added support to AllowDefaultCost on t_storagemanagers (S1PDGS-30147)
	
	Revision 1.16  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.15  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.14  2012/01/23 13:52:06  marpas
	interface aligned
	
	Revision 1.13  2010/08/10 09:51:53  marpas
	Oracle11 support fixed
	
	Revision 1.12  2010/08/10 09:37:45  marpas
	Oracle11 support completed
	
	Revision 1.11  2010/03/08 14:53:33  enrcar
	EC:: Oracle DB handled
	
	Revision 1.10  2009/06/19 13:44:03  ivafam
	Bug Fixed
	
	Revision 1.9  2009/06/17 09:26:18  ivafam
	Aligned to database change
	
	Revision 1.8  2009/06/15 13:54:02  ivafam
	Bug Fixed
	
	Revision 1.7  2009/02/05 17:26:44  ivafam
	Bug Fixed VEN-989
	
	Revision 1.6  2009/02/02 16:04:26  ivafam
	Bug fixed in query
	
	Revision 1.5  2009/01/30 09:06:48  ivafam
	Work in progress
	
	Revision 1.4  2009/01/29 10:57:18  marpas
	stored procedure invocation to remopve obsolete actions
	
	Revision 1.3  2008/12/03 17:19:16  ivafam
	Work in progress
	
	Revision 1.2  2008/11/11 14:58:20  ivafam
	Work in progess
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	

*/

#include <dbEraseInventoryFile.h>

#include <dbFileActionFactoryChain.h>
#include <GFASMTmp.h>
#include <MCFilterables.h>
#include <dbConv.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <SMInterface.h>
#include <SMInterfaceScopeGuard.h>
#include <SMInterfaceFactoryMethod.h>
#include <dbSMactions.h>
#include <dbPersistentScopeGuard.h>
#include <dbPersistentFactoryChain.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbInventoryObjectOperatorIF.h>
#include <dbInventoryObject.h>
#include <FileGuard.h>
#include <File.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc ;




ACS_CLASS_DEFINE_DEBUG_LEVEL(dbEraseInventoryFile)

// constructor
dbEraseInventoryFile::dbEraseInventoryFile(dbConnectionPool &pool, dbInventoryObject &s, int storageid) :
	dbEraseFile(pool, s,storageid) ,
	_inventory(s) // PRQA S 2528
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


dbEraseInventoryFile::~dbEraseInventoryFile() ACS_NOEXCEPT  
{
    // PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    // PRQA L:L1
}


void dbEraseInventoryFile::start() // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	ACS_COND_THROW(isComplete(),dbPersistent::InvalidUse("File has been already erased.")) ; // PRQA S 3081 
	 
	
	ostringstream exM;
	exM << "Starting to erase file \"" << _inventory.getFileName() << "\" version \"" << _inventory.getFileVersion() << "\". ";
	if(0 != _storageid) {
		exM << "On storage  " << _storageid;
	}
    else {
		exM << "On All storage  " ;
    }
	ACS_LOG_PRIVINFO(exM.str());
	
    dbConnectionWrapper pw(getPool()) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	dbInventoryObjectOperatorIF oper;
	
	//check if isFromDatabase.
	if(!_inventory.isFromDatabase())
	{
		if(!_inventory.load(theConnection))
		{
			ACS_THROW(PreconditionException("Error erasing file \"" + _inventory.getFileName() + "\" version \"" + _inventory.getFileVersion() + "\". Inventory object has not been saved in database.")) ; // PRQA S 3081
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "preconditions passed.") ;
	
	const string invSmTableName(dbFileActionFactoryChain::instance()->getInvSmTableName(_inventory)) ;
	ACS_COND_THROW( invSmTableName.empty(), exIllegalValueException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inventorysm table name=\"" << invSmTableName << '\"');


    // be sure that the t_storagemanager table has the field allowdefaultcost
    bool hasADCfield = dbSchemaMetaDataProvider::hasField ("t_storagemanager", "allowdefaultcost", theConnection); // PRQA S 4412

    SQLString countStar(dbKey, SQLString::FUNCTION ) ;
    countStar.setFunctionName("COUNT");
    countStar.addFunctionParameter("*");

	// subquery for the select count
	SQLString subSql (dbKey, SQLString::SELECT);
	subSql.addSelectList (countStar);
	subSql.addTableList ("t_storagepriority");

	{
		dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_storagepriority", "id", dbParam::EQUAL_TO , "ism.StorageManID")) ;
		subSql.addANDWhereParameter (pp);
	}


	SQLString sqlString (dbKey, SQLString::SELECT );

   	sqlString.addSelectList( "ism.id" );	
   	sqlString.addSelectList( "ism.StorageManId" );	
   	sqlString.addSelectList( "sm.type" );	
   	sqlString.addSelectList( "p.erasecost" );	
   	sqlString.addSelectList( subSql, "policycount" );	// the select count subquery
    if (hasADCfield)
    {            
	   	sqlString.addSelectList( "sm.allowdefaultcost", "adc" );	
    }

    sqlString.addTableList( invSmTableName, "ism" );

	// Add Join on t_storagemanager
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "sm", "id", dbParam::EQUAL_TO , "ism.StorageManId")) ;
    	sqlString.addJoinList(SQLDialect::LEFT, " t_storagemanager", "sm", pp) ;
	}

	// Add Join on t_storagepriority
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "p", "id", dbParam::EQUAL_TO , "ism.StorageManId")) ;
		pp.addParameter(dbParam(dbKey, "p", "filetype", dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getFileType()) )) ;
    	sqlString.addJoinList(SQLDialect::LEFT, " t_storagepriority", "p", pp) ;
	}

	// Where parameters
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "ism", "inv_id", dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId()) )) ;
		sqlString.addANDWhereParameter (pp);
	}

	if(_storageid != 0 )
	{
    	dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "ism", "StorageManId", dbParam::EQUAL_TO , db::toSQL(dbKey, _storageid) )) ;
		sqlString.addANDWhereParameter (pp);	
	}

    sqlString.addOrderByList("p.erasecost", SQLString::DESC);

	dbQuery query(theConnection,sqlString, "FAERASEL SM") ;
	dbSet smset;
	query.execForSet(smset) ;

	vector <GFASMTmp> SMList ;

	int notAllowed = 0 ;
    size_t smsetsize = smset.size() ;
	for (size_t i=0; i < smsetsize; ++i) {
		dbRow row(dbKey) ;
		smset.getNextRow(row);

		string smtype;
		int storageManId = 0 ;
        int id = 0 ;
		int cost = 0 ;
		bool isNull = false ;
        bool adc = true; 
        unsigned int policycounter = 0;            

		row.column(0,id);
		row.column(1,storageManId);
		row.column(2,smtype);   // PRQA S 4400 6
		row.column(3,cost,&isNull);            
        row.column(4,policycounter);
        if (hasADCfield)
        {
            row.column(5,adc);
        }            

        // Algo is:
        // 1 - if no policies (policycounter = 0) allow the operation (cost set to 1)
        // 2 - if some policies
        //     2.1 - if cost is set use the cost
        //     2.2 - if cost is not set use +1 if ADC is true or -1 if ADC is false

        if (policycounter == 0)
        {
            cost = 1;
        }
        else
        {
            if (isNull)
            {
                cost = ( adc ? 1 : -1 ); // allow default cost // PRQA S 3380
            }
        }

		if (cost >=0) { // ignore negative cost
			GFASMTmp sm(storageManId,smtype,id) ; // PRQA S 3000
			sm.cost = cost ;
			SMList.push_back(sm);
		}
		else {
			notAllowed++ ;
        }
	}


	// There are the following cases:
	// 1) there is no SM on which the file is stored
	// 2) there are some SMs which do not allow the erasure

	// note 4 - if the method was called once before and did not remove the BLOB, 
	// the blob size is still > 0, so the method will continue, and becasue no 
	// SM has the file any longer, ther SM erasure will stay true (see erase flag before the next loop
	// see note 1, 2, 3
	if(SMList.empty()) {
		if (notAllowed > 0) {
			; // nothing to do with SMs ... continue with standard erasure from DB
		}
		else if (_inventory.getValidityFlag()) {
			ACS_LOG_WARNING("The file " << _inventory.getFileName() << " version " 
			   << _inventory.getFileVersion() << " is not stored on any storage manager but its validity flag is true. Continuing erasure") ;
		}
		else if(!_inventory.getValidityFlag()) 
		{
			setCompleted(true) ;
			return;
		}
        else {}
	}

	// note 1, if erasure is not completed the exception is thown just after the following loop
	// see note 2 
	bool erased = true ;
    size_t smsize = SMList.size() ;
	for(size_t i=0; i< smsize; ++i)
	{
		try
		{
		        db::Transaction transaction(theConnection) ;
			transaction.begin_work("") ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "getting interface for type:" << SMList[i].type 
                    << " Smid:" << SMList[i].id) ;
			SMInterface* storage=  SMInterfaceFactoryMethod::getInterface(SMList[i].type, SMList[i].id, &_inventory, getPool()); // PRQA S 3000
			SMInterfaceScopeGuard guard(storage);
			if(0 == storage)
			{
				ostringstream os ;
				os << "No storage manager interface for SMid: " << SMList[i].id<< " type: " << SMList[i].type ;
				ACS_THROW(EraseException(os.str())); // PRQA S 3081
			}

            storage->setClientName(getClientName()) ; // set the client name for the current action

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got interface for type:" << storage->SMType() 
					<< " Smid:" << storage->SMId()) ;

			string keyFile = File::tempname("/tmp/Remove_", "key") ; // PRQA S 3050
            // do not merge the above line and the one below !!!
			FileGuard KeyGuard(keyFile) ; // guard over a file that does not exists now !!!
			try
			{
				storage->erase(SMList[i].sminvrecordid,oper,&keyFile) ;
			}
			catch (exception& ex)
			{
				ostringstream os;
				os << "An error occured during the erase of invsm record with id #" << SMList[i].sminvrecordid << " on sm " << storage->SMId() << endl;
				// only synchronuos storage can throw an exception
				if (storage->isEraseSynchronous()) 
				{
					ACS_THROW(SyncEraseException(ex, os.str()));
				};
				os << "No async action will be emitted." << endl;
				ACS_LOG_WARNING(os.str());
				ACS_LOG_NOTIFY_EX(ex);
				// Key for t_smaction not saved
				keyFile = "";
			}

			{
				// remove all download and upload action from the smaction table
				SQLString cleanString(dbKey, SQLString::DELETE) ;
				string table = "T_SMactions" ;
                cleanString.addTableList( table ) ;
				{
                    dbQueryParameters storageparameters(dbKey) ; 
					storageparameters.addParameter(dbParam(dbKey,table,"smid", dbParam::EQUAL_TO, db::toSQL(dbKey, storage->SMId()))) ; // PRQA S 3050, 3081
                    cleanString.addANDWhereParameter(storageparameters) ;
        		}

				{
                    dbQueryParameters inventoryIdparam(dbKey) ; 
					inventoryIdparam.addParameter(dbParam(dbKey,table,"inv_id", dbParam::EQUAL_TO, db::toSQL(dbKey, _inventory.getId()))) ; // PRQA S 3050, 3081
					cleanString.addANDWhereParameter(inventoryIdparam) ;
        		}
				{
                    dbQueryParameters operparameters(dbKey) ; 
                    vector<string> ops ; 
                    ops.push_back("UPLOAD") ;
                    ops.push_back("DOWNLOAD") ;
					operparameters.addParameter(dbParam(dbKey,table,"operation", dbParam::IN, ops) ) ; 
					cleanString.addANDWhereParameter(operparameters) ;
                }

				{
                    dbQueryParameters endparameters(dbKey) ; 
					DateTime currentdate;
					endparameters.addParameter(dbParam(dbKey,table,"enddate", dbParam::MAJOR_OF,  db::toSQL(dbKey, currentdate,dbConv::YearToFraction))) ; // PRQA S 3050, 3081
					cleanString.addANDWhereParameter(endparameters) ;
                }


				dbQuery cleanquery(theConnection,cleanString, "FAERADEL T_SMactions") ;

				try
				{
					cleanquery.execForStatus() ;
				}
				catch(exception &ex)
				{
					/// Leggere path
                    string path = File::tempname("/tmp/erase_inventory_action", "sql") ; // PRQA S 3050
					ofstream coutF(path.c_str());
					coutF << cleanquery.sqlString() << "\n";
					coutF.close();
                    ACS_LOG_ERROR("Cannot delete selected SMactions, SQL left in: \"" << path << "\"") ;
					throw;
				}

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "removed all download e upload action from the smaction table" << storage->SMType() 
						<< " Smid:" << storage->SMId()
						<<  " query " << cleanquery.sqlString()) ;

			}

			if(!keyFile.empty() && !storage->isEraseSynchronous())
			{
				 // Add a remove action in the smaction table

				 dbSMactions* eraseaction = dynamic_cast<dbSMactions*>(dbPersistentFactoryChain::instance()->newObject(dbSMactions::staticClassName(), theConnection.getKey())) ; // PRQA S 3081
				 dbPersistentScopeGuard scopeG(eraseaction) ;
				 ACS_COND_THROW(!eraseaction, exCriticalException("Cannot create objects of type dbSMactions")) ; // PRQA S 3081

				 eraseaction->setSMId(storage->SMId(), theConnection); // PRQA S 3000
				 eraseaction->setFileName(_inventory.getFileName(), theConnection);
				 eraseaction->setFileVersion(_inventory.getFileVersion(), theConnection);
				 eraseaction->setOperation("ERASE", theConnection);
				 eraseaction->setFilePath("parameter",keyFile, theConnection);
				 eraseaction->setInventoryId (_inventory.getId(), theConnection);
				 eraseaction->setUniqueId (_inventory.getUniqueId(), theConnection);


				 try
				 { 
					eraseaction->save(theConnection);
				 }
				 catch(exception &ex)
				 {
					/// Leggere path
					bool blobFieldsPresent = false;
					SQLString cleanquerystring = eraseaction->saveQuery(blobFieldsPresent, theConnection);
					string path=File::tempname("/tmp/erase_inventory_action", "sql") ; // PRQA S 3050
                    ofstream coutF(path.c_str());
					coutF << cleanquerystring << "\n";
					ACS_LOG_ERROR("Cannot save keyfile on SMactions , SQL left in: \"" << path << "\"") ;
                    throw;
				 }

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "delete action from the smaction table" << storage->SMType() 
						<< " Smid:" << storage->SMId()) ;
			}


			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File Erased: " << boolalpha << erased) ;

			transaction.commit() ;

				// APF-270 delete from _storageid
				ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
								 mc_metric(Metric_DELETE) << 
								 mc_param("equipmentid", SMList[i].id) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;

		}
		catch(exception &ex)
		{
			ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
							 mc_param("equipmentid", SMList[i].id) <<
							 mc_param("message", string("Unable to remove from the storage manager: ") + ex.what()) <<
							 mc_metric(Metric_FAIL) << 
							 MCCode ("ARC") << MCFunction ("Archive")) ;		        
                                                         
			if (dbSchemaMetaDataProvider::existTable("t_sm_sync_missingactions", theConnection))
                        {
				try
				{
					// APF-335: handling of erase failure on synchronous SM
					db::Transaction transaction(theConnection) ;
					transaction.begin_work("handleErase") ;

					// add a record on the t_sm_pendingerase to trace the erase failure
					dbSmartPersistent therecordE ("t_sm_sync_missingactions", theConnection);
					therecordE.set("sm_id", SMList[i].id, theConnection);
					therecordE.set("invsm_id", SMList[i].sminvrecordid, theConnection);
					string operation("ERASE");
					therecordE.set("operation", operation, theConnection);
					therecordE.save(theConnection);
					ACS_LOG_PRIVINFO("Added " << invSmTableName << " id# " << SMList[i].sminvrecordid << " in t_sm_sync_missingactions");

					// Set to NULL inv_id on the record on the database
					dbSmartPersistent therecord (invSmTableName, theConnection);
					therecord.load( theConnection, SMList[i].sminvrecordid); 
					therecord.unset("inv_id",  theConnection);
					therecord.update(theConnection);
					ACS_LOG_PRIVINFO("Set to null inv_id for record id# " << SMList[i].sminvrecordid << " in " << invSmTableName);

					transaction.commit() ;
				}
				catch(exception &x)
				{
					ACS_LOG_ERROR("An error occurred during updating of t_sm_sync_missingactions and " << invSmTableName << ". No update performed.");
					ACS_LOG_NOTIFY_EX(x);
				}
            }

#ifdef COSMO
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
#endif
			ACS_LOG_NOTIFY_EX(ex) ;
			ACS_LOG_WARNING("Continuing erasure on next SM if any.") ;
			erased=false;
#ifdef COSMO
			ACS_CLASS_END_DEBUG
#endif
		}
	}


	// The removal of obsolete actions is needed only when the file
	// is no more available for download from any sync storage manager
	if ( not oper.canGetFromSynchSM (_inventory, theConnection) )
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "File no more available in sync SM for download. Calling p_remove_obsolete_action now.");
		SQLString qryStr(dbKey, SQLString::CUSTPROCEDURE ) ;

		// Add Table list
		qryStr.setProcedureName("p_remove_obsolete_actions");
		qryStr.addProcedureParameter(db::toSQL(dbKey, _inventory.getFileName()));
		qryStr.addProcedureParameter(db::toSQL(dbKey, _inventory.getFileVersion()));

		dbQuery qry(theConnection, qryStr, "FAERADEL ObsoleteSmAction") ;
		qry.execForStatus() ;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "File can be retrieved from sync sm. Procedure p_remove_obsolete_action has not been called.");
	}


  	//ACS_COND_THROW(!erased,EraseException("Error erasing file \"" + _inventory.getFileName() + "\" giving up." )); // PRQA S 3081
	// note 2 after the exception is thrown, the next catches will exit the method, 
	// causing the blob (if any) to stay in the DB. No modification is done to the validity flag.
	// see note 3 
	
	SQLString countFun (dbKey, SQLString::FUNCTION);
	countFun.setFunctionName("COUNT") ;
    countFun.addFunctionParameter("*") ;
    

    SQLString sqlStringSM (dbKey, SQLString::SELECT);
	sqlStringSM.addSelectList( countFun ) ;

    sqlStringSM.addTableList(invSmTableName);

    	
 	{
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, invSmTableName,"inv_id" ,  dbParam::EQUAL_TO ,  db::toSQL(dbKey, _inventory.getId()) )) ; // PRQA S 3050, 3081
        sqlStringSM.addANDWhereParameter(pp) ;
    }
            
	int numberofcurrentStorage=0 ;
    try
    {
		dbQuery querySM(theConnection,sqlStringSM, "FAERASEL "+invSmTableName) ;
        dbSet set ;
        querySM.execForSet(set) ; // no need to check tuples number: is a count
        dbRow row(dbKey);
        set.getNextRow(row) ; // first and only 
        row.column(0, numberofcurrentStorage) ;
   	}
    catch (exception& e)
    {
        ACS_THROW(EraseException( e, "EraseInventoryFile: cannot query for inventory count" )) ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query returned: " << numberofcurrentStorage) ;

	
	if((0 == numberofcurrentStorage))
	{
		// anyway 
		_inventory.setValidityFlag(false) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to update database.") ;
		try
		{
			_inventory.update(theConnection);
			ACS_LOG_INFO("Erased file \""  << _inventory.getFileName() << "\" \"" << _inventory.getFileVersion() << "\"") ;
		}
		catch(exception &ex)
		{
			ACS_THROW(EraseException(ex, "Error erasing file \"" + _inventory.getFileName() + "\" version \"" + _inventory.getFileVersion() + "\". Error invalidating"));
		}
	}
	setCompleted(true) ;


}



_ACS_END_NAMESPACE
