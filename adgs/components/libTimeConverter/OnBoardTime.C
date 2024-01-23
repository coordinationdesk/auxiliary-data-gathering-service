// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2012/04/18 08:48:40  giucas
	compilation warnings removed
	
	Revision 2.1  2012/02/13 17:04:09  marpas
	removing compiler warnings
	
	Revision 2.0  2006/02/28 09:14:47  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/09/17 13:59:33  giucas
	Added ability to handle counter in double format
	
	Revision 1.1  2003/09/03 14:41:31  giucas
	First release
	

*/

#include <OnBoardTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OnBoardTime)


//Class constructors
OnBoardTime::OnBoardTime() :
	_valueVec(),
	_value(0.0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
} 

OnBoardTime::OnBoardTime(const std::vector<unsigned long int>& value) :
	_valueVec(value),
	_value(0.0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
} 

OnBoardTime::OnBoardTime(const double& value) :
	_valueVec(),
	_value(value)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
} 


//Class destructor
OnBoardTime::~OnBoardTime() 
{
}



//copy constructor
OnBoardTime::OnBoardTime(const OnBoardTime & e) :
	_valueVec(e._valueVec),
	_value(e._value)
{
}


//operator = 
OnBoardTime&  OnBoardTime::operator=(const OnBoardTime & e)
{
	if (this != &e) 
	{
		_value		= e._value;
		_valueVec	= e._valueVec;
	}
	return *this ;
}
_ACS_END_NAMESPACE
