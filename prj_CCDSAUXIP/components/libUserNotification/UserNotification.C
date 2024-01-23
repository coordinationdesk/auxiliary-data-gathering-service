// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libUserNotification$

	$Id$

	$Author$

	$Log$
	Revision 5.4  2016/12/07 09:53:24  marfav
	Adding dbKey in the ctor parameters
	Using a serialization point to execute SQL statements to avoid multiple connections
	
	Revision 5.3  2016/09/30 15:01:07  marpas
	avoiding exceptions from ctor in case of no 'enabled' configured for the specific _rootNotificationConf
	
	Revision 5.2  2013/11/12 18:37:50  nicvac
	S2PDGS-521: do not log detailed exception info (like stacktrace).
	
	Revision 5.1  2013/06/17 13:22:48  marpas
	finalized adoption of libException 5.x standards
	coding best practices applied
	qa rules applied
	
	Revision 5.0  2013/06/13 18:24:46  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.2  2013/05/24 18:31:27  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/ 

#include <UserNotification.h>
#include "UserNotificationV.h"

#include <ConfigurationSingleton.h>

#include <dbQuery.h>
#include <dbSet.h>
#include <dbConnectionPool.h>
#include <Application.h>

#include <Filterables.h>

using namespace std;
using namespace acs;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UserNotification)

namespace { // unnamed 
    UserNotificationV version;
}

UserNotification::UserNotification(string const& rootNotificationConf, string const& dbKey):
    Thread(),
	_description(),
	_rootNotificationConf(rootNotificationConf),
	_isServiceEnabled(true),
	_isPerformed(false),
	_isSent(false),
	_dbAccessMutex(),
	_dbKey (dbKey)
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Id: "<< Id) ;

    try {
	    rsResourceSet& conf = ConfigurationSingleton::instance()->get();
    
	    conf.getValue( _rootNotificationConf+".enabled", _isServiceEnabled );
    }
    catch(exception &x) {
        ;
    }

	//Log service status
	static bool logOnce = true ;
	if ( logOnce ) {
		ACS_LOG_PRIVINFO(_rootNotificationConf<<" notification service is "<<((_isServiceEnabled)?"enabled":"disabled")) ; // PRQA S 3380
		logOnce = false;
	}
}

UserNotification::~UserNotification() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}

UserNotification& UserNotification::setDescription(std::string const& description) {
	_description = description;
	return *this;
}

bool UserNotification::isPerformed() const throw() { // PRQA S 4120
	return _isPerformed;
}

bool UserNotification::isSent() const throw() {// PRQA S 4120
	return _isSent;
}

bool UserNotification::isServiceEnabled() const throw() {// PRQA S 4120
	return _isServiceEnabled;
}

//Scope support class to let UserNotifier the run ends
class UserNotificationScope { // PRQA S 2109
public:
	explicit UserNotificationScope(bool& isPerformed): _isPerformed(isPerformed) {} // PRQA S 2528 2
    UserNotificationScope(const UserNotificationScope& r) : _isPerformed(r._isPerformed) {} 
	~UserNotificationScope() { 
		_isPerformed = true;
	}
private:
    UserNotificationScope() ; // not implemented
    UserNotificationScope & operator=(const UserNotificationScope&) ; // not implemented
private:
	bool & _isPerformed;
};

void UserNotification::run() {

	//Let know if run ends
    UserNotificationScope userNotificationScope( _isPerformed );

	try {
		if ( this->setUp() && this->prepare()  ) {

			_isSent = this->send();
		}

		this->clean();

	} catch(exception& ex) {

		ACS_LOG_WARNING("Got problem sending notification ["<<_description<<"]: "<<ex.what()) ;
	}
}


void UserNotification::executeSQLStatement (const SQLString& theSqlString, dbSet& set) const
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB);

	// Max one connection per process will be used by the user notification framework
	ThreadSafe::Lock dbLock (_dbAccessMutex);

	// Get a db connection and execute the SQL string
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called.") ;

	try 
	{

		dbConnectionWrapper pw( db::ConnPools::instance()->getPool(_dbKey) );
		dbConnection& conn = pw; // PRQA S 3050

		//Execute the given statement 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "executing query: " <<theSqlString.getSQL()) ;
	
		dbQuery query( conn, theSqlString,  "UserNotification::executeSQLStatement" );
		query.execForSet( set );

	} catch(exception& ex) {
		ACS_THROW( exUserNotificationSQLException(ex, "Error executing SQL statement") );
	}
}

