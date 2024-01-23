// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 1.1  2014/03/11 17:02:14  lucio.pulvirenti
	Work in progress.
	
	
        
*/ 

#include <ESAStandardPreInventory.h>

#include <NameIdAssociator.h>

#include <Filterables.h>
#include <StringUtils.h>
#include <DateTime.h>
#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ESAStandardPreInventory)

const map< string, string > ESAStandardPreInventory::_satellitesIdMap = ESAStandardPreInventory::createSatellitesIdMap();

ESAStandardPreInventory::ESAStandardPreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

ESAStandardPreInventory::~ESAStandardPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string ESAStandardPreInventory::getClassId()
{
     //return the class name
	return "ESAStandardPreInventory";
}
        


void ESAStandardPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");
	
	string satelliteName = filename.substr(0, 2);
	map<string, string>::const_iterator it = _satellitesIdMap.find(satelliteName);
	throwConditionedException(it == _satellitesIdMap.end(), exESAStandardPreInventoryException("Can't find \"" + satelliteName + "\" key in satellites map"));	 // PRQA S 3081
	fne.Satellite = it->second;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << "\"");
	
#warning "Extract here mission, filetype and other parameters of interest"
	
	vector<string> tokens;
	StringUtils::Tokenize(filename.substr(16), tokens, "_");

	fne.StartDate = tokens[0];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");

	fne.StopDate = tokens[1];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");

	fne.GeneratingCentre = tokens[2];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << "\"");
	
	{
		string orbit = tokens[3];
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Absolut orbit number from name=\""  << orbit << "\"");
		istringstream iss(orbit);
		int orbitNum ;
		iss >> orbitNum;
		if( 0 != orbitNum ) {
			fne.OrbitNumber = orbit;
		}
	}
	
	fne.CreationDate = fne.StartDate;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");

}

short int ESAStandardPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satellitename" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.id(satellite);

}


_ACS_END_NAMESPACE
