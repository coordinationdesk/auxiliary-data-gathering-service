// PRQA S 1050 EOF
/*
	Copyright 2013, Advanced Computer Systems , Inc.
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
	Revision 1.1.1.1  2013/06/28 16:28:59  lucio.pulvirenti
	Import libSafePreInventory
	
        
*/ 

#include <SafePreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>
#include <SafePreInventory.h>
#include <LTAS1SafePreInventory.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SafePreInventoryFactory)
ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAS1SafePreInventoryFactory)

const string SafePreInventoryFactory::_SafePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.SafePreInventoryFactory";
const string LTAS1SafePreInventoryFactory::_LTAS1SafePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.LTAS1SafePreInventoryFactory";

// **********************************************************************
// ***************** SAFE PREINVENTORY FACTORY **************************
// **********************************************************************
//

SafePreInventoryFactory::SafePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_SafePreInventoryConfigKey, &SafePreInventoryFactory::buildObject) 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

SafePreInventoryFactory::~SafePreInventoryFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
    // PRQA L:L1 
}



PreInventoryBase* SafePreInventoryFactory::buildObject() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method Called");
    return new SafePreInventory;
}

// **********************************************************************
// ************* LTA S1 SAFE PREINVENTORY FACTORY ***********************
// **********************************************************************
//

LTAS1SafePreInventoryFactory::LTAS1SafePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_LTAS1SafePreInventoryConfigKey, &LTAS1SafePreInventoryFactory::buildObject) 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

LTAS1SafePreInventoryFactory::~LTAS1SafePreInventoryFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method Called");
    // PRQA L:L1 
}



PreInventoryBase* LTAS1SafePreInventoryFactory::buildObject() 
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Method Called");
    return new LTAS1SafePreInventory;
}


// *************************************************************
// // ***************** EMBEDDED FACTORY **************************
// // *************************************************************
namespace { // unnamed
    SafePreInventoryFactory EmbeddedSafePreInventoryFactory;
    LTAS1SafePreInventoryFactory EmbeddedLTAS1SafePreInventoryFactory;
}
_ACS_END_NAMESPACE

