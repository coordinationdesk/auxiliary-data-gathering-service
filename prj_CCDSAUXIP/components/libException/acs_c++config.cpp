/*

	Copyright 2012-2022, Exprivia SpA - DFDA-AS
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
	Revision 5.8  2018/09/21 13:12:19  marpas
	fixing paths. When it is relative to exe the base path is the parent of the directory containing the executable
	
	Revision 5.7  2018/09/04 16:16:04  marpas
	support for complete relocation of runtime project components added
	it uses new ProcessStatus singleton to discover executable path and dir, then
	compilation macros to build paths relative to the executable are provided
	
	Revision 5.6  2016/11/14 09:40:15  marfav
	Added default PID files permission flag DEFAULT_ACS_PID_PERMISSIONS
	
	Revision 5.5  2016/02/04 09:53:40  marpas
	useless const removed from declarations
	
	Revision 5.4  2015/06/10 09:03:54  tergem
	defaultMCLogPath added
	
	Revision 5.3  2014/08/26 10:30:18  marfav
	S3PDGS-1970: adding possibility to reconfigure log files permission at compile time
	
	Revision 5.2  2014/01/08 14:46:53  marfav
	Adding support to configurable core path
	
	Revision 5.1  2013/07/05 11:46:43  marpas
	introducing defaultLockPath() method and respective DEFAULT_ACS_LOCK_PATH define
	
	Revision 5.0  2013/06/04 15:35:00  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.6  2013/02/25 16:48:38  marpas
	DEFAULT_ACS_SMPLUGINS_FILENAME macro added defaulted to smplugins.conf
	defaultSMPluginsPath() function added
	defaultSMPluginsFileName() function added
	
	Revision 4.5  2012/06/12 17:37:00  marpas
	qacpp instrumentation
	
	Revision 4.4  2012/06/05 17:07:00  marpas
	resolving some quality issues and documentation
	
	Revision 4.3  2012/05/02 15:49:57  nicvac
	S1PDGS-3312: path override by makefile configuration.
	
	Revision 4.2  2012/03/01 13:08:42  marpas
	acs::base::defaultDbConfFileName() introduced
	
	Revision 4.1  2012/02/29 17:53:34  marpas
	base config concept introduced
	
	
	

*/


#include <acs_c++config.hpp>
#ifdef ACS_PATHS_EXE_REL
 #include <ProcessStatus.h>
 #include <string>
 #include <cstring>
 #include <libgen.h>
#endif

_ACS_BEGIN_NESTED_NAMESPACE(acs, base)

namespace { //unnamed
#ifdef ACS_PATHS_EXE_REL
    std::string get_rel_path(std::string const &r)
    {
        static std::string basedir="" ; // the static run time initialization is foreseen for the whole file
        if (basedir.empty()) { // first time
            std::string dir_ = acs::os::ProcessStatus::instance()->getExeDir() ; ; 
            basedir = std::string(dirname(const_cast<char *>(dir_.c_str()))) ; // NOSONAR - dirname wants char *
        }
        return basedir + r ;
    }
#endif
}

char const * defaultAcsPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
    // the directory father of the directory that contains that contains executable  
    static char const * const _defAcsPath = strdup(get_rel_path("").c_str()) ;
#else
    // the DEFAULT_ACS_PATH macro is intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
    static char const * const _defAcsPath = DEFAULT_ACS_PATH ;
#endif
    return _defAcsPath ;
    
}

char const * defaultConfPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_CONF_PATH
    #warning DEFAULT_ACS_CONF_PATH overrides the relative default conf path
    static char const * const _defConfPath = strdup(get_rel_path(DEFAULT_ACS_CONF_PATH).c_str()) ;
 #else
    static char const * const _defConfPath = strdup(get_rel_path("/conf").c_str()) ; 
 #endif
#else
    // the DEFAULT_ACS_PATH & DEFAULT_ACS_CONF_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_CONF_PATH
    #warning DEFAULT_ACS_CONF_PATH overrides the default conf path
    static char const * const _defConfPath = DEFAULT_ACS_CONF_PATH ;
 #else
    static char const * const _defConfPath = DEFAULT_ACS_PATH "/conf" ;
 #endif
#endif
    return _defConfPath ;
}

char const * defaultOptConfPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_OPT_CONF_PATH
    #warning DEFAULT_ACS_OPT_CONF_PATH overrides the relative default optional conf path
    static char const * const _defOptConfPath = strdup(get_rel_path(DEFAULT_ACS_OPT_CONF_PATH).c_str()) ;
 #else
    static char const * const _defOptConfPath = strdup(get_rel_path("/conf_opt").c_str()) ; 
 #endif
#else
    // the DEFAULT_ACS_PATH & DEFAULT_ACS_OPT_CONF_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_OPT_CONF_PATH
    #warning DEFAULT_ACS_OPT_CONF_PATH overrides the default conf path
    static char const * const _defOptConfPath = DEFAULT_ACS_OPT_CONF_PATH ;
 #else
    static char const * const _defOptConfPath = DEFAULT_ACS_PATH "/conf_opt" ;
 #endif
#endif
    return _defOptConfPath ;
}

char const * defaultDbConfPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_DBCONF_PATH
    #warning DEFAULT_ACS_DBCONF_PATH overrides the relative default dbconf path
    static char const * const _defDbConfPath = strdup(get_rel_path(DEFAULT_ACS_DBCONF_PATH).c_str()) ;
 #else
    static char const * const _defDbConfPath = strdup(get_rel_path("/conf/database").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_DBCONF_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_DBCONF_PATH
    #warning DEFAULT_ACS_DBCONF_PATH overrides the default dbconf path
    static char const * const _defDbConfPath = DEFAULT_ACS_DBCONF_PATH ;
 #else
    static char const * const _defDbConfPath = DEFAULT_ACS_PATH "/conf/database" ;
 #endif
#endif
    return _defDbConfPath ;
}

char const * defaultDbConfFileName() ACS_NOEXCEPT 
{
    // the DEFAULT_ACS_DBCONF_FILENAME macro is intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
    static char const * const _defDbConfFileName = DEFAULT_ACS_DBCONF_FILENAME ;
    return _defDbConfFileName ;   
}

char const * defaultDebugPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_DEBUG_PATH
   #warning DEFAULT_ACS_DEBUG_PATH overrides the relative default debug path
    static char const * const _defDebugPath = strdup(get_rel_path(DEFAULT_ACS_DEBUG_PATH).c_str()) ;
 #else
    static char const * const _defDebugPath = strdup(get_rel_path("/debug").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_DEBUG_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_DEBUG_PATH
    #warning DEFAULT_ACS_DEBUG_PATH overrides the default debug path
    static char const * const _defDebugPath = DEFAULT_ACS_DEBUG_PATH ;
 #else
    static char const * const _defDebugPath = DEFAULT_ACS_PATH "/debug" ;
 #endif
#endif
    return _defDebugPath ;
}

char const * defaultSMPluginsPath() ACS_NOEXCEPT 
{
    return defaultDbConfPath() ;
}

char const * defaultSMPluginsFileName() ACS_NOEXCEPT 
{
    // the DEFAULT_ACS_SMPLUGINS_FILENAME macro is intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
    static char const * const SMPluginsFileName = DEFAULT_ACS_SMPLUGINS_FILENAME ;
    return SMPluginsFileName ;   
}

char const * defaultStatusPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_STATUS_PATH
    #warning DEFAULT_ACS_STATUS_PATH overrides the relative default status path
    static char const * const _defStatusPath = strdup(get_rel_path(DEFAULT_ACS_STATUS_PATH).c_str()) ;
 #else
    static char const * const _defStatusPath = strdup(get_rel_path("/status").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_STATUS_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_STATUS_PATH
    #warning DEFAULT_ACS_STATUS_PATH overrides the default status path
    static char const * const _defStatusPath = DEFAULT_ACS_STATUS_PATH ;
 #else
    static char const * const _defStatusPath = DEFAULT_ACS_PATH "/status" ;
 #endif
#endif
    return _defStatusPath ;
}

char const * defaultLogPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_LOG_PATH
    #warning DEFAULT_ACS_LOG_PATH overrides the relative default log path
    static char const * const _defLogPath = strdup(get_rel_path(DEFAULT_ACS_LOG_PATH).c_str()) ;
 #else
    static char const * const _defLogPath = strdup(get_rel_path("/log").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_LOG_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_LOG_PATH
    #warning DEFAULT_ACS_LOG_PATH overrides the default log path
    static char const * const _defLogPath = DEFAULT_ACS_LOG_PATH ;
 #else
    static char const * const _defLogPath = DEFAULT_ACS_PATH "/log" ;
 #endif
#endif
    return _defLogPath ;
}
char const * defaultPersistencePath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_PERSISTENCE_PATH
    #warning DEFAULT_ACS_PERSISTENCE_PATH overrides the relative default log path
    static char const * const _defPersistencePath = strdup(get_rel_path(DEFAULT_ACS_PERSISTENCE_PATH).c_str()) ;
 #else
    static char const * const _defPersistencePath = strdup(get_rel_path("/persistence").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_PERSISTENCE_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_PERSISTENCE_PATH
    #warning DEFAULT_ACS_PERSISTENCE_PATH overrides the default log path
    static char const * const _defPersistencePath = DEFAULT_ACS_PERSISTENCE_PATH ;
 #else
    static char const * const _defPersistencePath = DEFAULT_ACS_PATH "/persistence" ;
 #endif
#endif
    return _defPersistencePath ;
}

char const * defaultMCLogPath() ACS_NOEXCEPT 
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_MC_ACS_LOG_PATH
    #warning DEFAULT_MC_ACS_LOG_PATH overrides the relative default log path
    static char const * const _defMCLogPath = strdup(get_rel_path(DEFAULT_MC_ACS_LOG_PATH).c_str()) ;
 #else
    static char const * const _defMCLogPath = strdup(get_rel_path("/log").c_str()) ;
 #endif
#else
    // the DEFAULT_MC_ACS_LOG_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_MC_ACS_LOG_PATH
    #warning DEFAULT_MC_ACS_LOG_PATH overrides the default log path
    static char const * const _defMCLogPath = DEFAULT_MC_ACS_LOG_PATH ;
 #else
    static char const * const _defMCLogPath = DEFAULT_ACS_PATH "/log" ;
 #endif
#endif
    return _defMCLogPath ;
}

char const * defaultBinPath() ACS_NOEXCEPT
{
#ifdef ACS_PATHS_EXE_REL
 #ifdef DEFAULT_ACS_BIN_PATH
    #warning DEFAULT_ACS_BIN_PATH overrides the relative default bin path
    static char const * const _defBinPath = strdup(get_rel_path(DEFAULT_ACS_BIN_PATH).c_str()) ;
 #else
    static char const * const _defBinPath = strdup(get_rel_path("/bin").c_str()) ;
 #endif
#else
    // the DEFAULT_ACS_BIN_PATH & DEFAULT_ACS_PATH macros are intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
 #ifdef DEFAULT_ACS_BIN_PATH
    #warning DEFAULT_ACS_BIN_PATH overrides the default bin path
    static char const * const _defBinPath = DEFAULT_ACS_BIN_PATH ;
 #else
    static char const * const _defBinPath = DEFAULT_ACS_PATH "/bin" ;
 #endif
#endif
    return _defBinPath ;
}

char const * defaultLockPath() ACS_NOEXCEPT
{
    // the DEFAULT_ACS_LOCK_PATH is intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
    static char const * const _defLockPath = DEFAULT_ACS_LOCK_PATH ;
    return _defLockPath ;
}

char const * defaultCorePath() ACS_NOEXCEPT
{
    // the DEFAULT_ACS_CORE_PATH is intended to be replaced via a compiler define 
    // hence cannot be declared as a const std::string 
    static char const * const _defCorePath = DEFAULT_ACS_CORE_PATH ;
    return _defCorePath ;
}

mode_t defaultLogPermissions() ACS_NOEXCEPT
{
	static mode_t const _defLogPermissions = mode_t(DEFAULT_ACS_LOG_PERMISSIONS) ; // NOSONAR
	return _defLogPermissions;
} 

mode_t defaultPidPermissions() ACS_NOEXCEPT
{
	static mode_t const _defPidPermissions = mode_t(DEFAULT_ACS_PID_PERMISSIONS) ; // NOSONAR
	return _defPidPermissions;
} 
mode_t defaultUuidPermissions() ACS_NOEXCEPT
{
	static mode_t const _defUuidPermissions = mode_t(DEFAULT_ACS_UUID_PERMISSIONS) ; // NOSONAR
	return _defUuidPermissions;
} 

_ACS_END_NESTED_NAMESPACE
