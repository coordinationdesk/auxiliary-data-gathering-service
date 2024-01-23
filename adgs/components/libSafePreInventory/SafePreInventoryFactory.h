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
	
        
*/

#ifndef _SafePreInventoryFactory_H_
#define _SafePreInventoryFactory_H_

#include <acs_c++config.hpp>
#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * Safe PreInventory builder 
 **/

class SafePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** SafePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exSafePreInventoryFactoryException, exException) ; // Base SafePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exSafePreInventoryFactoryCriticalException, exSafePreInventoryFactoryException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	SafePreInventoryFactory();
	/** Destructor */
	virtual ~SafePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	SafePreInventoryFactory(const SafePreInventoryFactory & ); // not implemented
	SafePreInventoryFactory &operator=(const SafePreInventoryFactory &); // not implemented

private:
    static const std::string _SafePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SafePreInventoryFactory);
};
	
/**
 * \brief
 * LTA S1 Safe PreInventory builder 
 **/

class LTAS1SafePreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	/** LTAS1SafePreInventoryFactory Exceptions */
	exDECLARE_EXCEPTION(exLTAS1SafePreInventoryFactoryException, exException) ; // Base LTAS1SafePreInventoryFactory Exception. // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exLTAS1SafePreInventoryFactoryCriticalException, exLTAS1SafePreInventoryFactoryException) ; // Critical Exception. // PRQA S 2131, 2153, 2502

	/** Default Class constructor */
	LTAS1SafePreInventoryFactory();
	/** Destructor */
	virtual ~LTAS1SafePreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	LTAS1SafePreInventoryFactory(const LTAS1SafePreInventoryFactory & ); // not implemented
	LTAS1SafePreInventoryFactory &operator=(const LTAS1SafePreInventoryFactory &); // not implemented

private:
    static const std::string _LTAS1SafePreInventoryConfigKey ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAS1SafePreInventoryFactory);
};


_ACS_END_NAMESPACE

#endif //_SafePreInventoryFactory_H_

