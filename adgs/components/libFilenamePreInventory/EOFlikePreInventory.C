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
	Revision 1.4  2015/11/30 14:22:33  francesco.avanzi
	./conf/S3MPM/libFilenamePreInventory.so.xml
	
	Revision 1.3  2015/10/06 15:22:12  lucio.pulvirenti
	S3MPM-628: DoyDateFormat tag added to configuration to manage DOYDate properly. Flag added to getDateFromFilename and getStdFormatDate method added to
	convert DOYDate to std one (e.g. YYMMDDThhmmss).
	
	Revision 1.2  2015/06/05 13:50:41  lucio.pulvirenti
	S3MPM-504: Check on dates demanded to new getDateFromFilename method. Also OLTC Data Files managed.
	
	Revision 1.1  2015/06/04 16:03:10  lucio.pulvirenti
	S3MPM-504: First issue.
	
	
        
*/ 

#include <EOFlikePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    const size_t EOFfiletypeLength = 10;
	const size_t timesFormatLength = 15;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFlikePreInventory)

EOFlikePreInventory::EOFlikePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

EOFlikePreInventory::~EOFlikePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string EOFlikePreInventory::getClassId()
{
     //return the class name
	return "EOFlikePreInventory";
}
        
std::string  EOFlikePreInventory::getGeneratingCentreFromFilename(const std::string& filename, size_t pos)
{
    return filename.substr(pos, 3);
}


void EOFlikePreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	
// it is used so far for
// S3MPM Dissemination Reports: S3__OPER_REP_DR_<centre>_<YYYYMMDDThhmmss>_I<NNNN>.xml (D006.D_S3PDGS ICD 014 -i2r4- S3PDGS ICD - Volume 2)
// S3MPM Manoeuvre Planning Ground track Evolution File: MMM_CCCC_TTTTTTTTTT_<instance_id> (SENTINELS FOS File Format Specification GM-IC-ESC-FS-3001 issue 1.8 section 19)
// S3MPM OLTC Data Files: S3A_OPER_OBS_MIMG___IPTOLTC__20140424_235959_0001.TGZ (Sentinel3 PDGS <-> SALP ICD S3A-ID-M-00012-CN issue 1.3 App 8)
// S3MPM SOR Files: S1A_OPER_SOR_PDS001_2012123T123456_0001.doc (SENTINELS FOS File Format Specification GM-IC-ESC-FS-3001 issue 1.8 section 21)
	
// WARNING At least one date between creation and start validity must occur.
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");

	fne.Satellite = filename.substr(0, 3);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite=\""  << fne.Satellite << '\"');
	
	fne.ClassId = filename.substr(4, 4);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Class=\""  << fne.ClassId << '\"');

	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	size_t filetypeLength = EOFfiletypeLength;
	if( findConfigurationValue(fileType, rs, "FtypeFieldLength", filetypeLength) ) {
		ACS_COND_THROW( 0 == filetypeLength, exEOFlikePreInventoryException("FtypeFieldLength tag not valorized") );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FtypeFieldLength=" << filetypeLength );
	}
	
	size_t startPos = 9;
	if( (startPos + filetypeLength) > File::removeFExt(filename).length() ) {
		ostringstream exMsg;
		exMsg << "Cannot extract " << filetypeLength << " chars long filetype starting from pos " << startPos << " from filename \"" << filename << '\"';
		ACS_THROW( exEOFlikePreInventoryException(exMsg.str() ) );
	}
    fne.File_Type = filename.substr(startPos, filetypeLength);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File type=\""  << fne.File_Type << '\"');
	
	
	bool DOYDateFormat = false;
	string tag = "DOYDateFormat";
	if( findConfigurationValue(fileType, rs, tag, DOYDateFormat) ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DOYDateFormat="  << boolalpha << DOYDateFormat);
	}
	
	startPos = 0;
	tag = "CreationDatePos";
// If creation date does not occur set it to start validity.
	if( findConfigurationValue(fileType, rs, tag, startPos) ) {
		ACS_COND_THROW( 0 == startPos, exEOFlikePreInventoryException('\"' + tag + "\" not valorized") );
		fne.CreationDate = getDateFromFilename(filename, startPos, DOYDateFormat);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << '\"');
	}

	startPos = 0;
	tag = "ValidityStartTimePos";
// If start validity date does not occur set it to creation, if occurring otherwise throw exception.
	if( findConfigurationValue(fileType, rs, tag, startPos) ) {
		ACS_COND_THROW( 0 == startPos, exEOFlikePreInventoryException('\"' + tag + "\" not valorized") );
		fne.StartDate = getDateFromFilename(filename, startPos, DOYDateFormat);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Start Time=\""  << fne.StartDate << '\"');
		if( fne.CreationDate.empty() ) {
			fne.CreationDate = fne.StartDate;
		}
	}
	else {
		ACS_COND_THROW( fne.CreationDate.empty(), exEOFlikePreInventoryException("Neither Creation nor Start validity Dates occurring") );
		fne.StartDate = fne.CreationDate;
	}

	startPos = 0;
	tag = "ValidityStopTimePos";
	if( findConfigurationValue(fileType, rs, tag, startPos) ) {
		ACS_COND_THROW( 0 == startPos, exEOFlikePreInventoryException('\"' + tag + "\" not valorized") );
		fne.StopDate = getDateFromFilename(filename, startPos, DOYDateFormat);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity Stop Time=\""  << fne.StopDate << '\"');
	}
	else {
		fne.StopDate = convertUncorrectDate( getPluginConfSpaceId(), "99999999T999999");
	}
    startPos = 0;
    tag = "GeneratingCentrePos" ;
    if( findConfigurationValue(fileType, rs, tag, startPos) )
    {
        ACS_COND_THROW( 0 == startPos, exEOFlikePreInventoryException('\"' + tag + "\" not valorized") );
        fne.GeneratingCentre = getGeneratingCentreFromFilename(filename, startPos);
        
    }
	startPos = 0; 
	tag = "VersionPos";
	if( findConfigurationValue(fileType, rs, tag, startPos) ) {
		ACS_COND_THROW( 0 == startPos, exEOFlikePreInventoryException('\"' + tag + "\" not valorized") );
		if( (startPos + 4) > File::removeFExt(filename).length() ) {
			ostringstream exMsg;
			exMsg << "Cannot extract 4 chars long Version starting from pos " << startPos << " from filename \"" << filename << '\"';
			ACS_THROW( exEOFlikePreInventoryException(exMsg.str() ) );
		}
		fne.File_Version = filename.substr(startPos, 4);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File version=\""  << fne.File_Version << '\"');
	}


}


short int EOFlikePreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
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

string EOFlikePreInventory::getDateFromFilename(const std::string& filename, size_t pos, bool DOYFmt)
{
	size_t currTimesFormatLength = timesFormatLength;
	if( DOYFmt ) {
		currTimesFormatLength--;
	}
	
	if( (pos + currTimesFormatLength) > File::removeFExt(filename).length() ) {
		ostringstream exMsg;
		exMsg << "Cannot extract date time " << currTimesFormatLength << " chars long starting from pos " << pos << " from filename \"" << filename << '\"';
		ACS_THROW( exEOFlikePreInventoryException(exMsg.str() ) );
	}
	string date = filename.substr(pos, currTimesFormatLength);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Date extracted from \"" << filename << "\" filename starting from pos " << pos << ": " << date);
	size_t Tpos = date.find('T');
	if( Tpos == string::npos ) {
		size_t firstPartLength = 8;
		if(DOYFmt) {
			firstPartLength = 7;
		} 
		date.replace(firstPartLength,1,1,'T'); 
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Date format corrected: " << date);
	}
	
	bool converted;
	date = convertUncorrectDate( getPluginConfSpaceId(), date, &converted );
	if( DOYFmt && not converted ) {
		date = getStdFormatDate(date);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Standard day format from DOY: " << date);
	}
	return date;
	
}

string EOFlikePreInventory::getStdFormatDate(const string & DOYDate)
{
	// it is assumed DOYDate fmt is YYYYDDDTHHMMSS
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DOY date=" << DOYDate );
	unsigned short y;
	{
		string tmp = DOYDate.substr(0, 4);
		istringstream iss(tmp);
		iss >> y;
	}
	
	size_t Tpos = DOYDate.find('T');
	unsigned short doy;
	{
		size_t doyLenght = Tpos - 4;
		string tmp = DOYDate.substr(4, doyLenght);
		istringstream iss(tmp);
		iss >> doy;
	}
	
	DateTime::UTCD u = DateTime::UTCD::dayOfMonth(doy,y) ;
	DateTime date(u);
	string dateStr = date.YYYYMMDDThhmmss();
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Date converted from year=" << y << " doy=" << doy << " is " << dateStr );
	
	string hrs = DOYDate.substr(Tpos+1, 6);
	return dateStr.replace(9, hrs.length(), hrs);

	
}

_ACS_END_NAMESPACE
