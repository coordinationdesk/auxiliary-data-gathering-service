// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $

 */


#ifndef _InvPolicySelectorFactory_H_
#define _InvPolicySelectorFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class InvPolicySelector ;
class InvPolicy;
class RollingAction;

using pattern::Singleton ;


// This class implement the factory pattern for InvPolicySelector 
class InvPolicySelectorFactory  : public Singleton<InvPolicySelectorFactory > // PRQA S 2109, 2153
{	
	friend InvPolicySelectorFactory* Singleton< InvPolicySelectorFactory >::instance(); // PRQA S 2107

public:
	// class ThreadException declaration
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

	ACS_DECLARE_NO_COPY_CLASS(InvPolicySelectorFactory);

public:
	// Return the right selector for the specified policy
	InvPolicySelector *newSelector(const InvPolicy & ) ;

	// destructor
	virtual ~InvPolicySelectorFactory() throw() ;

protected:
	// Costructor
	InvPolicySelectorFactory() ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvPolicySelectorFactory)
} ;

_ACS_END_NAMESPACE

#endif /* _InvPolicySelectorFactory_H_ */

