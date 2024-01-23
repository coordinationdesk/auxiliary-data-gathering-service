// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. ISM SM client library $

	$Id$

	$Author$

	$Log$
	Revision 6.6  2017/11/09 09:33:00  lucio.pulvirenti
	S2PDGS-1843: work in progress.
	
	Revision 6.5  2017/09/29 09:49:59  lucio.pulvirenti
	S2PDGS-1843: getGroupFromConfSpace method added.
	
	Revision 6.4  2017/09/22 15:00:46  lucio.pulvirenti
	S2DPGS-1843: work in progress.
	
	Revision 6.3  2016/05/25 12:39:37  lucio.pulvirenti
	CSGACQ-79: assignOper and getParameters method now protected. Both getOper and getSimulationChecker protected methos added.
	
	Revision 6.2  2016/04/29 16:04:09  marpas
	ids are of dbPersistent::IdType - to be overall consistent
	coding best practices applied
	
	Revision 6.1  2016/04/12 12:21:59  marpas
	wrong exception inheritance fixed
	
	Revision 6.0  2013/09/20 08:03:34  marpas
	new t_inventorysm schema supported
	
	Revision 5.0  2013/06/20 13:37:44  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa rules
	
	Revision 1.11  2013/04/09 08:23:23  marpas
	new Statistics
	
	Revision 1.10  2013/04/03 13:48:58  marpas
	run does not longer gets a void*
	
	Revision 1.9  2013/04/03 12:51:48  marpas
	Thread::run gets a void*
	
	Revision 1.8  2013/03/29 15:12:12  marpas
	statistics
	
	Revision 1.7  2013/03/12 17:48:00  marpas
	dbCOnnectionPool is mandatory in SM interfaces and factory methods
	
	Revision 1.6  2012/05/25 15:48:30  marfav
	Addedd SMUnknownFileException management as per S1PDGS-3291
	
	Revision 1.5  2012/02/13 17:09:49  marpas
	refactoring in progress
	
	Revision 1.4  2011/04/05 16:51:46  marpas
	Tree management work in progress
	
	Revision 1.3  2009/06/26 10:43:59  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.2  2008/11/04 16:38:59  ivafam
	Erase Interface changed
	
	Revision 1.1.1.1  2008/06/16 15:14:58  marpas
	Importing libIsmSMClient
	

*/


#ifndef _ISMStorageManager_H_
#define _ISMStorageManager_H_ 

#include <SMInterface.h>
#include <dbPersistent.h>
#include <dbDownloadFile.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;
class dbInventoryObject ;
class rsResourceSet ;

_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

/**
 * Abstract class to deal with different Storage managers.
 */
class ISMStorageManager : public SMInterface // PRQA S 2109
{

public:
	
	class Simulate {
	public:
		Simulate() ;
		~Simulate() throw() ;
		Simulate(const Simulate &) {}
		Simulate& operator=(const Simulate &) { return *this ;} // PRQA S 4072
		bool simulate() const ;
	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL(Simulate) ;
	} ;

	enum Action {
		NONE,
		REMOVE_FILE ,
		GIVEUP_REQUEST
	};
	
	//
	// Exceptions
	//
	
	/**
	 * class ExternalRepositoryException declaration 
	 * This exception is internally used.
	 */

	/*! class ISMStorageManagerException declaration */
	exDECLARE_EXCEPTION(ISMStorageManagerException,ThreadException) ; // PRQA S 2131, 2153, 2502

	/*! class ISMUnknownFileException declaration */
	exDECLARE_EXCEPTION(ISMUnknownFileException, dbDownloadFile::SMUnknownFileException) ; // PRQA S 2131, 2153, 2502
	
	/** smid is the primary key into the t_storagemanager,
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 */
	ISMStorageManager(const std::string &smtype, dbPersistent::IdType smid,const dbPersistent* obj, dbConnectionPool &) ;
	virtual ~ISMStorageManager() throw() ;

	/**
	 *
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 * path is where to put the file.
	 */
	virtual void download(dbPersistent::IdType sminvid, const std::string &localpath, const dbPersistentOperatorIF&);
	
	virtual void upload(const std::string &path, const dbPersistentOperatorIF&);
	
	virtual void erase(dbPersistent::IdType sminvid, const dbPersistentOperatorIF&, std::string* p=0) ;
	
	/*! \brief Get size information on ISM caches separated per file class (i.e. PDS group). Each file class can occur more than once in output since can be associated to different caches
		\param v vector of pair elements where the first one is the file class associated to a given cache, and the other one is the corresponding free room in MB
	*/
	virtual void getAvailableStorageInfo(std::vector<std::pair<std::string, long long int> > &v) ;
	
	 /**
	 * \brief ask the storage manager release any pending order armed by previous download requests
	 * \param sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
     * \param The adapter to deal with any dbPersistent
	 */
	virtual void giveup(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &) ;


		
	virtual bool uploading();
	virtual bool isUploadSynchronous() const ;
	virtual bool isDownloadSynchronous() const ;
	virtual bool isEraseSynchronous() const ;
	virtual bool hasTreeManagement() const ;
	// <path, <fname, fversion> >
	virtual void updateDbReferences(const std::vector< std::pair< std::string, std::pair <std::string, std::string > > > &, dbConnection &)  ;
	
protected:
	void assignOper(const dbPersistentOperatorIF &) ;
	void getParameters(dbConnection &, const std::string &parampath, rsResourceSet &params, std::string &invrelation) const ;
	dbPersistentOperatorIF* getOper() const throw() { return _oper; }
	Simulate getSimulationChecker() const throw() { return _check; }
	
#if __cplusplus >= 201103L
    const std::string &getPath() const noexcept { return _path ; }
#else
    const std::string &getPath() const throw() { return _path ; }
#endif

		
private:
    static math::StatNode &statN() ;
	virtual void run();
	// declared but not implemented to prevent their use
	ISMStorageManager() ;
	ISMStorageManager(const ISMStorageManager &) ;
	ISMStorageManager &operator=(const ISMStorageManager &) ;
	
	std::string getGroupFromConfSpace(dbConnection &);
	
	virtual void doAction(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &, const Action &, std::string & ismpath);
	std::string getActionString(const Action & ) ;
private:
	std::string _path ;
	Simulate _check ;
#ifdef ACS_TEST
public:
#else
private:
#endif
	dbPersistentOperatorIF* _oper;
	std::vector< std::pair<std::string , std::string> > _references ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ISMStorageManager) ;
} ; 

_ACS_END_NAMESPACE


#endif // _ISMStorageManager_H_
