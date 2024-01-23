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

	$Prod: A.C.S. Enhanced String Factory Chain Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/07 17:35:32  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 1.2  2004/12/03 18:00:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:29:09  marfav
	Import libStringFactoryChain
	

*/

#ifndef _StringFactoryChain_H_
#define _StringFactoryChain_H_

#include <acs_c++config.hpp>
#include <AbstractFactoryChainSingleton.h>
#include <StringKey.h>



_ACS_BEGIN_NAMESPACE(acs)

template <typename O, typename R = void>
class StringFactoryChain : public AbstractFactoryChainSingleton <O, StringKey, StringExtendedEQ, R> // PRQA S 2153
{
public:
    virtual ~StringFactoryChain() throw() {} // PRQA S 2131
        
protected:
	StringFactoryChain() : AbstractFactoryChainSingleton <O, StringKey, StringExtendedEQ,R> ()
	{}

private:
	// Defined but not implemented
	StringFactoryChain (const StringFactoryChain&);
	StringFactoryChain& operator= (const StringFactoryChain&);
};

_ACS_END_NAMESPACE

#endif // _StringFactoryChain_H_

