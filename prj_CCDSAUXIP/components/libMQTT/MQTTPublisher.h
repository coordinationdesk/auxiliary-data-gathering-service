// PRQA S 1050 EOF
/*
   Copyright 2019-2022 Exprivia - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT Subscriber base class
*/ 

#ifndef _MQTTPublisher_H_
#define _MQTTPublisher_H_

#include <MQTTWorker.h>
#include <ThreadSafe.h>
#include <set>

_ACS_BEGIN_NAMESPACE(acs) 

class MQTTPublisher : public MQTTWorker // NOSONAR
{

public:
	MQTTPublisher(const std::string& name, const MQTTWorker::Configuration& config,  StopController const& stopController = AppStopController()) ;
	MQTTPublisher (const MQTTPublisher&) = delete ;
	MQTTPublisher& operator= (const MQTTPublisher&) = delete ;
	MQTTPublisher (MQTTPublisher&&) = delete ;
	MQTTPublisher& operator= (MQTTPublisher&&) = delete ;

	virtual ~MQTTPublisher() noexcept ;

	void setTopic(const std::string&);
	void resetTopic();
	const std::string& getTopic(bool&) const;
	
	/**
	MQTT Protocol specifications  for QOS values:
	higher levels of QoS are more reliable, but involve higher latency and have higher bandwidth requirements.

		QOS_0: The broker/client will deliver the message once, with no confirmation.
		QOS_1: The broker/client will deliver the message at least once, with confirmation required.
		QOS_2: The broker/client will deliver the message exactly once by using a four step handshake.
	**/

	/** Useful functions to check topics validity for publishers */
	static bool checkTopic(const std::string& mqttTopic);
	static bool checkTopicWithError(const std::string& mqttTopic, std::string& error);

	/**
	 *  API to send strings. The publisher sends only the payload and not the char terminator
	 */
	int publishText (const std::string& message, MQTT_QOS qos = QOS_0);
	int publishTextOnTopic (const std::string& message, const std::string& topic, MQTT_QOS qos = QOS_0);
	
	/**
	 *  API to send binary message
	 */
	int publishBinary (void* payload, int payloadsize, MQTT_QOS qos = QOS_0);
	int publishBinaryOnTopic (void* payload, int payloadsize, const std::string& topic, MQTT_QOS qos = QOS_0);

	// Retained messages will be received at any connection by subscribers
	// To delete a retained message use the dedicated interface
	int publishRetainText (const std::string& message, MQTT_QOS qos = QOS_0);
	int publishRetainTextOnTopic (const std::string& message, const std::string& topic, MQTT_QOS qos = QOS_0);
	int publishRetainBinary (void* payload, int payloadsize, MQTT_QOS qos = QOS_0);
	int publishRetainBinaryOnTopic (void* payload, int payloadsize, const std::string& topic, MQTT_QOS qos = QOS_0);
	
	// Clean retained messages (no QOS or payload are needed to exercise this interface)
	int cleanRetainedMessage ();
	int cleanRetainedMessageOnTopic (const std::string& topic);
	
	// Timed wait on the emtpy queue condition
	bool waitPendingTransfers (unsigned long timeoutMs = 5000) const;
	
	// Start the connection
	// Will try to wait the connection stability, and return true if the connection is ready
	bool startPublishing(unsigned int timeoutMs = 5000);
	
	// Terminate connection if any
	void stopPublishing(unsigned int timeoutMs = 5000);
	
protected:
	// Manage condition counting the pending elements in the queue
	virtual void internal_onPublish (int mid);
	
private:

	int internalPublish (const void* payload, int payloadsize, const std::string& topic, MQTT_QOS qos, bool retain);
	
private:
	bool _internalTopicSet;
	std::string _internalTopic;
	
	// Not all the queued elements must be tracked
	std::set<int> _queuedItems;
	Condition _queueCondition;
	StopController const &_stopController ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTPublisher) ;


};

_ACS_END_NAMESPACE

#endif //_MQTTPublisher_H_
