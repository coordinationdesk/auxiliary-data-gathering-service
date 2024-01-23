// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.2  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 1.1  2008/10/22 16:20:13  giucas
	First issue
	

*/

#include <AlosL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
AlosL0ProductTypeDescriptor::AlosL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Alos")
{
}


//class constructor
AlosL0ProductTypeDescriptor::AlosL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
AlosL0ProductTypeDescriptor::~AlosL0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE
