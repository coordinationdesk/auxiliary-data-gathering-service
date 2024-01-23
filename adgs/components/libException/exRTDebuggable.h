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
	Revision 5.8  2018/09/04 16:12:30  marpas
	exRTDebug::registerDebug does not need any longer the class pointer (was useless)
	exRTDebuggable new explicit ctor to support debug value link between classes
	
	Revision 5.7  2016/05/19 09:56:15  marpas
	deprecating old ex_XXXX_DEFINE_DEBUG_LEVEL
	
	Revision 5.6  2013/09/26 18:09:35  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.5  2013/06/17 09:03:43  marpas
	qa warning removed
	
	Revision 5.4  2013/06/11 08:44:25  marpas
	macros deprecation improved
	
	Revision 5.3  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.2  2013/06/05 15:55:51  marpas
	new RTDebuggable class in utility namespace to have a new approach to real-time debugging.
	previous exRTDebuggable left for backward compatibility, but it will sonn deprecated
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.4  2013/02/07 08:57:41  marpas
	pattern singleton used in debug support
	exception notification improved in debug subsystem support
	namespaces enforced
	
	Revision 4.3  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.2  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.1  2012/06/06 17:12:20  marpas
	qa rules application in progress
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.4  2009/06/26 10:41:55  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.3  2009/03/11 17:09:02  marpas
	c++rules violation removed
	
	Revision 2.2  2006/05/11 20:37:11  marpas
	low level memory management to avoid string repository problems with shared objects
	
	Revision 2.1  2006/02/28 08:24:37  marpas
	new release, exException inherits from std::exception
	
	Revision 1.3  2005/10/12 22:23:10  ivafam
	MP: robustness improved against threads, multiple initialization (shared objects)
	
	Revision 1.2  2005/10/06 18:04:26  marpas
	initialization with environment and include macros to help users
	
	Revision 1.1  2005/10/06 10:30:24  marpas
	Runtime debug almost completed
		
*/

#ifndef _exRTDebuggable_H_
#define _exRTDebuggable_H_ 

#include <acs_c++config.hpp>
#include <exStackTrace.h> // helper to users
#include <exMacros.h> // helper to users
#include <string>
#include <typeinfo>
#include <iostream>

_ACS_BEGIN_NAMESPACE(acs)


/*! \class exRTDebuggable
	
	\brief support class for runtime debugging
	
	This class is the base class for runtime debug. 
	It is used by exCLASS_DECLARE_RTDEBUG_LEVEL, exCLASS_DEFINE_RTDEBUG_LEVEL, 
	exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL macros. Users do not
	need to deal with this class directly, but through the macros defined in the exMacros.h file.
    This class is in obsolescent path - it can be removed along the exCLASS_XXX deprecated macros above	
*/
class exRTDebuggable // PRQA S 2109
{
public:
	explicit  __attribute__((deprecated)) exRTDebuggable(const std::string &, int startValue=0) ; 
	~exRTDebuggable() ACS_NOEXCEPT ;
	
	void setDebug(int v ) ACS_NOEXCEPT { _debugLevel = v ; } 

	int getDebug() const ACS_NOEXCEPT ; // inlined for perfo
	const char * getDebugRef() const ACS_NOEXCEPT ;
	int *get_ref() ACS_NOEXCEPT { return &_debugLevel ; } // PRQA S 4024
private:
#if __cplusplus >= 201103L	
	exRTDebuggable() = delete ; // PRQA S 2038 5
    exRTDebuggable(const exRTDebuggable &) = delete ;
    exRTDebuggable(exRTDebuggable &&) = delete ;
    exRTDebuggable & operator= (const exRTDebuggable &) = delete ;
    exRTDebuggable & operator= (exRTDebuggable &&) = delete ;
#else
	// declared but not implemented to prevent their use
	exRTDebuggable() ;
	exRTDebuggable(const exRTDebuggable &) ;
	exRTDebuggable &operator=(const exRTDebuggable &) ;
#endif

private:	
	int _debugLevel ;
	char *_debugRefString ;
} ;


// all the inline template functions to deal with debug (int or exRTDebuggable)
// PRQA S 2134 L1
template <typename T> inline int asInt(const T &d) ACS_NOEXCEPT { return d.getDebug() ; }
template <> inline int asInt(const int &d) ACS_NOEXCEPT { return d ; } // PRQA S 2620

template <typename T> inline void setInt(T &d, int v) ACS_NOEXCEPT { d.setDebug(v) ; }  
template <> inline void setInt(int &d, int v) ACS_NOEXCEPT { d = v ; }  
// PRQA L:L1

_ACS_END_NAMESPACE


_ACS_BEGIN_NESTED_NAMESPACE(acs,utility)


/*! \addtogroup utility 
 *  @{
 */


/*! \class RTDebuggable
 * 
 * \brief support class for runtime debugging
 * 
 * This class is the base class for runtime debug. 
 * It is used by CLASS_DECLARE_RTDEBUG_LEVEL, CLASS_DEFINE_RTDEBUG_LEVEL, 
 * CLASS_DECLARE_DEBUG_LEVEL, CLASS_DEFINE_DEBUG_LEVEL macros. Users do not
 * need to deal with this class directly, but through the macros defined in the DebugMacros.hpp file.
 * 
 */
template <typename T_D_>
class RTDebuggable // PRQA S 2109
{
public:
	explicit RTDebuggable(const std::string &dbg_string, int start=0) ACS_NOEXCEPT ;	 
	explicit RTDebuggable(int start=0) ACS_NOEXCEPT ;	 
	~RTDebuggable() ACS_NOEXCEPT ;
		
	RTDebuggable &operator=(int v)
	{
		_debugLevel = v ;
		return * this ;
	}
	
	/*! \brief cast operator 
	 * useful to write code like this 
	 * \code
	 *  RDTDebuggable someClass ;
	 *  
	 *  if (someClass > 2) {
	 *     ...
	 *  }
	 * \endcode
	 * \note inlined for perfo
	 */
	operator const int &() const ACS_NOEXCEPT { return _debugLevel ; } // PRQA S 2181, 2183

	/*! \brief set the debug level */
	void setDebug(int v) ACS_NOEXCEPT { _debugLevel = v ; }

	/*! \brief returns the debug level */
	int getDebug() const ACS_NOEXCEPT { return _debugLevel ; }


	/*! \brief returns the debug string to be used in env */
	static std::string getDebugRef() 
    {
	    return std::string(strings::RTTr(exStackTrace::demangle(typeid(T_D_).name()))) ;
    }

private:
#if __cplusplus >= 201103L	
    RTDebuggable(const RTDebuggable &) = delete ; // PRQA S 2038 4
    RTDebuggable(RTDebuggable &&) = delete ;
    RTDebuggable & operator= (const RTDebuggable &) = delete ;
    RTDebuggable & operator= (RTDebuggable &&) = delete ;
#else
    RTDebuggable(const RTDebuggable &) ; /*! prohibited */
    RTDebuggable & operator= (const RTDebuggable &) ; /*! prohibited */
#endif
private:	
	int _debugLevel ; ///< the debug level
} ;

/*! @}*/

void registerDebug(const std::string &, int *) ACS_NOEXCEPT ;
void registerDebug(const std::string &, int *, const std::string &) ACS_NOEXCEPT ;
		

template<typename T_D_>
RTDebuggable<T_D_>::RTDebuggable(const std::string &dbg_string, int start) ACS_NOEXCEPT :
	_debugLevel(start)
{
	std::string myself_ref_ = std::string(strings::RTTr(exStackTrace::demangle(typeid(T_D_).name()))) ; // PRQA S 3050, 3081
	if (0 == _debugLevel) { strings::RTTr::envget_(dbg_string, _debugLevel) ; }
	registerDebug(dbg_string, &_debugLevel, myself_ref_) ;
}

template<typename T_D_>
RTDebuggable<T_D_>::RTDebuggable(int start) ACS_NOEXCEPT :
	_debugLevel(start)
{
	std::string ref_ = std::string(strings::RTTr(exStackTrace::demangle(typeid(T_D_).name()))) ; // PRQA S 2839, 3050, 3081

	if (0 == _debugLevel) { strings::RTTr::envget_(ref_, _debugLevel) ; }
	registerDebug(ref_, &_debugLevel) ;
}

template<typename T_D_>
RTDebuggable<T_D_>::~RTDebuggable() ACS_NOEXCEPT 
{
}


_ACS_END_NESTED_NAMESPACE

#include <exException.h> // helper to users

#endif // _exRTDebuggable_H_
