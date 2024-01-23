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
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/

#ifndef _UserNotificationV_H_
#define _UserNotificationV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class UserNotificationV { // PRQA S 2109
public:
	UserNotificationV() ;
	~UserNotificationV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
    UserNotificationV(const UserNotificationV &) ; // not implemented
    UserNotificationV& operator= (const UserNotificationV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif 


