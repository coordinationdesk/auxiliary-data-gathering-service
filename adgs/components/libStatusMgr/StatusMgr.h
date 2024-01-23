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

    $Prod: A.C.S. Status Manager Library $

    $Id$

    $Author$

    $Log$
    Revision 2.10  2015/12/01 16:17:08  marpas
    optimization of parameter passed by value

    Revision 2.9  2015/11/27 17:37:31  marpas
    coding best practices application in progress

    Revision 2.8  2015/11/27 16:03:58  marpas
    coding best practices applyed

    Revision 2.7  2013/02/21 11:56:04  marfav
    Compiler warnings removed

    Revision 2.6  2012/02/13 17:12:27  marpas
    refactoring in progress

    Revision 2.5  2011/01/24 15:46:39  marpas
    Mutex.h does not longer exixts

    Revision 2.4  2011/01/24 13:01:03  marpas
    exDECLARE_EXCEPTION libException(3.8) is now inlined and can be used inside templates

    Revision 2.3  2010/09/13 16:21:57  marpas
    libException 3.4 interface adopted

    Revision 2.2  2009/03/11 18:43:51  marpas
    new exceptions copy ctor added

    Revision 2.1  2006/06/12 10:59:36  marfav
    Code for NO_SLOT support removed

    Revision 2.0  2006/02/28 10:06:27  marpas
    Exception 2.1 I/F adopted

    Revision 1.11  2004/12/23 18:33:18  marfav
    Multislot status activated

    Revision 1.10  2004/12/03 17:59:56  marfav
    Header fixed

    Revision 1.9  2004/05/11 15:40:07  marfav
    Prepared for multi-slot status environement

    Revision 1.8  2004/05/06 16:32:59  marfav
    Now StatusManagerGuard can be forced to not release the status at DTOR

    Revision 1.7  2004/04/06 15:41:06  marfav
    Does not use ACE anymore

    Revision 1.6  2003/07/10 14:06:57  marfav
    clone method added to exceptions

    Revision 1.5  2003/04/18 16:28:52  fracar
    align to new compiler version

    Revision 1.4  2003/04/18 13:06:18  marpas
    get rid of typename warning

    Revision 1.3  2003/04/18 12:54:39  marfav
    Using namespaces removed from .h

    Revision 1.2  2003/03/11 18:28:39  marfav
    Singleton interface added

    Revision 1.1.1.1  2003/03/05 15:12:59  marfav
    Import libStatusMgr


*/


#ifndef _STATUS_MGR_H
#define _STATUS_MGR_H

#include <exException.h>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

template <typename status_type, typename dispatcher> class StatusMgr // PRQA S 2109
{
public:
    template <typename _S, typename _D> class StatusMgrGuard // PRQA S 2109
    {
        public:
            StatusMgrGuard(StatusMgr<_S, _D>& theMgr, const _S& theStatus, unsigned int slot = 0) :
                _release(true),
                _mgr(theMgr), // PRQA S 2528 2
                _status(theStatus),
                _slot (slot)
            {
                _mgr.enterStatus(_status, _slot);
            }

            ~StatusMgrGuard() throw()
            {
                try {
                    if (_release) {
                        _mgr.exitStatus(_status, _slot);
                    }
                }
                catch(std::exception &x) {
                    ACS_LOG_NOTIFY_EX(x) ;
                }
            }

            void release (bool haveToRelease)
            {
                _release = haveToRelease;
            }
        private:
            StatusMgrGuard() ; // not implemented
            StatusMgrGuard(const StatusMgrGuard &) ; // not implemented
            StatusMgrGuard& operator= (const StatusMgrGuard &) ; // not implemented
        private:
            bool _release;
            StatusMgr<_S, _D>& _mgr;
            _S _status;
            unsigned int _slot;

    }; // class StatusMgrGuard


    typedef StatusMgrGuard<status_type, dispatcher> guard;


    /*! class StatusMgrException declaration */
    exDECLARE_EXCEPTION(StatusMgrException, acs::exException) ; // PRQA S 2131, 2502

    StatusMgr(const dispatcher &statusDispatcher, status_type initialStatus):
        _statusString(),
        _dispatcher (statusDispatcher),
        _initialStatus (initialStatus),
        _currentStatus(),
        _flags(),
        _mutex()
    {
    }


    ~StatusMgr() throw() {} ;
    void enterStatus (const status_type& status, unsigned int slot = 0)
    {
        Lock _guard(_mutex);
        checkInitialization(slot);
        {
            _flags[slot][status]++;
        }
        setStatus(slot);

    }

    void exitStatus  (const status_type& status, unsigned int slot = 0)
    {
        Lock _guard(_mutex);
        checkInitialization(slot);
        {
            if (_flags[slot][status] == 0)
            {
                std::ostringstream os;
                os << "Invalid call to exitStatus ( " << int(status) << " ) for slot [" << slot << "]. Never entered this status."; // PRQA S 3081
                ACS_THROW(StatusMgrException (os.str())); // PRQA S 3081
            }
            _flags[slot][status]--;
        }
        setStatus(slot);

    }

    void pulseStatus (const status_type& status, unsigned int slot = 0)
    {
        enterStatus (status, slot);
        exitStatus (status, slot);
    }


    void clearStatus (const status_type& status, unsigned int slot = 0)
    {
        Lock _guard(_mutex);
        checkInitialization(slot);
        {
            _flags[slot][status]=0;
        }
        setStatus(slot);

    }

    status_type getStatus (unsigned int slot = 0) // PRQA S 4020
    {
        Lock _guard(_mutex);
        if (_currentStatus.find(slot) != _currentStatus.end())
        {
            return _currentStatus[slot];
        }
        else
        {
            return _initialStatus;
        }
    }


private:

    static bool isPositive (const std::pair<const status_type, unsigned long>& A)
    { return A.second > 0; }

    void setStatus (unsigned int slot = 0)
    {
        Lock _guard(_mutex);
        typename std::map<status_type, unsigned long>::iterator p;
        status_type newStatus;
        {
            p = std::find_if (_flags[slot].begin(), _flags[slot].end(), isPositive);
            _currentStatus[slot] = newStatus = p->first;
        }
        _dispatcher (newStatus, slot);
    }

    void checkInitialization (unsigned int slot = 0)
    {
        Lock _guard(_mutex);
        if ( _flags.find(slot) == _flags.end() )
        {
            _flags[slot][_initialStatus]=1;
        }
    }

    StatusMgr () ; // not implemented
    StatusMgr (const StatusMgr&) ; // not implemented
    StatusMgr& operator = (const StatusMgr &); // not implemented

private:
    std::vector<std::string> _statusString;
    dispatcher _dispatcher;
    status_type _initialStatus;
    std::map <unsigned int, status_type> _currentStatus;
    // _flags[slotNumber][status] = count
    std::map<unsigned int, std::map<status_type,unsigned long> > _flags;
    acs::ThreadSafe _mutex;
}; //class StatusMgr


_ACS_END_NAMESPACE

#endif // _STATUS_MGR_H
