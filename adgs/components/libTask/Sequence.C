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
	Revision 2.4  2013/07/07 18:22:08  marpas
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.3  2012/02/10 16:50:37  marpas
	refactoring in progress
	
	Revision 2.2  2008/04/22 11:07:46  marfav
	Task Table elemenets now use the conditioned visitor design pattern
	
	Revision 2.1  2006/03/27 18:03:24  fracar
	added id/def management
	
	Revision 2.0  2006/03/06 11:54:23  marfav
	Exception 2.1 I/F adopted
	
	Revision 1.1  2006/03/06 11:43:26  marfav
	New Task Library having recursive definition
	

*/

#include <Sequence.h>

#include <TaskTableVisitor.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Sequence)

Sequence::Sequence() : Pool()
{
}

Sequence::Sequence(const Sequence& s) : Pool(s)
{
}

Sequence::~Sequence() throw() 
{
	// Empty DTOR
}


TaskTableElement*
Sequence::clone() const
{
	Sequence* newSequence = new Sequence (*this);
	return newSequence;
}


Sequence&
Sequence::operator= (const Sequence& s)
{
	if (this != &s) 
	{
		Pool::operator= (s);
	}
	return *this;
}

bool
Sequence::acceptVisit (TaskTableVisitor& theVisitor)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return theVisitor.visit (*this);
}


bool
Sequence::acceptEnterVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.enterVisit(*this);
}


bool
Sequence::acceptExitVisit(TaskTableVisitor& v)
{
    ACS_CLASS_ANNOUNCE_DEBUG_TH ( ACS_MID_VERB )

    return v.exitVisit(*this);
}

_ACS_END_NAMESPACE
