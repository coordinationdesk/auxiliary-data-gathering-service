// PRQA S 1050 EOF
/*

	Copyright 2012-2013, Advanced Computer Systems , Inc.
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
	
	Revision 5.1  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:50  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:18  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.3  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.2  2013/02/06 16:02:41  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.1  2012/11/16 13:01:12  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
*/

#include <ADCFixedHeaderFactory.h>
#include <EarthExplorerFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h> 


_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


string ADCS1FixedHeaderFactory::centreid_ = "UNDEFCENTRE" ;

void ADCS1FixedHeaderFactory::checkConfiguration(void *)  
{
    try {
        ConfigurationSingleton::instance()->get().getValue("Station.CentreID", centreid_) ;
    }
    catch(exception &x) {
        ACS_LOG_ERROR("No Station.CentreID in configuration") ;
    }
}


ADCS1FixedHeaderFactory::ADCS1FixedHeaderFactory() : dbFixedHeaderFactory("","ADCS1") 
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("StationConf")) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(checkConfiguration, 0) ;
}

ADCS1FixedHeaderFactory::~ADCS1FixedHeaderFactory() throw()
{
}


void ADCS1FixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	bool satset = false ;
	
	int satId = inv.getSatelliteId(&satset) ;
	string satName = "" ;
	if (satset) {
		satName = SatelliteName::instance("")->name(satId) ; // PRQA S 3000
	}

	fhw = new EarthExplorerFixedHeaderWriter(satName , inv, system, os) ; // satellite got from inventory object
}




string ADCS1FixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{

	bool set = false ;
	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	ACS_COND_THROW(!set ||fileClass.empty(),exIllegalValueException("fileClass must be set for system S1PDGS")) ; // PRQA S 3081

	string filetype=inventoryObj.getFileType(&set) ;
	ACS_COND_THROW(!set ||filetype.empty(),exIllegalValueException("filetype must be set for system S1PDGS")); // PRQA S 3081

	DateTime gentime=inventoryObj.getGenerationTime(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStart must be set for system S1PDGS")) ; // PRQA S 3081

    checkConfiguration(0) ;

	DateTime now ;

	res = "S1__" +
		 fileClass + "_" +
		 filetype+  "_" +
         centreid_ + "_" +
		 gentime.YYYYMMDDThhmmss() ;

	return res;
}



int ADCS1FixedHeaderFactory::getSatelliteId() const
{
	return 0 ;
}



_ACS_END_NAMESPACE
