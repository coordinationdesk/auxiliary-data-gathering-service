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

	$Prod: A.C.S. ThinLayer TaskTableVisitables Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2015/04/29 10:29:54  marfav
	Stack protected by a mutex. Warning removed
	
	Revision 2.4  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.3  2013/03/04 11:42:08  marfav
	C2GTS-181 - Severe memory leakage fixed
	
	Revision 2.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.1  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <TaskTableVisitable.h>
#include <TaskTableVisitor.h>
#include <exStream.h>

#include <Task.h>



_ACS_BEGIN_NAMESPACE(acs)
	
using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableVisitable)


TaskTableVisitable::TaskTableVisitable() :
	_aggregates(),
	_visitStatusStack(),
    _stackMutex()

{
	// Empty CTOR
}

TaskTableVisitable::~TaskTableVisitable() throw() 
{
	// delete all the aggregates - C2GTS-181
    TaskTableVisitable::clear(); // PRQA S 4631
}

TaskTableVisitable::TaskTableVisitable(const TaskTableVisitable& e) :
    _aggregates(),
	_visitStatusStack(),
    _stackMutex()
{
	// COPY CTOR
	*this = e;
}

TaskTableVisitable&
TaskTableVisitable::operator= (const TaskTableVisitable& e)
{
	if (this != &e) 
	{
        Mutex::Lock lock1 (_stackMutex);
        Mutex::Lock lock2 (e._stackMutex);
		_visitStatusStack = e._visitStatusStack;

		// delete all the aggregates - C2GTS-181
        TaskTableVisitable::clear();

        // Then copy the new ones from the new object
		vector <TaskTableVisitable*> const& theElements = e.const_getElements();
		for (vector<TaskTableVisitable*>::const_iterator it=theElements.begin(); it!=theElements.end(); ++it)
		{
			this->addElement ((*it)->clone());
		}
	}

	return *this;
}


void
TaskTableVisitable::addElement (const TaskTableVisitable& e)
{
	this->addElement (e.clone());
}

void
TaskTableVisitable::addElement (TaskTableVisitable* e)
{
	_aggregates.push_back (e);
}



std::vector<TaskTableVisitable*>&
TaskTableVisitable::getElements ()
{
	return _aggregates; 
}

size_t
TaskTableVisitable::size() const
{
	return _aggregates.size();
}


void
TaskTableVisitable::clear()
{
    // C2GTS-181
    // Clean up the pointed elements before cleaning up the aggregates array
	for (vector<TaskTableVisitable*>::iterator it=_aggregates.begin(); it!=_aggregates.end(); ++it) // PRQA S 4238
	{
		delete *it;
	}
	_aggregates.clear();
}

const std::vector<TaskTableVisitable*>&
TaskTableVisitable::const_getElements () const
{
	return _aggregates;
}



bool
TaskTableVisitable::acceptVisit(TaskTableVisitor& v)
{
	return v.visit(*this);
}


bool
TaskTableVisitable::acceptEnterVisit(TaskTableVisitor& v)
{
	return v.enterVisit(*this);
}


bool
TaskTableVisitable::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}


bool
TaskTableVisitable::acceptVisitor (TaskTableVisitor& v)
{
	/*
	 * anticipated traversal... the logic is the following:
	 *
	 * 1 - enter the node
	 *     this involves decision if actually a visit is needed
	 *     the task is accomplished calling the acceptEnterVisit() call
	 *     the decision is communicated returning true or false
	 *     in the proper enterVisit(<the involved item>) method of the visitor
	 *
	 * 2 - visit the node
	 *     this implements whatever part of the visit has to be completed
	 *     before visiting the children, and decision if actually navigate the children
	 *     the task is accomplished with the acceptVisit() call
	 *     the result of the visit (and the decision on children navigation)
	 *     is communicated returning true or false
	 *     in the proper visit(<the involved item>) method of the visitor
	 *     it is saved on the stack, and is made available to following calls
	 *     calls through the getVisitResult() call
	 *
	 * 3 - navigate the node
	 *     this navigates the children
	 *     the task is accomplished with the acceptVisitor() call to each children
	 *     the children communicate if continue or not visiting their siblings
	 *     returning true or false
	 *     in the proper exitVisit(<the involved children item>) method of the visitor
	 *     the result of the navigation is saved on the stack,
	 *     and made available to following calls through the getNavigationResult() call
	 *
	 * 4 - exiting the node
	 *     this implements whatever part of the visit has to be completed
	 *     after visiting the children, followed by the decision on whether to stop siblings navigation
	 *     the task is accomplished calling the acceptExitVisit() call
	 *     the decision if to continue visiting my siblings is communicated returning true or false
	 *     in the proper exitVisit(<the involved item>) method of the visitor
	 *     the exitVisit() call can access results of previous steps (node visit and navitagion)
	 *     through the getVisitResult() and getNavigationResult() calls
	 *
	 */




	bool _visit_result = true;
	bool _navigation_result = true;

	bool _enter_visit_result = acceptEnterVisit(v);
	if (_enter_visit_result)
	{
		_visit_result = acceptVisit(v);

		vector <TaskTableVisitable*>& e = getElements();
		_navigation_result = true;
		if (_visit_result) {
		    for (vector <TaskTableVisitable*>::iterator it = e.begin(); (it != e.end() ) && _navigation_result; ++it )
		    {
			    _navigation_result = (*it)->acceptVisitor(v);
		    }
        }
	}

	StackData navData; // PRQA S 4102
	navData.enterVisitResult = _enter_visit_result;
	navData.visitResult = _visit_result;
	navData.navigationResult = _navigation_result;
    {
        Mutex::Lock lock1 (_stackMutex);
        _visitStatusStack.push(navData);
    }

	bool _exit_visit_result = acceptExitVisit (v);

    {
        Mutex::Lock lock1 (_stackMutex);
        _visitStatusStack.pop();
    }

	return _exit_visit_result;
}


bool TaskTableVisitable::getVisitResult() const // PRQA S 4214   
{
    ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Stack size is " << _visitStatusStack.size() );

    bool result;
    {
        Mutex::Lock lock1 (_stackMutex);
        result = _visitStatusStack.top().visitResult;
    }

	return result;
}

bool TaskTableVisitable::getNavigationResult() const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Stack size is " << _visitStatusStack.size() );

    bool result;
    {
        Mutex::Lock lock1 (_stackMutex);
        result = _visitStatusStack.top().navigationResult;
    }

	return result;

}

bool TaskTableVisitable::getEnterVisitResult() const // PRQA S 4214
{
    ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Stack size is " << _visitStatusStack.size() );

    bool result;
    {
        Mutex::Lock lock1 (_stackMutex);
        result = _visitStatusStack.top().enterVisitResult;
    }

    return result;
}

_ACS_END_NAMESPACE
