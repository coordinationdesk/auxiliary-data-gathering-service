// PRQA S 1050 EOF
/*

	Copyright 1995-2020, Advanced Computer Systems , Inc.
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
	
	Revision 2.4  2012/12/11 17:30:29  marpas
	qa rules
	
	Revision 2.3  2012/02/13 16:53:02  marpas
	refactoring
	
	Revision 2.2  2011/04/05 16:53:29  marpas
	Tree management work in progress
	
	Revision 2.1  2008/10/21 15:43:06  danalt
	aligned newEraseAction interface
	
	Revision 2.0  2006/02/28 09:09:03  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/21 10:50:16  marpas
	file actions for dbConfiguration implemented
	
*/


#ifndef _dbConfigurationFileActionFactory_H_
#define _dbConfigurationFileActionFactory_H_ 

#include <dbFileActionFactory.h>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConfigurationFileActionFactory : public dbFileActionFactory // PRQA S 2109
{	
public:
	dbConfigurationFileActionFactory() ;
	virtual ~dbConfigurationFileActionFactory() throw() ;

	virtual dbFileAction *newEraseAction(dbConnectionPool &, dbPersistent &, int);
	virtual dbFileAction *newUploadAction(dbConnectionPool &, dbPersistent &,const std::string & path);
	virtual dbFileAction *newDownloadAction(dbConnectionPool &, dbPersistent &, const std::string & path ="", const std::string& clientName="", bool downloadInvalidFile=false) ;
	virtual std::string capabilities() const ;
	virtual std::string getInvSmTableName(dbPersistent const &) const ;
	
private:
	// declared but not implemented	
	dbConfigurationFileActionFactory(const dbConfigurationFileActionFactory &) ;
	dbConfigurationFileActionFactory &operator=(const dbConfigurationFileActionFactory &) ;	
	
private:
	static dbConfigurationFileActionFactory _theFactory ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConfigurationFileActionFactory) ;
} ;


_ACS_END_NAMESPACE

#endif /* _dbConfigurationFileActionFactory_H_ */

