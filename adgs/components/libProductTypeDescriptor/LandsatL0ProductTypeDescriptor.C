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
	Revision 1.3  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 1.2  2008/03/27 08:40:43  giucas
	getNameByApidVcidClass method implemented.
	
	Revision 1.1  2006/10/05 12:35:56  giucas
	Added
	

*/

#include <LandsatL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

//default class constructor
LandsatL0ProductTypeDescriptor::LandsatL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Landsat")
{
}


//class constructor
LandsatL0ProductTypeDescriptor::LandsatL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
LandsatL0ProductTypeDescriptor::~LandsatL0ProductTypeDescriptor() throw() 
{
}
_ACS_END_NAMESPACE

