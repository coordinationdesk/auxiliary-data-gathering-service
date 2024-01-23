// PRQA S 1050 EOF
/*
	Copyright 2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.
		
*/

#include <S6MOG2DFilenamePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
	const size_t filetypeDefaultLength = 13;
	const size_t generatingCentreDefaultStartPos = 83;
	const size_t generatingCentreDefaultLength = 4 ;
	const size_t fileClassDefaultStartPos = 88;
	const size_t fileClassDefaultLength = 3;
	
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(S6MOG2DFilenamePreInventory)

S6MOG2DFilenamePreInventory::S6MOG2DFilenamePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S6MOG2DFilenamePreInventory::~S6MOG2DFilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S6MOG2DFilenamePreInventory::getClassId()
{
     //return the class name
	return "S6MOG2DFilenamePreInventory";
}
        


void S6MOG2DFilenamePreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	// e.g. S6__P4_2__MOGP_AX_20210629T180000_20210629T180000_20210630T095521__________________SALP_OPE_ST____.DBL
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");
	
	size_t startPos = 0;
	size_t fieldLength = 3;
	fne.Satellite = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

	startPos += ( fieldLength + 1 );
	fieldLength = filetypeDefaultLength ;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	if( findConfigurationValue(fileType, rs, "FileTypeLength", fieldLength) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileTypeLength=" << fieldLength );
	}
	
	const size_t filenameLength = filename.length() ; 
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "Product Type" ) ;

    fne.File_Type = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << '\"');

	startPos += ( fieldLength + 1 );
	fieldLength = 15;
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "Start Time" ) ;
    fne.StartDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << '\"');
    
	startPos += ( fieldLength + 1 );
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "Stop Time" ) ;
	fne.StopDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << '\"');
	
	startPos += ( fieldLength + 1 );
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "Creation Date" ) ;	
	fne.CreationDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Date=\""  << fne.CreationDate << '\"');
	
	startPos = generatingCentreDefaultStartPos ;
	if( findConfigurationValue(fileType, rs, "GeneratingCentrePosition", startPos) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GeneratingCentrePosition=" << startPos );
	}

	fieldLength = generatingCentreDefaultLength ;
	if( findConfigurationValue(fileType, rs, "GeneratingCentreLength", fieldLength) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GeneratingCentreLength=" << fieldLength );
	}
	
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "Generating Centre" ) ;
    fne.GeneratingCentre = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << '\"');
	
	startPos = fileClassDefaultStartPos ;
	if( findConfigurationValue(fileType, rs, "FileClassPosition", startPos) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileClassPosition=" << startPos );
	}

	fieldLength = fileClassDefaultLength ;
	if( findConfigurationValue(fileType, rs, "FileClassLength", fieldLength) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FileClassLength=" << fieldLength );
	}
	
	checkFieldLenWithinFilename(startPos, fieldLength, filenameLength, "File Class" ) ;
	string tmpFileClass = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File class as extracted from filename=\""  << tmpFileClass << '\"');
	
	fne.ClassId = convertOriginalFileClass( getPluginConfSpaceId(), fileType, tmpFileClass ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File class after conversion by LUT=\""  << fne.ClassId << '\"');
	
	
}


short int S6MOG2DFilenamePreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_COND_THROW( satellite.empty(), exS6MOG2DFilenamePreInventoryException("Satellite not valorized!") ) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.idExact(satellite);

}


_ACS_END_NAMESPACE
