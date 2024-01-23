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

#include <S3GPDFilenamePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
	const size_t versionDefaultStartPos = 61;
	const size_t versionDefaultLength = 3;
	
	bool IsNotDigit(char c) { return not isdigit(c) ; }
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3GPDFilenamePreInventory)

S3GPDFilenamePreInventory::S3GPDFilenamePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3GPDFilenamePreInventory::~S3GPDFilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3GPDFilenamePreInventory::getClassId()
{
     //return the class name
	return "S3GPDFilenamePreInventory";
}
        


void S3GPDFilenamePreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	// S3A_GPD_20180820T230000_20180822T010000_20210425T000000_xxxx_Vxx.nc
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");
	
	size_t startPos = 0;
	size_t fieldLength = 3;
	fne.Satellite = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

	startPos += ( fieldLength + 1 );
    fne.File_Type = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << '\"');

	startPos += ( fieldLength + 1 );
	fieldLength = 15;
    fne.StartDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");
    
	startPos += ( fieldLength + 1 );
	fne.StopDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");
	
	startPos += ( fieldLength + 1 );
	fne.CreationDate = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");
	
	// get version...
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	startPos = versionDefaultStartPos ;
	if( findConfigurationValue(fileType, rs, "VersionPosition", startPos) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VersionPosition=" << startPos );
	}

	fieldLength = versionDefaultLength ;
	if( findConfigurationValue(fileType, rs, "VersionLength", fieldLength) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VersionLength=" << fieldLength );
	}
	
	checkFieldLenWithinFilename(startPos, fieldLength, filename.length(), "Version" ) ;
	
	string fileVersion = filename.substr(startPos, fieldLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Version field from filename=\""  << fileVersion << "\"");
	
	
	if( fileVersion[0] != 'V' && fileVersion[0] != 'v' ) {
		ostringstream exMsg ;
		exMsg << "Version field of \"" << filename << "\" filename begins with unrecognized character: " << fileVersion[0] << " (\'V\' expected)" ;
		ACS_THROW( exS3GPDFilenamePreInventoryException( exMsg.str() ) ) ; 
	}
	
	{
// check numerical values only occur...
		string tmp = fileVersion.substr(1, (fieldLength-1) ) ;
		
		if( find_if (tmp.begin(), tmp.end(), IsNotDigit) != tmp.end() ) {
			ostringstream exMsg ;
			exMsg << "Version fields of \"" << filename << "\" filename does not contain numerical values: " << tmp ;
			ACS_THROW( exS3GPDFilenamePreInventoryException( exMsg.str() ) ) ; 
			
		}
		istringstream is ( tmp, istringstream::in);
		int fileVersion ;
		is >> fileVersion;
		fne.File_Version = getFormattedVersion(fileVersion);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version=\""  << fne.File_Version << '\"');
	}
	

}


short int S3GPDFilenamePreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{	
	ACS_COND_THROW( satellite.empty(), exS3GPDFilenamePreInventoryException("Satellite not valorized!") ) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.idExact(satellite);

}


_ACS_END_NAMESPACE
