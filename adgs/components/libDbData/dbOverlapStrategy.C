// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.3  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.2  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.1  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.3  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.2  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.1  2003/01/13 11:05:00  marpas
	dbOverlapStrategy class added
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#include <dbOverlapStrategy.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOverlapStrategy) 


//
// dbOverlapStrategy::dbOverlapStrategy() :
//
dbOverlapStrategy::dbOverlapStrategy(const dbGeneralQuery &query) :
	_query(query) // PRQA S 2528

{
}

//
// dbOverlapStrategy::~dbOverlapStrategy()
//
dbOverlapStrategy::~dbOverlapStrategy()
{
}

_ACS_END_NAMESPACE
