// PRQA S 1050 EOF
/*
 * 
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.3  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.2  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.1  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/14 09:01:02  marpas
	Strategy interface changed - there are cases where the strategy could update the inventoryobject itself
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	
*/


#ifndef _SMPackingStrategyFactory_H_
#define _SMPackingStrategyFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class SMPackingStrategy ;
class dbPersistent ;
class dbConnectionPool ;


class SMPackingStrategyFactory  // PRQA S 2109
{	
public:

	/*! class NotFoundException declaration */
	exDECLARE_EXCEPTION(NotFoundException,exException) ;    // PRQA S 2131, 2502

public:
	SMPackingStrategyFactory() ;
	SMPackingStrategyFactory(const SMPackingStrategyFactory &) = delete ;
	SMPackingStrategyFactory &operator=(const SMPackingStrategyFactory &) = delete ;
	SMPackingStrategyFactory( SMPackingStrategyFactory &&) = delete ;
	SMPackingStrategyFactory &operator=( SMPackingStrategyFactory &&) = delete ;
	
	virtual ~SMPackingStrategyFactory() noexcept ;

	virtual SMPackingStrategy *getStrategy(const std::string &strategy, dbPersistent &, dbConnectionPool &) = 0;
	virtual std::string capabilities() const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() noexcept ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SMPackingStrategyFactory) ;
} ;


_ACS_END_NAMESPACE


#endif /* _SMPackingStrategyFactory_H_ */

