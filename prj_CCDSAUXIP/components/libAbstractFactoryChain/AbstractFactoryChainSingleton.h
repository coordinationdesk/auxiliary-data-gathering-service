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

	$Prod: A.C.S. Abstract Factory Library $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/07/07 17:34:12  marpas
	coding best practices applied
	qa warnings removed
	some compilation warning removed
	
	Revision 2.1  2013/01/25 11:25:19  marpas
	using pattern::Singleton from libException
	
	Revision 2.0  2006/02/28 10:08:02  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:42:50  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:28:30  marfav
	Import libAbstractFactoryChain
	

*/

#ifndef _AbstractFactoryChainSingleton_H_
#define _AbstractFactoryChainSingleton_H_

#include <acs_c++config.hpp>
#include <AbstractFactoryChain.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

template <typename O, typename K = std::string, typename P = DefaultAbstractFactoryChainPredicate<K> ,  typename B = void>
class AbstractFactoryChainSingleton : public Singleton <AbstractFactoryChain<O,K,P,B> > // PRQA S 2153
{

public:
	typedef AbstractFactoryChain<O,K,P,B> FactoryType;
	virtual ~AbstractFactoryChainSingleton() throw() {} // PRQA S 2131

protected:
	AbstractFactoryChainSingleton() : Singleton <AbstractFactoryChain<O,K,P,B> > ()
	{}

private:
	// declared but not implemented
	AbstractFactoryChainSingleton (const AbstractFactoryChainSingleton&);
	AbstractFactoryChainSingleton& operator= (const AbstractFactoryChainSingleton&);

};

_ACS_END_NAMESPACE


#endif// _AbstractFactoryChainSingleton_H_
