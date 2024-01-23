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

	$Prod: A.C.S. Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.5  2013/03/14 15:27:23  marpas
	connection pool concept use enforced
	interface rationalization
	
	Revision 2.4  2013/01/24 19:22:39  marpas
	connection pool management improved
	
	Revision 2.3  2013/01/23 17:23:10  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.2  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.1  2008/12/16 14:51:37  marpas
	dbSystemMode implemented
	
*/


#include <dbSystemModeFactory.h>
#include <dbSystemMode.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbSystemModeFactory)

// constructor
dbSystemModeFactory::dbSystemModeFactory(dbConnectionPool &cp) :
    SystemModeFactory(),
    _connectionPool(cp) // PRQA S 2528
{
}


dbSystemModeFactory::~dbSystemModeFactory() throw() 
{
}


SystemMode *dbSystemModeFactory::createItem(const string &app, const string &sub)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	return new dbSystemMode(_connectionPool,app,sub) ;
}



_ACS_END_NAMESPACE
