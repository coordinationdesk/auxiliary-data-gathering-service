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


#ifndef _OrderCreatorFactory_H_
#define _OrderCreatorFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)


class OrderCreator ;
class dbConnection ;

//* Base class for orders creator factory
class OrderCreatorFactory // PRQA S 2109
{	
public:
	/*! class NotFoundException declaration */
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:

	OrderCreatorFactory() ;
	virtual ~OrderCreatorFactory() throw() ;

	virtual OrderCreator *newCreator(const std::string &orderType, const std::string &algo, unsigned int satId, dbConnection &) = 0 ;
	virtual void capabilities(std::map<std::string,std::string> &) const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() ;

	// declared but not implemented	
	OrderCreatorFactory(const OrderCreatorFactory &) ;
	OrderCreatorFactory &operator=(const OrderCreatorFactory &) ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderCreatorFactory) ;
} ;

_ACS_END_NAMESPACE


#endif /* _OrderCreatorFactory_H_ */

