// PRQA S 1050 EOF
/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    PRIPRolling

*/ 

#ifndef _PRIPRolling_h_
#define _PRIPRolling_h_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

class PRIPRolling : public PROJECT_APP {
public:
	/** Deleted special functions */
	PRIPRolling()                                   = delete;
	PRIPRolling(const PRIPRolling&)                 = delete;
	PRIPRolling& operator=(const PRIPRolling&)      = delete;
	PRIPRolling(PRIPRolling&&) noexcept             = delete;
	PRIPRolling& operator=(PRIPRolling&&) noexcept  = delete;

	/** Ctor / Dtor */
	PRIPRolling(const std::string& appname, const std::string& appsubsys);
	virtual ~PRIPRolling() ACS_NOEXCEPT;

	/** Main function */
	virtual int main(int argc, char const * const * argv, char const * const * env);
	
	virtual std::string getDefaultLogName() const;

private:
	pthread_t _mainThread;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PRIPRolling);
};

_ACS_END_NAMESPACE
#endif /* _PRIPRolling_h_ */
