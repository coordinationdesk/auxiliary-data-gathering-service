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


#include <MQTTSubscriber.h>
#include <MosquittoMessageQueue.h>
#include <Filterables.h>

#include <Timer.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTSubscriber)

bool MQTTSubscriber::_mqtt_disabled_force_is_connected = false ;
bool MQTTSubscriber::_mqtt_disabled_force_is_subscribed = false ; 


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MQTTSubscriber
--                                                                             --
-------------------------------------------------------------------------------*/

MQTTSubscriber::MQTTSubscriber(const std::string& name, const std::string& topic, const MQTTWorker::Configuration& config, StopController const& stopController) :
	MQTTWorker (name, config, stopController),
	_queue_p(nullptr),
	_deallocate_queue(false),
	_topic(),
	_isSubscribed(),
	_qos(QOS_0),
	_queue_is_master(true),
	_stopController(stopController)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"")
	ACS_CLASS_END_DEBUG

	setTopic(topic);
	setMessagesAreQueuedExternally(true) ; /* As default, the messages will be handled by an instance of thread-class: MosquittoMessageQueue */
}

MQTTSubscriber::~MQTTSubscriber() ACS_NOEXCEPT 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"")
	ACS_CLASS_END_DEBUG

	try
	{
		// Stop the queue emitter thread
		if ( (true == getMessagesAreQueuedExternally()) && (true == _queue_is_master) && (nullptr != _queue_p) ) 
		{ stopQueueEmitterThread() ; }
	
		// Deactivate the subscription before destroyng. Also it stops the queue, if any.
		deactivateSubscription();
		
		// Disconnet and shut down the mqtt loop event
		// interrupting also callback notifications
		shutDownMQTTLayer();
		
		if ( _deallocate_queue && (nullptr != _queue_p) )
		{  
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Deallocating the queue")
			ACS_CLASS_END_DEBUG

			delete _queue_p ; 
			_queue_p = nullptr ; 
		}
		
	}
	catch (/*NOSONAR*/const std::exception& ex)
	{
		ACS_LOG_WARNING ("Exception got during dtor ~MQTTSubscriber");
		ACS_LOG_PRIVINFO (ex);
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"")
	ACS_CLASS_END_DEBUG
}


void MQTTSubscriber::allocateQueue() 
{ 
	// Check if the external queue is employed
	ACS_COND_THROW( (false == getMessagesAreQueuedExternally()), exIllegalValueException("Error: Operation on External Queue not supported")) ;
	
	/**
		A new queue is allocated, if needed (i.e. if still a NULL pointer)
	*/

	if (nullptr == _queue_p)
	{
		_queue_p = new MosquittoMessageQueue(getName(), *this, _stopController) ; 
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Allocated a new queue with pointer: " << static_cast<void*>(_queue_p))
		ACS_CLASS_END_DEBUG

		ACS_COND_THROW ( (nullptr == _queue_p), exIllegalValueException("Invalid (NULL) pointer was obtained.") ) ;
		_deallocate_queue = true ;	
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Pointer has value: " << static_cast<void*>(_queue_p))
	ACS_CLASS_END_DEBUG
} 


void MQTTSubscriber::setQueue(MosquittoMessageQueue* p) 
{ 
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Pointer has value: " << static_cast<void*>(_queue_p))
	ACS_CLASS_END_DEBUG

	// Check if the external queue is employed
	ACS_COND_THROW( (false == getMessagesAreQueuedExternally()), exIllegalValueException("Error: Operation on External Queue not supported")) ;

	ACS_COND_THROW( (nullptr != _queue_p), exIllegalValueException("Internal error: Setting a pointer already valid") ) ; 
	_queue_p = p ; _deallocate_queue = false ; 
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Pointer has value: " << static_cast<void*>(_queue_p))
	ACS_CLASS_END_DEBUG
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
void MQTTSubscriber::setTopic(const std::string& topic)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Going to set the topic: \"" << topic << "\"")
	ACS_CLASS_END_DEBUG
	
	// Safety check first
	std::string mqtterr;
	if (!checkTopicWithError(topic, mqtterr))
	{
		ACS_THROW ( MQTTSubscriber::BadTopic () << "Trying to set an invalid subscription topic. Error is: " << mqtterr );
	}
	
	// Lock on parent mutex
	Mutex::Lock lock (getMutex());

	// Ok can update the topic now
	_topic = topic;
}

const std::string& MQTTSubscriber::getTopic () const
{
	// Lock on parent mutex
	Mutex::Lock lock (getMutex());

	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method returning topic: \"" << _topic << "\"")
	ACS_CLASS_END_DEBUG
	
	return _topic;
}

bool MQTTSubscriber::isSubscribed() const
{
#ifndef DISABLE_MQTT

	// Lock on parent mutex
	Mutex::Lock lock (getMutex());

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method finished.  Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"" \
			<< "  QueueIsMaster: " << (_queue_is_master?"T":"F")  \
			<< "  MQTT Host/Port : " << getHost() \
			<< " : " << getPort() << "  Returning: " << (_isSubscribed?"T":"F") ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG

	return _isSubscribed;
#else
	return _mqtt_disabled_force_is_subscribed ; 
#endif
}


bool MQTTSubscriber::isConnected() const
{
#ifndef DISABLE_MQTT
	return MQTTWorker::isConnected() ;
#else
	return _mqtt_disabled_force_is_connected ; 
#endif
}


void MQTTSubscriber::setSubscribed(bool s)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called with argument: " << (s?"T":"F"))
	ACS_CLASS_END_DEBUG
	
	// Lock on parent mutex
	Mutex::Lock lock (getMutex());
	_isSubscribed = s;
}

MQTTWorker::MQTT_QOS MQTTSubscriber::getQOS() const
{
	// Lock on parent mutex
	Mutex::Lock lock (getMutex());
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method returning: " << MQTT_QOSToString(_qos))
	ACS_CLASS_END_DEBUG
	
	return _qos;
}

void MQTTSubscriber::setQOS(MQTT_QOS qos)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called with argument: " << MQTT_QOSToString(qos))
	ACS_CLASS_END_DEBUG
	
	// Lock on parent mutex
	Mutex::Lock lock (getMutex());
	_qos = qos;
}


bool MQTTSubscriber::activateSubscription(MQTT_QOS qos, unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called.  Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"" \
			<< "  QueueIsMaster: " << (_queue_is_master?"T":"F")  \
			<< "  QOS: " << MQTT_QOSToString(qos) << "  timeoutMs: " << timeoutMs << "  MQTT Host/Port : " \
			<< getHost() << " : " << getPort() << ".  " ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG
	
	bool s (true) ;
	
#ifndef DISABLE_MQTT
	if (true == getMessagesAreQueuedExternally())
	{
		if (true == _queue_is_master)
		{
			/**
				Start the thread in charge of emitting the messages in the queue, 
				i.e. to invoke the user's callback onMessage in case of messages in the queue.
			*/

			startQueueEmitterThread() ;
		}
		else if (nullptr == _queue_p)
		{
			// IF HERE:
			// The MQTTSubscriber is _NOT_ a queue-master (so, it relies on some external queue) but the queue
			// has not been provided via a call to method: setQueue
			//
			// THIS IS A MISTAKE !
			//

			ACS_THROW ( exIllegalValueException("Internal Error: a valid pointer for the external queue was not provided") ) ;
		}
	}	

	// To activate a subscription:
	// - a valid qos must be provided
	// - no subscriptions shall be active
	// - a good topic must be set
	
	if ( false == getConfiguration().host.empty() )
	{
		// Empty topic check
		ACS_COND_THROW ( getTopic().empty(), PreconditionFailure ("Cannot subscribe. Topic not set") );

		// No active subsciptions
		ACS_COND_THROW ( (true == isSubscribed() ) , PreconditionFailure ("Cannot subscribe. Another subscription is active.") );

		setQOS (qos);

		// Ok.. now connect. This shall not lock or deadlock
		// Will force the underlying MQTT loop thread to stop and restart
		connect(timeoutMs);

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

		s = isSubscribed() ;
		ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "MQTT Subscriber started. Topic: \"" << getTopic() << "\"  TimeWait: " << timeoutMs << "  Connected to MQTT ? " << (isConnected()? "TRUE" : "FALSE") ); 
	}
	else
	{
        ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Name: \"" << getName() << "\" ClientId: \"" << getConfiguration().clientId << "\" MQTT Configuration is serverless.");
	}
#endif

	ACS_CLASS_WRITE_DEBUG_TH (ACS_MID_VERB, "Method finished.  Name: \"" << getName() << "\"" << "  ClientId: \"" << getConfiguration().clientId << "\"" \
			<< "  QueueIsMaster: " << (_queue_is_master?"T":"F") << "  QOS: " << MQTT_QOSToString(qos) << "  timeoutMs: " << timeoutMs << "  MQTT Host/Port : " \
			<< getHost() << " : " << getPort() << ".  Returning: " << (s?"T":"F") );

	return s ;
}


bool MQTTSubscriber::deactivateSubscription(unsigned int timeoutMs)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called. Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"" \
			<< "  QueueIsMaster: " << (_queue_is_master?"T":"F") \
			<<"   timeoutMs: " << timeoutMs << "  MQTT Host/Port : " << getHost() << " : " << getPort() << ".  " ;
		ACS_LOG_DEBUG(o.str()) ;	
	ACS_CLASS_END_DEBUG

	bool s(true) ; 
	
#ifndef DISABLE_MQTT

	if ( false == getConfiguration().host.empty() )
	{
		// unsubscribe if the session is clean
		if (true == getConfiguration().clean_session)
		{
			// ignore errors if any
			int mqResult = mosquitto_unsubscribe (getClient(), nullptr, getTopic().c_str());
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

				while ( (true == isSubscribed()) && ( counter < maxCount ) /* && ( false == _stopController.requestedCancel() ) */ ) /* NOSONAR */
				{
					// Delay the sleepTime
					Timer::delay (sleepPeriod);
					++counter;
				}
			}
		}

		// close also the connection (REALLY?)
		disconnect();

		s = (! isSubscribed()) ;
	}

	// Clean the queue, to immediately get rid of any previous message still stacked
	if ( (true == getMessagesAreQueuedExternally()) && (true == _queue_is_master) )
	{
		if (nullptr != _queue_p)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Cleaning the queue")
			ACS_CLASS_END_DEBUG

			_queue_p->clear_queue(); 	
		}
	}
#endif

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method finished.  Name: \"" << getName() << "\"" \
			<< "  ClientId: \"" << getConfiguration().clientId << "\"" \
			<< "  QueueIsMaster: " << (_queue_is_master?"T":"F") \
			<<"   timeoutMs: " << timeoutMs << "  MQTT Host/Port : " << getHost() << " : " << getPort() \
			<< "  Returning: " << (s?"T":"F") ;
		ACS_LOG_DEBUG(o.str()) ; 
	ACS_CLASS_END_DEBUG

	return s ;
}

void MQTTSubscriber::internal_onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS)
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
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)

#ifndef DISABLE_MQTT
	// Do subscribe... connection has been just created or restored
	// After connection send the subscription request
	// Fire the message
	int mqResult = mosquitto_subscribe (getClient(), nullptr, getTopic().c_str(), getQOS());

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
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	// Disconnected... subscription is not valid anymore
	setSubscribed(false);
}


/*-------------------------------------------------------------------------------

		MQTTSubscriber::internal_onMessage

-------------------------------------------------------------------------------*/
void MQTTSubscriber::internal_onMessage (const mosquitto_message *msg) 
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. QueueIsMaster: " << (_queue_is_master?"T":"F"))
	ACS_CLASS_END_DEBUG

	if (true == getMessagesAreQueuedExternally())
	{
		// Copy the received message (that will be immediately purged by the MQTT infrastructure) and store it to the external queue.
		ACS_COND_THROW( (nullptr == _queue_p), exIllegalValueException("Internal error: Queue not provided.") ) ; 
	
		// Check if the queue emitter thread is running (as expected).
		// If not running, an exception is emitted in order to warn the user.
		const bool emit_exception(true) ; 
		_queue_p->self_check(emit_exception) ; 

		_queue_p->push_to_queue_and_signal(msg) ; 

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			std::ostringstream o ; o << SimpleDebugSignatureThread << "Messages in queue: " << _queue_p->queue_size() ;
			ACS_LOG_DEBUG(o.str()) ; 
		ACS_CLASS_END_DEBUG
	}
	// else: nothing to do, since the MQTTWorker will automatically call upper class' method: onMessage
}


/*-------------------------------------------------------------------------------

		MQTTSubscriber::startQueueEmitterThread

-------------------------------------------------------------------------------*/
bool MQTTSubscriber::startQueueEmitterThread() 
{
	bool r (false) ; 
#ifndef DISABLE_MQTT
	/**
		Start the thread in charge of emitting the messages in the queue, 
		i.e. to invoke the user's callback onMessage in case of messages in the queue.
	*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called.")
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW( (false == getMessagesAreQueuedExternally()), exIllegalValueException("Error: Operation on External Queue not supported")) ;

	/**
		If queue is still a null pointer, a new queue is allocated. O/W: Do nothing.
	*/
	allocateQueue() ;
		
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Thread Running? " << (_queue_p->running()?"T":"F"))
	ACS_CLASS_END_DEBUG

	/* 
		The condition of checking if the thread is _not_ yet running is probably useless.
		Theoretically, the user should call method activateSubscription just once, then before calling it again, he should call
		method deactivateSubscription (which also stops the thread) and so on.
		One possible reason to call method activateSubscription twice is that activateSubscription has been called 
		but it has returned false (not subscribed), so the user tried to change some parameter and call it a second time, 
		without calling deactivateSubscription (since never activated).
		This is probably a bad behaviour, but also a test has been built it this way.
		Therefore, for legacy and tests compatibility, it should be possible to call this method more than once, 
		thus it must be checked if the thread is not already running.
	*/


	// Start the thread in charge of periodically calling the upper-level onMessage:
	if ( (false == _queue_p->running()) && (false == _stopController.requestedCancel()) )
	{
		_queue_p->start() ; 	

		// Ensure that the thread was successfully started -- otherwise throw
		for (size_t i=0; ((i<1000) && (false == _queue_p->running()) && (false == _stopController.requestedCancel())) ; i++) 
		{ usleep (5000) ; /* 5 ms */ }

		ACS_COND_THROW( ((false == _queue_p->running()) && (false == _stopController.requestedCancel()) && (false == _queue_p->requestedCancel())), \
			exIllegalValueException("Unable to start thread") ) ; 
	}
	
	r = _queue_p->running() ;
#endif	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Running? " << (r?"T":"F"))
	ACS_CLASS_END_DEBUG

	return r ;
}


/*-------------------------------------------------------------------------------

		MQTTSubscriber::stopQueueEmitterThread

-------------------------------------------------------------------------------*/
void MQTTSubscriber::stopQueueEmitterThread() 
{
#ifndef DISABLE_MQTT
	/**
		Stop the thread in charge of emitting the messages in the queue, 
		i.e. to invoke the user's callback onMessage in case of messages in the queue.
	*/
	if (nullptr != _queue_p) /*&& (_queue_p->running())*/ 
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Running? " << (_queue_p->running()?"T":"F"))
		ACS_CLASS_END_DEBUG

		// Stop the thread in charge of periodically calling the upper-level onMessage:
		_queue_p->stop() ; 	

		// Ensure that the thread was successfully stopped -- otherwise throw
		for (size_t i=0; ((i<1000) && (true == _queue_p->running())); i++) { usleep (5000) ; /* 5 ms */ }
		ACS_COND_THROW( (true == _queue_p->running()), exIllegalValueException("Unable to stop thread") ) ; 

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Running? " << (_queue_p->running()?"T":"F"))
		ACS_CLASS_END_DEBUG
	}
	else
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Nothing to do.")
		ACS_CLASS_END_DEBUG
	}
#endif	
}


_ACS_END_NAMESPACE
