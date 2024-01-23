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

	$Prod: A.C.S. libProductNaming$

	$Id$

	$Author$

	$Log$
	Revision 1.7  2013/11/12 09:40:29  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.6  2012/02/23 16:39:21  marant
	compilation warning solved
	
	Revision 1.5  2010/09/22 09:35:59  damdec
	Methods for building filenames according to Sentinel 3 PDGS File Naming Convention moved in SentinelProductName class.
	
	Revision 1.4  2010/09/06 09:00:33  damdec
	Handling of filenames without extension introduced.
	
	Revision 1.3  2010/08/04 15:19:22  damdec
	Optional parameter extension added to getS3SafeSralName method. getS3SafeAuxName method added for handling auxiliary filenames.
	
	Revision 1.2  2010/06/03 15:45:33  sara.farese
	getS3SafeSralName() method added.
	
	Revision 1.1  2010/04/16 08:59:33  marant
	SafeProductName added
	
        
*/ 

#include <SafeProductName.h>
#include <DateTime.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SafeProductName)

namespace {
    string jd502string(const long double& time)
    {
        return DateTime(DateTime::JD50(time)).YYYYMMDDhhmmss() ; // PRQA S 3081
    }
}

string SafeProductName::getName(const string& platformName, 
					 int missionNumber, 
					 const string& productType, 
					 long double validityStartTimeJd50, 
					 long double validityStopTimeJd50, 
					 const string& originatingFacility, 
					 unsigned int absoluteOrbitNum, 
					 int manifestCrc)
{
   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

   //check the input:
   ACS_COND_THROW(platformName.size()!=2, exSafeProductNameException("Invalid platform name (2 characters are foreseen)")); // PRQA S 3081 5
   ACS_COND_THROW((missionNumber < 0) || (missionNumber > 99), exSafeProductNameException("Invalid Mission number."));
   ACS_COND_THROW(productType.size()!=10, exSafeProductNameException("Invalid Product Type (10 characters are foreseen)"));
   ACS_COND_THROW(originatingFacility.size()!=3, exSafeProductNameException("Invalid Originating Facility (3 characters are foreseen)"));
   ACS_COND_THROW(absoluteOrbitNum > 999999, exSafeProductNameException("Invalid absolute orbit"));

   string start = jd502string(validityStartTimeJd50);
   string stop = jd502string(validityStopTimeJd50);

   unsigned int crc = 0 ;
   if ( (manifestCrc >= 0) &&  (manifestCrc <= 0xffff) ) { // PRQA S 4400
       crc = manifestCrc ; // PRQA S 3000
   }
   else {
       crc = 0;
   }
        
   char buffer[256]; // PRQA S 4403
   sprintf(buffer, 
           "%s%02d_%s_%s_%s_%s_%d_%04X.SAFE", 
           platformName.c_str(), 
           missionNumber, 
           productType.c_str(), 
           start.c_str(), 
           stop.c_str(), 
           originatingFacility.c_str(), 
           absoluteOrbitNum, 
           crc);

   return buffer ;
}





_ACS_END_NAMESPACE
