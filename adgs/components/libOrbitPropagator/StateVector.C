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

#include <StateVector.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


//Class constructors
StateVector::StateVector() :
	procTime_(),
	asciiTime_(),
	_pos(),
	_vel(),
	_acc(),
	_refSystem(PropagFormat::OP_REFCOORD_UNDEF)
{
} 




//Class destructor
StateVector::~StateVector() throw()
{
}



//copy constructor
StateVector::StateVector(const StateVector & e) :
	procTime_(e.procTime_),
	asciiTime_(e.asciiTime_),
	_pos(e._pos),
	_vel(e._vel),
	_acc(e._acc),
	_refSystem(e._refSystem)
{
}

//operator = 
StateVector&  StateVector::operator=(const StateVector & e)
{
	if (this != &e) {
		procTime_	= e.procTime_;
		asciiTime_	= e.asciiTime_;
		_pos		= e._pos;
		_vel		= e._vel;
		_acc		= e._acc;
		_refSystem	= e._refSystem;
	}
	return *this ;
}

string StateVector::show() const 
{
	ostringstream msg;
	msg << "StateVector::show" << '\n'
		<< "--- refSystem  = " <<  static_cast<int>(_refSystem) << '\n' // PRQA S 3081
		<< "--- procTime = " << setprecision(16) << procTime_.getValue() << '\n'; // PRQA S 4400 L1
	if(_pos.size() == 3)
	{
		msg << "--- X Position = " << setprecision(12) << _pos[0] << " m " << '\n'
			<< "--- Y Position = " << setprecision(12) << _pos[1] << " m " << '\n'
			<< "--- Z Position = " << setprecision(12) << _pos[2] << " m "  << '\n';
	}
	else
	{
		msg << "--- X Position = 0 m" << '\n'
			<< "--- Y Position = 0 m" << '\n'
			<< "--- Z Position = 0 m" << '\n';	
	}
	
	if(_vel.size() == 3)
	{
		msg << "--- X Velocity = " << setprecision(12) << _vel[0] << " m/s "<< '\n'
			<< "--- Y Velocity = " << setprecision(12) << _vel[1] << " m/s "<< '\n'
			<< "--- Z Velocity = " << setprecision(12) << _vel[2] << " m/s "<< '\n';
	}
	else
	{
		msg << "--- X Velocity = 0 m/s " << '\n'
			<< "--- Y Velocity = 0 m/s " << '\n'
			<< "--- Z Velocity = 0 m/s " << '\n';	
	}
	
	if(_acc.size() == 3)
	{
		msg << "--- X Acceleration = " << setprecision(12) << _acc[0] << " m/s^2 "<< '\n'
			<< "--- Y Acceleration = " << setprecision(12) << _acc[1] << " m/s^2 "<< '\n'
			<< "--- Z Acceleration = " << setprecision(12) << _acc[2] << " m/s^2 "<< '\n';
	}
	else
	{
		msg << "--- X Acceleration = 0 m/s^2 " << '\n'
			<< "--- Y Acceleration = 0 m/s^2 " << '\n'
			<< "--- Z Acceleration = 0 m/s^2 " << '\n';	
	}
	// PRQA L:L1
	return msg.str();
}

_ACS_END_NAMESPACE
