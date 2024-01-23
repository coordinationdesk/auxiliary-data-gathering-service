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

#include <JersSAROPSPreInventory.h>

#include <NameIdAssociator.h>

#include <Filterables.h>
#include <StringUtils.h>
#include <DateTime.h>
#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(JersSAROPSPreInventory)

string const JersSAROPSPreInventory::satelliteName="Jers";

JersSAROPSPreInventory::JersSAROPSPreInventory() : 
    	FilenamePreInventory()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

JersSAROPSPreInventory::~JersSAROPSPreInventory() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}

string JersSAROPSPreInventory::getClassId()
{
     //return the class name
	return "JersSAROPSPreInventory";
}
        


void JersSAROPSPreInventory::parseFilename( const string& pathFileName,const string& fileType, FilenameElements& fne) // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);	
	
	string filename = File::getFileName(pathFileName);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filename=\""  << filename << "\"");
	
	fne.Satellite = satelliteName;

	vector<string> tokens;
	StringUtils::Tokenize(filename, tokens, ",");
	
	if( tokens[0] != "NULL" ) {
		fne.OrbitNumber = tokens[0];
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Orbit number=\""  << fne.OrbitNumber << "\"");
	}
	
	unsigned short hour;
	{
		istringstream iss(tokens[2]);
		iss >> hour;
	}
		
	unsigned short min;
	{
		istringstream iss(tokens[3]);
		iss >> min;
	}
	
	unsigned short sec;
	{
		istringstream iss(tokens[4]);
		iss >> sec;
	}
		
	unsigned int msec;
	{
		istringstream iss(tokens[5]);
		iss >> msec;
	}
	
	unsigned short year;
	{
		istringstream iss(tokens[6]);
		iss >> year;
	}
	
	unsigned short month;
	{
		istringstream iss(tokens[7]);
		iss >> month;
	}
	
	unsigned short day;
	{
		istringstream iss(tokens[8]);
		iss >> day;
	}

	DateTime startTime( DateTime::UTCD(year, month, day, hour, min, sec, msec) );
	{
		ostringstream strTime;
		startTime.asISO8601( strTime, 6);
		fne.StartDate = strTime.str();
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start Time=\""  << fne.StartDate << "\"");

	{
		unsigned short hour2;
		{
			istringstream iss(tokens[9]);
			iss >> hour2;
		}

		unsigned short min2;
		{
			istringstream iss(tokens[10]);
			iss >> min2;
		}

		unsigned short sec2;
		{
			istringstream iss(tokens[11]);
			iss >> sec2;
		}

		unsigned int msec2;
		{
			istringstream iss(tokens[12]);
			iss >> msec2;
		}
		DateTime dt( DateTime::UTCD(year, month, day, hour2, min2, sec2, msec2) );

		if( dt.compare(startTime) < 0 ) { // check if month day and year are the same of the stop time
			dt += static_cast<DateTime::JD50>(1.0); 
		}

		ostringstream strTime;
		dt.asISO8601( strTime, 6);
		fne.StopDate = strTime.str();
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop Time=\""  << fne.StopDate << "\"");
	}
	
	fne.GeneratingCentre = tokens[13];
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generating Centre=\""  << fne.GeneratingCentre << "\"");
	
	{
		unsigned short year2;
		{
			istringstream iss(tokens[14]);
			iss >> year2;
		}

		unsigned short month2;
		{
			istringstream iss(tokens[15]);
			iss >> month2;
		}

		unsigned short day2;
		{
			istringstream iss(tokens[16]);
			iss >> day2;
		}
		
		unsigned short hour2;
		{
			istringstream iss(tokens[17]);
			iss >> hour2;
		}

		unsigned short min2;
		{
			istringstream iss(tokens[18]);
			iss >> min2;
		}

		unsigned short sec2;
		{
			istringstream iss(tokens[19]);
			iss >> sec2;
		}

		unsigned int msec2;
		{
			istringstream iss(tokens[20]);
			iss >> msec2;
		}
		
		DateTime dt( DateTime::UTCD(year2, month2, day2, hour2, min2, sec2, msec2) );
		{
			ostringstream strTime;
			dt.asISO8601( strTime, 6);
			fne.CreationDate = strTime.str();
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Creation Time=\""  << fne.CreationDate << "\"");

}

short int JersSAROPSPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satellitename" , conn.getKey()) ;
	nameIdAssociator.load() ;
	return nameIdAssociator.id(satellite);

}


_ACS_END_NAMESPACE
