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
	Revision 1.2  2015/06/16 13:05:41  damdec
	ACS macros used.
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#ifndef _AlternativeCheckerFactoryChain_H_
#define _AlternativeCheckerFactoryChain_H_

#include<Alternative_PCC.h>

#include<exException.h>
#include<StringKeyFactory.h>



_ACS_BEGIN_NAMESPACE(acs)

typedef StringKeyFactory<Alternative_PCC, PCC_Base::PCC_ParamsPtr>::SingletonType AlternativeCheckerFactoryChain;

_ACS_END_NAMESPACE


#endif // _AlternativeCheckerFactoryChain_H_
