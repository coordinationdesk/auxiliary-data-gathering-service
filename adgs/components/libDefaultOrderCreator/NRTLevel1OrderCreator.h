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
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/

#ifndef _NRTLevel1OrderCreator_H_
#define _NRTLevel1OrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbInventoryObject ;

class dbConnection ;
class dbConnectionPool ;
class TemporaryOrder ;
class rsResourceSet ;
class dbOrder ;
class NRTLevel1OrderCreator: public OrderCreator // PRQA S 2109
{		
public:
	NRTLevel1OrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection &);
	virtual ~NRTLevel1OrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;

private:
	// declared but not implemented 
	NRTLevel1OrderCreator();
	NRTLevel1OrderCreator(const NRTLevel1OrderCreator&);
	NRTLevel1OrderCreator & operator=(const NRTLevel1OrderCreator&);
    
	void downloadAndEmitOrder(const TemporaryOrder&, dbInventoryObject *, dbConnection &,ConfigurationSingleton::StopPredicate &, dbConnectionPool &) const ;
	void emitOrder(const rsResourceSet &, const TemporaryOrder &, dbConnection &) const ;
    void fillOrder(const rsResourceSet &, const std::string &, const TemporaryOrder &, dbOrder &) const ;
    void setOnDemandDist(const rsResourceSet &, const std::string &, const dbOrder &, dbConnection &) const ;
    void goodOrdersSummary() const ;
	void appendGoodOrder(dbPersistent::IdType) const ;

private:
	mutable std::vector<dbPersistent::IdType> _goodOrders ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NRTLevel1OrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _NRTLevel1OrderCreator_H_ */
