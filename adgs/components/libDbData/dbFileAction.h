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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.9  2013/04/17 17:36:10  marpas
	simulation mode added
	
	Revision 2.8  2013/03/28 13:49:15  marpas
	coding best practices improved
	statistics improved to allow integrators to check wueries efficiency
	Connection and ConnectionPool usage enforced
	
	Revision 2.7  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.6  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.5  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2008/11/11 16:09:12  marpas
	all scope guards were rewritten in terms of auto_ptr
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.2  2005/01/04 10:25:26  ivafam
	Added dbFileActionScopeGuard
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.2  2004/11/12 11:29:09  paoscu
	dbFileAction interface robustness improved
	
	Revision 1.1  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	
*/


#ifndef _dbFileAction_H_
#define _dbFileAction_H_ 

#include <dbException.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnectionPool ;

//
//
//
// class dbFileAction
//
//
//
class dbFileAction // PRQA S 2109
{
public:


	/*! class Exception declaration */
	exDECLARE_EXCEPTION(Exception,dbException) ; // PRQA S 2131, 2153, 2502

	/*! class PreconditionException declaration 
	 *
	 * This exception is thrown by uploading() if there's an error
	 * in the dbInventoryObject.
	 */
	exDECLARE_EXCEPTION(PreconditionException,Exception) ; // PRQA S 2131, 2153, 2502

	dbFileAction(dbConnectionPool &, dbPersistent &);
	virtual ~dbFileAction() ACS_NOEXCEPT ;

    dbFileAction() = delete ;
	dbFileAction(const dbFileAction &)  = delete ;
	dbFileAction &operator=(const dbFileAction &)  = delete ;
	dbFileAction(dbFileAction &&)  = delete ;
	dbFileAction &operator=(dbFileAction &&)  = delete ;

	virtual void start() = 0 ;
	virtual bool isComplete() const = 0;
    void setSimulationMode(bool) ACS_NOEXCEPT ;
    bool isSimulated() const ACS_NOEXCEPT ;

    void setClientName(const std::string &) ;
    const std::string &getClientName() const ACS_NOEXCEPT ;
    
protected:
    dbConnectionPool &getPool() const ;
	const dbPersistent &getDbPersistent() const ;	
	dbPersistent &getDbPersistent() ;
	bool getCompleted() const ;
	void setCompleted(bool) ;	
	
private:
	dbConnectionPool &_pool ;
    dbPersistent &_persObj ;
	bool _completed ;
    bool _simulationMode ;
    std::string clientName_ ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbFileAction)
} ; 


_ACS_END_NAMESPACE

#endif // _dbFileAction_H_
