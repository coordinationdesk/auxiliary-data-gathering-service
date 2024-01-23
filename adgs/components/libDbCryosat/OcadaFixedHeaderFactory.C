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
	
	Revision 5.2  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:51  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 1.7  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 1.6  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 1.5  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 1.4  2010/10/18 16:17:08  marpas
	Satellite name got from inventory
	
	Revision 1.3  2010/09/14 11:58:41  marpas
	filename generated include a datetime with now
	
	Revision 1.2  2009/04/08 10:08:19  marpas
	system concept introduced in order to have a writer for a system with no satellite
	
	Revision 1.1  2009/04/03 17:12:07  marpas
	class forgot
	
	Revision 1.2  2009/02/16 17:56:46  marpas
	filename should start with VE_
	
	Revision 1.1  2008/12/16 15:56:51  marpas
	just added ... hoping someone test the contents
	

*/

#include <OcadaFixedHeaderFactory.h>
#include <EarthExplorerFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

OcadaFixedHeaderFactory::OcadaFixedHeaderFactory() : dbFixedHeaderFactory("","OCADA") 
{
}

OcadaFixedHeaderFactory::~OcadaFixedHeaderFactory() throw()
{
}


void OcadaFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	bool satset = false ;
	
	int satId = inv.getSatelliteId(&satset) ;
	string satName = "" ;
	if (satset) {
		satName = SatelliteName::instance("")->name(satId) ; // PRQA S 3000
	}

	fhw = new EarthExplorerFixedHeaderWriter(satName , inv, system, os) ; // satellite got from inventory object
}




string OcadaFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set = false ;
	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	ACS_COND_THROW(!set ||fileClass.empty(),exIllegalValueException("fileClass must be set for system OCADA")) ; // PRQA S 3081

	string filetype=inventoryObj.getFileType(&set) ;
	ACS_COND_THROW(!set ||filetype.empty(),exIllegalValueException("filetype must be set for system OCADA")); // PRQA S 3081

	DateTime validityStart=inventoryObj.getValidityStart(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStart must be set for system OCADA")) ; // PRQA S 3081

	DateTime validityStop=inventoryObj.getValidityStop(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStop must be set for system OCADA")) ; // PRQA S 3081
	
	string fileVersion=inventoryObj.getFileVersion(&set) ;
	ACS_COND_THROW(!set ||fileVersion.empty(),exIllegalValueException("fileVersion must be set for system OCADA")) ; // PRQA S 3081

	DateTime now ;

	res= "OC_" +
		 fileClass + "_" +
		 filetype+  "_" +
		 validityStart.YYYYMMDDThhmmss() + "_" +
		 validityStop.YYYYMMDDThhmmss() +"_" +
		 now.YYYYMMDDThhmmss() +"_" +
		 fileVersion;

	return res;
}



int OcadaFixedHeaderFactory::getSatelliteId() const
{
	return 0 ;
}


_ACS_END_NAMESPACE
