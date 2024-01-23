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
	Revision 2.2  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.1  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.0  2006/02/28 10:24:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/

#ifndef _MergeOrderCreator_H_
#define _MergeOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <TemporaryOrder.h>


_ACS_BEGIN_NAMESPACE(acs)
	
class dbConnection ;
class dbOrder ;

class MergeOrderCreator: public OrderCreator // PRQA S 2109
{		
public:
	MergeOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection & );
	virtual ~MergeOrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;

private:
	// declared but not implemented 
	MergeOrderCreator();
	MergeOrderCreator(const MergeOrderCreator&);
	MergeOrderCreator & operator=(const MergeOrderCreator&);
protected:
	void createOrder(const std::vector<TemporaryOrder> &tl, size_t first , size_t last, dbConnection &) const;
    virtual void emitOrders(const std::vector<TemporaryOrder> &, dbConnection &) const ;
	void fillOrder(dbOrder &order, const std::vector<TemporaryOrder> &, size_t first , size_t last) const  ;
	virtual std::string createParamFile(const std::vector<TemporaryOrder> &, \
		const size_t& first , const size_t& last, dbConnection &theConnection) const ;
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MergeOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _MergeOrderCreator_H_ */
