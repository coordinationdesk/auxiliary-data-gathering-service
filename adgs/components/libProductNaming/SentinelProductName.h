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
	Revision 1.5  2013/07/19 15:54:30  enrcar
	EC:: Added overload method: getS3Name
	
	Revision 1.4  2013/07/19 11:49:23  enrcar
	EC:: interface updated to fullfill new naming convention
	
	Revision 1.3  2012/10/19 09:14:28  damdec
	New getS3Name method added.
	
	Revision 1.2  2010/09/23 15:02:24  damdec
	getS3Name and getS3AuxName methods interface modified.
	
	Revision 1.1  2010/09/22 09:33:45  damdec
	Class for building Sentinel Filenames added.
	
        
*/

#ifndef _SentinelProductName_H_
#define _SentinelProductName_H_

#include <exException.h>
#include <DateTime.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Class for building Sentinel Products and Auxiliary Filenames
**/

class SentinelProductName  { 
public:

	/** SentinelProductName Exceptions */
	exDECLARE_EXCEPTION(exSentinelProductNameException, exException) ; // Base SentinelProductName Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exSentinelProductNameCriticalException, exSentinelProductNameException) ; // Critical Exception. // PRQA S 2153
    	/**
	 * \brief Returns a SAFE Product file name of the form specified in "Sentinel 3 PDGS File Naming Convention" - EUM/LEO-SEN3/SPE/10/0070: 
	 *		  Mission ID + Data source/consumer + Processing Level + Data Type ID + Validity Start Date and Time + Validity Stop Date and Time +
	 *	 	  InstanceId + Product Generating Centre + platform + Product timeliness/consolidation +  3 letters/digits Free text + 
	 *		  File versioning + . + extension
	 * The following checks are performed on the input parameters:
	 * Mission Identifier must be 3 chars long
	 * Output File Type must be 8 chars lon
	 * Validity Start Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * Validity Stop Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * Creation Date must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * InstanceId must be 17 digits long
	 * platform must be 1 char long
	 * baseDataUs must be 3 digits long
	 * File versioning must be 2 digits long
	 * \param missionID = Mission Identifier 
	 * \param outFileType = Output File Type (wich includes Processing Level and Data Type ID)
	 * \param validityStartTime = Validity Start Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param validityStopTime = Validity Stop Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param instanceId = Instance Id (its content changes according to the product type)
	 * \param generatingCentre = Product Generating Centre
	 * \param platformId = platform (O for operational, F for reference, D for development, R for reprocessing)
	 * \param procStage = Processing Stage (NRT, NTC or STC)
	 * \param baseDataUs = 3 letters/digits. Free text for indicating the baseline collection (001, 002 ?.) or data usage (e.g. test, GSV, etc)
	 * \param ext = Extension (default="SEN3")
	 */
	static std::string getS3Name(const std::string& missionID, 
						const std::string& outFileType, 
						const DateTime& start, 
						const DateTime& stop, 
						const DateTime& creation_date, 
						const std::string& instanceId, 
						const std::string& generatingCentre, 
						const std::string& classId, 
						const std::string & ext="SEN3");

	static std::string getS3Name(const std::string& missionID, 
						const std::string& outFileType, 
						const std::string& start, 
						const std::string& stop, 
						const std::string& creation_date, 
						const std::string& instanceId, 
						const std::string& generatingCentre, 
						const std::string& classId, 
						const std::string & ext="SEN3");

    	/**
	 * \brief Returns a SAFE Product file name of the form specified in "Sentinel 3 PDGS File Naming Convention" - EUM/LEO-SEN3/SPE/10/0070 for SRAL data: 
	 *		  Mission ID + Data source/consumer + Processing Level + Data Type ID + Validity Start Date and Time + Validity Stop Date and Time +
		 	  Duration + Cycle number + Relative orbit number + 4 underscores +
	 *		  Product Generating Centre + platform + Product timeliness/consolidation +  3 letters/digits Free text + File versioning + . + extension
	 * The following checks are performed on the input parameters:
	 * Mission Identifier must be 3 chars long
	 * Data source/consumer must be 2 chars long
	 * Output File Type must be 8 chars lon
	 * Validity Start Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * Validity Stop Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * Cycle number must be 3 digits long
	 * Relative orbit number must be 3 digits long
	 * platform must be 1 char long
	 * baseDataUs must be 3 digits long
	 * File versioning must be 2 digits long
	 * \param missionID = Mission Identifier 
	 * \param dataSource = Data source/consumer
	 * \param outFileType = Output File Type (wich includes Processing Level and Data Type ID)
	 * \param validityStartTime = Validity Start Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param validityStopTime = Validity Stop Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param cycleNumber = Cycle number
	 * \param relativeOrbitNumber = Relative orbit number
	 * \param generatingCentre = Product Generating Centre
	 * \param platformId = platform (O for operational, F for reference, D for development, R for reprocessing)
	 * \param procStage = Processing Stage (NRT, NTC or STC)
	 * \param baseDataUs = 3 letters/digits. Free text for indicating the baseline collection (001, 002 ?.) or data usage (e.g. test, GSV, etc)
	 * \param fileVers = File versioning
	 * \param ext = Extension (default="SAFE")
	 */
	static std::string getS3Name(const std::string& missionID, 
					const std::string& dataSource, 
					const std::string& outFileType, 
					const DateTime& start, 
					const DateTime& stop,  
					long cycleNumber, 
					long relativeOrbitNumber, 
					const std::string& generatingCentre, 
					const std::string& platformId, 
					const std::string& procStage, 
					const std::string& baseDataUs, 
					int fileVers, const std::string & ext = "SAFE");

    	/**
	 * \brief Returns a file name of the form specified in "Sentinel 3 PDGS File Naming Convention" - EUM/LEO-SEN3/SPE/10/0070 for Auxiliary data: 
	 *		 Mission ID + Data source/consumer + underscore + Data Type ID + Validity Start Date and Time + Validity Stop Date and Time +
		 	 Duration + InstanceID + 2 underscores +
	 *		 Product Generating Centre + platform + Product timeliness/consolidation +  3 letters/digits Free text + File versioning + . + extension
	 * The following checks are performed on the input parameters:
	 * Mission Identifier must be 3 chars long
	 * Data source/consumer must be 2 chars long
	 * Output File Type must be 8 chars lon
	 * Validity Start Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * Validity Stop Date and Time must be 26 chars long (YYYY-MM-DD HH:MM:SS.mmmmmm format is foreseen)
	 * InstanceID must be 17 chars long (YYYYMMDDTHHMMSS format is foreseen)
	 * platform must be 1 char long
	 * baseDataUs must be 3 digits long
	 * File versioning must be 2 digits long
	 * \param missionID = Mission Identifier 
	 * \param dataSource = Data source/consumer
	 * \param outFileType = Output File Type (wich includes Processing Level and Data Type ID)
	 * \param validityStartTime = Validity Start Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param validityStopTime = Validity Stop Date and Time (YYYY-MM-DD HH:MM:SS.mmmmmm format)
	 * \param instanceId = auxiliary data file creation date
	 * \param generatingCentre = Product Generating Centre
	 * \param classId = TBD
	 * \param fileVers = File versioning
	 * \param ext = Extension
	 */
	static std::string getS3AuxName(const std::string& missionID, 
					const std::string& dataSource, 
					const std::string& outFileType, 
					const DateTime& start, 
					const DateTime& stop,  
					const DateTime& instanceId, 
					const std::string& generatingCentre, 
					const std::string& classId, 
					int fileVers, const std::string & ext);
		
		
    static std::string dateTime2String(DateTime d) ;
					
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SentinelProductName) ;

};



_ACS_END_NAMESPACE

#endif //_SentinelProductName_H_

