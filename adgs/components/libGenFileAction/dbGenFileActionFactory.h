// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 6.1  2013/09/25 16:58:18  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.5  2013/03/12 17:46:06  marpas
	dbConnectionPool used in any file action
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.2  2011/04/05 16:52:35  marpas
	Tree management work in progress
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
		

*/


#ifndef _dbGenFileActionFactory_H_
#define _dbGenFileActionFactory_H_ 

#include <dbFileActionFactory.h>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbGenFileActionFactory : public dbFileActionFactory // PRQA S 2109
{	
public:
	dbGenFileActionFactory() ;
	virtual ~dbGenFileActionFactory() throw() ;

	virtual dbFileAction *newEraseAction(dbConnectionPool &, dbPersistent &, int storageid=0);
	virtual dbFileAction *newUploadAction(dbConnectionPool &, dbPersistent &,const std::string & path);
	virtual dbFileAction *newDownloadAction(dbConnectionPool &, dbPersistent &,const std::string & path ="", const std::string& clientName="", bool downloadInvalidFile=false) ;
	virtual dbFileAction *newGiveUpAction(dbConnectionPool &, dbPersistent&,const std::string& clientName="") ;
	virtual std::string capabilities() const ;
	virtual std::string getInvSmTableName(dbPersistent const &) const ;
	
private:
	// declared but not implemented	
	dbGenFileActionFactory(const dbGenFileActionFactory &) ;
	dbGenFileActionFactory &operator=(const dbGenFileActionFactory &) ;	
	
private:
	static dbGenFileActionFactory _theFactory ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbGenFileActionFactory) ;
} ;

_ACS_END_NAMESPACE
 
#endif /* _dbGenFileActionFactory_H_ */

