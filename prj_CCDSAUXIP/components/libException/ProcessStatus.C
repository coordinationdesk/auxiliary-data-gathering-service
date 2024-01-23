/*
	Copyright 2014-2022 Exprivia, SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.
	
	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2018/09/27 13:25:10  marpas
	missing include file
	
	Revision 5.2  2018/09/04 15:53:43  marpas
	getExePath and getExeDir methods added
	
	Revision 5.1  2014/06/27 10:01:57  marpas
	ProcessStatus singleton added
	it will be initialized at process startup and, for the moment stores the current umask
	
	

*/

#include <ProcessStatus.h>
#include <Filterables.h>

#include <sys/types.h> 
#include <sys/stat.h>
#include <climits>
#include <cstring>
#include <unistd.h>
#include <libgen.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)


namespace {
    mode_t get_umask() noexcept
    { 
        mode_t m = umask(0) ; // NOSONAR - get actual mode
        umask(m) ;  // set actual mode again
        return m ; 
    }
    
    using std::string ;
    
    string get_exepath() noexcept
    {
        std::array<char,PATH_MAX> buf ; // NOSONAR 
        buf[0] = 0 ;
        pid_t const pid = getpid() ;
        std::ostringstream path ; 
        path << "/proc/" << pid << "/exe" ;
        // readlink will return an absolute canonicalized pathname.
        ssize_t const ret = readlink(path.str().c_str(), buf.begin(), PATH_MAX) ;
        if (ret >= 0) {
            buf[ret] = '\0' ;
        }
        return string(buf.cbegin()) ; // PRQA S 2839, 3081 # see above termination
    }

}

using std::string ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProcessStatus) 
			
ProcessStatus::ProcessStatus() noexcept : 
    pattern::Singleton<ProcessStatus>(),
    startupUmask_(get_umask()), // NOSONAR - dinamically computed
    exePath_(get_exepath()) // NOSONAR - dinamically computed
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "umask: " << std::setw(3) << std::setfill('0') << std::right << std::hex << startupUmask_) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "  exe: " << exePath_) ;

}
mode_t ProcessStatus::getStartupUmask() const noexcept { return startupUmask_ ; } // PRQA S 4120

const string & ProcessStatus::getExePath() const noexcept { return exePath_ ; } // PRQA S 4120


string ProcessStatus::getExeDir() const
{ 
    char dir[getExePath().length()+3] ; // NOSONAR 
    strcpy(dir, getExePath().c_str()) ; // NOSONAR -- dir is of right size
    return string(dirname(dir)) ; // PRQA S 3081
}


namespace {
    class PStatusInitializer {
    public:
        PStatusInitializer() noexcept { 
            try {
                ProcessStatus::instance() ; 
            } catch (const std::exception &x) {
                ACS_LOG_NOTIFY_EX(x) ;
            }
        }
    } ; 
    const PStatusInitializer pstatusInitializer_ ; // just to stimulate the ProcessStatus singleton
}

_ACS_END_NESTED_NAMESPACE

