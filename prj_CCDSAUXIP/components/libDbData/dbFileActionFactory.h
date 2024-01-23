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
	
	Revision 2.2  2011/04/05 16:52:57  marpas
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


#ifndef _dbFileActionFactory_H_
#define _dbFileActionFactory_H_ 

#include <acs_c++config.hpp>
#include <dbUploadFile.h>
#include <exException.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbFileAction ;
class dbPersistent ;
class dbConnectionPool ;


class dbFileActionFactory // PRQA S 2109
{	
public:
	/*! class NotFoundException declaration 
	 *
	 */
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:
	dbFileActionFactory() ;
	virtual ~dbFileActionFactory() noexcept ;
	// declared but not implemented	
	dbFileActionFactory(const dbFileActionFactory &) = delete ;
	dbFileActionFactory &operator=(const dbFileActionFactory &) = delete ;
	dbFileActionFactory(dbFileActionFactory &&) = delete ;
	dbFileActionFactory &operator=(dbFileActionFactory &&) = delete ;

	virtual dbFileAction *newEraseAction(dbConnectionPool &, dbPersistent&,int storageid=0) = 0;
	virtual dbFileAction *newUploadAction(dbConnectionPool &, dbPersistent&,const std::string & path ) { return 0 ; }
	virtual dbFileAction *newDownloadAction(dbConnectionPool &, dbPersistent&,const std::string & path, const std::string& clientName="", bool downloadInvalidFile=false) = 0;
	virtual dbFileAction *newGiveUpAction(dbConnectionPool &, dbPersistent&,const std::string& clientName="") { return 0 ; }

	virtual dbFileAction *newIsOnCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) { return 0 ; }
	virtual dbFileAction *newPutOnCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) { return 0 ; }
	virtual dbFileAction *newReleaseCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) { return 0 ; }

	virtual std::string capabilities() const = 0 ;
	virtual std::string getInvSmTableName(dbPersistent const &) const = 0 ;
		
private:
	void registerToChain() ;
	void unregisterFromChain() noexcept ;
	
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbFileActionFactory)
} ;


_ACS_END_NAMESPACE

#endif /* _dbFileActionFactory_H_ */

