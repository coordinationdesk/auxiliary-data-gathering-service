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


#include <MosquittoMessageQueue.h>
#include <Filterables.h>

#include <Timer.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif
#include <string.h>
#include <unistd.h>


_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosquittoMessageQueue)

ThreadSafe MosquittoMessageQueue::_mqtt_queue_mutex ;


/*-------------------------------------------------------------------------------
--                                                                             --
		Class: MosquittoMessage
--                                                                             --
-------------------------------------------------------------------------------*/
MosquittoMessage::MosquittoMessage()
#ifndef DISABLE_MQTT
: _msg()
{
	// No memory is allocated, pointers are all-zero:
	memset(&_msg, 0, sizeof(_msg)) ;
}
#else
{}
#endif

MosquittoMessage::MosquittoMessage(const mosquitto_message* msgp_in) 
#ifndef DISABLE_MQTT
: _msg()
{
	// So far, no memory is allocated, pointers are all-zero:
	memset(&_msg, 0, sizeof(_msg)) ;
	// The external message is copied (i.e. NEW MEMORY IS malloc-ALLOCATED) in the internal structure. (*3) -> see also: (*4)
	const int mqResult ( mosquitto_message_copy(&_msg, msgp_in) ) ;
	if (MOSQ_ERR_SUCCESS != mqResult) 
	{
		std::ostringstream o ; o << "Invalid return copying MQTT message: " << mosquitto_strerror (mqResult) ;		
		ACS_THROW( exIllegalValueException(o.str()) ) ;		
	}
}
#else
{}
#endif

MosquittoMessage::~MosquittoMessage() noexcept 
{
#ifndef DISABLE_MQTT
	// free-DEALLOCATE memory (assuming it is allocated, i.e. pointers are different from zero. O/W the free will be ignored)
	// (*4) -> see also: (*3)
	mosquitto_message_free_contents(&_msg) ; memset(&_msg, 0, sizeof(_msg)) ; 
#endif
}


MosquittoMessage::MosquittoMessage(const MosquittoMessage& msg_in) 
#ifndef DISABLE_MQTT
: _msg()
{
	memset(&_msg, 0, sizeof(_msg)) ;
	// The external message is copied (i.e. NEW MEMORY IS malloc-ALLOCATED) in the internal structure. (*3) -> see also: (*4)
	const int mqResult ( mosquitto_message_copy(&_msg, msg_in.ptr()) ) ;
	if (MOSQ_ERR_SUCCESS != mqResult) 
	{
		std::ostringstream o ; o << "Invalid return copying MQTT message: " << mosquitto_strerror (mqResult) ;		
		ACS_THROW( exIllegalValueException(o.str()) ) ;		
	}
}
#else
{}
#endif

MosquittoMessage& MosquittoMessage::operator= (const MosquittoMessage& msg_in)
{
#ifndef DISABLE_MQTT
	if (this != &msg_in) 
	{
		// This is the assigment operator. First, it must deallocate the memory of the recepient structure, which will be lost 
		// (assuming it is allocated, i.e. pointers are different from zero)
		mosquitto_message_free_contents(&_msg) ; memset(&_msg, 0, sizeof(_msg)) ; 

		// The external message is copied (i.e. NEW MEMORY IS malloc-ALLOCATED) in the internal structure. (*3) -> see also: (*4)
		const int mqResult ( mosquitto_message_copy(&_msg, msg_in.ptr()) ) ;
		if (MOSQ_ERR_SUCCESS != mqResult) 
		{
			std::ostringstream o ; o << "Invalid return copying MQTT message: " << mosquitto_strerror (mqResult) ;		
			ACS_THROW( exIllegalValueException(o.str()) ) ;		
		}
	}
#endif
	return *this;
}



/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::MosquittoMessageQueue

-------------------------------------------------------------------------------*/
MosquittoMessageQueue::MosquittoMessageQueue(const std::string& name, MQTTWorker& sub, StopController const& stopController) 
:
	Thread("Queue_" + name),
	_sub(sub),
	_mqtt_messages_queue(),
	_queue_capacity(100*1024),
	_queue_trim_pcg(50 /* Queue reduced up to 50% */),
	_condition(),
	_stopController(stopController)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG
}

/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::~MosquittoMessageQueue

-------------------------------------------------------------------------------*/
MosquittoMessageQueue::~MosquittoMessageQueue() noexcept 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Running: " << (running()?"T":"F")) ;
	ACS_CLASS_END_DEBUG
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::self_check

-------------------------------------------------------------------------------*/
bool MosquittoMessageQueue::self_check(const bool emit_exception) const
{
	/**
		Check if the thread is running. If requested, throw an exception when not running in order to raise the issue to the user. 
	*/

	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called") ;
	ACS_CLASS_END_DEBUG

	const bool status (running()) ;
	
	if (false == status)
	{
		std::shared_ptr <const std::exception> e ( runException() ) ;
		try
		{
			if(nullptr != e)
			{
				ACS_THROW(exIllegalValueException(*e, "External Queue Emitter Thread: MosquittoMessageQueue is not running.")) ; 
			}
			else 
			{
				ACS_THROW(exIllegalValueException("External Queue Emitter Thread: MosquittoMessageQueue is not running.")) ; 
			}
		}
		catch (/*NOSONAR*/const std::exception& exc)
		{
			ACS_LOG_ERROR(exc.what()) ;
			if (true == emit_exception) { throw ; }
		}
	}

	return status ; 
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::queue_size

-------------------------------------------------------------------------------*/
size_t MosquittoMessageQueue::queue_size() const
{
	/**
		Get queue size. 
	*/
	
	size_t qs ; 
	
	{
		ThreadSafe::Lock lock(_mqtt_queue_mutex) ;
		qs = _mqtt_messages_queue.size() ; 
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method finished. Returning: " << qs ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG

	return qs ;
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::push_to_queue

-------------------------------------------------------------------------------*/
size_t MosquittoMessageQueue::push_to_queue(const mosquitto_message *msg_p)
{
	/**
		Add a message to the FIFO internal queue. Bulk-remove messages from the queue if overflooding.
	*/

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called. Initial queue size: " << queue_size() ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG
	
	size_t qs ; 
	
	{
		ThreadSafe::Lock lock(_mqtt_queue_mutex) ;
				
		MosquittoMessage msg_to_store(msg_p) ;
		_mqtt_messages_queue.push_back(msg_to_store) ; 
		qs = _mqtt_messages_queue.size() ; 
	}
	
	if (qs > get_queue_capacity())
	{
		std::ostringstream o ; 
		o << "MQTT Message queue exceeded its maximum capacity: " << qs << " vs. " << get_queue_capacity() \
			<< ". Oldest messages in the queue will be forcibly removed" ;
		ACS_LOG_WARNING(o.str()) ; 
	
		qs = trim_queue_to_pcg( get_queue_trim_pcg() ) ; 
	}
	
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Queue new size: " << qs) ;
	ACS_CLASS_END_DEBUG
	
	return qs ;
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::pop_from_queue

-------------------------------------------------------------------------------*/
bool MosquittoMessageQueue::pop_from_queue(MosquittoMessage& msg_out)
{
	/**
		Extract the first (i.e.) oldest message the FIFO internal queue, if any.
	*/

	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called. Queue size: " << queue_size() ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG

	bool found (false) ; 
	size_t qs ; 
	
	{
		ThreadSafe::Lock lock(_mqtt_queue_mutex) ;

		ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Current queue size: " << _mqtt_messages_queue.size()) ;
		ACS_CLASS_END_DEBUG

		if ( true == (found = (not _mqtt_messages_queue.empty())) )
		{
			msg_out = _mqtt_messages_queue.front() ; // The raw-copy here is used to deallocate the message previously stored in the queue
			_mqtt_messages_queue.pop_front() ; 
		}
	
		qs = _mqtt_messages_queue.size() ; 
	
	} // END: SCOPE
	
	if (true == found)
	{
		ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << (found?"T":"F") << " remaining queue size: " << qs) ;
		ACS_CLASS_END_DEBUG
	} else {
		ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
			ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << (found?"T":"F") << " remaining queue size: " << qs) ;
		ACS_CLASS_END_DEBUG
	}

	return found ; 
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::push_to_queue_and_signal

-------------------------------------------------------------------------------*/
size_t MosquittoMessageQueue::push_to_queue_and_signal(const mosquitto_message *msg_p)
{
	/**
		Add a message to the FIFO internal queue, and send a signal to the waiting thread. 
	*/

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called. Initial queue size: " << queue_size() ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG
	
	size_t sz ;
	
	// Add a message to the queue and signal it
	{		
		// Note that the caller, when executing a wait, will remain halted UNTIL the scope of this mutex is lost.
		Mutex::Lock lock (_condition);
		sz = push_to_queue(msg_p) ; 
		_condition.signal() ;	
	}
	
	// Out of the mutex scope. Only at this moment the caller will be allowed to leave the wait 
		
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Current queue size: " << sz) ;
	ACS_CLASS_END_DEBUG

	return sz ;
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::wait_and_pop_from_queue

-------------------------------------------------------------------------------*/
bool MosquittoMessageQueue::wait_and_pop_from_queue(MosquittoMessage& msg_out, const size_t& max_wait_msec)
{
	/**
		Extract the oldest message from the FIFO queue, if any. If empty, wait until the "push" signal is received or the timeout is reached.
	*/
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		std::ostringstream o ; o << SimpleDebugSignatureThread << "Method called. Queue size: " << queue_size() ;
		ACS_LOG_DEBUG(o.str()) ;
	ACS_CLASS_END_DEBUG

	bool found (false) ; 
	
	if (false == (found = pop_from_queue(msg_out)))
	{
		{
			Mutex::Lock lock (_condition);
			_condition.timedwait(max_wait_msec) ;

			// WHEN HERE: Either the timer has expired, or a signal interrupted it.
		}
		
		found = pop_from_queue(msg_out) ;
	}
		
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << (found?"T":"F")) ;
	ACS_CLASS_END_DEBUG

	return found ; 
}


/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::trim_queue_to_pcg

-------------------------------------------------------------------------------*/
size_t MosquittoMessageQueue::trim_queue_to_pcg(const size_t& pcg)
{
	/**
		Shorten the queue up to a given percentage (0-100). 0 means -> queue must be filled to 0% (i.e. must be completely empty, 0 elements)
		100 means -> queue filled up to 100% (can be completely full, do not clean it)
		35 means -> queue filled up to 35% (if queue limit is 1000 elements, it will be shortened to keep no more than 350 elements).
		Return the final size obtained. 
	*/
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method called. Initial queue size: " << queue_size() \
			<< "  Final percentage: " << static_cast<int>(pcg)) ;
	ACS_CLASS_END_DEBUG

	size_t qs(0) ;

	{
		ThreadSafe::Lock lock(_mqtt_queue_mutex) ;

		// Check the current size of the queue; the queue cannot be empty:
		if ( (qs = _mqtt_messages_queue.size()) > 0 )
		{
			// Compute the maximum final size:
			const size_t max_messages_in_queue (get_queue_capacity() * (pcg/100.0)) ; // NOSONAR: type capacity not exceeded
		
			// Check if the queue is going to be trimmed, i.e. only partially empied:
			if (max_messages_in_queue>0) 
			{
				// Partially empty the queue:
				if (qs > max_messages_in_queue) 
				{
					const size_t items_to_remove (qs - max_messages_in_queue) ; 
					
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Queue current size:" << qs << "  maximum size: " << max_messages_in_queue \
							<< "  Messages to remove: " << items_to_remove) ;
					ACS_CLASS_END_DEBUG
					
					std::deque <MosquittoMessage>::iterator it_start ( _mqtt_messages_queue.begin() ) ; 
					std::deque <MosquittoMessage>::iterator it_stop ( it_start + items_to_remove ) ; 
					
					_mqtt_messages_queue.erase(it_start, it_stop) ;
				}
			} 
			else
			{
				// Completely purge the queue:
			
				_mqtt_messages_queue.clear() ; 
			}
		
			// Compute the new size
			qs = _mqtt_messages_queue.size() ;
	
		}	// END: if queue is not empty

	}	// END: scope
		
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Method finished. Returning: " << qs) ;
	ACS_CLASS_END_DEBUG

	return qs ; 
}



/*-------------------------------------------------------------------------------

		MosquittoMessageQueue::run

-------------------------------------------------------------------------------*/
void MosquittoMessageQueue::run()
{
#ifndef DISABLE_MQTT
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Thread is running.") ;
	ACS_CLASS_END_DEBUG

	const size_t max_wait_msec(50) ; // Max delay is the queue is empty

	while ( (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
	{
		/**
			Aim of this loop, i.e. of this thread, is simply to verify if the internal message queue is full.
			If it's full, method pop_from_mqtt_messages_queue RETURNS IMMEDIATELY after filled the mosquitto_message.
			Otherwise, the method waits for up to 50 msec.
			
			THEREFORE THERE IS NO NEED TO ADD A DELAY IN THIS LOOP: EITHER IT IS AUTOMATICALLY WAITING FOR THE QUEUE,
			OR THE QUEUE IS FULL AND MESSAGES SHOULD BE RE-TRANSMITTED ASAP.
		*/

		MosquittoMessage msg_from_queue ; 

		if (true == wait_and_pop_from_queue(msg_from_queue, max_wait_msec))
		{
			ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
				ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Extracted message from queue. Calling onMessage ...") ;
			ACS_CLASS_END_DEBUG
		
			// Transmit the message, originally stored in the queue 
			_sub.onMessage(msg_from_queue.ptr()) ; 
		}

		usleep (10) ;	// Minimal delay, just 10 microsec, to prevent a huge CPU load if a large number of messages has been stacked.
						// The resulting rate, despite the delay, would be 0.1 Mega-messages per second !!
	} ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Thread is not running.") ;
	ACS_CLASS_END_DEBUG
#else
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_LOG_DEBUG(SimpleDebugSignatureThread << "Thread intentionally not launched.") ;
	ACS_CLASS_END_DEBUG
#endif // #ifndef DISABLE_MQTT
}

_ACS_END_NAMESPACE
