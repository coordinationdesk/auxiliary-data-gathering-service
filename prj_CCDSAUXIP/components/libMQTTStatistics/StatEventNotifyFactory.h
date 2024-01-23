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

#ifndef _StatEventNotifyFactory_H_
#define _StatEventNotifyFactory_H_

#include <EventNotifyFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * Factory class that is able to create instances of StatisticsNotifier singleton.
 */
class StatEventNotifyFactory : public EventNotifyFactory {
public:
	StatEventNotifyFactory();
	virtual ~StatEventNotifyFactory() ACS_NOEXCEPT;

	StatEventNotifyFactory(const StatEventNotifyFactory&) = delete;
	StatEventNotifyFactory& operator=(const StatEventNotifyFactory&) = delete;

	StatEventNotifyFactory(StatEventNotifyFactory&&) = delete;
	StatEventNotifyFactory& operator=(StatEventNotifyFactory&&) = delete;
    
    virtual void shutdown();
    virtual bool safeToExit();

protected:
	/**
	 * Returns an instance of the StatisticsNotifier singleton
	 */
	virtual EventNotifier* createItem();

private:
    bool _isActive;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StatEventNotifyFactory);
};

_ACS_END_NAMESPACE

#endif /* _StatEventNotifyFactory_H_ */
