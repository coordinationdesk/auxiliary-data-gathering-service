// PRQA S 1050 EOF
/*
   Copyright 2019-2022, Exprivia - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT class
*/ 

#include <MQTTWorker.h>
#include <MQTTCallbackBridge.h>
#include <MosquittoInitializer.h>

#include <ThreadSafe.h>
#include <Filterables.h>


#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif

_ACS_BEGIN_NAMESPACE (acs)


// Static attributes initialization
ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTCallbackBridge)

std::map < mosquitto*, MQTTWorker* > MQTTCallbackBridge::_mqttworkers;
Mutex MQTTCallbackBridge::_mutex;

// CTOR and DTOR
MQTTCallbackBridge::MQTTCallbackBridge() noexcept
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	
	// Do initialization, if needed
	MosquittoInitializer::instance();
}

MQTTCallbackBridge::~MQTTCallbackBridge() noexcept 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
}


void MQTTCallbackBridge::registerWorker (struct mosquitto* mqtt, MQTTWorker* worker) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
#ifndef DISABLE_MQTT
// MP no op in case of MQTT DISABLED
	Mutex::Lock lock (_mutex);
	// Safety checks
	if ( (nullptr == mqtt) || (nullptr == worker) )
	{
		// Already registered... throw an exception
		ACS_THROW ( exIllegalValueException ("NULL pointer has been passed to registerWorker function."));
	}

	// Check if the mosquitto instance has already been registered in the bridge
	if ( _mqttworkers.find (mqtt) != _mqttworkers.end() )
	{
		// Already registered... throw an exception
		ACS_THROW ( exIllegalValueException ("The mosquitto instance of the worker \"" + worker->getName() + "\" has already been registered."));
	}

	// Register it in the map
	_mqttworkers[mqtt] = worker;

	// Activate callbacks
	mosquitto_connect_callback_set     (mqtt, MQTTCallbackBridge::connectCallback);
	mosquitto_disconnect_callback_set  (mqtt, MQTTCallbackBridge::disconnectCallback);
	mosquitto_publish_callback_set     (mqtt, MQTTCallbackBridge::publishCallback);
	mosquitto_message_callback_set     (mqtt, MQTTCallbackBridge::messageCallback);
	mosquitto_subscribe_callback_set   (mqtt, MQTTCallbackBridge::subscribeCallback);
	mosquitto_unsubscribe_callback_set (mqtt, MQTTCallbackBridge::unsubscribeCallback);
	mosquitto_log_callback_set         (mqtt, MQTTCallbackBridge::logCallback);

	// Done!
#endif
}

void MQTTCallbackBridge::unregisterWorker (struct mosquitto* mqtt) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	Mutex::Lock lock (_mutex);
	
	// Remove from the map. Any following callbacks call will be ignored.
	_mqttworkers.erase (mqtt);
}

// Return the registered worker or throw
MQTTWorker* MQTTCallbackBridge::findWorker (struct mosquitto* mosq) const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	Mutex::Lock lock (_mutex);

	// If found return it.. otherwise throw an exception
	MQTTWorkersType::iterator it = _mqttworkers.find (mosq);

	if ( it == _mqttworkers.end() )
	{
		// Sorry.. not found
		ACS_THROW ( exBoundsException ("Worker not found for the given mosquitto instance") );
	}

	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Returning a worker named \"" << it->second->getName() << "\"");
	return it->second;
}

// Callback implementations
void MQTTCallbackBridge::connectCallback (struct mosquitto *mosq, void *obj, int rc) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->connectCallback(rc);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}
}

void MQTTCallbackBridge::disconnectCallback (struct mosquitto *mosq, void *obj, int rc) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->disconnectCallback(rc);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}
}

void MQTTCallbackBridge::publishCallback (struct mosquitto *mosq, void *obj, int mid) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->publishCallback(mid);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}

}

void MQTTCallbackBridge::messageCallback (struct mosquitto *mosq, void *obj, const mosquitto_message *msg) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->messageCallback(msg);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}

}

void MQTTCallbackBridge::subscribeCallback (struct mosquitto *mosq, void *obj/*NOSONAR*/, int mid, \
	int qos_count, const int * granted_qos) 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->subscribeCallback(mid, qos_count, granted_qos);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}
}

void MQTTCallbackBridge::unsubscribeCallback (struct mosquitto *mosq, void *obj, int mid) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->unsubscribeCallback(mid);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}
}

void MQTTCallbackBridge::logCallback (struct mosquitto *mosq, void *obj, int level, const char *str) // NOSONAR
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Log callback (level " << level <<"): " << str);

	try
	{
		// forward the call if possible
		MQTTCallbackBridge::instance()->findWorker(mosq)->logCallback(level, str);
	}
	catch (/*NOSONAR*/const std::exception&)
	{
		// do nothing
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "callback failed due to unregistered mosquitto instance")
	}
}

_ACS_END_NAMESPACE
