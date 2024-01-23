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

#ifndef _MetricsHandlerFactory_H_
#define _MetricsHandlerFactory_H_

#include <BaseMetricsHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

class MetricsHandlerFactory {
public:

	typedef enum {
		DAILY = 0,
		MONTHLY
	} MetricsHandlerType;

	static std::string metricsHandlerTypeToString(MetricsHandlerType);
	static MetricsHandlerType stringToMetricsHandlerType(const std::string&);

public:
	MetricsHandlerFactory()                   = default;
	virtual ~MetricsHandlerFactory() noexcept = default;
	
	static BaseMetricsHandler* createItem(MetricsHandlerType);

private:
	MetricsHandlerFactory(const MetricsHandlerFactory&)                = delete;
	MetricsHandlerFactory(const MetricsHandlerFactory&&)               = delete;
	MetricsHandlerFactory& operator=(const MetricsHandlerFactory&)     = delete;
	MetricsHandlerFactory& operator=(const MetricsHandlerFactory&&)    = delete;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MetricsHandlerFactory) ;

};

_ACS_END_NAMESPACE

#endif /* _MetricsHandlerFactory_H_ */
