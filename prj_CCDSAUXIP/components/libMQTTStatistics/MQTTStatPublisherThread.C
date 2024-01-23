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

#include <MQTTStatPublisherThread.h>
#include <Application.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTStatPublisherThread);

Mutex MQTTStatPublisherThread::_configLoadedMutex;
Condition MQTTStatPublisherThread::_confCondition;
bool MQTTStatPublisherThread::_configLoaded = false;

void MQTTStatPublisherThread::setConfigLoadedFlag(bool v) {
	{
		// A separate mutex must be used to avoid deadlocks
		Lock lconf(_configLoadedMutex);
		_configLoaded = v;
	}
	
	_confCondition.broadcast();
}

bool MQTTStatPublisherThread::getConfigLoadedFlag() {
	Lock lconf(_configLoadedMutex);
	return _configLoaded;
}

Condition& MQTTStatPublisherThread::getConfigCondition() {
	return _confCondition;
}


MQTTStatPublisherThread::MQTTStatPublisherThread(const std::string& name) :
	Thread("MQTTStatPublisherThread"),
	_mutex(),
	_config(),
	_statPublisher(),
	_pushBacklog(new StatisticsBacklog()),
	_forwardBacklog(new StatisticsBacklog()),
	_appName("unknown"),
	_appNameSet(false),
	_emitBacklogFullWarning(true),
	_emitConnectionErrorWarning(true)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

MQTTStatPublisherThread::~MQTTStatPublisherThread() ACS_NOEXCEPT {
	try { // PRQA S 4631 L1
		if (0 != _statPublisher.get()) {
			_statPublisher->stopPublishing();
		}
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
	} // PRQA L:L1
}

std::string MQTTStatPublisherThread::getApplicationName() { // PRQA S 4020
	if (_appNameSet) { return _appName; }
	
	try {
		Application* app = Application::instance();
		if (app != 0) {
			std::string tmp = app->applicationName();
			if (!tmp.empty()) {
				_appName = tmp;
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty application name found. Default value will be used.");
			}
			_appNameSet = true;
		}
	} catch (std::exception& e) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Application instance not found. Using default process name \"unknown\"");
	}
	
	return _appName;
}

std::string MQTTStatPublisherThread::buildTopic() {
	std::ostringstream ss; 
	ss << "/statistics/" << Application::hostname() << "/" << getApplicationName() << "/" << getpid();
	return ss.str();
}

/** Push the new statistic point, then wakeup the thread on the condition */
bool MQTTStatPublisherThread::pushStatPoint(const InfluxDbStatisticPoint& point) {
	bool pushed = false;
	if (_config.ignoreStatistics) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Statistic \"" << point.asLineProtocol() << "\" ignored as requested.");
		return pushed;
	}
	
	// Lock required to protect backlogs pointers from swap operation
	Lock lock(_mutex);
	if (_pushBacklog->size() < _config.maxBacklogSize) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Pushing statistic point " << point.asLineProtocol());
		_pushBacklog->pushStat(point);
		pushed = true;
	}
	
	// Max size reached
	else {
		if (_emitBacklogFullWarning) {
			std::ostringstream ss;
			ss << "Backlog max size (" << _config.maxBacklogSize << " items) reached. Statistic \"" << point.asLineProtocol() << "\" ignored.";
			ACS_LOG_WARNING(ss.str());
			// Notify just once
			_emitBacklogFullWarning = false;
		}
	}
	
	return pushed;
}

void MQTTStatPublisherThread::configure() { // PRQA S 5500,5510
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock lock(_confCondition);	// PRQA S 3050

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuring publisher.");

	rsResourceSet& rset = ConfigurationSingleton::instance()->get();
	
	// Ignore statistics
	if (rset.hasKey("MQTTStatistics.IgnoreStatistics")) {
		rset.getValue<bool>("MQTTStatistics.IgnoreStatistics", _config.ignoreStatistics);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ignore statistics flag set to " << std::boolalpha << _config.ignoreStatistics);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Ignore statistics flag not found, using default " << std::boolalpha << _config.ignoreStatistics);
	}

	// Max backlog size
	if (rset.hasKey("MQTTStatistics.MaxBacklogSize")) {
		rset.getValue<size_t>("MQTTStatistics.MaxBacklogSize", _config.maxBacklogSize);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Max backlog size set to " << _config.maxBacklogSize);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Max backlog size configuration not found, using default " << _config.maxBacklogSize);
	}
	
	// Publish condition timeout
	if (rset.hasKey("MQTTStatistics.PollingTime")) {
		rset.getValue<unsigned long>("MQTTStatistics.PollingTime", _config.pollingTime);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Polling time set to " << _config.pollingTime);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Polling time configuration not found, using default " << _config.pollingTime);
	}
	
	// Exit timeout on cancel
	if (rset.hasKey("MQTTStatistics.ExitOnCancelTimeout")) {
		rset.getValue<unsigned long>("MQTTStatistics.ExitOnCancelTimeout", _config.exitTimeout);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exit on cancel timeout set to " << _config.exitTimeout);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exit on cancel timeout configuration not found, using default " << _config.exitTimeout);
	}

	// Load connection timeout
	if (rset.hasKey("MQTTStatistics.BrokerConnection.ConnectionTimeout")) {
		rset.getValue<unsigned int>("MQTTStatistics.BrokerConnection.ConnectionTimeout", _config.connTimeout);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Connection timeout set to " << _config.connTimeout);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Connection timeout configuration not found, using default " << _config.connTimeout);
	}

	// Load clientId
	if (rset.hasKey("MQTTStatistics.BrokerConnection.ClientID")) {
		rset.getValue<std::string>("MQTTStatistics.BrokerConnection.ClientID", _config.mqttConfig.clientId);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ClientId set to " << _config.mqttConfig.clientId);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ClientId configuration not found, using default " << _config.mqttConfig.clientId);
	}
	
	// Load clean session flag
	if (rset.hasKey("MQTTStatistics.BrokerConnection.CleanSessionFlag")) {
		rset.getValue<bool>("MQTTStatistics.BrokerConnection.CleanSessionFlag", _config.mqttConfig.clean_session);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Clean session flag set to " << std::boolalpha << _config.mqttConfig.clean_session);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Clean session flag configuration not found, using default " << std::boolalpha << _config.mqttConfig.clean_session);
	}
	
	// Load hostname
	if (rset.hasKey("MQTTStatistics.BrokerConnection.Hostname")) {
		rset.getValue<std::string>("MQTTStatistics.BrokerConnection.Hostname", _config.mqttConfig.host);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Hostname set to " << _config.mqttConfig.host);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Hostname configuration not found, using default " << _config.mqttConfig.host);
	}
	
	// Load port number
	if (rset.hasKey("MQTTStatistics.BrokerConnection.Port")) {
		rset.getValue<int>("MQTTStatistics.BrokerConnection.Port", _config.mqttConfig.port);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Port number set to " << _config.mqttConfig.port);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Port number configuration not found, using default " << _config.mqttConfig.port);
	}
	
	// Load keep alive interval
	if (rset.hasKey("MQTTStatistics.BrokerConnection.KeepAliveInterval")) {
		rset.getValue<int>("MQTTStatistics.BrokerConnection.KeepAliveInterval", _config.mqttConfig.keepalive);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Keepalive interval set to " << _config.mqttConfig.keepalive);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Keepalive interval configuration not found, using default " << _config.mqttConfig.keepalive);
	}
	
	// Load username
	if (rset.hasKey("MQTTStatistics.BrokerConnection.Username")) {
		rset.getValue<std::string>("MQTTStatistics.BrokerConnection.Username", _config.mqttConfig.username);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Username set to " << _config.mqttConfig.username);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Username configuration not found, using default " << _config.mqttConfig.username);
	}
	
	// Load password
	if (rset.hasKey("MQTTStatistics.BrokerConnection.Password")) {
		rset.getValue<std::string>("MQTTStatistics.BrokerConnection.Password", _config.mqttConfig.password);
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Password not configured.");
	}
	
}

void MQTTStatPublisherThread::initPublisher() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// Mutexes are recursive, no risk for deadlocks from the same thread
	Lock lock(_mutex);	// PRQA S 3050

	if (0 == _statPublisher.get()) {
		_statPublisher.reset(new MQTTPublisher(_config.mqttConfig.clientId, _config.mqttConfig));
		ACS_COND_THROW((0 == _statPublisher.get()), MQTTStatPublisherThreadException("An error occurs while creating an instance of MQTTPublisher.")); // PRQA S 3081
	}
}

bool MQTTStatPublisherThread::connect() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_COND_THROW((0 == _statPublisher.get()), MQTTStatPublisherThreadException("MQTTPublisher not initialzied.")); // PRQA S 3081
	
	_statPublisher->startPublishing(_config.connTimeout);
	
	return _statPublisher->isConnected();
}

void MQTTStatPublisherThread::forwardStatistics(size_t start, size_t end) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	size_t idx = start;
	try {
		while (idx < end) {
			if (_statPublisher->getConnTrackStatus() == MQTTWorker::ConnStatusIDLE) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher is not connected to brokers. Connecting it.");
				connect();
			}

			MQTTWorker::ConnStatus connStatus = _statPublisher->getConnTrackStatus();
			if (connStatus != MQTTWorker::ConnStatusCONNECTED) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Status is " << MQTTWorker::connStatusToString(connStatus) << ". Waiting for connection. Retry later.");
				break;
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher successfully connected to mqtt broker.");
				// Reset the flag at each successful connection
				_emitConnectionErrorWarning = true;
			}
			
			// Get the next stat point to be published
			InfluxDbStatisticPoint pnt = _forwardBacklog->front();

			// Update processname if exists
			pnt.addTagSet("processname", getApplicationName());

			std::string topic = buildTopic();
			std::string lineProtocol = pnt.asLineProtocol();

			// Build the message
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publishing text:\n\t- message: \"" << lineProtocol << "\"\n\t- topic: \"" << topic << "\"");

			// No need to wait pending event, publishing is at best effort (QOS 0)
			_statPublisher->publishTextOnTopic(lineProtocol, topic, _config.qosLevel);

			// Pop current element
			_forwardBacklog->popStat();

			++idx;
		}
		
		size_t publishedCount = (end - start);
		if ((_forwardBacklog->size() + publishedCount) != 0 && idx != start) { // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Published " << publishedCount << "/" << (_forwardBacklog->size() + publishedCount) << " statistics points."); // PRQA S 3084
		}
	} catch (MQTTWorker::ConnectionError& e) {
		if (_emitConnectionErrorWarning) {
			ACS_LOG_WARNING("Cannot connect to MQTT brokers. Reason was: " << e.what());
			_emitConnectionErrorWarning = false;
		}
	}
}


/** Reimplement Thread::run method */
void MQTTStatPublisherThread::run() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Thread " << threadName() << " started.");
	
	bool exitRq = false;
	
	// Wait for configuration to be loaded
	{
		Lock lock(_confCondition); // PRQA S 3050
		while (true) {
			if (requestedCancel()) {
				exitRq = true;
				break;
			}
			
			{
				// Check if configuration have been loaded
				Lock l(_configLoadedMutex);
				if (_configLoaded) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuration space loaded!");
					break;
				}
			}
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuration space not laoded yet.");

			// Release the condition lock before going to sleep
			_confCondition.timedwait(_config.pollingTime); // returns false in case of timeout
			// Re-acquire the condition lock before returning
		}
	} // Conf condition released here

	if (!exitRq) {
		// Load the configuration
		configure();
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Configuration space loaded.");
		
		// Initialize publisher thread
		initPublisher();
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Publisher initialized.");
		
		double exitTimeoutDay = _config.exitTimeout / (1000.0 * 86400.0);
		
		bool cancelled = false;
		DateTime cancelTime;
		do {
			{
				// Acquire lock to protect pointers before swapping, no event can be enqueued now
				Lock lock(_mutex); // PRQA S 3050

				// Swap backlogs only if there are available slots in the forward backlog.
				// Do not swap if the forward backlog is full (it is useless, maybe the push backlog has still slots)
				if (_forwardBacklog->size() < _config.maxBacklogSize) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "BEFORE: Published: " << _pushBacklog->size() << ", Forward: " << _forwardBacklog->size());
					_pushBacklog.swap(_forwardBacklog);
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AFTER: Published: " << _pushBacklog->size() << ", Forward: " << _forwardBacklog->size());
				}

				// The lock is released here to allow push of new statistics from the applications.
				// Since the backlog is consumed always by 1 thread (i.e. publisher thread), 
				// it is granted by construction that during forwarding operation
				// there will always be at least endBacklogIdx statistics point in the backlog.
			}

			// Forward statistics points up to the right limit
			forwardStatistics(0, _forwardBacklog->size());
			
			StopController::timeoutOrCancel(_config.pollingTime);
			
			if (!cancelled && requestedCancel()) {
				cancelTime = DateTime();
				cancelled = true;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cancel requested. Backlog empty: " << std::boolalpha <<  _pushBacklog->empty() << " timeout: " << (DateTime().diffDay(cancelTime) >= exitTimeoutDay));
				
			}

			/*
			not (cancelled && (empty || timeout))
			cancelled		empty		timeout			out
			0					0			0			1
			0					0			1			1
			0					1			0			1
			0					1			1			1
			1					0			0			1
			1					0			1			0
			1					1			0			0
			1					1			1			0
			*/
		} while (not haltImmediate() && not (cancelled && (_pushBacklog->empty() || (DateTime().diffDay(cancelTime) >= exitTimeoutDay))));
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Exiting after stop request.");
	}
    
    // before exiting the run shall also stop the MQTTPublisher
    if (0 != _statPublisher.get()) 
    {
        // delete the _statPublisher and force the MQTT layer to shut down
        _statPublisher.reset(0);
        
    }
}

_ACS_END_NAMESPACE
