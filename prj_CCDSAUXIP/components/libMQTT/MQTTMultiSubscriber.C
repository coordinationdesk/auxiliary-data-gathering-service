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

   libMQTT Subscriber base class

   $Id: [CPS-101] $

*/ 


#include <MQTTMultiSubscriber.h>
#include <Filterables.h>

#include <Timer.h>

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTSubscribersPool)
ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTMultiSubscriber)

ThreadSafe MQTTSubscribersPool::_pool_mutex ;

/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTSubscribersPool
--                                                                             --
-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::MQTTSubscribersPool

-------------------------------------------------------------------------------*/
MQTTSubscribersPool::MQTTSubscribersPool(const MQTTWorker::Configuration& config, StopController const& stopController) 
:	_pool_config(config),
	_subscribers_V(),
	_stopController(stopController)
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::~MQTTSubscribersPool

-------------------------------------------------------------------------------*/
MQTTSubscribersPool::~MQTTSubscribersPool() ACS_NOEXCEPT 
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::get_pool_config

-------------------------------------------------------------------------------*/
MQTTWorker::Configuration MQTTSubscribersPool::get_pool_config(const bool randomize_id) const
{
	if (true == randomize_id)
	{
		return (MQTTWorker::buildConfiguration(_pool_config)) ; // Randomize clientId
	}

	return _pool_config ;
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::activateSubscription

-------------------------------------------------------------------------------*/
bool MQTTSubscribersPool::activateSubscription(MQTTWorker::MQTT_QOS qos, unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG

	ThreadSafe::Lock lock(_pool_mutex) ;

	bool is_subscribed( true ) ; // Initial value, needed to handle the AND operator
	bool found(false) ; 

	for (size_t i=0; i<_subscribers_V.size(); i++)
	{
		if (nullptr != _subscribers_V.at(i))
		{
			const bool b (_subscribers_V.at(i)->activateSubscription(qos, timeoutMs)) ;
			is_subscribed = (is_subscribed && b) ; 
			found = true ; 
		}
	}
		
	if (false == found) { is_subscribed = false ; }

	return is_subscribed ;
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::deactivateSubscription

-------------------------------------------------------------------------------*/
bool MQTTSubscribersPool::deactivateSubscription(unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG

	ThreadSafe::Lock lock(_pool_mutex) ;

	bool is_subscribed( false ) ; // Initial value

	for (size_t i=0; i<_subscribers_V.size(); i++)
	{
		if (nullptr != _subscribers_V.at(i))
		{
			const bool b (_subscribers_V.at(i)->deactivateSubscription(timeoutMs)) ;
			is_subscribed = (is_subscribed || b) ; 
		}
	}
		
	return !is_subscribed;
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::isSubscribed

-------------------------------------------------------------------------------*/
bool MQTTSubscribersPool::isSubscribed() const
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
	
	ThreadSafe::Lock lock(_pool_mutex) ;
	
	bool is_subscribed( true ) ; // Initial value, needed to handle the AND operator
	bool found(false) ; 

	for (size_t i=0; i<_subscribers_V.size(); i++)
	{
		if (nullptr != _subscribers_V.at(i))
		{
			const bool b (_subscribers_V.at(i)->isSubscribed()) ;
			is_subscribed = (is_subscribed && b) ; 
			found = true ;
		}
	}

	if (false == found) { is_subscribed = false ; }

	return is_subscribed ; 
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::add_to_pool

-------------------------------------------------------------------------------*/
size_t MQTTSubscribersPool::add_to_pool_with_queue(const std::string& name, const std::string& topic, MosquittoMessageQueue* queue_p) 
{
	/**
		Add a new MQTTSubscribers, based on the already provided configuration 
	*/	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Name: \"" << name << "\"  Topic: \"" << topic << "\"") ;
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW ( (nullptr == queue_p), exIllegalValueException("Internal Error: a valid pointer for the external queue was not provided") ) ; 

	std::unique_ptr<MQTTSubscriber> p_element ( std::make_unique<MQTTSubscriber>(name, topic, get_pool_config(), _stopController) ) ;
	ACS_COND_THROW ( (nullptr == p_element), exIllegalValueException("MQTTSubscriberElement: Null pointer") ) ; 

	// Each subscriber is not a queue master, and received the same common queue
	p_element->setQueueMaster(false) ;
	p_element->setQueue(queue_p) ;
	size_t sz ;

	{
		ThreadSafe::Lock lock(_pool_mutex) ;
		_subscribers_V.push_back( std::unique_ptr<MQTTSubscriber>(nullptr) ) ; // Add an initially empty value
		p_element.swap(_subscribers_V.back()) ; // Atomically move the pointer to the vector
		sz = _subscribers_V.size() ; 
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << sz) ;
	ACS_CLASS_END_DEBUG
	
	return sz ;
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::clear_pool

-------------------------------------------------------------------------------*/
void MQTTSubscribersPool::clear_pool() 
{
	/**
		Remove all the MQTTSubscribers from the vector (the d'tor automatically stop the associated process)   
	*/

	ThreadSafe::Lock lock(_pool_mutex) ;

	_subscribers_V.clear() ; // Also, remove all the elements from the vector
}


/*-------------------------------------------------------------------------------

		MQTTSubscribersPool::remove_from_pool

-------------------------------------------------------------------------------*/
void MQTTSubscribersPool::remove_from_pool(const size_t& idx) 
{
	/**
		Remove a specific MQTTSubscribers from the vector (the d'tor automatically stop the associated process)   
	*/

	ThreadSafe::Lock lock(_pool_mutex) ;

	ACS_COND_THROW ( (idx>=_subscribers_V.size()), exIllegalValueException("Index out of range") ) ;

	if (nullptr != _subscribers_V.at(idx))
	{
		_subscribers_V.at(idx).reset(nullptr) ; 
	}
	
	// Note that the element is _NOT_ removed from the vector. It is a NULL pointer so it will be ignored
}


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTMultiSubscriber
--                                                                             --
-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------

		MQTTMultiSubscriber::MQTTMultiSubscriber

-------------------------------------------------------------------------------*/
MQTTMultiSubscriber::MQTTMultiSubscriber(	const MQTTWorker::Configuration& config, 
											StopController const& stopController) :
	MQTTSubscriber (("MULTISUBSCRIBER_" + buildConfiguration().clientId), /* Just a meaningless unique Id */			
					"MULTISUBSCRIBER_EMPTY_TOPIC", 
					buildConfiguration(), /* Empty configuration -- PROVIDED MQTT CONF. WILL BE EMPLOYED ONLY IN MQTTSubscribersPool */
					stopController),
	MQTTSubscribersPool(config, stopController), 
	_stopController(stopController)
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
}


/*-------------------------------------------------------------------------------

		MQTTMultiSubscriber::~MQTTMultiSubscriber

-------------------------------------------------------------------------------*/
MQTTMultiSubscriber::~MQTTMultiSubscriber() ACS_NOEXCEPT 
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
}

/*-------------------------------------------------------------------------------

		MQTTMultiSubscriber::add_to_pool

-------------------------------------------------------------------------------*/
size_t MQTTMultiSubscriber::add_to_pool(const std::string& name, const std::string& topic) 
{
	/**
		Add a new MQTTSubscribers, based on the already provided configuration 
	*/	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Name: \"" << name << "\"  Topic: \"" << topic << "\"") ;
	ACS_CLASS_END_DEBUG

	// If queue is still a null pointer, a new queue is allocated. O/W: Do nothing. 
	allocateQueue() ; 

	ACS_COND_THROW ( (nullptr == getQueue()), exIllegalValueException("Internal Error: a valid pointer for the external queue was not allocated") ) ; 
		
	return MQTTSubscribersPool::add_to_pool_with_queue(name, topic, getQueue()) ;
}


/*-------------------------------------------------------------------------------

		MQTTMultiSubscriber::activateSubscription

-------------------------------------------------------------------------------*/
bool MQTTMultiSubscriber::activateSubscription(MQTT_QOS qos, unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG

	/**
		Start the thread in charge of emitting the messages in the queue, 
		i.e. to invoke the user's callback onMessage in case of messages in the queue.
	*/
	startQueueEmitterThread() ;


	// Activate the base class:
	const bool is_base_subscribed ( MQTTSubscriber::activateSubscription(qos, timeoutMs) ) ;

	// Activate each single subscriber:
	const bool is_pool_subscribed( MQTTSubscribersPool::activateSubscription(qos, timeoutMs) ) ;


	const bool is_subscribed (is_base_subscribed && is_pool_subscribed) ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << (is_subscribed?"T":"F")) ;
	ACS_CLASS_END_DEBUG

	return is_subscribed ;
}


/*-------------------------------------------------------------------------------

		MQTTMultiSubscriber::deactivateSubscription

-------------------------------------------------------------------------------*/
bool MQTTMultiSubscriber::deactivateSubscription(unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG


	// Deactivate the base class:
	const bool is_base_deactivated ( MQTTSubscriber::deactivateSubscription(timeoutMs) ) ;

	// Deactivate each single subscriber:
	const bool is_pool_subscribed( MQTTSubscribersPool::deactivateSubscription(timeoutMs) ) ;


	const bool is_deactivated (is_base_deactivated && is_pool_subscribed) ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << (is_deactivated?"T":"F")) ;
	ACS_CLASS_END_DEBUG

	return is_deactivated ;
}


_ACS_END_NAMESPACE
