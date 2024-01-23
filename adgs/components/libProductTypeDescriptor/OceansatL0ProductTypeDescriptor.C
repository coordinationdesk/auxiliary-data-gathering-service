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
	Revision 1.4  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 1.3  2008/10/22 16:17:42  giucas
	Call to setSatellite fixed
	
	Revision 1.2  2008/10/22 16:11:03  giucas
	Call to base class fixed
	
	Revision 1.1  2008/09/29 13:39:24  giucas
	First issue
	


*/

#include <OceansatL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
OceansatL0ProductTypeDescriptor::OceansatL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Oceansat")
{
}


//class constructor
OceansatL0ProductTypeDescriptor::OceansatL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}

//Class destructor
OceansatL0ProductTypeDescriptor::~OceansatL0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE
