// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator
	
		

*/


#ifndef _OrderCreatorFactoryChain_H_
#define _OrderCreatorFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <exException.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)


class OrderCreatorFactory ;
class OrderCreator ;
class dbConnection ;

using pattern::Singleton ;


class OrderCreatorFactoryChain : public Singleton<OrderCreatorFactoryChain > // PRQA S 2109, 2153
{
	friend OrderCreatorFactoryChain* Singleton< OrderCreatorFactoryChain >::instance(); // PRQA S 2107

public:	
	// registers a new factory
	void registerFactory(OrderCreatorFactory *) ;
	// unregisters a new factory
	void unregisterFactory(OrderCreatorFactory *) ;
	
	OrderCreator *newCreator(const std::string &orderType, const std::string &algo, unsigned int satId, dbConnection &) ;
	std::string capabilities() const ;

	virtual ~OrderCreatorFactoryChain() throw() ;
	
protected:
	OrderCreatorFactoryChain() ;

private:
	// declared but not implemented	
	OrderCreatorFactoryChain(const OrderCreatorFactoryChain &) ;
	OrderCreatorFactoryChain &operator=(const OrderCreatorFactoryChain &) ; 

private:
	std::vector<OrderCreatorFactory *> _factories;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderCreatorFactoryChain) ;
} ;

_ACS_END_NAMESPACE


#endif

