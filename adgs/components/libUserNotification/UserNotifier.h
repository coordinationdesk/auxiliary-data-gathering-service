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
	Revision 5.0  2013/06/13 18:24:46  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.2  2013/05/24 18:31:28  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/

#ifndef _UserNotifier_H_
#define _UserNotifier_H_

#include <UserNotification.h>

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>

#include <deque>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* The singleton ASYNCH service accessed by clients to notification to the users
**/

class UserNotifier : public Singleton <UserNotifier> { // PRQA S 2109, 2153
	friend UserNotifier* Singleton<UserNotifier>::instance(); //SINGLETON // PRQA S 2107
public:

	/** UserNotifier Exceptions */
	exDECLARE_EXCEPTION(exUserNotifierException, exException) ; // Base UserNotifier Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exUserNotifierCriticalException, exUserNotifierException) ; // Critical Exception. // PRQA S 2153

	virtual ~UserNotifier() throw() {} // PRQA S 2131

	/** ASYNCH Call. Send the notification. Never throws. Errors are just logged */
	void sendAsynch(UserNotificationSptr const& userNotificationSptr) throw();

	/** Do not accept notification to send anymore.
	 *  Running notifications continue their work */
	void close(); // PRQA S 2502

	/** Do not accept notification to send anymore.
	 *  Running notifications are cancelled */
	void abort(); // PRQA S 2502

	/** True if closed and all requests have been processed */
	bool isCommitted();

	/** Close the notifier, wait for all pending notifications, return when notifications flushed 
	    abort: send abort to running notifications */
	void closeAndFlushAndJoin(bool abort);

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	UserNotifier();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	UserNotifier(const UserNotifier & );
	/** Operator = */
	UserNotifier &operator=(const UserNotifier &);

private:
	/** Current running notifications */
	std::deque<UserNotificationSptr> _userNotifications;

	/** Do not accept requests anymore */
	bool _closed;

	/** Lock on queue */
	ThreadSafe _queueMutex;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(UserNotifier) ;

};

_ACS_END_NAMESPACE

#endif //_UserNotifier_H_

