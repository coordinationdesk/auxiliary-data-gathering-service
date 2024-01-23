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

#include <SigMaskGuard.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(SigMaskGuard)

SigMaskGuard::SigMaskGuard(int sig) : _signal(sig)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding a guard to singleton for signal [" << _signal << "]") ;

	// This can throw exceptions
	// The exceptions are not handled and filter out to the owner of the class
	SigMaskManagerSingleton::instance() -> addGuard(_signal);

}

SigMaskGuard::~SigMaskGuard() ACS_NOEXCEPT 
{
    try { // PRQA S 4631 L1
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "removing a guard to singleton for signal [" << _signal << "]") ; 

	    // This can throw exceptions
	    // The exceptions are not handled and filter out to the owner of the class
	    SigMaskManagerSingleton::instance() -> removeGuard(_signal);
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    } // PRQA L:L1
}


_ACS_END_NAMESPACE

