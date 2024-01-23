/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNETCDFPreInventory$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2012/10/22 10:25:47  giufor
	Import libNETCDFPreInventory
	
	
        
*/ 

#include <NETCDFPreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>
#include <NETCDFPreInventory.h>

#include <Filterables.h>

using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(NETCDFPreInventoryFactory)


static const char* _netcdfPreInventoryConfigKey = "PreInventory.PluginsRegistrationSettings.NETCDFPreInventoryFactory";

// **********************************************************************
// ***************** NETCDF PREINVENTORY FACTORY **************************
// **********************************************************************
//

NETCDFPreInventoryFactory::NETCDFPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> (_netcdfPreInventoryConfigKey, &NETCDFPreInventoryFactory::buildObject) 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB," Id: "<< Id );
}

NETCDFPreInventoryFactory::~NETCDFPreInventoryFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}



PreInventoryBase* NETCDFPreInventoryFactory::buildObject() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
    return new NETCDFPreInventory;
}

// *************************************************************
// // ***************** EMBEDDED FACTORY **************************
// // *************************************************************

NETCDFPreInventoryFactory EmbeddedNETCDFPreInventoryFactory;


