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
	
	Revision 5.2  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/10/31 17:07:17  marpas
	UniqueNameFormatter concept added
	getReportUniqueNameFormatter method added
	this allow a report to be generated using a unique name.
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.5  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.4  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.3  2012/03/15 09:34:39  marpas
	filename format fixed
	
	Revision 2.2  2012/03/12 15:16:15  marpas
	empty header for S1
	
	Revision 2.1  2012/03/09 10:53:23  marpas
	S1PDGSFixedHeaderFactory created and tested
	all test compilation fixed
	
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

#include <S1PDGSFixedHeaderFactory.h>
#include <dbFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h> 

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


string S1PDGSFixedHeaderFactory::centreid_ = "UNDEFCENTRE" ;

void S1PDGSFixedHeaderFactory::checkConfiguration(void *)  
{
    try {
        ConfigurationSingleton::instance()->get().getValue("Station.CentreID", centreid_) ;
    }
    catch(exception &x) {
        ACS_LOG_ERROR("No Station.CentreID in configuration") ;
    }
}


S1PDGSFixedHeaderFactory::S1PDGSFixedHeaderFactory() : 
    dbFixedHeaderFactory("","S1PDGS"), 
    repunamefmt_() 
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("StationConf")) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(checkConfiguration, 0) ;
}

S1PDGSFixedHeaderFactory::~S1PDGSFixedHeaderFactory() throw()
{
}


void S1PDGSFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	bool satset = false ;
	
	int satId = inv.getSatelliteId(&satset) ;
	string satName = "" ;
	if (satset) {
		satName = SatelliteName::instance("")->name(satId) ; // PRQA S 3000
	}

	fhw = new dbFixedHeaderWriter(inv, system, os) ; // satellite got from inventory object
}




string S1PDGSFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
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

	res= string("S1__") +
		 fileClass + "_" +
		 filetype+  "_" +
         centreid_ + "_" +
		 gentime.YYYYMMDDThhmmss() ;

	return res;
}



int S1PDGSFixedHeaderFactory::getSatelliteId() const
{
	return 0 ;
}

dbFixedHeaderFactory::UniqueNameFormatter &S1PDGSFixedHeaderFactory::getReportUniqueNameFormatter() const 
{ return repunamefmt_ ; }




S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::S1RepUniqueNameFormatter() :
    UniqueNameFormatter(),
    counter_(0)
{
}

S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::~S1RepUniqueNameFormatter() throw() {}

S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::S1RepUniqueNameFormatter(const S1RepUniqueNameFormatter &r) :
    UniqueNameFormatter(r),
    counter_(r.counter_) 
{}

S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter& 
S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::operator=(const S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter &r)
{ 
    if (this != &r) {
        UniqueNameFormatter::operator=(r) ;
        counter_ = r.counter_ ;
    }
    return *this ;
}

string S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::next(const std::string &previous) 
{
    char prevadd[1024] ; // PRQA S 4403
    memset(prevadd, 0, sizeof(prevadd)) ;
    
    sprintf(prevadd,"_I%04d", counter_) ;
    counter_++ ; 
    ACS_COND_THROW(counter_ > 9999, exIllegalValueException("Too many attempt for unique name")) ; // PRQA S 3081
    char nextadd[1024] ; // PRQA S 4403
    memset(nextadd, 0, sizeof(nextadd)) ;
    sprintf(nextadd,"_I%04d", counter_) ;
    size_t s = previous.rfind(prevadd) ;
    string n = previous ;
    if (s != string::npos) {
        n = previous.substr(0, s) ;
    }
    return n + nextadd ;
}


void S1PDGSFixedHeaderFactory::S1RepUniqueNameFormatter::reset() 
{
    counter_ = 0 ;
}

_ACS_END_NAMESPACE
