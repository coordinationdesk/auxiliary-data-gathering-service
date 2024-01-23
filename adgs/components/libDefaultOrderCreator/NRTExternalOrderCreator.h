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
	Revision 2.3  2013/03/12 17:54:38  marpas
	dbConnectionPool is mandatory in file actions
	
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
	
	Revision 1.1  2005/11/28 20:18:02  marpas
	NRTExternalOrderCreator class added - supports Envisat for EXTERNAL algorithm
	
	
	
*/

#ifndef _NRTExternalOrderCreator_H_
#define _NRTExternalOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)
	
class dbInventoryObject ;

class dbConnectionPool ;
class dbConnection ;
class TemporaryOrder ;
class rsResourceSet ;
class NRTExternalOrderCreator: public OrderCreator  // PRQA S 2109
{		
public:
	NRTExternalOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection &);
	virtual ~NRTExternalOrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;

private:
	// declared but not implemented 
	NRTExternalOrderCreator();
	NRTExternalOrderCreator(const NRTExternalOrderCreator&);
	NRTExternalOrderCreator & operator=(const NRTExternalOrderCreator&);
    
	void downloadAndEmitOrder(const TemporaryOrder&, dbInventoryObject *, dbConnection &, ConfigurationSingleton::StopPredicate &, dbConnectionPool &) const ;
	void emitOrder(const rsResourceSet &, const TemporaryOrder &, dbConnection &) const ;
	void goodOrdersSummary() const ;
	void appendGoodOrder(dbPersistent::IdType) const ;

private:
	mutable std::vector<dbPersistent::IdType> _goodOrders ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NRTExternalOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _NRTExternalOrderCreator_H_ */
