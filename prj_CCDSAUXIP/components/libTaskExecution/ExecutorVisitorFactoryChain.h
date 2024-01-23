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

	$Prod: A.C.S. TaskTableExecution Library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2017/09/20 13:17:16  davide.tiriticco
	Renamed guard macros in headers
	
	Revision 1.2  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 1.1  2017/09/19 09:51:05  davide.tiriticco
	First add
		
*/

#ifndef _ExecutorVisitorFactoryChain_h_
#define _ExecutorVisitorFactoryChain_h_

#include <exException.h>
#include <StringKeyFactory.h>
#include <ExecutorVisitor.h>

_ACS_BEGIN_NAMESPACE(acs)

typedef StringKeyFactory<ExecutorVisitor, ExecutorVisitor::BuildParameters> ExecutorVisitorFactory;
typedef ExecutorVisitorFactory::SingletonType ExecutorVisitorFactoryChain;

_ACS_BEGIN_NAMESPACE(defaultFactories)

// Keep the default builders
ExecutorVisitor* buildExecutorVisitor (const ExecutorVisitor::BuildParameters&);

_ACS_END_NAMESPACE


// inline here is very important.. avoids multiple declarations of the init specialization
template<> inline
void ExecutorVisitorFactoryChain::FactoryType::init()
{
	setDefaultBuilder (&defaultFactories::buildExecutorVisitor, "TaskTableExecutorVisitor_DefaultBuilder");
}

_ACS_END_NAMESPACE

#endif /* _ExecutorVisitorFactoryChain_h_ */
