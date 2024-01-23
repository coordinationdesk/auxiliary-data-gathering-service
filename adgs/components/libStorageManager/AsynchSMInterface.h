// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.4  2016/04/28 10:48:28  marpas
	coding best practices applied
	I/F robustness improved
	
	Revision 6.3  2015/02/17 16:30:42  marfav
	S2PDGS-1063 asynch storage managers can log operations on the database
	
	Revision 6.2  2015/01/08 17:01:19  marfav
	Added skipLocalSMUpload interface
	
	Revision 6.1  2014/12/09 17:14:00  marfav
	S2PDGS-950 adding support to asynch sm interface defined queries on packages
	
	Revision 6.0  2013/12/11 11:53:49  marfav
	Issuing major release 6.0
	
	Revision 5.2  2013/12/04 11:02:37  marfav
	S1PDGS-30219: addedd suport for getStatus request file generation
	
	Revision 5.1  2013/12/02 16:33:06  marfav
	S1PDGS-30219: Implementation started
	
	Revision 5.0  2013/06/18 14:40:01  marpas
	adoption of libException 5.x standards
	
	Revision 2.8  2013/03/14 15:31:27  marpas
	interface is symmetric with SMInterface
	dbConnectionPool introduced
	
	Revision 2.7  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.6  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.5  2008/12/04 11:24:40  ivafam
	StoreKey and GetKey method added
	
	Revision 2.4  2008/11/11 18:13:30  ivafam
	ScopeGard Removed
	
	Revision 2.3  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.2  2008/11/11 11:14:09  ivafam
	work in progess
	
	Revision 2.1  2008/11/11 11:00:54  ivafam
	Added
	

*/


#ifndef _AsynchSMInterface_H_
#define _AsynchSMInterface_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class SQLString ;

/**
 * Abstract class to deal with different Storage managers.
 */
class AsynchSMInterface // PRQA S 2109
{
public:

    enum operationStatus
    {
        INPROGRESS,
        COMPLETED,
        ERROR
    };

public:
	
	/*! class AsynchSMInterfaceException declaration 
	 *
	 * This exception is thrown by start() if there was an error
	 * while saving record in database.
	 */
	exDECLARE_EXCEPTION(AsynchSMInterfaceException,exException) ;   // PRQA S 2131, 2502
	
	/** smid is the primary key into the t_storagemanager,
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 */
	AsynchSMInterface(const std::string &smtype, size_t smid, dbConnectionPool &) ;
	virtual ~AsynchSMInterface() throw() ;

	/**
	 *
	 * sminvid is the primary key into the t_inventorysm and into the t_storagemanager.invrelationtable
	 * path is where to put the file.
	 */
	virtual void download(const std::string &path, const std::string& keyfile, const std::string& answerfile) = 0 ;
	
	virtual void upload(const std::string &path, std::string& keyfile, const std::string& answerfile) = 0 ;
	
	virtual void erase(const std::string& keyfile, const std::string& answerfile) = 0 ;

    /**
     *  This function, by default, returns an exception
     *  any derived class that implements an asynchronous interaction must re-implement it 
     */

    // requestFile format
    // <Request>
    //   <Type></Type> can be UPLOAD DOWNLOAD or ERASE
    //   <AsynchIFParams>
    //   Dump of the AsynchIFParams field of the smqueue
    //   </AsynchIFParams>
    //   <ExtParams>
    //   Dump of the ExtParam field of the smqueue
    //   </ExtParams>
    // </Request>

    static void buildStatusRequestFile (const std::string& type, const std::string& ifparams, const std::string& extparams, const std::string& outfile);

    virtual operationStatus getStatus (const std::string& requestfile, const std::string& keyfile);
	
	const std::string &SMType() const ;
	size_t SMId() const ;
	
	virtual bool isUploadSynchronous() const = 0 ;
	virtual bool isDownloadSynchronous() const = 0 ;
	virtual bool isEraseSynchronous() const = 0 ;
	virtual void readkey(const std::string &keyfile) = 0 ;
	virtual void writekey(const std::string &keyfile) = 0 ;
	virtual AsynchSMInterface* clone() = 0 ;  // PRQA S 2502

    // S2PDGS-306 adding the possibility to modify the SQL string of the 
    // query used to retrieve packages from t_smqueue
    virtual void customizePackageRetrieveQuery (SQLString& sqls, const std::string& opreration) {} // PRQA S 2131

    // S2PDGS-976 adding the possibility to notify the migration manager framework
    // to skip the local sm upload phase that is implemented directly by the AsyncSMInterface
    virtual bool skipLocalSMUpload () const { return false; } // PRQA S 2131

    // S2PDGS-1063 enable the support of operation logging on the t_asynchsm_operation table
    virtual bool logAsyncSMOperations () const { return false; } // PRQA S 2131


    static std::string uploadOperationSignature() { return "UPLOAD"; }
    static std::string downloadOperationSignature() { return "DOWNLOAD"; }
    static std::string eraseOperationSignature() { return "ERASE"; }

    // verify download constraints 
    // this function will emit an exception if the constraints are not met
    // by default no costraint has to be verified
    virtual void verifySMDownloadConstraints() {} // PRQA S 2131

protected :
    dbConnectionPool &getPool() ;
	
private:
	// declared but not implemented to prevent their use
	AsynchSMInterface() ;
	AsynchSMInterface(const AsynchSMInterface &) ;
	AsynchSMInterface &operator=(const AsynchSMInterface &) ;

private:
	std::string const _smType ;
	size_t _storageManagerId ;
    dbConnectionPool &_connectionPool ;
	
	
} ; 

_ACS_END_NAMESPACE


#endif // _AsynchSMInterface_H_
