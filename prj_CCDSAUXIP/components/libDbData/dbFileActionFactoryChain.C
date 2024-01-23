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
	Revision 5.1  2013/09/25 16:55:02  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.8  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.7  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.6  2013/01/25 11:39:33  marpas
	using pattern::Singleton from libException
	
	Revision 2.5  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.3  2011/04/05 16:52:57  marpas
	Tree management work in progress
	
	Revision 2.2  2011/03/01 15:53:10  marpas
	GCC 4.4.x support
	
	Revision 2.1  2008/10/09 17:12:48  ivafam
	dbEraseFile I/F changed
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/05/31 14:15:16  integrator
	file actions interfaces changed
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.2  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.1  2004/10/15 08:48:34  ivafam
	Added
	
	
*/


#include <dbFileActionFactoryChain.h>
#include <dbFileActionFactory.h>
#include <Filterables.h>

#if defined __linux__ 
  #if (__GNUC__ < 3) ||  (__GNUC__ == 4) 
#include <algorithm>
  #endif
#endif

#include <cstdlib>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbFileActionFactoryChain)

dbFileActionFactoryChain::dbFileActionFactoryChain() : 
    Singleton<dbFileActionFactoryChain >(),
    _factories()
{
}

dbFileActionFactoryChain::~dbFileActionFactoryChain() noexcept {}


void dbFileActionFactoryChain::registerFactory(dbFileActionFactory *f)
{
	_factories.insert(_factories.begin(),f) ;
}

void dbFileActionFactoryChain::unregisterFactory(dbFileActionFactory *f) noexcept 
{
	
	vector<dbFileActionFactory *>::iterator tbd ;
	while( (tbd = find(_factories.begin(), _factories.end(), f)) != _factories.end()) {
		_factories.erase(tbd) ;
    }
}

dbFileAction *dbFileActionFactoryChain::newEraseAction(dbConnectionPool &pool, dbPersistent& object, int storageid) // PRQA S 4020 
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *oc = _factories[i]->newEraseAction(pool, object,storageid) ;
		if (0 != oc) {
			return oc ;
        }
    }
	return 0 ;
}

dbFileAction *dbFileActionFactoryChain::newUploadAction(dbConnectionPool &pool, dbPersistent& object,const std::string & path)  // PRQA S 4020 
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *oc = _factories[i]->newUploadAction(pool, object,path) ;
		if (0 != oc) {
			return oc ;
        }
    }
	return 0 ;
}


dbFileAction *dbFileActionFactoryChain::newDownloadAction(dbConnectionPool &pool, dbPersistent& object,const std::string & path, const std::string& clientName, bool downloadInvalidFile)  // PRQA S 4020
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *oc = _factories[i]->newDownloadAction(pool, object, path, clientName, downloadInvalidFile);
		if (0 != oc) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setClientName(\"" << clientName << "\")") ;
            oc->setClientName(clientName) ;
			return oc ;
        }
    }
	return 0 ;
}


dbFileAction *dbFileActionFactoryChain::newGiveUpAction(dbConnectionPool &pool, dbPersistent& object,const std::string& clientName)  // PRQA S 4020
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *oc = _factories[i]->newGiveUpAction(pool, object);
		if (0 != oc) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setClientName(\"" << clientName << "\")") ;
            oc->setClientName(clientName) ;
			return oc ;
        }
    }
	return 0 ;
}


dbFileAction *dbFileActionFactoryChain::newIsOnCacheAction(dbConnectionPool &pool, dbPersistent& object, int storageid, const std::string& clientName)  // PRQA S 4020
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *ac = _factories[i]->newIsOnCacheAction(pool, object, storageid);
		if (0 != ac) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setClientName(\"" << clientName << "\")") ;
            ac->setClientName(clientName) ;
			return ac ;
        }
    }
	return 0 ;
}


dbFileAction *dbFileActionFactoryChain::newPutOnCacheAction(dbConnectionPool &pool, dbPersistent& object, int storageid, const std::string& clientName)  // PRQA S 4020
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *ac = _factories[i]->newPutOnCacheAction(pool, object, storageid);
		if (0 != ac) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setClientName(\"" << clientName << "\")") ;
            ac->setClientName(clientName) ;
			return ac ;
        }
    }
	return 0 ;
}


dbFileAction *dbFileActionFactoryChain::newReleaseCacheAction(dbConnectionPool &pool, dbPersistent& object, int storageid, const std::string& clientName)  // PRQA S 4020
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
	    dbFileAction *ac = _factories[i]->newReleaseCacheAction(pool, object, storageid);
		if (0 != ac) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "setClientName(\"" << clientName << "\")") ;
            ac->setClientName(clientName) ;
			return ac ;
        }
    }
	return 0 ;
}

string dbFileActionFactoryChain::capabilities() const 
{
	string cap ;
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
		cap += _factories[i]->capabilities() ;
    }
	return cap ;
}

string dbFileActionFactoryChain::getInvSmTableName(dbPersistent const &v) const  // PRQA S 4020 
{
    size_t fs = _factories.size() ;
	for (size_t i=0; i < fs; ++i) {
		string invSmTableName = _factories[i]->getInvSmTableName(v) ;
		if (not invSmTableName.empty()) {
            return invSmTableName ;
        }
    }
	return "" ;
}


_ACS_END_NAMESPACE
