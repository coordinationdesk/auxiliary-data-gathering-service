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
	Revision 1.12  2016/05/10 20:48:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.11  2014/07/02 14:08:00  enrcar
	EC:: method asISO8601 replaced by asYYYYMMDD since the former has been modified
	
	Revision 1.10  2014/04/24 16:35:23  tergem
	Regression introduced in previous commit: time format restored to expected values in getS3Name method
	
	Revision 1.9  2013/11/12 09:40:30  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.8  2013/07/19 15:54:30  enrcar
	EC:: Added overload method: getS3Name
	
	Revision 1.7  2013/07/19 11:49:23  enrcar
	EC:: interface updated to fullfill new naming convention
	
	Revision 1.6  2012/10/19 09:14:28  damdec
	New getS3Name method added.
	
	Revision 1.5  2012/02/23 16:39:21  marant
	compilation warning solved
	
	Revision 1.4  2011/03/02 15:39:11  damdec
	Generating center corretly handled.
	
	Revision 1.3  2010/11/15 08:34:57  marant
	getS3Name() and getS3AuxName() fixed: buffer size was too small to hold the whole product name
	
	Revision 1.2  2010/09/23 15:02:24  damdec
	getS3Name and getS3AuxName methods interface modified.
	
	Revision 1.1  2010/09/22 09:33:45  damdec
	Class for building Sentinel Filenames added.
	
        
*/ 

#include <SentinelProductName.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SentinelProductName)

static DateTime roundDateTime(const DateTime& in)
{
	DateTime::JD50 rinjd50(rintl(in.jd50()*86400.0)/86400.0) ;
	DateTime rout(rinjd50) ;
	return(rout) ;
}

string SentinelProductName::getS3Name(const std::string& missionID, 
					const std::string& outFileType, 
					const DateTime& start, 
					const DateTime& stop, 
					const DateTime& creation_date, 
					const string& instanceId, 
					const std::string& generatingCentre, 
					const std::string& classId, 
					const std::string & ext)
{
	
	DateTime rstart = roundDateTime(start) ;
	DateTime rstop = roundDateTime(stop) ;
	
	return getS3Name(	missionID, 
						outFileType, 
						rstart.YYYYMMDDThhmmss(), 
						rstop.YYYYMMDDThhmmss(), 
						creation_date.YYYYMMDDThhmmss(), 
						instanceId, 
						generatingCentre, 
						classId, 
						ext) ;
}


string SentinelProductName::getS3Name(const std::string& missionID, 
					const std::string& outFileType, 
					const string& start, 
					const string& stop, 
					const string& creation_date, 
					const string& instanceId, 
					const std::string& generatingCentre, 
					const std::string& classId, 
					const std::string & ext)
{

/*
convention is identified by the sequence of fields described here below:
MMM_SS_L_TTTTTT_yyyymmddThhmmss_YYYYMMDDTHHMMSS_YYYYMMDDTHHMMSS_<instance_ID>_GGG_<class ID>.<extension>
*/

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

	//check the input:
	ACS_COND_THROW(missionID.size()!=3, exSentinelProductNameException("Invalid Mission ID name (3 characters are foreseen)")); // PRQA S 3081 3
	ACS_COND_THROW(outFileType.size()!=11, exSentinelProductNameException("Invalid Output File_Type (11 characters are foreseen)"));	
	ACS_COND_THROW(instanceId.size()!=17, exSentinelProductNameException("Invalid Instance ID (17 characters are foreseen)"));	

	
	//The Generating Centre has to be 3 chars long
    string genCentre = generatingCentre;
    if (genCentre.size() != 3) { genCentre.resize(3,'_'); } // PRQA S 4400
	
	//The ClassID has to be 8 chars long
	string cid = classId ;
	if (cid.size() != 8) { cid.resize(8, '_'); } // PRQA S 4400
	
	const string extension = (ext.empty()? "" : ("."+ext) ) ; // PRQA S 3384, 3385

	ostringstream buffer ;
	buffer	<< missionID << "_"
			<< outFileType << "_" 
			<< start << "_"  
			<< stop << "_" 
			<< creation_date << "_" 
			<< instanceId  << "_" 
			<< genCentre << "_" 
			<< cid 
			<< extension ;

   	return buffer.str() ;	
}

string SentinelProductName::getS3Name(const std::string& missionID, 
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
					int fileVers,
					const std::string & ext)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

	//check the input:
	ACS_COND_THROW(missionID.size()!=3, exSentinelProductNameException("Invalid Mission ID name (3 characters are foreseen)")); // PRQA S 3081 3
   	ACS_COND_THROW(dataSource.size()!=2, exSentinelProductNameException("Invalid Data source/consumer name (2 characters are foreseen)"));
	ACS_COND_THROW(outFileType.size()!=8, exSentinelProductNameException("Invalid Output File_Type (8 characters are foreseen)"));	
	
    	//Calculate Orbit Duration
	int orbitDuration = (stop.jd50()-start.jd50())*86400 ; // as int // PRQA S 3011

 	//Write validityStartTime and validityStopTime in the YYYYMMDDTHHMMSS format
	//string validityStart = validityStartTime.substr(0,4) + validityStartTime.substr(5,2) + validityStartTime.substr(8,2) + 'T'+ validityStartTime.substr(11,2) + validityStartTime.substr(14,2) + validityStartTime.substr(17,2);
 	//string validityStop = validityStopTime.substr(0,4) + validityStopTime.substr(5,2) + validityStopTime.substr(8,2) + 'T'+ validityStopTime.substr(11,2) + validityStopTime.substr(14,2) + validityStopTime.substr(17,2);

		
	ACS_COND_THROW(cycleNumber > 999, exSentinelProductNameException("Invalid Cycle Number (3 digits are foreseen)")); // PRQA S 3081 2
	ACS_COND_THROW(relativeOrbitNumber > 999, exSentinelProductNameException("Invalid Relative Orbit Number (3 digits are foreseen)"));
	
	//The Generating Centre has to be 3 chars long
	string genCentre = generatingCentre;
	if (genCentre.size() != 3) { // PRQA S 4400 2
		genCentre.resize(3,'_');
    }
           
    	
	ACS_COND_THROW(platformId.size()!=1, exSentinelProductNameException("Invalid Platform ID name (1 character is foreseen)")); // PRQA S 3081
	
	std::string procStageId;
	if(procStage == "NRT") { procStageId = "NR"; }
	else if(procStage == "STC") { procStageId = "ST"; }
	else if(procStage == "NTC") { procStageId = "NT"; }
	else { ACS_THROW(exSentinelProductNameException("Invalid Processing Stage")); } // PRQA S 3081
	
	ACS_COND_THROW(baseDataUs.size()!=3, exSentinelProductNameException("Invalid Invalid baseline/data usage (3 characters are foreseen)")); // PRQA S 3081 2
	ACS_COND_THROW(fileVers > 99, exSentinelProductNameException("Invalid File Versioning number (2 digits are foreseen)"));


   	char buffer[256]; // PRQA S 4403
	std::string extension = (ext.empty()? ext :"."+ext); // PRQA S 3384, 3385
   	sprintf(buffer, 
            "%s_%s_%s_%s_%s_%04d_%03ld_%03ld______%s_%s_%s_%s_%02d%s", 
            missionID.c_str(), 
            dataSource.c_str(), 
            outFileType.c_str(), 
			start.YYYYMMDDThhmmss().c_str(), 
			stop.YYYYMMDDThhmmss().c_str(), 
            orbitDuration, 
            cycleNumber, 
            relativeOrbitNumber, 
            genCentre.c_str(), 
            platformId.c_str(), 
            procStageId.c_str(), 
            baseDataUs.c_str(), 
            fileVers, 
            extension.c_str());

   	return buffer ;	
}

string SentinelProductName::getS3AuxName(const std::string& missionID, 
					const std::string& dataSource, 
					const std::string& outFileType, 
					const DateTime& start, 
					const DateTime& stop, 
					const DateTime& instanceId, 
					const std::string& generatingCentre, 
					const std::string& classId, 
					int fileVers,
					const std::string & ext)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

	//check the input:
	ACS_COND_THROW(missionID.size()!=3, exSentinelProductNameException("Invalid Mission ID name (3 characters are foreseen)")); // PRQA S 3081 3
   	ACS_COND_THROW(dataSource.size()!=2, exSentinelProductNameException("Invalid Data source/consumer name (2 characters are foreseen)"));
	ACS_COND_THROW(outFileType.size()!=8, exSentinelProductNameException("Invalid Output File_Type (8 characters are foreseen)"));	

 	//Write validityStartTime and validityStopTime in the YYYYMMDDTHHMMSS format
//	validityStartTime = validityStartTime.substr(0,4) + validityStartTime.substr(5,2) + validityStartTime.substr(8,2) + 'T'+ validityStartTime.substr(11,2) + validityStartTime.substr(14,2) + validityStartTime.substr(17,2);
//	validityStopTime = validityStopTime.substr(0,4) + validityStopTime.substr(5,2) + validityStopTime.substr(8,2) + 'T'+ validityStopTime.substr(11,2) + validityStopTime.substr(14,2) + validityStopTime.substr(17,2);

//	ACS_COND_THROW(instanceId.size()!=15, exSentinelProductNameException("Instance ID (YYYYMMDDTHHMMSS format is foreseen)"));
		
	
	//The Generating Centre has to be 3 chars long
    string genCentre = generatingCentre;
    if (genCentre.size() != 3) { // PRQA S 4400 2
        genCentre.resize(3,'_');
    }
	
	ACS_COND_THROW(classId.size()!=8, exSentinelProductNameException("Invalid class ID name (8 characters are foreseen)")); // PRQA S 3081 2
	
	ACS_COND_THROW(fileVers > 99, exSentinelProductNameException("Invalid File Versioning number (2 digits are foreseen)"));

   	char buffer[256]; // PRQA S 4403
	std::string extension = (ext.empty()? ext :"."+ext); // PRQA S 3384, 3385
   	sprintf(buffer, 
            "%s_%s_%s_%s_%s_%s___%s_%s_%02d%s", 
            missionID.c_str(), 
            dataSource.c_str(), 
            outFileType.c_str(), 
			start.YYYYMMDDThhmmss().c_str(), 
			stop.YYYYMMDDThhmmss().c_str(), 
			instanceId.YYYYMMDDThhmmss().c_str(),
			genCentre.c_str(), 
			classId.c_str(), 
            fileVers, 
            extension.c_str());

   	return buffer ;	
}



_ACS_END_NAMESPACE



