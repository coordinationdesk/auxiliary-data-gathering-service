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

	$Prod: A.C.S. FileGuard Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 2.8  2013/03/11 13:04:54  marpas
	FDGuard is from libException (Guard.hpp)
	FileGuard completely rewritten
	interface rationalization, test compilation fiexed
	
	Revision 2.7  2013/02/27 12:14:31  marpas
	ose() is now public
	
	Revision 2.6  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.5  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.4  2009/03/13 15:26:45  marpas
	some rule violations fixed
	
	Revision 2.3  2008/10/24 10:13:44  marpas
	FileGuard class improved
	FDGuard class added (guard on file descriptors)
	both have detach method now
	
	Revision 2.2  2008/06/20 09:48:09  manuel.maccaroni
	Added assignment operator as const function.
	
	Revision 2.1  2008/06/18 12:53:20  manuel.maccaroni
	added copy constructor, assignment operator overloading function
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/11/30 17:01:03  marpas
	const std::string &str() const  method added
	
	Revision 1.4  2005/11/02 15:20:32  marpas
	str method added
	
	Revision 1.3  2005/09/27 09:25:55  marpas
	insertion operators added
	
	Revision 1.2  2005/09/09 08:44:23  paoscu
	Cast to char* operator introduced.
	
	Revision 1.1  2005/09/06 08:43:27  marpas
	new FileGuard class added
	
		

*/

#ifndef _FileGuard_H_
#define _FileGuard_H_

#include <acs_c++config.hpp>
#include <Guard.hpp>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

int unlinkPath(std::string) ;

typedef class pattern::AutoGuard<std::string, pattern::PathTraits< &unlinkPath > > FileGuard ; // PRQA S 2502

_ACS_END_NAMESPACE
 

#endif /* _CThread_H_ */

