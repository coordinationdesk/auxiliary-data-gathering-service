// PRQA S 1050 EOF
/*
	Copyright 1995-2019, Advanced Computer Systems , Inc.
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
	Revision 1.6  2015/05/28 14:33:41  francesco.avanzi
	*** empty log message ***
	
	Revision 1.5  2015/05/27 15:09:24  francesco.avanzi
	EOFADMPreInventory Added
	
	Revision 1.4  2013/11/13 12:43:34  lucio.pulvirenti
	Debug macros substituted.
	
	Revision 1.3  2013/02/20 16:40:07  lucio.pulvirenti
	qa rules applied.
	
	Revision 1.2  2012/10/11 10:39:59  giufor
	created empty class
	
	Revision 1.1.1.1  2012/10/11 09:56:12  giufor
	Import libEOFPreInventory
	
        
*/ 

#include <EOFPreInventoryFactory.h>
#include <PreInventoryFactoryChain.h>
#include <EOFPreInventory.h>
#include <EOFADMPreInventory.h>
#include <EOFMPMPreInventory.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


// **********************************************************************
// ***************** EOF PREINVENTORY FACTORY **************************
// **********************************************************************
//

EOFPreInventoryFactory::EOFPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> ("PreInventory.PluginsRegistrationSettings.EOFPreInventoryFactory", &buildObject) 
{
}

EOFPreInventoryFactory::~EOFPreInventoryFactory() throw()
{
}



PreInventoryBase* EOFPreInventoryFactory::buildObject() 
{
    return new EOFPreInventory;
}
// **********************************************************************
// ***************** EOFADM PREINVENTORY FACTORY ************************
// **********************************************************************
//

EOFADMPreInventoryFactory::EOFADMPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> ("PreInventory.PluginsRegistrationSettings.EOFADMPreInventoryFactory", &buildObject) 
{
}

EOFADMPreInventoryFactory::~EOFADMPreInventoryFactory() throw()
{
}



PreInventoryBase* EOFADMPreInventoryFactory::buildObject() 
{
    return new EOFADMPreInventory;
}
// **********************************************************************
// ***************** EOFMPM PREINVENTORY FACTORY ************************
// **********************************************************************
//

EOFMPMPreInventoryFactory::EOFMPMPreInventoryFactory():
	StringKeyFactory <PreInventoryBase> ("PreInventory.PluginsRegistrationSettings.EOFMPMPreInventoryFactory", &buildObject) 
{
}

EOFMPMPreInventoryFactory::~EOFMPMPreInventoryFactory() throw()
{
}



PreInventoryBase* EOFMPMPreInventoryFactory::buildObject() 
{
    return new EOFMPMPreInventory;
}



// *************************************************************
// // ***************** EMBEDDED FACTORY **************************
// // *************************************************************

namespace { // unnamed
    EOFPreInventoryFactory EmbeddedEOFPreInventoryFactory;
    EOFADMPreInventoryFactory EmbeddedEOFADMPreInventoryFactory;
    EOFMPMPreInventoryFactory EmbeddedEOFMPMPreInventoryFactory;

}
_ACS_END_NAMESPACE

