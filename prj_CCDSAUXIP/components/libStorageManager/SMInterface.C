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

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 6.8  2016/08/02 09:09:24  marfav
	CSGACQ-94 managing LOBs with smart persistent
	
	Revision 6.7  2016/07/26 15:15:04  nicvac
	CSGACQ-86: Oracle porting. Clob handling.
	
	Revision 6.6  2016/05/10 14:45:46  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 6.5  2016/04/29 16:02:19  marpas
	ids are dbPersistent::IdType - to be overall consistent
	coding best practice applied
	
	Revision 6.4  2016/04/28 10:48:28  marpas
	coding best practices applied
	I/F robustness improved
	
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
	
	Revision 2.11  2013/03/28 14:02:23  marpas
	coding best practices applied
	dbConnectionPool concept forced
	qa warnings
	
	Revision 2.10  2013/03/12 17:45:35  marpas
	dbConnectionPool used in SM interfaces and factory methods
	
	Revision 2.9  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.8  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.7  2011/04/05 16:56:05  marpas
	Tree management work in progress
	
	Revision 2.6  2009/06/26 10:50:07  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.5  2009/01/29 17:19:50  ivafam
	I/F changed
	
	Revision 2.4  2008/12/04 11:24:40  ivafam
	StoreKey and GetKey method added
	
	Revision 2.3  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.2  2008/11/11 11:00:54  ivafam
	Added
	
	Revision 2.1  2008/06/20 10:19:52  marpas
	version file moved
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/09/26 12:43:25  marpas
	interface changed for synchronous/asynchronous operatios
	
	Revision 1.7  2005/09/26 12:33:18  marpas
	isSynchromous method (should be final)
	
	Revision 1.6  2005/09/26 11:29:25  marpas
	interface emthod name fixed
	
	Revision 1.5  2005/09/26 11:27:01  marpas
	interface method name changed to isSynchronousUpload
	
	Revision 1.4  2005/07/15 13:58:23  ivafam
	I/F changed
	
	Revision 1.3  2005/06/07 09:56:56  integrator
	split in progress
	
	Revision 1.2  2005/05/30 09:58:22  integrator
	Added dbPersistent
	
	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
	Importing libStorageManager
	

*/


#include <SMInterface.h>
#include <XMLIstream.h>
#include <dbFileActionFactoryChain.h>
#include <dbPersistentOperatorIF.h>
#include <SQLString.h>
#include <dbQuery.h>
#include <stormgrV.h>

#include <dbRow.h>
#include <dbSet.h>

#include <dbSmartPersistent.h>

#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    stormgrV version ;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(SMInterface)

using std::string ;

SMInterface::SMInterface(const string &smtype, dbPersistent::IdType smid, const dbPersistent* obj, dbConnectionPool &pool) : 
	Thread(smtype+"_SMInterface"),
	_smType(smtype),
    clientName_(),
	_storageManagerId(smid),
	_obj(obj),
    _connectionPool(pool),
	_path(),
	_oper(0)
{
}

SMInterface:: ~SMInterface() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB) ; // PRQA S 4631 2
	delete _oper ;
}



std::string SMInterface::getInvSmTableName() ACS_NOEXCEPT 
{
	string invSmTableName ;
    try { invSmTableName = dbFileActionFactoryChain::instance()->getInvSmTableName(*_obj);
	    ACS_CLASS_WRITE_DEBUG_TH(ACS_MID_VERB, "SM Inventory table name=\"" << invSmTableName << '\"');
	    ACS_COND_THROW( invSmTableName.empty(), SMInterfaceException( "dbFileActionFactoryChain::getInvSmTableName method returned empty string" ) );
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    return invSmTableName ;
}


const string & SMInterface::SMType() const // PRQA S 4120
{
	return _smType;
}

dbPersistent::IdType SMInterface::SMId() const    // PRQA S 4120
{
	return _storageManagerId;
}

const dbPersistent* SMInterface::getObject() const  // PRQA S 4120
{
	ACS_COND_THROW(_obj == 0, SMInterfaceException ("Object NULL pointer") ) ;
	return _obj;
}

void SMInterface::readkey(const string &keyfile)
{
	ACS_THROW(SMInterfaceException("SMInterface::readkey method not implemented")) ;
}

void SMInterface::writekey(const string &keyfile)
{
	ACS_THROW(SMInterfaceException("SMInterface::writekey method not implemented")) ;
}

void SMInterface::storekey(dbPersistent::IdType smid, 
                           dbPersistent::IdType packageid, 
                           const dbPersistentOperatorIF& oper, 
                           dbConnection &,string& keyPath)
{
	ACS_THROW(SMInterfaceException("SMInterface::storekey method not implemented")) ;
}

void SMInterface::getkey( dbPersistent::IdType packageid, string& keyPath,const dbPersistentOperatorIF &oper)
{
	ACS_THROW(SMInterfaceException("SMInterface::getkey method not implemented")) ;
}

bool SMInterface::hasTreeManagement() const 
{
	return false ;
}

using std::vector ;
using std::pair ;
void SMInterface::updateDbReferences(const vector< pair< string, pair <string, string > > > &, dbConnection &)  
{
	ACS_THROW(SMInterfaceException("SMInterface::updateDbReferences method not implemented")) ;
}


dbConnectionPool &SMInterface::getPool() { return _connectionPool ; } // PRQA S 4024, 4120


dbPersistent::IdType SMInterface::insertInvSM(dbConnection &conn, dbPersistent::IdType invid) 
{

	// Create the smart persistent item
	dbSmartPersistent item (getInvSmTableName(), conn);

	// Set all the fields
	item.set ("inv_id", invid, conn);
	item.set ("storagemanid", SMId(), conn);

	try
	{	
		return item.save (conn);
	}
	catch (std::exception& e)
	{
	   ACS_THROW(dbQuery::QueryExecuteException(e, "Cannot insert " + getInvSmTableName() + " record")) ;
	}
}


void SMInterface::deleteInvSM(dbConnection &conn, dbPersistent::IdType sminvid) 
{
	// Create the smart persistent item
	dbSmartPersistent item (getInvSmTableName(), conn);
	// Set all the fields
	item.set ("id", sminvid, conn);
	item.set ("storagemanid", SMId(), conn);
	try
	{	
		item.remove(conn);
	}
	catch (std::exception& e)
	{
	   ACS_THROW(dbQuery::QueryExecuteException(e) << "Cannot remove from " + getInvSmTableName() + " id: " << sminvid) ;
	}
}

void SMInterface::giveup(dbPersistent::IdType sminvid, const dbPersistentOperatorIF &opif)
{

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "sminvid: " << sminvid) ;
        if (0 != _obj) {
            ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, " filename: \"" << opif.getFileName(*_obj) << "\"") ;
        }
        ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, " clientName: \"" << clientName_ << "\"") ;
   ACS_CLASS_END_DEBUG 

}

void SMInterface::setClientName(const string &c) { clientName_ = c ; }
const string &SMInterface::getClientName() const ACS_NOEXCEPT { return clientName_ ; }


void SMInterface::getParameters(dbConnection &conn, const string &parampath, rsResourceSet &params, string &invrelation, math::StatNode & statN) const  // PRQA S 4214
{
    Timer timer ;
    timer.start() ;
    statN["getParameters"] += 1;
    const string &dbKey = conn.getKey() ;
    SQLString qString( dbKey, SQLString::SELECT ) ;
    string tableName = "t_storagemanager" ;
    string paramName = "invrelationtable" ;
    qString.addTableList(tableName);
    qString.addSelectList(paramName); 
    qString.setLimit(1) ;

    {
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, tableName, "Id", dbParam::EQUAL_TO, db::toSQL(dbKey, SMId()) )) ; 
        qString.addANDWhereParameter(pp) ;
    }

    std::ostringstream queryName ; queryName << "StorageManagerGetInvRel_" << SMId() ;
	dbQuery query(conn, qString, queryName.str()) ;	

	dbRow myRow(dbKey) ;
	try { 
		dbSet set;
		query.execForSet(set) ;
		ACS_COND_THROW(not set.getNextRow(myRow), SMInterfaceException( "No record found" )); // PRQA S 3081
	}
	catch(SMInterfaceException &x) {
		std::ostringstream os ;
		os << "cannot get clob parameters file for smid=" << SMId() << ": No " << tableName << " record found";
		ACS_THROW(exIOException(os.str())) ; // PRQA S 3081 
	}
	catch(std::exception &x) {
		std::ostringstream os ;
		os << "cannot get clob parameters file for smid:" << SMId() ;
		ACS_THROW(exIOException(x,os.str())) ; // PRQA S 3081 
	}

	myRow.column(0,invrelation);


    {
        SQLString queryString( dbKey, SQLString::LOBJ_DOWNLOAD ) ;
        queryString.setColumnAndFileName( "Parameters", parampath );
        queryString.addTableList( tableName );
        
        {
            dbQueryParameters p(dbKey) ;
            p.addParameter(dbParam(dbKey, tableName, "Id", dbParam::EQUAL_TO, db::toSQL(dbKey,SMId()) )) ; 
            queryString.addANDWhereParameter(p) ;
        }
	    
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading file: " << parampath) ;


        std::ostringstream qName ; qName << "StorageManagerDwnlParam_" << SMId() ;

		dbQuery qry(conn, queryString, qName.str()) ;	

        try
        {
            qry.downloadClob();
        }
        catch(std::exception &x)
        {
            std::ostringstream os ;
            os << "cannot get clob parameters file for smid:" << SMId() ;
            ACS_THROW(exIOException(x, os.str())); // PRQA S 3081
        }
    }
    
    
	// change permissions allowing other users to override this file later.
	File::chmod(parampath,0664) ; // PRQA S 4400

	XMLIstream ifile( parampath );
	ifile >> params ;
    timer.stop() ;
    statN["getParameters"]["elapsed [ms]"]+= double(timer.elapsed()) ; // PRQA S 3081
}



void SMInterface::assignOper(const dbPersistentOperatorIF &o) 
{
	delete _oper ;
	_oper=0 ;
	_oper=o.clone() ;
}



void SMInterface::normalUpload(const string &path, const dbPersistentOperatorIF& oper )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "path: \"" << path <<"\"") ;
	assignOper(oper) ;
	if(!running())
	{
		_path = path;
		Thread::start();	
	}
	else
	{
		ACS_THROW(dbPersistent::InvalidUse("Thread is already running.")) ; // PRQA S 3081
	}
}



_ACS_END_NAMESPACE

