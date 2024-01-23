// PRQA S 1050 EOF
/*

	Copyright 2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: SRV Metrics Library $


*/

#ifndef _MonthlyMetricsHandler_H_
#define _MonthlyMetricsHandler_H_

#include <exMacros.h>
#include <exException.h>
#include <Filterables.h>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include <dbConnection.h>

_ACS_BEGIN_NAMESPACE(acs)

class BaseMetricsHandler;
class MetricsHandlerFactory;

class MonthlyMetricsHandler : public BaseMetricsHandler {
public:
	friend class MetricsHandlerFactory;
	
	exDECLARE_EXCEPTION(MonthlyMetricsHandlerException, exException);

	virtual ~MonthlyMetricsHandler() noexcept = default;
	
	/**
	 * Compute the metrics in the interval [ref - 30days, ref]
	 */
	void computeAndStore(dbConnection& conn, const DateTime& ref = DateTime());

protected:
	MonthlyMetricsHandler();

private:
	void loadMetrics(const std::string& groupByFunc, const std::string& sqlLikeNamePatt, const DateTime& start, 
						const DateTime& stop, std::map<std::string, float>& metric, dbConnection& conn);
	void storeMetrics(const std::map<std::string, float>& mets, const DateTime& ref, dbConnection& conn);
private:
	MonthlyMetricsHandler(const MonthlyMetricsHandler&)               = delete;
	MonthlyMetricsHandler(const MonthlyMetricsHandler&&)              = delete;
	MonthlyMetricsHandler& operator=(const MonthlyMetricsHandler&)    = delete;
	MonthlyMetricsHandler& operator=(const MonthlyMetricsHandler&&)   = delete;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MonthlyMetricsHandler);
};

_ACS_END_NAMESPACE

#endif /* _MonthlyMetricsHandler_H_ */
