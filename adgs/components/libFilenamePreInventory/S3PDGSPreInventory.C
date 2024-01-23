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
	Revision 1.4  2015/05/29 16:21:53  lucio.pulvirenti
	S3MPM-504: S3PDGSPreInventory class modified to manage ADC Reports.
	
	Revision 1.3  2015/03/19 15:31:07  lucio.pulvirenti
	S3MPM-359: NameIdAssociator::id method turned into NameIdAssociator::idExact for t_satellites.satelliteacronym for the ambiguity between S# and S#_.
	
	Revision 1.2  2015/01/15 16:32:22  lucio.pulvirenti
	S3GEOCAL-11: added getSatelliteId private method. If associateSatellite from conf is true, fill satelliteId.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
        
*/ 

#include <S3PDGSPreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3PDGSPreInventory)

S3PDGSPreInventory::S3PDGSPreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3PDGSPreInventory::~S3PDGSPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3PDGSPreInventory::getClassId()
{
     //return the class name
	return "S3PDGSPreInventory";
}
        


void S3PDGSPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");

	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	bool associateSatellite = true;
	if( findConfigurationValue(fileType, rs, "AssociateSatellite", associateSatellite) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AssociateSatellite=" << boolalpha << associateSatellite);
		if( associateSatellite ) {
			fne.Satellite = filename.substr(0, 3);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');
		}
	}

// No naming convention for ADC Reports available: is looks like that filetype is 10 char long instead of 11	
	bool adcReport = false;
	size_t filetypeLength = 11;
	if( findConfigurationValue(fileType, rs, "ADCReport", adcReport) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ADCReport=" << boolalpha << adcReport);
		if( adcReport ) {
			filetypeLength = 10;
		}
	}
	

    fne.File_Type = filename.substr(4, filetypeLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << "\"");
	
	size_t startPos = 4 + (filetypeLength+1);
	
    fne.StartDate = filename.substr(startPos, 15);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");
    
	startPos+=(15+1);
	fne.StopDate = filename.substr(startPos, 15);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");
	
	startPos+=(15+1);
	fne.CreationDate = filename.substr(startPos, 15);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");
	
#warning "Get orbit number for products"
	
	startPos+=34;
	if( adcReport ) {
// No length is defined for ADC Report generating centre: it could be either 4 or 3 chars
		size_t endPos = filename.find('_', startPos);
		if( endPos != string::npos ) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Extracting Generating Centre from pos "  << startPos << " to pos " << endPos);
			fne.GeneratingCentre = filename.substr(startPos, (endPos-startPos));
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << '\"');
		}
	}
	else {	
		fne.GeneratingCentre = filename.substr(startPos, 3);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << "\"");

		string classId = filename.substr(86, 8);
		if( string("________") == classId ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Class Id Not Applicable");
		}
		else {
			fne.ClassId = classId;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Class Id=\""  << fne.ClassId << "\"");
		}
	}

}


short int S3PDGSPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
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
