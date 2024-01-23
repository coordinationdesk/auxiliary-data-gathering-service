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
	Revision 5.5  2015/10/29 12:14:00  marfav
	PDSEV-17 printing the stack of all the running threads in case of crash
	
	Revision 5.4  2014/06/05 18:44:25  marpas
	getting rid of confusing exAssert.h with all its content
	
	Revision 5.3  2014/03/28 09:48:14  marpas
	debug on cerr removed (was erroneusly left)
	
	Revision 5.2  2014/03/28 09:42:49  marpas
	stack trace for static symbols and functions inside the unnamed namespace are reported as they are instead of empty lines
	
	Revision 5.1  2013/06/06 08:33:27  marpas
	using new signature macro
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.11  2013/03/01 12:49:06  marpas
	interface rationalization
	Guard used for memory
	
	Revision 4.10  2013/02/26 17:10:04  marpas
	interface rationalization
	
	Revision 4.9  2012/12/20 13:25:44  marpas
	coding best practices
	explain used instead of what()
	tests improved
	
	Revision 4.8  2012/11/14 18:15:55  marpas
	qa rules enforced
	
	Revision 4.7  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.6  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.4  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.3  2012/05/18 09:45:30  marpas
	fixed exceptions from string::erase computing stack trace
	
	Revision 4.2  2012/01/31 14:15:23  marpas
	missing include addded
	
	Revision 4.1  2012/01/31 14:13:04  marpas
	getting rid of boost and compilation warning reduced
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.5  2011/09/12 17:30:48  marpas
	default stack notification is now true
	
	Revision 3.4  2011/03/18 14:04:20  marpas
	dangerous exception catching removed
	
	Revision 3.3  2010/09/13 16:02:37  marpas
	smaller exException interface
	stack format changed
	
	Revision 3.2  2010/09/13 13:25:31  marpas
	the stack is clearer for the reader (and stops to main instead of going over)
	
	Revision 3.1  2009/12/02 11:46:10  marpas
	order of static variable dtor calls improved. No exeption from exstream::allow() reimplementation and better management into ThreadSafe dtro method.
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.5  2009/07/01 16:42:51  marpas
	no longer dynamically allocated trough instance() call.
	The instance() call returns always a static object.
	
	Revision 2.4  2009/06/26 14:10:44  marpas
	robustness improved on mutex when process exits
	
	Revision 2.3  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.2  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.1  2007/11/22 11:13:21  marpas
	Stack trace feature added - enabled via exStackTrace singleton
	
	

*/

#include <exStackTrace.h>
#include <Guard.hpp>
#include <exException.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <cstring>


_ACS_BEGIN_NAMESPACE(acs)

bool exStackTrace::_allow = true  ;

using std::map ;
using std::string ;
using std::exception ;

// Static attributes
ThreadSafe exStackTrace::_threadListMutex;
map<pthread_t, string> exStackTrace::_threadList;

//Thread list management
void exStackTrace::addLiveThreadId (pthread_t id, const string& name) ACS_NOEXCEPT
{
	Lock lock (_threadListMutex);
	// this will never fail
	_threadList[id] = name;
}

void exStackTrace::removeLiveThreadId (pthread_t id) ACS_NOEXCEPT
{
	Lock lock (_threadListMutex);
	// this will never fail
	_threadList.erase (id);
}

map<pthread_t, string> exStackTrace::getLiveThreadIdSet () ACS_NOEXCEPT
{
	map<pthread_t, string> retval;
	{
		Lock lock (_threadListMutex);
		retval = _threadList;
	}
	return retval;
}



void exStackTrace::allow(bool v)  ACS_NOEXCEPT 
{
    _allow = v ;
}

bool exStackTrace::allow() ACS_NOEXCEPT 
{
	return _allow ;
}

string exStackTrace::demangle(const char* name) ACS_NOEXCEPT // PRQA S 4020
{
	// need to demangle C++ symbols
	char* realname = 0 ;
	size_t len = 0;
	int stat = 0 ;

	realname = abi::__cxa_demangle(name, 0, &len, &stat); // PRQA S 4126
    pattern::CMemoryScopeGuard rn_scope(realname) ;

	if (realname != 0) {
		try {
		    return realname ; // can throw from string ctor ?
        }
        catch(exception &x) {
            std::cerr << explain(x) ;
        }
	}

	return name;
}

string exStackTrace::stackLineBeauty(const char* cline) ACS_NOEXCEPT // PRQA S 4020
{
	try {
		if ((0 == cline) || ('\0' == *cline)) { // PRQA S 3051
            // null line or zero length
            return string() ;
        }
		string line(cline) ;
		// removes [ xxx ]  at end of line as in ./stream(_ZN3acs11exExceptionD1Ev+0xd9) [0x408669]
		size_t last_open_bkt = line.find_last_of('[') ;
        if (string::npos != last_open_bkt) { // PRQA S 3278
            line.erase(last_open_bkt) ; // ./stream(_ZN3acs11exExceptionD1Ev+0xd9)
        }
		size_t ell_pos = line.find_first_of('(');
		string result = line.substr(0,ell_pos) ; // "./stream"
		result += " ( " ; // "./stream ( "
        // check kinf of "_ZN3acs11exExceptionD1Ev+0xd9"
        if (line[ell_pos+1] == ')') { // unnamed // PRQA S 3084
            return cline ;
        }
        else {
		    string fname = line.substr(ell_pos+1, line.find_last_of(')')) ; // PRQA S 3084
		    size_t first_plus = fname.find_first_of('+') ;
            if (string::npos != first_plus) { // PRQA S 3278
                fname.erase(first_plus) ; // "_ZN3acs11exExceptionD1Ev"
            }
		    result += demangle(fname.c_str()) + " )" ; // "./stream ( exException(..) )"
		    return result ;
        }
		
	} catch(exception &x_) { std::cerr << SimpleDebugSignature << explain(x_) ; }
	
	return cline ;
}

string exStackTrace::getStack(bool force) ACS_NOEXCEPT // PRQA S 4020
{
	if (_allow || force) { // shoud I notify the stack ?
		try {
			// allocate memory and get the backtrace 
			void *array[backtraceElements]; 
			int size = ::backtrace(array, sizeof(array)/sizeof(array[0])); // PRQA S 3000,3084
			char ** strings = ::backtrace_symbols(array, size); // PRQA S 2425 # no way to avoid two levels
			// note the use of free is reccomended by backtrace_symbols man page
			pattern::CMemoryScopeGuard strings_scope(strings) ;
			std::ostringstream os;
			for (int i=1 ; i < size; ++i) {
				if (strstr(strings[i],"__libc_start_main")) { // PRQA S 2824, 3706 # ::backtrace fills correctly size
					break ;
                }
				os << "\t" << stackLineBeauty(strings[i]) << '\n' ; // PRQA S 3706 # ::backtrace fills correctly size
			}
			return os.str();
		} catch (exception &) {
			//ignored.
		}
	}
	return "";
}


_ACS_END_NAMESPACE

