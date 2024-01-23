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

	$Prod: A.C.S. ThinLayer TaskTable Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/11/19 17:35:12  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 5.0  2013/07/07 18:24:08  marpas
	itroducing libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings partially removed
	
	Revision 2.1  2010/08/03 14:28:06  nicvac
	Task Table plugins management
	

*/

#include <TaskTableFactoryChain.h>
#include <TaskTable.h>

_ACS_BEGIN_NESTED_NAMESPACE (acs, defaultFactories)

TaskTable* buildDefaultTaskTable ()
{
	return new TaskTable;
}

_ACS_END_NESTED_NAMESPACE

