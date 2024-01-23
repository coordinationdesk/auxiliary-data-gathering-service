// PRQA S 1050 EOF
/*

	Copyright 2018-2021, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. MQTT Statistics Library
*/

#include <StatEventNotifyFactory.h>
#include <StatisticsNotifier.h>

#include <ConfigurationSingleton.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(StatEventNotifyFactory);

StatEventNotifyFactory::StatEventNotifyFactory() :
	EventNotifyFactory(), // Register the factory
    _isActive(true)
{
	ConfigurationSingleton::instance()->addCallback(StatisticsNotifier::onConfigLoaded);
	ConfigurationSingleton::instance()->addSpace(ConfigurationSingleton::ConfSpace("libMQTTStatistics", "", false));
}

StatEventNotifyFactory::~StatEventNotifyFactory() ACS_NOEXCEPT { }

/*
 * Returns the instance of the StatisticsNotifier singleton
 */
EventNotifier* StatEventNotifyFactory::createItem() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // take the lock on _isActive flag
	if (_isActive)
    {
        // This factory returns a singleton
        return StatisticsNotifier::instance();
    }
    // In case is not active... simply return 0 to avoid further interactions
    return 0;
}

void StatEventNotifyFactory::shutdown() 
{
    // prevent any other create item fron now on
    _isActive=false;
    // force the shutdown 
    StatisticsNotifier::instance()->forceExit();
}

bool StatEventNotifyFactory::safeToExit() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // ask the StatisticsNotifier 
    return StatisticsNotifier::instance()->safeToExit();
}

namespace {
	StatEventNotifyFactory factory;
}

_ACS_END_NAMESPACE
