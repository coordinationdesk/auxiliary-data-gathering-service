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

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2016/05/10 14:41:08  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.6  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2012/06/05 15:39:19  marfav
	code improved to remove compiler warnings
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/05/06 09:54:29  marfav
	Added complete getLocalInputList search method
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <TaskTableElement.h>
#include <exStream.h>

#include <Task.h>



#include <algorithm>
#include <functional>




_ACS_BEGIN_NAMESPACE(acs)
	
using namespace std;


// Helper class for STL algos
// This class compares different InputDescriptors
// and searches for the specified orderType
class OrderType_eq // PRQA S 2109
{
public:
	explicit OrderType_eq(const string& oo) : o(oo) {}
	bool operator() (const InputDescriptor& d) const
	{
		return d.getOrderType() == o;
	}
private:
    OrderType_eq() ; // not implemented 
private:
	string o;

};



// static const char* _alwaysOrderType = "ALWAYS";
const string TaskTableElement::_alwaysOrderType = "ALWAYS";



// Helper class for STL algos
// This class extends the search criteria for OrderType in inputDescriptors elements
// It takes in account the ALWAYS value for OrderType that have to match to any other OrderType
class ExtOrderType_eq  // PRQA S 2109
{
public:
	explicit ExtOrderType_eq(const string& oo) : o(oo) {}
	bool operator() (const InputDescriptor& d) const
	{
		return d.getOrderType() == o || d.getOrderType() == TaskTableElement::_alwaysOrderType;
	}
private:
    ExtOrderType_eq() ; // not implemented 
private:
	string o;

};




ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableElement)


TaskTableElement::TaskTableElement() : 
	TaskTableVisitable(),
	_assignedPercentage(0),
	_assignedPercentageSet(false),
	_isDetached(false),
	_killingSignal(9), // PRQA S 4400
	_quitSignal(0),
	_isDetachedSet(false),
	_killingSignalSet(false),
	_quitSignalSet(false),
	_interruptable(true)
{
	// Empty CTOR
}


TaskTableElement::~TaskTableElement() throw() 
{
}


TaskTableElement::TaskTableElement(const TaskTableElement& e) :
	TaskTableVisitable(),
	_assignedPercentage(0),
	_assignedPercentageSet(false),
	_isDetached(false),
	_killingSignal(9), // PRQA S 4400
	_quitSignal(0),
	_isDetachedSet(false),
	_killingSignalSet(false),
	_quitSignalSet(false),
	_interruptable(true)
{
	// COPY CTOR
	*this = e;
}


TaskTableElement&
TaskTableElement::operator= (const TaskTableElement& e)
{
	if (this != &e) 
	{
		_assignedPercentageSet = e._assignedPercentageSet;
		_assignedPercentage = e._assignedPercentage;
		_isDetached = e._isDetached;
		_isDetachedSet = e._isDetachedSet;
		_killingSignal = e._killingSignal;
		_quitSignal = e._quitSignal;
		_killingSignalSet = e._killingSignalSet;
		_quitSignalSet = e._quitSignalSet;
		_interruptable = e._interruptable;
		
		TaskTableVisitable::operator= (e);
	}
	return *this;
}


void
TaskTableElement::getInputList(vector<InputDescriptor>& aList, bool erase) const
{
	getList(aList, always_true_pred(aList), true, erase);
}

void
TaskTableElement::getInputPtrList(TaskTableSmartVector<InputDescriptor>& aList, bool erase) const
{
	getPtrList(aList, always_true<InputDescriptor> (), true, erase);
}

void
TaskTableElement::getLocalInputList(vector<InputDescriptor>& aList, bool erase) const
{
	getList(aList, always_true_pred(aList), false, erase);
}


void
TaskTableElement::getInputList(vector<InputDescriptor>& aList,
                               const string& anOrderType,
			       const string& theTask,
			       bool erase, bool nested) const
{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getList(aList, OrderType_eq(anOrderType), false, erase); // PRQA S 3081
	}
	else
	{
		// First add local inputs, and then the nested if required
		getList(aList, OrderType_eq(anOrderType), nested, erase); // PRQA S 3081
	}
}

void
TaskTableElement::getInputPtrList 
	(
		TaskTableSmartVector<InputDescriptor>& aList,
		const string& anOrderType,
		const string& theTask,
		bool erase, bool nested
	) const
{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getPtrList(aList, OrderType_eq(anOrderType), false, erase); // PRQA S 3081
	}
	else
	{
		// First add local inputs, and then the nested if required
		getPtrList(aList, OrderType_eq(anOrderType), nested, erase); // PRQA S 3081
	}
}

void
TaskTableElement::getExtInputList(vector<InputDescriptor>& aList,
				  const string& anOrderType,
				  const string& theTask,
				  bool erase, bool nested) const
{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getList(aList, ExtOrderType_eq(anOrderType), false, erase); // PRQA S 3081
	}
	else
	{
		getList(aList, ExtOrderType_eq(anOrderType), nested, erase); // PRQA S 3081
	}
}

void
TaskTableElement::getExtInputPtrList
	(
		TaskTableSmartVector<InputDescriptor>& aList,
		const string& anOrderType,
		const string& theTask,
		bool erase, bool nested
	) const

{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getPtrList(aList, ExtOrderType_eq(anOrderType), false, erase); // PRQA S 3081
	}
	else
	{
		getPtrList(aList, ExtOrderType_eq(anOrderType), nested, erase); // PRQA S 3081
	}
}


void
TaskTableElement::getOutputList(vector<OutputDescriptor>& aList, const string& theTask, bool erase, bool nested) const
{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getList(aList, always_true_pred(aList), false, erase);
	}
	else
	{
		getList(aList, always_true_pred(aList), nested, erase);
	}
}

void
TaskTableElement::getBreakpointList(vector<BreakpointDescriptor>& aList, const string& theTask, bool erase, bool nested) const
{
	// If a particular task is required
	// Only one task is searched using [] operator
	// Exception will be raised if the task is not found
	
	if (theTask != "") 
	{
		((*this)[theTask]).getList(aList, always_true_pred(aList), false, erase);
	}
	else
	{
		getList(aList, always_true_pred(aList), nested, erase);
	}
}


void 
TaskTableElement::getTaskList (vector<Task>& aList, bool erase, bool nested) const
{
	getList(aList, always_true_pred(aList), nested, erase);
}


void 
TaskTableElement::getLocalTaskList (vector<Task>& aList, bool erase) const
{
	getList(aList, always_true_pred(aList), false, erase);
}


void
TaskTableElement::getLocalInputList(vector<InputDescriptor>& aList, const string& anOrderType, bool erase) const
{
	getList(aList, OrderType_eq(anOrderType), false, erase); // PRQA S 3081
}


void
TaskTableElement::getExtLocalInputList(vector<InputDescriptor>& aList, const string& anOrderType, bool erase) const
{
	getList(aList, ExtOrderType_eq(anOrderType), false, erase); // PRQA S 3081
}

void
TaskTableElement::getLocalOutputList(vector<OutputDescriptor>& aList, bool erase) const
{
	getList(aList, always_true_pred(aList), false, erase); // PRQA S 3081
}

void
TaskTableElement::getLocalBreakpointList(vector<BreakpointDescriptor>& aList, bool erase) const
{
	getList(aList, always_true_pred(aList), false, erase); // PRQA S 3081
}



// Helper class for STL algos
// Search tasks using the name as the key
class TaskName_eq // PRQA S 2109 
{
public:
	explicit TaskName_eq(const string& ss) : s(ss) {}
	bool operator() (const Task& t) const { return t.getName() == s; }
private:
    TaskName_eq() ; // not implemented
private:
	string s;

};


bool
TaskTableElement::exists(const string& TaskName, bool nested, Task* foundTask) const
{
	vector<Task> tasks;
	getList(tasks, TaskName_eq(TaskName), nested); // PRQA S 3081

	if (not tasks.empty()  &&  foundTask) {
		*foundTask = tasks[0];
    }

	return not tasks.empty() ;
}

Task
TaskTableElement::operator [] (const string& TaskName) const
{
	Task theTask;
	if (!exists (TaskName, true, &theTask) ) 
	{
		ACS_THROW(TaskNotFoundException("Task " + TaskName + " not found")) ; // PRQA S 3081
	}

	return theTask;
}


bool
TaskTableElement::isSetDetached() const // PRQA S 4120
{
	return _isDetachedSet;
}

bool 
TaskTableElement::isDetached () const // PRQA S 4120
{
	return _isDetached;
}

void 
TaskTableElement::isDetached (bool b)
{
	_isDetachedSet = true;
	_isDetached = b;
}


bool 
TaskTableElement::isSetKillSignal() const // PRQA S 4120
{
	return _killingSignalSet;
}

int
TaskTableElement::getKillSignal () const // PRQA S 4120
{
	return _killingSignal;
}

void 
TaskTableElement::setKillSignal (int s)
{
	_killingSignalSet = true;
	_killingSignal = s;
}


bool
TaskTableElement::isSetQuitSignal () const // PRQA S 4120
{
	return _quitSignalSet;
}

int
TaskTableElement::getQuitSignal () const // PRQA S 4120
{
	return _quitSignal;
}

void 
TaskTableElement::setQuitSignal (int s)
{
	_quitSignalSet = true;
	_quitSignal = s;
}

bool
TaskTableElement::isSetAssignedPercentage() const // PRQA S 4120
{
	return 	_assignedPercentageSet;
}

void
TaskTableElement::setAssignedPercentage(float pct)
{
	_assignedPercentageSet = true;
	_assignedPercentage = pct;
}

float
TaskTableElement::getAssignedPercentage() const // PRQA S 4120
{
	return _assignedPercentage;
}

_ACS_END_NAMESPACE
