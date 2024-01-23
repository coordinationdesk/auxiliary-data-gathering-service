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

	$Prod: A.C.S. Storage Manager  Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.2  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.1  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	
	
*/ 


#include <SMPackingStrategyFactory.h>
#include <SMPackingStrategyFactoryChain.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SMPackingStrategyFactory);

SMPackingStrategyFactory::SMPackingStrategyFactory()
{
	registerToChain() ;
}

SMPackingStrategyFactory::~SMPackingStrategyFactory() throw() 
{
    unregisterFromChain() ;
}

void SMPackingStrategyFactory::registerToChain() 
{
	SMPackingStrategyFactoryChain::instance()->registerFactory(this) ;
}

void SMPackingStrategyFactory::unregisterFromChain() throw() 
{
	SMPackingStrategyFactoryChain::instance()->unregisterFactory(this) ;
}


_ACS_END_NAMESPACE
