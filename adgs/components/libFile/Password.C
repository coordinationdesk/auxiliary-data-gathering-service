// PRQA S 1050 EOF
/*

	Copyright 2013-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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

using namespace std ;

ostream &operator << (ostream &os, const Password::passwd_w &p) 
{
    os << "pw_name: " << p.pw_name << " pw_passwd(len): " << p.pw_passwd.length() 
       << " pw_uid: " << p.pw_uid << " pw_gid: " << p.pw_gid ;
    return os ; // PRQA S 4028
}

acs::exostream &operator << (acs::exostream &os, const Password::passwd_w &p) // PRQA S 4222
{
    os << "pw_name: " << p.pw_name << " pw_passwd(len): " << p.pw_passwd.length() 
       << " pw_uid: " << p.pw_uid << " pw_gid: " << p.pw_gid ;
    return os ; // PRQA S 4028
}



ostream &operator << (ostream &os, const Password::group_w &g) 
{
    os << "gr_name: " << g.gr_name << " gr_passwd(len): " << g.gr_passwd.length() 
       << " gr_gid: " << g.gr_gid << " gr_mem(size) : " << g.gr_mem.size() ;
    return os ; // PRQA S 4028
}

acs::exostream &operator << (acs::exostream &os, const Password::group_w &g)  // PRQA S 4222
{
    os << "gr_name: " << g.gr_name << " gr_passwd(len): " << g.gr_passwd.length() 
       << " gr_gid: " << g.gr_gid << " gr_mem(size) : " << g.gr_mem.size() ;
    return os ; // PRQA S 4028
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(Password)       

Password::passwd_w::passwd_w() :
    pw_name(),
    pw_passwd(),
    pw_uid(),
    pw_gid(),
    pw_gecos(),
    pw_dir(),
    pw_shell()
{
}

Password::passwd_w::~passwd_w() throw() {}

Password::passwd_w::passwd_w(const passwd_w &r) :
    pw_name(r.pw_name),
    pw_passwd(r.pw_passwd),
    pw_uid(r.pw_uid),
    pw_gid(r.pw_gid),
    pw_gecos(r.pw_gecos),
    pw_dir(r.pw_dir),
    pw_shell(r.pw_shell)
{
}

Password::passwd_w& Password::passwd_w::operator= (const passwd_w &r) 
{
    if (&r != this) {
        pw_name = r.pw_name ;
        pw_passwd = r.pw_passwd ;
        pw_uid = r.pw_uid ;
        pw_gid = r.pw_gid ;
        pw_gecos = r.pw_gecos ;
        pw_dir = r.pw_dir ;
        pw_shell = r.pw_shell ;
    }
    return *this ;
}


Password::group_w::group_w() :
    gr_name(),
    gr_passwd(),
    gr_gid(),
    gr_mem()
{
}

Password::group_w::~group_w() throw() {}

Password::group_w::group_w(const group_w &r) :
    gr_name(r.gr_name),
    gr_passwd(r.gr_passwd),
    gr_gid(r.gr_gid),
    gr_mem(r.gr_mem)
{
}

Password::group_w& Password::group_w::operator= (const group_w &r) 
{
    if (&r != this) {
        gr_name = r.gr_name ;
        gr_passwd = r.gr_passwd ;
        gr_gid = r.gr_gid ;
        gr_mem = r.gr_mem ;
    }
    return *this ;
}



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
        char buffer[buflen] ;
        struct passwd pwd ; // PRQA S 4102
        struct passwd *result = 0 ;
        int res = getpwnam_r(name.c_str(), &pwd, buffer, buflen, &result) ; // PRQA S 3000

        if (ERANGE == res)
        {
             buflen = buflen * 2; // PRQA S 4400
        }
        else
        {
            retry = false;
            ACS_COND_THROW(res!=0,exSystemException("getpwnam_r error for user:\"" + name + "\"", res)) ;// PRQA S 3081
            ACS_COND_THROW(0 == result,exSystemException("getpwnam_r not found for user:\"" + name + "\"")) ;// PRQA S 3081
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
        char buffer[buflen] ;
        struct passwd pwd ; // PRQA S 4102
        struct passwd *result = 0 ;
        int res = getpwuid_r(uid, &pwd, buffer, buflen, &result) ; // PRQA S 3000

        if (ERANGE == res)
        {
            buflen = buflen * 2; // PRQA S 4400
        }
        else
        {
            retry = false;
            if ((res != 0) || (0 == result)) {
                ostringstream os ; 
                if (res !=0) {
                    os << "getpwnam_r error for uid: " << uid ;
                    ACS_THROW(exSystemException(os.str(), res)) ; // PRQA S 3081
                }
                else {
                    os << "getpwnam_r not found for uid: " << uid ;
                    ACS_THROW(exSystemException(os.str())) ; // PRQA S 3081
                }
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
        char buffer[buflen] ;
        struct group grp ; // PRQA S 4102
        struct group *result = 0 ;
        int res = getgrnam_r(name.c_str(), &grp, buffer, buflen, &result) ; // PRQA S 3000
        if (ERANGE == res)
	{
            buflen = buflen*2; // PRQA S 4400
	}
	else
	{
            retry = false;
            ACS_COND_THROW(res!=0,exSystemException("getgrnam_r error for group:\"" + name + "\"", res)) ;// PRQA S 3081
            ACS_COND_THROW(0 == result,exSystemException("getgrnam_r not found for group:\"" + name + "\"")) ;// PRQA S 3081
    
            g.gr_name = grp.gr_name ;
            g.gr_passwd = grp.gr_passwd ; 
            g.gr_gid = grp.gr_gid ;
            for (char ** m = grp.gr_mem; *m != 0 ; ++m) {
                g.gr_mem.push_back(string(*m)) ; // PRQA S 3081
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
        char buffer[buflen] ;
        struct group grp ; // PRQA S 4102
        struct group *result = 0 ;
        int res = getgrgid_r(gid, &grp, buffer, buflen, &result) ; // PRQA S 3000

        if (ERANGE == res)
        {
            buflen = buflen * 2; // PRQA S 4400
        }
        else
        {
            retry = false;
            if ((res != 0) || (0 == result)) {
                ostringstream os ; 
                if (res !=0) {
                    os << "getgrgid_r error for gid: " << gid ;
                    ACS_THROW(exSystemException(os.str(), res)) ; // PRQA S 3081
                }
                else {
                    os << "getgrgid_r not found for gid: " << gid ;
                    ACS_THROW(exSystemException(os.str())) ; // PRQA S 3081
                }
            }
    
            g.gr_name = grp.gr_name ;
            g.gr_passwd = grp.gr_passwd ; 
            g.gr_gid = grp.gr_gid ;
            for (char ** m = grp.gr_mem; *m != 0 ; ++m) {
                g.gr_mem.push_back(string(*m)) ; // PRQA S 3081
        	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "mem: " << g.gr_mem.back());
            }
        }

    }
    return g ;
}


_ACS_END_NESTED_NAMESPACE
