// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
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

#include <EnvisatL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
EnvisatL0ProductTypeDescriptor::EnvisatL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Envisat")
{
}


//class constructor
EnvisatL0ProductTypeDescriptor::EnvisatL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
EnvisatL0ProductTypeDescriptor::~EnvisatL0ProductTypeDescriptor() throw()
{
}


_ACS_END_NAMESPACE
