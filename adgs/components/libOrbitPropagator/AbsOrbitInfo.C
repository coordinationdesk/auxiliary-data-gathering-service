// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/02/13 11:03:11  giucas
	New log macros adopted
	

*/

#include <AbsOrbitInfo.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//Class constructors
AbsOrbitInfo::AbsOrbitInfo() :
	_absOrbit(0),	
	_deltaSecondsANX(0),
	_microseconds(0)
{
} 

//Class destructor
AbsOrbitInfo::~AbsOrbitInfo() throw()
{
}

//copy constructor
AbsOrbitInfo::AbsOrbitInfo(const AbsOrbitInfo & e) :
	_absOrbit(e._absOrbit),	
	_deltaSecondsANX(e._deltaSecondsANX),
	_microseconds(e._microseconds)
{
}

//operator = 
AbsOrbitInfo&  AbsOrbitInfo::operator=(const AbsOrbitInfo & e)
{
	if (this != &e) {
		_absOrbit			= e._absOrbit;
		_deltaSecondsANX	= e._deltaSecondsANX;
		_microseconds		= e._microseconds;
	}
	return *this ;
}

string AbsOrbitInfo::show() const 
{
	ostringstream msg;
	msg << "AbsOrbitInfo::show" << '\n'
		<< "--- absOrbit	    = " << _absOrbit << '\n'
		<< "--- deltaSecondsANX	= " << _deltaSecondsANX << '\n'
		<< "--- microseconds	= " << _microseconds << '\n';
	return msg.str();
}
_ACS_END_NAMESPACE
