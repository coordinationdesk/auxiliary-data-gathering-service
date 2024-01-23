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
	Revision 1.4  2016/05/31 09:34:12  francesco.avanzi
	issue involving wrong filename parsing fixed
	
	Revision 1.3  2016/05/19 14:31:27  francesco.avanzi
	minor bug concerning satelliteId fixed
	
	Revision 1.2  2016/04/19 09:18:09  francesco.avanzi
	minor issues solved
	
	Revision 1.1  2016/04/18 19:22:42  francesco.avanzi
	first addition
	
	Revision 1.2  2016/04/04 15:29:28  francesco.avanzi
	introducing new preInventory S3ExelisFilenamePreInventory.C
	
	Revision 1.1  2016/04/04 14:02:54  francesco.avanzi
	first addition
	
	Revision 1.2  2015/08/05 07:57:51  francesco.avanzi
	Minor issue involving checks fixed
	
	Revision 1.1  2015/08/04 16:02:05  francesco.avanzi
	added
	
	Revision 1.1  2015/06/30 10:40:46  lucio.pulvirenti
	ALSAT1B-32: Specialization introduced for ALSAT 1B sensor model Aux data file: S3ExelisFilenamePreInventory class developed.
	
	
	
        
*/ 

#include <S3ExelisFilenamePreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>
#include <DateTime.h>
#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(S3ExelisFilenamePreInventory)

S3ExelisFilenamePreInventory::S3ExelisFilenamePreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

S3ExelisFilenamePreInventory::~S3ExelisFilenamePreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string S3ExelisFilenamePreInventory::getClassId()
{
     //return the class name
	return "S3ExelisFilenamePreInventory" ;
}
        


void S3ExelisFilenamePreInventory::parseFilename( const string& pathFileName, const string& fileType, FilenameElements& fne) // PRQA S 4211
{

    string filename = File::getFileName(pathFileName);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << '\"');
    rsResourceSet& rs = ConfigurationSingleton::instance()->get();
    
    string stopDate ;
    getEndOfMission(rs, stopDate);
    
    
    fne.File_Type = fileType ;
    if( "N1" == File::getExt(filename) )
    {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MERIS FILE" )
	fne.StartDate = DateTime( filename.substr(18, 8) + "T" + filename.substr(27,6) ).asISO8601(0) ;
        fne.CreationDate = fne.StartDate ;
     	fne.StopDate = DateTime(stopDate).asISO8601(0) ;
    }
    else if(File::getExt(filename) == "nat")
    {
        fne.StartDate = DateTime( filename.substr(16, 8) + "T" + filename.substr(24,6) ).asISO8601(0) ;
        fne.StopDate  = DateTime( filename.substr(32, 8) + "T" + filename.substr(40,6) ).asISO8601(0) ;
        fne.CreationDate = DateTime( filename.substr(52, 8) + "T" + filename.substr(60,6) ).asISO8601(0) ;
        
    }
    else if(File::getExt(filename) == "h5")
    {
        DateTime dt(filename.substr(16, 8) +"T" + filename.substr(24, 6)+ "." + filename.substr(30, 6) ) ;
        fne.StartDate =  dt.asISO8601(6) ;
        fne.CreationDate = fne.StartDate ;
        fne.StopDate = DateTime(stopDate).asISO8601(0) ;

    }
    else if(File::getExt(filename) == "nc")
    {
        unsigned int from ;
        unsigned int to ;
        
        to = filename.find_first_of('.' ) ;
        from = filename.find_last_of('_') ;
        
        
        
        string date  = filename.substr( from +1, to - from -1) ;    
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
   
}

_ACS_END_NAMESPACE
