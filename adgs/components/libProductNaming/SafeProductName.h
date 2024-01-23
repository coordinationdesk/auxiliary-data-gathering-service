// PRQA S 1050 EOF
/*
	Copyright 1995-203, Advanced Computer Systems , Inc.
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
	Revision 1.4  2010/09/22 09:35:59  damdec
	Methods for building filenames according to Sentinel 3 PDGS File Naming Convention moved in SentinelProductName class.
	
	Revision 1.3  2010/08/04 15:19:22  damdec
	Optional parameter extension added to getS3SafeSralName method. getS3SafeAuxName method added for handling auxiliary filenames.
	
	Revision 1.2  2010/06/03 15:45:33  sara.farese
	getS3SafeSralName() method added.
	
	Revision 1.1  2010/04/16 08:59:33  marant
	SafeProductName added
	
        
*/

#ifndef _SafeProductName_H_
#define _SafeProductName_H_

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* <Insert Class Description> 
**/

class SafeProductName  { 
public:

	/** SafeProductName Exceptions */
	exDECLARE_EXCEPTION(exSafeProductNameException, exException) ; // Base SafeProductName Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exSafeProductNameCriticalException, exSafeProductNameException) ; // Critical Exception. // PRQA S 2153




    /**
		 * \brief Returns a SAFE Product file name of the form : 
		 *		  PlatformName + MissionNumber + ProductType + ValidityStartDate and Time + Validity Stop Date and Time +
		 *		  Originating Facility + Absolute Orbit + SAFE Manifest CRC + . + extension 
		 * The following checks are performed on the input parameters:
		 * PlatformName must be 2 chars long
		 * ProductType must be 10 chars long
	     	 * Extension must be "SAFE"
		 * no check is performed on validity start time and duration
         	 * \param satelliteName = Satellite name 
		 * \param missionNum = Mission number 
		 * \param productType = Product type (10 uppercase letters, digits and underscores)
		 * \param validityStartTime = Validity Start Time (expressed in JD50)
		 * \param validityStopTime = Validity Stop Time (expressed as JD50)
		 * \param originatingFacility = SAFE product originating facility (3 chars)
		 * \param absoluteOrbitNum = orbit absolute number
		 * \param safeManifestCrc = Safe Manifest CRC-16. 
		 * \param fileName = Result file name
		 */
		static std::string getName(const std::string& platformName, 
					 int missionNumber, 
					 const std::string& productType, 
					 long double validityStartTimeJd50, 
					 long double validityStopTimeJd50, 
					 const std::string& originatingFacility, 
					 unsigned int absoluteOrbitNum, 
					 int manifestCrc);



protected:

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SafeProductName)

};


_ACS_END_NAMESPACE

#endif //_SafeProductName_H_

