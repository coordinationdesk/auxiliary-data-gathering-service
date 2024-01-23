// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.2  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.1  2006/03/09 14:25:57  giucas
	Task Operator [] fixed
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/


#include <Pool.h>

#include <TaskTableVisitor.h>
#include <WeightNormalizer.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Pool)



Pool::Pool() : TaskTableElement()
{
}

Pool::Pool(const Pool& p) : TaskTableElement(p)
{     	
}

Pool::~Pool() throw() 
{
	// No operation here!!
}


TaskTableElement*
Pool::clone() const
{
	Pool* newPool = new Pool (*this);
	return newPool;
}

Pool&
Pool::operator = (const Pool& p)
{
	if (this != &p)
	{
		TaskTableElement::operator= (p);
	}
	return *this;
}

void 
Pool::getLocalInputList (vector<InputDescriptor>& aList, const string& theOrderType, bool erase) const
{
	// No local input list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
	return;
}

void 
Pool::getExtLocalInputList (vector<InputDescriptor>& aList, const string& theOrderType, bool erase) const
{
	// No local input list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
	return;
}

void 
Pool::getLocalOutputList (vector<OutputDescriptor>& aList, bool erase) const
{
	// No local output list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
	return;
}

void 
Pool::getLocalBreakpointList (vector<BreakpointDescriptor>& aList, bool erase) const
{
	// No local breakpoint list... delete if required and return
	if (erase) 
	{
		aList.clear();
	}
	return;
}

void 
Pool::normalize(WeightNormalizer* theNormalizer)
{
	ACS_SMARTPTR<WeightNormalizer> defNormalizer;

	if (!theNormalizer) 
	{
		// Instantiate the default normalizer
		// the ACS_SMARTPTR will delete it when exiting the scope
		defNormalizer.reset(new WeightNormalizer());
		theNormalizer = defNormalizer.get();
	}

	// Normalize the tasks, the pools and the sequences
	// empty structure are handled inside the normalization loop
	theNormalizer->normalize (*this);
}

void 
Pool::getLocalTaskList (vector<Task> & aList, bool erase) const
{
	if (erase) 
	{
		aList.clear();
	}
	
	// No tasks here
}



bool
Pool::acceptVisit (TaskTableVisitor& theVisitor)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )
    return theVisitor.visit (*this);
}


bool
Pool::acceptEnterVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )
    return v.enterVisit(*this);
}


bool
Pool::acceptExitVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )
    return v.exitVisit(*this);
}

_ACS_END_NAMESPACE
