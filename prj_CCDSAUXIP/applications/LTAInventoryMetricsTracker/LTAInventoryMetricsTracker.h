/*

    Copyright 1995-2021, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    LTAInventoryMetricsTracker app

*/ 

#ifndef _LTAInventoryMetricsTracker_H_
#define _LTAInventoryMetricsTracker_H_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#include <ProjectAppIncludes>

#include <acs_c++config.hpp>

#include <InventoryTotalMetrics.h>
#include <LTAInventoryMetricsTrackerConfig.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class LTAInventoryMetricsTracker : public PROJECT_APP {
	
	
public:

	/**
	 * Default constructor
	 */
	LTAInventoryMetricsTracker(const std::string& appname, const std::string& appsubsys);

	/**
	 * Destructor
	 */
	virtual ~LTAInventoryMetricsTracker() ACS_NOEXCEPT;

	/**
	 * The main function
	 */
	virtual int main(int argc, char const * const * argv, char const * const * env);
	
	virtual std::string getDefaultLogName() const;

private:
	// Defined but not implemented
	LTAInventoryMetricsTracker();
	LTAInventoryMetricsTracker(const LTAInventoryMetricsTracker&);
	LTAInventoryMetricsTracker& operator=(const LTAInventoryMetricsTracker&);
	
	void init(const LTAInventoryMetricsTrackerConfig& config);
	
	static const unsigned long defaultWaitInterval = 10;
	static const unsigned long defaultWaitIntervalAfterTailReached = 120;
	
private:
	InventoryTotalMetrics metrics_;
	std::string appInstance_;
	unsigned long waitInterval_;
	unsigned long waitIntervalAfterTailReached_;
	pthread_t mainThread_;
	
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAInventoryMetricsTracker);
};

_ACS_END_NAMESPACE

#endif /* _LTAInventoryMetricsTracker_H_ */
