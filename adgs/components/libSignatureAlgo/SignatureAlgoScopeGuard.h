// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Signature Algo Library$

	$Id$

	$Author$

	$Log$
	Revision 2.1  2006/03/08 14:37:24  ivafam
	Added in PDS Rose Model
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	

*/


#ifndef _SignatureAlgoScopeGuard_H_
#define _SignatureAlgoScopeGuard_H_ 

#include <acs_c++config.hpp>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class SignatureAlgo ;

typedef ACS_SMARTPTR<SignatureAlgo> SignatureAlgoScopeGuard ;


_ACS_END_NAMESPACE


#endif // _SignatureAlgoScopeGuard_H_
