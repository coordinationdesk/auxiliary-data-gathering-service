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
	Revision 1.2  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 1.1  2017/09/19 09:51:05  davide.tiriticco
	First add
		
*/


#include <ExecutorVisitorFactoryChain.h>
#include <DefaultExecutorVisitor.h>

_ACS_BEGIN_NESTED_NAMESPACE (acs, defaultFactories)

ExecutorVisitor* buildExecutorVisitor (const ExecutorVisitor::BuildParameters& params)
{
	return new DefaultExecutorVisitor(params);
}

_ACS_END_NESTED_NAMESPACE
