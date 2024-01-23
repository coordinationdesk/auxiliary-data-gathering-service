// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/03/26 21:45:50  marpas
	new database related I/F
	use of dbConnection is enforced as reference, not pointer
	moreover, dbConnection can be obtained only through a dbConnectionPool
	dbConnectionManager and dbConnectionGuard classes are not longer available
	
	Revision 5.2  2016/03/21 18:28:37  marpas
	dbConnection is obtained only through a Pool
	
	Revision 5.1  2014/02/07 18:38:59  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.10  2013/03/26 00:33:15  marpas
	qa warnings removed
	
	Revision 2.9  2013/03/11 12:58:11  marpas
	FileGuard is a template specialization now
	
	Revision 2.8  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.7  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.6  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.4  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.3  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2008/06/04 16:03:20  enrcar
	EC:: Debug improved
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2006/01/11 06:30:01  marpas
	DB's plugin initialization BEFORE trying to use DB dialects
	
	Revision 1.5  2005/11/16 09:33:52  enrcar
	debug improved
	
	Revision 1.4  2005/11/09 11:21:30  danalt
	C read used instaed of C++ stream.
	
	Revision 1.3  2005/11/03 09:54:19  marpas
	dbOperation01Factory class added and used
	
	Revision 1.2  2005/11/02 17:56:05  marpas
	almost completed
	
	Revision 1.1  2005/10/28 17:36:02  marpas
	dbOperation01 class added
	

*/


#include <dbOperation01.h>
#include <dbConnectionPool.h>
#include <dbApplicationStatus.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <FileGuard.h>
#include <Filterables.h>
#include <ftmpstream.h>



#include <fcntl.h>
#include <sys/types.h> // PRQA S 1013
#include <sys/stat.h> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbOperation01)

// constructor
dbOperation01::dbOperation01(const string &feature, 
							 const string &instance) :
	Operation01(feature,instance)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "feature: " << feature << " instance: " << instance) ;
	ACS_CLASS_END_DEBUG

}


// copy constructor
dbOperation01::dbOperation01(const dbOperation01 &rhs) :
	Operation01(rhs) 
{
}	


dbOperation01::~dbOperation01() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
    // PRQA L:L1
}


// operator =
dbOperation01 &dbOperation01::operator=(const dbOperation01 &rhs)
{
	if(this != &rhs)
	{
		Operation01::operator=(rhs) ;
	}
	return *this ;
}




size_t dbOperation01::get(void *b, size_t size, string &whichmod, const vector<string> &mods) const // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "inbuffer size is " << size 
        << " mods vector size is: " << mods.size()) ;
	
    dbConnectionPool &pool = db::ConnPools::instance()->getPool("") ; // PRQA S 4412
    dbConnectionWrapper pw(pool) ;
    dbConnection &connection = pw ; // PRQA S 3050
	
	dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", connection.getKey())) ; // PRQA S 3081, 4412
	dbPersistentScopeGuard statusGuard(statusRecord) ;
	ACS_COND_THROW(!statusRecord, exIllegalValueException("dbApplicationStatus dynamic cast failed")); // PRQA S 3081

	ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "trying feature: " << getFeature()) ;
	ACS_CLASS_END_DEBUG
	

	string mod ;
	for (size_t i=0; i < mods.size(); i++) {
		statusRecord->setApplication(getFeature() + mods[i]);
		statusRecord->setInstance(getInstance());
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "trying instance: " << mods[i]) ;
		ACS_CLASS_END_DEBUG
		if(statusRecord->load(connection)) {
			mod = mods[i] ;
			break ;
		}
	} 
	
	if (statusRecord->isFromDatabase()) { // got it 
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "got " << statusRecord->getApplication() << " " << statusRecord->getInstance()) ; 
		ACS_CLASS_END_DEBUG
		// download it 
        string statusFilePath = File::tempname("/tmp/dbOp01", mod) ; // PRQA S 3050
 		FileGuard statusFileGuard(statusFilePath) ;
		statusRecord->saveStatusFile(statusFilePath, "", connection); 
		
		int fileD = open (statusFilePath.c_str() , O_RDONLY) ;
		if (fileD < 0) { return 0 ; }
        size_t extracted = read (fileD , b , size ) ; // PRQA S 3000
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "mod is: !" << mod << "! size is " << extracted) ;
		ACS_CLASS_END_DEBUG
		close(fileD) ;


		whichmod = mod ;
		return extracted ;
	}
	
	ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "not found") ;
	ACS_CLASS_END_DEBUG
	
	
	return 0 ;
}


void dbOperation01::set(const void *b, size_t size, const string &whichmod) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	
    dbConnectionPool &pool = db::ConnPools::instance()->getPool("") ; // PRQA S 4412
    dbConnectionWrapper pw(pool) ;
    dbConnection &connection = pw ; // PRQA S 3050
	dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", connection.getKey())) ; // PRQA S 3081, 4412
	dbPersistentScopeGuard statusGuard(statusRecord) ;
	ACS_COND_THROW(!statusRecord, exIllegalValueException("dbApplicationStatus dynamic cast failed")); // PRQA S 3081

	statusRecord->setApplication(getFeature() + whichmod);
	statusRecord->setInstance(getInstance());
	ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, statusRecord->getApplication() << " " <<  statusRecord->getInstance()) ;
	ACS_CLASS_END_DEBUG
    string tmpStatusPath = File::tempname("/tmp/dbOp01", whichmod) ; // PRQA S 3050
 	FileGuard statusFileGuard(tmpStatusPath) ;

	ofstream statusFile(tmpStatusPath.c_str());
	ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
		ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "tmp file: " << tmpStatusPath << " size: " << size ) ;
	ACS_CLASS_END_DEBUG
	statusFile.write(reinterpret_cast<const char *>(b),size) ; // PRQA S 3081, 3000
	if (!statusFile.good()) {
		ostringstream os ;
		os << "dbOperation::set writing " << size  << " bytes." ;
		exIOException x(os.str(),errno) ; // errno needed before calling close
		statusFile.close();
		ACS_THROW(x) ;
	}
	statusFile.close();
	if(statusRecord->load(connection)) {
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "is present") ;
		ACS_CLASS_END_DEBUG
		statusRecord->replaceStatusFile(statusFileGuard, connection) ; // PRQA S 3050
	}
	else {
		ACS_CLASS_EQ_DEBUG(171279) // PRQA S 3050
			ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "is not present file is " << tmpStatusPath) ;
		ACS_CLASS_END_DEBUG
		statusRecord->setStatusUploadFileName(statusFileGuard); // PRQA S 3050
		statusRecord->save(connection);
	}
	
}


bool dbOperation01::remove(const string &mod) const // PRQA S 4020
{
    dbConnectionPool &pool = db::ConnPools::instance()->getPool("") ; // PRQA S 4412
    dbConnectionWrapper pw(pool) ;
    dbConnection &connection = pw ; // PRQA S 3050
	dbApplicationStatus *statusRecord=dynamic_cast<dbApplicationStatus*>(dbPersistentFactoryChain::instance()->newObject("dbApplicationStatus", connection.getKey())) ; // PRQA S 3081, 4412
	dbPersistentScopeGuard statusGuard(statusRecord) ;
	ACS_COND_THROW(!statusRecord, exIllegalValueException("dbApplicationStatus dynamic cast failed")); // PRQA S 3081

	statusRecord->setApplication(getFeature() + mod);
	statusRecord->setInstance(getInstance());
	try {
		statusRecord->remove(connection) ;
	}
	catch(exception &) {
		return false ;
	}
	
	return true ;
}

_ACS_END_NAMESPACE
