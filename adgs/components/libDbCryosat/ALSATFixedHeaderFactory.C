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
	
	Revision 5.2  2016/04/19 13:53:46  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.1  2015/08/07 09:46:45  tergem
	ALSATFixedHeaderFactory class added
	
	

*/

#include <ALSATFixedHeaderFactory.h>
#include <dbFixedHeaderWriter.h>
#include <dbInventoryObject.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h> 

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


string ALSATFixedHeaderFactory::centreid_ = "UNDEFCENTRE" ;

void ALSATFixedHeaderFactory::checkConfiguration(void *)  
{
    try {
        ConfigurationSingleton::instance()->get().getValue("Station.CentreID", centreid_) ;
    }
    catch(exception &x) {
        ACS_LOG_ERROR("No Station.CentreID in configuration") ;
    }
}


ALSATFixedHeaderFactory::ALSATFixedHeaderFactory() : 
    dbFixedHeaderFactory("","ALSAT"), 
    repunamefmt_() 
{
    ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("StationConf")) ; // PRQA S 3081
    ConfigurationSingleton::instance()->addCallback(checkConfiguration, 0) ;
}

ALSATFixedHeaderFactory::~ALSATFixedHeaderFactory() throw()
{
}


void ALSATFixedHeaderFactory::getWriter(dbInventoryObject const &inv, string const &system, XMLOstream &os, dbFixedHeaderWriter *&fhw) const
{
	bool satset = false ;
	
	int satId = inv.getSatelliteId(&satset) ;
	string satName = "" ;
	if (satset) {
		satName = SatelliteName::instance("")->name(satId) ; // PRQA S 3000
	}

	fhw = new dbFixedHeaderWriter(inv, system, os) ; // satellite got from inventory object
}




string ALSATFixedHeaderFactory::getFilename(dbInventoryObject const &inventoryObj) const 
{
	bool set = false ;
	string res;

	string fileClass=inventoryObj.getFileClass(&set) ;
	ACS_COND_THROW(!set ||fileClass.empty(),exIllegalValueException("fileClass must be set for system ALSAT")) ; // PRQA S 3081

	string filetype=inventoryObj.getFileType(&set) ;
	ACS_COND_THROW(!set ||filetype.empty(),exIllegalValueException("filetype must be set for system ALSAT")); // PRQA S 3081

	DateTime gentime=inventoryObj.getGenerationTime(&set) ;
	ACS_COND_THROW(!set,exIllegalValueException("validityStart must be set for system ALSAT")) ; // PRQA S 3081

    checkConfiguration(0) ;

	DateTime now ;

	res= "AST_" +
		 fileClass + "_" +
		 filetype+  "_" +
         centreid_ + "_" +
		 gentime.YYYYMMDDThhmmss() ;

	return res;
}



int ALSATFixedHeaderFactory::getSatelliteId() const
{
	return 0 ;
}

dbFixedHeaderFactory::UniqueNameFormatter &ALSATFixedHeaderFactory::getReportUniqueNameFormatter() const 
{ return repunamefmt_ ; }




ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::ALSATRepUniqueNameFormatter() :
    UniqueNameFormatter(),
    counter_(0)
{
}

ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::~ALSATRepUniqueNameFormatter() throw() {}

ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::ALSATRepUniqueNameFormatter(const ALSATRepUniqueNameFormatter &r) :
    UniqueNameFormatter(r),
    counter_(r.counter_) 
{}

ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter& 
ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::operator=(const ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter &r)
{ 
    if (this != &r) {
        UniqueNameFormatter::operator=(r) ;
        counter_ = r.counter_ ;
    }
    return *this ;
}

string ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::next(const std::string &previous) 
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


void ALSATFixedHeaderFactory::ALSATRepUniqueNameFormatter::reset() 
{
    counter_ = 0 ;
}

_ACS_END_NAMESPACE
