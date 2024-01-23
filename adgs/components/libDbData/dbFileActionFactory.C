// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.3  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.2  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.1  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/05/31 14:15:16  integrator
	file actions interfaces changed
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.1  2004/10/15 08:48:34  ivafam
	Added
	
	
*/ 


#include <dbFileActionFactory.h>
#include <dbFileActionFactoryChain.h>
#include <dbGiveUpFile.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbFileActionFactory) 

dbFileActionFactory::dbFileActionFactory()
{
	registerToChain() ;
}

dbFileActionFactory::~dbFileActionFactory() ACS_NOEXCEPT
{
	unregisterFromChain() ;
}

void dbFileActionFactory::registerToChain() 
{
	dbFileActionFactoryChain::instance()->registerFactory(this) ;
}

void dbFileActionFactory::unregisterFromChain() throw()
{
	dbFileActionFactoryChain::instance()->unregisterFactory(this) ;
}

_ACS_END_NAMESPACE
