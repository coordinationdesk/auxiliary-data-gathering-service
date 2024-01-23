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

#include <MQTTPublisher.h>
#include <Filterables.h>

#include <Timer.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTPublisher) ;

MQTTPublisher::MQTTPublisher(const string& name, const struct MQTTWorker::Configuration& config, StopController const& stopController ) :
	MQTTWorker (name, config, stopController),
	_internalTopicSet(false),
	_internalTopic(),
	_queuedItems(),
	_queueCondition(),
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
}

MQTTPublisher::~MQTTPublisher() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)

	// Disconnet and shut down the mqtt loop event
	// interrupting also callback notifications
	shutDownMQTTLayer();

}


bool MQTTPublisher::checkTopic(const std::string& mqttTopic) {
	std::string mqtterr;
	return checkTopicWithError(mqttTopic, mqtterr);
}

bool MQTTPublisher::checkTopicWithError(const std::string& mqttTopic, std::string& error) {
#ifndef DISABLE_MQTT
// MP in case of MQTT DISABLED, any topic is valid.
	bool valid = !mqttTopic.empty();
	if (valid) {
		int mqResult = mosquitto_pub_topic_check (mqttTopic.c_str());
		if (MOSQ_ERR_SUCCESS != mqResult) {
			error = mosquitto_strerror (mqResult);
			valid = false;
		}
	}
	return valid;
#else
    return true ;
#endif
}

void MQTTPublisher::resetTopic()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Going to reset the internal topic.");
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	
	_internalTopic = "";
	_internalTopicSet = false;
}

void MQTTPublisher::setTopic(const string& topic)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Safety check first
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Going to set the topic \"" << topic << "\"")
	
	std::string mqtterr;
	if ( !checkTopicWithError(topic, mqtterr) )
	{
		ostringstream os;
		os << "Trying to set an invalid publishing topic. Error is: " << mqtterr;
		ACS_THROW ( MQTTPublisher::BadTopic (os.str()) );
	}
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());

	// Ok can update the topic now
	_internalTopic = topic;
	_internalTopicSet = true;
}

const std::string& MQTTPublisher::getTopic (bool& set) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	// Lock on parent mutex
	ThreadSafe::Lock lock (getMutex());
	
	set = _internalTopicSet;
	return _internalTopic;
}

// Start the connection
bool MQTTPublisher::startPublishing(unsigned int timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	// Use base class connection
	return connect(timeoutMs);
}

// Terminate connection if any
void MQTTPublisher::stopPublishing(unsigned int timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	// Use base class connection
	disconnect(timeoutMs);
}


/**
 *  API to send strings. The publisher sends only the payload and not the char terminator
 */
int MQTTPublisher::publishText (const string& message, MQTTWorker::MQTT_QOS qos)  
{
	return internalPublish (message.c_str(), message.size(), "", qos, false);
}

/**
 *  API to send strings. The publisher sends only the payload and not the char terminator
 */
int MQTTPublisher::publishTextOnTopic (const std::string& message, const std::string& topic, MQTTWorker::MQTT_QOS qos)
{
	std::string errmsg;
	bool tpcValid = checkTopicWithError(topic, errmsg);
	if (!tpcValid) {
		ACS_THROW(MQTTPublisher::BadTopic("No valid topic found. Error was: " + errmsg));
	}
	
	return internalPublish (message.c_str(), message.size(), topic, qos, false);
}

int MQTTPublisher::publishBinary (void* payload, int payloadsize, MQTTWorker::MQTT_QOS qos)  
{
	return internalPublish (payload, payloadsize, "", qos, false);
}

int MQTTPublisher::publishBinaryOnTopic (void* payload, int payloadsize, const std::string& topic, MQTTWorker::MQTT_QOS qos)
{
	std::string errmsg;
	bool tpcValid = checkTopicWithError(topic, errmsg);
	if (!tpcValid) {
		ACS_THROW(MQTTPublisher::BadTopic("No valid topic found. Error was: " + errmsg));
	}
	
	return internalPublish (payload, payloadsize, topic, qos, false);
}

int MQTTPublisher::publishRetainText (const string& message, MQTTWorker::MQTT_QOS qos)  
{
	return internalPublish (message.c_str(), message.size(), "", qos, true);
}

int MQTTPublisher::publishRetainTextOnTopic (const std::string& message, const std::string& topic, MQTTWorker::MQTT_QOS qos)
{
	std::string errmsg;
	bool tpcValid = checkTopicWithError(topic, errmsg);
	if (!tpcValid) {
		ACS_THROW(MQTTPublisher::BadTopic("No valid topic found. Error was: " + errmsg));
	}
	
	return internalPublish (message.c_str(), message.size(), topic, qos, true);
}

int MQTTPublisher::publishRetainBinary (void* payload, int payloadsize, MQTTWorker::MQTT_QOS qos)  
{
	return internalPublish (payload, payloadsize, "", qos, true) ;
}

int MQTTPublisher::publishRetainBinaryOnTopic (void* payload, int payloadsize, const std::string& topic, MQTTWorker::MQTT_QOS qos)
{
	std::string errmsg;
	bool tpcValid = checkTopicWithError(topic, errmsg);
	if (!tpcValid) {
		ACS_THROW(MQTTPublisher::BadTopic("No valid topic found. Error was: " + errmsg));
	}
	
	return internalPublish (payload, payloadsize, topic, qos, true) ;
}

int MQTTPublisher::cleanRetainedMessage ()
{
	return publishRetainBinary (NULL, 0, QOS_2);
}

int MQTTPublisher::cleanRetainedMessageOnTopic (const std::string& topic)
{
	std::string errmsg;
	bool tpcValid = checkTopicWithError(topic, errmsg);
	if (!tpcValid) {
		ACS_THROW(MQTTPublisher::BadTopic("No valid topic found. Error was: " + errmsg));
	}
	
	return publishRetainBinaryOnTopic (NULL, 0, topic, QOS_2);
}

int MQTTPublisher::internalPublish (const void* payload, int payloadsize, const std::string& topic, MQTTWorker::MQTT_QOS qos, bool retain) 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
#ifndef DISABLE_MQTT
	// MP in case of MQTT DISABLED, any publish will do
    // a static counter is incremented to return an increasing counter
	std::string tpc = topic;
	
	// Input topic is not valid...
	if (topic == "") {
		bool intTopicSet = false;
		std::string intTpc = getTopic(intTopicSet);
		
		// Internal topic not set... nothing to do...
		if (!intTopicSet) {
			ACS_THROW(MQTTPublisher::BadTopic("No valid topic found."));
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Using internal topic \"" << _internalTopic << "\"");
		
		// Use internal topic, it has already been verified by the setTopic operation
		tpc = intTpc;
	}
	
	int messageId = 0;

	{
		// Lock the queue condition mutex here
		ThreadSafe::Lock lock (_queueCondition);

		// Fire the message
		int mqResult = mosquitto_publish (getClient(), &messageId, tpc.c_str(), payloadsize, payload, qos, retain);
		if ( MOSQ_ERR_SUCCESS != mqResult )
		{
			ostringstream os;
			os << "Publish message failed. Error is " << mosquitto_strerror (mqResult);
			ACS_THROW (ConnectionError (os.str()) );
		}

		// Ok a new item has been queued. Update the queue tracker if the qos is not zero
		if (qos > QOS_0)
		{
			_queuedItems.insert(messageId);
			// Reset empty queue event counter, to signal the queue is not empty now
			_queueCondition.resetEventCounter();
		}
		
		// Unlock the condition, so onPublish callback can work
	}
    
	return messageId;
#else
    static int messageId = 0;
    return ++messageId ;
#endif
}

void MQTTPublisher::internal_onPublish (int mid)
{
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Publishing for item id " << mid << " has been notified");
	
	// Manage the queue counter
	{
		ThreadSafe::Lock lock (_queueCondition);
		
		// Remove the message id from the tracked queue (if exists)
		_queuedItems.erase (mid);
		
		// In case the queue is emtpy now, broadcast on the condition to wake up sleepers
		if (_queuedItems.empty() )
		{
			_queueCondition.broadcast();
		}
	}
}


// Timed wait on the emtpy queue condition
bool MQTTPublisher::waitPendingTransfers (unsigned long timeoutMs) const
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

	bool queueEmpty = false;
	// Lock the queue condition and then wait or exit
	{
		ThreadSafe::Lock lock (_queueCondition);
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Condition event counter is " << _queueCondition.getEventCounter());
		// No broadcast issued at the present time
		if ( 0 == _queueCondition.getEventCounter() && !_queuedItems.empty() )
		{

			// The queue is not empty. Wait for a broadcast
			_queueCondition.timedwait (timeoutMs);
		}
		
		// Check if the queue is empty right now
		if ( _queuedItems.empty() )
		{
			queueEmpty = true;
		}
	}
	
	return queueEmpty;
}


_ACS_END_NAMESPACE
