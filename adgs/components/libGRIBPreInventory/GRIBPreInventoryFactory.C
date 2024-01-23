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

	$Prod: A.C.S. libGRIBPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/18 15:07:33  giufor
	Import libGRIBPreInventory
	
        
*/ 

#include <GRIBPreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>
#include <GRIBPreInventory.h>

#include <Filterables.h>
_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GRIBPreInventoryFactory)
namespace {
    const string _gribPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.GRIBPreInventoryFactory";
}

// **********************************************************************
// ***************** GRIB PREINVENTORY FACTORY **************************
// **********************************************************************
//

GRIBPreInventoryFactory::GRIBPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_gribPreInventoryConfigKey, &GRIBPreInventoryFactory::buildObject) 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id );
}

GRIBPreInventoryFactory::~GRIBPreInventoryFactory() throw()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called."); // PRQA S 4631
}



PreInventoryBase* GRIBPreInventoryFactory::buildObject() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    return new GRIBPreInventory;
}

namespace {
    // *************************************************************
    // // ***************** EMBEDDED FACTORY **************************
    // // *************************************************************

    GRIBPreInventoryFactory EmbeddedGRIBPreInventoryFactory;
}

_ACS_END_NAMESPACE
