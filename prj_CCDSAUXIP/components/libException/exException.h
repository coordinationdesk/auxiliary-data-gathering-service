/*

    Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

    $Prod: A.C.S. Exception Library $

    $Id$

    $Author$

    $Log$
    Revision 5.8  2018/10/04 16:18:35  marpas
    introducing noexcept that will be translated into throw() or noexecpt depending on the compiler version
    
    Revision 5.7  2018/09/27 13:23:16  marpas
    compilation warning fixed
    
    Revision 5.6  2018/09/12 10:05:39  marpas
    exception's stack trace management revived into streams.
    tested
    
    Revision 5.5  2014/03/26 19:13:13  marpas
    qa gives errors sometimes - silenced with ifdef
    
    Revision 5.4  2014/02/07 18:03:07  marpas
    introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
    documentation improved and tests aligned
    
    Revision 5.3  2014/01/22 17:22:10  marpas
    -DSILENT_MACROS_DEPRECATED_WARNING can be used to silent exDEFINE_EXCEPTION_MACRO warnings too
    
    Revision 5.2  2013/11/15 13:19:30  marpas
    introducing the isFilterable concept into an exostream.
    It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
    
    Revision 5.1  2013/06/04 17:23:46  marpas
    introducing ne RTDebuggable template
    
    Revision 5.0  2013/06/04 15:35:00  marpas
    introducing new log macros, debug macros
    work in progress
    
    Revision 4.17  2013/05/14 17:58:44  marpas
    exSystemException added
    
    Revision 4.16  2013/04/09 07:47:38  marpas
    documentation improved
    
    Revision 4.15  2012/12/20 13:25:44  marpas
    coding best practices
    explain used instead of what()
    tests improved
    
    Revision 4.14  2012/12/17 16:43:27  marpas
    Guard pattern included and released
    
    Revision 4.13  2012/11/15 11:00:54  marpas
    qa rules enforced
    
    Revision 4.12  2012/11/14 18:15:55  marpas
    qa rules enforced
    
    Revision 4.11  2012/08/02 16:37:00  marpas
    new exception defined:exNotImplemented
    
    Revision 4.10  2012/07/24 09:57:17  marpas
    qacpp warnings
    
    Revision 4.9  2012/06/25 17:42:05  marpas
    qacpp instrumentation
    
    Revision 4.8  2012/06/22 13:41:47  marpas
    pr qa instrumentation
    
    Revision 4.7  2012/06/14 17:16:41  marpas
    qacpp instrumentation
    
    Revision 4.6  2012/06/12 17:37:00  marpas
    qacpp instrumentation
    
    Revision 4.5  2012/06/06 17:12:20  marpas
    qa rules application in progress
    
    Revision 4.4  2012/06/05 17:07:00  marpas
    resolving some quality issues and documentation
    
    Revision 4.3  2012/05/30 16:20:51  marpas
    S3PDGS-741 - exception's format depends on exMsgFmt
    
    Revision 4.2  2012/02/20 15:04:03  marpas
    refactoring
    
    Revision 4.1  2012/02/08 16:20:38  marpas
    deprecated exDEFINE_EXCEPTION macro
    
    Revision 4.0  2011/10/18 11:21:59  marpas
    new statistics
    
    Revision 3.9  2011/09/29 15:32:22  marpas
    minor change
    
    Revision 3.8  2011/09/28 14:31:25  marpas
    exDECLARE_EXCEPTION macro changed when instrumented with codecheck
    
    Revision 3.7  2011/09/08 09:17:01  marpas
    code intrumented for CODECHECK
    
    Revision 3.6  2011/06/06 16:38:56  marpas
    hidden base method fixed
    
    Revision 3.5  2011/01/24 12:57:43  marpas
    exDECLARE_EXCEPTION macros modified (inlined) in order to provide convenience into templates too.
    As a consequence, the exDEFINE_EXCEPTION macro is a no-op and its use is deprecated (will be removed in the PDS_2 stream)
    The above change has impact on (at least) the following libraries:
    libAbstractFactoryChain
    libBroadcast
    libIpcBase
    libStatusMgr
    libTclFtp
    
    Revision 3.4  2011/01/20 19:21:27  marpas
    getErrno function implemented
    
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
    
    Revision 2.5  2009/03/11 18:09:35  marpas
    operator= in exceptions is private
    
    Revision 2.4  2009/03/11 17:09:02  marpas
    c++rules violation removed
    
    Revision 2.3  2008/02/15 16:58:53  marpas
    each stream can have its own formatter setFormatter method added
    exception::what() has no longer the stack if enabled
    exception::stack() returns the stack (if enabled)
    exception stack are no longer notified if present unless the stream is instructed to do that via notifyStackFromEx method
    Filterables now has details, they can be set via exFilterable::detail(...) method
    Filterables details are notified by the stream having the appropriate flag set via notifyDetailFromFilterable method
    exception inserted into a Filterable will fill its detail with their stack (if any)
    
    Revision 2.2  2007/11/22 11:13:21  marpas
    Stack trace feature added - enabled via exStackTrace singleton
    
    Revision 2.1  2006/02/28 08:24:36  marpas
    new release, exException inherits from std::exception
    
    Revision 1.16  2005/03/18 17:01:34  marpas
    comment improved
    
    Revision 1.15  2004/04/14 09:05:09  marpas
    many changes
    
    Revision 1.14  2004/04/02 15:54:44  marpas
    Legacy code compilation fixed
    
    Revision 1.13  2004/04/02 14:15:14  marpas
    ThreadSafe class introduced and used inside
    
    Revision 1.12  2003/08/06 14:56:56  marpas
    string exException::text() method added
    
    Revision 1.11  2003/07/17 10:16:36  marpas
    thread name and thread id concept added to exceptions
    new clone method to set threads data
    exception text slightly changed and reduced in size
    
    Revision 1.10  2003/07/16 13:09:03  marpas
    throwConditionedException macro defined
    
    Revision 1.9  2003/07/10 14:41:46  marpas
    new CriticalException
    
    Revision 1.8  2003/07/08 23:32:09  marpas
    exException::clone method implemented
    
    Revision 1.7  2003/04/30 09:57:38  marpas
    using namespace std was removed from includes
    
    Revision 1.6  2003/04/29 09:33:20  marpas
    new handler installed automatically
    
    Revision 1.5  2003/04/18 08:36:55  marpas
    example code improved
    
    Revision 1.4  2003/04/17 13:42:41  marpas
    new_handler implementation almost complete
    
    Revision 1.3  2003/01/16 11:21:27  marpas
    exThreadException added as base exception class
    
    Revision 1.2  2002/11/04 11:36:10  paoscu
    Little changes in doxygen documentation.
    
    Revision 1.1.1.1  2002/09/30 19:36:15  marpas
    Import libException
    

*/


// MP DO NOT MOVE THE FOLLOWING INCLUDE INSIDE THE DEFINITION
// OF _exException_H_
// THIS IS BECAUSE ALL THE LEGACY CODE INCLUDES exException.h AND
// EXPECTS TO HAVE excerr DEFINED !!!
// 
// THE CORRECT WAY IS TO INCLUDE exStream.h TO HAVE excerr DEFINED
// AND DO NOT RELY ON exException.h
//

#include <acs_c++config.hpp>

#ifndef DOXYGENPARSE
#ifndef _ThreadSafe_H_
#include <exStream.h>
#endif
#endif

#ifndef _exException_H_
#define _exException_H_ 

#include <exMacros.h>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <cerrno>
#include <sstream>

_ACS_BEGIN_NAMESPACE(acs)

/*! \addtogroup macro_exception 
 *  @{
 */
/*!	\brief convenience macro to throw exceptions

    \def ACS_THROW(x)

    This macro should be used to throw exceptions.
    It provides a simple way to assign line and file
    to the exceptions thrown ( invokes debugInfo() )
    e.g.
    
    \code

    // ...

    if (i >= rings()) { // asking for an un-existing ring number
        ostringstream os ;
        os << "dbGeoObject::ring " << i << " greater than " << rings()-1 ; // create the message

        ACS_THROW(acs::exBoundsException(string(os.str()))) ; // set debug info and throws the exception
    }

    // ...
    
    \endcode

    It is also possible to compose the message using the
    exception operator<<().
    e.g.
    
    \code

    // ...

    if (i >= rings()) { // asking for an un-existing ring number
        ACS_THROW(acs::exBoundsException() << "dbGeoObject::ring " << i << " greater than " << rings()-1) ; // create the message, set debug info and throws the exception
    }

    // ...
    
    \endcode
    
    \see exException::debugInfo(), exException::line(), exException::file()
*/

/*! @}*/

/*! \brief internal convenience function 
    
    The function allows to write code like this:

    \code

    // ...
    
    ACS_THROW(FileOpenException(string("Cannot open file: ") + filename) ;
    
    // ...
    
    \endcode
    
    The thrown object will be fully described with file and line number.
    The function is internally called from ACS_THROW macro.
    
*/
// PRQA S 2134 3
template <typename T>
inline 
		
void __attribute__((noreturn)) exceptionThrower(T const& t, std::string const& file, std::string const& function,int line) // NOSONAR
{
    t.debugInfo(file, function , line) ;
    throw t ;
}

/*! \addtogroup macro_exception 
 *  @{
 */

/*!	\brief convenience macro to throw exceptions conditioned to an espression

    \def ACS_THROW(x)

    This macro should be used to throw exceptions.

    In case the macro is invoked as last statement of functions returning non void, 
    the compiler must know there is a throw of an exception here, this is the reason of the *fake* 
    throw std::exception() ;  as last statement of the macro.
    This will never be invoked as the exceptionThrower(x) throws x.

    \note this macro is needed to invoke the exceptionThrower with __FILE__, __PRETTY_FUNCTION__ 
    and __LINE__ of the point where the ACS_THROW is *really* called, so
    it is necessary to define a macro and not an inline template function.
*/
#define ACS_THROW(x)  { exceptionThrower((x),__FILE__,__PRETTY_FUNCTION__,__LINE__) ; } 



/*!	\brief convenience macro to throw exceptions conditioned to an espression

    \def ACS_COND_THROW(x)

    This macro should be used to throw exceptions if the given expression is true.
    It calls ACS_THROW macro defined in this file.
    Because it needs to 

    \code

    // ...

    ACS_COND_THROW(i >= rings(),acs::exBoundsException() << "dbGeoObject::ring " << i << " greater than " << rings()-1) ; // asking for an un-existing ring number
    // ...
    
    \endcode
    
    \see acs::exException::debugInfo(), acs::exException::line(), acs::exException::file()
*/
// PRQA S 1020,1023 L3
#define ACS_COND_THROW(c,x)  { if (c) { ACS_THROW(x) ; } }

#ifndef throwException_compat

/*!	\brief Deprecated

    \def throwException(x)
    \deprecated use ACS_THROW(x) instead
*/
#define throwException(x) _Pragma ("GCC warning \"'throwException' macro is deprecated. Use ACS_THROW\"") ACS_THROW(x) 

/*!	\brief Deprecated

    \def throwConditionedException(c,x)
    \deprecated use ACS_COND_THROW(c,x) instead
*/
#define throwConditionedException(c,x) _Pragma ("GCC warning \"'throwConditionedException' macro is deprecated. Use ACS_COND_THROW\"") ACS_COND_THROW(c,x) 
#endif
// PRQA L:L3

/*! @}*/



/*! \class exException

    \brief Base class for exceptions
    
    This class is the base class for all exceptions. 
    \see exException.h for some specialized implementation 
    
    the virtual inheritance is harmless
*/
class exException: public std::exception { // PRQA S 2109, 2153
public:

    /*! \brief constructor

        Build an exception
        if the errno passed is different from 0, adds the associated description
        as in strerror(3C) manual page.
    */
    explicit exException(error_t eno=0) noexcept ; // PRQA S 4634

    /*! \brief constructor

        Build an exception based on a previous one
        if the errno passed is different from 0, adds the associated description
        as in strerror(3C) manual page.
    */
    exException(const std::exception &, error_t eno=0) noexcept ;

    /*! \brief constructor

        Build an exception with the given description
        if the errno passed is different from 0, adds the associated description
        as in strerror(3C) manual page.
    */
    explicit exException(const std::string &description, error_t eno=0) noexcept ; // PRQA S 4634

    /*! \brief constructor

        Build an exception based on a previous one with the given description;
        if the errno passed is different from 0, adds the associated description
        as in strerror(3C) manual page.
    */
    exException(const std::exception &,const std::string &, error_t eno=0) noexcept ;

    /*! \brief destructor

        The destructor is virtual as this class is designed to be inherited
    */
    ~exException() override = default ;
    
    /*! \brief copy ctor
    
        Copies the exception
    */
    exException(const exException&) noexcept ;
    
    /*! \brief move ctor
    
        Moves the exception
    */
    exException(exException &&) noexcept ;
        
	exException &operator=(const exException &) noexcept = delete ;
    exException &operator=(exException &&) noexcept = delete ; 

   /*! \brief set description
    
        \param d the description to be set in the object
        
        Set the description in the object.
    */
    void description(const std::string &d) ;
    
    /*! \brief get description
        
        \returns the description as string
    */
    std::string description() const noexcept { return computeDescr() ; }


    /*! \brief get file
    
        Get the file where the exception was thrown.
        
        \see exException::debugInfo() and ACS_THROW for further details
    
        \returns the file where the exception was thrown\
    */
    const std::string &file() const noexcept { return _file ; }

    /*! \brief get function name

     Get the function name where the exception was thrown.

     \see exException::debugInfo() and ACS_THROW for further details

     \returns the file where the exception was thrown\
    */
    std::string const& function() const noexcept { return _function ; } 

    /*! \brief get thread name
    
        Get the thread name who cloned the exception.
        
        \see exException::clone() and exException::clone(const std::string &) for further details
    
        \returns the thread name where the exception was cloned\
    */
    const std::string &thread() const noexcept { return _thread ; } 

    /*! \brief get threadId 
    
        Get the thread id who cloned the exception. The information is significant, only if 
        the thread() method returns a non empty string, meaning the exception was cloned
        
        \see exException::clone() and exException::clone(const std::string &) for further details
    
        \returns the thread id where the exception was cloned\
    */
    pthread_t threadId() const noexcept { return _threadId ; }  


    /*! \brief get line number
    
        Get the line number in the file where the exception was thrown.
        
        \see exException::debugInfo() and ACS_THROW for further details
    
        \returns the line number where the exception was thrown
    */
    int line() const noexcept { return _line ; }

    /*! \brief assigns debug informations
    
        Assigns _line and _file to the object.
    
        \param file should be the file where the exception was originally thrown
        \param line should be the line where the exception was originally thrown
        \param function should be the function where the exception was originally thrown
        \see ACS_THROW macro for further details
    */
    virtual void debugInfo(const std::string &file, const std::string &function, int line) const noexcept ;
    
    /*! \brief type info
    
        \returns the exception type text
        
        This virtual function must be reimplemented in each derived class,
        even in the case that exException is not the direct base.
    */
    std::string type() const ; 
    
    /*! \brief notifying facility
    
        The method calls excerr.notify()
        \see exStream_withassign::notify() for further detail
    */
    void notify() const ;
    
    /*! \brief clone an exception
    
        The method must be reimplemented in derived classes. Note that the memory
        returned shall be freed by the caller.
    */
    virtual std::exception *clone() const = 0 ;         // PRQA S 2502, 2504

    /*! \brief clone an exception assigning a thread name for further references.
    
        The method assign the thread name to the cloned exception. It subsequently calls
        the abstract method clone and then assign the string
    */
    virtual std::exception *clone(const std::string &threadName) const ; // PRQA S 2502, 2504


    /*! \brief reimplemented from std::exception.
    
        Returns a C-style character string describing the general cause
        of the current error.
    */
    const char* what() const noexcept override ;
    
    virtual const std::string &stack() const noexcept { return _stack ; } // PRQA S 2131
    
    virtual error_t errorNumber() const noexcept { return _errno ; } // PRQA S 2131

protected:
    /*! \brief append parts to the description
    
        Allowing the exception description to be filled as if it were a std::ostringstream.

        Since it returns the exception itself, it can be used inside ACS_THROW and ACS_COND_THROW macros
            
        \returns a reference of the exception
    */
    template<class DescriptionType> exException &operator<<(DescriptionType t) { _descr << t; return *this; };

private:
    std::string computeDescr() const noexcept;

private: // NOSONAR - for clarity

    std::ostringstream _descr = std::ostringstream() ;
    mutable std::string _file = "" ;
    mutable int _line = 0 ;
    mutable std::string _function = "" ;
    std::string _thread = "" ;
    pthread_t _threadId ;
    mutable std::string _text = "" ;
    std::string _stack = "" ;
    error_t _errno;
    std::string _causedBy = "" ;
} ;



/*! \brief extracts errno from an exception (if any)
 *
 * returns error_t(-1) if the errno can't be extracted from the exception
 */
// PRQA S 2134 2
inline error_t getErrno(const std::exception *e) 
{
    auto ex = dynamic_cast<const exException *>(e) ; // PRQA S 3081
    return ex ? ex->errorNumber() : error_t(-1) ;                // PRQA S 3081, 3099, 3382
}

/*! \brief extracts errno from an exception (if any)
 *
 * returns -1 if the errno can't be extracted from the exception
 */
// PRQA S 2134 2
inline error_t getErrno(const std::exception &e) 
{
    return getErrno(&e) ;
}

/*! @}*/
  
  

/*! 
 * \brief std::exception and derived classes manipulator inserter 
 *
 * \see explain
 */
std::string toString(const std::exception &, bool stack=true) ;



/*! \addtogroup macro_exception 
 *  @{
 */
 
/*!	\brief exception class declaration convenience macro

    \def exDECLARE_EXCEPTION(Derived,Base)

    The macro can be used to declare an exception class into an header file.
    
    example:
    
    \code
    
    exDECLARE_EXCEPTION(MyClassException,BaseException) ;
    
    \endcode
    
    would result in a 
    
    
    \code
    
        class MyClassException : public BaseException 
        { 
            public: 
                explicit MyClassException(const std::string &description=std::string(""), error_t eno=0) noexcept ;
                explicit MyClassException(const std::exception &,const std::string &, error_t eno=0) noexcept ;
                MyClassException(const MyClassException &) noexcept 
                MyClassException(MyClassException &&) noexcept 
                virtual ~MyClassException() noexcept ;
                virtual std::exception *clone() const ;
            private: 
                explicit Derived() noexcept ; 
                Derived &operator=(const Derived &) noexcept ;
        }
    
    \endcode
    
    
*/
// PRQA S 1020,1023,1030 L1
#define exDECLARE_EXCEPTION(Derived,Base) \
        class Derived : public Base \
        { \
            public: \
\
                explicit Derived(error_t eno=0) noexcept : /* PRQA S 4634 */ \
                    Base(eno) \
                    { } \
\
                explicit Derived(const std::exception &e, error_t eno=0) noexcept : \
                    Base(e,eno) \
                    { } \
\
                explicit Derived(const std::string &d, error_t eno=0) noexcept : /* PRQA S 4634 */ \
                    Base(d, eno) \
                    { } \
\
                Derived(const std::exception &e, const std::string &d, error_t eno=0) noexcept : \
                    Base(e,d,eno) \
                    { } \
\
                Derived(const Derived &e) noexcept : \
                    Base(e) \
                    { } \
\
                Derived(Derived &&e) noexcept : \
                    Base(std::move(e)) \
                    { } \
\
                virtual ~Derived() noexcept \
                    { } \
\
                virtual std::exception *clone() const \
                    { return new Derived(*this) ; }  \
\
                virtual std::exception *clone(const std::string &s) const \
                    { return Base::clone(s) ; } \
\
                template<class DescriptionType> Derived &operator<<(DescriptionType t) { Base::operator<<(t); return *this; }; \
            private: /* copy ctor and assignment both declared but not implemented to prevent their use */ \
                Derived &operator=(const Derived &) noexcept = delete ; \
                Derived &operator=(Derived &&) noexcept = delete ; \
        } \
        
// PRQA L:L1
// deprecated macro trick - harmless function used at compile time to emit deprecated warning
static inline void ATTRIBUTE_DEPRECATED exDEFINE_EXCEPTION_MACRO() { ; } // PRQA S 2134,2314, 2645, 4649
// PRQA S 1020,1023 2
#define DEPRECATE(name, derived) static inline void deprecate_ ## derived ## name () { exDEFINE_EXCEPTION_MACRO() ; }

// end of deprecated macro trick 

/*!	\brief deprecated macro

    \def exDEFINE_EXCEPTION(ParentNs,Derived,Base)

    The macro is deprecated	
*/
#define exDEFINE_EXCEPTION(ParentNs,Derived,Base) DEPRECATE(exDEFINE_EXCEPTION, Derived)  // PRQA S 1020



/*! class exFileOpenException declaration */
exDECLARE_EXCEPTION(exFileOpenException,exException) ;      // PRQA S 2131, 2502

/*! class exMathException declaration */
exDECLARE_EXCEPTION(exMathException,exException) ;          // PRQA S 2131, 2502

/*! class exBoundsException declaration */
exDECLARE_EXCEPTION(exBoundsException,exException) ;        // PRQA S 2131, 2502
    
/*! class exIllegalValueException declaration */
exDECLARE_EXCEPTION(exIllegalValueException,exException) ;  // PRQA S 2131, 2502

/*! class exIOException declaration */
exDECLARE_EXCEPTION(exIOException,exException) ;            // PRQA S 2131, 2502

/*! class exThreadException declaration */
exDECLARE_EXCEPTION(exThreadException,exException) ;        // PRQA S 2131, 2502

/*! class exThreadException declaration */
exDECLARE_EXCEPTION(exSystemException,exException) ;        // PRQA S 2131, 2502

/*! class exCriticalException declaration */
exDECLARE_EXCEPTION(exCriticalException,exException) ;      // PRQA S 2131, 2502

/*! class exNotImplemented declaration */
exDECLARE_EXCEPTION(exNotImplemented,exException) ;         // PRQA S 2131, 2502
/*! @}*/

_ACS_END_NAMESPACE


#endif // PRQA S 1000
