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
	Revision 2.4  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.3  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.2  2006/04/21 11:54:59  marfav
	Breakpoints have no file name type in the TT xml schema
	but have the related attribute in the class
	PHYSICAL value is used as default to avoid Job Order building problems
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <BreakpointDescriptor.h>

#include <TaskTableVisitor.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(BreakpointDescriptor)
ACS_CLASS_DEFINE_DEBUG_LEVEL(BreakpointList)


BreakpointList::BreakpointList() : TaskTableVisitable()
{
}


BreakpointList::BreakpointList(const BreakpointList& l) : TaskTableVisitable()
{
	*this = l;
}

BreakpointList&
BreakpointList::operator =(const BreakpointList& l)
{
	if (this != &l)
	{
		TaskTableVisitable::operator= (l);
	}
	return *this;
}


// class BreakpointList
TaskTableVisitable*
BreakpointList::clone() const
{
	BreakpointList* newBreakpointList = new BreakpointList (*this);
	return newBreakpointList;
}


bool
BreakpointList::acceptEnterVisit(TaskTableVisitor& v)
{
	return v.enterVisit(*this);
}


bool
BreakpointList::acceptVisit(TaskTableVisitor& v)
{
	return v.visit(*this);
}


bool
BreakpointList::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}

// class BreakpointDescriptor
BreakpointDescriptor::BreakpointDescriptor() : 
	TaskTableVisitable(),
	_FileType(""),
	_FileTypeSet(false),
	_PathName(""),
	_PathNameSet(false),
	_NameType (FileNameTypePHYSICAL),
	_NameTypeSet(false)
	
{
}


BreakpointDescriptor::BreakpointDescriptor(const BreakpointDescriptor& d) : 
   	TaskTableVisitable(),
	_FileType(),
	_FileTypeSet(),
	_PathName(),
	_PathNameSet(),
	_NameType (),
	_NameTypeSet()

{
	*this = d;
}

BreakpointDescriptor&
BreakpointDescriptor::operator =(const BreakpointDescriptor& d)
{
	if (this != &d)
	{
		_FileType    = d._FileType;
		_PathName    = d._PathName;
		_NameType    = d._NameType;
		_FileTypeSet = d._FileTypeSet;
		_PathNameSet = d._PathNameSet;
		_NameTypeSet = d._NameTypeSet;
		TaskTableVisitable::operator= (d);
	}
	return *this;
}


bool
BreakpointDescriptor::isSetFileType() const // PRQA S 4120
{
	return _FileTypeSet;
}

string
BreakpointDescriptor::getFileType() const // PRQA S 4120
{
	return _FileType;
}

void
BreakpointDescriptor::setFileType(const string& aType)
{
	_FileTypeSet = true;
	_FileType = aType;
}


bool
BreakpointDescriptor::isSetFileNameType() const // PRQA S 4120
{
	return _NameTypeSet;
}

FileNameType
BreakpointDescriptor::getFileNameType() const // PRQA S 4120
{
	return _NameType;
}

void
BreakpointDescriptor::setFileNameType(FileNameType aType)
{
	_NameTypeSet = true;
	_NameType = aType;
}


bool
BreakpointDescriptor::isSetPathName() const // PRQA S 4120
{
	return _PathNameSet;
}

string
BreakpointDescriptor::getPathName() const // PRQA S 4120
{
	return _PathName;
}

void
BreakpointDescriptor::setPathName(const string& aPath)
{
	_PathNameSet = true;
	_PathName = aPath;
}

bool
BreakpointDescriptor::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
BreakpointDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
BreakpointDescriptor::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

TaskTableVisitable*
BreakpointDescriptor::clone() const
{
	BreakpointDescriptor* newBreakpointDescriptor = new BreakpointDescriptor (*this);
	return newBreakpointDescriptor;
}

_ACS_END_NAMESPACE
