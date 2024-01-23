// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. ISM SM client library $

	$Id$

	$Author$

	$Log$
	Revision 6.21  2017/11/09 09:33:00  lucio.pulvirenti
	S2PDGS-1843: work in progress.
	
	Revision 6.20  2017/09/29 09:51:59  lucio.pulvirenti
	S2PDGS-1843: group to insert into ISM file class is first searched in configuration (getGroupFromConfSpace new method). If not found
	ist is got from filetype as usual.
	
	Revision 6.19  2017/09/22 15:00:46  lucio.pulvirenti
	S2DPGS-1843: work in progress.
	
	Revision 6.18  2016/12/07 10:02:31  marfav
	Releasing dbConnection ASAP to limit multiple connection usage
	
	Revision 6.17  2016/05/30 08:40:01  lucio.pulvirenti
	dbQuery::execOnReow method replaced with dbQuery::ExecForSet.
	
	Revision 6.16  2016/04/29 16:04:09  marpas
	ids are of dbPersistent::IdType - to be overall consistent
	coding best practices applied
	
	Revision 6.15  2016/04/28 12:31:20  marfav
	Thread inheritance is now protected
	
	Revision 6.14  2015/12/11 13:58:35  lucio.pulvirenti
	APF-270: added both filesize and jobtime to IN and OUT messages according to new specifics: they are mandatory tags.
	
	Revision 6.13  2015/11/26 15:37:29  lucio.pulvirenti
	APF-270: MC messages issued for upload (IN), download (OUT) and erase (DELETE). Error issue with metric type FAIL.
	
	Revision 6.12  2015/11/12 15:56:16  lucio.pulvirenti
	APF-270: message added to highlight uploading on SM.
	
	Revision 6.11  2015/10/23 12:40:45  marfav
	APF-336 adding hostname and timestamp in ISM path
	
	Revision 6.10  2015/09/23 11:09:28  lucio.pulvirenti
	APF-312: IsmSoapClient setConnParameters method now get SM id.
	
	Revision 6.9  2014/12/15 10:45:51  marpas
	using libGenFileAction 6.12 I/F
	
	Revision 6.8  2014/06/27 14:40:52  marpas
	coding best practices applied
	
	Revision 6.7  2014/06/27 08:07:31  marpas
	stats fixed
	
	Revision 6.6  2014/02/12 13:33:21  francesco.avanzi
	throwException moved to ACS_THROW and throwConditionedException moved to ACS_COND_THROW
	
	Revision 6.5  2014/02/12 13:24:31  francesco.avanzi
	throwException moved to ACS_THROW and throwConditionedException moved to ACS_COND_THROW
	
	Revision 6.4  2014/01/29 12:29:22  marpas
	new db::Transaction used
	signature computation time added to statistics
	
	Revision 6.3  2014/01/27 17:42:04  marpas
	noChangingHardlinkedFilesMode used to avoid mode change (chmod)
	
	Revision 6.2  2013/10/24 12:36:59  marpas
	statistics improved
	
	Revision 6.1  2013/09/20 14:13:45  marfav
	aligned to new insertInvSM signature allowing default null package id
	
	Revision 6.0  2013/09/20 08:03:34  marpas
	new t_inventorysm schema supported
	
	Revision 5.2  2013/09/17 15:24:31  marpas
	uses SMInterface::deleteInvSM method used
	
	Revision 5.1  2013/09/16 17:05:43  marpas
	use of new SMInterface::insertInvSM method because new t_inventorysm table structure
	some compilation warnings removed
	
	Revision 5.0  2013/06/20 13:37:44  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa rules
	
	Revision 1.25  2013/04/09 08:23:23  marpas
	new Statistics
	
	Revision 1.24  2013/04/03 13:48:58  marpas
	run does not longer gets a void*
	
	Revision 1.23  2013/04/03 12:51:48  marpas
	Thread::run gets a void*
	
	Revision 1.22  2013/03/29 15:12:12  marpas
	statistics
	
	Revision 1.21  2013/03/12 17:48:00  marpas
	dbCOnnectionPool is mandatory in SM interfaces and factory methods
	
	Revision 1.20  2013/03/11 12:50:46  marpas
	FileGuard is a template specialization now
	
	Revision 1.19  2013/01/24 12:19:02  marpas
	query stat names improved
	
	Revision 1.18  2012/11/28 12:19:05  marpas
	useless statements removed
	
	Revision 1.17  2012/05/25 15:48:30  marfav
	Addedd SMUnknownFileException management as per S1PDGS-3291
	
	Revision 1.16  2012/02/13 17:09:49  marpas
	refactoring in progress
	
	Revision 1.15  2011/04/19 16:27:28  marpas
	when uploading trees, path are always relative
	upload events were missing (only IDLE was issued) - fixed
	
	Revision 1.14  2011/04/06 16:11:27  marpas
	chmod for directories implemented
	
	Revision 1.13  2011/04/05 16:51:46  marpas
	Tree management work in progress
	
	Revision 1.12  2010/08/11 08:15:04  marpas
	work in progress
	
	Revision 1.11  2010/08/11 08:05:39  marpas
	bug and test compilation fixed
	
	Revision 1.10  2010/08/10 09:14:57  marpas
	fully db independence implemented using SQL String
	
	Revision 1.9  2009/06/26 10:43:59  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.8  2009/06/19 17:54:17  marpas
	minor changes
	
	Revision 1.7  2009/06/17 09:18:52  marpas
	custom attributes for ISM: got from applicationName if any
	
	Revision 1.6  2009/06/16 16:10:25  marpas
	new libIsmSoapClient interface added
	
	Revision 1.5  2009/03/10 18:24:18  lucio.pulvirenti
	custom attribute changed
	
	Revision 1.4  2008/11/04 16:38:59  ivafam
	Erase Interface changed
	
	Revision 1.3  2008/10/07 15:54:58  crivig
	porting to postgres8
	
	Revision 1.2  2008/06/16 15:39:33  marpas
	tested
	
	Revision 1.1.1.1  2008/06/16 15:14:58  marpas
	Importing libIsmSMClient
	

*/


#include <ISMStorageManager.h>
#include <File.h>
#include <FileGuard.h>
#include <Application.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <XMLIstream.h>
#include <SignatureAlgoFactory.h>
#include <SignatureAlgoScopeGuard.h>
#include <SignatureAlgo.h>
#include <dbPersistentOperatorIF.h>
#include <dbSmartPersistent.h>
#include <IsmSoapClient.h>
#include <Transaction.h>
#include <dbConnectionPool.h>

#include <MCFilterables.h>
#include <Filterables.h>
#include <Statistics.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;
using namespace acs::mc ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(ISMStorageManager::Simulate);


ISMStorageManager::Simulate::Simulate()
{
}

ISMStorageManager::Simulate::~Simulate() throw() {}

bool ISMStorageManager::Simulate::simulate() const 
{
    return ACS_CLASS_GET_DEBUG() > 0 ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ISMStorageManager) 
acs::math::StatNode &ISMStorageManager::statN() { return (*acs::math::Statistics::instance())()["ISMStorageManager"] ; }

ISMStorageManager::ISMStorageManager(const string &smtype, dbPersistent::IdType smid,const dbPersistent* obj, dbConnectionPool &pool) : 
    SMInterface(smtype,smid,obj, pool),
    _path(),
    _check(),
	_oper(0),
    _references()
{
    statN()["instances"]+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Type "<< smtype << "; ID " << smid << "; SMId " << SMId()) ;

}

ISMStorageManager:: ~ISMStorageManager() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631 2
	delete _oper ;
}


void ISMStorageManager::getParameters(dbConnection &conn, const string &parampath, rsResourceSet &params, string &invrelation) const  // PRQA S 4214
{
    Timer timer ;
    timer.start() ;
    statN()["getParameters"] += 1;
    const string &dbKey = conn.getKey() ;
    SQLString qString( dbKey, SQLString::SELECT ) ;
    string tableName = "t_storagemanager" ;
    string paramName = "invrelationtable" ;
    qString.addTableList(tableName);
    qString.addSelectList(paramName); 
    qString.setLimit(1) ;

    {
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, tableName, "Id", dbParam::EQUAL_TO, db::toSQL(dbKey, SMId()) )) ; 
        qString.addANDWhereParameter(pp) ;
    }

    ostringstream queryName ; queryName << "StorageManagerGetInvRel_" << SMId() ;
	dbQuery query(conn, qString, queryName.str()) ;	

	dbRow myRow(dbKey) ;
	try { 
		dbSet set;
		query.execForSet(set) ;
		ACS_COND_THROW(not set.getNextRow(myRow), ISMStorageManagerException( "No record found" )); // PRQA S 3081
	}
	catch(ISMStorageManagerException &x) {
		ostringstream os ;
		os << "cannot get clob parameters file for smid=" << SMId() << ": No " << tableName << " record found";
		ACS_THROW(exIOException(os.str())) ; // PRQA S 3081 
	}
	catch(exception &x) {
		ostringstream os ;
		os << "cannot get clob parameters file for smid:" << SMId() ;
		ACS_THROW(exIOException(x,os.str())) ; // PRQA S 3081 
	}

	myRow.column(0,invrelation);


    {
        SQLString queryString( dbKey, SQLString::LOBJ_DOWNLOAD ) ;
        queryString.setColumnAndFileName( "Parameters", parampath );
        queryString.addTableList( tableName );
        
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter(dbParam(dbKey, tableName, "Id", dbParam::EQUAL_TO, db::toSQL(dbKey,SMId()) )) ; 
            queryString.addANDWhereParameter(p) ;
        }
	    
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading file: " << parampath) ;


        ostringstream qName ; qName << "StorageManagerDwnlParam_" << SMId() ;

		dbQuery qry(conn, queryString, qName.str()) ;	

        try
        {
            qry.downloadClob();
        }
        catch(std::exception &x)
        {
            ostringstream os ;
            os << "cannot get clob parameters file for smid:" << SMId() ;
            ACS_THROW(exIOException(x, os.str())); // PRQA S 3081
        }
    }
    
    
	// change permissions allowing other users to override this file later.
	File::chmod(parampath,0664) ; // PRQA S 4400

	XMLIstream ifile( parampath );
	ifile >> params ;
    timer.stop() ;
    statN()["getParameters"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
}



void ISMStorageManager::download(dbPersistent::IdType sminvid, const string &localpath, const dbPersistentOperatorIF& oper)
{
    Timer timer ;
    timer.start() ;
    statN()["download"]+=1 ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SMId() " << SMId()) ;

	string completepath = "/tmp/ISMConf";
	ostringstream os;
	os << SMId();
	completepath += os.str();
    string paramfile = File::tempname(completepath, "rs") ; // PRQA S 3050
	FileGuard paramfile_guard(paramfile) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "param file " << paramfile) ;

	
	
	string smRemotepath; 
	try {	
        const string &dbKey = getPool().getKey() ;
		rsResourceSet rset ;
		string table ;
		try {
		    dbConnectionWrapper pw(getPool()) ;
		    dbConnection &theConnection = pw ; // PRQA S 3050
			getParameters(theConnection, paramfile, rset, table) ;
		}
		catch(exception &e) {
			ACS_THROW(ISMStorageManagerException(e,"unable to get parameters")) ; // PRQA S 3081
		}
		
		int tpretrtimeout = 0 ;
		string user ;
		
		rset.getValue("SMConfiguration.user", user);
		rset.getValue("SMConfiguration.TapeRetrievalTimeOut", tpretrtimeout);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "user " << user 
				   << " tpretrtimeout " << tpretrtimeout );

		string signatureAlgo;
		bool checkSignature = false ;
		try {
			rset.getValue("SMConfiguration.Signature.Algo", signatureAlgo);
			rset.getValue("SMConfiguration.Signature.Check", checkSignature);
		}
		catch(rsResourceSet::NotFoundException &ex)
		{
			checkSignature=false;
		}
		bool noChangingHardlinkedFilesMode = false ;
		try {
			rset.getValue("SMConfiguration.noChangingHardlinkedFilesMode", noChangingHardlinkedFilesMode);
		}
		catch(rsResourceSet::NotFoundException &ex)
		{
			noChangingHardlinkedFilesMode=false;
		}
        
		
    	SQLString sqlstring( dbKey, SQLString::SELECT ) ;
        sqlstring.addSelectList("path");
		if(checkSignature) {
        	sqlstring.addSelectList("signature");
        }
        sqlstring.addTableList(table);
		{
			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, table,"Id" ,  dbParam::EQUAL_TO, db::toSQL(dbKey, sminvid))) ; 
			sqlstring.addANDWhereParameter(pp) ;
        }
		

		string storedSignature="";
		string sql3 ;
		dbRow myRow3(dbKey) ; 
		{
		    dbConnectionWrapper pw(getPool()) ;
		    dbConnection &theConnection = pw ; // PRQA S 3050
			dbQuery query3(theConnection, sqlstring, "SelectPathSignature") ;	

			dbSet set;
			query3.execForSet(set) ;
			if( not set.getNextRow(myRow3) ) {
				ostringstream exMsg;
				exMsg << "No " << table << " record found with id=" << sminvid;
				ACS_THROW(ISMStorageManagerException( exMsg.str() )); // PRQA S 3081
			}
		}

		myRow3.column(0,smRemotepath);
		if(checkSignature) {
			myRow3.column(1,storedSignature);
        }
		
		// events represent just the ISM I/F status, not the whole upload status
		ACS_LOG_EVENT("ISM_IF",EXFE_BUSY) ;

		IsmSoapClient ismclient ;
		ismclient.setConnParameters(rset, SMId()) ;
		
		string clientName = getClientName();
		if( not clientName.empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Client name \"" << clientName << '\"' );
			ismclient.setClientReference(clientName) ;
		}
		
		Timer dwntimer;
		dwntimer.start();
		ismclient.download(smRemotepath , user , localpath , tpretrtimeout  ) ;
		dwntimer.stop();
		double jobTime = dwntimer.elapsed() * 0.001; // PRQA S 3011, 3084
		// APF-270
		ACS_LOG_MC_INFO( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) << mc_param("fileid", smRemotepath) << mc_param("smid", SMId()) << // PRQA S 3081
						mc_param("filesize", File::size(localpath)) << mc_param("jobtime", jobTime) <<
						 mc_metric(Metric_OUT) << MCCode ("SM") << MCFunction ("Archive")) ;


		// events represent just the ISM I/F status, not the whole upload status
		ACS_LOG_EVENT("ISM_IF",EXFE_IDLE) ;
		

        if (!noChangingHardlinkedFilesMode) {
		    // change permissions allowing other users to override this file later.
		    mode_t oldmode = File::getmod(localpath) ;
		    bool isdir = File::isDir(localpath) ;
		    File::chmod(localpath,oldmode & (isdir ? 0777775 :0777664)) ; // PRQA S 3000, 3003, 3084, 3380, 4400
        }
        
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "download from ISM successful.") ;

		/////// Check Signature ////////
		if(checkSignature)
		{
            Timer sig_timer ;
            sig_timer.start() ;
			SignatureAlgoFactory* factory = SignatureAlgoFactory::instance();
			SignatureAlgo* algo = factory->getAlgo(signatureAlgo,localpath);
			SignatureAlgoScopeGuard guard(algo);
			if (!algo) {
				ACS_THROW(SignatureAlgoFactory::NotFoundException("No SignatureAlgo got with name " + signatureAlgo  )) ; // PRQA S 3081
			}
			algo->setManageDirectories(true);
			string signature = algo->computeSignature();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed signature: " << signature << " Stored signature: " << storedSignature) ;
			
            sig_timer.stop() ;
            statN()["download"]["signature"]["elapsed [ms]"]+= double(sig_timer.elapsed()) ; // PRQA S 3081

			if(signature!=storedSignature)
			{
				ACS_THROW(ISMStorageManagerException("Check Signature Failed for file \"" + oper.getFileName(*getObject()) + "\" version \"" + oper.getFileVersion(*getObject())  + "\".")); // PRQA S 3081
			}
		
		}
		
	}
    catch (dbDownloadFile::SMUnknownFileException& ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) // PRQA S 3081, 3380, 3385
						<< ( smRemotepath.empty() ? mc_null_param() : mc_param("fileid", smRemotepath) ) << mc_param("smid", SMId())
						<< mc_param("message", "File unknown to ISM.") << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;

        statN()["download"]["unknown"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ISM_IF",EXFE_ERROR) ;
        ACS_THROW(ISMUnknownFileException (ex, "File \"" + oper.getFileName(*getObject()) + "\" version \"" + oper.getFileVersion(*getObject())  + "\" unknown to ISM.")); // PRQA S 3081
    }
    catch (dbDownloadFile::FileToDownloadIsNearlineExc& ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) // PRQA S 3081, 3380, 3385
						<< ( smRemotepath.empty() ? mc_null_param() : mc_param("fileid", smRemotepath) ) << mc_param("smid", SMId())
						<< mc_param("message", "File is near-online") << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;

        statN()["download"]["nearline"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ISM_IF",EXFE_ERROR) ;
	throw;
    }
    catch (dbDownloadFile::FileToDownloadIsOfflineExc& ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) // PRQA S 3081, 3380, 3385
						<< ( smRemotepath.empty() ? mc_null_param() : mc_param("fileid", smRemotepath) ) << mc_param("smid", SMId())
						<< mc_param("message", "File is offline") << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;

        statN()["download"]["offline"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ISM_IF",EXFE_ERROR) ;
	throw;
    }
    catch(exception &ex)
    {
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject()))  // PRQA S 3081, 3380, 3385
						<< ( smRemotepath.empty() ? mc_null_param() : mc_param("fileid", smRemotepath) ) << mc_param("smid", SMId())
						<< mc_param("message", string("Downloading error ") + ex.what()) << mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
		
        statN()["download"]["error"]+= 1 ; // PRQA S 3081
        ACS_LOG_EVENT("ISM_IF",EXFE_ERROR) ;
        ACS_THROW(ISMStorageManagerException(ex, "Error downloading to path \"" + localpath + "\" from ISM")); // PRQA S 3081
    }
    timer.stop() ;
    statN()["download"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
}	

void ISMStorageManager::upload(const string &path, const dbPersistentOperatorIF& oper )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: \"" << path <<"\"") ;
	assignOper(oper) ;
	if(!running())
	{
		_path = path;
		Thread::start();	
	}
	else
	{
		ACS_THROW(dbPersistent::InvalidUse("Thread is already running.")) ; // PRQA S 3081
	}
}

void ISMStorageManager::run()
{

    Timer timer ;
    timer.start() ;
    off_t file_size = 0 ; 
    statN()["upload"]+=1 ;
	string filename = _oper->getFileName(*getObject());
	string fileversion = _oper->getFileVersion(*getObject());
	
	string completepath = "/tmp/ISMConf";
	ostringstream os;
	os << SMId();
	completepath += os.str();
	
    string paramfile = File::tempname(completepath, "rs") ; // PRQA S 3050
	FileGuard paramfile_guard(paramfile) ;
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "param file " << paramfile) ;
	if (_check.simulate())
	{
		ACS_LOG_WARNING("ISM in simulation mode. ") ;
	}
	try {	
		string invreltable ;
		rsResourceSet rset ;
		try {
	        dbConnectionWrapper pw(getPool()) ;
	        dbConnection &theConnection = pw ; // PRQA S 3050
			getParameters(theConnection, paramfile, rset, invreltable) ;
		}
		catch(exception &e) {
			ACS_THROW(ISMStorageManagerException(e,"unable to get upload parameters")) ; // PRQA S 3081
		}


		string group ;
		short permission = 0 ;
		string user;
		
		rset.getValue("SMConfiguration.user", user);
		rset.getValue("SMConfiguration.group", group);
		rset.getValue("SMConfiguration.permission", permission);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "user " << user ) ;
		
		string signatureAlgo;
		bool computeSignature = false ;
		try {
			rset.getValue("SMConfiguration.Signature.Algo", signatureAlgo);
			rset.getValue("SMConfiguration.Signature.Compute", computeSignature);
		}
		catch(rsResourceSet::NotFoundException &ex)
		{
			computeSignature=false;
		}
		
		/////// Compute Signature ////////
		string signature;
		if(computeSignature)
		{
            Timer sig_timer ;
            sig_timer.start() ;

			SignatureAlgoFactory* factory = SignatureAlgoFactory::instance();
			SignatureAlgo* algo = factory->getAlgo(signatureAlgo,_path);
			SignatureAlgoScopeGuard guard(algo);
			if (!algo) {
				ACS_THROW(SignatureAlgoFactory::NotFoundException("No SignatureAlgo got with name " + signatureAlgo  )) ; // PRQA S 3081
			}
			algo->setManageDirectories(true);
			signature = algo->computeSignature();
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Computed signature: " << signature ) ;
            sig_timer.stop() ;
            statN()["upload"]["signature"]["elapsed [ms]"]+= double(sig_timer.elapsed()) ; // PRQA S 3081
		}

		
		//Path composition
		ostringstream ismpath;
		ismpath << "/" ;
		DateTime currentTime;
		DateTime::UTCD utcd = currentTime ; // PRQA S 3050
		ismpath << setfill('0') << setw(4) << utcd.year // PRQA S 4400 3
		   << '-' << setw(2) << utcd.month
		   << '-' << setw(2) << utcd.day ;

		// APF-336
		// add the hostname to allow cluster management

		string theHostName;
		try
		{
			theHostName = Application::hostname();
			if (theHostName.empty())
			{
				ACS_LOG_WARNING ("Got an empty hostname from the current host");
				theHostName = "NoHostname";
			}
		}
		catch (exception& ex)
		{
			ACS_LOG_WARNING ("Got an exception while getting the hostname from the current host");
			theHostName = "GotException";
		}

		ismpath << "/" << theHostName;
		ismpath << "/" <<filename ;

		if (!fileversion.empty())
		{
			ismpath << "_V" << fileversion ;
		}

		// APF-336
		// add also a timestamp with milliseconds to improve the resilience to multiple storage of the same file
		ismpath << "_" << setw(2) << utcd.hour << setw(2) << utcd.min << setw(2) << utcd.sec << setw(3) << utcd.millisec(); // PRQA S 4400

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ISM path is \"" <<  ismpath.str()) ;

		IsmSoapClient::fileDefinition fdef ;
		

		fdef.filePath = ismpath.str() ;
		// if is a directory size will be computed 
		fdef.size = File::size(_path) ;
		fdef.user = user;
		fdef.group = group;
		fdef.permissions = permission ;

		//Set group in CustomAttrib
		string filegroup;
		{
	        dbConnectionWrapper pw(getPool()) ;
	        dbConnection &theConnection = pw ; // PRQA S 3050
			try {
				filegroup = getGroupFromConfSpace(theConnection);
			}
			catch( exception &e )
			{
				ACS_LOG_NOTIFY_EX(e);
				ACS_LOG_WARNING("Cannot get group from configuration");
			}
			
			if( filegroup.empty() ) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File group not found in conf space: getting from filetype");
				filegroup = _oper->getGroup(*getObject(), theConnection);
			}
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "file group :\"" << filegroup << "\"") ;
		
		fdef.fileClass = filegroup;

		string creator ;
		creator = Application::appName() ;
		if (creator.empty()) {
			creator = "ISMStorageManager" ;
        }

		fdef.customAttributes["creator"] = creator ;
		fdef.originalFilename = filename;

		IsmSoapClient ismclient(this) ;
        file_size = fdef.size ;
		string finalIsmPath ;
		try {
			ACS_LOG_EVENT("ISM_IF",EXFE_BUSY) ;
			
			if (!_check.simulate()) {
				//finally upload file.
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Before put in Ism "
						   << " Path " << _path 
						   << " size (bytes) " << file_size 
						   << " ismpath " << ismpath.str()) ;				
				ismclient.setConnParameters(rset, SMId()) ;

				Timer upltimer;
				upltimer.start();
				ismclient.upload(fdef , _path , true, _references ) ;
				// got IsmSoapClient upload return instead of ismpath.str() for general case (e.g. to manage also AWS_S3 )
				finalIsmPath = _references[0].second;
				upltimer.stop();
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "after put in Ism: final path=\"" << finalIsmPath << '\"' ) ;
				
				double jobTime = upltimer.elapsed() * 0.001; // PRQA S 3011, 3084
				
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("fileid", finalIsmPath) << mc_param("smid", SMId()) // PRQA S 3081
								<< mc_param("filesize", file_size) << mc_param("jobtime", jobTime)
								<< mc_metric(Metric_IN) << MCCode ("SM") << MCFunction ("Archive")) ;

			}
			else {
				ACS_LOG_WARNING("Simulating Ism.upload of file \"" << _path << "\"") ;
				// simulate an upload delay proportional to the file size as speed 
                // would be 10MB/sec
				usleep(file_size/10) ; // PRQA S 3000, 3010, 4400
			}
			
			// events represent just the ISM I/F status, not the whole upload status
			ACS_LOG_EVENT("ISM_IF",EXFE_IDLE) ;
		}
		catch(exception &) {	
	
			ACS_LOG_EVENT("ISM_IF",EXFE_ERROR) ;
			throw ;
		}

		dbConnectionWrapper pw(getPool()) ;
        dbConnection &myconn = pw ; // PRQA S 3050
	    const string &dbKey = myconn.getKey() ;
		
        db::Transaction transaction(myconn) ;

		transaction.begin_work("INS InvRel") ;

		try {		
            
            dbPersistent::IdType newID =  insertInvSM(myconn, _oper->getId(*getObject())) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "newID " << newID) ;
			
			SQLString sqlTAB( dbKey, SQLString::INSERT ) ;
        	sqlTAB.addTableList(invreltable);
	       	// Add Values
            sqlTAB.addValuesList("id", db::toSQL(dbKey, newID) ) ; 
            sqlTAB.addValuesList("path", db::toSQL(dbKey, finalIsmPath) ) ;
			if(computeSignature) {
            	sqlTAB.addValuesList("signature", db::toSQL(dbKey, signature) ) ; // PRQA S 3050, 3081
            }

			dbQuery queryTAB(myconn, sqlTAB, "InsertSMInventoryRel") ;	

			if (!queryTAB.execForStatus())
			{
			   dbQuery::QueryExecuteException e("Cannot insert into " + invreltable + ": Command : "
			    								+ sqlTAB.getSQL() + " Error : " + queryTAB.errorString()) ;
			   ACS_THROW(e) ;
			}
            
            transaction.commit() ;
		}
		catch (exception &)
		{
			//Erase the file from ISM.
			try{
				transaction.roll_back() ;
				ismclient.remove( finalIsmPath, user ) ;
		
			}
			catch(exception &e)
			{
				ACS_LOG_OPALERT("Probable wrong remotion of " + finalIsmPath + " from ISM: please check, exception follows.") ;
			    ACS_LOG_NOTIFY_EX(e) ;
            }

			throw;
		}
			
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ism uploading done. Reference = \"" << finalIsmPath <<"\"");
	}
	catch(exception &be)
	{
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", filename) << mc_param("fileversion", fileversion) << mc_param("smid", SMId()) << // PRQA S 3081
						 mc_param("message", string("Uploading error ") + be.what() ) <<
						 mc_metric(Metric_FAIL) << 
						 MCCode ("SM") << MCFunction ("Archive")) ;
		
        statN()["upload"]["error"]+= 1 ; // PRQA S 3081
		ostringstream os1 ;
		os1 << "Uploading inventory: " << _oper->getFileName(*getObject()) 
		   << " version " << _oper->getFileVersion(*getObject()) 
		   << " on SmId " << SMId() << " failed from path: " << _path ;
		ACS_THROW(ISMStorageManagerException (be, os1.str())) ; // PRQA S 3081
	}
    timer.stop() ;
    statN()["upload"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081 3
    statN()["upload"]["size [Kb]"]+= file_size/1024.0 ; // PRQA S 3011, 4400
    statN()["upload"]["ok"]+= 1 ;
}	

void ISMStorageManager::erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF& oper,string*) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;
	
    Timer timer ;
    timer.start() ;
    statN()["erase"]+= 1 ;
	
	string ismpath ;
	doAction(sminvid, oper, REMOVE_FILE, ismpath)	;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << ismpath << "\" file erased");
		
	try
	{
		dbConnectionWrapper pw(getPool()) ;
		dbConnection &theConnection = pw ; // PRQA S 3050
        deleteInvSM(theConnection,sminvid ) ;
	}
	catch (exception &e)
	{
		ostringstream mcMessage;
		mcMessage << getInvSmTableName() << " record[" << sminvid << "] deletion failed ";
		// APF-270
		ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) << mc_param("fileid", ismpath) << mc_param("smid", SMId()) // PRQA S 3081
							<< mc_param("message", mcMessage.str() + e.what()) 
							<< mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
		ostringstream os1 ;
		os1 << getInvSmTableName() << " record[" << sminvid << "] deletion failed. Inventory could be incoherent for SmId." << SMId() 
		   << " inventory["<< oper.getId(*getObject()) <<"]: " << oper.getFileName(*getObject()) << " version " << oper.getFileVersion(*getObject()) ;
		ACS_THROW(ISMStorageManagerException(e, os1.str() )) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "record deletion done. Id = \"" << sminvid <<"\" .") ;
    timer.stop() ;
    statN()["erase"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
}

void ISMStorageManager::getAvailableStorageInfo(vector<pair<string, long long int> > &v) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    Timer timer ;
    timer.start() ;
    statN()["getStorageInfo"]+= 1 ;

	string completepath = "/tmp/ISMConf";
	ostringstream os;
	os << SMId();
	completepath += os.str();
	
	string paramfile = File::tempname(completepath, "rs") ; // PRQA S 3050
	FileGuard paramfile_guard(paramfile) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "param file " << paramfile) ;


	try {	
		string dummy ;
		rsResourceSet rset ;
		try {
			dbConnectionWrapper pw(getPool()) ;
			dbConnection &theConnection = pw ; // PRQA S 3050
			getParameters(theConnection, paramfile, rset, dummy) ;
		}
		catch(exception &e) {
			ACS_THROW(ISMStorageManagerException(e,"unable to get server parameters")) ; // PRQA S 3081
		}

		// events represent just the ARF I/F status, not the whole upload status
		ACS_LOG_EVENT("ISM_IF",EXFE_BUSY) ;

		IsmSoapClient ismclient ;
		ismclient.setConnParameters(rset, SMId()) ;
		
		ismclient.getCachesStorageInfo(v) ;	
	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Storage info successfully got") ;
	} //try
	catch(exception &ex)
	{
		ostringstream exMsg;
		exMsg << "Error getting storage info for ISM with id=" << SMId();
		ACS_THROW(ISMStorageManagerException( ex, exMsg.str() )) ;
	}
		
    timer.stop() ;
    statN()["getStorageInfo"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081

}

void ISMStorageManager::giveup(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &oper)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ;

    Timer timer ;
    timer.start() ;
    statN()["giveup"]+= 1 ;

	string ismpath;
	doAction(sminvid, oper, GIVEUP_REQUEST, ismpath)	;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request for \"" << ismpath << "\" file given up");
			
    timer.stop() ;
    statN()["giveup"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081

}

bool ISMStorageManager::uploading() // PRQA S 4020
{
	if( running() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Still uploading") ;
		return true;
	}
	
	std::exception *e = 0 ;
	runException(e) ;
	
	if(e)
	{
		ISMStorageManagerException e2(*e,"Error in uploading thread");
		delete e ;
		ACS_THROW(e2);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "done.") ;
	
	return false;
}

bool ISMStorageManager::isUploadSynchronous() const 
{
	return true ;
}

bool ISMStorageManager::isDownloadSynchronous() const 
{
	return true ;
}


bool ISMStorageManager::isEraseSynchronous() const 
{
	return true ;
}




void ISMStorageManager::assignOper(const dbPersistentOperatorIF &o) 
{
	delete _oper ;
	_oper=0 ;
	_oper=o.clone() ;
}


bool ISMStorageManager::hasTreeManagement() const 
{
	return true ;
}
	

void ISMStorageManager::updateDbReferences(const vector< pair< string, pair < string, string> > > &pl, dbConnection &conn)  
{
    const string &dbKey = conn.getKey() ;
	string invrelspectable ; // specific relation table e.g. acsisminvrelationtab1 ..
	{
		// get the specific relation table from t_storagemanager (my smid is always returned from SMId()
		ostringstream queryName ; queryName << "StorageManagerGetInvRel_" << SMId() ;
		SQLString queryString( dbKey, SQLString::SELECT ) ;
    	queryString.addTableList("t_storagemanager");
    	queryString.addSelectList("invrelationtable"); 

    	{
        	dbQueryParameters pp(dbKey) ;
	    	pp.addParameter(dbParam(dbKey, "t_storagemanager", "Id", dbParam::EQUAL_TO, db::toSQL(dbKey, SMId()) )) ;
        	queryString.addANDWhereParameter(pp) ;
    	}


		dbQuery query(conn, queryString, queryName.str()) ;	

		dbRow myRow(dbKey) ;
		try { 
			dbSet set;
			query.execForSet(set) ;
			ACS_COND_THROW(not set.getNextRow(myRow), ISMStorageManagerException( "No record found" )); // PRQA S 3081
		}
		catch(ISMStorageManagerException &x) {
			ostringstream os ;
			os << "cannot get t_storagemanager.invrelationtable for smid=" << SMId() << ": No record found";
			ACS_THROW(exIllegalValueException(os.str())) ;  
		}
		catch(exception &x) {
			ostringstream os ;
			os << "cannot get t_storagemanager.invrelationtable for smid:" << SMId() ;
			ACS_THROW(exIllegalValueException(x,os.str()));
		}

		myRow.column(0,invrelspectable);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "specific invrelation table is " << invrelspectable) ;
	}
	
	
	vector<int> onDb ; // will contains 0 if no rec into t_inventorysm, 1 if only into t_inventorysm 2 also into invrelspectable
	vector<dbPersistent::IdType> invrelationid ; // in case latter is > 0, this will be the id
	
	{
		ostringstream queryName ; queryName << "InventorySmGetId_" << SMId() ;
		for (size_t i=0; i < pl.size(); i++) {
			string fname = pl[i].second.first ;
			string fversion = pl[i].second.second ;

    		SQLString queryString(dbKey, SQLString::SELECT ) ;
    		queryString.addTableList(getInvSmTableName());
    		queryString.addSelectList(getInvSmTableName()+".id");
    		queryString.addSelectList(invrelspectable+".id");

    		{
        		dbQueryParameters pp(dbKey) ;
	    		pp.addParameter(dbParam(dbKey, getInvSmTableName(), "filename", dbParam::EQUAL_TO, db::toSQL(dbKey, fname) )) ; // PRQA S 3050, 3081 3
	    		pp.addParameter(dbParam(dbKey, getInvSmTableName(), "fileversion", dbParam::EQUAL_TO, db::toSQL(dbKey, fversion) )) ;
	    		pp.addParameter(dbParam(dbKey, getInvSmTableName(), "storagemanid", dbParam::EQUAL_TO, db::toSQL(dbKey, SMId()) )) ;
        		queryString.addANDWhereParameter(pp) ;
    		}
			{
            	dbQueryParameters pp(dbKey) ;
	        	pp.addParameter(dbParam(dbKey,invrelspectable,"id" , dbParam::EQUAL_TO , getInvSmTableName()+".id" )) ;
	        	queryString.addJoinList(SQLDialect::LEFT,invrelspectable,pp) ;
			}

			dbQuery query(conn, queryString, queryName.str()) ;	

			dbRow myRow(dbKey) ;
			invrelationid.push_back(0) ;
			try { 
				dbSet dbset;
				query.execForSet(dbset) ;
				if( dbset.getNextRow(myRow) ) {
					myRow.column(0,invrelationid[i]); // get the id 
					bool set = false ;
					dbPersistent::IdType invrelationspecid = 0 ;
					myRow.columnSet(1,invrelationspecid, set); // get the id on specific (invspecreltable)
					onDb.push_back(set ? 2 : 1) ; // exists on db 2 exists on specific , 1 only on generic // PRQA S 3380, 4400
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fname " << fname
							   << " id on invsm: " << invrelationid[i]
							   << " set on specific: " << boolalpha << set) ;
				}
				else {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fname " << fname << " not on invsm") ;				
					onDb.push_back(0) ; // not exists on t_inventorysm 
				}
			}
			catch(exception &x) {
				ostringstream os ;
				os << "asking for " << getInvSmTableName() << " " << fname << " " << fversion << " for smid:" << SMId() ;
				ACS_THROW(exIOException(x,os.str())); // PRQA S 3081
			}
		}
	}

	string path = _references[0].first ;
	if (path[path.length()-1] == '/' ) { // PRQA S 3084 2
		path = path.substr(0,path.length() -1) ; // remove trailing '/' (if any)
	}
	size_t skip = path.length() - File::getFileName(path).length() ; // PRQA S 3084

    db::Transaction transaction(conn) ; // nb: auto roll_back when out of scope
	transaction.begin_work("updateDbReferences") ; // now the 'hard' job -- begin transaction

	for (size_t i=0; i < pl.size(); i++) {
		string fname = pl[i].second.first ;
		string fversion = pl[i].second.second ;
		string ismpath ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fname " << fname << " references: " << _references.size()) ;
		for (vector< pair<string , string> >::const_iterator r = _references.begin();  // PRQA S 4238
			 r  != _references.end(); 
			 ++r ) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "ref \"" << r->first.c_str()+skip << "\",\"" << r->second << "\"") ; // PRQA S 3084 2
			if (pl[i].first == r->first.c_str()+skip) {
				ismpath = r->second ; // the path(key) assigned from the last upload
				break ;				
			}
		}

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fname " << fname << " ismpath \"" << ismpath << "\"") ;
		ACS_COND_THROW(ismpath.empty(), // PRQA S 3081
					exIllegalValueException("Cannot find sm key for \"" + pl[i].first + "\"")) ;

  		ostringstream queryName ; queryName << "InsertSMIDInventoryRef_" << SMId() ;

		if (onDb[i] < 1) { // no record on t_inventorysm
			SQLString sqlSM( dbKey, SQLString::INSERT ) ;
        	sqlSM.addTableList(getInvSmTableName());
	       	// Add Values
            sqlSM.addValuesList("filename", db::toSQL(dbKey,fname) ) ; 
            sqlSM.addValuesList("fileversion", db::toSQL(dbKey,fversion) ) ;
            sqlSM.addValuesList("storagemanid", db::toSQL(dbKey,SMId()) ) ;

			dbQuery querySM(conn, sqlSM, queryName.str()) ;	

			if (!querySM.execForStatus()) {
			   dbQuery::QueryExecuteException e("Cannot find "+getInvSmTableName()+" record: Command : "
												+ sqlSM.getSQL() + " Error : " + querySM.errorString()) ;
			   ACS_THROW(e) ;
			}

			invrelationid[i] = conn.getLastSerial(getInvSmTableName()) ;

			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "newID " << invrelationid[i] ) ;
		}

		if (onDb[i] < 2) { // no record on invrelspectable // PRQA S 4400
 			ostringstream qName ; qName << "InsertSMInventoryRel_" << invrelspectable ;
			SQLString sqlTAB( dbKey, SQLString::INSERT ) ;
        	sqlTAB.addTableList(invrelspectable);
	       	// Add Values
            sqlTAB.addValuesList("id", db::toSQL(dbKey,invrelationid[i]) ) ; 
            sqlTAB.addValuesList("path", db::toSQL(dbKey,ismpath) ) ;

			dbQuery queryTAB(conn, sqlTAB, qName.str()) ;	

			if (!queryTAB.execForStatus())
			{
			       dbQuery::QueryExecuteException e("Cannot insert into " + invrelspectable + " table: Command : "
			    									+ sqlTAB.getSQL() +" Error : " + queryTAB.errorString()) ;
			       ACS_THROW(e) ;
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "fname " << fname << " inserted on specific: " << invrelationid[i]) ;
		}
		else { // already a record on specific table 'invrelspectable': update it
 			ostringstream qName ; qName << "InsertSMInventoryRel_" << invrelspectable ;
			SQLString sqlTAB( dbKey, SQLString::UPDATE ) ;
        	sqlTAB.addTableList(invrelspectable);
	       	// Add Values
            sqlTAB.addSetList("path", db::toSQL(dbKey,ismpath) ) ;

    		{
        		dbQueryParameters pp(dbKey) ;
	    		pp.addParameter(dbParam(dbKey, invrelspectable, "id", dbParam::EQUAL_TO, db::toSQL(dbKey,invrelationid[i]) )) ;
        		sqlTAB.addANDWhereParameter(pp) ;
    		}
			dbQuery queryTAB(conn, sqlTAB, qName.str()) ;	

			if (!queryTAB.execForStatus())
			{
			       dbQuery::QueryExecuteException e("Cannot update " + invrelspectable + ": Command : "
			    									+ sqlTAB.getSQL() + " Error : " + queryTAB.errorString()) ;
			       ACS_THROW(e) ;
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"fname " << fname << " updated on specific: " << invrelationid[i]) ;
		}
	}
    transaction.commit() ;
}

string ISMStorageManager::getGroupFromConfSpace(dbConnection & conn)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631 2
	
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	string localRootTag = "PDSGroupsRedefinition.Groups.Pair";
    size_t elements = rs.getArraySize( localRootTag ); // PRQA S 3000, 3010, 4412
	dbSmartPersistent r("t_storagemanager", conn);
	r.setId(SMId());
	if( not r.load(conn) ) {
		ostringstream exMsg;
		exMsg << "Cannot load t_storagemanager record with id=" << SMId();
		ACS_THROW( ISMStorageManagerException( exMsg.str() ) );
	}
	string ismName;
	bool isNull;
	r.get("name" , ismName , isNull, conn) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag=" << localRootTag << ", elements " << elements );
	string group;
	for( size_t i=0;  i < elements && group.empty(); i++)
	{
		string tag = localRootTag;
		if( elements > 1 ) {
			ostringstream os ;
			os << "[" << i << "]" ;
			tag += os.str();
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Tag=\"" << tag << "\" going to search Ism name " << ismName);

		string currentIsmName ;
		rs.getValue( tag + ".Ism.value", currentIsmName);
		if( StringUtils::equals(currentIsmName,ismName) ) {
			rs.getValue( tag + ".group.value", group);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ism SM found: " << currentIsmName << "-->\"" << group << '\"' );
		}
	}
	
	return group;
}

void ISMStorageManager::doAction(const dbPersistent::IdType sminvid, const dbPersistentOperatorIF & oper, const Action & action, string & ismpath)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631 2
	
	string completepath = "/tmp/ISMConf";
	ostringstream os;
	os << SMId();
	completepath += os.str();
	
	string paramfile = File::tempname(completepath, "rs") ; // PRQA S 3050
	FileGuard paramfile_guard(paramfile) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "param file " << paramfile) ;

    const string &dbKey = getPool().getKey() ;
	try {	
		string table ;
		rsResourceSet rset ;
		try {
			dbConnectionWrapper pw(getPool()) ;
			dbConnection &theConnection = pw ; // PRQA S 3050
			getParameters(theConnection, paramfile, rset, table) ;
		}
		catch(exception &e) {
			ACS_THROW(ISMStorageManagerException(e,"unable to get upload parameters")) ; // PRQA S 3081
		}

	
    	SQLString sqlstring( dbKey, SQLString::SELECT ) ;
        sqlstring.addSelectList("path");
        sqlstring.addTableList(table);
		{
			dbQueryParameters pp(dbKey) ;
			pp.addParameter(dbParam(dbKey, table,"Id" ,  dbParam::EQUAL_TO, db::toSQL(dbKey, sminvid))) ; 
			sqlstring.addANDWhereParameter(pp) ;
        }
		
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to perform the following query: " << sqlstring.getSQL() );
		dbRow myRow3(dbKey) ; 


		{
			dbConnectionWrapper pw(getPool()) ;
			dbConnection &theConnection = pw ; // PRQA S 3050
			dbQuery query3(theConnection, sqlstring, "SelectSMPath") ;	
			dbSet set;
			query3.execForSet(set) ;
			if( not set.getNextRow(myRow3) ) {
				ostringstream exMsg;
				exMsg << "No " << table << " record found with id=" << sminvid;
				ACS_THROW(ISMStorageManagerException( exMsg.str() )); // PRQA S 3081
			}
		}

		myRow3.column(0,ismpath);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform action \"" << getActionString(action) << "\" on path \"" << ismpath << '\"');
		

		string user ;
		
		rset.getValue("SMConfiguration.user", user);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "user " << user) ;

		// events represent just the ARF I/F status, not the whole upload status
		ACS_LOG_EVENT("ISM_IF",EXFE_BUSY) ;

		IsmSoapClient ismclient ;
		ismclient.setConnParameters(rset, SMId()) ;

		switch (action)  {
    		case REMOVE_FILE: 
			{
				ismclient.remove( ismpath, user ) ;	
				// APF-270
				ACS_LOG_MC_INFO( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) << mc_param("fileid", ismpath) << mc_param("smid", SMId()) // PRQA S 3081
								 << mc_metric(Metric_DELETE) << MCCode ("SM") << MCFunction ("Archive")) ;
				break;
			}
    		case GIVEUP_REQUEST: 
			{ 
				ismclient.giveupFileRequest( ismpath, user, getClientName() ) ;	
				break;
			}
			default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: action to do not managed: " << int(action); // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
		}	
		
	} //try
	catch(exception &ex)
	{
		ostringstream exMsg ;
		switch (action)  {
    		case REMOVE_FILE: 
			{
				string mcErrorMsg;
				if( dynamic_cast<IsmSoapClient::FileToDeleteOnMigratingMediumExc*> (&ex) ) {
					mcErrorMsg = "Cannot erase since medium migration is in progress; retry later." ;
					exMsg << "Cannot erase file \"" << oper.getFileName(*getObject()) << "\" version \"" << oper.getFileVersion(*getObject()) << "\" since medium migration is in progress; retry later." ;
				}
				else {
					mcErrorMsg = string("Erase error ") + ex.what() ;
					exMsg << "Error erasing file \"" << oper.getFileName(*getObject()) << "\" version \"" << oper.getFileVersion(*getObject()) << "\"." ;
				}
				// APF-270
				ACS_LOG_MC_ERROR( mc_param("filename", oper.getFileName(*getObject())) << mc_param("fileversion", oper.getFileVersion(*getObject())) << mc_param("fileid", ismpath) << mc_param("smid", SMId()) // PRQA S 3081
									<< mc_param("message", mcErrorMsg) 
									<< mc_metric(Metric_FAIL) <<  MCCode ("SM") << MCFunction ("Archive")) ;
				
				break;
			}
    		case GIVEUP_REQUEST: 
			{ 
				exMsg << "Error deleting request for file \"" << oper.getFileName(*getObject()) << "\" version \"" << oper.getFileVersion(*getObject()) << "\"." ;
				break;
			}
			default:
        	{
		    	exMsg << "Exception got for unknown operation: entity to delete not managed: " << int(action); // PRQA S 3081 2
        	}				
		}	

		ACS_THROW( ISMStorageManagerException(ex, exMsg.str()) ) ;
	}
	
}

string ISMStorageManager::getActionString(const Action & action) // PRQA S 4020
{	
	string actionString ;
	switch (action)  {
    	case NONE: 
			{ 
				actionString = "None"; 
				break;
			}
		case REMOVE_FILE: 
			{ 
				actionString = "RemoveFile"; 
				break;
			}
		case GIVEUP_REQUEST:
			{
				actionString = "GivupRequest"; 
				break;
			}
		default:
        	{
		    	ostringstream exMsg;
		    	exMsg << "Internal ERROR: action (" << int(action) << ") unrecognized"; // PRQA S 3081 2
		    	ACS_THROW( exIllegalValueException( exMsg.str() ) ) ;
        	}				
	}
	return actionString	;
}



_ACS_END_NAMESPACE
