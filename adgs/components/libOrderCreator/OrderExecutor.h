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

	$Prod: libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/13 18:24:26  marpas
	new class introduced to execute order (application level)
	coding best practices applied
	
	
*/



#ifndef _OrderExecutor_H_
#define _OrderExecutor_H_ 

#include <acs_c++config.hpp>
#include <ConfigurationSingleton.h>
#include <exMacros.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class dbConnection ;

class OrderExecutor // PRQA S 2109
{		
public:

	OrderExecutor();
	virtual ~OrderExecutor() throw() ;
    void process(dbConnectionPool & pool, ConfigurationSingleton::StopPredicate &) ; // PRQA S 4400

private:
    void process(const std::string &, dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) ;
	OrderExecutor(const OrderExecutor&);
	OrderExecutor & operator=(const OrderExecutor&);	

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderExecutor) ;
};


_ACS_END_NAMESPACE

#endif /* _OrderExecutor_H_ */
