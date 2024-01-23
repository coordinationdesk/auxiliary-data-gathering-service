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
	Revision 5.6  2015/05/08 09:33:02  davide.tiriticco
	unistd include added
	
	Revision 5.5  2014/09/18 15:18:51  marfav
	Doubling buffers in case of ERANGE errors
	Last buffer size is saved to avoid doubling loops on each call
	
	Revision 5.4  2014/02/07 18:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/10/23 08:11:03  marpas
	qa warnings
	
	Revision 5.2  2013/10/22 17:13:41  marpas
	getGrNam & getGrGid methods added
	
	Revision 5.1  2013/10/22 14:04:53  marpas
	introducing thread safe Password class
	

*/

#include <Password.h>
#include <Filterables.h>
#include <cstring>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)

std::ostream &operator << (std::ostream &os, const Password::passwd_w &p) 
{
    os << "pw_name: " << p.pw_name << " pw_passwd(len): " << p.pw_passwd.length() 
       << " pw_uid: " << p.pw_uid << " pw_gid: " << p.pw_gid ;
    return os ; 
}

acs::exostream &operator << (acs::exostream &os, const Password::passwd_w &p) 
{
    os << "pw_name: " << p.pw_name << " pw_passwd(len): " << p.pw_passwd.length() 
       << " pw_uid: " << p.pw_uid << " pw_gid: " << p.pw_gid ;
    return os ; 
}



std::ostream &operator << (std::ostream &os, const Password::group_w &g) 
{
    os << "gr_name: " << g.gr_name << " gr_passwd(len): " << g.gr_passwd.length() 
       << " gr_gid: " << g.gr_gid << " gr_mem(size) : " << g.gr_mem.size() ;
    return os ; 
}

acs::exostream &operator << (acs::exostream &os, const Password::group_w &g)  
{
    os << "gr_name: " << g.gr_name << " gr_passwd(len): " << g.gr_passwd.length() 
       << " gr_gid: " << g.gr_gid << " gr_mem(size) : " << g.gr_mem.size() ;
    return os ; 
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(Password)       


namespace {

    long getPwdBufSize() 
    {
        static long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX) ;
        return bufsize ;
    }
}
    

Password::passwd_w Password::getPwNam(const std::string &name) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "in: " << name);

    static long buflen = getPwdBufSize() ;
    Password::passwd_w p ;
    bool retry = true;
    
    while (retry)
    {
        char buffer[buflen] ; // NOSONAR - need to be on the stack - size changed over iterations
        struct passwd pwd ; 
        struct passwd *result = nullptr ;
        int res = getpwnam_r(name.c_str(), &pwd, buffer, buflen, &result) ; 

        if (ERANGE == res)
        {
             buflen = buflen * 2; 
        }
        else
        {
            retry = false;
            ACS_COND_THROW(res!=0,exSystemException("getpwnam_r error for user:\"" + name + "\"", res)) ;
            ACS_COND_THROW(0 == result,exSystemException("getpwnam_r not found for user:\"" + name + "\"")) ;
            p.pw_name = pwd.pw_name ;
            p.pw_passwd = pwd.pw_passwd ;
            p.pw_uid = pwd.pw_uid ;
            p.pw_gid = pwd.pw_gid ;
            p.pw_gecos = pwd.pw_gecos ;
            p.pw_dir = pwd.pw_dir ;
            p.pw_shell = pwd.pw_shell ;
        }
    }
    return p ;
}

Password::passwd_w Password::getPwUid(uid_t uid) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "in: " << uid);

    static long buflen = getPwdBufSize() ;
    Password::passwd_w p ;
    bool retry = true;

    while (retry)
    {
        char buffer[buflen] ; // NOSONAR - need to be on the stack - size changed over iterations
        struct passwd pwd ; 
        struct passwd *result = nullptr ;
        int res = getpwuid_r(uid, &pwd, buffer, buflen, &result) ; 

        if (ERANGE == res)
        {
            buflen = buflen * 2; 
        }
        else
        {
            retry = false;
            if (res || result == nullptr) {
                ACS_THROW(exSystemException(res) << "getpwnam_r error for uid: " << uid) ; 
            }
            p.pw_name = pwd.pw_name ;
            p.pw_passwd = pwd.pw_passwd ;
            p.pw_uid = pwd.pw_uid ;
            p.pw_gid = pwd.pw_gid ;
            p.pw_gecos = pwd.pw_gecos ;
            p.pw_dir = pwd.pw_dir ;
            p.pw_shell = pwd.pw_shell ;
        }
    }

    return p ;
}

namespace {

    long getGrpBufSize() 
    {
        static long bufsize = sysconf(_SC_GETGR_R_SIZE_MAX) ;
        return bufsize ;
    }
}

Password::group_w Password::getGrNam(const std::string &name) 
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "in: " << name);
    static long buflen = getGrpBufSize() ;
    Password::group_w g ;
    bool retry = true;

    while (retry)
    {
        char buffer[buflen] ; // NOSONAR - need to be on the stack - size changed over iterations
        struct group grp ; 
        struct group *result = nullptr ;
        int res = getgrnam_r(name.c_str(), &grp, buffer, buflen, &result) ; 
        if (ERANGE == res) {
            buflen = buflen*2; 
	    }
	    else
	    {
            retry = false;
            ACS_COND_THROW(res!=0,exSystemException("getgrnam_r error for group:\"" + name + "\"", res)) ;
            ACS_COND_THROW(0 == result,exSystemException("getgrnam_r not found for group:\"" + name + "\"")) ;
    
            g.gr_name = grp.gr_name ;
            g.gr_passwd = grp.gr_passwd ; 
            g.gr_gid = grp.gr_gid ;
            for (char ** m = grp.gr_mem; *m != nullptr ; ++m) {
                g.gr_mem.emplace_back(*m) ; 
	            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mem: " << g.gr_mem.back());
            }
	    }
    }    
    
    return g ;
}


Password::group_w Password::getGrGid(gid_t gid) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "in: " << gid);
    static long buflen = getGrpBufSize() ;
    Password::group_w g ;
    bool retry = true;

    while (retry)
    {
        char buffer[buflen] ; // NOSONAR - need to be on the stack - size changed over iterations
        struct group grp ; 
        struct group *result = nullptr ;
        int res = getgrgid_r(gid, &grp, buffer, buflen, &result) ; 

        if (ERANGE == res)
        {
            buflen = buflen * 2; 
        }
        else
        {
            retry = false;
            if (res || result == nullptr) {
                ACS_THROW(exSystemException(res) << "getgrgid_r error for gid: " << gid) ; 
            }
    
            g.gr_name = grp.gr_name ;
            g.gr_passwd = grp.gr_passwd ; 
            g.gr_gid = grp.gr_gid ;
            for (char ** m = grp.gr_mem; *m != nullptr ; ++m) {
                g.gr_mem.emplace_back(*m) ; 
        	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mem: " << g.gr_mem.back());
            }
        }

    }
    return g ;
}


_ACS_END_NESTED_NAMESPACE
