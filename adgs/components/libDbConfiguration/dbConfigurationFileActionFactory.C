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

	$Prod: A.C.S. db Configuration Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/11 14:16:32  marpas
	adopting libException 5.x standards
	
	Revision 2.6  2013/03/12 17:44:57  marpas
	dbConnectionPool used in file actions
	
	Revision 2.5  2013/01/23 17:23:09  marpas
	introducing dbConnectionPool for factories
	coding best practices
	interface rationalization
	qa rules
	
	Revision 2.4  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.3  2011/04/05 16:53:29  marpas
	Tree management work in progress
	
	Revision 2.2  2009/03/30 17:48:54  marpas
	robust implementation
	rules ...
	
	Revision 2.1  2008/10/21 15:43:06  danalt
	aligned newEraseAction interface
	
	Revision 2.0  2006/02/28 09:09:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/21 10:50:16  marpas
	file actions for dbConfiguration implemented
		

*/


#include <dbConfigurationFileActionFactory.h>
#include <dbConfigurationSpace.h>
#include <dbPersistentFactoryChain.h>
#include <dbDownloadConfigurationSpace.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

dbConfigurationFileActionFactory dbConfigurationFileActionFactory::_theFactory ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConfigurationFileActionFactory)

dbConfigurationFileActionFactory::dbConfigurationFileActionFactory() : dbFileActionFactory()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

dbConfigurationFileActionFactory::~dbConfigurationFileActionFactory() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    // PRQA L:L1
}


dbFileAction * dbConfigurationFileActionFactory::newEraseAction(dbConnectionPool&, dbPersistent&, int) 
{
	return 0 ;
}



dbFileAction * dbConfigurationFileActionFactory::newUploadAction(dbConnectionPool&, dbPersistent& invobj, const string & path) 
{
	return 0 ;
}

dbFileAction * dbConfigurationFileActionFactory::newDownloadAction(dbConnectionPool &pool, dbPersistent& invobj, const string & path, const string& clientName, bool downloadInvalidFile)  // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
	// try to check if it was a dbConfigurationSpace
	dbConfigurationSpace * c = dynamic_cast<dbConfigurationSpace*>(&invobj) ; // PRQA S 3081
	if(c) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "calling dbDownloadConfigurationSpace(...)") ;
		// return a ne istance filled
		return new dbDownloadConfigurationSpace(pool, *c, path);
	}	
	return 0;
}


string dbConfigurationFileActionFactory::capabilities() const
{
	return "ConfigurationSpace\n" ;
}

string dbConfigurationFileActionFactory::getInvSmTableName(dbPersistent const &) const
{
	return "" ;
}

_ACS_END_NAMESPACE
