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
	Revision 2.3  2012/06/05 15:39:18  marfav
	code improved to remove compiler warnings
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/


#include <OutputDescriptor.h>

#include <TaskTableVisitor.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


// class OutputList
ACS_CLASS_DEFINE_DEBUG_LEVEL(OutputList)


OutputList::OutputList(): TaskTableVisitable()
{
}


OutputList::OutputList(const OutputList& l) : TaskTableVisitable()
{
	*this = l;
}

OutputList&
OutputList::operator =(const OutputList& l)
{
	if (this != &l)
	{
		TaskTableVisitable::operator= (l);
	}
	return *this;
}

TaskTableVisitable*
OutputList::clone() const
{
	OutputList* newOutputList = new OutputList (*this);
	return newOutputList;
}


bool
OutputList::acceptEnterVisit(TaskTableVisitor& v)
{
	return v.enterVisit(*this);
}


bool
OutputList::acceptVisit(TaskTableVisitor& v)
{
	return v.visit(*this);
}


bool
OutputList::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}



// class OutputDescriptor

ACS_CLASS_DEFINE_DEBUG_LEVEL(OutputDescriptor)

OutputDescriptor::OutputDescriptor() : 
	TaskTableVisitable(),
	_Destination(OutputDestDB),
	_IsMandatory(false),
	_FileType(),
	_FileTypeTag("File_Type"),
	_PathName(),
	_NameType(FileNameTypeUNKNOWN),
	_DestinationSet(false),
	_IsMandatorySet(false),
	_FileTypeSet(false),
	_PathNameSet(false),
	_NameTypeSet(false)
	
{
}


OutputDescriptor::OutputDescriptor(const OutputDescriptor& d) : 
    TaskTableVisitable(),
	_Destination(OutputDestDB),
	_IsMandatory(false),
	_FileType(),
	_FileTypeTag("File_Type"),
	_PathName(),
	_NameType(FileNameTypeUNKNOWN),
	_DestinationSet(false),
	_IsMandatorySet(false),
	_FileTypeSet(false),
	_PathNameSet(false),
	_NameTypeSet(false)
{
	*this = d;
}

OutputDescriptor&
OutputDescriptor::operator =(const OutputDescriptor& d)
{
	if (this != &d)
	{
		_Destination    = d._Destination;
		_FileType       = d._FileType;
		_FileTypeTag    = d._FileTypeTag;
		_IsMandatory    = d._IsMandatory;
		_PathName       = d._PathName;
		_NameType       = d._NameType;
		_DestinationSet = d._DestinationSet;
		_FileTypeSet    = d._FileTypeSet;
		_IsMandatorySet = d._IsMandatorySet;
		_PathNameSet    = d._PathNameSet;
		_NameTypeSet    = d._NameTypeSet;
		TaskTableVisitable::operator= (d);
	}
	return *this;
}

bool
OutputDescriptor::isSetFileNameType() const // PRQA S 4120
{
	return _NameTypeSet;
}


FileNameType
OutputDescriptor::getFileNameType() const // PRQA S 4120
{
	return _NameType;
}

void
OutputDescriptor::setFileNameType(FileNameType aType)
{
	_NameTypeSet = true;
	_NameType = aType;
}


bool
OutputDescriptor::isSetDestination() const // PRQA S 4120
{
	return _DestinationSet;
}

OutputDest
OutputDescriptor::getDestination() const // PRQA S 4120
{
	return _Destination;
}     	


bool
OutputDescriptor::isSetMandatory() const // PRQA S 4120
{
	return _IsMandatorySet;
}


bool
OutputDescriptor::isMandatory() const // PRQA S 4120
{
	return _IsMandatory;
}

void
OutputDescriptor::isMandatory(bool val)
{
	_IsMandatorySet = true;
	_IsMandatory = val;
}


bool
OutputDescriptor::isSetFileType () const // PRQA S 4120
{
	return _FileTypeSet;
}

string
OutputDescriptor::getFileType() const // PRQA S 4120
{
	return _FileType;
}             	

string
OutputDescriptor::getPathName() const // PRQA S 4120
{
	return _PathName;
}


void
OutputDescriptor::setDestination(OutputDest aDestination)
{
	_DestinationSet = true;
	_Destination = aDestination;
}

void
OutputDescriptor::setFileType(const string& aType)
{
	_FileTypeSet = true;
	_FileType = aType;
}

void
OutputDescriptor::setPathName(const string& aPath)
{
	_PathNameSet = true;
	_PathName = aPath;
}

void OutputDescriptor::setFileTypeTag(const string& aTypeTag)
{
	_FileTypeTag = aTypeTag ;
}

string OutputDescriptor::getFileTypeTag() const
{
	return _FileTypeTag ;
}

bool
OutputDescriptor::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
OutputDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
OutputDescriptor::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

TaskTableVisitable*
OutputDescriptor::clone() const
{
	OutputDescriptor* newOutputDescriptor = new OutputDescriptor (*this);
	return newOutputDescriptor;
}

_ACS_END_NAMESPACE
