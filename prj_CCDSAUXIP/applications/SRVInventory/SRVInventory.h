/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRVInventory app

*/ 


#ifndef _SRVInventory_H_
#define _SRVInventory_H_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <acs_c++config.hpp>
#include <LTAFileInventory.h>

_ACS_BEGIN_NAMESPACE(acs)

class SRVInventory : public PROJECT_APP {
public:

	exDECLARE_EXCEPTION(SRVInventoryException, exException);
	exDECLARE_EXCEPTION(SRVInventoryCleanStopException, SRVInventoryException);
	exDECLARE_EXCEPTION(SRVInventoryImmediateStopException, SRVInventoryException);

	/** Deleted special functions */
	SRVInventory()                                 = delete;
	SRVInventory(const SRVInventory&)              = delete;
	SRVInventory& operator=(const SRVInventory&)   = delete;
	SRVInventory(SRVInventory&&)                   = delete;
	SRVInventory& operator=(SRVInventory&&)        = delete;

	/** Defaulted special functions */
	virtual ~SRVInventory() noexcept               = default;

	/** Ctor/Dtor */
	SRVInventory(const std::string& appname, const std::string& appsubsys);

	/** Main function */
	virtual int main(int argc, char const * const * argv, char const * const * env);
	virtual void customSigTermHandler();
	
	/** SRVInventory is a processor, the logname is generated always in the current directory */
	virtual std::string getDefaultLogName() const;
	virtual int usage(const std::string &exename) const;
	
private:
	void performInventory(const std::string& joborder, bool forceNotValid = false);
	
private:
	pthread_t mainThread_;
	lta::LTAFileInventory* fileInventory_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVInventory);
};

_ACS_END_NAMESPACE

#endif /* _SRVInventory_H_ */
