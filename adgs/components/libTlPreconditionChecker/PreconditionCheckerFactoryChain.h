// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2008/04/29 12:45:40  marfav
	Added support for PreconditionCheckFactoryChain
	
	

*/

#ifndef _PreconditionCheckerFactoryChain_H_
#define _PreconditionCheckerFactoryChain_H_

#include<ThinLayer_PCC.h>
#include<StringKeyFactory.h>

#include<exException.h>



_ACS_BEGIN_NAMESPACE(acs)

typedef StringKeyFactory<ThinLayer_PCC, StringKey>::SingletonType PreconditionCheckerFactoryChain;

_ACS_END_NAMESPACE


#endif // _PreconditionCheckerFactoryChain_H_
