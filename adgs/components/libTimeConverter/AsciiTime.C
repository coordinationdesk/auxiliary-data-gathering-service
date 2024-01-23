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
    Revision 2.3  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization
 
*/

#include <AsciiTime.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


AsciiTime::AsciiTime(const string& v,
					 TimeFormat::TimeReference r, 
					 TimeFormat::AsciiTimeFormat  f) :
	_value(v),	
	_reference(r),
	_format(f)
{
} 


//Class destructor
AsciiTime::~AsciiTime() throw()
{
}



//copy constructor
AsciiTime::AsciiTime(const AsciiTime & e) :
	_value(e._value),	
	_reference(e._reference),
	_format(e._format)
{
}


/////////////////////////////////////////////////////////////////////////////////////////
// operators 
/////////////////////////////////////////////////////////////////////////////////////////

//operator = 
AsciiTime&  AsciiTime::operator=(const AsciiTime & e)
{
	if (this != &e) {
		_value		= e._value;
		_reference	= e._reference;
		_format		= e._format;
	}
	return *this ;
}
_ACS_END_NAMESPACE

