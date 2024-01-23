// PRQA S 1050 EOF
/*
	Copyright 2014-2021, Advanced Computer Systems , Inc.
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
	Revision 1.3  2015/01/21 14:23:26  lucio.pulvirenti
	S3GEOCAL-11: generating centre no more extracted by filename.
	
	Revision 1.2  2014/09/30 08:14:46  lucio.pulvirenti
	Version no more taken from filename. updateProductAndMetadata added to IF not to write DUP string into qualityInfo t_inventory field.
	
	Revision 1.1  2014/09/29 14:35:52  lucio.pulvirenti
	First issue.
	
	Revision 1.2  2013/09/20 11:17:23  lucio.pulvirenti
	Geographic coordinates put into filename elements proper vector.
	
	Revision 1.1.1.1  2013/09/19 17:39:45  lucio.pulvirenti
	Import libFilenamePreInventory
	
        
*/ 

#include <S3OpticalProductsPreInventory.h>


#include <Filterables.h>
#include <StringUtils.h>
#include <NameIdAssociator.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3OpticalProductsPreInventory)


S3OpticalProductsPreInventory::S3OpticalProductsPreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3OpticalProductsPreInventory::~S3OpticalProductsPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3OpticalProductsPreInventory::getClassId()
{
     //return the class name
	return "S3OpticalProductsPreInventory";
}
        


void S3OpticalProductsPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	

// naming convention format MMM_SS_TTTTTTTTTTTT_yyyymmddThhmmss_YYYYMMDDTHHMMSS_OOOOOO_DDDDD_LLL_GGG_CCCC_VV.ext according to S3-RS-ACR-SY-0001 issue 7 Rev 0

	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');

	fne.Satellite = filename.substr(0, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

    fne.File_Type = filename.substr(7, 12);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << '\"');

	size_t instanceIdStartPos = 20 ; // may be useful for NAVATT and got by configuration
	size_t startPos = instanceIdStartPos;
    fne.StartDate = filename.substr(startPos, 15);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << '\"');
    
	startPos += 16;
	fne.StopDate = filename.substr(startPos, 15);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << '\"');

	fne.CreationDate = fne.StartDate;
	
	startPos += 16;
	{
		string orbit = filename.substr(startPos, 6);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Absolute orbit number from name=\""  << orbit << '\"');
		
// remove padding zero
 		fne.OrbitNumber = StringUtils::trimBegin(orbit, '0');
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Orbit number=\""  << fne.OrbitNumber << '\"');

		
	startPos += 21;	
	fne.ClassId = "TEST";
	{
		string classId = filename.substr(startPos, 4);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "File class from name=\""  << classId << '\"');
		if( classId.substr(0, 2) == string("OP") ) {
			fne.ClassId = "OPER";
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File classId=\""  << fne.ClassId << '\"');
		
}

short int S3OpticalProductsPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.idExact(satellite);

}

void S3OpticalProductsPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << "\"");
			
}


_ACS_END_NAMESPACE
