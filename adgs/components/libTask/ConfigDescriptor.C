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

#include <ConfigDescriptor.h>

#include <TaskTableVisitor.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ConfigDescriptor)

// class ConfigDescriptor

ConfigDescriptor::ConfigDescriptor(const string& aVersion, const string& aPath) :
	TaskTableVisitable(),
	_Version(aVersion), 
	_VersionSet(false),
	_Path(aPath),
	_PathSet(false)
{
}


ConfigDescriptor::ConfigDescriptor(const ConfigDescriptor& d) :
	TaskTableVisitable(d),
	_Version(d._Version), 
	_VersionSet(d._VersionSet),
	_Path(d._Path),
	_PathSet(d._PathSet)
{
}

ConfigDescriptor&
ConfigDescriptor::operator =(const ConfigDescriptor& d)
{
	if (this != &d)
	{
		_Version    = d._Version;
		_Path       = d._Path;
		_VersionSet = d._VersionSet;
		_PathSet    = d._PathSet;
		TaskTableVisitable::operator= (d);
	}
	
	return *this;
}


bool
ConfigDescriptor::isSetPath () const // PRQA S 4120
{
	return _PathSet;
}

void
ConfigDescriptor::setPath(const string& aPath)
{
	_PathSet = true;
	_Path = aPath;
}

string
ConfigDescriptor::getPath() const // PRQA S 4120
{
	return _Path;
}


bool
ConfigDescriptor::isSetVersion() const // PRQA S 4120
{
	return _VersionSet;
}

void
ConfigDescriptor::setVersion(const string& aVersion)
{
	_VersionSet = true;
	_Version = aVersion;
}

string
ConfigDescriptor::getVersion() const // PRQA S 4120
{
	return _Version;
}

bool
ConfigDescriptor::acceptEnterVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.enterVisit (*this);
}

bool
ConfigDescriptor::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}

bool
ConfigDescriptor::acceptExitVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.exitVisit (*this);
}

TaskTableVisitable*
ConfigDescriptor::clone() const
{
	ConfigDescriptor* newConfigDescriptor = new ConfigDescriptor (*this);
	return newConfigDescriptor;
}

_ACS_END_NAMESPACE
