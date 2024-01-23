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
        
*/ 


#include <SnavonsPreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SnavonsPreInventory)


string SnavonsPreInventory::getClassId()
{
     //return the class name
	return "SnavonsPreInventory";
}


SnavonsPreInventory::SnavonsPreInventory():
        FilenamePreInventory()
        {}
        
void SnavonsPreInventory::extractParamFromConf(const string &fType, const string &parName, unsigned int &parValue)
{
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	if( findConfigurationValue(fType, rs, parName, parValue) ) {
		ACS_COND_THROW( 0 == parValue, exIllegalValueException(parName + " tag not valorized") );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, parName << '=' << parValue );
	}


}
void SnavonsPreInventory::parseFilename( const std::string &fileName, const std::string &fType, FilenameElements &fne )
{

    ACS_LOG_INFO("------------------------- PARSE '" << fileName << "' ------------------");
    
	string fName = File::getFileName (fileName);
	unsigned int tagPos = 0 ;
	unsigned int tagLen = 0 ;
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	if( findConfigurationValue(fType, rs, "StartDatePos", tagPos) && findConfigurationValue(fType, rs, "StartDateLen", tagLen)  ) 
	{
		ACS_COND_THROW( 0 == tagLen , exIllegalValueException( "StartDateLen not valorized" ));
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StartDatePos: [ tagPos = " << tagPos << " ] [ tagLen = " << tagLen << " ]" ) ;
		unsigned int cut = 8;
        if( not findConfigurationValue(fType, rs, "StartDateCut", cut) )
        {
            ACS_LOG_WARNING("StartDateCut not Valorized. Using default value [ 8 ] " ) ; 
        }
        ostringstream date ;
 
        if(0 != cut)
        {
            date << fName.substr( tagPos, cut ) << 'T' << fName.substr( tagPos + cut , tagLen - cut );
        }
        else
        {
            date << fName.substr( tagPos, tagLen) ;
        }
        DateTime dt( date.str() ) ;
         
        fne.StartDate = dt.asISO8601(0);
	}
    tagPos = 0 ;
    tagLen = 0 ;
	if( findConfigurationValue(fType, rs, "StopDatePos", tagPos) && findConfigurationValue(fType, rs, "StopDateLen", tagLen) ) 
	{
		ACS_COND_THROW( 0 == tagLen , exIllegalValueException( "StopDateLen not valorized" ));
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StopDatePos: [ tagPos = " << tagPos << " ] [ tagLen = " << tagLen << " ]" ) ;

		unsigned int cut = 8;
        if( not findConfigurationValue(fType, rs, "StopDateCut", cut) )
        {
            ACS_LOG_WARNING("StopDateCut not Valorized. Using default value [ 8 ] " ) ; 
        }
        ostringstream date ;
        if(0 != cut)
        {
            date << fName.substr( tagPos, cut ) << 'T' << fName.substr( tagPos + cut , tagLen - cut );
        }
        else
        {
            date << fName.substr( tagPos, tagLen) ;
        }
        DateTime dt( date.str() ) ;
 		
        fne.StopDate = dt.asISO8601(0);
	}
    
    tagPos = 0 ;
    tagLen = 0 ;
	if( findConfigurationValue(fType, rs, "MissionPos", tagPos) && findConfigurationValue(fType, rs, "MissionLen", tagLen) ) 
	{
		ACS_COND_THROW( 0 == tagLen , exIllegalValueException( "MissionLen not valorized" ));
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MissionPos: [ tagPos = " << tagPos << " ] [ tagLen = " << tagLen << " ]" ) ;
        std::string mission_id = fName.substr(tagPos, tagLen);
        ACS_LOG_INFO(" --------- Mission: " << mission_id);
        fne.Mission = mission_id;
	}

    tagPos = 0 ;
    tagLen = 0 ;
	if( findConfigurationValue(fType, rs, "SatellitePos", tagPos) && findConfigurationValue(fType, rs, "SatelliteLen", tagLen) ) 
	{
		ACS_COND_THROW( 0 == tagLen , exIllegalValueException( "SatelliteLen not valorized" ));
	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SatellitePos: [ tagPos = " << tagPos << " ] [ tagLen = " << tagLen << " ]" ) ;
        std::string satellite_id = fName.substr(tagPos, tagLen);
        ACS_LOG_INFO(" --------- Satellite: " << satellite_id);
        fne.Satellite = satellite_id;
	}

    DateTime dt ;
    fne.CreationDate =  dt.asISO8601(0) ;
}

short int SnavonsPreInventory::getSatelliteId(const string& satellite, dbConnection &conn)
{
	ACS_LOG_INFO("[ DEBUG ] - satellite = '" << satellite << "'");

	ACS_COND_THROW(satellite.empty(), exSnavonsPreInventoryException("Satellite id not valorized!"));
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Satellite name=\""  << satellite << "\"");
	NameIdAssociator<short int> nameIdAssociator( "t_satellites", "satelliteid", "satelliteacronym" , conn.getKey()) ;
	nameIdAssociator.load() ;
    short int sat_id = nameIdAssociator.idExact(satellite);

    ACS_LOG_INFO("[ DEBUG ] - satellite id = " << sat_id);

    return sat_id;
}

_ACS_END_NAMESPACE














