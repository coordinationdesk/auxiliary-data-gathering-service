// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $
*/


#ifndef _dbLTAFileActionFactory_H_
#define _dbLTAFileActionFactory_H_ 

#include <dbFileActionFactory.h>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbLTAFileActionFactory : public dbFileActionFactory // PRQA S 2109
{	
public:
	dbLTAFileActionFactory() ;
	virtual ~dbLTAFileActionFactory() = default ;

	virtual dbFileAction *newEraseAction(dbConnectionPool &, dbPersistent &, int storageid=0);
	virtual dbFileAction *newUploadAction(dbConnectionPool &, dbPersistent &,const std::string & path );
	virtual dbFileAction *newDownloadAction(dbConnectionPool &, dbPersistent &,const std::string & path ="", const std::string& clientName="", bool downloadInvalidFile=false) ;
	virtual dbFileAction *newGiveUpAction(dbConnectionPool &, dbPersistent&,const std::string& clientName="") ;

	virtual dbFileAction *newIsOnCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) ;
	virtual dbFileAction *newPutOnCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) ;
	virtual dbFileAction *newReleaseCacheAction(dbConnectionPool &, dbPersistent&,int storageid=0) ;

	virtual std::string capabilities() const ;
	virtual std::string getInvSmTableName(dbPersistent const &) const ;
	
private:
	// declared but not implemented	
	dbLTAFileActionFactory(const dbLTAFileActionFactory &) = delete ;
	dbLTAFileActionFactory &operator=(const dbLTAFileActionFactory &)  = delete ;	
	
private:
	static dbLTAFileActionFactory _theFactory ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbLTAFileActionFactory) ;
} ;

_ACS_END_NAMESPACE
 
#endif /* _dbLTAFileActionFactory_H_ */

