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

*/

#include <RelOrbitInfo.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


//Class constructors
RelOrbitInfo::RelOrbitInfo() :
	_relOrbit(0),	
	_cycle(0),
	_phase(0),
	_extraInfo()
{
} 



//Class destructor
RelOrbitInfo::~RelOrbitInfo() throw()
{
}



//copy constructor
RelOrbitInfo::RelOrbitInfo(const RelOrbitInfo & e) :
	_relOrbit(e._relOrbit),	
	_cycle(e._cycle),
	_phase(e._phase),
	_extraInfo(e._extraInfo)
{
}

//operator = 
RelOrbitInfo&  RelOrbitInfo::operator=(const RelOrbitInfo & e)
{
	if (this != &e) 
	{
		_relOrbit		= e._relOrbit;
		_cycle			= e._cycle;
		_phase			= e._phase;
		_extraInfo		= e._extraInfo;
	}
	return *this ;
}

string RelOrbitInfo::show() const 
{
	ostringstream msg;
	msg << "RelOrbitInfo::show" << '\n'
		<< "--- relOrbit = " <<  _relOrbit << '\n'
		<< "--- cycle    = " <<  _cycle << '\n'
		<< "--- phase    = " <<  _phase << '\n';
	return msg.str();
}
_ACS_END_NAMESPACE


