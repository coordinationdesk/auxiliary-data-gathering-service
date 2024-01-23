// PRQA S 1050 EOF
/*
   Copyright 2019-2022 Exprivia S.p.A. - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A. ;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia S.p.A. 

   libMQTT Subscriber base class - HEADER FILE

   $Id: [CPS-101] $

*/ 

#ifndef _MQTTSubscriber_H_
#define _MQTTSubscriber_H_

#include <MQTTWorker.h>
#include <MosquittoMessageQueue.h>

// forward declaration of libmosquitto stuff
struct mosquitto;
struct mosquitto_message;

_ACS_BEGIN_NAMESPACE(acs) 

class StopController ;


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTSubscriber
--                                                                             --
-------------------------------------------------------------------------------*/

class MQTTSubscriber : public MQTTWorker // NOSONAR
{
public:
	MQTTSubscriber(const std::string& name, const std::string& topic, const MQTTWorker::Configuration& config,  StopController const& stopController = AppStopController()) ;
	MQTTSubscriber (const MQTTSubscriber&) = delete ;
	MQTTSubscriber& operator= (const MQTTSubscriber&) = delete ;

	virtual ~MQTTSubscriber() noexcept ;


	/**
		Set the QueueMaster flag
	*/
	void setQueueMaster(const bool b) { _queue_is_master = b ;}
	/**
		Get the QueueMaster flag
	*/
	bool getQueueMaster() const { return _queue_is_master ; }

	/**
		Set the external queue
	*/
	void setQueue(MosquittoMessageQueue* p) ;
	
	/**
		Get the external queue
	*/
	inline MosquittoMessageQueue* getQueue() const	{ return _queue_p ; }

	/** Setters/getters for the queue limits. A new queue is allocated if not provided yet */
	inline size_t get_queue_capacity()				{ allocateQueue() ; return _queue_p->get_queue_capacity() ; }
	inline size_t get_queue_trim_pcg()				{ allocateQueue() ; return _queue_p->get_queue_trim_pcg() ; }				
	inline void set_queue_capacity(const size_t& s)	{ allocateQueue() ; _queue_p->set_queue_capacity(s) ; }	
	inline void set_queue_trim_pcg(const size_t& s)	{ allocateQueue() ; _queue_p->set_queue_trim_pcg(s) ; }
	
	
	/** Handle the MQTT Connection parameters: host and port */
	inline void setPort(const int& port)			{ MQTTWorker::Configuration c(getConfiguration()) ; c.port = port ; setConfiguration(c) ; } // NOSONAR overload 
	inline void setHost(const std::string & host)	{ MQTTWorker::Configuration c(getConfiguration()) ; c.host = host ; setConfiguration(c) ; } // NOSONAR overload
	inline int getPort() const						{ return getConfiguration().port ; } // NOSONAR overload
	inline std::string getHost() const 				{ return getConfiguration().host ; } // NOSONAR overload
	
	
	/** Useful functions to check topics validity for subscribers */
	static bool checkTopic(const std::string& mqttTopic);
	static bool checkTopicWithError(const std::string& mqttTopic, std::string& error);

	// Set the topic, throws if the topic is not good for a subsription
	const std::string& getTopic() const;

	// It will start connection and register the subsription
	bool activateSubscription(MQTT_QOS qos, unsigned int timeoutMs = 5000); // NOSONAR overload

	// It will unsubscribe and terminate
	bool deactivateSubscription(unsigned int timeoutMs = 5000); // NOSONAR overload

	// Returns the actual subscription status
	bool isSubscribed() const; // NOSONAR overload

	// Returns the actual connected status
	bool isConnected() const; // NOSONAR overload

	// In case of compile-time flag DISABLE_MQTT set, return a pre-defined constant for methods: isSubscribed, isConnected
	inline static void set_mqtt_disabled_force_is_connected(const bool b=true)  { _mqtt_disabled_force_is_connected = b ; }
	inline static void set_mqtt_disabled_force_is_subscribed(const bool b=true) { _mqtt_disabled_force_is_subscribed = b ; }

protected:
	/**
		A new queue is allocated, if needed (i.e. if still a NULL pointer)
	*/
	void allocateQueue() ; 

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

	// Add to message queue:
	virtual void internal_onMessage (const mosquitto_message *msg) ;

	/**
		Start the thread in charge of emitting the messages in the queue, 
		i.e. to invoke the user's callback onMessage in case of messages in the queue.
	*/
	bool startQueueEmitterThread() ;

	/**
		Stop the thread in charge of emitting the messages in the queue, 
		i.e. to invoke the user's callback onMessage in case of messages in the queue.
	*/
	void stopQueueEmitterThread() ;

private:
	// attributes
	MosquittoMessageQueue* _queue_p ;
	bool _deallocate_queue ; 
	std::string _topic;
	bool _isSubscribed;
	MQTT_QOS _qos;
	bool _queue_is_master ;
	StopController const &_stopController ;
	static bool _mqtt_disabled_force_is_connected ;
	static bool _mqtt_disabled_force_is_subscribed ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTSubscriber) ;
};

_ACS_END_NAMESPACE

#endif //_MQTTSubscriber_H_
