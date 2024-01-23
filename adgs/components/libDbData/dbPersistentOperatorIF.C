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
	Revision 5.4  2016/04/29 15:59:57  marpas
	getting rid of deprecated methods
	
	Revision 5.3  2014/12/11 11:57:35  marpas
	enforcing dbConnection for indirect queries (e.g getGroup via file type)
	general robustness improved
	OLD schema no longer supported
	
	Revision 5.2  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/09/16 16:57:57  marpas
	getId() method implemented in support of serial id on t_inventory
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.7  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.6  2013/01/22 18:18:30  marpas
	interface improved
	
	Revision 2.5  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/11/28 13:11:51  marpas
	PDS_OLD_SCHEMA related interfaces removed if compiled with no flag
	
	Revision 2.3  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.2  2007/05/18 09:53:47  marpas
	getOnlineLatencyTime method added
	
	Revision 2.1  2007/04/10 16:39:51  ivafam
	Method get ProductType added
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/09/29 19:33:48  marpas
	interface slightly changed
	
	Revision 1.2  2005/09/27 15:57:39  marpas
	clone method added
	
	Revision 1.1  2005/07/22 09:44:22  ivafam
	Added
	

*/


#include <dbPersistentOperatorIF.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbPersistentOperatorIF)

// constructor
dbPersistentOperatorIF::dbPersistentOperatorIF() 
{
}



dbPersistentOperatorIF::~dbPersistentOperatorIF() throw()
{
}
dbPersistentOperatorIF::dbPersistentOperatorIF(const dbPersistentOperatorIF &) 
{
}


dbPersistentOperatorIF &dbPersistentOperatorIF::operator=(const dbPersistentOperatorIF &) // PRQA S 4072
{
	return *this ;
}

dbPersistent::IdType dbPersistentOperatorIF::getId(const dbPersistent &) const 
{
	ACS_THROW(dbPersistentOperatorException("Not implemented in base class")) ; // PRQA S 3081
}


const std::string dbPersistentOperatorIF::getFileName(const dbPersistent &p) const 
{
	ACS_THROW(dbPersistentOperatorException("Not implemented in base class")) ; // PRQA S 3081
}

const std::string dbPersistentOperatorIF::getFileVersion(const dbPersistent &p) const 
{
	ACS_THROW(dbPersistentOperatorException("Not implemented in base class")) ; // PRQA S 3081
}

const std::string dbPersistentOperatorIF::getGroup(const dbPersistent &p, dbConnection &) const 
{
	ACS_THROW(dbPersistentOperatorException("Not implemented in base class")) ; // PRQA S 3081
}

const std::string dbPersistentOperatorIF::getFileType(const dbPersistent &p) const 
{
	ACS_THROW(dbPersistentOperatorException("Not implemented in base class")) ; // PRQA S 3081
}



_ACS_END_NAMESPACE
