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

#ifndef _GRIBPreInventoryFactory_H_
#define _GRIBPreInventoryFactory_H_

#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* GRIB PreInventory builder 
**/

class GRIBPreInventoryFactory : private StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2193
{

public:

	/** Default Class constructor */
	GRIBPreInventoryFactory();
	/** Destructor */
	virtual ~GRIBPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:  
	GRIBPreInventoryFactory(const GRIBPreInventoryFactory & ); // not implemented
	GRIBPreInventoryFactory &operator=(const GRIBPreInventoryFactory &); // not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GRIBPreInventoryFactory) ;

};

_ACS_END_NAMESPACE

#endif //_GRIBPreInventoryFactory_H_

