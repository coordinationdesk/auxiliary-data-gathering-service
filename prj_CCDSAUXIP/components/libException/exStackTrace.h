// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
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
	Revision 5.1  2015/10/29 12:14:00  marfav
	PDSEV-17 printing the stack of all the running threads in case of crash
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.8  2013/03/01 12:49:06  marpas
	interface rationalization
	Guard used for memory
	
	Revision 4.7  2013/02/26 17:10:04  marpas
	interface rationalization
	
	Revision 4.6  2012/12/11 10:10:00  marpas
	qa rules
	
	Revision 4.5  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.4  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.2  2012/06/06 17:12:21  marpas
	qa rules application in progress
	
	Revision 4.1  2012/01/31 14:13:04  marpas
	getting rid of boost and compilation warning reduced
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.1  2010/09/13 13:25:31  marpas
	the stack is clearer for the reader (and stops to main instead of going over)
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.4  2009/07/01 16:42:51  marpas
	no longer dynamically allocated trough instance() call.
	The instance() call returns always a static object.
	
	Revision 2.3  2009/06/26 14:10:44  marpas
	robustness improved on mutex when process exits
	
	Revision 2.2  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.1  2007/11/22 11:13:21  marpas
	Stack trace feature added - enabled via exStackTrace singleton
	
*/


#ifndef _exStackTrace_H_
#define _exStackTrace_H_ 

#include <acs_c++config.hpp>
#include <map>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class ThreadSafe ;
using Mutex = ThreadSafe ;
/*! \class exStackTrace

	\brief class container for stacktrace related ops
	
	This class is just a container for stack tracing functions 
	It is not meant to be constructed, so no instances can be created.
*/
// MP: this class could be transformed into a namespace with the same name, hiding all the data 
// inside the compilation module
class exStackTrace // PRQA S 2109
{
public:
	exStackTrace() = delete ;
	~exStackTrace() = delete ;
	
	static void allow(bool) noexcept ;
	static bool allow() noexcept ;
	static std::string getStack(bool force = false) noexcept ;

	/*! \brief symbols demangle function.
	
	    This is an helper function (used internally).
	    It is left public in order to allow programmers to demangle other symbols.
	    
	    \note uses abi::__cxa_demangle
	    
	 */
	static std::string demangle(const char* name) noexcept ;

	/*! \brief stack line beautifier.
	
	    This is an helper function (used internally).
	    It is left public in order to allow programmers to beautify stack lines obtained through ::backtrace.
	    
	 */
	static std::string stackLineBeauty(const char* cline) noexcept ;

	/**
	 *\brief  Adds a thread_id at the list of the live threads
     *\param id a thread id
	 */
	static void addLiveThreadId (pthread_t id, const std::string&) noexcept;

	/**
	 *\brief  Deletes a thread_id at the list of the live threads
     *\param  id a thread id
	 */
	static void removeLiveThreadId (pthread_t id) noexcept;

	/**
	 *\brief  Gets a copy of the live threads list
	 */
	static std::map<pthread_t, std::string> getLiveThreadIdSet () noexcept;

private:
    static const int backtraceElements = 256 ;
	static bool _allow ;

	// Static attributes used to mantain the list of the active thread id of the process
	static std::map <pthread_t, std::string> _threadList;
	static Mutex _threadListMutex;


} ;

_ACS_END_NAMESPACE



#endif // PRQA S 1000
