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

	$Prod: A.C.S. ThinLayer Task Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management

*/

#include <TaskTableVisitor.h>
#include <Filterables.h>

#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableVisitor)




// class TaskTableVisitor
TaskTableVisitor::TaskTableVisitor()
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_MID_VERB)
}


TaskTableVisitor::~TaskTableVisitor() throw() 
{
    // PRQA S 4631 L1
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_MID_VERB)
    // PRQA L:L1
}



bool
TaskTableVisitor::enterVisit (TaskTableVisitable&)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_MID_VERB)

    return true;
}


bool
TaskTableVisitor::visit (TaskTableVisitable&)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH (ACS_MID_VERB)

    return true;
}


bool
TaskTableVisitor::exitVisit (TaskTableVisitable& v)
{
	    ACS_CLASS_BGN_DEBUG (ACS_MID_VERB)
		    excout << "TaskTableVisitor::exitVisit(TaskTableVisitable) : called\n";
		    excout << "\t(v).getVisitResult() = " << boolalpha << (v).getVisitResult() << noboolalpha << "\n";
		    excout << "\t(v).getNavigationResult() = " << boolalpha << (v).getNavigationResult() << noboolalpha << "\n";
	    ACS_CLASS_END_DEBUG


    return (v).getVisitResult()  && (v).getNavigationResult();
}


_ACS_END_NAMESPACE

