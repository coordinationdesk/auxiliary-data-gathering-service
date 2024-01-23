// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Application Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/07/05 12:29:55  marpas
	adopting new libException 5.x standards
	using acs::base::defaultLockPath() to define application lock path
	
	Revision 5.0  2013/06/06 08:37:35  marpas
	new ibException 5.0 nterfaces
	
	Revision 4.3  2013/02/07 09:11:46  marpas
	namespaces enforced
	some interfaces modification in singleton's classes
	
	Revision 4.2  2012/11/28 17:19:24  marpas
	qa rules and robustness
	
	Revision 4.1  2012/03/01 15:48:09  marpas
	using acs::base for default satic paths
	
	Revision 4.0  2011/10/18 12:43:58  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.2  2009/06/26 10:37:04  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2009/04/03 15:39:24  marpas
	some rules fixed
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/10/14 09:53:42  marpas
	method mode() added
	
	Revision 1.3  2005/10/13 19:37:04  marpas
	new ApplicationDebug modes implemented - tested in libCryosatApp
	
	Revision 1.2  2005/10/10 15:19:27  marpas
	SIGUSR1 handler instead of SIGUSR2
	
	Revision 1.1  2005/10/06 19:41:32  marpas
	ApplicationDebug concept introduced, installs a SIGUSR1 handler in order to change debug values at runtime
	

*/

#ifndef _ApplicationDebug_H_
#define _ApplicationDebug_H_

#include <acs_c++config.hpp>
#include <exRTDebuggable.h>
#include <Thread.h>
#include <pthread.h>

_ACS_BEGIN_NAMESPACE(acs)

// PRQA S 1703 L1
class ApplicationDebug // PRQA S 2109, 2153
{
public:
	explicit ApplicationDebug(const std::string &path = base::defaultDebugPath()) ;
	virtual ~ApplicationDebug() ACS_NOEXCEPT ;

	void setPath(const std::string &) ;
	void updateDebug() ;

private:
	void refreshDebug() ;
	
	// declared but not defined to prevent their use
	// ApplicationDebug() ;
	ApplicationDebug(const ApplicationDebug&) ;
	ApplicationDebug &operator=(const ApplicationDebug&) ;
	
private:
	std::string _path ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ApplicationDebug) ;
};

// PRQA L:L1

_ACS_END_NAMESPACE

#endif /* _ApplicationDebug_H_ */
