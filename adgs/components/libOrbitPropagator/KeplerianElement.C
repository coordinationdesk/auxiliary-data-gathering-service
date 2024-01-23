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

*/

#include <KeplerianElement.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


//Class constructors
KeplerianElement::KeplerianElement() :
	procTime_(),
    asciiTime_(),
	_semiMajorAxisNormalised(0.0), 
    _eccentricity(0.0), 
    _inclination(0.0),
	_rightAscentionANX(0.0), 
    _argumentOfPerigee(0.0), 
    _meanAnomaly(0.0),
	_refSystem(PropagFormat::OP_REFCOORD_UNDEF)
{
} 




//Class destructor
KeplerianElement::~KeplerianElement() throw()
{
}



//copy constructor
KeplerianElement::KeplerianElement(const KeplerianElement & e) :
	procTime_(e.procTime_),
    asciiTime_(e.asciiTime_),
	_semiMajorAxisNormalised(e._semiMajorAxisNormalised), 
    _eccentricity(e._eccentricity), 
    _inclination(e._inclination),
	_rightAscentionANX(e._rightAscentionANX), 
    _argumentOfPerigee(e._argumentOfPerigee), 
    _meanAnomaly(e._meanAnomaly),
	_refSystem(e._refSystem)
{
}

//operator = 
KeplerianElement&  KeplerianElement::operator=(const KeplerianElement & e)
{
	if (this != &e) {
		procTime_					= e.procTime_ ;
		asciiTime_					= e.asciiTime_ ;
		_semiMajorAxisNormalised	= e._semiMajorAxisNormalised;
		_eccentricity				= e._eccentricity;
		_inclination				= e._inclination;
		_rightAscentionANX			= e._rightAscentionANX;
		_argumentOfPerigee			= e._argumentOfPerigee;
		_meanAnomaly				= e._meanAnomaly;
		_refSystem					= e._refSystem;
	}
	return *this ;
}

string KeplerianElement::show() const 
{
	ostringstream msg;
	msg << "KeplerianElement::show" << '\n'
		<< "--- semiMajorAxisNormalised = " << setprecision(12) << _semiMajorAxisNormalised << '\n' // PRQA S 4400 6
		<< "--- eccentricity            = " << setprecision(12) << _eccentricity << '\n'
		<< "--- inclination	            = " << setprecision(12) << _inclination << '\n'
		<< "--- rightAscentionANX	    = " << setprecision(12) << _rightAscentionANX << '\n'
		<< "--- argumentOfPerigee	    = " << setprecision(12) << _argumentOfPerigee << '\n'
		<< "--- meanAnomaly	            = " << setprecision(12) << _meanAnomaly << '\n'
		<< "--- refSystem	            = " << static_cast<int>(_refSystem) << '\n'; // PRQA S 3081
	return msg.str();
}
_ACS_END_NAMESPACE

