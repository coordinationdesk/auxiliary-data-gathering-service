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

#ifndef _SigPipeGuard_h_
#define _SigPipeGuard_h_

#include <acs_c++config.hpp>
#include <SigPipeManagerSingleton.h>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SigPipeGuard // PRQA S 2109
{
public:
	SigPipeGuard();
	~SigPipeGuard();
private:
	SigPipeGuard(const SigPipeGuard &); // not implemented
	SigPipeGuard& operator= (const SigPipeGuard &); // not implemented

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SigPipeGuard) ;
};


_ACS_END_NAMESPACE

#endif // _SigPipeGuard_h_

