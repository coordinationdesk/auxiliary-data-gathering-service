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
	Revision 2.3  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.2  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.1  2008/11/24 15:58:36  marpas
	triggering files work in progress
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/


#ifndef _OfflineAnx2AnxOrderCreator_H_
#define _OfflineAnx2AnxOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <TemporaryOrder.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class dbConnection ;
class dbConnectionPool ;
class dbOrder ;

class OfflineAnx2AnxOrderCreator: public OrderCreator // PRQA S 2109
{		
public:
	enum OrbitMode {First, Last, Multiple, MultipleButLast } ;
private:
	class A2AOrder { // PRQA S 2109
	public:
		A2AOrder(const TemporaryOrder &, long orbit, OrderCreator &) ;
		~A2AOrder() throw() ;
		A2AOrder(const A2AOrder &) ;
		A2AOrder &operator=(const A2AOrder &) ;

		void add(const A2AOrder &) ;

		bool isExpired(double maxTime) const ;

		long absOrbit() const ;
		const std::vector<TemporaryOrder> orders() const ;
		const DateTime &startAnx() const ;
		const DateTime &stopAnx() const ;

		static void getOrbits(const TemporaryOrder &, OrbitMode,  OrderCreator &oc, std::vector<long int> &orbits) ;
		const std::vector<TemporaryOrder>& getTmpOrders() const ;
	private:
		// obsolescent kept for further reference
		A2AOrder() ;
	private:
		long _absOrbit ;
		DateTime _inventoryDate ;
		DateTime _startAnx ;
        DateTime _stopAnx ;
		std::vector<TemporaryOrder> _orders ;
	} ;

public:
	OfflineAnx2AnxOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection &);
	virtual ~OfflineAnx2AnxOrderCreator() throw() ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;

	// returns true if finds in the vector an order fro the same orbit and same processor.
	// pos will assume the position where the match is found.
	static bool find(const A2AOrder &, const std::vector<A2AOrder> &, size_t &pos) ; // PRQA S 2502

private:
	// declared but not implemented 
	OfflineAnx2AnxOrderCreator();
	OfflineAnx2AnxOrderCreator(const OfflineAnx2AnxOrderCreator&);
	OfflineAnx2AnxOrderCreator & operator=(const OfflineAnx2AnxOrderCreator&);
private:
    void fillL2O(const std::vector<TemporaryOrder> &tmporders, std::vector <A2AOrder> &) const ;
    void fillOrder(dbOrder &, const A2AOrder &) const ;
	void createOrder(const A2AOrder &, dbConnection &) const;
	OrbitMode orbitMode(const TemporaryOrder &) const ;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OfflineAnx2AnxOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _OfflineAnx2AnxOrderCreator_H_ */
