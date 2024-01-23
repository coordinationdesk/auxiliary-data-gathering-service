// PRQA S 1050 EOF
/*
   Copyright 1995-2021, Exprivia SpA
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

#include <Filterables.h>
#include <Timer.h>

#ifndef DISABLE_MQTT
#include <mosquitto.h>
#else 
#warning MQTT has been disabled
#endif

using namespace std;

_ACS_BEGIN_NAMESPACE (acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(MQTTWorker) ;


// NOTICE:
// The mqtt mosquitto library is thread safe
// So mutex protection is needed only when managing shared internal attributes

MQTTWorker::MQTTWorker(const string& name, const struct MQTTWorker::Configuration& config, StopController const& stopController) :
	_name (name),
	_config (config),
	_mqttinstance (),
	_connected(false),
	_loopActive(false),
	_pendingConnect(false),
	_connTrackStatus(MQTTWorker::ConnStatusIDLE),
	_connTrackMessage(""),
	_mutex(),
	_eventLoopMutex(),
	_stopController(stopController)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);

	// Do initialization, if needed
	MosquittoInitializer::instance();
	
	//Create the first MQTT instance
	createMQTTInstance();
}

MQTTWorker::~MQTTWorker() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	destroyMQTTInstance();
}

void MQTTWorker::createMQTTInstance()
{
	//Create an instance of mosquitto struct
	//and then register to the callback manager
	
#ifndef DISABLE_MQTT
	// MP - no _mqttinstance to be created - will 
	//Ensure configuration consistency
	const char* clientIdPtr = _config.clientId.c_str();
	if ( _config.clientId.empty() )
	{
		// No client id. Must force the clean session flag
		_config.clean_session = true;
		// Null client id ptr
		clientIdPtr = NULL;
	}
	_mqttinstance = mosquitto_new(clientIdPtr, _config.clean_session, NULL);
	if(NULL == _mqttinstance)
	{
		ACS_THROW (MQTTException ("Unable to create mosquitto instance", errno));
	}
	
	// Register into the callback bridge
	MQTTCallbackBridge::instance()->registerWorker(_mqttinstance, this);
#endif
}

void MQTTWorker::shutDownMQTTLayer()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

#ifndef DISABLE_MQTT
		// disconnect
		disconnect();
		
		// terminate the event loop
		stopEventLoop(true);
#endif
}

void MQTTWorker::destroyMQTTInstance() ACS_NOEXCEPT
{
#ifndef DISABLE_MQTT
// MP - no _mqttinstance has been created - hence n othing to do
	if (NULL != _mqttinstance)
	{
		// disconnect and stop the mosquitto event loop
		shutDownMQTTLayer();
		
		// Then unregister
		MQTTCallbackBridge::instance()->unregisterWorker(_mqttinstance);
		
		// And destroy the instance
		mosquitto_destroy (_mqttinstance);
		_mqttinstance = NULL;
	}
#endif
}
void MQTTWorker::refreshMQTTInstance()
{
	destroyMQTTInstance();
	createMQTTInstance();
}

void MQTTWorker::startEventLoop()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled
	ThreadSafe::Lock lock (_eventLoopMutex);

	// Safety check - this can throw
	checkMQTTInstance();  

	if (!isLoopActive())
	{		
		// Start threaded interface
		// The thread must be started before trying to connect_async or automatic reconnection
		// at the first attempt failure will not be performed
		int mqResult = mosquitto_loop_start (_mqttinstance);
		if ( MOSQ_ERR_SUCCESS != mqResult )
		{
			// This is not good too. Throw an exception.
			ostringstream os;
			os << "Unable to start MQTT client threaded loop. Error is: " << mosquitto_strerror (mqResult);
			ACS_THROW ( MQTTWorker::ConnectionError (os.str()) );
		}
		setLoopActive(true);
	}
#endif
}

void MQTTWorker::stopEventLoop(bool force)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled
	ThreadSafe::Lock lock (_eventLoopMutex);
	// Used in DTOR.. can not throw
	if (NULL != _mqttinstance)
	{
		// The event loop has been started??
		if (isLoopActive())
		{
			// in order to prevent too long delay during the mosquitto loop stop (e.g. when trying to stop the loop on a unreachable server) a shorter timeout is set.
			const int delay ( 5 );
			const int max_delay ( 5 );
			const bool exponential_incr_rate (false);
			mosquitto_reconnect_delay_set(_mqttinstance, delay, max_delay, exponential_incr_rate);
			int mqResult = mosquitto_loop_stop (_mqttinstance, force);
			if ( MOSQ_ERR_SUCCESS != mqResult )
			{
				// Is it really an issue?
				ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Unable to stop loop")
			}
		}
	}

	// Track the loop is not active now
	setLoopActive (false);

	// Reset the state machine
	setConnTrackStatus (ConnStatusIDLE);
	setConnTrackMessage (0, "");
#endif
}

void MQTTWorker::setLoopActive(bool b)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	// Update status attribute
	ThreadSafe::Lock lock (_mutex);
	_loopActive = b;
}

void MQTTWorker::setConnected (bool b)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	// Update status attribute
	ThreadSafe::Lock lock (_mutex);
	_connected = b;
}


const string& MQTTWorker::getName() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	ThreadSafe::Lock lock (_mutex);
	return _name;
}

const MQTTWorker::Configuration& MQTTWorker::getConfiguration() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	ThreadSafe::Lock lock (_mutex);
	return _config;
}

void MQTTWorker::checkMQTTInstance() const 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled - do not throw
	ThreadSafe::Lock lock (_mutex);
	// This should never happen... but...
	if ( NULL == _mqttinstance )
	{
		ACS_THROW (MQTTException ("Invalid (NULL) Mosquitto instance detected.") );
	}
#endif
}

// Business logic

// Start thread managing the broker connection
// It will automatically try to re-connect in case of failures

bool MQTTWorker::connect(unsigned long timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	
	// Preliminary check
	checkMQTTInstance();

	// If any pending connection / an actual connection is on going do a controlled disconnection
	disconnect();

	// stop the event loop if running, and force stop
	// because in some cases of client not connected and so 
	// with no active subscriptions, they may hang
	stopEventLoop(true);

	// Refresh the mqtt instance (delete and create back) to reset internal status
	refreshMQTTInstance();

	// re-start the event loop now.. 
	startEventLoop();


#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 

	// Critical area
	{
		ThreadSafe::Lock lock (_mutex);
		
		// MQ return value interface
		int mqResult;
		
		// Register for connection, will not block
		const char* username = (_config.username.empty() ? NULL : _config.username.c_str());
		const char* password = (_config.password.empty() ? NULL : _config.password.c_str());
		
		// Verify if credentials must be provided
		if ( username != NULL )
		{
			mqResult = mosquitto_username_pw_set( _mqttinstance, username, password);
			if ( MOSQ_ERR_SUCCESS != mqResult )
			{
				ostringstream os;
				os << "Unable to set credentials for the connection. Error is: " << mosquitto_strerror (mqResult);
				ACS_THROW ( MQTTWorker::ConnectionError (os.str()) );
			}
		}
		
		mqResult = mosquitto_connect_async (_mqttinstance, _config.host.c_str(), _config.port, _config.keepalive);
		if ( MOSQ_ERR_SUCCESS != mqResult )
		{
			if (mqResult == MOSQ_ERR_EAI)
			{
				// When here: EAI ERROR ("Error getting DNS Addressing Informations")
				// Example: invalid website, like google25.com or DNS server unreachable
				// Usally the error appears as: Lookup Error.

				ostringstream os;
				os << "Unable to get Hostname Address Information / DNS Error. Error is: " << mosquitto_strerror (mqResult);
				ACS_THROW ( MQTTWorker::ConnectionLookupError (os.str()) );
			}
			else 
			{
				ostringstream os;
				os << "Unable to perform the connection call on MQTT client. Error is: " << mosquitto_strerror (mqResult);
				ACS_THROW ( MQTTWorker::ConnectionError (os.str()) );
			}
		}

		
		// Keep track that a connection request is pending (will be managed by the mosquitto event loop)
		setPendingConnect(true);
		
		// Force the state transition
		setConnTrackStatus (ConnStatusCONNECTING);
	}

	// Now wait for the connection ackn
	const unsigned int sleepPeriod = 50; // msecs
	unsigned int maxCount = timeoutMs / sleepPeriod;
	unsigned int counter = 0;
	
	while ( ( false == isConnected() ) && ( counter < maxCount)  && ( false == _stopController.requestedCancel() ) )
	{
		// Delay the sleepTime
		Timer::delay (sleepPeriod);
		++counter;
	}

// MP - isConnected should return false 

#endif	
	return isConnected();
}

// Terminate connection if any
bool MQTTWorker::disconnect(unsigned long timeoutMs)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// Preliminary check
	checkMQTTInstance();

#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 
	// Disconnect only if a connection has been requested
	if (isPendingConnect() )
	{
		ThreadSafe::Lock lock (_mutex);

		// Disconnect, will not block
		int result = mosquitto_disconnect (_mqttinstance);
		if ( MOSQ_ERR_SUCCESS != result )
		{
			// Is it really an issue?
			ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Unable to disconnect")
		}
		// No more pending connection reqests
		setPendingConnect(false);
	}

	// Now wait for the dis-connection ack
	const unsigned int sleepPeriod = 50; // msecs
	unsigned int maxCount = timeoutMs / sleepPeriod;
	unsigned int counter = 0;
	
	while ( ( true == isConnected() ) && ( counter < maxCount ) )
	{
		// Delay the sleepTime
		Timer::delay (sleepPeriod);
		++counter;
	}

// MP - isConnected should return false 

#endif

	return !isConnected() ;
}


bool MQTTWorker::isConnected() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_HIG_VERB);

	ThreadSafe::Lock lock (_mutex);
	return _connected;
}

bool MQTTWorker::isLoopActive() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_HIG_VERB);

	ThreadSafe::Lock lock (_mutex);
	return _loopActive;
}

void MQTTWorker::setPendingConnect(bool b)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	// Update status attribute
	ThreadSafe::Lock lock (_mutex);
	_pendingConnect = b;
}

bool MQTTWorker::isPendingConnect() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_HIG_VERB);

	ThreadSafe::Lock lock (_mutex);
	return _pendingConnect;
}

// Used to implement thread safety also on derived classes
const Mutex& MQTTWorker::getMutex() const
{
	// No debug here. Just return the mutex.
	return _mutex;
}

struct mosquitto* MQTTWorker::getClient() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	ThreadSafe::Lock lock (_mutex);
	return _mqttinstance;
}

/**
 *  CALLBACK LOGIC
 **/

// callback prototypes
void MQTTWorker::connectCallback (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Connection established (" << rc << "): " << mosquitto_connack_string (rc));
#else
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Connection established (MQTT DISABLED)");
#endif
	
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 
	// Check if the connection has been established or not due to real errors
	if ( 0 != rc )
	{
		// set the connection tracking status
		setConnTrackStatus (ConnStatusERROR);
		
		// store also the error
		setConnTrackMessage (rc, mosquitto_connack_string (rc));

		// If here a real problem with the connect exists 
		// and is not a matter of not reachable broker
		// must shut down the connection to avoid hammering the server
		// there is no reasons to wait any further
		disconnect(0);
	}
	else
	{
		// It's connected now
		setConnected(true);
		// set the connection tracking status
		setConnTrackStatus (ConnStatusCONNECTED);

		// clear any previous error
		setConnTrackMessage (0, "");
	}

	try
	{
		// Internal callback management in derived classes
		internal_onConnect (rc);

		// Call specific implementation of derived class
		// if any
		onConnect (rc);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The connect callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
    
#endif
}

void MQTTWorker::disconnectCallback (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Disconnection occurred (" << rc << "): " << mosquitto_strerror (rc));
#else
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Disconnection occurred (MQTT DISABLED)");
#endif
	
#ifndef DISABLE_MQTT
// MP - no op in case of MQTT disabled 
	// Status management logic
	switch (getConnTrackStatus())
	{
		case ConnStatusCONNECTED:
			// if expected go to IDLE otherwise to CONNECTING (connection lost)
			if (0 == rc)
			{
				// Nominal disconnect, started by client
				setConnTrackStatus (ConnStatusIDLE);
				// clear any previous error
				setConnTrackMessage (0, "");
			}
			else
			{
				// Connection lost, keep the message and go ahead with the connection attempts
				// set the connection tracking status
				setConnTrackStatus (ConnStatusCONNECTING);

				// store also the error
				setConnTrackMessage (rc, mosquitto_strerror (rc));
			}
		break;
		case ConnStatusCONNECTING:
		case ConnStatusERROR:
		default:
			// NO-OP... if any trouble keep the message
			if (0 != rc)
			{
				setConnTrackMessage (rc, mosquitto_strerror (rc));
			}
		break;
	} // SWITCH ENDS HERE

	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onDisconnect (rc);

		// Call specific implementation of derived class
		// if any
		onDisconnect (rc);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The disconnect callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
	
	// SET CONNECTED AT THE END
	setConnected(false);
#endif

}

void MQTTWorker::publishCallback (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onPublish (mid);

		// Call specific implementation of derived class
		// if any
		onPublish (mid);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The publish callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
	
}

void MQTTWorker::messageCallback (const struct mosquitto_message *msg)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onMessage (msg);

		// Call specific implementation of derived class
		// if any
		onMessage (msg);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The message callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}


}

void MQTTWorker::subscribeCallback (int mid, int qos_count, const int * granted_qos)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// Try a C++ mapping of types
	vector <MQTT_QOS> grantedQOS;

	for (int idx=0; idx < qos_count; idx++)
	{
		grantedQOS.push_back (intToMQTT_QOS(granted_qos[idx]));
	}


	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onSubscribe (mid, grantedQOS);

		// Call specific implementation of derived class
		// if any
		onSubscribe (mid, grantedQOS);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The subscribe callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
}

void MQTTWorker::unsubscribeCallback (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onUnsubscribe (mid);

		// Call specific implementation of derived class
		// if any
		onUnsubscribe (mid);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The unsubscribe callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
}

void MQTTWorker::logCallback (int level, const char *str)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);

	// Call specific implementation of derived class
	// if any
	string logMessage (str);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, getName() << " - MQTTLOG:" << logMessage);

	// MUST TRY CATCH ANY INTERNAL AND ON EVENT HANDLER CALL (avoid explosions)
	try
	{
		// Internal callback management in derived classes
		internal_onLog (level, logMessage);

		onLog (level, logMessage);
	}
	catch (exception& ex)
	{
		ACS_LOG_WARNING ("MQTTWorker: The log callback has been interrupted by an execption");
		ACS_LOG_PRIVINFO (ex);
	}
}

// Empty hooks

void MQTTWorker::onConnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onDisconnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onPublish (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onMessage (const struct mosquitto_message *msg)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onSubscribe (int mid, const vector<MQTT_QOS> & grantedQOS)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onUnsubscribe (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::onLog (int level, const string& message)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onConnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onDisconnect (int rc)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onPublish (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onMessage (const struct mosquitto_message *msg)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onSubscribe (int mid, const std::vector<MQTT_QOS> & grantedQOS)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onUnsubscribe (int mid)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

void MQTTWorker::internal_onLog (int level, const std::string& message)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
}

// Manage connection tracking transactions
void MQTTWorker::setConnTrackStatus(const MQTTWorker::ConnStatus& status)
{
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Entering conn status: " << connStatusToString(status) );
	ThreadSafe::Lock lock (_mutex);
	_connTrackStatus = status;
}

void MQTTWorker::setConnTrackMessage(int rc, const string& msg)
{
	ostringstream os;
	if (!msg.empty())
	{
		os << msg << " (" << rc << ")";
	}
	ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "Setting conn track message: " << os.str() );
	ThreadSafe::Lock lock (_mutex);
	_connTrackMessage = os.str();
}

const MQTTWorker::ConnStatus& MQTTWorker::getConnTrackStatus() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	ThreadSafe::Lock lock (_mutex);
	return _connTrackStatus;
}

const string& MQTTWorker::getConnTrackMessage() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_MID_VERB);
	ThreadSafe::Lock lock (_mutex);
	return _connTrackMessage;
}

_ACS_END_NAMESPACE
