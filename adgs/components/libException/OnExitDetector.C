// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2014/03/26 11:17:54  marfav
	Using static local variable to implement the singleton
	
	Revision 5.2  2014/03/25 17:37:49  marfav
	Removing the singleton template and using un-guarded singleton implementation
	
	Revision 5.1  2014/03/25 14:49:28  marfav
	Completed RWMutex coding. OnExitDetector introduced
	
*/

#include <OnExitDetector.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace
{
    class OnExitDetectorCreator // PRQA S 2109
    {
    public:
        OnExitDetectorCreator () ACS_NOEXCEPT :
            _aBoolean (acs::OnExitDetector::instance()->exiting()) // this force the instantiation
        {}

        ~OnExitDetectorCreator() ACS_NOEXCEPT {}

    private:
        OnExitDetectorCreator (const OnExitDetectorCreator&); // not implemented
        const OnExitDetectorCreator& operator = (const OnExitDetectorCreator&); // not implemented

    private:
        bool _aBoolean; // used only to force instantiation of OnExitDetector

    } theCreator; // after the creation this is useless
}


OnExitDetector::OnExitDetector ()  ACS_NOEXCEPT :
    _exiting (false)
{
    atexit (OnExitDetector::exitHandler); // PRQA S 0228
}

void OnExitDetector::exitHandler (void)
{
    OnExitDetector::instance()->_exiting=true;
}

bool OnExitDetector::exiting() const ACS_NOEXCEPT // PRQA S 4120
{
    return _exiting;
}

OnExitDetector* OnExitDetector::instance()
{
    static OnExitDetector* instance = 0;
    // there is no reason to protect against multithread: this object is initialized during 
    // .so initialization - see OnExitDetectorCreator above
    if (0 == instance)
    {
        instance = new OnExitDetector; // never freed
    }
    return instance;
}

_ACS_END_NAMESPACE

