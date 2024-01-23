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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/06/19 14:54:14  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.3  2013/02/13 18:25:27  marpas
	qa warning removed
	
	Revision 2.2  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.1  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/


#ifndef _NRTCreatorFactory_H_
#define _NRTCreatorFactory_H_ 

#include <acs_c++config.hpp>
#include <OrderCreatorFactory.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class dbConnection ; 

//* Base class for orders creator factory
class NRTCreatorFactory : public OrderCreatorFactory // PRQA S 2109
{	

public:

	NRTCreatorFactory() ;
	virtual ~NRTCreatorFactory() throw() ;

	virtual OrderCreator *newCreator(const std::string &orderType, const std::string &algo, unsigned int satId, dbConnection &) ;
	virtual void capabilities(std::map<std::string,std::string> &) const ;
				
private:

	// declared but not implemented	
	NRTCreatorFactory(const NRTCreatorFactory &) ;
	NRTCreatorFactory &operator=(const NRTCreatorFactory &) ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NRTCreatorFactory) ;
} ;

_ACS_END_NAMESPACE


#endif /* _NRTCreatorFactory_H_ */

