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

	$Prod: A.C.S. ThinLayer Task Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2015/04/29 10:30:07  marfav
	Warning removed
	
	Revision 2.7  2014/06/10 15:47:46  marfav
	Addedd support to number of CPUs
	
	Revision 2.6  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.4  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.3  2008/10/20 13:18:56  marfav
	Added support to task cmdline parameters
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.19  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	
	Revision 1.18  2006/01/19 18:17:35  marfav
	Fileclass attribute added to InputAlternative
	
	Revision 1.17  2005/06/28 08:21:09  marfav
	Using strings only order type definitions
	
	Revision 1.16  2005/03/09 09:27:19  marfav
	Modified less than operator
	
	Revision 1.15  2004/12/03 18:01:40  marfav
	Header fixed
	
	Revision 1.14  2004/09/28 15:05:42  marfav
	Added percentage support
	
	Revision 1.13  2004/08/03 13:52:17  marfav
	Using TimeRange instead of dbGeoTimeRange
	
	Revision 1.12  2004/04/05 13:17:24  marfav
	Using exStream.h
	
	Revision 1.11  2003/07/10 14:05:53  marfav
	clone method added to exceptions
	
	Revision 1.10  2003/07/04 15:24:13  marfav
	Enhanced management of input and output descriptors
	
	Revision 1.9  2003/06/05 14:17:16  marfav
	Added Criticality Level support
	
	Revision 1.8  2003/05/21 08:57:37  marfav
	FileNameType added to OutputDescriptor and IInput Alternative
	
	Revision 1.7  2003/04/30 14:44:18  marfav
	Align to ICD v2.0
	
	Revision 1.6  2003/04/18 12:47:37  marfav
	Using namespaces removed from .h
	
	Revision 1.5  2003/02/18 09:12:14  fracar
	improved logging
	
	Revision 1.4  2003/02/10 18:52:29  fracar
	prepared for multi-interval management
	
	Revision 1.3  2003/02/05 13:40:20  fracar
	handling T0 and T1 fields inside the task table
	
	Revision 1.2  2003/01/31 08:44:55  marfav
	Bug fixed
	
	Revision 1.1.1.1  2003/01/30 18:25:34  marfav
	Import libTask
	
	


*/

#include <tltV.h>

#include <Task.h>
#include <TaskTableVisitor.h>

#include <exStream.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
    tltV version;
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(Task)



// class Task
Task::Task() : 
	TaskTableElement(),
	_name(""),
	_version(""),
	_path(""),
	_isCritical(false),
	_criticalityLevel(0),
	_nameSet(false),
	_versionSet(false),
	_pathSet(false),
	_isCriticalSet(false),
	_criticalityLevelSet(false),
    _numberOfCPUs(0),
    _isNumberOfCPUsSet(false),
	_isPosixExitCodeSet (false),
	_isPosixExitCode(true),
    _theInputDescriptors(0),
	_theOutputDescriptors(0),
	_theBreakpointDescriptors(0),
	_theParameterDescriptors(0)
{

	_theParameterDescriptors = new TaskParameterList;
   	_theBreakpointDescriptors = new BreakpointList;
	_theInputDescriptors = new InputList;
	_theOutputDescriptors = new OutputList;

    addElement(_theParameterDescriptors);
	addElement(_theInputDescriptors);
	addElement(_theOutputDescriptors);
	addElement(_theBreakpointDescriptors);

	// Be careful!!!
	// delete of the pointers will be made by the TaskTableElement destructor
}

Task::Task(const Task& aTask) : 
    TaskTableElement (),
   	_name(""),
	_version(""),
	_path(""),
	_isCritical(false),
	_criticalityLevel(0),
	_nameSet(false),
	_versionSet(false),
	_pathSet(false),
	_isCriticalSet(false),
	_criticalityLevelSet(false),
    _numberOfCPUs(0),
    _isNumberOfCPUsSet(false),  
	_isPosixExitCodeSet (false),
	_isPosixExitCode(true),
    _theInputDescriptors(0),
	_theOutputDescriptors(0),
	_theBreakpointDescriptors(0),
	_theParameterDescriptors(0)
{
	*this = aTask;
}   

Task::~Task() throw() 
{
	// Empty DTOR
}

TaskTableElement*
Task::clone() const
{
	Task* newTask = new Task (*this);
	return newTask;
}

Task&
Task::operator = (const Task& aTask)
{
	if (this != &aTask)
	{
		_name                      = aTask._name;
		_version                   = aTask._version;
		_path                      = aTask._path;
		_isCritical                = aTask._isCritical;
		_criticalityLevel          = aTask._criticalityLevel;
		_nameSet                   = aTask._nameSet;
		_versionSet                = aTask._versionSet;
		_pathSet                   = aTask._pathSet;
		_isCriticalSet             = aTask._isCriticalSet;
		_criticalityLevelSet       = aTask._criticalityLevelSet;
        _numberOfCPUs              = aTask._numberOfCPUs;
        _isNumberOfCPUsSet         = aTask._isNumberOfCPUsSet;
		_isPosixExitCodeSet        = aTask._isPosixExitCodeSet;
		_isPosixExitCode           = aTask._isPosixExitCode;
        
		TaskTableElement::operator= (aTask);

		// now re-assign pointers to containers...
		// (they have been changed, since the operator= calls clone() of elements

        // First item should be Parmaters (according to constructor)
		if (dynamic_cast<TaskParameterList*>(_aggregates[0])) {// PRQA S 3081
		    _theParameterDescriptors = static_cast<TaskParameterList*>(_aggregates[0]); // PRQA S 3081
        }
		else if (dynamic_cast<InputList*>(_aggregates[0])) {// PRQA S 3081
		    _theInputDescriptors = static_cast<InputList*>(_aggregates[0]); // PRQA S 3081
        }
		else if (dynamic_cast<OutputList*>(_aggregates[0])) {// PRQA S 3081
		    _theOutputDescriptors = static_cast<OutputList*>(_aggregates[0]); // PRQA S 3081
        }
		else if (dynamic_cast<BreakpointList*>(_aggregates[0])) {// PRQA S 3081
		    _theBreakpointDescriptors = static_cast<BreakpointList*>(_aggregates[0]); // PRQA S 3081
        }
        else { /* nothing */ }

		// second item should be Input (according to constructor)
		if (dynamic_cast<InputList*>(_aggregates[1])) {// PRQA S 3081
		    _theInputDescriptors = static_cast<InputList*>(_aggregates[1]); // PRQA S 3081
        }
		else if (dynamic_cast<OutputList*>(_aggregates[1])) {// PRQA S 3081
		    _theOutputDescriptors = static_cast<OutputList*>(_aggregates[1]); // PRQA S 3081
        }
		else if (dynamic_cast<BreakpointList*>(_aggregates[1])) {// PRQA S 3081
		    _theBreakpointDescriptors = static_cast<BreakpointList*>(_aggregates[1]); // PRQA S 3081
        }
		else if (dynamic_cast<TaskParameterList*>(_aggregates[1])) {// PRQA S 3081
		    _theParameterDescriptors = static_cast<TaskParameterList*>(_aggregates[1]); // PRQA S 3081
        }
        else { /* nothing */ }


		// third item should be Output (according to constructor)
		if (dynamic_cast<OutputList*>(_aggregates[2])) {// PRQA S 3081, 4400
		    _theOutputDescriptors = static_cast<OutputList*>(_aggregates[2]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<InputList*>(_aggregates[2])) {// PRQA S 3081, 4400
		    _theInputDescriptors = static_cast<InputList*>(_aggregates[2]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<BreakpointList*>(_aggregates[2])) {// PRQA S 3081, 4400
		    _theBreakpointDescriptors = static_cast<BreakpointList*>(_aggregates[2]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<TaskParameterList*>(_aggregates[2])) {// PRQA S 3081, 4400
		    _theParameterDescriptors = static_cast<TaskParameterList*>(_aggregates[2]); // PRQA S 3081, 4400
        }
        else { /* nothing */ }

		// fourth item should be Breakpoint (according to constructor)
		if (dynamic_cast<BreakpointList*>(_aggregates[3])) {// PRQA S 3081, 4400
		    _theBreakpointDescriptors = static_cast<BreakpointList*>(_aggregates[3]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<InputList*>(_aggregates[3])) {// PRQA S 3081, 4400
		    _theInputDescriptors = static_cast<InputList*>(_aggregates[3]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<OutputList*>(_aggregates[3])) {// PRQA S 3081, 4400
		    _theOutputDescriptors = static_cast<OutputList*>(_aggregates[3]); // PRQA S 3081, 4400
        }
		else if (dynamic_cast<TaskParameterList*>(_aggregates[3])) {// PRQA S 3081, 4400
		    _theParameterDescriptors = static_cast<TaskParameterList*>(_aggregates[3]); // PRQA S 3081, 4400
        }
        else { /* nothing */ }


	}

	return *this;
}


void
Task::addInput(const InputDescriptor& aDescriptor) // PRQA S 4211
{
	_theInputDescriptors->addElement(aDescriptor);
}

void
Task::addInputs(const vector<InputDescriptor>::const_iterator &from, const vector<InputDescriptor>::const_iterator &to) // PRQA S 4211
{
	vector<InputDescriptor>::const_iterator p;
	for (p=from; p!=to; ++p) {
		_theInputDescriptors->addElement(*p);
    }
}

void
Task::addInputs(const vector<InputDescriptor>& aList)
{
	addInputs(aList.begin(), aList.end());
}

void
Task::addOutput(const OutputDescriptor& aDescriptor) // PRQA S 4211
{
	_theOutputDescriptors->addElement(aDescriptor);
}

void
Task::addBreakpoint(const BreakpointDescriptor& aDescriptor) // PRQA S 4211
{
	_theBreakpointDescriptors->addElement(aDescriptor);
}

void
Task::addParameter (const TaskParameterDescriptor& aDescriptor) // PRQA S 4211
{
	_theParameterDescriptors->addElement(aDescriptor);
}

void
Task::clear()
{
	// DON'T delete containers!!!!!
	// delete their contents, instead...
	clearInputs();
	clearOutputs();
	clearBreakpoints();
	clearParameters();
}



void
Task::clearInputs() // PRQA S 4211
{
	_theInputDescriptors->clear();
}


void
Task::clearOutputs() // PRQA S 4211
{
	_theOutputDescriptors->clear();
}


void
Task::clearBreakpoints() // PRQA S 4211
{
	_theBreakpointDescriptors->clear();
}

void
Task::clearParameters() // PRQA S 4211
{
	_theParameterDescriptors->clear();
}



bool
Task::isSetCriticalityLevel () const // PRQA S 4120
{
	return _criticalityLevelSet;
}

int
Task::getCriticalityLevel () const // PRQA S 4120
{
	return _criticalityLevel;
}

void
Task::setCriticalityLevel (int level)
{
	_criticalityLevelSet = true;
	_criticalityLevel = level;
}


bool
Task::isSetName () const // PRQA S 4120
{
	return _nameSet;
}

void
Task::setName(const string& NewName)
{
	_nameSet = true;
	_name = NewName;
}

const string &
Task::getName() const // PRQA S 4120
{
	return _name;
}


bool
Task::isSetVersion () const // PRQA S 4120
{
	return _versionSet;
}

void
Task::setVersion(const string& NewVersion)
{
	_versionSet = true;
	_version = NewVersion;
}

const string &
Task::getVersion() const // PRQA S 4120
{
	return _version;
}


bool
Task::isSetPath () const // PRQA S 4120
{
	return _pathSet;
}

void
Task::setPath(const string& NewPath)
{
	_pathSet = true;
	_path = NewPath;
}

const string &
Task::getPath() const // PRQA S 4120
{
	return _path;
}

bool
Task::isSetCritical () const // PRQA S 4120
{
	return _isCriticalSet;
}

bool
Task::isCritical() const // PRQA S 4120
{
	return _isCritical;
}

void
Task::isCritical(bool val)
{
	_isCriticalSet = true;
	_isCritical = val;
}

bool 
Task::isSetNumberOfCPUs() const
{
    return _isNumberOfCPUsSet;
}

unsigned int 
Task::getNumberOfCPUs () const
{
    return _numberOfCPUs;
}

void 
Task::setNumberOfCPUs (unsigned int c)
{
    _numberOfCPUs = c;
    _isNumberOfCPUsSet = true;
}



void 
Task::getLocalTaskList (vector<Task> & aList, bool erase) const
{
	if (erase) 
	{
		aList.clear();
	}

	aList.push_back (*this);
}

void 
Task::normalize(WeightNormalizer* theNormalizer)
{
	ACS_SMARTPTR<WeightNormalizer> defNormalizer ;

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



bool
Task::acceptEnterVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.enterVisit(*this);
}


bool
Task::acceptExitVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.exitVisit(*this);
}

bool
Task::acceptVisit (TaskTableVisitor& theVisitor)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return theVisitor.visit (*this);
}

bool 
Task::isSetPosixExitCode() const
{
	return _isPosixExitCodeSet;
}

bool 
Task::isPosixExitCode () const
{
	return _isPosixExitCode;
}

void 
Task::isPosixExitCode (bool isPosix)
{
	_isPosixExitCodeSet = true;
	_isPosixExitCode = isPosix;
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Posix exit code compliancy set to " << boolalpha << isPosix);
}
_ACS_END_NAMESPACE
