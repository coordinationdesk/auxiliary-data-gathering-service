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
    Revision 5.37  2018/10/04 15:43:00  marpas
    exLogStream::creationTime() added
    
    Revision 5.36  2018/09/27 10:37:40  marpas
    algorithm included
    
    Revision 5.35  2018/09/12 10:05:39  marpas
    exception's stack trace management revived into streams.
    tested
    
    Revision 5.34  2017/07/10 11:41:04  davide.tiriticco
    compiler warnings removed
    
    Revision 5.33  2017/02/09 13:04:21  marfav
    NotifyOnly replaced with FormattedOnly
    
    Revision 5.32  2017/02/09 09:09:03  marfav
    CSGACQ-113
    Addedd support to NotifyOnly mode to avoid free text insertions in streams
    
    Revision 5.31  2016/10/07 15:23:09  marfav
    do not chmod /dev/null streams
    
    Revision 5.30  2016/10/07 10:00:39  marfav
    S1PDGS-31809
    Applying the default log permission settings on every new open log
    
    Revision 5.29  2016/07/04 10:11:56  marfav
    CSGACQ-84
    Added functions to set/get the append flag in exLogStream
    Added function to rotate a log and open a new one in a different path
    
    Revision 5.28  2016/06/13 12:52:28  marpas
    exSyslog class allow to change facility from LOG_LOCAL0 (default) to any allowed values (see syslog(3) )for this a new method has been added: setFacility
    The private method open was never called: removed
    
    Revision 5.27  2015/06/16 14:45:43  marpas
    qa warnings
    
    Revision 5.26  2015/03/12 13:02:13  marpas
    fixing exSyslog::get_prio dynamic casts order due to new status hierarchy
    
    Revision 5.25  2015/03/12 12:43:15  marpas
    introducing exFWStatus and exFEStatus filterables along their macros ACS_LOG_WSTATUS and ACS_LOG_ESTATUS respectively.
    The sysinfo log handler uses LOG_WARNING and LOG_ERR prio for them.
    
    Revision 5.24  2014/11/04 13:48:04  marfav
    destroying check/set moved after global mutex lock in inserters and dtors
    
    Revision 5.23  2014/09/12 13:19:50  marfav
    LogStream can not throw exceptions during global dtors phase
    
    Revision 5.22  2014/03/21 13:57:18  marpas
    qa warnings
    
    Revision 5.21  2014/02/07 18:03:07  marpas
    introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
    documentation improved and tests aligned
    
    Revision 5.20  2013/11/19 17:53:10  marpas
    CoutLogger concept added
    doc updated
    debug improved
    
    Revision 5.19  2013/11/18 17:30:32  marpas
    syslog level for exFStatus set to LOG_NOTICE
    
    Revision 5.18  2013/11/15 13:19:30  marpas
    introducing the isFilterable concept into an exostream.
    It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
    
    Revision 5.17  2013/11/13 17:59:55  marpas
    removing useless debug
    
    Revision 5.16  2013/10/21 15:06:41  marpas
    reference used to avoid string copy
    
    Revision 5.15  2013/10/18 13:56:30  marfav
    Using a copy of message when notifying
    
    Revision 5.14  2013/10/18 12:25:47  marpas
    perfo improved, debug improved
    
    Revision 5.13  2013/10/18 08:27:33  marfav
    Passing unformatted messages to the syslog
    
    Revision 5.12  2013/10/11 09:39:48  marpas
    streams graph traversal implemented to avoid multiple notification of the same filterable due to complex graphs
    
    Revision 5.11  2013/09/27 14:05:17  marpas
    streams have names
    
    Revision 5.10  2013/09/26 18:17:44  marpas
    housekeeping, types renaming and cleanup
    
    Revision 5.9  2013/09/26 18:09:35  marpas
    robustness strongly improved:
    singleton functions (with static variables inside) used
    some private members are now accessed only through the above technique
    exRTDebuggable classes do not longer unregster themselves from exRTDebug.
    it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
    The above changes will lead in a more efficient process closure.
    moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
    
    Revision 5.8  2013/09/26 12:25:32  marpas
    debug improved
    debug variables are forced during construction of excerr etc...
    
    Revision 5.7  2013/07/26 14:05:00  marfav
    setDefaultOutLogger fixed
    
    Revision 5.6  2013/06/20 13:42:48  marpas
    excmcf is directed to Cmcfand no longer on stderr
    
    Revision 5.5  2013/06/10 16:13:59  marpas
    using and enforcing new standards
    
    Revision 5.4  2013/06/06 12:54:03  marpas
    ACS_VERYLOW_VERB changed into ACS_VLO_VERB
    
    Revision 5.3  2013/06/06 08:31:28  marpas
    useless include removed, namespaces enforced
    
    Revision 5.2  2013/06/05 15:57:11  marpas
    new real time debugging macros adopted
    
    Revision 5.1  2013/06/04 17:23:46  marpas
    introducing ne RTDebuggable template
    
    Revision 5.0  2013/06/04 15:35:02  marpas
    introducing new log macros, debug macros
    work in progress
    
    Revision 4.20  2013/04/09 07:48:03  marpas
    documentation fix
    
    Revision 4.19  2013/02/07 08:57:41  marpas
    pattern singleton used in debug support
    exception notification improved in debug subsystem support
    namespaces enforced
    
    Revision 4.18  2012/12/20 13:25:45  marpas
    coding best practices
    explain used instead of what()
    tests improved
    
    Revision 4.17  2012/10/29 14:29:52  marpas
    robustness improved
    
    Revision 4.16  2012/06/25 17:42:06  marpas
    qacpp instrumentation
    
    Revision 4.15  2012/06/14 17:16:41  marpas
    qacpp instrumentation
    
    Revision 4.14  2012/06/12 17:37:01  marpas
    qacpp instrumentation
    
    Revision 4.13  2012/06/11 15:25:53  marpas
    qa fixing in progress
    
    Revision 4.12  2012/06/06 17:12:21  marpas
    qa rules application in progress
    
    Revision 4.11  2012/06/05 17:07:00  marpas
    resolving some quality issues and documentation
    
    Revision 4.10  2012/05/30 16:44:16  marpas
    empty filterables or exceptions do not print a carriage return
    
    Revision 4.9  2012/05/30 16:20:51  marpas
    S3PDGS-741 - exception's format depends on exMsgFmt
    
    Revision 4.8  2012/05/18 09:57:18  marpas
    syslog call fixed
    
    Revision 4.7  2012/04/23 14:01:44  marpas
    exSyslog class added
    
    Revision 4.6  2012/03/30 09:19:56  marpas
    flushing log after insertion of filterable or exception
    
    Revision 4.5  2012/03/22 17:08:02  marpas
    in case of log creation problem, size is always 0
    
    Revision 4.4  2012/03/22 14:48:04  marpas
    readability of method improved
    
    Revision 4.3  2012/03/19 15:49:25  marpas
    improving debug and getting rid of std::e_n_d_l()
    
    Revision 4.2  2012/02/14 09:03:32  marpas
    operator<< overloaded for bool
    
    Revision 4.1  2012/01/31 14:13:04  marpas
    getting rid of boost and compilation warning reduced
    
    Revision 4.0  2011/10/18 11:22:00  marpas
    new statistics
    
    Revision 3.10  2011/09/29 11:32:52  marpas
    signature slightly changed
    
    Revision 3.9  2011/05/12 06:38:54  marpas
    message reversed
    
    Revision 3.8  2011/05/11 15:46:44  marpas
    stacks in exception are chained
    robustness improved
    
    Revision 3.7  2011/03/30 10:59:07  marpas
    in case of exLogStream accessing problem, an exception is raised and notified (but on the log itself).
    
    Revision 3.6  2011/03/21 08:58:25  marpas
    multithread robustness improved
    
    Revision 3.5  2011/03/18 18:09:34  marpas
    multithread robustness improved,
    exLogStream now implements a rotate(....) method in order to give atomicity trait to the rotation, used from Appllication class
    
    Revision 3.4  2011/03/01 11:07:03  marpas
    GCC 4.4.x support
    
    Revision 3.3  2010/09/15 10:20:52  marpas
    exostream::notifyStackFromEx  changed into exostream::allowStackTrace
    
    Revision 3.2  2010/09/13 16:02:37  marpas
    smaller exException interface
    stack format changed
    
    Revision 3.1  2009/10/08 12:38:17  marpas
    exLogStringStream class added
    
    Revision 3.0  2009/10/08 10:48:10  marpas
    Statistics class added
    rules application improved
    
    Revision 2.14  2009/06/26 10:41:55  marpas
    some classes do not throw any exception from their dtors
    
    Revision 2.13  2009/03/11 17:09:02  marpas
    c++rules violation removed
    
    Revision 2.12  2009/03/02 17:02:39  marpas
    CODECHECK parsing - no regressions
    
    Revision 2.11  2008/06/20 16:02:32  marpas
    exostream deletion check added
    
    Revision 2.10  2008/06/09 13:38:29  marpas
    exLogStream dtor now locks to avoid other threads to use an invalid file stream
    
    Revision 2.9  2008/05/06 16:15:23  marpas
    locking during the whole exLogStream ctor was needed (APF-218)
    
    Revision 2.8  2008/05/05 15:12:18  marpas
    wrong static initialization fixed _threadSafe was initialized after the exostream instances (APF-218)
    
    Revision 2.7  2008/02/15 16:58:54  marpas
    each stream can have its own formatter setFormatter method added
    exception::what() has no longer the stack if enabled
    exception::stack() returns the stack (if enabled)
    exception stack are no longer notified if present unless the stream is instructed to do that via allowStackTrace method
    Filterables now hase details, they can be set via exFilterable::detail(...) method
    Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
    exception inserted into a Filterable will fill its detail with their stack (if any)
    
    Revision 2.6  2007/07/20 09:57:25  marpas
    exception message clarified
    
    Revision 2.5  2007/07/20 09:07:32  marpas
    logfile now supports permission setting via setPermission method
    
    Revision 2.4  2007/04/18 10:19:04  marpas
    global dtor behaviour enforced, so declaration order is fixed
    
    Revision 2.3  2006/11/03 01:05:34  marpas
    changes for moving window's messages tree
    
    Revision 2.2  2006/10/31 20:41:52  marpas
    defaultNotify implementation almost done
    
    Revision 2.1  2006/02/28 08:24:37  marpas
    new release, exException inherits from std::exception
    
    Revision 1.32  2005/10/12 22:21:59  ivafam
    exostream::ThreadSafe initailization moved in forced initialization section
    
    Revision 1.31  2005/09/29 19:29:18  marpas
    debug improved
    
    Revision 1.30  2005/06/16 15:29:24  marpas
    warning added if not LONG_LONG support
    
    Revision 1.29  2005/05/12 07:36:49  marpas
    bug fixed in adding children
    
    Revision 1.28  2005/03/18 10:57:43  marpas
    loop check implemented, an exception is raised attempting to create stream loops.
    redirect(int) implemented
    
    Revision 1.27  2005/03/16 09:04:18  marpas
    exmcf changed into excmcf
    
    Revision 1.26  2005/03/16 08:46:52  marpas
    exmcf exostream object created.
    
    Revision 1.25  2005/03/16 08:43:11  marpas
    *** empty log message ***
    
    Revision 1.24  2005/03/01 18:17:31  marpas
    long double support added
    
    Revision 1.23  2004/09/27 09:23:06  marpas
    exFilterable I/F changed
    
    Revision 1.22  2004/09/20 15:26:06  marpas
    exBaseStream class introduced, is inside the inheritance tree between exLogStream and exStream
    
    Revision 1.21  2004/05/11 14:03:55  marpas
    checkFd changed
    
    Revision 1.20  2004/04/14 11:06:13  marpas
    filter installation improved
    
    Revision 1.19  2004/04/14 09:05:09  marpas
    many changes
    
    Revision 1.18  2004/04/06 10:35:51  marpas
    cleanup
    
    Revision 1.17  2004/04/05 14:39:04  marpas
    unitBuffered & isUnitBuffered methods added
    
    Revision 1.16  2004/04/02 15:54:44  marpas
    Legacy code compilation fixed
    
    Revision 1.15  2004/04/02 14:15:14  marpas
    ThreadSafe class introduced and used inside
    
    Revision 1.14  2003/07/21 09:21:57  marpas
    mutex is unique for all chains
    
    Revision 1.13  2003/07/10 14:42:38  marpas
    LogStream::size method added
    
    Revision 1.12  2003/06/24 08:25:12  marpas
    long long types supported
    
    Revision 1.11  2003/04/30 09:57:38  marpas
    using namespace std was removed from includes
    
    Revision 1.10  2003/04/15 17:12:09  marpas
    GCC3.2
    
    Revision 1.9  2003/02/27 13:27:24  marpas
    ::flush method fixed
    
    Revision 1.8  2002/12/06 16:11:44  marpas
    Manipulators fix for gcc 2.96
    
    Revision 1.7  2002/11/29 14:13:10  marpas
    notifyException now returns true and throws nothing.
    Now its even better
    
    Revision 1.6  2002/11/28 18:55:22  marpas
    now is better (look by yourself)
    
    Revision 1.5  2002/11/26 10:31:18  marpas
    Thread safe implementation improved.
    
    Revision 1.4  2002/11/22 13:59:26  marpas
    Thread-Safe implementation
    
    Revision 1.3  2002/11/21 17:57:03  marpas
    - exostream modified for exFilterableManagement
    - exFEvent type added
    - exMsgFmtFactory added to provide tag and other values to the exFMessage
    class
    
    Revision 1.2  2002/10/03 15:05:38  marpas
    exFilter and exFilterable added
    
    Revision 1.1.1.1  2002/09/30 19:36:15  marpas
    Import libException
    
*/

#include <algorithm>
#include <exStream.h>
#include <exException.h>
#include <Filterables.h>
#include <exFMCFilterables.h>
#include <exFilter.h>
#include <exMsgFmtFactory.h>
#include <exV.h>

#include <unistd.h>

#include <cerrno>          
#include <sys/stat.h>       // PRQA S 1013
#include <syslog.h>         

#include <OnExitDetector.h>


_ACS_BEGIN_NAMESPACE(acs)

namespace {
    const exV version ; // NOSONAR - version mgmt
}

using std::exception ;
using std::hex ;
using std::dec ;
using std::oct ;
using std::ios_base ;
using std::ostringstream ;
using std::ostream ;
using std::ofstream ;
using std::streampos ;
using std::vector ;
using std::string ;
using std::cout ;
using std::cerr ;
using std::endl ;

int exostream_Init::_initcount = 0 ;


exostream excerr("excerr", exostream::StdCerr) ; // NOSONAR - global stream
exostream excout("excout", exostream::StdCout) ; // NOSONAR - global stream
exostream excmcf("excmcf", exostream::Cmcf) ; // NOSONAR - global stream

ACS_CLASS_DEFINE_DEBUG_LEVEL(exostream_Init) 


exostream_Init::exostream_Init() 
{
    if (0 == _initcount) { // very firs time 
        new(&exostream_Init::exCDebugLevel) acs::utility::RTDebuggable<exostream_Init> ; // PRQA S 3802 2
        new(&exostream::exCDebugLevel) acs::utility::RTDebuggable<exostream> ;
    }
    CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "count: " << _initcount) ;
    if (0 == _initcount) { // very firs time 
        CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "creating excerr") ;
        CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "creating excout") ;
        // note the use of new ... we pass it the address in which we want to build - no bad_alloc
        new(&excerr) exostream("excerr",exostream::StdCerr) ;    // PRQA S 3802 2
        new(&excout) exostream("excout",exostream::StdCout) ;    
    }
    ++_initcount;
}

exostream_Init::~exostream_Init() noexcept
{
    // PRQA S 4631 exostream_Init_dtor
    --_initcount; // decrement initialized count
    try {
        CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "count: " << _initcount) ;
    }
    catch(const exception &) { // NOSONAR - ignore
        // ignore - cerr doesn't work here
    }
    // PRQA L:exostream_Init_dtor
}

//
//
// exostream
//
//

ACS_CLASS_DEFINE_DEBUG_LEVEL(exostream) 
Mutex &exostream::globalMutex() noexcept { static Mutex mutex_ ; return mutex_ ; } // PRQA S 2328

exostream *exostream::_defaultErrLogger = nullptr ;
exostream *exostream::_defaultOutLogger = nullptr ;
exostream *exostream::_defaultCoutLogger = nullptr ;

// the cmcf stream (whose name has be chosen to be like cerr, cout) is linked to /dev/null
ofstream exostream::cmcf("/dev/null") ; // PRQA S 4634

exostream::exostream(const std::string& name, StreamFdEquiv fd) :
    _sysFd(fd),
    name_(name)
{
    Lock const lock(globalMutex()) ;
    CERR_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getName() << " this: " << hex << this << " _sysFd:" << _sysFd) ;
    checkFd() ; // check the file descriptor
}

exostream::exostream(const std::string& name, exostream *ex, StreamFdEquiv fd) :
    _sysFd(fd),
    name_(name)
{
    Lock const lock(globalMutex()) ;
    CERR_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getName() << " this: " << hex << this << " _sysFd:" << _sysFd) ;
    checkFd() ;

    _parents.push_back(ex) ; // we have a parent .. 
    appendToParents() ; // append my self as child 
}

exostream::exostream(const std::string& name, const std::vector<exostream *> &parents, StreamFdEquiv fd) :
    _parents(parents),
    _sysFd(fd),
    name_(name)
{
    Lock const lock(globalMutex()) ;
    CERR_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getName() << " this: " << hex << this << " _sysFd:" << _sysFd) ;
    checkFd() ;

    appendToParents() ; // a number of parents to which be added
}

exostream::~exostream() noexcept
{
// PRQA S 4631 exostream_dtor
    try {
        Lock const lock(globalMutex()) ;
        destroying() ;
        CERR_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getName() << " this: " << hex << this) ;

        try { // NOSONAR - separate try-block
            removeFromParents() ;
        } 
        catch(const exception &x) { // NOSONAR
            // cannot do anything but this
            cerr << explain(x) ; // NOSONAR - can't use streams here
        }

        std::for_each(_filters.begin(), 
                 _filters.end(),
                 [&](auto &i){ if (i) i->streamDestroying(this) ; }
                ) ;

    }
    catch(const exception &x) { // NOSONAR
        // cannot do anything but this
        cerr << explain(x) ; // NOSONAR - can't use streams here
    }
// PRQA L:exostream_dtor
}


exostream &exostream::flush() // PRQA S 4020 
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return *this ; // give up
    }

    preflush() ; // call preflush virtual method in order to fill the streams
    stream().flush() ; // flush the streams 
    for (auto i : _connected) { i->flush() ; }
    postflush() ; // call postflush ... to recover if needed
    return *this ;
}


void exostream::appendChild(exostream *s) // PRQA S 4020
{
    Lock const lock(globalMutex()) ;
    if ((!s) || isDescendant(s)) { // already a desceendant
        return ; // nothing to do
    } 

    CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, "appending: " << hex << s << " to: " << this) ;
    if (s->isDescendant(this)) { // i am a descendant of s - WARNING is a loop
        ACS_THROW(exIllegalValueException("exostream loop: the given stream is already a descendant of this one.")) ; // PRQA S 3081
    }
    
    _connected.push_back(s) ; // add it to my descendant
}

bool exostream::isDescendant(const exostream *s) const // PRQA S 4020, 4214
{
    Lock const lock(globalMutex()) ;
    
    if (s == this) { return true ; } // I am a descendant of myself 

    CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, "me: " << this << " rhs: " << s) ;
    
    for (auto c : _connected) { // each connected stream 
        if ((s == c) || c->isDescendant(s)) {
            // s is a connected or a descendant of it
            CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, "found") ;
            return true ; // hence is my descendant
        }
    }
/*
    for (size_t i=0; i < _connected.size(); ++i) { // each connected stream 
        if ((s == _connected[i]) || _connected[i]->isDescendant(s)) {
            // s is a connected or a descendant of it
            CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, "found") ;
            return true ; // hence is my descendant
        }
    }
*/   
    CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, "not found") ;
    return false ; // no descendant
}

exostream::StreamFdEquiv exostream::redirect(StreamFdEquiv v)
{
    Lock const lock(globalMutex()) ;
    StreamFdEquiv ret = _sysFd ;
    _sysFd = v ;
    _nullStream.reset() ;
    checkFd() ;
    return ret ;
}

void exostream::set_stream(std::ostream *s) noexcept 
{
    // PRQA S 4631 set_stream_end
    try {
        Lock const lock(globalMutex()) ;
        _stream = s ;
        _sysFd = NullStream ;
        _nullStream.reset() ;
    }
    catch(const exception &e) { // NOSONAR - any exception
        cerr << explain(e) ; // NOSONAR - can't use streams here 
    }
    // PRQA L:set_stream_end
    
}



void exostream::removeChild(exostream *s)
{
    Lock const lock(globalMutex()) ;
    const auto ctdb = find(_connected.begin(),_connected.end(), s) ;
    if (ctdb != _connected.end()) {
        _connected.erase(ctdb) ;
    }
}

void exostream::appendToParents()
{
    Lock const lock(globalMutex()) ;
    
    std::for_each(_parents.begin(), _parents.end(), 
                  [this](auto i) { if (i) i->appendChild(this) ; }
                 ) ;
}

void exostream::removeFromParents()
{
    Lock const lock(globalMutex()) ;

    std::for_each(_parents.begin(), _parents.end(), 
                  [this](auto i) { if (i) i->removeChild(this) ; }
                 ) ;
}

exostream &exostream::notify(const std::exception &x) noexcept // PRQA S 4020 
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return *this ; // give up
    }

    CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, getName()) ;
    
    notifyException(x) ; // notify it
    // pass the exception to every connected stream
    std::for_each(_connected.begin(), _connected.end(), 
                  [&x](auto i) { if (i) i->notify(x) ; }
                 ) ;
    return *this ;
}

exostream &exostream::notify(exFilterable &s) // PRQA S 4020 
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return *this ; // give up
    }
    
    s.insertionLock() ;
    StreamGraphTraversal const traversed(s) ; // register the traversal to the filterable
    // if the filterable has not been already traversed and the filter passes
    if ((! s.traversed(this)) && applyFilter(s)) { // filter pass ? // PRQA S 3229, 3230
        CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, getName() << " notifying") ;
        notifyFilterable(s) ; // notify 
    }
    // pass the filterable to every connected stream
    for (auto i : _connected) {
        CERR_CLASS_WRITE_DEBUG(ACS_MID_VERB, getName() << " notify to " << i->getName()) ;
        i->notify(s) ;
    }
    return *this ;
}

bool exostream::notifyException(const std::exception &x) noexcept // PRQA S 4020 
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return false ; // give up
    }
    
    if (_mode != Silent) { // am i  verbose or silent ?
        const string &xm = (_formatter ? _formatter->text(x, _allowStackTrace): toString(x, _allowStackTrace)) ; // PRQA S 3384, 3385
        if (! xm.empty()) {
            stream() << xm << endl ; // will flush: wanted 
        }
    }
    
    return true ;
}  

void exostream::notifyFilterable(exFilterable &s) // PRQA S 4020
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return ; // give up
    }
    
    if (_mode != Silent) { // am i  verbose or silent ?
        const string &sm = ( _formatter ? _formatter->text(s): s.text()) ; // PRQA S 3384, 3385
        if (! sm.empty()) {
            try {
                // verbose: notify 
                stream() << sm ; 
                // also details if is the case
                if (_notifyDetailFromFilterable && ! s.detail().empty()) {
                    stream() << '\n' << s.detail() ;
                }
                stream() << endl ; // will flush: wanted 
            }
            catch(const exception &e) { // NOSONAR - no exception here !
                cerr << explain(e) ; // NOSONAR - can't use streams here 
            }      
        }
    }  
}  



ostream & exostream::stream() // PRQA S 4020 
{ 
    Lock const lock(globalMutex()) ;

    CERR_CLASS_WRITE_DEBUG(ACS_LOW_VERB, getName() << " _stream: " << _stream << " _sysFd: " << _sysFd) ;
    
    if (_stream) { return *_stream ; } 
    
    switch(_sysFd) {
    case NullStream:
        // -1 -> NullStream
        if (nullptr == _nullStream) { 
            _nullStream = std::make_unique<ofstream>("/dev/null") ; // NOSONAR # dtor deallocate it
        }
        return *_nullStream ; // PRQA S 2844 # _nullStream can't be null
    case Cmcf:    return cmcf ; 
    case StdCout: return cout ; // NOSONAR 
    case StdCerr: return cerr ; // NOSONAR 
    default: // PRQA S 4612 # falltrough
        ACS_THROW(exIllegalValueException() << "Invalid value: " << _sysFd << " the fd passed at ctor must be in the [0,2] range ") ; // PRQA S 3081
    }
} 


void exostream::checkFd() const
{
    switch(_sysFd) { // allowed fd are 0, 1, 2 // PRQA S 4018 # switch is more readable
    case NullStream: // special case
    case Cmcf:
    case StdCout:
    case StdCerr:
        break ;
    default: // PRQA S 4612 # fall through
        ACS_THROW(exIllegalValueException() << "Invalid value: " << _sysFd << " the fd passed at ctor must be in the [0,2] range ") ; // PRQA S 3081
    }
}


void exostream::silent() 
{ 
    Lock const lock(globalMutex()) ;
    _mode = Silent ;
} 

void exostream::verbose() 
{ 
    Lock const lock(globalMutex()) ;
    _mode = Verbose ;
} 

void exostream::formattedOnly()
{ 
    Lock const lock(globalMutex()) ;
    _mode = FormattedOnly ;
} 


void exostream::setFormatter(exMsgFmt *f) 
{ 
    Lock const lock(globalMutex()) ;
    _formatter = f ;
} 


void exostream::allowStackTrace(bool v) 
{ 
    Lock const lock(globalMutex()) ;
    _allowStackTrace = v ;
} 

void exostream::notifyDetailFromFilterable(bool v) 
{ 
    Lock const lock(globalMutex()) ;
    _notifyDetailFromFilterable = v ;
} 

exostream::Mode exostream::mode() const 
{
    Lock const lock(globalMutex()) ;
    return _mode ;
}

void exostream::unitBuffered(bool f) // PRQA S 4020
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return ; // yes: give up
    }
    
    if (f) { // true ?
        stream().setf(std::ios::unitbuf) ;
    }
    else  { // false ?
        stream().unsetf(std::ios::unitbuf) ;
    }
}

bool exostream::isUnitBuffered() // PRQA S 4020
{
    Lock const lock(globalMutex()) ;
    if (isDestroying()) {
        return false ;
    }
    std::ios_base::fmtflags ff = stream().flags() & std::ios::unitbuf ; // PRQA S 3227
    return ff != 0 ; // PRQA S 3051
}

void exostream::installFilter(exFilter *f) // PRQA S 4215
{
    Lock const lock(globalMutex()) ;
    if (f && (find(_filters.begin(), _filters.end(), f) == _filters.end())) { // PRQA S 3232, 3235
        _filters.push_back(f) ;
    }
}

void exostream::installFilters(const std::vector<exFilter *> &f) 
{
    Lock const lock(globalMutex()) ;
    for (const auto &i : f) {
        if (i && (find(_filters.begin(), _filters.end(), i) == _filters.end())) {
            _filters.push_back(i) ;
        }
    }
}


void exostream::uninstallFilter(exFilter *f) 
{
    Lock const lock(globalMutex()) ;
    if (f) { 
        const auto ftbd = find(_filters.begin(),_filters.end(), f) ;
        if (ftbd != _filters.end()) { // found: erase it 
            _filters.erase(ftbd) ;
        }
    }
}



bool exostream::applyFilter(exFilterable &s) // PRQA S 4020
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroyng ... 
        return false ; // no filter applyed: just return 
    }
    
    if (s.defaultNotify()) { // has defaultnotify ? 
        // filter it according filters (if any)
        
        for (auto i : _filters) {
            if (!i->filterInsertion(s)) { return false ; }
        }
        return true ;
    }

    return ! defaultNotify() ; 
}

bool exostream::defaultNotify() const
{
    return true ;
}

bool exostream::wouldFilter(exFilterable &s) 
{
    Lock const lock(globalMutex()) ;
    return applyFilter(s) ;
}

void exostream::installedFilters(std::vector <exFilter *> &f) const 
{
    Lock const lock(globalMutex()) ;
    f = _filters ;
}

void exostream::destroying() noexcept 
{
    setDestroying() ;

}

void exostream::setDestroying() noexcept 
{
    Lock const lock(globalMutex()) ;
    _destroying = true ;

    if (this == _defaultErrLogger) { // default cerr
        _defaultErrLogger = nullptr ;
    }
    
    if (this == _defaultOutLogger) { // the default logger for macros
        _defaultOutLogger = nullptr ;
    }

    if (this == _defaultCoutLogger) { // the default cout logger
        _defaultCoutLogger = nullptr ;
    }
}

bool exostream::isDestroying() const noexcept 
{
    Lock const lock(globalMutex()) ;
    return _destroying ;
}

// PRQA S 4222 EOF
exostream &operator<<(exostream &o, exFilterable &s) { return o.notify(s) ; }
exostream &operator<<(exostream &o, const std::exception &x) noexcept { return o.notify(x) ; }
exostream &operator<<(exostream &o, const std::string &v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, const char *v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, bool v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, char v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, short v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, int v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, long v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, float v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, double v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, long double v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, unsigned char v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, unsigned short v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, unsigned int v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, signed long long int v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, unsigned long long int v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, unsigned long v) { return o.inserter(v) ; }
exostream &operator<<(exostream &o, void *v) { return o.inserter(v) ; } // NOSONAR
exostream &operator<<(exostream &o, const void *v) { return o.inserter(v) ; } // NOSONAR


exostream &operator<<(exostream &o, std::ostream & (*func)(std::ostream &)) // NOSONAR
{ return o.inserter(func) ; }


exostream &operator<<(exostream &o, ios_base & (*func)(ios_base &)) // NOSONAR
{ return o.inserter(func) ; }


exostream & operator<<(exostream &o, std::_Resetiosflags m) { return o.inserter(m) ; }
exostream & operator<<(exostream &o, std::_Setiosflags m) { return o.inserter(m) ; }
exostream & operator<<(exostream &o, std::_Setbase m) { return o.inserter(m) ; }
exostream & operator<<(exostream &o, std::_Setfill<char> m) { return o.inserter(m) ; }
exostream & operator<<(exostream &o, std::_Setprecision m) { return o.inserter(m) ; }
exostream & operator<<(exostream &o, std::_Setw m) { return o.inserter(m) ; }

std::ostream & operator <<(std::ostream &os , exostream::Mode m) noexcept
{
    switch(m) {
    case exostream::FormattedOnly: 
        os << "FormattedOnly" ; // PRQA S 3804 
        break ;
    case exostream::Verbose: 
        os << "Verbose" ; // PRQA S 3804 
        break ;
    case exostream::Silent: 
        os << "Silent" ; // PRQA S 3804 
        break ;
    default: // do nothing 
        break ;
    }
   
    // as for each standard inserter, returns left hand arg, allowing for right associative chaining 
    return os ; // PRQA S 4028 
}

exostream & operator <<(exostream &os , exostream::Mode m) noexcept
{
    switch(m) {
    case exostream::FormattedOnly: 
        os << "FormattedOnly" ; // PRQA S 3804 
        break ;
    case exostream::Verbose: 
        os << "Verbose" ; // PRQA S 3804 
        break ;
    case exostream::Silent: 
        os << "Silent" ; // PRQA S 3804 
        break ;
    default: // do nothing 
        break ;
    }
   
    // as for each standard inserter, returns left hand arg, allowing for right associative chaining 
    return os ; // PRQA S 4028 
}



std::ostream & operator <<(std::ostream &os , exostream::StreamFdEquiv f) noexcept
{
   switch(f) {
   case exostream::Cmcf:
       os << "Cmcf" ; // PRQA S 3804 
       break ;
   case exostream::StdCout:
       os << "StdCout" ; // PRQA S 3804 
       break ;
   case exostream::StdCerr:
       os << "StdCerr" ; // PRQA S 3804 
       break ;
   case exostream::NullStream:
       os << "NullStream" ; // PRQA S 3804 
       break ;
   default: // do nothing 
       break ;
   }
   
   // as for each standard inserter, returns left hand arg, allowing for right associative chaining 
   return os ; // PRQA S 4028 
}

exostream & operator <<(exostream &os , exostream::StreamFdEquiv f) noexcept
{
   switch(f) {
   case exostream::Cmcf:
       os << "Cmcf" ; // PRQA S 3804 
       break ;
   case exostream::StdCout:
       os << "StdCout" ; // PRQA S 3804 
       break ;
   case exostream::StdCerr:
       os << "StdCerr" ; // PRQA S 3804 
       break ;
   case exostream::NullStream:
       os << "NullStream" ; // PRQA S 3804 
       break ;
   default: // do nothing 
       break ;
   }
   
   // as for each standard inserter, returns left hand arg, allowing for right associative chaining 
   return os ; // PRQA S 4028 
}


exostream &exostream::getDefaultErrLogger() { // PRQA S 4020
    Lock const lock(globalMutex()) ;
    if (! _defaultErrLogger) { return excerr ; }
    return *_defaultErrLogger ;
}

exostream *exostream::setDefaultErrLogger(exostream *s) noexcept 
{
    Lock const lock(globalMutex()) ;
    exostream *old = _defaultErrLogger ;
    _defaultErrLogger = s ; // PRQA S 2919 # by design s shall be kept alive
    CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "new: " << s << " (" << (s ? s->getName() : "nil") << ") old: " << old << " (" << (old ? old->getName() : "nil") << ")") ; // PRQA S 3380, 3385
    return old ;
}
 
exostream &exostream::getDefaultOutLogger() noexcept {  // PRQA S 4020
    Lock const lock(globalMutex()) ;
    if (! _defaultOutLogger) { return excerr ; }
    return *_defaultOutLogger ;
}

exostream *exostream::setDefaultOutLogger(exostream *s) noexcept 
{
    Lock const lock(globalMutex()) ;
    exostream *old = _defaultOutLogger ;
    _defaultOutLogger = s ; // PRQA S 2919 # by design s shall be kept alive 
    CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "new: " << s << " (" << (s ? s->getName() : "nil") << ") old: " << old << " (" << (old ? old->getName() : "nil") << ")") ; // PRQA S 3380, 3385
    return old ;
}
 
exostream &exostream::getDefaultCoutLogger() noexcept {  // PRQA S 4020
    Lock const lock(globalMutex()) ;
    if (! _defaultCoutLogger) { return excout ; }
    return *_defaultCoutLogger ;
}

exostream *exostream::setDefaultCoutLogger(exostream *s) noexcept 
{
    Lock const lock(globalMutex()) ;
    exostream *old = _defaultCoutLogger ;
    _defaultCoutLogger = s ; // PRQA S 2919 # by design s shall be kept alive
    CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "new: " << s << " (" << (s ? s->getName() : "nil") << ") old: " << old << " (" << (old ? old->getName() : "nil") << ")") ; // PRQA S 3380, 3385
    return old ;
}
 


//
//
// exLogStream
//
//
// PRQA S 2017 EOF
exLogStream::exLogStream(const std::string &n, const std::string &fname, bool appendIfExists,  const Lock & ) :
    exostream(n, &excerr),
    _fname(fname),
    _append(appendIfExists)
{
    open() ;
}

exLogStream::exLogStream(
    const std::string &n, 
    const std::string &fname, 
    exostream *parent, 
    bool appendIfExists,
    const Lock &
) :
    exostream(n, parent),
    _fname(fname),
    _append(appendIfExists)
{
    open() ;
}

exLogStream::exLogStream(
    const std::string &n, 
    const std::string &fname, 
    const std::vector<exostream *> &parents, 
    bool appendIfExists,
    const Lock & 
) :
    exostream(n, parents),
    _fname(fname),
    _append(appendIfExists)
{
    open() ;
}

exLogStream::~exLogStream() noexcept
{
    try {
        Lock const lock(globalMutex()) ; // PRQA S 4631
        close() ;
    }
    catch(const exception &e) { // NOSONAR - any exception
        cerr << explain(e) ; // NOSONAR - can't use streams here 
    }
}

streampos exLogStream::size() 
{
    Lock const lock(globalMutex()) ;
    return _logfile ? _logfile->tellp() : streampos(0) ; // PRQA S 3081, 3382, 3385
}

// this method is intentionally non-const
time_t exLogStream::creationTime() // NOSONAR - non-const - is intentional 
{
    struct stat logstat ; // PRQA S 4102
    if(0 != ::stat(_fname.c_str(), &logstat)) { // initializes logstat
        ACS_THROW(exIOException("Cannot stat file \"" + _fname + "\"",errno)) ;  // PRQA S 3081
    }
    return logstat.st_ctime ;

}


// this method is intentionally non-const
void exLogStream::setPermission(mode_t mode) // NOSONAR - non-const: technically changes 
                                             // a resource controlled by the class
{
    if(::chmod(_fname.c_str(),mode) < 0) { // can't change permissions ... 
        ostringstream os ;
        os << "Cannot chmod file \"" << _fname << "\" with \\" << 0 << oct << mode << dec << " value" ;
        ACS_THROW(exIOException(os.str(),errno)) ;  // PRQA S 3081
    }
    
}



void exLogStream::rotate(const std::string &oname, const std::string &msg)
{
    Lock const lock(globalMutex()) ;
    close() ;
    _logfile.reset() ; 
    ::rename(_fname.c_str(), oname.c_str()) ;
    open() ;
    if (!msg.empty()) {
        ACS_LOG_INFO(msg) ;
    }
}

void exLogStream::rotateOnNewFile (const std::string &newname, const std::string &msg)
{
    Lock const lock(globalMutex()) ;
    close() ;
    _logfile.reset() ; 
    _fname = newname;
    open() ;
    if (!msg.empty()) {
        ACS_LOG_INFO(msg) ;
    }
}


void exLogStream::appendIfExists (bool append)
{
    Lock const lock(globalMutex()) ;
    _append = append;
}

bool exLogStream::appendIfExists () const
{
    Lock const lock(globalMutex()) ;
    return _append;
}

void exLogStream::open()
{
    Lock const lock(globalMutex()) ;

    try {
        try { // NOSONAR - keep as separate try-block 
            bool append = _append && (0 == access(_fname.c_str(),W_OK)) ;
            // note that if the file is not writable, the append flag will be false
            // and the file open will fail, so no problem ...

            _logfile = std::make_unique<ofstream>(_fname.c_str(), // NOSONAR
                                    append ? // PRQA S 3380, 3385
                                        (std::ios::out | std::ios::app | std::ios::ate) : 
                                        (std::ios::out | std::ios::trunc)
                                  ) ;
            if (!_logfile) { // eval to false if nullptr
                silent() ; // cannot use myself
                ACS_THROW(exFileOpenException("Cannot open log as: " + _fname + " THE LOG IS USELESS.",errno)) ; // PRQA S 3081
            }
            _logfile->exceptions(ios_base::badbit | ios_base::failbit) ;
        }
        catch(const exception &e) { // NOSONAR
            _logfile.reset() ;
            silent() ; // cannot use myself
            ACS_THROW(exFileOpenException(e,"Cannot open log as: " + _fname,errno)) ; // PRQA S 3081
        }
        
        try { // NOSONAR - keep as separate try-block
            // Do not try to change permissions to the /dev/null redirected logs
            if (_fname != "/dev/null")
            {
                setPermission(acs::base::defaultLogPermissions());
            }
        }
        catch (const exception& x) // NOSONAR - any exception 
        {
            ACS_LOG_WARNING ("Unable to change log file permissions. Exception dump follows.");
            ACS_LOG_NOTIFY_EX (x);
        }
    }
    catch(const exFileOpenException &err) {
        ACS_LOG_NOTIFY_EX(err) ;
    }
    // here _logfile could be nullptr due to exceptions above
    set_stream(_logfile.get()) ; 
}

void exLogStream::close() noexcept
{
    Lock const lock(globalMutex()) ;
    set_stream(nullptr) ;
    try {
        if (_logfile) {
            if (OnExitDetector::instance()->exiting())
            {
                // Disabling exception during the global dtors
                // It is not possible for iostream to throw exceptions during global dtors
                _logfile->exceptions(ios_base::goodbit);
            }
            _logfile->flush() ; // flush 
            _logfile->close() ; // close
            _logfile->clear() ; 
        }
    }
    catch(const exception &x) { // NOSONAR
        cerr << explain(x) ; // NOSONAR - can't use streams here
    }
}

//
//
// exBaseStream
//
//
exBaseStream::exBaseStream(const std::string &n) :
    exLogStream(n, "/dev/null") 
{
}

exBaseStream::exBaseStream(const std::string &n, exostream *parent) :
    exLogStream(n, "/dev/null",parent)
{
}

exBaseStream::exBaseStream(const std::string &n, const std::vector<exostream *> &parents) :
    exLogStream(n, "/dev/null",parents) 
{
}

exBaseStream::~exBaseStream() noexcept
{
    // PRQA S 4631 exBaseStream_dtor
    try {
        Lock const lock(globalMutex()) ; 
        destroying() ;
    }
    catch(const exception &e) { // NOSONAR - any exception
        cerr << explain(e) ; // NOSONAR - can't use streams here
    }
    // PRQA L:exBaseStream_dtor
}


//
//
// exSyslog
//
//
ACS_CLASS_DEFINE_DEBUG_LEVEL(exSyslog) 

exSyslog::exSyslog(const std::string &n) :
    exBaseStream(n),
    facility_(LOG_LOCAL0) // NOSONAR - avoid to include syslog. header file
{
}

exSyslog::exSyslog(const std::string &n, exostream *parent) :
    exBaseStream(n,parent),
    facility_(LOG_LOCAL0) // NOSONAR - avoid to include syslog. header file
{
}

exSyslog::exSyslog(const std::string &n, const std::vector<exostream *> &parents) :
    exBaseStream(n,parents),
    facility_(LOG_LOCAL0) // NOSONAR - avoid to include syslog. header file
{
}

exSyslog::~exSyslog() noexcept
{
    // PRQA S 4631 exSyslog_dtor
    try {
        // no exception from here
        Lock const lock(globalMutex()) ; 

        destroying() ;
        ::closelog (); 
    }
    catch(const exception &e) { // NOSONAR - any exception 
        cerr << explain(e) ; // NOSONAR - can't use streams here
    }
    // PRQA L:exSyslog_dtor
}
/*
void exSyslog::open()
{
    CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "facility: " << facility_) ;
    openlog ("", LOG_CONS | LOG_PID | LOG_NDELAY, facility_);
}
*/
namespace {
	int get_prio(const exFilterable &s) noexcept // PRQA S 4020
	{
	    // PRQA S 3081 L3
	    if (dynamic_cast<const exFOpAlert *>(&s)) { return LOG_NOTICE ; }
	    if (dynamic_cast<const exFEvent *>(&s)) { return LOG_NOTICE ; }
	    if (dynamic_cast<const exFEStatus *>(&s)) { return LOG_ERR ; } // NB inherits from exFError 
	    if (dynamic_cast<const exFWStatus *>(&s)) { return LOG_WARNING ; } // NB inherits from exFWarning
	    if (dynamic_cast<const exFStatus *>(&s)) { return LOG_NOTICE ; }
	    if (dynamic_cast<const exFWarning *>(&s)) { return LOG_WARNING ; }
	    if (dynamic_cast<const exFError *>(&s)) { return LOG_ERR ; }
	    if (dynamic_cast<const exFDebug *>(&s)) { return LOG_DEBUG ; }
	    
	    return LOG_INFO ; 
	    // PRQA L:L3
	}
}

void exSyslog::notifyFilterable(exFilterable &s) // PRQA S 4020
{
    Lock const lock(globalMutex()) ;

    if (isDestroying()) { // am i destroying ?
        return ; // give up
    }

    if (mode() != Silent) { // am i  verbose or silent ?
        CERR_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "facility: " << facility_) ;
        syslog (facility_ | get_prio(s), "%s", s.message().c_str()); // PRQA S 3003
    }  
}

//
//
// exLogStringStream
//
//
exLogStringStream::exLogStringStream(const std::string &n) : exBaseStream(n), _stream()
{
    set_stream(&_stream) ;
}

exLogStringStream::exLogStringStream(const std::string &n, exostream *parent) : exBaseStream(n, parent), _stream()
{
}

exLogStringStream::exLogStringStream(const std::string &n, const std::vector<exostream *> &parents) : exBaseStream(n, parents), _stream()
{
}

exLogStringStream::~exLogStringStream() noexcept
{
    set_stream(nullptr) ;
}

streampos exLogStringStream::size() 
{
    Lock const lock(globalMutex()) ;
    return _stream.str().length() ; // PRQA S 3000
}

const std::ostringstream &exLogStringStream::str() const // PRQA S 4120
{
    return _stream ;
}



//
//
// exHandler
//
//
exHandler::exHandler(const std::string &n) :
    exBaseStream(n)
{
}

exHandler::exHandler(const std::string &n, exostream *parent) :
    exBaseStream(n,parent)
{
}

exHandler::exHandler(const std::string &n, const std::vector<exostream *> &parents) :
    exBaseStream(n,parents)
{
}

exHandler::~exHandler() noexcept
{
    // PRQA S 4631 exHandler_dtor
    try {
        Lock const lock(globalMutex()) ; // PRQA S 4631
        destroying() ;
    }
    catch(const exception &e) { // NOSONAR - any exception
        cerr << explain(e) ; // NOSONAR - can't use streams here
    }
    // PRQA L:exHandler_dtor
}


bool exHandler::notifyException(const std::exception &x) noexcept
{
    auto ex = dynamic_cast<const exException *>(&x) ; // PRQA S 3081
    if (ex) { // is an exException ?
        ACS_LOG_INFO("acs::exException handled: " << ex->type()) ;
    }
    else { // or just its base class ?
        ACS_LOG_INFO("std::exception handled") ;
    }
    return true ; // return it was handled
}


_ACS_END_NAMESPACE
