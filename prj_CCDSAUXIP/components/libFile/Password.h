/*

	Copyright 2013-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/10/22 17:13:41  marpas
	getGrNam & getGrGid methods added
	
	Revision 5.1  2013/10/22 14:04:53  marpas
	introducing thread safe Password class
	

*/

#ifndef _os_Password_H_
#define _os_Password_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <string>
#include <sys/types.h> // PRQA S 1013

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)

/*!	\mainpage
	This library is intended to support password convenience functions.
*/

/*! \class Password
	
	Utility class to support network convenience functions.
*/
class Password                                  // PRQA S 2109
{
public:
    struct passwd_w {
        std::string pw_name = "" ;      /* username */
        std::string pw_passwd = "" ;    /* user password */
        uid_t   pw_uid = 0 ;            /* user ID */
        gid_t   pw_gid = 0 ;            /* group ID */
        std::string pw_gecos = "" ;     /* real name */
        std::string pw_dir = "" ;       /* home directory */
        std::string pw_shell = "" ;     /* shell program */
    } ;
       
    struct group_w {
        std::string gr_name = "" ;              /* group name */
        std::string gr_passwd = "" ;            /* group password */
        gid_t   gr_gid = 0 ;                    /* group ID */
        std::vector<std::string> gr_mem = {} ;  /* group members */
    } ;
    
    
	/*! This method returns passwd_w for user with the name
	*/
	static passwd_w getPwNam(const std::string &) ;
	/*! This method returns passwd_w for user with the uid
	*/
	static passwd_w getPwUid(uid_t) ;
    
	/*! This method returns group_w for group with the name
	*/
    static group_w getGrNam(const std::string &) ;

	/*! This method returns group_w for group with the name
	*/
    static group_w getGrGid(gid_t) ;
    
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Password) ;
} ;

std::ostream &operator << (std::ostream &, const Password::passwd_w &) ;
acs::exostream &operator << (acs::exostream &, const Password::passwd_w &) ;    // PRQA S 2072

std::ostream &operator << (std::ostream &, const Password::group_w &) ;
acs::exostream &operator << (acs::exostream &, const Password::group_w &) ;    // PRQA S 2072
_ACS_END_NESTED_NAMESPACE

#endif	/* _os_Password_H_ */
