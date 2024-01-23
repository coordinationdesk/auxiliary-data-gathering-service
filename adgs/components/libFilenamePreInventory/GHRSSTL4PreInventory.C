// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
        
*/ 

#include <GHRSSTL4PreInventory.h>

#include <Filterables.h>
#include <StringUtils.h>
#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GHRSSTL4PreInventory)

string const GHRSSTL4PreInventory::InventoryTimeForMissionEnd	="99991231T235959";

GHRSSTL4PreInventory::GHRSSTL4PreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

GHRSSTL4PreInventory::~GHRSSTL4PreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string GHRSSTL4PreInventory::getClassId()
{
     //return the class name
	return "GHRSSTL4PreInventory";
}
        


void GHRSSTL4PreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");

	vector<string> tokens;
	StringUtils::Tokenize(filename, tokens, "-");
	
	throwConditionedException( tokens[0].size() != 14, exGHRSSTL4PreInventoryException("Wrong date lenghy: 14 chars expected") );
	string date = tokens[0].substr(0, 8) + "T" + tokens[0].substr(8, 6);
	
	fne.StartDate = date;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");
	
	fne.StopDate = InventoryTimeForMissionEnd;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");
	
	fne.CreationDate = date;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");
	
	fne.GeneratingCentre = tokens[1];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << "\"");
	
	string additionalSegregator = tokens[5];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Additional Segregator=\""  << additionalSegregator << "\"");
	
	fillGeoCoordinates(additionalSegregator, fne.geoCoords);
	
	string lastToken = tokens[tokens.size()-1];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Last token=\""  << lastToken << "\"");
	
	fne.File_Version = lastToken.substr(2, 4);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version=\""  << fne.File_Version << "\"");

}

void GHRSSTL4PreInventory::fillGeoCoordinates(const string& key, std::vector<std::string> & geoCoords)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
//     GLOB 																
//     90°S to 90°N, 180°W to 180°E 										
//     Global coverage data sets											
//     MED																	
//     30°N to 46°N, 18°W to 36.5°E 										
//     Mediterranean Sea area												
//     AUS																	
//     70S to 20N, 60E to 170W												
//     Australian regional analysis area									
//     NWE																	
//     43°N to 60°N, 13°W to 9°E											
//     North-West of Europe 												
//     NSEABALTIC															
//     66°N to 48°N, 10°W to 30°E											
//     North Sea and Baltic Region											
//     GAL																	
//     20°S to 20°N, 120°W to 69°W											
//     Area around the Galapagos Islands									
//     NCAMERICA															
//     20°S to 62°N, 165°W to 30°W											
//     Area around the east and west coasts of North and Central America	
	
	if( key == "GLOB" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Global coverage data sets");
// Postgresql 13 does not accept a geographic polygon for Global coverage
//		geoCoords.push_back("90");
//		geoCoords.push_back("-180");
//		geoCoords.push_back("-90");
//		geoCoords.push_back("180");
	}
	else if( key == "MED" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Mediterranean Sea area");
		geoCoords.push_back("46");
		geoCoords.push_back("-18");
		geoCoords.push_back("30");
		geoCoords.push_back("36.5");		
	} 
	else if( key == "AUS" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Australian regional analysis area");
		geoCoords.push_back("20");
		geoCoords.push_back("-170");
		geoCoords.push_back("-70");
		geoCoords.push_back("60");
	} 
	else if( key == "NWE" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"North-West of Europe");
		geoCoords.push_back("60");
		geoCoords.push_back("-13");
		geoCoords.push_back("43");
		geoCoords.push_back("9");
		
	} 
	else if( key == "NSEABALTIC" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"North Sea and Baltic Region");
		geoCoords.push_back("66");
		geoCoords.push_back("-10");
		geoCoords.push_back("48");
		geoCoords.push_back("30");
		
	} 
	else if( key == "GAL" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Area around the Galapagos Islands");
		geoCoords.push_back("20");
		geoCoords.push_back("-120");
		geoCoords.push_back("-20");
		geoCoords.push_back("-69");
		
	} 
	else if( key == "NCAMERICA" ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Area around the east and west coasts of North and Central America");
		geoCoords.push_back("62");
		geoCoords.push_back("-165");
		geoCoords.push_back("-20");
		geoCoords.push_back("-30");
	} 
	else {
		ACS_LOG_WARNING("Geographic Area not recognized!");
	}
	
	
}
_ACS_END_NAMESPACE
