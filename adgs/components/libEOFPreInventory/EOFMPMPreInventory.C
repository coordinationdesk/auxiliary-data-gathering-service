// PRQA S 1050 EOF
/*
	Copyright 1995-2015 Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libEOFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.6  2015/12/09 15:37:27  francesco.avanzi
	now inserting Originator instead of originator_id into .MTD file
	
	Revision 1.5  2015/12/03 14:06:23  francesco.avanzi
	Minor issue involving a non void non returning function issue solved
	WorkInProgress reguarding Order_ID assignement
	
	Revision 1.4  2015/12/02 14:59:17  francesco.avanzi
	Now only setting 3 chars for phase (chain)
	
	Revision 1.3  2015/12/02 12:41:36  francesco.avanzi
	now checking if db connection exists
	
	Revision 1.2  2015/11/30 14:46:15  francesco.avanzi
	minor issue solved involving wrong typing for NameIdAssociators
	
	Revision 1.1  2015/11/30 11:01:05  francesco.avanzi
	new class added
	
	
        
*/ 

#include <EOFMPMPreInventory.h>
#include <rsResourceSet.h>
#include <NameIdAssociator.h>
#include <Application.h>
#include <dbConnectionPool.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFMPMPreInventory);

//default Public Constructor
EOFMPMPreInventory::EOFMPMPreInventory():
                                        EOFPreInventory() ,
                                        sensorId_(),
                                        chain_(),
                                        originator_()
{}

EOFMPMPreInventory::~EOFMPMPreInventory() throw()
{
}

void EOFMPMPreInventory::addFurtherNodes(vector<string> &nodes) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
    nodes.push_back("Earth_Explorer_File/Data_Block/DIP/Instrument/value") ;
    nodes.push_back("Earth_Explorer_File/Data_Block/DIP/Chain/value") ;
    nodes.push_back("Earth_Explorer_File/Data_Block/DIP/Created_by/value") ;
    nodes.push_back("Earth_Explorer_File/Data_Block/Delta_DIP/Instrument/value") ;
    nodes.push_back("Earth_Explorer_File/Data_Block/Delta_DIP/Chain/value") ;
    nodes.push_back("Earth_Explorer_File/Data_Block/Delta_DIP/Created_by/value") ;
   
}
void EOFMPMPreInventory::extractFurther(rsResourceSet &inXml)
{
    
    
    
    if(inXml.hasKey("Data_Block.DIP"))
    {
        inXml.getValue("Data_Block.DIP.Instrument.value", sensorId_) ;
        inXml.getValue("Data_Block.DIP.Chain.value",chain_) ;
        inXml.getValue("Data_Block.DIP.Created_by.value", originator_) ;
    }
    else if(inXml.hasKey("Data_Block.Delta_DIP"))
    {
        inXml.getValue("Data_Block.Delta_DIP.Instrument.value", sensorId_) ;
        inXml.getValue("Data_Block.Delta_DIP.Chain.value",chain_) ;
        inXml.getValue("Data_Block.Delta_DIP.Created_by.value", originator_) ;
    
    }
    else
    {
        ACS_THROW(rsResourceSet::NotFoundException("Unable to find needed tags")) ; //PRQA S 3081
    }
    
    chain_ = chain_.substr(0,3);
    
    Application *theApp = Application::instance() ;
    if (0 != theApp) 
    {
        dbConnectionPool *pool = theApp->getDefaultConnectionPool() ;
        if (0 != pool) {
            dbConnectionWrapper w(*pool) ;
            dbConnection &conn = w ;// PRQA S 3050
            {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sensorId: " << sensorId_)
                NameIdAssociator<string> nameIdAssociator( "t_sensors", "sensorid", "sensorname" , conn.getKey()) ;
	            nameIdAssociator.load() ;
	            sensorId_ = nameIdAssociator.idExact(sensorId_); //PRQA S 3010
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sensorId: " << sensorId_)
            }
        }
        else {ACS_THROW(exIllegalValueException("connPool ==0"))}
    }else{ACS_THROW(exIllegalValueException("theApp == 0")) }
    

    
}
void EOFMPMPreInventory::setFurther(Metadata &metadata) 
{
    metadata.setMetadataValue( "Sensor_Id", sensorId_ );
    metadata.setMetadataValue( "System", originator_);
    metadata.setMetadataValue( "Phase", chain_);

}
_ACS_END_NAMESPACE
