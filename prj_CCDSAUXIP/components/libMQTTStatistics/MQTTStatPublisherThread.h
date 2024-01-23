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

#ifndef _MQTTStatPublisherThread_H_
#define _MQTTStatPublisherThread_H_

#include <acs_c++pers.hpp>
#include <MQTTPublisher.h>
#include <InfluxDbStatisticPoint.h>
#include <StatisticsBacklog.h>
#include <Thread.h>
#include <exMacros.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class MQTTStatPublisherThread : public Thread {
public:
	exDECLARE_EXCEPTION(MQTTStatPublisherThreadException, exException);
	
	/** Struct containning publisher thread configuration */
	typedef struct StatPublisherConfig {
	public:
		StatPublisherConfig() :
			mqttConfig(),
			qosLevel(MQTTWorker::QOS_0),
			pollingTime(100),
			exitTimeout(2000),
			connTimeout(5000),
			maxBacklogSize(100000),
			ignoreStatistics(false)
		{ }
		
		StatPublisherConfig(const StatPublisherConfig& config) :
			mqttConfig(config.mqttConfig),
			qosLevel(config.qosLevel),
			pollingTime(config.pollingTime),
			exitTimeout(config.exitTimeout),
			connTimeout(config.connTimeout),
			maxBacklogSize(config.maxBacklogSize),
			ignoreStatistics(config.ignoreStatistics)
		{ }
		
		StatPublisherConfig& operator=(const StatPublisherConfig& config) {
			if (this != &config) {
				mqttConfig = config.mqttConfig;
				qosLevel = config.qosLevel;
				pollingTime = config.pollingTime;
				exitTimeout = config.exitTimeout;
				connTimeout = config.connTimeout;
				maxBacklogSize = config.maxBacklogSize;
				ignoreStatistics = config.ignoreStatistics;
			}
			return *this;
		}
		
	public:
		MQTTWorker::Configuration mqttConfig;
		MQTTWorker::MQTT_QOS qosLevel;
		unsigned long pollingTime;
		unsigned long exitTimeout;
		unsigned int connTimeout;
		size_t maxBacklogSize;
		bool ignoreStatistics;
	} StatPublisherConfig;
	
	/** Static methods to return config loaded flag and config condition */
	Condition& getConfigCondition();
	static bool getConfigLoadedFlag();
	static void setConfigLoadedFlag(bool v);
	
	/** Constructor/Destructor */
	explicit MQTTStatPublisherThread(const std::string& name);
	virtual ~MQTTStatPublisherThread() ACS_NOEXCEPT;

	/** Push the new statistic point, then wakeup the thread on the condition */
	bool pushStatPoint(const InfluxDbStatisticPoint& point);

	/** Reimplement Thread::run method */
	virtual void run();
	
	/** 
	 * Returns true if the thread has forwarder at least one message, false otherwise.
	 * It can be used to decide weather to restart the thread in case of errors or not.
	 */ 
	bool hasForwarded() const;
    

protected:
	/** Returns the application name if any, otherwise returns unknown */
	std::string getApplicationName();

	/** Builds the topic string */
	virtual std::string buildTopic();
	
	/** Reads the configuration from config space */
	virtual void configure();
	
	/** Initialize the publisher */
	void initPublisher();
	
	/** Creates the publisher if needed and connects it to brokers */
	bool connect() const;
	
	/** Forward accumulated statistics to brokers */
	void forwardStatistics(size_t start, size_t end);

protected:
	static Mutex _configLoadedMutex; // This mutex protect the config loaded flag
	static bool _configLoaded;
	
	// This condition is used to activate the thread when the configuration 
	// is loaded. This is not used to protect the config loaded flag because 
	// it could cause a deadlock in the configure() procedure ==> in the configure()
	// function this thread waits for the lock on the ResourceSet::hasKey of the ConfigurationSingleton 
	// but the configuration singleton sends also statistic (after acquiring the same resourceSet lock)
	// during conf space merge causing the StatEventNotifyFactory::createItem 
	// to lock on the confCondition to get the config loaded flag..... DEADLOCK!
	static Condition _confCondition;
	
	Mutex _mutex;
	StatPublisherConfig _config;
	
	std::unique_ptr<MQTTPublisher> _statPublisher;
	// this pointer identifies the backlog to be used to push new statistics
	std::unique_ptr<StatisticsBacklog> _pushBacklog;
	// this pointer identifies the backlog to be used to forward new statistics
	std::unique_ptr<StatisticsBacklog> _forwardBacklog;

private:
	// defined but not implemented
	MQTTStatPublisherThread();
	MQTTStatPublisherThread(const MQTTStatPublisherThread&);
	MQTTStatPublisherThread& operator=(const MQTTStatPublisherThread&);

private:
	std::string _appName;
	bool _appNameSet;
	
	// If true the backlog warning will be logged
	bool _emitBacklogFullWarning;
	bool _emitConnectionErrorWarning;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTStatPublisherThread);
};

_ACS_END_NAMESPACE

#endif /* _MQTTStatPublisherThread_H_ */
