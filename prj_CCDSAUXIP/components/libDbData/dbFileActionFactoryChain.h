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
	Revision 5.2  2013/10/31 11:25:36  marpas
	qa warning fixed
	
	Revision 5.1  2013/09/25 16:55:03  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.6  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.5  2013/01/25 11:39:33  marpas
	using pattern::Singleton from libException
	
	Revision 2.4  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.2  2011/04/05 16:52:58  marpas
	Tree management work in progress
	
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


#ifndef _dbFileActionFactoryChain_H_
#define _dbFileActionFactoryChain_H_ 

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <dbUploadFile.h>
#include <exException.h>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;



class dbFileActionFactory ;
class dbFileAction ;
class dbPersistent ;
class dbConnectionPool ;

class dbFileActionFactoryChain : public Singleton<dbFileActionFactoryChain > // PRQA S 2109, 2153
{
	friend dbFileActionFactoryChain* Singleton< dbFileActionFactoryChain >::instance(); // PRQA S 2107
public:	
	virtual ~dbFileActionFactoryChain() noexcept ;
	dbFileActionFactoryChain(const dbFileActionFactoryChain &) = delete ;
	dbFileActionFactoryChain &operator=(const dbFileActionFactoryChain &) = delete ;
	dbFileActionFactoryChain(dbFileActionFactoryChain &&) = delete ;
	dbFileActionFactoryChain &operator=(dbFileActionFactoryChain &&) = delete ;

	// registers a new factory
	void registerFactory(dbFileActionFactory *) ;
	// unregisters a new factory
	void unregisterFactory(dbFileActionFactory *) noexcept ;
	
	dbFileAction *newEraseAction(dbConnectionPool&, dbPersistent&, int storageid=0 );
	dbFileAction *newUploadAction(dbConnectionPool&, dbPersistent&,const std::string & path);
	dbFileAction *newDownloadAction(dbConnectionPool&, dbPersistent&,const std::string & path, const std::string& clientName="", bool downloadInvalidFile=false); // PRQA S 4412
	dbFileAction *newGiveUpAction(dbConnectionPool&, dbPersistent&,const std::string& clientName="") ; // PRQA S 4412

	dbFileAction *newIsOnCacheAction(dbConnectionPool &, dbPersistent&, int storageid=0, const std::string& clientName="") ;
	dbFileAction *newPutOnCacheAction(dbConnectionPool &, dbPersistent&, int storageid=0, const std::string& clientName="") ;
	dbFileAction *newReleaseCacheAction(dbConnectionPool &, dbPersistent&, int storageid=0, const std::string& clientName="") ;

	std::string capabilities() const;
	std::string getInvSmTableName(dbPersistent const &) const ;
	
protected:
	dbFileActionFactoryChain() ;


private:
	std::vector<dbFileActionFactory *> _factories;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbFileActionFactoryChain)
} ;


_ACS_END_NAMESPACE

#endif
