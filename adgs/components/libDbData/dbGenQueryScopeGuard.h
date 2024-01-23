// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.3  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.2  2013/02/20 17:08:27  marpas
	qa warning
	
	Revision 2.1  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.3  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.2  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.1  2003/02/13 13:53:55  marpas
	*** empty log message ***
	
		

*/


#ifndef _dbGenQueryScopeGuard_H_
#define _dbGenQueryScopeGuard_H_ 

#include <acs_c++config.hpp>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class dbGeneralQuery ;
typedef ACS_SMARTPTR<dbGeneralQuery> dbGenQueryScopeGuard ;

_ACS_END_NAMESPACE


#endif // _dbGenQueryScopeGuard_H_
