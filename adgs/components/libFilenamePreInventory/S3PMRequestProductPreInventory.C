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
	Revision 1.1  2015/06/05 16:19:52  lucio.pulvirenti
	S3MPM-504: First issue.
	
	
        
*/ 

#include <S3PMRequestProductPreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3PMRequestProductPreInventory)

S3PMRequestProductPreInventory::S3PMRequestProductPreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3PMRequestProductPreInventory::~S3PMRequestProductPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3PMRequestProductPreInventory::getClassId()
{
     //return the class name
	return "S3PMRequestProductPreInventory";
}
        


void S3PMRequestProductPreInventory::parseFilename( const string& pathFileName, const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
// it is used for S3MPM Request Product: <MMM>_REP_PR_<GGG>_<P>_<PM>_<PdR>_<YYYYMMDDThhmmssSSS>	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');

	fne.Satellite = filename.substr(0, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

    fne.File_Type = filename.substr(4, 10);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product type=\""  << fne.File_Type << '\"');

	fne.GeneratingCentre = filename.substr(11, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << '\"');
	
	fne.ClassId = "TEST";
	char platformId = filename.at(15);
	if( 'O' == platformId ) {
		fne.ClassId = "OPER";
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Class Id=\""  << fne.ClassId << '\"');
	
	{
		string tmp = filename.substr(31, 15) + '.' + filename.substr(46, 3) ;
		fne.CreationDate = tmp;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << '\"');
	
	fne.StartDate = fne.CreationDate;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Start Time=\""  << fne.StartDate << '\"');

	fne.StopDate = convertUncorrectDate( getPluginConfSpaceId(), "99999999T999999");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Stop Time=\""  << fne.StopDate << '\"');

}


short int S3PMRequestProductPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
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
