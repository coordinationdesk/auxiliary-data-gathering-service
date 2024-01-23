// PRQA S 1050 EOF
/*

    Copyright 1995-2015, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ThinLayer Daemon $

    $Id$

    $Author$

    $Log$
    Revision 2.1  2015/11/27 16:03:58  marpas
    coding best practices applied

    Revision 2.0  2006/02/28 10:06:27  marpas
    Exception 2.1 I/F adopted

    Revision 1.7  2004/12/03 17:59:56  marfav
    Header fixed

    Revision 1.6  2004/03/15 14:31:50  marfav
    singleton relese at exit conditioned by the compilation flag RELEASE_SINGLETON_AT_EXIT

    Revision 1.5  2003/05/06 13:02:42  marfav
    Some private attributes are now protected

    Revision 1.4  2003/04/18 16:28:52  fracar
    align to new compiler version

    Revision 1.3  2003/04/18 13:06:18  marpas
    get rid of typename warning

    Revision 1.2  2003/04/18 12:54:39  marfav
    Using namespaces removed from .h

    Revision 1.1  2003/03/11 18:28:39  marfav
    Singleton interface added


*/


#ifndef  _STATUS_MGR_SINGLETON_H_
#define _STATUS_MGR_SINGLETON_H_

#include <StatusMgr.h>


_ACS_BEGIN_NAMESPACE(acs)

//
//
// class StatusMgrSingleton
//
//

// This class is no longer a Singleton. Derived classes must be Singleton
template <typename status_type, typename dispatcher> class StatusMgrSingleton // PRQA S 2109
{
public:
    typedef acs::StatusMgr<status_type, dispatcher> MgrType;
    typedef typename MgrType::guard StatusGuard;

    virtual ~StatusMgrSingleton() {}

    MgrType *getMgr();

protected:
    StatusMgrSingleton() : _mgr(0) {}
    virtual MgrType *buildMgr() = 0;

private:
    StatusMgrSingleton (const StatusMgrSingleton &) ; // disabled: defined but not implemented
    StatusMgrSingleton& operator = (const StatusMgrSingleton &); // disabled: defined but not implemented
    MgrType	*_mgr;
} ;

// -------------------------------------------------------------------------------------
//                       getStatusMgr
// -------------------------------------------------------------------------------------
template <typename status_type, typename dispatcher> typename StatusMgrSingleton<status_type, dispatcher>::MgrType *
StatusMgrSingleton<status_type, dispatcher>::getMgr()
{
    if(_mgr == 0)
    {
        _mgr = buildMgr();
    }

    return _mgr ;
}

_ACS_END_NAMESPACE

#endif // _STATUS_MGR_SINGLETON_H_

