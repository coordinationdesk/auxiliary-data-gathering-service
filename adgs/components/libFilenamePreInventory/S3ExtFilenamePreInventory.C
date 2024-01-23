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
	Revision 1.5  2016/05/19 11:30:57  francesco.avanzi
	minor bug concerning satelliteId fixed
	
	Revision 1.4  2016/04/20 08:10:43  francesco.avanzi
	minor issue involving a EndOfMission Tag handled
	
	Revision 1.3  2016/04/19 09:18:09  francesco.avanzi
	minor issues solved
	
	Revision 1.2  2016/04/04 15:29:28  francesco.avanzi
	introducing new preInventory S3ExtFilenamePreInventory.C
	
	Revision 1.1  2016/04/04 14:02:54  francesco.avanzi
	first addition
	
	Revision 1.2  2015/08/05 07:57:51  francesco.avanzi
	Minor issue involving checks fixed
	
	Revision 1.1  2015/08/04 16:02:05  francesco.avanzi
	added
	
	Revision 1.1  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: S3ExtFilenamePreInventory class developed.
	
	
	
        
*/ 

#include <S3ExtFilenamePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>
#include <DateTime.h>
#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3ExtFilenamePreInventory)

S3ExtFilenamePreInventory::S3ExtFilenamePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3ExtFilenamePreInventory::~S3ExtFilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3ExtFilenamePreInventory::getClassId()
{
     //return the class name
	return "S3ExtFilenamePreInventory" ;
}
        


void S3ExtFilenamePreInventory::parseFilename( const string& pathFileName, const string& fileType, FilenameElements& fne) // PRQA S 4211
{

	string filename = File::getFileName(pathFileName);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    
    string stopDate ;
    
    if(rs.hasKey("PreInventory.ConfigFilenamePreInventory.EndOfMission"))
    {
        rs.getValue("PreInventory.ConfigFilenamePreInventory.EndOfMission", stopDate) ;
    }
    else 
    {
        stopDate = "20991231T235959" ;
    }
    
    fne.File_Type = fileType ;
    if( "N1" == File::getExt(filename) )
    {
    	fne.StartDate =  DateTime(filename.substr(14,8) + "T" + filename.substr(23,6) ).asISO8601(0) ;
	fne.CreationDate = fne.StartDate ;
	fne.StopDate = DateTime(stopDate).asISO8601(0) ;
    }
    else if( string::npos != filename.find("_LAC_") )
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MODIS FILE" )
        string date = filename.substr(1, 13) ;    
        unsigned short date_d = atoi(date.substr(4,3).c_str()) ;
        unsigned short day ;
        unsigned short month ;
        unsigned short year = atoi(date.substr(0,4).c_str()) ;
        cout << "date: " << date << "\nyear: " <<  year << '\n' << "Date: " << date_d << '\n'  ;
        DateTime::UTCD::dayAndMonthFromDOY( date_d, year, day, month ) ;
		
        DateTime::UTCD tmp( year, month, day, atoi(date.substr(7,2).c_str()), atoi(date.substr(9,2).c_str()), atoi(date.substr(11,2).c_str()), 0, 0) ;
        
        DateTime dt(tmp) ;
        
        fne.StartDate = dt.asISO8601(0) ;
        fne.CreationDate = fne.StartDate ;
        fne.StopDate = DateTime(stopDate).asISO8601(0) ;
    }
    else if( string::npos !=  filename.find("_npp_"))    
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "VIIRS FILE" )
        DateTime dt(filename.substr(70, 8) +"T" + filename.substr(78, 6)+ "." + filename.substr(74, 6) ) ;
        fne.StartDate =  dt.asISO8601(6) ;
        fne.CreationDate = fne.StartDate ;
        fne.StopDate = DateTime(stopDate).asISO8601(0) ;
    }
    else if( string::npos !=  filename.find("_M0"))
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "EPF FILE" )
        fne.StartDate = DateTime( filename.substr(16, 8) + "T" + filename.substr(24,6) ).asISO8601(0) ;
        fne.StopDate  = DateTime( filename.substr(32, 8) + "T" + filename.substr(40,6) ).asISO8601(0) ;
        fne.CreationDate = DateTime( filename.substr(52, 8) + "T" + filename.substr(60,6) ).asISO8601(0) ;
    }
    else
    {
        ACS_THROW( exIllegalValueException("The file does not match any of the given name-conv.") ) ;
    }
}



_ACS_END_NAMESPACE
