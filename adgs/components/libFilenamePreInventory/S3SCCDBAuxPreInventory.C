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
	Revision 1.2  2015/01/15 12:04:10  lucio.pulvirenti
	S3GEOCAL-11: extended to SL_1_GEX_AX, OL_1_CAL_AX, OL_1_INS_AX file types.
	
	Revision 1.1  2014/09/30 15:44:55  lucio.pulvirenti
	First issue.
	
        
*/ 

#include <S3SCCDBAuxPreInventory.h>


#include <Filterables.h>
#include <NameIdAssociator.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3SCCDBAuxPreInventory)

string const S3SCCDBAuxPreInventory::InventoryTimeForMissionEnd	="99991231T235959";


S3SCCDBAuxPreInventory::S3SCCDBAuxPreInventory() : 
    	FilenamePreInventory()
{	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3SCCDBAuxPreInventory::~S3SCCDBAuxPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3SCCDBAuxPreInventory::getClassId()
{
     //return the class name
	return "S3SCCDBAuxPreInventory";
}
        


void S3SCCDBAuxPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	

// naming convention format S3X_xx_CCDB_[CHAR|CALI]_<extended name>.yyyymmdd[T]hhmmss_[yyyymmddThhmmss_]i.nc4  according to S3-IF-TAF-SY-00861 issue 5 and S.Mica proposal (see S3GEOCAL-11 S.Mica comment of 16/Dec/2014)

	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');

	fne.Satellite = filename.substr(0, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');

 	size_t startPos = filename.find('.');
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\".\" character found in position " << startPos);
	
	ACS_COND_THROW( string::npos == startPos, exS3SCCDBAuxPreInventoryException("\".\" character not found in \"" + filename + "\" filename") );
	
	size_t dateDelimiter = filename.find('_', startPos);
	ACS_COND_THROW( string::npos == dateDelimiter, exS3SCCDBAuxPreInventoryException("\"_\" character not found after \".\" in \"" + filename + "\" filename") );
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "First date delimiter=" << dateDelimiter );
	
	startPos++;
	size_t DateLength = dateDelimiter - startPos ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "First date length=" << DateLength );
	
	string creationDate = filename.substr(startPos, DateLength);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Creation date from filename=\"" << creationDate << '\"');
	
	fne.CreationDate = creationDate;
	if( creationDate.find('T') == string::npos ) {
		ACS_COND_THROW( (creationDate.find_first_not_of("0123456789") != string::npos), 
						exS3SCCDBAuxPreInventoryException(creationDate + " date extracted from filename \"" + filename + "\" does not contain all numeric characters" ) );
		fne.CreationDate = creationDate.substr(0, 8) + "T" + creationDate.substr(8, 6);
	}
	else {
		fne.CreationDate = creationDate;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\"" << fne.CreationDate << '\"');
// just a check
	try {
		DateTime d(fne.CreationDate);
	}
	catch( exception &)
	{
		ACS_THROW(exS3SCCDBAuxPreInventoryException("Wrong format creation date: "+fne.CreationDate));
	}
	
	
	startPos+=(DateLength+1);
	if( startPos >= filename.length() ) {
		ostringstream exMsg;
		exMsg << "The new computed start position " << startPos << " is outside the filename length " << filename.length();
		ACS_THROW(exS3SCCDBAuxPreInventoryException(exMsg.str()));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "New start position=" << startPos );
	
	dateDelimiter = filename.find('_', startPos);
	if( dateDelimiter != string::npos ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Second date delimiter=" << dateDelimiter );
		DateLength = dateDelimiter - startPos ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Second date length=" << DateLength );
		string validityStart = filename.substr(startPos, DateLength);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Validity start date from filename=\"" << validityStart << '\"');
	// just a check
		try {
			DateTime d(validityStart);
		}
		catch( exception &)
		{
			ACS_THROW(exS3SCCDBAuxPreInventoryException("Wrong format validity start date: "+validityStart));
		}
		fne.StartDate = validityStart;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity start time not occurring after the creation date");
    	fne.StartDate = fne.CreationDate;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\"" << fne.StartDate << '\"');
    
	fne.StopDate = InventoryTimeForMissionEnd;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\"" << fne.StopDate << '\"');
		
}

short int S3SCCDBAuxPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.idExact(satellite);

}

void S3SCCDBAuxPreInventory::updateProductAndMetadata( const std::string& newVersion, Metadata& metadata)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	metadata.setMetadataValue( "File_Version", newVersion);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File_Version field updated with the value \"" << newVersion << "\"");
			
}


_ACS_END_NAMESPACE
