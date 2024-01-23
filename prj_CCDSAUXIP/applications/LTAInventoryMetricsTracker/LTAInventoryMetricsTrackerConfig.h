// PRQA S 1050 EOF
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

#ifndef _LTAInventoryMetricsTrackerConfig_H_
#define _LTAInventoryMetricsTrackerConfig_H_

#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)

class LTAInventoryMetricsTrackerConfig {
public:
	exDECLARE_EXCEPTION(LTAInventoryMetricsTrackerConfigException, exException);

	LTAInventoryMetricsTrackerConfig(const rsResourceSet& rset);
	virtual ~LTAInventoryMetricsTrackerConfig() noexcept;
	
	unsigned long getRestartTime() const;
	bool isRestartEnabled() const;
	std::string getInstance(bool* found = 0) const;
	std::string getWorkingDir(bool* found = 0) const;
	size_t getQueryBlockSize(bool* found = 0) const;
	unsigned long getWaitInterval(bool* found = 0) const;
	unsigned long getInventoryTailReachedWaitInterval(bool* found = 0) const;
	
	void show() const;
private:
	LTAInventoryMetricsTrackerConfig()                                                       = delete;
	LTAInventoryMetricsTrackerConfig(const LTAInventoryMetricsTrackerConfig&)                = delete;
	LTAInventoryMetricsTrackerConfig(const LTAInventoryMetricsTrackerConfig&&)               = delete;
	LTAInventoryMetricsTrackerConfig& operator=(const LTAInventoryMetricsTrackerConfig&)     = delete;
	LTAInventoryMetricsTrackerConfig& operator=(const LTAInventoryMetricsTrackerConfig&&)    = delete;
	
	template <typename T>
	void getKey(const std::string& key, T& val, bool* found = 0) const {
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
		bool exists = false;
		if (rset_.hasKey(key)) {
			rset_.getValue<T>(key, val);
			exists = true;
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Found value " << val << " for tag " << key);
		}
		
		if (0 != found) {
			*found = exists;
		} else {
			ACS_COND_THROW(!exists, LTAInventoryMetricsTrackerConfigException("Tag " + key + " not found"));
		}
	}
	
private:
	static const std::string RESTART_TIME_TAG;
	static const std::string RESTART_ENABLED_TAG;
	static const std::string INSTANCE_TAG;
	static const std::string WORKING_DIR_TAG;
	static const std::string QUERY_BLOCK_SIZE_TAG;
	static const std::string WAIT_INTERVAL_TAG;
	static const std::string INVENTORY_TAIL_REACHED_WAIT_INTERVAL_TAG;

	static const unsigned long defaultRestartTime = 86400;
	static const bool defaultRestartEnabled = true;

private:
	rsResourceSet rset_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAInventoryMetricsTrackerConfig);
};

_ACS_END_NAMESPACE

#endif /* _LTAInventoryMetricsTrackerConfig_H_ */
