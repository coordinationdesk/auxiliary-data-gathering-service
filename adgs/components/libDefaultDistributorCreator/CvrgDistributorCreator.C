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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.13  2016/08/30 14:40:44  marfav
	CSN-1536
	Fixed pointer management when dealing with geo objects
	
	Revision 2.12  2016/07/13 15:12:46  marfav
	CSGACQ-86 oracle sql code reviewed and fixed
	
	Revision 2.11  2016/04/11 13:37:35  marpas
	adopting new db interfaces
	
	Revision 2.10  2016/03/26 21:36:55  marpas
	using new db I/F
	
	Revision 2.9  2014/07/29 16:02:18  marpas
	qa & gcc warnings fixed
	
	Revision 2.8  2014/02/05 15:39:14  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.7  2013/05/14 18:50:49  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.6  2013/04/12 12:19:57  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.5  2013/04/09 08:22:03  marpas
	NameIdAssociator refactoring
	
	Revision 2.4  2013/02/26 18:30:47  marpas
	new dbSet I/F adopted
	
	Revision 2.3  2013/02/11 09:46:05  marpas
	using  dbConnection::localMaster method
	
	Revision 2.2  2013/02/07 17:52:26  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.1  2013/02/04 15:41:21  marpas
	disseminationpackingalgo_id transferred from temporarydistribution to distributionitems
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.19  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.18  2011/10/05 11:41:43  crivig
	bug CSN-746 fixed ;ing core dump
	
	Revision 1.17  2011/03/31 13:32:02  crivig
	bug in new query
	
	Revision 1.16  2011/03/31 12:44:49  crivig
	oracle geoObject handled: time range read from validitystart and validitystop
	
	Revision 1.15  2010/08/11 09:15:27  marpas
	Oracle11 SQL syntax fixed
	
	Revision 1.14  2010/05/12 14:08:48  marpas
	Postgres/Oracle cast for string onstant in SELECT
	
	Revision 1.13  2010/04/14 13:01:29  enrcar
	EC:: Oracle11 compatibility improved
	
	Revision 1.12  2010/04/08 09:51:09  crivig
	check on filetype added; bug solved in intersect check
	
	Revision 1.11  2010/03/08 15:25:51  enrcar
	EC:: Oracle DB handled
	
	Revision 1.10  2009/06/25 09:19:08  crivig
	Algo string converted to upper case-> no conversion for db queries
	
	Revision 1.9  2009/06/24 09:27:41  marpas
	insideGeo method name changed into isInsideGeo
	
	Revision 1.8  2009/06/23 16:28:10  marpas
	still working
	debug improved
	
	Revision 1.7  2009/06/23 15:42:48  marpas
	debug improved
	
	Revision 1.6  2009/06/23 14:58:07  marpas
	Sensor and Satellite can accept null values
	
	Revision 1.5  2009/06/23 13:34:10  marpas
	if the inventory object has no geographic localization, it will be considered covering the whole world
	
	Revision 1.4  2009/06/22 14:42:29  crivig
	checkdataInfo added when performing intersect/inside methods
	
	Revision 1.3  2009/06/12 08:15:47  crivig
	bug fixed
	
	Revision 1.2  2009/06/09 08:40:06  crivig
	still coding
	
	Revision 1.1  2009/06/05 08:52:42  crivig
	added new distribution algo
	
*/
 
#include <CvrgDistributorCreator.h>
#include <StringUtils.h>
#include <DistributionAlgo.h>
#include <dbConnectionPool.h>
#include <dbSmartPersistent.h>
#include <dbGeoObject.h>
#include <dbGeoObjectGuard.h>
#include <dbGeoTimeRange.h>
#include <dbGeoPntList.h>
#include <dbGeoBox.h>
#include <dbGeoPnt.h>
#include <dbRow.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbPersistentScopeGuard.h>
#include <dbPersistentFactoryChain.h>
#include <DateTime.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <SensorName.h>
#include <SatelliteName.h>
#include <Filterables.h>
#include <File.h>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(CvrgDistributorCreator);

CvrgDistributorCreator::CvrgDistributorCreator(const string &algo, bool total) :
	DistributorCreator(algo),
    _agelimit(3.0), // PRQA S 4400
    _totalCvrg(total)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "algo:\"" << algo << '\"');
	// get from configuration the ageLimit after which files have to be deleted
	string key ;
	try 
	{
		getConf("AgeLimit", key, _agelimit, "") ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
        ACS_LOG_WARNING("no AgeLimit found in configuration, default set to: \""  << _agelimit << " days\"");
	}
}

CvrgDistributorCreator::~CvrgDistributorCreator() throw()
{
}



void CvrgDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred ) const // PRQA S 4020, 5510
{

	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
	const string &dbKey = theConnection.getKey() ;
	const string &dialectName = SQLDialectFactory::dialectName(dbKey);
    int algoid = DistributionAlgo::instance(dbKey)->id(_algo); // PRQA S 3000
	
	// now select all the records from t_temporarydistribution 
	string sqlCommand;
	if(dialectName == "Oracle11" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), CAST ('qualifierFile' as varchar(20) ), t.policy, t.disseminationpackingalgo_id \
 			FROM t_temporarydistribution  t  WHERE t.preselection = ";
	}
	else if(dialectName == "Postgres8") {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), 'qualifierFile'::VARCHAR,t.policy, t.disseminationpackingalgo_id \
 			FROM t_temporarydistribution  t  WHERE t.preselection = ";
	}
	else if( dialectName == "Informix9" ) {
		sqlCommand = "SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.timestamp, \
			(CASE 		\
				WHEN t.cartid IS NULL THEN -1 \
				ELSE t.cartid 		\
			END), 		\
			(CASE 		\
				WHEN t.subreq IS NULL THEN -1 \
				ELSE t.subreq 		\
			END), 'qualifierFile', t.policy, t.disseminationpackingalgo_id \
			FROM t_temporarydistribution  t  WHERE t.preselection = ";
	} else {
		ACS_THROW( exIllegalValueException("Unknown db: "+dialectName ));  // PRQA S 3081
	}
	sqlCommand += db::toSQL(dbKey, true);     // PRQA S 3050, 3081
	sqlCommand += " AND t.algo = ";
	sqlCommand += db::toSQL(dbKey, algoid);     // PRQA S 3050, 3081
    
	vector <DistributorCreator::TemporaryInfo> TmpInfo; 
	queryFromTempDist(sqlCommand,"t_temporarydistribution Cvrg", TmpInfo, theConnection);
	
	
	vector <dbPersistent::IdType> deleteId;
	// Loop on all records in t_temporarydistribution
	// for the current algo
    size_t size = TmpInfo.size() ;
	for (size_t i = 0; i<size; ++i ) 
	{
        if (stop_pred()) { return ; }
		try
		{

			// update the TmpInfo structure
			// according to the records in t_standingdistribution
			vector<DistributorCreator::TemporaryInfo> newTmpInfo;
			updateDistribution(TmpInfo[i], newTmpInfo, theConnection);

			// these temporary orders can be sent in distribution...
			if (newTmpInfo.empty())
			{
				//intersection or inclusion failed ->
				//no distribution will be performed
				deleteId.push_back(TmpInfo[i].id);
                ACS_LOG_WARNING("id = " << TmpInfo[i].id << " not distributed! Intersection /inclusion criteria failed!");
                appendSummary(TemporaryDistributionIdHasProblem,TmpInfo[i].id) ;
			} 
            else {
                sendInDistribution(newTmpInfo, theConnection);
            }
		}
		catch(exException &ex) 
		{
            appendSummary(TemporaryDistributionIdHasProblem,TmpInfo[i].id) ;
			ACS_LOG_NOTIFY_EX(ex) ;
		}
	}
	deleteFromTempDist(deleteId, theConnection);
}

bool CvrgDistributorCreator::intersectGeoObjects(   // PRQA S 4214
    const dbGeoObject * geoInv ,
    std::vector<boost::shared_ptr<dbGeoObject> > & geoObjArr ,
    vector<dbPersistent::IdType> & idArr, 
    int satId,
    int sensorId,
    bool ascFlag,
    const string & fileType,
    dbConnection &theConnection ) const
{	
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

	// if geoInv intersects at least one geoObject in geoObjArr then return true
	bool intersect = false;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "geoInv->timeRange(): " 
                          << geoInv->timeRange().start().asISO8601(6) << ' ' 
                          << geoInv->timeRange().stop().asISO8601(6)) ;
	
	const dbGeoTimeRange trng1 = geoInv->timeRange();
	
	
	// loop on the geObjArr size
    size_t size = geoObjArr.size() ;
	for (size_t i = 0; i < size ; ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "checking " << i << " of " << geoObjArr.size()-1 ); // PRQA S 3084

		try
		{
			// check data info
			intersect = checkDataInfo(idArr[i], satId, sensorId,ascFlag, fileType, theConnection);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dataInfo check is  " << boolalpha << intersect );
			
			if(!intersect) {
                continue;
            }
		
		
			// check for time intersection
			intersect = trng1.intersects(geoObjArr[i]->timeRange(), TimeRange::ClosedInterval, TimeRange::ClosedInterval );
			// if no time intersection then skip to the next geoobject
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "timerange intersection is  " << boolalpha << intersect );
		
			if(!intersect) {
                continue;
            }

			// check for partial/total geographic coverage
			if (_totalCvrg ) { intersect = geoInv->isInsideGeo(*(geoObjArr[i])); }
			else { intersect = geoInv->intersectGeo(*(geoObjArr[i])); }
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "geometric intersection is  " << boolalpha << intersect );
			
			// all checks are correct:  exit loop and return true			
			if (intersect ) {
                break;
            }

		}
		catch(exException &ex) 
		{
			ACS_LOG_NOTIFY_EX(ex) ; 
		}
		
	}
    size = geoObjArr.size() ;
	geoObjArr.clear();
	idArr.clear();
	return intersect;
} 

void CvrgDistributorCreator::updateDistribution(TemporaryInfo & TmpInfo, vector<TemporaryInfo> &newTmpInfoArr, dbConnection &theConnection) const // PRQA S 4020, 4214
{
	
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);


	// download the dbGeoObject from t_inventory 
	dbInventoryObject *s= dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", theConnection.getKey())); // PRQA S 3081, 4412
	dbPersistentScopeGuard sGuard(s) ;
	s->setFileName(TmpInfo.filename) ;
	s->setFileVersion(TmpInfo.fileversion) ;
	if(!s->load(theConnection))
	{
		ACS_LOG_INFO(" Cannot load dbInventoryObject for filename "<< TmpInfo.filename);
		return; 
	}
	bool setFlag = true;
	const dbGeoObject *constgeoInv = s->getGeographicLocalization(&setFlag);
	ACS_SMARTPTR <dbGeoObject> geoInv ;
	vector<boost::shared_ptr < dbGeoObject > > geoArr;
	if (setFlag) 
	{
        geoInv.reset ( constgeoInv->clone() );
    }
	else
	{
		 // assume the whole earth in case there is no geo loc
		geoInv.reset ( new dbGeoBox(dbGeoPnt(-90,-180), dbGeoPnt(90,180)) ); // PRQA S 4400
	}

	{
		geoInv->timeRange(dbGeoTimeRange(s->getValidityStart(),s->getValidityStop())) ;
		int satId = s->getSatelliteId();
		int sensId = s->getSensorId();
		bool ascFlag = s->getAscendingFlag();
		string fileType =  s->getFileType();
        const string &dbKey = theConnection.getKey() ;
		const string &dialectName = SQLDialectFactory::dialectName(dbKey);
		// select all the records from t_standingorders associated to the  TmpInfo distribution policy
		// and order them by cartid
		string sqlCommand;
		if( dialectName == "Informix9" || dialectName == "Postgres8" || dialectName == "Oracle11" ) {
			sqlCommand =   "SELECT id, " ;
			sqlCommand += "(CASE " ;
			sqlCommand += "  WHEN cartsid IS NULL THEN -1 " ;
			sqlCommand += "  ELSE cartsid " ;
			sqlCommand += "END) as cart," ;
			sqlCommand += "geographiclocalization , validitystart, validitystop" ;
 			sqlCommand += " FROM t_standingdistribution WHERE  distpoliciesid =";
			sqlCommand += db::toSQL(dbKey,TmpInfo.policy);  // PRQA S 3050, 3081
			sqlCommand += " ORDER BY cart";
		} else {
			ACS_THROW( exIllegalValueException("Unknown db: "+dialectName )); // PRQA S 3081
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing \"" << sqlCommand << "\"");

		// execute the query 
		SQLString sqlString(dbKey, SQLString::COMMAND) ;	
		sqlString.forceSQLString(sqlCommand) ;
		dbQuery query(theConnection, sqlString, "t_standingdistribution" ) ;
		dbSet set ;
		query.execForSet(set) ;

		size_t numRec = set.size(); // PRQA S 3010
		//loop on all records in t_standingdistribution
		long long cartid = 0 ;
        long long oldcartid = -2 ; // PRQA S 4400 
        dbPersistent::IdType id = 0;
		vector<dbPersistent::IdType> idArr;
		for (size_t j=0; j < numRec ; j++)
		{
 			// retreive id, cartid and geoobject
			dbRow row(dbKey) ;
			set.getNextRow(row) ;
            // PRQA S 4400 L1
			row.column(0,id) ;
			row.column(1,cartid) ;
			string geoObj ;
			row.column(2,geoObj);
            string valStart; 
			row.column(3,valStart);
            string valStop;
			row.column(4,valStop);
            // PRQA L:L1
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "id = " << id <<
				 								"cartid = " << cartid<<
				 								"t_temporarydistribution cartid = " << TmpInfo.cartid<<
												"geoObject = " << geoObj<<
												"validity start = " << valStart <<
				 								"validity stop = " << valStop );
												
			dbGeoObject * geo = dbGeoObject::newDbGeo(geoObj);
			geo->timeRange(dbGeoTimeRange(valStart, valStop)) ;
			// if cartid in t_temporarydistribution is NULL 
			if (TmpInfo.cartid == -1) 
			{
				// take the t_standingdistribution cartid and associate a distribution
				// to each cartid group.
				// Records with the same cartid are associated to 
				// the same distribution
				if ((cartid != oldcartid) && (oldcartid != -2)) // PRQA S 4400
				{
					// check for intersection or inclusion
					bool intersect = intersectGeoObjects(geoInv.get(), geoArr, idArr,satId, sensId, ascFlag, fileType, theConnection);
					if (intersect) 
					{
						// If itersects then add to distribution order
						TemporaryInfo newTmpInfo = TmpInfo;
						newTmpInfo.cartid = oldcartid;
						newTmpInfoArr.push_back(newTmpInfo);	
					}
				} 
				geoArr.push_back(boost::shared_ptr < dbGeoObject >(geo)); // PRQA S 3081
				idArr.push_back(id);
				oldcartid = cartid;
			}
			// if cartid in t_temporarydistribution is not NULL
			// just archive all geoobjects for intersection
			else 
			{
				geoArr.push_back(boost::shared_ptr < dbGeoObject >(geo)); // PRQA S 3081
				idArr.push_back(id);
			}

		}

		if(not geoArr.empty())  
		{
			bool intersect = intersectGeoObjects(geoInv.get(), geoArr, idArr,satId, sensId, ascFlag, fileType, theConnection);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "intersects is: " << boolalpha << intersect );
			if (intersect) 
			{
				// If itersects then add to distribution order
				TemporaryInfo newTmpInfo = TmpInfo;
				if (TmpInfo.cartid == -1) { newTmpInfo.cartid = cartid; }
				newTmpInfoArr.push_back(newTmpInfo);	
			}
		}
	}
}

bool CvrgDistributorCreator::checkDataInfo(dbPersistent::IdType id, // PRQA S 4020, 4214
                                           int SatId,
                                           int SensorId,
                                           bool AscFlag, 
                                           const string & fileType, 
                                           dbConnection & theConnection) const 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	
	// Load the row associated to id value
	dbSmartPersistent sp("t_standingdistribution", theConnection) ;
    
	sp.load(theConnection, id);
	
	// download the blob data field
	string dwnName = File::tempname("/tmp/ord_stnd_",".rs") ; // Temporary filename // PRQA S 3050
	FileGuard dwnNameGuard(dwnName) ;
	sp.downloadFile("data",dwnName, theConnection) ;
	
	// read the resource set data
	// load files list

	rsResourceSet prfRs ;
	{
		ifstream fs(dwnName.c_str()) ;
		fs >> prfRs ; 
		fs.close() ;
	}
	
    const string &dbKey = theConnection.getKey() ;
	// get the satellite Name from resource set
	string dataSatName = getOption(prfRs, "platformName");
	if (!StringUtils::equalsNoCase(dataSatName, "All")) {
		string satName = SatelliteName::instance(dbKey)->name(SatId) ; // PRQA S 3000
		if (!StringUtils::equalsNoCase(dataSatName, satName)) {
			return false;	
        }
	}	


	// get the sensor Name from resource set
	string dataSensName = getOption(prfRs, "instrumentName");
	if (!StringUtils::equalsNoCase(dataSatName, "All")) {
		string SensName = SensorName::instance(dbKey)->name(SensorId) ; // PRQA S 3000
		if (!StringUtils::equalsNoCase(dataSensName, SensName)) {
			return false;	
        }
	}
	


	// get the product FileType
	string productFileType =  getOption(prfRs, "productType");
	if (!StringUtils::equalsNoCase(productFileType, "All")) {
		if (!StringUtils::equalsNoCase(productFileType, fileType))  {
			return false;	
        }
	}
	
	// get the orbit direction from resource set
	string dataOrbitDirection = getOption(prfRs, "orbitDirection");
	// if is all then return true
	if (StringUtils::equalsNoCase(dataOrbitDirection, "All")) {
        return false;	
    }
	// get orbit direction from given AscFlag
	string orbitDirection = "Descending";
	if (AscFlag) { orbitDirection = "Ascending"; }
	if (StringUtils::equalsNoCase(orbitDirection, dataOrbitDirection)) {
		return true;
    }
	return false;

	
}

string CvrgDistributorCreator::getOption(rsResourceSet &plrs, const string &optName) const // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "optName: " << optName );
	string prokey = "processing" ;
	size_t numProcessing = plrs.getArraySize(prokey) ;
	
	for (size_t i=0; i < numProcessing; i++) {
		ostringstream key ;
		key << prokey << "[" << i << "]" ;
		string name ;
		plrs.getValue(key.str()+".optionName",name) ;
		if (StringUtils::equalsNoCase(name, optName)) {
			string OptionValue ;
			plrs.getValue(key.str()+".optionSelectedValues",OptionValue) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "OptionValue: " << OptionValue );
			return OptionValue ;
		}
	}
	
	ACS_THROW(rsResourceSet::NotFoundException("No processing option with name: \""+optName+"\"")) ; // PRQA S 3081
}


_ACS_END_NAMESPACE
