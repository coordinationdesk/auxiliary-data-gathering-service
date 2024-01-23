// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 5.4  2017/10/10 15:51:23  marpas
	trivial get methods implemented
	
	Revision 5.3  2015/04/21 15:30:52  marfav
	Adding the possibility to expand nomerge by inheritance
	
	Revision 5.2  2015/02/27 10:36:21  marfav
	APF-259 added baseline management
	
	Revision 5.1  2013/07/22 13:42:06  marpas
	ADC-193 implemented
	coding best practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.6  2013/03/08 10:04:28  marpas
	fixing some anomalies and asymmetries with other classes
	
	Revision 2.5  2013/03/07 19:47:16  marpas
	DUP inventory management as in jira ADC-122
	
	Revision 2.4  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.3  2012/03/07 09:42:42  marpas
	refactoring in progress
	
	Revision 2.2  2009/05/08 14:57:17  marpas
	concept of aging policy on temporary introduced - no regression by default aging policy
	
	Revision 2.1  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/

#ifndef _NoMergeOrderCreator_H_
#define _NoMergeOrderCreator_H_ 

#include <acs_c++config.hpp>
#include <OrderCreator.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)
	

class dbInventoryObject ;

class dbConnection ;
class TemporaryOrder ;
class rsResourceSet ;
class NoMergeOrderCreator: public OrderCreator // PRQA S 2109
{		
public:
	NoMergeOrderCreator(const std::string &type, const std::string &algo, unsigned int satId, dbConnection &, bool dupAllowed);
	NoMergeOrderCreator() = delete ;
	NoMergeOrderCreator(const NoMergeOrderCreator&) = delete ;
	NoMergeOrderCreator & operator=(const NoMergeOrderCreator&) = delete ;
	NoMergeOrderCreator(NoMergeOrderCreator&&) = delete ;
	NoMergeOrderCreator & operator=(NoMergeOrderCreator&&) = delete ;
    
	void setAgingPolicyOnTmpOrder(bool) ;
	void setDupAllowed(bool);
	bool getAgingPolicyOnTmpOrder() const ;
	virtual ~NoMergeOrderCreator() ACS_NOEXCEPT ;
	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const ;
    bool dupAllowed() const ACS_NOEXCEPT { return _dupAllowed ; }
    bool agingOnTmpTStamp() const ACS_NOEXCEPT { return _agingOnTmpTStamp ; }
    bool readMaxTime() const ACS_NOEXCEPT { return _readMaxTime ; }
    void setSticky(bool sticky) ACS_NOEXCEPT { _isSticky = sticky ; }

#ifdef TEST_ORDER_CREATOR
public:
#else
protected:
#endif
	virtual void createOrder(const TemporaryOrder &, dbConnection &) const;
    virtual std::string getAlgoKeyString() const ; // inherited from OrderCreator
    virtual void appendTmpOrder(double, const TemporaryOrder &, std::vector<TemporaryOrder> &, dbConnection &) const ;
    std::string getBaseline (const std::string&, const std::string&, dbConnection& ) const;
    void readMaxTime (bool b) ACS_NOEXCEPT { _readMaxTime = b; }
    std::string getJobResponsible(const std::string &fname, const std::string &fversion, dbConnection& ) const ;
	// declared but not implemented 
#ifdef TEST_ORDER_CREATOR
public:
#else
private:
#endif
    void loadConf() const ;

private:
	bool _agingOnTmpTStamp ;
    bool _dupAllowed ;
    bool _isSticky ;
    bool _readMaxTime;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NoMergeOrderCreator) ;
};

_ACS_END_NAMESPACE


#endif /* _NoMergeOrderCreator_H_ */
