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
	Revision 5.9  2018/09/04 16:16:05  marpas
	support for complete relocation of runtime project components added
	it uses new ProcessStatus singleton to discover executable path and dir, then
	compilation macros to build paths relative to the executable are provided
	
	Revision 5.8  2016/11/14 09:40:15  marfav
	Added default PID files permission flag DEFAULT_ACS_PID_PERMISSIONS
	
	Revision 5.7  2016/02/04 09:53:40  marpas
	useless const removed from declarations
	
	Revision 5.6  2015/06/10 09:03:54  tergem
	defaultMCLogPath added
	
	Revision 5.5  2014/08/26 10:30:18  marfav
	S3PDGS-1970: adding possibility to reconfigure log files permission at compile time
	
	Revision 5.4  2014/01/09 08:50:05  marfav
	Comments fixed
	
	Revision 5.3  2014/01/08 14:46:53  marfav
	Adding support to configurable core path
	
	Revision 5.2  2013/07/05 11:46:43  marpas
	introducing defaultLockPath() method and respective DEFAULT_ACS_LOCK_PATH define
	
	Revision 5.1  2013/06/18 13:31:43  marpas
	define PREVENT_ACS_BASE_WARNINGS to avoid repetitive warnings on defaults paths
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.7  2013/02/25 16:48:38  marpas
	DEFAULT_ACS_SMPLUGINS_FILENAME macro added defaulted to smplugins.conf
	defaultSMPluginsPath() function added
	defaultSMPluginsFileName() function added
	
	Revision 4.6  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/07 18:13:00  marpas
	qa suggestion in progress
	
	Revision 4.4  2012/06/05 17:07:00  marpas
	resolving some quality issues and documentation
	
	Revision 4.3  2012/05/02 15:49:57  nicvac
	S1PDGS-3312: path override by makefile configuration.
	
	Revision 4.2  2012/03/01 13:08:42  marpas
	acs::base::defaultDbConfFileName() introduced
	
	Revision 4.1  2012/02/29 17:53:34  marpas
	base config concept introduced
	
	Revision 1.5  2011/09/22 08:52:58  marpas
	slightly changed
	
	Revision 1.4  2011/08/08 16:21:36  marpas
	new macros cor copy ctor and assignement,
	new rules documented
	
	Revision 1.3  2011/06/01 14:28:47  marpas
	major changes
	coverage introduction
	
	Revision 1.2  2011/05/27 12:04:33  marpas
	doc moved into MAIN.h
	
	Revision 1.1.1.1  2011/05/26 17:02:39  marpas
	Creating libAcsBase
	
	

*/

// the following macros are intensively used in all the code
#ifndef _acs_config_HPP_
#define _acs_config_HPP_

#include <acs_c++pers.hpp> 

#include <sys/stat.h> // PRQA S 1013

// PRQA S 0040 EOF # directives not recognized by the QA tool


/*! @addtogroup macro_misc */
//@{
/*!
    This macro can be used in a file to declare a nested namespace

    @code
    _ACS_BEGIN_NAMESPACE(acs)
    
        // here we are in the acs namespace
         ...
    _ACS_END_NAMESPACE
    @endcode

    Notice that a semicolon must be used after this macro.

    @see _ACS_BEGIN_NESTED_NAMESPACE, _ACS_END_NAMESPACE
 */
 // PRQA S 1020,1023 3
# define _ACS_BEGIN_NAMESPACE(X) namespace X {
# define _ACS_END_NAMESPACE } 

/*!
    This macro can be used in a file to declare a nested namespace

    @code
    _ACS_BEGIN_NESTED_NAMESPACE(acs, os)

         // here we are in the acs::os namespace
        ...
    _ACS_END_NESTED_NAMESPACE
    @endcode

    Notice that a semicolon must be used after this macro.

    @see _ACS_BEGIN_NAMESPACE, _ACS_END_NESTED_NAMESPACE
 */
// PRQA S 1020,1023 3
# define _ACS_BEGIN_NESTED_NAMESPACE(X, Y)  namespace X { namespace Y {
# define _ACS_END_NESTED_NAMESPACE } }

/*!
    This macro can be used in a class declaration to disable the generation of
    default assignment operator.

    Some classes have a well-defined copy constructor but cannot have an
    assignment operator, typically because they can't be modified once created.
    In such case, this macro can be used to disable the automatic assignment
    operator generation.
    Moreover, when the use of such assignment operator is prohibited by inheritance, the compiler 
    can still complain about its lack. This is a good way to avoid the compiler warning.
    @code
    class Foo
    {
    public:
        Foo();
        ...

    private:
        // Foo can't be assigned
        ACS_DECLARE_NO_ASSIGN_CLASS(Foo);
    };
    @endcode

    Notice that a semicolon must be used after this macro.

    @see ACS_DECLARE_NO_COPY_CLASS()
 */
 
// PRQA S 1020,1030,1031 2
# define ACS_DECLARE_NO_ASSIGN_CLASS(classname) _Pragma ("Deprecated macro use language feature instead") \
   private: classname &operator=(const classname &)

/*!
    This macro can be used in a class declaration to disable the generation of
    default copy ctor and assignment operator.

    Some classes don't have a well-defined copying semantics. In this case the
    standard C++ convention is to not allow copying them. One way of achieving
    it is to use this macro which simply defines a private copy constructor and
    assignment operator.

    Beware that simply not defining copy constructor and assignment operator is
    @em not enough as the compiler would provide its own automatically-generated
    versions of them -- hence the usefulness of this macro.

    Moreover, when the use of such copying semantic is prohibited by inheritance, the compiler 
    can still complain about its lack. This is a good way to avoid the compiler warning.
    Example of use:
    @code
    class Foo
    {
    public:
        Foo();
        ...

    private:
        // Foo can't be copied
        ACS_DECLARE_NO_COPY_CLASS(FooWidget);
    };
    @endcode

    Notice that a semicolon must be used after this macro.

    @see ACS_DECLARE_NO_ASSIGN_CLASS(), ACS_DECLARE_NO_COPY_TEMPLATE_CLASS()
 */
// PRQA S 1020,1023,1030,1031 4
#define ACS_DECLARE_NO_COPY_CLASS(classname) _Pragma ("Deprecated macro use language feature instead") \
   ACS_DECLARE_NO_ASSIGN_CLASS(classname) ; \
   classname(const classname &) 

/*!
    Analog of ACS_DECLARE_NO_COPY_CLASS() for template classes.

    This macro can be used for template classes (with a single template
    parameter) for the same purpose as ACS_DECLARE_NO_COPY_CLASS() is used with the
    non-template classes.

    @param classname The name of the template class.
    @param arg The name of the template parameter.

    @see ACS_DECLARE_NO_COPY_TEMPLATE_CLASS_2
 */
// PRQA S 1020,1030 2
#define ACS_DECLARE_NO_COPY_TEMPLATE_CLASS(classname, arg) _Pragma ("Deprecated macro use language feature instead") ACS_DECLARE_NO_COPY_CLASS( classname < arg > )

/*!
    Analog of ACS_DECLARE_NO_COPY_TEMPLATE_CLASS() for templates with 2
    parameters.

    This macro can be used for template classes with two template
    parameters for the same purpose as ACS_DECLARE_NO_COPY_CLASS() is used with
    the non-template classes.

    @param classname The name of the template class.
    @param arg1 The name of the first template parameter.
    @param arg2 The name of the second template parameter.

    @see ACS_DECLARE_NO_COPY_TEMPLATE_CLASS
 */
// PRQA S 1020,1030 2 
#define ACS_DECLARE_NO_COPY_TEMPLATE_CLASS_2(classname, arg1, arg2) _Pragma ("Deprecated macro use language feature instead") ACS_DECLARE_NO_COPY_CLASS( classname < arg1, arg2 > )

// PRQA S 1020 EndDefinedMacros

/*! This macro is intended to be passed via -D if all the macros below nedd to be computed 
    relatively to the exe path.
    if the exe path is /a/b/executable the exe dir will be computed as /a/b.
 */
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_PATH
   #warning ACS_PATHS_EXE_REL defined, DEFAULT_ACS_PATH would have no effect
 #endif
#else
 #ifndef DEFAULT_ACS_PATH
  /*! This macro is intended to be passed via -D in case the default here has to be replaced 
   */
  #define DEFAULT_ACS_PATH "/usr/acs"
  #ifndef PREVENT_ACS_BASE_WARNINGS
   #warning *** Default path as "/usr/acs" *** (DEFAULT_ACS_PATH)
  #endif
 #endif
#endif


#ifndef DEFAULT_ACS_DBCONF_FILENAME
#define DEFAULT_ACS_DBCONF_FILENAME "db" // NOSONAR
#ifndef PREVENT_ACS_BASE_WARNINGS
#warning *** Default conf name as "db" *** (DEFAULT_ACS_DBCONF_FILENAME) 
#endif
#endif

#ifndef DEFAULT_ACS_SMPLUGINS_FILENAME
#define DEFAULT_ACS_SMPLUGINS_FILENAME "smplugins.conf" // NOSONAR
#endif

#ifndef DEFAULT_ACS_LOCK_PATH
 #define DEFAULT_ACS_LOCK_PATH "/tmp" // NOSONAR
 #ifndef PREVENT_ACS_BASE_WARNINGS
  #warning *** Default lock path as "/tmp" *** (DEFAULT_ACS_LOCK_PATH)
 #endif
#endif

#ifndef DEFAULT_ACS_CORE_PATH
 #define DEFAULT_ACS_CORE_PATH "/tmp" // NOSONAR
 #ifndef PREVENT_ACS_BASE_WARNINGS
  #warning *** Default core path as "/tmp" *** (DEFAULT_ACS_CORE_PATH)
 #endif
#endif


#ifndef DEFAULT_ACS_LOG_PERMISSIONS
 #ifdef DEFAULT_LOG_PERMISSIONS
  #warning DEFAULT_LOG_PERMISSIONS deprecated: use DEFAULT_ACS_LOG_PERMISSIONS
  #define DEFAULT_ACS_LOG_PERMISSIONS DEFAULT_LOG_PERMISSIONS 
 #else
  #define DEFAULT_ACS_LOG_PERMISSIONS 0666 // NOSONAR
  #ifndef PREVENT_ACS_BASE_WARNINGS
   #warning *** Default log files permissions as 0666 *** (DEFAULT_ACS_LOG_PERMISSIONS)
  #endif
 #endif
#endif

#ifndef DEFAULT_ACS_PID_PERMISSIONS
 #define DEFAULT_ACS_PID_PERMISSIONS 0666 // NOSONAR
 #ifndef PREVENT_ACS_BASE_WARNINGS
  #warning *** Default pid files permissions as 0666 *** (DEFAULT_ACS_PID_PERMISSIONS)
 #endif
#endif

#ifndef DEFAULT_ACS_UUID_PERMISSIONS
 #define DEFAULT_ACS_UUID_PERMISSIONS 0666 // NOSONAR
 #ifndef PREVENT_ACS_BASE_WARNINGS
  #warning *** Default uuid files permissions as 0666 *** (DEFAULT_ACS_UUID_PERMISSIONS)
 #endif
#endif

// PRQA L:EndDefinedMacros
_ACS_BEGIN_NESTED_NAMESPACE(acs, base)

char const *defaultAcsPath() ACS_NOEXCEPT ;    // DEFAULT_ACS_PATH
char const *defaultConfPath() ACS_NOEXCEPT ;   // defaultAcsPath() + "/conf"
char const *defaultOptConfPath() ACS_NOEXCEPT ;   // defaultAcsPath() + "/conf_opt"
char const *defaultDbConfPath() ACS_NOEXCEPT ; // defaultAcsPath() + "/conf/database"
char const *defaultDbConfFileName() ACS_NOEXCEPT ; // DEFAULT_ACS_DBCONF_FILENAME
char const *defaultSMPluginsPath() ACS_NOEXCEPT ;  // defaultDbConfPath() 
char const *defaultSMPluginsFileName() ACS_NOEXCEPT ; // DEFAULT_ACS_SMPLUGINS_FILENAME
char const *defaultDebugPath() ACS_NOEXCEPT ;  // defaultAcsPath() + "/debug"
char const *defaultStatusPath() ACS_NOEXCEPT ; // defaultAcsPath() + "/status"
char const *defaultLogPath() ACS_NOEXCEPT ;    // defaultAcsPath() + "/log"
char const *defaultPersistencePath() ACS_NOEXCEPT ;    // defaultAcsPath() + "/persistence"
char const *defaultMCLogPath() ACS_NOEXCEPT ;    // defaultAcsPath() + "/log"
char const *defaultBinPath() ACS_NOEXCEPT ;    // defaultAcsPath() + "/bin"
char const *defaultLockPath() ACS_NOEXCEPT ;    // "/tmp" DEFAULT_ACS_LOCK_PATH
char const *defaultCorePath() ACS_NOEXCEPT ;    // "/tmp" DEFAULT_ACS_CORE_PATH

mode_t defaultLogPermissions() ACS_NOEXCEPT ; // 0666 DEFAULT_LOG_PERMISSIONS
mode_t defaultPidPermissions() ACS_NOEXCEPT ; // 0666 DEFAULT_ACS_PID_PERMISSIONS
mode_t defaultUuidPermissions() ACS_NOEXCEPT ; // 0666 DEFAULT_ACS_UUID_PERMISSIONS

_ACS_END_NESTED_NAMESPACE

//@}

#endif // _acs_config_HPP_
