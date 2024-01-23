// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

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
        OnExitDetectorCreator () noexcept 
        {
            _aBoolean = acs::OnExitDetector::instance()->exiting() ; // this force the instantiation
        }

    private:
        bool _aBoolean = false ; // used only to force instantiation of OnExitDetector

    } ; 

    const OnExitDetectorCreator theCreator ; // after the creation this is useless
}


OnExitDetector::OnExitDetector ()  noexcept :
    _exiting ()
{
    atexit (OnExitDetector::exitHandler); // PRQA S 0228
}

void OnExitDetector::exitHandler (void)
{
    OnExitDetector::instance()->_exiting=true;
}

OnExitDetector* OnExitDetector::instance()
{
    static OnExitDetector* instance = nullptr ;
    // there is no reason to protect against multithread: this object is initialized during 
    // .so initialization - see OnExitDetectorCreator above
    // no thread can be started during that phase
    if (nullptr == instance)
    {
        instance = new OnExitDetector; // never freed // NOSONAR
    }
    return instance;
}

_ACS_END_NAMESPACE

