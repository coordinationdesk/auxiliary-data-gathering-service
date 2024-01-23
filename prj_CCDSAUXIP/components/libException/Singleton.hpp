/*

    Copyright 2011-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

    $Prod: A.C.S. Base Library $

    $Id$

    $Author$

    $Log$
    Revision 5.6  2018/09/27 13:24:54  marpas
    auto_ptr changed into unique_ptr if supported (C++11)
    shared_ptr got from STL if supported (C++11)

    Revision 5.5  2015/11/19 16:46:47  marpas
    qa warning removed

    Revision 5.4  2013/12/03 13:42:44  marpas
    qa warnings

    Revision 5.3  2013/10/04 08:48:40  marpas
    testing unicity of singleton mutexes

    Revision 5.2  2013/09/26 18:09:35  marpas
    robustness strongly improved:
    singleton functions (with static variables inside) used
    some private members are now accessed only through the above technique
    exRTDebuggable classes do not longer unregister themselves from exRTDebug.
    it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
    The above changes will lead in a more efficient process closure.
    moreover the exException class now uses the reentrant method strerror_t to obtain the errno explanation.

    Revision 5.1  2013/07/12 08:55:19  marpas
    comment fixed

    Revision 5.0  2013/06/04 15:35:00  marpas
    introducing new log macros, debug macros
    work in progress

    Revision 4.6  2013/05/24 09:02:17  marpas
    release at exit logic flow improved

    Revision 4.5  2013/05/14 17:56:12  marpas
    Singleton's "release at exit" concept implemented

    Revision 4.4  2013/04/09 07:50:01  marpas
    ParamConstructibleSingletonMap template introduced

    Revision 4.3  2013/04/04 19:30:06  marpas
    ParamConstructibleSingleton introduced (experimental)

    Revision 4.2  2013/01/25 11:16:43  marpas
    qa warning and rationalization

    Revision 4.1  2013/01/24 10:45:37  marpas
    the following patterns are new and defined into the acs::pattern namespace:
    Singleton
    InitializableSingleton
    ParamInitializableSingleton
    coverage test added

    Revision 1.3  2011/12/23 15:58:13  marpas
    design enforced

    Revision 1.2  2011/06/01 14:28:48  marpas
    major changes
    coverage introduction

    Revision 1.1.1.1  2011/05/26 17:02:39  marpas
    Creating libAcsBase



*/


#ifndef _Pattern_Singleton_H_
#define _Pattern_Singleton_H_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <exStream.h>
#include <memory>
#include <exception>
#include <map>

_ACS_BEGIN_NESTED_NAMESPACE(acs,pattern)

/*! \addtogroup pattern
 *  @{
 */
template <typename T_ > class NoReleaseAtExitTraits // PRQA S 2109
{
public:
    static bool releaseAtExit() noexcept { return false ; }
} ;

/*!
 * \class ReleaseAtExitTraits
 *
 * \brief a wrong attempt to get rid of Singleton resources at exit DO NOT USE
 *
 */
template <typename T_ > class ReleaseAtExitTraits // PRQA S 2109
{
public:
    static bool __attribute__((deprecated("Could use released resources")))releaseAtExit() noexcept { return false ; } // do not use anymore 
} ;


/*!
 * \class Singleton
 *
 * \brief a singleton pattern template
 *
 * There are two different patterns for using this template:
 *
 * - <b> Pattern 1 </b>
 * \code
 *    template <typename T> class Factory
 *    {
 *    public:
 *       Factory() : _ciccio(1234) {}
 *       int ciccio() {return _ciccio;}
 *       void ciccio(int i) {_ciccio=i;}
 *
 *    private:
 *       int _ciccio;
 *    };
 *
 *    template <typename T> class FactorySingleton : public Singleton<Factory<T> > {};
 *
 * \endcode
 * In this way I can use instances of the Factory<T> class also separately.
 * Using the singleton I can access the same instance from everywhere
 *
 * - <b> Pattern 2 </b>
 *
 * \code
 *
 *    template <typename T> class Factory : public Singleton < Factory<T> > {
 *    protected:
 *       Factory() : _ciccio(1234) {}
 *
 *    public:
 *       int ciccio() {return _ciccio;}
 *       void ciccio(int i) {_ciccio=i;}
 *
 *    private:
 *       int _ciccio;
 *
 *       friend Factory<T>* Singleton< Factory<T> >::instance();
 *    };
 *
 * \endcode
 * In this way the Factory<T> cannot be used alone
 *
 * \note the "friend" declaration that is the unique way to access the protected CTOR from the Singleton::instance()
 * static method.
 *
 * \note allow default copy ctor and assignment operator; this class has no data member
 *
 * \note remember the namespace 'pattern'
 *
 * \sa http://en.wikipedia.org/wiki/Singleton_pattern
 *
 */
template <typename T_, typename Traits_=NoReleaseAtExitTraits<T_> > class Singleton // PRQA S 2109
{
public:
    //  a singleton cannot be copied nor assigned
    Singleton(const Singleton &) = delete ;
    Singleton &operator=(const Singleton &) = delete ;
    virtual ~Singleton() = default ;

    static bool _M_created() noexcept { return s_instance_ != nullptr ; }

#ifdef TEST_SINGLETON
public:
#else
protected:
#endif
    static Mutex &_M_mutex() noexcept { static acs::Mutex s_mutex__ ; return s_mutex__ ; } // PRQA S 2328, 4044, 4122

protected: // NOSONAR - for clarity
    Singleton() = default ;
    static void _M_release()
    {
        try {
            acs::Lock l_(_M_mutex()); // acquire the lock
            if (_M_created()) {
                delete s_instance_ ; // NOSONAR
            }
            s_instance_ = nullptr ;
            // release the lock
        }
        catch(const std::exception &x) { // NOSONAR
            excerr << x ;
        }
    }

    static void _at_exit_release() noexcept
    {
        if (Traits_::releaseAtExit()) {
            acs::Lock l_(_M_mutex()); // acquire the lock
            try {
                if (_M_created()) {
                    _M_release() ;
                }
            }
            catch(const std::exception &x) { // NOSONAR
                excerr << x ;
            }
        }
    }
public:
    static T_* instance() // PRQA S 4020
    {
        // do not instance useless locks
        if (!_M_created())
        {
            acs::Lock l_(_M_mutex()); // acquire the lock
            if (!_M_created()) // avoid another thread has already initialized it
            {
                try
                {
                    std::unique_ptr<T_> pointer (new T_); // NOSONAR # purpose of the smart pointer is to deallocate
                    s_instance_ = pointer.release();
                    if (atexit(_at_exit_release) != 0) { // PRQA S 0228
                        tryCatchAndNotify(exSystemException("No atexit for this Singleton", errno),excerr) ;  // PRQA S 3081
                    }
                }
                catch (const std::exception &)
                {
                    // Just throw the exception outside
                    _M_release() ;
                    throw;
                }
            }
            return s_instance_;
            // release the lock
        }
        return s_instance_;
    }


private:
    static T_ *s_instance_;
};

/*! @}*/


template <typename T_, typename Traits > T_* Singleton<T_, Traits >::s_instance_ = nullptr; // PRQA S 1067



/*! \addtogroup pattern
 *  @{
 */

/*!
 * \class InitializableSingleton
 *
 * \brief an intitializable singleton pattern template
 *
 * This class must be used with template arguments
 * that define the method "init()" even statically.
 * The init will be called just after the default constructor
 * in order to correctly resolve any virtual method that has to be called for the initialization
 * For the above reason the template param (typename T_) MUST be default constructible
 *
 * \note allow default copy ctor and assignment operator; this class has no data member
 *
 */
template <typename T_, typename Traits_=NoReleaseAtExitTraits<T_> >
class InitializableSingleton : public Singleton<T_, Traits_ >
{
public:
    static T_* instance()
    {
        // do not instance useless locks
        if (!Singleton<T_>::_M_created())
        {
            acs::Lock l_(Singleton<T_, Traits_>::_M_mutex()); // acquire the lock
            if (!Singleton<T_, Traits_>::_M_created())
            {
                try
                {
                    T_ * s__ = Singleton<T_, Traits_>::instance(); // to completely create the class tree

                    s__->init();
                    return s__ ;
                }
                catch (...)
                {
                    // Just cleanup and throw it away
                    Singleton<T_, Traits_>::_M_release();
                    throw;
                }
            }
        }
        return Singleton<T_>::instance() ;
        // release the lock
    }
protected:
    InitializableSingleton() = default ;
};


/*! @}*/






/*! \addtogroup pattern
 *  @{
 */



/*!
 * \class ParamInitializableSingleton
 * \brief an initializable singleton pattern template
 *
 * This class must be used with template arguments
 * that define the method "init(something)" even statically.
 * The init will be called just after the default constructor
 * in order to correctly resolve any virtual method that has to be called for the initialization
 * For the above reason the template param (typename T_) MUST be default constructible
 *
 * \note allow default copy ctor and assignment operator; this class has no data member
 *
 */

template <typename T_, typename P_, typename Traits_=NoReleaseAtExitTraits<T_> >
class ParamInitializableSingleton : public Singleton<T_, Traits_ > // PRQA S 2153
{
    // allow default copy ctor and assignment operator; this class has no
    // data member
public:
    ParamInitializableSingleton(const ParamInitializableSingleton &) = delete ;
    ParamInitializableSingleton& operator=(const ParamInitializableSingleton &) = delete ;

    /*!
     * is the instance method of the template
     * \note has a default parameter of type P_ that is defaulted to a const defined in T_ and called defaultInitValue
     */
    static T_* instance(const P_ &p_ = T_::defaultInitValue ) // PRQA S 4020
    {
           // do not instance useless locks
        if (!Singleton<T_, Traits_>::_M_created())
        {
            acs::Lock l_(Singleton<T_, Traits_>::_M_mutex()); // acquire the lock
            if (!Singleton<T_, Traits_>::_M_created())
            {
                try
                {
                    T_ * s__ = Singleton<T_, Traits_>::instance(); // to completely create the class tree

                    s__->init(p_);
                    return s__ ;
                }
                catch (...)
                {
                    // Just cleanup and throw it away
                    Singleton<T_, Traits_>::_M_release();
                    throw;
                }
            }
        }
        return Singleton<T_>::instance() ;
        // release the lock
    }
    virtual ~ParamInitializableSingleton() = default ;

protected:
    ParamInitializableSingleton() = default ;
};

/*! @}*/


/*! \addtogroup pattern
 *  @{
 */


/*!
 * \class ParamConstructibleSingleton
 * \brief a constructible singleton pattern template
 *
 * This class must be used with template arguments
 * that define its ctor "ctor(something)".
 * The ctor will be called passing to it a const reference to a typename P_.
 *
 * \note allow default copy ctor and assignment operator; this class has no data member
 *
 */
template <typename T_, typename P_, typename Traits_=NoReleaseAtExitTraits<T_> >
class ParamConstructibleSingleton // PRQA S 2109
{
public:
    ParamConstructibleSingleton(const ParamConstructibleSingleton &) = delete ;
    ParamConstructibleSingleton &operator=(const ParamConstructibleSingleton &) = delete ;
    virtual ~ParamConstructibleSingleton() = default ;
    //! the instance method that build (if necessary) the instance of the T_ class.

protected:
    ParamConstructibleSingleton() = default ;
#ifdef TEST_SINGLETON
public:
#else
protected:  // NOSONAR - for clarity
#endif
    static Mutex &_M_mutex() noexcept { static acs::Mutex pcs_mutex__ ; return pcs_mutex__ ; }
protected: // NOSONAR - for clarity and to reset above relaxation when testing
    static bool _M_created() noexcept { return s_instance_ != nullptr ; }
    static void _M_release()
    {
        try {
            acs::Lock l_(_M_mutex()); // acquire the lock
            if (_M_created()) {
                delete s_instance_ ; // NOSONAR
            }
            s_instance_ = nullptr ;
        }
        catch(const std::exception &x) {
            excerr << x ;
        }
        // release the lock
    }

    static void _at_exit_release() noexcept
    {
        acs::Lock l_(_M_mutex()); // acquire the lock
        try {
            if (_M_created() && Traits_::releaseAtExit()) {
                _M_release() ;
            }
        }
        catch(const std::exception &x) {
            excerr << x ;
        }
    }
public:
    static T_* instance(const P_& p_)  // PRQA S 4020
    {
        // do not instance useless locks
        if (!_M_created())
        {
            acs::Lock l_(_M_mutex()); // acquire the lock
            if (!_M_created()) // avoid another thread has already initialized it
            {
                try
                {
                    std::unique_ptr<T_> pointer (new T_(p_)); // NOSONAR
                    s_instance_ = pointer.release();
                    if (atexit(_at_exit_release) != 0) {
                        tryCatchAndNotify(exSystemException("No atexit for this ParamConstructibleSingleton", errno),excerr) ;
                    }
                }
                catch (const std::exception &)
                {
                    // Just throw the exception outside
                    _M_release() ;
                    throw;
                }
            }
            return s_instance_;
            // release the lock
        }
        return s_instance_;
    }
private:
    static T_ *s_instance_;
};

/*! @}*/


template <typename T_, typename P_, typename Traits_> T_* ParamConstructibleSingleton<T_, P_, Traits_ >::s_instance_ = 0; // PRQA S 1067



/*! \addtogroup pattern
 *  @{
 */

/*!
 * \class ParamConstructibleSingletonMap
 * \brief a constructible singleton map pattern template.
 *
 * This class must be used with template arguments
 * that define its ctor "ctor(something)".
 * The ctor will be called passing to it a reference to a typename P_.
 * For each P_ passed to the instance method, the class ensure only one instance of the T_ will exists.
 *
 * \note allow default copy ctor and assignment operator; this class has no data member
 *
 */
template <typename T_, typename P_, typename Traits_=NoReleaseAtExitTraits<T_>  >
class ParamConstructibleSingletonMap // PRQA S 2109
{
public:
    ParamConstructibleSingletonMap(const ParamConstructibleSingletonMap &) = delete ;
    ParamConstructibleSingletonMap &operator=(const ParamConstructibleSingletonMap &) = delete ;
    virtual ~ParamConstructibleSingletonMap()= default ;
    //! the instance method that build (if necessary) the instance of the T_ class.
protected:
    ParamConstructibleSingletonMap() = default ;
#ifdef TEST_SINGLETON
public:
#else
protected: // NOSONAR - for clarity
#endif
    static Mutex &_M_mutex() noexcept { static acs::Mutex pcsm_mutex__ ; return pcsm_mutex__ ; }
protected: // NOSONAR - for clarity and to reset above relaxation when testing
    static bool _M_created(const P_& p_) noexcept
    { return s_instances_.find(p_) != s_instances_.end() ; }

    static void _M_release(const P_& p_) noexcept
    {
        try {
            acs::Lock l_(_M_mutex()) ; // acquire the lock
            if (_M_created(p_)) {
                s_instances_.erase(s_instances_.find(p_)) ;
            }
            // release the lock
        }
        catch(const std::exception &x) {
            excerr << x ;
        }
    }
public:
    static T_* instance(const P_& p_) // PRQA S 4020
    {
        // do not instance useless locks
        if (!_M_created(p_))
        {
            acs::Lock l_(_M_mutex()); // acquire the lock
            if (!_M_created(p_)) // avoid another thread has already initialized it
            {
                try
                {
                    std::shared_ptr<T_> pointer (new T_(p_)); // NOSONAR
                    s_instances_[p_] = pointer ;
                    // it's a shared ptr, existence of the just allocated payload
                    // is guaranteed by the element in the map stored above.
                    return pointer.get() ;
                }
                catch (const std::exception &)
                {
                    // Just throw the exception outside
                    _M_release(p_) ;
                    throw;
                }
            }
            return s_instances_[p_].get() ;
            // release the lock
        }
        return s_instances_[p_].get() ;
    }

private:
    static std::map<P_, std::shared_ptr< T_ > > s_instances_;
};

/*! @}*/


template <typename T_, typename P_, typename Traits_> std::map<P_, std::shared_ptr< T_ > > ParamConstructibleSingletonMap<T_,P_, Traits_>::s_instances_ ; // PRQA S 1067

_ACS_END_NESTED_NAMESPACE



#endif // _Pattern_Singleton_H_
