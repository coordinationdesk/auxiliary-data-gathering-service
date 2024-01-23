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

	$Prod: A.C.S. Scheduler Default Log Plugin library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:54:43  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
	Import libScDefaultLogGroup
	

*/



#ifndef _ScDefaultLogGroupFactory_H_
#define _ScDefaultLogGroupFactory_H_

#include <ScLogHandlerFactoryChain.h>
#include <StringKeyFactory.h>

#include <exException.h>
#include <scOrder.h>

_ACS_BEGIN_NAMESPACE(acs)

class ScDefaultLogGroupFactory : public StringKeyFactory <ScLogHandlerGroup, scOrder>
{
public:
	ScDefaultLogGroupFactory();
	static acs::ScLogHandlerGroup* buildScDefaultLogGroup(const scOrder&);

private:
	ScDefaultLogGroupFactory (const ScDefaultLogGroupFactory&);
	ScDefaultLogGroupFactory& operator= (const ScDefaultLogGroupFactory&);
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ScDefaultLogGroupFactory)
};


_ACS_END_NAMESPACE

#endif //  _ScDefaultLogGroupFactory_H_



