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
	
	Revision 5.3  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.2  2016/04/19 15:23:45  marpas
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
	
	Revision 3.0  2013/09/19 14:57:18  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.5  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.4  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.3  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2007/07/02 14:56:25  serfol
	changed the file naming convention.
	
	Revision 2.1  2007/06/28 14:59:56  serfol
	adding the EnvisatFixedHeaderFactory first time
	

*/


#include <EnvisatFixedHeaderFactory.h>
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
	       << setw(4) << utcd.year // PRQA S 4400 3
	       << setw(2) << utcd.month
	       << setw(2) << utcd.day
	       <<"_"
	       << setw(2) << utcd.hour  // PRQA S 4400 3
	       << setw(2) << utcd.min
	       << setw(2) << utcd.sec;

	    return os.str();

    }
}
EnvisatFixedHeaderFactory::EnvisatFixedHeaderFactory() : dbFixedHeaderFactory("ENVISAT") 
{
}

EnvisatFixedHeaderFactory::~EnvisatFixedHeaderFactory() throw()
{
}


void EnvisatFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	fhw = new EarthExplorerFixedHeaderWriter("ENVISAT" , inv, system, os) ;
}




string EnvisatFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set = false ;

	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	if(!set ||fileClass.empty() )
	{
		exIllegalValueException e("fileClass must be set for satellite ENVISAT");
		ACS_THROW(e);
	}

	string filetype=inventoryObj.getFileType(&set) ;
	if(!set ||filetype.empty())
	{
		exIllegalValueException e("filetype must be set for satellite ENVISAT");
		ACS_THROW(e);
	}

	DateTime validityStart=inventoryObj.getValidityStart(&set) ;
	if(!set)
	{
		exIllegalValueException e("validityStart must be set for satellite ENVISAT");
		ACS_THROW(e);
	}

	DateTime validityStop=inventoryObj.getValidityStop(&set) ;
	if(!set)
	{
		exIllegalValueException e("validityStop must be set for satellite ENVISAT");
		ACS_THROW(e);
	}

	string fileVersion=inventoryObj.getFileVersion(&set) ;
	if(!set ||fileVersion.empty())
	{
		exIllegalValueException e("fileVersion must be set for satellite ENVISAT");
		ACS_THROW(e);
	}
	
	DateTime currentTime;

	res=     filetype + 
		 fileClass.substr(0,1) + "ACS" +
		 nameDate(currentTime) + "_" +
		 nameDate(validityStart) + "_" +
		 nameDate(validityStop);

	return res;

	
}



int EnvisatFixedHeaderFactory::getSatelliteId() const
{
	return SatelliteName::instance("")->id(name(),true) ; // PRQA S 3000
}


_ACS_END_NAMESPACE
