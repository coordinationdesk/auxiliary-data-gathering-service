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
	Revision 1.1.1.1  2013/03/15 18:04:38  lucio.pulvirenti
	Import libEOFFilenamePreInventory
	
        
*/ 

#include <EOFFilenamePreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>
#include <EOFFilenamePreInventory.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EOFFilenamePreInventoryFactory)

const string EOFFilenamePreInventoryFactory::_EOFFilenamePreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.EOFFilenamePreInventoryFactory";

// **********************************************************************
// ***************** EOF PREINVENTORY FACTORY **************************
// **********************************************************************
//

EOFFilenamePreInventoryFactory::EOFFilenamePreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_EOFFilenamePreInventoryConfigKey, &EOFFilenamePreInventoryFactory::buildObject) 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id );
}

EOFFilenamePreInventoryFactory::~EOFFilenamePreInventoryFactory() throw()
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // PRQA L:L1 
}



PreInventoryBase* EOFFilenamePreInventoryFactory::buildObject() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    return new EOFFilenamePreInventory;
}

// *************************************************************
// // ***************** EMBEDDED FACTORY **************************
// // *************************************************************
namespace { // unnamed
    EOFFilenamePreInventoryFactory EmbeddedEOFFilenamePreInventoryFactory;
}
_ACS_END_NAMESPACE

