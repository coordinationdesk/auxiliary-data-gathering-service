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

#include <LTAInventoryMetricsTrackerConfig.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAInventoryMetricsTrackerConfig);

const std::string LTAInventoryMetricsTrackerConfig::RESTART_TIME_TAG = "LTAInventoryMetricsTracker.RestartTime";
const std::string LTAInventoryMetricsTrackerConfig::RESTART_ENABLED_TAG = "LTAInventoryMetricsTracker.RestartEnabled";
const std::string LTAInventoryMetricsTrackerConfig::INSTANCE_TAG = "LTAInventoryMetricsTracker.Instance";
const std::string LTAInventoryMetricsTrackerConfig::WORKING_DIR_TAG = "LTAInventoryMetricsTracker.WorkingDir";
const std::string LTAInventoryMetricsTrackerConfig::QUERY_BLOCK_SIZE_TAG = "LTAInventoryMetricsTracker.QueryBlockSize";
const std::string LTAInventoryMetricsTrackerConfig::WAIT_INTERVAL_TAG = "LTAInventoryMetricsTracker.WaitInterval";
const std::string LTAInventoryMetricsTrackerConfig::INVENTORY_TAIL_REACHED_WAIT_INTERVAL_TAG = "LTAInventoryMetricsTracker.InventoryTailReachedWaitInterval";


LTAInventoryMetricsTrackerConfig::LTAInventoryMetricsTrackerConfig(const rsResourceSet& rset) :
	rset_(rset)
{ }

LTAInventoryMetricsTrackerConfig::~LTAInventoryMetricsTrackerConfig() noexcept { }

unsigned long LTAInventoryMetricsTrackerConfig::getRestartTime() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool found = false;
	unsigned long val = defaultRestartTime;
	getKey<unsigned long>(RESTART_TIME_TAG, val, &found);
	return val;
}

bool LTAInventoryMetricsTrackerConfig::isRestartEnabled() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	bool found = false;
	bool val = defaultRestartEnabled;
	getKey<bool>(RESTART_ENABLED_TAG, val, &found);
	return val;
}

std::string LTAInventoryMetricsTrackerConfig::getInstance(bool* found) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	getKey<std::string>(INSTANCE_TAG, val, found);
	return val;
}

std::string LTAInventoryMetricsTrackerConfig::getWorkingDir(bool* found) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string val;
	getKey<std::string>(WORKING_DIR_TAG, val, found);
	return val;
}

size_t LTAInventoryMetricsTrackerConfig::getQueryBlockSize(bool* found) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	size_t val = 0;
	getKey<size_t>(QUERY_BLOCK_SIZE_TAG, val, found);
	return val;
}

unsigned long LTAInventoryMetricsTrackerConfig::getWaitInterval(bool* found) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unsigned long val = 0;
	getKey<unsigned long>(WAIT_INTERVAL_TAG, val, found);
	return val;
}

unsigned long LTAInventoryMetricsTrackerConfig::getInventoryTailReachedWaitInterval(bool* found) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	unsigned long val = 0;
	getKey<unsigned long>(INVENTORY_TAIL_REACHED_WAIT_INTERVAL_TAG, val, found);
	return val;
}

void LTAInventoryMetricsTrackerConfig::show() const {
	bool found = false;
	
	std::ostringstream ss;
	ss << "Loaded configuration:\n";
	ss << "    RestartTime:                " << getRestartTime() << "\n";
	ss << "    RestartEnabled:             " << std::boolalpha << isRestartEnabled() << "\n";
	ss << "    Instance:                   " << getInstance() << "\n";
	
	std::string str = getWorkingDir(&found);
	if (found) {
		ss << "    WorkingDir:                 " << str << "\n";
	}
	size_t query = getQueryBlockSize(&found);
	if (found) {
		ss << "    QueryBlockSize:             " << query << "\n";
	}
	unsigned long wait1 = getWaitInterval(&found);
	if (found) {
		ss << "    WaitInterval:               " << wait1 << "\n";
	}
	unsigned long wait2 = getInventoryTailReachedWaitInterval(&found);
	if (found) {
		ss << "    InventoryTailWaitInterval:  " << wait2 << "\n";
	}
	
	ACS_LOG_INFO(ss.str());
}


_ACS_END_NAMESPACE
