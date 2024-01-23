// PRQA S 1050 EOF
/*
   Copyright 2019-2021, Exprivia SpA
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia, SpA

	$Prod: MQTT DbSubscriber $

	$Log$

*/ 


#include <MQTTDbMultiSubscriber.h>
#include <MQTTDbSubscriberThread.h>
#include <errno.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs) 

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTDbMultiSubscriber) ;

ThreadSafe MQTTDbMultiSubscriber::_resourceMutex ;


/***************************************************************************************
**
                             MQTTDbMultiSubscriber
**
***************************************************************************************/
MQTTDbMultiSubscriber::MQTTDbMultiSubscriber(	const bool heartbeat_enabled, 
												dbConnectionPool& cp, 
												const string& host, 
												const int& port,
												void (*callback)(bool, size_t, pthread_t),
												StopController const& stopController,
												const string& thdName) :
	Thread(thdName),
	_connectionPool(cp),
	_messages_V(),
	_message_received(false),
	_threads_V(),
	_host(host),
	_port(port),
	_sem_threads(),
	_callback(callback),
	_callback_enabled(false),
	_heartbeat_enabled(heartbeat_enabled),
	_stopController(stopController),
	_signalReceiverThreadId(0)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	
	const int flag_inter_process (0) ; 		// False, since semaphore is _NOT_ inter-process
	const unsigned int initial_value (0) ;	// Already locked when initialized
	ACS_COND_THROW ( (0 != sem_init (&_sem_threads, flag_inter_process, initial_value)), exIllegalValueException("Fatal error, unable to initialize semaphore") ) ; 

}


/***************************************************************************************
**
                             ~MQTTDbMultiSubscriber
**
***************************************************************************************/
MQTTDbMultiSubscriber::~MQTTDbMultiSubscriber() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB)
	
	unsubscribe () ;
	
	stop() ; 
	
	sem_destroy (&_sem_threads);
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}

void MQTTDbMultiSubscriber::setPort(int port)
{
	_port = port;
}
void MQTTDbMultiSubscriber::setHost(const std::string & host)
{
	_host = host;
}

/***************************************************************************************
**
                               [STATIC] compute_wait_time   
**
***************************************************************************************/
void MQTTDbMultiSubscriber::compute_wait_time (const uint16_t& timeout_ms, struct timespec& ts)  
{
	/*
		Obtain a time IN THE FUTURE, by adding an interval to the current time
	*/

	clock_gettime(CLOCK_REALTIME, &ts) ;								// Get the current time

	ts.tv_nsec += (timeout_ms * 1000000L /* from [ms] to [ns] */) ;	// Compute target time

	// Make up the nanoseconds, in order not to exceed 1s
	if      (ts.tv_nsec >= 1999999999)	{ ts.tv_sec += (ts.tv_nsec / 1000000000) ; ts.tv_nsec = (ts.tv_nsec % 1000000000) ; /* General case */ }
	else if (ts.tv_nsec >   999999999)	{ ts.tv_nsec -= 1000000000 ; ts.tv_sec ++ ; /* Within one second, avoid division, which is slow */ }

	return ;
}


/***************************************************************************************
**
                             subscribe
**
***************************************************************************************/
size_t MQTTDbMultiSubscriber::subscribe (const bool messages_enabled, const string& table, const enum SQLDialect::SQLToken& action, const MQTTDbSubscriber::Enum_TimeWait& time_wait)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Table: \"" << table 
			<< "\"  action: " << toString(action) << "  time_wait: " << MQTTDbSubscriber::toStr(time_wait) << endl ;
	ACS_CLASS_END_DEBUG

	dbConnectionWrapper pw(_connectionPool) ;
	MQTTDbTopicGenerator mqtt_dbtopic_generator(pw) ; 
	const string topic ( mqtt_dbtopic_generator.generate_topic (table, action) ) ;
	
	return subscribe(messages_enabled, topic, time_wait) ; 
}

/***************************************************************************************
**
                             subscribe
**
***************************************************************************************/
size_t MQTTDbMultiSubscriber::subscribe (const bool messages_enabled, const string& table, const string& action, const MQTTDbSubscriber::Enum_TimeWait& time_wait) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Table: \"" << table 
			<< "\"  action: \"" << action << "\"  time_wait: " << MQTTDbSubscriber::toStr(time_wait) << endl ;
	ACS_CLASS_END_DEBUG

	dbConnectionWrapper pw(_connectionPool) ;
	MQTTDbTopicGenerator mqtt_dbtopic_generator(pw) ; 
	const string topic ( mqtt_dbtopic_generator.generate_topic (table, action) ) ;

	return subscribe(messages_enabled, topic, time_wait) ; 
}

/***************************************************************************************
**
                             subscribe
**
***************************************************************************************/
size_t MQTTDbMultiSubscriber::subscribe (const bool messages_enabled, const string& topic, const MQTTDbSubscriber::Enum_TimeWait& time_wait) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Topic: \"" << topic 
			<< "\"  time_wait: " << MQTTDbSubscriber::toStr(time_wait) << endl ;
	ACS_CLASS_END_DEBUG

	size_t index ; 
	
	// In order to obtain the index of the (still-to-be-launched) thread, a not-running NULL-thread is added
	// to the thread vector:
	{
		ThreadSafe::Lock lock(_resourceMutex) ;
		const bool enabled (false) ; 
		_threads_V.push_back ( pair<MQTTDbSubscriberThread*, bool>(NULL, enabled) ) ;
		index = (_threads_V.size()-1) ; // 0-relative
	}

	// WHEN HERE: the index of the thread has been alredy computed.
	ostringstream o_threadName ; o_threadName << "MQTTDbSubscriberThread" << "_" << index ; 
	
	MQTTDbSubscriberThread* p_subscriber_th = new MQTTDbSubscriberThread(heartbeat_enabled(), messages_enabled, topic, time_wait, \
		( _host.empty()? MQTTDbSubscriber::buildConfiguration(): MQTTDbSubscriber::buildConfiguration(_host, _port) ), \
		&_sem_threads, _stopController, o_threadName.str()) ; 

	// Check if the thread could start
	for (size_t i=0; \
		( (i<10) && (false==p_subscriber_th->running()) \
			&& (false == _stopController.requestedCancel()) && (false == requestedCancel()) ); \
		i++) 
	{ sleep(1) ; /* Wait up to 10s */ }
	
	if ( (false == p_subscriber_th->running()) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
	{
		// Not running, and not stopping either
		ACS_THROW ( exIllegalValueException("Unable to start thread (" + o_threadName.str() + ")") ) ; 
	}

	// --  Customize the MQTTDbSubscriber  ---------------------------------------
	
	// So far, only the macro_standard_loop parameter is passed to the subscriber.
	// Note that the macro IMMEDIATELY TRIGGERS the first forced-hit immediately after the call.
	p_subscriber_th->subscriber()->macro_standard_loop() ;
	// ---------------------------------------------------------------------------
	

	// Update the thread pointer in the vector, at the same index.
	{
		ThreadSafe::Lock lock(_resourceMutex) ;
		const bool enabled (true) ; 
		_threads_V[index] = pair<MQTTDbSubscriberThread*, bool>(p_subscriber_th, enabled) ;
	}


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning index: " << index << endl ;
	ACS_CLASS_END_DEBUG
	
	return index ; 
}


/***************************************************************************************
**
                             unsubscribe
**
***************************************************************************************/
bool MQTTDbMultiSubscriber::unsubscribe (const size_t& index) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. index: " << index 
			<< "  vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	if ( index >= _threads_V.size() )
	{
		ostringstream o ; o << "Index " << index << "  exceeds vector size: " << _threads_V.size() ; 
		ACS_THROW ( exBoundsException(o.str()) ) ; 
	}
	
	const bool enabled = _threads_V.at(index).second ;
	
	if (true == enabled)
	{
		MQTTDbSubscriberThread* p_subscriber_th = _threads_V.at(index).first ;
		if ( NULL == p_subscriber_th )
		{
			ostringstream o ; o << "Unsubscribing from index " << index << "  which was enabled with a NULL ptr" ; 
			ACS_LOG_WARNING (o.str()) ; 
		}
		else
		{ 
			delete p_subscriber_th ; 
		}
		
		_threads_V[index] = pair<MQTTDbSubscriberThread*, bool>(NULL, false) ;
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << (enabled?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return enabled ;
}

/***************************************************************************************
**
                             unsubscribe
**
***************************************************************************************/
void MQTTDbMultiSubscriber::unsubscribe () 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	for (size_t i=0; i<_threads_V.size(); i++)
	{
		unsubscribe(i) ; 
	}

	_threads_V.clear() ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             messages_enabled
**
***************************************************************************************/
bool MQTTDbMultiSubscriber::messages_enabled () const 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called.  vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	bool ret_value (true) ; 

	for (size_t i=0; i<_threads_V.size(); i++)
	{
		if ( (true == _threads_V.at(i).second) && (NULL != _threads_V.at(i).first) )
		{
			const bool b ( (_threads_V.at(i).first)->messages_enabled() ) ;

			ret_value = (ret_value && b) ;
		}
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning: " << ret_value << endl ;
	ACS_CLASS_END_DEBUG
	
	return ret_value ;
}

/***************************************************************************************
**
                             enable_messages
**
***************************************************************************************/
void MQTTDbMultiSubscriber::enable_messages (const bool b, const size_t& index) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. index: " << index 
			<< "  vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	if ( index >= _threads_V.size() )
	{
		ostringstream o ; o << "Index " << index << "  exceeds vector size: " << _threads_V.size() ; 
		ACS_THROW ( exBoundsException(o.str()) ) ; 
	}
	
	
	if ( (true == _threads_V.at(index).second) && (NULL != _threads_V.at(index).first) )
	{
		(_threads_V.at(index).first)->enable_messages(b) ;
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}

/***************************************************************************************
**
                             enable_messages
**
***************************************************************************************/
void MQTTDbMultiSubscriber::enable_messages (const bool b) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	for (size_t i=0; i<_threads_V.size(); i++)
	{
		enable_messages(b, i) ; 
	}

	if (false == b)
	{
		// If the messages from ALL THE THREADS are disabled, the local stack is purged, too
		// (if interested, the user should retrieve the stack before disabled the messages)
		_messages_V.clear() ; 
		_message_received = false ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             monitorThisThreadIsRunning
**
***************************************************************************************/
void MQTTDbMultiSubscriber::monitorThisThreadIsRunning()
{
	/** Restart the thread in case of crash */

	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
	
	if (false == running())
	{
		for (size_t i=0; \
			( (i<10) && (false==running()) \
				&& (false == _stopController.requestedCancel()) && (false == requestedCancel()) ); \
			i++) 
		{ sleep(1) ; /* Wait up to 10s */ }
	
		{
			ThreadSafe::Lock lock(_resourceMutex) ;
			if ( (false == running()) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
			{
				std::exception* ex = 0;
				runException(ex);
				if (0 != ex) {
					ACS_LOG_ERROR("Thread \"" << threadName() \
						<< "\" exited with the following error: " << ex->what());
					delete ex ; ex = 0 ; 
				}
				ostringstream o ; o << "Problem during execution of thread: " << threadName() \
					<< "  --  it will be restarted" ;
				ACS_LOG_WARNING(o.str()) ; 
				start() ; 
			}
		}
	}	// END: if (false == running())

	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/***************************************************************************************
**
                             operator[]
**
***************************************************************************************/
MQTTDbSubscriber* MQTTDbMultiSubscriber::operator[](size_t index) 
{
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. index: " << index 
			<< "  vector size: " << _threads_V.size() << endl ;
	ACS_CLASS_END_DEBUG
		
	ThreadSafe::Lock lock(_resourceMutex) ;

	if ( index >= _threads_V.size() )
	{
		ostringstream o ; o << "Index " << index << "  exceeds vector size: " << _threads_V.size() ; 
		ACS_THROW ( exBoundsException(o.str()) ) ; 
	}
	
	if ( false == _threads_V.at(index).second )
	{
		ostringstream o ; o << "Index " << index << "  is a disabled subscriber" ; 
		ACS_THROW ( exIllegalValueException(o.str()) ) ; 
	}

	if ( NULL == _threads_V.at(index).first )
	{
		ostringstream o ; o << "Index " << index << "  is a NULL subscriber" ; 
		ACS_THROW ( exIllegalValueException(o.str()) ) ; 
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG

	return (_threads_V.at(index).first)->subscriber() ;
}


/***************************************************************************************
**
                             [VIRTUAL FUNCTION] filterMessage
**
***************************************************************************************/
bool MQTTDbMultiSubscriber::filterMessage(const MQTTMessageHandler& message)
{
	ACS_CLASS_BGN_DEBUG(ACS_INS_VERB) 
		excerr << SimpleDebugSignatureThread << "Default method called." << endl ;
	ACS_CLASS_END_DEBUG
	
	return true ; 
}


/***************************************************************************************
**
                             retrieveMessages
**
***************************************************************************************/
bool MQTTDbMultiSubscriber::retrieveMessages(vector <MQTTMessageHandler>& remote_messages_V)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
	
	bool ret_status ;

	monitorThisThreadIsRunning() ; // Use this user-call to verify if the thread is still running, and restart if needed
	
	{
		ThreadSafe::Lock lock(_resourceMutex) ;

		ret_status = _message_received ;
		
		// The swap below has two effects
		// - It will copy to remote_messages_V the local stack: _messages_V 
		// - Since it swaps with an EMPTY vector, _messages_V will be cleared
		remote_messages_V.clear() ; 
		std::swap(remote_messages_V, _messages_V) ; 

		_message_received = false ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning " << (ret_status?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return ret_status ;
}


/***************************************************************************************
**
                             gotMessages
**
***************************************************************************************/
bool MQTTDbMultiSubscriber::gotMessages(size_t& messages, const bool reset)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
	
	bool ret_status ;
	
	monitorThisThreadIsRunning() ; // Use this user-call to verify if the thread is still running, and restart if needed

	{
		ThreadSafe::Lock lock(_resourceMutex) ;

		ret_status = _message_received ;
		messages = _messages_V.size() ; 
		
		if (true == reset)
		{ 
			_message_received = false ; 
			_messages_V.clear() ; 
		}
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning " << (ret_status?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return ret_status ;
}
void MQTTDbMultiSubscriber::init()
{
}

/****************************************************************************
**
**		run	
**
****************************************************************************/	
void MQTTDbMultiSubscriber::run() 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	// initialization: each listener implements its own events subscription
	init();

	const uint16_t timeout_ms (1000) ; // 1-sec. Since this timed condition is only used to check the stop flag.
	struct timespec ts ;
	
	while ( (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
	{
		compute_wait_time (timeout_ms, ts) ;
		const bool success = (0 == sem_timedwait(&_sem_threads, &ts) ) ; 

		if ( (true == success) && (false == _stopController.requestedCancel()) && (false == requestedCancel()) )
		{
			// Loop on all the threads to extract the new messages
		
			ThreadSafe::Lock lock(_resourceMutex) ;
		
			bool message_received(false) ; // Will be true if at least one thread received a new message
			
			for (size_t i=0; i<_threads_V.size(); i++)
			{
				if ( (true == _threads_V.at(i).second) && (NULL != _threads_V.at(i).first) )
				{
					/*------------------------------------------------------------------------------------*/
					// Check if the dbsubscriber thread #i is still running
					if (false == (_threads_V.at(i).first)->running())
					{
						std::exception* ex = 0;
						(_threads_V.at(i).first)->runException(ex);
						if (0 != ex) {
							ACS_LOG_ERROR("Thread \"" << (_threads_V.at(i).first)->threadName() \
								<< "\" exited with the following error: " << ex->what());
							delete ex ; ex = 0 ; 
						}
						ostringstream o ; o << "Problem running thread: " << (_threads_V.at(i).first)->threadName() \
							<< " with index: " << i << ": it will be unsubscribed" ;
						ACS_LOG_WARNING (o.str()) ; 
						unsubscribe(i) ; 
						continue ; // Check the next thread
					}	
					/*------------------------------------------------------------------------------------*/
				
					vector <MQTTMessageHandler> new_messages_V ; // Low scope messages -- they will be eventually moved to the local stack
				
					// Move the remote stack to the local stack
					const bool message_received_by_thread ( (_threads_V.at(i).first)->retrieveMessages(new_messages_V) ) ;
					const bool messages_enabled_in_thread ( (_threads_V.at(i).first)->messages_enabled() )  ;

					if ( (true == message_received_by_thread) && (true == messages_enabled_in_thread) )
					{
						if (new_messages_V.size() > 0)
						{
							bool found (false) ; 
							for (size_t j=0; j<new_messages_V.size(); j++)
							{
								if ( true == filterMessage(new_messages_V.at(j)) )
								{
									_messages_V.push_back(new_messages_V.at(j)) ; 
									found = true ; 
								}	
							}	// END: for j

							if (found == true) { message_received = true ; } // The return value is based on the result of the filter
						}	
						else 
						{
							message_received = true ; // The return value is ALWAYS TRUE since either has been triggered by a non-filterable message 
							// (boolean set without any message) -OR- no filtering has to be done				
						}

					}	// END: if ( (true == message_received_by_thread) && (true == messages_enabled_in_thread) )

				}	// END: if valid thread
						
			}	// END: for all threads 
				
			if (true == message_received) 
			{ 
				_message_received = true ; 

				if ( (true == callback_enabled()) && (NULL != _callback)  && (_signalReceiverThreadId != 0) )
				{
		 			(_callback)(message_received, _messages_V.size(), _signalReceiverThreadId) ; 
				} // END: if (NULL != _callback) 
			}
		
		}	// END scope: _resourceMutex
				
	}	// END: while		

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}
bool MQTTDbMultiSubscriber::callback_enabled() const			 
{ 
	return _callback_enabled ; 
}
void MQTTDbMultiSubscriber::enable_callback(const bool b)   
{ 
	_signalReceiverThreadId = ::pthread_self(); 
	_callback_enabled = b ; 
}
void MQTTDbMultiSubscriber::disable_callback()  				 
{ 
	enable_callback(false) ; 
}
bool MQTTDbMultiSubscriber::heartbeat_enabled() const			 
{ 
	return _heartbeat_enabled ; 
}
void MQTTDbMultiSubscriber::enable_heartbeat(const bool b)  
{
	_heartbeat_enabled = b ; 
}
void MQTTDbMultiSubscriber::disable_heartbeat() 
{ 
	enable_heartbeat(false) ; 
}


_ACS_END_NAMESPACE
