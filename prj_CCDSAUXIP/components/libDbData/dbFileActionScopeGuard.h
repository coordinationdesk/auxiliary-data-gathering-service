// PRQA S 1050 EOF 
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2015/05/28 09:12:49  marpas
	qa warning fixed
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.2  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.1  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/01/04 10:25:26  ivafam
	Added dbFileActionScopeGuard
	
	Revision 1.3  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.2  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.1  2003/02/13 13:53:55  marpas
	*** empty log message ***
	
	Revision 1.1  2003/02/07 17:46:38  marpas
	dbFileActionScopeGuard class added
	
	Revision 1.5  2003/02/03 18:58:17  marpas
	Exception hierarchy tree modified to hilight db exceptions
	
	Revision 1.4  2003/02/03 15:55:19  marpas
	getLastSerial method added
	
	Revision 1.3  2003/01/28 17:32:29  paoscu
	Interface modified
	
	Revision 1.2  2003/01/28 15:23:56  paoscu
	inheritance chain slightly modified
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/


#ifndef _dbFileActionScopeGuard_H_
#define _dbFileActionScopeGuard_H_ 

#include <acs_c++config.hpp>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class dbFileAction ;

typedef ACS_SMARTPTR<dbFileAction> dbFileActionScopeGuard ;

_ACS_END_NAMESPACE


#endif // _dbFileActionScopeGuard_H_
