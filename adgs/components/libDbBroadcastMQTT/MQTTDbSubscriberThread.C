/*
	Copyright 2021-, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. MQTTDbSubscriberThread $

*/ 

#include <MQTTDbSubscriberThread.h>
#include <fstream>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbSubscriberThread)

ThreadSafe MQTTDbSubscriberThread::_messagesMutex ;


/****************************************************************************
**
**		MQTTDbSubscriberThread	
**
****************************************************************************/	
MQTTDbSubscriberThread::MQTTDbSubscriberThread(	
			const bool hb_enabled,								/* HB Enabled */
			const bool messages_enabled,						/* Messages Enabled [for the caller, as a container] */
			const string& topic,								/* Topic */
			const MQTTDbSubscriber::Enum_TimeWait time_wait,
 			const MQTTWorker::Configuration config,				/* MQTT Configuration (includes a randomized ID) */
			sem_t* p_sem_threads,								/* Semaphore to retrieve the updated Containers */
			StopController const& stopController,
			const string& thdName) :
	Thread(thdName),
	_p_subscriber(NULL),
	_hb_enabled(hb_enabled),
	_topic(topic),
	_messages_V(),
	_message_received(),
	_time_wait(time_wait),
	_config(config),
	_p_sem_threads(p_sem_threads),
	_messages_enabled(messages_enabled),
	_stopController(stopController)
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]") 	

	// Connect to the MQTT server
	connect() ; 

	if (not _config.host.empty())
	{
		// Report success/failure of the connection
		if ( (NULL != _p_subscriber) && (_p_subscriber->isConnected()) ) 
		{ 
			ACS_LOG_INFO( "[" << threadName() << "]\tSuccessfully connected to MQTT Server: " << _config.host << ":" << _config.port );
		}
		else { 
			ACS_LOG_WARNING( "[" << threadName() << "]\tCould not initiate connection to MQTT Server: " << _config.host << ":" << _config.port );
		}
	}
	else
	{
		ACS_LOG_INFO( "[" << threadName() << "]\tEmpty MQTT Server was provided" );
	}

	// Start the subscriber thread
	start() ; 

	// Check if the thread could start
	for (size_t i=0; \
		( (i<10) && (false==running()) \
			&& (false == _stopController.requestedCancel()) && (false == requestedCancel()) ); \
		i++) 
	{ sleep(1) ; /* Wait up to 10s */ }
	
	if ( (false == running()) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
	{
		// Not running, and not stopping either
		ACS_THROW ( exIllegalValueException("Unable to self-start thread (" + threadName() + ")") ) ; 
	}

	ACS_LOG_INFO("MQTT DbSubscriber Thread started: " + threadName()) ; 
}


/****************************************************************************
**
**		~MQTTDbSubscriberThread	
**
****************************************************************************/	
MQTTDbSubscriberThread::~MQTTDbSubscriberThread() throw() 
{
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "] dtor bgn") 	

	stop() ; 

	disconnect() ;	

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "] dtor end") 	
}


/****************************************************************************
**
**		connect	
**
****************************************************************************/	
void MQTTDbSubscriberThread::connect() 
{
	/** Create the instance and connect to the MQTT server */

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "] Connecting to MQTT Server: " << _config.host << ":" << _config.port ) 	 ;

	_p_subscriber = new MQTTDbSubscriber(_hb_enabled, _topic, _config, _stopController, _time_wait) ; // This also connects

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "] finished" ) 	 ;
}


/****************************************************************************
**
**		disconnect	
**
****************************************************************************/	
void MQTTDbSubscriberThread::disconnect() 
{
	/** Disconnect from the MQTT server and destroy instance */

	if (NULL != _p_subscriber)
	{
        ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tMethod called. Disconnecting from MQTT Server: " << _config.host << ":" << _config.port) ;
		
		delete _p_subscriber ;	// This also disconnects
	}
	
	_p_subscriber = NULL ;

    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tMethod finished.") ;
}


/****************************************************************************
**
**		retrieveMessages	
**
****************************************************************************/	
bool MQTTDbSubscriberThread::retrieveMessages(vector <MQTTMessageHandler>& remote_messages_V) 
{
	/**
		Return TRUE accordingly to the condition of reporting and move the message stack to the caller, resetting the local stack/flag
	*/
    ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]") ;

	bool ret_status ;

	{
		ThreadSafe::Lock lock(_messagesMutex) ;

		ret_status = _message_received ;
		
		// The swap below has two effects
		// - It will copy to remote_messages_V the local stack: _messages_V 
		// - Since it swaps with an EMPTY vector, _messages_V will be cleared
		remote_messages_V.clear() ; 
		std::swap(remote_messages_V, _messages_V) ; 

		_message_received = false ; 
	}

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tMethod finished. Returning: " << boolalpha << ret_status) ;

	return ret_status ; 
}


/****************************************************************************
**
**		run	
**
****************************************************************************/	
void MQTTDbSubscriberThread::run() {

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tMethod called.") ;

	while ( (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
	{
		// Check if there are new messages -- the messages are _NOT_ added to the local stack in this phase, since a MUTEX-block
		// is required to prevent race condition on the vector
		
		vector <MQTTMessageHandler> new_messages_V ; // Low scope messages -- they will be eventually moved to the local stack
		
		bool message_received (false) ;

		while ( \
			(false == (message_received = _p_subscriber->retrieveMessages(new_messages_V))) \
			&& \
			(false == _stopController.requestedCancel()) && (false == requestedCancel()) \
		) 
		{
			// Method retrieveMessages contains a timed-wait, so _NO_ MORE WAITS are needed here.
			//
			// DO NOTHING -- NO CPU LOAD
			//

			ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tInside loop. Messages enabled:" << boolalpha << messages_enabled()) ;
		} ;
		
		// Note that method retrieveMessages above also REMOVED the messages from the remote stack (no overflow)
			
		// Copied the messages to the local stack	
		if ( \
			(true == message_received) \
			&& \
			(true == messages_enabled())		
			&& \
			(false == _stopController.requestedCancel()) && (false == requestedCancel()) 
		)
		{
			{
				// The local stack messages is handled in a MUTEX-block
				ThreadSafe::Lock lock(_messagesMutex) ;

				if (_messages_V.size() > (1024*1024))
				{
					// If there are more than 1M UNREAD messages in the local stack, probably the caller has some problem ... ...
					// To avoid overflooding, the whole local stack is PURGED, so it will only contain the new message !
					_messages_V.clear() ; // Emergency clean !

					ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "Local stack purged, since it was overwhelming !") ;
				}

				_message_received = true ; 

				// Copy the low-scope messages to the local stack
				if (new_messages_V.size() > 0)
				{
					const size_t orig_size_messages_V (_messages_V.size()) ; 
					const size_t new_size_messages_V (orig_size_messages_V + new_messages_V.size()) ;

					// Resize the target to the new size (i.e. extend it to contain the data to add)
					_messages_V.resize(new_size_messages_V) ;

					// Copy the values from the source to the target (which has been pre-allocated)
					// -- this is FASTER than any "push at the end" (which extends the vector size of 1 element each time)
					std::copy( new_messages_V.begin(), new_messages_V.end(), (_messages_V.begin() + orig_size_messages_V) );
				}
			}	// END SCOPE: _messagesMutex

			// Release the semaphore, unless it's ALREADY available (the semaphore is outside the class-mutex, being multi-process)
			if (NULL != _p_sem_threads)
			{
				int sem_value ; 
				ACS_COND_THROW ( (0 != sem_getvalue(_p_sem_threads, &sem_value)), exIllegalValueException("Fatal error, unable to read semaphore") ) ; 

				/* If counter unset (0 means unset, <0 means unset with user waiting) INCREASE the counter */
				if (sem_value <= 0) { ACS_COND_THROW ( (0 != sem_post(_p_sem_threads)), exIllegalValueException("Fatal error, unable to increase semaphore") ) ; }
			}	// END BLOCK: Release the semaphore
			
		}	// END: if ( (true == message_received) && ...
	
	} ; // END: while ( (false == _stopController.requestedCancel()) && (false == requestedCancel()) )

	ACS_CLASS_WRITE_DEBUG_TH(ACS_LOW_VERB, "[" << threadName() << "]\tMethod finished.") ;

	return ;
}

_ACS_END_NAMESPACE
