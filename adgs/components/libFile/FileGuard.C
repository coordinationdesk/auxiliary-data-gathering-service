// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.12  2013/03/11 13:04:54  marpas
	FDGuard is from libException (Guard.hpp)
	FileGuard completely rewritten
	interface rationalization, test compilation fiexed
	
	Revision 2.11  2012/12/02 11:53:08  marpas
	CompressionMode insertion operators added
	
	Revision 2.10  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.9  2012/02/13 10:59:05  marpas
	refactoring in progress
	
	Revision 2.8  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.7  2009/07/14 08:41:07  matteo.airoldi
	unistd include added
	
	Revision 2.6  2009/05/14 13:48:15  enrcar
	EC:: c++ rules
	
	Revision 2.5  2009/03/13 15:26:45  marpas
	some rule violations fixed
	
	Revision 2.4  2008/10/24 10:13:44  marpas
	FileGuard class improved
	FDGuard class added (guard on file descriptors)
	both have detach method now
	
	Revision 2.3  2008/09/29 16:31:03  lucio.pulvirenti
	No change but warning: after File::unlink method can remove a directory tree,
	FileGuard can be invoked on a directory too
	
	Revision 2.2  2008/06/20 09:48:04  manuel.maccaroni
	Added assignment operator as const function.
	
	Revision 2.1  2008/06/18 12:53:24  manuel.maccaroni
	added copy constructor, assignment operator overloading function
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/11/30 17:01:03  marpas
	const std::string &str() const  method added
	
	Revision 1.5  2005/11/02 15:20:32  marpas
	str method added
	
	Revision 1.4  2005/09/27 09:25:55  marpas
	insertion operators added
	
	Revision 1.3  2005/09/26 15:23:42  marpas
	FileGuard dtor more robust
	
	Revision 1.2  2005/09/09 08:44:23  paoscu
	Cast to char* operator introduced.
	
	Revision 1.1  2005/09/06 08:43:27  marpas
	new FileGuard class added
	

*/

#include <File.h>
#include <FileGuard.h>
#include <exStream.h>
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

int unlinkPath(std::string s) // PRQA S 2010, 4020
{
	bool changePerm = true;  // directories with permissions r-x cannot be removed: change permissions 
    if (File::unlink(s, changePerm)) {
        return 0 ;
    }
    else {
        return 1 ;
    }
}


_ACS_END_NAMESPACE
