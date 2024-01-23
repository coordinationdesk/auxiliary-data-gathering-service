// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	
	Revision 5.1  2016/04/19 15:23:45  marpas
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
	
	Revision 2.4  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.3  2013/02/06 16:02:41  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.2  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2006/04/04 15:03:41  paoscu
	GoceFixedHeaderFactory added.
	EarthExplorerFixedHeaderWriter replaces CryosatFixedHeaderWriter
	
	Revision 2.0  2006/02/28 09:09:34  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/03/09 18:03:23  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.3  2004/09/08 17:46:06  paoscu
	dbFixedHeaderFactoryChain bug fixed. Now ignoreCase for SatelliteName is implemented
	CryosatFixedHeaderFactory minor bug fixed when returning satelliteId
	
	Revision 1.2  2004/05/05 18:23:38  paoscu
	Work in progress.
	
	Revision 1.1  2004/05/03 14:53:59  paoscu
	New haeader writers.
	
		

*/

#include <CryosatFixedHeaderFactory.h>
#include <EarthExplorerFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

namespace {
//*********** Utility functions ******************************
string nameDate(DateTime const &d)
{
	ostringstream os;
    DateTime::UTCD utcd = d ; // PRQA S 3050 
	os << setfill('0')
	   << setw(4) << utcd.year  // PRQA S 4400 3
	   << setw(2) << utcd.month
	   << setw(2) << utcd.day
	   <<"T"
	   << setw(2) << utcd.hour  // PRQA S 4400 3
	   << setw(2) << utcd.min
	   << setw(2) << utcd.sec;
	   
	return os.str();
	
}

}

CryosatFixedHeaderFactory::CryosatFixedHeaderFactory() : dbFixedHeaderFactory("CRYOSAT") 
{
}

CryosatFixedHeaderFactory::~CryosatFixedHeaderFactory() throw()
{
}


void CryosatFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	fhw = new EarthExplorerFixedHeaderWriter("CryoSat" , inv, system, os) ;
}




string CryosatFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set = false ;

	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	if(!set ||fileClass.empty() )
	{
		ACS_THROW(exIllegalValueException("fileClass must be set for satellite CRYOSAT")) ; // PRQA S 3081
	}

	string filetype=inventoryObj.getFileType(&set) ;
	if(!set ||filetype.empty())
	{
		ACS_THROW(exIllegalValueException("filetype must be set for satellite CRYOSAT")) ; // PRQA S 3081
	}

	DateTime validityStart=inventoryObj.getValidityStart(&set) ;
	if(!set)
	{
		ACS_THROW(exIllegalValueException("validityStart must be set for satellite CRYOSAT")) ; // PRQA S 3081
	}

	DateTime validityStop=inventoryObj.getValidityStop(&set) ;
	if(!set)
	{
		ACS_THROW(exIllegalValueException("validityStop must be set for satellite CRYOSAT")) ; // PRQA S 3081
	}

	string fileVersion=inventoryObj.getFileVersion(&set) ;
	if(!set ||fileVersion.empty())
	{
		ACS_THROW(exIllegalValueException("fileVersion must be set for satellite CRYOSAT")) ; // PRQA S 3081
	}


	res= "CS_" +
		 fileClass + "_" +
		 filetype+  "_" +
		 nameDate(validityStart) + "_" +
		 nameDate(validityStop) +"_" +
		 fileVersion;

	return res;

	
}



int CryosatFixedHeaderFactory::getSatelliteId() const
{
	return SatelliteName::instance("")->id(name(),true) ; // PRQA S 3000
}






_ACS_END_NAMESPACE

