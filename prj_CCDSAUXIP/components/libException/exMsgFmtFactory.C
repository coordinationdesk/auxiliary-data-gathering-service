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
	Revision 5.10  2018/09/13 10:14:09  marpas
	ying to decouple instances of formatter from global ctor and dtor
	
	Revision 5.9  2018/09/13 09:48:45  marpas
	atexit handler removed
	
	Revision 5.8  2018/09/12 10:05:39  marpas
	exception's stack trace management revived into streams.
	tested
	
	Revision 5.7  2017/07/10 11:41:04  davide.tiriticco
	compiler warnings removed
	
	Revision 5.6  2017/01/30 15:32:01  clanas
	fixed compiler anchor >= 4
	
	Revision 5.5  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.4  2013/11/15 13:19:30  marpas
	introducing the isFilterable concept into an exostream.
	It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
	
	Revision 5.3  2013/09/26 18:17:44  marpas
	housekeeping, types renaming and cleanup
	
	Revision 5.2  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.1  2013/06/06 08:34:16  marpas
	namespaces and includes fixed
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.10  2013/05/14 17:58:44  marpas
	exSystemException added
	
	Revision 4.9  2012/12/20 13:25:44  marpas
	coding best practices
	explain used instead of what()
	tests improved
	
	Revision 4.8  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.7  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.6  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/11 15:25:53  marpas
	qa fixing in progress
	
	Revision 4.4  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.3  2012/05/30 16:20:51  marpas
	S3PDGS-741 - exception's format depends on exMsgFmt
	
	Revision 4.2  2012/03/19 15:49:25  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.1  2012/01/31 14:20:06  marpas
	compilation warning remotion
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.1  2011/03/01 11:07:03  marpas
	GCC 4.4.x support
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.5  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.4  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.3  2009/03/02 17:02:39  marpas
	CODECHECK parsing - no regressions
	
	Revision 2.2  2008/02/15 16:58:54  marpas
	each stream can have its own formatter setFormatter method added
	exception::what() has no longer the stack if enabled
	exception::stack() returns the stack (if enabled)
	exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
	Filterables now hase details, they can be set via exFilterable::detail(...) method
	Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
	exception inserted into a Filterable will fill its detail with their stack (if any)
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.9  2004/04/28 10:11:22  marpas
	Lock policy improved
	ThreadSafe::SignalAction implemented
	
	Revision 1.8  2004/04/26 15:05:41  paoscu
	exostream::_threadSafe used for locks.
	
	Revision 1.7  2004/04/06 10:35:51  marpas
	cleanup
	
	Revision 1.6  2004/04/06 09:55:18  marpas
	new message architecture implemented: more freedom in formatting messages
	new class exFDebug added
	
	Revision 1.5  2004/03/24 11:21:54  marpas
	time method implemented in the factory to allow new factories to change the time format
	
	Revision 1.4  2004/03/24 11:11:10  marpas
	separator added
	
	Revision 1.3  2003/07/10 14:43:39  marpas
	header make-up
	
	Revision 1.2  2003/04/30 09:57:38  marpas
	using namespace std was removed from includes
	
	Revision 1.1  2002/11/21 17:57:03  marpas
	- exostream modified for exFilterableManagement
	- exFEvent type added
	- exMsgFmtFactory added to provide tag and other values to the exFMessage
	class

*/

#include <exMsgFmtFactory.h>
#include <exFilterable.h>
#include <exStream.h>
#include <ThreadSafe.h>

#include <sstream>
#include <iomanip>
#include <algorithm>

#include <sys/time.h> // PRQA S 1013
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs)

namespace {

    std::vector<exMsgFmt *>  *_instances() 
    {
        static std::vector<exMsgFmt *>  *_v = nullptr ;
        if (nullptr == _v) {
            _v = new std::vector<exMsgFmt *> ; // NOSONAR - act as a singleton
        }
        return _v ;
    } 
}

exMsgFmt *exMsgFmtFactory::instance() 
{
	if (_instances()->empty()) {
		new exMsgFmtFactory ; // NOSONAR - act as a singleton
	}
	
	return _instances()->back() ;
}

// PRQA S 2010 2
exMsgFmtFactory::exMsgFmtFactory(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) :
exMsgFmt(pid,time,start,end,sep)
{
	Lock const lock(exostream::globalMutex()) ;
	_instances()->push_back(this) ;
}


exMsgFmtFactory::exMsgFmtFactory() :
    exMsgFmt()
{
	Lock const lock(exostream::globalMutex()) ;
	_instances()->push_back(this) ;
}

exMsgFmtFactory::~exMsgFmtFactory() noexcept
{
	try { // no exception out of this method 
		Lock const lock(exostream::globalMutex()) ; // PRQA S 4631 4
		auto tbd = find(_instances()->begin(),_instances()->end(),this) ;
		if (tbd != _instances()->end()) {
			_instances()->erase(tbd) ;
        }
	}
	catch(const std::exception &x) {
        // note it goes on cerr and not on excerr (bcause it uses this)
		std::cerr << explain(x) ; // NOSONAR - cannot use streams
	}
}







// PRQA S 2010 2
exMsgFmt::exMsgFmt(bool pid, bool time, const std::string &start, const std::string &end, const std::string &sep) :
	_emitPid(pid),
	_emitTime(time),
	_startTag(start),
	_endTag(end),
	_separator(sep)
{
}


bool exMsgFmt::emitPid() const
{
	return _emitPid ;
}

void exMsgFmt::emitPid(bool s) 
{
	_emitPid = s ;
}

bool exMsgFmt::emitTime() const 
{
	return _emitTime ;
}

void exMsgFmt::emitTime(bool s) 
{
	_emitTime = s ;
}

std::string exMsgFmt::startTag() const
{
	Lock const lock(exostream::globalMutex()) ;
	return _startTag ;
}

void exMsgFmt::startTag(const std::string &s) 
{
	Lock const lock(exostream::globalMutex()) ;
	_startTag = s ;
}

std::string exMsgFmt::endTag() const
{
	Lock const lock(exostream::globalMutex()) ;
	return _endTag ;
}

void exMsgFmt::endTag(const std::string &s) 
{
	Lock const lock(exostream::globalMutex()) ;
	_endTag = s ;
}

std::string exMsgFmt::separator() const
{
	Lock const lock(exostream::globalMutex()) ;
	return _separator ;
}

void exMsgFmt::separator(const std::string &s) 
{
	Lock const lock(exostream::globalMutex()) ;
	_separator = s ;
}

std::string exMsgFmt::time() const
{
	std::ostringstream tims ;
	struct timezone tz ; // PRQA S 4102 
    struct timeval tm_fine ; // PRQA S 4102 
    // note thos fills both the POD structs
	::gettimeofday(&tm_fine,&tz) ;
    
    struct tm tmloc ; 
	const struct tm * tm_coarse = localtime_r(&tm_fine.tv_sec, &tmloc) ; 
    // PRQA S 4400 L1
	// the following to write dd/mm hh:mm:sec.fff
    using std::setw ;
    using std::setfill ;
	tims << setw(2) << setfill('0') << tm_coarse->tm_mday << '/'
		 <<	setw(2) << setfill('0') << tm_coarse->tm_mon + 1 << ' ' 
		 << setw(2) << setfill('0') << tm_coarse->tm_hour << ':'
		 << setw(2) << setfill('0') << tm_coarse->tm_min << ':'
		 << setw(2) << setfill('0') << tm_coarse->tm_sec << '.'
		 << setw(3) << setfill('0') << tm_fine.tv_usec/1000 ;
    // PRQA L:L1
	return tims.str() ;
}


std::string exMsgFmt::text(const exFilterable &m) const 
{
	// prepare the text to be returned
	Lock const lock(exostream::globalMutex()) ;
	std::string out ;

	out += _startTag ;

	if (_emitTime) {
		out += time() + _separator ;
    }
    
	out += _appName ;
	if (!_appName.empty()) {
		out += _separator ;
    }
    
	if (_emitPid) {
		std::ostringstream pids ;
		pids << ::getpid() ;
		out += pids.str() + _separator;
	}
	
	std::string msg = m.message() ;
	
    if (!msg.empty()) {
	    // probably not the best way but does the job.
	    size_t last = msg.length()-1 ; // PRQA S 3084
	    if (msg[last] == '\n') {
		    msg[last] = ! _separator.empty() ?  _separator[0] : ' ' ; // PRQA S 3380
        }

	    out += msg ;
	}
    
	out += _endTag ;
	
	
	return out ; // return the string
}

std::string exMsgFmt::text(const std::exception &x, bool stack) const 
{
    std::ostringstream out ;
    out << explain(x, stack) ;
    return out.str() ;
}


void exMsgFmt::appName(const std::string &a) 
{
	Lock const lock(exostream::globalMutex()) ;
	_appName = a ;
}

const std::string &exMsgFmt::appName() const
{
	Lock const lock(exostream::globalMutex()) ;
	return _appName ;
}



_ACS_END_NAMESPACE
