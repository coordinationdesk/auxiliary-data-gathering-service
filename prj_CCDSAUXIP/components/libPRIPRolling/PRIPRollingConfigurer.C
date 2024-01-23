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

    PRIPRolling Library

*/ 

#include <PRIPRollingConfigurer.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(PRIPRollingConfigurer);


const std::string PRIPRollingConfigurer::PRIP_ROLLING_DATABASE_POLLING_TIME_TAG = "PRIPRolling.DatabasePollingTime";
const std::string PRIPRollingConfigurer::PRIP_ROLLING_RESTART_TIME_TAG = "PRIPRolling.RestartTime";
const std::string PRIPRollingConfigurer::PRIP_ROLLING_MAXITEMS_TAG = "PRIPRolling.MaxItems";
const std::string PRIPRollingConfigurer::PRIP_ROLLING_REMOVE_RECORD_TAG = "PRIPRolling.RemoveRecord";


const size_t PRIPRollingConfigurer::defaultDbPollingTime = 5000;
const size_t PRIPRollingConfigurer::defaultRestartTime = 86400;
const size_t PRIPRollingConfigurer::defaultMaxItems = 50;
const bool PRIPRollingConfigurer::defaultRemoveRecord = true;

/* Special functions */
PRIPRollingConfigurer::PRIPRollingConfigurer(const rsResourceSet& rset) :
	rset_(rset)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

PRIPRollingConfigurer::PRIPRollingConfigurer(const PRIPRollingConfigurer& c) :
	rset_(c.rset_)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

PRIPRollingConfigurer& PRIPRollingConfigurer::operator=(const PRIPRollingConfigurer& c) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (this != &c) {
		rset_ = c.rset_;
	}
	return *this;
}

 PRIPRollingConfigurer::~PRIPRollingConfigurer() noexcept {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/* Getters */
size_t PRIPRollingConfigurer::getDatabasePollingTime(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	size_t val = defaultDbPollingTime;
	bool found = rset_.hasKey(PRIP_ROLLING_DATABASE_POLLING_TIME_TAG);
	if (found) {
		rset_.getValue(PRIP_ROLLING_DATABASE_POLLING_TIME_TAG, val);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_DATABASE_POLLING_TIME_TAG << "\" found. Loaded value " << val);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_DATABASE_POLLING_TIME_TAG << "\" not found on configuration. Using default " << val);
	}
	
	if (0 != set) {
		*set = found;
	}

	return val;
	
}

size_t PRIPRollingConfigurer::getRestartTime(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	size_t val = defaultRestartTime;
	bool found = rset_.hasKey(PRIP_ROLLING_RESTART_TIME_TAG);
	if (found) {
		rset_.getValue(PRIP_ROLLING_RESTART_TIME_TAG, val);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_RESTART_TIME_TAG << "\" found. Loaded value " << val);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_RESTART_TIME_TAG << "\" not found on configuration. Using default " << val);
	}
	
	if (0 != set) {
		*set = found;
	}

	return val;
}

size_t PRIPRollingConfigurer::getMaxItems(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	size_t val = defaultMaxItems;
	bool found = rset_.hasKey(PRIP_ROLLING_MAXITEMS_TAG);
	if (found) {
		rset_.getValue(PRIP_ROLLING_MAXITEMS_TAG, val);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_MAXITEMS_TAG << "\" found. Loaded value " << val);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_MAXITEMS_TAG << "\" not found on configuration. Using default " << val);
	}
	
	if (0 != set) {
		*set = found;
	}

	return val;
}

bool PRIPRollingConfigurer::getRemoveRecord(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool val = defaultRemoveRecord;
	bool found = rset_.hasKey(PRIP_ROLLING_REMOVE_RECORD_TAG);
	if (found) {
		rset_.getValue(PRIP_ROLLING_REMOVE_RECORD_TAG, val);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_REMOVE_RECORD_TAG << "\" found. Loaded value " << val);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tag \"" << PRIP_ROLLING_REMOVE_RECORD_TAG << "\" not found on configuration. Using default " << val);
	}
	
	if (0 != set) {
		*set = found;
	}

	return val;
}

void PRIPRollingConfigurer::show() const {
	std::ostringstream ss;
	ss << "\n****************************************************\n";
	ss << "PRIPRolling Configuration:\n";
	ss << "    - DatabasePollingTime[msec]......" << getDatabasePollingTime() << "\n";
	ss << "    - RestartTime[secs].............." << getRestartTime() << "\n";
	ss << "    - MaxItems......................." << getMaxItems() << "\n";
	ss << "    - RemoveRecord..................." << std::boolalpha << getRemoveRecord() << "\n";
	ss << "****************************************************\n";
	ACS_LOG_PRIVINFO(ss.str());
}

_ACS_END_NAMESPACE
