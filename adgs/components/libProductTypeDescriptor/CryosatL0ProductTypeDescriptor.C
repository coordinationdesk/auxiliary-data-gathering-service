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
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.1  2004/08/02 16:04:06  giucas
	Imported source.
	


*/

#include <CryosatL0ProductTypeDescriptor.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
CryosatL0ProductTypeDescriptor::CryosatL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Cryosat")
{
}


//class constructor
CryosatL0ProductTypeDescriptor::CryosatL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
CryosatL0ProductTypeDescriptor::~CryosatL0ProductTypeDescriptor() throw()
{
}


_ACS_END_NAMESPACE

