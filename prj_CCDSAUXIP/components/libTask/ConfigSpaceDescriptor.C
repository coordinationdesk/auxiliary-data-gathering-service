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

#include <ConfigSpaceDescriptor.h>  

#include <TaskTableVisitor.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


// class ConfigSpaceDescriptor

ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigSpaceDescriptor)


ConfigSpaceDescriptor::ConfigSpaceDescriptor(const string& aSpaceName, const string& aPath) :
	TaskTableVisitable(),
	_SpaceName (aSpaceName),
	_Path (aPath),
	_SpaceNameSet(false),
	_PathSet(false)
{
}


ConfigSpaceDescriptor::ConfigSpaceDescriptor(const ConfigSpaceDescriptor& d) : 
    TaskTableVisitable(),
   	_SpaceName (),
	_Path (),
	_SpaceNameSet(),
	_PathSet()
{
	*this = d;
}

ConfigSpaceDescriptor&
ConfigSpaceDescriptor::operator =(const ConfigSpaceDescriptor& d)
{
	if (this != &d)
	{
		_SpaceName    = d._SpaceName;
		_Path         = d._Path;
		_SpaceNameSet = d._SpaceNameSet;
		_PathSet      = d._PathSet;
		TaskTableVisitable::operator= (d);
	}
	return *this;
}


bool
ConfigSpaceDescriptor::isSetSpaceName () const // PRQA S 4120
{
	return _SpaceNameSet;
}

void
ConfigSpaceDescriptor::setSpaceName(const string& aSpaceName)
{
	_SpaceNameSet = true;
	_SpaceName = aSpaceName;
}


string
ConfigSpaceDescriptor::getSpaceName() const // PRQA S 4120
{
	return _SpaceName;
}


bool
ConfigSpaceDescriptor::isSetPath () const // PRQA S 4120
{
	return _PathSet;
}


void
ConfigSpaceDescriptor::setPath(const string& aPath)
{
	_PathSet = true;
	_Path= aPath;
}

string
ConfigSpaceDescriptor::getPath() const // PRQA S 4120
{
	return _Path;
}

bool
ConfigSpaceDescriptor::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
ConfigSpaceDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
ConfigSpaceDescriptor::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

TaskTableVisitable*
ConfigSpaceDescriptor::clone() const
{
	ConfigSpaceDescriptor* newConfigSpaceDescriptor = new ConfigSpaceDescriptor (*this);
	return newConfigSpaceDescriptor;
}

_ACS_END_NAMESPACE
