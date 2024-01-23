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

#ifndef _MetricsDbFunctions_H_
#define _MetricsDbFunctions_H_

#include <exMacros.h>
#include <exException.h>
#include <Filterables.h>
#include <dbConnection.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * \brief This class encapsulate all the common functionalities
 * used to handle metrics on database.
 */ 
class MetricsDbFunctions {
public:
	exDECLARE_EXCEPTION(MetricsFunctionsException, exException);
	
	/**
	 * \brief This functions deletes all the metrics from t_srv_metrics older than the input aging in seconds
	 */
	static bool cleanupAgedMetrics(unsigned long long secs, dbConnection& conn);
	
private:
	static std::string getNowSql(const std::string& dbKey);
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MetricsDbFunctions);
};

_ACS_END_NAMESPACE

#endif /* _MetricsDbFunctions_H_ */
