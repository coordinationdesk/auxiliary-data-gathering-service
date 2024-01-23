// PRQA S 1050 EOF
/*
 * 
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.9  2017/11/08 14:46:38  marpas
	qa warning fixed
	
	Revision 6.8  2017/11/07 17:51:01  lucio.pulvirenti
	S2PDGS-1843: getAvailableStorageInfo method added.
	
	Revision 6.7  2016/04/29 16:02:19  marpas
	ids are dbPersistent::IdType - to be overall consistent
	coding best practice applied
	
	Revision 6.6  2016/04/28 10:48:28  marpas
	coding best practices applied
	I/F robustness improved
	
	Revision 6.5  2016/04/12 12:18:48  marpas
	wrong exception inheritance fixed
	
	Revision 6.4  2014/12/11 12:04:27  marpas
	declaration simplified
	
	Revision 6.3  2014/11/27 16:45:17  marfav
	S2PDGS-992 completed
	
	Revision 6.2  2014/11/27 11:25:50  marfav
	Added support to clob on t_inventorysm - one per package at max
	
	Revision 6.1  2014/11/26 17:23:42  marfav
	S2PDGS-992 work in progress
	
	Revision 6.0  2013/12/11 11:53:49  marfav
	Issuing major release 6.0
	
	Revision 5.3  2013/09/20 14:13:06  marfav
	insertInvSM signature changed to allow default null package id
	
	Revision 5.2  2013/09/17 15:20:51  marpas
	deleteInvSM method introduced
	
	Revision 5.1  2013/09/16 17:03:19  marpas
	insertInvSM method implemented
	
	Revision 5.0  2013/06/18 14:40:01  marpas
	adoption of libException 5.x standards
	
	Revision 2.9  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.8  2013/03/12 17:45:35  marpas
	dbConnectionPool used in SM interfaces and factory methods
	
	Revision 2.7  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.6  2011/04/05 16:56:05  marpas
	Tree management work in progress
	
	Revision 2.5  2009/06/26 10:50:08  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.4  2009/01/29 17:19:38  ivafam
	I/F changed
	
	Revision 2.3  2008/12/04 11:24:40  ivafam
	StoreKey and GetKey method added
	
	Revision 2.2  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.1  2008/11/11 11:00:54  ivafam
	Added
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/09/27 15:58:24  marpas
	light changes to the main interfaces
	
	Revision 1.11  2005/09/26 12:43:25  marpas
	interface changed for synchronous/asynchronous operatios
	
	Revision 1.10  2005/09/26 12:33:18  marpas
	isSynchromous method (should be final)
	
	Revision 1.9  2005/09/26 11:29:25  marpas
	interface emthod name fixed
	
	Revision 1.8  2005/09/26 11:27:01  marpas
	interface method name changed to isSynchronousUpload
	
	Revision 1.7  2005/07/25 15:03:38  ivafam
	Interface changed. Added dbPersistentOperatorIF
	
	Revision 1.6  2005/07/15 13:58:23  ivafam
	I/F changed
	
	Revision 1.5  2005/06/07 09:56:56  integrator
	split in progress
	
	Revision 1.4  2005/05/30 10:24:31  ivafam
	Work in progress
	
	Revision 1.3  2005/05/30 09:58:22  integrator
	Added dbPersistent
	
	Revision 1.2  2005/05/23 17:25:08  ivafam
	*** empty log message ***
	
	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
	Importing libStorageManager
	

*/


#ifndef _SMInterface_H_
#define _SMInterface_H_ 

#include <acs_c++config.hpp>
#include <Thread.h>
#include <dbPersistent.h>
#include <dbFileAction.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbConnectionPool ;
class dbPersistent ;
class dbPersistentOperatorIF;


/**
 * Abstract class to deal with different Storage managers.
 * it is not thread safe !!!
 */
class SMInterface : protected Thread // PRQA S 2109, 2194
{
public:
	
	/*! class SMInterfaceException declaration 
	 *
	 * This exception is thrown by start() if there was an error
	 * while saving record in database.
	 */
	exDECLARE_EXCEPTION(SMInterfaceException,ThreadException) ; // PRQA S 2131, 2153, 2502
	
	/** smid is the primary key into the t_storagemanager,
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 */
	SMInterface(const std::string &smtype, dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &) ;
	virtual ~SMInterface() throw() ;

    std::string getInvSmTableName() noexcept ; 
	/**
	 * \brief asks the storage manager to download a file
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param path where to put the file.
     * \param opif the adapter to deal with any dbPersistent
	 *
	 */
	virtual void download(dbPersistent::IdType sminvid, const std::string &path, const dbPersistentOperatorIF &opif) = 0 ;
	
    /**
	 * \brief asks the storage manager release any pending order armed by previous download requests
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param opif the adapter to deal with any dbPersistent
	 * \note the default implementation does nothing - it is not foreseen the method should give some evidence to the caller
     * about the effectiveness of the request
	 */
	virtual void giveup(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif) ;
	
    /**
	 * \brief asks the storage manager to upload a file 
	 * \param path the file to be uploaded
     * \param opif the adapter to deal with any dbPersistent
	 */
	virtual void upload(const std::string &path,const dbPersistentOperatorIF &opif) = 0 ;
	
    /**
     * \brief asks the storage manager to erase a file 
     * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param fname the filename used for M&C messages
     * \param fvers the fileversion used for M&C messages
     * \param keyfile the possible key to reference what needs to be erased into the SM
     */
	virtual void erase(dbPersistent::IdType sminvid, const std::string &fname, const std::string &fvers, std::string* keyfile=0) = 0 ;
	
    /**
	 * \brief asks the storage manager to erase a file 
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param opif the adapter to deal with any dbPersistent
     * \param keyfile the possible key to reference what needs to be erased into the SM
	 */
	[[deprecated("Use erase(dbPersistent::IdType sminvid, const std::string &fname, const std::string &fvers, std::string* keyfile=0) instead.")]]
	virtual void erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif, std::string* keyfile=0) = 0 ;
	
    /**
	 * \brief asks the storage manager if an inventory is "onCache" 
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param opif the adapter to deal with any dbPersistent
     * \returns dbFileAction::ON_CACHE 
     *
     * \note when reimplemented will return dbFileAction::ON_CACHE if the inventory is actually on cache, dbFileAction::OFF_CACHE if it is not on cache 
     * and dbFileAction::NOT_AVAILABLE if the inventory is not known
	 */
	virtual dbFileAction::CacheStatus isOnCache(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif) { return dbFileAction::ON_CACHE ;}
	
    /**
	 * \brief asks the storage manager to bring an inventory on cache 
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param opif the adapter to deal with any dbPersistent
     * \returns dbFileAction::ON_CACHE 
     *
     * \note when reimplemented will return dbFileAction::ON_CACHE if the inventory is actually on cache, dbFileAction::IN_PROGRESS if it is moving toward the cache 
     * and dbFileAction::NOT_AVAILABLE if the inventory is not known
	 */
	virtual dbFileAction::CacheStatus putOnCache(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif) { return dbFileAction::ON_CACHE ;}
	
    /**
	 * \brief signals the storage manager that a specific file is no longer needed on cache. The SM can use the information to free cache space
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param opif the adapter to deal with any dbPersistent
     *
     * \note no operation neither exception if the file is not known.
	 */
	virtual void releaseCache(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif) {}
	
    /**
	 * \brief signals that if a specific file is UNKNOWN for the storage manager, the link between inventory and the archive handler can be removed (t_inventorysm record).
     * \returns true: the link can be removed 
	 */
	virtual bool shallRemoveUnknownLinks () { return true ; }
	
	/** 
	 *
	 * fills the given vector with the available space (in MB) 
     * for each label that would select the storage partition.
	 */
	virtual void getAvailableStorageInfo(std::vector<std::pair<std::string, long long int> > &) {} ; // PRQA S 2131
		
	const std::string &SMType() const ;
	dbPersistent::IdType SMId() const ;
	const dbPersistent* getObject() const ;

	virtual bool hasTreeManagement() const ;
	
	virtual bool isUploadSynchronous() const = 0 ;
    
    /** 
     * giveup is always synchronous 
     */
	bool isGiveupSynchronous() const { return true ; }
	virtual bool isDownloadSynchronous() const = 0 ;
	virtual bool isEraseSynchronous() const = 0 ;
	virtual bool uploading() = 0 ;
	virtual void readkey(const std::string &keyfile) ;
	virtual void writekey(const std::string &keyfile) ;
	virtual void storekey(dbPersistent::IdType smid, dbPersistent::IdType packageid, const dbPersistentOperatorIF& oper, dbConnection &,std::string& keyPath);
	virtual void getkey( dbPersistent::IdType packageid, std::string& keyPath,const dbPersistentOperatorIF &oper) ;
	virtual void updateDbReferences(const std::vector< std::pair< std::string, std::pair <std::string, std::string > > > &, dbConnection &)  ;

    dbPersistent::IdType insertInvSM(dbConnection &, dbPersistent::IdType invid) ; 
    void deleteInvSM(dbConnection &, dbPersistent::IdType sminvid ) ;

    void setClientName(const std::string &) ;
    const std::string & getClientName() const ACS_NOEXCEPT ;
protected :
	dbConnectionPool &getPool() ;
	void getParameters(dbConnection &, const std::string &parampath, rsResourceSet &params, std::string &invrelation, math::StatNode &) const ;
	void normalUpload(const std::string &path, const dbPersistentOperatorIF&);
	void assignOper(const dbPersistentOperatorIF &) ;

#if __cplusplus >= 201103L
    const std::string &getPath() const noexcept { return _path ; }
#else
    const std::string &getPath() const throw() { return _path ; }
#endif
	
	dbPersistentOperatorIF* getOper() const throw() { return _oper; }
	
private:
	// declared but not implemented to prevent their use
	SMInterface() ;
	SMInterface(const SMInterface &) ;
	SMInterface &operator=(const SMInterface &) ;

private:
	std::string const _smType ;
	std::string clientName_ ;
	dbPersistent::IdType _storageManagerId ;
	const dbPersistent* _obj ;
	dbConnectionPool &_connectionPool ;
	std::string _path ;

#ifdef ACS_TEST
public:
#else
private:
#endif
	dbPersistentOperatorIF* _oper;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SMInterface) ;
	
} ; 

_ACS_END_NAMESPACE


#endif // _SMInterface_H_
