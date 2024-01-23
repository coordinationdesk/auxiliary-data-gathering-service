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

	$Prod: A.C.S. ThinLayer TaskTableElements Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/04/22 11:07:45  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <AtExitSequence.h>

#include <TaskTableVisitor.h>



using namespace std;
using namespace acs;


ACS_CLASS_DEFINE_DEBUG_LEVEL(AtExitSequence)

AtExitSequence::AtExitSequence() : 
	Sequence(),
	_onExitStatus(ExitStatusUNKNOWN),
	_overrideStatus(false),
	_onExitStatusSet(false),
	_overrideStatusSet(false)
{
	_interruptable = false;
}

AtExitSequence::AtExitSequence(const AtExitSequence& s) : 
	Sequence(s),
	_onExitStatus(s._onExitStatus),
	_overrideStatus(s._overrideStatus),
	_onExitStatusSet(s._onExitStatusSet),
	_overrideStatusSet(s._overrideStatusSet)
{
}

AtExitSequence::~AtExitSequence() throw() 
{
}

TaskTableElement*
AtExitSequence::clone() const
{
	AtExitSequence* newAtExitSequence = new AtExitSequence (*this);
	return newAtExitSequence;
}


bool
AtExitSequence::acceptEnterVisit(TaskTableVisitor& v)
{
	return v.enterVisit(*this);
}

bool
AtExitSequence::acceptVisit (TaskTableVisitor& theVisitor)
{
	return theVisitor.visit (*this);
}


bool
AtExitSequence::acceptExitVisit(TaskTableVisitor& v)
{
	return v.exitVisit(*this);
}

AtExitSequence&
AtExitSequence::operator = (const AtExitSequence& s)
{
	if (this != &s)
	{
		_onExitStatus = s._onExitStatus;
		_overrideStatus = s._overrideStatus;
		_onExitStatusSet = s._onExitStatusSet;
		_overrideStatusSet = s._overrideStatusSet;
		Sequence::operator= (s);
	}
	return *this;
}


bool
AtExitSequence::isSetExitStatus() const // PRQA S 4120
{
	return _onExitStatusSet;
}

void 
AtExitSequence::setExitStatus (ExitStatus e)
{
	_onExitStatusSet = true;
	_onExitStatus = e;
}

ExitStatus 
AtExitSequence::getExitStatus () const // PRQA S 4120
{
	return _onExitStatus;
}


bool
AtExitSequence::isSetOverrideStatus() const // PRQA S 4120
{
	return _overrideStatusSet;
}

void 
AtExitSequence::setOverrideStatus (bool o)
{
	_overrideStatusSet = true;
	_overrideStatus = o;
}

bool
AtExitSequence::getOverrideStatus () const // PRQA S 4120
{
	return _overrideStatus;
}


bool
AtExitSequence::isSetSequence() const
{
	return (_aggregates.size() > 0);
}


void 
AtExitSequence::normalize(WeightNormalizer* theNormalizer)
{
	ACS_SMARTPTR<WeightNormalizer> defNormalizer;

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


