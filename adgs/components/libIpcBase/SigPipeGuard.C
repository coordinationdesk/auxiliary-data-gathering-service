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

	$Prod: A.C.S. ThinLayer IPC Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2007/02/19 11:05:17  marant
	Import libIpcBase
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/

#include <SigPipeGuard.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SigPipeGuard)

SigPipeGuard::SigPipeGuard()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "adding a guard to singleton") ;

	// This can throw exceptions
	// The exceptions are not handled and filter out to the owner of the class
	SigPipeManagerSingleton::instance() -> addGuard();

}

SigPipeGuard::~SigPipeGuard()
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "removing a guard from singleton" );

	// This can throw exceptions
	// The exceptions are not handled and filter out to the owner of the class
	SigPipeManagerSingleton::instance() -> removeGuard();
    // PRQA L:L1
}

_ACS_END_NAMESPACE


