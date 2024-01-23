// PRQA S 1050 EOF
/*
	Copyright 2014-2018, Advanced Computer Systems , Inc.
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

#include <sys/types.h> // PRQA S 1013 2
#include <sys/stat.h>
#include <climits>
#include <cstring>
#include <unistd.h>
#include <libgen.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, os)


namespace {
    mode_t get_umask() // PRQA S 2502
    { 
        mode_t m = mode_t(0) ;  // PRQA S 3081 2
        umask(m=umask(mode_t(0))) ;  
        return m ; 
    }

    using std::string ;
    
    string get_exepath() 
    {
        char buf[PATH_MAX] ;
        buf[0] = 0 ;
        pid_t const pid = getpid() ;
        std::ostringstream path ; 
        path << "/proc/" << pid << "/exe" ;
        // readlink will return an absolute canonicalized pathname.
        ssize_t const ret = readlink(path.str().c_str(), buf, sizeof(buf)) ;
        if (ret >= 0) {
            buf[ret] = '\0' ;
        }
        return string(buf) ; // PRQA S 2839, 3081 # see above termination
    }

}

using std::string ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProcessStatus) 

ProcessStatus::~ProcessStatus() ACS_NOEXCEPT {}
			
ProcessStatus::ProcessStatus() : 
    pattern::Singleton<ProcessStatus>(),
    startupUmask_(get_umask()),
    exePath_(get_exepath())
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "umask: " << std::setw(3) << std::setfill('0') << std::right << std::hex << startupUmask_) ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "  exe: " << exePath_) ;

}
mode_t ProcessStatus::getStartupUmask() const ACS_NOEXCEPT { return startupUmask_ ; } // PRQA S 4120

const string & ProcessStatus::getExePath() const ACS_NOEXCEPT { return exePath_ ; } // PRQA S 4120


string ProcessStatus::getExeDir() const
{ 
    char dir[getExePath().length()+3] ; // PRQA S 3084, 4403, 0154
    strcpy(dir, getExePath().c_str()) ;
    return string(dirname(dir)) ; // PRQA S 3081
}


namespace {
    class PStatusInitializer {
    public:
        PStatusInitializer() ACS_NOEXCEPT { 
            try { ProcessStatus::instance() ; } catch (std::exception &) {}
        }
    } pstatusInitializer_ ;
}

_ACS_END_NESTED_NAMESPACE

