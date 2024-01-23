// PRQA S 1050 EOF
/*
   Copyright 1995-2019, Exprivia
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Advanced Computer Systems, Inc. 

   libMQTT Subscriber base class
*/ 


#ifndef _MQTTSubscriber_H_
#define _MQTTSubscriber_H_

#include <MQTTWorker.h>

_ACS_BEGIN_NAMESPACE(acs) 

class MQTTSubscriber : public MQTTWorker 
{
public:
	MQTTSubscriber(const std::string& name, const std::string& topic, const struct MQTTWorker::Configuration& config,  StopController const& stopController = AppStopController()) ;
	virtual ~MQTTSubscriber() ACS_NOEXCEPT ;

	/** Useful functions to check topics validity for subscribers */
	static bool checkTopic(const std::string& mqttTopic);
	static bool checkTopicWithError(const std::string& mqttTopic, std::string& error);

	// Set the topic, throws if the topic is not good for a subsription
	const std::string& getTopic() const;

	// It will start connection and register the subsription
	bool activateSubscription(MQTT_QOS qos, unsigned int timeoutMs = 5000);

	// It will unsubscribe and terminate
	bool deactivateSubscription(unsigned int timeoutMs = 5000);

	// Returns the actual subscription status
	bool isSubscribed() const;

protected:
	// Setter needed for thread safety
	void setTopic(const std::string&);
	void setSubscribed(bool);
	MQTT_QOS getQOS() const;
	void setQOS(MQTT_QOS);

/**
 * This has been extracted from the mosquitto headers and can change
 * verify it after updating to another version of mosquitto library
 * 
 * be aware that payload can be NULL (and payloadlen can be ZERO)
 * 
	struct mosquitto_message{
			int mid;
			char *topic;
			void *payload;
			int payloadlen;
			int qos;
			bool retain;
	};
 */
 
	// event notification of new incoming messages
	virtual void internal_onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS);
	virtual void internal_onUnsubscribe (int mid);
	virtual void internal_onConnect (int rc);
	virtual void internal_onDisconnect (int rc);

private:
	MQTTSubscriber (const MQTTSubscriber&);
	MQTTSubscriber& operator= (const MQTTSubscriber&);

private:
	// attributes
	std::string _topic;
	bool _isSubscribed;
	MQTT_QOS _qos;
	StopController const &_stopController ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTSubscriber) ;
};

_ACS_END_NAMESPACE

#endif //_MQTTSubscriber_H_
