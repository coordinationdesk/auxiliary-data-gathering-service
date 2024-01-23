/*

	Copyright 2011-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Base Library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2018/09/27 12:20:33  marpas
	no more exception from guard's dtors - C++11 call terminate if an exception throws from a dtor
	
	Revision 5.5  2016/02/08 12:13:03  marpas
	warning message conditioned to define
	
	Revision 5.4  2015/01/20 14:07:36  marfav
	FDGuard release exit status monitored
	
	Revision 5.3  2015/01/15 08:52:06  davide.tiriticco
	unistd.h include added.
	
	Revision 5.2  2013/11/28 15:37:24  marpas
	qa warning
	
	Revision 5.1  2013/06/18 09:49:54  marpas
	qa warning
	
	Revision 5.0  2013/06/04 15:34:59  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.10  2013/03/29 17:49:43  marpas
	missing include added
	
	Revision 4.9  2013/03/29 17:44:05  marpas
	AutoGurad now does not catch silently the exception got from relase, but outputs them
	
	Revision 4.8  2013/03/28 09:55:34  marpas
	qa warning fixed
	
	Revision 4.7  2013/03/11 16:26:37  lucio.pulvirenti
	class definition for older gcc version of compiler fixed
	
	Revision 4.6  2013/03/11 13:03:51  marpas
	interface rationalization
	
	Revision 4.5  2013/03/07 12:03:52  marpas
	Guard, robusteness and usage degree improved
	
	Revision 4.4  2013/03/04 17:58:29  marpas
	interface rationalization,
	robustness improved
	
	Revision 4.3  2013/01/22 17:48:08  marpas
	qa warnings
	
	Revision 4.2  2012/12/20 14:56:48  marpas
	old compiler version took into account
	test improved
	
	Revision 4.1  2012/12/17 16:43:27  marpas
	Guard pattern included and released
	
	Revision 1.7  2011/09/15 18:15:55  marpas
	FDGuard is now a typedef into acs::pattern namespace
	
	Revision 1.6  2011/09/05 11:48:12  marpas
	doc improved
	
	Revision 1.5  2011/08/03 13:13:20  enrcar
	EC:: Signature of funcion called by PathTraits::destroy changed. Now it should return int (instead of: void)
	
	Revision 1.4  2011/08/02 10:20:54  marpas
	AutoGuard template added
	
	Revision 1.3  2011/06/09 15:52:38  marpas
	still working and porting libACS_SSL
	
	Revision 1.2  2011/06/01 14:28:47  marpas
	major changes
	coverage introduction
	
	Revision 1.1.1.1  2011/05/26 17:02:39  marpas
	Creating libAcsBase
	
	

*/


#ifndef _Pattern_Guard_H_
#define _Pattern_Guard_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>
#include <exException.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs,pattern)

/*! \addtogroup pattern 
 *  @{
 */

/*!
 * \class  RAIIGuard
 *
 * \brief template convenience method to perform <em>Resource Allocation Is Initialization</em> (\b RAII) \sa http://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization
 *
 * the template is a convenience method to acquire a resource R_ with its own method R_::Acq_()
 * when creating and release the resource with the R_::Rel_() method when destroying
 * \tparam R_ the class resource that will be guarded
 * \tparam R_::*Acq_ the function to 'acquire' the resource
 * \tparam R_::*Rel_ the function to 'release' the resource
 *
 * Example of use:
 * \code
 * 
 *    class M {
 *    public:
 *      ...
 *      void lock() { g_= true ; debug() ;}
 *      void unlock() { g_= false ; debug() ;}
 *      ...
 *    } ;
 * 
 *    typedef RAIIGuard<M, &M::lock, &M::unlock> MG ;
 * 
 *    M m ;
 *    {
 *      MG mg (m) ; // acquire the lock
 *      ...
 *      // release the lock - MG out of scope
 *    }
 * 
 * \endcode
 */
template<class R_, void (R_::*Acq_)(), void (R_::*Rel_)()>
class RAIIGuard { // PRQA S 2109
public:
	RAIIGuard(R_ &r) : r_(r), acquired_(false) { acquire() ; }
	
    /*! 
     * \brief dtor - read documentation
     * 
     * \warning the destructor do not leave with an exception if is called during stack unwinding
     * anyway, because the design of the guard itself, if an exception should be thrown from the Rel_ function it will 
     * leave the destructor.
     */
#if __cplusplus >= 201103L
	virtual ~RAIIGuard() noexcept(false) { // PRQA S 2045 # it is invoked expecting the exception
#else
	virtual ~RAIIGuard() { 
#endif
		bool st_unwid_ = std::uncaught_exception() ;

        try {
            release() ; 
        }
        catch(std::exception &x) {
            if (not st_unwid_) { throw ; }
            std::cerr << exDebugFullSignature << explain(x) ;
        }
    } ;
	
    /*! 
     * \note the implementation of this method do not check for exceptions
     * if an exception is launched by the R_::*Acq function, the acquired_ flag is left to false, so when the guard would be destroyed
     * there will be no attempt to \a release the <em>acquired resource</em>
     */
    void acquire() { if (not acquired_) (r_.*Acq_)() ; acquired_ = true ; } ;
    void release() {  if (acquired_) (r_.*Rel_)() ; acquired_ = false ; } ;
    void detach() { acquired_ = false ; }
private:
    RAIIGuard() ; // not implemented
    RAIIGuard(const RAIIGuard & ) ; // not implemented
    RAIIGuard& operator= (const RAIIGuard & ) ; // not implemented
private:
    R_ &r_ ; ///< a reference to the <i>acquired</i> resource
    bool acquired_ ; ///< the flag to remember the <i>acquired</i> status
};



/*!
 * \class ScopeGuard
 *
 * \brief resource 'life scoping' 
 *
 * the template is a convenience method to release a resource T_ with a function that gets a T_
 * \tparam T_ the class resource that will be scoped
 * \tparam Release the function to 'release' the resource
 *
 */
template <typename T_, void (*Release_)(T_)> 
class ScopeGuard // PRQA S 2109
{
public:
    ScopeGuard() : addr_() { }
	explicit ScopeGuard(T_ p) : addr_(p) { }
	ScopeGuard(const ScopeGuard &rhs) : addr_(rhs.addr_) { rhs.detach() ; }
#if __cplusplus >= 201103L
	virtual ~ScopeGuard() noexcept(false) { // PRQA S 2045 # it is invoked expecting the exception
#else
	virtual ~ScopeGuard() { 
#endif
            // the st_unwind_ is true if the stack is unwinding because 
            // an exception has been thrown and still not caugth
            bool st_unwid_ = std::uncaught_exception() ; // PRQA S 2983 # used few lines below 
            try {
                if (addr_) { (Release_)(addr_); } // PRQA S 4631 # can throw !!!
            }
            catch(std::exception &x) {
                if (not st_unwid_) { throw ; } // no exception handling in progres it is safe to throw this one
                std::cerr << exDebugFullSignature << explain(x) ; // PRQA S 4631
            }
        } 
    
    ScopeGuard &operator=(const ScopeGuard &rhs) 
    {
        if (this != &rhs) {
            if (addr_ != rhs.addr_) {
                release() ; // release current resource 
                addr_ = rhs.addr_ ; // acquire rhs one
            }
            rhs.detach() ; // detach rhs's
        }
        return *this ;
    }
	void release() { if (addr_ ) { (Release_)(addr_); addr_ = 0 ; } } 
    void detach() const ACS_NOEXCEPT { addr_ = 0 ; } 
private:
    mutable T_ addr_ ;
} ;

#if __GNUC__ > 4 || \
              (__GNUC__ == 4 && (__GNUC_MINOR__ > 4 || \
                                 (__GNUC_MINOR__ == 4 && \
                                  __GNUC_PATCHLEVEL__ > 3)))
/*!
 * \class CMemoryScopeGuard
 *
 * A memory scope guard over a pointer. It calls free when the CMemoryScopeGuard is destroyed.
 * \see ScopeGuard
 *
 */
typedef pattern::ScopeGuard<void*, &std::free> CMemoryScopeGuard ; // PRQA S 0228


#else
 #ifndef PREVENT_ACS_BASE_WARNINGS
  #warning CMemoryScopeGuard is not a typedef
 #endif
class CMemoryScopeGuard 
{
public:
    CMemoryScopeGuard() : s_() {} ;
	explicit CMemoryScopeGuard(void *p) : s_(p) { }
	CMemoryScopeGuard(const CMemoryScopeGuard &rhs) : s_(rhs.s_) {}
	~CMemoryScopeGuard() ACS_NOEXCEPT {}  
	CMemoryScopeGuard &operator=(const CMemoryScopeGuard &rhs) 
    {
        if (this != &rhs) { s_ = rhs.s_ ; }
        return *this ;
    }
	void release() { s_.release() ; } 
    void detach() const ACS_NOEXCEPT { s_.detach() ; } 
private:
    ScopeGuard<void *, free> s_ ;
} ;

#endif

/*! 
    \typedef Release_w_string
    \brief convenience typedef to func pointer  \see AutoGuard, PathTraits
    
 *  it is defined as 
 \code
 int (*Release_w_string)(std::string )
 \endcode
*/
typedef int (*Release_w_string)(std::string ) ; 

/*!
 * \class  PathTraits 
 * \brief trait to release a resource passing a string
 *
 * A template class to be used with AutoGuard when resource is a string (e.g. a path).
 *
 * \tparam Release_ a release method over a string resource
 * \note the template defines also an empty static method returning the "empty" value. 
 * \see Release_w_string 
 */
template <Release_w_string T> 
struct PathTraits {
	static std::string emptyValue() { return "" ; }
	static void releaseResource(std::string s) { T(s) ; } // PRQA S 2010
} ;

/*! \typedef Release_w_int
    \brief convenience typedef to func pointer \see AutoGuard, FDTraits
    
 *  it is defined as 
 \code
 int (*Release_w_int)(int )
 \endcode
*/
typedef int (*Release_w_int)(int ) ; /*! \brief convenience pointer to func typedef */

/*!
 * \class  FDTraits
 * \brief trait to release a resource passing an int
 * A template class to be used with AutoGuard when resource is an int (e.g. a file descriptor).
 *
 * \tparam Release_ a release method over an int resource
 * \note the template defines also an empty static method returning the "empty" value. 
 * \see Release_w_int
 */

template <Release_w_int T > 
struct FDTraits {
	static int emptyValue() { return -1 ; }
	static void releaseResource(int fd) 
	{ 
        if (0 != (T)(fd) ) {
            ACS_THROW(exIOException ("Release error occurred (::close syscall?)", errno)); // PRQA S 3224
        }
	}
} ;


/*!
 * \class  AutoGuard
 *
 * \brief template convenience class to perform resource release.
 *
 * the template is a convenience class to perform resource relase. Peculiarity of the class is 
 * that the ownership of the resource is transferred during assignement (copy ctor and assignement op)
 * \tparam T_ the class resource that will be guarded
 * \tparam Traits_ the traits template defining the destroy method. See PathTraits and FDTraits
 *  
 * Example of use:
 * \code
 * 
 *    typedef class AutoGuard<int, FDTraits< &::close > > FDGuard ;
 * 
 *    FDGuard file_guard(open("/tmp/myfile") ) ;
 *    read(file_guard, ... ) // file_guard returns the fd when converted to int
 * 
 * \endcode
 *
 * the above example shows how to automatically close an opened file when file_guard goes out of scope.
 */
template <typename T_, typename Traits_>
class AutoGuard { // PRQA S 2109
public:
    typedef Traits_ traits ;
    typedef T_ type ;

    explicit AutoGuard(const type &v = traits::emptyValue() ) : guarded_(v) {}
    ~AutoGuard() ACS_NOEXCEPT { release() ; }
    AutoGuard(const AutoGuard &rhs) : guarded_(rhs.guarded_) { rhs.detach() ; }
    AutoGuard& operator=(const AutoGuard &rhs) {
        if (this != &rhs) {
            if ((guarded_ !=  traits::emptyValue()) && (guarded_ != rhs.guarded_)) { 
                release() ; 
            } 
            guarded_ = rhs.guarded_ ;   
            rhs.detach() ;
        }
        
        return *this ;
    } 

    /*! detaches the guarded resource
    */
    void detach() const ACS_NOEXCEPT { guarded_ = traits::emptyValue() ; }

    /*! releases the resource if different from emptyValue()
    */
    void release() const ACS_NOEXCEPT { // PRQA S 4214
        if (guarded_ != traits::emptyValue()) { 
            try { traits::releaseResource(guarded_) ; } 
            catch(std::exception &x) { ACS_LOG_NOTIFY_EX(x) ;} 
            detach() ; 
        } 
    }

    /*!
     * \brief cast operator
     *
     * a cast operator that can be used to access the guarded resource. 
     */
    operator const type &() const { return guarded_ ; } // PRQA S 2181

private:

	mutable type guarded_ ;    
} ;


/*!
 * \typedef  FDGuard
 *
 * \brief an AutoGuard template specialization for file descriptor
 *
 * the specialization will invoke close on the gived file descriptor when exiting from the scope.
 *  
 * Example of use:
 * \code
 * {
 *    FDGuard file_guard(open("/tmp/myfile") ) ;
 *    read(file_guard, ... ) // file_guard returns the fd when converted to int
 *    // here file_guard exits from the scope and calls close
 * } 
 * \endcode
 *
 * the above example shows how to automatically close an opened file when file_guard goes out of scope.
 */
typedef class AutoGuard<int, FDTraits< &::close > > FDGuard ; // PRQA S 0228, 2502


/*! @}*/

_ACS_END_NESTED_NAMESPACE

#endif // _Pattern_Guard_H_
