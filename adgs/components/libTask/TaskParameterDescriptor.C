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
	Revision 1.2  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 1.1  2008/10/21 09:03:30  marfav
	Adding TaskParameterDescriptor classes
	

*/


#include <TaskParameterDescriptor.h>

#include <TaskTableVisitor.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


// Task Parameter List
// Class Implementation
// START

// class TaskParameterList
ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskParameterList)


TaskParameterList::TaskParameterList() :
    TaskTableVisitable() 
{
}


TaskParameterList::TaskParameterList(const TaskParameterList& l) : TaskTableVisitable()
{
	*this = l;
}

TaskParameterList&
TaskParameterList::operator =(const TaskParameterList& l)
{
	if (this != &l)
	{
		TaskTableVisitable::operator= (l);
	}
	return *this;
}

TaskTableVisitable*
TaskParameterList::clone() const
{
	TaskParameterList* newTaskParameterList = new TaskParameterList (*this);
	return newTaskParameterList;
}


bool
TaskParameterList::acceptEnterVisit(TaskTableVisitor& v)
{
	return v.enterVisit(*this);
}


bool
TaskParameterList::acceptVisit(TaskTableVisitor& v)
{
	return v.visit(*this);
}


bool
TaskParameterList::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}

// Task Parameter List
// Class Implementation
// END



// class TaskParameterDescriptor

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskParameterDescriptor)

TaskParameterDescriptor::TaskParameterDescriptor() : 
	TaskTableVisitable(),
	_name (""),
	_value (""),
    _mandatory (true),
  	_nameSet (false),
	_valueSet (false),
    _mandatorySet (false)
{
}

TaskParameterDescriptor::TaskParameterDescriptor(const TaskParameterDescriptor& d) : 
	TaskTableVisitable(d),
	_name(d._name),
	_value(d._value),
    _mandatory(d._mandatory),
    _nameSet(d._nameSet),
	_valueSet(d._valueSet),
    _mandatorySet(d._mandatorySet)
{
}

TaskParameterDescriptor&
TaskParameterDescriptor::operator =(const TaskParameterDescriptor& d)
{
	if (this != &d)
	{
		_name = d._name;
		_value = d._value;
        _mandatory = d._mandatory;
        _nameSet = d._nameSet;
		_valueSet = d._valueSet;
        _mandatorySet = d._mandatorySet;
		TaskTableVisitable::operator= (d);
	}
	return *this;
}

bool
TaskParameterDescriptor::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
TaskParameterDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
TaskParameterDescriptor::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

string
TaskParameterDescriptor::getName (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _nameSet;
	}
	return _name;
}

void
TaskParameterDescriptor::setName (const string& name)
{
	_name = name;
	_nameSet = true;
}

string
TaskParameterDescriptor::getValue (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _valueSet;
	}
	return _value;
}

void
TaskParameterDescriptor::setValue (const string& value)
{
	_value = value;
	_valueSet = true;
}

bool 
TaskParameterDescriptor::isMandatory (bool* isSet) const
{
	if (isSet)
	{
		*isSet = _mandatorySet;
	}
	return _mandatory;
}

void 
TaskParameterDescriptor::isMandatory (bool b)
{
    _mandatory = b;
    _mandatorySet = true;
}

TaskTableVisitable*
TaskParameterDescriptor::clone() const
{
	TaskParameterDescriptor* newTaskParameterDescriptor = new TaskParameterDescriptor (*this);
	return newTaskParameterDescriptor;
}


_ACS_END_NAMESPACE
