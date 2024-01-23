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
	
	Revision 5.2  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
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
	
	Revision 1.6  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 1.5  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 1.4  2012/02/14 12:52:45  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 1.3  2009/05/13 09:09:55  marpas
	makeup and some rule fixing
	
	Revision 1.2  2009/02/16 17:56:46  marpas
	filename should start with VE_
	
	Revision 1.1  2008/12/16 15:56:51  marpas
	just added ... hoping someone test the contents
	

*/

#include <VenusFixedHeaderFactory.h>
#include <EarthExplorerFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

//*********** Utility functions ******************************
static string nameDate(DateTime const &d)
{
	// method to return part of the string 
	// composing the name 
	// as in 
	// 19991231T010203
	ostringstream os;
    DateTime::UTCD utcd = d ;
	os << setfill('0')
	   << setw(4) << utcd.year 
	   << setw(2) << utcd.month
	   << setw(2) << utcd.day
	   <<"T"
	   << setw(2) << utcd.hour 
	   << setw(2) << utcd.min
	   << setw(2) << utcd.sec;
	   
	return os.str();
	
}

VenusFixedHeaderFactory::VenusFixedHeaderFactory() : dbFixedHeaderFactory("VENUS") 
{
}

VenusFixedHeaderFactory::~VenusFixedHeaderFactory() throw() {}


void VenusFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	fhw = new EarthExplorerFixedHeaderWriter("Venus" , inv, system, os) ;
}




string VenusFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set;

	string res;

	// the file class - MUST not be empty
	string fileClass=inventoryObj.getFileClass(&set) ;
	ACS_COND_THROW(!set ||fileClass.empty(), exIllegalValueException("fileClass must be set for satellite VENUS")) ;

	// the file type - MUST not be empty
	string filetype=inventoryObj.getFileType(&set) ;
	ACS_COND_THROW(!set ||filetype.empty(),exIllegalValueException("filetype must be set for satellite VENUS")) ;

	// the val start - MUST not be empty
	DateTime validityStart=inventoryObj.getValidityStart(&set) ;
	ACS_COND_THROW(!set, exIllegalValueException("validityStart must be set for satellite VENUS")) ;

	// the val stop - MUST not be empty
	DateTime validityStop=inventoryObj.getValidityStop(&set) ;
	ACS_COND_THROW(!set, exIllegalValueException("validityStop must be set for satellite VENUS")) ;

	// the fileversion - MUST not be empty
	string fileVersion=inventoryObj.getFileVersion(&set) ;
	ACS_COND_THROW(!set ||fileVersion.empty(), exIllegalValueException("fileVersion must be set for satellite VENUS")) ;

	// eventually compute the filename .. 
	res= string("VE_") +
		 fileClass + "_" +
		 filetype+  "_" +
		 nameDate(validityStart) + "_" +
		 nameDate(validityStop) +"_" +
		 fileVersion;

	// .. and return it 
	return res;

	
}



int VenusFixedHeaderFactory::getSatelliteId() const
{
	return SatelliteName::instance("")->id(name(),true) ;
}



_ACS_END_NAMESPACE



