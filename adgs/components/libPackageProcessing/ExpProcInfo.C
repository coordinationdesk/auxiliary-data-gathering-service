/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
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
	Revision 1.17  2016/12/19 18:32:37  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.16  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.15  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.14  2016/03/18 17:35:43  marpas
	adapted to c2evo dev
	
	Revision 1.13  2016/01/29 11:15:16  marpas
	messaging strategy slightly revised
	throw xxx is deprecated: ACS macro use enforced
	useless cast to string removed
	
	Revision 1.12  2013/11/06 13:41:13  nicvac
	Used macro namespace
	
	Revision 1.11  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.10  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.9  2013/03/21 14:44:27  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.8  2013/03/20 17:13:58  nicvac
	minor. load dump signature.
	
	Revision 1.7  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.6  2013/03/11 17:32:16  nicvac
	More than one Circulation id supported
	
	Revision 1.5  2013/02/22 17:38:22  nicvac
	S2PDGS-186: mail notification implemented.
	
	Revision 1.4  2013/02/07 16:03:35  nicvac
	load and dump for serializable objects.
	
	Revision 1.3  2012/12/12 16:17:19  nicvac
	Ref to distribution and datapackage file.
	
	Revision 1.2  2012/11/23 17:35:45  nicvac
	Implementation.
	
	Revision 1.1  2012/11/22 18:06:09  nicvac
	Implementation.
	
        
*/ 

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>


#include <ExpProcInfo.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ExpProcInfo)

#warning NV Refactoring pending: _datapackageFilename and _distributionFilename, along their getter and setter  has to be removed.
#warning NV Refactoring pending: Datapackage and Distribution dumping in working dir to be removed because users of this class must use _datapackageContent and _distributionContent.

ExpProcInfo::ExpProcInfo():
	_datapackageAbsPath(""),
	_workingDir(""),
    _datapackageFilename(""),
    _datapackageContent(),
    _distributionFilename(""),
    _distributionContent(),
	_parametersFilename(""),
	_cacheDownloadRepository(""),
	_cacheAvailableDownloadSpaceSize(0),
	_cacheItemsToRelease(0),
	_circulationIds(),
	_packageNames(),
	_distributionStatus(ExpProcInfoDistributionNONE),
	_notificationListOfFiles(),
	_notificationDeliverySlipFilename(""),
	_notificationContentFilename("")

{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

ExpProcInfo::~ExpProcInfo() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}


string const& ExpProcInfo::getDatapackageAbsPath() const { return _datapackageAbsPath; }

string const& ExpProcInfo::getWorkingDir() const { return _workingDir; }

string const& ExpProcInfo::getDatapackageFilename() const { return _datapackageFilename; }

dataprovider::DataPackage const& ExpProcInfo::getDatapackageContent() const { return _datapackageContent; }

string const& ExpProcInfo::getDistributionFilename() const { return _distributionFilename; }

DistributionSerializable const& ExpProcInfo::getDistributionContent() const { return _distributionContent; }

string const& ExpProcInfo::getParametersFilename() const { return _parametersFilename; }

string const& ExpProcInfo::getCacheDownloadRepository() const { return _cacheDownloadRepository; }

off_t const& ExpProcInfo::getCacheAvailableDownloadSpaceSize() const { return _cacheAvailableDownloadSpaceSize; }

vector<string> const& ExpProcInfo::getCacheItemsToRelease() const { return _cacheItemsToRelease; }

vector<string> const& ExpProcInfo::getCirculationIds() const { return _circulationIds; }

vector<string> const& ExpProcInfo::getPackageNames() const { return _packageNames; }

ExpProcInfoDistributionStatus const& ExpProcInfo::getDistributionStatus() const { return _distributionStatus; }

vector<string> const& ExpProcInfo::getNotificationListOfFiles         () const { return _notificationListOfFiles         ; }
string const&         ExpProcInfo::getNotificationDeliverySlipFilename() const { return _notificationDeliverySlipFilename; }
string const&         ExpProcInfo::getNotificationContentFilename     () const { return _notificationContentFilename     ; }



void ExpProcInfo::setDatapackageAbsPath(string const& datapackageAbsPath) { _datapackageAbsPath=datapackageAbsPath;}

void ExpProcInfo::setWorkingDir(string const& workingDir) { _workingDir=workingDir; }

void ExpProcInfo::setDatapackageFilename(std::string const& datapackageFilename) { _datapackageFilename=datapackageFilename; }

void ExpProcInfo::setDatapackageContent( dataprovider::DataPackage const& datapackageContent) { _datapackageContent = datapackageContent; }

void ExpProcInfo::setDistributionFilename(std::string const& distributionFilename) { _distributionFilename=distributionFilename; }

void ExpProcInfo::setDistributionContent(DistributionSerializable const& distributionContent) { _distributionContent = distributionContent; }

void ExpProcInfo::setParametersFilename(std::string const& parametersFilename) { _parametersFilename=parametersFilename; }

void ExpProcInfo::setCacheDownloadRepository(std::string const& cacheDownloadRepository) { _cacheDownloadRepository=cacheDownloadRepository; }

void ExpProcInfo::setCacheAvailableDownloadSpaceSize(off_t const& cacheAvailableDownloadSpaceSize){ _cacheAvailableDownloadSpaceSize=cacheAvailableDownloadSpaceSize; }

void ExpProcInfo::setCirculationIds(vector<string> const& circulationIds) { _circulationIds = circulationIds; }

void ExpProcInfo::setPackageNames(vector<string> const& pkgNames) { _packageNames = pkgNames; }

void ExpProcInfo::addCirculationId(string const& circulationId) { _circulationIds.push_back( circulationId ); }

void ExpProcInfo::addPackageName (string const& pkgName) { _packageNames.push_back( pkgName ); }

void ExpProcInfo::setDistributionStatus(ExpProcInfoDistributionStatus const& distributionStatus) { _distributionStatus=distributionStatus; }

void ExpProcInfo::setNotificationListOfFiles         (vector<string> const& notificationListOfFiles         ) { _notificationListOfFiles         =notificationListOfFiles         ; }
void ExpProcInfo::setNotificationDeliverySlipFilename(string const&         notificationDeliverySlipFilename) { _notificationDeliverySlipFilename=notificationDeliverySlipFilename; }
void ExpProcInfo::setNotificationContentFilename     (string const&         notificationContentFilename     ) { _notificationContentFilename     =notificationContentFilename     ; }


void ExpProcInfo::appendCacheItemsToRelease(vector<string> const& cacheItemsToRelease) { 
	_cacheItemsToRelease.insert( _cacheItemsToRelease.end(), cacheItemsToRelease.begin(), cacheItemsToRelease.end() );
}

void ExpProcInfo::resetCacheItemsToRelease() { _cacheItemsToRelease.clear(); }

void ExpProcInfo::dump(string const& filename) const {

	std::ofstream ofs(filename.c_str());
	ACS_COND_THROW( ! ofs.good(), exExpProcInfoCriticalException("Cannot dump ExpProcInfo on file "+filename) );

	this->dump( ofs );
}

void ExpProcInfo::dump(ostream& oss) const {

	//Dump the datapackage
	boost::archive::xml_oarchive oa(oss);

	const ExpProcInfo& expProcInfo = *this;

	// dump into the archive
	oa << BOOST_SERIALIZATION_NVP( expProcInfo );

}


void ExpProcInfo::load(string const& filename) {

	std::ifstream ifs( filename.c_str() );
	ACS_COND_THROW( ! ifs.good(), exExpProcInfoCriticalException("Cannot load ExpProcInfo from file "+filename) );

	this->load( ifs );

}

void ExpProcInfo::load(istream& iss) {

	//Load the datapackage
	boost::archive::xml_iarchive ia( iss );

	ExpProcInfo& expProcInfo = *this;

	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP( expProcInfo );

}

_ACS_END_NAMESPACE

