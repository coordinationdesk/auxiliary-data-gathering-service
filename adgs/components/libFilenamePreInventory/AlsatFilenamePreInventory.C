// PRQA S 1050 EOF
/*
	Copyright 2013-2015, Advanced Computer Systems , Inc.
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
	Revision 1.1  2015/08/04 16:02:05  francesco.avanzi
	added
	
	Revision 1.1  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: AlsatFilenamePreInventory class developed.
	
	
	
        
*/ 

#include <AlsatFilenamePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(AlsatFilenamePreInventory)

AlsatFilenamePreInventory::AlsatFilenamePreInventory() : 
    	FilenamePreInventory()
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

AlsatFilenamePreInventory::~AlsatFilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string AlsatFilenamePreInventory::getClassId()
{
     //return the class name
	return "AlsatFilenamePreInventory";
}
        


void AlsatFilenamePreInventory::parseFilename( const string& pathFileName, const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
// it is used for Alsat 1B Sensor Model Aux data file (e.g. AB_SNM_20140203120000_99999999999999_0001.xml) detailed in Alsat-1B Geometric Processor Interface Control Document ALSAT1B-ID-AC-GS-0500-1.0
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');

	fne.Satellite = filename.substr(0, 2);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

    fne.File_Type = filename.substr(3, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << '\"');
	
	fne.StartDate = filename.substr(7, 8) + 'T' + filename.substr(15, 6);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Start Time=\""  << fne.StartDate << '\"');
	
	fne.CreationDate = fne.StartDate;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << '\"');

	{
		string tmp = filename.substr(22, 8) + 'T' + filename.substr(30, 6); 
		fne.StopDate = convertUncorrectDate( getPluginConfSpaceId(), tmp);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Stop Time=\""  << fne.StopDate << '\"');
    if ( '.' != filename[36] )
    {
	    fne.File_Version = filename.substr(37, 4) ;
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version=\""  << fne.File_Version << '\"');
    }
    else
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version not found" );
    }
    
}


short int AlsatFilenamePreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	
	ACS_COND_THROW(satellite.empty(), exAlsatFilenamePreInventoryException("Satellite id not valorized!"));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.idExact(satellite);

}


_ACS_END_NAMESPACE
