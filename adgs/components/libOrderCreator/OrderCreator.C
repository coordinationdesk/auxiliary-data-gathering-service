// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia, S.p.A.  --  DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia, S.p.A. ;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, S.p.A.

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$

*/

#include <OrderCreator.h>
#include <OGDBSimulationSingleton.h>
#include <OrbitPropagatorSingleton.h>
#include <OrbitPropagator.h>
#include <TemporaryOrder.h>
#include <RelOrbitInfo.h>
#include <DateTime.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <Application.h>
#include <Filterables.h>
#include <exFMCFilterables.h>
#include <algorithm>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>

#ifdef NRT_TAI_UTC_CONVERSION
	#include <ExplLibWrapSingleton.h>
#endif
#ifdef __linux__
  #if (__GNUC__ < 3)
	#include <algorithm>
  #endif
#endif


_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;
using namespace mc ;


map<OrderCreator::SummaryType, std::vector<dbPersistent::IdType> > OrderCreator::_summary ;
set<dbPersistent::IdType>	OrderCreator::_tmpOrderIdWithProblems;



ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderCreator) 


OrderCreator::OrderCreator(const string &type, const string &algo, unsigned int satId) :
	_type(type),
	_algo(algo),
	_satelliteId(satId),
	_creationTime(DateTime(false)), 
#ifdef GOCE
	_anxInfo(SatelliteName::instance(getConnKeyFromApp())->name(_satelliteId)),
#else
	_actualOrbitStartValidity(-1),
	_actualOrbitStopValidity(-1),
#endif
    _allowedQualified(),
    _discardedQualified()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

OrderCreator::~OrderCreator() throw()
{
}

bool OrderCreator::simulation()  const 
{
	return OGDBSimulationSingleton::instance()->simulation() ;
}

string OrderCreator::getConnKeyFromApp() const // PRQA S 4020 
{
    try {
        if (Application::exists()) {
            dbConnectionPool *pool = Application::instance()->getDefaultConnectionPool() ;
            if (pool) {
                return pool->getKey() ;
            }
        }
        return "" ;
    }
    catch(exception &x) {
        ACS_LOG_WARNING("Ignored exception try to get Application Db Connection Key: " << x) ;
        return "" ;
    }
}

const string &OrderCreator::type() const // PRQA S 4120
{
	return _type ;
}

const string &OrderCreator::algo() const // PRQA S 4120
{
	return _algo ;
}

unsigned int OrderCreator::satelliteId(bool *isNull) const 
{
	if (isNull) { *isNull = (_satelliteId == getNullSatId()) ; }
	return _satelliteId ;
}

string OrderCreator::satelliteName() const 
{
	SatelliteName* sat = SatelliteName::instance(getConnKeyFromApp());

	return (_satelliteId == getNullSatId() ) ? getNullSatName() : sat->name(_satelliteId) ; // PRQA S 3000, 3384, 3385
}

template<class T> struct uf_capitalize_to_firstnonalpha { // PRQA S 2171, 2173, 2175
    uf_capitalize_to_firstnonalpha() :
        _counter(0), _firstNonAlpha(0) {}
    T operator()(T x) {
		_firstNonAlpha+=isalpha(x) ? 0 : 1 ;
		if (!_firstNonAlpha) {
        	return _counter++  ? tolower(x) : toupper(x);
		}
		return x ;
    }
private:
    int _counter;
	int _firstNonAlpha ;
};

const std::string &OrderCreator::capitalize_to_firstnonalpha(std::string &s) 
{
    transform(s.begin(), s.end(), s.begin(), uf_capitalize_to_firstnonalpha<char> ());
    return s; // PRQA S 4028
}


string OrderCreator::getAlgoKeyString() const // PRQA S 4020
{
	if (StringUtils::equalsNoCase(_algo,"LEVEL1")) { return "Level1" ; }
	else if (StringUtils::equalsNoCase(_algo,"LEVEL2")) { return "Level2" ;  }
	else if (StringUtils::equalsNoCase(_algo,"MERGE")) { return "Merge" ; }
	else if (StringUtils::equalsNoCase(_algo,"NOMERGE")) { return "NoMerge" ; }
	else if (StringUtils::equalsNoCase(_algo,"NAMEDNOMERGE")) {	return "NamedNoMerge" ; }
	else if (StringUtils::equalsNoCase(_algo,"EXTERNAL")) { return "External" ; }
	else if (StringUtils::equalsNoCase(_algo,"L1ANX2ANX")) { return "Level1Anx2Anx" ; }
	else if (StringUtils::equalsNoCase(_algo,"L2ANX2ANX")) { return "Level2Anx2Anx" ; }
	else if (StringUtils::equalsNoCase(_algo,"ANX2ANX")) { return "Anx2Anx" ; }
	else if (StringUtils::equalsNoCase(_algo,"GROUPEDCOLLECTION")) { return "GroupedCollection" ; }
	else if (StringUtils::equalsNoCase(_algo,"TIMECUT")) { return "TimeCut" ; }
	else if (StringUtils::equalsNoCase(_algo,"MERGEBYTIMERANGE")) { return "MergeByTimeRange" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPNOMERGE")) { return "CDPNoMerge" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPS1MERGESEGMENTS")) { return "CDPS1MergeSegments" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPPUGFRAMES")) { return "CDPPugFrames" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPPUGANX2ANX")) { return "CDPPugAnx2Anx" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPPUGPARTIALORBIT")) { return "CDPPugPartialOrbit" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPS1MERGESLICES")) { return "CDPS1MergeSlices" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPMERGESLICESGEO")) { return "CDPMergeSlicesGeo" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPMERGESLICESOCEAN")) { return "CDPMergeSlicesOcean" ; }
	else if (StringUtils::equalsNoCase(_algo,"CDPMERGESLICESCUSTOM")) { return "CDPMergeSlicesCustom" ; }
	else { return "" ; }
}


string OrderCreator::getTypeName() const // PRQA S 4020
{
	if (StringUtils::equalsNoCase(_type, "OFFL")) {	return "Offline" ; }
	else if (StringUtils::equalsNoCase(_type, "RPRO")) { return "RPRO" ; }
	else if (StringUtils::equalsNoCase(_type, "External")) { return "External" ; }
	else if (StringUtils::equalsNoCase(_type, "NRT")) {	return "NRT" ; }
	else if (StringUtils::equalsNoCase(_type, "CONS")) { return "CONS" ; }
	else if (StringUtils::equalsNoCase(_type, "LTA_")) { return "LTA_" ; }
    else { return _type ; }
}

string OrderCreator::buildKey(const string &keyEnd, const string &subparam) const
{
	string typeName = getTypeName() ;
	
	SatelliteName* sat = SatelliteName::instance(getConnKeyFromApp());

	string satName = (_satelliteId == getNullSatId() ) ? getNullSatName() : sat->name(_satelliteId) ; // PRQA S 3000, 3384, 3385
#warning default satellite is CRYOSAT key Cryosat
	string satKeyString = "Cryosat" ;
	satKeyString = (_satelliteId == getNullSatId()) ? satName : capitalize_to_firstnonalpha(satName) ; // PRQA S 3380, 3385

	string algoKeyString = getAlgoKeyString() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, 
               "typeName: " << typeName 
            << " satName: " << satName 
            << " satKeyString: " << satKeyString
            << " algoKeyString: " << algoKeyString) ;

	
	string key = "OrderGeneration." ;
	key += typeName + "." ;
	key += satKeyString + "." ;
	key += algoKeyString + "." ;
	if (not subparam.empty()) { key += subparam + "." ; }
	key += keyEnd ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"(" << keyEnd << "," << subparam << ") -> " << key) ;
	
	return key ;
}


void OrderCreator::getConf(const string &keyEnd, string &key, bool &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void OrderCreator::getConf(const string &keyEnd, string &key, int &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void OrderCreator::getConf(const string &keyEnd, string &key, double &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void OrderCreator::getConf(const string &keyEnd, string &key, string &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}


double OrderCreator::getMaxMergeDistance(const string &keyalt, const string &subparam) const // PRQA S 4020, 4214
{
	string key ;
	string keyaltEnd ;
	double value = 0;
	try {
		keyaltEnd = "MaxL0MergeDistance" ;
		getConf("MaxMergeDistance", key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(keyaltEnd, subparam)) ;
	}
	
	try {
		keyaltEnd = "MaxL1MergeDistance" ;
		getConf("MaxL0MergeDistance", key, value,subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(keyaltEnd, subparam)) ;
	}

	try {
		keyaltEnd = keyalt ;
		getConf("MaxL1MergeDistance", key, value,subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (keyaltEnd.empty()) {
			throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(keyaltEnd, subparam)) ;
	}
	
	
	try {
		getConf(keyaltEnd, key, value,subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getMaxMergeDistance(keyalt) ;
		}
		else {
			throw ;
        }
	}

}


double OrderCreator::getMaxWaitingTime(const string &keyalt, const string &subparam) const // PRQA S 4020, 4214
{
	string key ;
	string keyaltEnd ;
	double value = 0 ;
	try {
		keyaltEnd = keyalt ;
		getConf("TemporaryOrdersMaxWaitingTime", key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (keyaltEnd.empty()) {
            throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(keyaltEnd, subparam)) ;
	}
		
	try {
		getConf(keyaltEnd, key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getMaxWaitingTime(keyalt) ;
		}
		else {
			throw ;
        }
	}

}

double OrderCreator::getMaxPendingTime(const string &keyalt, const string &subparam) const // PRQA S 4020, 4214
{
	string key ;
	string keyaltEnd ;
	double value = 0 ;
	try {
		keyaltEnd = keyalt ;
		getConf("TemporaryOrdersMaxPendingTime", key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (keyaltEnd.empty()) {
            throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(keyaltEnd, subparam)) ;
	}
		
	try {
		getConf(keyaltEnd, key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getMaxPendingTime(keyalt) ;
		}
		else {
			throw ;
        }
	}

}

double OrderCreator::getOrderLength(const string &keyalt, const string &subparam) const // PRQA S 4020, 4214
{
	string key ;
	string altKey ;
	double value = 0 ;
	try {
		altKey = keyalt ;
		getConf("OrderLength", key, value, subparam) ;
		return value ;
	}
	catch (rsResourceSet::NotFoundException &e) {
		if (altKey.empty()) {
			throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key
		    << " trying with " << buildKey(altKey, subparam)) ;
	}

	try {
		getConf(altKey, key, value, subparam) ;
		return value ;
	}
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getOrderLength(keyalt) ;
		}
		else {
			throw ;
        }
	}

}


double OrderCreator::getTimeWindow(const string &keyalt, const string &subparam) const  // PRQA S 4020, 4214
{
	string key ;
	string altKey ;
	double value = 0 ;
	try {
		altKey = keyalt ;
		getConf("TimeWindow", key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (altKey.empty()) {
			throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(altKey, subparam)) ;
	}
		
	try {
		getConf(altKey, key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getTimeWindow(keyalt) ;
		}
		else {
			throw ;
        }
	}

}



string OrderCreator::getTempDownloadPath(const string &keyalt) const // PRQA S 4020, 4214
{
	string key ;
	string altKey ;
	string value ;
	try {
		altKey = keyalt ;
		getConf("TemporaryDownloadPath", key, value, "") ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (altKey.empty()) {
			throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(altKey, "")) ;
	}
		
	getConf(altKey, key, value, "") ;
	return value ;

}


string OrderCreator::getDistributionFormat(const string &keyalt) const  // PRQA S 4020, 4214
{
	string key ;
	string altKey ;
	string value ;
	try {
		altKey = keyalt ;
		getConf("DistributionFormat", key, value, "") ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
		if (altKey.empty()) {
			throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(altKey, "")) ;
	}
		
	getConf(altKey, key, value, "") ;
	return value ;

}



string OrderCreator::getOrbitMode(const string &keyalt, const string &subparam) const  // PRQA S 4020, 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "keyalt: " << keyalt << " subparam:" << subparam) ;
	string key ;
	string altKey ;
	string value ;
	try {
		altKey = keyalt ;
		getConf("OrbitMode", key, value, subparam) ;
		return value ;
	} 
	catch (rsResourceSet::NotFoundException &e) {
        if (altKey.empty()) {
            throw ;
        }
		ACS_LOG_WARNING("Cannot find configuration value for " << key 
		    << " trying with " << buildKey(altKey, subparam)) ;
	}
	
	try {	
		getConf(altKey, key, value, subparam) ;
		return value ;
	}
	catch (rsResourceSet::NotFoundException &e) {
		if (not subparam.empty()) {
			// here if a subparam was given and no value foud for it, so try with global (subparam set to empty string)
			return getOrbitMode(keyalt) ;
		}
		else {
			ACS_LOG_WARNING("Cannot find configuration value for " << key 
		   		<< " Default has been set to \"First\"") ;
			return "First" ;
		}
	}

}



string OrderCreator::getGeneralConf(const string &subparam) const // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "subparam:" << subparam) ;
	string key ;
	string value ;
	getConf(subparam, key, value, "") ;
	return value ;
}








void OrderCreator::initOrbitPropagator(const DateTime &dstart, const DateTime &dstop) 
{
#ifdef GOCE
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "GOCE: no esplicit initialization") ;
		
#else
	double start = DateTime::MJD2000(dstart) ;  // PRQA S 3012, 3050, 3081 2
	double stop = DateTime::MJD2000(dstop) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[start,stop] [" << setprecision(17) << start << "," << stop << "]") ; // PRQA S 4400
	OrbitPropagator *propagator = OrbitPropagatorSingleton::Instance()->getPropagator();
	
	SatelliteName* sat = SatelliteName::instance(getConnKeyFromApp());
	ACS_COND_THROW(_satelliteId == getNullSatId(),TemporaryOrder::OrderException("Cannot initialize orbit propagatro with null satellite")) ; // PRQA S 3081
	// null sat checked above.
	string satName = sat->name(_satelliteId) ; // PRQA S 3000
	
	if(StringUtils::equalsNoCase(satName,"CRYOSAT"))
	{
		propagator->setSatelliteID(PropagFormat::OP_SAT_CRYOSAT);
		propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, start, stop, PropagFormat::OP_PROPAG_MODEL_MEAN_KEPL, PropagFormat::OP_SEL_FILE);
 		propagator->getValidationInterval(_actualOrbitStartValidity, _actualOrbitStopValidity);
	}
	else if(StringUtils::equalsNoCase(satName,"GOCE"))
	{
		propagator->setSatelliteID(PropagFormat::OP_SAT_GOCE);
		propagator->initPropagFromFile(PropagFormat::OP_TIMEMOD_FOS_PREDICTED, start, stop);
 		propagator->getValidationInterval(_actualOrbitStartValidity, _actualOrbitStopValidity);
	}
    else {
        // nothing   
    }
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "library initialized for interval[" 
            << setprecision(17) << _actualOrbitStartValidity << "," << _actualOrbitStopValidity << "]") ; // PRQA S 4400 
#endif
}

pair <long int, long int> OrderCreator::absOrbitFromTime(const DateTime &dstart, const DateTime &dstop) 
{
#ifdef GOCE
#warning GOCE will initialize the CFIs with a default -86400 secs on start
	double startTimeInitExtent = -86400.0 ; // default to 1 day 
	string key ;
	try {
		getConf("StartTimeInitExtent", key, startTimeInitExtent, "") ;
	}
	catch (rsResourceSet::NotFoundException &e) {
		ACS_LOG_WARNING("startTimeInitExtent was not found: exception was " << e << " defaulted to " << startTimeInitExtent << "[sec] ") ;
	}
	double convStopTime = (DateTime::MJD2000(dstop)).jd() ;
	double convStartTime = (DateTime::MJD2000(dstart)).jd() ;
	vector<double> anxs ;
	vector<int> orbits ;
	
	// now the first call with exteded interval - should initialize correctly
	_anxInfo.get(convStartTime+startTimeInitExtent/86400.0, convStopTime, anxs, orbits); // note that the extent is in seconds
	
	// and then the real one ... 
	anxs.clear() ;
	orbits.clear() ;
	_anxInfo.get(convStartTime, convStopTime, anxs, orbits);
	if (orbits.size() == 0) { // no orbit returned 
		ostringstream os ; 
		os << "Got 0 orbits using " << dstart.YYYYMMDDhhmmss() << " to " << dstop.YYYYMMDDhhmmss() << " as interval" ;
		ACS_THROW(exBoundsException(os.str())) ;
	}
	int startOrbit = orbits.front() ;
	int stopOrbit = orbits.back() ;
#else
	{
		double convStopTime = (DateTime::MJD2000(dstop)).jd() ; // PRQA S 3012, 3050, 3081 2 
		double convStartTime = (DateTime::MJD2000(dstart)).jd() ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "[start,stop] [" << setprecision(17) << convStartTime << "," <<  convStopTime << "]") ; // PRQA S 4400
		if ( (convStopTime  <= _actualOrbitStartValidity) || (convStopTime  >= _actualOrbitStopValidity) || 
			 (convStartTime <= _actualOrbitStartValidity) || (convStartTime >= _actualOrbitStopValidity)  ) { 
			
            initOrbitPropagator(dstart,dstop) ;
        }
	}
		
	AbsOrbitInfo absOrbInfo;
	OrbitPropagator *propagator = OrbitPropagatorSingleton::Instance()->getPropagator();

	//start orbit
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "computing start orbit with " << setprecision(17) << DateTime::MJD2000(dstart).jd()) ; // PRQA S 3050, 3081, 4400
		ProcessingTime inTime((DateTime::MJD2000(dstart)).jd()); // PRQA S 3050, 3081
		inTime.setRef(TimeFormat::TC_TIME_UTC);
		inTime.setFormat(TimeFormat::TC_PROC);
		propagator->timeToAbsOrbit(inTime, absOrbInfo);
	}
	long int startOrbit = absOrbInfo.absOrbit() ;
	
	// stop orbit
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "computing start orbit with " << setprecision(17) << DateTime::MJD2000(dstart).jd()) ; // PRQA S 3050, 3081, 4400
		ProcessingTime inTime((DateTime::MJD2000(dstop)).jd()); // PRQA S 3050, 3081
		inTime.setRef(TimeFormat::TC_TIME_UTC);
		inTime.setFormat(TimeFormat::TC_PROC);
		propagator->timeToAbsOrbit(inTime, absOrbInfo);
	}
	long int stopOrbit = absOrbInfo.absOrbit() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "orbits [first,last] [" << startOrbit << "," <<  stopOrbit << "]") ;
#endif
	return pair<long int, long int> (startOrbit,stopOrbit) ;
}

DateTime OrderCreator::getAnxFromOrbit(long int orbit) 
{
#ifdef GOCE
	double time = _anxInfo.anx(orbit) ;
#else
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "orbit: " << orbit) ;
	OrbitPropagator *propagator = OrbitPropagatorSingleton::Instance()->getPropagator();
	RelOrbitInfo oInfo ;
	propagator->relOrbitFromAbs(orbit, oInfo) ;	
	std::vector<double> ei = oInfo.extraInfo() ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "anx mjd2000: " << ei[PropagFormat::OP_ORBIT_INFO_EXTRA_UTC_ANX]) ;
	double time = ei[PropagFormat::OP_ORBIT_INFO_EXTRA_UTC_ANX] ;

#endif
	DateTime::MJD2000 anx(time) ;
	return anx ;
} 

void OrderCreator::insertTriggeringFiles(
	const vector<TemporaryOrder> &os, 
	dbPersistent::IdType orderid,
	dbConnection &conn) const
{
    size_t osn = os.size() ;
	for (size_t i=0; i < osn; ++i) {
		insertTriggeringFile(os[i], orderid, conn) ;
    }
}

void OrderCreator::insertTriggeringFile(
	const TemporaryOrder &to, 
	dbPersistent::IdType orderid,
	dbConnection &conn) const
{
	insertTriggeringFile(to.fileName(), to.fileVersion(), orderid, conn) ;
}

vector<TriggeringFile >
    OrderCreator::getTriggeringFiles(const TemporaryOrder &to) const 
{
    vector<TriggeringFile > tf ;
    tf.push_back(TriggeringFile(to)) ; // PRQA S 3081
    return tf ;
}

class tf_inserter {
public:
    explicit tf_inserter(vector<TriggeringFile > &v) : v_(v) {} // PRQA S 2528
    void operator()(const TemporaryOrder &to) 
    {
        v_.push_back(TriggeringFile(to)) ; // PRQA S 3081
    }
private:
    tf_inserter() ;
    vector<TriggeringFile > &v_ ;
} ;

vector<TriggeringFile >
    OrderCreator::getTriggeringFiles(const vector<TemporaryOrder> &to) const 
{
    vector<TriggeringFile > tf ;
    for_each(to.begin(), to.end(), tf_inserter(tf) ) ; // PRQA S 3081
    return tf ;
}

void OrderCreator::insertTriggeringFile(
	const std::string &filename, 
	const std::string &version, 
	dbPersistent::IdType orderid,
	dbConnection &conn) const
{
    const string &dbKey = conn.getKey() ;
	SQLString ins(dbKey, SQLString::INSERT ) ;
    ins.addTableList("t_ordtriggeringfiles");   // PRQA S 3050 4
    ins.addValuesList("filename", db::toSQL(dbKey, filename) ) ;
    ins.addValuesList("fileversion", db::toSQL(dbKey, version) ) ;
    ins.addValuesList("orderid", db::toSQL(dbKey, orderid) ) ;

	dbQuery query(conn, ins, "OrdersTriggeringInsert") ;
	query.execForStatus() ;
}

void OrderCreator::appendSummary(SummaryType t, dbPersistent::IdType id) 
{
	_summary[t].push_back(id) ;
}

void OrderCreator::emitSummary() 
{
	dbConnectionWrapper cw(db::ConnPools::instance()->getPool(""));
	dbConnection &conn = cw;

	const string &dbKey = conn.getKey();
	if (!_summary[TemporaryOrderIdHasProblem].empty()) {
		ostringstream war ;
		war << "Some temporary order got problem, see " << Application::instance()->applicationName() << " log file(s) for detail. Temporary id(s) follow: " ;
		size_t tohps = _summary[TemporaryOrderIdHasProblem].size() ;
        for (size_t i=0; i<tohps; ++i) {
			if (i != 0) { war << "," ; }
			war << _summary[TemporaryOrderIdHasProblem][i] ;
			// CRYO2PDS-96: added messages for Splunk
			if (_tmpOrderIdWithProblems.find(_summary[TemporaryOrderIdHasProblem][i]) == _tmpOrderIdWithProblems.end())
			{
				_tmpOrderIdWithProblems.insert(_summary[TemporaryOrderIdHasProblem][i]); 

				// notify problems to splunk just the first time
				try
				{
					SQLString sqlString ( dbKey, SQLString::SELECT ) ;
					sqlString.addTableList("t_temporaryorders") ;
					sqlString.addSelectList( "processorname" );
					{
						dbQueryParameters qp(dbKey) ;
						qp.addParameter(dbParam(dbKey, "t_temporaryorders","id" , dbParam::EQUAL_TO, db::toSQL(dbKey,_summary[TemporaryOrderIdHasProblem][i]) ) ) ; // PRQA S 3050, 3081
						sqlString.addANDWhereParameter(qp) ;
					}
					dbQuery q(conn, sqlString, "SEL t_temporaryorders") ;
					dbSet set ;
					q.execForSet(set) ;
					size_t setSize = set.size();
					if( setSize != 1) {
						ostringstream exMsg;
						exMsg << "Unxpected result for query: temporary order id " << _summary[TemporaryOrderIdHasProblem][i];
						ACS_THROW( exCriticalException(exMsg.str()) ); // PRQA S 3081
					}
					string procName;
					dbRow row(dbKey) ;
					set.getNextRow(row) ;
					row.column(0, procName) ;
					ostringstream os;
					os << "The order id for processor name " << procName << " has not been successfully emitted";
					ACS_LOG_MC_INFO(  mc_param("processorname", procName ) <<
						 mc_param("message",  os.str() ) <<
						 mc_metric(Metric_FAIL) << MCCode ("DPR") << MCFunction ("Processing Chain")) ;
				}
				catch (exException & e) 
				{
					ACS_LOG_WARNING("Exception in processorname retrieval from Db");
					ACS_LOG_NOTIFY_EX(e);
				}
			}
		
		}
		ACS_LOG_WARNING(war.str()) ;

	}
	
	if (!_summary[OrderIdSuccesfullyCreated].empty()) {
		ostringstream msg ;
		msg << "The following orders has been succesfully created (see " << Application::instance()->applicationName() << " log file(s) for detail): " ;
		size_t oscs = _summary[OrderIdSuccesfullyCreated].size() ;
        for (size_t i=0; i<oscs; ++i) {
			if (i != 0) { msg << "," ; }
			msg << _summary[OrderIdSuccesfullyCreated][i] ;
			// CRYO2PDS-96: added messages for Splunk
			try
			{
				SQLString sqlString ( dbKey, SQLString::SELECT ) ;
				sqlString.addTableList("t_ordersqueue") ;
				sqlString.addSelectList( "processorname" );
				{
					dbQueryParameters qp(dbKey) ;
					qp.addParameter(dbParam(dbKey, "t_ordersqueue","orderid" , dbParam::EQUAL_TO, db::toSQL(dbKey,_summary[OrderIdSuccesfullyCreated][i]) ) ) ; // PRQA S 3050, 3081
					sqlString.addANDWhereParameter(qp) ;
				}
				dbQuery q(conn, sqlString, "SEL t_ordersqueue") ;
				dbSet set ;
				q.execForSet(set) ;
				size_t setSize = set.size();
				if( setSize != 1) {
					ostringstream exMsg;
					exMsg << "Unxpected result for query: order id " << _summary[OrderIdSuccesfullyCreated][i];
					ACS_THROW( exCriticalException(exMsg.str()) ); // PRQA S 3081
				}
				string procName;
				dbRow row(dbKey) ;
				set.getNextRow(row) ;
				row.column(0, procName) ;
				ostringstream os;
				os << "The order id " << _summary[OrderIdSuccesfullyCreated][i] << " has just been successfully emitted";
				ACS_LOG_MC_INFO( mc_param("jobid", _summary[OrderIdSuccesfullyCreated][i] ) <<
						 mc_param("processorname", procName ) <<
						 mc_param("message",  os.str() ) <<
						 mc_metric(Metric_LOG) << 
						 MCCode ("DPR") << 
						 MCFunction ("Processing Chain")) ;
			}
			catch (exException & e)
			{
				ACS_LOG_WARNING("Exception in processorname retrieval from Db");
				ACS_LOG_NOTIFY_EX(e);
			}
		}
		
		ACS_LOG_INFO(msg.str()) ;
	}

	
	_summary.clear() ;
}


bool OrderCreator::dupOrNotQualified(const TemporaryOrder &ord) const  // PRQA S 4020
{
 	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checking " << ord.id()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ord.isDup() " << boolalpha << ord.isDup()) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ord.qualified() " << ord.qualified()) ;
    if (ord.isDup()) { 
        return true ; 
    }
    
    // if the actual qualified flag is not found between allowed ones
    // then is not qualified by definition
    return find(_allowedQualified.begin(), _allowedQualified.end(), ord.qualified()) == _allowedQualified.end() ;
}

bool OrderCreator::isQualifiedAllowed(const TemporaryOrder& ord) const {
	return find(_allowedQualified.begin(), _allowedQualified.end(), ord.qualified()) != _allowedQualified.end(); 
}

bool OrderCreator::isQualifiedDiscarded(const TemporaryOrder& ord) const {
	return find(_discardedQualified.begin(), _discardedQualified.end(), ord.qualified()) != _discardedQualified.end(); 
}


#ifdef NRT_TAI_UTC_CONVERSION
void OrderCreator::initTimeConverter(const string &start, const string &stop) const 
{
	// time converter library initialization
	AsciiTime ascStartPeriod(start) ;
	ascStartPeriod.setRef(TimeFormat::TC_TIME_UTC) ;
	ascStartPeriod.setFormat(TimeFormat::TC_ASCII_CCSDSA_REF) ;
	ProcessingTime procStartPeriod ;
	procStartPeriod.setRef(TimeFormat::TC_TIME_UTC) ;
	procStartPeriod.setFormat(TimeFormat::TC_PROC) ;
	TimeConverter *TimeConv = ExplLibWrapSingleton::Instance()->getConverter() ;
	TimeConv->convert(ascStartPeriod,procStartPeriod) ;
	double dStartPeriodMJD2000 = procStartPeriod.getValue() ;
	AsciiTime ascStopPeriod(stop) ;
	ascStopPeriod.setRef(TimeFormat::TC_TIME_UTC) ;
	ascStopPeriod.setFormat(TimeFormat::TC_ASCII_CCSDSA_REF) ;
	ProcessingTime procStopPeriod ;
	procStopPeriod.setRef(TimeFormat::TC_TIME_UTC) ;
	procStopPeriod.setFormat(TimeFormat::TC_PROC) ;
	TimeConv->convert(ascStopPeriod,procStopPeriod) ;
	double dStopPeriodMJD2000 = procStopPeriod.getValue() ;
	TimeConv->setSatelliteID(TimeFormat::TC_SAT_CRYOSAT);
	// HAI VISTO MAI ..
	TimeConv->initFromFile(TimeFormat::TC_TIMEMOD_FOS_PREDICTED, dStartPeriodMJD2000, dStopPeriodMJD2000) ;
}


DateTime OrderCreator::tai2utc(const string &tai) const 
{
	AsciiTime ascTai(tai) ;
	ascTai.setRef(TimeFormat::TC_TIME_TAI) ;
	ascTai.setFormat(TimeFormat::TC_ASCII_CCSDSA_REF_MICROSEC) ;
	ProcessingTime procUtc ;
	procUtc.setRef(TimeFormat::TC_TIME_UTC) ;
	procUtc.setFormat(TimeFormat::TC_PROC) ;
	TimeConverter *TimeConv = ExplLibWrapSingleton::Instance()->getConverter() ;
	TimeConv->convert(ascTai,procUtc) ;
	DateTime::MJD2000 mjd(procUtc.getValue()) ;
	return mjd ;
}


			

#endif

void OrderCreator::loadAllowedQualified(const string &dbkey) 
{
    vector <string> allowedQ ;
    string confkey = buildKey("QualifiedFlags") ;
    ConfigurationSingleton::instance()->get().getArray(confkey, allowedQ) ;// PRQA S 4412
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "got " << allowedQ.size() << " \"QualifiedFlags\"") ;
    if (not allowedQ.empty()) {
        NameIdAssociator<int> qflagAss ("T_QualifiedFlag", "id", "name", dbkey);
        qflagAss.load();
        vector<int> qflags ;
        size_t allowedQ_size = allowedQ.size() ;
        for (size_t i=0 ; i < allowedQ_size; ++i) {
            try {
                qflags.push_back(qflagAss.id (allowedQ[i])) ;
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "T_QualifiedFlag id: " << qflagAss.id (allowedQ[i]) << " added") ;
            }
            catch(exception &x) {
                ACS_LOG_ERROR("Error: " << allowedQ[i] << " is not recognized in the T_QualifiedFlag table, ignored.") ;
            }
            
        }
        if (not qflags.empty()) {
            _allowedQualified = qflags ;
        }
    }
    else {
        ACS_LOG_WARNING("Warning any qualified flag will be treated as not qualified (" << confkey << ")") ;
    }
}

void OrderCreator::loadDiscardedQualified(const string &dbKey) 
{
    vector <string> discardedQ ;
    string confkey = buildKey("DiscardedQualifiedFlags") ;
    ConfigurationSingleton::instance()->get().getArray(confkey, discardedQ) ;// PRQA S 4412
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "got " << discardedQ.size() << " \"DiscardedQualifiedFlags\"") ;
    if (not discardedQ.empty()) {
        NameIdAssociator<int> qflagAss ("T_QualifiedFlag", "id", "name", dbKey);
        qflagAss.load();
        vector<int> qflags ;
        size_t discardedQ_size = discardedQ.size() ;
        for (size_t i=0 ; i < discardedQ_size; ++i) {
            try {
                qflags.push_back(qflagAss.id (discardedQ[i])) ;
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "T_QualifiedFlag id: " << qflagAss.id (discardedQ[i]) << " added") ;
            }
            catch(exception &x) {
                ACS_LOG_ERROR("Error: " << discardedQ[i] << " is not recognized in the T_QualifiedFlag table, ignored.") ;
            }
            
        }
        if (not qflags.empty()) {
            _discardedQualified = qflags ;
        }
    }
    else {
        ACS_LOG_WARNING("Warning any qualified flag will be treated as not qualified (" << confkey << ")") ;
    }
}

_ACS_END_NAMESPACE
