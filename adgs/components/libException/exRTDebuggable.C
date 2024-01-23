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
	Revision 5.6  2018/09/04 16:13:20  marpas
	exRTDebug::registerDebug does not need any longer the class pointer (was useless)
	
	Revision 5.5  2016/05/19 10:07:01  marpas
	useless and dangerous unregister methods removed
	using only one registerDebug method of exRTDebug
	
	Revision 5.4  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.3  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.2  2013/06/05 15:55:51  marpas
	new RTDebuggable class in utility namespace to have a new approach to real-time debugging.
	previous exRTDebuggable left for backward compatibility, but it will sonn deprecated
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.6  2013/02/07 08:57:41  marpas
	pattern singleton used in debug support
	exception notification improved in debug subsystem support
	namespaces enforced
	
	Revision 4.5  2012/07/24 09:55:48  marpas
	regression fixed
	
	Revision 4.4  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.2  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.1  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.1  2011/03/01 11:07:03  marpas
	GCC 4.4.x support
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.7  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.6  2009/04/07 14:39:06  marpas
	template string are also translated: '>' and '<' become '_'
	
	Revision 2.5  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.4  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.3  2006/05/11 20:37:11  marpas
	low level memory management to avoid string repository problems with shared objects
	
	Revision 2.2  2006/05/05 08:16:12  giucas
	bug fixed. The string is reset if the RTDebuggable instance is already assigned and do not need unregistration
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.4  2005/11/14 19:01:24  ivafam
	:: are replaced with _ in debug name.
	
	Revision 1.3  2005/10/12 22:23:10  ivafam
	MP: robustness improved against threads, multiple initialization (shared objects)
	
	Revision 1.2  2005/10/06 18:04:26  marpas
	initialization with environment and include macros to help users
	
	Revision 1.1  2005/10/06 10:30:24  marpas
	Runtime debug almost completed
	
*/

#include <exRTDebuggable.h>
#include <exRTDebug.h>
#include <iostream>
#include <algorithm>
#include <cstring>


_ACS_BEGIN_NAMESPACE(acs)

_ACS_BEGIN_NAMESPACE(utility)

void registerDebug(const std::string &ref_, int *_debugLevel) ACS_NOEXCEPT
{
    exRTDebug::instance()->registerDebug(ref_, _debugLevel) ;
}

void registerDebug(const std::string &ref_, int *_debugLevel, const std::string &myself_ref_) ACS_NOEXCEPT
{
    exRTDebug::instance()->registerDebug(ref_, _debugLevel, myself_ref_) ;
}



_ACS_END_NAMESPACE


exRTDebuggable::exRTDebuggable(const std::string &r, int startValue) :
	_debugLevel(startValue),
	_debugRefString(0)
{
	// trasform any '::' into a '_'
	std::string refString = r ;
	size_t pos=0;
	do {
		pos=refString.find("::",pos);
		if(pos != std::string::npos) { // PRQA S 3278
            // 2 is the size of '::'
			refString.replace(pos,2, "_") ; // PRQA S 4400
        }
	} while(pos != std::string::npos) ; // PRQA S 3278

    using std::equal_to ;
    
	// trasform any '<' into a '_'
	replace_if(refString.begin(), refString.end(), bind2nd(equal_to<char>(),'<'), '_') ;
	// trasform any '>' into a '_'
	replace_if(refString.begin(), refString.end(), bind2nd(equal_to<char>(),'>'), '_') ;
	// trasform any ',' into a '_'
	replace_if(refString.begin(), refString.end(), bind2nd(equal_to<char>(),','), '_') ;

	_debugRefString = strdup(refString.c_str()) ;
	if (0 == _debugLevel) {
		const char *exDbgGE = getenv(_debugRefString) ;
		_debugLevel =  exDbgGE ? atoi(exDbgGE) : -1 ; // PRQA S 2956,3380,3385 # getenv returns a "good" pointer or null
	}
	exRTDebug::instance()->registerDebug(_debugRefString, &_debugLevel) ;
}

// PRQA S 4631 L1
// note that no exception from this 
exRTDebuggable::~exRTDebuggable() ACS_NOEXCEPT 
{
	// _debugRefString was obtained by strdup --> malloc hence free and not delete
	free(_debugRefString) ;
	_debugRefString = 0 ;
}
// PRQA L:L1


const char *exRTDebuggable::getDebugRef() const ACS_NOEXCEPT // PRQA S 4120
{
	return _debugRefString ;
}


int exRTDebuggable::getDebug() const ACS_NOEXCEPT // PRQA S 4120
{
	return _debugLevel ;
}


_ACS_END_NAMESPACE
