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
	Revision 1.14  2017/02/21 18:09:12  nicvac
	S3EVOL-13: Datapackage serialization.
	
	Revision 1.13  2017/02/02 10:19:58  davide.tiriticco
	S2PDGS-1655: Library aligned with packageId type update from string to dbPersistent::IdType
	
	Revision 1.12  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.11  2016/08/03 14:57:06  damdec
	CSGACS-94: handling of clobs changed.
	
	Revision 1.10  2016/01/29 11:17:38  marpas
	typos fixed
	
	Revision 1.9  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.8  2013/11/06 13:41:13  nicvac
	Used macro namespace
	
	Revision 1.7  2013/10/03 15:32:12  nicvac
	acs sdk warnings processed.
	
	Revision 1.6  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.5  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.4  2013/03/21 14:44:28  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.3  2013/03/18 10:24:14  nicvac
	ADC-83: WaitingAcknowledge Step implemented. Test fixing.
	
	Revision 1.2  2013/03/15 16:25:35  nicvac
	ADC-83 (WaitingAcknowledge Step implemented)
	ConnectionPool
	
	Revision 1.1  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
        
*/ 

#include <PackageProcessorCheck.h>

#include <ExporterServiceResolver.h>

#include <PackageTaskTableManager.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>


#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#include <File.h>

#include <Filterables.h>
#include <dbSmartPersistent.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)
using namespace dataprovider;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageProcessorCheck)


PackageProcessorCheck::PackageProcessorCheck():
	PackageProcessor()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);

	//Load from configuration the processing steps to be performed on the data package.
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
	conf.getArray("PackageProcessing.List_of_CheckingSteps.ProcessingStep", _processingSteps);
}

PackageProcessorCheck::~PackageProcessorCheck() throw() {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}

void PackageProcessorCheck::init( dbPersistent::IdType const& dataPackageId ) {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	//Read ExpProcInfo from DB and create DataPackage from
	// dumped DataPackage. It was dumped previously by PackageProcessorFmtDist
	dbSet set;
	try
	{
		dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
		dbConnection& conn = pw;

		dbSmartPersistent disq ( "t_distributionqueue", conn);
        string expProcInfo ;
		if (disq.load( conn, dataPackageId) && (disq.isSet ("delserviceparam", conn) ) )
		{
			// tasktable found
			// download the blob data field

			string dwnName = File::tempname("/tmp/delServiceParamLoad", ".clob"); //Temp filename
			FileGuard dwnNameGuard (dwnName);
			disq.downloadFile("delserviceparam", dwnName, conn) ;
			{
				std::ifstream ifs( dwnName.c_str() );
				std::stringstream buffer;
				buffer << ifs.rdbuf();
				expProcInfo = buffer.str();
			}

		}

		//Restore the ExpProcInfo created at PackageProcessorFmtDist time
		istringstream iss; iss.str( expProcInfo );
		_currentExpProcInfo.load( iss );

		//Restore the DataPackage from current ExpProcInfo (first created at PackageProcessorFmtDist time)

		_dataPackageSptr = boost::shared_ptr<DataPackage>(new DataPackage);
		*_dataPackageSptr = _currentExpProcInfo.getDatapackageContent();
	
	}
	catch (exception& ex) {
		ACS_THROW(exPackageProcessorCheckCriticalException(ex, "Cannot initialize PackageProcessorCheck"));
	}
    
}

void PackageProcessorCheck::setUpWorkingDir() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	_workingDir = _currentExpProcInfo.getWorkingDir();

	if ( ! File::exists(_workingDir) ) {
		ACS_THROW( exPackageProcessorCheckCriticalException("Working dir ["+_workingDir+"] does not exist anymore. Manually removed?") );
	}
}

void PackageProcessorCheck::setUp() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.");

	dbPersistent::IdType dpid = _dataPackageSptr->getPackageId();

	//Query the DB retrieving repository and Task table info

	dbSet set;
	string sqlStr("");
	try
	{
		dbConnectionWrapper pw( *Application::instance()->getDefaultConnectionPool() );
		dbConnection& conn = pw;
        const string &dbKey = conn.getKey() ;
		try {

			/*/string sqlString = 
				"SELECT \
				disq.retrycounter, disr.connretries, disr.id_distributionrule, \
				ttcheck.id_tasktable \
				FROM \
				t_distributionqueue as disq, \
				t_distributionitems as disit, \
				t_distributionrules as disr, \
				t_tasktables as ttcheck \
				WHERE \
				disq.packageid = "+ _dataPackageSptr->getPackageId() +" AND \
				disq.packageid = disit.packageid AND \
				disit.distributionrule_id = disr.id_distributionrule AND \
				disr.delcheck_tt_id = ttcheck.id_tasktable \
				FETCH FIRST 1 ROW ONLY"
				;
			//*/

			SQLString sqlString(dbKey, SQLString::SELECT );
			sqlString.setLimit(1);

			sqlString.addSelectList("disq.retrycounter");
			sqlString.addSelectList("disr.connretries" ); 	
			sqlString.addSelectList("disr.id_distributionrule");
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				sqlString.addSelectList("tt"+ *it +".id_tasktable ");
			}

			sqlString.addTableList("t_distributionqueue", "disq"   ); sqlString.addTableList("t_distributionitems", "disit"  );
			sqlString.addTableList("t_distributionrules", "disr"   ); 
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				sqlString.addTableList("t_tasktables", "tt"+ *it);
			}

			dbQueryParameters pp(dbKey);
			pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, db::toSQL(dbKey, _dataPackageSptr->getPackageId()) ));
			pp.addParameter( dbParam(dbKey, "disq" ,"packageid"          , dbParam::EQUAL_TO, "disit.packageid"                        ));
			pp.addParameter( dbParam(dbKey, "disit","distributionrule_id", dbParam::EQUAL_TO, "disr.id_distributionrule"               ));
			for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
				pp.addParameter( dbParam(dbKey, "disr", *it, dbParam::EQUAL_TO, "tt"+ *it +".id_tasktable"));
			}

			sqlString.addANDWhereParameter( pp );

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL());

			sqlStr = sqlString.getSQL();

			dbQuery query ( conn, sqlString, "GetElementInDistQueueToProcess");
        	query.execForSet ( set );
		} catch (exception& ex) {
			ACS_THROW(exPackageProcessorCriticalException(ex, "Cannot retrieve Task Table info from DB, starting fro package id " + dpid)) ;
		}

		//Expected just one row as result
		ACS_COND_THROW( 
			set.size()!=1, 
			exPackageProcessorCriticalException( "Retrieving Task Table info, expected exactly one row from query: "+sqlStr) );


		//Extract from the result the Repository and Task Table info
		dbRow row(dbKey);
		set.getRow( 1, row );

		bool isNull(false); //If Null on DB, the attribute is left with its original value
		int i(0);
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

		for (vector<string>::iterator it=_processingSteps.begin(); it!=_processingSteps.end(); ++it) {
			int ttId(-1); row.column(i++, ttId, &isNull);
			_ttMap[ *it ] = PackageTaskTableManager::instance()->getTTFilename( ttId );
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, 
				  "_retryCurrent  : "<<_retryCurrent   <<'\n'
				<<"_retryThreshold: "<<_retryThreshold) ;

	} catch (exception& ex) {
		ostringstream os;
		os << "Setup failed: " << ex.what() << endl;
		ACS_THROW(exPackageProcessorCriticalException(ex, os.str())) ;
	}
}

_ACS_END_NAMESPACE
