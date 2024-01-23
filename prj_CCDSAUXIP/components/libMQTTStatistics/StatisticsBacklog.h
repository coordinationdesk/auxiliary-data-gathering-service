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

#ifndef _StatisticsBacklog_H_
#define _StatisticsBacklog_H_

#include <InfluxDbStatisticPoint.h>

_ACS_BEGIN_NAMESPACE(acs)

class StatisticsBacklog {
public:
	StatisticsBacklog();
	virtual ~StatisticsBacklog() ACS_NOEXCEPT;

	void pushStat(const InfluxDbStatisticPoint&);
	InfluxDbStatisticPoint popStat();
	void clear();
	
	InfluxDbStatisticPoint front() const;
	InfluxDbStatisticPoint back() const;
	StatisticsBacklog* clone() const;
	bool empty() const;
	size_t size() const;

	
private:
	StatisticsBacklog(const StatisticsBacklog&)            = delete;
	StatisticsBacklog& operator=(const StatisticsBacklog&) = delete;
private:
	std::list<InfluxDbStatisticPoint> backlog_;
	size_t size_;
	Mutex mutex_;
};

_ACS_END_NAMESPACE

#endif /* _StatisticsBacklog_H_ */
