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
	Revision 1.5  2015/05/28 14:33:29  francesco.avanzi
	 ./EOFPreInventoryFactory.C
	
	Revision 1.4  2015/05/27 15:09:24  francesco.avanzi
	EOFADMPreInventory Added
	
	Revision 1.3  2013/02/20 16:40:07  lucio.pulvirenti
	qa rules applied.
	
	Revision 1.2  2012/10/11 10:39:59  giufor
	created empty class
	
	Revision 1.1.1.1  2012/10/11 09:56:12  giufor
	Import libEOFPreInventory
	
        
*/

#ifndef _EOFPreInventoryFactory_H_
#define _EOFPreInventoryFactory_H_

#include <acs_c++config.hpp>
#include <PreInventoryFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief
 * EOF PreInventory builder 
 **/

class EOFPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	EOFPreInventoryFactory();
	virtual ~EOFPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	EOFPreInventoryFactory(const EOFPreInventoryFactory & ); // not implemented
	EOFPreInventoryFactory &operator=(const EOFPreInventoryFactory &); // not implemented
};


/**
 * \brief
 * EOF ADM PreInventory builder 
 **/
class EOFADMPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	EOFADMPreInventoryFactory();

    virtual ~EOFADMPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	EOFADMPreInventoryFactory(const EOFADMPreInventoryFactory & ); // not implemented
	EOFADMPreInventoryFactory &operator=(const EOFADMPreInventoryFactory &); // not implemented
};


/**
 * \brief
 * EOF MPM PreInventory builder 
 **/
class EOFMPMPreInventoryFactory : public StringKeyFactory<PreInventoryBase> // PRQA S 2109, 2153
{

public:

	EOFMPMPreInventoryFactory();

    virtual ~EOFMPMPreInventoryFactory() throw() ;

	static acs::PreInventoryBase* buildObject();

private:
	EOFMPMPreInventoryFactory(const EOFMPMPreInventoryFactory & ); // not implemented
	EOFMPMPreInventoryFactory &operator=(const EOFMPMPreInventoryFactory &); // not implemented
};

_ACS_END_NAMESPACE

#endif //_EOFPreInventoryFactory_H_

