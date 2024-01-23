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

	$Prod: A.C.S. ApplicationLock Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2017/09/12 13:15:16  marpas
	added actualInstance method to return the actual instance number that provided a succesfull lock.
	
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
	
	Revision 4.1  2012/02/14 14:17:42  marpas
	refactoring
	
	Revision 4.0  2011/10/18 12:43:58  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/08 10:48:56  marpas
	uses new Statistics from libException 3.x
	
	Revision 2.1  2009/04/03 15:39:24  marpas
	some rules fixed
	
	Revision 2.0  2006/02/28 08:35:39  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2003/07/22 20:58:39  paoscu
	Enhanced management of locks.
	
	Revision 1.1  2003/06/26 10:13:31  paoscu
	lock implemented
	
	

*/

#ifndef _ApplicationLock_H_
#define _ApplicationLock_H_

#include <acs_c++config.hpp>
#include <Application.h>
#include <File.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * ApplicationLock class.
 * The class is a commodity to allow only a fixed number of application instances to be running
 * on the same host.
 */

class ApplicationLock // PRQA S 2109
{
public:
	explicit ApplicationLock(unsigned short maxInstances = 1,
					const std::string& lockName = Application::appName()) ACS_NOEXCEPT ;	
	~ApplicationLock() ACS_NOEXCEPT ;
	
	const std::string &lock(bool exception = true) ;
	void unlock() ;
	void writePid() ;
	unsigned short actualInstance() const ACS_NOEXCEPT { return actualInstance_ ; }
private:
	//Not implemented to prevent their use
	ApplicationLock(ApplicationLock const &);
	ApplicationLock & operator=(ApplicationLock const &);


private:
	
	std::string _lockName ;
	std::string _actualLockName ;
	unsigned short actualInstance_ ;
	unsigned short _maxInstances ;
	File::Lock _lock ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ApplicationLock) ;
};

_ACS_END_NAMESPACE

#endif /* _ApplicationLock_H_ */
