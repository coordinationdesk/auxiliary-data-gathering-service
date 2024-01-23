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
	Revision 5.0  2013/06/13 18:24:45  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/

#ifndef _FacebookUserNotification_H_
#define _FacebookUserNotification_H_

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* <Insert Class Description> 
**/

class FacebookUserNotification  { // PRQA S 2109
public:

	/** FacebookUserNotification Exceptions */
	exDECLARE_EXCEPTION(exFacebookUserNotificationException, exException) ; // Base FacebookUserNotification Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exFacebookUserNotificationCriticalException, exFacebookUserNotificationException) ; // Critical Exception. // PRQA S 2153 


	/** Default Class constructor */
	FacebookUserNotification();
	/** Destructor */
	virtual ~FacebookUserNotification();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	FacebookUserNotification(const FacebookUserNotification & );
	/** Operator = */
	FacebookUserNotification &operator=(const FacebookUserNotification &);
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(FacebookUserNotification) ;

};

_ACS_END_NAMESPACE

#endif //_FacebookUserNotification_H_

