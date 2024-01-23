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

	$Prod: A.C.S. TL Default TaskTable Factory $

	$Id$

	$Author$

    $Log$
    Revision 5.1  2013/11/19 17:35:12  marpas
    coding best practices applied
    qa warnings removed
    compilation warnings removed


*/

#ifndef _DefaultTaskTableFactory_H_
#define _DefaultTaskTableFactory_H_

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)


class TaskTable; 

class DefaultTaskTableFactory 
{
public:
	__attribute__((deprecated)) DefaultTaskTableFactory();
	~DefaultTaskTableFactory() throw() ;
	static acs::TaskTable* buildDefaultTaskTable();
	void registerFactory();
	void unregisterFactory();

private:
	DefaultTaskTableFactory (const DefaultTaskTableFactory&); // not implemented
	DefaultTaskTableFactory& operator= (const DefaultTaskTableFactory&); // not implemented
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultTaskTableFactory)
};

_ACS_END_NAMESPACE
	
#endif //  _DefaultTaskTableFactory_H_
