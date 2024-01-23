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
	Revision 2.2  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.1  2007/07/02 15:18:41  nicvac
	Mos added.
	

*/

#include <MosL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

//default class constructor
MosL0ProductTypeDescriptor::MosL0ProductTypeDescriptor() :
	L0ProductTypeDescriptor("Mos")
{
}


//class constructor
MosL0ProductTypeDescriptor::MosL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


//Class destructor
MosL0ProductTypeDescriptor::~MosL0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE

