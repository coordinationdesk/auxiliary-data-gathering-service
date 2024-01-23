// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Generic FileAction Library $

	$Id$

	$Author$

	$Log$
	Revision 7.4  2016/10/17 15:19:02  lucio.pulvirenti
	S1PDGS-31826: work in progress.
	
	Revision 7.3  2016/10/12 16:43:05  francesco.avanzi
	S2PDGS-1556: Work In Progress
	
	Revision 7.2  2016/04/29 16:03:17  marpas
	ids are of dbPersistent::IdType type - to be overall consistent
	coding best practices applied
	
	Revision 7.1  2015/06/17 10:51:50  lucio.pulvirenti
	S2PDGS-1234: willBeRestoredOnSynchSM IF added to check if product can be restored on synchronous SM with no errors. Common code with
	restoreOnSynchSM method put into new private canBeRestoredOnSynchSM method.
	
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.7  2015/01/09 16:12:51  lucio.pulvirenti
	S2PDGS-1010: restoreOnSynchSM public IF added.
	
	Revision 6.6  2015/01/08 17:17:46  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.5  2014/12/18 18:21:08  lucio.pulvirenti
	S2PDGS-1010: work in progress.
	
	Revision 6.4  2014/12/17 10:12:15  lucio.pulvirenti
	S2PDGS-1010: canGetFromSynchSM IF added.
	
	Revision 6.3  2014/12/15 17:11:57  lucio.pulvirenti
	S2PDGS-1010 work in progress.
	
	Revision 6.2  2014/12/11 11:59:36  marpas
	follows interface changes on libDbData 5.14
	rewritten to improve readability
	
	Revision 6.1  2013/10/10 17:42:46  marpas
	dbUploadInventoryFile can be configured to upload blobs possibly conditioned by size
	cond space gives an example of configuration
	PDS_OLD_SCHEMA no longer supported.
	
	Revision 6.0  2013/09/19 15:40:45  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.1  2013/09/16 16:59:16  marpas
	getId() method implemented
	
	Revision 5.0  2013/07/23 17:07:22  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.5  2013/03/28 14:30:53  marpas
	coding best practices applied
	ConnectionPool and connections usage enforced
	dbQueries statistics updated
	qa rules
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/11/20 09:04:04  marpas
	define PDS_OLD_SCHEMA to use old schema features
	
	Revision 1.2  2012/02/14 14:40:48  marpas
	possession guard removed
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	
	
*/


#ifndef _dbInventoryObjectOperatorIF_H_
#define _dbInventoryObjectOperatorIF_H_ 

#include <dbException.h>
#include <string>
#include <dbPersistentOperatorIF.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnection ;
class SQLString;
class dbInventoryObject;

//
//
//
// class dbInventoryObjectOperatorIF
//
//
//
class dbInventoryObjectOperatorIF : public dbPersistentOperatorIF // PRQA S 2109
{
public:

	/*! class dbInventoryObjectOperatorException declaration */
	exDECLARE_EXCEPTION(dbInventoryObjectOperatorException,dbPersistentOperatorException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(NotMigrableException,dbInventoryObjectOperatorException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(TryAgainLaterToMigrateException,dbInventoryObjectOperatorException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(MigrationFailedException,dbInventoryObjectOperatorException) ; // PRQA S 2131, 2153, 2502
	exDECLARE_EXCEPTION(dbInteractionException,dbInventoryObjectOperatorException) ; // PRQA S 2131, 2153, 2502

	dbInventoryObjectOperatorIF();
	virtual ~dbInventoryObjectOperatorIF() throw() ;

	virtual dbPersistentOperatorIF *clone() const ; // PRQA S 2502

	virtual dbPersistent::IdType getId(const dbPersistent &) const ;
	virtual const std::string getFileName(const dbPersistent &) const ;
	virtual const std::string getFileVersion(const dbPersistent &) const ;
	virtual const std::string getFileType(const dbPersistent &) const ;

	virtual const std::string getGroup(const dbPersistent &, dbConnection &) const ;
	
	virtual bool canGetFromSynchSM(const dbPersistent &, dbConnection &) const ;
	
    
	virtual std::pair<bool, dbPersistent::IdType> restoreOnSynchSM (const dbPersistent&, dbConnection&) const;

/*! \brief check if possible to restore file on Synchronous SM 
	\param t_inventory record 
	\param db connection 
	\return false if already on syncronous SM, true if can restore (possibily restore already in progress)

    \throw  - dbInteractionException if cannot query candidate SM
            - NotMigrableException: file either occurs on no SM or cannot be downloaded (e.g. negative download cost)
			- TryAgainLaterException: error got on download attempts by all asynch SM and configured timeout not yet expired.
			- MigrationFailedException: download attemps by all asynch SM are in error and cannot be repeated (error_recovery_period = 0 )
*/
	virtual bool willBeRestoredOnSynchSM (const dbPersistent&, dbConnection&) const;
    
    virtual bool isOnAsyncSM(const dbPersistent&, dbConnection&) const ;  //still to be named
protected:	
	dbInventoryObjectOperatorIF(const dbInventoryObjectOperatorIF &) ;
	dbInventoryObjectOperatorIF &operator=(const dbInventoryObjectOperatorIF &) ;

#ifdef ACS_TEST
public:
#else
private:
#endif
	SQLString getSMsForDownloadQuery(const dbInventoryObject*, bool hasErrorRecoveryPeriod, dbConnection&) const;
	dbPersistent::IdType insertDownloadRequest(const dbInventoryObject*, dbPersistent::IdType smId, dbConnection&) const;
	std::pair<bool, dbPersistent::IdType> canBeRestoredOnSynchSM (const dbInventoryObject*, bool insertDownlRequest, dbConnection&) const;
	dbPersistent::IdType getRequestId(dbPersistent::IdType smId, dbPersistent::IdType invId, bool occurringOnSynchSM, dbConnection&) const;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryObjectOperatorIF) ;
	
private:
	static const int defaultSMErrorRecoveryPeriod ;
} ; 


_ACS_END_NAMESPACE


#endif // _dbInventoryObjectOperatorIF_H_
