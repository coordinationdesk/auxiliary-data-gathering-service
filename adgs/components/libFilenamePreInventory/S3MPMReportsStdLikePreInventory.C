// PRQA S 1050 EOF
/*
	Copyright 2013-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2015/06/19 09:59:07  lucio.pulvirenti
	S3MPM-528: file type is built as SS_L_TTTTTT + _ + <Prefix>. convertUncorrectDate method used.
	
	Revision 1.1  2015/06/18 15:44:56  lucio.pulvirenti
	S3MPM-528: First Issue.
	
       
*/ 

#include <S3MPMReportsStdLikePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3MPMReportsStdLikePreInventory)

S3MPMReportsStdLikePreInventory::S3MPMReportsStdLikePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3MPMReportsStdLikePreInventory::~S3MPMReportsStdLikePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3MPMReportsStdLikePreInventory::getClassId()
{
     //return the class name
	return "S3MPMReportsStdLikePreInventory";
}
        


void S3MPMReportsStdLikePreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	

// Used for Reports following naming convention <Id>_MMM_SS_L_TTTTTT_[XXX]_<Coverage period>_<Creation Date>_<MPMF ID>.<Ext> (S3PDGS ICD Vol.3 Mission Performance Monitoring [S3PDGS.ICD.015 2.1])
// XXX in the name is cycle and its string length (possibly 0, i.e. not occurring) must be configured
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");

	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	string identifier;
	if( findConfigurationValue(fileType, rs, "Identifier", identifier) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Identifier from configuration=\"" << identifier << '\"');
	}
	else {
		size_t pos = filename.find('_');
		ACS_COND_THROW( pos == string::npos, exS3MPMReportsStdLikePreInventoryException("Cannot find \"_\" char in \""+filename+"\" name") );
		identifier = filename.substr(0, pos);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Identifier extracted from name=\"" << identifier << '\"');
	}
	ACS_COND_THROW( identifier.empty(), exS3MPMReportsStdLikePreInventoryException("Identifier has null length for \""+fileType+"\" file type") );
	
	size_t startPos = identifier.length() + 1;
	size_t fieldLength = 3;
	fne.Satellite = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

	startPos += ( fieldLength + 1 );
	fieldLength = 11;
	{
		string tmp = filename.substr(startPos, fieldLength);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Product type from filename=\""  << tmp << '\"');
    	fne.File_Type = tmp + '_' + fileType;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Product type =\""  << fne.File_Type << '\"');

	size_t cycleLength = 0;
	if( findConfigurationValue(fileType, rs, "CycleLength", cycleLength) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CycleLength=" << cycleLength );
		ACS_COND_THROW( cycleLength < 0, exS3MPMReportsStdLikePreInventoryException("CycleLength configured with a negative value (positive is required)") );
		if( cycleLength > 0 ) {
			startPos += ( fieldLength + 1 );
			fieldLength = cycleLength;
			// cycle not used so far since no field occurring in t_inventory where to store. Extracted in debug mode only
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				string cycle = filename.substr(startPos, fieldLength);
				ACS_LOG_DEBUG(SimpleDebugSignature << "Cycle from filename=\""  << cycle << '\"');
			ACS_CLASS_END_DEBUG
		}
	}
	
	startPos += ( fieldLength + 1 );
	fieldLength = 15;
    fne.StartDate = convertUncorrectDate( getPluginConfSpaceId(), filename.substr(startPos, fieldLength));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");
    
	startPos += ( fieldLength + 1 );
	fieldLength = 15;
	fne.StopDate = convertUncorrectDate( getPluginConfSpaceId(), filename.substr(startPos, fieldLength));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");
	
	startPos += ( fieldLength + 1 );
	fieldLength = 15;
	fne.CreationDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");
	
	startPos += ( fieldLength + 1 );
	fieldLength = 3;
	fne.GeneratingCentre = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << '\"');


}


short int S3MPMReportsStdLikePreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	short int satelliteId = 0;
	if( not satellite.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
		NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
		nameIdAssociator.load() ;
		satelliteId = nameIdAssociator.idExact(satellite);
	}
	
	return satelliteId;

}


_ACS_END_NAMESPACE
