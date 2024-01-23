// PRQA S 1050 EOF
/*

	Copyright 2021-, Exprivia SpA
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
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.8  2013/05/14 19:03:44  marpas
	simulation support added
	
	Revision 2.7  2013/04/17 17:36:10  marpas
	simulation mode added
	
	Revision 2.6  2013/03/26 00:33:15  marpas
	qa warnings removed
	
	Revision 2.5  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.4  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.3  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.2  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.1  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.3  2005/05/26 12:07:55  marpas
	pointer initialization fixed
	
	Revision 1.2  2005/01/04 10:25:26  ivafam
	Added dbFileActionScopeGuard
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.2  2004/11/12 11:29:09  paoscu
	dbFileAction interface robustness improved
	
	Revision 1.1  2004/10/07 09:40:56  ivafam
	dbFileAction added
	

*/


#include <dbFileAction.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbFileAction)

// constructor
dbFileAction::dbFileAction(dbConnectionPool &pool, dbPersistent &p) :
    _pool(pool),    // PRQA S 2528 2
	_persObj(p),
	_completed(),
    _simulationMode(),
    clientName_()
{
}



dbFileAction::~dbFileAction() ACS_NOEXCEPT
{
}

const dbPersistent &dbFileAction::getDbPersistent() const // PRQA S 4120
{
	return _persObj ;
}

dbPersistent &dbFileAction::getDbPersistent()  // PRQA S 4120
{
	return _persObj ; // PRQA S 4024
}


bool dbFileAction::getCompleted() const  // PRQA S 4120
{
	return _completed ; 
}


void dbFileAction::setCompleted(bool v)  // PRQA S 4121
{
	_completed = v  ;
}

dbConnectionPool &dbFileAction::getPool() const  // PRQA S 4120
{
    return _pool ; // PRQA S 4024
}


void dbFileAction::setSimulationMode(bool v) ACS_NOEXCEPT { _simulationMode = v ; } // PRQA S 4121
bool dbFileAction::isSimulated() const ACS_NOEXCEPT { return _simulationMode ; } // PRQA S 4120

void dbFileAction::setClientName(const std::string &c ) { clientName_ = c; }
const std::string &dbFileAction::getClientName() const ACS_NOEXCEPT { return clientName_ ; }
    

_ACS_END_NAMESPACE
