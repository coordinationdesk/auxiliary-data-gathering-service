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

#ifndef _StatisticsNotifier_H_
#define _StatisticsNotifier_H_

#include <MQTTPublisher.h>
#include <EventNotifyFactory.h>
#include <InfluxDbStatisticPoint.h>
#include <MQTTStatPublisherThread.h>
#include <Application.h>
#include <exMacros.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

// This object must be a singleton
class StatisticsNotifier : public EventNotifier, public Singleton<StatisticsNotifier> {
public:
	exDECLARE_EXCEPTION(StatisticsNotifierException, exException);

	/**
	 * This is the configuration space callback. Initializes the MQTTPublisher and
	 * publish stored statistics
	 */
	static void onConfigLoaded(void* v);
	
public:
	virtual ~StatisticsNotifier() ACS_NOEXCEPT;

	StatisticsNotifier(const StatisticsNotifier&) = delete;
	StatisticsNotifier& operator=(const StatisticsNotifier&) = delete;
	StatisticsNotifier(StatisticsNotifier&&) = delete;
	StatisticsNotifier& operator=(StatisticsNotifier&&) = delete;
    
	Mutex& getMutex() { return _mutex; }
	
	void startPublisherThread();
	
	// Inherited from EventNotifier
	virtual void notifyStatistics(const std::string& statNodePath, int);
	virtual void notifyStatistics(const std::string& statNodePath, long);
	virtual void notifyStatistics(const std::string& statNodePath, long long);
	virtual void notifyStatistics(const std::string& statNodePath, size_t);
	virtual void notifyStatistics(const std::string& statNodePath, float);
	virtual void notifyStatistics(const std::string& statNodePath, double);
	virtual void notifyStatistics(const std::string& statNodePath, long double);
	virtual void notifyStatistics(const std::string& statNodePath, const std::string& item);
	
	template <typename T>
	void notifyStatisticsToInfluxDb(const std::string& statNodePath, const T& item) {
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
		// catch all
		try {
			if (_publishEnabled) {
				std::ostringstream sspid;
				sspid << getpid();
				
				InfluxDbStatisticPoint pnt;
				pnt.addTagSet("statnodepath", statNodePath);
				pnt.addTagSet("hostname", Application::hostname());
				pnt.addTagSet("pid", sspid.str());
				pnt.addFieldSet<T>("value", item);
				
				// If the thread is not running and the configuration has been loaded, try to start the thread
				// This can happen only if another EventNotifier has been registered to the EventNotifyFactory
				// preventing the configuration callback to start the thread upon configuration load
				// in case the new EventNotifier de-registers it may happen that a new event is notified through
				// this class. In this case the thread shall be started.
				if (!_statPublisherThread->running() && MQTTStatPublisherThread::getConfigLoadedFlag()) {
					startPublisherThread();
				}

				bool enqueued = _statPublisherThread->pushStatPoint(pnt);
				if (!enqueued) {
					std::ostringstream ss;
					ss << "Statistic " << statNodePath << " having value " << item << " can not be registered. It will be skipped.";
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ss.str());
				} else {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Statistic " << statNodePath << " having value " << item << " successfully pushed.")
				}
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publishing of new statistics is not enabled.");
			}
		} catch (std::exception& e) {
			ACS_LOG_NOTIFY_EX(e);
		}
	}

	void forceExit();
	bool safeToExit();

protected:
	StatisticsNotifier();



private:
	Mutex _mutex;
	ACS_SMARTPTR<MQTTStatPublisherThread> _statPublisherThread;
	bool _publishEnabled;
	
	friend StatisticsNotifier* Singleton<StatisticsNotifier>::instance();

	ACS_CLASS_DECLARE_DEBUG_LEVEL(StatisticsNotifier);
};

_ACS_END_NAMESPACE

#endif /* _StatisticsNotifier_H_ */
