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
    Revision 5.7  2018/09/12 10:05:39  marpas
    exception's stack trace management revived into streams.
    tested
    
    Revision 5.6  2014/02/07 18:03:07  marpas
    introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
    documentation improved and tests aligned
    
    Revision 5.5  2013/12/05 18:22:20  marpas
    fixing strerror_r call (different semantics for posix and gnu ones having same signature)
    
    Revision 5.4  2013/11/15 13:19:30  marpas
    introducing the isFilterable concept into an exostream.
    It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
    
    Revision 5.3  2013/10/02 18:08:28  marpas
    uninitialized memory fixed
    
    Revision 5.2  2013/09/26 18:09:35  marpas
    robustness strongly improved:
    singleton functions (with static variables inside) used
    some private members are now accessed only through the above technique
    exRTDebuggable classes do not longer unregster themselves from exRTDebug.
    it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
    The above changes will lead in a more efficient process closure.
    moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
    
    Revision 5.1  2013/06/06 08:32:09  marpas
    notify method now uses default stream instead of exstd::cerr
    
    Revision 5.0  2013/06/04 15:35:00  marpas
    introducing new log macros, debug macros
    work in progress
    
    Revision 4.9  2013/05/02 17:49:27  marpas
    qa warning fixed
    
    Revision 4.8  2012/12/20 13:25:44  marpas
    coding best practices
    explain used instead of what()
    tests improved
    
    Revision 4.7  2012/12/17 16:43:27  marpas
    Guard pattern included and released
    
    Revision 4.6  2012/06/25 17:42:05  marpas
    qacpp instrumentation
    
    Revision 4.5  2012/06/14 17:16:41  marpas
    qacpp instrumentation
    
    Revision 4.4  2012/05/30 16:20:50  marpas
    S3PDGS-741 - exception's format depends on exMsgFmt
    
    Revision 4.3  2012/02/20 15:04:03  marpas
    refactoring
    
    Revision 4.2  2012/02/08 16:20:38  marpas
    deprecated exDEFINE_EXCEPTION macro
    
    Revision 4.1  2012/01/31 14:13:04  marpas
    getting rid of boost and compilation warning reduced
    
    Revision 4.0  2011/10/18 11:21:59  marpas
    new statistics
    
    Revision 3.8  2011/05/11 15:46:44  marpas
    stacks in exception are chained
    robustness improved
    
    Revision 3.7  2011/03/31 13:04:48  enrcar
    EC:: threadId in exception reported around square (instead for curly) brackets, for uniformity.
    
    Revision 3.6  2011/03/30 10:59:07  marpas
    in case of exLogStream accessing problem, an exception is raised and notified (but on the log itself).
    
    Revision 3.5  2011/03/18 13:30:22  marpas
    minor changes
    
    Revision 3.4  2011/03/01 11:07:03  marpas
    GCC 4.4.x support
    
    Revision 3.3  2010/11/12 13:12:29  micmaz
    added the errno
    
    Revision 3.2  2010/09/13 16:02:37  marpas
    smaller exException interface
    stack format changed
    
    Revision 3.1  2010/09/13 13:26:47  marpas
    aligned to new exStackTrace interface
    function name reported into exception first what line
    
    Revision 3.0  2009/10/08 10:48:08  marpas
    Statistics class added
    rules application improved
    
    Revision 2.5  2009/03/02 17:02:39  marpas
    CODECHECK parsing - no regressions
    
    Revision 2.4  2008/02/26 16:52:27  marpas
    what() method fixed (now description is present again)
    
    Revision 2.3  2008/02/15 16:58:53  marpas
    each stream can have its own formatter setFormatter method added
    exception::what() has no longer the stack if enabled
    exception::stack() returns the stack (if enabled)
    exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
    Filterables now hase details, they can be set via exFilterable::detail(...) method
    Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
    exception inserted into a Filterable will fill its detail with their stack (if any)
    
    Revision 2.2  2007/11/22 11:13:21  marpas
    Stack trace feature added - enabled via exStackTrace singleton
    
    Revision 2.1  2006/02/28 08:24:36  marpas
    new release, exException inherits from std::exception
    
    Revision 1.15  2005/10/20 17:41:33  marpas
    newHandler singleton is not release at exit
    
    Revision 1.14  2004/04/02 15:54:44  marpas
    Legacy code compilation fixed
    
    Revision 1.13  2004/04/02 14:15:14  marpas
    ThreadSafe class introduced and used inside
    
    Revision 1.12  2003/08/06 23:10:31  marpas
    ispeed improved
    
    Revision 1.11  2003/08/06 14:56:56  marpas
    string exException::text() method added
    
    Revision 1.10  2003/07/17 10:16:36  marpas
    thread name and thread id concept added to exceptions
    new clone method to set threads data
    exception text slightly changed and reduced in size
    
    Revision 1.9  2003/07/10 14:41:46  marpas
    new CriticalException
    
    Revision 1.8  2003/07/08 23:32:09  marpas
    exException::clone method implemented
    
    Revision 1.7  2003/04/30 09:57:38  marpas
    using namespace std was removed from includes
    
    Revision 1.6  2003/04/29 09:33:20  marpas
    new handler installed automatically
    
    Revision 1.5  2003/04/17 13:42:41  marpas
    new_handler implementation almost complete
    
    Revision 1.4  2003/04/15 17:12:09  marpas
    GCC3.2
    
    Revision 1.3  2003/03/11 17:31:54  marpas
    slightly improved, exFEvent has a new constructor
    
    Revision 1.2  2003/01/16 11:21:27  marpas
    exThreadException added as base exception class
    
    Revision 1.1.1.1  2002/09/30 19:36:15  marpas
    Import libException
    

*/

#include <exException.h>
#include <exStackTrace.h>
#include <exMsgFmtFactory.h>
#include <cstring>
#include <typeinfo>


_ACS_BEGIN_NAMESPACE(acs)


// class exWrapUnexpected

exWrapUnexpected::exWrapUnexpected(Unexpected f) : _oldHandler()
{ 
    _oldHandler = std::set_unexpected(f) ; 
}
        
exWrapUnexpected::~exWrapUnexpected() ACS_NOEXCEPT 
{ 
    std::set_unexpected(_oldHandler) ; 
}


// class exWrapTerminate

exWrapTerminate::exWrapTerminate(Terminate f) : _oldHandler()
{ 
    _oldHandler = std::set_terminate(f) ; 
}

exWrapTerminate::~exWrapTerminate() ACS_NOEXCEPT 
{ 
    std::set_unexpected(_oldHandler) ; 
}

namespace {
    std::string str_error_r(error_t eno) ACS_NOEXCEPT // PRQA S 4020
    {
        char errs[1024] ; // PRQA S 4403
        errs[0] = 0 ;
# if defined __USE_XOPEN2K && !defined __USE_GNU
        int ret = strerror_r(eno, errs, sizeof(errs)) ;
        if (0 == ret) {
            return errs ;
        }
# else
        char *ret = strerror_r(eno, errs, sizeof(errs)) ;
        if (ret) {
            return ret ; 
        }
# endif
        else {
            return "<wrong strerror_r call>" ;
        }
    }
}

// class exException

exException::exException(error_t eno) ACS_NOEXCEPT :
    exception(),
    _descr(),
    _file(),
    _line(0),
    _function(),
    _thread(),
    _threadId(pthread_self()),
    _text(),
    _stack(),
    _errno(eno),
    _causedBy()
{
    _stack = exStackTrace::getStack() ;
}

exException::exException(const std::exception &x, error_t eno) ACS_NOEXCEPT :
    exception(),
    _descr(),
    _file(),
    _line(0),
    _function(),
    _thread(),
    _threadId(pthread_self()),
    _text(),
    _stack(),
    _errno(eno),
    _causedBy(x.what())
{
    try {
        // get the stack trace 
        _stack = exStackTrace::getStack() ;
        const exException *xx_ = dynamic_cast<const exException *>(&x) ; // PRQA S 3081
        if (xx_) { 
            _stack += "\n\tthrown by:\n\t" + xx_->_stack ;
        }
    }
    catch(exception &azz_) {
        std::cerr << explain(azz_) ;
    }
}

exException::exException(const std::string &description, error_t eno) ACS_NOEXCEPT :
    exception(),
    _descr(),
    _file(),
    _line(0),
    _function(),
    _thread(),
    _threadId(pthread_self()),
    _text(),
    _stack(),
    _errno(eno),
    _causedBy()
{
    try {
        _descr << description;

        // get the stack trace 
        _stack = exStackTrace::getStack() ;
    }
    catch(exception &azz_) {
        std::cerr << explain(azz_) ;
    }
}

exException::exException(const std::exception &x, const std::string &description, error_t eno) ACS_NOEXCEPT :
    exception(),
    _descr(),
    _file(),
    _line(0),
    _function(),
    _thread(),
    _threadId(pthread_self()),
    _text(),
    _stack(),
    _errno(eno),
    _causedBy(x.what())
{
    try {
        _descr << description;

        // get the stack trace
        _stack = exStackTrace::getStack() ;
        const exException *xx_ = dynamic_cast<const exException *>(&x) ; // PRQA S 3081
        if (xx_) {
            _stack += "\n\tthrown by:\n\t" + xx_->_stack ;
        }
    }
    catch(exception &azz_) {
        std::cerr << explain(azz_) ;
    }
}

exException::~exException() ACS_NOEXCEPT
{
}



exException::exException(const exException &x) ACS_NOEXCEPT :
    exception(x),
    _descr(),
    _file(x._file),
    _line(x._line),
    _function(x._function),
    _thread(x._thread),
    _threadId(x._threadId),
    _text(x._text),
    _stack(x._stack),
    _errno(x._errno),
    _causedBy(x._causedBy)
{
    _descr << x._descr.str();
}

exException::exException(exException &&x) ACS_NOEXCEPT :
    exception(std::move(x)),
    _descr(std::move(x._descr)),
    _file(std::move(x._file)),
    _line(std::move(x._line)),
    _function(std::move(x._function)),
    _thread(std::move(x._thread)),
    _threadId(std::move(x._threadId)),
    _text(std::move(x._text)),
    _stack(std::move(x._stack)),
    _errno(std::move(x._errno)),
    _causedBy(std::move(x._causedBy))
{
}


void exException::debugInfo(std::string const& file, std::string const& function, int line) const ACS_NOEXCEPT {
    try { _file = file;} catch (exception &e) { std::cerr << explain(e) ; }
    try { _function = function;} catch (exception &e) { std::cerr << explain(e) ; }
    try { _line = line;} catch (exception &e) { std::cerr << explain(e) ; }
}


void exException::notify() const // PRQA S 4214
{
    ACS_LOG_NOTIFY_EX(*this) ;
}

std::string exException::type() const 
{
    return exStackTrace::demangle(typeid(*this).name()) ;
}



void exException::description(const std::string &d) // PRQA S 4121
{
    _descr.str("");
    _descr << d ;
}

std::exception *exException::clone(const std::string &threadName) const
{
    std::exception *cloned = clone() ; // clone it
    
    exException *x = dynamic_cast<exException *>(cloned) ; // PRQA S 3081
    if (x) { // is an exException
        x->_thread = threadName ; // attach the info about current thread
        x->_threadId = pthread_self() ;
    }
    return cloned ; // return it 
}

const char* exException::what() const ACS_NOEXCEPT
{
    try {
        // return the what string 
        std::ostringstream os ;
        os << type() << " in " << file() << "(" << line() << ") from " << _function;
        // exception in file.C:123

        if (!_thread.empty() || (0 != _threadId) ) { // has thread info ?
            os << " thread \"" << thread() << "\" [" << _threadId << "]" ;
        }
        
        os << "\n\t" << computeDescr() ; // attach the description
        _text = os.str() ; // fill up the text
    }
    catch(exception &azz_) {
        std::cerr << explain(azz_) ;
    }
    
    return _text.c_str() ; // and return it as const char *
}



std::string exException::computeDescr() const ACS_NOEXCEPT
{
    try
    {
        std::ostringstream os;
        os << _descr.str();
        if (0 != _errno) {
            os << " \"[" <<  _errno << "]: " << str_error_r(_errno) <<  "\"" ; // PRQA S 3081
        }

        if (!_causedBy.empty()) { // prepend (in case) the "caused by"
            os << "\n\tcaused by:\n\t" + _causedBy ;
        }

        return os.str();
    }
    catch(exception &azz_) {
        std::cerr << explain(azz_) ;
    }

    return "";
}


std::string toString(const std::exception &x, bool stack) 
{
    return exMsgFmtFactory::instance()->text(x, stack) ;
}

namespace {
_ExplainEx explain_internal(const std::exception &e_, bool s_) ACS_NOEXCEPT
{ 
  _ExplainEx __x; 
  __x._M_s.reserve(256) ; // PRQA S 4400
  __x._M_s = e_.what() ; // \n added at the end

  const exException *x_ = 0 ;
  
  if (s_) {
      try {
          x_ = dynamic_cast<const exException *>(&e_) ; // PRQA S 3081
      }
      catch(std::bad_cast &) {
          x_ = 0 ;
      }
      if (x_ && not x_->stack().empty()) {
        __x._M_s += "\nstack trace\n" +  x_->stack() ;
      }
  }

  return __x ; 
  // note no endline 
}

}

_ExplainEx explain(const std::exception &e_) ACS_NOEXCEPT
{ 
    return explain_internal(e_, true) ;
}

_ExplainEx explain(const std::exception &e_, bool stack_) ACS_NOEXCEPT
{ 
    return explain_internal(e_, stack_) ;
}

std::ostream &operator<<(std::ostream &s_, const _ExplainEx &e_) ACS_NOEXCEPT
{
    return s_ << e_._M_s << std::endl ; // want to flush !
}

_ACS_END_NAMESPACE
