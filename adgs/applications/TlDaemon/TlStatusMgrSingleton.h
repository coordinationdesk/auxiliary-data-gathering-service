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
    Revision 2.5  2015/11/27 17:38:06  marpas
    coding best practice application in progress

    Revision 2.4  2013/11/19 17:50:24  marpas
    TaskTable has private copy ctor & operator=

    Revision 2.3  2013/03/11 16:33:41  marfav
    Compiler warning reduced

    Revision 2.2  2012/02/09 17:26:24  marpas
    refactoring in progress

    Revision 2.1  2006/06/12 11:05:40  marfav
    Code for NO_SLOTS support removed

    Revision 2.0  2006/02/28 17:02:07  paoscu
    Exception 2.1 I/F adopted

    Revision 1.11  2005/03/18 11:22:03  marfav
    Calval environment merged to standard one

    Revision 1.10  2005/03/18 10:27:01  marfav
    excmcf support added

    Revision 1.9  2004/12/23 18:34:26  marfav
    Multislot Environment now active
    WSConfig dump changed using WSConfigMessage class

    Revision 1.8  2004/12/03 17:24:47  marfav
    Header Fixed

    Revision 1.7  2004/05/12 08:19:53  marfav
    Preparing for multislot processing status environment

    Revision 1.6  2004/05/05 14:22:05  marfav
    Patch for SUSPENDED order state management added (CalVal environment only)

    Revision 1.5  2004/04/07 13:24:35  marfav
    Does not use ACE anymore
    Unsing exStream.h

    Revision 1.4  2003/05/07 15:06:27  marfav
    Improved memory allocation management and exception handling

    Revision 1.3  2003/04/18 12:59:33  marfav
    Using namespaces removed from .h

    Revision 1.2  2003/03/24 16:35:26  marfav
    Included JobOrderFactory support

    Revision 1.1  2003/03/11 18:25:50  marfav
    Patrol message handling added


*/


#ifndef  _TL_STATUS_MGR_SINGLETON_H_
#define _TL_STATUS_MGR_SINGLETON_H_

#include <Singleton.hpp>
#include <StatusMgrSingleton.h>


#include <Filterables.h>
#include <exStream.h>

#include <iostream>
#include <string>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)


// -------------------------------------------------------------------------------------
//                       CLASS DISPATCHER FUNCTOR
// -------------------------------------------------------------------------------------
template <typename status_type>
class disp_functor
{
public:
// -------------------------------------------------------------------------------------
//                       constructor
// -------------------------------------------------------------------------------------
    disp_functor () :
        _statusTag(),
        _statusMap()
    {}


// -------------------------------------------------------------------------------------
//                       constructor (string)
// -------------------------------------------------------------------------------------
    disp_functor (const std::string& statusTag, const std::map<status_type,std::string>& statusMap) :
        _statusTag(statusTag),
        _statusMap(statusMap)
    {
    }


// -------------------------------------------------------------------------------------
//                       copy constructor
// -------------------------------------------------------------------------------------
    disp_functor (const disp_functor& d) :
        _statusTag (d._statusTag),
        _statusMap (d._statusMap)
    {
    }


// -------------------------------------------------------------------------------------
//                       operator =
// -------------------------------------------------------------------------------------
    disp_functor& operator =  (const disp_functor& d)
    {
        if (this != &d)
        {
            _statusTag=d._statusTag;
            _statusMap = d._statusMap;
        }
    return *this;
    }
    ~disp_functor() throw() {}

// -------------------------------------------------------------------------------------
//                       operator ()
// -------------------------------------------------------------------------------------
    void operator () (status_type status, unsigned int slot)
    {
        std::ostringstream os;
        os << _statusTag;
        if (slot > 0) {
            os << "#" << slot;
        }
        ACS_LOG_EVENT(os.str(),_statusMap[status]);
    }

private:
    std::string _statusTag;
    std::map<status_type, std::string > _statusMap;

};   // CLASS DISPATCHER FUNCTOR end

//
//
// class TlStatusMgrSingleton
//
//

enum tl_status { TL_ERROR, TL_WARNING, TL_PROCESSING_ORDER, TL_CHECKING_QUEUE, TL_IDLE};

class TlStatusMgrSingleton : public StatusMgrSingleton<tl_status, disp_functor<tl_status> >, public pattern::Singleton < TlStatusMgrSingleton >
{
public:
    virtual ~TlStatusMgrSingleton() throw() {} // PRQA S 2131

protected:
    TlStatusMgrSingleton () : StatusMgrSingleton<tl_status, disp_functor<tl_status> >() {}
    virtual MgrType *buildMgr();

private:
    TlStatusMgrSingleton (const TlStatusMgrSingleton &) ;            // disabled: defined but not implemented
    TlStatusMgrSingleton& operator = (const TlStatusMgrSingleton &); // disabled: defined but not implemented

    friend TlStatusMgrSingleton* pattern::Singleton< TlStatusMgrSingleton >::instance();
} ;



//
//
// class ProcessorStatusMgrSingleton
//
//
//
enum processor_status { PR_ERROR, PR_WARNING, PR_SUSPENDED, PR_RUNNING, PR_IDLE};

#ifndef EXFE_SUSPENDED
#warning It is not defined a string for the Processor Status SUSPENDED - Overriding EXFE_SUSPENDED
#define EXFE_SUSPENDED     ("SUSPENDED")
#endif //EXFE_SUSPENDED


class ProcessorStatusMgrSingleton : public StatusMgrSingleton<processor_status, disp_functor<processor_status> >, public pattern::Singleton < ProcessorStatusMgrSingleton >
{
public:
    virtual ~ProcessorStatusMgrSingleton() throw() {} // PRQA S 2131

protected:
    ProcessorStatusMgrSingleton () : StatusMgrSingleton<processor_status, disp_functor<processor_status> >(){}
    virtual MgrType *buildMgr();

private:
    ProcessorStatusMgrSingleton (const ProcessorStatusMgrSingleton &) ;            // disabled: defined but not implemented
    ProcessorStatusMgrSingleton& operator = (const ProcessorStatusMgrSingleton &); // disabled: defined but not implemented

    friend ProcessorStatusMgrSingleton* pattern::Singleton< ProcessorStatusMgrSingleton >::instance();
} ;




//
//
// class DownloadStatusMgrSingleton
//
//
//

enum download_status {DL_ERROR, DL_BUSY, DL_IDLE};

class DownloadStatusMgrSingleton : public StatusMgrSingleton<download_status, disp_functor<download_status> >, public pattern::Singleton < DownloadStatusMgrSingleton >
{
public:
    virtual ~DownloadStatusMgrSingleton() throw() {} // PRQA S 2131

protected:
    DownloadStatusMgrSingleton () : StatusMgrSingleton<download_status, disp_functor<download_status> >(){}
    virtual MgrType *buildMgr();

private:
    DownloadStatusMgrSingleton (const DownloadStatusMgrSingleton &) ;            // disabled: defined but not implemented
    DownloadStatusMgrSingleton& operator = (const DownloadStatusMgrSingleton &); // disabled: defined but not implemented

    friend DownloadStatusMgrSingleton* pattern::Singleton< DownloadStatusMgrSingleton >::instance();
} ;


_ACS_END_NAMESPACE


#endif // _TL_STATUS_MGR_SINGLETON_H_

