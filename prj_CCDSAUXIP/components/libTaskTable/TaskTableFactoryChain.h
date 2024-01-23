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

#ifndef _TaskTableFactoryChain_H_
#define _TaskTableFactoryChain_H_

// #include<TaskTable.h>

#include<exException.h>
#include<StringKeyFactory.h>


_ACS_BEGIN_NAMESPACE(acs)

class TaskTable;


typedef StringKeyFactory<TaskTable>::SingletonType TaskTableFactoryChain;

_ACS_BEGIN_NAMESPACE(defaultFactories)

// Keep the default builders
TaskTable* buildDefaultTaskTable ();

_ACS_END_NAMESPACE


// inline here is very important.. avoids multiple declarations of the init specialization
template<> inline
void TaskTableFactoryChain::FactoryType::init()
{
	setDefaultBuilder (&defaultFactories::buildDefaultTaskTable, "TaskTable_DefaultBuilder");
}

_ACS_END_NAMESPACE



#endif // _TaskTableFactoryChain_H_
