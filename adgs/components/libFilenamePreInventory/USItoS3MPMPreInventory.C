// PRQA S 1050 EOF
/*
	Copyright 2016-2021, Advanced Computer Systems , Inc.
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
	
        
*/ 

#include <USItoS3MPMPreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
	const string parFilesId = "ruleconfig";
	const size_t timesFormatLength = 15;
	const size_t parTimesFormatLength = 19;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(USItoS3MPMPreInventory)

USItoS3MPMPreInventory::USItoS3MPMPreInventory() : 
    	FilenamePreInventory()
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

USItoS3MPMPreInventory::~USItoS3MPMPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string USItoS3MPMPreInventory::getClassId()
{
     //return the class name
	return "USItoS3MPMPreInventory";
}
        

void USItoS3MPMPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	
// it is used so far for the following filetypes:
// PAR File to SPM e.g. ruleconfig.xml-2016-02-02_08-58-14
// subscription from CDS/ESA e.g. GSC_SUB_SCI_20160315_115000000.xml
// subscription from EUM e.g. eum_eop_S3_subscriptionInformation-20160315T112700Z.xml
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");
	
	size_t pos = 0;
	const string tag = "CreationDatePos";
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
// Creation date must be taken from name
	if( findConfigurationValue(fileType, rs, tag, pos) ) {
		ACS_COND_THROW( 0 == pos, exUSItoS3MPMPreInventoryException('\"' + tag + "\" not valorized") );
	}
	else {
		ACS_THROW( exUSItoS3MPMPreInventoryException('\"' + tag + "\" not occurring in configuration") );
	}

	if( filename.find(parFilesId) != string::npos ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << parFilesId << "\" substring found in name");
		
		// PAR File to SPM
		fne.CreationDate = filename.substr(pos, parTimesFormatLength);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation date extracted from filename=\""  << fne.CreationDate << '\"');
		// expected date format is e.g. 2016-02-02_08-58-14
		fne.CreationDate.erase(remove(fne.CreationDate.begin(), fne.CreationDate.end(), '-'), fne.CreationDate.end());	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation date after manipulation=\""  << fne.CreationDate << '\"');	
		
	}
	else {
		fne.CreationDate = filename.substr(pos, timesFormatLength);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation date extracted from filename=\""  << fne.CreationDate << '\"');
	}
	
	fne.CreationDate[8] = 'T';
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation date in final format=\""  << fne.CreationDate << '\"');	
	
	getBeginOfMission(rs , fne.StartDate) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configured Validity Start Time=\""  << fne.StartDate << '\"');
	
	getEndOfMission(rs , fne.StopDate );
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configured Validity Stop Time=\""  << fne.StopDate << '\"');


}


_ACS_END_NAMESPACE
