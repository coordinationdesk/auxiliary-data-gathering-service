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

	$Prod: A.C.S. libUserNotification$

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/06/20 14:57:17  marpas
	qa warnings fixed
	
	Revision 5.1  2013/06/17 13:22:49  marpas
	finalized adoption of libException 5.x standards
	coding best practices applied
	qa rules applied
	
	Revision 5.0  2013/06/13 18:24:46  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.2  2013/05/24 18:31:28  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/

#ifndef _UserNotification_H_
#define _UserNotification_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <Thread.h>

#include <dbSet.h>
#include <SQLString.h>

#include <boost/shared_ptr.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Base class for User Notification 
**/

class UserNotification: public Thread  { // PRQA S 2109
public:

	/** UserNotification Exceptions */
	exDECLARE_EXCEPTION(exUserNotificationException, exException) ; // Base UserNotification Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exUserNotificationSQLException, exUserNotificationException) ; // exUserNotificationSQLException Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exUserNotificationCriticalException, exUserNotificationException) ; // Critical Exception. // PRQA S 2153


	/** Class constructor. rootNotificationConf: root of configuration space for 
	 * specialized Notification settings */
	explicit UserNotification(std::string const& rootNotificationConf, const std::string& dbKey = "");
	/** Destructor */
	virtual ~UserNotification() throw();

	/** true if run executed and performed */
	bool isPerformed() const throw() ;

	/** true if notification sent */
	bool isSent() const throw() ;

	/** true if the service has been enabled by configuration */
	bool isServiceEnabled() const throw() ;

	/** Set description of notification (Let the notification be identified in the log) */
	UserNotification& setDescription(std::string const& description);

    const std::string &getDescription() const throw() { return _description ; }

	const std::string &getDbKey() const throw() { return _dbKey; }

protected:

	/** Interface to execute queries using serialization (use one connection) **/
	void executeSQLStatement (const SQLString&, dbSet& ) const;

	/** Thread interface implementation */
	virtual void run();

	/** Setup myself (filling my private attributes) */
	virtual bool setUp()=0;
	/** Message Preparation (format, presentation, ...) */
	virtual bool prepare()=0;
	/** Message Sending */
    virtual bool send()=0; // PRQA S 2502
	/** Clean after notification */
	virtual void clean()=0;

private:  // default, copy constructor and operator= defined but not implemented
	/** Default Constructor */
	UserNotification();
	/** Copy Constructor */
	UserNotification(const UserNotification & );
	/** Operator = */
	UserNotification &operator=(const UserNotification &);

private:
	/** A description of notification (Let the notification be identified in the log) */
	std::string _description;

	/** The root in the conf space to access Notification configuration (notification enabled, ...) */
	std::string _rootNotificationConf;

	/** The service for this notification is enabled */
	bool _isServiceEnabled;

	/** run called and finished */
	bool _isPerformed;

	/** Notification has been sent correctly */
	bool _isSent;

private:
	/** Mutex to allow serialization on database connection use **/
	Mutex _dbAccessMutex;

	/** Database key to interact with the connection pool. Default is "" -> Default connection pool **/
	std::string _dbKey;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(UserNotification) ;



};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<UserNotification> UserNotificationSptr;
typedef boost::shared_ptr<const UserNotification> UserNotificationConstSptr;

_ACS_END_NAMESPACE

#endif //_UserNotification_H_

