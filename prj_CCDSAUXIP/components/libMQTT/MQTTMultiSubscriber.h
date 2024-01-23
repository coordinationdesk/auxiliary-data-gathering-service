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

#ifndef _MQTTMultiSubscriber_H_
#define _MQTTMultiSubscriber_H_

#include <MQTTSubscriber.h>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs) 


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTSubscribersPool
--                                                                             --
-------------------------------------------------------------------------------*/
class MQTTSubscribersPool // NOSONAR
{
public:
	MQTTSubscribersPool(const MQTTWorker::Configuration& config, StopController const& stopController) ;
	MQTTSubscribersPool (const MQTTSubscribersPool&) = delete ;
	MQTTSubscribersPool& operator= (const MQTTSubscribersPool&) = delete ;

	virtual ~MQTTSubscribersPool() noexcept ;

	/** Handle the MQTT Connection parameters: host and port */
	void setPort(const int& port)				 { _pool_config.port = port ; } // NOSONAR overload
	void setHost(const std::string & host)  	 { _pool_config.host = host ; } // NOSONAR overload
	int getPort() const 						 { return _pool_config.port ; } // NOSONAR overload
	std::string getHost() const 				 { return _pool_config.host ; } // NOSONAR overload


	// It will start connection and register the subsription
	bool activateSubscription(MQTTWorker::MQTT_QOS qos, unsigned int timeoutMs = 5000) ; // NOSONAR overload

	// It will unsubscribe and terminate
	bool deactivateSubscription(unsigned int timeoutMs = 5000) ; // NOSONAR overload

	// Returns the actual subscription status (for all the entries)
	bool isSubscribed() const; // NOSONAR overload

	/**
		Remove a specific MQTTSubscribers from the vector (the d'tor automatically stop the associated process)   
	*/	
	void remove_from_pool(const size_t& idx) ; 
	/**
		Remove all the MQTTSubscribers from the vector (the d'tor automatically stop the associated process)   
	*/	
	void clear_pool() ; 

protected:

	/**
		Add a new MQTTSubscribers, based on the already provided configuration 
	*/	
	size_t add_to_pool_with_queue(const std::string& name, const std::string& topic, MosquittoMessageQueue* queue_p) ; 

	MQTTWorker::Configuration get_pool_config(const bool randomize_id=true) const ;
	

private:
	struct MQTTWorker::Configuration				_pool_config ; 
	std::vector < std::unique_ptr<MQTTSubscriber> >	_subscribers_V ; 
	StopController const & 							_stopController ;
	static Mutex 									_pool_mutex ; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTSubscribersPool) ;
};


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTMultiSubscriber
--                                                                             --
-------------------------------------------------------------------------------*/
class MQTTMultiSubscriber : public MQTTSubscriber, public MQTTSubscribersPool // NOSONAR
{
public:
	MQTTMultiSubscriber(const MQTTWorker::Configuration& config, StopController const& stopController = AppStopController()) ;
	MQTTMultiSubscriber (const MQTTMultiSubscriber&) = delete ;
	MQTTMultiSubscriber& operator= (const MQTTMultiSubscriber&) = delete ;

	virtual ~MQTTMultiSubscriber() noexcept ;


	/** Handle the MQTT Connection parameters: host and port. Since this class does not directly connect to MQTT, only the pool is updated */
	void setPort(const int& port)				 { MQTTSubscribersPool::setPort(port) ; } // NOSONAR overload
	void setHost(const std::string & host)  	 { MQTTSubscribersPool::setHost(host) ; } // NOSONAR overload
	int getPort() const 						 { return MQTTSubscribersPool::getPort() ; } // NOSONAR overload
	std::string getHost() const 				 { return MQTTSubscribersPool::getHost() ; } // NOSONAR overload

	// It will start connection and register the subsription
	bool activateSubscription(MQTT_QOS qos, unsigned int timeoutMs = 5000) ; // NOSONAR overload

	// It will unsubscribe and terminate
	bool deactivateSubscription(unsigned int timeoutMs = 5000) ; // NOSONAR overload 

	// Returns the actual subscription status (for all the entries)
	inline bool isSubscribed() const { return MQTTSubscribersPool::isSubscribed() ; } // NOSONAR overload


	/**
		Add a new MQTTSubscribers, based on the already provided configuration 
	*/	
	size_t add_to_pool(const std::string& name, const std::string& topic) ; 

protected:

	// The MQTTMultiSubscriber is just a container for the master queue, and hosts the emitter thread.
	// Therefore _NO_ ACTUAL SUBSCRIPTION SHOULD EXISTS (to force that, an empty MQTT Configuration is enforced)
	// For this reason, the MQTT methods below should never be invoked:

	inline virtual void internal_onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS) \
		{ ACS_THROW(exIllegalValueException("This functionality should be disabled.")) ; } 
	inline virtual void internal_onUnsubscribe (int mid) \
		{ ACS_THROW(exIllegalValueException("This functionality should be disabled.")) ; } 
	inline virtual void internal_onConnect (int rc) \
		{ ACS_THROW(exIllegalValueException("This functionality should be disabled.")) ; } 
	inline virtual void internal_onDisconnect (int rc) \
		{ ACS_THROW(exIllegalValueException("This functionality should be disabled.")) ; } 
	inline virtual void internal_onMessage (const mosquitto_message *msg) \
		{ ACS_THROW(exIllegalValueException("This functionality should be disabled.")) ; } 

private:
	StopController const & _stopController ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MQTTMultiSubscriber) ;
};


_ACS_END_NAMESPACE

#endif //_MQTTMultiSubscriber_H_
