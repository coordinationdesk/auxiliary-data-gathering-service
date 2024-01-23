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
	Revision 1.3  2016/01/13 15:17:54  francesco.avanzi
	minor issues fixed.
	now setting generationDate properly.
	UTC format implemented for StartDate and StopDate.
	
	Revision 1.2  2016/01/12 16:28:37  francesco.avanzi
	 spaceNavFilenamePreInventory put to work
	
	Revision 1.1  2016/01/12 14:00:42  francesco.avanzi
	module added
	
	
	
        
*/ 


#include <SpaceNavPreInventory.h>
#include <Filterables.h>
#include <NameIdAssociator.h>

#include <File.h>
#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SpaceNavPreInventory)


string SpaceNavPreInventory::getClassId()
{
     //return the class name
	return "SpaceNavPreInventory";
}


SpaceNavPreInventory::SpaceNavPreInventory():
        FilenamePreInventory()
        {}
        
void SpaceNavPreInventory::extractParamFromConf(const string &fType, const string &parName, unsigned int &parValue)
{
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	
	if( findConfigurationValue(fType, rs, parName, parValue) ) {
		ACS_COND_THROW( 0 == parValue, exIllegalValueException(parName + " tag not valorized") );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, parName << '=' << parValue );
	}


}
void SpaceNavPreInventory::parseFilename( const std::string &fileName, const std::string &fType, FilenameElements &fne )
{
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
 		
        fne.StopDate = dt.asISO8601(0);
	}
    DateTime dt ;
    fne.CreationDate =  dt.asISO8601(0) ;
}

_ACS_END_NAMESPACE














