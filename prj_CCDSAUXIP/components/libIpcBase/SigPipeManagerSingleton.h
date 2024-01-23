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
	Revision 1.3  2013/01/25 12:09:56  marpas
	using pattern::Singleton from libException
	
	Revision 1.2  2011/01/24 15:53:27  marpas
	Mutex.h does not longer exixts
	
	Revision 1.1.1.1  2007/02/19 11:05:17  marant
	Import libIpcBase
	
	Revision 2.0  2006/02/28 10:10:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 18:04:18  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:33:14  marfav
	Import libTlIpc
	


*/


#ifndef _SigPipeManagerSingleton_h_
#define _SigPipeManagerSingleton_h_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <exException.h>

#include <map>

#include <csignal>
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


class SigPipeManagerSingleton : public Singleton<SigPipeManagerSingleton> // PRQA S 2109, 2153
{

    friend SigPipeManagerSingleton* Singleton<SigPipeManagerSingleton>::instance(); // PRQA S 2107 
public:
	/*! class SigPipeManagerException declaration */
	exDECLARE_EXCEPTION(SigPipeManagerException, exException) ; // PRQA S 2131, 2502

	virtual ~SigPipeManagerSingleton() throw() {} // PRQA S 2131

public:
	void addGuard();
	void removeGuard();

protected:
	SigPipeManagerSingleton();
private:
    SigPipeManagerSingleton(const SigPipeManagerSingleton &) ; // not implemented
    SigPipeManagerSingleton& operator=(const SigPipeManagerSingleton &) ; // not implemented
private:
	acs::ThreadSafe _mutex;
	std::map<pid_t, int> _guardCount;
	std::map<pid_t, struct sigaction> _oldHandlers;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SigPipeManagerSingleton) ;
};

_ACS_END_NAMESPACE

#endif // _SigPipeManagerSingleton_h_


