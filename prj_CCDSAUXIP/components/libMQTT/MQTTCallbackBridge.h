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

#ifndef _MQTTCallbackBridge_H_
#define _MQTTCallbackBridge_H_

#include <acs_c++pers.hpp>
#include <Singleton.hpp>
#include <ThreadSafe.h>

// Forward declaration of libmosquitto elements
struct mosquitto;
struct mosquitto_message;

_ACS_BEGIN_NAMESPACE(acs)

// Forward declaration
class MQTTWorker; 


using acs::pattern::Singleton ;


// Singleton implementation
class MQTTCallbackBridge : public Singleton<MQTTCallbackBridge> // NOSONAR PRQA S 2109, 2153
{
	friend MQTTCallbackBridge* Singleton<MQTTCallbackBridge>::instance(); // PRQA S 2107

public:
	// Helper type definition
	typedef std::map < struct mosquitto*, MQTTWorker* > MQTTWorkersType;

	MQTTCallbackBridge(const MQTTCallbackBridge &) = delete ;
	MQTTCallbackBridge & operator= (const MQTTCallbackBridge &) = delete ;
	MQTTCallbackBridge(MQTTCallbackBridge &&) = delete ;
	MQTTCallbackBridge & operator= (MQTTCallbackBridge &&) = delete ;
	virtual ~MQTTCallbackBridge() noexcept ; 

	static void connectCallback (struct mosquitto *mosq, void *obj, int rc);
	static void disconnectCallback (struct mosquitto *mosq, void *obj, int rc);
	static void publishCallback (struct mosquitto *mosq, void *obj, int mid);
	static void messageCallback (struct mosquitto *mosq, void *obj, const mosquitto_message *msg);
	static void subscribeCallback (struct mosquitto *mosq, void *obj, int mid, int qos_count, const int * granted_qos);
	static void unsubscribeCallback (struct mosquitto *mosq, void *obj, int mid);
	static void logCallback (struct mosquitto *mosq, void *obj, int level, const char *str);
	
	void registerWorker (struct mosquitto* mqtt, MQTTWorker* worker) const ;
	void unregisterWorker (struct mosquitto* mqtt) const ;
	
protected:
	MQTTCallbackBridge() noexcept;
	
private:
	// Return the registered worker or throw
	MQTTWorker* findWorker (struct mosquitto* mosq) const;
	
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTCallbackBridge) ;

	// static attributes to be used from static operations and callbacks
	static MQTTWorkersType _mqttworkers;
	static Mutex _mutex;
};

_ACS_END_NAMESPACE

#endif //_MQTTCallbackBridge_H_
