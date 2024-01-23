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
class MQTTCallbackBridge : public Singleton<MQTTCallbackBridge> // PRQA S 2109, 2153
{
	friend MQTTCallbackBridge* Singleton<MQTTCallbackBridge>::instance(); // PRQA S 2107

public:
	// Helper type definition
	typedef std::map < struct mosquitto*, MQTTWorker* > MQTTWorkersType;

	virtual ~MQTTCallbackBridge() ACS_NOEXCEPT ;

	static void connectCallback (struct mosquitto *mosq, void *obj, int rc);
	static void disconnectCallback (struct mosquitto *mosq, void *obj, int rc);
	static void publishCallback (struct mosquitto *mosq, void *obj, int mid);
	static void messageCallback (struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
	static void subscribeCallback (struct mosquitto *mosq, void *obj, int mid, int qos_count, const int * granted_qos);
	static void unsubscribeCallback (struct mosquitto *mosq, void *obj, int mid);
	static void logCallback (struct mosquitto *mosq, void *obj, int level, const char *str);
	
	void registerWorker (struct mosquitto* mqtt, MQTTWorker* worker);
	void unregisterWorker (struct mosquitto* mqtt);
	
protected:
	MQTTCallbackBridge() ACS_NOEXCEPT;
	
private:
	// Return the registered worker or throw
	MQTTWorker* findWorker (struct mosquitto* mosq) const;
	
private:
	// Cannot copy a singleton
	MQTTCallbackBridge(const MQTTCallbackBridge &);
	MQTTCallbackBridge & operator= (const MQTTCallbackBridge &);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTCallbackBridge) ;
	
private:
	// static attributes to be used from static operations and callbacks
	static MQTTWorkersType _mqttworkers;
	static Mutex _mutex;
};

_ACS_END_NAMESPACE

#endif //_MQTTCallbackBridge_H_
