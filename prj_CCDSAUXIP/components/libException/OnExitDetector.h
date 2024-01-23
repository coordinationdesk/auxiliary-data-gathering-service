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

	$Prod: A.C.S. Thread Library $

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


#ifndef _OnExitDetector_H_
#define _OnExitDetector_H_


#include <acs_c++config.hpp>
#include <exException.h>
#include <pthread.h>

_ACS_BEGIN_NAMESPACE(acs)


// NB: although this is a Singleton, it cannot be implemented deriving from the 
// class Singleton, because it needs to be independent from mutexes 
class OnExitDetector // PRQA S 2109, 2153, 2647, 2658
{
    
public:
    bool exiting() const noexcept { return _exiting ; }
    
	OnExitDetector(const OnExitDetector &) = delete ;
	OnExitDetector &operator=(const OnExitDetector &) = delete ;

    static OnExitDetector* instance();

private:
    OnExitDetector() noexcept ;
    static void exitHandler () ;	 

// private data members
    bool _exiting;
};


_ACS_END_NAMESPACE

#endif // _OnExitDetector_H_
