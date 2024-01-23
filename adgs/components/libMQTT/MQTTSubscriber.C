// PRQA S 1050 EOF
/*
   Copyright 1995-2019, Exprivia
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA

   libMQTT Subscriber base class
*/ 

#include <MQTTSubscriber.h>
#include <Filterables.h>

#include <Timer.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTSubscriber)

MQTTSubscriber::MQTTSubscriber(const string& name, const std::string& topic, const struct MQTTWorker::Configuration& config, StopController const& stopController) :
	MQTTWorker (name, config, stopController),
	_topic(),
	_isSubscribed(false),
	_qos(QOS_0),
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
	setTopic(topic);
}

MQTTSubscriber::~MQTTSubscriber() ACS_NOEXCEPT 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
	// Deactivate the subscription before destroyng
	deactivateSubscription();
	// Disconnet and shut down the mqtt loop event
	// interrupting also callback notifications
	shutDownMQTTLayer();
}

bool MQTTSubscriber::checkTopic(const std::string& mqttTopic) {
	std::string mqtterr;
	return checkTopicWithError(mqttTopic, mqtterr);
}

bool MQTTSubscriber::checkTopicWithError(const std::string& mqttTopic, std::string& error) {
#ifndef DISABLE_MQTT

	bool valid = !mqttTopic.empty();
	// is not empty, check format
	if (valid) {
		int mqResult = mosquitto_sub_topic_check (mqttTopic.c_str());
		if (MOSQ_ERR_SUCCESS != mqResult) {
			error = mosquitto_strerror (mqResult);
			valid = false;
		}
	}
	return valid;
#else
// prefer to return true in case of MQTT DISABLED to avoid complaints
	return true;
#endif
}

// Topic management
void MQTTSubscriber::setTopic(const string& topic)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Safety check first
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Going to set the topic \"" << topic << "\"")
	
	
	std::string mqtterr;
	if (!checkTopicWithError(topic, mqtterr))
	{
		ostringstream os;
		os << "Trying to set an invalid subscription topic. Error is: " << mqtterr;
		ACS_THROW ( MQTTSubscriber::BadTopic (os.str()) );
	}
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());

	// Ok can update the topic now
	_topic = topic;
}

const string& MQTTSubscriber::getTopic () const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	return _topic;
}

bool MQTTSubscriber::isSubscribed() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	return _isSubscribed;
}

void MQTTSubscriber::setSubscribed(bool s)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	_isSubscribed = s;
}

MQTTWorker::MQTT_QOS MQTTSubscriber::getQOS() const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	return _qos;
}

void MQTTSubscriber::setQOS(MQTT_QOS qos)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	_qos = qos;
}

bool MQTTSubscriber::activateSubscription(MQTT_QOS qos, unsigned int timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// To activate a subscription:
	// - a valid qos must be provided
	// - no subscriptions shall be active
	// - a good topic must be set
	
	{
		// Empty topic check
		ACS_COND_THROW ( getTopic().empty(), PreconditionFailure ("Cannot subscribe. Topic not set") );

		// No active subsciptions
		ACS_COND_THROW ( (true == isSubscribed() ) , PreconditionFailure ("Cannot subscribe. Another subscription is active.") );

		setQOS (qos);

		// Ok.. now connect. This shall not lock or deadlock
		// Will force the underlying MQTT loop thread to stop and restart
		connect();

		// Now wait for the subscription to complete (via callbacks)
		const unsigned int sleepPeriod = 50; // msecs
		unsigned int maxCount = timeoutMs / sleepPeriod;
		unsigned int counter = 0;
		
		while ( ( false == isSubscribed() ) && ( counter < maxCount ) && ( false == _stopController.requestedCancel() ) )
		{
			// Delay the sleepTime
			Timer::delay (sleepPeriod);
			++counter;
		}
	}
	return isSubscribed();
}

bool MQTTSubscriber::deactivateSubscription(unsigned int timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
#ifndef DISABLE_MQTT
// MP: no op in case of MQTT DISABLED - isSubscribed should return false
	// Ask for subscription deactivation

	// unsubscribe if the session is clean
	if (true == getConfiguration().clean_session)
	{
		// ignore errors if any
		int mqResult = mosquitto_unsubscribe (getClient(), NULL, getTopic().c_str());
		if ( MOSQ_ERR_SUCCESS != mqResult )
		{
			// Is it really an issue?
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Unable to unsubscribe")
		}
		else
		{
			const unsigned int sleepPeriod = 50; // msecs
			unsigned int maxCount = timeoutMs / sleepPeriod;
			unsigned int counter = 0;
			
			while ( (true == isSubscribed()) && ( counter < maxCount ) /* && ( false == _stopController.requestedCancel() ) */ )
			{
				// Delay the sleepTime
				Timer::delay (sleepPeriod);
				++counter;
			}
		}
	}
	
	// close also the connection (REALLY?)
	disconnect();
#endif
	return !isSubscribed();
}

void MQTTSubscriber::internal_onSubscribe (int mid, const vector<MQTT_QOS> & grantedQOS)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	{
		setSubscribed(true);
	}
}

void MQTTSubscriber::internal_onUnsubscribe (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	{
		setSubscribed(false);
	}
}

void MQTTSubscriber::internal_onConnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP: no op in case of MQTT DISABLED 

	// Do subscribe... connection has been just created or restored
	// After connection send the subscription request
	// Fire the message
	int mqResult = mosquitto_subscribe (getClient(), NULL, getTopic().c_str(), getQOS());

	// Callback shall never throw
	if ( MOSQ_ERR_SUCCESS != mqResult )
	{
		ACS_LOG_WARNING ("MQTTSubscriber. Failed to subscribe. Error is " << mosquitto_strerror (mqResult) );
		setSubscribed(false);
	}
#endif
}

void MQTTSubscriber::internal_onDisconnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	// Disconnected... subscription is not valid anymore
	setSubscribed(false);
}

_ACS_END_NAMESPACE
