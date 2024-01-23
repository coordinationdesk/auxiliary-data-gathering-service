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
	Revision 2.6  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.5  2011/06/07 07:19:00  giucas
	Slicing parameters made double
	
	Revision 2.4  2011/06/01 06:43:09  giucas
	Added slice parameters retrieval
	
	Revision 2.3  2009/12/11 13:19:18  giucas
	Added handling of ListOfFields
	
	Revision 2.2  2009/11/12 17:08:28  giucas
	Satellite name fixed
	
	Revision 2.1  2009/11/09 10:10:09  giucas
	Added
	

*/

#include <Sentinel1L0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
Sentinel1L0ProductTypeDescriptor::Sentinel1L0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Sentinel1")
{
}


//class constructor
Sentinel1L0ProductTypeDescriptor::Sentinel1L0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
Sentinel1L0ProductTypeDescriptor::~Sentinel1L0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE
