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

#include <StatisticsNotifier.h>
#include <EventNotifyFactory.h>
#include <Application.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(StatisticsNotifier);


/** The configuration callback */
void StatisticsNotifier::onConfigLoaded(void* v) {
	MQTTStatPublisherThread::setConfigLoadedFlag(true);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Callback called!");
	
	/*
		Dynamic cast is used to ensure that the pointer returned by the factory is actually
		of the same type of this notifier, so no factory with different notifiers has registered into the singleton.
		Otherwise the dynamic cast fails and no thread is started
		Remember also that even if the the callback is executed before that the notifier is created, 
		this instruction will instanciate it for the first time
	*/
	StatisticsNotifier* snPtr = dynamic_cast<StatisticsNotifier*>(EventNotifyFactorySingleton::instance()->newItem()); // PRQA S 3081
	if (0 != snPtr) {
		snPtr->startPublisherThread();
	}
}

StatisticsNotifier::StatisticsNotifier() :
	EventNotifier(),
	Singleton<StatisticsNotifier>(),
	_mutex(),
	_statPublisherThread(),
	_publishEnabled(true)
{
	_statPublisherThread.reset(new MQTTStatPublisherThread("MQTTStatPublisherThread"));
}

StatisticsNotifier::~StatisticsNotifier() ACS_NOEXCEPT {
	try { // PRQA S 4631 L1
		if (_statPublisherThread->running()) {
			_statPublisherThread->stop(); // join also
		}
		
		std::exception* ex = 0;
		_statPublisherThread->runException(ex);
		if (0 != ex) {
			ACS_LOG_NOTIFY_EX(ex);
			delete ex;
		}
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
	} // PRQA L:L1
}

void StatisticsNotifier::startPublisherThread() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	// The thread is not running
	Lock l(_mutex);
	if (!_statPublisherThread->running()) {
		std::exception* ex = 0;
		_statPublisherThread->runException(ex);
		if (0 != ex) {
			// Publishing thread exited on error, disable further stats publishing
			ACS_LOG_NOTIFY_EX(*ex);
			delete ex;
			
			ACS_LOG_WARNING("Publishing of statistics will be disabled because of unexpected errors in the publishing thread.");
			_publishEnabled = false;
		}
		if (_publishEnabled)
        {
            _statPublisherThread->start();
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher thread started.");
        }
        else
        {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher thread not started because publish is disabled.");
        }
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher thread is already running.");
	}
}

// Inherited from EventNotifier
void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, int val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<int>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, long val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<long>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, long long val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<long long>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, size_t val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<size_t>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, float val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<float>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, double val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<double>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, long double val) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	notifyStatisticsToInfluxDb<long double>(statNodePath, val);
}

void StatisticsNotifier::notifyStatistics(const std::string& statNodePath, const std::string& item) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void StatisticsNotifier::forceExit()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    // Prevent statistics generation
    _publishEnabled = false;
    // Ask the thread to stop but do not join with it
    _statPublisherThread->stop(false);
}

bool StatisticsNotifier::safeToExit()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
    return not (_publishEnabled || _statPublisherThread->running());
}


_ACS_END_NAMESPACE
