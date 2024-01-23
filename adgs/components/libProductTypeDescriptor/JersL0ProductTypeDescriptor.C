// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProductTypeDescriptor$

	$Id$

	$Author$

	$Log$
	Revision 2.2  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.1  2007/10/01 13:40:17  nicvac
	Jers added.
	
        
*/ 

#include <JersL0ProductTypeDescriptor.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

JersL0ProductTypeDescriptor::JersL0ProductTypeDescriptor():
	L0ProductTypeDescriptor("Jers")
{
}

JersL0ProductTypeDescriptor::JersL0ProductTypeDescriptor(const string& satName) :
	L0ProductTypeDescriptor(satName)
{
}


JersL0ProductTypeDescriptor::~JersL0ProductTypeDescriptor() throw()
{
}

_ACS_END_NAMESPACE

