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

#ifndef _EOFFilenamePreInventoryFactory_H_
#define _EOFFilenamePreInventoryFactory_H_

#include <acs_c++config.hpp>
#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * EOF PreInventory builder 
 **/

class EOFFilenamePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** EOFFilenamePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exEOFFilenamePreInventoryFactoryException, exException) ; // Base EOFFilenamePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exEOFFilenamePreInventoryFactoryCriticalException, exEOFFilenamePreInventoryFactoryException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	EOFFilenamePreInventoryFactory();
	/** Destructor */
	virtual ~EOFFilenamePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	EOFFilenamePreInventoryFactory(const EOFFilenamePreInventoryFactory & ); // not implemented
	EOFFilenamePreInventoryFactory &operator=(const EOFFilenamePreInventoryFactory &); // not implemented

private:
    static const std::string _EOFFilenamePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EOFFilenamePreInventoryFactory);
};

_ACS_END_NAMESPACE

#endif //_EOFFilenamePreInventoryFactory_H_

