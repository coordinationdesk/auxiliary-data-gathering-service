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

#include <MetricsHandlerFactory.h>
#include <DailyMetricsHandler.h>
#include <MonthlyMetricsHandler.h>
#include <StringUtils.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MetricsHandlerFactory) ;

std::string MetricsHandlerFactory::metricsHandlerTypeToString(MetricsHandlerType type) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	std::string str;
	switch (type) {
	case DAILY:
		str = "DAILY";
		break;
	case MONTHLY:
		str = "MONTHLY";
		break;
	default:
		ACS_THROW(exIllegalValueException("Unknown metrics handler type"));
	};
	
	return str;
}

MetricsHandlerFactory::MetricsHandlerType MetricsHandlerFactory::stringToMetricsHandlerType(const std::string& str) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	MetricsHandlerType type;
	if (StringUtils::equalsNoCase(str, "DAILY")) {
		type = DAILY;
	} else if (StringUtils::equalsNoCase(str, "MONTHLY")) {
		type = MONTHLY;
	} else {
		ACS_THROW(exIllegalValueException("Unknown metrics handler string " + str));
	}
	
	return type;
}

BaseMetricsHandler* MetricsHandlerFactory::createItem(MetricsHandlerFactory::MetricsHandlerType type) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	BaseMetricsHandler* ptr = 0;
	switch (type) {
	case DAILY:
		ptr = new DailyMetricsHandler();
		break;
	case MONTHLY:
		ptr = new MonthlyMetricsHandler();
		break;
	default:
		ACS_THROW(exIllegalValueException("Unknown metrics handler type"));
	};
	
	return ptr;
}

_ACS_END_NAMESPACE
