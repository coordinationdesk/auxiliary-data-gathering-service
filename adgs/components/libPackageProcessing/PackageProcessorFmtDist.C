// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.22  2017/02/21 18:09:12  nicvac
	S3EVOL-13: Datapackage serialization.
	
	Revision 1.21  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.20  2016/12/19 18:32:37  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.19  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.18  2016/09/07 08:30:53  nicvac
	CSGACQ-86: Oracle porting.
	
	Revision 1.17  2016/08/03 14:57:06  damdec
	CSGACS-94: handling of clobs changed.
	
	Revision 1.16  2016/01/29 11:17:38  marpas
	typos fixed
	
	Revision 1.15  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.14  2015/10/05 16:57:55  nicvac
	ALSAT1B-53: format task table for cart scenario.
	
	Revision 1.13  2015/08/31 13:22:11  nicvac
	ALSAT1B-44: remote path wants double slash
	
	Revision 1.12  2015/08/20 16:38:09  nicvac
	ALSAT1B-44
	
	Revision 1.11  2015/08/20 16:18:30  nicvac
	ALSAT1B-44: cart handling
	
	Revision 1.10  2013/11/06 13:41:13  nicvac
	Used macro namespace
	
	Revision 1.9  2013/11/04 10:00:33  marpas
	avoiding useless macros
	
	Revision 1.8  2013/10/03 15:32:13  nicvac
	acs sdk warnings processed.
	
	Revision 1.7  2013/07/16 16:01:26  nicvac
	S2PDGS-326: t_repositories updated.
	
	Revision 1.6  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.5  2013/06/14 16:28:14  nicvac
	ADC-182: Temp transder strategy implemented.
	
	Revision 1.4  2013/03/29 17:26:31  nicvac
	S2PDGS-256: implementation.
	
	Revision 1.3  2013/03/21 14:44:28  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.2  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.1  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
        
*/ 

#include <PackageProcessorFmtDist.h>

#include <ExporterServiceResolver.h>
#include <PackageTaskTableManager.h>

#include <DataProviderConfigurer.h>
#include <DistributionSerializable.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <RemoteSiteUtility.h>

#include <File.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)
using namespace dataprovider;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageProcessorFmtDist)


PackageProcessorFmtDist::PackageProcessorFmtDist():
	PackageProcessor()
{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);

	//Load from configuration the processing steps to be performed on the data package.
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
	conf.getArray("PackageProcessing.List_of_ProcessingSteps.ProcessingStep", _processingSteps);

	//Set the Processing Info I/O parameter
	CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();

	_currentExpProcInfo.setCacheDownloadRepository( cacheSptr->getPath() );
	_currentExpProcInfo.setCacheAvailableDownloadSpaceSize( cacheSptr->maxSize() );

}

PackageProcessorFmtDist::~PackageProcessorFmtDist() throw() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}

void PackageProcessorFmtDist::init( DataPackageSptr dataPackageSptr ) {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	//Retain the pointer of the Datapackage
	_dataPackageSptr = dataPackageSptr;
}


void PackageProcessorFmtDist::dumpPackageInfos() const {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	//Distribution dump
	DistributionSerializable const& distribution = _currentExpProcInfo.getDistributionContent();
	distribution.dump( _currentExpProcInfo.getDistributionFilename() );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _currentExpProcInfo.getDistributionFilename()<<" dumped.");

	//Datapackage dump
	_dataPackageSptr->dump( _currentExpProcInfo.getDatapackageFilename() );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, _currentExpProcInfo.getDatapackageFilename()<<" dumped.");

	//Parameters dump
	if ( ! _parameters.empty() ) {
		std::ofstream ofs( _currentExpProcInfo.getParametersFilename().c_str() );
		ofs << _parameters;
	}

}

void PackageProcessorFmtDist::setUpWorkingDir() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	dbPersistent::IdType dpid = _dataPackageSptr->getPackageId();

	//Prepare the working dir:
	//- create the working dir at the same path of the downloaded data package (packageId).
	//- link (symbolic) the data package directory in the created working dir.


	//*/Create working dir: given as input "/path/downl/10/" create wd "/path/downl/10_ProcWD/"

	//Files have been downloaded into a "packaged id dir". Take the path from one of the downloaded files.
	std::stringstream ssWorkingDir, ssdpDownPath, ssSymlink;
	ssdpDownPath << acs::conf::DPConfigurerSingleton::instance()->getProductPath() << "/" << _dataPackageSptr->getPackageId() << "/";
	ssWorkingDir << _dataPackageSptr->getPackageId() << "_ProcWD/";
	
	string dpDownloadedPath = ssdpDownPath.str();
	string wdName =  ssWorkingDir.str();
	_workingDir = File::getDirPath ( dpDownloadedPath ) + "/" + wdName;
	
	if ( File::exists(_workingDir) ) {
		ACS_LOG_PRIVINFO("[DP "<<dpid<<"] "<<_workingDir<<" already exists. Cleaning and recreating...");
		File::unlink(_workingDir);
	}
	File::mkdirtree( _workingDir );

	ACS_LOG_PRIVINFO("[DP "<<dpid<<"] Processing data package id "<<_dataPackageSptr->getPackageId()<<" into working dir "<<_workingDir);
	//*//

	//*/Link (symbolic) the data package directory in the created working dir.
	ssSymlink << _workingDir << "/" << _dataPackageSptr->getPackageId();
	File::symlink(dpDownloadedPath, ssSymlink.str());
	//*/

	//Set the Proc Info	
	_currentExpProcInfo.setDatapackageAbsPath( ssSymlink.str() );
	_currentExpProcInfo.setWorkingDir( _workingDir );

	//Set Processor interface Files: Filetype - Filename 
	_procFileIF[ _fileTypeExpDatapackage ] =  _workingDir +"/datapackage.xml"; 
	_procFileIF[ _fileTypeExpDistribution ] = _workingDir +"/distribution.xml";
	_procFileIF[ _fileTypeExpParameters ] =   _workingDir +"/expParameters.xml"; //The ExpParameters file coming from the distribution rule

	_currentExpProcInfo.setDatapackageFilename( _procFileIF[ _fileTypeExpDatapackage ] );

	_currentExpProcInfo.setDatapackageContent( *_dataPackageSptr );

	_currentExpProcInfo.setDistributionFilename(_procFileIF[ _fileTypeExpDistribution ] );
	// Note: Distribution Content is set in setUp() method.

	if ( ! _parameters.empty() ) {
		_currentExpProcInfo.setParametersFilename(  _procFileIF[ _fileTypeExpParameters ] );
	}

	//Dump the datapackage and distribution objects to files
	//Dump also parameters
	this->dumpPackageInfos();

}

void PackageProcessorFmtDist::setUp() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	dbPersistent::IdType dpid = _dataPackageSptr->getPackageId();

	//Query the DB retrieving repository and Task table info
	try
	{	
		dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
		dbConnection& conn = pw;
        const string &dbKey = conn.getKey() ;

		dbSet set;
		string sqlStr("");
		try {
			/*/string sqlString = 
				"SELECT \
				disq.retrycounter, disr.connretries, disr.id_distributionrule, \
				prot.protocol, rep.username, rep.password, hosts.hostname, rep.remotepath, repTmp.tempname, rep.tempname_value \
				ttfmt.id_tasktable, ttdis.id_tasktable
				FROM \
				t_distributionqueue as disq, \
				t_distributionitems as disit, \
				t_distributionrules as disr, \
				t_repositories as rep, \
				t_transferprotocols as prot, \
				t_knownhosts as hosts, \
				t_tmpnamestrategy as repTmp, \
				t_tasktables as ttfmt, \
				t_tasktables as ttdis, \
				t_tasktables as ttcheck \
				WHERE \
				disq.packageid = "+ _dataPackageSptr->getPackageId() +" AND \
				disq.packageid = disit.packageid AND \
				disit.distributionrule_id = disr.id_distributionrule AND \
				rep.transferprotocol_id = prot.id AND \
		    	rep.host_id = hosts.id AND \
				rep.tempname_id = repTmp.id_tmpnamestrategy AND \
				disr.repository_id = rep.id_repository AND \
				disr.format_tt_id = ttfmt.id_tasktable AND \
				disr.delivery_tt_id = ttdis.id_tasktable AND \
				disr.delcheck_tt_id = ttcheck.id_tasktable \
				FETCH FIRST 1 ROW ONLY"
				;
			//*/

			SQLString sqlString( dbKey, SQLString::SELECT );
			sqlString.setLimit(1);

			sqlString.addSelectList("disq.retrycounter");		
			sqlString.addSelectList("disr.connretries" );		/* sqlString.addSelectList("disr.parameters"); */
			sqlString.addSelectList("disr.id_distributionrule" );
			sqlString.addSelectList("prot.protocol"    );		sqlString.addSelectList("rep.username"   );
			sqlString.addSelectList("rep.password"     );		sqlString.addSelectList("hosts.host_name");
			sqlString.addSelectList("rep.remotepath"   );
			sqlString.addSelectList("repTmp.tempname"   );		sqlString.addSelectList("rep.tempname_value");
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				sqlString.addSelectList("tt"+ *it +".id_tasktable ", "lbl_"+*it);
			}

			sqlString.addTableList("t_distributionqueue", "disq"); sqlString.addTableList("t_distributionitems", "disit" );
			sqlString.addTableList("t_distributionrules", "disr"); sqlString.addTableList("t_repositories"     , "rep"   );
			sqlString.addTableList("t_transferprotocols", "prot"); sqlString.addTableList("t_knownhosts"       , "hosts" );
			sqlString.addTableList("t_tmpnamestrategy",   "repTmp");
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				sqlString.addTableList("t_tasktables", "tt"+ *it);
			}

			dbQueryParameters pp(dbKey);
			pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, db::toSQL(dbKey, _dataPackageSptr->getPackageId()) ));
			pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, "disit.packageid"                        ));
			pp.addParameter( dbParam(dbKey, "disit","distributionrule_id", dbParam::EQUAL_TO, "disr.id_distributionrule"               ));
			pp.addParameter( dbParam(dbKey, "disr" ,"repository_id"      , dbParam::EQUAL_TO, "rep.id_repository"                      ));
			pp.addParameter( dbParam(dbKey, "rep"  ,"transferprotocol_id", dbParam::EQUAL_TO, "prot.id"                                ));
			pp.addParameter( dbParam(dbKey, "rep"  ,"host_id"            , dbParam::EQUAL_TO, "hosts.id"                               ));
			pp.addParameter( dbParam(dbKey, "rep"  ,"tempname_id"        , dbParam::EQUAL_TO, "repTmp.id_tmpnamestrategy"              ));
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				pp.addParameter( dbParam(dbKey, "disr", *it, dbParam::EQUAL_TO, "tt"+ *it +".id_tasktable"));
			}

			sqlString.addANDWhereParameter( pp );

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL());

			sqlStr = sqlString.getSQL();

			dbQuery query ( conn, sqlString, "GetElementInDistQueueToFmt" );
        	query.execForSet ( set );
		} catch (exception& ex) {
			ACS_THROW(exPackageProcessorCriticalException(ex, "Cannot retrieve Repository and Task Table info from DB, starting fro package id "+dpid)) ;
		}

		//Expected just one row as result
		ACS_COND_THROW( 
			set.size()!=1, 
			exPackageProcessorCriticalException( "Retrieving repository and Task Table info, expected exactly one row from query: "+sqlStr) );


		//Extract from the result the Repository and Task Table info
		dbRow row(dbKey);
		set.getRow( 1, row );

		bool isNull(false); //If Null on DB, the attribute is left with its original value
		int i(0);
		string rep_protocol(""), rep_username(""), rep_password(""), rep_hostname(""), rep_remotepath("");
		string rep_tempStrategyName(""), rep_tempStrategyParam("");

		dbPersistent::IdType distr_id(0);
		row.column(i++, _retryCurrent, &isNull);	++_retryCurrent; //current retry = db retry + 1
		row.column(i++, _retryThreshold, &isNull);

		row.column(i++, distr_id, &isNull);
		dbSmartPersistent distR ("t_distributionrules", conn);

		if (distR.load( conn, distr_id) && (distR.isSet ("parameters", conn) ) )
		{
			// parameters found
			// download the blob data field

			string dwnName = File::tempname("/tmp/ParametersLoad", ".clob"); //Temp filename
			FileGuard dwnNameGuard (dwnName);
			distR.downloadFile("parameters", dwnName, conn) ;
			{
				std::ifstream ifs( dwnName.c_str() );
				std::stringstream buffer;
				buffer << ifs.rdbuf();
				_parameters = buffer.str();
			}
		}

		row.column(i++, rep_protocol, &isNull);
		row.column(i++, rep_username, &isNull); 
		row.column(i++, rep_password, &isNull);
		row.column(i++, rep_hostname, &isNull);	
		row.column(i++, rep_remotepath, &isNull);
		row.column(i++, rep_tempStrategyName, &isNull); 
		row.column(i++, rep_tempStrategyParam, &isNull);

		for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
			int ttId(-1); row.column(i++, ttId, &isNull);
			_ttMap[ *it ] = PackageTaskTableManager::instance()->getTTFilename( ttId );
		}

		//*/@@@ Temporary implementation! Cart architecture to be revised! (Cf. http://jira.acsys.it/browse/PDSEV-9)
		//@@@ In Current design the above query is performed fine because, even in case of cart scenario, the
		//@@@ distribution is linked to ad-hoc distribution rule, ad-hoc repository, ad-hoc user.
		//@@@ These ad-hoc entities are applicable to all the cart requests.
		if ( DataPackage::NullCartId !=  _dataPackageSptr->getCartId()  ) {

			//Overwrite values from Cart

			rsResourceSet& conf = ConfigurationSingleton::instance()->get();

			rsResourceSet cartInfo;
			_dataPackageSptr->getCartInfo( cartInfo );

			/*@@@ Cart example: Legacy architecture - to be revised! (Cf. http://jira.acsys.it/browse/PDSEV-9)
			mediaType:FTP
			format:defaultDistribFormat
			uploadApp:RemoteUpload
			uploadAppOpt:
			mailWarning:true
			email:foo@bar.com
			userId:opalsat
			remoteUrl:data-srv/data/alsat
			repository:NO-REP
			ftpUserId:opalsat
			ftpPassword:12qwas
			orderType:
			pkgExtRule:KEEPORIG
			pkgRequired:false
			compRequired:false
			compMode:
			secureFtp:false
			compress:false
			*/

			string mediaType; cartInfo.getValue( "mediaType", mediaType );
			ACS_COND_THROW( mediaType != "FTP",
					exPackageProcessorFmtDistCriticalException( "Mediatype ["+mediaType+"] from Cart is not supported. Configure a specific Task Table in distribution rule to handle this media type." ) );

			bool secureFtp; cartInfo.getValue( "secureFtp", secureFtp );

			rep_protocol = secureFtp ? "SFTP" : "FTP" ;
			cartInfo.getValue( "ftpUserId", rep_username );
			cartInfo.getValue( "ftpPassword", rep_password );

			bool cartPwdEncrypt(true);
			conf.getValue( "PackageProcessing.CartSettings.EncryptPassword", cartPwdEncrypt, false );
			if ( cartPwdEncrypt ) {
				rep_password = curl::RemoteSiteUtility::encrypt(rep_password);
			}

			{
				string hostAndPath; cartInfo.getValue( "remoteUrl", hostAndPath );
				size_t pos = hostAndPath.find("/");
				ACS_COND_THROW( pos == string::npos,
						exPackageProcessorFmtDistCriticalException( "Cannot found '/' in Cart url ["+hostAndPath+"] to get remote host and path." ) );

				rep_hostname = hostAndPath.substr( 0, pos );
				rep_remotepath = hostAndPath.substr( pos );
			}

			//Update the format Task table from Cart
			string formatStep;	conf.getValue("PackageProcessing.List_of_ProcessingSteps.ProcessingStep[0]", formatStep);
			string ttName;		cartInfo.getValue( "format", ttName );
			_ttMap[ formatStep ] = 	PackageTaskTableManager::instance()->getTTFilename( ttName );
		}

		//Set the Distribution object
		DistributionSerializable distribution;

		distribution.setProtocol  ( rep_protocol );
		distribution.setUsername  ( rep_username );
		distribution.setPassword  ( rep_password );
		distribution.setHostname  ( rep_hostname );
		distribution.setRemotepath( rep_remotepath );
		distribution.setTempStrategyName( rep_tempStrategyName );
		distribution.setTempStrategyParam( rep_tempStrategyParam );

		_currentExpProcInfo.setDistributionContent( distribution );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, 
				  "_retryCurrent  : "<<_retryCurrent   <<'\n'
				<<"_retryThreshold: "<<_retryThreshold <<'\n'
				<<"rep_protocol   : "<<rep_protocol    <<'\n'
				<<"rep_username   : "<<rep_username    <<'\n'
				<<"rep_password   : "<<rep_password    <<'\n'
				<<"rep_hostname   : "<<rep_hostname    <<'\n'
				<<"rep_remotepath : "<<rep_remotepath);
	} catch (exception& ex) {
		ostringstream os;
		os << "Setup failed: " << ex.what() << endl;
		ACS_THROW(exPackageProcessorCriticalException(ex, ex.what())) ;
	}

}

_ACS_END_NAMESPACE
