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

	$Prod: A.C.S. Signal Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/01/25 11:16:20  marpas
	using Singleton pattern form libException
	coding best practices
	qa rules
	
	Revision 2.2  2012/11/06 17:44:33  marpas
	Quality improved, coverage test done
	
	Revision 2.1  2012/02/09 14:30:28  marpas
	refactoring
	
	Revision 2.0  2006/02/28 08:29:41  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/14 12:13:38  marfav
	Added SigMask classes to library
	

*/


#ifndef _SigMaskGuard_h_
#define _SigMaskGuard_h_

#include <acs_c++config.hpp>

#include <SigMaskManagerSingleton.h>

#include <exException.h>

// This class registers a Mask for a Signal in the CTOR
// using the SigMaskManagerSingleton object
// and unregisters it in the DTOR
// Nested guards are allowed for the same signal and are managed by the SigMaskManagerSingleton object

_ACS_BEGIN_NAMESPACE(acs)

class SigMaskGuard      // PRQA S 2109
{
public:
	explicit SigMaskGuard(int);
	~SigMaskGuard() ACS_NOEXCEPT ;
private:
    SigMaskGuard() = delete ;
    SigMaskGuard( const SigMaskGuard &) = delete ;
    SigMaskGuard & operator= ( const SigMaskGuard &) = delete ;
    SigMaskGuard( SigMaskGuard &&) = delete ;  // PRQA S 2038 2
    SigMaskGuard & operator= ( SigMaskGuard &&) = delete ;

private:
	int _signal;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SigMaskGuard) ;
};


_ACS_END_NAMESPACE


#endif // _SigMaskGuard_h_

