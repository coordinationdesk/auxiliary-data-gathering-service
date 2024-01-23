// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/05/05 13:13:25  marpas
	improving maintenance adopting dbPersistent convenience macros
	coding best practice application in progress
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:20  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.2  2013/06/12 15:37:14  masdal
	EarthExplorerFixedHeaderWriter changed in dbFixedHeaderWriter
	
	Revision 2.1  2013/06/11 13:33:57  marpas
	adding S3PDGSFixedHeaderFactory
	
	
	

*/

#include <S3PDGSFixedHeaderFactory.h>
#include <EarthExplorerFixedHeaderWriter.h>

#include <dbInventoryObject.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h> 

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


string S3PDGSFixedHeaderFactory::centreid_ = "NDC" ;
string S3PDGSFixedHeaderFactory::platform_ = "U" ;

void S3PDGSFixedHeaderFactory::checkConfiguration(void *)  
{
    try {
        ConfigurationSingleton::instance()->get().getValue("Station.CentreID", centreid_) ;
    }
    catch(exception &x) {
        ACS_LOG_ERROR("No Station.CentreID in configuration") ;
    }
    try {
        ConfigurationSingleton::instance()->get().getValue("Station.Platform", platform_) ;
    }
    catch(exception &x) {
        ACS_LOG_ERROR("No Station.Platform in configuration") ;
    }
}


S3PDGSFixedHeaderFactory::S3PDGSFixedHeaderFactory() : dbFixedHeaderFactory("","S3PDGS") 
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("StationConf")) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(checkConfiguration, 0) ;
}

S3PDGSFixedHeaderFactory::~S3PDGSFixedHeaderFactory() throw()
{
}


void S3PDGSFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	bool satset = false ;
	
	dbPersistent::IdType satId = inv.getSatelliteId(&satset) ;
	string satName = "" ;
	if (satset) {
		satName = SatelliteName::instance("")->name(satId) ;
	}

	fhw = new dbFixedHeaderWriter(inv, system, os) ; // satellite got from inventory object
}



//S2X_
//OPER
//_
//REP_OPMCC_
//_
//PDMC
//_
//YYYYMMDDTHHMMSS
string S3PDGSFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set = false ;
	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	ACS_COND_THROW(!set ||fileClass.empty(),exIllegalValueException("fileClass must be set for system S3PDGS")) ; // PRQA S 3081

	string filetype=inventoryObj.getFileType(&set) ;
	ACS_COND_THROW(!set ||filetype.empty(),exIllegalValueException("filetype must be set for system S3PDGS")); // PRQA S 3081

	DateTime gentime=inventoryObj.getGenerationTime(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStart must be set for system S3PDGS")) ; // PRQA S 3081

	DateTime validityStart=inventoryObj.getValidityStart(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStart must be set for system S3PDGS")) ; // PRQA S 3081

	DateTime validityStop=inventoryObj.getValidityStop(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStop must be set for system S3PDGS")) ; // PRQA S 3081

    checkConfiguration(0) ;

	DateTime now ;

	res= "S3__" +
		 filetype+  "_" +
		 validityStart.YYYYMMDDThhmmss() + "_" +
		 validityStop.YYYYMMDDThhmmss() +"_" +
		 gentime.YYYYMMDDThhmmss() +"_" +
         string(17,'_') + "_" + // PRQA S 4400
         centreid_ + "_" +
         platform_ + string(7,'_') ; // PRQA S 4400

	return res;
}



int S3PDGSFixedHeaderFactory::getSatelliteId() const
{
	return 0 ;
}


_ACS_END_NAMESPACE
