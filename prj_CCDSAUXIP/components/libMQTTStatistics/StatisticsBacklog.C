// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. MQTT Statistics Library
*/

#include <StatisticsBacklog.h>

_ACS_BEGIN_NAMESPACE(acs)

StatisticsBacklog::StatisticsBacklog() :
	backlog_(),
	size_(),
	mutex_()
{ }

StatisticsBacklog::~StatisticsBacklog() ACS_NOEXCEPT { }

void StatisticsBacklog::pushStat(const InfluxDbStatisticPoint& pnt) {
	Lock l(mutex_);
	backlog_.push_back(pnt);
	++size_;
}

InfluxDbStatisticPoint StatisticsBacklog::front() const {
	Lock l(mutex_);
	ACS_COND_THROW(backlog_.empty(), exIllegalValueException("No stats in backlog"));
	return backlog_.front();
}

InfluxDbStatisticPoint StatisticsBacklog::back() const {
	Lock l(mutex_);
	ACS_COND_THROW(backlog_.empty(), exIllegalValueException("No stats in backlog"));
	return backlog_.back();
}

InfluxDbStatisticPoint StatisticsBacklog::popStat() {
	Lock l(mutex_);

	ACS_COND_THROW(backlog_.empty(), exIllegalValueException("No stats in backlog"));

	InfluxDbStatisticPoint pnt = backlog_.front();
	backlog_.pop_front();
	--size_;
	return pnt;
}

bool StatisticsBacklog::empty() const {
	Lock l(mutex_);
	return backlog_.empty();
}

size_t StatisticsBacklog::size() const {
	Lock l(mutex_);
	
	// Do not use size() function defined in std::list!
	// The size of the backlog must be managed separately from std::list
	// because the function is very inefficient on RHEL 6 and RHEL 7 (performs a scan! => O(N))
	return size_;
}

void StatisticsBacklog::clear() {
	Lock l(mutex_);
	backlog_.clear();
	size_ = 0;
}

StatisticsBacklog* StatisticsBacklog::clone() const {
	StatisticsBacklog* cloned = new StatisticsBacklog();

	Lock l(mutex_);
	cloned->backlog_ = backlog_;
	cloned->size_ = size_;
	return cloned;
}

_ACS_END_NAMESPACE
