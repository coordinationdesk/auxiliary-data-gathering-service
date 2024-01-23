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
	
	Revision 1.4  2013/06/12 10:32:47  nicvac
	Fixed exception delete.
	
	Revision 1.3  2013/05/27 10:25:03  nicvac
	Test completed
	
	Revision 1.2  2013/05/24 18:31:28  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/ 

#include <UserNotifier.h>

#include <ConfigurationSingleton.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UserNotifier)



UserNotifier::UserNotifier():
    Singleton<UserNotifier>(),
	_userNotifications(),
	_closed(false),
	_queueMutex()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id) ;
}


bool userNotifier_isPerformed( UserNotificationSptr const& userNotificationSptr) {

	//isPerformed set even if Thead received a stop/lazystop
	if ( userNotificationSptr->isPerformed() ) {
		//Nominally will return immediatelly because not running
		userNotificationSptr->waitExitOrTimeout( 10000 ); // PRQA S 4400

		//Check exceptions on performed notification
		exception* ex = 0 ;
		if ( userNotificationSptr->runException(ex) && ex) {
			ACS_LOG_ERROR("Error occurred sending notification: " << *ex) ;
			delete ex;
		}
	}

    return userNotificationSptr->isPerformed();
}


void UserNotifier::sendAsynch(UserNotificationSptr const& userNotificationSptr) throw() { // PRQA S 4020

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "called") ;

	if (! userNotificationSptr->isServiceEnabled() ) {
		return;
	}

	if ( _closed ) {
		ACS_LOG_WARNING("Cannot queue Notifications anymore.") ;
	}

	try {
		Lock lock(_queueMutex);

		//Clean the queue: remove previously finished notifications
		_userNotifications.erase(
			remove_if( _userNotifications.begin(), _userNotifications.end(), userNotifier_isPerformed ),
			_userNotifications.end());

		//Retain reference to Notification and run it
		_userNotifications.push_back( userNotificationSptr );
		userNotificationSptr->start();

	} catch(exception& ex) {
		ACS_LOG_ERROR("Error sending notification: "<<ex) ;
	}
}

void UserNotifier::close() {
	_closed = true;
}

//STL helper: stop Notifications
struct userNotifier_quit {
	void operator()(UserNotificationSptr& e) const { e->stop(false); }
};

void UserNotifier::abort() {

	Lock lock(_queueMutex);

	ACS_LOG_INFO("UserNotifier::abort: Abort request received. Stopping all the User Notifications... ") ;

	for_each( _userNotifications.begin(), _userNotifications.end(), userNotifier_quit() );
}

bool UserNotifier::isCommitted() {

	Lock lock(_queueMutex);

	//Clean the queue: remove finished notifications
	_userNotifications.erase(
		remove_if( _userNotifications.begin(), _userNotifications.end(), userNotifier_isPerformed ),
		_userNotifications.end());

	return ( _closed && _userNotifications.empty() );
}

void UserNotifier::closeAndFlushAndJoin(bool abort) {

	ACS_LOG_INFO("Waiting for pending User Notifications... ") ;

	Lock lock(_queueMutex);

	this->close();

	if (abort) {
		this->abort();
	}

	while ( ! this->isCommitted() ) {
		_userNotifications.front()->waitExitOrTimeout(1000) ; // PRQA S 4400
		ACS_LOG_INFO("Still "<<_userNotifications.size()<<" Notifications pending ... ") ;
	}

	ACS_LOG_INFO("No Notifications pending.") ;
}


_ACS_END_NAMESPACE
