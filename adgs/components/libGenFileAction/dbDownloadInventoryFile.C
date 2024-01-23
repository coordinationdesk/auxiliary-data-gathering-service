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
	Revision 7.15  2018/02/26 10:19:10  lucio.pulvirenti
	S2PDGS-1939: registerClient: transaction removed. Catch performed on dbQuery::QueryExecuteException to go on if one of more simultaneous threads
	fails in record insert.
	
	Revision 7.14  2017/11/17 10:45:38  marpas
	qa warnings fixed
	code best practice applied
	
	Revision 7.13  2016/12/06 16:13:18  lucio.pulvirenti
	S3MPM-843: dbInventoryObjectCacheManager cache layer used.
	
	Revision 7.12  2016/09/16 08:51:30  nicvac
	S2PDGS-1544: Log handling revision (PRIVINFO messages).
	
	Revision 7.11  2016/08/02 12:18:30  damdec
	CGSACQ-94: LOBJ handling updated.
	
	Revision 7.10  2016/07/21 10:29:50  nicvac
	CSGACQ-86: Oracle porting. Time annotation for item downloaded.
	
	Revision 7.9  2016/07/20 10:07:56  nicvac
	CSGACQ-86: Oracle porting. Time annotation for item downloaded.
	
	Revision 7.8  2016/04/29 16:03:17  marpas
	ids are of dbPersistent::IdType type - to be overall consistent
	coding best practices applied
	
	Revision 7.7  2016/03/26 21:42:14  marpas
	using new db I/F
	
	Revision 7.6  2015/11/26 14:59:41  marfav
	mc_params are now lowercase
	
	Revision 7.5  2015/11/03 15:25:50  damdec
	PDSEV-12: operations on t_smactions can be disabled for Async SM.
	
	Revision 7.4  2015/10/28 12:03:49  damdec
	PDSEV-16: blob handling removed.
	
	Revision 7.3  2015/10/23 10:03:16  marpas
	some info filterables moved into private info group
	
	Revision 7.2  2015/08/13 15:25:13  marfav
	APF-270 work in progress
	
	Revision 7.1  2015/03/12 10:44:57  lucio.pulvirenti
	S2PDGS-1125: if dbInventoryObjectOperatorIF::TryAgainLaterToMigrateException is caught, no more DownloadAsynchException re-thrown and proper message is issued.
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.8  2015/02/03 10:34:08  lucio.pulvirenti
	S2PDGS-977: downloading: if either dbInventoryObjectOperatorIF::NotMigrableException or dbInventoryObjectOperatorIF::MigrationFailedException caught,
	FileNotDownloadableException is launched.
	
	Revision 6.7  2015/02/02 16:08:06  marfav
	S2PDGS-1072 upload without triggers implemented
	
	Revision 6.6  2015/01/29 17:05:30  lucio.pulvirenti
	S2PDGS-1010: dbInventoryObjectOperatorIF canGetFromSynchSM and restoreOnSynchSM IFs integrated.
	
	Revision 6.5  2015/01/29 10:35:34  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.4  2014/02/07 18:50:09  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	coding best practices applied
	qa and compilation warning fixed
	robustness improved
	
	Revision 6.3  2014/01/31 16:31:22  nicvac
	S2PDGS-623: Client registration: multiprocess insert could cause (nominal) exception. Handled.
	
	Revision 6.2  2013/11/13 14:00:26  marfav
	Limiting the scope of any connection
	
	Revision 6.1  2013/09/25 16:58:18  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.1  2013/09/19 15:39:56  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.27  2013/04/19 15:27:42  lucio.pulvirenti
	If base class simulatedDownload flag is true, download from storage manager is skipped. In this
	case a fake xml file is created by newly added simulateDownload method. In 50% of download processes on
	statistical base, given the real file size, an amount of time corresponding to a transfer rate of 10 MB/s is waited
	before the fake file is created on disk.
	
	Revision 1.26  2013/04/12 12:16:30  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 1.25  2013/04/03 13:47:32  marpas
	run does not longer gets a void*
	
	Revision 1.24  2013/04/03 12:49:28  marpas
	Thread::run gets a void*
	
	Revision 1.23  2013/03/29 13:57:14  marpas
	lation fixed
	
	Revision 1.22  2013/03/29 13:53:03  marpas
	fix a regression introduced with the new File::tempname interface
	
	Revision 1.21  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.20  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.19  2013/03/11 12:51:26  marpas
	FileGuard is a template specialization now
	deprecated functions substituted
	
	Revision 1.18  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.17  2012/12/14 14:11:16  marfav
	Passing the connection to the schema provider
	
	Revision 1.16  2012/12/13 17:30:48  marfav
	Async storage does not need a storage plugin anymore
	
	Revision 1.15  2012/12/12 16:37:53  marfav
	Added support to AllowDefaultCost on t_storagemanagers (S1PDGS-30147)
	
	Revision 1.14  2012/05/17 15:47:57  marfav
	Addedd file invalidation if data not available on any SM as per S1PDGS-3291
	
	Revision 1.13  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.12  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.11  2011/04/06 16:10:49  marpas
	debug improved
	
	Revision 1.10  2010/09/13 18:06:17  marpas
	exception message improved
	
	Revision 1.9  2010/08/10 09:37:45  marpas
	Oracle11 support completed
	
	Revision 1.8  2010/03/08 14:53:33  enrcar
	EC:: Oracle DB handled
	
	Revision 1.7  2009/11/04 14:40:06  ivafam
	Bug Fixed in query
	
	Revision 1.6  2009/06/26 10:43:07  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.5  2009/02/02 16:04:25  ivafam
	Bug fixed in query
	
	Revision 1.4  2009/01/29 10:58:11  marpas
	eless include removed
	
	Revision 1.3  2008/12/03 17:19:16  ivafam
	Work in progress
	
	Revision 1.2  2008/11/11 14:58:20  ivafam
	Work in progess
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	

*/


#include <dbDownloadInventoryFile.h>

#include <dbFileActionFactoryChain.h>
#include <dbInventoryObject.h>
#include <File.h>
#include <Filterables.h>
#include <SMInterface.h>
#include <SMInterfaceFactoryMethod.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbConv.h>
#include <SMInterfaceScopeGuard.h>
#include <dbInventoryObjectOperatorIF.h>
#include <dbInventoryObjectCacheManager.h>
#include <FileGuard.h>
#include <SMPackingStrategyScopeGuard.h>
#include <SMPackingStrategy.h>
#include <SMPackingStrategyFactory.h>
#include <SMPackingStrategyFactoryChain.h>
#include <XMLIstream.h>
#include <dbSchemaMetaDataProvider.h>

#include <ConfigurationSingleton.h>

#include <MCFilterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace acs::mc ;

_ACS_BEGIN_NAMESPACE(conf)

class dbDIFConfNotifier
{
public:
    dbDIFConfNotifier ()
    {
        ConfigurationSingleton::instance() -> addSpace (ConfigurationSingleton::ConfSpace ("libGenFileAction", "", false));
        ConfigurationSingleton::instance() -> addCallback (dbDIFConfNotifierCallback, this);
    }
    
    static void dbDIFConfNotifierCallback (void *cp)
    {
        if ( (not _messageSent) )
        {
            _messageSent = true;
            bool canInvalidate (false);
            if (not ( ConfigurationSingleton::instance() -> get() ).hasKey ("DownloadInventoryObject.CanInvalidateFile"))
            {
                ACS_LOG_PRIVINFO("Configuration key DownloadInventoryObject.CanInvalidateFile not found. Using default (false)") ;
            }
            else
            {
                (ConfigurationSingleton::instance() -> get() ).getValue ("DownloadInventoryObject.CanInvalidateFile", canInvalidate);
            }
            ACS_LOG_PRIVINFO("Invalidate files unknown to all storage managers set to [" << boolalpha << canInvalidate << ']') ;

        	if ( ! ( ConfigurationSingleton::instance()->get()).hasKey("DownloadInventoryObject.RegisterClient") ) {

        		ACS_LOG_PRIVINFO("Configuration key DownloadInventoryObject.RegisterClient not found. Using default (false)") ;
        	} else {

        		bool clientRegister(false);
        		(ConfigurationSingleton::instance()->get()).getValue ("DownloadInventoryObject.RegisterClient.enabled.value", clientRegister);
        		ACS_LOG_PRIVINFO("Inventory downloads Client registration enabled: "<<boolalpha<<clientRegister);
        	}

        }
    }
    
private:
    static bool _messageSent;
};

bool dbDIFConfNotifier::_messageSent (false);
namespace {
    dbDIFConfNotifier embeddeddbDIFConfNotifier;
}

_ACS_END_NAMESPACE
 


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDownloadInventoryFile)

// constructor
dbDownloadInventoryFile::dbDownloadInventoryFile(dbConnectionPool &pool, dbInventoryObject &s, const string &path,
												const string& clientName, bool downloadInvalidFile) :
	dbDownloadFile(pool, s,path) ,  
	_inventory(s) , // PRQA S 2528 
	_path(path),
	_clientName(clientName),
	_preconditionPassed(false) ,
	_downloadInvalidFile(downloadInvalidFile),
	_invSmTableName(dbFileActionFactoryChain::instance()->getInvSmTableName(s))
{
	ACS_COND_THROW( _invSmTableName.empty(), exIllegalValueException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileName: " << _inventory.getFileName() 
		    << " FileVersion: " << _inventory.getFileVersion()
		    << " Path: " << _path
		    << " ClientName: "<<_clientName) ;
}


dbDownloadInventoryFile::~dbDownloadInventoryFile() ACS_NOEXCEPT {}


void dbDownloadInventoryFile::start() 
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	ACS_COND_THROW(isComplete(), dbPersistent::InvalidUse("File has been already downloaded.")) ; // PRQA S 3081 2
    ACS_COND_THROW(running(), dbPersistent::InvalidUse("Thread is already running.")) ;
	Thread::start();	
}


bool dbDownloadInventoryFile::downloading()  // PRQA S 4020 
{
	if( running() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Still downloading.") ;
		
		return true;
	}
	
	exception *e_i = 0 ;
	runException(e_i) ;
    ACS_SMARTPTR<exception> e(e_i) ;
	if(e.get())
	{
		if (!_preconditionPassed)
		{
			// APF-270
			ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
							 mc_param("message", "Some precondition failed") <<
							 mc_metric(Metric_FAIL) << 
							 MCCode ("ARC") << MCFunction ("Archive")) ;
			ACS_THROW(PreconditionException (*e,"Error in downloading thread: some precondition failed")) ; //  PRQA S 3081
		}

		if( not isComplete() ) 
		{
			if ( dynamic_cast<dbInventoryObjectOperatorIF::MigrationFailedException*>(e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
								 mc_param("message", "Error in downloading thread") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(FileNotDownloadableException(*e,"Error in downloading thread")) ; // PRQA S 3081
			}
			
			if ( dynamic_cast<dbInventoryObjectOperatorIF::NotMigrableException*>   (e.get()) != 0 )
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
								 mc_param("message", "Error in downloading thread") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(FileDefinetelyNotDownloadableException(*e,"Definitive error in downloading thread")) ; // PRQA S 3081
			}

			if (dynamic_cast<dbInventoryObjectOperatorIF::TryAgainLaterToMigrateException*>(e.get()) != 0) // PRQA S 3081
			{
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
								 mc_param("message", "Cannot download now, try again later") <<
								 mc_metric(Metric_FAIL) << 
								 MCCode ("ARC") << MCFunction ("Archive")) ;
				ACS_THROW(dbInventoryObjectOperatorIF::TryAgainLaterToMigrateException(*e,"Cannot download file now, try later"));
			}

			// APF-270
			ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
							 mc_param("message", "Error in downloading thread") <<
							 mc_metric(Metric_FAIL) << 
							 MCCode ("ARC") << MCFunction ("Archive")) ;
			ACS_THROW(DownloadException(*e,"Error in downloading thread")) ; // PRQA S 3081

		}

		ACS_COND_THROW(dynamic_cast<DownloadAsynchException*>(e.get()) != 0,DownloadAsynchException(*e, "Async request issued")) ; // PRQA S 3081

		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
						 mc_param("message", "Unexpected exception got during download") <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("ARC") << MCFunction ("Archive")) ;
        ACS_THROW(dbFileAction::Exception(*e,"Unexpected exception")) ; // PRQA S 3081		
	}
	
	if(isComplete())
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "done") ;
		// to remove the progress bar
	}
	return false;
}


void dbDownloadInventoryFile::registerClient() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	bool confRegisterClientEnabled(false);
	string confUnkClientName("");

	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	if ( conf.hasKey("DownloadInventoryObject.RegisterClient") ) {
		conf.getValue("DownloadInventoryObject.RegisterClient.enabled.value", confRegisterClientEnabled);
		conf.getValue("DownloadInventoryObject.RegisterClient.UnknownClientName", confUnkClientName);
	}

	if (confRegisterClientEnabled) {

		//Register Client
		const string& clientName = _clientName.empty() ? confUnkClientName : _clientName ; // PRQA S 3384

        // There is a transaction inside. Do not try to release this connection at any stage
		dbConnectionWrapper pw(getPool()) ;
		dbConnection &conn = pw; // PRQA S 3050
		const string &ck = conn.getKey() ;
		string dialect = SQLDialectFactory::dialectName(ck) ;

		//Insert the new client name if not already existing
		try {

			string stFromTable( "" );
			if ("Oracle11" == dialect) { stFromTable = "from dual"; }

			ostringstream ossSql;
			ossSql <<"\
					INSERT INTO t_inv_client(name) \
					select "<<db::toSQL(conn, clientName )<<" "<<stFromTable<<" \
					where not exists ( \
						select id_inv_client from t_inv_client where name = "<<db::toSQL(conn, clientName )<<"\
					)";

			SQLString sqlString(ck, ossSql.str() );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<']');

			dbQuery insert(conn, sqlString, "INS t_inv_client");

			ACS_COND_THROW( ! insert.execForStatus(), // PRQA S 3081
					dbDownloadInventoryClientRegisterException("DB Updating returned error status. Sql was ["+sqlString.getSQL()+']'));

		} catch(dbQuery::QueryExecuteException& ex) {

			//In multiprocess scenario insert.execForStatus can raise exception or fail
			// because two processes can try to do the same insert at the same time. Only one does well,
			// the other gets the exception. If exception is due to other problems (db problems), the exception will be raised
			// by the next SELECT

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": Inserting new entry in t_inv_client, Got exception ["<< ex.what()
					<<"]. Could be nominal in multiprocess scenario.");
		}

		//Get the id just inserted
		dbPersistent::IdType clientId(0);
		{
			ostringstream ossSql;
			ossSql << "select id_inv_client from t_inv_client where name = "<<db::toSQL(conn, clientName );
			SQLString sqlString(ck, ossSql.str() );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<']');

			dbQuery select(conn, sqlString, "SEL t_inv_client");
			dbSet set;
			select.execForSet(set);

			ACS_COND_THROW( set.empty(), // PRQA S 3081
					dbDownloadInventoryClientRegisterException("No client id returned from Sql query ["+sqlString.getSQL()+']'));

			dbRow row(ck); 
            set.getRow(1, row);
			row.column(0, clientId); //is not null by schema
		}

		//Insert the entry in t_inv_downloads if not already existing
		try {
			string stFromTable( "" );
			if ("Oracle11" == dialect) { stFromTable = "from dual"; }

			ostringstream ossSql;
			ossSql << "\
					INSERT INTO t_inv_downloads(inv_id, client_id) \
					select "<<db::toSQL(conn, _inventory.getId())<<", "
							<<db::toSQL(conn, clientId) <<" "<<
					stFromTable << "\
					where not exists ( \
						select inv_id, client_id from t_inv_downloads where \
							inv_id = "<<db::toSQL(conn, _inventory.getId())<<" and \
							client_id = "<<db::toSQL(conn, clientId)<<" \
					)";
			SQLString sqlString(ck, ossSql.str() );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<']');

			dbQuery insert(conn, sqlString, "INS t_inv_downloads");
			ACS_COND_THROW( ! insert.execForStatus(), // PRQA S 3081
					dbDownloadInventoryClientRegisterException("DB Updating returned error status. Sql was ["+sqlString.getSQL()+']'));

		}  catch(dbQuery::QueryExecuteException& ex) {

			//In multiprocess scenario insert.execForStatus can raise exception or fail
			// because two processes can try to do the same insert at the same time. Only one does well,
			// the other gets the exception. If exception is due to other problems (db problems), the exception will be raised
			// by the next UPDATE

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": Inserting new entry in t_inv_downloads, Got exception ["<< ex.what()
					<<"]. Could be nominal in multiprocess scenario.");
		}

		//Update the download time to now
		{
			DateTime dtNow;
			ostringstream ossSql;
			ossSql <<"\
					UPDATE t_inv_downloads \
					SET downloadtime = "	<<db::toSQL(conn, dtNow, dbConv::YearToFractionExtended)<<" \
					WHERE 	inv_id = "		<<db::toSQL(conn, _inventory.getId())<<" and \
							client_id = "	<<db::toSQL(conn, clientId);
			SQLString sqlString(ck, ossSql.str() );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": executing query ["<<sqlString.getSQL()<<']');

			dbQuery update(conn, sqlString, "UPD t_inv_downloads");
			ACS_COND_THROW( ! update.execForStatus(), // PRQA S 3081
					dbDownloadInventoryClientRegisterException("DB Updating returned error status. Sql was ["+sqlString.getSQL()+']'));
		}
	}
}

void dbDownloadInventoryFile::run()
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ACS_LOG_PRIVINFO("Start downloading file \"" <<_inventory.getFileName() << "\" in path " << _path) ;

    const string &dbKey = getPool().getKey() ;
	

    // Every connection is gathered from the pool and released ASAP

    { // Connectionwrapper scope
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050

	    ACS_COND_THROW( // PRQA S 3081
            ( (not _inventory.isFromDatabase()) and ( not _inventory.load(theConnection) ) ),
            PreconditionException("Error downloading file: inventory \"" + _inventory.getFileName() + "\" can't be found in database.")
        ) ;
    } // Connectionwrapper scope
	
	ACS_COND_THROW(  // PRQA S 3081
        ( ( not _downloadInvalidFile ) and ( not _inventory.getValidityFlag() ) ),
        PreconditionException("Cannot download inventory: validity flag is false.") 
    ) ;

	ACS_COND_THROW(  // PRQA S 3081
        File::exists(_path), 
	    PreconditionException("Error downloading a file to \"" +  File::getDirPath(_path) + "/" +_inventory.getFileName() + "\" because it exists.")
    ) ;


	_preconditionPassed=true;
	dbInventoryObjectOperatorIF oper;
	
	{ //the file could be on Storage manager(s)

		dbPersistent * recordPtr = &_inventory ;
		
		bool canGetFromSynchSM = false;
		{
        	dbConnectionWrapper pw(getPool()) ;
        	dbConnection &theConnection = pw ; // PRQA S 3050
			canGetFromSynchSM = oper.canGetFromSynchSM(*recordPtr, theConnection);
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file " << _inventory.getFileName() << " can" << (canGetFromSynchSM ? " " : " NOT " ) << "be downloaded from synchronous SM") ; // PRQA S 3380

		// Exception counters to detect UnknownFileException
        // and generic download exception thrown by the SMs
        int genericExceptionCounter (0);
		vector<dbPersistent::IdType> unknownInventorysm;
		
        // Container for the exception that has to be notified and thrown
        // if the download is not successful
        const string dexMessage = "Error downloading file \"" + _inventory.getFileName() + "\" version \"" + _inventory.getFileVersion() ;
        ACS_SMARTPTR<DownloadException> dex(new DownloadException(dexMessage)); 

		bool downloaded=false ;
		while( not downloaded ) { // PRQA S 4236

			if( not canGetFromSynchSM ) {
			
				if (smActionsDisabled())
				{
					ACS_THROW(DownloadAsynchException("Operations on Async SM disabled.")); // PRQA S 3081
				}
				else
				{
					pair<bool, dbPersistent::IdType> downloadRequest;
					try {
        				dbConnectionWrapper pw(getPool()) ;
        				dbConnection &theConnection = pw ; // PRQA S 3050
						downloadRequest = oper.restoreOnSynchSM(*recordPtr, theConnection);
					}
					catch(exception &e) {

						if( ( dynamic_cast<dbInventoryObjectOperatorIF::NotMigrableException*> (&e) || dynamic_cast<dbInventoryObjectOperatorIF::MigrationFailedException*> (&e) ) && not unknownInventorysm.empty() ) { // PRQA S 3081
						// if here, an attempt to download file from sync SMs has already been performed and ALL OF THEM do not know the file: if another exception occurred, it would have been already thrown in the previous iteration)
                        
                        // ---------------------------------------------------------------------------------------------------------------
                        // LP: it should invalidate the file also if an attempt to download file from sync SMs has never been performed, 
                        // unknownInventorysm.empty(), but it will change behaviour w.r.t the old version..."
                        // ---------------------------------------------------------------------------------------------------------------
                		
                        	// Is the system configured to allow file invalidation upon download requests ?
                			bool canInvalidate (false);
                			if ( ( ConfigurationSingleton::instance() -> get() ).hasKey ("DownloadInventoryObject.CanInvalidateFile"))
                			{
                    			(ConfigurationSingleton::instance() -> get() ).getValue ("DownloadInventoryObject.CanInvalidateFile", canInvalidate);
                			}

                			if (canInvalidate)
                			{           
                    			// This file cannot be downloaded anymore... it is time to invalidate it
                    			ACS_LOG_WARNING("File not available on any storage manager. Going to invalidate it [" << _inventory.getFileName() << ", " 
                        			<< _inventory.getFileVersion() << ']') ;
                    			_inventory.setValidityFlag( false );

                    			try
                    			{
                        			dbConnectionWrapper pw(getPool()) ;
                        			dbConnection &theConnection = pw ; // PRQA S 3050

                        			_inventory.update(theConnection);
                    			}
                    			catch (exception& exupdate)
                    			{
                        			// Notify the exception and then go on
                        			ACS_LOG_WARNING("Unable to invalidate the inventory item") ;
                        			ACS_LOG_NOTIFY_EX(exupdate) ;
                    			}
                			}

            			} // if END

						throw;
					}

					if( downloadRequest.first ) {

						setCompleted(true) ;
						ostringstream os ;
						os << "Request to download file " << _inventory.getFileName() << " with version " <<  _inventory.getFileVersion() << " from asynchronous SM occurs:  LTA Download request id= " << downloadRequest.second;

						ACS_THROW( DownloadAsynchException(os.str()) ) ; // PRQA S 3081

					}
					else {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << _inventory.getFileName() << "\" with version " <<  _inventory.getFileVersion() << " occurs on synchronous SM");
					}
				}

			} // if( not canGetFromSynchSM )

			// be sure that the t_storagemanager table has the field allowdefaultcost
        	bool hasADCfield (false);
        	{
            	dbConnectionWrapper pw(getPool()) ;
            	dbConnection &theConnection = pw ; // PRQA S 3050
            	hasADCfield = dbSchemaMetaDataProvider::hasField ( "t_storagemanager", "allowdefaultcost", theConnection); // PRQA S 4412
        	}

			// It returns synchronous SMs where file is, ordered by retrieve cost ASC
			SQLString sqlString = getSyncSMsForDownloadQuery(hasADCfield);

			dbSet smset;
			{
            	dbConnectionWrapper pw(getPool()) ;
            	dbConnection &theConnection = pw ; // PRQA S 3050

		    	dbQuery query(theConnection,sqlString, "FADWNSEL prior, cost") ;
		    	query.execForSet(smset) ;
        	}
			
			// reset
			genericExceptionCounter = 0; 
			unknownInventorysm.clear();
			
			size_t smsetsize = smset.size() ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file \"" + _inventory.getFileName() + "\" can be downloaded from " << smsetsize << " synchronous SMs.") ;
			
			string abspath = File::getAbsolutePath(_path);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "_path: \"" << _path << "\"" << " abspath: " << abspath << '\"') ;

			
			dbInventoryObjectCacheManager cacheManager;
			bool uploadOnCacheLayer = false;
			if( smsetsize > 0 ) {
// try to download from cache layer
				try {
					bool cacheManagerConfigured = cacheManager.checkIsConfigured(_inventory);
					if( cacheManagerConfigured ) {
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << cacheManager.getCachePath() << "\" bidirectional cache layer configured: try to download");
						if( cacheManager.downloadFile(abspath, &uploadOnCacheLayer) ) {
							downloaded=true ;
							// APF-270
							ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
											 mc_param("filesize", _inventory.getDataSize() + _inventory.getHeaderSize() ) << mc_metric(Metric_OUT) << 
											 MCCode ("ARC") << MCFunction ("Archive")) ;
							ACS_LOG_PRIVINFO("Filename \"" <<  _inventory.getFileName() << "\" succesfully downloaded from \"" << cacheManager.getCachePath() << "\" cache");
						}
						else if( uploadOnCacheLayer ) {
							ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The file \"" << _inventory.getFileName() 
												<< "\" has NOT been downloaded from cache layer. Upload on cache will be performed after downloading from SM into path \"" << abspath << '\"');
						}
						else {
							ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The file \"" << _inventory.getFileName() << "\" has NOT been downloaded from cache layer. No upload will be performed next.");
						}
					}
					else { // just for debug
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Bidirectional cache layer not configured.");
					}
				}
				catch( exception &e )
				{
					
					// APF-270
					ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
					mc_param("message", "Cache layer Manager reports an error during the download") <<
					mc_metric(Metric_FAIL) << 
					MCCode ("ARC") << MCFunction ("Archive")) ;

					ACS_LOG_NOTIFY_EX(e);
					ACS_LOG_WARNING("Try to download from SMs");
				}

			}
			
			for (size_t ri=0; (ri < smsetsize) && (not downloaded); ++ri) {

				dbPersistent::IdType storageManId = 0 ;
				try {

					dbRow row(dbKey) ;
					smset.getNextRow(row);

 					unsigned short ci = 0;
					row.column(ci++,storageManId);
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Trying smid: " << storageManId) ;

					dbPersistent::IdType id = 0 ;
					row.column(ci++,id);

					string smtype;
					row.column(ci++,smtype);

					int defstrategy = 0 ;
					row.column(ci++,defstrategy);
					bool hasParameters = ( 0 != defstrategy)  ; // note int to bool implicit

					SMInterface* storage= SMInterfaceFactoryMethod::getInterface(smtype, storageManId, &_inventory, getPool()); // PRQA S 3000
					SMInterfaceScopeGuard guard(storage);
					if(!storage)
					{	
						ACS_THROW(DownloadException() << "No SMInterface got for smid: " << storageManId << " type : " << smtype);
					}
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got SMInterface for smid: " << storage->SMId()
							<< " type: " << storage->SMType()) ;

                    storage->setClientName(getClientName()) ; // set the client name for the current action

					if( isSimulated() ) {
						simulateDownload(_inventory, abspath);
					}
					else {
						storage->download(id,abspath,oper);

						unpack(storageManId ,hasParameters,abspath); 
						// APF-270
						ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
										 mc_param("filesize", _inventory.getDataSize() + _inventory.getHeaderSize() ) << mc_metric(Metric_OUT) << 
										 MCCode ("ARC") << MCFunction ("Archive")) ;

					}

					downloaded=true ; // signal loop exit because downloaded

					break ;

				}
   				catch(dbDownloadFile::SMUnknownFileException &ex)
				{

					ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
									 mc_param("equipmentid", storageManId) <<
									 mc_param("message", "Storage Manager reports the file as Unknown") <<
									 mc_metric(Metric_FAIL) << 
									 MCCode ("ARC") << MCFunction ("Archive")) ;

   	    			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SMUnknownFileException caught from SM id " << storageManId) ;
                	ACS_LOG_NOTIFY_EX(ex) ;
					unknownInventorysm.push_back(storageManId);
					ACS_LOG_WARNING("Trying with next SM, if any") ;

				}
            	catch (exception &ex)
				{
					ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
									 mc_param("equipmentid", storageManId) <<
									 mc_param("message", "Storage Manager reports a generic error during the download") <<
									 mc_metric(Metric_FAIL) << 
									 MCCode ("ARC") << MCFunction ("Archive")) ;

   	    			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generic exception caught from SM id " << storageManId) ;

                	dex = ACS_SMARTPTR<DownloadException>(new DownloadException (ex, dexMessage)) ; // PRQA S 3050, 3081
                	genericExceptionCounter++;
#ifdef COSMO
					ACS_CLASS_BGN_DEBUG(10) 
#endif

                    	ACS_LOG_NOTIFY_EX(ex) ;
				    	ACS_LOG_WARNING("Trying with next SM, if any") ;

#ifdef COSMO
                	ACS_CLASS_END_DEBUG
#endif
				}

			} // for (size_t i=0; i < smsetsize; ++i)

   	    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exception counters: SMUnknownFileException [" << unknownInventorysm.size() << "], std::exception [" 
                	<< genericExceptionCounter << ']') ;

			if( not downloaded ) {
				
				if( 0 == genericExceptionCounter ) { // SMUnknownFileException > 0
					
					try {// remove t_inventorysm rows
        				dbConnectionWrapper pw(getPool()) ;
        				dbConnection &theConnection = pw ; // PRQA S 3050
						deleteUnknownInventorysm(unknownInventorysm, theConnection);
					}
					catch(exception &e)
					{
						ACS_THROW( DownloadException(e, "Error while trying to remove UNKNOWN "+_invSmTableName+" rows") );
					}
					
        			dbConnectionWrapper pw(getPool()) ;
        			dbConnection &theConnection = pw ; // PRQA S 3050
					canGetFromSynchSM = oper.canGetFromSynchSM(*recordPtr, theConnection);
					ACS_LOG_WARNING("Going to try to download the file " << _inventory.getFileName() << " from " << ( canGetFromSynchSM ? "synchronous SM again" : "asynchronous SM" ) ); // PRQA S 3380
				}
				else { // genericExceptionCounter > 0
					if (dex.get()) { 
						ACS_THROW(*dex); 
					}
					else {
					// it should not occur!!!!
						ACS_THROW( DownloadException("***UNEXPECTED*** Some generic exceptions caught. " + dexMessage) ); // PRQA S 3081
					}
				}
			}  		
			else
  			{
				
				if( uploadOnCacheLayer ) {
					try {
						if( cacheManager.uploadFile(abspath) ) {
							ACS_LOG_PRIVINFO("Filename \"" <<  _inventory.getFileName() << "\" succesfully uploaded on \"" << cacheManager.getCachePath() 
											<< "\" cache (it was missing)");
						}
						else {
							ACS_LOG_WARNING("Cannot upload filename \"" <<  _inventory.getFileName() << "\" on \"" << cacheManager.getCachePath() << "\" cache");
						}
					}
					catch( exception &e )
					{
						ACS_LOG_NOTIFY_EX(e) ;
						ACS_LOG_WARNING("WARNING exception got while uploading on cache layer.") ;
						// APF-270
						ACS_LOG_MC_INFO( mc_param("filename", _inventory.getFileName()) << mc_param("fileversion", _inventory.getFileVersion()) << // PRQA S 3081
										 mc_param("message", "Exception got from cache layer manager") <<
										 mc_metric(Metric_FAIL) << 
										 MCCode ("ARC") << MCFunction ("Archive")) ;
						
					}
					
				}
				
  				try {
  					this->registerClient();
  				} 
				catch(exception& ex) {
  					ACS_LOG_WARNING("Unable to register the client ["<<_clientName<<"] for downloading ["
  							<<_inventory.getFileName()<<", "<< _inventory.getFileVersion() << "]. Problem occurred: "<<ex.what() );
  				}
  			}
		} // while( not downloaded )
	}
	
	ACS_LOG_PRIVINFO("Finished to download file \"" <<_inventory.getFileName() << "\" in path " << _path) ;
	
	setCompleted(true) ;
}


void dbDownloadInventoryFile::unpack(dbPersistent::IdType storageManId , bool defaultpacking, string& newpath)
{
	string strategyconf = File::tempname("/tmp/storage", "rs") ; // this do not leave the file on disk !!! // PRQA S 3050
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
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ;// PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}
	else
	{
        string table = "T_Storagepriority" ;
        querySMString.addTableList( table ) ;
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "Id" , dbParam::EQUAL_TO, db::toSQL(dbKey, storageManId)) ) ;// PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter( dbParam(dbKey, table, "FileType" , dbParam::EQUAL_TO, db::toSQL(dbKey, _inventory.getFileType())) ) ;// PRQA S 3050, 3081
            querySMString.addANDWhereParameter(p) ;
        }
	}

    {
        dbConnectionWrapper pw(getPool()) ;
        dbConnection &theConnection = pw ; // PRQA S 3050

    	dbQuery smQuery(theConnection, querySMString, "FADWLSEL strategy") ;	
        smQuery.downloadClob();
    }

    // "check if correctly" downloaded not required.

	// change permissions allowing other users to override this file later.
	File::chmod(strategyconf,0664) ; // PRQA S 3050, 4400

	rsResourceSet rset;
	XMLIstream ifile(strategyconf);// PRQA S 3050
	ifile >> rset ;

	rset.getValue("SMConfiguration.PackingStrategy.name",packingstrategy);

	SMPackingStrategy* strategy = SMPackingStrategyFactoryChain::instance()->getStrategy(packingstrategy, _inventory, getPool());
	SMPackingStrategyScopeGuard guard(strategy) ;
	if (!strategy) {
		ACS_THROW(SMPackingStrategyFactory::NotFoundException("No PackingStrategy got with name " + packingstrategy + " for dbInventory" )) ; // PRQA S 3081
	}
	
	strategy->unpack(newpath, ifile , newpath);

	return ;
}


void dbDownloadInventoryFile::simulateDownload(dbInventoryObject &obj, const string &path)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
		
// It replicates the behaviour of ISM Client lib
	ACS_COND_THROW( File::exists(path), DownloadException("\"" + path + "\" file already exists.") ) ; // PRQA S 3081 2
			
	// get a random number between 0 and 1
    srand(time(0)) ; // random seed // PRQA S 3000, 3010
	bool randomBehaviour = (0 != rand()%2) ; // remainder: 0 or 1 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Random behaviour: " << boolalpha << randomBehaviour) ;
	if( randomBehaviour ) {
		off_t fileSize = obj.getHeaderSize();
		fileSize += obj.getDataSize(); // PRQA S 3084
				
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


SQLString dbDownloadInventoryFile::getSyncSMsForDownloadQuery(bool hasADCfield) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

// SELECT StorageManId, id, type, paramNull as defstrategy
// FROM (
// SELECT ism.StorageManId, ism.id, sm.type, CASE WHEN p.parameters IS NULL THEN 1 ELSE 0 END as paramNull, p.retrievecost,  
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

    const string &dbKey = getPool().getKey() ;
	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	sqlString.addSelectList("StorageManId");
	sqlString.addSelectList("id");
	sqlString.addSelectList("type");
	sqlString.addSelectList("paramNull", "defstrategy");

	const string runTimeTable("inner_table");
	{
		SQLString nestedSelect( dbKey, SQLString::SELECT ) ;

		nestedSelect.addSelectList("ism.StorageManId");
		nestedSelect.addSelectList("ism.id");
		nestedSelect.addSelectList("sm.type");
		nestedSelect.addSelectList("CASE WHEN p.parameters IS NULL THEN 1 ELSE 0 END", "paramNull");
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
    	if (hasADCfield) {            
        	nestedSelect.addSelectList("allowdefaultcost", "adc"); 
    	}

		nestedSelect.addTableList(_invSmTableName, "ism");

		// Add Join
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "sm","id" , dbParam::EQUAL_TO, "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "sm","syncdownload" , dbParam::EQUAL_TO, db::toSQL(dbKey, true) )) ;
			nestedSelect.addJoinList(SQLDialect::INNER,"t_storagemanager sm",pp) ;
		}
		{
   			dbQueryParameters pp( dbKey);
 			pp.addParameter(dbParam( dbKey, "p","id" , dbParam::EQUAL_TO , "ism.StorageManId")) ;
 			pp.addParameter(dbParam( dbKey, "p","filetype" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getFileType()) )) ;
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
		if (hasADCfield ) {
			pp.addParameter(dbParam(dbKey, runTimeTable ,"adc" , dbParam::EQUAL_TO , db::toSQL(dbKey, true) )) ;
		}
		sqlString.addORWhereParameter(pp) ;
	}

	sqlString.addOrderByList("CASE WHEN retrievecost IS NULL THEN 1 ELSE retrievecost END", SQLString::ASC) ;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following SQL query: "  << sqlString.getSQL() );
	
	return sqlString;

}


void dbDownloadInventoryFile::deleteUnknownInventorysm( const vector<dbPersistent::IdType> & unknownInventorysm, dbConnection &theConnection) const // PRQA S 4020
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	if( unknownInventorysm.empty() ) {
		return;
	}
	
    const string &dbKey=theConnection.getKey() ; 
	SQLString sqlDelete(dbKey,  SQLString::DELETE) ;
	sqlDelete.addTableList(_invSmTableName) ;
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter( dbParam(dbKey, _invSmTableName, "inv_id" , dbParam::EQUAL_TO , db::toSQL(dbKey, _inventory.getId())) ) ;
		pp.addParameter( dbParam(dbKey, _invSmTableName, "StorageManId", dbParam::IN, unknownInventorysm) ) ;
		sqlDelete.addANDWhereParameter(pp);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute the following command: " << sqlDelete.getSQL()) ;
	
	dbQuery query(theConnection,sqlDelete, "Delete unknown "+_invSmTableName) ;
	
	if( not query.execForStatus() ) {
		string ErrorString ;
		ErrorString += "Query Execution error SQL command is \"" ;
		ErrorString += sqlDelete.getSQL() ;
		ErrorString += "\". Error is \"" + query.errorString()  + "\".";

		ACS_THROW (dbQuery::QueryExecuteException(ErrorString)) ; // PRQA S 3081
	}

}

_ACS_END_NAMESPACE
